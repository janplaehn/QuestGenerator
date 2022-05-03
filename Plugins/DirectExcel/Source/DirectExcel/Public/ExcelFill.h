// Copyright 2018 Jianzhao Fu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ExcelTypes.h"
#include "ExcelColor.h"
#include "Layout/Margin.h"
#include "ExcelFill.generated.h"


/**
 * Excel cell pattern fill
 */
USTRUCT(BlueprintType, Category = "DirectExcel")
struct DIRECTEXCEL_API FExcelPatternFill
{
	GENERATED_BODY()
public:
	FExcelPatternFill() = default;

	FExcelPatternFill(xlnt::pattern_fill val) :
		Type((ExcelPatternFillType)val.type())
	{
		if (val.foreground().is_set())
		{
			ForegroundColor = FExcelColor(val.foreground().get());
		}
		if (val.background().is_set())
		{
			BackgroundColor = FExcelColor(val.background().get());
		}
	}

	xlnt::pattern_fill ToXlnt()const
	{
		xlnt::pattern_fill val;
		val.type((xlnt::pattern_fill_type)Type);
		val.foreground(ForegroundColor.ToXlnt());
		val.background(BackgroundColor.ToXlnt());
		return val;
	}
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DirectExcel|Fill")
		ExcelPatternFillType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DirectExcel|Fill")
		FExcelColor ForegroundColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DirectExcel|Fill")
		FExcelColor BackgroundColor;
};


/**
 * Excel cell gradient fill
 */
USTRUCT(BlueprintType, Category = "DirectExcel")
struct DIRECTEXCEL_API FExcelGradientFill
{
	GENERATED_BODY()
public:
	FExcelGradientFill() = default;
	
	FExcelGradientFill(xlnt::gradient_fill val):
		Type((ExcelGradientFillType)val.type()),
		Degree(val.degree()),
		Distance(val.left(),val.top(),val.right(),val.bottom())
	{
		for (const auto& kv:val.stops())
		{
			Stops.Add(kv.first, FExcelColor(kv.second));
		}
	}

	xlnt::gradient_fill ToXlnt()const
	{
		xlnt::gradient_fill val;
		val.type((xlnt::gradient_fill_type)Type);
		val.degree(Degree);
		val.left(Distance.Left);
		val.right(Distance.Right);
		val.top(Distance.Top);
		val.bottom(Distance.Bottom);
		for (const auto& kv:Stops)
		{
			val.add_stop(kv.Key, kv.Value.ToXlnt());
		}
	
		return val;
	}
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DirectExcel|Fill")
		ExcelGradientFillType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DirectExcel|Fill")
		float Degree;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DirectExcel|Fill")
		FMargin Distance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DirectExcel|Fill")
		TMap<float, FExcelColor> Stops;
};


/**
 * Excel cell fill
 */
USTRUCT(BlueprintType, Category = "DirectExcel")
struct DIRECTEXCEL_API FExcelFill
{
	GENERATED_BODY()
public:
	FExcelFill() = default;

	FExcelFill(xlnt::fill val) :
		Type((ExcelFillType)val.type())
	{
		switch (Type)
		{
		case ExcelFillType::Pattern:
			PatternFill = FExcelPatternFill(val.pattern_fill());
			break;
		case ExcelFillType::Gradient:
			GradientFill = FExcelGradientFill(val.gradient_fill());
			break;
		default:
			break;
		}
	}

	xlnt::fill ToXlnt()const
	{
		xlnt::fill val;
		val.type_=(xlnt::fill_type)Type;

		switch (Type)
		{
		case ExcelFillType::Pattern:
			val.pattern_ = PatternFill.ToXlnt();
			break;
		case ExcelFillType::Gradient:
			val.gradient_ = GradientFill.ToXlnt();
			break;
		default:
			break;
		}

		return val;
	}
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DirectExcel|Fill")
		ExcelFillType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DirectExcel|Fill")
		FExcelPatternFill PatternFill;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DirectExcel|Fill")
		FExcelGradientFill GradientFill;
};
