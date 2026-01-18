// By hzFishy - 2026 - Do whatever you want with it.

using UnrealBuildTool;

public class PointGraphPathfinding : ModuleRules
{
	public PointGraphPathfinding(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicDefinitions.Add("ENABLE_GRAPH_ASTAR_LOGGING=1");
		
		PublicDependencyModuleNames.AddRange(new string[]
			{
				"Core",
				"FishyUtils",
				"SlateCore", "Slate",
			}
		);
		
		PrivateDependencyModuleNames.AddRange(new string[]
			{
				"CoreUObject", "Engine",
			}
		);
	}
}
