// Copyright 2018 Jianzhao Fu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ExcelTypes.h"
#include "ExcelColor.h"

#include "ExcelFont.generated.h"


/**
 * Excel cell font
 */
USTRUCT(BlueprintType, Category = "DirectExcel")
struct DIRECTEXCEL_API FExcelFont
{
	GENERATED_BODY()
public:
	FExcelFont();
	
	FExcelFont(xlnt::font val) :
		IsBold(val.bold()),
		IsItalic(val.italic()),
		IsSuperScript(val.subscript()),
		IsStrikeThrough(val.strikethrough()),
		IsOutline(val.outline()),
		IsShadow(val.shadow()),
		UnderlineType((ExcelUnderlineType)val.underline()),
		Family(val.has_family() ? val.family() : 0),
		Charset(val.has_charset() ? val.charset() : 0)
	{
		if (val.has_color())
		{
			Color = FExcelColor(val.color());
		}

		if (val.has_scheme())
		{
			Scheme = val.scheme().c_str();
		}
		if (val.has_size())
		{
			Size = val.size();
		}

		if (val.has_name())
		{
			Name = val.name().c_str();
		}
	}

	xlnt::font ToXlnt()const
	{
		std::string nameStr = TCHAR_TO_UTF8(*Name);
		std::string schemeStr = TCHAR_TO_UTF8(*Scheme);

		xlnt::font val;
		val.name(nameStr);
		val.size(Size);
		val.bold(IsBold);
		val.italic(IsItalic);
		val.superscript(IsSuperScript);
		val.strikethrough(IsStrikeThrough);
		val.outline(IsOutline);
		val.shadow(IsShadow);
		val.underline((xlnt::font::underline_style)UnderlineType);
		val.color(Color.ToXlnt());
		val.family(Family);
		val.charset(Charset);
		val.scheme(schemeStr);

		return val;
	}
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DirectExcel|Font")
		FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DirectExcel|Font")
		float Size=0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DirectExcel|Font")
		bool IsBold = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DirectExcel|Font")
		bool IsItalic = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DirectExcel|Font")
		bool IsSuperScript = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DirectExcel|Font")
		bool IsStrikeThrough = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DirectExcel|Font")
		bool IsOutline = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DirectExcel|Font")
		bool IsShadow = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DirectExcel|Font")
		ExcelUnderlineType UnderlineType=ExcelUnderlineType::None;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DirectExcel|Font")
		FExcelColor Color;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DirectExcel|Font")
		int Family=0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DirectExcel|Font")
		int Charset=0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DirectExcel|Font")
		FString Scheme;
};
