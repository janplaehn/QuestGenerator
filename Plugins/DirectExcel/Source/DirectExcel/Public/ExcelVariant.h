// Copyright 2018 Jianzhao Fu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ExcelTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "ExcelVariant.generated.h"

/**
 *Excel variant value in excel property
 */
USTRUCT(BlueprintType, Category = "DirectExcel")
struct DIRECTEXCEL_API FExcelVariant
{
	GENERATED_BODY()
public:
	friend class UExcelVariantLibrary;
public:
	FExcelVariant() {}
	FExcelVariant(bool value) :mType(ExcelVariantType::Bool),mBoolValue(value) {}
	FExcelVariant(int32 value) :mType(ExcelVariantType::Int32),mIntValue(value) {}
	FExcelVariant(FString value) :mType(ExcelVariantType::String),mStringValue(value) {}
	FExcelVariant(FDateTime value) :mType(ExcelVariantType::DateTime),mDateTimeValue(value) {}

	FExcelVariant(TArray<bool> value);
	FExcelVariant(TArray<int32> value);
	FExcelVariant(TArray<FString> value);
	FExcelVariant(TArray<FDateTime> value);
	FExcelVariant(TArray<FExcelVariant> value) :mType(ExcelVariantType::Array),mArrayValue(value) {}
public:
	ExcelVariantType Type()const { return mType; }
	void SetType(ExcelVariantType value) { mType = value; }
	bool BoolValue() const { return mBoolValue; }
	void SetBoolValue(bool val) { mBoolValue = val; }
	int32 IntValue() const { return mIntValue; }
	void SetIntValue(int32 val) { mIntValue = val; }
	FString StringValue() const { return mStringValue; }
	void SetStringValue(FString val) { mStringValue = val; }
	FDateTime DateTimeValue() const { return mDateTimeValue; }
	void SetDateTimeValue(FDateTime val) { mDateTimeValue = val; }
	TArray<FExcelVariant>& MutableArrayValue() { return mArrayValue; }
	const TArray<FExcelVariant>& ArrayValue()const { return mArrayValue; }

private:
	ExcelVariantType mType = ExcelVariantType::None;
	bool mBoolValue = false;
	int32 mIntValue = 0;
	FString mStringValue;
	FDateTime mDateTimeValue;
	TArray<FExcelVariant> mArrayValue;
};

/**
* Helper functions to convert variant
*/
UCLASS(DisplayName = "DirectExcel", Category = "DirectExcel")
class DIRECTEXCEL_API UExcelVariantLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Variant")
		static FExcelVariant MakeVariantWithBool(bool value) { return FExcelVariant(value); }
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Variant")
		static FExcelVariant MakeVariantWithInt(int32 value) { return FExcelVariant(value); }
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Variant")
		static FExcelVariant MakeVariantWithString(FString value) { return FExcelVariant(value); }
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Variant")
		static FExcelVariant MakeVariantWithDateTime(FDateTime value) { return FExcelVariant(value); }

	UFUNCTION(BlueprintPure, Category = "DirectExcel|Variant")
		static FExcelVariant MakeVariantWithBoolArray(TArray<bool> value) { return FExcelVariant(value); }
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Variant")
		static FExcelVariant MakeVariantWithIntArray(TArray<int32> value) { return FExcelVariant(value); }
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Variant")
		static FExcelVariant MakeVariantWithStringArray(TArray<FString> value) { return FExcelVariant(value); }
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Variant")
		static FExcelVariant MakeVariantWithDateTimeArray(TArray<FDateTime> value) { return FExcelVariant(value); }

	UFUNCTION(BlueprintPure, Category = "DirectExcel|Variant")
		static FExcelVariant MakeVariantWithDateVariantArray(TArray<FExcelVariant> value) { return FExcelVariant(value); }

	UFUNCTION(BlueprintPure, Category = "DirectExcel|Variant")
		static ExcelVariantType GetType(FExcelVariant value) { return value.mType; }
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Variant")
		static bool ToBool(FExcelVariant value) { return value.mBoolValue; }
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Variant")
		static int32 ToInt(FExcelVariant value) { return value.mIntValue; }
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Variant")
		static FString ToString(FExcelVariant value) { return value.mStringValue; }
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Variant")
		static FDateTime ToDateTime(FExcelVariant value) { return value.mDateTimeValue; }
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Variant")
		static TArray<FExcelVariant> ToArray(FExcelVariant value) { return value.mArrayValue; }

	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Variant")
		static void SetBool(FExcelVariant target, bool value) { target.mBoolValue = value; target.mType = ExcelVariantType::Bool; }
	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Variant")
		static void SetInt(FExcelVariant target, int32 value) { target.mIntValue = value; target.mType = ExcelVariantType::Int32; }
	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Variant")
		static void SetString(FExcelVariant target, FString value) { target.mStringValue = value; target.mType = ExcelVariantType::String; }
	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Variant")
		static void SetDateTime(FExcelVariant target, FDateTime value) { target.mDateTimeValue = value; target.mType = ExcelVariantType::DateTime; }
};
