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

/////////////////////////////////////////////////////////////////
//// Includes ///////////////////////////////////////////////////

#include "sdk.h"
#include "SEC_interface.h"
#include "GL_GraphicLib.h"
#include "TlvTree.h"
#include "GTL_Assert.h"
#include "GTL_Convert.h"

#include "EPSTOOL_Convert.h"
#include "EPSTOOL_TlvTree.h"
#include "EPSTOOL_PinEntry.h"
#include "EPSTOOL_Unicode.h"
#include "ClessSample_Implementation.h"
#include "cu_pin.h"
#include "cu_UserInterface.h"
#include "cu_UserInterfaceDisplay.h"
#include "Gpinpad.h"

//// Macros & preprocessor definitions //////////////////////////

// Account Type Selection
#define  ACCOUNT_TYPE_DEFAULT						0x00 	//!< Default account type
#define  ACCOUNT_TYPE_SAVINGS						0x10	//!< Saving account
#define  ACCOUNT_TYPE_CHEQUE_DEBIT					0x20	//!< Check or debit account
#define  ACCOUNT_TYPE_CREDIT						0x30	//!< Credit account

//// Types //////////////////////////////////////////////////////

//// Static function definitions ////////////////////////////////

static T_GL_COORD __APEMV_UI_MessageAuthorisationUpdate(T_GL_COORD value);
static void __APEMV_UI_PinEntry_Refresh(struct EPSTOOL_PinEntry_Infos_t *interface, T_GL_HWIDGET label, int pinLength);

//// Global variables ///////////////////////////////////////////

static unsigned long __APEMV_UI_AuthorisationStartTime;		//!< Used to display a fake progress bar during authorisation.
T_GL_HWIDGET __APEMV_GuiGoal_window = NULL;            ///< to avoid desktop display
T_GL_HWIDGET __APEMV_GuiGoal_windowPP = NULL;            ///< to avoid desktop display

//// Functions //////////////////////////////////////////////////

//! \brief Setup the user interface environment to perform a transaction.
int APEMV_UI_TransactionBegin(void)
{
	int result;

	result = 0;
	if (DisplayHeader(_OFF_) == _ON_)
		result |= 0x01;
	if (DisplayFooter(_OFF_) == _ON_)
		result |= 0x02;

	// create window2 to mask the desktop
	__APEMV_GuiGoal_window = GL_Window_Create(APEMV_UI_GoalHandle());

	GL_Window_Show(__APEMV_GuiGoal_window);
	GL_Window_Dispatch(__APEMV_GuiGoal_window, 10);

	if ((CUTERIsPinpadPresent() == B_TRUE) && CUTERMIsGraphicPP())
	{
		__APEMV_GuiGoal_windowPP = GL_Window_Create(APEMV_UI_GoalHandlePP());
		GL_Window_Show(__APEMV_GuiGoal_windowPP);
		GL_Window_Dispatch(__APEMV_GuiGoal_windowPP, 10);
	}

	EchoMessage_ppad(2);// XEROX  : For display on DDT screen , i send you :	011	001	TM,105	Veuillez introduire votre Carte Bancaire

	return result;
}

//! \brief Restore the user interface environment as it was before the transaction.
void APEMV_UI_TransactionEnd(int previous)
{
	DisplayHeader((previous & 0x01) ? _ON_ : _OFF_);
	DisplayFooter((previous & 0x02) ? _ON_ : _OFF_);

	if(__APEMV_GuiGoal_window != NULL)
	{   // Destroy the previous window
		GL_Widget_Destroy(__APEMV_GuiGoal_window);
		__APEMV_GuiGoal_window = NULL;
	}

	if(__APEMV_GuiGoal_windowPP != NULL)
	{   // Destroy the previous window
		GL_Widget_Destroy(__APEMV_GuiGoal_windowPP);
		__APEMV_GuiGoal_windowPP = NULL;
	}
}

