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
				AreAllValuesEqual = false;
			}
		}
		case API_PropertyRealValueType:
		{
			if (value.singleVariant.variant.doubleValue != i_prop.value.singleVariant.variant.doubleValue)
			{
				value.singleVariant.variant.doubleValue = 0.0;
				AreAllValuesEqual = false;
			}
		}
		case API_PropertyBooleanValueType:
		{
			if (value.singleVariant.variant.boolValue != i_prop.value.singleVariant.variant.boolValue)
			{
				value.singleVariant.variant.boolValue = false;
				AreAllValuesEqual = false;
			}
		}
		case API_PropertyStringValueType:
		{
			if (value.singleVariant.variant.uniStringValue != i_prop.value.singleVariant.variant.uniStringValue)
			{
				value.singleVariant.variant.uniStringValue = "";
				AreAllValuesEqual = false;
			}
		}
		}	}
	case API_PropertyListCollectionType:
	case API_PropertyMultipleChoiceEnumerationCollectionType:
	{
		if (value.listVariant.variants != i_prop.value.listVariant.variants)
		{
			value.listVariant.variants = {};
			AreAllValuesEqual = false;
		}
	}
	}

	representedPropertieS.Push(i_prop.definition.guid);
	representedObjectS.Push(i_guidObj);
}

GS::UniString DisplayedProperty::_toUniString(const API_Variant &i_variant)
{
	switch (i_variant.type)
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
		if (!AreAllValuesEqual)
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
					sResult.Append("; ");
				sResult.Append(_toUniString(_var));
			}

			auto _a = sResult.ToCStr().Get();
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

bool operator==(const API_Variant& i_this, const API_Variant& i_other)
{
	if (i_this.type != i_other.type)
		return false;
	else
		switch (i_this.type)
		{
			case API_PropertyUndefinedValueType:
			{
				return false;
				break;
			}
			case API_PropertyIntegerValueType:
			{
				return i_this.intValue == i_other.intValue;
				break;
			}
			case API_PropertyRealValueType:
			{
				return i_this.doubleValue == i_other.doubleValue;
				break;
			}
			case API_PropertyStringValueType:
			{
				return i_this.uniStringValue == i_other.uniStringValue;
				break;
			}
			case API_PropertyBooleanValueType:
			{
				return i_this.boolValue == i_other.boolValue;
				break;
			}
			case API_PropertyGuidValueType:
			{
				return i_this.guidValue == i_other.guidValue;
				break;
			}
			default:
				return true;
		}
}
