#pragma once
#include <CoreMinimal.h>
#include <Misc/AutomationTest.h>
#include "DetoxEditorAutomationTestContext.h"

DEFINE_LATENT_AUTOMATION_COMMAND_ONE_PARAMETER(FDetoxJoinTestSuite, FDetoxEditorAutomationTestContext&, Context);

DEFINE_LATENT_AUTOMATION_COMMAND_ONE_PARAMETER(FDetoxSetConsoleVariables, FDetoxEditorAutomationTestContext&, Context);

DEFINE_LATENT_AUTOMATION_COMMAND_ONE_PARAMETER(FDetoxRevertConsoleVariables, FDetoxEditorAutomationTestContext&, Context);
