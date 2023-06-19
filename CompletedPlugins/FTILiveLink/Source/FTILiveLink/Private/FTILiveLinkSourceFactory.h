// Copyright (c) 2022 Faceware Technologies, Inc. All rights reserved.

#pragma once

#include "LiveLinkSourceFactory.h"
#include "Interfaces/IPv4/IPv4Endpoint.h"
#include "FTILiveLinkSourceFactory.generated.h"

class SFTILiveLinkSourceEditor;

UCLASS()
class UFTILiveLinkSourceFactory : public ULiveLinkSourceFactory
{
public:

	GENERATED_BODY()

	virtual FText GetSourceDisplayName() const override;
	virtual FText GetSourceTooltip() const override;

	virtual EMenuType GetMenuType() const override { return EMenuType::SubPanel; }
	virtual TSharedPtr<SWidget> BuildCreationPanel(FOnLiveLinkSourceCreated OnLiveLinkSourceCreated) const override;
	TSharedPtr<ILiveLinkSource> CreateSource(const FString& ConnectionString) const override;
private:
	void OnOkClicked(FIPv4Endpoint Endpoint, FName SubjectName, FOnLiveLinkSourceCreated OnLiveLinkSourceCreated) const;
};