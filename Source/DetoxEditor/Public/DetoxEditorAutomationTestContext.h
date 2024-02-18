#pragma once
#include <CoreMinimal.h>
#include <Templates/Tuple.h>

class ADetoxTestSuiteActor;

class FDetoxEditorAutomationTestContext
{
public:
    ADetoxTestSuiteActor* TestSuite = nullptr;
    TArray<TTuple<FString, FString>> LastConsoleVariables;
};
