#ifndef _S_PROPERTY_GROUP_HPP
#define _S_PROPERTY_GROUP_HPP

#include	"../APIEnvir.h"
#include	"ACAPinc.h"					// also includes APIdefs.h
#include	"../APICommon.h"
#include	"PropertyRow.hpp"


class S_PropertyGroup : public API_PropertyGroup, public PropertyRow {
public:
	GS::HashSet<API_Guid> propertieS;
	S_PropertyGroup(API_PropertyGroup& i_group) : API_PropertyGroup(i_group) {};
	GS::HashSet<API_Guid> GetFilteredProperties(GS::UniString const& i_sFilter);
	GS::HashSet<API_Guid> GetPropertiesByType(API_VariantType & i_variant);
	inline API_Guid GetGuid() { return guid; };
};

#endif // !_S_PROPERTY_GROUP_HPP

