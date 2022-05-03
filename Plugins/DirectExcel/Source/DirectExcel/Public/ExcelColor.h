// Copyright 2018 Jianzhao Fu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ExcelTypes.h"

#include "ExcelColor.generated.h"


/**
 * Excel cell color
 */
USTRUCT(BlueprintType, Category = "DirectExcel")
struct DIRECTEXCEL_API FExcelColor
{
	GENERATED_BODY()
public:
	FExcelColor() = default;
	FExcelColor(ExcelColorType type, int colorIndex, FLinearColor colorValue, double tint, bool isAutoColor) :
		Type(type),
		ColorIndex(colorIndex),
		ColorValue(colorValue),
		Tint(tint),
		IsAutoColor(isAutoColor)
	{}

	FExcelColor(xlnt::color val) :
		Type((ExcelColorType)val.type()),
		Tint((float)val.tint()),
		IsAutoColor(val.auto_())
	{
		switch (val.type())
		{
		case xlnt::color_type::indexed:
			ColorIndex = val.indexed().index();
			break;
		case xlnt::color_type::theme:
			ColorIndex = val.theme().index();
			break;
		case xlnt::color_type::rgb:
			ColorValue.R = val.rgb().red()/255.f;
			ColorValue.G = val.rgb().green() / 255.f;
			ColorValue.B = val.rgb().blue() / 255.f;
			ColorValue.A = val.rgb().alpha() / 255.f;
			break;
		default:
			break;
		}
	}

	xlnt::color ToXlnt()const
	{
		xlnt::color val;
		val.type_ = (xlnt::color_type)Type;
		val.tint_ = Tint;
		val.auto__ = IsAutoColor;
		switch (val.type())
		{
		case xlnt::color_type::indexed:
			val.indexed().index(ColorIndex);
			break;
		case xlnt::color_type::theme:
			val.theme().index(ColorIndex);
			break;
		case xlnt::color_type::rgb:
			val.rgb_ = xlnt::rgb_color(ColorValue.R*255.f, ColorValue.G*255.f, ColorValue.B*255.f, ColorValue.A*255.f);
			break;
		default:
			break;
		}
		return val;
	}
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DirectExcel|Color")
		ExcelColorType Type=ExcelColorType::RGB;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DirectExcel|Color")
		int ColorIndex;// indexed_color or theme_color

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DirectExcel|Color")
		FLinearColor ColorValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DirectExcel|Color")
		float Tint=0.f; //The tint of this color

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DirectExcel|Color")
		bool IsAutoColor= false; // Whether or not this is an auto color

};
