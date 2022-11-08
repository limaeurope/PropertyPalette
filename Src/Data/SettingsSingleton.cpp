#include "SettingsSingleton.hpp"

SettingsSingleton& (*SETTINGS)() = SettingsSingleton::GetInstance;

SettingsSingleton::SettingsSingleton()
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
	m_iPropCurrentlyEdited = &m_propIdx[i_idx];

	return m_propIdx[i_idx];
}

