//! \file
//! Module that gives developers an helping interface for menu creation


/////////////////////////////////////////////////////////////////
//// Includes ///////////////////////////////////////////////////

#include "ClessSample_Implementation.h"

/////////////////////////////////////////////////////////////////
//// Macros & preprocessor definitions //////////////////////////

/////////////////////////////////////////////////////////////////
//// Types //////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
//// Global variables ///////////////////////////////////////////

static T_GL_HGRAPHIC_LIB gs_goalHandle = NULL;	//!< The GOAL handle.
static Telium_File_t *gs_displayDriver = NULL;			//!< The display driver handle.
static Telium_File_t *gs_keyboardDriver = NULL;			//!< The keyboard driver handle.
static Telium_File_t *gs_touchDriver = NULL;				//!< The touch screen driver handle.
static int gs_previous = 0;
static T_GL_HWIDGET gs_background = NULL;

static MSGinfos gs_MessageLine_1;							/*!< Structure containing the 1st line message. */
static MSGinfos gs_MessageLine_2;							/*!< Structure containing the 2nd line message. */
static MSGinfos gs_MessageLine_3;							/*!< Structure containing the 3rd line message. */
static MSGinfos gs_MessageLine_4;							/*!< Structure containing the 4th line message. */
static MSGinfos gs_MessageLine_5;							/*!< Structure containing the 5th line message. */
static MSGinfos gs_MessageLine_6;							/*!< Structure containing the 6th line message. */

static const unsigned char gs_ucBlankLine[] = " ";

/////////////////////////////////////////////////////////////////
//// Static function definitions ////////////////////////////////

static void __ClessSample_Goal_PinEntryRefresh(struct PinEntry_Infos_t *interface, T_GL_HWIDGET label, int pinLength);
static void __ClessSample_Goal_ClearMessageStructure (MSGinfos * pMessageStructure);
static void __ClessSample_Goal_ClearScreen (void);

/////////////////////////////////////////////////////////////////
//// Functions //////////////////////////////////////////////////

int ClessSample_Goal_IsAvailable(void)
{
	return (ClessSample_Goal_Handle() != NULL);
}

//! \brief Create the graphic library instance.
//! \return  pointer to graphic library handle.
T_GL_HGRAPHIC_LIB ClessSample_Goal_Handle(void)
{
	if (gs_goalHandle == NULL)
	{
		if (GL_IsPresent())
		{
			gs_goalHandle = GL_GraphicLib_Create();
			if (gs_goalHandle != NULL)
			{
				// Hide header and footer
				gs_previous = 0;
				if (DisplayHeader(_OFF_) == _ON_)
					gs_previous |= 0x01;
				if (DisplayFooter(_OFF_) == _ON_)
					gs_previous |= 0x02;

				// Create background window
				gs_background = GL_Window_Create(gs_goalHandle);
				GL_Widget_SetSize(gs_background, 100, 100, GL_UNIT_PERCENT);
				GL_Widget_SetBackColor(gs_background, 0xff000000);
				GL_Window_Show(gs_background);
				GL_Window_Dispatch(gs_background, 0);
			}
		}
	}

	return gs_goalHandle;
}

//! \brief Destroy the graphic library instance.
void ClessSample_Goal_Destroy(void)
{
	if (gs_goalHandle != NULL)
	{
		if (gs_background != NULL)
		{
			GL_Widget_Destroy(gs_background);
			gs_background = NULL;
		}

		GL_GraphicLib_Destroy(gs_goalHandle);
		gs_goalHandle = NULL;
		DisplayHeader((gs_previous & 0x01) ? _ON_ : _OFF_);
		DisplayFooter((gs_previous & 0x02) ? _ON_ : _OFF_);
	}
}

void ClessSample_Goal_ClearScreen(void)
{
#ifdef __TELIUM3__
	if (gs_background != NULL)
		GL_Window_Dispatch(gs_background, 0);
#else
	MSGinfos tMsg;
	tMsg.message = " ";
	Helper_DisplayTextCustomer(ERASE, HELPERS_MERCHANT_LINE_3, &tMsg, CLESSSAMPLE_ALIGN_CENTER, NOLEDSOFF);
	Helper_RefreshScreen(NOWAIT, HELPERS_CUSTOMER_SCREEN);
#endif
}

//! \brief Open the GOAL interface (open drivers ...).
void ClessSample_Goal_Open(void)
{
#ifndef __TELIUM3__
	if (gs_displayDriver == NULL)
		gs_displayDriver = Telium_Fopen("DISPLAY", "w*");
	if (gs_keyboardDriver == NULL)
	{
		//gs_keyboardDriver = Telium_Fopen("KEYBOARD", "r*");
		//APEMV_UI_MessageDisplayDelay("BBBB.01.xx",2);
	}
	if (gs_touchDriver == NULL)
		gs_touchDriver = Telium_Fopen("TSCREEN", "r*");
#endif
}

