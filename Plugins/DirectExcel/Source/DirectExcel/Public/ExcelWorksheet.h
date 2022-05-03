// Copyright 2018 Jianzhao Fu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ThirdParty.h"

#include "ExcelCell.h"
#include "ExcelCellReference.h"
#include "ExcelCellRangeReference.h"
#include "ExcelTableHeader.h"
#include "LogTypes.h"

#include "ExcelWorksheet.generated.h"


class UExcelWorksheetDataTable;
/**
* Excel sheet struct
*/
UCLASS(Blueprintable, BlueprintType, Category = "DirectExcel")
class DIRECTEXCEL_API UExcelWorksheet :public UObject
{
	GENERATED_BODY()

public:
	void Initialize(class UExcelWorkbook* workbook, xlnt::worksheet data);

	void SetWorkbook(class UExcelWorkbook* val);

	xlnt::worksheet Data()const { return mData; }
	void SetData(xlnt::worksheet data) { mData = data; }
public:
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Workbook")
		UExcelWorkbook* ParentWorkbook()const;

	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		bool IsValid() { return mData != nullptr; }

	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		FString Title()const;

	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Worksheet", meta = (ToolTip = "Set title, cannot use *:/\\?[]"))
		void SetTitle(FString title);

	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		int32 Id()const;

	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		int32 Index()const;

	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Worksheet")
		FString Print(bool bPrintToScreen = true, FLinearColor TextColor = FLinearColor(0.0, 0.66, 1.0), float Duration = 2.f);

public:	//row
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		int32 RowCount(bool skipNull = true)const;

	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		int32 LowestRow()const;

	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		int32 HighestRow()const;
	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Worksheet")
		void ClearRow(int32 rowIndex = 1);

	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		bool IsValidRow(int32 rowIndex)const;

	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Worksheet")
		FString PrintRow(int32 rowIndex = 1, bool bPrintToScreen = true, FLinearColor TextColor = FLinearColor(0.0, 0.66, 1.0), float Duration = 2.f);
public:	//column
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		int32 ColumnCount(bool skipNull = true)const;

	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		int32 LowestColumn()const;

	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		int32 HighestColumn()const;

	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		bool IsValidColumn(int32 columnIndex)const;

	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Worksheet")
		void ClearColumn(int32 columnIndex);

	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Worksheet")
		void ClearColumnWithString(FString columnString);

	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Worksheet")
		FString PrintColumn(int32 columnIndex, bool bPrintToScreen = true, FLinearColor TextColor = FLinearColor(0.0, 0.66, 1.0), float Duration = 2.f);

	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Worksheet")
		FString PrintColumnWithString(FString columnString, bool bPrintToScreen = true, FLinearColor TextColor = FLinearColor(0.0, 0.66, 1.0), float Duration = 2.f);

public://cells
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		TArray<UExcelCell*> CellsAtRow(int32 rowIndex = 1, ExcelSortType sortType = ExcelSortType::None)const;

	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		TArray<UExcelCell*> CellsAtColumn(int32 columnIndex, ExcelSortType sortType = ExcelSortType::None)const;

	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		TArray<UExcelCell*> CellsAtColumnString(FString columnString, ExcelSortType sortType = ExcelSortType::None)const;

	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		FExcelCellRangeReference CalculateDimension()const;

	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Worksheet")
		void MergeCells(FExcelCellRangeReference rangeReference);
	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Worksheet")
		void UnmergeCells(FExcelCellRangeReference rangeReference);
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		TArray<FExcelCellRangeReference> AllMergedRanges();
public:	//cell
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		bool HasCell(FExcelCellReference cellReference)const;
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		UExcelCell* CellAt(int32 column = 1, int32 row = 1)const;
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		UExcelCell* CellAtRef(FExcelCellReference cellReference)const;
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		UExcelCell* CellAtId(FString cellString)const;
	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Worksheet")
		void ClearCell(FExcelCellReference cellReference);

	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Worksheet")
		void ClearCells(FExcelCellRangeReference rangeReference);

	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		bool HasActiveCell()const;
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		UExcelCell* ActiveCell()const;
public:
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		bool ToBool(FExcelCellReference cellReference)const;
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		int32 ToInt(FExcelCellReference cellReference)const;
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		float ToFloat(FExcelCellReference cellReference)const;
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		FString ToString(FExcelCellReference cellReference)const;
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		FDateTime ToDateTime(FExcelCellReference cellReference)const;
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		FVector ToVector(FExcelCellReference cellReference)const { return CellAtRef(cellReference)->ToVector(); }
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		FRotator ToRotator(FExcelCellReference cellReference)const { return CellAtRef(cellReference)->ToRotator(); }
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		FTransform ToTransform(FExcelCellReference cellReference)const { return CellAtRef(cellReference)->ToTransform(); }
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		FColor ToColor(FExcelCellReference cellReference)const { return CellAtRef(cellReference)->ToColor(); }
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		FMatrix ToMatrix(FExcelCellReference cellReference)const { return CellAtRef(cellReference)->ToMatrix(); }

	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		bool ToBoolAt(int32 column = 1, int32 row = 1)const;
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		int32 ToIntAt(int32 column = 1, int32 row = 1)const;
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		float ToFloatAt(int32 column = 1, int32 row = 1)const;
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		FString ToStringAt(int32 column = 1, int32 row = 1)const;
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		FDateTime ToDateTimeAt(int32 column = 1, int32 row = 1)const;
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		FVector ToVectorAt(int32 column = 1, int32 row = 1)const { return CellAt(column, row)->ToVector(); }
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		FRotator ToRotatorAt(int32 column = 1, int32 row = 1)const { return CellAt(column, row)->ToRotator(); }
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		FTransform ToTransformAt(int32 column = 1, int32 row = 1)const { return CellAt(column, row)->ToTransform(); }
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		FColor ToColorAt(int32 column = 1, int32 row = 1)const { return CellAt(column, row)->ToColor(); }
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		FMatrix ToMatrixAt(int32 column = 1, int32 row = 1)const { return CellAt(column, row)->ToMatrix(); }

	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		bool ToBoolWithId(FString cellReferenceString)const;
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		int32 ToIntWithId(FString cellReferenceString)const;
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		float ToFloatWithId(FString cellReferenceString)const;
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		FString ToStringWithId(FString cellReferenceString)const;
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		FDateTime ToDateTimeWithId(FString cellReferenceString)const;
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		FVector ToVectorWithId(FString cellReferenceString)const { return CellAtId(cellReferenceString)->ToVector(); }
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		FRotator ToRotatorWithId(FString cellReferenceString)const { return CellAtId(cellReferenceString)->ToRotator(); }
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		FTransform ToTransformWithId(FString cellReferenceString)const { return CellAtId(cellReferenceString)->ToTransform(); }
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		FColor ToColorWithId(FString cellReferenceString)const { return CellAtId(cellReferenceString)->ToColor(); }
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		FMatrix ToMatrixWithId(FString cellReferenceString)const { return CellAtId(cellReferenceString)->ToMatrix(); }


	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		TArray<bool> BoolArrayAtRow(int32 rowIndex)const;
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		TArray<int32> IntArrayAtRow(int32 rowIndex)const;
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		TArray<float> FloatArrayAtRow(int32 rowIndex)const;
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		TArray<FString> StringArrayAtRow(int32 rowIndex)const;
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		TArray<FDateTime> DateTimeArrayAtRow(int32 rowIndex)const;
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		TArray<FVector> VectorArrayAtRow(int32 rowIndex)const;
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		TArray<FRotator> RotatorArrayAtRow(int32 rowIndex)const;
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		TArray<FTransform> TransformArrayAtRow(int32 rowIndex)const;
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		TArray<FColor> ColorArrayAtRow(int32 rowIndex)const;
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		TArray<FMatrix> MatrixArrayAtRow(int32 rowIndex)const;

	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		TArray<bool> BoolArrayAtColumn(int32 columnIndex)const;
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		TArray<int32> IntArrayAtColumn(int32 columnIndex)const;
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		TArray<float> FloatArrayAtColumn(int32 columnIndex)const;
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		TArray<FString> StringArrayAtColumn(int32 columnIndex)const;
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		TArray<FDateTime> DateTimeArrayAtColumn(int32 columnIndex)const;
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		TArray<FVector> VectorArrayAtColumn(int32 columnIndex)const;
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		TArray<FRotator> RotatorArrayAtColumn(int32 columnIndex)const;
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		TArray<FTransform> TransformArrayAtColumn(int32 columnIndex)const;
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		TArray<FColor> ColorArrayAtColumn(int32 columnIndex)const;
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		TArray<FMatrix> MatrixArrayAtColumn(int32 columnIndex)const;

	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		TArray<bool> BoolArrayAtColumnString(FString columnString)const;
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		TArray<int32> IntArrayAtColumnString(FString columnString)const;
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		TArray<float> FloatArrayAtColumnString(FString columnString)const;
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		TArray<FString> StringArrayAtColumnString(FString columnString)const;
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		TArray<FDateTime> DateTimeArrayAtColumnString(FString columnString)const;
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		TArray<FVector> VectorArrayAtColumnString(FString columnString)const;
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		TArray<FRotator> RotatorArrayAtColumnString(FString columnString)const;
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		TArray<FTransform> TransformArrayAtColumnString(FString columnString)const;
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		TArray<FColor> ColorArrayAtColumnString(FString columnString)const;
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		TArray<FMatrix> MatrixArrayAtColumnString(FString columnString)const;


	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		TArray<bool> BoolArrayAtRowSorted(int32 rowIndex, TArray<int32>& outColumnIndices, ExcelSortType sortType = ExcelSortType::None)const;
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		TArray<int32> IntArrayAtRowSorted(int32 rowIndex, TArray<int32>& outColumnIndices, ExcelSortType sortType = ExcelSortType::None)const;
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		TArray<float> FloatArrayAtRowSorted(int32 rowIndex, TArray<int32>& outColumnIndices, ExcelSortType sortType = ExcelSortType::None)const;
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		TArray<FString> StringArrayAtRowSorted(int32 rowIndex, TArray<int32>& outColumnIndices, ExcelSortType sortType = ExcelSortType::None)const;
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		TArray<FDateTime> DateTimeArrayAtRowSorted(int32 rowIndex, TArray<int32>& outColumnIndices, ExcelSortType sortType = ExcelSortType::None)const;

	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		TArray<bool> BoolArrayAtColumnSorted(int32 columnIndex, TArray<int32>& outRowIndices, ExcelSortType sortType = ExcelSortType::None)const;
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		TArray<int32> IntArrayAtColumnSorted(int32 columnIndex, TArray<int32>& outRowIndices, ExcelSortType sortType = ExcelSortType::None)const;
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		TArray<float> FloatArrayAtColumnSorted(int32 columnIndex, TArray<int32>& outRowIndices, ExcelSortType sortType = ExcelSortType::None)const;
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		TArray<FString> StringArrayAtColumnSorted(int32 columnIndex, TArray<int32>& outRowIndices, ExcelSortType sortType = ExcelSortType::None)const;
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		TArray<FDateTime> DateTimeArrayAtColumnSorted(int32 columnIndex, TArray<int32>& outRowIndices, ExcelSortType sortType = ExcelSortType::None)const;

	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		TArray<bool> BoolArrayAtColumnStringSorted(FString columnString, TArray<int32>& outRowIndices, ExcelSortType sortType = ExcelSortType::None)const;
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		TArray<int32> IntArrayAtColumnStringSorted(FString columnString, TArray<int32>& outRowIndices, ExcelSortType sortType = ExcelSortType::None)const;
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		TArray<float> FloatArrayAtColumnStringSorted(FString columnString, TArray<int32>& outRowIndices, ExcelSortType sortType = ExcelSortType::None)const;
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		TArray<FString> StringArrayAtColumnStringSorted(FString columnString, TArray<int32>& outRowIndices, ExcelSortType sortType = ExcelSortType::None)const;
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		TArray<FDateTime> DateTimeArrayAtColumnStringSorted(FString columnString, TArray<int32>& outRowIndices, ExcelSortType sortType = ExcelSortType::None)const;
public:
	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Worksheet")
		void SetBool(bool value, FExcelCellReference cellReference);
	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Worksheet")
		void SetInt(int32 value, FExcelCellReference cellReference);
	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Worksheet")
		void SetFloat(float value, FExcelCellReference cellReference);
	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Worksheet")
		void SetString(FString value, FExcelCellReference cellReference);
	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Worksheet")
		void SetDateTime(FDateTime value, FExcelCellReference cellReference);
	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Worksheet")
		void SetVector(FVector value, FExcelCellReference cellReference) { CellAtRef(cellReference)->SetVector(value); }
	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Worksheet")
		void SetRotator(FRotator value, FExcelCellReference cellReference) { CellAtRef(cellReference)->SetRotator(value); }
	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Worksheet")
		void SetTransform(FTransform value, FExcelCellReference cellReference) { CellAtRef(cellReference)->SetTransform(value); }
	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Worksheet")
		void SetColor(FColor value, FExcelCellReference cellReference) { CellAtRef(cellReference)->SetColor(value); }
	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Worksheet")
		void SetMatrix(FMatrix value, FExcelCellReference cellReference) { CellAtRef(cellReference)->SetMatrix(value); }

	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Worksheet")
		void SetBoolAt(bool value, int32 column = 1, int32 row = 1);
	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Worksheet")
		void SetIntAt(int32 value, int32 column = 1, int32 row = 1);
	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Worksheet")
		void SetFloatAt(float value, int32 column = 1, int32 row = 1);
	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Worksheet")
		void SetStringAt(FString value, int32 column = 1, int32 row = 1);
	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Worksheet")
		void SetDateTimeAt(FDateTime value, int32 column = 1, int32 row = 1);
	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Worksheet")
		void SetVectorAt(FVector value, int32 column = 1, int32 row = 1) { CellAt(column,row)->SetVector(value); }
	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Worksheet")
		void SetRotatorAt(FRotator value, int32 column = 1, int32 row = 1) { CellAt(column, row)->SetRotator(value); }
	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Worksheet")
		void SetTransformAt(FTransform value, int32 column = 1, int32 row = 1) { CellAt(column, row)->SetTransform(value); }
	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Worksheet")
		void SetColorAt(FColor value, int32 column = 1, int32 row = 1) { CellAt(column, row)->SetColor(value); }
	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Worksheet")
		void SetMatrixAt(FMatrix value, int32 column = 1, int32 row = 1) { CellAt(column, row)->SetMatrix(value); }


	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Worksheet")
		void SetBoolWithId(bool value, FString cellReferenceString);
	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Worksheet")
		void SetIntWithId(int32 value, FString cellReferenceString);
	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Worksheet")
		void SetFloatWithId(float value, FString cellReferenceString);
	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Worksheet")
		void SetStringWithId(FString value, FString cellReferenceString);
	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Worksheet")
		void SetDateTimeWithId(FDateTime value, FString cellReferenceString);
	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Worksheet")
		void SetVectorWithId(FVector value, FString cellReferenceString) { CellAtId(cellReferenceString)->SetVector(value); }
	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Worksheet")
		void SetRotatorWithId(FRotator value, FString cellReferenceString) { CellAtId(cellReferenceString)->SetRotator(value); }
	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Worksheet")
		void SetTransformWithId(FTransform value, FString cellReferenceString) { CellAtId(cellReferenceString)->SetTransform(value); }
	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Worksheet")
		void SetColorWithId(FColor value, FString cellReferenceString) { CellAtId(cellReferenceString)->SetColor(value); }
	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Worksheet")
		void SetMatrixWithId(FMatrix value, FString cellReferenceString) { CellAtId(cellReferenceString)->SetMatrix(value); }

public://find
	//return row index
	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Worksheet")
		UExcelCell* FindBoolAtColumn(int32 columnIndex, bool searchValue, int32 startRowIndex = 1)const;
	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Worksheet")
		UExcelCell* FindIntAtColumn(int32 columnIndex, int32 searchValue, int32 startRowIndex = 1)const;
	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Worksheet")
		UExcelCell* FindFloatAtColumn(int32 columnIndex, float searchValue, int32 startRowIndex = 1)const;
	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Worksheet")
		UExcelCell* FindStringAtColumn(int32 columnIndex, FString searchValue, int32 startRowIndex = 1)const;
	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Worksheet")
		UExcelCell* FindDateTimeAtColumn(int32 columnIndex, FDateTime searchValue, int32 startRowIndex = 1)const;

	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Worksheet")
		UExcelCell* FindBoolAtColumnString(FString columnString, bool searchValue, int32 startRowIndex = 1)const;
	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Worksheet")
		UExcelCell* FindIntAtColumnString(FString columnString, int32 searchValue, int32 startRowIndex = 1)const;
	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Worksheet")
		UExcelCell* FindFloatAtColumnString(FString columnString, float searchValue, int32 startRowIndex = 1)const;
	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Worksheet")
		UExcelCell* FindStringAtColumnString(FString columnString, FString searchValue, int32 startRowIndex = 1)const;
	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Worksheet")
		UExcelCell* FindDateTimeAtColumnString(FString columnString, FDateTime searchValue, int32 startRowIndex = 1)const;

