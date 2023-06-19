// Copyright (c) 2022 Faceware Technologies, Inc. All rights reserved.

#include "FTILiveLinkSourceSettings.h"

#define LOCTEXT_NAMESPACE "FTILiveLinkSourceSettings"

#if WITH_EDITOR
bool UFTILiveLinkSourceSettings::CanEditChange(const FProperty* InProperty) const
{
	const bool defaultVal = Super::CanEditChange(InProperty);

	if (InProperty->GetFName() == GET_MEMBER_NAME_CHECKED(UFTILiveLinkSourceSettings, ConnectionString))
	{
		return false;
	}
	return defaultVal;
}
#endif

#undef LOCTEXT_NAMESPACE