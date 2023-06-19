// Copyright (c) 2022 Faceware Technologies, Inc. All rights reserved.

#include "FTILiveLinkSource.h"
#include "FTILiveLinkSourceSettings.h"
#include "ILiveLinkClient.h"
#include "LiveLinkTypes.h"
#include "Roles/LiveLinkAnimationRole.h"
#include "Roles/LiveLinkAnimationTypes.h"

#include "Async/Async.h"
#include "Networking.h"
#include "Common/TcpSocketBuilder.h"
#include "HAL/RunnableThread.h"
#include "Json.h"
#include "Sockets.h"
#include "SocketSubsystem.h"

#include "Roles/LiveLinkCameraTypes.h"
#include "Roles/LiveLinkCameraRole.h"
#include "Roles/LiveLinkLightRole.h"
#include "Roles/LiveLinkLightTypes.h"
#include "Roles/LiveLinkTransformRole.h"
#include "Roles/LiveLinkTransformTypes.h"

#include <string>
//enable logging step 2
DEFINE_LOG_CATEGORY(FTILiveLink)

#define LOCTEXT_NAMESPACE "FTILiveLinkSource"

#define RECV_BUFFER_SIZE 1024 * 1024
#define HEADER_SIZE 4

FFTILiveLinkSource::FFTILiveLinkSource(FIPv4Endpoint InEndpoint, FName SubjectName)
: Socket(nullptr)
, Stopping(true)
, Thread(nullptr)
, WaitTime(1.0f)
, TimeOutSeconds(4.0f)
, LiveLinkSubjectName(SubjectName)
{
	UE_LOG(FTILiveLink, Verbose, TEXT("Enter FFTILiveLinkSource constructor %s"), *SubjectName.ToString());

	// defaults
	DeviceEndpoint = InEndpoint;

	SourceStatus = LOCTEXT("SourceStatus_DeviceNotFound", "Device Not Found");
	SourceType = LOCTEXT("FTILiveLinkSourceType", "Faceware Live Link");
	SourceMachineName = DeviceEndpoint.Address.ToText();

	UE_LOG(FTILiveLink, Log, TEXT("Setup socket"));
	RecvBuffer.SetNumUninitialized(RECV_BUFFER_SIZE);
	Start();
	SourceStatus = LOCTEXT("SourceStatus_Receiving", "Receiving");
	
	UE_LOG(FTILiveLink, Verbose, TEXT("Exit FFTILiveLinkSource constructor"));
}

FFTILiveLinkSource::~FFTILiveLinkSource()
{
	UE_LOG(FTILiveLink, Verbose, TEXT("Enter FFTILiveLinkSource destructor"));

	Stop();
	if (Thread != nullptr)
	{
		Thread->WaitForCompletion();
		delete Thread;
		Thread = nullptr;
	}
	if (Socket != nullptr)
	{
		Socket->Close();
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(Socket);
	}
		
	UE_LOG(FTILiveLink, Verbose, TEXT("Exit FFTILiveLinkSource destructor"));
}

TSubclassOf<ULiveLinkSourceSettings> FFTILiveLinkSource::GetSettingsClass() const
{ 
	return UFTILiveLinkSourceSettings::StaticClass(); 
}

void FFTILiveLinkSource::ReceiveClient(ILiveLinkClient* InClient, FGuid InSourceGuid)
{
	Client = InClient;
	SourceGuid = InSourceGuid;
}

bool FFTILiveLinkSource::IsSourceStillValid() const
{
	// Source is valid if we have a valid thread and socket
	bool bIsSourceValid = !Stopping && Thread != nullptr && Socket != nullptr;
	return bIsSourceValid;
}

bool FFTILiveLinkSource::RequestSourceShutdown()
{
	Stop();

	return true;
}

// FRunnable interface
bool FFTILiveLinkSource::Init()
{
	UE_LOG(FTILiveLink, Verbose, TEXT("call FFTILiveLinkSource::Init"));
	Stopping = false;
	IsListening = false;
	PreframeDataSize = -1;
	return true;
}

