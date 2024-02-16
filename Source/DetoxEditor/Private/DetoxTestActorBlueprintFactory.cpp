#include "DetoxTestActorBlueprintFactory.h"

#include <Kismet2/KismetEditorUtilities.h>
#include <Kismet2/BlueprintEditorUtils.h>
#include <BlueprintEditorSettings.h>
#include <K2Node_Event.h>
#include <K2Node_CallFunction.h>

#include "DetoxTestActor.h"
#include "DetoxTestActorBlueprint.h"

//#include <BlueprintEditorSettings.h>
//#include <EdGraphSchema_K2.h>
//#include <EdGraph/EdGraph.h>
//#include <Engine/Blueprint.h>
//#include <Engine/BlueprintGeneratedClass.h>
//#include <Kismet2/BlueprintEditorUtils.h>
//#include <Kismet2/KismetEditorUtilities.h>

UDetoxTestActorBlueprintFactory::UDetoxTestActorBlueprintFactory(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    bCreateNew = true;
    bEditAfterNew = true;
    SupportedClass = UDetoxTestActorBlueprint::StaticClass();
}

UObject* UDetoxTestActorBlueprintFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
    // Create blueprint asset
    UBlueprint* Blueprint = FKismetEditorUtilities::CreateBlueprint(
        ADetoxTestActor::StaticClass(),
        InParent,
		Name,
		BPTYPE_Normal,
		UDetoxTestActorBlueprint::StaticClass(),
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
        FKismetEditorUtilities::AddDefaultEventNode(Blueprint, EdGraph, FName(TEXT("EventPremise")), ADetoxTestActor::StaticClass(), NodePositionY);
        FKismetEditorUtilities::AddDefaultEventNode(Blueprint, EdGraph, FName(TEXT("EventSetup")), ADetoxTestActor::StaticClass(), NodePositionY);
        /*UK2Node_Event* EventNodeRun = */FKismetEditorUtilities::AddDefaultEventNode(Blueprint, EdGraph, FName(TEXT("EventRun")), ADetoxTestActor::StaticClass(), NodePositionY);
        FKismetEditorUtilities::AddDefaultEventNode(Blueprint, EdGraph, FName(TEXT("EventTeardown")), ADetoxTestActor::StaticClass(), NodePositionY);
        //TODO: Add call function node for ADetoxTestActor::Funish and link it to EventRun
        //EdGraph->AddNode(, false, false);
    }
    return Blueprint;
}

