// Copyright 2018 Jianzhao Fu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ExcelTypes.h"
#include "ExcelCellReference.generated.h"

/**
 *Excle cell reference. e.g "A1" or "B2"
 */
USTRUCT(BlueprintType, Category = "DirectExcel")
struct DIRECTEXCEL_API FExcelCellReference
{
	GENERATED_BODY()
public:
	friend class UExcelCellReferenceLibrary;
	FExcelCellReference() = default;
	FExcelCellReference(int32 column, int32 row);

	bool operator==(const FExcelCellReference& other)const { return Column == other.Column && Row == other.Row; }
	bool operator!=(const FExcelCellReference& other)const { return Column != other.Column || Row != other.Row; }

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DirectExcel|CellReference")
		int32 Column = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DirectExcel|CellReference")
		int32 Row = 1;
};

/**
*Blueprint functions to make cell reference
*/
UCLASS(DisplayName = "DirectExcel", Category = "DirectExcel")
class DIRECTEXCEL_API UExcelCellReferenceLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "DirectExcel|CellReference")
		static FString ToString(FExcelCellReference cellRef);

	UFUNCTION(BlueprintPure, Category = "DirectExcel|CellReference")
		static FExcelCellReference MakeCellReferenceWithString(FString referenceString);

	UFUNCTION(BlueprintPure, Category = "DirectExcel|CellReference")
		static FExcelCellReference MoveBy(FExcelCellReference cellRef, int32 columnOffset, int32 rowOffset);

	UFUNCTION(BlueprintPure, Category = "DirectExcel|CellReference")
		static int32 ToColumnIndex(FString columString);

	UFUNCTION(BlueprintPure, Category = "DirectExcel|CellReference")
		static FString ToColumnString(int32 columnIndex);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Equal (ExcelCellReference)", CompactNodeTitle = "==", Keywords = "== equal"), Category = "DirectExcel|CellReference")
		static bool Equal_ExcelCellReference(FExcelCellReference A, FExcelCellReference B);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "NotEqual (ExcelCellReference)", CompactNodeTitle = "!=", Keywords = "!= not equal"), Category = "DirectExcel|CellReference")
		static bool NotEqual_ExcelCellReference(FExcelCellReference A, FExcelCellReference B);
};