void FFTILiveLinkSource::Start()
{
	UE_LOG(FTILiveLink, Verbose, TEXT("call FFTILiveLinkSource::Start"));

	check(!Thread && "Thread wasn't null at the start!");
	check(FPlatformProcess::SupportsMultithreading() && "This platform doesn't support multithreading!");	
	if (Thread)
	{
		UE_LOG(FTILiveLink, Log, TEXT("Log: Thread isn't null. It's: %s"), *Thread->GetThreadName());
	}
	ThreadName = "FTI TCP Receiver ";
	ThreadName.AppendInt(FAsyncThreadIndex::GetNext());
	Thread = FRunnableThread::Create(this, *ThreadName, 128 * 1024, TPri_AboveNormal, FPlatformAffinity::GetPoolThreadMask());
	UE_LOG(FTILiveLink, Log, TEXT("Log: Created thread"));
}

void FFTILiveLinkSource::Stop()
{
	UE_LOG(FTILiveLink, Verbose, TEXT("call FFTILiveLinkSource::Stop"));
	Stopping = true;
}

bool FFTILiveLinkSource::ParseFTIStreamData(const TArray<uint8>& streamData)
{
	UE_LOG(FTILiveLink, Verbose, TEXT("call FFTILiveLinkSource::ParseFTIStreamData"));

	// first data packet and no header packet received then just ignore it
	if ((streamData[0] == '{' || streamData.Num() != 4) && PreframeDataSize < 0)
	{
		UE_LOG(FTILiveLink, Warning, TEXT("Invalid first data"));
		return false;
	}

	int start = 0;
	int end;

	while (!Stopping)
	{
		// data start
		if (streamData[start] == '{')
		{
			end = start + PreframeDataSize;

			// Verify if the data is valid
			if (end > streamData.Num())
			{
				UE_LOG(FTILiveLink, Warning, TEXT("Invalid data: dataSize:%d, start: %d, end: %d, streamData: %d"), 
					PreframeDataSize, start, end, streamData.Num());
				return false;
			}

			std::string cstr(reinterpret_cast<const char*>(streamData.GetData()+start), PreframeDataSize);
			FString streamString(cstr.c_str());

      UE_LOG(FTILiveLink, Verbose, TEXT("HandleReceivedData %s"), *streamString);
			HandleReceivedData(streamString);
		}
		//header start
		else
		{
			end = start + HEADER_SIZE;
			
			// Verify if the data is valid
			if (end > streamData.Num())
			{
				UE_LOG(FTILiveLink, Warning, TEXT("Invalid header data: start:%d, end: %d, streamData: %d"), 
					start, end, streamData.Num());
				return false;
			}

			// The first 4 bytes is little endian conversion bytes of int for data size
			PreframeDataSize = int(streamData[start] | streamData[start+1] << 8 |
	 									 			   streamData[start+2] << 16 | streamData[start+3] << 24);
			UE_LOG(FTILiveLink, Verbose, TEXT("Receive header: %d"), PreframeDataSize);
		}

		if (end >= streamData.Num())
		{
			UE_LOG(FTILiveLink, Verbose, TEXT("End data at %d beyond buffer size: %d"), 
					end, streamData.Num());
			break;
		}
		start = end;
	}
	return true;
}

void FFTILiveLinkSource::CloseConnection()
{
	UE_LOG(FTILiveLink, Verbose, TEXT("call FFTILiveLinkSource::CloseConnection"));
	IsListening = false;
	if (Socket != nullptr)
	{
		Socket->Close();
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(Socket);
		Socket = nullptr;
	}
	FPlatformProcess::Sleep(WaitTime);
}

