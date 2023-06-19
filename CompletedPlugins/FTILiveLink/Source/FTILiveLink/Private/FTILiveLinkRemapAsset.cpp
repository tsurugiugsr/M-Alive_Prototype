// Copyright (c) 2022 Faceware Technologies, Inc. All rights reserved.

#include "FTILiveLinkRemapAsset.h"

void UFTILiveLinkRemapAsset::Initialize()
{
}

UFTILiveLinkRemapAsset::UFTILiveLinkRemapAsset(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AnimationNamesMap =
	{
		{TEXT("browDownLeft"), TEXT("browDownLeft")},
		{TEXT("browOuterUpLeft"), TEXT("browOuterUpLeft")},
		{TEXT("browInnerDown"), TEXT("browInnerDown")},
		{TEXT("browInnerUp"), TEXT("browInnerUp")},
		{TEXT("browDownRight"), TEXT("browDownRight")},
		{TEXT("browOuterUpRight"), TEXT("browOuterUpRight")},
		{TEXT("cheekSquintLeft"), TEXT("cheekSquintLeft")},
		{TEXT("cheekSquintRight"), TEXT("cheekSquintRight")},
		{TEXT("eyeBlinkLeft"), TEXT("eyeBlinkLeft")},
		{TEXT("eyeWideLeft"), TEXT("eyeWideLeft")},
		{TEXT("eyeLookDown"), TEXT("eyeLookDown")},
		{TEXT("eyeLookLeft"), TEXT("eyeLookLeft")},
		{TEXT("eyeLookRight"), TEXT("eyeLookRight")},
		{TEXT("eyeLookUp"), TEXT("eyeLookUp")},
		{TEXT("eyeBlinkRight"), TEXT("eyeBlinkRight")},
		{TEXT("eyeWideRight"), TEXT("eyeWideRight")},
		{TEXT("headDown"), TEXT("headDown")},
		{TEXT("headLeft"), TEXT("headLeft")},
		{TEXT("headTiltLeft"), TEXT("headTiltLeft")},
		{TEXT("headRight"), TEXT("headRight")},
		{TEXT("headTiltRight"), TEXT("headTiltRight")},
		{TEXT("headUp"), TEXT("headUp")},
		{TEXT("jawLeft"), TEXT("jawLeft")},
		{TEXT("jawOpen"), TEXT("jawOpen")},
		{TEXT("jawRight"), TEXT("jawRight")},
		{TEXT("mouthDown"), TEXT("mouthDown")},
		{TEXT("mouthLeft"), TEXT("mouthLeft")},
		{TEXT("mouthFrownLeft"), TEXT("mouthFrownLeft")},
		{TEXT("mouthNarrowLeft"), TEXT("mouthNarrowLeft")},
		{TEXT("mouthSmileLeft"), TEXT("mouthSmileLeft")},
		{TEXT("mouthStretchLeft"), TEXT("mouthStretchLeft")},
		{TEXT("mouthLowerDownLeft"), TEXT("mouthLowerDownLeft")},
		{TEXT("mouthLowerDownRight"), TEXT("mouthLowerDownRight")},
		{TEXT("mouthFunnel"), TEXT("mouthFunnel")},
		{TEXT("mouthPress"), TEXT("mouthPress")},
		{TEXT("mouthPucker"), TEXT("mouthPucker")},
		{TEXT("mouthRight"), TEXT("mouthRight")},
		{TEXT("mouthFrownRight"), TEXT("mouthFrownRight")},
		{TEXT("mouthNarrowRight"), TEXT("mouthNarrowRight")},
		{TEXT("mouthSmileRight"), TEXT("mouthSmileRight")},
		{TEXT("mouthStretchRight"), TEXT("mouthStretchRight")},
		{TEXT("mouthUp"), TEXT("mouthUp")},
		{TEXT("mouthUpperUpLeft"), TEXT("mouthUpperUpLeft")},
		{TEXT("mouthUpperUpRight"), TEXT("mouthUpperUpRight")},
		{TEXT("noseSneerLeft"), TEXT("noseSneerLeft")},
		{TEXT("noseSneerRight"), TEXT("noseSneerRight")},
	};
}

FName UFTILiveLinkRemapAsset::GetRemappedCurveName_Implementation(FName CurveName) const 
{
	if (auto Found = AnimationNamesMap.Find(CurveName))
	{
		return *Found;
	}
	return NAME_None;
}