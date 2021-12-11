// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestLabelCollectionDetails.h"

#include "DetailCategoryBuilder.h"
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
	
	
	DataTable->ForeachRow<FQuestLabelRow>("FQuestLabelCollectionDetails::CustomizeDetails",
		[this, &LabelBox](const FName& RowName, const FQuestLabelRow& DataRow)
		{
			FName Label = DataRow.Label;
			
			LabelBox->AddSlot()
			.Padding(3,2)
			[
				SNew(SButton)
				.Text(FText::FromName(Label))
			];
		
		}
	);
}
