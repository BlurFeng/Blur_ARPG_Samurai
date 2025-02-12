// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class BlurARPGFramework : ModuleRules
{
	public BlurARPGFramework(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				//~ Begin Engine.
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"GameplayTags",
				"GameplayTasks",
				"AIModule",
				"AnimGraphRuntime",
				"UMG",
				"NavigationSystem",
				"MoviePlayer",
				//~ End Engine.
				//~ Begin Plugins.
				"EnhancedInput",
				"GameplayAbilities",
				"Niagara",
				"MotionWarping",
				//~ End Plugins.
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
