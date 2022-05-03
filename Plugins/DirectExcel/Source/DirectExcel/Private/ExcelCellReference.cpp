// Copyright 2018 Jianzhao Fu. All Rights Reserved.
#include "ExcelCellReference.h"
#include "ThirdParty.h"
#include "LogTypes.h"


FExcelCellReference::FExcelCellReference(int32 column, int32 row)
	:Column(column), Row(row)
{
	if (column == 0 || row == 0)
	{
		UE_LOG(LogDirectExcel, Error, TEXT("Row/Column Index should >0. Begins from 1."));
	}
}

FString UExcelCellReferenceLibrary::ToString(FExcelCellReference cellRef)
{
	xlnt::cell_reference ref(cellRef.Column, cellRef.Row);
	std::string str = ref.to_string();
	return UTF8_TO_TCHAR(str.c_str());
}

FExcelCellReference UExcelCellReferenceLibrary::MakeCellReferenceWithString(FString referenceString)
{
	std::string str = TCHAR_TO_UTF8(*referenceString);
	xlnt::cell_reference ref(str);
	return FExcelCellReference(ref.column().index, ref.row());
}

FExcelCellReference UExcelCellReferenceLibrary::MoveBy(FExcelCellReference cellRef, int32 columnOffset, int32 rowOffset)
{
	xlnt::cell_reference ref(cellRef.Column, cellRef.Row);
	ref = ref.make_offset(columnOffset, rowOffset);
	return FExcelCellReference(ref.column().index, ref.row());
}

int32 UExcelCellReferenceLibrary::ToColumnIndex(FString columString)
{
	std::string str = TCHAR_TO_UTF8(*columString);
	return xlnt::column_t::column_index_from_string(str);
}

FString UExcelCellReferenceLibrary::ToColumnString(int32 columnIndex)
{
	std::string str = xlnt::column_t::column_string_from_index(columnIndex);
	return str.c_str();
}

bool UExcelCellReferenceLibrary::Equal_ExcelCellReference(FExcelCellReference A, FExcelCellReference B)
{
	return A == B;
}

bool UExcelCellReferenceLibrary::NotEqual_ExcelCellReference(FExcelCellReference A, FExcelCellReference B)
{
	return A != B;
}

