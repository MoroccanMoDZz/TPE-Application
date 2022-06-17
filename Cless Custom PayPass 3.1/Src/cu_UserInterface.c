/**
* \file
* \brief This module manages the user interface environment (GOAL context...).
*
* \author Ingenico
* \author Copyright (c) 2012 Ingenico, 28-32, boulevard de Grenelle,\n
* 75015 Paris, France, All Rights Reserved.
*
* \author Ingenico has intellectual property rights relating to the technology embodied\n
* in this software. In particular, and without limitation, these intellectual property rights may\n
* include one or more patents.\n
* This software is distributed under licenses restricting its use, copying, distribution, and\n
* and decompilation. No part of this software may be reproduced in any form by any means\n
* without prior written authorisation of Ingenico.
**/

/////////////////////////////////////////////////////////////////
//// Includes ///////////////////////////////////////////////////

#include "sdk.h"
#include "GL_GraphicLib.h"
#include "TlvTree.h"
#include "GTL_Assert.h"
#include "ClessSample_Implementation.h"

//#include "EPSTOOL_Convert.h"
//#include "EPSTOOL_TlvTree.h"

//#include "APEMV_UserInterface.h"

//// Macros & preprocessor definitions //////////////////////////

//// Types //////////////////////////////////////////////////////

//// Static function definitions ////////////////////////////////

////  variables ///////////////////////////////////////////

static T_GL_HGRAPHIC_LIB __APEMV_UI_goalHandle = NULL;	//!< The GOAL handle.
static T_GL_HGRAPHIC_LIB __APEMV_UI_goalHandlePP = NULL;	//!< The GOAL handle for pinpad.
static Telium_File_t *__APEMV_UI_displayDriver = NULL;			//!< The display driver handle.
static Telium_File_t *__APEMV_UI_keyboardDriver = NULL;			//!< The keyboard driver handle.
static Telium_File_t *__APEMV_UI_touchDriver = NULL;				//!< The touch screen driver handle.

//// Functions //////////////////////////////////////////////////

//! \brief Create the graphic library instance.
//! \return  pointer to graphic library handle.
T_GL_HGRAPHIC_LIB APEMV_UI_GoalHandle(void)
{
	if (__APEMV_UI_goalHandle == NULL)
	{
		__APEMV_UI_goalHandle = GL_GraphicLib_Create();
		GL_GraphicLib_SetCharset(__APEMV_UI_goalHandle, GL_ENCODING_ISO_8859_1);

		GL_GraphicLib_SetBackgroundImage(__APEMV_UI_goalHandle, "file://flash/HOST/whitebg.bmp");
		GL_GraphicLib_SetSkinedColor(__APEMV_UI_goalHandle, GL_COLOR_DIALOG_TEXT, GL_COLOR_BLACK);
	}

	return __APEMV_UI_goalHandle;
}




//! \brief Create the graphic library instance on pinpad.
//! \return  pointer to graphic library handle.
T_GL_HGRAPHIC_LIB APEMV_UI_GoalHandlePP(void)
{
	if (__APEMV_UI_goalHandlePP == NULL)
	{
		__APEMV_UI_goalHandlePP = GL_GraphicLib_CreateWithParam(GL_VIEWER_PINPAD, NULL);
	}

	return __APEMV_UI_goalHandlePP;
}

//! \brief Destroy the graphic library instance.
void APEMV_UI_GoalDestroy(void)
{
	if (__APEMV_UI_goalHandle != NULL)
	{
		GL_GraphicLib_Destroy(__APEMV_UI_goalHandle);
		__APEMV_UI_goalHandle = NULL;
	}
}

//! \brief Destroy the graphic library instance on pinpad.
void APEMV_UI_GoalDestroyPP(void)
{
	if (__APEMV_UI_goalHandlePP != NULL)
	{
		GL_GraphicLib_Destroy(__APEMV_UI_goalHandlePP);
		__APEMV_UI_goalHandlePP = NULL;
	}
}

//! \brief Open the GOAL interface (open drivers ...).
void APEMV_UI_GoalOpen(void)
{
	if (__APEMV_UI_displayDriver == NULL)
		__APEMV_UI_displayDriver = Telium_Fopen("DISPLAY", "w*");
	if (__APEMV_UI_keyboardDriver == NULL)
	{
		//__APEMV_UI_keyboardDriver = Telium_Fopen("KEYBOARD", "r*");
		//APEMV_UI_MessageDisplayDelay("BBBB.04",2);
	}
	if (__APEMV_UI_touchDriver == NULL)
		__APEMV_UI_touchDriver = Telium_Fopen("TSCREEN", "r*");
}

//! \brief Close the GOAL interface (close drivers ...).
void APEMV_UI_GoalClose(void)
{
	if (__APEMV_UI_displayDriver != NULL)
	{
		Telium_Fclose(__APEMV_UI_displayDriver);
		__APEMV_UI_displayDriver = NULL;
	}
	if (__APEMV_UI_keyboardDriver != NULL)
	{
		Telium_Fclose(__APEMV_UI_keyboardDriver);
		__APEMV_UI_keyboardDriver = NULL;
	}
	if (__APEMV_UI_touchDriver != NULL)
	{
		Telium_Fclose(__APEMV_UI_touchDriver);
		__APEMV_UI_touchDriver = NULL;
	}
}
