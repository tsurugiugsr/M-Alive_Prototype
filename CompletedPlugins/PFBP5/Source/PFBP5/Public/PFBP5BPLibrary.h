// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include <string>
#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Interfaces/IHttpRequest.h"
#include "PFBP5BPLibrary.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHttpResponseDelegatge, int32, Code, const TArray<uint8>&, Data);

UCLASS()
class UPFBP5BPLibrary : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	FString result;

	UFUNCTION(BlueprintCallable, Category = "ProtobuffPlugin", Meta = (DisplayName = "Encode ProtoBuf"))
		static FString encodeRequest(const FString& content, const FString& tts_type);
	UFUNCTION(BlueprintCallable, Category = "ProtobuffPlugin", Meta = (DisplayName = "Decode ProtoBuf"))
		static FString decodeRequest(const FString& r, FString& content, FString& tts_type);

	UFUNCTION(BlueprintCallable, Category = "ProtobuffPlugin", Meta = (DisplayName = "Audio 2 Face"))
		static FString Audio2Face(const TArray<uint8>& pbAudio, FString& content, float& emotion, TArray<float>& bs_value, TArray<FName>& bs_key, TArray<uint8>& wav_data);

	UFUNCTION(BlueprintCallable, Category = "ProtobuffPlugin", Meta = (DisplayName = "Http Post"))
		static UPFBP5BPLibrary* requestPost(const FString& url, const FString& data, const FString& content, const FString& tts_type);

	void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	UFUNCTION(BlueprintCallable, Category = "ProtobuffPlugin", Meta = (DisplayName = "Get Post Result"))
		static FString getResult(UPFBP5BPLibrary* p);

	void sendPOSTRequest(const FString& url, const FString& data, const FString& content, const FString& tts_type);

	UPROPERTY(BlueprintAssignable)
		FHttpResponseDelegatge OnSuccess;

	UPROPERTY(BlueprintAssignable)
		FHttpResponseDelegatge OnFail;
};
