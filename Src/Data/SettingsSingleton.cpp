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

DisplayedProperty SettingsSingleton::GetFromPropertyList(const short i_idx)
{
	if (!m_propIdx.ContainsKey(i_idx))
		throw std::exception();

	m_iPropCurrentlyEdited = &m_propIdx[i_idx];

	return m_propIdx[i_idx];
}

