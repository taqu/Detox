# Introduction
Detox is forked from [Daedalic Test Automation Plugin](https://github.com/DaedalicEntertainment/ue4-test-automation), and rewrite enormously.
This facilitates creating and running integration tests with the [Gauntlet Automation Framework](https://docs.unrealengine.com/en-US/Programming/Automation/Gauntlet/index.html) of [Unreal Engine](https://www.unrealengine.com).


# Running Tests

## Gantlet
```bat
"<path to engine directory>\Engine\Build\BatchFiles\RunUAT.bat"^
 RunUnreal^
 -Project="<path to project directory>\Project.uproject"^
 -ScriptDir="<path to engine directory>\Engine\Binaries\DotNET\AutomationTool\AutomationScripts\Detox"^
 -Platform=Win64^
 -Configuration=Development^
 -Build=local^
 -Tests="Detox.Automation.DetoxGauntletTest"^
 -Device=XXX
```

Refer to `RunUAT RunUnreal -help`.

| | |
|:---|:---|
| Project | Target project file |
| ScriptDir | Directory that includes script DLLs |
| Platform | Platforms to perform tests on and their params |
| Configuration | Configuration to perform tests on |
| Build | Reference to the build that is being tested |
| Tests | List of gauntlet tests to run |
| Device | List of devices to use for tests |




# License
This is released under the MIT License.
