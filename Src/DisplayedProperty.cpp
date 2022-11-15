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
		}	
	}
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

GS::UniString DisplayedProperty::toUniString()
{
	GS::UniString result;
	ACAPI_Property_GetPropertyValueString(*this, &result);
	return result;
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

GS::Array<S_Variant> DisplayedProperty::GetVariants() const
{
	GS::Array<S_Variant> result;

	for (auto _var : definition.possibleEnumValues)
	{
		//TODO
		result.Push(S_Variant{ _var.displayVariant.uniStringValue, false });
	}

	return result;
}

DisplayedProperty::DisplayedProperty(const API_Property& i_prop, const API_Guid& i_guidObj)
	: API_Property	(i_prop) 
{
	switch (definition.collectionType)
	{
	case API_PropertySingleCollectionType:
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
	case API_PropertySingleChoiceEnumerationCollectionType:
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
