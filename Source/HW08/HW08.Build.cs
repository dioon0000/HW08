// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class HW08 : ModuleRules
{
	public HW08(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		
		// Public
		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"UMG" });

		// Private
		PrivateDependencyModuleNames.AddRange(new string[] {  });

	}
}