//! \brief Display a message on pinpad if possible.
void APEMV_UI_Dialog_MessagePP(const char * text, T_GL_DIALOG_ICON icon, T_GL_DURATION duration)
{
	// Test if graphical function can be used on pinpad
	if (CUTERMIsGraphicPP())
	{
		GL_Dialog_Message(APEMV_UI_GoalHandlePP(), NULL, text, icon, GL_BUTTON_NONE, duration);
	}
	else
	{
		PPS_firstline();
		PPS_Display((char *)text);
	}
}

//! \brief Display the "Insert card" message.
void APEMV_UI_Dialog_Message(const char * text, T_GL_DIALOG_ICON icon, T_GL_DURATION duration)
{
	APEMV_UI_GoalOpen();

	if (CUTERIsPinpadPresent() == B_TRUE)
		APEMV_UI_Dialog_MessagePP(text, icon, 0);

	GL_Dialog_Message(APEMV_UI_GoalHandle(), NULL, text, icon, GL_BUTTON_NONE, duration);

	APEMV_UI_GoalClose();
}

//! \brief Display the "Insert card" message.
void APEMV_UI_MessageInsertCard(void)
{
	APEMV_UI_Dialog_Message("Insert card", GL_ICON_INFORMATION, 0);
}

//! \brief Display the "Remove card" message.
void APEMV_UI_MessageRemoveCard(void)
{
	APEMV_UI_Dialog_Message("Remove card", GL_ICON_INFORMATION, 0);
}

//! \brief Display the "Please wait" message.
void APEMV_UI_MessagePleaseWait(void)
{
	APEMV_UI_Dialog_Message("Please wait", GL_ICON_INFORMATION, 0);
}

//! \brief Display the "No remaining PIN" message.
void APEMV_UI_MessageNoRemainingPin(void)
{
	APEMV_UI_Dialog_Message("No remaining PIN", GL_ICON_WARNING, 2 * GL_TIME_SECOND);
}

//! \brief Display the "Wrong PIN" message.
void APEMV_UI_MessageWrongPin(void)
{
	APEMV_UI_Dialog_Message("Wrong PIN", GL_ICON_WARNING, 2 * GL_TIME_SECOND);
}

//! \brief Display the "Last attempt" message.
void APEMV_UI_MessagePinLastAttempt(void)
{
	APEMV_UI_Dialog_Message("Last attempt", GL_ICON_WARNING, 2 * GL_TIME_SECOND);
}

//! \brief Display the "Correct PIN" message.
void APEMV_UI_MessageCorrectPin(void)
{
	APEMV_UI_Dialog_Message("Correct PIN", GL_ICON_INFORMATION, 2 * GL_TIME_SECOND);
}

//! \brief Display the "Approved" message.
void APEMV_UI_MessageApproved(void)
{
	APEMV_UI_Dialog_Message("Approved", GL_ICON_INFORMATION, 2 * GL_TIME_SECOND);
}

//! \brief Display the "Declined" message.
void APEMV_UI_MessageDeclined(void)
{
	APEMV_UI_Dialog_Message("Declined", GL_ICON_ERROR, 2 * GL_TIME_SECOND);
}

//! \brief Display the "Service not allowed" message.
void APEMV_UI_MessageServiceNotAllowed(void)
{
	APEMV_UI_Dialog_Message("Service\nnot allowed", GL_ICON_ERROR, 2 * GL_TIME_SECOND);
}

//! \brief Display the "Transaction Cancelled" message.
void APEMV_UI_MessageTransactionCancelled(void)
{
	APEMV_UI_Dialog_Message("Transaction\ncancelled", GL_ICON_ERROR, 2 * GL_TIME_SECOND);
}

//! \brief Display the "Card blocked" message.
void APEMV_UI_MessageCardBlocked(void)
{
	APEMV_UI_Dialog_Message("Card blocked", GL_ICON_ERROR, 2 * GL_TIME_SECOND);
}

