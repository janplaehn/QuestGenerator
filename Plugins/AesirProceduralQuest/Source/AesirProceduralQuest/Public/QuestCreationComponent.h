// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestProviderPreferences.h"
#include "Components/ActorComponent.h"
#include "QuestCreationComponent.generated.h"

class UQuest;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AESIRPROCEDURALQUEST_API UQuestCreationComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UQuestCreationComponent();

	UQuest* CreateQuest(UQuestProviderPreferences* Preferences);

protected:
	/**
	* DataTable that holds all possible quest actions
	* */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = ( RowType="QuestActionRow" ))
	UDataTable* QuestActionDataTable;

	UPROPERTY(EditAnywhere)
	int QuestActionCount = 5;

	TSoftObjectPtr<UQuestAction> GetRandomQuestAction() const;
};
