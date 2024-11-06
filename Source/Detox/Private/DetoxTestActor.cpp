#include "DetoxTestActor.h"

ADetoxTestActor::ADetoxTestActor(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, Flags(0)
{
}

void ADetoxTestActor::BeginPlay()
{
	Flags = 0;
	Result.TestName = GetName();
}

void ADetoxTestActor::Run(UObject* Parameter)
{
	if (!CheckFlag(Flag_Premised)) {
		SetFlag(Flag_Premised);
		Result.Assertions.Reset();
		Result.StartTime = FDateTime::Now();
		NotifyPremise(Parameter);
	}
	if (!SkipReason.IsEmpty()) {
		SetFlag(Flag_Finished);
		Result.EndTime = FDateTime::Now();
		Result.Assertions.Add({ DetoxTestResultType::Skipped, SkipReason, FDateTime::Now() });
		NotifySkipped(Parameter, SkipReason);
		return;
	}

	if (!CheckFlag(Flag_Setupped)) {
		SetFlag(Flag_Setupped);
		NotifySetup(Parameter);
	}
	NotifyRun(Parameter);

	if (!CheckFlag(Flag_Finished)) {
		return;
	}
	NotifyTeardown(Parameter);
	Result.EndTime = FDateTime::Now();

	OnFinished.Broadcast(this, Parameter);
}

void ADetoxTestActor::Finish()
{
	SetFlag(Flag_Finished);
}

void ADetoxTestActor::NotifySkipped(UObject* Parameter, const FString& InSkipReason)
{
	OnSkipped.Broadcast(this, Parameter, InSkipReason);
}

void ADetoxTestActor::NotifyPremise(UObject* Parameter)
{
	EventPremise(Parameter);
}

void ADetoxTestActor::NotifySetup(UObject* Parameter)
{
	EventSetup(Parameter);
}

void ADetoxTestActor::NotifyRun(UObject* Parameter)
{
	EventRun(Parameter);
}

void ADetoxTestActor::NotifyTeardown(UObject* Parameter)
{
	EventTeardown(Parameter);
}

void ADetoxTestActor::Assert(bool Expression, const FString& Message)
{
	DetoxTestResultType Type = (Expression) ? DetoxTestResultType::Succeeded : DetoxTestResultType::Failed;
	Result.Assertions.Add({ Type, Message, FDateTime::Now() });
}

void ADetoxTestActor::AssertFail(const FString& Message)
{
	Result.Assertions.Add({ DetoxTestResultType::Failed, Message, FDateTime::Now() });
}

void ADetoxTestActor::AssertTrue(bool Expression, const FString& Message)
{
	DetoxTestResultType Type = (Expression) ? DetoxTestResultType::Succeeded : DetoxTestResultType::Failed;
	Result.Assertions.Add({ Type, Message, FDateTime::Now() });
}

void ADetoxTestActor::AssertFalse(bool Expression, const FString& Message)
{
	DetoxTestResultType Type = (!Expression) ? DetoxTestResultType::Succeeded : DetoxTestResultType::Failed;
	Result.Assertions.Add({ Type, Message, FDateTime::Now() });
}

void ADetoxTestActor::AssertBool(bool Actual, bool Expected, const FString& Message)
{
	Assert<bool>(Actual, Expected, Message);
}

void ADetoxTestActor::AssertInt32(int32 Actual, int32 Expected, const FString& Message)
{
	Assert<int32>(Actual, Expected, Message);
}

void ADetoxTestActor::AssertInt64(int64 Actual, int64 Expected, const FString& Message)
{
	Assert<int64>(Actual, Expected, Message);
}

void ADetoxTestActor::AssertUint32(uint32 Actual, uint32 Expected, const FString& Message)
{
	Assert<uint32>(Actual, Expected, Message);
}

void ADetoxTestActor::AssertUint64(uint64 Actual, uint64 Expected, const FString& Message)
{
	Assert<uint64>(Actual, Expected, Message);
}

void ADetoxTestActor::AssertFloat(float Actual, float Expected, const FString& Message, float Epsilon)
{
	Assert(FMath::IsNearlyEqual(Actual, Expected, Epsilon), Message);
}

void ADetoxTestActor::AssertDouble(double Actual, double Expected, const FString& Message, double Epsilon)
{
	Assert(FMath::IsNearlyEqual(Actual, Expected, Epsilon), Message);
}

void ADetoxTestActor::AssertValid(UObject* Ptr, const FString& Message)
{
	Assert(IsValid(Ptr), Message);
}

const FDetoxTestResult& ADetoxTestActor::GetResult() const
{
	return Result;
}

void ADetoxTestActor::EventRun_Implementation(UObject* Parameter)
{
	Finish();
}
