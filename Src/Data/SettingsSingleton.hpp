#ifndef _SETTINGS_SINGLETON_HPP
#define _SETTINGS_SINGLETON_HPP

#include	"../APIEnvir.h"
#include	"ACAPinc.h"					// also includes APIdefs.h
#include	"../APICommon.h"

#include	<mutex>
#include	"../Enums/PropertySelectMode.hpp"
#include	"../DisplayedProperty.hpp"


class SettingsSingleton
{
	SettingsSingleton();
	~SettingsSingleton();
	SettingsSingleton(const SettingsSingleton&);
	void operator=(const SettingsSingleton&) {};
	
	PropertySelectMode m_propertySelectMode;
	GS::HashTable<short, DisplayedProperty> m_propIdx;
	DisplayedProperty* m_iPropCurrentlyEdited;
public:
	static SettingsSingleton& GetInstance();
	
	inline void SetPropertySelectMode(const PropertySelectMode i_propertySelectMode) { m_propertySelectMode = i_propertySelectMode; }
	inline PropertySelectMode GetPropertySelectMode() const { return m_propertySelectMode; };

	inline void AddToPropertyList(const short i_idx, const DisplayedProperty& i_prop) { m_propIdx.Put(i_idx, i_prop); };
	DisplayedProperty GetFromPropertyList(const short i_idx);
	inline DisplayedProperty GetFromPropertyList() { return *m_iPropCurrentlyEdited; };
};

static std::mutex _mutex;
extern SettingsSingleton& (*SETTINGS)();

#endif // !_SETTINGS_SINGLETON_HPP

