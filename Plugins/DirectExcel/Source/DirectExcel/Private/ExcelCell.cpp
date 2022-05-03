// Copyright 2018 Jianzhao Fu. All Rights Reserved.
#include "ExcelCell.h"
#include "ThirdParty.h"
#include "ExcelWorkbook.h"
#include "ExcelWorksheet.h"
#include "DataTableUtils.h"



UExcelWorksheet* UExcelCell::ParentSheet() const
{
	return mWorksheet.Get();
}

void UExcelCell::SetWorksheet(UExcelWorksheet* worksheet)
{
	mWorksheet = worksheet;
}

ExcelCellType UExcelCell::Type()const
{
	return (ExcelCellType)mData.data_type();
}

bool UExcelCell::HasValue()const
{
	return mData.has_value();
}

void UExcelCell::Clear()
{
	mData.clear_value();
}


FExcelCellReference UExcelCell::Reference()const
{
	return FExcelCellReference((int32)mData.column().index, (int32)mData.row());
}

int32 UExcelCell::Column()const
{
	return (int32)mData.column().index;
}

int32 UExcelCell::Row()const
{
	return (int32)mData.row();
}

bool UExcelCell::IsMerged()const
{
	return mData.is_merged();
}

UExcelCell* UExcelCell::MakeCellWithOffset(int32 columnOffset, int32 rowOffset)
{
	xlnt::cell c2 = mData.offset(columnOffset, rowOffset);
	UExcelCell* cell = NewObject<UExcelCell>();
	cell->SetWorksheet(ParentSheet());
	cell->SetData(c2);
	return cell;
}

int32 UExcelCell::Compare(UExcelCell* other) const
{
	if (HasValue())
	{
		if (other->HasValue())
		{
			if (Type() != other->Type())
			{
				return -2;
			}

			switch (Type())
			{
			case ExcelCellType::Boolean:
				return (int32)ToBool() - (int32)other->ToBool();
			case ExcelCellType::Date:
				return (int32)(ToDateTime() - other->ToDateTime()).GetTotalMilliseconds();
			case ExcelCellType::InlineString:
			case ExcelCellType::SharedString:
			case ExcelCellType::FormulaString:
				return ToString().Compare(other->ToString());
			case ExcelCellType::Number:
				return (int32)(ToFloat() - other->ToFloat());
			default:
				break;
			}
		}
		return 1;
	}
	else
	{
		if (other->HasValue())
		{
			return -1;
		}
		return 0;
	}
}

bool UExcelCell::ToBool()const
{
	return mData.value<bool>();
}

int32 UExcelCell::ToInt()const
{
	return mData.value<int32>();
}

float UExcelCell::ToFloat()const
{
	return mData.value<float>();
}

FString UExcelCell::ToString()const
{
	std::string str = mData.to_string();
	return UTF8_TO_TCHAR(str.c_str());
}
FDateTime UExcelCell::ToDateTime()const
{
	xlnt::datetime date = mData.value<xlnt::datetime>();
	return FDateTime(date.year, date.month, date.day, date.hour, date.minute, date.second, date.microsecond);
}

UObject* UExcelCell::ToUObject() const
{
	FString path = ToString();
	return LoadObject<UObject>(nullptr, *path);
}

void UExcelCell::SetBool(bool value)
{
	mData.value(value);
}

void UExcelCell::SetInt(int32 value)
{
	mData.value(value);
}

void UExcelCell::SetFloat(float value)
{
	mData.value(value);
}

void UExcelCell::SetString(FString value)
{
	std::string str = TCHAR_TO_UTF8(*value);
	mData.value(str);
}

void UExcelCell::SetDateTime(FDateTime value)
{
	xlnt::datetime d(value.GetYear(), value.GetMonth(), value.GetDay(), value.GetHour(), value.GetMinute(), value.GetSecond(), value.GetMillisecond());
	mData.value(d);
}

void UExcelCell::SetObject(const UObject* value)
{
	if (value)
	{
		FString path = value->GetFullName();
		SetString(path);
	}
}

bool UExcelCell::HasHyperlink()const
{
	return mData.has_hyperlink();
}

FString UExcelCell::GetHyperlink()const
{
	return FString(mData.hyperlink().url().c_str());
}

void UExcelCell::SetHyperlink(FString url, FString display/*=TEXT("")*/)
{
	std::string str = TCHAR_TO_UTF8(*url);
	std::string str2 = TCHAR_TO_UTF8(*display);
	mData.hyperlink(str, str2);
}



bool UExcelCell::HasFormula()const
{
	return mData.has_formula();
}

FString UExcelCell::GetFormula()const
{
	return FString(mData.formula().c_str());
}

void UExcelCell::SetFormula(FString formula)
{
	std::string str = TCHAR_TO_UTF8(*formula);
	mData.formula(str);
}

void UExcelCell::ClearFormula()
{
	mData.clear_formula();
}

bool UExcelCell::HasComment()const
{
	return mData.has_comment();
}

void UExcelCell::GetComment(FString& Text, FString& Author)const
{
	auto c1 = mData.comment();
	Text = c1.plain_text().c_str();
	Author = c1.author().c_str();
}

void UExcelCell::SetComment(FString text, FString author)
{
	std::string str = TCHAR_TO_UTF8(*text);
	std::string str2 = TCHAR_TO_UTF8(*author);
	mData.comment(str, str2);
}

void UExcelCell::ClearComment()
{
	mData.clear_comment();
}




bool UExcelCell::HasFormat()const
{
	return mData.has_format();
}

UExcelFormat* UExcelCell::GetFormat()const
{
	UExcelWorkbook* workbook = ParentSheet()->ParentWorkbook();
	xlnt::cell* mutableData = (xlnt::cell*)&mData;

	if (!mData.has_format())
	{
		xlnt::format newFormat = workbook->Data()->create_format();
		mutableData->format(newFormat);
	}
	UExcelFormat* result = NewObject<UExcelFormat>();
	result->SetCell((UExcelCell*)this);
	result->SetData(mData.format());
	return result;
}

void UExcelCell::SetFormat(UExcelFormat* format)
{
	mData.format(format->Data());
}

void UExcelCell::ClearFormat()
{
	mData.clear_format();
}