//! \brief Display the "Card removed" message.
void APEMV_UI_MessageCardRemoved(void)
{
	APEMV_UI_Dialog_Message("Card removed", GL_ICON_ERROR, 2 * GL_TIME_SECOND);
}

//! \brief Display the "Invalid card" message.
void APEMV_UI_MessageCardError(void)
{
	APEMV_UI_Dialog_Message("Invalid card", GL_ICON_ERROR, 2 * GL_TIME_SECOND);
}

//! \brief Display the "Transaction error" message.
void APEMV_UI_MessageTransactionError(void)
{
	APEMV_UI_Dialog_Message("Transaction\nerror", GL_ICON_ERROR, 2 * GL_TIME_SECOND);
}


//! \brief Display the message.
void APEMV_UI_MessageDisplay(char *message)
{
	ASSERT(message != NULL);

	APEMV_UI_Dialog_Message(message, GL_ICON_NONE, 2 * GL_TIME_SECOND);
}

void APEMV_UI_MessageDisplayDelay(char *message, int duration)
{
	DisplayHeader (_ON_);

	ASSERT(message != NULL);

	APEMV_UI_Dialog_Message(message, GL_ICON_NONE, duration * GL_TIME_SECOND);
}

//! \brief Display the message without time out
void APEMV_UI_InfoDisplay(char *message)
{
	ASSERT(message != NULL);

	APEMV_UI_Dialog_Message(message, GL_ICON_NONE, 0 * GL_TIME_SECOND);
}

//! \brief Called regularly by GOAL, from function \ref APEMV_UI_MessageAuthorisation.
//! Used to update the authorisation progress bar.
//! \param[in] value The actual value of the progress bar.
//! \return The new value of the progress bar.
//! \remarks This function is used to simulate an authorisation of 1 second.
static T_GL_COORD __APEMV_UI_MessageAuthorisationUpdate(T_GL_COORD value)
{
	value = ((100 * (TMT_Retrieve_Clock  () - __APEMV_UI_AuthorisationStartTime)) / 100);
	if (value > 100)
		value = 100;
	return value;
}

//! \brief Display the "Authorisation in progress" message with a progress bar.
//! \return \a TRUE if successful, \a FALSE if it has been cancelled.
//! \remarks This function is used to simulate an authorisation of 1 second.
int APEMV_UI_MessageAuthorisation(char * message)
{
	int cancel;

	APEMV_UI_GoalOpen();
	__APEMV_UI_AuthorisationStartTime = TMT_Retrieve_Clock ();
	if (CUTERIsPinpadPresent() == B_TRUE)
	{
		APEMV_UI_Dialog_MessagePP(message, GL_ICON_NONE, 0);
	}

	cancel = (GL_Dialog_Progress(APEMV_UI_GoalHandle(), NULL, message, NULL, NULL, 0, 100,
		__APEMV_UI_MessageAuthorisationUpdate, GL_BUTTON_CANCEL, GL_TIME_INFINITE) != GL_KEY_NONE);

	APEMV_UI_GoalClose();

	return !cancel;
}
/*
void APEMV_UI_MessageReferral(void)
{
}
*/

//! \brief Perform the signature capture and stores the signature in a file.
void APEMV_UI_RequestSignature(void)
{
	int previousHeader;
	int previousFooter;

	if (GL_GraphicLib_IsTouchPresent(APEMV_UI_GoalHandle()))
	{
		APEMV_UI_GoalOpen();

		// Remove the header and footer if present
		previousHeader = DisplayHeader(_OFF_);
		previousFooter = DisplayFooter(_OFF_);

		// Request for signature
		// TODO: Store the signature with the transaction data
		GL_Dialog_Signature(APEMV_UI_GoalHandle(), NULL, "Signature", "file://flash/HOST/SIGNATURE.SIG", GL_TIME_MINUTE);
		GL_File_Delete( "file://flash/HOST/SIGNATURE.SIG" );

		// Restore the header and footer context
		DisplayFooter(previousFooter);
		DisplayHeader(previousHeader);

		APEMV_UI_GoalClose();
	}
}

