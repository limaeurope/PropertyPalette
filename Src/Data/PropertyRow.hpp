#ifndef _PROPERTY_ROW_HPP
#define _PROPERTY_ROW_HPP

#include	"../APIEnvir.h"
#include	"ACAPinc.h"					// also includes APIdefs.h
#include	"../APICommon.h"


typedef enum {
	PT_PropertyGroup,
	PT_Property,
	PT_InstanceProperty,
} PropertyType;


class PropertyRow {
	bool			m_isOpened = true;
public:
	PropertyType	m_propertyType;
	inline void SetDisplayItems(bool const i_isOpened) { m_isOpened = i_isOpened; };
	inline bool GetDisplayItems() { return m_isOpened; };
	virtual API_Guid GetGuid() { return APINULLGuid; };
};

#endif // !_PROPERTY_ROW_HPP

