// Copyright 2018 Jianzhao Fu. All Rights Reserved.
using System.IO;
using UnrealBuildTool;

public class DirectExcel : ModuleRules
{
	public DirectExcel(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		PublicIncludePaths.Add(ModuleDirectory);
		PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "xlnt"));
		PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "utfcpp"));

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine",
				"Projects",
				"SlateCore",
				// ... add other public dependencies that you statically link with here ...
			}
			);
		
		 PrivateDependencyModuleNames.AddRange(
		    new string[]
		    {
			    "CoreUObject",
		    });

        //bEnableExceptions = true;

	}
}
