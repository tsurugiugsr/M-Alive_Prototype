// Copyright (c) 2022 Faceware Technologies, Inc. All rights reserved.

#include "FTILiveLinkSourceFactory.h"
#include "FTILiveLinkSource.h"
#include "SFTILiveLinkSourceFactory.h"

#define LOCTEXT_NAMESPACE "FTILiveLinkSourceFactory"
#define ENDPOINT_KEY_NAME "EndPoint"
#define SUBJECTNAME_KEY_NAME "SubjectName"

FText UFTILiveLinkSourceFactory::GetSourceDisplayName() const
{
	return LOCTEXT("SourceDisplayName", "Faceware Live Link");
}

FText UFTILiveLinkSourceFactory::GetSourceTooltip() const
{
	return LOCTEXT("SourceTooltip", "Creates a connection to a Faceware Studio Stream");
}

TSharedPtr<SWidget> UFTILiveLinkSourceFactory::BuildCreationPanel(FOnLiveLinkSourceCreated InOnLiveLinkSourceCreated) const
{
	return SNew(SFTILiveLinkSourceFactory)
		.OnOkClicked(SFTILiveLinkSourceFactory::FOnOkClicked::CreateUObject(this, &UFTILiveLinkSourceFactory::OnOkClicked, InOnLiveLinkSourceCreated));
}

TSharedPtr<ILiveLinkSource> UFTILiveLinkSourceFactory::CreateSource(const FString& InConnectionString) const
{
	UE_LOG(FTILiveLink, Verbose, TEXT("call UFTILiveLinkSourceFactory::CreateSource"));
	
	// Parse connection Json string
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(InConnectionString);

	if (!FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		UE_LOG(FTILiveLink, Verbose, TEXT("UFTILiveLinkSourceFactory::CreateSource deserialize connection json string failure"));

		return TSharedPtr<ILiveLinkSource>();
	}

	FIPv4Endpoint DeviceEndPoint;
	FName SubjectName;

	for (TPair<FString, TSharedPtr<FJsonValue>>& JsonField : JsonObject->Values)
	{
		FName KeyName(*JsonField.Key);
		if (KeyName == ENDPOINT_KEY_NAME)
		{
			FString endPointString = JsonField.Value->AsString();
			if (!FIPv4Endpoint::Parse(endPointString, DeviceEndPoint))
			{
				UE_LOG(FTILiveLink, Verbose, TEXT("UFTILiveLinkSourceFactory::CreateSource parse endpoint failure"));

				return TSharedPtr<ILiveLinkSource>();
			}
		}
		else if (KeyName == SUBJECTNAME_KEY_NAME)
		{
			SubjectName = FName(*JsonField.Value->AsString());
		}
	}

	UE_LOG(FTILiveLink, Verbose, TEXT("call UFTILiveLinkSourceFactory::CreateSource SubjectName: %s"), *SubjectName.ToString());
	if (SubjectName.IsNone())
	{
		return MakeShared<FFTILiveLinkSource>(DeviceEndPoint);
	}
	return MakeShared<FFTILiveLinkSource>(DeviceEndPoint, SubjectName);
}

void UFTILiveLinkSourceFactory::OnOkClicked(FIPv4Endpoint InEndpoint, FName SubjectName, FOnLiveLinkSourceCreated InOnLiveLinkSourceCreated) const
{
	UE_LOG(FTILiveLink, Verbose, TEXT("call UFTILiveLinkSourceFactory::OnOkClicked"));

	// Create a Json string for saving endpoint and subject name
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	JsonObject->SetStringField(ENDPOINT_KEY_NAME, InEndpoint.ToString());
	JsonObject->SetStringField(SUBJECTNAME_KEY_NAME, SubjectName.ToString());
	FString JsonString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	InOnLiveLinkSourceCreated.ExecuteIfBound(MakeShared<FFTILiveLinkSource>(InEndpoint, SubjectName), JsonString);
}

#undef LOCTEXT_NAMESPACE