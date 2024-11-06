#include "DetoxEditorAutomationTestCommands.h"

#include "LevelEditor.h"
#include "Editor.h"
#include <EngineUtils.h>
#include "DetoxEditor.h"
#include "DetoxTestSuiteActor.h"
#include "DetoxSettings.h"

bool FDetoxJoinTestSuite::Update()
{
	if(nullptr == GEditor) {
		return false;
	}

	if(!GEditor->PlayWorld) {
		return false;
	}

	if(nullptr == Context.TestSuite) {
		for(TActorIterator<ADetoxTestSuiteActor> Itr(GEditor->PlayWorld); Itr; ++Itr) {
			Context.TestSuite = *Itr;
			break;
		}
	}

	if(nullptr != Context.TestSuite) {
		return !Context.TestSuite->IsRunning();
	}
	return false;
}

bool FDetoxSetConsoleVariables::Update()
{
	Context.LastConsoleVariables.Reset();

	const UDetoxSettings* Settings = GetDefault<UDetoxSettings>();
	for(const FDetoxKeyValuePair& KeyValuePair : Settings->ConsoleVariables) {
		if(KeyValuePair.Key.IsEmpty() || KeyValuePair.Value.IsEmpty()){
			continue;
		}
		IConsoleVariable* ConsoleVariable = IConsoleManager::Get().FindConsoleVariable(*KeyValuePair.Key);
		if(nullptr == ConsoleVariable){
			continue;
		}
		FString LastValue = ConsoleVariable->GetString();
		ConsoleVariable->Set(*KeyValuePair.Value);
		Context.LastConsoleVariables.Add(MakeTuple(KeyValuePair.Key, KeyValuePair.Value));
		UE_LOG(LogDetoxEditor, Log, TEXT("Set console variable %s = %s"), *KeyValuePair.Key, *KeyValuePair.Value);
	}
	return true;
}

bool FDetoxRevertConsoleVariables::Update()
{
	for(const TTuple<FString, FString>& Tuple: Context.LastConsoleVariables) {
		IConsoleVariable* ConsoleVariable = IConsoleManager::Get().FindConsoleVariable(*Tuple.Get<0>());
		if(nullptr == ConsoleVariable){
			continue;
		}
		ConsoleVariable->Set(*Tuple.Get<1>());
		UE_LOG(LogDetoxEditor, Log, TEXT("Revert console variable %s = %s"), *Tuple.Get<0>(), *Tuple.Get<1>());
	}
	return true;
}
