// Copyright 2018 Jianzhao Fu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ExcelTypes.h"

#include "ExcelAlignment.generated.h"


/**
 * Excel cell alignment
 */
USTRUCT(BlueprintType, Category = "DirectExcel")
struct DIRECTEXCEL_API FExcelAlignment
{
	GENERATED_BODY()
public:
	FExcelAlignment() = default;
	FExcelAlignment(bool shrink, bool wrapText, int indent,int textRotation, ExcelHorizontalAlignment horizontalAlignment, ExcelVerticalAlignment verticalAlignment) :
		ShrinkToFit(shrink),
		WrapText(wrapText),
		Indent(indent),
		TextRotation(textRotation),
		HorizontalAlignment(horizontalAlignment),
		VerticalAlignment(verticalAlignment)
	{}

	
	FExcelAlignment(xlnt::alignment val):
		ShrinkToFit(val.shrink()),
		WrapText(val.wrap()),
		Indent(val.indent().is_set() ? val.indent().get() : 0),
		TextRotation(val.rotation().is_set() ? val.rotation().get() : 0),
		HorizontalAlignment(val.horizontal().is_set() ? (ExcelHorizontalAlignment)val.horizontal().get() : ExcelHorizontalAlignment::General),
		VerticalAlignment(val.vertical().is_set() ? (ExcelVerticalAlignment)val.vertical().get() : ExcelVerticalAlignment::Top)
	{

	}

	xlnt::alignment ToXlnt()const
	{
		xlnt::alignment xf;
		xf.shrink(ShrinkToFit);
		xf.wrap(WrapText);
		xf.indent(Indent);
		xf.rotation(TextRotation);
		xf.horizontal((xlnt::horizontal_alignment)HorizontalAlignment);
		xf.vertical((xlnt::vertical_alignment)VerticalAlignment);
		return xf;
	}
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DirectExcel|Alignment")
		bool ShrinkToFit = false;//Whether or not to shrink font size until it fits on one line
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DirectExcel|Alignment")
		bool WrapText = false;//Whether or not to wrap text to the next line
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DirectExcel|Alignment")
		int Indent = 0;//The indent in number of spaces from the side
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DirectExcel|Alignment")
		int TextRotation= 0;//The text roation in degrees
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DirectExcel|Alignment")
		ExcelHorizontalAlignment HorizontalAlignment;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DirectExcel|Alignment")
		ExcelVerticalAlignment VerticalAlignment;
};
