// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "LotusBpLibBPLibrary.generated.h"

/* 
*	Function library class.
*	Each function in it is expected to be static and represents blueprint node that can be called in any blueprint.
*
*	When declaring function you can define metadata for the node. Key function specifiers will be BlueprintPure and BlueprintCallable.
*	BlueprintPure - means the function does not affect the owning object in any way and thus creates a node without Exec pins.
*	BlueprintCallable - makes a function which can be executed in Blueprints - Thus it has Exec pins.
*	DisplayName - full name of the node, shown when you mouse over the node and in the blueprint drop down menu.
*				Its lets you name the node using characters not allowed in C++ function names.
*	CompactNodeTitle - the word(s) that appear on the node.
*	Keywords -	the list of keywords that helps you to find node when you search for it using Blueprint drop-down menu. 
*				Good example is "Print String" node which you can find also by using keyword "log".
*	Category -	the category your node will be under in the Blueprint drop-down menu.
*
*	For more info on custom blueprint nodes visit documentation:
*	https://wiki.unrealengine.com/Custom_Blueprint_Node_Creation
*/

//float HeadPositionX = 0;
//float HeadPositionY = 0;
//float HeadPositionZ = 0;

//int32 nVpaState = 0;


//TArray<FName> ArKitCurveName ;
//TArray<float> ArKitCurveValue = {};

UCLASS()
class ULotusBpLibBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Execute Sample function", Keywords = "LotusBpLib sample test testing"), Category = "LotusBpLib")
	static float LotusBpLibSampleFunction(float Param);

	UFUNCTION(BlueprintPure, Category = "LotusBpLib")
	static int64 GetUtcTime();

	UFUNCTION(BlueprintPure, Category = "LotusBpLib")
	static TArray<FName> GetBsName();

	UFUNCTION(BlueprintPure, Category = "LotusBpLib")
	static TArray<FName> GetDazBsName();

	UFUNCTION(BlueprintPure, Category = "LotusBpLib")
	static TArray<FName> GetMetaHumanBsName();

	UFUNCTION(BlueprintPure, Category = "LotusBpLib")
	static TArray<float> ReadMorphDataFromFile(FString FilePathName);

	UFUNCTION(BlueprintPure, Category = "LotusBpLib")
	static float GetHeadPositionX();
	UFUNCTION(BlueprintPure, Category = "LotusBpLib")
	static float GetHeadPositionY();
	UFUNCTION(BlueprintPure, Category = "LotusBpLib")
	static float GetHeadPositionZ();

	UFUNCTION(BlueprintCallable, Category = "LotusBpLib")
	static void SetHeadPosition(float x, float y, float z);

	UFUNCTION(BlueprintPure, Category = "LotusBpLib")
	static TArray<float> ReadBodyDataFromFile(FString FilePathName);

	UFUNCTION(BlueprintPure, Category = "LotusBpLib")
	static int32 GetVpaState();

	UFUNCTION(BlueprintCallable, Category = "LotusBpLib")
	static void SetVpaState(int32 State);

	UFUNCTION(BlueprintCallable, Category = "LotusBpLib")
	static TArray<FString> LoadA2FName(FString FilePathName);
	UFUNCTION(BlueprintCallable, Category = "LotusBpLib")
	static TArray<float> LoadA2FData(FString FilePathName);

	UFUNCTION(BlueprintCallable, Category = "LotusBpLib")
	static void SetArKitCurveName(TArray<FName> NameList);

	UFUNCTION(BlueprintCallable, Category = "LotusBpLib")
	static void SetArKitCurveValue(TArray<float> ValueList);




	UFUNCTION(BlueprintPure, Category = "LotusBpLib")
	static FString AsrCreateUrl(FString url, FString AppId, FString ApiKey, FString ApiSecret);


	UFUNCTION(BlueprintPure, Category = "LotusBpLib")
	static TArray<uint8> GetSoundWavePCMData(const class USoundWave* SoundWave);
};
