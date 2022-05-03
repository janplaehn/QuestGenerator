// Copyright 2018 Jianzhao Fu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ExcelTypes.h"
#include "ExcelCellReference.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "ExcelCellRangeReference.generated.h"

/**
 *Excel cell ranges. e.g. "A1:C3"
 */
USTRUCT(BlueprintType, Category = "DirectExcel")
struct DIRECTEXCEL_API FExcelCellRangeReference
{
	GENERATED_BODY()
public:
	friend class UExcelCellRangeReferenceLibrary;
	FExcelCellRangeReference() = default;
	FExcelCellRangeReference(FExcelCellReference topLeft, FExcelCellReference bottomRight)
		:TopLeft(topLeft), BottomRight(bottomRight) {}
	FExcelCellRangeReference(int32 leftColumn, int32 topRow, int32 rightColumn, int32 bottomRow);

	bool operator==(const FExcelCellRangeReference& other)const { return TopLeft == other.TopLeft&&BottomRight == other.BottomRight; }
	bool operator!=(const FExcelCellRangeReference& other)const { return TopLeft != other.TopLeft || BottomRight != other.BottomRight; }
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DirectExcel|CellRange")
		FExcelCellReference TopLeft;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DirectExcel|CellRange")
		FExcelCellReference BottomRight;
};

/**
*Blueprint functions to make cell range
*/
UCLASS(DisplayName = "DirectExcel", Category = "DirectExcel")
class DIRECTEXCEL_API UExcelCellRangeReferenceLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "DirectExcel|CellRange")
		static FExcelCellRangeReference ToRange(FExcelCellReference cellRef);
public:
	UFUNCTION(BlueprintPure, Category = "DirectExcel|CellRange")
		static FString ToString(FExcelCellRangeReference rangeReference);

	//TopLeft:BottomRight
	UFUNCTION(BlueprintPure, Category = "DirectExcel|CellRange")
		static FExcelCellRangeReference MakeCellRangeWithString(FString rangeString);

	UFUNCTION(BlueprintPure, Category = "DirectExcel|CellRange")
		static bool IsSingleCell(FExcelCellRangeReference rangeReference);

	UFUNCTION(BlueprintPure, Category = "DirectExcel|CellRange")
		static int32 Width(FExcelCellRangeReference rangeReference);
	UFUNCTION(BlueprintPure, Category = "DirectExcel|CellRange")
		static int32 Height(FExcelCellRangeReference rangeReference);

	UFUNCTION(BlueprintPure, Category = "DirectExcel|CellRange")
		static FExcelCellRangeReference MoveBy(FExcelCellRangeReference rangeReference, int32 columnOffset, int32 rowOffset);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Equal (ExcelCellRangeReference)", CompactNodeTitle = "==", Keywords = "== equal"), Category = "DirectExcel|CellRangeReference")
		static bool Equal_ExcelCellRangeReference(FExcelCellRangeReference A, FExcelCellRangeReference B);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "NotEqual (ExcelCellRangeReference)", CompactNodeTitle = "!=", Keywords = "!= not equal"), Category = "DirectExcel|CellRangeReference")
		static bool NotEqual_ExcelCellRangeReference(FExcelCellRangeReference A, FExcelCellRangeReference B);

};
