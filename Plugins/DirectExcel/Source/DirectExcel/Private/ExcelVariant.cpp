// Copyright 2018 Jianzhao Fu. All Rights Reserved.
#include "ExcelVariant.h"
#include "ThirdParty.h"

FExcelVariant::FExcelVariant(TArray<bool> value)
	:mType(ExcelVariantType::Array)
{
	for (bool v:value)
	{
		mArrayValue.Emplace(v);
	}
}


FExcelVariant::FExcelVariant(TArray<int32> value)
	:mType(ExcelVariantType::Array)
{
	for (int32 v : value)
	{
		mArrayValue.Emplace(v);
	}
}


FExcelVariant::FExcelVariant(TArray<FString> value)
	:mType(ExcelVariantType::Array)
{
	for (FString v : value)
	{
		mArrayValue.Emplace(v);
	}
}


FExcelVariant::FExcelVariant(TArray<FDateTime> value)
	:mType(ExcelVariantType::Array)
{
	for (FDateTime v : value)
	{
		mArrayValue.Emplace(v);
	}
}
