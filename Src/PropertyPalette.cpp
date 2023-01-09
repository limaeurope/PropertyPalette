// *****************************************************************************

// *****************************************************************************

// ---------------------------------- Includes ---------------------------------
#define UNICODE

#include	"APIEnvir.h"
#include	"ACAPinc.h"					// also includes APIdefs.h
#include	"APICommon.h"

//#include	"PropertyPalette.h"
#include	"DG.h"
//#include	"Algorithms.hpp"
//#include	"FileSystem.hpp"
////#include <iostream>
//#include	"LibXL/libxl.h"
//#include	"DGFileDialog.hpp"
//
//#include	"Property.h"
#include	"DisplayedProperty.hpp"
#include	"Enums/PropertySelectMode.hpp"
#include	"Enums/PropertySelectMode.hpp"
#include	"Enums/FilterTypes.hpp"
#include	"Data/SettingsSingleton.hpp"
#include	"Data/S_PropertyGroup.hpp"
//#include	<exception>
//#include	"Logger/Logger.hpp"
#include	"DGModule.hpp"
//#include <windows.h>
#include <shellapi.h>


using namespace std;

// ---------------------------------- Types ------------------------------------

 struct CntlDlgData {

} ;

// ---------------------------------- Variables --------------------------------

static CntlDlgData			cntlDlgData;
static bool					isPopupInitialized = false;

// ---------------------------------- Prototypes -------------------------------


void AddOrUpdateGroup(GS::HashTable<API_Guid, S_PropertyGroup* >& i_groupS, API_Property& i_prop)
{
	if (!i_groupS.ContainsKey(i_prop.definition.groupGuid))
	{
		API_PropertyGroup group;
		group.guid = i_prop.definition.groupGuid;

		GSErrCode err = ACAPI_Property_GetPropertyGroup(group);

		i_groupS.Add(i_prop.definition.groupGuid, new S_PropertyGroup(group));
	}

	if (!i_groupS[i_prop.definition.groupGuid]->propertieS.Contains(i_prop.definition.guid))
		i_groupS[i_prop.definition.groupGuid]->propertieS.Add(i_prop.definition.guid);
}


// -----------------------------------------------------------------------------
// RebuildList
// -----------------------------------------------------------------------------
static GSErrCode __ACENV_CALL RebuildList(const API_Neig* selElemNeig)
{
	UNUSED_PARAMETER(selElemNeig);
	GSErrCode            err;
	API_SelectionInfo    selectionInfo;
	GS::Array<API_Neig>  selNeigS;

	GS::HashTable<API_Guid, S_PropertyGroup*> groupS;
	GS::HashTable< API_Guid, DisplayedProperty*> collectedPropertieS;
	GS::HashSet< API_Guid> collectedPropertieSSet;

	bool isFirstElement = true;

	err = ACAPI_Selection_Get(&selectionInfo, &selNeigS, true);
	BMKillHandle((GSHandle*)&selectionInfo.marquee.coords);
	if (err == APIERR_NOSEL)
		err = NoError;

	for (API_Neig const & selNeig : selNeigS)
	{
		GS::Array<API_Property> propertieS{};
		GS::HashSet< API_Guid> _propSet{};
		GS::Array<API_PropertyDefinition> propertyDefinitionS{};

		err = ACAPI_Element_GetPropertyDefinitions(selNeig.guid, API_PropertyDefinitionFilter_UserDefined, propertyDefinitionS);
		err = ACAPI_Element_GetPropertyValues(selNeig.guid, propertyDefinitionS, propertieS);

		for (auto& prop : propertieS)
		{
			if (collectedPropertieS.ContainsKey(prop.definition.guid))
				collectedPropertieS[prop.definition.guid]->addExample(prop, selNeig.guid);
			else
				collectedPropertieS.Add(prop.definition.guid, new DisplayedProperty(prop, selNeig.guid));

			AddOrUpdateGroup(groupS, prop);

			_propSet.Add(prop.definition.guid);
		}

		PropertySelectMode _psm = isFirstElement ? PSM_LastAdded : SETTINGS().GetPropertySelectMode();

		switch (_psm)
		{
		case PSM_Intersection:
		{
			collectedPropertieSSet.Intersect(_propSet);

			break;
		}
		case PSM_Union:
		{
			collectedPropertieSSet.Unify(_propSet);

			break;
		}
		case PSM_LastAdded:
		{
			collectedPropertieSSet = _propSet;

			break;
		}
		}

		isFirstElement = false;
	}

	GS::HashSet<API_Guid> collectedGuids;

	for (auto& v : collectedPropertieS.Values())
		if	(	(	!SETTINGS().GetFilterText().GetLength()
					||	v->definition.name.ToLowerCase().Contains(SETTINGS().GetFilterText()))
			&&	(	!SETTINGS().GetFilterType()
					||	*(v) == SETTINGS().GetFilterType())
			)
			collectedGuids.Add(v->definition.guid);

	collectedPropertieSSet.Intersect(collectedGuids);

	for (auto& prop : collectedPropertieSSet)
		AddOrUpdateGroup(groupS, *collectedPropertieS[prop]);

	DGListDeleteItem(32400, SingleSelList_0, DG_ALL_ITEMS);

	short iDialogListPos = 1;

	for (auto& group : groupS)
	{
		GS::HashSet<API_Guid> _groupPropS{ collectedPropertieSSet };

		_groupPropS.Intersect((*group.value)->propertieS);

		if (_groupPropS.GetSize()) 
		{
			DGListInsertItem(32400, SingleSelList_0, DG_LIST_BOTTOM);

			DGListSetTabItemText(32400, SingleSelList_0, DG_LIST_BOTTOM, 1, (*group.value)->name);
			DGListSetItemBackgroundColor(32400, SingleSelList_0, DG_LIST_BOTTOM, 0xd8d8, 0xd8d8, 0xd8d8);
			SETTINGS().AddToRowList((*group.value)->guid, (*group.value));
			SETTINGS().AddToGuidList(iDialogListPos++, (*group.value)->guid);
			//SETTINGS().AddOrUpdateLists(iDialogListPos++, *group.value);

			if ((*group.value)->GetDisplayItems())
				for (auto& propGuid : _groupPropS)
				{
					DisplayedProperty *dProp = collectedPropertieS[propGuid];

					DGListInsertItem(32400, SingleSelList_0, DG_LIST_BOTTOM);

					DGListSetTabItemText(32400, SingleSelList_0, DG_LIST_BOTTOM, 1, dProp->definition.name);
					DGListSetTabItemText(32400, SingleSelList_0, DG_LIST_BOTTOM, 2, dProp->toUniString());

					SETTINGS().AddToRowList(dProp->definition.guid, dProp);
					SETTINGS().AddToGuidList(iDialogListPos++, dProp->definition.guid);
					//SETTINGS().AddOrUpdateLists(iDialogListPos++, dProp);
				}
		}
	}

	return NoError;
}


