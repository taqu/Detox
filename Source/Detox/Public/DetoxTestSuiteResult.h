#pragma once
#include "DetoxTestResult.h"

/**
 * @brief Result set of whole rest suite.
*/
class DETOX_API FDetoxTestSuiteResult
{
public:
    FString MapName; //!< Name of the map the test suite ran in.

    FString TestSuiteName; //!< This test suite's name.

    FDateTime StartTime; //!< Time the test suite start
    FDateTime EndTime; //!< Time the test suite end

    TArray<FDetoxTestResult> Results; //!< Results of all test in this suite.
};
