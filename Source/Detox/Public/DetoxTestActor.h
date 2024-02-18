#pragma once
/**
 */
#include <CoreMinimal.h>
#include <GameFramework/Actor.h>
#include <Misc/DateTime.h>
#include "DetoxTestResult.h"
#include "DetoxTestActor.generated.h"

class UDetoxReporterInterface;

/**
 * @brief Single automated test
*/
UCLASS()
class DETOX_API ADetoxTestActor: public AActor
{
	GENERATED_BODY()
public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSucceeded, ADetoxTestActor*, Test, UObject*, Parameter);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnFailed, ADetoxTestActor*, Test, UObject*, Parameter, const FString&, Message);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnSkipped, ADetoxTestActor*, Test, UObject*, Parameter, const FString&, SkipReason);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnFinished, ADetoxTestActor*, Test, UObject*, Parameter);

	ADetoxTestActor(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

	/**
	 * @brief Tick test
	 * @param Parameter 
	*/
	void Run(UObject* Parameter);

	/**
	 * @brief Finishes execution of this test
	*/
	UFUNCTION(BlueprintCallable)
	void Finish();

	/**
	 * @brief Event when this test has skipped.
	 * @param Parameter 
	 * @param SkipReason 
	*/
	virtual void NotifySkipped(UObject* Parameter, const FString& SkipReason);

	/**
	 * @brief Event to verify preconditions of this test.
	 * @param Parameter 
	*/
	virtual void NotifyPremise(UObject* Parameter);
	/**
	 * @brief Event to setup environment for this test.
	 * @param Parameter 
	*/
	virtual void NotifySetup(UObject* Parameter);
	/**
	 * @brief Event to execute. Finish should be called, when test finishes.
	 * @param Parameter 
	*/
	virtual void NotifyRun(UObject* Parameter);
	/**
	 * @brief Event to clean environment.
	 * @param Parameter 
	*/
	virtual void NotifyTeardown(UObject* Parameter);

	/**
	 * @brief Event to verify preconditions of this test.
	 * @param Parameter 
	*/
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "TestPremise"))
	void EventPremise(UObject* Parameter);

	/**
	 * @brief Event to setup environment for this test.
	 * @param Parameter 
	*/
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "TesSetup"))
	void EventSetup(UObject* Parameter);

	/**
	 * @brief Event to execute. Finish should be called, when test finishes.
	 * @param Parameter 
	*/
	UFUNCTION(BlueprintNativeEvent, meta = (DisplayName = "TesRun"))
	void EventRun(UObject* Parameter);

	/**
	 * @brief Event to clean environment.
	 * @param Parameter 
	*/
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "TesTeardown"))
	void EventTeardown(UObject* Parameter);

	//--- Assertions
	//--------------------------------------------------------
	UFUNCTION(BlueprintCallable, Category = Detox)
	void Assert(bool Expression, const FString& Message);

	UFUNCTION(BlueprintCallable, Category = Detox)
	void AssertFail(const FString& Message);

	UFUNCTION(BlueprintCallable, Category = Detox)
	void AssertTrue(bool Expression, const FString& Message);

	UFUNCTION(BlueprintCallable, Category = Detox)
	void AssertFalse(bool Expression, const FString& Message);

	UFUNCTION(BlueprintCallable, Category = Detox)
	void AssertBool(bool Actual, bool Expected, const FString& Message);

	UFUNCTION(BlueprintCallable, Category = Detox)
	void AssertInt32(int32 Actual, int32 Expected, const FString& Message);

	UFUNCTION(BlueprintCallable, Category = Detox)
	void AssertInt64(int64 Actual, int64 Expected, const FString& Message);

	void AssertUint32(uint32 Actual, uint32 Expected, const FString& Message);

	void AssertUint64(uint64 Actual, uint64 Expected, const FString& Message);

	UFUNCTION(BlueprintCallable, Category = Detox)
	void AssertFloat(float Actual, float Expected, const FString& Message, float Epsilon=1.0e-5f);

	UFUNCTION(BlueprintCallable, Category = Detox)
	void AssertDouble(double Actual, double Expected, const FString& Message, double Epsilon=1.0e-5);

	template<class T>
	void Assert(T Actual, T Expected, const FString& Message)
	{
		DetoxTestResultType Type = (Actual == Expected)? DetoxTestResultType::Succeeded : DetoxTestResultType::Failed;
		Result.Assertions.Add({Type, Message, FDateTime::Now()});
	}

	UFUNCTION(BlueprintCallable, Category = Detox)
	void AssertValid(UObject* Ptr, const FString& Message);

	const FDetoxTestResult& GetResult() const;

	FOnSkipped OnSkipped;
	FOnFinished OnFinished;

protected:
	/**
	 * @brief How long this test is allowed to run in seconds.
	*/
	UPROPERTY(EditAnywhere)
    float TimeoutInSeconds = -1.0f;

private:
	inline static constexpr uint32 Flag_Premised = 0x01U << 0; //!< Whether already premised
	inline static constexpr uint32 Flag_Setupped = 0x01U << 1; //!< Whether already setup finished
	inline static constexpr uint32 Flag_Finished = 0x01U << 2; //!< Whether already finished

	/**
	 * @brief Check flag is on
	 * @param Flag 
	 * @return 
	*/
	inline bool CheckFlag(uint32 Flag) const
	{
		return Flag == (Flag & Flags);
	}

	/**
	 * @brief Set flag on
	 * @param Flag 
	*/
	inline void SetFlag(uint32 Flag)
	{
		Flags |= Flag;
	}

	uint32 Flags; //!< Flags indicate state of this test.

	UPROPERTY(EditAnywhere)
	FString SkipReason; //!< Message when this is skipped.

	FDetoxTestResult Result; //!< Set of assertions what have occurred.
};
