// Copyright 2018 Jianzhao Fu. All Rights Reserved.
#include "DirectExcelLibrary.h"
#include "ExcelWorkbook.h"
#include "Misc/Paths.h"
#include "LogTypes.h"
#include "DataTableUtils.h"
#include "ExcelWorksheetDataTable.h"
#include "Engine/Engine.h"

FString UDirectExcelLibrary::ToAbsolutePath(FString projectReleativePath, ExcelFileRelateiveDir relativeDir /*= ExcelFileRelateiveDir::Absolute*/)
{
	FString baseDir;
	switch (relativeDir)
	{
	case ExcelFileRelateiveDir::ProjectDir:
		baseDir = FPaths::ProjectDir();
		break;
	case ExcelFileRelateiveDir::ProjectContentDir:
		baseDir = FPaths::ProjectContentDir();
		break;
	case ExcelFileRelateiveDir::ProjectConfigDir:
		baseDir = FPaths::ProjectConfigDir();
		break;
	case ExcelFileRelateiveDir::ProjectPluginsDir:
		baseDir = FPaths::ProjectPluginsDir();
		break;
	case ExcelFileRelateiveDir::ProjectSavedDir:
		baseDir = FPaths::ProjectSavedDir();
		break;
	case ExcelFileRelateiveDir::ProjectModsDir:
		baseDir = FPaths::ProjectModsDir();
		break;
	default:
		break;
	}
	FString fullPath = FPaths::Combine(baseDir, projectReleativePath);
	fullPath = FPaths::ConvertRelativePathToFull(fullPath);
	return fullPath;
}

bool UDirectExcelLibrary::DoesExcelFileExists(FString path, ExcelFileRelateiveDir relativeDir /*= ExcelFileRelateiveDir::Absolute*/)
{
	path = ToAbsolutePath(path, relativeDir);
	return FPaths::FileExists(path);
}

UExcelWorkbook* UDirectExcelLibrary::LoadExcel(FString path, ExcelFileRelateiveDir relativeDir /*= ExcelFileRelateiveDir::Absolute*/)
{
	UExcelWorkbook* wb = NewObject<UExcelWorkbook>();
	if (wb->Load(path, relativeDir))
	{
		return wb;
	}
	return nullptr;
}

bool UDirectExcelLibrary::SaveExcel(UExcelWorkbook* workbook, FString path, ExcelFileRelateiveDir relativeDir /*= ExcelFileRelateiveDir::ProjectSavedDir*/)
{
	return workbook->SaveAs(path, relativeDir);
}

UExcelWorkbook* UDirectExcelLibrary::CreateExcel()
{
	return NewObject<UExcelWorkbook>();
}

//static bool ReadStructAtRowIndex(UExcelWorksheet* sheet, int32 row, FTableRowBase& OutItem) { return false; }

DEFINE_FUNCTION(UDirectExcelLibrary::execReadStructAtRowIndex)
{
	P_GET_OBJECT(UExcelWorksheet, sheet);
	P_GET_PROPERTY(FIntProperty, row);

	bool result;
	Stack.MostRecentPropertyAddress = nullptr;
	Stack.StepCompiledIn<FStructProperty>(nullptr);

	void* OutItemDataPtr = Stack.MostRecentPropertyAddress;
	FStructProperty* OutItemProp = CastField<FStructProperty>(Stack.MostRecentProperty);
	UScriptStruct* OutputStruct = OutItemProp->Struct;
	P_FINISH;
	P_NATIVE_BEGIN;

	result = ReadStructAtRowIndexRaw(sheet, row, OutputStruct, OutItemDataPtr);

	P_NATIVE_END;
	*(bool*)RESULT_PARAM = result;
}

