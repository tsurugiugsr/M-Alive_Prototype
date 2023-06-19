// Copyright (c) 2022 Faceware Technologies, Inc. All rights reserved.

#include "FTILiveLinkRole.h"

#include "FTILiveLinkType.h"
#include "LiveLinkRoleTrait.h"

#define LOCTEXT_NAMESPACE "FTILiveLinkRole"

/**
 * UFTILiveLinkRole
 */

UScriptStruct* UFTILiveLinkRole::GetStaticDataStruct() const
{
	return FFTIAnimationStaticData::StaticStruct();
}

UScriptStruct* UFTILiveLinkRole::GetFrameDataStruct() const
{
	return FFTIAnimationFrameData::StaticStruct();
}

UScriptStruct* UFTILiveLinkRole::GetBlueprintDataStruct() const
{
	return FFTIAnimationBlueprintData::StaticStruct();
}

bool UFTILiveLinkRole::InitializeBlueprintData(const FLiveLinkSubjectFrameData& InSourceData, FLiveLinkBlueprintDataStruct& OutBlueprintData) const
{
	bool is_success = false;

	FFTIAnimationBlueprintData* blueprintData = OutBlueprintData.Cast<FFTIAnimationBlueprintData>();
	const FFTIAnimationFrameData* frameData = InSourceData.FrameData.Cast<FFTIAnimationFrameData>();
	if (blueprintData && frameData)
	{
		blueprintData->AnimationValues = frameData->AnimationValues;
		blueprintData->HeadPos = frameData->HeadPos;
		blueprintData->HeadRot = frameData->HeadRot;
		is_success = true;
	}

	return is_success;
}

FText UFTILiveLinkRole::GetDisplayName() const {

	return LOCTEXT("FTILiveLinkRole", "FacewareLiveLink");
}


#undef LOCTEXT_NAMESPACE