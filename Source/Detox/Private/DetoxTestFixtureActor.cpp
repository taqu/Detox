#include "DetoxTestFixtureActor.h"

UObject* ADetoxTestFixtureActor::GetParameter_Implementation(const ADetoxTestActor* Target)
{
    return 0<Parameters.Num()? Parameters[0] : nullptr;
}

