// Copyright 2018 Jianzhao Fu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ExcelTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "ExcelNumberFormat.generated.h"


/**
 * Excel cell font
 */
USTRUCT(BlueprintType, Category = "DirectExcel")
struct DIRECTEXCEL_API FExcelNumberFormat
{
	GENERATED_BODY()
public:
	FExcelNumberFormat() = default;
	FExcelNumberFormat(xlnt::number_format val) :
		Id(val.id()),
		FormatString(val.format_string().c_str())
	{

	}

	xlnt::number_format ToXlnt()const
	{
		std::string formatStr = TCHAR_TO_UTF8(*FormatString);

		xlnt::number_format val;
		val.id(Id);
		val.format(formatStr);

		return val;
	}
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DirectExcel|NumberFormat")
		int Id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DirectExcel|NumberFormat")
		FString FormatString;

};


/**
* Blueprint functions to Read/Write Excel cell's value
*/
UCLASS(DisplayName = "DirectExcel", Category = "DirectExcel")
class DIRECTEXCEL_API UExcelNumberFormatLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure, Category = "DirectExcel|NumberFormat")
		static FExcelNumberFormat MakeNumberFormat_General() { return FExcelNumberFormat(xlnt::number_format::general()); }
	UFUNCTION(BlueprintPure, Category = "DirectExcel|NumberFormat")
		static FExcelNumberFormat MakeNumberFormat_Text() { return FExcelNumberFormat(xlnt::number_format::text()); }
	UFUNCTION(BlueprintPure, Category = "DirectExcel|NumberFormat")
		static FExcelNumberFormat MakeNumberFormat_Number() { return FExcelNumberFormat(xlnt::number_format::number()); }
	UFUNCTION(BlueprintPure, Category = "DirectExcel|NumberFormat")
		static FExcelNumberFormat MakeNumberFormat_Number00() { return FExcelNumberFormat(xlnt::number_format::number_00()); }
	UFUNCTION(BlueprintPure, Category = "DirectExcel|NumberFormat")
		static FExcelNumberFormat MakeNumberFormat_NumberCommaSeparated1() { return FExcelNumberFormat(xlnt::number_format::number_comma_separated1()); }
	UFUNCTION(BlueprintPure, Category = "DirectExcel|NumberFormat")
		static FExcelNumberFormat MakeNumberFormat_Percentage() { return FExcelNumberFormat(xlnt::number_format::percentage()); }
	UFUNCTION(BlueprintPure, Category = "DirectExcel|NumberFormat")
		static FExcelNumberFormat MakeNumberFormat_Percentage00() { return FExcelNumberFormat(xlnt::number_format::percentage_00()); }

	UFUNCTION(BlueprintPure, Category = "DirectExcel|NumberFormat")
		static FExcelNumberFormat MakeNumberFormat_Date_yyyymmdd2() { return FExcelNumberFormat(xlnt::number_format::date_yyyymmdd2()); }
	UFUNCTION(BlueprintPure, Category = "DirectExcel|NumberFormat")
		static FExcelNumberFormat MakeNumberFormat_Date_yymmdd() { return FExcelNumberFormat(xlnt::number_format::date_yymmdd()); }
	UFUNCTION(BlueprintPure, Category = "DirectExcel|NumberFormat")
		static FExcelNumberFormat MakeNumberFormat_Date_ddmmyyyy() { return FExcelNumberFormat(xlnt::number_format::date_ddmmyyyy()); }
	UFUNCTION(BlueprintPure, Category = "DirectExcel|NumberFormat")
		static FExcelNumberFormat MakeNumberFormat_Date_dmyslash() { return FExcelNumberFormat(xlnt::number_format::date_dmyslash()); }
	UFUNCTION(BlueprintPure, Category = "DirectExcel|NumberFormat")
		static FExcelNumberFormat MakeNumberFormat_Date_dmyminus() { return FExcelNumberFormat(xlnt::number_format::date_dmyminus()); }
	UFUNCTION(BlueprintPure, Category = "DirectExcel|NumberFormat")
		static FExcelNumberFormat MakeNumberFormat_Date_dmminus() { return FExcelNumberFormat(xlnt::number_format::date_dmminus()); }
	UFUNCTION(BlueprintPure, Category = "DirectExcel|NumberFormat")
		static FExcelNumberFormat MakeNumberFormat_Date_myminus() { return FExcelNumberFormat(xlnt::number_format::date_myminus()); }
	UFUNCTION(BlueprintPure, Category = "DirectExcel|NumberFormat")
		static FExcelNumberFormat MakeNumberFormat_Date_xlsx14() { return FExcelNumberFormat(xlnt::number_format::date_xlsx14()); }
	UFUNCTION(BlueprintPure, Category = "DirectExcel|NumberFormat")
		static FExcelNumberFormat MakeNumberFormat_Date_xlsx15() { return FExcelNumberFormat(xlnt::number_format::date_xlsx15()); }
	UFUNCTION(BlueprintPure, Category = "DirectExcel|NumberFormat")
		static FExcelNumberFormat MakeNumberFormat_Date_xlsx16() { return FExcelNumberFormat(xlnt::number_format::date_xlsx16()); }
	UFUNCTION(BlueprintPure, Category = "DirectExcel|NumberFormat")
		static FExcelNumberFormat MakeNumberFormat_Date_xlsx17() { return FExcelNumberFormat(xlnt::number_format::date_xlsx17()); }
	UFUNCTION(BlueprintPure, Category = "DirectExcel|NumberFormat")
		static FExcelNumberFormat MakeNumberFormat_Date_xlsx22() { return FExcelNumberFormat(xlnt::number_format::date_xlsx22()); }
	UFUNCTION(BlueprintPure, Category = "DirectExcel|NumberFormat")
		static FExcelNumberFormat MakeNumberFormat_Date_datetime() { return FExcelNumberFormat(xlnt::number_format::date_datetime()); }
	UFUNCTION(BlueprintPure, Category = "DirectExcel|NumberFormat")
		static FExcelNumberFormat MakeNumberFormat_Date_time1() { return FExcelNumberFormat(xlnt::number_format::date_time1()); }
	UFUNCTION(BlueprintPure, Category = "DirectExcel|NumberFormat")
		static FExcelNumberFormat MakeNumberFormat_Date_time2() { return FExcelNumberFormat(xlnt::number_format::date_time2()); }
	UFUNCTION(BlueprintPure, Category = "DirectExcel|NumberFormat")
		static FExcelNumberFormat MakeNumberFormat_Date_time3() { return FExcelNumberFormat(xlnt::number_format::date_time3()); }
	UFUNCTION(BlueprintPure, Category = "DirectExcel|NumberFormat")
		static FExcelNumberFormat MakeNumberFormat_Date_time4() { return FExcelNumberFormat(xlnt::number_format::date_time4()); }
	UFUNCTION(BlueprintPure, Category = "DirectExcel|NumberFormat")
		static FExcelNumberFormat MakeNumberFormat_Date_time5() { return FExcelNumberFormat(xlnt::number_format::date_time5()); }
	UFUNCTION(BlueprintPure, Category = "DirectExcel|NumberFormat")
		static FExcelNumberFormat MakeNumberFormat_Date_time6() { return FExcelNumberFormat(xlnt::number_format::date_time6()); }

};

