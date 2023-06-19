// Copyright (c) 2022 Faceware Technologies, Inc. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "LiveLinkRemapAsset.h"

#include "FTILiveLinkRemapAsset.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class FTILIVELINK_API UFTILiveLinkRemapAsset : public ULiveLinkRemapAsset
{
 	GENERATED_BODY()

  virtual void Initialize() override;
	
public:
	UFTILiveLinkRemapAsset(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, Category = "FacewareLiveLink")
	TMap<FName, FName> AnimationNamesMap;

	FName GetRemappedCurveName_Implementation(FName CurveName) const override;
};
