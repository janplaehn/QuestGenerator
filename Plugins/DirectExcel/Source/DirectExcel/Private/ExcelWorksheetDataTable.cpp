// Copyright 2018 Jianzhao Fu. All Rights Reserved.
#include "ExcelWorksheetDataTable.h"
#include "ExcelWorksheet.h"
#include "DirectExcelLibrary.h"


void UExcelWorksheetDataTable::InitializeFromSheet(const UExcelWorksheet* sheet, const UScriptStruct* rowStruct)
{
	RowStruct = const_cast<UScriptStruct*>(rowStruct);

	int32 rowMin= sheet->GetTableHeader().DataRowStartIndex;
	rowMin=FMath::Max(rowMin, sheet->LowestRow());

	int32 rowMax = sheet->GetTableHeader().DataRowEndIndex;

	if (rowMax <= 0)
	{
		rowMax = sheet->HighestRow();
	}
	else
	{
		rowMax = FMath::Min(rowMax, sheet->HighestRow());
	}

	int32 columnIndex= sheet->GetTableHeader().ColumnHeaderStartColumnIndex;

	for (int32 i = rowMin; i <= rowMax; ++i)
	{
		FString rowNameStr = sheet->ToStringAt(columnIndex, i);
		FName rowName = FName(rowNameStr);
		if (RowMap.Contains(rowName))
		{
			UE_LOG(LogDirectExcel, Error, TEXT("Duplicate row name: %s found while enable DataTable mode. '%s'!"), *rowNameStr, *GetPathName());
			continue;
		}

		uint8* RowData = (uint8*)FMemory::Malloc(RowStruct->GetStructureSize());
		RowStruct->InitializeStruct(RowData);
		if (UDirectExcelLibrary::ReadStructAtRowIndexRaw(sheet, i, RowStruct, RowData))
		{
			AddRowInternal(rowName, RowData);
		}
		else
		{
			FMemory::Free(RowData);
		}
	}
}
