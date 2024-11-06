#pragma once
/**
 */
#include <CoreMinimal.h>
#include <GameFramework/Actor.h>
#include "DetoxTestSuiteResult.h"
#include "DetoxTestSuiteActor.generated.h"

class ADetoxTestFixtureActor;
class UDetoxReporterInterface;
class ADetoxTestActor;

UCLASS()
class DETOX_API ADetoxTestSuiteActor: public AActor
{
	GENERATED_BODY()
public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFinish, ADetoxTestSuiteActor*, TestSuite);

	ADetoxTestSuiteActor(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	virtual void NotifyBeforeAll();
	virtual void NotifyAfterAll();
	virtual void NotifyBeforeEach();
	virtual void NotifyAfterEach();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "BeforeAll"))
	void EventBeforeAll();
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "AfterAll"))
	void EventAfterAll();
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "BeforeEach"))
	void EventBeforeEach();
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "AfterEach"))
	void EventAfterEach();

	void SetInGauntlet();
	void RunAll();

	bool IsRunning() const;

	const FDetoxTestSuiteResult& GetResult() const;

	FOnFinish OnFinished;
private:
	ADetoxTestActor* GetCurrent();
	UObject* GetCurrentParameter(const ADetoxTestActor* Target);
	void RunNext();
	void Report();

	UFUNCTION()
	void OnTestSkipped(ADetoxTestActor* Test, UObject* Parameter, const FString& SkipReason);
	UFUNCTION()
	void OnTestFinished(ADetoxTestActor* Test, UObject* Parameter);

	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<UClass>> ReporterClasses; //!< Test reporters. Default is single JUnit reporter.

	UPROPERTY(EditInstanceOnly)
	TObjectPtr<ADetoxTestFixtureActor> Fixture;

	UPROPERTY(EditInstanceOnly)
	TArray<TObjectPtr<ADetoxTestActor>> Tests;

	UPROPERTY(EditAnywhere)
	bool RunInPIE;

	UPROPERTY(EditAnywhere)
	bool ReportInEditor;

	bool InGauntlet;
	int32 Current;
	FDetoxTestSuiteResult Result;
};
