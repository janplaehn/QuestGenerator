// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "AesirProceduralQuestEditorStyle.h"

class FAesirProceduralQuestEditorCommands : public TCommands<FAesirProceduralQuestEditorCommands>
{
public:

	FAesirProceduralQuestEditorCommands()
		: TCommands<FAesirProceduralQuestEditorCommands>(TEXT("AesirProceduralQuestEditor"), NSLOCTEXT("Contexts", "AesirProceduralQuestEditor", "AesirProceduralQuestEditor Plugin"), NAME_None, FAesirProceduralQuestEditorStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};