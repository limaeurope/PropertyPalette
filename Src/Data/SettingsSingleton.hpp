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

	//GS::HashTable<short, DisplayedProperty>		m_propIdx;
	//GS::HashTable<short, S_PropertyGroup>		m_groupIdx;

	GS::HashTable<API_Guid, PropertyRow*>		m_rowIdx;
	GS::HashTable<short, API_Guid>				m_guidIdx;

	DisplayedProperty*							m_iPropCurrentlyEdited;
	PropertyRow*								m_selectedRow;
	GS::UniString								m_appName;
	GS::UniString								m_companyName;
	Logger										m_logger;
	GS::UniString								m_sFilter;
	TypeFilter									m_typeFilter;
	GS::UniString								m_HelpURL;
public:
	static SettingsSingleton& GetInstance();
	
	inline void SetPropertySelectMode(const PropertySelectMode i_propertySelectMode) { m_propertySelectMode = i_propertySelectMode; }
	inline PropertySelectMode GetPropertySelectMode() const { return m_propertySelectMode; };

	inline DisplayedProperty* GetCurrentlyEditedProperty() { return m_iPropCurrentlyEdited; };
	DisplayedProperty* GetCurrentlyEditedProperty(const short i_idx);

	PropertyRow* GetSelectedRow(const short i_idx);

	inline void AddToRowList(const API_Guid i_guid, PropertyRow * const i_row) { m_rowIdx.Put(i_guid, i_row); };
	PropertyRow* GetFromRowList(const short i_idx);
	PropertyRow* GetFromRowList(const API_Guid i_guid);

	inline void AddToGuidList(const short i_idx, const API_Guid i_guid) { m_guidIdx.Put(i_idx, i_guid); };
	API_Guid GetFromGuidList(const short i_idx);

	PropertyRow* AddOrUpdateLists(const short i_idx, PropertyRow * const i_row);
	
	inline Logger& GetLogger() { return m_logger; };

	inline void SetFilterText(GS::UniString const& i_sFilter) { m_sFilter = i_sFilter.ToLowerCase(); };
	inline GS::UniString GetFilterText() const { return m_sFilter ; } ;

	void SetFilterType(GS::UniString &  i_sTypeFilter);
	inline void SetFilterType(TypeFilter & i_typeFilter) { m_typeFilter = i_typeFilter; }
	inline TypeFilter GetFilterType() const { return m_typeFilter; };

	inline GS::UniString GetAppName() const { return m_appName; };
	inline GS::UniString GetCompanyName() const { return m_companyName; };
	inline wchar_t* GetHelpURL() const { return (wchar_t*)m_HelpURL.ToUStr().Get(); };
};

static std::mutex _mutex;
extern SettingsSingleton& (*SETTINGS)();

#endif // !_SETTINGS_SINGLETON_HPP

