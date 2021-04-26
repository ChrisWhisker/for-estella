// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Gardening : ModuleRules
{
	public Gardening(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "UMG" });
		MinFilesUsingPrecompiledHeaderOverride = 1;
		
		// Added lines below
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        // PrivatePCHHeaderFile = "MaulProtoPrecompiled.h";
        MinFilesUsingPrecompiledHeaderOverride = 1;
        bUseUnity = false;
	}
}