static GSErrCode __ACENV_CALL RefreshList()
{}


// ============================================================================
// Install Notification Handlers
//
//
// ============================================================================
void	Do_SelectionMonitor(bool switchOn)
{
	if (switchOn)
		ACAPI_Notify_CatchSelectionChange(RebuildList);
	else
		ACAPI_Notify_CatchSelectionChange(nullptr);

	return;
}		/* Do_SelectionMonitor */


static short DGCALLBACK CntlDlgCallBack(short message, short dialID, short item, DGUserData userData, DGMessageData msgData)
{
	switch (message){
	case DG_MSG_INIT:
	{
		RebuildList(NULL);

		break;
	}
	case DG_MSG_CLICK:
	{
		try
		{
			switch (item)
			{
			case -1:
			case Button_0:
			{
				DGHideModelessDialog(dialID);

				Do_SelectionMonitor(false);

				break;
			}
			case Button_1:
			{
				ShellExecute(0, 0, SETTINGS().GetHelpURL(), 0, 0, SW_SHOW);

				break;
			}
			case SingleSelList_0:
			{
				short _id = DGListGetSelected(dialID, SingleSelList_0, DG_LIST_TOP);

				try {
					PropertyRow* _pr = SETTINGS().GetSelectedRow(_id);
					if (_pr->m_propertyType != PT_Property)
						return 0;

					DisplayedProperty _prop = *static_cast<DisplayedProperty*>(_pr);

					if (_prop.HasExpression())
						return 0;

					DGListSetDialItemOnTabField(dialID, SingleSelList_0, 2, _prop.GetOnTabType());

					OnTabTypes gott = _prop.GetOnTabType();

					switch (gott)
					{
					case CheckBox_0:
					{
						DGSetItemValLong(dialID, gott, _prop.AreAllValuesEqual ? _prop.value.singleVariant.variant.boolValue ? 1 : 0 : 0);

						//DGListSetTabItemIcon(dialID, SingleSelList_0, _id, 2, DG_LIST_CHECKEDICON);
						break;
					}
					case RealEdit_0:
					case AngleEdit_0:
					case LengthEdit_0:
					case AreaEdit_0:
					case VolumeEdit_0:
					{
						DGSetItemValDouble(dialID, gott, _prop.AreAllValuesEqual ? _prop.value.singleVariant.variant.doubleValue : 0);

						break;
					}
					case IntEdit_0:
					{
						DGSetItemValLong(dialID, gott, _prop.AreAllValuesEqual ? _prop.value.singleVariant.variant.intValue : 0);

						break;
					}
					case TextEdit_0:
					{
						DGSetItemText(dialID, gott, _prop.AreAllValuesEqual ? _prop.toUniString() : "");

						break;
					}
					case PopupControl_0:
					{
						DGPopUpDeleteItem(dialID, PopupControl_0, DG_ALL_ITEMS);

						for (auto &_var : _prop.GetVariants())
						{
							DGPopUpInsertItem(dialID, PopupControl_0, DG_LIST_BOTTOM);
							DGPopUpSetItemText(dialID, PopupControl_0, DG_LIST_BOTTOM, _var.sName);
							if (_var.isSelected)
							{
								DGPopUpSetItemIcon(dialID, PopupControl_0, DG_LIST_BOTTOM, DG::Icon(ACAPI_GetOwnResModule(), 27101));
								DGPopUpSelectItem(dialID, PopupControl_0, DG_LIST_BOTTOM);
							}
							else
							{
								DGPopUpSetItemIcon(dialID, PopupControl_0, DG_LIST_BOTTOM, DG::Icon(ACAPI_GetOwnResModule(), 27102));
								DGPopUpSelectItem(dialID, PopupControl_0, DG_LIST_BOTTOM);
							}
						}

						break;
					}
					}

					break;
				}
				catch (exception) {
					RebuildList(NULL);
				}
			}

			default:
				break;
			}

			break;
		}
		catch (exception)
		{
			RebuildList(NULL);
		}
	}
	case DG_MSG_CHANGE:
	{
		switch (item)
		{
		case CheckBox_0:
		{
			*SETTINGS().GetCurrentlyEditedProperty() = DGGetItemValLong(dialID, item) ? true : false;

			break;
		}
		case RealEdit_0:
		case AngleEdit_0:
		case LengthEdit_0:
		case AreaEdit_0:
		case VolumeEdit_0:
		case IntEdit_0:
		{
			short _id = DGListGetSelected(dialID, SingleSelList_0, DG_LIST_TOP);

			*SETTINGS().GetCurrentlyEditedProperty(_id) = DGGetItemValLong(dialID, item);

			break;
		}
		case TextEdit_0:
		{
			short _id = DGListGetSelected(dialID, SingleSelList_0, DG_LIST_TOP);

			*SETTINGS().GetCurrentlyEditedProperty(_id) = DGGetItemText(dialID, item);

			break;
		}
		case PopupControl_0:
		{
			short _id = DGListGetSelected(dialID, SingleSelList_0, DG_LIST_TOP);
			DisplayedProperty* _prop = SETTINGS().GetCurrentlyEditedProperty(_id);

			short _i = DGPopUpGetSelected(dialID, item);

			auto _variant = _prop->GetVariants()[_i - 1];

			_variant.isSelected = !_variant.isSelected;

			*SETTINGS().GetCurrentlyEditedProperty() = _variant;

			RebuildList(NULL);

			break;
		}
		case PushRadio_1:
		{
			SETTINGS().SetPropertySelectMode(PSM_Intersection);

			RebuildList(NULL);

			break;
		}
		case PushRadio_2:
		{
			SETTINGS().SetPropertySelectMode(PSM_Union);

			RebuildList(NULL);

			break;
		}
		case PushRadio_3:
		{
			SETTINGS().SetPropertySelectMode(PSM_LastAdded);

			RebuildList(NULL);

			break;
		}
		case TextEdit_1:
		{
			SETTINGS().SetFilterText(DGGetItemText(dialID, TextEdit_1));

			RebuildList(NULL);

			break;
		}
		case PopupControl_1:
		{
			TypeFilter _a = (TypeFilter)(DGGetItemValLong(dialID, PopupControl_1) - 1);
			SETTINGS().SetFilterType(_a);

			RebuildList(NULL);

			break;
		}
		}

		break;
	}
	case DG_MSG_DOUBLECLICK:
	{
		switch (item)
		{
		case SingleSelList_0:
		{
			short _id = DGListGetSelected(dialID, SingleSelList_0, DG_LIST_TOP);

			PropertyRow* _row = SETTINGS().GetFromRowList(_id);

			auto _ = _row->GetDisplayItems();

			_row->SetDisplayItems(!_);

			RebuildList(NULL);

			break;
		}
		}

		break;
	}
	case DG_MSG_GROW:
	{
		short	hgrow = DGGetHGrow(msgData);
		short	vgrow = DGGetVGrow(msgData);

		//DGResizeMsgData* _msgData = static_cast<DGResizeMsgData*> ((void*)msgData);

		DGBeginMoveGrowItems(dialID);
		DGGrowItem(dialID, SingleSelList_0, hgrow, vgrow);
		DGMoveItem(dialID, Button_0, hgrow, vgrow);
		//DGMoveItem(dialID, PopupControl_1, hgrow, 0);
		DGMoveItem(dialID, PushRadio_1, 0, vgrow);
		DGMoveItem(dialID, PushRadio_2, 0, vgrow);
		DGMoveItem(dialID, PushRadio_3, 0, vgrow);
		//DGMoveItem(dialID, Button_1, 0, vgrow);
		DGEndMoveGrowItems(dialID);
		break;
	}
	case DG_MSG_CLOSE:
	{
		DGHideModelessDialog(dialID);

		Do_SelectionMonitor(false);

		break;
	}
	}
	return 0;
}


