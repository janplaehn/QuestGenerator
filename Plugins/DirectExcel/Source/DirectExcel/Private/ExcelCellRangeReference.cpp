// Copyright 2018 Jianzhao Fu. All Rights Reserved.
#include "ExcelCellRangeReference.h"
#include "ThirdParty.h"


FExcelCellRangeReference::FExcelCellRangeReference(int32 leftColumn, int32 topRow, int32 rightColumn, int32 bottomRow)
	:TopLeft(leftColumn, topRow), BottomRight(rightColumn, bottomRow)
{

}


FExcelCellRangeReference UExcelCellRangeReferenceLibrary::ToRange(FExcelCellReference cellRef)
{
	xlnt::cell_reference ref(cellRef.Column, cellRef.Row);
	xlnt::range_reference r = ref.to_range();
	xlnt::cell_reference topLeft = r.top_left();
	xlnt::cell_reference bottomRight = r.bottom_right();
	return FExcelCellRangeReference(topLeft.column().index, topLeft.row(), bottomRight.column().index, bottomRight.row());
}

FString UExcelCellRangeReferenceLibrary::ToString(FExcelCellRangeReference rangeReference)
{
	xlnt::range_reference r(rangeReference.TopLeft.Column, rangeReference.TopLeft.Row, rangeReference.BottomRight.Column, rangeReference.BottomRight.Row);
	std::string str = r.to_string();
	return UTF8_TO_TCHAR(str.c_str());
}

FExcelCellRangeReference UExcelCellRangeReferenceLibrary::MakeCellRangeWithString(FString rangeString)
{
	std::string str = TCHAR_TO_UTF8(*rangeString);
	xlnt::range_reference r(str);
	xlnt::cell_reference topLeft = r.top_left();
	xlnt::cell_reference bottomRight = r.bottom_right();
	return FExcelCellRangeReference(topLeft.column().index, topLeft.row(), bottomRight.column().index, bottomRight.row());
}

bool UExcelCellRangeReferenceLibrary::IsSingleCell(FExcelCellRangeReference rangeReference)
{
	xlnt::range_reference r(rangeReference.TopLeft.Column, rangeReference.TopLeft.Row, rangeReference.BottomRight.Column, rangeReference.BottomRight.Row);
	return r.is_single_cell();
}

int32 UExcelCellRangeReferenceLibrary::Width(FExcelCellRangeReference rangeReference)
{
	xlnt::range_reference r(rangeReference.TopLeft.Column, rangeReference.TopLeft.Row, rangeReference.BottomRight.Column, rangeReference.BottomRight.Row);
	return r.width();
}

int32 UExcelCellRangeReferenceLibrary::Height(FExcelCellRangeReference rangeReference)
{
	xlnt::range_reference r(rangeReference.TopLeft.Column, rangeReference.TopLeft.Row, rangeReference.BottomRight.Column, rangeReference.BottomRight.Row);
	return r.height();
}

FExcelCellRangeReference UExcelCellRangeReferenceLibrary::MoveBy(FExcelCellRangeReference rangeReference, int32 columnOffset, int32 rowOffset)
{
	xlnt::range_reference r(rangeReference.TopLeft.Column, rangeReference.TopLeft.Row, rangeReference.BottomRight.Column, rangeReference.BottomRight.Row);
	xlnt::range_reference r2 = r.make_offset(columnOffset, rowOffset);
	xlnt::cell_reference topLeft = r2.top_left();
	xlnt::cell_reference bottomRight = r2.bottom_right();
	return FExcelCellRangeReference(topLeft.column().index, topLeft.row(), bottomRight.column().index, bottomRight.row());
}

bool UExcelCellRangeReferenceLibrary::Equal_ExcelCellRangeReference(FExcelCellRangeReference A, FExcelCellRangeReference B)
{
	return A == B;
}

bool UExcelCellRangeReferenceLibrary::NotEqual_ExcelCellRangeReference(FExcelCellRangeReference A, FExcelCellRangeReference B)
{
	return A != B;
}