//! \brief Display a screen to select an AID.
//! \param[in] title Title to display
//! \param[in] numOfAids Number of AIDs.
//! \param[in] aids Possible AIDs.
//! \return Index of the selected AID (starting from 0), or (-1) if the input is cancelled, or (-2) if there is not enough memory.
int APEMV_UI_MenuSelectAid(const char* title, int numOfAids, const APEMV_UI_SelectAidItem_t *aids)
{
	int result;
	char **menu;
	int index;
	int usePreferredName;

	ASSERT(numOfAids > 0);
	ASSERT(aids != NULL);

	// Allocate memory to fill with the menu data
	menu = umalloc((numOfAids + 1) * sizeof(char*));
	if (menu != NULL)
	{
		memclr(menu, (numOfAids + 1) * sizeof(char*));

		// Set the menu item with the Application Label or with to Application Preferred Name
		result = 0;
		index = 0;
		while((index < numOfAids) && (result >= 0))
		{
			// Check if label or preferred name must be used
			usePreferredName = FALSE;
			if (aids[index].preferredName[0] != '\0')
			{
				// There is a Preferred Name

				// TODO: Check if the Issuer Code Table Index is supported or not.
				// By default, GOAL support ISO8859 1, 2, 3, 4, 9, 10, 13, 14, 15, 16.
				// But EMVCo limits it from ISO8859 1 to 10.
				switch(aids[index].issuerCodeTableIndex)
				{
				case 1:
				case 2:
				case 3:
				case 4:
				case 9:
				case 10:
					usePreferredName = TRUE;
					break;
				}
			}

			if (usePreferredName)
			{
				// Use the Application Preferred Name
				// Convert 'aids[index].preferredName' into UTF-8
				menu[index] = EPSTOOL_Unicode_CharsetToNewUtf8(aids[index].preferredName,
						sizeof(aids[index].preferredName),
						aids[index].issuerCodeTableIndex - 1 + EPSTOOL_UNICODE_ISO_8859_1);
				if (menu[index] == NULL)
				{
					result = -2;
				}
			}
			else if (aids[index].applicationLabel[0] != '\0')
			{
				// Use the Application Label (there is no need to convert it in UTF-8 as it is ASCII with no special characters)
				menu[index] = umalloc(strlen(aids[index].applicationLabel) + 1);
				if (menu[index] != NULL)
				{
					strcpy(menu[index], aids[index].applicationLabel);
				}
				else
				{
					result = -2;
				}
			}
			else
			{
				// There is neither Application Label nor Application Preferred Name
				// Use the AID instead
				menu[index] = umalloc(2 * aids[index].aidLength + 1);
				if (menu[index] != NULL)
				{
					// Convert the AID into an ASCII string
					GTL_Convert_DcbToAscii(aids[index].aid, menu[index], 0, 2 * aids[index].aidLength);
				}
				else
				{
					result = -2;
				}
			}
			index++;
		}

		if (index >= 0)
		{
			// Display the menu
			// Note that last item 'menu[numOfAids]' is equal to NULL because of the memclr(menu)
			if ((CUTERIsPinpadPresent() == B_TRUE) && (!CUTERMIsGraphicPP()))
				result = APEMV_UI_MenuSelectAlphaNumPP(menu);
			else
			{
				// It is a graphic terminal or pinpad
				if (CUTERIsPinpadPresent() == B_TRUE)
				{
					result = GL_Dialog_Menu(APEMV_UI_GoalHandlePP(), title,
								(const char * const *)menu, 0, GL_BUTTON_VALID_CANCEL, GL_KEY_NONE, 2 * GL_TIME_MINUTE);
				}
				else
				{
					APEMV_UI_GoalOpen();

					result = GL_Dialog_Menu(APEMV_UI_GoalHandle(), title,
						(const char * const *)menu, 0, GL_BUTTON_VALID_CANCEL, GL_KEY_NONE, 2 * GL_TIME_MINUTE);

					APEMV_UI_GoalClose();
				}
				if ((result < 0) || (result > numOfAids))
				{
					// Cancelled
					result = -1;
				}
			}
		}

		// Free the memory
		for(index = 0; index < numOfAids; index++)
		{
			if (menu[index] != NULL)
			{
				ufree(menu[index]);
			}
		}
		ufree(menu);
	}
	else
	{
		result = -2;
	}

	return result;
}


