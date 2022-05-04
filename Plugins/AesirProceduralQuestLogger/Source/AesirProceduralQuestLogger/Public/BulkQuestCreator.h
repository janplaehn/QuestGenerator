// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestCreationComponent.h"
#include "Components/ActorComponent.h"
#include "BulkQuestCreator.generated.h"

USTRUCT()
struct AESIRPROCEDURALQUESTLOGGER_API FGenerationProperties
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	float MinLocalIterations = 20.0f;
	
	UPROPERTY(EditAnywhere)
	float AbandonBias = 0.05f;

	UPROPERTY(EditAnywhere)
	int32 QuestActionCount = 8;

	UPROPERTY(EditAnywhere)
	int32 MaxQuestSampleCount = 100;

	FGenerationProperties InterpolateTo(const FGenerationProperties& Other, const float Alpha)
	{
		FGenerationProperties Interpolation;
		Interpolation.MinLocalIterations = FMath::Lerp(MinLocalIterations, Other.MinLocalIterations, Alpha);
		Interpolation.AbandonBias = FMath::Lerp(AbandonBias, Other.AbandonBias, Alpha);
		Interpolation.QuestActionCount = FMath::RoundToInt(FMath::Lerp((float)QuestActionCount, (float)Other.QuestActionCount, Alpha));
		Interpolation.MaxQuestSampleCount = FMath::RoundToInt(FMath::Lerp((float)MaxQuestSampleCount, (float)Other.MaxQuestSampleCount, Alpha));
		return Interpolation;
	}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPropertyChange);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable, BlueprintType)
class AESIRPROCEDURALQUESTLOGGER_API UBulkQuestCreator : public UActorComponent
{
	GENERATED_BODY()

public:
	UBulkQuestCreator();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void InjectDependencies(UQuestCreationComponent* InQuestCreator);

	FOnPropertyChange OnPropertyChange;

protected:
	UPROPERTY(EditAnywhere)
	TArray<UQuestProviderPreferences*> AllPreferences;

	void ApplyGenerationProperties(const FGenerationProperties& NewProperties);
	
	UPROPERTY(EditAnywhere)
	float GenerationTime = 3.0f;

	UPROPERTY(EditAnywhere)
	int32 GenerationSteps = 10;

	UPROPERTY(EditAnywhere)
	int32 IterationsPerSetting = 10;
	
	UPROPERTY(EditAnywhere)
	FGenerationProperties StartGenerationProperties;

	UPROPERTY(EditAnywhere)
	FGenerationProperties EndGenerationProperties;

	UFUNCTION()
	void HandleOnTimerFinished();

private:
	TWeakObjectPtr<UQuestCreationComponent> QuestCreator;

	FGuid GenerationId;

	int32 CurrentIteration = 0;

	int32 CurrentPreferenceIndex = 0;

	int32 CurrentGenerationStep = 0;

	FTimerHandle TimerHandle;
};
