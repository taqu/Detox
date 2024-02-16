#include "DetoxTestSuiteActor.h"
#include "Detox.h"
#include "DetoxTestFixtureActor.h"
#include "DetoxTestActor.h"

ADetoxTestSuiteActor::ADetoxTestSuiteActor(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, Fixture(nullptr)
	, RunInPIE(true)
	, Current(-1)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bTickEvenWhenPaused = true;
}

void ADetoxTestSuiteActor::BeginPlay()
{
	Super::BeginPlay();
	Current = -1;
	Result.MapName = GetWorld()->GetMapName();
	Result.TestSuiteName = GetName();
	Result.StartTime = FDateTime::Now();
	Result.Results.Reset();
}

void ADetoxTestSuiteActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if(!IsRunning()) {
		if(RunInPIE && GetWorld()->IsPlayInEditor() && Current < 0) {
			RunAll();
		}
		return;
	}
	ADetoxTestActor* Test = GetCurrent();
	if(IsValid(Test)) {
		UObject* Parameter = GetCurrentParameter(Test);
		Test->Run(Parameter);
	} else {
		Result.EndTime = FDateTime::Now();
		OnFinished.Broadcast(this);
	}
}

void ADetoxTestSuiteActor::NotifyBeforeAll()
{
	EventBeforeAll();
}

void ADetoxTestSuiteActor::NotifyAfterAll()
{
	EventAfterAll();
}

void ADetoxTestSuiteActor::NotifyBeforeEach()
{
	EventBeforeEach();
}

void ADetoxTestSuiteActor::NotifyAfterEach()
{
	EventAfterEach();
}

void ADetoxTestSuiteActor::RunAll()
{
	UE_LOG(LogDetox, Display, TEXT("Detox Test Suite %s - Run all tests."), *GetName());
	NotifyBeforeAll();
	Current = -1;
	RunNext();
}

bool ADetoxTestSuiteActor::IsRunning() const
{
	return Tests.IsValidIndex(Current);
}

const FDetoxTestSuiteResult& ADetoxTestSuiteActor::GetResult() const
{
	return Result;
}

void ADetoxTestSuiteActor::GetReporters(TArray<TObjectPtr<UDetoxReporterInterface>>& Reporters) const
{
	for(const TObjectPtr<ADetoxTestActor>& Test: Tests) {
		const TArray<TObjectPtr<UDetoxReporterInterface>>& TestReporters = Test->GetReporters();
		for(const TObjectPtr<UDetoxReporterInterface>& Reporter: TestReporters) {
			Reporters.Add(Reporter);
		}
	}
}

ADetoxTestActor* ADetoxTestSuiteActor::GetCurrent()
{
	return Tests.IsValidIndex(Current) ? Tests[Current] : nullptr;
}

UObject* ADetoxTestSuiteActor::GetCurrentParameter(const ADetoxTestActor* Target)
{
	check(nullptr != Target);
	return nullptr != Fixture ? Fixture->GetParameter(Target) : nullptr;
}

void ADetoxTestSuiteActor::RunNext()
{
	ADetoxTestActor* Test = GetCurrent();
	if(IsValid(Test)) {
		Test->OnSkipped.RemoveDynamic(this, &ADetoxTestSuiteActor::OnTestSkipped);
		Test->OnFinished.RemoveDynamic(this, &ADetoxTestSuiteActor::OnTestFinished);
	}

	++Current;
	Test = GetCurrent();
	if(IsValid(Test)) {
		Test->OnSkipped.AddDynamic(this, &ADetoxTestSuiteActor::OnTestSkipped);
		Test->OnFinished.AddDynamic(this, &ADetoxTestSuiteActor::OnTestFinished);
		// Start new test
		UE_LOG(LogDetox, Display, TEXT("Detox Test Suite %s - Run a test %s."), *GetName(), *Test->GetName());
		NotifyBeforeEach();
		UObject* Parameter = GetCurrentParameter(Test);
		Test->Run(Parameter);
	} else {
		// All tests finished
		UE_LOG(LogDetox, Display, TEXT("Detox Test Suite %s - All tests finished."), *GetName());
		NotifyAfterAll();
	}
}

void ADetoxTestSuiteActor::OnTestSkipped(ADetoxTestActor* Test, UObject* Parameter, const FString& SkipReason)
{
	if(IsValid(Test)) {
		UE_LOG(LogDetox, Display, TEXT("Detox Test Suite %s - A test %s is skipped."), *GetName(), *Test->GetName());
	}
	NotifyAfterEach();
	RunNext();
}

void ADetoxTestSuiteActor::OnTestFinished(ADetoxTestActor* Test, UObject* Parameter)
{
	if(IsValid(Test)) {
		UE_LOG(LogDetox, Display, TEXT("Detox Test Suite %s - A test %s finished."), *GetName(), *Test->GetName());
	}
	NotifyAfterEach();
	RunNext();
}
