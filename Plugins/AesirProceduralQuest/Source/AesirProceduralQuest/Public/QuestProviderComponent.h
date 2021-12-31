// Copyright 2022 Jan Plähn. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "QuestData.h"
#include "Quest.h"
#include "Components/ActorComponent.h"
#include "QuestProviderComponent.generated.h"

class UQuestLocation;
class UQuestProviderPreferences;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AESIRPROCEDURALQUEST_API UQuestProviderComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UQuestProviderComponent();

	UFUNCTION(BlueprintCallable)
	bool TryGetQuestData(FQuestData& OutQuestData) const;

	UFUNCTION(BlueprintCallable)
	UQuestProviderPreferences* GetPreferences() const;

	UFUNCTION(BlueprintCallable)
	void SetQuest(UQuest* NewQuest);

	UFUNCTION(BlueprintCallable)
	TSoftObjectPtr<UQuest> GetQuest() const;

protected:
	UFUNCTION(BlueprintCallable)
	bool RequestAsyncQuestGeneration();

	UFUNCTION(BlueprintCallable)
	bool PauseAsyncQuestGeneration();

	TSoftObjectPtr<UQuest> Quest;
	
	UPROPERTY(EditAnywhere)
	FText Name = FText::FromString("GenericQuestProvider");

	UPROPERTY(EditAnywhere)
	UQuestProviderPreferences* Preferences;
};
