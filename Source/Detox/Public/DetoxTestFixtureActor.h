#pragma once
#include <CoreMinimal.h>
#include <GameFramework/Actor.h>
#include "DetoxTestFixtureActor.generated.h"

class ADetoxTestActor;

/**
 * @brief Provides data, parameters, and other environments for one test suite.
*/
UCLASS()
class DETOX_API ADetoxTestFixtureActor: public AActor
{
	GENERATED_BODY()

public:
	/**
	 * @brief Get a parameter that is suitable for a target test.
	 * @param Target 
	 * @return 
	*/
	UFUNCTION(BlueprintNativeEvent)
	UObject* GetParameter(const ADetoxTestActor* Target);

	UPROPERTY(EditInstanceOnly)
	TArray<UObject*> Parameters; //!< Parameters for test cases.
};
