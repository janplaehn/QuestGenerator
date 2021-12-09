// Copyright 2022 Jan Plähn. All Rights Reserved.

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
