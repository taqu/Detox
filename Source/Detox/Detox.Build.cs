
namespace UnrealBuildTool.Rules
{
    public class Detox : ModuleRules
    {
        public Detox(ReadOnlyTargetRules Target) : base(Target)
        {
            PCHUsage = PCHUsageMode.NoSharedPCHs;
            PrivatePCHHeaderFile = "Private/DetoxPrivatePCH.h";
            CppStandard = CppStandardVersion.Cpp17;

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
                    "EngineSettings",
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
