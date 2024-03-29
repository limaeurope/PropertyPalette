#ifndef _FILTER_TYPES_HPP
#define _FILTER_TYPES_HPP

#include	"../APIEnvir.h"
#include	"ACAPinc.h"					// also includes APIdefs.h
#include	"../APICommon.h"

#include	"../DisplayedProperty.hpp"

typedef enum {
	FT_None,
	FT_Int,
	FT_Real,
	FT_Bool,
	FT_String,
	FT_List,
	FT_SingleEnum,
	FT_MultiEnum,
} TypeFilter;

const GS::Array<GS::UniString> TypeFilterNameS{
	"",
	"Integer",
	"Real",
	"Boolean",
	"String",
	"List",
	"Single Selection Enum",
	"Multi Selection Enum",
};

inline bool operator == (TypeFilter i_this, DisplayedProperty i_that)
{
	switch (i_that.definition.collectionType)
	{
	case API_PropertySingleChoiceEnumerationCollectionType:
	{
		return i_this == FT_SingleEnum;
	}
	case API_PropertyMultipleChoiceEnumerationCollectionType:
	{
		return i_this == FT_MultiEnum;
	}
	default:
		switch (i_that.definition.valueType)
		{
		case API_PropertyIntegerValueType:
		{
			return i_this == FT_Int;
		}
		case API_PropertyRealValueType:
		{
			return i_this == FT_Real;
		}
		case API_PropertyBooleanValueType:
		{
			return i_this == FT_Bool;
		}
		case API_PropertyStringValueType:
		{
			return i_this == FT_String;
		}
		default:
			return false;
		}
	}
}

inline bool operator == (DisplayedProperty i_this, TypeFilter i_that)
{
	return i_that == i_this;
}

#endif // !_FILTER_TYPES_HPP

