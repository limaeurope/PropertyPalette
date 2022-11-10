#include "S_PropertyGroup.hpp"

GS::HashSet<API_Guid> S_PropertyGroup::GetFilteredProperties(GS::UniString const& i_sFilter)
{
	GS::HashSet<API_Guid> resultSet;

	for (auto propGuid : propertieS)
	{
		API_PropertyDefinition propDef;

		propDef.guid = propGuid;

		GSErrCode err = ACAPI_Property_GetPropertyDefinition(propDef);
		
		if (propDef.name.Contains(i_sFilter))
			resultSet.Add(propGuid);
	}
	
	return resultSet;
}

GS::HashSet<API_Guid> S_PropertyGroup::GetPropertiesByType(API_VariantType& i_variant)
{
	GS::HashSet<API_Guid> resultSet;

	for (auto propGuid : propertieS)
	{
		API_PropertyDefinition propDef;

		propDef.guid = propGuid;

		GSErrCode err = ACAPI_Property_GetPropertyDefinition(propDef);

		if (propDef.valueType == i_variant)
			resultSet.Add(propGuid);
	}

	return resultSet;
}
