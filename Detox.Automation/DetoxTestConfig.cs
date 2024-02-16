using Gauntlet;

namespace Detox.Automation
{
	public class DetoxTestConfig : EpicGame.EpicGameTestConfig
	{
		/// <summary>
		/// Where to write a JUnit XML report to.
		/// </summary>
		[AutoParam]
        public string JUnitReportPath = string.Empty;

        /// <summary>
        /// Where to write test reports to.
        /// </summary>
        [AutoParam]
        public string ReportPath = string.Empty;

        /// <summary>
        /// Which single test to run, instead of all available tests.
        /// </summary>
        [AutoParam]
        public string TestName = string.Empty;

        public override void ApplyToConfig(UnrealAppConfig AppConfig, UnrealSessionRole ConfigRole, IEnumerable<UnrealSessionRole> OtherRoles)
        {
            base.ApplyToConfig(AppConfig, ConfigRole, OtherRoles);

            if (!string.IsNullOrEmpty(JUnitReportPath))
            {
                AppConfig.CommandLine += string.Format(" -JUnitReportPath=\"{0}\"", JUnitReportPath);
            }

            if (!string.IsNullOrEmpty(ReportPath))
            {
                AppConfig.CommandLine += string.Format(" -ReportPath=\"{0}\"", ReportPath);
            }

            if (!string.IsNullOrEmpty(TestName))
            {
                AppConfig.CommandLine += string.Format(" -TestName=\"{0}\"", TestName);
            }
        }
	}
}
