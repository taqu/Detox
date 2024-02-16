#pragma once
#include <CoreMinimal.h>
#include <Factories/Factory.h>
#include "DetoxTestSuiteActorBlueprintFactory.generated.h"

UCLASS(HideCategories = Object, MinimalAPI)
class UDetoxTestSuiteActorBlueprintFactory: public UFactory
{
	GENERATED_BODY()

public:
	UDetoxTestSuiteActorBlueprintFactory(const FObjectInitializer& ObjectInitializer);

	// UFactory Implementation
	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
};