//! \brief Close the GOAL interface (close drivers ...).
void ClessSample_Goal_Close(void)
{
	if (gs_displayDriver != NULL)
	{
		Telium_Fclose(gs_displayDriver);
		gs_displayDriver = NULL;
	}
	if (gs_keyboardDriver != NULL)
	{
		Telium_Fclose(gs_keyboardDriver);
		gs_keyboardDriver = NULL;
	}
	if (gs_touchDriver != NULL)
	{
		Telium_Fclose(gs_touchDriver);
		gs_touchDriver = NULL;
	}
}

//! \brief Displays a message with a choice (validate, cancel).
//! \param[in] MessageL1 Message displayed on the first line.
//! \param[in] MessageL2 Message displayed on the second line.
//! \return
//!		- \ref TRUE if user selected the left choice.
//!		- \ref FALSE if user selected the right choice.

int ClessSample_Goal_DisplayChoice(char* MessageL1, char* MessageL2)
{
	int result;
	char text[256];

	text[0] = 0;
	if (MessageL1 != NULL)
		strcpy(text, MessageL1);
	strcat(text, " \n");
	if (MessageL2 != NULL)
		strcpy(text, MessageL2);

	ClessSample_Goal_Open();
	result = (GL_Dialog_Message(ClessSample_Goal_Handle(), NULL, text, GL_ICON_QUESTION, GL_BUTTON_VALID_CANCEL, GL_TIME_INFINITE)
			== GL_KEY_VALID);
	ClessSample_Goal_Close();

	return result;
}

//! \brief Displays a message with signature capture and a choice (validate, cancel).
//! \param[in] LeftChoice Message to validate (function returns \ref TRUE in this case).
//! \param[in] LeftChoice Message to cancel (function returns \ref FALSE in this case).
//! \param[in] MessageL1 Message displayed on the first line.
//! \param[in] MessageL2 Message displayed on the second line.
//! \return
//!		- \ref TRUE if user selected the left choice.
//!		- \ref FALSE if user selected the right choice.

int ClessSample_Goal_SignatureCapture (char* MessageL1, char* MessageL2)
{
	int result;
	char text[256];
	int previousHeader;
	int previousFooter;

	if (GL_GraphicLib_IsTouchPresent(ClessSample_Goal_Handle()))
	{
		text[0] = 0;
		if (MessageL1 != NULL)
			strcpy(text, MessageL1);
		strcat(text, " \n");
		if (MessageL2 != NULL)
			strcpy(text, MessageL2);

		// Remove the header and footer if present
		previousHeader = DisplayHeader(_OFF_);
		previousFooter = DisplayFooter(_OFF_);

		// Request for signature
		// TODO: Store the signature with the transaction data
		ClessSample_Goal_Open();
		result = (GL_Dialog_Signature(ClessSample_Goal_Handle(), NULL, text, "file://flash/HOST/SIGNATURE.SIG", GL_TIME_INFINITE)
				== GL_KEY_VALID);
		ClessSample_Goal_Close();
		GL_File_Delete( "file://flash/HOST/SIGNATURE.SIG" );

		// Restore the header and footer context
		DisplayFooter(previousFooter);
		DisplayHeader(previousHeader);
	}
	else result = GL_KEY_CANCEL;

	return result;
}

//! \brief Called when the display must be updated during a PIN entry.
//! \param[in] interface PIN entry parameters.
//! \param[in] label Handle of the GOAL widget that is used to display the PIN entry.
//! \param[in] pinLength Current length of the entered PIN.
static void __ClessSample_Goal_PinEntryRefresh(struct PinEntry_Infos_t *interface, T_GL_HWIDGET label, int pinLength)
{
	const char defaultString[] = "- - - -";
	char string[64];
	int index;

	if (pinLength <= 0)
	{
		// No PIN entered => display a default message
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
		GL_Widget_SetText(label, string);
	}
}

