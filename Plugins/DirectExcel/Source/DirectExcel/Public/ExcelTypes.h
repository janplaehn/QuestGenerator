// Copyright 2018 Jianzhao Fu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ThirdParty.h"

#include "ExcelTypes.generated.h"


/**
*Excel cell's sort type
*/
UENUM(Blueprintable, BlueprintType, Category = "DirectExcel")
enum class ExcelSortType :uint8
{
	None,
	Ascending,
	Descending
};

/**
*Excel property value type
*/
UENUM(Blueprintable, BlueprintType,Category="DirectExcel")
enum class ExcelVariantType :uint8
{
	None,
	Bool,
	Int32,
	String,
	DateTime,
	Array,
};


/**
*Excel date's base
*/
UENUM(Blueprintable, BlueprintType, Category = "DirectExcel")
enum class ExcelBaseDate :uint8
{
	Windows1900,
	Mac1904
};

/**
*Excel cell data type
*/
UENUM(Blueprintable, BlueprintType, Category = "DirectExcel")
enum class ExcelCellType :uint8
{
	/// no value
	Empty,
	/// value is TRUE or FALSE
	Boolean,
	/// value is an ISO 8601 formatted date
	Date,
	/// value is a known error code such as \#VALUE!
	Error,
	/// value is a string stored in the cell
	InlineString,
	/// value is a number
	Number,
	/// value is a string shared with other cells to save space
	SharedString,
	/// value is the string result of a formula
	FormulaString
};
/**
*Excel core property type
*/
UENUM(Blueprintable, BlueprintType, Category = "DirectExcel")
enum class ExcelCoreProperty :uint8
{
	Category,
	Content_status,
	Created,	//date
	Creator,//string
	Description,
	Identifier,
	Keywords,
	Language,
	LastModifiedBy,	//string
	LastPrinted,//string
	Modified,	//date
	Revision,
	Subject,
	Title,//string
	Version
};

/**
*Excel extended property type
*/
UENUM(Blueprintable, BlueprintType, Category = "DirectExcel")
enum class ExcelExtendedProperty :uint8
{
	Application,//string
	AppVersion,//string
	Characters,
	CharactersWithWpaces,
	Company,//string
	Digsig,
	DocSecurity,	//int32
	HeadingPairs,
	HiddenSlides,
	HLinks,
	HyperlinkBase,
	HyperlinksChanged,//bool
	Lines,
	LinksUpToDate,	//bool
	Manager,
	MMClips,
	Notes,
	Pages,
	Paragraphs,
	PresentationFormat,
	ScaleCrop,//bool
	SharedDoc,//bool
	Slides,
	Template,
	TitlesOfParts,
	TotalTime,
	Words
};

/**
*Excel cell's Horizontal Alignment
*/
UENUM(Blueprintable, BlueprintType, Category = "DirectExcel")
enum class ExcelHorizontalAlignment :uint8
{
	General,
	Left,
	Center,
	Right,
	Fill,
	Justify,
	CenterContinuous,
	Distributed
};


/**
*Excel cell's Vertical Alignment
*/
UENUM(Blueprintable, BlueprintType, Category = "DirectExcel")
enum class ExcelVerticalAlignment :uint8
{
	Top,
	Center,
	Bottom,
	Justify,
	Distributed
};

/**
 Cells can have borders that go from the top-left to bottom-right
or from the top-right to bottom-left, or both, or neither.
Used by style->border.
*/
UENUM(Blueprintable, BlueprintType, Category = "DirectExcel")
enum class ExcelBorderDiagonalDirection :uint8
{
	None,
	Up,
	Down,
	Both
};

/**
*Excel cell's border side
*/
UENUM(Blueprintable, BlueprintType, Category = "DirectExcel")
enum class ExcelBorderSideType :uint8
{
	Start,
	End,
	Top,
	Bottom,
	Diagonal,
	Vertical,
	Horizontal
};

/**
*Excel cell's border style
*/
UENUM(Blueprintable, BlueprintType, Category = "DirectExcel")
enum class ExcelBorderStyle :uint8
{
	None,
	DashDot,
	DashDotDot,
	Dashed,
	Dotted,
	Double,
	Hair,
	Medium,
	MediumDashDot,
	MediumDashDotDot,
	MediumDashed,
	SlantDashDot,
	Thick,
	Thin
};

/**
*Excel cell's border style
*/
UENUM(Blueprintable, BlueprintType, Category = "DirectExcel")
enum class ExcelColorType :uint8
{
	Indexed,
	Theme,
	RGB
};

/**
*Excel cell's pattern fill type
*/
UENUM(Blueprintable, BlueprintType, Category = "DirectExcel")
enum class ExcelPatternFillType:uint8
{
	None,
	Solid,
	Mediumgray,
	Darkgray,
	LightGray,
	DarkHorizontal,
	DarkVertical,
	DarkDown,
	DarkUp,
	DarkGrid,
	DarkTrellis,
	LightHorizontal,
	LightVertical,
	LightDown,
	LightUp,
	LightGrid,
	LightTrellis,
	Gray125,
	Gray0625
};

/**
*Excel cell's gradient fill type
*/
UENUM(Blueprintable, BlueprintType, Category = "DirectExcel")
enum class ExcelGradientFillType :uint8
{
	Linear,
	Path
};

/**
*Excel cell's fill type
*/
UENUM(Blueprintable, BlueprintType, Category = "DirectExcel")
enum class ExcelFillType :uint8
{
	Pattern,
	Gradient
};


/**
*Excel cell's font underline type
*/
UENUM(Blueprintable, BlueprintType, Category = "DirectExcel")
enum class ExcelUnderlineType :uint8
{
	None,
	Double,
	DoubleAccounting,
	Single,
	SingleAccounting
};


/**
*Excel file's directory, used when load/save file
*/
UENUM(Blueprintable, BlueprintType, Category = "DirectExcel")
enum class ExcelFileRelateiveDir :uint8
{
	Absolute,	//this is a absolute path
	ProjectDir,
	ProjectContentDir,
	ProjectConfigDir,
	ProjectSavedDir,
	ProjectPluginsDir,
	ProjectModsDir
};


UENUM(Blueprintable, BlueprintType, Category = "DirectExcel")
enum class ExcelTableFormat :uint8
{
	Normal,
	TableWithFirstRowColumnHeader,
	UnrealDataTableExportCSV,
};