void Do_ShowPropertyPalette()
{
	Do_SelectionMonitor(true);

	if (!isPopupInitialized)
	{
		DGModelessInit(ACAPI_GetOwnResModule(), 32400, ACAPI_GetOwnResModule(), CntlDlgCallBack, (DGUserData)&cntlDlgData, true);

		DGListSetTabFieldCount(32400, SingleSelList_0, 3);

		DGListSetTabFieldData(32400, SingleSelList_0, 1, 0, 350, DG_IS_LEFT, DG_IS_TRUNCEND, true, true);
		DGListSetTabFieldData(32400, SingleSelList_0, 2, 350, 450, DG_IS_LEFT, DG_IS_TRUNCEND, true, true);

		DGHideItem(32400, PopupControl_0);
		DGHideItem(32400, TextEdit_0);
		DGHideItem(32400, RealEdit_0);
		DGHideItem(32400, AngleEdit_0);
		DGHideItem(32400, LengthEdit_0);
		DGHideItem(32400, AreaEdit_0);
		DGHideItem(32400, VolumeEdit_0);
		DGHideItem(32400, IntEdit_0);
		DGHideItem(32400, CheckBox_0);

		DGListSetItemHeight(32400, SingleSelList_0, 28);

		DGSelectRadio(32400, PushRadio_1);

		isPopupInitialized = true;

		for (auto ftn : TypeFilterNameS)
		{
			DGPopUpInsertItem(32400, PopupControl_1, DG_LIST_BOTTOM);
			DGPopUpSetItemText(32400, PopupControl_1, DG_LIST_BOTTOM, ftn);
		}
	}
	else
		if (!DGIsModelessDialogVisible(32400))
			DGShowModelessDialog(32400, DG_TOP_MODAL);
}