//! \brief Ask for a PIN entry.
//! \param[in] pinType \a INPUT_PIN_OFF for an offline PIN entry or \a INPUT_PIN_ON for an online PIN entry.
//! \param[in] pinTryCounter The PIN try counter from the card. Give (-1) if unknown.
//! \param[out] pinLength Length of entered PIN.
//! \return Any value of \ref PinEntry_Status_e.
PinEntry_Status_e ClessSample_Goal_PinEntry(const char *title, const char *text, const char *help, int *pinLength)
{
	PinEntry_Infos_t pinEntryInfo;
	PinEntry_Status_e status;

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
	// Online PIN goes to the PIN encryption module
	pinEntryInfo.secureType = C_SEC_PINCODE;

	// TODO: Set here if PIN bypass is allowed or not
	pinEntryInfo.pinBypassAllowed = FALSE;
	// TODO: Set here some cancel events (CAM0 ...)
	pinEntryInfo.eventsToWait = 0;

	// Customisation functions
	pinEntryInfo.userEvents = NULL;
	pinEntryInfo.refresh = __ClessSample_Goal_PinEntryRefresh;
	pinEntryInfo.privateData = NULL;

	// Ask for the PIN
	ClessSample_Goal_Open();
	status = ClessSample_PinEntryGoal(ClessSample_Goal_Handle(), NULL, text,
			help, &pinEntryInfo, pinLength);
	ClessSample_Goal_Close();

	return status;
}

//! \brief Clears a message structure.
//! \param[in] pMessageStructure Message structure to be cleared.

static void __ClessSample_Goal_ClearMessageStructure (MSGinfos * pMessageStructure)
{
	if (pMessageStructure != NULL)
		memset (pMessageStructure, 0, sizeof(MSGinfos));
}



//! \brief Clears all the messages.

static void __ClessSample_Goal_ClearScreen (void)
{
	__ClessSample_Goal_ClearMessageStructure (&gs_MessageLine_1);
	__ClessSample_Goal_ClearMessageStructure (&gs_MessageLine_2);
	__ClessSample_Goal_ClearMessageStructure (&gs_MessageLine_3);
	__ClessSample_Goal_ClearMessageStructure (&gs_MessageLine_4);
	__ClessSample_Goal_ClearMessageStructure (&gs_MessageLine_5);
	__ClessSample_Goal_ClearMessageStructure (&gs_MessageLine_6);
}



//! \brief Display a message on the diaply using WGUI.
//! \param[in] nline Line where the message shall be displayed.
//! \param[in] ptrMSG Message structure to be used.
//! \param[in] bmode : TRUE to erase before display something, else nothing

void ClessSample_Goal_SetMessage (int nline, MSGinfos * ptrMSG, unsigned char bMode)
{
	if (bMode == YESCLEAR) // Clear the screen
		__ClessSample_Goal_ClearScreen();

	switch (nline)
	{
	case (HELPERS_CUSTOMER_LINE_1):
	///case (HELPERS_MERCHANT_LINE_1):
		if (ptrMSG != NULL)
			memcpy (&gs_MessageLine_1, ptrMSG, sizeof (MSGinfos));
		else
			__ClessSample_Goal_ClearMessageStructure (&gs_MessageLine_1);
	break;

	case (HELPERS_CUSTOMER_LINE_2):
	///case (HELPERS_MERCHANT_LINE_2):
		if (ptrMSG != NULL)
			memcpy (&gs_MessageLine_2, ptrMSG, sizeof (MSGinfos));
		else
			__ClessSample_Goal_ClearMessageStructure (&gs_MessageLine_2);
	break;

	case (HELPERS_CUSTOMER_LINE_3):
	///case (HELPERS_MERCHANT_LINE_3):
		if (ptrMSG != NULL)
			memcpy (&gs_MessageLine_3, ptrMSG, sizeof (MSGinfos));
		else
			__ClessSample_Goal_ClearMessageStructure (&gs_MessageLine_3);
	break;

	case (HELPERS_CUSTOMER_LINE_4):
	///case (HELPERS_MERCHANT_LINE_4):
		if (ptrMSG != NULL)
			memcpy (&gs_MessageLine_4, ptrMSG, sizeof (MSGinfos));
		else
			__ClessSample_Goal_ClearMessageStructure (&gs_MessageLine_4);
	break;

	case (HELPERS_CUSTOMER_LINE_5):
	///case (HELPERS_MERCHANT_LINE_5):
		if (ptrMSG != NULL)
			memcpy (&gs_MessageLine_5, ptrMSG, sizeof (MSGinfos));
		else
			__ClessSample_Goal_ClearMessageStructure (&gs_MessageLine_5);
	break;

	case (HELPERS_CUSTOMER_LINE_6):
	///case (HELPERS_MERCHANT_LINE_6):
		if (ptrMSG != NULL)
			memcpy (&gs_MessageLine_6, ptrMSG, sizeof (MSGinfos));
		else
			__ClessSample_Goal_ClearMessageStructure (&gs_MessageLine_6);
	break;

	default:
		break;
	}
}


static T_GL_HWIDGET gs_myMainWindow = NULL;

