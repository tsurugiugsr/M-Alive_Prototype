// Copyright (c) 2022 Faceware Technologies, Inc. All rights reserved.

#include "SFTILiveLinkSourceFactory.h"
#include "Interfaces/IPv4/IPv4Endpoint.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"

#define LOCTEXT_NAMESPACE "FTILiveLinkSourceEditor"
#define DEFAULT_PORT_NUMBER 802
#define DEFAULT_SUBJECT_NAME "animationValues"

void SFTILiveLinkSourceFactory::Construct(const FArguments& Args)
{
	OkClicked = Args._OnOkClicked;

	FIPv4Endpoint Endpoint;
	Endpoint.Address = FIPv4Address::InternalLoopback;
	Endpoint.Port = DEFAULT_PORT_NUMBER;
	FName SubjectName = DEFAULT_SUBJECT_NAME;

	ChildSlot
	[
		SNew(SBox)
		.WidthOverride(250)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Left)
				.FillWidth(0.5f)
				[
					SNew(STextBlock)
					.Text(LOCTEXT("FTIIpAddress", "Hostname or IP"))
				]
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Fill)
				.FillWidth(0.5f)
				[
					SAssignNew(IpAddressEditabledText, SEditableTextBox)
					.Text(Endpoint.Address.ToText())
					.OnTextCommitted(this, &SFTILiveLinkSourceFactory::OnIpAddressChanged)
				]
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Left)
				.FillWidth(0.5f)
				[
					SNew(STextBlock)
					.Text(LOCTEXT("FTIPortNumber", "Port Number"))
				]
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Fill)
				.FillWidth(0.5f)
				[
					SAssignNew(PortNumberEditabledText, SEditableTextBox)
					.Text(FText::FromString(FString::FromInt(Endpoint.Port)))
					.OnTextCommitted(this, &SFTILiveLinkSourceFactory::OnPortNumberChanged)
				]
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Left)
				.FillWidth(0.5f)
				[
					SNew(STextBlock)
					.Text(LOCTEXT("FTISubjectName", "Subject Name"))
				]
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Fill)
				.FillWidth(0.5f)
				[
					SAssignNew(SubjectNameEditabledText, SEditableTextBox)
					.Text(FText::FromName(SubjectName))
					.OnTextCommitted(this, &SFTILiveLinkSourceFactory::OnSubjectNameChanged)
				]
			]
			+ SVerticalBox::Slot()
			.HAlign(HAlign_Right)
			.AutoHeight()
			[
				SNew(SButton)
				.OnClicked(this, &SFTILiveLinkSourceFactory::OnOkClicked)
				[
					SNew(STextBlock)
					.Text(LOCTEXT("Ok", "Ok"))
				]
			]
		]
	];
}

void SFTILiveLinkSourceFactory::OnIpAddressChanged(const FText& NewValue, ETextCommit::Type type)
{
	TSharedPtr<SEditableTextBox> IpAddressEditabledTextPin = IpAddressEditabledText.Pin();
	TSharedPtr<SEditableTextBox> PortNumberEditabledTextPin = PortNumberEditabledText.Pin();
	if (IpAddressEditabledTextPin.IsValid() && PortNumberEditabledTextPin.IsValid())
	{
		FIPv4Endpoint Endpoint;
		FString endPointString = FString::Printf(TEXT("%s:%s"), 
				*NewValue.ToString(), *PortNumberEditabledTextPin->GetText().ToString());
		if (!FIPv4Endpoint::Parse(endPointString, Endpoint) && !FIPv4Endpoint::FromHostAndPort(endPointString, Endpoint))
		{
			Endpoint.Address = FIPv4Address::InternalLoopback;
			Endpoint.Port = DEFAULT_PORT_NUMBER;
			IpAddressEditabledTextPin->SetText(Endpoint.Address.ToText());
			PortNumberEditabledTextPin->SetText(FText::FromString(FString::FromInt(Endpoint.Port)));
		}

		if (type == ETextCommit::OnEnter)
		{
			OnOkClicked();
		}
	}
}

