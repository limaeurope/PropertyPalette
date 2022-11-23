#ifndef _SETTINGS_SINGLETON_HPP
#define _SETTINGS_SINGLETON_HPP

#include	"../APIEnvir.h"
#include	"ACAPinc.h"					// also includes APIdefs.h
#include	"../APICommon.h"

#include	<mutex>
#include	"../Enums/PropertySelectMode.hpp"
#include	"../Enums/FilterTypes.hpp"
#include	"../DisplayedProperty.hpp"
#include	"../Data/S_PropertyGroup.hpp"
#include	"Logger/Logger.hpp"


class SettingsSingleton
{
	SettingsSingleton();
	~SettingsSingleton();
	SettingsSingleton(const SettingsSingleton&);
	void operator=(const SettingsSingleton&) {};
	
	PropertySelectMode							m_propertySelectMode;
	GS::HashTable<short, DisplayedProperty>		m_propIdx;
	//GS::HashTable<short, S_PropertyGroup>		m_groupIdx;
	DisplayedProperty*							m_iPropCurrentlyEdited;
	GS::UniString								m_appName;
	GS::UniString								m_companyName;
	Logger										m_logger;
	GS::UniString								m_sFilter;
	TypeFilter									m_typeFilter;
public:
	static SettingsSingleton& GetInstance();
	
	inline void SetPropertySelectMode(const PropertySelectMode i_propertySelectMode) { m_propertySelectMode = i_propertySelectMode; }
	inline PropertySelectMode GetPropertySelectMode() const { return m_propertySelectMode; };

	inline void AddToPropertyList(const short i_idx, const DisplayedProperty& i_prop) { m_propIdx.Put(i_idx, i_prop); };
	DisplayedProperty* GetCurrentlyEditedProperty(const short i_idx);
	inline DisplayedProperty* GetCurrentlyEditedProperty() { return m_iPropCurrentlyEdited; };

	//inline void AddToGroupList(const short i_idx, const S_PropertyGroup& i_group) { m_groupIdx.Put(i_idx, i_group); };
	//S_PropertyGroup& GetFromGroupList(const short i_idx);

	inline Logger& GetLogger() { return m_logger; };

	inline void SetFilterText(GS::UniString const& i_sFilter) { m_sFilter = i_sFilter.ToLowerCase(); };
	inline GS::UniString GetFilterText() const { return m_sFilter ; } ;

	void SetFilterType(GS::UniString &  i_sTypeFilter);
	inline void SetFilterType(TypeFilter & i_typeFilter) { m_typeFilter = i_typeFilter; }
	inline TypeFilter GetFilterType() const { return m_typeFilter; };
};

static std::mutex _mutex;
extern SettingsSingleton& (*SETTINGS)();

#endif // !_SETTINGS_SINGLETON_HPP