//! \brief Display a screen to select a value (cardholder language, AID...).
//! \param[in] title of scream displayed
//! \param[in] menu contains items to be selected
//! \param[in] forceOnMerchantScreen if TRUE, the menu is displayed on the merchant screen even if a pinpad is available.
//! \return index of selected language or(-1) if the input is not correct or cancelled.
ulong APEMV_UI_MenuSelectTopic(const char* title, const char* menu, int forceOnMerchantScreen)
{
	ulong result;

	ASSERT(title != NULL);
	ASSERT(menu != NULL);

	// Display the menu
	// Note that last item 'menu[]' is equal to NULL because of the memclr(menu)
	if ((CUTERIsPinpadPresent() == B_TRUE) && (forceOnMerchantScreen == FALSE))
		result = GL_Dialog_Menu(APEMV_UI_GoalHandlePP(), title, (const char * const *)(const void*)menu, 0, GL_BUTTON_VALID_CANCEL, GL_KEY_NONE, 2 * GL_TIME_MINUTE);
	else
	{
		APEMV_UI_GoalOpen();
		result = GL_Dialog_Menu(APEMV_UI_GoalHandle(), title, (const char * const *)(const void*)menu, 0, GL_BUTTON_VALID_CANCEL, GL_KEY_NONE, 2 * GL_TIME_MINUTE);
		APEMV_UI_GoalClose();
	}

	//if ((result < 0) || (result > numOfAids))
	if ((result < 0) || (result == GL_RESULT_INACTIVITY) || (result == GL_KEY_CANCEL))
	{
		// Cancelled
		result = -1;
	}


	return (result);
}

ulong APEMV_UI_MenuSelectAlphaNumPP(char **menu)
{
	ulong result;
	T_LABEL	Tab[20 + 1];
	int i, nbItem;

	for (i=0 ; menu[i] != NULL ; i++ )
	{
		strcpy((char *)Tab[i],(char *)(menu[i]));
	}

	nbItem = i;

//	result = Select_on_pinpad(Tab, nbItem);

	if ((result < 0) || (result >= nbItem))
	{
		// Cancelled
		result = -1;
	}

	return (result);
}


//! \brief Ask the merchant to force the transaction.
//! \return \a TRUE if transaction is forced by the merchant, \a FALSE if not.
int APEMV_UI_MenuForceAcceptance(const char* messag, const char* answer, const char* menu )
{
	/*const char *menu[]={
			"No",
			"Yes",
			NULL
	};*/
	int result;

	APEMV_UI_GoalOpen();
	GL_Dialog_Message(APEMV_UI_GoalHandle(), NULL, messag, GL_ICON_NONE, GL_BUTTON_NONE, 2 * GL_TIME_SECOND);
	result = (GL_Dialog_Menu(APEMV_UI_GoalHandle(), answer, (const char * const *)(const void*)menu, 0, GL_BUTTON_VALID_CANCEL, GL_KEY_NONE, 2 * GL_TIME_MINUTE) == 0);
	APEMV_UI_GoalClose();

	return result;
}