uint32 FFTILiveLinkSource::Run()
{
	UE_LOG(FTILiveLink, Log, TEXT("Starting Tcp socket thread."));
	FDateTime startTime = FDateTime::UtcNow();

	while (!Stopping)
	{
		// Connect
		if (!IsListening)
		{
			if (!Socket)
			{
				//Setup socket: make sure create Socket through PLATFORM_SOCKETSUBSYSTEM, otherwise
				//observed the socket is not working cross computers in Windows
				Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("FTISOCKET"), false);
				int32 ActualRecvBufferSize;
				int32 ActualSendBufferSize;
				Socket->SetReceiveBufferSize(RECV_BUFFER_SIZE, ActualRecvBufferSize);
				Socket->SetSendBufferSize(RECV_BUFFER_SIZE, ActualSendBufferSize);
			}

			if (!Socket)
			{
				UE_LOG(FTILiveLink, Verbose, TEXT("Create socket failed"));
				return 0;
			}

			TSharedRef<FInternetAddr> internetAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
			internetAddr->SetIp(DeviceEndpoint.Address.Value);
			internetAddr->SetPort(DeviceEndpoint.Port);

			IsListening = Socket->Connect(*internetAddr);
			if (IsListening) 
			{
				UE_LOG(FTILiveLink, Log, TEXT("Connected."));
			}
			else 
			{
				UE_LOG(FTILiveLink, Log, TEXT("Couldn't connect"));
				CloseConnection();
				continue;
			}
		}

		if (!Socket)
		{
			UE_LOG(FTILiveLink, Error, TEXT("Socket is null."));
			Stopping = true;
			continue;
		}

		// Check if server side is ready, otherwise retry
		Socket->SetNonBlocking(true);
		int32 t_BytesRead;
		uint8 t_Dummy;
		if (!Socket->Recv(&t_Dummy, 1, t_BytesRead, ESocketReceiveFlags::Peek))
		{
			UE_LOG(FTILiveLink, Warning, TEXT("Socket connection is broken."));
			CloseConnection();
			continue;
		}
		
		Socket->SetNonBlocking(false);
		uint32 pendingDataSize = 0;
		int32 bytesReadTotal = 0;

		// Collect all pending data
		while (!Stopping)
		{
			// no data received
			if (!Socket->HasPendingData(pendingDataSize))
			{
				break;
			}

			RecvBuffer.SetNumUninitialized(bytesReadTotal + pendingDataSize);

			int32 bytesRead = 0;
			if (!Socket->Recv(RecvBuffer.GetData() + bytesReadTotal, pendingDataSize, bytesRead))
			{
				UE_LOG(FTILiveLink, Warning, TEXT("Failed receiving data."));					
				break;
			}
			bytesReadTotal += bytesRead;
		}

		// If there is data received then parse the data and call data handler
		if (!Stopping && bytesReadTotal != 0 && RecvBuffer.Num() > 0)
		{
			startTime = FDateTime::UtcNow();
			ParseFTIStreamData(RecvBuffer);
		}
		else 
		{
			// If connection state changed or timeout threshold reached, then we
			// think the connection is broken
			float noDataDuration = (FDateTime::UtcNow() - startTime).GetTotalSeconds();
			if ((Socket->GetConnectionState() != ESocketConnectionState::SCS_Connected) ||
					(noDataDuration > TimeOutSeconds))
			{
				UE_LOG(FTILiveLink, Warning, TEXT("Connection broken"));
				startTime = FDateTime::UtcNow();
				CloseConnection();
			}
		}
	}

	return 0;
}

