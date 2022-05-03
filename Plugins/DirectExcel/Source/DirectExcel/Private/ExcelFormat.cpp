// Copyright 2018 Jianzhao Fu. All Rights Reserved.
#include "ExcelFormat.h"
#include "ExcelCell.h"
#include "ThirdParty.h"

void UExcelFormat::SetCell(UExcelCell* cell)
{
	mCell = cell;
}

UExcelCell* UExcelFormat::ParentCell() const
{
	return mCell.Get();
}

FExcelAlignment UExcelFormat::GetAlignment()const
{
	return FExcelAlignment(mData.alignment());
}

void UExcelFormat::SetAlignment(FExcelAlignment alignment)
{
	mData = mData.alignment(alignment.ToXlnt());
	ParentCell()->SetFormat(this);
}

bool UExcelFormat::IsAlignmentApplied()const
{
	return mData.alignment_applied();
}

FExcelBorder UExcelFormat::GetBorder()const
{
	return FExcelBorder(mData.border());
}

void UExcelFormat::SetBorder(FExcelBorder border)
{
	mData = mData.border(border.ToXlnt());
	ParentCell()->SetFormat(this);
}

bool UExcelFormat::IsBorderApplied()const
{
	return mData.border_applied();
}

FExcelFill UExcelFormat::GetFill()const
{
	return FExcelFill(mData.fill());
}

void UExcelFormat::SetFill(FExcelFill fill)
{
	mData = mData.fill(fill.ToXlnt());
	ParentCell()->SetFormat(this);
}

bool UExcelFormat::IsFillApplied()const
{
	return mData.fill_applied();
}

FExcelFont UExcelFormat::GetFont()const
{
	return FExcelFont(mData.font());
}

void UExcelFormat::SetFont(FExcelFont font)
{
	mData=mData.font(font.ToXlnt());
	ParentCell()->SetFormat(this);
}

bool UExcelFormat::IsFontApplied()const
{
	return mData.font_applied();
}

FExcelNumberFormat UExcelFormat::GetNumberFormat()const
{
	return FExcelNumberFormat(mData.number_format());
}

void UExcelFormat::SetNumberFormat(FExcelNumberFormat numberFormat)
{
	mData = mData.number_format(numberFormat.ToXlnt());
	ParentCell()->SetFormat(this);
}

bool UExcelFormat::IsNumberFormatApplied()const
{
	return mData.number_format_applied();
}

FExcelProtection UExcelFormat::GetProtection()const
{
	return FExcelProtection(mData.protection());
}

void UExcelFormat::SetProtection(FExcelProtection protection)
{
	mData = mData.protection(protection.ToXlnt());
	ParentCell()->SetFormat(this);
}

bool UExcelFormat::IsProtectionApplied()const
{
	return mData.protection_applied();
}

bool UExcelFormat::IsPivotTable()const
{
	return mData.pivot_button();
}

void UExcelFormat::EnablePivotTable(bool value)
{
	 mData.pivot_button(value);
	 ParentCell()->SetFormat(this);
}

bool UExcelFormat::IsQuotePrefix()const
{
	return mData.quote_prefix();
}

void UExcelFormat::EnableQuotePrefix(bool value)
{
	mData.quote_prefix(value);
	ParentCell()->SetFormat(this);
}

