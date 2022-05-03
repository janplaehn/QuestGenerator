// Copyright 2018 Jianzhao Fu. All Rights Reserved.
#include "ExcelWorksheet.h"
#include "ExcelCell.h"
#include "ExcelWorkbook.h"
#include "DirectExcelLibrary.h"
#include "Engine.h"
#include "ExcelWorksheetDataTable.h"

void UExcelWorksheet::Initialize(class UExcelWorkbook* workbook, xlnt::worksheet data)
{
	mWorkbook = workbook;
	mData = data;
	mIsDataTableDirty=true;
	mDataTable=nullptr;
}

void UExcelWorksheet::SetTableHeader(const FExcelTableHeader& header)
{
	mHeader = header;
	mIsDataTableDirty=true;
	UpdateRuntimeColumnNames();
}

FExcelTableHeader& UExcelWorksheet::SetTableFormat(ExcelTableFormat tableFormat)
{
	if (mHeader.TableFormat!=tableFormat)
	{
		mHeader.TableFormat = tableFormat;
		UpdateRuntimeColumnNames();
		mIsDataTableDirty = true;
	}

	return mHeader;
}


void UExcelWorksheet::UpdateRuntimeColumnNames() const
{
	mRuntimeColumnNames.Empty();
	int32 startColumnIndex = mHeader.ColumnHeaderStartColumnIndex;
	int32 endColumnIndex = HighestColumn();

	for (int32 i = startColumnIndex; i <= endColumnIndex; ++i)
	{
		FString str = ToStringAt(i, mHeader.ColumnHeaderStartRowIndex);
		if (!str.IsEmpty())
		{
			mRuntimeColumnNames.Add(FName(str), i);	//start from 1
		}
	}
	mRuntimeColumnNames.Append(mHeader.ColumnNames);
}

int32 UExcelWorksheet::FindColumnIndex(FName columnName) const
{
	const int32* columnIndex = mRuntimeColumnNames.Find(columnName);
	if (columnIndex != nullptr)
	{
		return *columnIndex;
	}
	return -1;
}

bool UExcelWorksheet::HasValidDataTable()
{
	return  mHeader.Enabled && GetOrCreateDataTable() != nullptr && mRuntimeColumnNames.Num()>0;
}

void UExcelWorksheet::ClearDataTable()
{
	mDataTable=nullptr;
	mIsDataTableDirty=true;
	mRuntimeColumnNames.Empty();
}

UExcelWorksheetDataTable* UExcelWorksheet::SetRowStruct(UScriptStruct* RowStruct)
{
	UScriptStruct* currentStruct = GetRowStructInUsed();
	mHeader.RowStruct = RowStruct;
	UScriptStruct* newStruct = GetRowStructInUsed();
	if (currentStruct == newStruct)
	{
		return mDataTable;	//not changed
	}

	mIsDataTableDirty=true;
	return GetOrCreateDataTable();
}

UExcelWorksheetDataTable* UExcelWorksheet::GetOrCreateDataTable()
{
	if (!mIsDataTableDirty)
	{
		return mDataTable;
	}

	//parse column names
	UpdateRuntimeColumnNames();

	//create data table
	mDataTable = NewObject<UExcelWorksheetDataTable>(GetTransientPackage(), NAME_None);
	mDataTable->InitializeFromSheet(this, mHeader.RowStruct);

	mIsDataTableDirty=false;
	return mDataTable;
}

UScriptStruct* UExcelWorksheet::GetRowStructInUsed()const
{
	return mHeader.RowStruct;
}

void UExcelWorksheet::SetWorkbook(class UExcelWorkbook* val)
{
	mWorkbook = val;
}

UExcelWorkbook* UExcelWorksheet::ParentWorkbook() const
{
	return mWorkbook.Get();
}

FString UExcelWorksheet::Title()const
{
	return mData.title().c_str();
}

void UExcelWorksheet::SetTitle(FString title)
{
	std::string str = TCHAR_TO_UTF8(*title);
	mData.title(str);
}

int32 UExcelWorksheet::Id()const
{
	return (int32)mData.id();	//won't be bigger than int32
}

int32 UExcelWorksheet::Index()const
{
	return (int32)mData.workbook().index(mData);
}

FString UExcelWorksheet::Print(bool bPrintToScreen /*= true*/, FLinearColor TextColor /*= FLinearColor(0.0, 0.66, 1.0)*/, float Duration /*= 2.f*/)
{

	FString result;

	for (int32 i = LowestRow(); i <= HighestRow(); ++i)
	{
		result += PrintRow(i, false, TextColor, Duration);
		result += TEXT("\n");
	}

	if (bPrintToScreen)
	{
		GEngine->AddOnScreenDebugMessage(-1, Duration, TextColor.ToFColor(true), result);
	}
	return result;
}


int32 UExcelWorksheet::RowCount(bool skipNull/*=true*/)const
{
	return (int32)mData.rows(skipNull).length();
}

int32 UExcelWorksheet::LowestRow()const
{
	return mData.lowest_row();
}

int32 UExcelWorksheet::HighestRow()const
{
	return mData.highest_row();
}

void UExcelWorksheet::ClearRow(int32 rowIndex)
{
	mData.clear_row(rowIndex);
}

bool UExcelWorksheet::IsValidRow(int32 rowIndex) const
{
	if (rowIndex < LowestRow())
	{
		return false;
	}

	if (rowIndex > HighestRow())
	{
		return false;
	}
	return true;
}

