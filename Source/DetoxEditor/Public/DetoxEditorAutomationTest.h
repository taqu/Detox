#pragma once
#include <CoreMinimal.h>
#include <Misc/AutomationTest.h>

#include "DetoxEditorAutomationTestContext.h"

class DETOXEDITOR_API FDetoxEditorAutomationTest: FAutomationTestBase
{
public:
	FDetoxEditorAutomationTest(const FString& MapName);

	virtual uint32 GetTestFlags() const override;
	virtual uint32 GetRequiredDeviceNum() const override;
	virtual FString GetTestSourceFileName() const override;
	virtual int32 GetTestSourceFileLine() const override;

protected:
	virtual void GetTests(TArray<FString>& OutBeautifiedNames, TArray<FString>& OutTestCommands) const override;
	virtual bool RunTest(const FString& Parameters) override;
	virtual FString GetBeautifiedTestName() const override;

private:
	FDetoxEditorAutomationTestContext Context;
};

