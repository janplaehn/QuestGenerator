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

static const FName LabelsHandleId("Labels");
static const FName DataTableHandleId("DataTable");
static const FName RowNameHandleId("RowName"); 

FQuestLabelCollectionDetails::FQuestLabelCollectionDetails()
{
	DefaultStyle = FButtonStyle::GetDefault();
	DefaultStyle.Normal.ImageType = ESlateBrushImageType::NoImage;
	DefaultStyle.Normal.TintColor = FLinearColor::Transparent;

	HighlightedStyle = DefaultStyle;
	HighlightedStyle.Normal.TintColor = FLinearColor::White;

	HighlightedTextStyle = FCoreStyle::Get().GetWidgetStyle< FTextBlockStyle >("NormalText");
	DefaultTextStyle = HighlightedTextStyle;
	DefaultTextStyle.SetFontSize(9);
	
}

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
	ChildBuilder.AddCustomRow(FText::FromName(DataTableHandleId))
	.NameContent()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.Padding(FMargin(0, 1, 0, 1))
			.FillWidth(1.0f)
			[
				SNew(STextBlock)
				.Text(FText::FromName(LabelsHandleId))
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
			PropertyHandle->GetChildHandle(DataTableHandleId)->CreatePropertyValueWidget(false)
		]
    ];

	PropertyHandle->GetChildHandle(DataTableHandleId)->SetOnPropertyValueChanged(FSimpleDelegate::CreateLambda(
			[this, &ChildBuilder]()
			{
				ChildBuilder.GetParentCategory().GetParentLayout().ForceRefreshDetails();
			}
		));
	
	const TSharedPtr<IPropertyHandle> DataTablePropertyHandle =  PropertyHandle->GetChildHandle(DataTableHandleId);
	UObject* DataTableObject;
	DataTablePropertyHandle->GetValue(DataTableObject);
	UDataTable* DataTable = Cast<UDataTable>(DataTableObject);
	if (!IsValid(DataTable))
	{
		return;
	}

	FDetailWidgetDecl& LabelContentDecl = ChildBuilder.AddCustomRow(FText::FromName(LabelsHandleId)).ValueContent();

	LabelContentDecl.MinDesiredWidth(600);
    LabelContentDecl.MaxDesiredWidth(TOptional<float>());

	TSharedRef<SWrapBox> LabelBox = SNew(SWrapBox).UseAllottedSize(true);
	
	LabelContentDecl
	[
		LabelBox
	];

	TSharedPtr<IPropertyHandleArray> LabelPropertyHandleArray  = PropertyHandle->GetChildHandle(LabelsHandleId)->AsArray();
	
	DataTable->ForeachRow<FQuestLabelRow>("FQuestLabelCollectionDetails::CustomizeDetails",
		[this, &LabelBox, LabelPropertyHandleArray, PropertyHandle](const FName& RowName, const FQuestLabelRow& DataRow)
		{
			bool bIsLabelActive = false;
			uint32 LabelCount;
			LabelPropertyHandleArray->GetNumElements(LabelCount);
			for (uint32 Index = 0; Index < LabelCount; Index++)
			{
				const TSharedRef<IPropertyHandle> Handle = LabelPropertyHandleArray->GetElement(Index);
				FName CurrentRowName;
				Handle->GetChildHandle(RowNameHandleId)->GetValue(CurrentRowName);
				if (RowName == CurrentRowName)
				{
					bIsLabelActive = true;
				}
			}

			const FSlateColor ForegroundColor =  bIsLabelActive ? FLinearColor::Black : FLinearColor(0.25f, 0.25f, 0.25f);
			const FButtonStyle* Style = bIsLabelActive ? &HighlightedStyle : &DefaultStyle;
			const FTextBlockStyle* FontStyle = bIsLabelActive ? &HighlightedTextStyle : &DefaultTextStyle;
			const FName LabelName = bIsLabelActive ? FName(FString("x ").Append(DataRow.Label.ToString())) : FName(FString("+ ").Append(DataRow.Label.ToString()));
			
			LabelBox->AddSlot()
			.Padding(3,2)
			[
				SNew(SButton)
				.Text(FText::FromName(LabelName))
				.TextStyle(FontStyle)
				.ForegroundColor(ForegroundColor)
				.ButtonStyle(Style)
				.OnClicked(this, &FQuestLabelCollectionDetails::OnLabelClicked, RowName, PropertyHandle)
			];
		
		}
	);
}

FReply FQuestLabelCollectionDetails::OnLabelClicked(const FName RowName, const TSharedRef<IPropertyHandle> PropertyHandle) const
{
	uint32 LabelCount;
	TSharedPtr<class IPropertyHandleArray> LabelArray = PropertyHandle->GetChildHandle(LabelsHandleId)->AsArray();

	const TSharedPtr<IPropertyHandle> DataTablePropertyHandle =  PropertyHandle->GetChildHandle(DataTableHandleId);
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
		Handle->GetChildHandle(RowNameHandleId)->GetValue(CurrentName);
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
	
	const TSharedRef<IPropertyHandle> NewLabelPropertyHandle = LabelArray->GetElement(NewLabelCount-1);
	NewLabelPropertyHandle->GetChildHandle(DataTableHandleId)->SetValue(DataTable);
	NewLabelPropertyHandle->GetChildHandle(RowNameHandleId)->SetValue(RowName);

	//Hack to refresh the properties
	DataTablePropertyHandle->SetValue(DataTable);
	
	return FReply::Handled();
}
