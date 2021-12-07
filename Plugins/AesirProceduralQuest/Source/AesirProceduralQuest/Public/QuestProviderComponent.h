// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "QuestData.h"
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

protected:
	UPROPERTY(EditAnywhere)
	FText Name = FText::FromString("GenericQuestProvider");

	UPROPERTY(EditAnywhere)
	UQuestProviderPreferences* Preferences;
};