// -----------------------------------------------------------------------------

GSErrCode __ACENV_CALL ClassificationCopier (const API_MenuParams *menuParams)
{
	return ACAPI_CallUndoableCommand ("Property Palette",
		[&] () -> GSErrCode {
			switch (menuParams->menuItemRef.itemIndex) {
				case 1:		Do_ShowPropertyPalette();			break;
				default:										break;
			}

			return NoError;
		});
}		/* ClassificationCopier */


// -----------------------------------------------------------------------------
// Dependency definitions
// -----------------------------------------------------------------------------
API_AddonType __ACENV_CALL	CheckEnvironment (API_EnvirParams* envir)
{
	RSGetIndString (&envir->addOnInfo.name, 32000, 1, ACAPI_GetOwnResModule ());
	RSGetIndString (&envir->addOnInfo.description, 32000, 2, ACAPI_GetOwnResModule ());

	return APIAddon_Preload;
}		/* RegisterAddOn */


// -----------------------------------------------------------------------------
// Interface definitions
// -----------------------------------------------------------------------------
GSErrCode __ACENV_CALL	RegisterInterface (void)
{
	GSErrCode	err;

	//
	// Register menus
	//
	err = ACAPI_Register_Menu (32506, 0, MenuCode_UserDef, MenuFlag_Default);

	return err;
}		/* RegisterInterface */


// -----------------------------------------------------------------------------
// Called when the Add-On has been loaded into memory
// to perform an operation
// -----------------------------------------------------------------------------
GSErrCode __ACENV_CALL	Initialize (void)
{
	GSErrCode err;

	//
	// Install menu handler callbacks
	//

	err = ACAPI_Install_MenuHandler (32506, ClassificationCopier);

	return err;
}		/* Initialize */


// -----------------------------------------------------------------------------
// Called when the Add-On is going to be unloaded
// -----------------------------------------------------------------------------

GSErrCode __ACENV_CALL	FreeData (void)
{
	DGModelessClose(32400);

	return NoError;
}		/* FreeData */
