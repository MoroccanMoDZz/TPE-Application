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

#ifndef APEMV_USER_INTERFACE_H_INCLUDED
#define APEMV_USER_INTERFACE_H_INCLUDED

/////////////////////////////////////////////////////////////////
//// Macros & preprocessor definitions //////////////////////////

//// Types //////////////////////////////////////////////////////

//// Static function definitions ////////////////////////////////

//// Global variables ///////////////////////////////////////////

//// Functions //////////////////////////////////////////////////

T_GL_HGRAPHIC_LIB APEMV_UI_GoalHandle(void);
T_GL_HGRAPHIC_LIB APEMV_UI_GoalHandlePP(void);
void APEMV_UI_GoalDestroy(void);
void APEMV_UI_GoalDestroyPP(void);

void APEMV_UI_GoalOpen(void);
void APEMV_UI_GoalClose(void);

#endif // APEMV_USER_INTERFACE_H_INCLUDED
