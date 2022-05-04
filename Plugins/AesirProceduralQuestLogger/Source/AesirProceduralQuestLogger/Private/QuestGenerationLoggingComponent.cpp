// Fill out your copyright notice in the Description page of Project Settings.

#include "QuestGenerationLoggingComponent.h"
#include "DirectExcelLibrary.h"

UQuestGenerationLoggingComponent::UQuestGenerationLoggingComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UQuestGenerationLoggingComponent::InjectDependencies(UQuestCreationComponent* InQuestCreator, UBulkQuestCreator* InBulkQuestCreator)
{
	InQuestCreator->OnQuestStarted.AddDynamic(this, &UQuestGenerationLoggingComponent::HandleOnQuestStarted);
	InQuestCreator->OnIterationUpdated.AddDynamic(this, &UQuestGenerationLoggingComponent::HandleOnIterationUpdated);
	InQuestCreator->OnQuestCompleted.AddDynamic(this, &UQuestGenerationLoggingComponent::HandleOnQuestCompleted);

	if (IsValid(InBulkQuestCreator))
	{
		InBulkQuestCreator->OnPropertyChange.AddDynamic(this, &UQuestGenerationLoggingComponent::HandleOnQuestPropertyChange);
	}
}

void UQuestGenerationLoggingComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Workbook->SaveAs(SavePath.Path + "/" + WorkbookTitle + ".xlsx", ExcelFileRelateiveDir::Absolute);
	Workbook->ConditionalBeginDestroy();
	Workbook = nullptr;

	Super::EndPlay(EndPlayReason);
}

void UQuestGenerationLoggingComponent::SetRow(const uint32 NewRow)
{
	CurrentRow = NewRow;
}

void UQuestGenerationLoggingComponent::HandleOnQuestStarted(FQuestGenerationSnapshot Snapshot)
{
	LastLogTime = FPlatformTime::Seconds();
	TimeSinceStart = 0;
	GenerationIndex++;
	CurrentColumn = 0;

	if (!IsValid(Workbook))
	{
		//Todo: Load an existing excel
		Workbook = UDirectExcelLibrary::CreateExcel();
		Workbook->SetTitle(WorkbookTitle);
	}

	//Todo: Change between sheets
	const UExcelWorksheet* Sheet = Workbook->SheetAt(0);

	if (CurrentRow == 0)
	{
		UExcelCell* TitleCell = Sheet->CellAt(1, 1);
		const FString TitleFormula = FString::Printf(TEXT("=ROUND(%f,2)"), 0.0f);
		TitleCell->SetFormula(TitleFormula);
	}

	UExcelCell* ValueCell = Sheet->CellAt(1, 2 + CurrentRow);
	const FString ValueFormula = FString::Printf(TEXT("=ROUND(%f,4)"), 0.0f);
	ValueCell->SetFormula(ValueFormula);
}

void UQuestGenerationLoggingComponent::HandleOnIterationUpdated(FQuestGenerationSnapshot Snapshot)
{
	const float LogInterval = GetLogInterval();
	const double PlatformTime = FPlatformTime::Seconds();
	if (PlatformTime - LastLogTime < LogInterval)
	{
		return;
	}
	LastLogTime += LogInterval;
	TimeSinceStart += LogInterval;
	CurrentColumn++;
	const UExcelWorksheet* Sheet = Workbook->SheetAt(0);

	if (CurrentRow == 0)
	{
		UExcelCell* TitleCell = Sheet->CellAt(CurrentColumn+1, 1);
		const FString TitleFormula = FString::Printf(TEXT("=ROUND(%f,3)"), TimeSinceStart);
		TitleCell->SetFormula(TitleFormula);
	}

	UExcelCell* ValueCell = Sheet->CellAt(CurrentColumn+1, 2 + CurrentRow);
	const float Fitness = FMath::Max(0.0f, Snapshot.GlobalMaximumFitness);
	
	const float NewValue = ValueCell->HasValue() ? (ValueCell->ToFloat() / GenerationIndex) * (GenerationIndex - 1) + (Fitness / GenerationIndex) : Fitness;
	ValueCell->SetFloat(NewValue);
}

void UQuestGenerationLoggingComponent::HandleOnQuestCompleted(FQuestGenerationSnapshot Snapshot)
{

}

void UQuestGenerationLoggingComponent::HandleOnQuestPropertyChange()
{
	// const UExcelWorksheet* Sheet = Workbook->SheetAt(0);
	//
	// //Final cleanup: Round the numbers
	// for (int32 ColumnIndex = 1; ColumnIndex < Sheet->ColumnCount(); ColumnIndex++)
	// {
	// 	UExcelCell* Cell = Sheet->CellAt(ColumnIndex, 2 + CurrentRow);
	// 	if (!Cell->HasValue())
	// 	{
	// 		continue;
	// 	}
	// 	
	// 	const FString RoundedFormula = FString::Printf(TEXT("=ROUND(%f,6)"), Cell->ToFloat());
	// 	Cell->SetFormula(RoundedFormula);
	// }
	
	CurrentRow++;
	GenerationIndex = 0;
}

float UQuestGenerationLoggingComponent::GetLogInterval() const
{
	if (TimeSinceStart < 0.1f)
	{
		return LogIntervalBelowZeroPointOne;
	}
	if (TimeSinceStart < 1.0f)
	{
		return LogIntervalBelowOne;
	}
	return LogIntervalAboveOne;
}

