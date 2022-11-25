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

		break;
	}
	case API_PropertyListCollectionType:
	case API_PropertyMultipleChoiceEnumerationCollectionType:
	{
		if (value.listVariant.variants != i_prop.value.listVariant.variants)
		{
			value.listVariant.variants = {};
			AreAllValuesEqual = false;
		}

		break;
	}
	}

	representedPropertieS.Push(i_prop.definition.guid);
	representedObjectS.Push(i_guidObj);
}

GS::UniString DisplayedProperty::toUniString()
{
	GS::UniString result;

	if (HasExpression())
		return "<Expression>";

	if (value.singleVariant.variant.type == API_PropertyBooleanValueType)
		return value.singleVariant.variant.boolValue ? "True" : "False";

	API_Property _p = *this;

	GSErrCode err = ACAPI_Property_GetPropertyValueString(_p, &result);

	return result;
}

void DisplayedProperty::operator=(const GS::UniString i_value)
{
	isDefault = false;
	value.variantStatus = API_VariantStatusNormal;

	this->value.singleVariant.variant.uniStringValue = i_value;

	for (auto objGuid : representedObjectS)
		GSErrCode err = ACAPI_Element_SetProperty(objGuid, *this);
}

void DisplayedProperty::operator=(const bool i_value)
{
	isDefault = false;
	value.variantStatus = API_VariantStatusNormal;

	this->value.singleVariant.variant.boolValue = i_value;

	for (auto objGuid : representedObjectS)
		GSErrCode err = ACAPI_Element_SetProperty(objGuid, *this);
}

void DisplayedProperty::operator=(const double i_value)
{
	isDefault = false;
	value.variantStatus = API_VariantStatusNormal;

	this->value.singleVariant.variant.doubleValue = i_value;

	for (auto objGuid : representedObjectS)
		GSErrCode err = ACAPI_Element_SetProperty(objGuid, *this);
}

void DisplayedProperty::operator=(const int i_value)
{
	isDefault = false;
	value.variantStatus = API_VariantStatusNormal;

	this->value.singleVariant.variant.intValue = i_value;

	for (auto objGuid : representedObjectS)
		GSErrCode err = ACAPI_Element_SetProperty(objGuid, *this);
}

void DisplayedProperty::operator=(const S_Variant i_var)
{
	isDefault = false;
	value.variantStatus = API_VariantStatusNormal;

	switch (definition.collectionType)
	{
	//case API_PropertySingleCollectionType:
	case API_PropertySingleChoiceEnumerationCollectionType:
	{
		value.singleVariant.variant.guidValue = i_var.guid;

		break;
	}
	//case API_PropertyListCollectionType:
	case API_PropertyMultipleChoiceEnumerationCollectionType:
	default:
	{
		auto _v = API_Variant();
		_v.guidValue = i_var.guid;
		_v.type = API_PropertyGuidValueType;
		value.listVariant.variants.Push(_v);

		break;
	}
	}

	for (auto objGuid : representedObjectS)
		GSErrCode err = ACAPI_Element_SetProperty(objGuid, *this);
}

GS::Array<S_Variant> DisplayedProperty::GetVariants() const
{
	GS::Array<S_Variant> result;
	bool _b;

	for (auto _var : definition.possibleEnumValues)
	{
		_b = false;

		for (auto& _v: value.listVariant.variants)
		{
			if (_v.guidValue == _var.keyVariant.guidValue)
				_b = true;
		}

		if (value.singleVariant.variant.guidValue == _var.keyVariant.guidValue)
			_b = true;

		result.Push(S_Variant{ _var.displayVariant.uniStringValue, _b, _var.keyVariant.guidValue });
	}

	return result;
}

DisplayedProperty::DisplayedProperty(const API_Property& i_prop, const API_Guid& i_guidObj)
	: API_Property		(i_prop)
{
	SetDisplayItems(false);					//FIXME
	m_propertyType	= PT_Property;

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
			switch (definition.measureType)
			{
			case API_PropertyLengthMeasureType:
			{
				m_onTabType = LengthEdit_0;

				break;
			}
			case API_PropertyAreaMeasureType:
			{
				m_onTabType = AreaEdit_0;

				break;
			}
			case API_PropertyVolumeMeasureType:
			{
				m_onTabType = VolumeEdit_0;

				break;
			}
			case API_PropertyAngleMeasureType:
			{
				m_onTabType = AngleEdit_0;

				break;
			}
			default:
				m_onTabType = RealEdit_0;

				break;
			}
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

