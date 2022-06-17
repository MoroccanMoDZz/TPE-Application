/**
* \file
* \brief This module manages the user interface.
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

#ifndef APEMV_USER_INTERFACE_DISPLAY_H_INCLUDED
#define APEMV_USER_INTERFACE_DISPLAY_H_INCLUDED

/////////////////////////////////////////////////////////////////
//// Macros & preprocessor definitions //////////////////////////

//// Types //////////////////////////////////////////////////////

//! brief Structure that contains all necessary information of an AID. Used to display a menu to select an AID.
typedef struct
{
	int aidLength;							//!< The length of the AID.
	unsigned char aid[16];					//!< The AID.
	char applicationLabel[16 + 1];			//!< The Application Label.
	unsigned char issuerCodeTableIndex;		//!< The Issuer Code Table Index.
	char preferredName[16 + 1];				//!< The Application Preferred Name.
} APEMV_UI_SelectAidItem_t;

//// Static function definitions ////////////////////////////////

//// Global variables ///////////////////////////////////////////

//// Functions //////////////////////////////////////////////////

int APEMV_UI_TransactionBegin(void);
void APEMV_UI_TransactionEnd(int previous);

void APEMV_UI_MessageInsertCard(void);
void APEMV_UI_MessageRemoveCard(void);
void APEMV_UI_MessagePleaseWait(void);
void APEMV_UI_MessageNoRemainingPin(void);
void APEMV_UI_MessageWrongPin(void);
void APEMV_UI_MessagePinLastAttempt(void);
void APEMV_UI_MessageCorrectPin(void);
void APEMV_UI_MessageApproved(void);
void APEMV_UI_MessageDeclined(void);
void APEMV_UI_MessageServiceNotAllowed(void);
void APEMV_UI_MessageTransactionCancelled(void);
void APEMV_UI_MessageCardBlocked(void);
void APEMV_UI_MessageCardRemoved(void);
void APEMV_UI_MessageCardError(void);
void APEMV_UI_MessageTransactionError(void);
void APEMV_UI_MessageDisplay(char *message);
void APEMV_UI_MessageDisplayDelay(char *message, int duration);
void APEMV_UI_InfoDisplay(char *message);
int APEMV_UI_MessageAuthorisation(char * message);
void APEMV_UI_RequestSignature(void);
int APEMV_UI_MenuAcceptRefuse(char * answer, const char * menu);
int APEMV_UI_MenuForceAcceptance(const char* messag, const char* answer, const char* menu );

int APEMV_UI_MenuSelectAid(const char* title, int numOfAids, const APEMV_UI_SelectAidItem_t *aids);
int APEMV_UI_AmountEntry(const char *text, const char *currencyLabel, unsigned long *amount);

ulong APEMV_UI_MenuSelectTopic(const char *title, const char *menu, int forceOnMerchantScreen);
ulong APEMV_UI_MenuSelectAlphaNumPP(char **menu);
int APEMV_UI_NumericalEntry(const char *title,const char *text, unsigned char *entryString, const char *entryMask );

EPSTOOL_PinEntry_Status_e APEMV_UI_PinEntry(int pinType,  char * message_enter, char * message_safe, char * message_invalid, int *pinLength);

#endif // APEMV_USER_INTERFACE_DISPLAY_H_INCLUDED
