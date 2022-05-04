// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "BulkQuestCreator.h"
#include "ExcelWorkbook.h"
#include "QuestCreationComponent.h"
#include "Components/ActorComponent.h"
#include "QuestGenerationLoggingComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable, BlueprintType)
class AESIRPROCEDURALQUESTLOGGER_API UQuestGenerationLoggingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UQuestGenerationLoggingComponent();

	UFUNCTION(BlueprintCallable)
	void InjectDependencies(UQuestCreationComponent* InQuestCreator, UBulkQuestCreator* InBulkQuestCreator = nullptr);

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void SetRow(const uint32 NewRow);

protected:
	UPROPERTY(EditAnywhere, Category = "Excel Workbook")
	FString WorkbookTitle;

	UPROPERTY(EditAnywhere, Category = "Excel Workbook")
	FDirectoryPath SavePath;

	UPROPERTY(EditAnywhere, Category = "Logging")
	float LogIntervalBelowZeroPointOne = 0.001f;

	UPROPERTY(EditAnywhere, Category = "Logging")
	float LogIntervalBelowOne = 0.01f;
	
	UPROPERTY(EditAnywhere, Category = "Logging")
	float LogIntervalAboveOne = 0.1f;
	
	UFUNCTION()
	void HandleOnQuestStarted(FQuestGenerationSnapshot Snapshot);

	UFUNCTION()
	void HandleOnIterationUpdated(FQuestGenerationSnapshot Snapshot);

	UFUNCTION()
	void HandleOnQuestCompleted(FQuestGenerationSnapshot Snapshot);

	UFUNCTION()
	void HandleOnQuestPropertyChange();

	float GetLogInterval() const;

private:
	double LastLogTime = 0;

	float TimeSinceStart = 0;

	uint32 CurrentColumn = 0;
	int32 GenerationIndex = 0;

	int32 CurrentRow = 0;

	UPROPERTY(Transient)
	UExcelWorkbook* Workbook;
};
