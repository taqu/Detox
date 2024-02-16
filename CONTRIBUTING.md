# Contributing

You'd like to help make Daedalic Test Automation Plugin even more awesome? Seems like today's our lucky day! In order to maintain stability of the plugin and its code base, please adhere to the following steps, and we'll be pleased to include your additions in our next release.

Note that Daedalic Test Automation Plugin is distributed under the [MIT License](https://github.com/DaedalicEntertainment/ue4-test-automation/blob/develop/LICENSE). So will be your code.

## How to contribute

### Step 1: Choose what to do

If you've got no idea how to help, head over to our [issue tracker](https://github.com/DaedalicEntertainment/ue4-test-automation/issues) and see what you'd like to do most. You can basically pick anything you want to, as long as it's not already assigned to anyone.

If you know exactly what you're missing, [open a new issue](https://github.com/DaedalicEntertainment/ue4-test-automation/issues/new) to begin a short discussion about your idea and how it fits the project. If we all agree, you're good to go!

### Step 2: Fork the project and check out the code

Daedalic Test Automation Plugin is developed using the [GitFlow branching model](https://nvie.com/posts/a-successful-git-branching-model). In order to contribute, you should check out the latest `develop` branch, and create a new feature or hotfix branch to be merged back.

### Step 3: Implement your feature or bugfix

Clearly, everybody's got their own approach here. However, we'd still like you to keep a few things in mind, to ensure the stability and consistency of the plugin for everyone:

* We're using our own [Coding Conventions](https://github.com/DaedalicEntertainment/unreal-coding-conventions), which are largely based on the official [Coding Standard](https://docs.unrealengine.com/latest/INT/Programming/Development/CodingStandard/index.html) provided by Epic Games. If you're used to working with that one, in general, you should be fine.
* When you're adding support for a newer engine version, make sure that you don't break support for previously supported engine versions. We must not force our users to upgrade their engine just because of updating the plugin.
* Note that renaming or reordering parameters is a breaking change. This includes the blueprint macro libraries shipped with the plugin. You should avoid that, if possible.

### Step 4: Open a pull request

Finally, [open a pull request](https://help.github.com/en/github/collaborating-with-issues-and-pull-requests/creating-a-pull-request) so we can review your changes together, and finally integrate it into the next release.


## Release Checklist

Internally, we're using the following checklist when preparing for a new release:

* Check pending pull requests
* Create release branch
* Add examples for new features where appropriate
* Run all tests
* Update documentation (README, images, code samples, spelling, table of contents)
* Increase version number (and engine version, if necessary)
* Create plugin package
* Check plugin package in another project
* Merge release branch with tag
* Add a new GitHub release with release notes
* Update GitHub issues and milestones
* Notify community (e.g. forums)