//! \brief Ask the merchant to accep or refuse a answer
//! \return \a TRUE if transaction is forced by the merchant, \a FALSE if not.
int APEMV_UI_MenuAcceptRefuse(char * answer, const char * menu)
{
/*	const char *menu[]={
			"No",
			"Yes",
			NULL
	};*/
	int result;

	APEMV_UI_GoalOpen();
//	GL_Dialog_Message(APEMV_UI_GoalHandle(), NULL, title, GL_ICON_WARNING, GL_BUTTON_NONE, 2 * GL_TIME_SECOND);
	result = (GL_Dialog_Menu(APEMV_UI_GoalHandle(), answer, (const char * const *)(const void*)menu, 0, GL_BUTTON_VALID_CANCEL, GL_KEY_NONE, 2 * GL_TIME_MINUTE) == 0);
	APEMV_UI_GoalClose();

	return result;
}
//! \brief Ask for an amount.
//! \param[in] text Test to display.
//! \param[in] currencyLabel String for the currency.
//! \param[out] amount Entered amount.
//! \return \a TRUE if the amount is entered, \a FALSE if not (cancelled ...).
int APEMV_UI_AmountEntry(const char *text, const char *currencyLabel, unsigned long *amount)
{
	int result;
	const char entryMask[] = "/d/d/d,/d/d/D./D/D";
	//const char currencyLabel[] = "EUR";
	char amountString[32 + 1];

	ASSERT(amount != NULL);

	APEMV_UI_GoalOpen();

	result = (GL_Dialog_Amount(APEMV_UI_GoalHandle(), NULL, text, entryMask, amountString,
			sizeof(amountString) - 1, currencyLabel, GL_ALIGN_RIGHT, 2 * GL_TIME_MINUTE) == GL_KEY_VALID);
	if (result)
	{
		EPSTOOL_Convert_AsciiToUl(amountString, -1 , amount);
	}

	APEMV_UI_GoalClose();

	return result;
}

int APEMV_UI_NumericalEntry(const char *title, const char *text, unsigned char *entryString, const char *entryMask )
{
	int result;

	ASSERT(title != NULL);
	ASSERT(text != NULL);
	ASSERT(entryString != NULL);
	ASSERT(entryMask != NULL);

	APEMV_UI_GoalOpen();

	result = (GL_Dialog_Text(APEMV_UI_GoalHandle(), title, text, entryMask, (char*)entryString, sizeof(entryString), 2 * GL_TIME_MINUTE)==GL_KEY_VALID) ;

	APEMV_UI_GoalClose();

	return result;
}


//! \brief Called when the display must be updated during a PIN entry.
//! \param[in] interface PIN entry parameters.
//! \param[in] label Handle of the GOAL widget that is used to display the PIN entry.
//! \param[in] pinLength Current length of the entered PIN.
static void __APEMV_UI_PinEntry_Refresh(struct EPSTOOL_PinEntry_Infos_t *interface, T_GL_HWIDGET label, int pinLength)
{
	const char defaultString[] = "- - - -";
	char string[64];
	int index;

	ASSERT(interface != NULL);
	ASSERT(label != NULL);

	if (pinLength <= 0)
	{
		// No PIN entered => display a default message
		if(CUTERIsPinpadPresent() && !CUTERMIsGraphicPP())
		{
			PPS_firstline ();
			PPS_Display ("ENTER PIN");
		}
		else
			GL_Widget_SetText(label, defaultString);
	}
	else
	{
		// PIN entry is in progress
		//  => Update the display

		// Check PIN length to avoid buffer overflows
		if (pinLength > (sizeof(string) / 2))
			pinLength = sizeof(string) / 2;

		// Format a string to have a star '*' and a space ' ' for each digit
		// Ex: for a 4 digits PIN, the string will be "* * * *"
		for(index = 0; index < pinLength; index++)
		{
			string[2 * index] = '*';
			string[(2 * index) + 1] = ' ';
		}
		string[(2 * (pinLength - 1)) + 1] = '\0';

		// Display the string that contains the stars
		if(CUTERIsPinpadPresent() && !CUTERMIsGraphicPP())
		{
			PPS_clearline ();
			PPS_Display (string);
		}
		else
			GL_Widget_SetText(label, string);
	}
}

