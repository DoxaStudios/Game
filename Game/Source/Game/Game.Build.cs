// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Game : ModuleRules
{
	public Game(TargetInfo Target)
	{
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "OnlineSubsystem", "OnlineSubsystemUtils" });
       // PublicDependencyModuleNames.AddRange(new string[] { "OnlineSubsystem","OnlineSubsystemUtils"});
        DynamicallyLoadedModuleNames.Add("OnlineSubsystemSteam");
	}
}
