#pragma once
#include <CoreMinimal.h>
#include <GauntletTestController.h>
#include "DetoxTestSuiteResult.h"
#include "DetoxGauntletTestController.generated.h"

class ADetoxTestSuiteActor;
class UDetoxReporterInterface;

UCLASS()
class DETOX_API UDetoxGauntletTestController : public UGauntletTestController
{
    GENERATED_BODY()

public:
    virtual void OnInit() override;
    virtual void OnPostMapChange(UWorld* World) override;
    virtual void OnTick(float TimeDelta) override;

private:
    static FName State_GatherTests;
    static FName State_LoadNext;
    static FName State_Running;
    static FName State_Finished;

    void LoadNext();

    UFUNCTION()
    void OnTestSuiteFinished(ADetoxTestSuiteActor* TestSuite);

    FString ParseCommandLineOption(const TCHAR* Key);

    TArray<FName> MapNames;
    int32 MapIndex;

    UPROPERTY()
    TArray<TObjectPtr<UDetoxReporterInterface>> Reporters;

    TArray<FDetoxTestSuiteResult> Results;
};

