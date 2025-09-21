// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GOCLEAN : ModuleRules
{
	public GOCLEAN(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });


		// Personal Folder
		PublicIncludePaths.AddRange(new string[] {	"Public/GObjectSystem/Server",
													"Public/GObjectSystem/Client"});
	}
}
