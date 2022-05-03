// Copyright 2018 Jianzhao Fu. All Rights Reserved.
#include "ExcelTableHeader.h"
#include "ExcelWorksheet.h"

void UDirectExcelTableHeaderLibrary::SetColumnName(FExcelTableHeader& header, int32 columnIndex, FName columnName)
{
	header.ColumnNames.Add(columnName, columnIndex);
}

void UDirectExcelTableHeaderLibrary::SetColumnNames(FExcelTableHeader& header, const TMap<FName, int32>& columnNames)
{
	header.ColumnNames.Append(columnNames);
}

void UDirectExcelTableHeaderLibrary::SetColumnNamesInOrder(FExcelTableHeader& header, const TArray<FName>& columnNames, int32 columnIndex/*=1*/)
{
	header.ColumnNames.Empty();
	for (const FName& name : columnNames)
	{
		header.ColumnNames.Add(name, columnIndex++);
	}
}


bool UDirectExcelTableHeaderLibrary::RemoveColumnNameWithIndex(FExcelTableHeader& header, int32 columnIndex)
{
	const FName* columnName = header.ColumnNames.FindKey(columnIndex);
	if (columnName != nullptr)
	{
		header.ColumnNames.Remove(*columnName);
		return true;
	}
	return false;
}

bool UDirectExcelTableHeaderLibrary::RemoveColumnName(FExcelTableHeader& header, FName columnName)
{
	return header.ColumnNames.Remove(columnName) > 0;
}

int32 FExcelTableHeader::GetColumnHeaderStartRowIndex() const
{
	switch (TableFormat)
	{
	case ExcelTableFormat::TableWithFirstRowColumnHeader:
	case ExcelTableFormat::UnrealDataTableExportCSV:
	return 1;
	default:
		break;
	}

	return ColumnHeaderStartRowIndex;
}

int32 FExcelTableHeader::GetColumnHeaderStartColumnIndex() const
{
	switch (TableFormat)
	{
	case ExcelTableFormat::TableWithFirstRowColumnHeader:
		return 1;
	case ExcelTableFormat::UnrealDataTableExportCSV:
		return 2;
	default:
		break;
	}

	return ColumnHeaderStartColumnIndex;
}
