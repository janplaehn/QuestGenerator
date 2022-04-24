// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Quest.h"
#include "Components/ActorComponent.h"
#include "QuestActionDatabaseComponent.generated.h"


class UQuestAction;
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), BlueprintType, Blueprintable)
class AESIRPROCEDURALQUEST_API UQuestActionDatabaseComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UQuestActionDatabaseComponent();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

#if WITH_EDITORONLY_DATA
	virtual EDataValidationResult IsDataValid(TArray<FText>& ValidationErrors) override;
#endif
		
	UQuestAction* GetRandomAction();

	//Todo: Call this when applicable!
	void CleanActionInstances();

protected:
	UPROPERTY(EditAnywhere)
	int32 MaxNumInstancesToHold = 400;
	
	/**
	* DataTable that holds all possible quest actions
	* */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = ( RowType="QuestActionRow" ))
	UDataTable* QuestActionDataTable;
	
	void InitPossibleQuestActions();

	UPROPERTY(Transient)
	TArray<UQuestAction*> Archetypes;

	UPROPERTY(Transient)
	TMap<uint64,UQuestAction*> ActionInstances;
};