void SFTILiveLinkSourceFactory::OnPortNumberChanged(const FText& NewValue, ETextCommit::Type type)
{
	TSharedPtr<SEditableTextBox> IpAddressEditabledTextPin = IpAddressEditabledText.Pin();
	TSharedPtr<SEditableTextBox> PortNumberEditabledTextPin = PortNumberEditabledText.Pin();
	if (IpAddressEditabledTextPin.IsValid() && PortNumberEditabledTextPin.IsValid())
	{
		FIPv4Endpoint Endpoint;
		FString endPointString = FString::Printf(TEXT("%s:%s"), 
				*IpAddressEditabledTextPin->GetText().ToString(), *NewValue.ToString());
		if (!FIPv4Endpoint::Parse(endPointString, Endpoint) && !FIPv4Endpoint::FromHostAndPort(endPointString, Endpoint))
		{
			Endpoint.Address = FIPv4Address::InternalLoopback;
			Endpoint.Port = DEFAULT_PORT_NUMBER;
			IpAddressEditabledTextPin->SetText(Endpoint.Address.ToText());
			PortNumberEditabledTextPin->SetText(FText::FromString(FString::FromInt(Endpoint.Port)));
		}

		if (type == ETextCommit::OnEnter)
		{
			OnOkClicked();
		}
	}
}

void SFTILiveLinkSourceFactory::OnSubjectNameChanged(const FText& NewValue, ETextCommit::Type type)
{
	TSharedPtr<SEditableTextBox> SubjectNameEditabledTextPin = SubjectNameEditabledText.Pin();
	if (SubjectNameEditabledTextPin.IsValid())
	{
		FName SubjectName = FName(*NewValue.ToString());
		if (SubjectName.IsNone())
		{
			SubjectName = DEFAULT_SUBJECT_NAME;
			SubjectNameEditabledTextPin->SetText(FText::FromName(SubjectName));
		}
		
		if (type == ETextCommit::OnEnter)
		{
			OnOkClicked();
		}
	}
}

FReply SFTILiveLinkSourceFactory::OnOkClicked()
{
	UE_LOG(FTILiveLink, Verbose, TEXT("call SFTILiveLinkSourceFactory::OnOkClicked"));

	TSharedPtr<SEditableTextBox> IpAddressEditabledTextPin = IpAddressEditabledText.Pin();
	TSharedPtr<SEditableTextBox> PortNumberEditabledTextPin = PortNumberEditabledText.Pin();
	TSharedPtr<SEditableTextBox> SubjectNameEditabledTextPin = SubjectNameEditabledText.Pin();

	if (IpAddressEditabledTextPin.IsValid() && PortNumberEditabledTextPin.IsValid()
			&& SubjectNameEditabledTextPin.IsValid())
	{
		FIPv4Endpoint Endpoint;
		FString endPointString = FString::Printf(TEXT("%s:%s"), 
				*IpAddressEditabledTextPin->GetText().ToString(), 
				*PortNumberEditabledTextPin->GetText().ToString());
		
		UE_LOG(FTILiveLink, Verbose, TEXT("SFTILiveLinkSourceFactory::OnOkClicked endPointString: %s"), *endPointString);

		// Support both Host string and ip address
		if (FIPv4Endpoint::FromHostAndPort(endPointString, Endpoint) || FIPv4Endpoint::Parse(endPointString, Endpoint))
		{
			FName SubjectName = FName(*SubjectNameEditabledTextPin->GetText().ToString());
			if (!SubjectName.IsNone())
			{
				UE_LOG(FTILiveLink, Verbose, TEXT("SFTILiveLinkSourceFactory::OnOkClicked subjectName: %s"), *SubjectName.ToString());

				OkClicked.ExecuteIfBound(Endpoint, SubjectName);
			}
		}
	}
	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE