// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once
#include "IPropertyTypeCustomization.h"
#include "PropertyCustomizationHelpers.h"

/**
* Detail customization for PS4 target settings panel
*/
class FQuestLabelCollectionDetails : public IPropertyTypeCustomization
{
public:
	FQuestLabelCollectionDetails();
	
	/** Makes a new instance of this detail layout class for a specific detail view requesting it */
	static TSharedRef<IPropertyTypeCustomization> MakeInstance();

	/** IDetailCustomization interface */
	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils) override;
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils) override;

	FReply OnLabelClicked(const FName RowName, const TSharedRef<IPropertyHandle> PropertyHandle) const;

protected:
	FButtonStyle HighlightedStyle;

	FButtonStyle DefaultStyle;

	FTextBlockStyle HighlightedTextStyle;

	FTextBlockStyle DefaultTextStyle;
};