// Copyright 2018 Jianzhao Fu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ExcelTypes.h"

#include "ExcelProtection.generated.h"


/**
 * Excel cell font
 */
USTRUCT(BlueprintType, Category = "DirectExcel")
struct DIRECTEXCEL_API FExcelProtection
{
	GENERATED_BODY()
public:
	FExcelProtection() = default;
	FExcelProtection(xlnt::protection val):
		IsLocked(val.locked()),
		IsHidden(val.hidden())
	{

	}

	xlnt::protection ToXlnt()const
	{
		xlnt::protection val;
		val.locked(IsLocked);
		val.hidden(IsHidden);
		return val;
	}
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DirectExcel|Protection")
		bool IsLocked = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DirectExcel|Protection")
		bool IsHidden = false;
	
};
