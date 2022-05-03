// Copyright 2018 Jianzhao Fu. All Rights Reserved.
#include "ExcelWorkbook.h"
//#include "AllowWindowsPlatformTypes.h"
#ifdef WIN32
#include "Windows/WindowsHWrapper.h"
//#include "HideWindowsPlatformTypes.h"
#endif

#include "Misc/Paths.h"
#include "ExcelWorksheet.h"
#include "LogTypes.h"
#include "Engine.h"
#include "DirectExcelLibrary.h"


// Create a new workbook by reading sample1.xlsx in the current directory.
//xlnt::workbook wb;
//wb.load("sample1.xlsx");

//// The workbook class has begin and end methods so it can be iterated upon.
//// Each item is a sheet in the workbook.
//for (const auto sheet : wb)
//{
//	// Print the title of the sheet on its own line.
//	std::cout << sheet->get_title() << ": " << std::endl;

//	// Iterating on a range, such as from worksheet::rows, yields cell_vectors.
//	// Cell vectors don't actually contain cells to reduce overhead.
//	// Instead they hold a reference to a worksheet and the current cell_reference.
//	// Internally, calling worksheet::get_cell with the current cell_reference yields the next cell.
//	// This allows easy and fast iteration over a row (sometimes a column) in the worksheet.
//	for (auto row : sheet->rows())
//	{
//		for (auto cell : row)
//		{
//			// cell::operator<< adds a string represenation of the cell's value to the stream.
//			std::cout << cell << ", ";
//		}

//		std::cout << std::endl;
//	}
//}

/*xlnt::workbook wb;
xlnt::worksheet ws = wb.active_sheet();

ws.cell("A1").value(5);
ws.cell("B2").value("string data");
ws.cell("C3").formula("=RAND()");

ws.merge_cells("C3:C4");
ws.freeze_panes("B2");

wb.save("sample.xlsx");

printf("Hello");*/

FExcelVariant ToExcelVariant(const xlnt::variant& value)
{
	switch (value.value_type())
	{
	case xlnt::variant::type::boolean:
		return FExcelVariant(value.get<bool>());
	case xlnt::variant::type::i4:
		return FExcelVariant(value.get<int32>());
	case xlnt::variant::type::lpstr:
	{
		std::string str = value.get<std::string>();
		return FExcelVariant(FString(str.c_str()));
	}
	case xlnt::variant::type::date:
	{
		xlnt::datetime date = value.get<xlnt::datetime>();
		FDateTime fDate(date.year, date.month, date.day, date.hour, date.minute, date.second, date.microsecond);
		return FExcelVariant(fDate);
	}
	case xlnt::variant::type::vector:
	{
		FExcelVariant result;
		result.SetType(ExcelVariantType::Array);
		std::vector<xlnt::variant> arrayValues = value.get<std::vector<xlnt::variant>>();
		for (const xlnt::variant& v : arrayValues)
		{
			FExcelVariant temp = ToExcelVariant(v);
			result.MutableArrayValue().Add(temp);
		}
		return result;
	}

	}

	return FExcelVariant();
}

xlnt::variant ToXlntVariant(const FExcelVariant& value)
{
	switch (value.Type())
	{
	case ExcelVariantType::Bool:
		return xlnt::variant(value.BoolValue());
	case ExcelVariantType::Int32:
		return xlnt::variant(value.IntValue());
	case ExcelVariantType::String:
	{
		FString str = value.StringValue();
		std::string str2 = TCHAR_TO_UTF8(*str);
		return xlnt::variant(str2);
	}
	case ExcelVariantType::DateTime:
	{
		FDateTime date = value.DateTimeValue();
		xlnt::datetime d(date.GetYear(), date.GetMonth(), date.GetDay(), date.GetHour(), date.GetMinute(), date.GetSecond(), date.GetMillisecond());
		return xlnt::variant(d);
	}
	case ExcelVariantType::Array:
	{
		std::vector<xlnt::variant> items;
		for (const FExcelVariant& v : value.ArrayValue())
		{
			xlnt::variant temp = ToXlntVariant(v);
			items.push_back(temp);
		}
		return xlnt::variant(items);
	}
	break;
	default:
		break;
	}

	return xlnt::variant();
}

UExcelWorkbook::UExcelWorkbook()
{
	mFindHandle = NULL;

	mData = new xlnt::workbook();
	InitSheets();
}

UExcelWorkbook::~UExcelWorkbook()
{
	StopWatch(nullptr);
	Unload();
}



