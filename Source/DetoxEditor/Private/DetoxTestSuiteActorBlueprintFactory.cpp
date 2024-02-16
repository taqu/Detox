#include "DetoxTestSuiteActorBlueprintFactory.h"

#include <Kismet2/KismetEditorUtilities.h>
#include <Kismet2/BlueprintEditorUtils.h>
#include <BlueprintEditorSettings.h>
#include <K2Node_Event.h>

#include "DetoxTestSuiteActor.h"
#include "DetoxTestSuiteActorBlueprint.h"

UDetoxTestSuiteActorBlueprintFactory::UDetoxTestSuiteActorBlueprintFactory(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    bCreateNew = true;
    bEditAfterNew = true;
    SupportedClass = UDetoxTestSuiteActorBlueprint::StaticClass();
}

UObject* UDetoxTestSuiteActorBlueprintFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
    // Create blueprint asset
    UBlueprint* Blueprint = FKismetEditorUtilities::CreateBlueprint(
        ADetoxTestSuiteActor::StaticClass(),
        InParent,
		Name,
		BPTYPE_Normal,
		UDetoxTestSuiteActorBlueprint::StaticClass(),
		UBlueprintGeneratedClass::StaticClass());

    if (!IsValid(Blueprint))
    {
        return nullptr;
    }

	// Create new graph
	UEdGraph* EdGraph = FBlueprintEditorUtils::CreateNewGraph(
        Blueprint,
		TEXT("Test Blueprint Graph"),
		UEdGraph::StaticClass(),
		UEdGraphSchema_K2::StaticClass());

	if(0<Blueprint->UbergraphPages.Num())
    {
        FBlueprintEditorUtils::RemoveGraphs(Blueprint, Blueprint->UbergraphPages);
    }

    FBlueprintEditorUtils::AddUbergraphPage(Blueprint, EdGraph);
    Blueprint->LastEditedDocuments.Add(EdGraph);
    EdGraph->bAllowDeletion = false;

    UBlueprintEditorSettings* Settings = GetMutableDefault<UBlueprintEditorSettings>();

    if (nullptr != Settings && Settings->bSpawnDefaultBlueprintNodes)
    {
        int32 NodePositionY = 0;
        FKismetEditorUtilities::AddDefaultEventNode(Blueprint, EdGraph, FName(TEXT("EventBeforeAll")), ADetoxTestSuiteActor::StaticClass(), NodePositionY);
        FKismetEditorUtilities::AddDefaultEventNode(Blueprint, EdGraph, FName(TEXT("EventAfterAll")), ADetoxTestSuiteActor::StaticClass(), NodePositionY);
        FKismetEditorUtilities::AddDefaultEventNode(Blueprint, EdGraph, FName(TEXT("EventBeforeEach")), ADetoxTestSuiteActor::StaticClass(), NodePositionY);
        FKismetEditorUtilities::AddDefaultEventNode(Blueprint, EdGraph, FName(TEXT("EventAfterEach")), ADetoxTestSuiteActor::StaticClass(), NodePositionY);
    }
    return Blueprint;
}

