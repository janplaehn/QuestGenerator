// Copyright Epic Games, Inc. All Rights Reserved.

#include "AesirProceduralQuestEditor.h"

#include "AesirProceduralQuestBPLibrary.h"
#include "AesirProceduralQuestEditorStyle.h"
#include "AesirProceduralQuestEditorCommands.h"
#include "EditorUtilityWidget.h"
#include "EditorUtilitySubsystem.h"
#include "EditorUtilityWidgetBlueprint.h"
#include "LevelEditor.h"
#include "QuestLabel.h"
#include "QuestLabelCollectionDetails.h"
#include "QuestProviderComponent.h"
#include "QuestProviderPreferences.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"
#include "WidgetBlueprint.h"

static const FName AesirProceduralQuestEditorTabName("Quests");

#define LOCTEXT_NAMESPACE "FAesirProceduralQuestEditorModule"

void FAesirProceduralQuestEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FAesirProceduralQuestEditorStyle::Initialize();
	FAesirProceduralQuestEditorStyle::ReloadTextures();

	FAesirProceduralQuestEditorCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FAesirProceduralQuestEditorCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FAesirProceduralQuestEditorModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FAesirProceduralQuestEditorModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(AesirProceduralQuestEditorTabName, FOnSpawnTab::CreateRaw(this, &FAesirProceduralQuestEditorModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FAesirProceduralQuestEditorTabTitle", "Quest Editor"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);

	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.RegisterCustomPropertyTypeLayout(FName(FQuestLabelCollection::StaticStruct()->GetName()), FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FQuestLabelCollectionDetails::MakeInstance));
}

void FAesirProceduralQuestEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FAesirProceduralQuestEditorStyle::Shutdown();

	FAesirProceduralQuestEditorCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(AesirProceduralQuestEditorTabName);
}

TSharedRef<SDockTab> FAesirProceduralQuestEditorModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	const FSoftObjectPath WidgetPath("WidgetBlueprint'/AesirProceduralQuestEditor/WBP_ProceduralQuestEditor.WBP_ProceduralQuestEditor'");
	UObject* WidgetObject = WidgetPath.TryLoad();
	UWidgetBlueprint* WidgetBlueprint = Cast<UWidgetBlueprint>(WidgetObject);

	if (WidgetBlueprint->GeneratedClass->IsChildOf(UEditorUtilityWidget::StaticClass()))
	{
		UEditorUtilityWidget* Widget = NewObject<UEditorUtilityWidget>(GetTransientPackage(), WidgetBlueprint->GeneratedClass);
		
		return SNew(SDockTab)
				.TabRole(ETabRole::NomadTab)
				[
					Widget->TakeWidget()
				];
	}

	return SNew(SDockTab).TabRole(ETabRole::NomadTab);
}

void FAesirProceduralQuestEditorModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(AesirProceduralQuestEditorTabName);
}

void FAesirProceduralQuestEditorModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FAesirProceduralQuestEditorCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FAesirProceduralQuestEditorCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FAesirProceduralQuestEditorModule, AesirProceduralQuestEditor)