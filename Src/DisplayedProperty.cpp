#include "DisplayedProperty.hpp"


void DisplayedProperty::addExample(const API_Property& i_prop, const API_Guid& i_guidObj)
//Reset property if values are different
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
				m_areAllValuesEqual = false;
			}
		}
		case API_PropertyRealValueType:
		{
			if (value.singleVariant.variant.doubleValue != i_prop.value.singleVariant.variant.doubleValue)
			{
				value.singleVariant.variant.doubleValue = 0.0;
				m_areAllValuesEqual = false;
			}
		}
		case API_PropertyBooleanValueType:
		{
			if (value.singleVariant.variant.boolValue != i_prop.value.singleVariant.variant.boolValue)
			{
				value.singleVariant.variant.boolValue = false;
				m_areAllValuesEqual = false;
			}
		}
		case API_PropertyStringValueType:
		{
			if (value.singleVariant.variant.uniStringValue != i_prop.value.singleVariant.variant.uniStringValue)
			{
				value.singleVariant.variant.uniStringValue = "";
				m_areAllValuesEqual = false;
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

	representedPropertieS.Push(i_prop.definition.guid);
	representedObjectS.Push(i_guidObj);
}

GS::UniString DisplayedProperty::_toUniString(API_Variant i_variant)
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

GS::UniString DisplayedProperty::toUniString()
{
	{
		if (!m_areAllValuesEqual)
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

void DisplayedProperty::operator=(const GS::UniString i_value)
{
	GSErrCode err;

	this->value.singleVariant.variant.uniStringValue = i_value;

	for (auto objGuid : representedObjectS)
		err = ACAPI_Element_SetProperty(objGuid, *this);
}

void DisplayedProperty::operator=(const double i_value)
{
	GSErrCode err;

	this->value.singleVariant.variant.doubleValue = i_value;

	for (auto objGuid : representedObjectS)
		err = ACAPI_Element_SetProperty(objGuid, *this);
}

void DisplayedProperty::operator=(const int i_value)
{
	GSErrCode err;

	this->value.singleVariant.variant.intValue = i_value;

	for (auto objGuid : representedObjectS)
		err = ACAPI_Element_SetProperty(objGuid, *this);
}

DisplayedProperty::DisplayedProperty(const API_Property& i_prop, const API_Guid& i_guidObj)
	: API_Property	(i_prop) 
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
			m_onTabType = IntEdit_0;
			break;
		}
		case API_PropertyRealValueType:
		{
			m_onTabType = RealEdit_0;
			break;
		}
		case API_PropertyBooleanValueType:
		{
			m_onTabType = CheckBox_0;
			break;
		}
		case API_PropertyStringValueType:
		{
			m_onTabType = TextEdit_0;
			break;
		}
		default:
			m_onTabType = TextEdit_0;
			break;
		}
		break;
	}
	case API_PropertyListCollectionType:
	case API_PropertyMultipleChoiceEnumerationCollectionType:
	{
		m_onTabType = PopupControl_0;
		break;
	}
	default:
		m_onTabType = TextEdit_0;
	}

	representedPropertieS.Push(i_prop.definition.guid);
	representedObjectS.Push(i_guidObj);
}

