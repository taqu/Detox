#include "DetoxTestPointWanderer.h"

#include <EngineGlobals.h>
#include <RenderCore.h>
#include <UnrealClient.h>
#include <Engine/Engine.h>
#include <Engine/GameViewportClient.h>
#include <Engine/TargetPoint.h>
#include <Engine/World.h>
#include <GameFramework/DefaultPawn.h>
#include <GameFramework/GameModeBase.h>
#include <GameFramework/PlayerController.h>
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetMathLibrary.h>

#include "Detox.h"

#if WITH_ENGINE
// Imported from UnrealClient.cpp.
extern ENGINE_API float GAverageFPS;
#else
float GAverageFPS = 0.0f;
#endif // WITH_ENGINE

ADetoxTestPointWanderer::ADetoxTestPointWanderer(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PawnClass = ADefaultPawn::StaticClass();
}

void ADetoxTestPointWanderer::BeginPlay()
{
	Super::BeginPlay();

	CurrentTargetPointIndex = 0;
}

void ADetoxTestPointWanderer::NotifySetup(UObject* Parameter)
{
	ADetoxTestActor::NotifySetup(Parameter);
	if(FlightPath.Num() <= 0) {
		return;
	}

	// Spawn flying pawn.
	APlayerController* Player = UGameplayStatics::GetPlayerController(this, 0);
	ATargetPoint* StartingPoint = FlightPath[0];

	FRotator SpawnRotation(ForceInit);
	SpawnRotation.Yaw = StartingPoint->GetActorRotation().Yaw;
	FVector SpawnLocation = StartingPoint->GetActorLocation();
	FTransform SpawnTransform = FTransform(SpawnRotation, SpawnLocation);

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Instigator = GetInstigator();
	SpawnInfo.ObjectFlags |= RF_Transient; // We never want to save default player pawns into a map

	APawn* Pawn = GetWorld()->SpawnActor<APawn>(PawnClass, SpawnTransform, SpawnInfo);

	if(!IsValid(Pawn)) {
		UE_LOG(LogDetox, Error, TEXT("%s failed to spawn pawn %s at %s."), *GetName(), *PawnClass->GetName(), *SpawnTransform.ToString());
		return;
	}

	Player->Possess(Pawn);

	UE_LOG(LogDetox, Log, TEXT("%s spawned %s for %s at %s."), *GetName(), *Pawn->GetName(), *Player->GetName(), *StartingPoint->GetName());
}

void ADetoxTestPointWanderer::EventRun_Implementation(UObject* Parameter)
{
	UWorld* World = GetWorld();

	float Time = World->GetTimeSeconds();

	if(Time < InitialDelay) {
		return;
	}

	float DeltaSeconds = World->GetDeltaSeconds();
	APlayerController* Player = UGameplayStatics::GetPlayerController(this, 0);
	APawn* Pawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if(!IsValid(Player) || !IsValid(Pawn)) {
		return;
	}

	if(!FlightPath.IsValidIndex(CurrentTargetPointIndex)) {
		UE_LOG(LogDetox, Log, TEXT("%s has finished."), *GetName());
		Finish();
		return;
	}

	// Follow flight path.
	ATargetPoint* CurrentTargetPoint = FlightPath[CurrentTargetPointIndex];

	FVector ToTargetPoint = CurrentTargetPoint->GetActorLocation() - Pawn->GetActorLocation();
	float Distance = ToTargetPoint.Size();

	if(Distance <= AcceptanceRadius) {
		UE_LOG(LogDetox, Log, TEXT("%s has reached %s."), *GetName(), *CurrentTargetPoint->GetName());
		++CurrentTargetPointIndex;
		return;
	}
	// Move towards next point.
	FVector Direction = ToTargetPoint;
	Direction.Normalize();

	float Scale = FMath::Min(Distance, FlightSpeed * DeltaSeconds);
	Pawn->AddMovementInput(Direction, Scale);

	if(UseTargetRotation && CurrentTargetPointIndex > 0) {
		// Rotate between point a and b rotation angles based on the percentage of path traversed
		const ATargetPoint* PointA = FlightPath[CurrentTargetPointIndex - 1];
		const ATargetPoint* PointB = FlightPath[CurrentTargetPointIndex];

		// Subtract AcceptanceRadius from all size calculations to make rotation a little smoother at the end points
		const float CurrentPathLength = (PointB->GetActorLocation() - PointA->GetActorLocation()).Size() - AcceptanceRadius;
		const float PathCompletionAlpha = FMath::Clamp(1.f - ((Distance - AcceptanceRadius) / CurrentPathLength), 0.f, 1.f);

		const FRotator NewRotation = UKismetMathLibrary::RLerp(
			PointA->GetActorRotation(),
			PointB->GetActorRotation(),
			PathCompletionAlpha,
			true);

		Pawn->SetActorRotation(NewRotation);
	} else if(!UseTargetRotation) {
		// Smoothly rotate in movement direction.
		FRotator CurrentRotation = Pawn->GetActorRotation();
		FRotator TargetRotation = Direction.ToOrientationRotator();

		FRotator NewRotation =
			FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaSeconds, 1.0f);

		Pawn->SetActorRotation(NewRotation);
	}
}
