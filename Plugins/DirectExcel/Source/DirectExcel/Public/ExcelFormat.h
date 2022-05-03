// Copyright 2018 Jianzhao Fu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ExcelTypes.h"
#include "ExcelAlignment.h"
#include "ExcelBorder.h"
#include "ExcelFill.h"
#include "ExcelFont.h"
#include "ExcelNumberFormat.h"
#include "ExcelProtection.h"

#include "Kismet/BlueprintFunctionLibrary.h"

#include "ExcelFormat.generated.h"

class UExcelCell;


/**
 * Excel cell format
 */
UCLASS(Blueprintable, BlueprintType, Category = "DirectExcel")
class DIRECTEXCEL_API UExcelFormat :public UObject
{
	GENERATED_BODY()
public:
	xlnt::format Data()const { return mData; }
	void SetData(xlnt::format data) { mData = data; }

public:
	void SetCell(UExcelCell* cell);
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Format")
		UExcelCell* ParentCell()const;

public:
public://alignment
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Format")
		FExcelAlignment GetAlignment()const;

	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Format")
		void SetAlignment(FExcelAlignment alignment);

	UFUNCTION(BlueprintPure, Category = "DirectExcel|Format")
		bool IsAlignmentApplied()const;

public://border
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Format")
		FExcelBorder GetBorder()const;

	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Format")
		void SetBorder(FExcelBorder border);

	UFUNCTION(BlueprintPure, Category = "DirectExcel|Format")
		bool IsBorderApplied()const;
public://fill
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Format")
		FExcelFill GetFill()const;

	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Format")
		void SetFill(FExcelFill fill);

	UFUNCTION(BlueprintPure, Category = "DirectExcel|Format")
		bool IsFillApplied()const;

public://font
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Format")
		FExcelFont GetFont()const;

	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Format")
		void SetFont(FExcelFont font);

	UFUNCTION(BlueprintPure, Category = "DirectExcel|Format")
		bool IsFontApplied()const;
public://number format
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Format")
		FExcelNumberFormat GetNumberFormat()const;

	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Format")
		void SetNumberFormat(FExcelNumberFormat numberFormat);

	UFUNCTION(BlueprintPure, Category = "DirectExcel|Format")
		bool IsNumberFormatApplied()const;
public://protection
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Format")
		FExcelProtection GetProtection()const;

	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Format")
		void SetProtection(FExcelProtection protection);

	UFUNCTION(BlueprintPure, Category = "DirectExcel|Format")
		bool IsProtectionApplied()const;
public://pivot_button
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Format")
		bool IsPivotTable()const;

	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Format")
		void EnablePivotTable(bool value);

	UFUNCTION(BlueprintPure, Category = "DirectExcel|Format")
		bool IsQuotePrefix()const;

	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Format")
		void EnableQuotePrefix(bool value);
private:
	TWeakObjectPtr<UExcelCell> mCell;
	xlnt::format mData;
};

