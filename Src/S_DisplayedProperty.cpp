#include "S_DisplayedProperty.h"


void S_DisplayedProperty::addExample(API_Property& i_prop)
{
	switch (definition.collectionType)
	{
	case API_PropertySingleCollectionType:
	case API_PropertySingleChoiceEnumerationCollectionType:
	{
		switch (definition.valueType)
		{
		case API_PropertyIntegerValueType:
		{
			if (value.singleVariant.variant.intValue != i_prop.value.singleVariant.variant.intValue)
			{
				value.singleVariant.variant.intValue = 0;
				m_isValuesEqual = false;
			}
		}
		case API_PropertyRealValueType:
		{
			if (value.singleVariant.variant.doubleValue != i_prop.value.singleVariant.variant.doubleValue)
			{
				value.singleVariant.variant.doubleValue = 0.0;
				m_isValuesEqual = false;
			}
		}
		case API_PropertyBooleanValueType:
		{
			if (value.singleVariant.variant.boolValue != i_prop.value.singleVariant.variant.boolValue)
			{
				value.singleVariant.variant.boolValue = false;
				m_isValuesEqual = false;
			}
		}
		case API_PropertyStringValueType:
		{
			if (value.singleVariant.variant.uniStringValue != i_prop.value.singleVariant.variant.uniStringValue)
			{
				value.singleVariant.variant.uniStringValue = "";
				m_isValuesEqual = false;
			}
		}
		}	}
	case API_PropertyListCollectionType:
	case API_PropertyMultipleChoiceEnumerationCollectionType:
	{
		//if (value.listVariant.variants != i_prop.value.listVariant.variants)
		//{
		//	value.listVariant.variants = {};
		//	m_isValuesEqual = false;
		//}
	}
	}

	propertieS.Push(i_prop.definition.guid);
}

GS::UniString S_DisplayedProperty::_toUniString(API_Variant i_variant)
{
	switch (definition.valueType)
	{
	case API_PropertyIntegerValueType:
	case API_PropertyRealValueType:
	{
		char _s[32];
		itoa(i_variant.intValue, _s, 1);
		return GS::UniString(_s);
	}
	case API_PropertyBooleanValueType:
	{
		return GS::UniString(i_variant.boolValue ? "True" : "False");
	}
	case API_PropertyStringValueType:
	{
		return i_variant.uniStringValue;
	}
	}

	return GS::UniString("");
}

GS::UniString S_DisplayedProperty::toUniString()
{
	{
		if (!m_isValuesEqual)
			return "<Multiple values>";

		switch (definition.collectionType)
		{
		case API_PropertySingleCollectionType:
		case API_PropertySingleChoiceEnumerationCollectionType:
		{
			return _toUniString(value.singleVariant.variant);
		}
		case API_PropertyListCollectionType:
		case API_PropertyMultipleChoiceEnumerationCollectionType:
		{
			GS::UniString sResult;

			for (auto& _var : value.listVariant.variants)
			{
				if (sResult.GetLength())
					sResult += "; ";
				sResult += _toUniString(_var);
			}

			return sResult;
		}
		}
	}

	return GS::UniString("");
}