FString UExcelWorksheet::PrintRow(int32 rowIndex /*= 1*/, bool bPrintToScreen /*= true*/, FLinearColor TextColor /*= FLinearColor(0.0, 0.66, 1.0)*/, float Duration /*= 2.f*/)
{

	FString result;

	if (rowIndex <= 0)
	{
		UE_LOG(LogDirectExcel, Warning, TEXT("RowIndex should begins from 1."));
		return result;
	}

	for (int32 i = LowestColumn(); i <= HighestColumn(); ++i)
	{
		UExcelCell* cell = CellAtRef(FExcelCellReference(i, rowIndex));
		if (cell != nullptr)
		{
			result += cell->ToString();
			result += TEXT("\t,");
		}
	}

	if (result.EndsWith(TEXT(",")))
	{
		result.RemoveAt(result.Len() - 1);
	}

	if (bPrintToScreen)
	{
		GEngine->AddOnScreenDebugMessage(-1, Duration, TextColor.ToFColor(true), result);
	}
	return result;
}

int32 UExcelWorksheet::ColumnCount(bool skipNull /*= true*/)const
{
	return (int32)mData.columns(skipNull).length();
}

int32 UExcelWorksheet::LowestColumn()const
{
	return mData.lowest_column().index;
}

int32 UExcelWorksheet::HighestColumn()const
{
	return mData.highest_column().index;
}


bool UExcelWorksheet::IsValidColumn(int32 columnIndex) const
{
	if (columnIndex < LowestColumn())
	{
		return false;
	}

	if (columnIndex > HighestColumn())
	{
		return false;
	}
	return true;
}

void UExcelWorksheet::ClearColumn(int32 columnIndex)
{
	if (columnIndex <= 0 || columnIndex > HighestColumn())
	{
		UE_LOG(LogDirectExcel, Warning, TEXT("ColumnIndex should begins from 1."));
		return;
	}

	if (columnIndex > HighestColumn())
	{
		UE_LOG(LogDirectExcel, Warning, TEXT("ColumnIndex:%d should < %d."), columnIndex, HighestColumn());
		return;
	}


	for (auto row : mData.rows())
	{
		auto cell = row[columnIndex];
		cell.clear_value();
	}

}

void UExcelWorksheet::ClearColumnWithString(FString columnString)
{
	int32 index = UExcelCellReferenceLibrary::ToColumnIndex(columnString);
	return ClearColumn(index);
}

FString UExcelWorksheet::PrintColumn(int32 columnIndex, bool bPrintToScreen /*= true*/, FLinearColor TextColor /*= FLinearColor(0.0, 0.66, 1.0)*/, float Duration /*= 2.f*/)
{
	FString result;

	if (columnIndex <= 0)
	{
		UE_LOG(LogDirectExcel, Warning, TEXT("ColumnIndex should begins from 1."));
		return result;
	}

	for (int32 i = LowestRow(); i <= HighestRow(); ++i)
	{
		UExcelCell* cell = CellAtRef(FExcelCellReference(columnIndex, i));
		if (cell != nullptr)
		{
			result += cell->ToString();
			result += TEXT("\n");
		}
	}

	if (result.EndsWith(TEXT("\n")))
	{
		result.RemoveAt(result.Len() - 1);
	}

	if (bPrintToScreen)
	{
		GEngine->AddOnScreenDebugMessage(-1, Duration, TextColor.ToFColor(true), result);
	}
	return result;
}

FString UExcelWorksheet::PrintColumnWithString(FString columnString, bool bPrintToScreen /*= true*/, FLinearColor TextColor /*= FLinearColor(0.0, 0.66, 1.0)*/, float Duration /*= 2.f*/)
{
	int32 index = UExcelCellReferenceLibrary::ToColumnIndex(columnString);
	return PrintColumn(index, bPrintToScreen, TextColor, Duration);
}


TArray<UExcelCell*> UExcelWorksheet::CellsAtRow(int32 rowIndex, ExcelSortType sortType /*= ExcelSortType::None*/)const
{
	TArray<UExcelCell*> result;

	if (rowIndex <= 0)
	{
		UE_LOG(LogDirectExcel, Warning, TEXT("RowIndex should begins from 1."));
		return result;
	}

	for (int32 i = LowestColumn(); i <= HighestColumn(); ++i)
	{
		UExcelCell* cell = CellAtRef(FExcelCellReference(i, rowIndex));
		if (cell != nullptr)
		{
			result.Add(cell);
		}
	}

	if (sortType != ExcelSortType::None)
	{
		result.Sort([sortType](UExcelCell& left, UExcelCell& right)->bool
			{
				if (sortType == ExcelSortType::Ascending)
				{
					return UExcelCell::LessEqual_ExcelCellExcelCell(&left, &right);
				}
				return UExcelCell::GreaterEqual_ExcelCellExcelCell(&left, &right);
			});
	}

	return result;
}

TArray<UExcelCell*> UExcelWorksheet::CellsAtColumn(int32 columnIndex, ExcelSortType sortType /*= ExcelSortType::None*/)const
{
	TArray<UExcelCell*> result;
	if (columnIndex <= 0)
	{
		UE_LOG(LogDirectExcel, Warning, TEXT("ColumnIndex should begins from 1."));
		return result;
	}

	for (int32 i = LowestRow(); i <= HighestRow(); ++i)
	{
		UExcelCell* cell = CellAtRef(FExcelCellReference(columnIndex, i));
		if (cell != nullptr)
		{
			result.Add(cell);
		}
	}

	if (sortType != ExcelSortType::None)
	{
		result.Sort([sortType](UExcelCell& left, UExcelCell& right)->bool
			{
				if (sortType == ExcelSortType::Ascending)
				{
					return UExcelCell::LessEqual_ExcelCellExcelCell(&left, &right);
				}
				return UExcelCell::GreaterEqual_ExcelCellExcelCell(&left, &right);
			});
	}
	return result;
}


