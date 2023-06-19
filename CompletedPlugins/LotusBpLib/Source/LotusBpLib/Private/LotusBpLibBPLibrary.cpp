// Copyright Epic Games, Inc. All Rights Reserved.

#include "LotusBpLibBPLibrary.h"
#include "LotusBpLib.h"

#include "Misc/SecureHash.h"
#include "Misc/Base64.h"
#include "CryptoHelper/Public/CryptoHelperBPLibrary.h"
#include "CryptoHelper/Public/CryptoHelper.h"
#include "Sound/SoundWave.h"

ULotusBpLibBPLibrary::ULotusBpLibBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

float ULotusBpLibBPLibrary::LotusBpLibSampleFunction(float Param)
{
	return -1;
}

int64 ULotusBpLibBPLibrary::GetUtcTime()
{
	FDateTime startTime = FDateTime::UtcNow();
	return startTime.GetTicks() / 10000;
}

TArray<FName> ULotusBpLibBPLibrary::GetBsName()
{
	TArray<FName> temp = {
		"jawForward", "jawLeft", "jawRight",
		"jawOpen", "mouthClose", "mouthLeft",
		"mouthRight", "mouthSmileLeft", "mouthSmileRight",
		"mouthDimpleLeft", "mouthDimpleRight", "mouthStretchLeft",
		"mouthStretchRight", "mouthShrugLower", "mouthShrugUpper",
		"mouthPressLeft", "mouthPressRight", "mouthLowerDownLeft",
		"mouthLowerDownRight", "mouthUpperUpLeft", "mouthUpperUpRight",
		"mouthFunnel", "mouthPucker", "mouthFrownLeft",
		"mouthFrownRight", "mouthRollLower", "mouthRollUpper" };
	return temp;
}

TArray<FName> ULotusBpLibBPLibrary::GetDazBsName()
{
	TArray<FName> temp = {
		"Genesis8_1Female__facs_jnt_JawForward", "Genesis8_1Female__facs_jnt_JawLeft", "Genesis8_1Female__facs_jnt_JawRight",
		"Genesis8_1Female__facs_jnt_JawOpen", "Genesis8_1Female__facs_bs_MouthClose_div2", "Genesis8_1Female__facs_bs_MouthLeft_div2",
		"Genesis8_1Female__facs_bs_MouthRight_div2", "Genesis8_1Female__facs_bs_MouthSmileLeft_div2", "Genesis8_1Female__facs_bs_MouthSmileRight_div2",
		"Genesis8_1Female__facs_bs_MouthDimpleLeft_div2", "Genesis8_1Female__facs_bs_MouthDimpleRight_div2", "Genesis8_1Female__facs_bs_MouthStretchLeft_div2",
		"Genesis8_1Female__facs_bs_MouthStretchRight_div2", "Genesis8_1Female__facs_bs_MouthShrugLower_div2", "Genesis8_1Female__facs_bs_MouthShrugUpper_div2",
		"Genesis8_1Female__facs_bs_MouthPressLeft_div2", "Genesis8_1Female__facs_bs_MouthPressRight_div2", "Genesis8_1Female__facs_bs_MouthLowerDownLeft_div2",
		"Genesis8_1Female__facs_bs_MouthLowerDownRight_div2", "Genesis8_1Female__facs_bs_MouthUpperUpLeft_div2", "Genesis8_1Female__facs_bs_MouthUpperUpRight_div2",
		"Genesis8_1Female__facs_bs_MouthFunnel_div2", "Genesis8_1Female__facs_bs_MouthPucker_div2", "Genesis8_1Female__facs_bs_MouthFrownLeft_div2",
		"Genesis8_1Female__facs_bs_MouthFrownRight_div2", "Genesis8_1Female__facs_bs_MouthRollLower_div2", "Genesis8_1Female__facs_bs_MouthRollUpper_div2" };
	return temp;
}

TArray<FName> ULotusBpLibBPLibrary::GetMetaHumanBsName()
{
	TArray<FName> temp = {
		"Genesis8_1Female__facs_jnt_JawForward", "Genesis8_1Female__facs_jnt_JawLeft", "Genesis8_1Female__facs_jnt_JawRight",
		"Genesis8_1Female__facs_jnt_JawOpen", "Genesis8_1Female__facs_bs_MouthClose_div2", "Genesis8_1Female__facs_bs_MouthLeft_div2",
		"Genesis8_1Female__facs_bs_MouthRight_div2", "Genesis8_1Female__facs_bs_MouthSmileLeft_div2", "Genesis8_1Female__facs_bs_MouthSmileRight_div2",
		"Genesis8_1Female__facs_bs_MouthDimpleLeft_div2", "Genesis8_1Female__facs_bs_MouthDimpleRight_div2", "Genesis8_1Female__facs_bs_MouthStretchLeft_div2",
		"Genesis8_1Female__facs_bs_MouthStretchRight_div2", "Genesis8_1Female__facs_bs_MouthShrugLower_div2", "Genesis8_1Female__facs_bs_MouthShrugUpper_div2",
		"Genesis8_1Female__facs_bs_MouthPressLeft_div2", "Genesis8_1Female__facs_bs_MouthPressRight_div2", "Genesis8_1Female__facs_bs_MouthLowerDownLeft_div2",
		"Genesis8_1Female__facs_bs_MouthLowerDownRight_div2", "Genesis8_1Female__facs_bs_MouthUpperUpLeft_div2", "Genesis8_1Female__facs_bs_MouthUpperUpRight_div2",
		"Genesis8_1Female__facs_bs_MouthFunnel_div2", "Genesis8_1Female__facs_bs_MouthPucker_div2", "Genesis8_1Female__facs_bs_MouthFrownLeft_div2",
		"Genesis8_1Female__facs_bs_MouthFrownRight_div2", "Genesis8_1Female__facs_bs_MouthRollLower_div2", "Genesis8_1Female__facs_bs_MouthRollUpper_div2" };
	return temp;
}

