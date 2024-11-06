#pragma once
#include <CoreMinimal.h>
#include <UObject/Object.h>
#include <UObject/SoftObjectPath.h>
#include "DetoxSettings.generated.h"

USTRUCT()
struct FDetoxKeyValuePair
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere)
    FString Key;

    UPROPERTY(EditAnywhere)
    FString Value;
};

UCLASS(Config = Game, defaultconfig)
class DETOX_API UDetoxSettings : public UObject
{
    GENERATED_BODY()
public:
    DECLARE_MULTICAST_DELEGATE(FOnTestMapChanged)

    UDetoxSettings();

#if WITH_EDITOR
    virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

    bool IsTestMap(const FString& FileName, const FName& MapName) const;

    UPROPERTY(Config, EditAnywhere, meta=(ContentDir))
    TArray<FDirectoryPath> TestMapDirectories;

    UPROPERTY(Config, EditAnywhere)
    TArray<FSoftObjectPath> AdditionalTestMaps;

    UPROPERTY(Config, EditAnywhere)
    TArray<FSoftObjectPath> IgnoredMaps;

    UPROPERTY(Config, EditAnywhere)
    TArray<FDetoxKeyValuePair> ConsoleVariables;

    UPROPERTY(Config, EditAnywhere)
    TArray<UClass*> ReporterClasses;

    FOnTestMapChanged OnTestMapsChanged;
};
