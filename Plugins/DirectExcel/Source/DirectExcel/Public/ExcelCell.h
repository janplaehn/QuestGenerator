// Copyright 2018 Jianzhao Fu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ExcelTypes.h"
#include "ExcelCellReference.h"
#include "ExcelFormat.h"
#include "Utility/SerializationLibrary.h"
#include "ExcelCell.generated.h"



/**
 * Excel cell struct
 */
UCLASS(Blueprintable, BlueprintType, Category = "DirectExcel")
class DIRECTEXCEL_API UExcelCell :public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Cell")
		ExcelCellType Type()const;

	UFUNCTION(BlueprintPure, Category = "DirectExcel|Cell")
		bool IsValid() const { return mData != nullptr; }

	UFUNCTION(BlueprintPure, Category = "DirectExcel|Cell")
		bool IsMerged()const;

	UFUNCTION(BlueprintPure, Category = "DirectExcel|Cell")
		int32 Column()const;
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Cell")
		int32 Row()const;

	UFUNCTION(BlueprintPure, Category = "DirectExcel|Cell")
		FExcelCellReference Reference()const;

	UFUNCTION(BlueprintPure, Category = "DirectExcel|Cell")
		UExcelCell* MakeCellWithOffset(int32 columnOffset, int32 rowOffset);

public://compare
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Cell")
		int32 Compare(UExcelCell* other)const;

	UFUNCTION(BlueprintPure, meta = (DisplayName = "ExcelCell < ExcelCell", CompactNodeTitle = "<", Keywords = "< less"), Category = "DirectExcel|Cell")
		static bool Less_ExcelCellExcelCell(UExcelCell* A, UExcelCell* B) { return A->Compare(B) < 0; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "ExcelCell > ExcelCell", CompactNodeTitle = ">", Keywords = "> greater"), Category = "DirectExcel|Cell")
		static bool Greater_ExcelCellExcelCell(UExcelCell* A, UExcelCell* B) { return A->Compare(B) > 0; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "ExcelCell <= ExcelCell", CompactNodeTitle = "<=", Keywords = "<= less"), Category = "DirectExcel|Cell")
		static bool LessEqual_ExcelCellExcelCell(UExcelCell* A, UExcelCell* B) { return A->Compare(B) <= 0; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "ExcelCell >= ExcelCell", CompactNodeTitle = ">=", Keywords = ">= greater"), Category = "DirectExcel|Cell")
		static bool GreaterEqual_ExcelCellExcelCell(UExcelCell* A, UExcelCell* B) { return A->Compare(B) >= 0; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Equal (ExcelCell)", CompactNodeTitle = "==", Keywords = "== equal"), Category = "DirectExcel|Cell")
		static bool EqualEqual_ExcelCellExcelCell(UExcelCell* A, UExcelCell* B) { return A->Compare(B) == 0; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "NotEqual (ExcelCell)", CompactNodeTitle = "!=", Keywords = "!= not equal"), Category = "DirectExcel|Cell")
		static bool NotEqual_ExcelCellExcelCell(UExcelCell* A, UExcelCell* B) { return A->Compare(B) != 0; }
public:
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Cell")
		bool HasValue()const;
	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Cell")
		void Clear();

	UFUNCTION(BlueprintPure, Category = "DirectExcel|Cell")
		bool ToBool()const;
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Cell")
		int32 ToInt()const;
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Cell")
		float ToFloat()const;
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Cell")
		FString ToString()const;
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Cell")
		FDateTime ToDateTime()const;
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Cell")
		UObject* ToUObject()const;

	UFUNCTION(BlueprintPure, Category = "DirectExcel|Cell")
		FVector ToVector()const { return ReadItem<FVector>(); }
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Cell")
		FRotator ToRotator()const { return ReadItem<FRotator>(); }
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Cell")
		FTransform ToTransform()const { return ReadItem<FTransform>(); }
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Cell")
		FColor ToColor()const { return ReadItem<FColor>(); }
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Cell")
		FMatrix ToMatrix()const { return ReadItem<FMatrix>(); }