TArray<float> ULotusBpLibBPLibrary::ReadMorphDataFromFile(FString FilePathName)
{
	TArray<float> temp;

	TArray<FString> Result;
	FFileHelper::LoadFileToStringArray(Result, *FilePathName);

	TArray<FString> Result1;
	for (int i = 0; i < Result.Num(); i++)
	{
		temp.Add(FCString::Atof(*Result[i]));
	}

	return temp;
}

TArray<float> ULotusBpLibBPLibrary::ReadBodyDataFromFile(FString FilePathName)
{
	TArray<float> temp;

	TArray<FString> Result;
	FFileHelper::LoadFileToStringArray(Result, *FilePathName);

	TArray<FString> Result1;
	for (int i = 0; i < Result.Num(); i++)
	{
		temp.Add(FCString::Atof(*Result[i]));
	}

	return temp;
}


float ULotusBpLibBPLibrary::GetHeadPositionX()
{
	return 0;
}
float ULotusBpLibBPLibrary::GetHeadPositionY()
{
	return 0;
}
float ULotusBpLibBPLibrary::GetHeadPositionZ()
{
	return 0;
}

void ULotusBpLibBPLibrary::SetHeadPosition(float x, float y, float z)
{
	//HeadPositionX = x;
	//HeadPositionY = y;
	//HeadPositionZ = z;
}

int32 ULotusBpLibBPLibrary::GetVpaState()
{
	return 0;
}

void ULotusBpLibBPLibrary::SetVpaState(int32 State)
{
	//nVpaState = State;
}

// a2fname Load
TArray<FString> ULotusBpLibBPLibrary::LoadA2FName(FString FilePathName)
{
	TArray<FString> Result;
	FFileHelper::LoadFileToStringArray(Result, *FilePathName);

	return Result;
}

TArray<float> ULotusBpLibBPLibrary::LoadA2FData(FString FilePathName)
{
	TArray<FString> Result;
	FFileHelper::LoadFileToStringArray(Result, *FilePathName);

	TArray<float> Data;
	for (int i = 0; i < Result.Num(); i++)
	{
		Data.Add(FCString::Atof(*Result[i]));
	}
	return Data;
}

FString ULotusBpLibBPLibrary::AsrCreateUrl(FString url, FString AppId, FString ApiKey, FString ApiSecret)
{
	FString temp = "wss://ws-api.xfyun.cn/v2/iat?authorization=YXBpX2tleT0iOWUxMTZmNWU0NGYwYjU3NmFjODQyM2RlMTZiOTRhZmQiLCBhbGdvcml0aG09ImhtYWMtc2hhMjU2IiwgaGVhZGVycz0iaG9zdCBkYXRlIHJlcXVlc3QtbGluZSIsIHNpZ25hdHVyZT0id0NXZkJnY295MERHVjFZSXFPakNweVRVY0JsUkk1enRTSHROSTJyNldNST0i&date=Tue%2C+03+May+2022+03%3A37%3A33+GMT&host=ws-api.xfyun.cn";
	
	FDateTime DateTime = FDateTime::UtcNow();
	FString HttpDate = DateTime.ToHttpDate();

	FString temp1 = "host: ws-api.xfyun.cn\n";
	temp1 += "date: " + HttpDate + "\nGET /v2/iat HTTP/1.1";

	// 
	//FString StrData;
	const TCHAR* StrPtr = *ApiSecret;
	FTCHARToUTF8 UTF8String(StrPtr);
	int32 CTXSize = UTF8String.Length();
	TArray<uint8> DataApiSecret;
	DataApiSecret.SetNum(CTXSize);
	memcpy(DataApiSecret.GetData(), UTF8String.Get(), CTXSize);

	const TCHAR* StrPtr1 = *temp1;
	FTCHARToUTF8 UTF8String1(StrPtr1);
	int32 CTXSize1 = UTF8String1.Length();
	TArray<uint8> Datatemp1;
	Datatemp1.SetNum(CTXSize1);
	memcpy(Datatemp1.GetData(), UTF8String1.Get(), CTXSize1);


	uint8 OutCookie[20] = {};
	FSHA1::HMACBuffer(DataApiSecret.GetData(), DataApiSecret.Num(), Datatemp1.GetData(), Datatemp1.Num(), OutCookie);

	FString temp2 = FBase64::Encode(OutCookie, 20);


	FString temp3 = {};
	UCryptoHelperBPLibrary::HMACString(temp1, ApiKey, temp3);


	return temp;
}

void ULotusBpLibBPLibrary::SetArKitCurveName(TArray<FName> NameList)
{

}

void ULotusBpLibBPLibrary::SetArKitCurveValue(TArray<float> ValueList)
{
	
}


TArray<uint8> ULotusBpLibBPLibrary::GetSoundWavePCMData(const USoundWave* SoundWave)
{
	TArray<uint8> Resault;
	if (SoundWave != nullptr)
	{
		Resault.SetNum(SoundWave->RawPCMDataSize);
		memcpy(Resault.GetData(), SoundWave->RawPCMData, SoundWave->RawPCMDataSize);
	}
	return Resault;
}
