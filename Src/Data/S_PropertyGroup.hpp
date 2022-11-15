#ifndef _S_PROPERTYGROUP_HPP
#define _S_PROPERTYGROUP_HPP

#include	"../APIEnvir.h"
#include	"ACAPinc.h"					// also includes APIdefs.h
#include	"../APICommon.h"

class S_PropertyGroup : public API_PropertyGroup {
	bool	m_isVisible = true;
public:
	GS::HashSet<API_Guid> propertieS;
	S_PropertyGroup(API_PropertyGroup& i_group) : API_PropertyGroup(i_group) {};
	GS::HashSet<API_Guid> GetFilteredProperties(GS::UniString const& i_sFilter);
	GS::HashSet<API_Guid> GetPropertiesByType(API_VariantType & i_variant);
	inline void SetVisibility(bool const i_isVisible) { m_isVisible = i_isVisible; };
	inline bool GetVisibility() { return m_isVisible; };
};

#endif // !_S_PROPERTYGROUP_HPP

