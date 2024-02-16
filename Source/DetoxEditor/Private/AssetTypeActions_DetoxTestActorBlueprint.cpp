#include "AssetTypeActions_DetoxTestActorBlueprint.h"
#include "Detox\Public\DetoxTestActorBlueprint.h"
#include "DetoxTestActorBlueprintFactory.h"
#define LOCTEXT_NAMESPACE "Detox"

FAssetTypeActions_DetoxTestActorBlueprint::FAssetTypeActions_DetoxTestActorBlueprint(EAssetTypeCategories::Type InAssetTypeCategory)
    : AssetTypeCategory(InAssetTypeCategory)
{
}

FText FAssetTypeActions_DetoxTestActorBlueprint::GetName() const
{
    return NSLOCTEXT("Detox", "AssetTypeActions_DetoxTestActorBlueprint", "Test Actor Blueprint");
}

UClass* FAssetTypeActions_DetoxTestActorBlueprint::GetSupportedClass() const
{
    return UDetoxTestActorBlueprint::StaticClass();
}

uint32 FAssetTypeActions_DetoxTestActorBlueprint::GetCategories()
{
    return AssetTypeCategory;
}

UFactory* FAssetTypeActions_DetoxTestActorBlueprint::GetFactoryForBlueprintType(
    UBlueprint* InBlueprint) const
{
    return NewObject<UDetoxTestActorBlueprintFactory>();
}

#undef LOCTEXT_NAMESPACE
