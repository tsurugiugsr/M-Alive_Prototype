// Copyright (c) 2022 Faceware Technologies, Inc. All rights reserved.

#pragma once

#include "Roles/LiveLinkAnimationTypes.h"
#include "Roles/LiveLinkTransformTypes.h"

#include "FTILiveLinkType.generated.h"

/**
 * Struct for one FTI animation value
 */
USTRUCT(BlueprintType)
struct FFTIAnimationValueData
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FTIAnimationValues")
	float browDownLeft = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FTIAnimationValues")
	float browOuterUpLeft = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FTIAnimationValues")
	float browInnerDown = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FTIAnimationValues")
	float browInnerUp = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FTIAnimationValues")
	float browDownRight = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FTIAnimationValues")
	float browOuterUpRight = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FTIAnimationValues")
	float cheekSquintLeft = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FTIAnimationValues")
	float cheekSquintRight = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FTIAnimationValues")
	float eyeBlinkLeft = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FTIAnimationValues")
	float eyeWideLeft = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FTIAnimationValues")
	float eyeLookDown = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FTIAnimationValues")
	float eyeLookLeft = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FTIAnimationValues")
	float eyeLookRight = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FTIAnimationValues")
	float eyeLookUp = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FTIAnimationValues")
	float eyeBlinkRight = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FTIAnimationValues")
	float eyeWideRight = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FTIAnimationValues")
	float headDown = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FTIAnimationValues")
	float headLeft = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FTIAnimationValues")
	float headTiltLeft = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FTIAnimationValues")
	float headRight = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FTIAnimationValues")
	float headTiltRight = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FTIAnimationValues")
	float headUp = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FTIAnimationValues")
	float jawLeft = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FTIAnimationValues")
	float jawOpen = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FTIAnimationValues")
	float jawRight = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FTIAnimationValues")
	float mouthDown = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FTIAnimationValues")
	float mouthLeft = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FTIAnimationValues")
	float mouthFrownLeft = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FTIAnimationValues")
	float mouthNarrowLeft = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FTIAnimationValues")
	float mouthSmileLeft = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FTIAnimationValues")
	float mouthStretchLeft = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FTIAnimationValues")
	float mouthLowerDownLeft = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FTIAnimationValues")
	float mouthLowerDownRight = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FTIAnimationValues")
	float mouthFunnel = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FTIAnimationValues")
	float mouthPress = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FTIAnimationValues")
	float mouthPucker = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FTIAnimationValues")
	float mouthRight = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FTIAnimationValues")
	float mouthFrownRight = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FTIAnimationValues")
	float mouthNarrowRight = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FTIAnimationValues")
	float mouthSmileRight = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FTIAnimationValues")
	float mouthStretchRight = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FTIAnimationValues")
	float mouthUp = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FTIAnimationValues")
	float mouthUpperUpLeft = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FTIAnimationValues")
	float mouthUpperUpRight = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FTIAnimationValues")
	float noseSneerLeft = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FTIAnimationValues")
	float noseSneerRight = 0;
};

/**
 * Static data for FTI animation data
 */
USTRUCT(BlueprintType)
struct FTILIVELINK_API FFTIAnimationStaticData : public FLiveLinkBaseStaticData
{
	GENERATED_BODY()

public:
};


/**
 * Struct for one frame of FTI animation data
 */
USTRUCT(BlueprintType)
struct FTILIVELINK_API FFTIAnimationFrameData : public FLiveLinkBaseFrameData
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FTIAnimationValues")
	FFTIAnimationValueData AnimationValues;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FTIAnimationValues")
	FVector HeadPos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FTIAnimationValues")
	FQuat HeadRot;
};

/**
 * Facility structure to handle animation data in blueprint
 */
USTRUCT(BlueprintType)
struct FTILIVELINK_API FFTIAnimationBlueprintData : public FLiveLinkBaseBlueprintData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FacewareLiveLink")
	FFTIAnimationValueData AnimationValues;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FacewareLiveLink")
	FVector HeadPos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FacewareLiveLink")
	FQuat HeadRot;
};

