#include "DetoxReporterJUnit.h"
#include <Misc/CommandLine.h>
#include <Misc/Parse.h>
#include <Misc/Paths.h>
#include <HAL/PlatformFilemanager.h>

#include "Detox.h"
#include "DetoxTestSuiteResult.h"

int32 UDetoxReporterJUnit::CountTotalTests(const FDetoxTestSuiteResult& Result)
{
	int32 Count = 0;
	for(const FDetoxTestResult& TestResult: Result.Results) {
		Count += TestResult.Assertions.Num();
	}
	return Count;
}

int32 UDetoxReporterJUnit::CountSkippedTests(const FDetoxTestSuiteResult& Result)
{
	int32 Count = 0;
	for(const FDetoxTestResult& TestResult: Result.Results) {
		for(const FDetoxTestAssertion& TestCase: TestResult.Assertions) {
			switch(TestCase.ResultType) {
			case DetoxTestResultType::Skipped:
				++Count;
				break;
			}
		}
	}
	return Count;
}

int32 UDetoxReporterJUnit::CountFailedTests(const FDetoxTestSuiteResult& Result)
{
	int32 Count = 0;
	for(const FDetoxTestResult& TestResult: Result.Results) {
		for(const FDetoxTestAssertion& TestCase: TestResult.Assertions) {
			switch(TestCase.ResultType) {
			case DetoxTestResultType::Failed:
				++Count;
				break;
			}
		}
	}
	return Count;
}

double UDetoxReporterJUnit::GetTotalTimeSeconds(const FDetoxTestSuiteResult& Result)
{
	FTimespan TimeSpan = Result.EndTime - Result.StartTime;
	return TimeSpan.GetTotalSeconds();
}

FString UDetoxReporterJUnit::GetTimeStamp(const FDetoxTestSuiteResult& Result)
{
	return Result.StartTime.ToIso8601();
}

double UDetoxReporterJUnit::GetTotalTimeSeconds(const FDetoxTestResult& Result)
{
	FTimespan TimeSpan = Result.EndTime - Result.StartTime;
	return TimeSpan.GetTotalSeconds();
}

void UDetoxReporterJUnit::Write(const TArray<FDetoxTestSuiteResult>& Results, const FString& ReportPath) const
{
#define JUNIT_XML_LINE_TERMINATOR TEXT("\n")
	// Parse commandline
	FString JUnitReportPath;
	FParse::Value(FCommandLine::Get(), TEXT("JUnitReportPath"), JUnitReportPath);
	JUnitReportPath = JUnitReportPath.Mid(1);

	if(JUnitReportPath.IsEmpty()) {
		if(ReportPath.IsEmpty()) {
			return;
		}

		JUnitReportPath = FPaths::Combine(ReportPath, TEXT("report_junit.xml"));
	}

	// Ensure report path exists.
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	FString ReportDirectory = FPaths::GetPath(JUnitReportPath);
	if(!PlatformFile.DirectoryExists(*ReportDirectory)) {
		PlatformFile.CreateDirectoryTree(*ReportDirectory);
	}

	// Create JUnit XML. Refer to https://github.com/windyroad/JUnit-Schema
	FString XmlString;

	XmlString += TEXT("<?xml version=\"1.0\" encoding=\"UTF-8\"?>") JUNIT_XML_LINE_TERMINATOR;
	XmlString += TEXT("<testsuites>");

	for(const FDetoxTestSuiteResult& TestSuiteResult: Results)
	{
		XmlString += TEXT("<testsuite");
		XmlString += FString::Printf(TEXT(" name=\"%s\""), *TestSuiteResult.TestSuiteName);
		XmlString += FString::Printf(TEXT(" tests=\"%d\""), CountTotalTests(TestSuiteResult));
		XmlString += FString::Printf(TEXT(" skipped=\"%d\""), CountSkippedTests(TestSuiteResult));
		XmlString += FString::Printf(TEXT(" failures=\"%d\""), CountFailedTests(TestSuiteResult));
		XmlString += FString::Printf(TEXT(" errors=\"0\""));
		XmlString += FString::Printf(TEXT(" time=\"%.2lf\""), GetTotalTimeSeconds(TestSuiteResult));
		XmlString += FString::Printf(TEXT(" timestamp=\"%s\""), *GetTimeStamp(TestSuiteResult));
		XmlString += TEXT(">") JUNIT_XML_LINE_TERMINATOR;

		for(const FDetoxTestResult& TestResult: TestSuiteResult.Results) {
			double Time = GetTotalTimeSeconds(TestResult);
			for(const FDetoxTestAssertion& TestCase: TestResult.Assertions) {
				XmlString += TEXT("    <testcase");
				XmlString += FString::Printf(TEXT(" name=\"%s\""), *TestResult.TestName);
				XmlString += FString::Printf(TEXT(" classname=\"%s\""), *TestResult.TestName);
				XmlString += FString::Printf(TEXT(" time=\"%.2lf\""), Time);
				XmlString += TEXT(">") JUNIT_XML_LINE_TERMINATOR;

				switch(TestCase.ResultType) {
				case DetoxTestResultType::Failed:
					XmlString += FString::Format(TEXT("        <failure type=\"Assertion failed\">{0}</failure>"), {*TestCase.Message});
					XmlString += JUNIT_XML_LINE_TERMINATOR;
					break;
				case DetoxTestResultType::Skipped:
					XmlString += FString::Format(TEXT("        <skipped>{0}</skipped>"), {*TestCase.Message});
					XmlString += JUNIT_XML_LINE_TERMINATOR;
					break;
				default:
					break;
				}
				XmlString += TEXT("    </testcase>") JUNIT_XML_LINE_TERMINATOR;
			}
		}

		XmlString += TEXT("</testsuite>") JUNIT_XML_LINE_TERMINATOR;
	}
	XmlString += TEXT("</testsuites>") JUNIT_XML_LINE_TERMINATOR;

	FFileHelper::SaveStringToFile(XmlString, *JUnitReportPath, FFileHelper::EEncodingOptions::ForceUTF8);

	UE_LOG(LogDetox, Verbose, TEXT("Test report:\n%s"), *XmlString);
	UE_LOG(LogDetox, Display, TEXT("Writing test report to: %s"), *JUnitReportPath);
#undef JUNIT_XML_LINE_TERMINATOR
}
