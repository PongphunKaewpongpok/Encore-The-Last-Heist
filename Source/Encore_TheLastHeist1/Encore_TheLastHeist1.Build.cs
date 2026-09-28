// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Encore_TheLastHeist1 : ModuleRules
{
	public Encore_TheLastHeist1(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"Encore_TheLastHeist1",
			"Encore_TheLastHeist1/Variant_Platforming",
			"Encore_TheLastHeist1/Variant_Platforming/Animation",
			"Encore_TheLastHeist1/Variant_Combat",
			"Encore_TheLastHeist1/Variant_Combat/AI",
			"Encore_TheLastHeist1/Variant_Combat/Animation",
			"Encore_TheLastHeist1/Variant_Combat/Gameplay",
			"Encore_TheLastHeist1/Variant_Combat/Interfaces",
			"Encore_TheLastHeist1/Variant_Combat/UI",
			"Encore_TheLastHeist1/Variant_SideScrolling",
			"Encore_TheLastHeist1/Variant_SideScrolling/AI",
			"Encore_TheLastHeist1/Variant_SideScrolling/Gameplay",
			"Encore_TheLastHeist1/Variant_SideScrolling/Interfaces",
			"Encore_TheLastHeist1/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
