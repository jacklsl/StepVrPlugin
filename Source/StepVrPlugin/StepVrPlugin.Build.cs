// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
using System.IO;
using UnrealBuildTool;

public class StepVrPlugin : ModuleRules
{
    private string ModulePath
    {
        get { return ModuleDirectory; }
    }
    private string LibPath
    {
        get { return Path.GetFullPath(Path.Combine(ModulePath, "../../ThirdParty")); }
    }
    public StepVrPlugin(TargetInfo Target)
	{
		//外部链接头文件目录
		PublicIncludePaths.AddRange(
			new string[] {
				"StepVrPlugin/Public",
				// ... add public include paths required here ...
			}
			);
				
		
        //本模块连接头文件路径
		PrivateIncludePaths.AddRange(
			new string[] {
				"StepVrPlugin/Private",
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
                "HeadMountedDisplay",

                //Stepvr server need
                "Sockets",
                "Networking"
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
                "Core",
                "CoreUObject",
                "Engine",
                "InputCore",
                "RHI",
                "RenderCore",
                "Renderer",
                "ShaderCore",
                "InputDevice",
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);

        PrivateIncludePaths.Add(Path.Combine(LibPath, "include"));
        PublicLibraryPaths.Add(Path.Combine(LibPath, "lib"));
        PublicAdditionalLibraries.Add("StepVr.lib");

        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            PublicDelayLoadDLLs.Add("StepVR.dll");
            PublicDelayLoadDLLs.Add("hidapi.dll");
            RuntimeDependencies.Add(new RuntimeDependency(Path.Combine(LibPath, "lib", "StepVR.dll")));
            RuntimeDependencies.Add(new RuntimeDependency(Path.Combine(LibPath, "lib", "hidapi.dll")));
        }
    }
}