bool UExcelWorkbook::Load(FString path, ExcelFileRelateiveDir relativeDir /*= ExcelFileRelateiveDir::Absolute*/)
{
	if (path.IsEmpty())
	{
		UE_LOG(LogDirectExcel, Warning, TEXT("Fullpath is empty."));
		return false;
	}

	path = UDirectExcelLibrary::ToAbsolutePath(path, relativeDir);
	mPath = path;
	if (mData == nullptr)
	{
		mData = new xlnt::workbook();
	}

	TArray<uint8> fileData;
	if (!FFileHelper::LoadFileToArray(fileData, *mPath, FILEREAD_AllowWrite))
	{
		return false;
	}

	std::vector<std::uint8_t> copyData;
	copyData.reserve(fileData.Num());
	for (uint8 val : fileData)
	{
		copyData.push_back(val);
	}

	if (!mData->load(copyData))
	{
		return false;
	}
	InitSheets();
	return true;
}

bool UExcelWorkbook::Load(const std::vector<std::uint8_t>& data)
{
	if (mData == nullptr)
	{
		mData = new xlnt::workbook();
	}
	if (!mData->load(data))
	{
		return false;
	}

	InitSheets();

	return true;
}

bool UExcelWorkbook::SaveAs(FString path, ExcelFileRelateiveDir relativeDir /*= ExcelFileRelateiveDir::ProjectSavedDir*/)
{
	std::vector<uint8> outExcelData;
	if (!mData->save(outExcelData))
	{
		UE_LOG(LogDirectExcel, Warning, TEXT("Failed to save to data."));
		return false;
	}

	path = UDirectExcelLibrary::ToAbsolutePath(path, relativeDir);
	if (path.IsEmpty())
	{
		return false;
	}
	//std::string pathStr = TCHAR_TO_UTF8(*path);
	TArrayView<uint8> dataView(outExcelData.data(), outExcelData.size());
	if (!FFileHelper::SaveArrayToFile(dataView, *path))
	{
		UE_LOG(LogDirectExcel, Warning, TEXT("Failed to save to %s."), *path);
		return false;
	}

	UE_LOG(LogDirectExcel, Warning, TEXT("Success Save dataSize:%d to %s."), outExcelData.size(), *path);
	return true;
}


bool UExcelWorkbook::Save()
{
	return SaveAs(mPath, ExcelFileRelateiveDir::Absolute);
}

bool UExcelWorkbook::Save(std::vector<std::uint8_t>& outData)
{
	if (!mData->save(outData))
	{
		UE_LOG(LogDirectExcel, Warning, TEXT("Failed to save to data."));
		return false;
	}
	return true;
}

void UExcelWorkbook::Clear()
{
	mData->clear();
}

void UExcelWorkbook::InitSheets()
{
	xlnt::worksheet activeSheet = mData->active_sheet();

	mSheets.Empty();
	size_t count = mData->sheet_count();
	for (size_t i = 0; i < count; ++i)
	{
		xlnt::worksheet ws = mData->sheet_by_index(i);
		UExcelWorksheet* sheet = NewObject<UExcelWorksheet>();
		sheet->Initialize(this, ws);
		mSheets.Add(sheet);

		if (ws == activeSheet)
		{
			mActiveSheet = sheet;
		}
	}

}

void UExcelWorkbook::Unload()
{
	if (mData != nullptr)
	{
		delete mData;
		mData = nullptr;
	}
}

bool UExcelWorkbook::Reload()
{
	Unload();
	return Load(mPath);
}

void UExcelWorkbook::SetTableHeaders(const TMap<FName, FExcelTableHeader>& tableHeaders)
{
	for (const auto& kv : tableHeaders)
	{
		FString title = kv.Key.ToString();
		UExcelWorksheet* sheet = SheetWithTitle(title);
		if (!sheet)
		{
			UE_LOG(LogDirectExcel, Warning, TEXT("Cannot find sheet with title:%s"), *title);
			continue;
		}

		sheet->SetTableHeader(kv.Value);
	}
}


