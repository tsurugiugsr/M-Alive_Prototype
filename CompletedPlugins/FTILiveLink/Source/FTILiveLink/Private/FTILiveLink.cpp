// Copyright (c) 2022 Faceware Technologies, Inc. All rights reserved.

#include "FTILiveLink.h"

#define LOCTEXT_NAMESPACE "FFTILiveLinkModule"

void FFTILiveLinkModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FFTILiveLinkModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FFTILiveLinkModule, FTILiveLink)