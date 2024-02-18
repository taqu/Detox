// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

namespace UnrealBuildTool.Rules
{
    public class DetoxEditor : ModuleRules
    {
        public DetoxEditor(ReadOnlyTargetRules Target) : base(Target)
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
                    "Detox",
                    "UnrealEd",
                    "BlueprintGraph",
                    "AssetTools",
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
