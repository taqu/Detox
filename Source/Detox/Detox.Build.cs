
namespace UnrealBuildTool.Rules
{
    public class Detox : ModuleRules
    {
        public Detox(ReadOnlyTargetRules Target) : base(Target)
        {
            PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

            PublicIncludePaths.AddRange(
                new string[] {
                }
                );

            PrivateIncludePaths.AddRange(
                new string[] {
                }
                );

            PublicDependencyModuleNames.AddRange(
                new string[]
                {
                    "Core",
                    "CoreUObject",
                    "Engine",
                    "InputCore",
                    "Gauntlet",
                    "UMG",
                    "SlateCore",
                    "RenderCore",
                    "Projects",
                    "AssetRegistry",
                }
                );

            PrivateDependencyModuleNames.AddRange(
                new string[]
                {
                }
                );

            DynamicallyLoadedModuleNames.AddRange(
                new string[]
                {
                }
                );
        }
    }
}
