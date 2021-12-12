// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestLabelCollectionDetails.h"

#include "DetailCategoryBuilder.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "IPropertyTypeCustomization.h"
#include "QuestActionRow.h"
#include "QuestLabel.h"
#include "Widgets/Layout/SGridPanel.h"
#include "Widgets/Input/SButton.h"
#include "PropertyHandle.h"
#include "PropertyCustomizationHelpers.h"
#include "Widgets/Layout/SWrapBox.h"

TSharedRef<IPropertyTypeCustomization> FQuestLabelCollectionDetails::MakeInstance()
{
	return MakeShareable(new FQuestLabelCollectionDetails);
}

void FQuestLabelCollectionDetails::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle,
	FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
}

void FQuestLabelCollectionDetails::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle,
                                                     IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	ChildBuilder.AddCustomRow(FText::FromString("DataTable"))
	.NameContent()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.Padding(FMargin(0, 1, 0, 1))
			.FillWidth(1.0f)
			[
				SNew(STextBlock)
				.Text(FText::FromString("Labels"))
				//.Font(IPropertyTypeCustomizationUtils::GetRegularFont())
			]
		]
	.ValueContent()
	.MinDesiredWidth(400)
    [
    SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.Padding(FMargin(0, 1, 0, 1))
		.FillWidth(1.0f)
		[
			PropertyHandle->GetChildHandle("DataTable")->CreatePropertyValueWidget(false)
		]
    ];

	PropertyHandle->GetChildHandle("DataTable")->SetOnPropertyValueChanged(FSimpleDelegate::CreateLambda(
			[this, &ChildBuilder]()
			{
				ChildBuilder.GetParentCategory().GetParentLayout().ForceRefreshDetails();
			}
		));

	// PropertyHandle->GetChildHandle("Labels")->SetOnPropertyValueChanged(FSimpleDelegate::CreateLambda(
	// 		[this, &ChildBuilder]()
	// 		{
	// 			ChildBuilder.GetParentCategory().GetParentLayout().ForceRefreshDetails();
	// 		}
	// 	));
	
	const TSharedPtr<IPropertyHandle> DataTablePropertyHandle =  PropertyHandle->GetChildHandle("DataTable");
	UObject* DataTableObject;
	DataTablePropertyHandle->GetValue(DataTableObject);
	UDataTable* DataTable = Cast<UDataTable>(DataTableObject);
	if (!IsValid(DataTable))
	{
		return;
	}

	FDetailWidgetDecl& LabelContentDecl = ChildBuilder.AddCustomRow(FText::FromName("Labels")).ValueContent();

	LabelContentDecl.MinDesiredWidth(600);
    LabelContentDecl.MaxDesiredWidth(TOptional<float>());

	TSharedRef<SWrapBox> LabelBox = SNew(SWrapBox).UseAllottedSize(true);
	
	LabelContentDecl
	[
		LabelBox
	];

	TSharedPtr<IPropertyHandleArray> LabelPropertyHandleArray  = PropertyHandle->GetChildHandle("Labels")->AsArray();
	
	DataTable->ForeachRow<FQuestLabelRow>("FQuestLabelCollectionDetails::CustomizeDetails",
		[this, &LabelBox, LabelPropertyHandleArray, DataTable, PropertyHandle](const FName& RowName, const FQuestLabelRow& DataRow)
		{
			bool bIsLabelActive = false;
			uint32 LabelCount;
			LabelPropertyHandleArray->GetNumElements(LabelCount);
			for (uint32 Index = 0; Index < LabelCount; Index++)
			{
				const TSharedRef<IPropertyHandle> Handle = LabelPropertyHandleArray->GetElement(Index);
				FName CurrentRowName;
				Handle->GetChildHandle("RowName")->GetValue(CurrentRowName);
				if (RowName == CurrentRowName)
				{
					bIsLabelActive = true;
				}
			}
			const FSlateColor Color = bIsLabelActive ? FLinearColor::White : FLinearColor::Transparent;
			
			LabelBox->AddSlot()
			.Padding(3,2)
			[
				SNew(SButton)
				.Text(FText::FromName(DataRow.Label))
				.ButtonColorAndOpacity(Color)
				.OnClicked(this, &FQuestLabelCollectionDetails::OnLabelClicked, RowName, PropertyHandle)
			];
		
		}
	);
}

FReply FQuestLabelCollectionDetails::OnLabelClicked(const FName RowName, TSharedRef<IPropertyHandle> PropertyHandle)
{
	uint32 LabelCount;
	TSharedPtr<class IPropertyHandleArray> LabelArray = PropertyHandle->GetChildHandle("Labels")->AsArray();

	const TSharedPtr<IPropertyHandle> DataTablePropertyHandle =  PropertyHandle->GetChildHandle("DataTable");
	UObject* DataTableObject;
	DataTablePropertyHandle->GetValue(DataTableObject);
	UDataTable* DataTable = Cast<UDataTable>(DataTableObject);
	if (!IsValid(DataTable))
	{
		return FReply::Unhandled();
	}
	
	LabelArray->GetNumElements(LabelCount);
	for (uint32 Index = 0; Index < LabelCount; Index++)
	{
		const TSharedRef<IPropertyHandle> Handle = LabelArray->GetElement(Index);
		FName CurrentName;
		Handle->GetChildHandle("RowName")->GetValue(CurrentName);
		if (RowName == CurrentName)
		{
			LabelArray->DeleteItem(Index);
			
			//Hack to refresh the properties
			DataTablePropertyHandle->SetValue(DataTable);
			
			return  FReply::Handled();
		}
	}
	
	const FPropertyAccess::Result AddItemResult = LabelArray->AddItem();
	if (AddItemResult != FPropertyAccess::Result::Success)
	{
		return FReply::Unhandled();
	}
	uint32 NewLabelCount;
	LabelArray->GetNumElements(NewLabelCount);
	if (NewLabelCount == 0)
	{
		return FReply::Unhandled();
	}
	
	TSharedRef<IPropertyHandle> NewLabelPropertyHandle = LabelArray->GetElement(NewLabelCount-1);
	NewLabelPropertyHandle->GetChildHandle("DataTable")->SetValue(DataTable);
	NewLabelPropertyHandle->GetChildHandle("RowName")->SetValue(RowName);

	//Hack to refresh the properties
	DataTablePropertyHandle->SetValue(DataTable);
	
	return FReply::Handled();
}
