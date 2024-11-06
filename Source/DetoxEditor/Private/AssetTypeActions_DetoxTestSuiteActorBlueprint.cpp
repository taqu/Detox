#include "AssetTypeActions_DetoxTestSuiteActorBlueprint.h"
#include "Detox\Public\DetoxTestSuiteActorBlueprint.h"
#include "DetoxTestSuiteActorBlueprintFactory.h"
#define LOCTEXT_NAMESPACE "Detox"

FAssetTypeActions_DetoxTestSuiteActorBlueprint::FAssetTypeActions_DetoxTestSuiteActorBlueprint(EAssetTypeCategories::Type InAssetTypeCategory)
    : AssetTypeCategory(InAssetTypeCategory)
{
}

FText FAssetTypeActions_DetoxTestSuiteActorBlueprint::GetName() const
{
    return NSLOCTEXT("Detox", "AssetTypeActions_DetoxTestActorBlueprint", "Test Actor Blueprint");
}

UClass* FAssetTypeActions_DetoxTestSuiteActorBlueprint::GetSupportedClass() const
{
    return UDetoxTestSuiteActorBlueprint::StaticClass();
}

uint32 FAssetTypeActions_DetoxTestSuiteActorBlueprint::GetCategories()
{
    return AssetTypeCategory;
}

UFactory* FAssetTypeActions_DetoxTestSuiteActorBlueprint::GetFactoryForBlueprintType(
    UBlueprint* InBlueprint) const
{
    return NewObject<UDetoxTestSuiteActorBlueprintFactory>();
}

#undef LOCTEXT_NAMESPACE
