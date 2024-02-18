/**
 */
#include "DetoxEditor.h"
#include <AssetToolsModule.h>
#include <ISettingsModule.h>
#include <FileHelpers.h>
#include <Misc/PackageName.h>
#include "AssetTypeActions_DetoxTestActorBlueprint.h"
#include "AssetTypeActions_DetoxTestSuiteActorBlueprint.h"
#include "DetoxSettings.h"
#include "DetoxEditorAutomationTest.h"

#define LOCTEXT_NAMESPACE "Detox"

DEFINE_LOG_CATEGORY(LogDetoxEditor);

void FDetoxEditorModule::StartupModule()
{
	// Register asset type
	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();

	DetoxAssetCategory = AssetTools.RegisterAdvancedAssetCategory(
		FName(TEXT("Detox")),
		NSLOCTEXT("Detox", "DetoxAssetCategory", "Test Automation"));

	TSharedRef<IAssetTypeActions> TestSuiteActorBlueprintAction = MakeShared<FAssetTypeActions_DetoxTestSuiteActorBlueprint>(DetoxAssetCategory);
	RegisterAssetTypeAction(AssetTools, TestSuiteActorBlueprintAction);

	TSharedRef<IAssetTypeActions> TestActorBlueprintAction = MakeShared<FAssetTypeActions_DetoxTestActorBlueprint>(DetoxAssetCategory);
	RegisterAssetTypeAction(AssetTools, TestActorBlueprintAction);

	// TODO: Add test fixture
	// TSharedRef<IAssetTypeActions> TestActorBlueprintAction = MakeShared<FAssetTypeActions_DetoxTestActorBlueprint>(DetoxAssetCategory);
	// RegisterAssetTypeAction(AssetTools, TestActorBlueprintAction);

	// Register settings
	ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");
	if(nullptr != SettingsModule) {
		UDetoxSettings* Settings = GetMutableDefault<UDetoxSettings>();
		SettingsModule->RegisterSettings(
			TEXT("Project"),
			TEXT("Plugins"),
			TEXT("Detox"),
			LOCTEXT("DetoxSettingName", "Detox"),
			LOCTEXT("DetoxSettingDescription", "Configure the discovery of automated tests."),
			Settings);
		Settings->OnTestMapsChanged.AddRaw(this, &FDetoxEditorModule::OnTestMapsChanged);
		OnTestMapsChanged();
	}
}

void FDetoxEditorModule::ShutdownModule()
{
	// Unregister settings
	ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");
	if(nullptr != SettingsModule) {
		SettingsModule->UnregisterSettings("Editor", "Plugins", "Detox");
	}
	// Unregister asset types
	if(FModuleManager::Get().IsModuleLoaded("AssetTools")) {
		IAssetTools& AssetToolsModule = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();
		for(TSharedPtr<IAssetTypeActions>& AssetTypeAction: AssetTypeActions) {
			if(AssetTypeAction.IsValid()) {
				AssetToolsModule.UnregisterAssetTypeActions(AssetTypeAction.ToSharedRef());
			}
		}
	}
	AssetTypeActions.Reset();
}

void FDetoxEditorModule::RegisterAssetTypeAction(IAssetTools& AssetTools, TSharedRef<IAssetTypeActions> Action)
{
	AssetTools.RegisterAssetTypeActions(Action);
	AssetTypeActions.Add(Action);
}

void FDetoxEditorModule::OnTestMapsChanged()
{
	const UDetoxSettings* Settings = GetDefault<UDetoxSettings>();

	AutomationTests.Reset();

	// Register new tests
	TArray<FString> PackageFiles;
	FEditorFileUtils::FindAllPackageFiles(PackageFiles);

	// Iterate over all files, adding the ones with the map extension.
	for(const FString& FileName: PackageFiles) {
		bool IsMap = FPaths::GetExtension(FileName, true) == FPackageName::GetMapPackageExtension();
		if(IsMap){
			FName MapName = FName(*FPaths::GetBaseFilename(FileName));
			FString PackageName;
			if(FPackageName::TryConvertFilenameToLongPackageName(FileName, PackageName, nullptr)
				&& Settings->IsTestMap(PackageName, MapName)){
				TSharedPtr<FDetoxEditorAutomationTest> NewTest = MakeShared<FDetoxEditorAutomationTest>(FileName);
				AutomationTests.Add(NewTest);
			}
		}
	}
}

IMPLEMENT_MODULE(FDetoxEditorModule, DetoxEditor)

#undef LOCTEXT_NAMESPACE