	//return column index
	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Worksheet")
		UExcelCell* FindBoolAtRow(int32 rowIndex, bool searchValue, int32 startColumnIndex = 1)const;
	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Worksheet")
		UExcelCell* FindIntAtRow(int32 rowIndex, int32 searchValue, int32 startColumnIndex = 1)const;
	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Worksheet")
		UExcelCell* FindFloatAtRow(int32 rowIndex, float searchValue, int32 startColumnIndex = 1)const;
	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Worksheet")
		UExcelCell* FindStringAtRow(int32 rowIndex, FString searchValue, int32 startColumnIndex = 1)const;
	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Worksheet")
		UExcelCell* FindDateTimeAtRow(int32 rowIndex, FDateTime searchValue, int32 startColumnIndex = 1)const;

public://find multiple
	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Worksheet")
		TArray<UExcelCell*> FindMultipleBoolAtColumn(int32 columnIndex, bool searchValue, int32 startRowIndex = 1)const;
	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Worksheet")
		TArray<UExcelCell*> FindMultipleIntAtColumn(int32 columnIndex, int32 searchValue, int32 startRowIndex = 1)const;
	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Worksheet")
		TArray<UExcelCell*> FindMultipleFloatAtColumn(int32 columnIndex, float searchValue, int32 startRowIndex = 1)const;
	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Worksheet")
		TArray<UExcelCell*> FindMultipleStringAtColumn(int32 columnIndex, FString searchValue, int32 startRowIndex = 1)const;
	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Worksheet")
		TArray<UExcelCell*> FindMultipleDateTimeAtColumn(int32 columnIndex, FDateTime searchValue, int32 startRowIndex = 1)const;

	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Worksheet")
		TArray<UExcelCell*> FindMultipleBoolAtColumnString(FString columnString, bool searchValue, int32 startRowIndex = 1)const;
	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Worksheet")
		TArray<UExcelCell*> FindMultipleIntAtColumnString(FString columnString, int32 searchValue, int32 startRowIndex = 1)const;
	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Worksheet")
		TArray<UExcelCell*> FindMultipleFloatAtColumnString(FString columnString, float searchValue, int32 startRowIndex = 1)const;
	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Worksheet")
		TArray<UExcelCell*> FindMultipleStringAtColumnString(FString columnString, FString searchValue, int32 startRowIndex = 1)const;
	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Worksheet")
		TArray<UExcelCell*> FindMultipleDateTimeAtColumnString(FString columnString, FDateTime searchValue, int32 startRowIndex = 1)const;

