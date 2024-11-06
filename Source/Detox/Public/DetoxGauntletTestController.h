#pragma once
#include <CoreMinimal.h>
#include <GauntletTestController.h>
#include "DetoxTestSuiteResult.h"
#include "DetoxGauntletTestController.generated.h"

class ADetoxTestSuiteActor;
class UDetoxReporterInterface;

/**
 * @brief Controller launched by Gauntlet
*/
UCLASS()
class DETOX_API UDetoxGauntletTestController : public UGauntletTestController
{
    GENERATED_BODY()

public:
    virtual void OnInit() override;
    void SetDefaultLevel();

    /**
	 *	Called prior to a map change
	 */
	virtual void OnPreMapChange() override;
    virtual void OnPostMapChange(UWorld* World) override;
    virtual void OnTick(float TimeDelta) override;

private:
    static FName State_GatherTests; //!< State to search and gather tests to run
    static FName State_LoadNext; //!< State to load a next test
    static FName State_Running; //!< State running a test
    static FName State_Finished; //!< State to be finished

    /**
     * @brief Load a next test
    */
    void LoadNext();

    UFUNCTION()
    void OnTestSuiteFinished(ADetoxTestSuiteActor* TestSuite);

    FString ParseCommandLineOption(const TCHAR* Key);
    void Report();

    TArray<FSoftObjectPath> MapNames;
    int32 MapIndex;

    TArray<FDetoxTestSuiteResult> Results;
};

