#pragma once

#include "CoreMinimal.h"
#include "PrivateAccess.h"
#include "UObject/TextProperty.h"

INSIDER_STEAL_PRIVATE_MEMBER(FField, FFieldClass*, ClassPrivate);

namespace Serialization_Private
{

	template<typename T>
	FORCEINLINE UScriptStruct* FindScriptStruct()
	{
		return StaticStruct<T>();	//for user defined struct
	}

#define DEFINE_FIND_SCRIPT_STRUCT(TStruct)\
	template<>\
	FORCEINLINE UScriptStruct* FindScriptStruct<TStruct>()\
	{\
		static UScriptStruct* structClass = nullptr;\
		if (structClass == nullptr)\
		{\
			const TCHAR* structName=TEXT(#TStruct);\
			++structName;\
			structClass = Cast<UScriptStruct>(StaticFindObjectFast(UScriptStruct::StaticClass(), nullptr, structName, true, true));\
		}\
		return structClass;\
	}

	//core structs
	DEFINE_FIND_SCRIPT_STRUCT(FGuid);
	DEFINE_FIND_SCRIPT_STRUCT(FVector);
	DEFINE_FIND_SCRIPT_STRUCT(FVector4);
	DEFINE_FIND_SCRIPT_STRUCT(FVector2D);
	DEFINE_FIND_SCRIPT_STRUCT(FTwoVectors);
	DEFINE_FIND_SCRIPT_STRUCT(FPlane);
	DEFINE_FIND_SCRIPT_STRUCT(FRotator);
	DEFINE_FIND_SCRIPT_STRUCT(FQuat);
	DEFINE_FIND_SCRIPT_STRUCT(FIntPoint);
	DEFINE_FIND_SCRIPT_STRUCT(FIntVector);
	DEFINE_FIND_SCRIPT_STRUCT(FColor);
	DEFINE_FIND_SCRIPT_STRUCT(FLinearColor);
	DEFINE_FIND_SCRIPT_STRUCT(FBox);
	DEFINE_FIND_SCRIPT_STRUCT(FBox2D);
	DEFINE_FIND_SCRIPT_STRUCT(FBoxSphereBounds);
	DEFINE_FIND_SCRIPT_STRUCT(FOrientedBox);
	DEFINE_FIND_SCRIPT_STRUCT(FMatrix);
	DEFINE_FIND_SCRIPT_STRUCT(FTransform);
	DEFINE_FIND_SCRIPT_STRUCT(FSoftObjectPath);
	DEFINE_FIND_SCRIPT_STRUCT(FSoftClassPath);
	DEFINE_FIND_SCRIPT_STRUCT(FPrimaryAssetType);
	DEFINE_FIND_SCRIPT_STRUCT(FPrimaryAssetId);
	DEFINE_FIND_SCRIPT_STRUCT(FFloatRangeBound);
	DEFINE_FIND_SCRIPT_STRUCT(FFloatRange);
	DEFINE_FIND_SCRIPT_STRUCT(FInt32RangeBound);
	DEFINE_FIND_SCRIPT_STRUCT(FInt32Range);
	DEFINE_FIND_SCRIPT_STRUCT(FFrameNumberRangeBound);
	DEFINE_FIND_SCRIPT_STRUCT(FFrameNumberRange);
	DEFINE_FIND_SCRIPT_STRUCT(FFloatInterval);
	DEFINE_FIND_SCRIPT_STRUCT(FInt32Interval);

	//////////////////////////////////////////////////////////////////////////

	//struct
	template<typename T>
	FORCEINLINE FProperty* ConstructProperty(T value, const FProperty* owner = nullptr)
	{
		static FStructProperty itemProperty(owner, "", RF_Public);
		static bool isInitialized = false;
		if (!isInitialized)
		{
			INSIDER_REF_PRIVATE_MEMBER(&itemProperty, FField, ClassPrivate) = FStructProperty::StaticClass();
			itemProperty.Struct = FindScriptStruct<T>();
			FArchive dummy;
			itemProperty.Link(dummy);
			isInitialized = true;
		}
		
		return &itemProperty;
	}

	//primitive
#define CONSTRUCT_PROPERTY_PRIMITIVE(T,TProperty)\
	template<>\
	FORCEINLINE FProperty* ConstructProperty(T value, const FProperty* owner)\
	{\
		static TProperty itemProperty(owner, "", RF_Public);\
		INSIDER_REF_PRIVATE_MEMBER(&itemProperty, FField, ClassPrivate) = TProperty::StaticClass();\
		return &itemProperty;\
	}

	CONSTRUCT_PROPERTY_PRIMITIVE(uint8, FByteProperty);
	CONSTRUCT_PROPERTY_PRIMITIVE(int8, FInt8Property);
	CONSTRUCT_PROPERTY_PRIMITIVE(int16, FInt16Property);
	CONSTRUCT_PROPERTY_PRIMITIVE(uint16, FUInt16Property);

	CONSTRUCT_PROPERTY_PRIMITIVE(int32, FIntProperty);
	CONSTRUCT_PROPERTY_PRIMITIVE(uint32, FUInt32Property);

	CONSTRUCT_PROPERTY_PRIMITIVE(int64, FInt64Property);
	CONSTRUCT_PROPERTY_PRIMITIVE(uint64, FUInt64Property);

	CONSTRUCT_PROPERTY_PRIMITIVE(float, FFloatProperty);
	CONSTRUCT_PROPERTY_PRIMITIVE(double, FDoubleProperty);
	CONSTRUCT_PROPERTY_PRIMITIVE(bool, FBoolProperty);

	CONSTRUCT_PROPERTY_PRIMITIVE(FName, FNameProperty);
	CONSTRUCT_PROPERTY_PRIMITIVE(FString, FStrProperty);
	CONSTRUCT_PROPERTY_PRIMITIVE(FText, FTextProperty);

	//container
	template<typename T>
	FORCEINLINE FProperty* ConstructProperty(const TArray<T>& value, const FProperty* owner)
	{
		static FArrayProperty containerProperty(owner, "", RF_Public);
		static FProperty* innerProperty = ConstructProperty(T(), &containerProperty);

		static bool isInitialized = false;
		if (!isInitialized)
		{
			containerProperty.Inner = innerProperty;
			FArchive dummy;
			containerProperty.Link(dummy);
			isInitialized = true;
		}
		return &containerProperty;
	}
	template<typename T>
	FORCEINLINE FProperty* ConstructProperty(const TSet<T>& value, const FProperty* owner)
	{
		static FSetProperty containerProperty(owner, "", RF_Public);
		static FProperty* innerProperty = ConstructProperty(T(), &containerProperty);

		static bool isInitialized = false;
		if (!isInitialized)
		{
			containerProperty.ElementProp = innerProperty;
			FArchive dummy;
			containerProperty.Link(dummy);
			isInitialized = true;
		}
		return &containerProperty;
	}

	template<typename TKey, typename TValue>
	FORCEINLINE FProperty* ConstructProperty(const TMap<TKey, TValue>& value, const FProperty* owner)
	{
		static FMapProperty containerProperty(owner, "", RF_Public);
		static FProperty* keyProperty = ConstructProperty(TKey(), &containerProperty);
		static FProperty* valueProperty = ConstructProperty(TValue(), &containerProperty);

		static bool isInitialized = false;
		if (!isInitialized)
		{
			containerProperty.KeyProp = keyProperty;
			containerProperty.ValueProp = valueProperty;
			FArchive dummy;
			containerProperty.Link(dummy);
			isInitialized = true;
		}

		return &containerProperty;
	}
}


class USerializationLibrary
{
public:
	template<typename T>
	static FString ToString(const T& value)
	{
		FString result;
		const FProperty* p = Serialization_Private::ConstructProperty(T(), nullptr);
		p->ExportTextItem(result, &value, nullptr, nullptr, PPF_ExternalEditor, nullptr);
		return result;
	}

	template<typename T>
	static FString TryParse(const FString& str, T& outValue)
	{
		const FProperty* p = Serialization_Private::ConstructProperty(T(), nullptr);
		return p->ImportText(*str, &outValue, PPF_ExternalEditor, nullptr);
	}
public:
	static FString ToString(const UScriptStruct* structClass, const void* structData)
	{
		FString result;
		structClass->ExportText(result, structData, nullptr, nullptr, PPF_ExternalEditor,nullptr);
		return result;
	}

	static FString TryParse(const FString& str, UScriptStruct* structClass, void* structData)
	{
		FString result;
		FStringOutputDevice ImportError;
		structClass->ImportText(*str, structData, nullptr, PPF_ExternalEditor, &ImportError, structClass->GetName());
		return MoveTemp(ImportError);
	}
};

#undef DEFINE_FIND_SCRIPT_STRUCT
#undef CONSTRUCT_PROPERTY_PRIMITIVE