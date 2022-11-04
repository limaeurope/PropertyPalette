#include "Property.h"

bool _equals(const API_VariantType i_valType, 
	const API_Variant& i_this, 
	const API_Variant& i_other)
{
	switch (i_valType)
	{
	case API_PropertyIntegerValueType:
	{
		return i_this.intValue == i_other.intValue;
	}
	case API_PropertyRealValueType:
	{
		return i_this.doubleValue == i_other.doubleValue;
	}
	case API_PropertyBooleanValueType:
	{
		return i_this.boolValue == i_other.boolValue;
	}
	case API_PropertyStringValueType:
	{
		return i_this.uniStringValue == i_other.uniStringValue;
	}
	}

	return false;
}

bool operator == (const API_Property& i_this, const API_Property& i_other) 
{
	if (i_this.definition.valueType != i_other.definition.valueType)
		return false;

	switch (i_this.definition.collectionType)
	{
	case API_PropertySingleCollectionType:
	case API_PropertySingleChoiceEnumerationCollectionType:
	{
		return _equals(i_this.definition.valueType, i_this.value.singleVariant.variant, i_other.value.singleVariant.variant);
	}
	case API_PropertyListCollectionType:
	case API_PropertyMultipleChoiceEnumerationCollectionType:
	{
		switch (i_this.definition.valueType)
		{
		case API_PropertyIntegerValueType:
		{
			GS::HashSet<Int32> _thisSet, _otherSet;

			for (auto& _v : i_this.value.listVariant.variants)
				_thisSet.Add(_v.intValue);

			for (auto& _v : i_other.value.listVariant.variants)
				_otherSet.Add(_v.intValue);

			return _thisSet == _otherSet;
		}
		case API_PropertyRealValueType:
		{
			GS::HashSet<double> _thisSet, _otherSet;

			for (auto& _v : i_this.value.listVariant.variants)
				_thisSet.Add(_v.doubleValue);

			for (auto& _v : i_other.value.listVariant.variants)
				_otherSet.Add(_v.doubleValue);

			return _thisSet == _otherSet;
		}
		case API_PropertyBooleanValueType:
		{
			GS::HashSet<bool> _thisSet, _otherSet;

			for (auto& _v : i_this.value.listVariant.variants)
				_thisSet.Add(_v.boolValue);

			for (auto& _v : i_other.value.listVariant.variants)
				_otherSet.Add(_v.boolValue);

			return _thisSet == _otherSet;
		}
		case API_PropertyStringValueType:
		{
			GS::HashSet<GS::UniString> _thisSet, _otherSet;

			for (auto& _v : i_this.value.listVariant.variants)
				_thisSet.Add(_v.uniStringValue);

			for (auto& _v : i_other.value.listVariant.variants)
				_otherSet.Add(_v.uniStringValue);

			return _thisSet == _otherSet;
		}
		}
	}
	}

	return false;
}

