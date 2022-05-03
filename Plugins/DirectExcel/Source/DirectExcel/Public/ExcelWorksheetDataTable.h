// Copyright 2018 Jianzhao Fu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ExcelTypes.h"
#include "Engine/DataTable.h"

#include "ExcelWorksheetDataTable.generated.h"

class UExcelWorksheet;

UCLASS(Category = "DirectExcel")
class DIRECTEXCEL_API UExcelWorksheetDataTable : public UDataTable
{
	GENERATED_BODY()
public:
	void InitializeFromSheet(const UExcelWorksheet* sheet,const UScriptStruct* rowStruct);

};
