# Introduction
Detox is forked from [Daedalic Test Automation Plugin](https://github.com/DaedalicEntertainment/ue4-test-automation), and rewrite enormously.
This facilitates creating and running integration tests with the [Gauntlet Automation Framework](https://docs.unrealengine.com/en-US/Programming/Automation/Gauntlet/index.html) of [Unreal Engine](https://www.unrealengine.com).


Gauntlet
```
"C:\Projects\UnrealEngine\Engine\Build\BatchFiles\RunUAT.bat"
RunUnreal
-project="C:\Projects\UnrealGame\UnrealGame.uproject"
-scriptdir="C:\Projects\UnrealGame"
-platform=Win64
-configuration=Development
-build=editor
-test="DaedalicTestAutomationPlugin.Automation.DaeGauntletTest"
```

# License
This is released under the MIT License.
