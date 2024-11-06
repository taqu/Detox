#pragma once
/**
 */
#include <CoreMinimal.h>
#include "DetoxTestActor.h"
#include "DetoxTestPointWanderer.generated.h"

class ATargetPoint;

/**
 * @brief Actor that spawns a DefaultPawn to fly along a path through map.
 */
UCLASS()
class DETOX_API ADetoxTestPointWanderer: public ADetoxTestActor
{
	GENERATED_BODY()
public:
	ADetoxTestPointWanderer(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

	virtual void NotifySetup(UObject* Parameter) override;
	virtual void EventRun_Implementation(UObject* Parameter) override;

	/**
	 * @brief Event when reaching a next point.
	 * @param Parameter 
	*/
	virtual void NotifyReachPoint(int32 PointIndex);

	/**
	 * @brief Event when all points visited.
	 * @param Parameter 
	*/
	virtual void NotifyAllPointsVisited();

	/**
	 * @brief Event when reaching a next point.
	 * @param Parameter 
	*/
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "TesReachPoint"))
	void EventReachPoint(int32 PointIndex);

	/**
	 * @brief Event when all points visited.
	 * @param Parameter 
	*/
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "TestAllPointsVisited"))
	void EventAllPointsVisited();
private:
	UPROPERTY(EditInstanceOnly)
	TArray<TObjectPtr<ATargetPoint>> FlightPath; //!< Target points to fly through in order.

	UPROPERTY(EditAnywhere)
	TSubclassOf<APawn> PawnClass; //!< Pawn to use for flying through the level.

	UPROPERTY(EditAnywhere)
	bool UseTargetRotation = false; //!< Interpolates pawn rotation between current and next point's rotations when true.

	UPROPERTY(EditAnywhere)
	float InitialDelay = 0.0f; //!< How long to wait before starting to fly in seconds.

	UPROPERTY(EditAnywhere)
	float FlightSpeed = 100.0f; //!< How fast to fly in cm/sec.

	/** Radius around flight path points to consider the point as reached, in cm. */
	UPROPERTY(EditAnywhere)
	float AcceptanceRadius = 100.0f;

	int32 CurrentTargetPointIndex = 0;
};
