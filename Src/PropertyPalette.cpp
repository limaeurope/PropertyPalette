// *****************************************************************************

// *****************************************************************************

// ---------------------------------- Includes ---------------------------------

#include	"APIEnvir.h"
#include	"ACAPinc.h"					// also includes APIdefs.h
#include	"APICommon.h"

#include	"PropertyPalette.h"
#include	"DG.h"
#include	"Algorithms.hpp"
#include	"FileSystem.hpp"
//#include <iostream>
#include	"LibXL/libxl.h"
#include	"DGFileDialog.hpp"

#include	"Property.h"
#include	"S_DisplayedProperty.h"

using namespace std;

// ---------------------------------- Types ------------------------------------

 struct CntlDlgData {

} ;

// ---------------------------------- Variables --------------------------------

static CntlDlgData			cntlDlgData;
static bool					isPopupInitialized = false;

// ---------------------------------- Prototypes -------------------------------


enum {
	Button_0 = 1,
	PopupControl_0,
	TextEdit_0,
	RealEdit_0,
	AngleEdit_0,
	LengthEdit_0,
	AreaEdit_0,
	VolumeEdit_0,
	SingleSelList_0,
};

typedef enum {
	PSM_Union,
	PSM_Intersection,
	PSM_LastAdded,
} propertySelectMode;


struct S_PropertyGroup : API_PropertyGroup {
	GS::Array<API_Guid> propertieS;
	S_PropertyGroup(API_PropertyGroup& i_group) : API_PropertyGroup(i_group) {};
};


void AddOrUpdateGroup(GS::HashTable<API_Guid, S_PropertyGroup>& i_groupS, API_Property& i_prop)
{
	if (!i_groupS.ContainsKey(i_prop.definition.groupGuid))
	{
		API_PropertyGroup group;
		group.guid = i_prop.definition.groupGuid;

		GSErrCode err = ACAPI_Property_GetPropertyGroup(group);

		i_groupS.Add(i_prop.definition.groupGuid, S_PropertyGroup(group));
	}

	if (!i_groupS[i_prop.definition.groupGuid].propertieS.Contains(i_prop.definition.guid))
		i_groupS[i_prop.definition.groupGuid].propertieS.Push(i_prop.definition.guid);
}


// -----------------------------------------------------------------------------
// SelectionChangeHandlerProc
//
//  prints the recently selected element
// -----------------------------------------------------------------------------
static GSErrCode __ACENV_CALL SelectionChangeHandlerProc(const API_Neig* selElemNeig)
{
	GSErrCode            err;
	API_SelectionInfo    selectionInfo;
	GS::Array<API_Neig>  selNeigs;
	GS::Array<API_PropertyDefinition> propertyDefinitionS;
	GS::Array<API_Property> propertieS;

	GS::HashTable<API_Guid, S_PropertyGroup> groupS;
	GS::HashTable< API_Guid, S_DisplayedProperty> collectedPropertieS;

	bool isFirstElement = true;

	err = ACAPI_Selection_Get(&selectionInfo, &selNeigs, true);
	BMKillHandle((GSHandle*)&selectionInfo.marquee.coords);
	if (err == APIERR_NOSEL)
		err = NoError;

	propertySelectMode psm = PSM_Intersection;

	for (const API_Neig& selNeig : selNeigs)
	{
		GS::Array<GS::Pair<API_Guid, API_Guid>> systemItemPairs;
		err = ACAPI_Element_GetPropertyDefinitions(selNeig.guid, API_PropertyDefinitionFilter_UserDefined, propertyDefinitionS);
		err = ACAPI_Element_GetPropertyValues(selNeig.guid, propertyDefinitionS, propertieS);

		if (isFirstElement)
		{
			for (auto& prop : propertieS)
			{
				collectedPropertieS.Add(prop.definition.guid, S_DisplayedProperty(prop));

				AddOrUpdateGroup(groupS, prop);
			}

			isFirstElement = false;
		}
		else
		{
			for (auto& prop : propertieS)
			{
				if (collectedPropertieS.ContainsKey(prop.definition.guid))
				{
					collectedPropertieS[prop.definition.guid].addExample(prop);

					AddOrUpdateGroup(groupS, prop);
				}
				else
					switch (psm)
					{
					case PSM_Intersection:
						break;
					case PSM_Union:
					case PSM_LastAdded:
						collectedPropertieS.Add(prop.definition.guid, S_DisplayedProperty(prop));

						AddOrUpdateGroup(groupS, prop);

						break;
					}
			}
		}
	}

	DGListDeleteItem(32400, SingleSelList_0, DG_ALL_ITEMS);

	for (auto& group : groupS)
	{
		DGListInsertItem(32400, SingleSelList_0, DG_LIST_BOTTOM);

		DGListSetTabItemText(32400, SingleSelList_0, DG_LIST_BOTTOM, 1, group.value->name);
		DGListSetItemBackgroundColor(32400, SingleSelList_0, DG_LIST_BOTTOM, 0xd8d8, 0xd8d8, 0xd8d8);

		for (auto& propGuid : group.value->propertieS)
		{
			auto& prop = collectedPropertieS[propGuid];

			DGListInsertItem(32400, SingleSelList_0, DG_LIST_BOTTOM);

			DGListSetTabItemText(32400, SingleSelList_0, DG_LIST_BOTTOM, 1, prop.definition.name);
			DGListSetTabItemText(32400, SingleSelList_0, DG_LIST_BOTTOM, 2, prop.toUniString());
		}
	}


	isFirstElement = false;

	return NoError;
}


// ============================================================================
// Install Notification Handlers
//
//
// ============================================================================
void	Do_SelectionMonitor(bool switchOn)
{
	if (switchOn)
		ACAPI_Notify_CatchSelectionChange(SelectionChangeHandlerProc);
	else
		ACAPI_Notify_CatchSelectionChange(nullptr);

	return;
}		/* Do_SelectionMonitor */


static short DGCALLBACK CntlDlgCallBack(short message, short dialID, short item, DGUserData userData, DGMessageData msgData)
{
	switch (message){
	case DG_MSG_INIT:
	{


		break;
	}
	case DG_MSG_CLICK:
	{
		switch (item)
		{
		case -1:
		case Button_0:
		{
			DGHideModelessDialog(32400);

			Do_SelectionMonitor(false);

			break;
		}
		case SingleSelList_0:
		{
			DGListSetDialItemOnTabField(32400, SingleSelList_0, 2, 2);

			break;
		}
		default:
			break;
		}

		break;
	}
	case DG_MSG_CLOSE:
	{
		DGHideModelessDialog(32400);

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

		DGListSetItemHeight(32400, SingleSelList_0, 28);

		isPopupInitialized = true;
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
	return NoError;
}		/* FreeData */
