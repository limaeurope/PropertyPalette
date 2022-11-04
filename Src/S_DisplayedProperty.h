#ifndef DISPLAYED_PROPERTY_HPP
#define DISPLAYED_PROPERTY_HPP

#include	"APIEnvir.h"
#include	"ACAPinc.h"					// also includes APIdefs.h
#include	"APICommon.h"


class S_DisplayedProperty : public API_Property {
	bool m_isValuesEqual = true;
	GS::UniString _toUniString(API_Variant i_variant);
public:
	GS::Array<API_Guid> propertieS;
	S_DisplayedProperty(API_Property& i_prop): API_Property(i_prop) { };
	void addExample(API_Property& i_prop);
	GS::UniString toUniString();
};

#endif // !DISPLAYED_PROPERTY_HPP