TArray<UExcelCell*> UExcelWorksheet::CellsAtColumnString(FString columnString, ExcelSortType sortType /*= ExcelSortType::None*/)const
{
	int32 index = UExcelCellReferenceLibrary::ToColumnIndex(columnString);
	return CellsAtColumn(index, sortType);
}

FExcelCellRangeReference UExcelWorksheet::CalculateDimension()const
{

	xlnt::range_reference r = mData.calculate_dimension();
	xlnt::cell_reference topLeft = r.top_left();
	xlnt::cell_reference bottomRight = r.bottom_right();
	return FExcelCellRangeReference(topLeft.column().index, topLeft.row(), bottomRight.column().index, bottomRight.row());
}

void UExcelWorksheet::MergeCells(FExcelCellRangeReference rangeReference)
{

	xlnt::range_reference r(rangeReference.TopLeft.Column, rangeReference.TopLeft.Row, rangeReference.BottomRight.Column, rangeReference.BottomRight.Row);
	mData.merge_cells(r);
}

void UExcelWorksheet::UnmergeCells(FExcelCellRangeReference rangeReference)
{

	xlnt::range_reference r(rangeReference.TopLeft.Column, rangeReference.TopLeft.Row, rangeReference.BottomRight.Column, rangeReference.BottomRight.Row);
	mData.unmerge_cells(r);
}

TArray<FExcelCellRangeReference> UExcelWorksheet::AllMergedRanges()
{
	TArray<FExcelCellRangeReference> result;
	for (const xlnt::range_reference& r : mData.merged_ranges())
	{
		xlnt::cell_reference topLeft = r.top_left();
		xlnt::cell_reference bottomRight = r.bottom_right();
		result.Emplace(topLeft.column().index, topLeft.row(), bottomRight.column().index, bottomRight.row());
	}
	return result;
}

bool UExcelWorksheet::HasCell(FExcelCellReference cellReference)const
{
	return mData.has_cell(xlnt::cell_reference(cellReference.Column, cellReference.Row));
}

UExcelCell* UExcelWorksheet::CellAt(int32 column, int32 row) const
{
	return CellAtRef(FExcelCellReference(column, row));
}

UExcelCell* UExcelWorksheet::CellAtRef(FExcelCellReference cellReference)const
{
	xlnt::cell_reference cr(cellReference.Column, cellReference.Row);

	xlnt::worksheet* mutableData = (xlnt::worksheet*)&mData;
	mutableData->reserve(cellReference.Row);

	xlnt::cell c = mutableData->cell(cr);
	return CreateCell(c);
}

UExcelCell* UExcelWorksheet::CellAtId(FString cellString)const
{
	return CellAtRef(UExcelCellReferenceLibrary::MakeCellReferenceWithString(cellString));
}

void UExcelWorksheet::ClearCell(FExcelCellReference cellReference)
{
	xlnt::cell_reference cr(cellReference.Column, cellReference.Row);
	mData.clear_cell(cr);
}

void UExcelWorksheet::ClearCells(FExcelCellRangeReference rangeReference)
{
	for (int32 i = rangeReference.TopLeft.Row; i <= rangeReference.BottomRight.Row; ++i)
	{
		for (int32 j = rangeReference.TopLeft.Column; j <= rangeReference.BottomRight.Column; ++j)
		{
			ClearCell(FExcelCellReference(j, i));
		}
	}
}

bool UExcelWorksheet::HasActiveCell()const
{
	return mData.has_active_cell();
}

UExcelCell* UExcelWorksheet::ActiveCell()const
{
	if (!mData.has_active_cell())
	{
		return nullptr;
	}

	xlnt::cell c = mData.cell(mData.active_cell());
	return CreateCell(c);
}



bool UExcelWorksheet::ToBool(FExcelCellReference cellReference)const
{
	xlnt::cell_reference cr(cellReference.Column, cellReference.Row);
	if (!mData.has_cell(cr))
	{
		return false;
	}
	xlnt::cell c = mData.cell(cr);
	return c.value<bool>();
}

int32 UExcelWorksheet::ToInt(FExcelCellReference cellReference)const
{
	xlnt::cell_reference cr(cellReference.Column, cellReference.Row);
	if (!mData.has_cell(cr))
	{
		return -1;
	}
	xlnt::cell c = mData.cell(cr);
	return c.value<int32>();
}

float UExcelWorksheet::ToFloat(FExcelCellReference cellReference)const
{
	xlnt::cell_reference cr(cellReference.Column, cellReference.Row);
	if (!mData.has_cell(cr))
	{
		return 0.f;
	}
	xlnt::cell c = mData.cell(cr);
	return c.value<float>();
}

FString UExcelWorksheet::ToString(FExcelCellReference cellReference)const
{
	xlnt::cell_reference cr(cellReference.Column, cellReference.Row);
	if (!mData.has_cell(cr))
	{
		return FString();
	}
	xlnt::cell c = mData.cell(cr);
	return UTF8_TO_TCHAR(c.to_string().c_str());
}

FDateTime UExcelWorksheet::ToDateTime(FExcelCellReference cellReference)const
{
	FDateTime result;

	xlnt::cell_reference cr(cellReference.Column, cellReference.Row);
	if (!mData.has_cell(cr))
	{
		return result;
	}
	xlnt::cell c = mData.cell(cr);
	xlnt::datetime date = c.value<xlnt::datetime>();
	return FDateTime(date.year, date.month, date.day, date.hour, date.minute, date.second, date.microsecond);
}



