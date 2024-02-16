#pragma once
/**
 @author t-sakai
 */
#include <CoreMinimal.h>
#include <Modules/ModuleManager.h>
#include <AssetTypeCategories.h>
#include <IAssetTypeActions.h>

#include "DetoxEditorAutomationTest.h"

class IAssetTools;

#if (UE_BUILD_SHIPPING || UE_BUILD_TEST)
DECLARE_LOG_CATEGORY_EXTERN(LogDetoxEditor, Log, All);
#else
DECLARE_LOG_CATEGORY_EXTERN(LogDetoxEditor, Verbose, All);
#endif

class FDetoxEditorModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	void RegisterAssetTypeAction(IAssetTools& AssetTools, TSharedRef<IAssetTypeActions> Action);
	void OnTestMapsChanged();
	EAssetTypeCategories::Type DetoxAssetCategory;
	TArray<TSharedPtr<IAssetTypeActions>> AssetTypeActions;

	TArray<TSharedPtr<FDetoxEditorAutomationTest>> AutomationTests;
};

