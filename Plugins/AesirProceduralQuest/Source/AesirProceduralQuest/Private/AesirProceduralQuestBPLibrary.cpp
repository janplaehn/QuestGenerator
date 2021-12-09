// Copyright 2022 Jan Plähn. All Rights Reserved.

#include "AesirProceduralQuestBPLibrary.h"
#include "AesirProceduralQuest.h"
#include "EditorUtilityWidget.h"
#include "Engine.h"
#include "LevelEditor.h"
#include "WidgetBlueprint.h"
#include "AssetRegistry/AssetRegistryModule.h"

UAesirProceduralQuestBPLibrary::UAesirProceduralQuestBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

TArray<UObject*> UAesirProceduralQuestBPLibrary::LoadObjectLibrary(TSubclassOf<UObject> ObjectClass)
{
	TArray<UObject*> Assets;

	FAssetRegistryModule& AssetRegistryModule = FModuleManager::Get().LoadModuleChecked<FAssetRegistryModule>(AssetRegistryConstants::ModuleName);
	TArray<FAssetData> AssetDatas;
	AssetRegistryModule.Get().GetAssetsByClass(ObjectClass->GetFName(), AssetDatas);

	for (int32 i = 0; i < AssetDatas.Num(); ++i)
	{
		FAssetData& AssetData = AssetDatas[i];
		UObject* Asset = AssetData.GetAsset();
		if (Asset)
		{
			Assets.Add(AssetData.GetAsset());
			UE_LOG(LogTemp, Verbose, TEXT("%s"), *(Asset->GetName()));
		}
	}
	
	return Assets;
}