bool UExcelWorksheet::ToBoolAt(int32 column, int32 row) const
{
	return ToBool(FExcelCellReference(column, row));
}

int32 UExcelWorksheet::ToIntAt(int32 column, int32 row) const
{
	return ToInt(FExcelCellReference(column, row));
}

float UExcelWorksheet::ToFloatAt(int32 column, int32 row) const
{
	return ToFloat(FExcelCellReference(column, row));
}

FString UExcelWorksheet::ToStringAt(int32 column, int32 row) const
{
	return ToString(FExcelCellReference(column, row));
}

FDateTime UExcelWorksheet::ToDateTimeAt(int32 column, int32 row) const
{
	return ToDateTime(FExcelCellReference(column, row));
}

bool UExcelWorksheet::ToBoolWithId(FString cellReferenceString)const
{
	return ToBool(UExcelCellReferenceLibrary::MakeCellReferenceWithString(cellReferenceString));
}

int32 UExcelWorksheet::ToIntWithId(FString cellReferenceString)const
{
	return ToInt(UExcelCellReferenceLibrary::MakeCellReferenceWithString(cellReferenceString));
}

float UExcelWorksheet::ToFloatWithId(FString cellReferenceString)const
{
	return ToFloat(UExcelCellReferenceLibrary::MakeCellReferenceWithString(cellReferenceString));
}

FString UExcelWorksheet::ToStringWithId(FString cellReferenceString)const
{
	return ToString(UExcelCellReferenceLibrary::MakeCellReferenceWithString(cellReferenceString));
}

FDateTime UExcelWorksheet::ToDateTimeWithId(FString cellReferenceString)const
{
	return ToDateTime(UExcelCellReferenceLibrary::MakeCellReferenceWithString(cellReferenceString));
}

TArray<bool> UExcelWorksheet::BoolArrayAtRow(int32 rowIndex)const
{
	TArray<bool> result;
	for (int32 i = LowestColumn(); i <= HighestColumn(); ++i)
	{
		result.Add(ToBool(FExcelCellReference(i, rowIndex)));
	}
	return result;
}

TArray<int32> UExcelWorksheet::IntArrayAtRow(int32 rowIndex)const
{
	TArray<int32> result;
	for (int32 i = LowestColumn(); i <= HighestColumn(); ++i)
	{
		result.Add(ToInt(FExcelCellReference(i, rowIndex)));
	}
	return result;
}

TArray<float> UExcelWorksheet::FloatArrayAtRow(int32 rowIndex)const
{
	TArray<float> result;
	for (int32 i = LowestColumn(); i <= HighestColumn(); ++i)
	{
		result.Add(ToFloat(FExcelCellReference(i, rowIndex)));
	}
	return result;
}

TArray<FString> UExcelWorksheet::StringArrayAtRow(int32 rowIndex)const
{
	TArray<FString> result;
	for (int32 i = LowestColumn(); i <= HighestColumn(); ++i)
	{
		result.Add(ToString(FExcelCellReference(i, rowIndex)));
	}
	return result;
}

TArray<FDateTime> UExcelWorksheet::DateTimeArrayAtRow(int32 rowIndex)const
{
	TArray<FDateTime> result;
	for (int32 i = LowestColumn(); i <= HighestColumn(); ++i)
	{
		result.Add(ToDateTime(FExcelCellReference(i, rowIndex)));
	}
	return result;
}

TArray<FVector> UExcelWorksheet::VectorArrayAtRow(int32 rowIndex) const
{
	TArray<FVector> result;
	for (int32 i = LowestColumn(); i <= HighestColumn(); ++i)
	{
		result.Add(ToVector(FExcelCellReference(i, rowIndex)));
	}
	return result;
}

TArray<FRotator> UExcelWorksheet::RotatorArrayAtRow(int32 rowIndex) const
{
	TArray<FRotator> result;
	for (int32 i = LowestColumn(); i <= HighestColumn(); ++i)
	{
		result.Add(ToRotator(FExcelCellReference(i, rowIndex)));
	}
	return result;
}

TArray<FTransform> UExcelWorksheet::TransformArrayAtRow(int32 rowIndex) const
{
	TArray<FTransform> result;
	for (int32 i = LowestColumn(); i <= HighestColumn(); ++i)
	{
		result.Add(ToTransform(FExcelCellReference(i, rowIndex)));
	}
	return result;
}

TArray<FColor> UExcelWorksheet::ColorArrayAtRow(int32 rowIndex) const
{
	TArray<FColor> result;
	for (int32 i = LowestColumn(); i <= HighestColumn(); ++i)
	{
		result.Add(ToColor(FExcelCellReference(i, rowIndex)));
	}
	return result;
}

TArray<FMatrix> UExcelWorksheet::MatrixArrayAtRow(int32 rowIndex) const
{
	TArray<FMatrix> result;
	for (int32 i = LowestColumn(); i <= HighestColumn(); ++i)
	{
		result.Add(ToMatrix(FExcelCellReference(i, rowIndex)));
	}
	return result;
}

TArray<bool> UExcelWorksheet::BoolArrayAtColumn(int32 columnIndex)const
{
	TArray<bool> result;
	for (int32 i = LowestRow(); i <= HighestRow(); ++i)
	{
		result.Add(ToBool(FExcelCellReference(columnIndex, i)));
	}
	return result;
}

TArray<int32> UExcelWorksheet::IntArrayAtColumn(int32 columnIndex)const
{
	TArray<int32> result;
	for (int32 i = LowestRow(); i <= HighestRow(); ++i)
	{
		result.Add(ToInt(FExcelCellReference(columnIndex, i)));
	}
	return result;
}

