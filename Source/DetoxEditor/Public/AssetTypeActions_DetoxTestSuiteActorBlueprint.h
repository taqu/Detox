#pragma once
#include <CoreMinimal.h>
#include <AssetTypeCategories.h>
#include <AssetTypeActions/AssetTypeActions_Blueprint.h>

class UFactory;

class DETOXEDITOR_API FAssetTypeActions_DetoxTestSuiteActorBlueprint : public FAssetTypeActions_Blueprint
{
public:
    FAssetTypeActions_DetoxTestSuiteActorBlueprint(EAssetTypeCategories::Type AssetTypeCategory);

    // IAssetTypeActions Implementation
    virtual FText GetName() const override;
    virtual UClass* GetSupportedClass() const override;
    virtual uint32 GetCategories() override;

protected:
    virtual UFactory* GetFactoryForBlueprintType(UBlueprint* InBlueprint) const override;

private:
    EAssetTypeCategories::Type AssetTypeCategory;
};