bool UExcelWorkbook::StartWatch(const UObject* WorldContextObject, FExcelWorkbookChangedDelegate onChanged, bool autoReload /*= true*/, float rate/*=1.f*/)
{
#ifdef WIN32

	//wchar_t watchDirectory[512] = { 0 };
	//MultiByteToWideChar(CP_ACP, 0, (char*)*mPath, strlen((char*)*mPath), watchDirectory, sizeof(watchDirectory) / sizeof(wchar_t));
	FString path = FPaths::GetPath(mPath);
	const wchar_t* p = *path;

	mFindHandle = ::FindFirstChangeNotification(p, 0, FILE_NOTIFY_CHANGE_LAST_WRITE);
	if (INVALID_HANDLE_VALUE == mFindHandle)
	{
		DWORD errorCode = GetLastError();
		UE_LOG(LogDirectExcel, Error, L"%d", errorCode);
		return false;
	}

	UWorld* world = GEngine->GetWorldFromContextObjectChecked(WorldContextObject);
	world->GetTimerManager().SetTimer(mWatchTimer, [this, WorldContextObject, onChanged, autoReload]
		{
			DWORD dwWait = ::WaitForSingleObject(mFindHandle, 0);
			if (dwWait == WAIT_FAILED)
			{
				DWORD errorCode = GetLastError();
				UE_LOG(LogDirectExcel, Error, L"Watch Error: %d", errorCode);
				StopWatch(WorldContextObject);
				return;
			}

			if (WAIT_OBJECT_0 == dwWait)
			{
				bool isOpen = true;
				if (autoReload)
				{
					isOpen = Reload();
				}

				if (isOpen)
				{
					onChanged.ExecuteIfBound(this);
				}
				else
				{
					onChanged.ExecuteIfBound(nullptr);
				}

				if (!::FindNextChangeNotification(mFindHandle))
				{
					::FindCloseChangeNotification(mFindHandle);
					mFindHandle = NULL;
					UE_LOG(LogDirectExcel, Warning, TEXT("Lost watch."));
				}
			}

		}, rate, true);
#endif

	return true;

}

bool UExcelWorkbook::StopWatch(const UObject* WorldContextObject)
{
#ifdef WIN32

	if (mWatchTimer.IsValid() && WorldContextObject != nullptr)
	{
		UWorld* world = GEngine->GetWorldFromContextObjectChecked(WorldContextObject);
		world->GetTimerManager().ClearTimer(mWatchTimer);
	}

	if (INVALID_HANDLE_VALUE != mFindHandle)
	{
		::FindCloseChangeNotification(mFindHandle);
		mFindHandle = NULL;
	}
#endif
	return true;
}





const TArray<UExcelWorksheet*>& UExcelWorkbook::AllSheets()const
{
	return mSheets;
}

int32 UExcelWorkbook::SheetCount() const
{
	return (int32)mData->sheet_count();
}

TArray<FString> UExcelWorkbook::SheetTitles()const
{
	TArray<FString> result;

	auto titles = mData->sheet_titles();
	for (auto& title : titles)
	{
		result.Emplace(title.c_str());
	}
	return result;
}

UExcelWorksheet* UExcelWorkbook::SheetAt(int32 index)
{
	if (mSheets.IsValidIndex(index))
	{
		return mSheets[index];
	}
	return nullptr;
}

UExcelWorksheet* UExcelWorkbook::SheetWithId(int32 id)
{
	for (UExcelWorksheet* sheet : mSheets)
	{
		if (sheet->Data().id() == id)
		{
			return sheet;
		}
	}
	return nullptr;
}

UExcelWorksheet* UExcelWorkbook::SheetWithTitle(FString title)
{
	std::string str = TCHAR_TO_UTF8(*title);

	for (UExcelWorksheet* sheet : mSheets)
	{
		if (sheet->Data().title() == str)
		{
			return sheet;
		}
	}
	return nullptr;
}

bool UExcelWorkbook::ContainsSheet(FString title) const
{
	std::string str = TCHAR_TO_UTF8(*title);

	return mData->contains(str);
}

UExcelWorksheet* UExcelWorkbook::ActiveSheet()
{
	return mActiveSheet;
}

void UExcelWorkbook::SetActiveSheet(UExcelWorksheet* sheet)
{
	if (sheet != nullptr)
	{
		mActiveSheet = sheet;
	}
}

void UExcelWorkbook::SetActiveSheetAt(int32 index)
{
	SetActiveSheet(SheetAt(index));
}

void UExcelWorkbook::SetActiveSheetWithTitle(FString title)
{
	SetActiveSheet(SheetWithTitle(title));
}

UExcelWorksheet* UExcelWorkbook::CreateSheet(int32 index /*= -1*/, bool setActive /*= true*/)
{
	UExcelWorksheet* sheet = NewObject<UExcelWorksheet>();
	sheet->SetWorkbook(this);
	if (index < 0)
	{
		xlnt::worksheet ws = mData->create_sheet_default();
		sheet->SetData(ws);
	}
	else
	{
		if (index > SheetCount())
		{
			//UE_LOG(LogDirectExcel, Error, L"Index:%d cannot be bigger than sheet count:%d", index, SheetCount());
		}

		index = FMath::Clamp(index, 0, SheetCount());
		xlnt::worksheet ws = mData->create_sheet_default(index);
		sheet->SetData(ws);
	}

	mSheets.Add(sheet);
	if (setActive)
	{
		mActiveSheet = sheet;
	}
	return sheet;
}

