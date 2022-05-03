// Copyright 2018 Jianzhao Fu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ExcelTypes.h"
#include "ExcelColor.h"

#include "ExcelBorder.generated.h"


/**
 * Excel cell border side
 */
USTRUCT(BlueprintType, Category = "DirectExcel")
struct DIRECTEXCEL_API FExcelBorderSide
{
	GENERATED_BODY()
public:
	FExcelBorderSide() = default;
	FExcelBorderSide(FExcelColor color, ExcelBorderStyle style) :
		Color(color),
		Style(style)
	{}

	FExcelBorderSide(xlnt::border::border_property val)
	{
		if (val.color().is_set())
		{
			Color = val.color().get();
		}
		if (val.style().is_set())
		{
			Style = (ExcelBorderStyle)val.style().get();
		}
	}

	xlnt::border::border_property ToXlnt()const
	{
		xlnt::border::border_property val;
		val.color(Color.ToXlnt());
		val.style((xlnt::border_style)Style);

		return val;
	}

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DirectExcel|Border")
		FExcelColor Color;//The color of the side
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DirectExcel|Border")
		ExcelBorderStyle Style;	//The style of the side
};


/**
 * Excel cell border
 */
USTRUCT(BlueprintType, Category = "DirectExcel")
struct DIRECTEXCEL_API FExcelBorder
{
	GENERATED_BODY()
public:
	friend class UExcelCellLibrary;
	FExcelBorder() = default;
	FExcelBorder(
		FExcelBorderSide start,
		FExcelBorderSide end,
		FExcelBorderSide top,
		FExcelBorderSide bottom,
		FExcelBorderSide horizontal,
		FExcelBorderSide vertical,
		FExcelBorderSide diagonal,
		ExcelBorderDiagonalDirection diagonalDirection) :
		Start(start),
		End(end),
		Top(top),
		Bottom(bottom),
		Horizontal(horizontal),
		Vertical(vertical),
		Diagonal(diagonal),
		DiagonalDirection(diagonalDirection)
	{}

	FExcelBorder(xlnt::border val)
	{
		if (val.start_.is_set())
		{
			Start = FExcelBorderSide(val.start_.get());
		}
		if (val.end_.is_set())
		{
			End = FExcelBorderSide(val.end_.get());
		}
		if (val.top_.is_set())
		{
			Top = FExcelBorderSide(val.top_.get());
		}
		if (val.bottom_.is_set())
		{
			Bottom = FExcelBorderSide(val.bottom_.get());
		}
		if (val.horizontal_.is_set())
		{
			Horizontal = FExcelBorderSide(val.horizontal_.get());
		}
		if (val.vertical_.is_set())
		{
			Vertical = FExcelBorderSide(val.vertical_.get());
		}
		if (val.diagonal_.is_set())
		{
			Diagonal = FExcelBorderSide(val.diagonal_.get());
		}
		if (val.diagonal_direction_.is_set())
		{
			DiagonalDirection = ExcelBorderDiagonalDirection(val.diagonal_direction_.get());
		}
	}

	xlnt::border ToXlnt()const
	{
		xlnt::border val;
		val.start_ = Start.ToXlnt();
		val.end_ = End.ToXlnt();
		val.top_ = Top.ToXlnt();
		val.bottom_ = Bottom.ToXlnt();
		val.horizontal_ = Horizontal.ToXlnt();
		val.vertical_ = Vertical.ToXlnt();
		val.diagonal_ = Diagonal.ToXlnt();
		val.diagonal_direction_ = (xlnt::diagonal_direction)DiagonalDirection;

		return val;
	}
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DirectExcel|Border")
		FExcelBorderSide Start;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DirectExcel|Border")
		FExcelBorderSide End;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DirectExcel|Border")
		FExcelBorderSide Top;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DirectExcel|Border")
		FExcelBorderSide Bottom;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DirectExcel|Border")
		FExcelBorderSide Horizontal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DirectExcel|Border")
		FExcelBorderSide Vertical;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DirectExcel|Border")
		FExcelBorderSide Diagonal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DirectExcel|Border")
		ExcelBorderDiagonalDirection DiagonalDirection;
};
