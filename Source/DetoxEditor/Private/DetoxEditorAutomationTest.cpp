#include "DetoxEditorAutomationTest.h"
#include "DetoxEditor.h"
#include "DetoxEditorAutomationTestCommands.h"
#include <Tests/AutomationEditorCommon.h>

FDetoxEditorAutomationTest::FDetoxEditorAutomationTest(const FString& InMapName)
    : FAutomationTestBase(InMapName, false)
{
}

uint32 FDetoxEditorAutomationTest::GetTestFlags() const
{
    return EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter;
}

uint32 FDetoxEditorAutomationTest::GetRequiredDeviceNum() const
{
    return 1;
}

FString FDetoxEditorAutomationTest::GetTestSourceFileName() const
{
    return GetTestName();
}

int32 FDetoxEditorAutomationTest::GetTestSourceFileLine() const
{
    return 0;
}

void FDetoxEditorAutomationTest::GetTests(
    TArray<FString>& OutBeautifiedNames, TArray<FString>& OutTestCommands) const
{
    OutBeautifiedNames.Add(GetBeautifiedTestName());
    OutTestCommands.Add(FString());
}

bool FDetoxEditorAutomationTest::RunTest(const FString& Parameters)
{
    FString MapName = GetTestName();
    UE_LOG(LogDetoxEditor, Log, TEXT("Run test for a map: %s"), *MapName);

    ADD_LATENT_AUTOMATION_COMMAND(FDetoxSetConsoleVariables(Context));
    ADD_LATENT_AUTOMATION_COMMAND(FEditorLoadMap(MapName));
    ADD_LATENT_AUTOMATION_COMMAND(FStartPIECommand(false));
    ADD_LATENT_AUTOMATION_COMMAND(FDetoxJoinTestSuite(Context));
    ADD_LATENT_AUTOMATION_COMMAND(FEndPlayMapCommand());
    ADD_LATENT_AUTOMATION_COMMAND(FDetoxRevertConsoleVariables(Context));
    return true;
}

FString FDetoxEditorAutomationTest::GetBeautifiedTestName() const
{
    FString MapName = GetTestName();
    return TEXT("Detox.") + FPaths::GetBaseFilename(MapName);
}