	//return column index
	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Worksheet")
		TArray<UExcelCell*> FindMultipleBoolAtRow(int32 rowIndex, bool searchValue, int32 startColumnIndex = 1)const;
	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Worksheet")
		TArray<UExcelCell*> FindMultipleIntAtRow(int32 rowIndex, int32 searchValue, int32 startColumnIndex = 1)const;
	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Worksheet")
		TArray<UExcelCell*> FindMultipleFloatAtRow(int32 rowIndex, float searchValue, int32 startColumnIndex = 1)const;
	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Worksheet")
		TArray<UExcelCell*> FindMultipleStringAtRow(int32 rowIndex, FString searchValue, int32 startColumnIndex = 1)const;
	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Worksheet")
		TArray<UExcelCell*> FindMultipleDateTimeAtRow(int32 rowIndex, FDateTime searchValue, int32 startColumnIndex = 1)const;

public://column mapping
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		const FExcelTableHeader& GetTableHeader()const { return mHeader; }

	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Worksheet")
		void SetTableHeader(const FExcelTableHeader& header);

	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Worksheet")
		FExcelTableHeader& SetTableFormat(ExcelTableFormat tableFormat);

	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Worksheet")
		UExcelWorksheetDataTable* SetRowStruct(UScriptStruct* RowStruct);
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		UExcelWorksheetDataTable* GetOrCreateDataTable();


	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		UScriptStruct* GetRowStructInUsed()const;

