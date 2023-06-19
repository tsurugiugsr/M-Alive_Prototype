// Copyright (c) 2022 Faceware Technologies, Inc. All rights reserved.

#pragma once

#include "Roles/LiveLinkAnimationRole.h"
#include "Roles/LiveLinkTransformRole.h"

#include "FTILiveLinkType.h"

#include "FTILiveLinkRole.generated.h"

/**
 * Role associated for FTILiveLink. 
 */
UCLASS(BlueprintType, meta = (DisplayName = "FacewareLiveLink Role"))
class FTILIVELINK_API UFTILiveLinkRole : public ULiveLinkBasicRole
{
	GENERATED_BODY()

public:
	virtual UScriptStruct* GetStaticDataStruct() const override;
	virtual UScriptStruct* GetFrameDataStruct() const override;
	virtual UScriptStruct* GetBlueprintDataStruct() const override;

	bool InitializeBlueprintData(const FLiveLinkSubjectFrameData& InSourceData, FLiveLinkBlueprintDataStruct& OutBlueprintData) const override;

	virtual FText GetDisplayName() const override;
};

