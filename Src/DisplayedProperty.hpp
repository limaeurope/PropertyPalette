#ifndef DISPLAYED_PROPERTY_HPP
#define DISPLAYED_PROPERTY_HPP

#include	"APIEnvir.h"
#include	"ACAPinc.h"					// also includes APIdefs.h
#include	"APICommon.h"
#include	"Enums/OnTabTypes.hpp"

class DisplayedProperty : public API_Property {
	GS::UniString _toUniString(const API_Variant& i_variant);
	OnTabTypes m_onTabType;
	GS::Array<API_Guid> representedPropertieS;
	GS::Array<API_Guid> representedObjectS;
public:
	bool AreAllValuesEqual = true;
	DisplayedProperty(const API_Property& i_prop, const API_Guid& i_guidObj);
	void addExample(const API_Property& i_prop, const API_Guid& i_guidObj);
	GS::UniString toUniString();
	inline OnTabTypes GetOnTabType() const { return m_onTabType; };
	inline void SetOnTabType(const OnTabTypes i_onTabType) { m_onTabType = i_onTabType; };
	operator GS::UniString () { return toUniString(); };
	void operator= (const GS::UniString i_value);
	void operator= (const double i_value);
	void operator= (const int i_value);
};

bool operator== (const API_Variant& i_this, const API_Variant& i_other);

#endif // !DISPLAYED_PROPERTY_HPP