// En gros ça fait comme GL_Dialog_Message mais ça ne détruit pas la fenêtre à la fin.
void _GL_Dialog_Message_(T_GL_HGRAPHIC_LIB graphiclib, const char * title, const char * text, T_GL_DURATION duration)
{
	T_GL_HWIDGET label_titre;
	T_GL_HWIDGET label_text;
	T_GL_HWIDGET layout;
	int          row = 0;

	if (gs_myMainWindow)
	{
		GL_Widget_Destroy(gs_myMainWindow);
		gs_myMainWindow = NULL;
	}

	// Create My Window
	gs_myMainWindow = GL_Window_Create(graphiclib);
	// Create layout for text and title
	layout = GL_Layout_Create(gs_myMainWindow);
	// Create title
	if (title != NULL)
	{
		// Creating title into layout
		label_titre = GL_Label_Create(layout);
		GL_Widget_SetItem(label_titre, 0, row++);
		GL_Widget_SetGrow(label_titre, GL_DIRECTION_WIDTH);
		GL_Widget_SetExpand(label_titre, GL_DIRECTION_WIDTH);
		GL_Widget_SetBackColor(label_titre, GL_COLOR_WINDOW_BORDER);
		GL_Widget_SetForeColor(label_titre, GL_COLOR_WINDOW_BACKGROUND);
		GL_Widget_SetText(label_titre, title);
	}

	// Create text
	label_text = GL_Label_Create(layout);
	GL_Widget_SetItem(label_text, 0, row++);
	GL_Widget_SetText(label_text, text);
	GL_Window_Dispatch(gs_myMainWindow, duration);
}


//! \brief Refresh the screen

void ClessSample_Goal_RefreshScreen (void)
{
	char sStringToDisplay[256];

	// Init the string
	memset (sStringToDisplay, '\0', sizeof(sStringToDisplay));

	if (gs_MessageLine_1.message != NULL)
		strcat (sStringToDisplay, gs_MessageLine_1.message);
	strcat (sStringToDisplay, "\n");
	if (gs_MessageLine_2.message != NULL)
		strcat (sStringToDisplay, gs_MessageLine_2.message);
	strcat (sStringToDisplay, "\n");
	if (gs_MessageLine_3.message != NULL)
		strcat (sStringToDisplay, gs_MessageLine_3.message);
	strcat (sStringToDisplay, "\n");
	if (gs_MessageLine_4.message != NULL)
		strcat (sStringToDisplay, gs_MessageLine_4.message);
	strcat (sStringToDisplay, "\n");
	if (gs_MessageLine_5.message != NULL)
		strcat (sStringToDisplay, gs_MessageLine_5.message);
	strcat (sStringToDisplay, "\n");
	if (gs_MessageLine_6.message != NULL)
		strcat (sStringToDisplay, gs_MessageLine_6.message);
	//strcat (sStringToDisplay, "\n");

	ClessSample_Goal_Open();

	if (IsColorDisplay())
		GL_Dialog_Message(ClessSample_Goal_Handle(), NULL, sStringToDisplay, GL_ICON_NONE, GL_BUTTON_NONE, 0);
	else
		_GL_Dialog_Message_(ClessSample_Goal_Handle(), NULL, sStringToDisplay, 0);

	ClessSample_Goal_Close();
}

//! \brief Display a message on the diaply using WGUI.
//! \param[in] nline Line where the message shall be displayed.
//! \param[in] bmode : TRUE to erase before display something, else nothing

void ClessSample_Goal_SetBlankLine (int nline, unsigned char bMode)
{
	if (bMode == YESCLEAR) // Clear the screen
		__ClessSample_Goal_ClearScreen();

	switch (nline)
	{
	case (HELPERS_CUSTOMER_LINE_1):
	///case (HELPERS_MERCHANT_LINE_1):
		gs_MessageLine_1.message = (char *)gs_ucBlankLine;
	break;

	case (HELPERS_CUSTOMER_LINE_2):
	///case (HELPERS_MERCHANT_LINE_2):
		gs_MessageLine_2.message = (char *)gs_ucBlankLine;
	break;

	case (HELPERS_CUSTOMER_LINE_3):
	///case (HELPERS_MERCHANT_LINE_3):
		gs_MessageLine_3.message = (char *)gs_ucBlankLine;
	break;

	case (HELPERS_CUSTOMER_LINE_4):
	///case (HELPERS_MERCHANT_LINE_4):
		gs_MessageLine_4.message = (char *)gs_ucBlankLine;
	break;

	case (HELPERS_CUSTOMER_LINE_5):
	///case (HELPERS_MERCHANT_LINE_5):
		gs_MessageLine_5.message = (char *)gs_ucBlankLine;
	break;

	case (HELPERS_CUSTOMER_LINE_6):
	///case (HELPERS_MERCHANT_LINE_6):
		gs_MessageLine_6.message = (char *)gs_ucBlankLine;
	break;

	default:
		break;
	}
}