	void UpdateRuntimeColumnNames() const;
	int32 FindColumnIndex(FName columnName)const;
	bool HasValidDataTable();
	void ClearDataTable();
private:
	template<typename T>
	TArray<T> Internal_ArrayAtColumnStringSorted(FString columnString, TArray<int32>& outRowIndices, ExcelSortType sortType = ExcelSortType::None)const
	{
		int32 columnIndex = UExcelCellReferenceLibrary::ToColumnIndex(columnString);
		return Internal_ArrayAtColumnSorted<T>(columnIndex, outRowIndices, sortType);
	}

	template<typename T>
	TArray<T> Internal_ArrayAtColumnSorted(int32 columnIndex, TArray<int32>& outRowIndices, ExcelSortType sortType = ExcelSortType::None)const
	{
		TArray<UExcelCell*> cells = CellsAtColumn(columnIndex, sortType);
		TArray<T> result;
		for (int32 i = 0; i < cells.Num(); ++i)
		{
			UExcelCell* cell = cells[i];
			result.Add(DirectExcel::ToValue<T>(cell));
			outRowIndices.Add(cell->Row());
		}
		return result;
	}
	template<typename T>
	TArray<T> Internal_ArrayAtRowSorted(int32 rowIndex, TArray<int32>& outColumnIndices, ExcelSortType sortType = ExcelSortType::None)const
	{
		TArray<UExcelCell*> cells = CellsAtRow(rowIndex, sortType);
		TArray<T> result;
		for (int32 i = 0; i < cells.Num(); ++i)
		{
			UExcelCell* cell = cells[i];
			result.Add(DirectExcel::ToValue<T>(cell));
			outColumnIndices.Add(cell->Column());
		}
		return result;
	}

