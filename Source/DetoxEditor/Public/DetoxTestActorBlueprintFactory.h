#pragma once
#include <CoreMinimal.h>
#include <Factories/Factory.h>
#include "DetoxTestActorBlueprintFactory.generated.h"

UCLASS(HideCategories = Object, MinimalAPI)
class UDetoxTestActorBlueprintFactory: public UFactory
{
	GENERATED_BODY()

public:
	UDetoxTestActorBlueprintFactory(const FObjectInitializer& ObjectInitializer);

	// UFactory Implementation
	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
};
