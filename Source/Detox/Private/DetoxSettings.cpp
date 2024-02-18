#include "DetoxSettings.h"
#include "DetoxReporterJUnit.h"

UDetoxSettings::UDetoxSettings()
{
	ReporterClasses.Add(UDetoxReporterJUnit::StaticClass());
}

#if WITH_EDITOR
void UDetoxSettings::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	if(PropertyChangedEvent.GetPropertyName() == TEXT("TestMapDirectories")
	   || PropertyChangedEvent.GetPropertyName() == TEXT("AdditionalTestMaps")
	   || PropertyChangedEvent.GetPropertyName() == TEXT("IgnoredMaps")) {
		OnTestMapsChanged.Broadcast();
		return;
	}
}
#endif

bool UDetoxSettings::IsTestMap(const FString& FileName, const FName& MapName) const
{
	bool TestMap = false;
	for(const FDirectoryPath& Directory: TestMapDirectories) {
		TestMap |= FileName.Contains(*Directory.Path);
	}
	for(const FSoftObjectPath& Path : AdditionalTestMaps){
		TestMap |= Path.GetAssetFName() == MapName;
	}

	for(const FSoftObjectPath& Path : IgnoredMaps){
		TestMap &= Path.GetAssetFName() != MapName;
	}
	return TestMap;
}

