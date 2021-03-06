// Copyright 2022 Jan Plähn. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
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
	UQuestProviderPreferences* GetPreferences() const;

	UFUNCTION(BlueprintCallable)
	UQuest* GetQuest() const;

protected:
	UFUNCTION(BlueprintCallable)
	bool RequestAsyncQuestGeneration();

	UFUNCTION(BlueprintCallable)
	bool PauseAsyncQuestGeneration();

	UPROPERTY(Transient)
	TWeakObjectPtr<UQuest> Quest;
	
	UPROPERTY(EditAnywhere)
	FText Name = FText::FromString("GenericQuestProvider");

	UPROPERTY(EditAnywhere)
	UQuestProviderPreferences* Preferences;

	FGuid QuestGenerationId;
};
