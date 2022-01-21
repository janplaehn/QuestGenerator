// Copyright 2022 Jan Plähn. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "QuestProviderComponent.h"
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

	UFUNCTION(BlueprintCallable)
	void Initialize();
	
	void RequestQuestGeneration(UQuestProviderComponent* QuestProviderComponent);
	
	void PauseQuestGeneration(UQuestProviderComponent* QuestProviderComponent);

protected:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	/**
	* DataTable that holds all possible quest actions
	* */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = ( RowType="QuestActionRow" ))
	UDataTable* QuestActionDataTable;

	UQuest* CreateRandomQuest();

	UPROPERTY(EditAnywhere)
	FInt32Range QuestActionCountRange = FInt32Range(5,10);

	UPROPERTY(EditAnywhere)
	int GenerationIterationsPerFrame = 5.0f;

	UPROPERTY(EditAnywhere)
	int MaxQuestSampleCount = 30;

	UPROPERTY(EditAnywhere)
	bool bEnableConditionMatching = true;

	bool TryApplyNextQuestAction(UQuest* Quest, TMap<uint32, bool>& SimulatedConditionResolutions) const;

	UQuestAction* GetRandomQuestAction(UObject* Outer = nullptr) const;

	void InitPossibleQuestActions();

	UPROPERTY(Transient)
	TArray<UQuestAction*> CachedPossibleQuestActions;

	UPROPERTY(Transient)
	TSet<UQuestProviderComponent*> QuestRequesters;
};
