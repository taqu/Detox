#pragma once
/**
 */
#include <CoreMinimal.h>
#include <Engine/Blueprint.h>
#include "DetoxTestSuiteActorBlueprint.generated.h"

UCLASS(BlueprintType)
class DETOX_API UDetoxTestSuiteActorBlueprint: public UBlueprint
{
	GENERATED_BODY()
public:
#if WITH_EDITOR
	virtual bool SupportedByDefaultBlueprintFactory() const override
	{
		return false;
	}
#endif
};

