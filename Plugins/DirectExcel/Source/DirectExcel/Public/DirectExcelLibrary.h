// Copyright 2018 Jianzhao Fu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ExcelTypes.h"
#include "Engine/DataTable.h"
#include "ExcelCellReference.h"
#include "DirectExcelLibrary.generated.h"
/**
* Helper functions to load/save excel file
*/
UCLASS(DisplayName = "DirectExcel", Category = "DirectExcel")
class DIRECTEXCEL_API UDirectExcelLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	//async load
public:
	UFUNCTION(BlueprintCallable, Category = "DirectExcel")
		static class UExcelWorkbook* LoadExcel(FString path, ExcelFileRelateiveDir relativeDir = ExcelFileRelateiveDir::Absolute);
	UFUNCTION(BlueprintCallable, Category = "DirectExcel")
		static bool SaveExcel(UExcelWorkbook* workbook, FString path, ExcelFileRelateiveDir relativeDir = ExcelFileRelateiveDir::Absolute);
	UFUNCTION(BlueprintCallable, Category = "DirectExcel")
		static class UExcelWorkbook* CreateExcel();
	UFUNCTION(BlueprintPure, Category = "DirectExcel")
		static FString ToAbsolutePath(FString projectReleativePath, ExcelFileRelateiveDir relativeDir = ExcelFileRelateiveDir::Absolute);
	UFUNCTION(BlueprintPure, Category = "DirectExcel")
		static bool DoesExcelFileExists(FString path, ExcelFileRelateiveDir relativeDir = ExcelFileRelateiveDir::Absolute);

public: 
	UFUNCTION(BlueprintCallable, CustomThunk, Category = "DirectExcel|Worksheet", meta = (DisplayName = "ReadStructAtRowIndex", CustomStructureParam = "OutItem"))
		static bool ReadStructAtRowIndex(const UExcelWorksheet* sheet, int32 row, FTableRowBase& OutItem) { return false; }
	DECLARE_FUNCTION(execReadStructAtRowIndex);

	static bool ReadStructAtRowIndexRaw(const UExcelWorksheet* sheet, int32 row, const UScriptStruct* OutStruct, void* outStructData);
public:
	UFUNCTION(BlueprintCallable, CustomThunk, Category = "DirectExcel|Worksheet", meta = (DisplayName = "ReadStructWithRowName", CustomStructureParam = "OutItem"))
		static bool ReadStructWithRowName(const UExcelWorksheet* sheet, FName rowName, FTableRowBase& OutItem) { return false; }
	DECLARE_FUNCTION(execReadStructWithRowName);
	static bool ReadStructWithRowNameRaw(UExcelWorksheet* sheet, FName rowName, const UScriptStruct* OutStruct, void* outStructData);

public:
	UFUNCTION(BlueprintCallable, CustomThunk, Category = "DirectExcel|Worksheet", meta = (DisplayName = "ReadItemAtCell", CustomStructureParam = "OutItem"))
		static bool ReadItemAtCell(const UExcelWorksheet* sheet, FExcelCellReference cellReference,FTableRowBase& OutItem) { return false; }
	DECLARE_FUNCTION(execReadItemAtCell);

private:
	static FName GetPropertyColumnName(const FProperty& property);
};
