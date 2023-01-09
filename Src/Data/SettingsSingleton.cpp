#include "SettingsSingleton.hpp"
#include "Utils/Utils.hpp"
#include <exception>
#include "WinReg/WinReg.hpp"

SettingsSingleton& (*SETTINGS)() = SettingsSingleton::GetInstance;

SettingsSingleton::SettingsSingleton()
	:m_companyName				(GetStringFromResource_(32506, 1))
	,m_appName					(GetStringFromResource_(32000, 1))
	,m_logger					(Logger(m_companyName, m_appName))
	,m_iPropCurrentlyEdited		(NULL)
{
	m_propertySelectMode	= PSM_Intersection;
	m_typeFilter			= FT_None;
	const wchar_t* _URL		= L"https://limadesignkft.sharepoint.com/sites/bim/BIM%20developer%20wiki/PropertyPalette.aspx";
	m_HelpURL				= GetRegStringOrSetDefault(_URL, "HelpURL", "SOFTWARE\\" + GetCompanyName() + "\\" + GetAppName());
}

SettingsSingleton::~SettingsSingleton()
{
}

SettingsSingleton& SettingsSingleton::GetInstance()
{
	std::lock_guard<std::mutex> lock(_mutex);

	static SettingsSingleton singleton;

	return singleton;
}

DisplayedProperty* SettingsSingleton::GetCurrentlyEditedProperty(const short i_idx)
{
	//if (!m_propIdx.ContainsKey(i_idx))
	//	throw std::exception();

	//m_iPropCurrentlyEdited = &m_propIdx[i_idx];

	//return &m_propIdx[i_idx];

	if (!m_guidIdx.ContainsKey(i_idx))
		throw std::exception();

	auto _guid = m_guidIdx[i_idx];

	auto _dp = static_cast<DisplayedProperty*>(m_rowIdx[_guid]);

	m_iPropCurrentlyEdited = _dp;

	return _dp;
}

PropertyRow* SettingsSingleton::GetSelectedRow(const short i_idx)
{
	if (!m_guidIdx.ContainsKey(i_idx))
		throw std::exception();

	auto _guid = m_guidIdx[i_idx];

	m_selectedRow = m_rowIdx[_guid];

	return m_rowIdx[_guid];
}

PropertyRow* SettingsSingleton::GetFromRowList(const short i_idx)
{
	API_Guid _guid = GetFromGuidList(i_idx);

	if (!m_rowIdx.ContainsKey(_guid))
		throw std::exception();

	return m_rowIdx[_guid];
}

PropertyRow* SettingsSingleton::GetFromRowList(const API_Guid i_guid)
{
	if (!m_rowIdx.ContainsKey(i_guid))
		throw std::exception();

	return m_rowIdx[i_guid];
}

PropertyRow* SettingsSingleton::AddOrUpdateLists(const short i_idx, PropertyRow* const i_row)
{
	API_Guid _guid = i_row->GetGuid();

	if (m_rowIdx.ContainsKey(_guid))
	{
		auto _dp = dynamic_cast<DisplayedProperty*>(i_row);
		auto _dp2 = dynamic_cast<API_Property*>(m_rowIdx[_guid]);
		if (_dp)
			*_dp2 = *_dp;

		auto _spg = dynamic_cast<S_PropertyGroup*>(i_row);
		auto _spg2 = dynamic_cast<API_PropertyGroup*>(m_rowIdx[_guid]);
		if (_spg)
			*_spg2 = *_spg;
	}
	else
	{
		m_rowIdx.Put(_guid, i_row);
	}

	m_guidIdx.Put(i_idx, _guid);

	return m_rowIdx[_guid];
}

API_Guid SettingsSingleton::GetFromGuidList(const short i_idx)
{
	if (!m_guidIdx.ContainsKey(i_idx))
		throw std::exception();

	return m_guidIdx[i_idx];
}

void SettingsSingleton::SetFilterType(GS::UniString& i_sTypeFilter)
{
	if (i_sTypeFilter == "")
		m_typeFilter = FT_None;
	else if (i_sTypeFilter == "Integer")
		m_typeFilter = FT_Int;
	else if (i_sTypeFilter == "Real")
		m_typeFilter = FT_Real;
	else if (i_sTypeFilter == "Area")
		m_typeFilter = FT_Area;
	else if (i_sTypeFilter == "Length")
		m_typeFilter = FT_Length;
	else if (i_sTypeFilter == "Area")
		m_typeFilter = FT_Area;
	else if (i_sTypeFilter == "Volume")
		m_typeFilter = FT_Volume;
	else if (i_sTypeFilter == "Boolean")
		m_typeFilter = FT_Bool;
	else if (i_sTypeFilter == "String")
		m_typeFilter = FT_String;
	else if (i_sTypeFilter == "List")
		m_typeFilter = FT_List;
	else if (i_sTypeFilter == "Single Selection Enum")
		m_typeFilter = FT_SingleEnum;
	else if (i_sTypeFilter == "Multi Selection Enum")
		m_typeFilter = FT_MultiEnum;
}