	template<typename T>
	int32 FindRowAtColumn(int32 columnIndex, T searchValue)const
	{
		UExcelCell* cell = FindCellAtColumn(columnIndex, searchValue);
		return cell != nullptr ? cell->Row() : -1;
	}

	template<typename T>
	int32 FindColumnAtRow(int32 rowIndex, T searchValue)const
	{
		UExcelCell* cell = FindCellAtRow(rowIndex, searchValue);
		return cell != nullptr ? cell->Column() : -1;
	}

	template<typename T>
	UExcelCell* FindCellAtColumn(int32 columnIndex, T searchValue, int32 startRowIndex = 1)const
	{
		TArray<UExcelCell*> cells = CellsAtColumn(columnIndex);
		startRowIndex = FMath::Clamp(startRowIndex - 1, 0, cells.Num() - 1);
		for (int32 i = startRowIndex; i < cells.Num(); ++i)
		{
			UExcelCell* cell = cells[i];
			if (DirectExcel::ToValue<T>(cell) == searchValue)
			{
				return cell;
			}
		}
		return nullptr;
	}

	template<typename T>
	UExcelCell* FindCellAtRow(int32 rowIndex, T searchValue, int32 startColumnIndex = 1)const
	{
		TArray<UExcelCell*> cells = CellsAtRow(rowIndex);
		startColumnIndex = FMath::Clamp(startColumnIndex - 1, 0, cells.Num() - 1);
		for (int32 i = startColumnIndex; i < cells.Num(); ++i)
		{
			UExcelCell* cell = cells[i];
			if (DirectExcel::ToValue<T>(cell) == searchValue)
			{
				return cell;
			}
		}
		return nullptr;
	}