//! \brief Ask for a PIN entry.
//! \param[in] pinType \a EMV_INT_CV_STATUS_OFFLINE_PIN for an offline PIN entry or \a EMV_INT_CV_STATUS_ONLINE_PIN for an online PIN entry.
//! \param[in] message_enter "enter pin" in cardholder language
//! \param[in] message_safe "safe from .." in cardholder language
//! \param[in] message_invalid "invalid" in cardholder language
//! \param[out] pinLength Length of entered PIN.
//! \return Any value of \ref EPSTOOL_PinEntry_Status_e.
EPSTOOL_PinEntry_Status_e APEMV_UI_PinEntry(int pinType, char * message_enter, char * message_safe, char * message_invalid, int *pinLength)
{
	EPSTOOL_PinEntry_Infos_t pinEntryInfo;
	EPSTOOL_PinEntry_Status_e status;

	APEMV_UI_GoalOpen();

	// Setup the PIN entry parameters
	pinEntryInfo.pinEntryConfig.ucEchoChar = '*';
	pinEntryInfo.pinEntryConfig.ucFontWidth = 0;
	pinEntryInfo.pinEntryConfig.ucFontHeight = 0;
	pinEntryInfo.pinEntryConfig.ucEchoLine = 0;
	pinEntryInfo.pinEntryConfig.ucEchoColumn = 0;
	pinEntryInfo.pinEntryConfig.ucMinDigits = 4;
	pinEntryInfo.pinEntryConfig.ucMaxDigits = 12;
	pinEntryInfo.pinEntryConfig.iFirstCharTimeOut = 60000;
	pinEntryInfo.pinEntryConfig.iInterCharTimeOut = 10000;
	if (pinType == OFFLINE_PIN)
	{
		// Offline PIN goes to the card reader
		pinEntryInfo.secureType = C_SEC_CARD;
	}
	else
	{
		// Online PIN goes to the PIN encryption module
		pinEntryInfo.secureType = C_SEC_PINCODE;
	}
	// TODO: Set here if PIN bypass is allowed or not
	pinEntryInfo.pinBypassAllowed = FALSE;
	// TODO: Set here some cancel events (CAM0 ...)
	pinEntryInfo.eventsToWait = CAM0 | CAM2;

	// Customisation functions
	pinEntryInfo.userEvents = NULL;
	pinEntryInfo.refresh = __APEMV_UI_PinEntry_Refresh;
	pinEntryInfo.privateData = NULL;

	do
	{
		if (CUTERIsPinpadPresent() == B_TRUE)
		{
			if (CUTERMIsGraphicPP())
				status = EPSTOOL_PinEntry(APEMV_UI_GoalHandlePP(), NULL, message_enter, message_safe, &pinEntryInfo, pinLength);
			else
				status = EPSTOOL_PinEntry(NULL, NULL, NULL, NULL, &pinEntryInfo, pinLength);
		}
		else
			status = EPSTOOL_PinEntry(APEMV_UI_GoalHandle(), NULL, message_enter, message_safe, &pinEntryInfo, pinLength);
		// If the PIN is not valid (too short), restart the PIN entry
		if (status == EPSTOOL_PINENTRY_INVALID)
		{
			if (CUTERIsPinpadPresent() == B_TRUE)
				APEMV_UI_Dialog_MessagePP(message_invalid, GL_ICON_WARNING, 2 * GL_TIME_SECOND);
			else
				GL_Dialog_Message(APEMV_UI_GoalHandle(), NULL, message_invalid, GL_ICON_WARNING, GL_BUTTON_NONE, 2 * GL_TIME_SECOND);
		}
	} while(status == EPSTOOL_PINENTRY_INVALID);

	APEMV_UI_GoalClose();

	return status;
}

//void Helper_DisplayTextMerchant(int increase, int line,MSGinfos msg,  ){
//
//}
