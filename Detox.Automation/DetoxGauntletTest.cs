using Gauntlet;

namespace Detox.Automation
{
	public class DetoxGauntletTest : UnrealTestNode<DetoxTestConfig>
	{
		public DetoxGauntletTest(UnrealTestContext InContext) : base(InContext)
		{
		}

		public override DetoxTestConfig GetConfiguration()
        {
            DetoxTestConfig Config = base.GetConfiguration();

            // Start a single instance of the game.
            UnrealTestRole ClientRole = Config.RequireRole(UnrealTargetRole.Client);
            Config.MaxDuration = 2 * 60 * 60; // set timeout in seconds
            ClientRole.Controllers.Add("DetoxGauntletTestController");

            // Ignore user account management.
            Config.NoMCP = true;

            return Config;
        }
	}
}
