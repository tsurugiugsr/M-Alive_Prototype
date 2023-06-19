// Copyright (c) 2022 Faceware Technologies, Inc. All rights reserved.

#pragma once

#include "LiveLinkSourceSettings.h"
#include "FTILiveLinkSourceSettings.generated.h"

class SFTILiveLinkSourceEditor;

UCLASS()
class UFTILiveLinkSourceSettings : public ULiveLinkSourceSettings
{
public:
	GENERATED_BODY()

#if WITH_EDITOR
	virtual bool CanEditChange(const FProperty* InProperty) const override;
#endif
};