public:
	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Cell")
		void SetBool(bool value);
	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Cell")
		void SetInt(int32 value);
	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Cell")
		void SetFloat(float value);
	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Cell")
		void SetString(FString value);
	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Cell")
		void SetDateTime(FDateTime value);
	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Cell")
		void SetObject(const UObject* value);


	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Cell")
		void SetVector(const FVector& value) { SetItem(value);	}
	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Cell")
		void SetRotator(const FRotator& value) { SetItem(value); }
	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Cell")
		void SetTransform(const FTransform& value) { SetItem(value); }
	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Cell")
		void SetColor(const FColor& value) { SetItem(value); }
	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Cell")
		void SetMatrix(const FMatrix& value) { SetItem(value); }
public:
	//array support?

public://hyperlink
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Cell")
		bool HasHyperlink()const;

	UFUNCTION(BlueprintPure, Category = "DirectExcel|Cell")
		FString GetHyperlink()const;

	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Cell")
		void SetHyperlink(FString url, FString display = TEXT(""));	//need :
public://formula
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Cell")
		bool HasFormula()const;
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Cell")
		FString GetFormula()const;
	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Cell")
		void SetFormula(FString formula);
	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Cell")
		void ClearFormula();
public://comment
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Cell")
		bool HasComment()const;
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Cell")
		void GetComment(FString& Text, FString& Author)const;
	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Cell")
		void SetComment(FString text, FString author);
	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Cell")
		void ClearComment();
public://format
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Cell")
		bool HasFormat()const;

	UFUNCTION(BlueprintPure, Category = "DirectExcel|Cell")
		UExcelFormat* GetFormat()const;

	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Cell")
		void SetFormat(UExcelFormat* format);

	UFUNCTION(BlueprintCallable, Category = "DirectExcel|Cell")
		void ClearFormat();
public:
	xlnt::cell Data()const { return mData; }
	void SetData(xlnt::cell data) { mData = data; }
public:
	UFUNCTION(BlueprintPure, Category = "DirectExcel|Worksheet")
		UExcelWorksheet* ParentSheet()const;

	void SetWorksheet(UExcelWorksheet* worksheet);
public:
	template<typename T>
	FString SetItem(const T& value)
	{
		FString str = USerializationLibrary::ToString(value);
		SetString(str);
		return str;
	}
	template<typename T>
	T ReadItem()const
	{
		T outValue;
		TryReadItem(outValue);
		return outValue;
	}

	template<typename T>
	FString TryReadItem(T& outValue)const
	{
		return USerializationLibrary::TryParse(ToString(), outValue);
	}

private:
	TWeakObjectPtr<UExcelWorksheet> mWorksheet;
	xlnt::cell mData;
};

namespace DirectExcel
{
	template<typename T>
	FORCEINLINE T ToValue(const UExcelCell* cell);

	template<>
	FORCEINLINE bool ToValue(const UExcelCell* cell) { return cell->ToBool(); }

	template<>
	FORCEINLINE int32 ToValue(const UExcelCell* cell) { return cell->ToInt(); }

	template<>
	FORCEINLINE float ToValue(const UExcelCell* cell) { return cell->ToFloat(); }

	template<>
	FORCEINLINE FString ToValue(const UExcelCell* cell) { return cell->ToString(); }

	template<>
	FORCEINLINE FDateTime ToValue(const UExcelCell* cell) { return cell->ToDateTime(); }

	template<>
	FORCEINLINE FVector ToValue(const UExcelCell* cell) { return cell->ToVector(); }

	template<>
	FORCEINLINE FRotator ToValue(const UExcelCell* cell) { return cell->ToRotator(); }

	template<>
	FORCEINLINE FTransform ToValue(const UExcelCell* cell) { return cell->ToTransform(); }

	template<>
	FORCEINLINE FColor ToValue(const UExcelCell* cell) { return cell->ToColor(); }

	template<>
	FORCEINLINE FMatrix ToValue(const UExcelCell* cell) { return cell->ToMatrix(); }

}

#undef DEFINE_FIND_SCRIPT_STRUCT