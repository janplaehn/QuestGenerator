// Copyright 2018 Jianzhao Fu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ExcelTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "ExcelTableHeader.generated.h"

class UExcelWorksheet;

/*
Name, Icon, Price
A, Texture2D'/Game/Asset/Image/T_Shop_Bone.T_Shop_Bone', 1
B, Texture2D'/Game/Asset/Image/T_Shop_Cloth.T_Shop_Cloth', 2
C, Texture2D'/Game/Asset/Image/T_Shop_Meat.T_Shop_Meat', 4
D, Texture2D'/Game/Asset/Image/T_Shop_Stone.T_Shop_Stone', 8
E, Texture2D'/Game/Asset/Image/T_Shop_Wood.T_Shop_Wood', 16
*/

/**
 * Excel Table header info, used for column mapping
 */
USTRUCT(BlueprintType, Category = "DirectExcel")
struct DIRECTEXCEL_API FExcelTableHeader
{
	GENERATED_BODY()
public:
	FExcelTableHeader() = default;
	FExcelTableHeader(const TMap<FName, int32>& columnNames) :ColumnNames(columnNames)
	{}
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DirectExcel|TableHeader")
		bool Enabled = true;	//enabled for data source

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "DirectExcel|TableHeader")
		UScriptStruct* RowStruct;	// == DataTable.RowStruct

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DirectExcel|TableHeader")
		ExcelTableFormat TableFormat;	//csv table or normal

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DirectExcel|TableHeader")
		TMap<FName, int32> ColumnNames;	//field name-column index

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DirectExcel|TableHeader")
		int32 ColumnHeaderStartRowIndex = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DirectExcel|TableHeader")
		int32 ColumnHeaderStartColumnIndex=1;	// or 2

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DirectExcel|TableHeader")
		int32 DataRowStartIndex = 2;	//start from 2
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DirectExcel|TableHeader")
		int32 DataRowEndIndex = -1;
public:
	int32 GetColumnHeaderStartRowIndex()const;
	int32 GetColumnHeaderStartColumnIndex()const;

};


/**
* Helper functions to set table header
*/
UCLASS(DisplayName = "DirectExcelTableHeader", Category = "DirectExcel")
class DIRECTEXCEL_API UDirectExcelTableHeaderLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure, Category = "DirectExcel|TableHeader")
		static const TMap<FName, int32>& GetColumnNames(UPARAM(ref) FExcelTableHeader& header) { return header.ColumnNames; }
	UFUNCTION(BlueprintCallable, Category = "DirectExcel|TableHeader")
		static void SetColumnName(UPARAM(ref) FExcelTableHeader& header, int32 columnIndex, FName columnName);

	UFUNCTION(BlueprintCallable, Category = "DirectExcel|TableHeader")
		static void SetColumnNames(UPARAM(ref) FExcelTableHeader& header, const TMap<FName, int32>& columnNames);

	UFUNCTION(BlueprintCallable, Category = "DirectExcel|TableHeader")
		static void SetColumnNamesInOrder(UPARAM(ref) FExcelTableHeader& header, const TArray<FName>& columnNames, int32 startColumnIndex = 1);


	UFUNCTION(BlueprintCallable, Category = "DirectExcel|TableHeader")
		static bool RemoveColumnNameWithIndex(UPARAM(ref) FExcelTableHeader& header, int32 columnIndex);
	UFUNCTION(BlueprintCallable, Category = "DirectExcel|TableHeader")
		static bool RemoveColumnName(UPARAM(ref) FExcelTableHeader& header, FName columnName);

};