bool UDirectExcelLibrary::ReadStructAtRowIndexRaw(const UExcelWorksheet* sheet, int32 row, const UScriptStruct* OutStruct, void* outStructData)
{
	OutStruct->InitializeStruct(outStructData);

	for (TFieldIterator<const FProperty> i(OutStruct, EFieldIteratorFlags::IncludeSuper); i; ++i)
	{
		const FProperty* property = *i;
		FName columnName = GetPropertyColumnName(*property);
		int32 column = sheet->FindColumnIndex(columnName);
		if (column <= 0)
		{
			UE_LOG(LogDirectExcel,Error, TEXT("Cannot find column: '%s' on row '%d'"), *columnName.ToString(), row);
			return false;
		}

		FString val = sheet->ToString(FExcelCellReference(column, row));

		FString ErrorStr = DataTableUtils::AssignStringToProperty(val, property, (uint8*)outStructData);

		// If we failed, output a problem string
		if (ErrorStr.Len() > 0)
		{
			UE_LOG(LogDirectExcel,Error, TEXT("Problem assigning string '%s' to property '%s' on row '%d' : %s"), *val, *columnName.ToString(), row, *ErrorStr);
			return false;
		}

		++column;
	}

	return true;
}

DEFINE_FUNCTION(UDirectExcelLibrary::execReadStructWithRowName)
{
	P_GET_OBJECT(UExcelWorksheet, sheet);
	P_GET_PROPERTY(FNameProperty, rowName);

	bool result;
	Stack.MostRecentPropertyAddress = nullptr;
	Stack.StepCompiledIn<FStructProperty>(nullptr);

	void* OutItemDataPtr = Stack.MostRecentPropertyAddress;
	FStructProperty* OutItemProp = CastField<FStructProperty>(Stack.MostRecentProperty);
	UScriptStruct* OutputStruct = OutItemProp->Struct;
	P_FINISH;
	P_NATIVE_BEGIN;

	result = ReadStructWithRowNameRaw(sheet, rowName, OutputStruct, OutItemDataPtr);

	P_NATIVE_END;
	*(bool*)RESULT_PARAM = result;
}

bool UDirectExcelLibrary::ReadStructWithRowNameRaw(UExcelWorksheet* sheet, FName rowName, const UScriptStruct* OutStruct, void* outStructData)
{
	if (sheet->GetRowStructInUsed()!=OutStruct)
	{
		UE_LOG(LogDirectExcel, Error, TEXT("Cannot read row by '%s', need to set table header at first"), *rowName.ToString());
		return false;
	}

	UExcelWorksheetDataTable* dataTable = sheet->GetOrCreateDataTable();
	if (dataTable)
	{
		uint8* findData = dataTable->FindRowUnchecked(rowName);
		if (findData)
		{
			OutStruct->InitializeStruct(outStructData);
			OutStruct->CopyScriptStruct(outStructData, findData);
			return true;
		}
	}

	return false;
}

FName UDirectExcelLibrary::GetPropertyColumnName(const FProperty& property)
{
	//Name_2_C861169D466517CB284898BC1B15C476

	FString str= property.GetName();
	int32 outIndex=-1;
	if (str.FindChar('_', outIndex))
	{
		str=str.Left(outIndex);
	}

	return FName(*str);
}

DEFINE_FUNCTION(UDirectExcelLibrary::execReadItemAtCell)
{
	P_GET_OBJECT(UExcelWorksheet, sheet);
	P_GET_STRUCT(FExcelCellReference, cellReference);

	bool result = true;
	Stack.MostRecentPropertyAddress = nullptr;
	Stack.StepCompiledIn<FStructProperty>(nullptr);

	void* OutItemDataPtr = Stack.MostRecentPropertyAddress;
	FString str = sheet->ToString(cellReference);
	FStringOutputDevice ImportError;

	Stack.MostRecentProperty->ImportText(*str, OutItemDataPtr, PPF_ExternalEditor, nullptr, &ImportError);
	if (ImportError.Len() > 0)
	{
		UE_LOG(LogDirectExcel, Error, TEXT("Problem read struct:%s at row:%d column:%d : %s"), *Stack.MostRecentProperty->GetName(), cellReference.Row, cellReference.Column, *ImportError);
		result = false;
	}

	P_FINISH;
	P_NATIVE_BEGIN;


	P_NATIVE_END;
	*(bool*)RESULT_PARAM = result;
}

