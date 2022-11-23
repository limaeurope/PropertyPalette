#include "SettingsSingleton.hpp"
#include "Utils/Utils.hpp"
#include <exception>

SettingsSingleton& (*SETTINGS)() = SettingsSingleton::GetInstance;

SettingsSingleton::SettingsSingleton()
	:m_companyName				(GetStringFromResource_(32506, 1))
	,m_appName					(GetStringFromResource_(32000, 1))
	,m_logger					(Logger(m_companyName, m_appName))
	,m_iPropCurrentlyEdited	(NULL)
{
	m_propertySelectMode = PSM_Intersection;
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
	if (!m_propIdx.ContainsKey(i_idx))
		throw std::exception();

	m_iPropCurrentlyEdited = &m_propIdx[i_idx];

	return &m_propIdx[i_idx];
}

//S_PropertyGroup& SettingsSingleton::GetFromGroupList(const short i_idx)
//{
//	if (!m_groupIdx.ContainsKey(i_idx))
//		throw std::exception();
//
//	return m_groupIdx[i_idx];
//}

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

