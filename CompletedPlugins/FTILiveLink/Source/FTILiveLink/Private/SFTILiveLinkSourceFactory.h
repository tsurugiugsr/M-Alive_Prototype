// Copyright (c) 2022 Faceware Technologies, Inc. All rights reserved.

#pragma once

#include "Widgets/SCompoundWidget.h"
#include "Input/Reply.h"
#include "Types/SlateEnums.h"
#include "Widgets/DeclarativeSyntaxSupport.h"

class SEditableTextBox;

class SFTILiveLinkSourceFactory : public SCompoundWidget
{
public:
	DECLARE_DELEGATE_TwoParams(FOnOkClicked, FIPv4Endpoint, FName);

	SLATE_BEGIN_ARGS(SFTILiveLinkSourceFactory){}
		SLATE_EVENT(FOnOkClicked, OnOkClicked)
	SLATE_END_ARGS()

	void Construct(const FArguments& Args);

private:

	void OnIpAddressChanged(const FText& NewValue, ETextCommit::Type type);
	void OnPortNumberChanged(const FText& NewValue, ETextCommit::Type type);
	void OnSubjectNameChanged(const FText& NewValue, ETextCommit::Type type);

	FReply OnOkClicked();

	TWeakPtr<SEditableTextBox> IpAddressEditabledText;
	TWeakPtr<SEditableTextBox> PortNumberEditabledText;
	TWeakPtr<SEditableTextBox> SubjectNameEditabledText;

	FOnOkClicked OkClicked;
};