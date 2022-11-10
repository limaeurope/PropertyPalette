#ifndef _S_PROPERTYGROUP_HPP
#define _S_PROPERTYGROUP_HPP

#include	"../APIEnvir.h"
#include	"ACAPinc.h"					// also includes APIdefs.h
#include	"../APICommon.h"

struct S_PropertyGroup : API_PropertyGroup {
	GS::HashSet<API_Guid> propertieS;
	S_PropertyGroup(API_PropertyGroup& i_group) : API_PropertyGroup(i_group) {};
	GS::HashSet<API_Guid> GetFilteredProperties(GS::UniString const& i_sFilter);
	GS::HashSet<API_Guid> GetPropertiesByType(API_VariantType & i_variant);
};

#endif // !_S_PROPERTYGROUP_HPP