TArray<float> UExcelWorksheet::FloatArrayAtColumn(int32 columnIndex)const
{
	TArray<float> result;
	for (int32 i = LowestRow(); i <= HighestRow(); ++i)
	{
		result.Add(ToFloat(FExcelCellReference(columnIndex, i)));
	}
	return result;
}

TArray<FString> UExcelWorksheet::StringArrayAtColumn(int32 columnIndex)const
{
	TArray<FString> result;
	for (int32 i = LowestRow(); i <= HighestRow(); ++i)
	{
		result.Add(ToString(FExcelCellReference(columnIndex, i)));
	}
	return result;
}

TArray<FDateTime> UExcelWorksheet::DateTimeArrayAtColumn(int32 columnIndex)const
{
	TArray<FDateTime> result;
	for (int32 i = LowestRow(); i <= HighestRow(); ++i)
	{
		result.Add(ToDateTime(FExcelCellReference(columnIndex, i)));
	}
	return result;
}


TArray<FVector> UExcelWorksheet::VectorArrayAtColumn(int32 columnIndex) const
{
	TArray<FVector> result;
	for (int32 i = LowestRow(); i <= HighestRow(); ++i)
	{
		result.Add(ToVector(FExcelCellReference(columnIndex, i)));
	}
	return result;
}

TArray<FRotator> UExcelWorksheet::RotatorArrayAtColumn(int32 columnIndex) const
{
	TArray<FRotator> result;
	for (int32 i = LowestRow(); i <= HighestRow(); ++i)
	{
		result.Add(ToRotator(FExcelCellReference(columnIndex, i)));
	}
	return result;
}

TArray<FTransform> UExcelWorksheet::TransformArrayAtColumn(int32 columnIndex) const
{
	TArray<FTransform> result;
	for (int32 i = LowestRow(); i <= HighestRow(); ++i)
	{
		result.Add(ToTransform(FExcelCellReference(columnIndex, i)));
	}
	return result;
}

TArray<FColor> UExcelWorksheet::ColorArrayAtColumn(int32 columnIndex) const
{
	TArray<FColor> result;
	for (int32 i = LowestRow(); i <= HighestRow(); ++i)
	{
		result.Add(ToColor(FExcelCellReference(columnIndex, i)));
	}
	return result;
}

TArray<FMatrix> UExcelWorksheet::MatrixArrayAtColumn(int32 columnIndex) const
{
	TArray<FMatrix> result;
	for (int32 i = LowestRow(); i <= HighestRow(); ++i)
	{
		result.Add(ToMatrix(FExcelCellReference(columnIndex, i)));
	}
	return result;
}

TArray<bool> UExcelWorksheet::BoolArrayAtColumnString(FString columnString)const
{
	int32 index = UExcelCellReferenceLibrary::ToColumnIndex(columnString);
	return BoolArrayAtColumn(index);
}

TArray<int32> UExcelWorksheet::IntArrayAtColumnString(FString columnString)const
{
	int32 index = UExcelCellReferenceLibrary::ToColumnIndex(columnString);
	return IntArrayAtColumn(index);
}

TArray<float> UExcelWorksheet::FloatArrayAtColumnString(FString columnString)const
{
	int32 index = UExcelCellReferenceLibrary::ToColumnIndex(columnString);
	return FloatArrayAtColumn(index);
}

TArray<FString> UExcelWorksheet::StringArrayAtColumnString(FString columnString)const
{
	int32 index = UExcelCellReferenceLibrary::ToColumnIndex(columnString);
	return StringArrayAtColumn(index);
}

TArray<FDateTime> UExcelWorksheet::DateTimeArrayAtColumnString(FString columnString)const
{
	int32 index = UExcelCellReferenceLibrary::ToColumnIndex(columnString);
	return DateTimeArrayAtColumn(index);
}

TArray<FVector> UExcelWorksheet::VectorArrayAtColumnString(FString columnString) const
{
	int32 index = UExcelCellReferenceLibrary::ToColumnIndex(columnString);
	return VectorArrayAtColumn(index);
}

TArray<FRotator> UExcelWorksheet::RotatorArrayAtColumnString(FString columnString) const
{
	int32 index = UExcelCellReferenceLibrary::ToColumnIndex(columnString);
	return RotatorArrayAtColumn(index);
}

TArray<FTransform> UExcelWorksheet::TransformArrayAtColumnString(FString columnString) const
{
	int32 index = UExcelCellReferenceLibrary::ToColumnIndex(columnString);
	return TransformArrayAtColumn(index);
}

TArray<FColor> UExcelWorksheet::ColorArrayAtColumnString(FString columnString) const
{
	int32 index = UExcelCellReferenceLibrary::ToColumnIndex(columnString);
	return ColorArrayAtColumn(index);
}

TArray<FMatrix> UExcelWorksheet::MatrixArrayAtColumnString(FString columnString) const
{
	int32 index = UExcelCellReferenceLibrary::ToColumnIndex(columnString);
	return MatrixArrayAtColumn(index);
}

TArray<bool> UExcelWorksheet::BoolArrayAtRowSorted(int32 rowIndex, TArray<int32>& outColumnIndices, ExcelSortType sortType /*= ExcelSortType::None*/) const
{
	return Internal_ArrayAtRowSorted<bool>(rowIndex, outColumnIndices, sortType);
}

TArray<int32> UExcelWorksheet::IntArrayAtRowSorted(int32 rowIndex, TArray<int32>& outColumnIndices, ExcelSortType sortType /*= ExcelSortType::None*/) const
{
	return Internal_ArrayAtRowSorted<int32>(rowIndex, outColumnIndices, sortType);
}