void UExcelWorkbook::RemoveSheet(UExcelWorksheet* sheet)
{
	mData->remove_sheet(sheet->Data());
}

UExcelWorksheet* UExcelWorkbook::CopySheet(UExcelWorksheet* sheet, int32 index /*= -1*/)
{
	UExcelWorksheet* newSheet = NewObject<UExcelWorksheet>();
	newSheet->SetWorkbook(this);
	if (index < 0)
	{
		xlnt::worksheet newws = mData->copy_sheet(sheet->Data());
		newSheet->SetData(newws);
	}
	else
	{
		xlnt::worksheet newws = mData->copy_sheet(sheet->Data(), index);
		newSheet->SetData(newws);
	}
	mSheets.Add(newSheet);
	return newSheet;
}




bool UExcelWorkbook::HasCoreProperty(ExcelCoreProperty property) const
{

	return mData->has_core_property((xlnt::core_property)property);
}

TArray<ExcelCoreProperty> UExcelWorkbook::AllCoreProperties() const
{
	TArray<ExcelCoreProperty> result;

	result.Append((const ExcelCoreProperty*)mData->core_properties().data(), mData->core_properties().size());
	return result;
}


FExcelVariant UExcelWorkbook::GetCorePropertyValue(ExcelCoreProperty property) const
{

	xlnt::variant v = mData->core_property((xlnt::core_property)property);
	return ToExcelVariant(v);
}

void UExcelWorkbook::SetCorePropertyValue(ExcelCoreProperty property, FExcelVariant value)
{

	mData->core_property((xlnt::core_property)property, ToXlntVariant(value));
}

bool UExcelWorkbook::HasExtendedProperty(ExcelExtendedProperty property) const
{

	return mData->has_extended_property((xlnt::extended_property)property);
}

TArray<ExcelExtendedProperty> UExcelWorkbook::AllExtendedProperties() const
{
	TArray<ExcelExtendedProperty> result;

	result.Append((const ExcelExtendedProperty*)mData->extended_properties().data(), mData->extended_properties().size());
	return result;
}

FExcelVariant UExcelWorkbook::GetExtendedPropertyValue(ExcelExtendedProperty property) const
{

	xlnt::variant v = mData->extended_property((xlnt::extended_property)property);
	return ToExcelVariant(v);
}

void UExcelWorkbook::SetExtendedPropertyValue(ExcelExtendedProperty property, FExcelVariant value)
{

	mData->extended_property((xlnt::extended_property)property, ToXlntVariant(value));
}

bool UExcelWorkbook::HasCustomProperty(FString name) const
{
	std::string str = TCHAR_TO_UTF8(*name);

	return mData->has_custom_property(str);
}

TArray<FString> UExcelWorkbook::AllCustomProperties() const
{

	TArray<FString> result;
	for (std::string str : mData->custom_properties())
	{
		result.Emplace(str.c_str());
	}

	return result;
}

FExcelVariant UExcelWorkbook::GetCustomProperty(FString name) const
{

	std::string str = TCHAR_TO_UTF8(*name);
	xlnt::variant val = mData->custom_property(str);
	return ToExcelVariant(val);
}

void UExcelWorkbook::SetCustomProperty(FString name, FExcelVariant value)
{

	std::string nameStr = TCHAR_TO_UTF8(*name);
	mData->custom_property(nameStr, ToXlntVariant(value));
}



void UExcelWorkbook::ApplyToCells(const UObject* WorldContextObject, FExcelCellApplyDelegate onCell)
{
	mData->apply_to_cells(
		[this, onCell](xlnt::cell eachCell)
		{
			UExcelCell* cell = NewObject<UExcelCell>();
			cell->SetWorksheet((UExcelWorksheet*)this);
			cell->SetData(eachCell);
			onCell.ExecuteIfBound(cell);
		}
	);
}




ExcelBaseDate UExcelWorkbook::BaseDate() const
{

	return (ExcelBaseDate)mData->base_date();
}

void UExcelWorkbook::SetBaseDate(ExcelBaseDate baseDate)
{

	mData->base_date((xlnt::calendar)baseDate);
}

bool UExcelWorkbook::HasTitle() const
{

	return mData->has_title();
}

FString UExcelWorkbook::Title() const
{

	return FString(mData->title().c_str());
}

void UExcelWorkbook::SetTitle(FString value)
{
	std::string str = TCHAR_TO_UTF8(*value);
	mData->title(str);
}

