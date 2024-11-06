#include "DetoxGauntletTestController.h"
#include <GauntletModule.h>
#include <Engine/AssetManager.h>
#include <Kismet/GameplayStatics.h>
#include <Engine/World.h>
#include <EngineUtils.h>
#include <Misc/Paths.h>
#include <GenericPlatform/GenericPlatformProperties.h>
#include <AssetRegistry/AssetData.h>
#include "Misc/CommandLine.h"
#include "GameMapsSettings.h"

#include "Detox.h"
#include "DetoxSettings.h"
#include "DetoxTestSuiteActor.h"
#include "DetoxReporterJUnit.h"

FName UDetoxGauntletTestController::State_GatherTests = TEXT("Gauntlet_GatherTests");
FName UDetoxGauntletTestController::State_LoadNext = TEXT("Gauntlet_LoadNext");
FName UDetoxGauntletTestController::State_Running = TEXT("Gauntlet_Running");
FName UDetoxGauntletTestController::State_Finished = TEXT("Gauntlet_Finished");

void UDetoxGauntletTestController::OnInit()
{
	Super::OnInit();
	MapNames.Reset();
	MapIndex = -1;
	Results.Reset();
	const UDetoxSettings* DetoxSettings = GetDefault<UDetoxSettings>();

	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));
	TArray<FAssetData> AssetData;
    AssetRegistryModule.Get().SearchAllAssets(true);
    AssetRegistryModule.Get().GetAssetsByClass(UWorld::StaticClass()->GetClassPathName(), AssetData);

	for(auto AssetDataItr = AssetData.CreateConstIterator(); AssetDataItr; ++AssetDataItr) {
		const FAssetData& CurrentAssetData = *AssetDataItr;
		FString LongPackageName = CurrentAssetData.GetSoftObjectPath().ToString();
		if(!DetoxSettings->IsTestMap(LongPackageName, CurrentAssetData.AssetName)) {
			continue;
		}
		MapNames.Add(CurrentAssetData.GetSoftObjectPath());
		UE_LOG(LogDetox, Display, TEXT("Add a test %s:%s"), *CurrentAssetData.GetSoftObjectPath().GetAssetName(), *CurrentAssetData.GetSoftObjectPath().GetLongPackageName());
	}
	UE_LOG(LogDetox, Display, TEXT("Find %d tests"), MapNames.Num());

	// Set console variables.
	for(const FDetoxKeyValuePair& Pair: DetoxSettings->ConsoleVariables) {
		IConsoleVariable* ConsoleVariable = IConsoleManager::Get().FindConsoleVariable(*Pair.Key);
		if(nullptr == ConsoleVariable) {
			continue;
		}
		ConsoleVariable->Set(*Pair.Value);
		UE_LOG(LogDetox, Display, TEXT("Set CVar %s = %s"), *Pair.Key, *Pair.Value);
	}

	GetGauntlet()->BroadcastStateChange(FGauntletStates::Initialized);
	SetDefaultLevel();
}

void UDetoxGauntletTestController::SetDefaultLevel()
{
	++MapIndex;

	// Check command line
	FString PassedTestName = ParseCommandLineOption(TEXT("TestName"));

	if(!PassedTestName.IsEmpty()) {
		while(MapNames.IsValidIndex(MapIndex) && MapNames[MapIndex].ToString() != PassedTestName) {
			++MapIndex;
		}
	}

	if(MapNames.IsValidIndex(MapIndex)) {
		UE_LOG(LogDetox, Display, TEXT("Set Default Level %s"), *MapNames[MapIndex].ToString());
		UGameMapsSettings::SetGameDefaultMap(MapNames[MapIndex].ToString());
		GetGauntlet()->BroadcastStateChange(UDetoxGauntletTestController::State_GatherTests);
	}
}

void UDetoxGauntletTestController::OnPreMapChange()
{
	Super::OnPreMapChange();
}

void UDetoxGauntletTestController::OnPostMapChange(UWorld* World)
{
	Super::OnPostMapChange(World);
	if(IsValid(World)){
		UE_LOG(LogDetox, Log, TEXT("Detox Gauntlet - Map changed to %s"), *World->GetName());
	}
	UE_LOG(LogDetox, Log, TEXT("Detox Gauntlet -  %s"), *GetCurrentState().ToString());
	if(GetCurrentState() != UDetoxGauntletTestController::State_LoadNext) {
		return;
	}
	GetGauntlet()->BroadcastStateChange(UDetoxGauntletTestController::State_GatherTests);
}

