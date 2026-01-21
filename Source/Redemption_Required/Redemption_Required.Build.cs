// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Redemption_Required : ModuleRules
{
	public Redemption_Required(ReadOnlyTargetRules Target) : base(Target)
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
			"Redemption_Required",
			"Redemption_Required/Variant_Horror",
			"Redemption_Required/Variant_Horror/UI",
			"Redemption_Required/Variant_Shooter",
			"Redemption_Required/Variant_Shooter/AI",
			"Redemption_Required/Variant_Shooter/UI",
			"Redemption_Required/Variant_Shooter/Weapons"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