TArray<float> UExcelWorksheet::FloatArrayAtRowSorted(int32 rowIndex, TArray<int32>& outColumnIndices, ExcelSortType sortType /*= ExcelSortType::None*/) const
{
	return Internal_ArrayAtRowSorted<float>(rowIndex, outColumnIndices, sortType);
}

TArray<FString> UExcelWorksheet::StringArrayAtRowSorted(int32 rowIndex, TArray<int32>& outColumnIndices, ExcelSortType sortType /*= ExcelSortType::None*/) const
{
	return Internal_ArrayAtRowSorted<FString>(rowIndex, outColumnIndices, sortType);
}

TArray<FDateTime> UExcelWorksheet::DateTimeArrayAtRowSorted(int32 rowIndex, TArray<int32>& outColumnIndices, ExcelSortType sortType /*= ExcelSortType::None*/) const
{
	return Internal_ArrayAtRowSorted<FDateTime>(rowIndex, outColumnIndices, sortType);
}

TArray<bool> UExcelWorksheet::BoolArrayAtColumnSorted(int32 columnIndex, TArray<int32>& outRowIndices, ExcelSortType sortType /*= ExcelSortType::None*/) const
{
	return Internal_ArrayAtColumnSorted<bool>(columnIndex, outRowIndices, sortType);
}

TArray<int32> UExcelWorksheet::IntArrayAtColumnSorted(int32 columnIndex, TArray<int32>& outRowIndices, ExcelSortType sortType /*= ExcelSortType::None*/) const
{
	return Internal_ArrayAtColumnSorted<int32>(columnIndex, outRowIndices, sortType);
}

TArray<float> UExcelWorksheet::FloatArrayAtColumnSorted(int32 columnIndex, TArray<int32>& outRowIndices, ExcelSortType sortType /*= ExcelSortType::None*/) const
{
	return Internal_ArrayAtColumnSorted<float>(columnIndex, outRowIndices, sortType);
}

TArray<FString> UExcelWorksheet::StringArrayAtColumnSorted(int32 columnIndex, TArray<int32>& outRowIndices, ExcelSortType sortType /*= ExcelSortType::None*/) const
{
	return Internal_ArrayAtColumnSorted<FString>(columnIndex, outRowIndices, sortType);
}

TArray<FDateTime> UExcelWorksheet::DateTimeArrayAtColumnSorted(int32 columnIndex, TArray<int32>& outRowIndices, ExcelSortType sortType /*= ExcelSortType::None*/) const
{
	return Internal_ArrayAtColumnSorted<FDateTime>(columnIndex, outRowIndices, sortType);
}

TArray<bool> UExcelWorksheet::BoolArrayAtColumnStringSorted(FString columnString, TArray<int32>& outRowIndices, ExcelSortType sortType /*= ExcelSortType::None*/) const
{
	return Internal_ArrayAtColumnStringSorted<bool>(columnString, outRowIndices, sortType);
}

TArray<int32> UExcelWorksheet::IntArrayAtColumnStringSorted(FString columnString, TArray<int32>& outRowIndices, ExcelSortType sortType /*= ExcelSortType::None*/) const
{
	return Internal_ArrayAtColumnStringSorted<int32>(columnString, outRowIndices, sortType);
}

TArray<float> UExcelWorksheet::FloatArrayAtColumnStringSorted(FString columnString, TArray<int32>& outRowIndices, ExcelSortType sortType /*= ExcelSortType::None*/) const
{
	return Internal_ArrayAtColumnStringSorted<float>(columnString, outRowIndices, sortType);
}

TArray<FString> UExcelWorksheet::StringArrayAtColumnStringSorted(FString columnString, TArray<int32>& outRowIndices, ExcelSortType sortType /*= ExcelSortType::None*/) const
{
	return Internal_ArrayAtColumnStringSorted<FString>(columnString, outRowIndices, sortType);
}

TArray<FDateTime> UExcelWorksheet::DateTimeArrayAtColumnStringSorted(FString columnString, TArray<int32>& outRowIndices, ExcelSortType sortType /*= ExcelSortType::None*/) const
{
	return Internal_ArrayAtColumnStringSorted<FDateTime>(columnString, outRowIndices, sortType);
}

void UExcelWorksheet::SetBool(bool value, FExcelCellReference cellReference)
{
	xlnt::cell_reference cr(cellReference.Column, cellReference.Row);
	mData.reserve(cellReference.Row);
	xlnt::cell c = mData.cell(cr);
	c.value(value);
}

void UExcelWorksheet::SetInt(int32 value, FExcelCellReference cellReference)
{
	xlnt::cell_reference cr(cellReference.Column, cellReference.Row);
	mData.reserve(cellReference.Row);
	xlnt::cell c = mData.cell(cr);
	c.value(value);
}

void UExcelWorksheet::SetFloat(float value, FExcelCellReference cellReference)
{
	xlnt::cell_reference cr(cellReference.Column, cellReference.Row);
	mData.reserve(cellReference.Row);
	xlnt::cell c = mData.cell(cr);
	c.value(value);
}

void UExcelWorksheet::SetString(FString value, FExcelCellReference cellReference)
{
	std::string str = TCHAR_TO_UTF8(*value);

	xlnt::cell_reference cr(cellReference.Column, cellReference.Row);
	mData.reserve(cellReference.Row);
	xlnt::cell c = mData.cell(cr);
	c.value(str);
}