void UDetoxGauntletTestController::OnTick(float TimeDelta)
{
	Super::OnTick(TimeDelta);
	if(GetCurrentState() == FGauntletStates::Initialized) {
		bool Found = false;
		for(FSoftObjectPath& Path : MapNames){
			if(Path.GetAssetName() == GetCurrentMap()){
				Found = true;
				break;
			}
		}
		if(!Found){
			MapIndex = -1;
			LoadNext();
			return;
		} else {
			GetGauntlet()->BroadcastStateChange(UDetoxGauntletTestController::State_GatherTests);
		}
	} else if(GetCurrentState() == UDetoxGauntletTestController::State_LoadNext) {
		UE_LOG(LogDetox, Log, TEXT("Detox Gauntlet - Load map: %s"), *MapNames[MapIndex].ToString());
		UGameplayStatics::OpenLevel(this, MapNames[MapIndex].GetAssetFName());
	} else if(GetCurrentState() == UDetoxGauntletTestController::State_GatherTests) {
		// Find next test suite
		ADetoxTestSuiteActor* TestSuite = nullptr;
		for(TActorIterator<ADetoxTestSuiteActor> ActorIt(GetWorld()); ActorIt; ++ActorIt) {
			TestSuite = *ActorIt;
		}

		if(!IsValid(TestSuite)) {
			LoadNext();
			return;
		}

		// Start the first
		GetGauntlet()->BroadcastStateChange(UDetoxGauntletTestController::State_Running);

		TestSuite->OnFinished.AddDynamic(this, &UDetoxGauntletTestController::OnTestSuiteFinished);
		TestSuite->SetInGauntlet();
		TestSuite->RunAll();
	}
}

void UDetoxGauntletTestController::LoadNext()
{
	UE_LOG(LogDetox, Display, TEXT("Detox Gauntlet - LoadNext"));
	++MapIndex;

	// Check command line
	FString PassedTestName = ParseCommandLineOption(TEXT("TestName"));

	if(!PassedTestName.IsEmpty()) {
		while(MapNames.IsValidIndex(MapIndex) && MapNames[MapIndex].ToString() != PassedTestName) {
			++MapIndex;
		}
	}

	if(MapNames.IsValidIndex(MapIndex)) {
		// Load next test map in next tick. This is to avoid invocation list changes during OnPostMapChange.
		GetGauntlet()->BroadcastStateChange(UDetoxGauntletTestController::State_LoadNext);
	} else {
		// All tests finished. Finish Gauntlet.
		GetGauntlet()->BroadcastStateChange(UDetoxGauntletTestController::State_Finished);
		// Update test reports on disk
		Report();
		// Check test results, return 1 when finding failed result.
		for(const FDetoxTestSuiteResult& SuiteResult: Results) {
			for(const FDetoxTestResult& Result: SuiteResult.Results) {
				for(const FDetoxTestAssertion& Assertion: Result.Assertions) {
					if(DetoxTestResultType::Failed == Assertion.ResultType) {
						EndTest(1);
						return;
					}
				}
			}
		}
		EndTest(0);
	}
}

void UDetoxGauntletTestController::OnTestSuiteFinished(ADetoxTestSuiteActor* TestSuite)
{
	UE_LOG(LogDetox, Display, TEXT("Detox Gauntlet - OnTestSuiteFinished"));
	Results.Add(TestSuite->GetResult());
	LoadNext();
}

FString UDetoxGauntletTestController::ParseCommandLineOption(const TCHAR* Key)
{
	FString Value;
	FParse::Value(FCommandLine::Get(), Key, Value);
	return Value.Mid(1);
}

void UDetoxGauntletTestController::Report()
{
	UE_LOG(LogDetox, Display, TEXT("Detox Gauntlet - Report"));
	const UDetoxSettings* DetoxSettings = GetDefault<UDetoxSettings>();
	if(nullptr == DetoxSettings){
		return;
	}
	FString ReportPath = ParseCommandLineOption(TEXT("ReportPath"));
	if (ReportPath.IsEmpty()) {
		ReportPath = FPaths::AutomationReportsDir();
		FString PlatformName(FPlatformProperties::PlatformName());
		PlatformName.ToLowerInline();

		FDateTime Now = FDateTime::Now();
		FString FileName = TEXT("report_") + PlatformName + TEXT("_") + Now.ToFormattedString(TEXT("%Y%m%d_%H%M%S")) + TEXT(".xml");

		ReportPath = FPaths::Combine(ReportPath, FileName);
	}
	const TArray<UClass*>& ReporterClasses = DetoxSettings->ReporterClasses;
	for (UClass* ReporterClass : ReporterClasses) {
		UDetoxReporterInterface* Reporter = NewObject<UDetoxReporterInterface>((UObject*)GetTransientPackage(), ReporterClass);
		if(nullptr == Reporter){
			continue;
		}
		Reporter->Write(Results, ReportPath);
	}
}

