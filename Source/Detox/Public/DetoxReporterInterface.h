#pragma once
#include <CoreMinimal.h>
#include "DetoxTestSuiteResult.h"
#include "DetoxReporterInterface.generated.h"

UCLASS(Blueprintable)
class DETOX_API UDetoxReporterInterface : public UObject
{
    GENERATED_BODY()
public:
	UDetoxReporterInterface(const FObjectInitializer& ObjectInitializer)
		:Super(ObjectInitializer)
	{
	}

	virtual void Write(const TArray<FDetoxTestSuiteResult>& /*Results*/, const FString& /*ReportPath*/) const
	{
	}
};