	template<typename T>
	TArray<UExcelCell*> FindMultipleCellAtColumn(int32 columnIndex, T searchValue, int32 startRowIndex = 1)const
	{
		TArray<UExcelCell*> results;
		TArray<UExcelCell*> cells = CellsAtColumn(columnIndex);
		startRowIndex = FMath::Clamp(startRowIndex - 1, 0, cells.Num() - 1);
		for (int32 i = startRowIndex; i < cells.Num(); ++i)
		{
			UExcelCell* cell = cells[i];
			if (DirectExcel::ToValue<T>(cell) == searchValue)
			{
				results.Add(cell);
			}
		}
		return results;
	}
	template<typename T>
	TArray<UExcelCell*> FindMultipleCellAtRow(int32 rowIndex, T searchValue, int32 startColumnIndex = 1)const
	{
		TArray<UExcelCell*> results;
		TArray<UExcelCell*> cells = CellsAtRow(rowIndex);
		startColumnIndex = FMath::Clamp(startColumnIndex - 1, 0, cells.Num() - 1);
		for (int32 i = startColumnIndex; i < cells.Num(); ++i)
		{
			UExcelCell* cell = cells[i];
			if (DirectExcel::ToValue<T>(cell) == searchValue)
			{
				results.Add(cell);
			}
		}
		return results;
	}
private:
	UExcelCell* CreateCell(xlnt::cell c) const;
private:
	mutable bool mIsDataTableDirty = true;

	UPROPERTY(Transient)
		UExcelWorksheetDataTable* mDataTable;

	mutable TMap<FName, int32> mRuntimeColumnNames;
private:

	TWeakObjectPtr<class UExcelWorkbook> mWorkbook;
	xlnt::worksheet mData;
	FExcelTableHeader mHeader;
};
