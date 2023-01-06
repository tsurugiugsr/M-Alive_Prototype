// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once
#include <md5.h>
#include <hmac_sha1.h>
#include "Modules/ModuleManager.h"

class FSpeech2TextModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
