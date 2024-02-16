#pragma once
/**
*/
#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

#if (UE_BUILD_SHIPPING || UE_BUILD_TEST)
DECLARE_LOG_CATEGORY_EXTERN(LogDetox, Log, All);
#else
DECLARE_LOG_CATEGORY_EXTERN(LogDetox, Verbose, All);
#endif

class FDetoxModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

