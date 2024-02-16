#pragma once
#include <CoreMinimal.h>
#include <Misc/DateTime.h>

/**
 * @brief Type of one result
*/
enum class DETOX_API DetoxTestResultType
{
    Unknown = 0,
    Succeeded,
    Failed,
    Skipped,
};

/**
 * @brief Result of one assertion
*/
struct DETOX_API FDetoxTestAssertion
{
public:
    DetoxTestResultType ResultType; //!< Result of the test

    FString Message; //!< Additional message

    FDateTime DateTime; //!< Time the assertion is called.
};

/**
 * @brief Result of single test
*/
class DETOX_API FDetoxTestResult
{
public:
    FString TestName; //!< Name of the test

    FDateTime StartTime; //!< Time the test start
    FDateTime EndTime; //!< Time the test end

    TArray<FDetoxTestAssertion> Assertions; //!< Results of assertion
};
