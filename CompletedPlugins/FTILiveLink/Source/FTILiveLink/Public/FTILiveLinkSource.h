// Copyright (c) 2022 Faceware Technologies, Inc. All rights reserved.

#pragma once

#include "ILiveLinkSource.h"
#include "HAL/Runnable.h"
#include "HAL/ThreadSafeBool.h"
#include "IMessageContext.h"
#include "Interfaces/IPv4/IPv4Endpoint.h"

//enable logging step 1
DECLARE_LOG_CATEGORY_EXTERN(FTILiveLink, Log, All)

class FRunnableThread;
class FSocket;
class ILiveLinkClient;
class ISocketSubsystem;

class FTILIVELINK_API FFTILiveLinkSource : public ILiveLinkSource, public FRunnable
{
public:

	FFTILiveLinkSource(FIPv4Endpoint Endpoint, FName SubjectName = "animationValues");

	virtual ~FFTILiveLinkSource();

	// Begin ILiveLinkSource Interface
	
	virtual void ReceiveClient(ILiveLinkClient* InClient, FGuid InSourceGuid) override;

	virtual bool IsSourceStillValid() const override;

	virtual bool RequestSourceShutdown() override;

	virtual FText GetSourceType() const override { return SourceType; };
	virtual FText GetSourceMachineName() const override { return SourceMachineName; }
	virtual FText GetSourceStatus() const override { return SourceStatus; }
	virtual TSubclassOf<ULiveLinkSourceSettings> GetSettingsClass() const override;
	// End ILiveLinkSource Interface

	// Begin FRunnable Interface

	virtual bool Init() override;
	virtual uint32 Run() override;
	void Start();
	virtual void Stop() override;
	virtual void Exit() override { }

	// End FRunnable Interface

	void HandleReceivedData(const FString& JsonString);

private:
	bool ParseFTIStreamData(const TArray<uint8>& streamData);
	void CloseConnection();

private:

	ILiveLinkClient* Client = nullptr;
	TFuture<void> ClientConnectionFinishedFuture;

	// Our identifier in LiveLink
	FGuid SourceGuid;

	FMessageAddress ConnectionAddress;

	FText SourceType;
	FText SourceMachineName;
	FText SourceStatus;

	FIPv4Endpoint DeviceEndpoint;

	// Socket to receive data on
	FSocket* Socket;

	// Is the listening socket running 
	bool IsListening = false;
	int PreframeDataSize = 0;

	// Subsystem associated to Socket
	ISocketSubsystem* SocketSubsystem;

	// Threadsafe Bool for terminating the main thread loop
	FThreadSafeBool Stopping;

	// Thread to run socket operations on
	FRunnableThread* Thread;

	// Name of the sockets thread
	FString ThreadName;

	// Time to wait between attempted connection
	float WaitTime;

	// How long since we donnot receive data, then we think the connection is broken 
	float TimeOutSeconds;

	// List of subjects we've already encountered
	TSet<FName> EncounteredSubjects;

	// Buffer to receive socket data into
	TArray<uint8> RecvBuffer;

	// Name of the live link subject
	FName LiveLinkSubjectName;
};
