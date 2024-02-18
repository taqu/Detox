#include "DetoxTestSuiteActor.h"
#include <Kismet/KismetSystemLibrary.h>
#include "Detox.h"
#include "DetoxTestFixtureActor.h"
#include "DetoxTestActor.h"
#include "DetoxReporterJUnit.h"

ADetoxTestSuiteActor::ADetoxTestSuiteActor(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, Fixture(nullptr)
	, RunInPIE(true)
	, ReportInEditor(true)
	, InGauntlet(false)
	, Current(-1)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bTickEvenWhenPaused = true;
	ReporterClasses.Add(UDetoxReporterJUnit::StaticClass());
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

void ADetoxTestSuiteActor::SetInGauntlet()
{
	InGauntlet = true;
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
		if(!InGauntlet && ReportInEditor){
			Report();
		}
		NotifyAfterAll();
	}
}

void ADetoxTestSuiteActor::Report()
{
	FString ReportPath = FPaths::AutomationReportsDir();
	if (ReportPath.IsEmpty()) {
		return;
	}
	TArray<TObjectPtr<UDetoxReporterInterface>> Reporters;
	for (TObjectPtr<UClass>& ReporterClass : ReporterClasses) {
		UDetoxReporterInterface* Reporter = NewObject<UDetoxReporterInterface>((UObject*)GetTransientPackage(), ReporterClass);
		if(nullptr == Reporter){
			continue;
		}
		Reporters.Add(Reporter);
	}

	FDateTime Now = FDateTime::Now();
	FString Name = UKismetSystemLibrary::GetDisplayName(this);
	FString FileName = TEXT("report_") + Name + TEXT("_") + Now.ToIso8601() + TEXT(".xml");
	ReportPath = FPaths::Combine(ReportPath, FileName);
	ReportPath = FPaths::ConvertRelativePathToFull(ReportPath);
	TArray<FDetoxTestSuiteResult> Results = {Result};
	for (const TObjectPtr<UDetoxReporterInterface>& Reporter : Reporters) {
		Reporter->Write(Results, ReportPath);
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