void FFTILiveLinkSource::HandleReceivedData(const FString& JsonString)
{
	UE_LOG(FTILiveLink, Verbose, TEXT("call FFTILiveLinkSource::HandleReceivedData"));

	// If receiving is stopped or client is not set up then we just return
	if (Stopping || !Client)
	{
		return;
	}
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);
	FVector headPosVec;
	FQuat headRotQuat;
	FFTIAnimationValueData animationValues;
	TMap<FName, float> animationValuesMap;
	const FName ANIMATION_VALUE_KEY_NAME("animationValues");

	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		for (TPair<FString, TSharedPtr<FJsonValue>>& JsonField : JsonObject->Values)
		{
			FName KeyName(*JsonField.Key);
			if (KeyName == ANIMATION_VALUE_KEY_NAME)
			{
				animationValuesMap.Empty();

				const TSharedPtr<FJsonObject> animationValuesObject = JsonField.Value->AsObject();
				for (TPair<FString, TSharedPtr<FJsonValue>>& animationValueField : animationValuesObject->Values)
				{
					// animationValuesMap is used to set data property names and values so that UE can generate
					// pose data automatically
					animationValuesMap.Add(FName(*animationValueField.Key), animationValueField.Value->AsNumber());
					
					if (animationValueField.Key.Equals(TEXT("browDownLeft")))
					{
						animationValues.browDownLeft = animationValueField.Value->AsNumber();
					}
					else if (animationValueField.Key.Equals(TEXT("browOuterUpLeft")))
					{
						animationValues.browOuterUpLeft = animationValueField.Value->AsNumber();
					}
					else if (animationValueField.Key.Equals(TEXT("browInnerDown")))
					{
						animationValues.browInnerDown = animationValueField.Value->AsNumber();
					}
					else if (animationValueField.Key.Equals(TEXT("browInnerUp")))
					{
						animationValues.browInnerUp = animationValueField.Value->AsNumber();
					}
					else if (animationValueField.Key.Equals(TEXT("browDownRight")))
					{
						animationValues.browDownRight = animationValueField.Value->AsNumber();
					}
					else if (animationValueField.Key.Equals(TEXT("browOuterUpRight")))
					{
						animationValues.browOuterUpRight = animationValueField.Value->AsNumber();
					}
					else if (animationValueField.Key.Equals(TEXT("cheekSquintLeft")))
					{
						animationValues.cheekSquintLeft = animationValueField.Value->AsNumber();
					}
					else if (animationValueField.Key.Equals(TEXT("cheekSquintRight")))
					{
						animationValues.cheekSquintRight = animationValueField.Value->AsNumber();
					}
					else if (animationValueField.Key.Equals(TEXT("eyeBlinkLeft")))
					{
						animationValues.eyeBlinkLeft = animationValueField.Value->AsNumber();
					}
					else if (animationValueField.Key.Equals(TEXT("eyeWideLeft")))
					{
						animationValues.eyeWideLeft = animationValueField.Value->AsNumber();
					}
					else if (animationValueField.Key.Equals(TEXT("eyeLookDown")))
					{
						animationValues.eyeLookDown = animationValueField.Value->AsNumber();
					}
					else if (animationValueField.Key.Equals(TEXT("eyeLookLeft")))
					{
						animationValues.eyeLookLeft = animationValueField.Value->AsNumber();
					}
					else if (animationValueField.Key.Equals(TEXT("eyeLookRight")))
					{
						animationValues.eyeLookRight = animationValueField.Value->AsNumber();
					}
					else if (animationValueField.Key.Equals(TEXT("eyeLookUp")))
					{
						animationValues.eyeLookUp = animationValueField.Value->AsNumber();
					}
					else if (animationValueField.Key.Equals(TEXT("eyeBlinkRight")))
					{
						animationValues.eyeBlinkRight = animationValueField.Value->AsNumber();
					}
					else if (animationValueField.Key.Equals(TEXT("eyeWideRight")))
					{
						animationValues.eyeWideRight = animationValueField.Value->AsNumber();
					}
					else if (animationValueField.Key.Equals(TEXT("headDown")))
					{
						animationValues.headDown = animationValueField.Value->AsNumber();
					}
					else if (animationValueField.Key.Equals(TEXT("headLeft")))
					{
						animationValues.headLeft = animationValueField.Value->AsNumber();
					}
					else if (animationValueField.Key.Equals(TEXT("headTiltLeft")))
					{
						animationValues.headTiltLeft = animationValueField.Value->AsNumber();
					}
					else if (animationValueField.Key.Equals(TEXT("headRight")))
					{
						animationValues.headRight = animationValueField.Value->AsNumber();
					}
					else if (animationValueField.Key.Equals(TEXT("headTiltRight")))
					{
						animationValues.headTiltRight = animationValueField.Value->AsNumber();
					}
					else if (animationValueField.Key.Equals(TEXT("headUp")))
					{
						animationValues.headUp = animationValueField.Value->AsNumber();
					}
					else if (animationValueField.Key.Equals(TEXT("jawLeft")))
					{
						animationValues.jawLeft = animationValueField.Value->AsNumber();
					}
					else if (animationValueField.Key.Equals(TEXT("jawOpen")))
					{
						animationValues.jawOpen = animationValueField.Value->AsNumber();
					}
					else if (animationValueField.Key.Equals(TEXT("jawRight")))
					{
						animationValues.jawRight = animationValueField.Value->AsNumber();
					}
					else if (animationValueField.Key.Equals(TEXT("mouthDown")))
					{
						animationValues.mouthDown = animationValueField.Value->AsNumber();
					}
					else if (animationValueField.Key.Equals(TEXT("mouthLeft")))
					{
						animationValues.mouthLeft = animationValueField.Value->AsNumber();
					}
					else if (animationValueField.Key.Equals(TEXT("mouthFrownLeft")))
					{
						animationValues.mouthFrownLeft = animationValueField.Value->AsNumber();
					}
					else if (animationValueField.Key.Equals(TEXT("mouthNarrowLeft")))
					{
						animationValues.mouthNarrowLeft = animationValueField.Value->AsNumber();
					}
					else if (animationValueField.Key.Equals(TEXT("mouthSmileLeft")))
					{
						animationValues.mouthSmileLeft = animationValueField.Value->AsNumber();
					}
					else if (animationValueField.Key.Equals(TEXT("mouthStretchLeft")))
					{
						animationValues.mouthStretchLeft = animationValueField.Value->AsNumber();
					}
					else if (animationValueField.Key.Equals(TEXT("mouthLowerDownLeft")))
					{
						animationValues.mouthLowerDownLeft = animationValueField.Value->AsNumber();
					}
					else if (animationValueField.Key.Equals(TEXT("mouthLowerDownRight")))
					{
						animationValues.mouthLowerDownRight = animationValueField.Value->AsNumber();
					}
					else if (animationValueField.Key.Equals(TEXT("mouthFunnel")))
					{
						animationValues.mouthFunnel = animationValueField.Value->AsNumber();
					}
					else if (animationValueField.Key.Equals(TEXT("mouthPress")))
					{
						animationValues.mouthPress = animationValueField.Value->AsNumber();
					}
					else if (animationValueField.Key.Equals(TEXT("mouthPucker")))
					{
						animationValues.mouthPucker = animationValueField.Value->AsNumber();
					}
					else if (animationValueField.Key.Equals(TEXT("mouthRight")))
					{
						animationValues.mouthRight = animationValueField.Value->AsNumber();
					}
					else if (animationValueField.Key.Equals(TEXT("mouthFrownRight")))
					{
						animationValues.mouthFrownRight = animationValueField.Value->AsNumber();
					}
					else if (animationValueField.Key.Equals(TEXT("mouthNarrowRight")))
					{
						animationValues.mouthNarrowRight = animationValueField.Value->AsNumber();
					}
					else if (animationValueField.Key.Equals(TEXT("mouthSmileRight")))
					{
						animationValues.mouthSmileRight = animationValueField.Value->AsNumber();
					}
					else if (animationValueField.Key.Equals(TEXT("mouthStretchRight")))
					{
						animationValues.mouthStretchRight = animationValueField.Value->AsNumber();
					}
					else if (animationValueField.Key.Equals(TEXT("mouthUp")))
					{
						animationValues.mouthUp = animationValueField.Value->AsNumber();
					}
					else if (animationValueField.Key.Equals(TEXT("mouthUpperUpLeft")))
					{
						animationValues.mouthUpperUpLeft = animationValueField.Value->AsNumber();
					}
					else if (animationValueField.Key.Equals(TEXT("mouthUpperUpRight")))
					{
						animationValues.mouthUpperUpRight = animationValueField.Value->AsNumber();
					}
					else if (animationValueField.Key.Equals(TEXT("noseSneerLeft")))
					{
						animationValues.noseSneerLeft = animationValueField.Value->AsNumber();
					}
					else if (animationValueField.Key.Equals(TEXT("noseSneerRight")))
					{
						animationValues.noseSneerRight = animationValueField.Value->AsNumber();
					}
					else 
					{
						UE_LOG(FTILiveLink, Error, TEXT("HandleReceiveData - invalid animationValue"));
					}
				}
			}
			else if (KeyName == "headPos")
			{
				const TArray<TSharedPtr<FJsonValue>> headPosArray = JsonField.Value->AsArray();
				
				if (headPosArray.Num() == 3) // X, Y, Z
				{
					double x = headPosArray[0]->AsNumber();
					double y = headPosArray[1]->AsNumber();
					double z = headPosArray[2]->AsNumber();
					headPosVec = FVector(x, y, z);
				}
				else
				{
					// Invalid Json Format
					UE_LOG(FTILiveLink, Error, TEXT("HandleReceiveData - invalid headPos"));
					return;
				}
			}
			else if (KeyName == "headRot")
			{
				const TArray<TSharedPtr<FJsonValue>> headRotArray = JsonField.Value->AsArray();
				
				if (headRotArray.Num() == 3) // pitch, yaw, roll angles
				{
					float pitch = FMath::RadiansToDegrees<float>(headRotArray[0]->AsNumber());
					float yaw = FMath::RadiansToDegrees<float>(headRotArray[1]->AsNumber());
					float roll = FMath::RadiansToDegrees<float>(headRotArray[2]->AsNumber());
					headRotQuat = FQuat(FRotator(pitch, yaw, roll));
				}
				else
				{
					// Invalid Json Format
					UE_LOG(FTILiveLink, Error, TEXT("HandleReceiveData - invalid headRot"));
					return;
				}
			}

			bool bCreateSubject = !EncounteredSubjects.Contains(LiveLinkSubjectName);
			if (bCreateSubject && KeyName == ANIMATION_VALUE_KEY_NAME)
			{
				EncounteredSubjects.Add(LiveLinkSubjectName);
			}
		}
		
		// Update static data
		UE_LOG(FTILiveLink, Verbose, TEXT("HandleReceiveData - update static data"));
		FLiveLinkStaticDataStruct staticData(FFTIAnimationStaticData::StaticStruct());
		FFTIAnimationStaticData* animationStaticData = staticData.Cast<FFTIAnimationStaticData>();
		TArray<FName> propertyNames;
		animationValuesMap.GenerateKeyArray(propertyNames);
		animationStaticData->PropertyNames = propertyNames;
		Client->PushSubjectStaticData_AnyThread({ SourceGuid, LiveLinkSubjectName }, UFTILiveLinkRole::StaticClass(), MoveTemp(staticData));

		// Update frame data
		UE_LOG(FTILiveLink, Verbose, TEXT("HandleReceiveData - update frame data"));
		FLiveLinkFrameDataStruct frameData(FFTIAnimationFrameData::StaticStruct());
		FFTIAnimationFrameData* animationFrameData = frameData.Cast<FFTIAnimationFrameData>();
		TArray<float> propertyValues;
		animationValuesMap.GenerateValueArray(propertyValues);
		animationFrameData->PropertyValues = propertyValues;
		animationFrameData->AnimationValues = animationValues;
		animationFrameData->HeadPos = headPosVec;
		animationFrameData->HeadRot = headRotQuat;

		Client->PushSubjectFrameData_AnyThread({ SourceGuid, LiveLinkSubjectName }, MoveTemp(frameData));
		UE_LOG(FTILiveLink, Verbose, TEXT("HandleReceiveData - done"));
	}
}

#undef LOCTEXT_NAMESPACE
