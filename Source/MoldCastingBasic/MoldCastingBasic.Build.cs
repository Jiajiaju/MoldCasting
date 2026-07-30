using UnrealBuildTool;

public class MoldCastingBasic : ModuleRules
{
	public MoldCastingBasic(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine",
				"AIModule",
				"CommonInput",
				"CommonUI",
				"DeveloperSettings",
				"EnhancedInput",
				"GameplayTags",
				"InputCore",
				"NavigationSystem",
				"Mover",
				"MotionWarping",
				"UMG",
				"AngelscriptCode"
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Slate",
				"SlateCore"
			}
		);
	}
}
