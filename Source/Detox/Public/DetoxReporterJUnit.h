#pragma once
#include <CoreMinimal.h>
#include "DetoxReporterInterface.h"
#include "DetoxReporterJUnit.generated.h"

class FDetoxTestSuiteResult;
struct FDetoxTestAssertion;

UCLASS(Blueprintable)
class DETOX_API UDetoxReporterJUnit : public UDetoxReporterInterface
{
    GENERATED_BODY()
public:
    static int32 CountTotalTests(const FDetoxTestSuiteResult& Result);
    static int32 CountSkippedTests(const FDetoxTestSuiteResult& Result);
    static int32 CountFailedTests(const FDetoxTestSuiteResult& Result);
    static double GetTotalTimeSeconds(const FDetoxTestSuiteResult& Result);
    static FString GetTimeStamp(const FDetoxTestSuiteResult& Result);
    static double GetTotalTimeSeconds(const FDetoxTestResult& Result);

	virtual void Write(const TArray<FDetoxTestSuiteResult>& Results, const FString& ReportPath) const override;
};