void UExcelWorksheet::SetDateTime(FDateTime value, FExcelCellReference cellReference)
{
	xlnt::cell_reference cr(cellReference.Column, cellReference.Row);
	mData.reserve(cellReference.Row);
	xlnt::cell c = mData.cell(cr);
	xlnt::datetime d(value.GetYear(), value.GetMonth(), value.GetDay(), value.GetHour(), value.GetMinute(), value.GetSecond(), value.GetMillisecond());
	c.value(d);
}

void UExcelWorksheet::SetBoolAt(bool value, int32 column, int32 row)
{
	SetBool(value,FExcelCellReference(column, row));
}

void UExcelWorksheet::SetIntAt( int32 value, int32 column, int32 row)
{
	SetInt(value, FExcelCellReference(column, row));
}

void UExcelWorksheet::SetFloatAt(float value, int32 column, int32 row)
{
	SetFloat(value, FExcelCellReference(column, row));
}

void UExcelWorksheet::SetStringAt( FString value, int32 column, int32 row)
{
	SetString(value, FExcelCellReference(column, row));
}

void UExcelWorksheet::SetDateTimeAt( FDateTime value, int32 column, int32 row)
{
	SetDateTime(value, FExcelCellReference(column, row));
}

void UExcelWorksheet::SetBoolWithId(bool value, FString cellReferenceString)
{
	SetBool(value, UExcelCellReferenceLibrary::MakeCellReferenceWithString(cellReferenceString));
}

void UExcelWorksheet::SetIntWithId(int32 value, FString cellReferenceString)
{
	SetInt(value, UExcelCellReferenceLibrary::MakeCellReferenceWithString(cellReferenceString));
}

void UExcelWorksheet::SetFloatWithId( float value, FString cellReferenceString)
{
	SetFloat(value, UExcelCellReferenceLibrary::MakeCellReferenceWithString(cellReferenceString));
}

void UExcelWorksheet::SetStringWithId( FString value, FString cellReferenceString)
{
	SetString(value, UExcelCellReferenceLibrary::MakeCellReferenceWithString(cellReferenceString));
}

void UExcelWorksheet::SetDateTimeWithId(FDateTime value, FString cellReferenceString)
{
	SetDateTime(value, UExcelCellReferenceLibrary::MakeCellReferenceWithString(cellReferenceString));
}



UExcelCell* UExcelWorksheet::FindBoolAtColumn(int32 columnIndex, bool searchValue, int32 startRowIndex /*= 1*/) const
{
	return FindCellAtColumn(columnIndex, searchValue, startRowIndex);
}

UExcelCell* UExcelWorksheet::FindIntAtColumn(int32 columnIndex, int32 searchValue, int32 startRowIndex /*= 1*/) const
{
	return FindCellAtColumn(columnIndex, searchValue, startRowIndex);
}

UExcelCell* UExcelWorksheet::FindFloatAtColumn(int32 columnIndex, float searchValue, int32 startRowIndex /*= 1*/) const
{
	return FindCellAtColumn(columnIndex, searchValue, startRowIndex);
}

UExcelCell* UExcelWorksheet::FindStringAtColumn(int32 columnIndex, FString searchValue, int32 startRowIndex /*= 1*/) const
{
	return FindCellAtColumn(columnIndex, searchValue, startRowIndex);
}

UExcelCell* UExcelWorksheet::FindDateTimeAtColumn(int32 columnIndex, FDateTime searchValue, int32 startRowIndex /*= 1*/) const
{
	return FindCellAtColumn(columnIndex, searchValue, startRowIndex);
}

UExcelCell* UExcelWorksheet::FindBoolAtColumnString(FString columnString, bool searchValue, int32 startRowIndex /*= 1*/) const
{
	int32 columnIndex = UExcelCellReferenceLibrary::ToColumnIndex(columnString);
	return FindCellAtColumn(columnIndex, searchValue, startRowIndex);
}

UExcelCell* UExcelWorksheet::FindIntAtColumnString(FString columnString, int32 searchValue, int32 startRowIndex /*= 1*/) const
{
	int32 columnIndex = UExcelCellReferenceLibrary::ToColumnIndex(columnString);
	return FindCellAtColumn(columnIndex, searchValue, startRowIndex);
}

UExcelCell* UExcelWorksheet::FindFloatAtColumnString(FString columnString, float searchValue, int32 startRowIndex /*= 1*/) const
{
	int32 columnIndex = UExcelCellReferenceLibrary::ToColumnIndex(columnString);
	return FindCellAtColumn(columnIndex, searchValue, startRowIndex);
}

UExcelCell* UExcelWorksheet::FindStringAtColumnString(FString columnString, FString searchValue, int32 startRowIndex /*= 1*/) const
{
	int32 columnIndex = UExcelCellReferenceLibrary::ToColumnIndex(columnString);
	return FindCellAtColumn(columnIndex, searchValue);

}

UExcelCell* UExcelWorksheet::FindDateTimeAtColumnString(FString columnString, FDateTime searchValue, int32 startRowIndex /*= 1*/) const
{
	int32 columnIndex = UExcelCellReferenceLibrary::ToColumnIndex(columnString);
	return FindCellAtColumn(columnIndex, searchValue, startRowIndex);
}

UExcelCell* UExcelWorksheet::FindBoolAtRow(int32 rowIndex, bool searchValue, int32 startColumnIndex /*= 1*/) const
{
	return FindCellAtRow(rowIndex, searchValue, startColumnIndex);
}

UExcelCell* UExcelWorksheet::FindIntAtRow(int32 rowIndex, int32 searchValue, int32 startColumnIndex /*= 1*/) const
{
	return FindCellAtRow(rowIndex, searchValue, startColumnIndex);
}

