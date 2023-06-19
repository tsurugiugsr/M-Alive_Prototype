// Copyright (c) 2022 Faceware Technologies, Inc. All rights reserved.

using UnrealBuildTool;

public class FTILiveLink : ModuleRules
{
	public FTILiveLink(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"LiveLinkInterface",
				"LiveLinkAnimationCore",
				"Messaging"
			});

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"InputCore",
				"Json",
				"JsonUtilities",
				"Networking",
				"Slate",
				"SlateCore",
				"Sockets",
				"LiveLinkAnimationCore",
				"LiveLinkInterface"
			});
	}
}
