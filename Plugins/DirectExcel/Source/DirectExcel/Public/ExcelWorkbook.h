// Copyright 2018 Jianzhao Fu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ExcelTypes.h"
#include "ExcelWorksheet.h"
#include "ExcelVariant.h"
#include <vector>
#include "Engine/EngineTypes.h"

#include "ExcelWorkbook.generated.h"

class UExcelWorkbook;

DECLARE_DYNAMIC_DELEGATE_OneParam(FExcelWorkbookChangedDelegate, UExcelWorkbook*, workbook);
DECLARE_DYNAMIC_DELEGATE_OneParam(FExcelCellApplyDelegate, UExcelCell*, cell);

/**
*Excel document object
*/
UCLASS(Blueprintable, BlueprintType, Category = "DirectExcel")
class DIRECTEXCEL_API UExcelWorkbook : public UObject
{
	GENERATED_BODY()
public:
	UExcelWorkbook();
	virtual ~UExcelWorkbook();
public:	//Workbook
	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Workbook")
		bool Load(FString path, ExcelFileRelateiveDir relativeDir = ExcelFileRelateiveDir::Absolute);
	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Workbook")
		bool Save();
	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Workbook")
		bool SaveAs(FString path, ExcelFileRelateiveDir relativeDir = ExcelFileRelateiveDir::Absolute);
	bool Load(const std::vector<std::uint8_t>& data);
	bool Save(std::vector<std::uint8_t>& outData);
	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Workbook")
		void Clear();

	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Workbook")
		bool Reload();

	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Workbook")
	void SetTableHeaders(const TMap<FName, FExcelTableHeader>& tableHeaders);

public:
	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Workbook", meta = (WorldContext = "WorldContextObject"))
		bool StartWatch(const UObject* WorldContextObject, FExcelWorkbookChangedDelegate onChanged, bool autoReload = true, float rate = 1.f);

	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Workbook", meta = (WorldContext = "WorldContextObject"))
		bool StopWatch(const UObject* WorldContextObject);
public:	//Worksheet
	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Workbook")
		UExcelWorksheet* CreateSheet(int32 index = -1, bool setActive = true);

	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Workbook")
		UExcelWorksheet* CopySheet(UExcelWorksheet* sheet, int32 index = -1);

	UFUNCTION(BlueprintPure, Category = "DirectExcel|Workbook")
		const TArray<UExcelWorksheet*>& AllSheets()const;

	UFUNCTION(BlueprintPure, Category = "DirectExcel|Workbook")
		int32 SheetCount()const;

	UFUNCTION(BlueprintPure, Category = "DirectExcel|Workbook")
		UExcelWorksheet* ActiveSheet();

	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Workbook")
		void SetActiveSheet(UExcelWorksheet* sheet);
	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Workbook")
		void SetActiveSheetAt(int32 index);
	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Workbook")
		void SetActiveSheetWithTitle(FString title);

	UFUNCTION(BlueprintPure, Category = "DirectExcel|Workbook")
		TArray<FString> SheetTitles()const;

	UFUNCTION(BlueprintPure, Category = "DirectExcel|Workbook")
		UExcelWorksheet* SheetAt(int32 index);

	UFUNCTION(BlueprintPure, Category = "DirectExcel|Workbook")
		UExcelWorksheet* SheetWithId(int32 id);

	UFUNCTION(BlueprintPure, Category = "DirectExcel|Workbook")
		UExcelWorksheet* SheetWithTitle(FString title);

	UFUNCTION(BlueprintPure, Category = "DirectExcel|Workbook")
		bool ContainsSheet(FString title)const;

	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Workbook")
		void RemoveSheet(UExcelWorksheet* sheet);

public://property
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Workbook")
		bool HasCoreProperty(ExcelCoreProperty property)const;

	UFUNCTION(BlueprintPure, Category = "DirectExcel|Workbook")
		TArray<ExcelCoreProperty> AllCoreProperties()const;

	UFUNCTION(BlueprintPure, Category = "DirectExcel|Workbook")
		FExcelVariant GetCorePropertyValue(ExcelCoreProperty property)const;

	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Workbook")
		void SetCorePropertyValue(ExcelCoreProperty property, FExcelVariant value);

	//extended
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Workbook")
		bool HasExtendedProperty(ExcelExtendedProperty property)const;

	UFUNCTION(BlueprintPure, Category = "DirectExcel|Workbook")
		TArray<ExcelExtendedProperty> AllExtendedProperties()const;

	UFUNCTION(BlueprintPure, Category = "DirectExcel|Workbook")
		FExcelVariant GetExtendedPropertyValue(ExcelExtendedProperty property)const;

	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Workbook")
		void SetExtendedPropertyValue(ExcelExtendedProperty property, FExcelVariant value);

	//custom
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Workbook")
		bool HasCustomProperty(FString name)const;

	UFUNCTION(BlueprintPure, Category = "DirectExcel|Workbook")
		TArray<FString> AllCustomProperties()const;

	UFUNCTION(BlueprintPure, Category = "DirectExcel|Workbook")
		FExcelVariant GetCustomProperty(FString name)const;

	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Workbook")
		void SetCustomProperty(FString name, FExcelVariant value);
public:
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Workbook")
		ExcelBaseDate BaseDate()const;

	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Workbook")
		void SetBaseDate(ExcelBaseDate baseDate);

	UFUNCTION(BlueprintPure, Category = "DirectExcel|Workbook")
		bool HasTitle()const;
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Workbook")
		FString Title()const;
	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Workbook")
		void SetTitle(FString value);

public:	//cell
	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Workbook", meta = (WorldContext = "WorldContextObject"))
		void ApplyToCells(const UObject* WorldContextObject, FExcelCellApplyDelegate onCell);

	xlnt::workbook* Data() const { return mData; }
	void SetData(xlnt::workbook* val) { mData = val; }
private:
	void InitSheets();
	void Unload();
private:
	UPROPERTY(Transient)
		TArray<UExcelWorksheet*> mSheets;

	UPROPERTY(Transient)
		UExcelWorksheet* mActiveSheet;
private:

	FString mPath;
	xlnt::workbook* mData = nullptr;

	FTimerHandle mWatchTimer;
	void* mFindHandle = nullptr;
};