UExcelCell* UExcelWorksheet::FindFloatAtRow(int32 rowIndex, float searchValue, int32 startColumnIndex /*= 1*/) const
{
	return FindCellAtRow(rowIndex, searchValue, startColumnIndex);
}

UExcelCell* UExcelWorksheet::FindStringAtRow(int32 rowIndex, FString searchValue, int32 startColumnIndex /*= 1*/) const
{
	return FindCellAtRow(rowIndex, searchValue, startColumnIndex);
}

UExcelCell* UExcelWorksheet::FindDateTimeAtRow(int32 rowIndex, FDateTime searchValue, int32 startColumnIndex /*= 1*/) const
{
	return FindCellAtRow(rowIndex, searchValue, startColumnIndex);
}

TArray<UExcelCell*> UExcelWorksheet::FindMultipleBoolAtColumn(int32 columnIndex, bool searchValue, int32 startRowIndex /*= 1*/) const
{
	return FindMultipleCellAtColumn(columnIndex, searchValue, startRowIndex);
}

TArray<UExcelCell*> UExcelWorksheet::FindMultipleIntAtColumn(int32 columnIndex, int32 searchValue, int32 startRowIndex /*= 1*/) const
{
	return FindMultipleCellAtColumn(columnIndex, searchValue, startRowIndex);
}

TArray<UExcelCell*> UExcelWorksheet::FindMultipleFloatAtColumn(int32 columnIndex, float searchValue, int32 startRowIndex /*= 1*/) const
{
	return FindMultipleCellAtColumn(columnIndex, searchValue, startRowIndex);
}

TArray<UExcelCell*> UExcelWorksheet::FindMultipleStringAtColumn(int32 columnIndex, FString searchValue, int32 startRowIndex /*= 1*/) const
{
	return FindMultipleCellAtColumn(columnIndex, searchValue, startRowIndex);
}

TArray<UExcelCell*> UExcelWorksheet::FindMultipleDateTimeAtColumn(int32 columnIndex, FDateTime searchValue, int32 startRowIndex /*= 1*/) const
{
	return FindMultipleCellAtColumn(columnIndex, searchValue, startRowIndex);
}

TArray<UExcelCell*> UExcelWorksheet::FindMultipleBoolAtColumnString(FString columnString, bool searchValue, int32 startRowIndex /*= 1*/) const
{
	int32 columnIndex = UExcelCellReferenceLibrary::ToColumnIndex(columnString);
	return FindMultipleCellAtColumn(columnIndex, searchValue, startRowIndex);
}

TArray<UExcelCell*> UExcelWorksheet::FindMultipleIntAtColumnString(FString columnString, int32 searchValue, int32 startRowIndex /*= 1*/) const
{
	int32 columnIndex = UExcelCellReferenceLibrary::ToColumnIndex(columnString);
	return FindMultipleCellAtColumn(columnIndex, searchValue, startRowIndex);
}

TArray<UExcelCell*> UExcelWorksheet::FindMultipleFloatAtColumnString(FString columnString, float searchValue, int32 startRowIndex /*= 1*/) const
{
	int32 columnIndex = UExcelCellReferenceLibrary::ToColumnIndex(columnString);
	return FindMultipleCellAtColumn(columnIndex, searchValue, startRowIndex);
}

TArray<UExcelCell*> UExcelWorksheet::FindMultipleStringAtColumnString(FString columnString, FString searchValue, int32 startRowIndex /*= 1*/) const
{
	int32 columnIndex = UExcelCellReferenceLibrary::ToColumnIndex(columnString);
	return FindMultipleCellAtColumn(columnIndex, searchValue, startRowIndex);
}

TArray<UExcelCell*> UExcelWorksheet::FindMultipleDateTimeAtColumnString(FString columnString, FDateTime searchValue, int32 startRowIndex /*= 1*/) const
{
	int32 columnIndex = UExcelCellReferenceLibrary::ToColumnIndex(columnString);
	return FindMultipleCellAtColumn(columnIndex, searchValue, startRowIndex);
}

TArray<UExcelCell*> UExcelWorksheet::FindMultipleBoolAtRow(int32 rowIndex, bool searchValue, int32 startColumnIndex /*= 1*/) const
{
	return FindMultipleCellAtRow(rowIndex, searchValue, startColumnIndex);
}

TArray<UExcelCell*> UExcelWorksheet::FindMultipleIntAtRow(int32 rowIndex, int32 searchValue, int32 startColumnIndex /*= 1*/) const
{
	return FindMultipleCellAtRow(rowIndex, searchValue, startColumnIndex);
}

TArray<UExcelCell*> UExcelWorksheet::FindMultipleFloatAtRow(int32 rowIndex, float searchValue, int32 startColumnIndex /*= 1*/) const
{
	return FindMultipleCellAtRow(rowIndex, searchValue, startColumnIndex);
}

TArray<UExcelCell*> UExcelWorksheet::FindMultipleStringAtRow(int32 rowIndex, FString searchValue, int32 startColumnIndex /*= 1*/) const
{
	return FindMultipleCellAtRow(rowIndex, searchValue, startColumnIndex);
}

TArray<UExcelCell*> UExcelWorksheet::FindMultipleDateTimeAtRow(int32 rowIndex, FDateTime searchValue, int32 startColumnIndex /*= 1*/) const
{
	return FindMultipleCellAtRow(rowIndex, searchValue, startColumnIndex);
}


UExcelCell* UExcelWorksheet::CreateCell(xlnt::cell c)const
{
	UExcelCell* cell = NewObject<UExcelCell>();
	cell->SetWorksheet((UExcelWorksheet*)this);
	cell->SetData(c);
	return cell;
}
