/**
* \file cu_mess.c
* \brief This module contains the functions used for message management.
*
* \author Ingenico France
* \author Copyright (c) 2009 Ingenico France, 1 rue claude Chappe,\n
* 07503 Guilherand-Granges, France, All Rights Reserved.
*
* \author Ingenico France has intellectual property rights relating to the technology embodied\n
* in this software. In particular, and without limitation, these intellectual property rights may\n
* include one or more patents.\n
* This software is distributed under licenses restricting its use, copying, distribution, and\n
* and decompilation. No part of this software may be reproduced in any form by any means\n
* without prior written authorization of Ingenico France.
**/

/* ======================================================================== */
/* Include Files                                                            */
/* ======================================================================== */

//#include "sdk.h" // IngeDev automatic refactoring - 2014/04/29 14:29:58 - Replace '#include "SDK30.H"' to '#include "sdk.h"'
#include "SDK_Naps.h"
#include "_emvdctag_.h"
#include "TlvTree.h"
#include "cu_base.h"
#include "cu_term.h"
#include "cu_mess.h"
#include "MSGlib.h"
#include "MessagesDefinitions.h"
#include "cu_serv.h"
#include "def_tag.h"

#include "sec_interface.h"
#include "GL_GraphicLib.h"
#include "GTL_Assert.h"
#include "GTL_Convert.h"

#include "EPSTOOL_Convert.h"
#include "EPSTOOL_TlvTree.h"
#include "EPSTOOL_PinEntry.h"
#include "EPSTOOL_Unicode.h"
#include "cu_UserInterfaceDisplay.h"

/* ======================================================================== */
/* Macros & preprocessor definitions                                        */
/* ======================================================================== */

/* =========================================================================*/
/* Types declarations                                                       */
/* =========================================================================*/
#ifdef OUTPUT_MSG_ON_COM
void CUMESS_SendMsg(char * Msg);
#endif

/* ======================================================================== */
/* Constant declarations                                                    */
/* ======================================================================== */

// Multilanguage management
const t_lang_code lang_table[C_NBLANGHOLDER]=
{
	{"en"    ,"ENGLISH"    ,0       },
	{"fr"    ,"FRANCAIS"   ,1       },
	{"es"    ,"ESPANOL"    ,0xFF    }    // Not supported : english messages
};

/* Messages 01 to 13 are defined in EMV96, spec for payment systems         */
/* PART III, 1.2                                                            */
//!
//! \todo User's Interface implementation :
//! Modify the tables of messages to fit your needs.
//! You may need also to create new characters.
//!

const S_TAB_ICS_CONFIG table_ics_config[] =
{
	{ICS_MESS_TERMINAL_TYPE                         ,TAG_TERMINAL_TYPE                          },
	{ICS_MESS_TERMINAL_COUNTRY_CODE                 ,TAG_TERMINAL_COUNTRY_CODE                  },
	{ICS_MESS_TERMINAL_CURRENCY                     ,TAG_TRANSACTION_CURRENCY_CODE              },
	{ICS_MESS_TERMINAL_CAPABILITY                   ,TAG_TERMINAL_CAPABILITIES                  },
	{ICS_MESS_ADD_TERMINAL_CAPABILITY               ,TAG_ADD_TERMINAL_CAPABILITIES              },
	{ICS_MESS_PSE                                   ,TAG_USE_PSE                                },
	{ICS_MESS_CARDHOLDER_CONFIRMATION               ,TAG_CUST_IS_CARDHOLDER_CONFIRMATION        },
	{ICS_MESS_PREFERRED_ORDER_OF_DISPLAY            ,TAG_CUST_IS_PREFERRED_DISPLAY_ORDER        },
	{ICS_MESS_MULTI_LANGUAGE                        ,TAG_CUST_IS_MULTILANG_SUPPORTED            },
	{ICS_MESS_REVOCATION_OF_IPK_CERT                ,TAG_CUST_IS_REVOK_SUPPORTED                },
	{ICS_MESS_DEFAULT_DDOL                          ,TAG_CUST_IS_DEFAULT_DDOL                   },
	{ICS_MESS_BYPASS_PIN_ENTRY                      ,TAG_CUST_IS_PIN_BYPASS                     },
	{ICS_MESS_GETDATA_FOR_PIN_TRY_COUNTER           ,TAG_CUST_IS_GETDATA_PIN_TRY_COUNTER        },
	{ICS_MESS_FLOOR_LIMIT_CHECKING                  ,TAG_CUST_IS_FLOOR_LIMIT_CHECKING           },
	{ICS_MESS_RANDOM_TRANSACTION_SELECTION          ,TAG_CUST_IS_RANDOM_TRANSACTION_SELECT      },
	{ICS_MESS_VELOCITY_CHECKING                     ,TAG_CUST_IS_VELOCITY_CHECKING              },
	{ICS_MESS_TRANSACTION_LOG                       ,TAG_CUST_IS_TRANSACTION_LOG                },
	{ICS_MESS_EXCEPTION_FILE                        ,TAG_CUST_IS_EXCEPTION_FILE                 },
	{ICS_MESS_FORCE_ONLINE                          ,TAG_CUST_IS_FORCED_ONLINE                  },
	{ICS_MESS_FOERCE_ACCEPTANCE                     ,TAG_CUST_IS_FORCED_ACCEPTANCE              },
	{ICS_MESS_ADVICES                               ,TAG_CUST_IS_ONLINE_ADVICE                  },
	{ICS_MESS_ISSUER_REFFERALS                      ,TAG_CUST_IS_ISSUER_REFERRAL                },
	{ICS_MESS_CARD_REFFERALS                        ,TAG_CUST_IS_CARD_REFERRAL                  },
	{ICS_MESS_BATCH_DATA_CAPTURE                    ,TAG_CUST_IS_BATCH_CAPTURE                  },
	{ICS_MESS_ONLINE_DATA_CAPTURE                   ,TAG_CUST_IS_ONLINE_CAPTURE                 },
	{ICS_MESS_DEFAULT_TDOL                          ,TAG_CUST_IS_DEFAULT_TDOL                   },
	{ICS_MESS_POS_ENTRY_MODE                        ,TAG_CUST_IS_POS_ENTRY_MODE                 },
	{ICS_MESS_POS_ENTRY_VALUE                       ,TAG_POS_ENTRY_MODE_CODE                    },
	{ICS_MESS_PINPAD                                ,TAG_CUST_IS_PINPAD                         },
	{ICS_MESS_AMOUNT_AND_PIN_ENTERED_ON_SAME_KEYPAD ,TAG_CUST_IS_AMOUNT_PIN_SAME                },
	{ICS_MESS_TRM_OVERPASS_AIP                      ,TAG_CUST_TRM_OVERPASS_AIP                  },
	{ICS_MESS_SKIP_TAC_IAC_DAFAULT                  ,TAG_CUST_IS_SKIP_DEFAULT                   },
	{ICS_MESS_SKIP_TAC_IAC_ONLINE                   ,TAG_CUST_IS_SKIP_ONLINE                    },
	{ICS_MESS_ODA                                   ,TAG_CUST_IS_ODA_PROCESSING                 },
	{ICS_MESS_ACCOUNT_TYPE                          ,TAG_CUST_IS_ACCOUNT_TYPE                   },
	{ICS_MESS_PSEALGO                               ,TAG_PSE_ALGO                               },
	{ICS_MESS_PRIOR_TAA                             ,TAG_CUST_DETECT_FAIL_PRIOR_TAA             },
	{ICS_MESS_NEVER_GENAC1                          ,TAG_CUST_CDA_NEVER_REQUEST_ARQC_1GENAC     },
	{ICS_MESS_NEVER_GENAC2                          ,TAG_CUST_CDA_NEVER_REQUEST_ONLINE_2GENAC   },
	{ICS_MESS_SUBSEQUENT_PIN_BYPASS                 ,TAG_CUST_SUBSEQUENT_BYPASS_PIN             },
	{ICS_MESS_AMOUNT_UNKNOWN_BEF_CVM                ,TAG_CUST_IS_AMOUNT_UNKNOWN_BEFORE_CVM      },
	{ICS_MESS_SELECTABLE_KERNEL                     ,TAG_CUST_IS_SELECTABLE_KERNEL				},
	{ICS_MESS_DELETABLE_TAC                         ,TAG_CUST_IS_DELETABLE_TAC					},
	{ICS_MESS_DEFAULT_TDOL_TVR_SET_AFTER_1ST_GENAC  ,TAG_CUST_DEFAULT_TDOL_TVR_SET_AFTER_1ST_GENAC},
	{ICS_MESS_NUMBER_OF_SUPPORTED_AIDS              ,TAG_CUST_NUMBER_OF_SUPPORTED_AIDS			},
	{ICS_MESS_IS_RECEIPT                            ,TAG_CUST_IS_RECEIPT						},
	{ICS_MESS_IS_STORE_DECLINED_TRANSACTIONS        ,TAG_CUST_IS_STORE_DECLINED_TRANSACTIONS	},
	{(int)NULL                                      ,0                                          }
};


const unsigned char TimeOutPINCode[] =
{
	0x17,			// 60 seconds
	0x70
};

const unsigned char TimeOutInterCar[] =
{
	0x07,			// 20 seconds
	0xd0
};

/*-------------------------------------------------------------------------*/
/* Specific constants for currency management                              */
/*-------------------------------------------------------------------------*/

/* Definition of currency configuration                                    */

const T_AppConfCurcy ct_confCurrency [L_NBCURCY] =
{
	{C_IEP_CURCY_BEF , "BEF", 2},
	{C_IEP_CURCY_NLG , "NLG", 2},
	{C_IEP_CURCY_CHF , "CHF", 2},
	{C_IEP_CURCY_USD , "USD", 2},
	{C_IEP_CURCY_BRL , "BRL", 2},
	{C_IEP_CURCY_CAD , "CAD", 2},
	{C_IEP_CURCY_AUD , "AUD", 2},
	{C_IEP_CURCY_HKD , "HKD", 2},
	{C_IEP_CURCY_NZD , "NZD", 2},
	{C_IEP_CURCY_ESP , "ESP", 0},
	{C_IEP_CURCY_FRF , "FRF", 2},
	{C_IEP_CURCY_ARS , "ARS", 2},
	{C_IEP_CURCY_ATS , "ATS", 2},
	{C_IEP_CURCY_CLP , "CLP", 2},
	{C_IEP_CURCY_CLF , "CLF", 2},
	{C_IEP_CURCY_CRC , "CRC", 2},
	{C_IEP_CURCY_HRD , "HRK", 2},
	{C_IEP_CURCY_HNL , "HNL", 2},
	{C_IEP_CURCY_INR , "INR", 2},
	{C_IEP_CURCY_IDR , "IDR", 2},
	{C_IEP_CURCY_ILS , "ILS", 2},
	{C_IEP_CURCY_LUF , "LUF", 0},
	{C_IEP_CURCY_MYR , "MYR", 2},
	{C_IEP_CURCY_MXN , "MXN", 2},
	{C_IEP_CURCY_NOK , "NOK", 2},
	{C_IEP_CURCY_PHP , "PHP", 2},
	{C_IEP_CURCY_SEK , "SEK", 2},
	{C_IEP_CURCY_THB , "THB", 2},
	{C_IEP_CURCY_TRL , "TRL", 2},
	{C_IEP_CURCY_AED , "AED", 2},
	{C_IEP_CURCY_XEU , "XEU", 2},
	{C_IEP_CURCY_EURO, "EUR", 2},
	{C_IEP_CURCY_HUF,  "HUF", 0},
	{C_IEP_CURCY_POUND,"GBP", 2},
	{C_IEP_CURCY_MAD, "MAD", 2},
};

/*-------------------------------------------------------------------------*/
/* Constant definitions for global parameters                              */
/*-------------------------------------------------------------------------*/

/* Terminal Capabilities :                                                 */
/* IC with contacts                                                        */
/* Plaintext PIN for ICC verification                                      */
/* Enciphered PIN for online verification                                  */
/* Static Data Authentification                                            */
/* Dynamic Data Authentification                                           */

/* Additional Terminal Capabilities :                                      */
/* goods                                                                   */
/* Numeric Keys                                                            */
/* Function Keys                                                           */
/* Print Attendant                                                         */
/* Display, attendant                                                      */


/* ========================================================================	*/
/* Global variables                                                         */
/* ========================================================================	*/

MSGinfos CurrentMSG;
extern const unsigned char cu_lang[];				// Default messages in LANG


// All those following datas are reserved for a future use
// so they are useless for the moment
unsigned char ct_KernelSupportToolWare;
unsigned char ct_SystemSupportToolWare;
char ct_KernelVersion[20];
unsigned long ct_KernelCRC;
unsigned long ct_SystemVersion;
unsigned long ct_SystemCRC;

unsigned long ul_InputAmount;

extern int UCM_Present;

#define ID_MONTANT 1 // jrizki
#define ID_CONFIRM_IMG 2 // jrizki
#define ID_CANCEL_IMG 3 // jrizki

int MessageWithoutConfirmation(char* title_widget, char* image); // jrizki 14/01/2019
int MessageWithConfirmation(char* title); // jrizki 14/01/2019
int MessageWithWaitForAction(char* title, char* image, int delay); // jrizki 15/01/2019
int MessageWithConfirmation_(char* title); // jrizki 14/01/2019
int MessageWithDelay(char* title, char* image, int delay); // jrizki 190219
void detectCard(void); // jrizki 180419
void traceInt(int nb, int mode);
void traceString(char *mess);

char globalBuffer[50];


/* ===========================Traitement montant par jrizki =============================== */

typedef struct IntBuffer IntBuffer;


struct IntBuffer
{
    int nombre;
    IntBuffer *suivant;
};

typedef struct Pile Pile;
struct Pile
{
	IntBuffer *premier;
};

Pile *initialiser();
void empiler(Pile *pile, int nvNombre);
int depiler(Pile *pile);
char *getAmountStr(Pile *pile);
char *getReaderBuffer(Pile *pile);

Pile *amount = NULL;
Pile *reader_buffer = NULL;
int max_reader_buffer = 0;
int courant_reader_buffer = 0;

/* ====================================================================================== */

/* ========================================================================*/
/* External functions                                                      */
/* ========================================================================*/

/* ======================================================================== */
/* Functions                                                                */
/* ======================================================================== */

//===========================================================================
void CUMESS_Initialise(void)
{
	static int bInitialised = FALSE;
	char *NewMessages;     // Address of the loaded message file

	if (bInitialised)
		return;

	bInitialised = TRUE;

	//trace(0, strlen("CUMESS_Initialise()\n"), "CUMESS_Initialise()\n");

	// Load the message file in memory
	NewMessages=LoadMSG("/SYSTEM/LANG.SGN");
	if(NewMessages == NULL )
	{
		//trace(0, strlen("MSG file not found :\n /SYSTEM/LANG.SGN"), "MSG file not found :\n /SYSTEM/LANG.SGN");
		// Local messages are used
		DefCurrentMSG((char *)cu_lang); // Higher priority table
	}
	else
	{
		//trace(0, strlen("MSG file loaded :\n /SYSTEM/LANG.SGN"), "MSG file loaded :\n /SYSTEM/LANG.SGN");
		// New messages are used
		DefCurrentMSG(NewMessages);     // Higher priority table
	}

	DefDefaultMSG((char *)cu_lang); // If msg not found in higher priority table, this one will be used
	// Define current and default languages (English)
	DefCurrentLang(EN);
	DefDefaultLang(EN);
}

static bool GL_OnClick_Event(T_GL_HMESSAGE hMessage)
{
	char* buffer[50];
	int number = -1;
	T_GL_HWIDGET edit = GL_Widget_SearchId(GL_Message_GetWindow(hMessage), ID_MONTANT);
	T_GL_HWIDGET editConfirmIMG = GL_Widget_SearchId(GL_Message_GetWindow(hMessage), ID_CONFIRM_IMG);
	T_GL_HWIDGET editCancelIMG = GL_Widget_SearchId(GL_Message_GetWindow(hMessage), ID_CANCEL_IMG);

	switch (GL_Message_GetKey(hMessage))
	{
		case GL_KEY_0:
			if (countPileElements(amount) != 0)
				number = 0;
			break;
		case GL_KEY_1:
			number = 1;
			break;
		case GL_KEY_2:
			number = 2;
			break;
		case GL_KEY_3:
			number = 3;
			break;
		case GL_KEY_4:
			number = 4;
			break;
		case GL_KEY_5:
			number = 5;
			break;
		case GL_KEY_6:
			number = 6;
			break;
		case GL_KEY_7:
			number = 7;
			break;
		case GL_KEY_8:
			number = 8;
			break;
		case GL_KEY_9:
			number = 9;
			break;
		case GL_KEY_CORRECTION:
			if(countPileElements(amount) == 0){
				GL_Widget_SetVisible(editCancelIMG, false);
				GL_Widget_SetVisible(editConfirmIMG, false);
			}

			depiler(amount);
			break;
		case GL_KEY_CANCEL:
			GL_Message_SetResult(hMessage, GL_KEY_CANCEL);
			break;
		case GL_KEY_VALID:
			GL_Message_SetResult(hMessage, GL_KEY_VALID);
			break;

		default:
			break;
	}

	if(number >= 0 && countPileElements(amount) < 10) {
		empiler(amount, number);
		GL_Widget_SetVisible(editCancelIMG, true);
		GL_Widget_SetVisible(editConfirmIMG, true);
	}

	GL_Widget_SetText(edit, getAmountStr(amount));


	// Force the value to true when releasing the application from the main loop
	return true;
}


static bool GL_OnClick_Event_Annul(T_GL_HMESSAGE hMessage)
{

	switch (GL_Message_GetKey(hMessage))
	{
		case GL_KEY_CANCEL:
			GL_Message_SetResult(hMessage, GL_Message_GetKey(hMessage));
			break;
		case GL_KEY_VALID:
			GL_Message_SetResult(hMessage, GL_Message_GetKey(hMessage));
			break;

		default:
			break;
	}


	// Force the value to true when releasing the application from the main loop
	return true;
}


// ///////////////////////////////// info card //////////////////////////////////
static bool GL_OnClick_Event_InputReader(T_GL_HMESSAGE hMessage)
{
	char* buffer[50];
	int number = -1;
	T_GL_WCHAR ulKey;



	T_GL_HWIDGET edit = GL_Widget_SearchId(GL_Message_GetWindow(hMessage), ID_MONTANT);

	ulKey = GL_Message_GetKey(hMessage);
	switch (ulKey)
	{
		case GL_KEY_0:
			number = 0;
			break;
		case GL_KEY_1:
			number = 1;
			break;
		case GL_KEY_2:
			number = 2;
			break;
		case GL_KEY_3:
			number = 3;
			break;
		case GL_KEY_4:
			number = 4;
			break;
		case GL_KEY_5:
			number = 5;
			break;
		case GL_KEY_6:
			number = 6;
			break;
		case GL_KEY_7:
			number = 7;
			break;
		case GL_KEY_8:
			number = 8;
			break;
		case GL_KEY_9:
			number = 9;
			break;
		case GL_KEY_CORRECTION:
			depiler(reader_buffer);
			if(courant_reader_buffer>0)
				courant_reader_buffer --;
			break;
		case GL_KEY_CANCEL:
			GL_Message_SetResult(hMessage, GL_Message_GetKey(hMessage));
			courant_reader_buffer = 0;
			break;
		case GL_KEY_VALID:
			GL_Message_SetResult(hMessage, GL_Message_GetKey(hMessage));
			break;

		default:
			break;
	}

	if((ulKey != GL_KEY_CORRECTION) && (ulKey != GL_KEY_CANCEL) && (ulKey != GL_KEY_VALID))
		if(courant_reader_buffer<max_reader_buffer+1)
			courant_reader_buffer ++;


	if((number >= 0) && (courant_reader_buffer<max_reader_buffer+1))
		empiler(reader_buffer, number);

	if(courant_reader_buffer == max_reader_buffer+1)
		courant_reader_buffer --;
	GL_Widget_SetText(edit, getReaderBuffer(reader_buffer));


	// Force the value to true when releasing the application from the main loop
	return true;
}


static bool GL_OnClick_MessageWithWaitForAction(T_GL_HMESSAGE hMessage)
{

	if (GL_Message_GetKey(hMessage) == GL_KEY_VALID)
	{
		// Here we force the value to 0 to release the application of the loop on GL_Window_Dispatch,
		// this assignment is an example and may be replaced by a more complete treatment
		GL_Message_SetResult(hMessage, GL_Message_GetKey(hMessage));
	}
	// Force the value to true when releasing the application from the main loop
	return true;
}

static bool GL_OnTimeOut_MessageWithWaitForAction(T_GL_HMESSAGE hMessage)
{
	// Here we force the value to 0 to release the application of the loop on GL_Window_Dispatch,
	// this assignment is an example and may be replaced by a more complete treatment
	GL_Message_SetResult(hMessage, 0);

	// Force the value to true when releasing the application from the main loop
	return true;
}

static bool OnTimeout_AmountEntry(T_GL_HMESSAGE hMessage){

	// Here we force the value to 0 to release the application of the loop on GL_Window_Dispatch,
	// this assignment is an example and may be replaced by a more complete treatment
	GL_Message_SetResult(hMessage, 0);

	// Force the value to true when releasing the application from the main loop
	return true;
}



int putAmount(char *pc_l_DisplayAmt,unsigned long *entered_amount){

 	byte ucProduct;
	byte ucRange;
	// Return the product name
	ucRange = PSQ_Get_product_type(&ucProduct);
	char init_cmontantDH[8];
	// initialise montant



	amount = initialiser();

//	Os__xprintd(FreeSpace());

	T_GL_HWIDGET window = GL_Window_Create(APEMV_UI_GoalHandle());

	// Creates main layout
	T_GL_HWIDGET mainLayout = GL_Layout_Create(window);
	GL_Widget_SetBackColor(window, 0xFFFFFFFF);
	GL_Widget_SetItem(mainLayout, 0, 0);
	DisplayHeader (_ON_);


	// Montant titre

	T_GL_HWIDGET title = GL_Label_Create(mainLayout);
	GL_Widget_SetItem(title, 0, 0);
	//GL_Widget_SetText(title, "MONTANT");  pc_l_DisplayAmt
	GL_Widget_SetText(title, pc_l_DisplayAmt);
	if (ucRange == TYPE_TERMINAL_M3500 || ucRange == TYPE_TERMINAL_M5000)
		GL_Widget_SetTextPaddings(title, 0, 75, 0, 0, GL_UNIT_PIXEL); //75
	else if(ucRange == TYPE_TERMINAL_D3500)
		GL_Widget_SetTextPaddings(title, 0, 30, 0, 0, GL_UNIT_PIXEL);
	GL_Widget_SetForeColor(title, GL_COLOR_BLACK);
	if (ucRange == TYPE_TERMINAL_M3500 || ucRange == TYPE_TERMINAL_M5000)
		GL_Widget_SetFontSize(title, 20, 20, GL_UNIT_PIXEL);
	GL_Widget_SetExpand(title, GL_DIRECTION_WIDTH);
	GL_Widget_SetGrow(title, GL_DIRECTION_WIDTH);

	// Montant
	sprintf(init_cmontantDH,"0,00 MAD");

	if((*entered_amount > 0) && (*entered_amount < 10))
	{
		empiler(amount, (int)*entered_amount);
		init_cmontantDH[3]= (char)*entered_amount + 0x30;
	}

	T_GL_HWIDGET montant = GL_Label_Create(mainLayout);
	GL_Widget_SetItem(montant, 0, 1);
	GL_Widget_SetText(montant, init_cmontantDH);	//GL_Widget_SetText(montant, "0,00 MAD");
	if (ucRange == TYPE_TERMINAL_M3500 || ucRange == TYPE_TERMINAL_M5000){
		GL_Widget_SetMargins(montant, 0, 20, 0, 0, GL_UNIT_PIXEL); //20
		GL_Widget_SetFontSize(montant, 20, 20, GL_UNIT_PIXEL);
	}else if(ucRange == TYPE_TERMINAL_D3500)
		GL_Widget_SetMargins(montant, 0, 40, 0, 0, GL_UNIT_PIXEL);
	GL_Widget_SetId(montant, ID_MONTANT);


	GL_Widget_SetForeColor(montant, GL_COLOR_BLACK);
	GL_Widget_SetExpand(montant, GL_DIRECTION_WIDTH);
	GL_Widget_SetGrow(montant, GL_DIRECTION_WIDTH);
	GL_Widget_SetGrow(montant, GL_DIRECTION_WIDTH);

	/////////////////////////////////

	T_GL_HWIDGET layoutForPictures = GL_Layout_Create(mainLayout);
	GL_Widget_SetBackColor(window, 0xFFFFFFFF);
	GL_Widget_SetItem(layoutForPictures, 0, 2);
	GL_Widget_SetForeAlign(layoutForPictures, GL_ALIGN_BOTTOM);

	T_GL_HWIDGET leftPictureLayout = GL_Layout_Create(layoutForPictures);
	GL_Widget_SetBackColor(leftPictureLayout, 0xFFFFFFFF);
	GL_Widget_SetItem(leftPictureLayout, 0, 0);
	GL_Widget_SetForeAlign(leftPictureLayout, GL_ALIGN_LEFT);
	GL_Widget_SetGrow(leftPictureLayout, GL_DIRECTION_WIDTH);
	GL_Widget_SetExpand(leftPictureLayout, GL_DIRECTION_WIDTH);

	T_GL_HWIDGET rightPictureLayout = GL_Layout_Create(layoutForPictures);
	GL_Widget_SetBackColor(rightPictureLayout, 0xFFFFFFFF);
	GL_Widget_SetItem(rightPictureLayout, 1, 0);
	GL_Widget_SetForeAlign(rightPictureLayout, GL_ALIGN_RIGHT);
	GL_Widget_SetGrow(rightPictureLayout, GL_DIRECTION_WIDTH);
	GL_Widget_SetExpand(rightPictureLayout, GL_DIRECTION_WIDTH);

	// Confirm
	T_GL_HWIDGET confirm_widget = GL_Picture_Create(rightPictureLayout);
	GL_Widget_SetItem(confirm_widget, 0, 0);
	GL_Widget_SetSource(confirm_widget, "file://flash/HOST/naps-tpe-icons3.png");
	GL_Widget_SetVisible(confirm_widget, false);
	GL_Widget_SetMaxSize(confirm_widget, 50, 50, GL_UNIT_PIXEL);
	GL_Widget_SetTransformation(confirm_widget, GL_TRANSFORMATION_FIT_ALL);
	GL_Widget_SetId(confirm_widget, ID_CONFIRM_IMG);

	// Cancel
	T_GL_HWIDGET cancel_widget = GL_Picture_Create(leftPictureLayout);
	GL_Widget_SetItem(cancel_widget, 0, 0);
	GL_Widget_SetVisible(cancel_widget, false);
	GL_Widget_SetSource(cancel_widget, "file://flash/HOST/naps-tpe-icons-4.png");
	GL_Widget_SetMaxSize(cancel_widget, 50, 50, GL_UNIT_PIXEL);
	GL_Widget_SetTransformation(cancel_widget, GL_TRANSFORMATION_FIT_ALL);
	GL_Widget_SetId(cancel_widget, ID_CANCEL_IMG);

	 // Creating a timer
	T_GL_HWIDGET timer = GL_Timer_Create(window);
	// Set interval to 1s, timer is automatically started
	GL_Timer_SetInterval(timer, 60000);
	// Registering a callback. This callback will be called every second
	GL_Widget_RegisterCallback(timer, GL_EVENT_TIMER_OUT, OnTimeout_AmountEntry);



	GL_Widget_RegisterCallback(window, GL_EVENT_KEY_DOWN, GL_OnClick_Event);

	ulong res = GL_Window_MainLoop(window);

	if(amount != NULL)
		free(amount);
	GL_Widget_Destroy(window);

	if(res == GL_KEY_VALID) return 1;
	else return 0;

}

int customInputReader(UC id, char *info, int maxLen, Boolean hidden, char *output){



	reader_buffer = initialiser();
	max_reader_buffer = maxLen;
	courant_reader_buffer = 0;



	T_GL_HWIDGET window = GL_Window_Create(APEMV_UI_GoalHandle());
	T_GL_DURATION duration;
	// Creates main layout
	T_GL_HWIDGET mainLayout = GL_Layout_Create(window);
	GL_Widget_SetBackColor(window, 0xFFFFFFFF);
	GL_Widget_SetItem(mainLayout, 0, 0);
	DisplayHeader (_ON_);

	int merchLang = My_PSQ_Give_Language();



	T_GL_HWIDGET title = GL_Label_Create(mainLayout);
	GL_Widget_SetItem(title, 0, 0);
	GL_Widget_SetText(title, info);
	GL_Widget_SetMargins(title, 0, 0, 0, 10, GL_UNIT_PIXEL);
	GL_Widget_SetForeColor(title, GL_COLOR_BLACK);
	GL_Widget_SetExpand(title, GL_DIRECTION_WIDTH);
	GL_Widget_SetGrow(title, GL_DIRECTION_WIDTH);

	T_GL_HWIDGET msg = GL_Label_Create(mainLayout);
	GL_Widget_SetItem(msg, 0, 1);
	GL_Widget_SetText(msg, (char *) (ct_DefMess_Cmi[id][merchLang]));
	GL_Widget_SetFontSize(msg, 15, 15, GL_UNIT_PIXEL);
	GL_Widget_SetMargins(msg, 0, 0, 0, 10, GL_UNIT_PIXEL);
	GL_Widget_SetForeColor(msg, GL_COLOR_BLACK);
	GL_Widget_SetExpand(msg, GL_DIRECTION_WIDTH);
	GL_Widget_SetGrow(msg, GL_DIRECTION_WIDTH);

    // Card num

	T_GL_HWIDGET input = GL_Label_Create(mainLayout);
	GL_Widget_SetItem(input, 0, 3);
	GL_Widget_SetId(input, ID_MONTANT);
	GL_Widget_SetForeColor(input, GL_COLOR_BLACK);
	GL_Widget_SetExpand(input, GL_DIRECTION_WIDTH);
	GL_Widget_SetGrow(input, GL_DIRECTION_WIDTH);
	GL_Widget_SetGrow(input, GL_DIRECTION_WIDTH);

	 // Creating a timer belhadeg
	T_GL_HWIDGET timer = GL_Timer_Create(window);
	// Set interval to 1s, timer is automatically started
	GL_Timer_SetInterval(timer, 30000);
	// Registering a callback. This callback will be called every second
	GL_Widget_RegisterCallback(timer, GL_EVENT_TIMER_OUT, OnTimeout_AmountEntry);

    GL_Widget_RegisterCallback(window, GL_EVENT_KEY_DOWN, GL_OnClick_Event_InputReader);






    ulong res = GL_Window_MainLoop(window);
    GL_Widget_Destroy(window);

    if (res == GL_KEY_VALID){


    	char buff[50];

    	strcpy(buff, getReaderBuffer(reader_buffer));
    	memcpy(output, buff, strlen(buff));

    	free(reader_buffer);
    	courant_reader_buffer = 0;

    	return OK;
	}else if(res == GL_KEY_CANCEL){

		free(reader_buffer);
		courant_reader_buffer = 0;

		return KO;
	}

    free(reader_buffer);
    courant_reader_buffer = 0;

    return KO;
}

int customInputReader_(UC id, char *info, int maxLen, Boolean hidden, char *output){



	reader_buffer = initialiser();
	max_reader_buffer = maxLen;
	courant_reader_buffer = 0;



	T_GL_HWIDGET window = GL_Window_Create(APEMV_UI_GoalHandle());
	T_GL_DURATION duration;
	// Creates main layout
	T_GL_HWIDGET mainLayout = GL_Layout_Create(window);
	GL_Widget_SetBackColor(window, 0xFFFFFFFF);
	GL_Widget_SetItem(mainLayout, 0, 0);
	DisplayHeader (_ON_);

	int merchLang = My_PSQ_Give_Language();



	T_GL_HWIDGET title = GL_Label_Create(mainLayout);
	GL_Widget_SetItem(title, 0, 0);
	GL_Widget_SetText(title, info);
	GL_Widget_SetMargins(title, 0, 0, 0, 0, GL_UNIT_PIXEL);
	GL_Widget_SetForeColor(title, GL_COLOR_BLACK);
	GL_Widget_SetFontSize(title, 20, 20, 20);
	//GL_Widget_SetMargins(title, 10, 10, 0, 0, GL_UNIT_PIXEL);
	GL_Widget_SetExpand(title, GL_DIRECTION_WIDTH);
	GL_Widget_SetGrow(title, GL_DIRECTION_WIDTH);

	T_GL_HWIDGET msg = GL_Label_Create(mainLayout);
	GL_Widget_SetItem(msg, 0, 1);
	GL_Widget_SetText(msg, (char *) (ct_DefMess_Cmi[id][merchLang]));
	GL_Widget_SetFontSize(title, 20, 20, 20);
	GL_Widget_SetMargins(msg, 0, 0, 0, 0, GL_UNIT_PIXEL);
	GL_Widget_SetForeColor(msg, GL_COLOR_BLACK);
	GL_Widget_SetExpand(msg, GL_DIRECTION_WIDTH);
	GL_Widget_SetGrow(msg, GL_DIRECTION_WIDTH);

    // Card num

	T_GL_HWIDGET input = GL_Label_Create(mainLayout);
	GL_Widget_SetItem(input, 0, 3);
	GL_Widget_SetId(input, ID_MONTANT);
	GL_Widget_SetFontSize(title, 20, 20, 20);
	GL_Widget_SetForeColor(input, GL_COLOR_BLACK);
	GL_Widget_SetExpand(input, GL_DIRECTION_WIDTH);
	GL_Widget_SetGrow(input, GL_DIRECTION_WIDTH);
	GL_Widget_SetGrow(input, GL_DIRECTION_WIDTH);

	 // Creating a timer belhadeg
	T_GL_HWIDGET timer = GL_Timer_Create(window);
	// Set interval to 1s, timer is automatically started
	GL_Timer_SetInterval(timer, 30000);
	// Registering a callback. This callback will be called every second
	GL_Widget_RegisterCallback(timer, GL_EVENT_TIMER_OUT, OnTimeout_AmountEntry);

    GL_Widget_RegisterCallback(window, GL_EVENT_KEY_DOWN, GL_OnClick_Event_InputReader);






    ulong res = GL_Window_MainLoop(window);
    GL_Widget_Destroy(window);

    if (res == GL_KEY_VALID){


    	char buff[50];

    	strcpy(buff, getReaderBuffer(reader_buffer));
    	memcpy(output, buff, strlen(buff));

    	free(reader_buffer);
    	courant_reader_buffer = 0;

    	return OK;
	}else if(res == GL_KEY_CANCEL){

		free(reader_buffer);
		courant_reader_buffer = 0;

		return KO;
	}

    free(reader_buffer);
    courant_reader_buffer = 0;

    return KO;
}

int MessageWithWaitForAction(char* title, char* image, int delay){

//	delay = delay/1000;
//	if(image == NULL)
//		GL_Dialog_Message(APEMV_UI_GoalHandle(), NULL, title, GL_ICON_NONE, GL_BUTTON_NONE, delay);
//	else
//		GL_Dialog_Picture(APEMV_UI_GoalHandle(), title, NULL, image, GL_BUTTON_NONE, delay);



	T_GL_HWIDGET window = GL_Window_Create(APEMV_UI_GoalHandle());

	// Creates main layout
	T_GL_HWIDGET mainLayout = GL_Layout_Create(window);
	GL_Widget_SetBackColor(window, 0xFFFFFFFF);
	GL_Widget_SetItem(mainLayout, 0, 0);

	// afficher le header par default
	DisplayHeader (_ON_);


	// Title
	T_GL_HWIDGET title_widget = GL_Label_Create(mainLayout);
	GL_Widget_SetItem(title_widget, 0, 0);
	GL_Widget_SetText(title_widget, title);
	GL_Widget_SetForeColor(title_widget, GL_COLOR_BLACK);
	GL_Widget_SetExpand(title_widget, GL_DIRECTION_WIDTH);
	GL_Widget_SetGrow(title_widget, GL_DIRECTION_WIDTH);
	GL_Widget_SetFontSize(title_widget, 18, 18, GL_UNIT_PIXEL);
	GL_Widget_SetMargins(title_widget, 0, 10, 0, 5, GL_UNIT_PIXEL);

	T_GL_HWIDGET layoutForPicture = GL_Layout_Create(mainLayout);
	GL_Widget_SetBackColor(window, 0xFFFFFFFF);
	GL_Widget_SetItem(layoutForPicture, 0, 1);


	// Image
	T_GL_HWIDGET image_widget = GL_Picture_Create(layoutForPicture);
	GL_Widget_SetItem(image_widget, 0, 0);
	GL_Widget_SetSource(image_widget, image);
	GL_Widget_SetMaxSize(image_widget, 100, 100, GL_UNIT_PIXEL);
	GL_Widget_SetTransformation(image_widget, GL_TRANSFORMATION_FIT_ALL);


	GL_Widget_RegisterCallback(window, GL_EVENT_KEY_DOWN, GL_OnClick_MessageWithWaitForAction);

	T_GL_HWIDGET timer = GL_Timer_Create(window);

	// Set interval , timer is automatically started
	GL_Timer_SetInterval(timer, delay);

	// Registering a callback. This callback will be called every second
	GL_Widget_RegisterCallback(timer, GL_EVENT_TIMER_OUT, GL_OnTimeOut_MessageWithWaitForAction);

	GL_Window_MainLoop(window);

	GL_Widget_Destroy(window);
	return 0;

}

int MessageWithoutConfirmation(char* title, char* image){

//	Os__display(1 ,0 , title);
//	if(image == NULL)
//		GL_Dialog_Message(APEMV_UI_GoalHandle(), NULL, title, GL_ICON_NONE, GL_BUTTON_NONE, 0);
//	else
//		GL_Dialog_Picture(APEMV_UI_GoalHandle(), title, NULL, image, GL_BUTTON_NONE, 0);


	T_GL_HWIDGET window = GL_Window_Create(APEMV_UI_GoalHandle());
 	byte ucProduct;
	byte ucRange;
	// Return the product name
	ucRange = PSQ_Get_product_type(&ucProduct);

	// Creates main layout
	T_GL_HWIDGET mainLayout = GL_Layout_Create(window);
	GL_Widget_SetBackColor(window, 0xFFFFFFFF);
	GL_Widget_SetItem(mainLayout, 0, 0);

	// afficher le header par default
	DisplayHeader (_ON_);


    // Title
	T_GL_HWIDGET title_widget = GL_Label_Create(mainLayout);
	GL_Widget_SetItem(title_widget, 0, 0);
	GL_Widget_SetText(title_widget, title);
	GL_Widget_SetForeColor(title_widget, GL_COLOR_BLACK);
	GL_Widget_SetExpand(title_widget, GL_DIRECTION_WIDTH);
	GL_Widget_SetGrow(title_widget, GL_DIRECTION_WIDTH);
	GL_Widget_SetFontSize(title_widget, 18, 18, GL_UNIT_PIXEL);
	if (ucRange == TYPE_TERMINAL_M3500 || ucRange == TYPE_TERMINAL_M5000)
		GL_Widget_SetTextPaddings(title_widget, 0, 40, 0, 5, GL_UNIT_PIXEL); //40
	else if(ucRange == TYPE_TERMINAL_D3500)
		GL_Widget_SetTextPaddings(title_widget, 0, 10, 0, 5, GL_UNIT_PIXEL);


	if (image != NULL){

		T_GL_HWIDGET layoutForPicture = GL_Layout_Create(mainLayout);
		GL_Widget_SetBackColor(window, 0xFFFFFFFF);
		GL_Widget_SetItem(layoutForPicture, 0, 1);

		// Image
		T_GL_HWIDGET image_widget = GL_Picture_Create(layoutForPicture);
		GL_Widget_SetItem(image_widget, 0, 0);
		GL_Widget_SetSource(image_widget, image);
		GL_Widget_SetMaxSize(image_widget, 100, 100, GL_UNIT_PIXEL);
		GL_Widget_SetTransformation(image_widget, GL_TRANSFORMATION_FIT_ALL);

	}


    GL_Window_Dispatch(window, 0);

    GL_Widget_Destroy(window);
	return 0;
}

static bool GL_OnTimeOut_MessageWithDelay(T_GL_HMESSAGE hMessage)
{
	// Here we force the value to 0 to release the application of the loop on GL_Window_Dispatch,
	// this assignment is an example and may be replaced by a more complete treatment
	GL_Message_SetResult(hMessage, 0);

	// Force the value to true when releasing the application from the main loop
	return true;
}

int MessageWithDelay(char* title, char* image, int delay){

//	APEMV_UI_MessageDisplayDelay(title, delay/1000);
//	delay = delay/1000;
//	if(image == NULL)
//		GL_Dialog_Message(APEMV_UI_GoalHandle(), NULL, title, GL_ICON_NONE, GL_BUTTON_NONE, delay);
//	else
//		GL_Dialog_Picture(APEMV_UI_GoalHandle(), title, NULL, image, GL_BUTTON_NONE, delay);

	T_GL_HWIDGET window = GL_Window_Create(APEMV_UI_GoalHandle());

	// Creates main layout
	T_GL_HWIDGET mainLayout = GL_Layout_Create(window);
	GL_Widget_SetBackColor(window, 0xFFFFFFFF);
	GL_Widget_SetItem(mainLayout, 0, 0);

	// afficher le header par default
	DisplayHeader (_ON_);


    // Title
	T_GL_HWIDGET title_widget = GL_Label_Create(mainLayout);
	GL_Widget_SetItem(title_widget, 0, 0);
	GL_Widget_SetText(title_widget, title);
	GL_Widget_SetForeColor(title_widget, GL_COLOR_BLACK);
	GL_Widget_SetExpand(title_widget, GL_DIRECTION_WIDTH);
	GL_Widget_SetGrow(title_widget, GL_DIRECTION_WIDTH);
	GL_Widget_SetFontSize(title_widget, 18, 18, GL_UNIT_PIXEL);
	GL_Widget_SetMargins(title_widget, 0, 10, 0, 5, GL_UNIT_PIXEL);

	if (image != NULL){

		T_GL_HWIDGET layoutForPicture = GL_Layout_Create(mainLayout);
		GL_Widget_SetBackColor(window, 0xFFFFFFFF);
		GL_Widget_SetItem(layoutForPicture, 0, 1);

		// Image
		T_GL_HWIDGET image_widget = GL_Picture_Create(layoutForPicture);
		GL_Widget_SetItem(image_widget, 0, 0);
		GL_Widget_SetSource(image_widget, image);
		GL_Widget_SetMaxSize(image_widget, 100, 100, GL_UNIT_PIXEL);
		GL_Widget_SetTransformation(image_widget, GL_TRANSFORMATION_FIT_ALL);

	}

	T_GL_HWIDGET timer = GL_Timer_Create(window);

	// Set interval , timer is automatically started
	GL_Timer_SetInterval(timer, delay);

	// Registering a callback. This callback will be called every second
	GL_Widget_RegisterCallback(timer, GL_EVENT_TIMER_OUT, GL_OnTimeOut_MessageWithDelay);


    GL_Window_MainLoop(window);

    GL_Widget_Destroy(window);
	return 0;

}

int MessageWithConfirmation(char* title){

	T_GL_HWIDGET window = GL_Window_Create(APEMV_UI_GoalHandle());

	byte ucProduct;
	byte ucRange;
	// Return the product name
	ucRange = PSQ_Get_product_type(&ucProduct);

	// Creates main layout
	T_GL_HWIDGET mainLayout = GL_Layout_Create(window);
	GL_Widget_SetBackColor(window, 0xFFFFFFFF);
	GL_Widget_SetItem(mainLayout, 0, 0);

	// afficher le header par default
	DisplayHeader (_ON_);


    // Title
	T_GL_HWIDGET title_widget = GL_Label_Create(mainLayout);
	GL_Widget_SetItem(title_widget, 0, 0);
	GL_Widget_SetText(title_widget, title);
	GL_Widget_SetForeColor(title_widget, GL_COLOR_BLACK);
	GL_Widget_SetExpand(title_widget, GL_DIRECTION_WIDTH);
	GL_Widget_SetGrow(title_widget, GL_DIRECTION_WIDTH);
	GL_Widget_SetFontSize(title_widget, 18, 18, GL_UNIT_PIXEL);
	if (ucRange == TYPE_TERMINAL_M3500 || ucRange == TYPE_TERMINAL_M5000)
		GL_Widget_SetMargins(title_widget, 0, 70, 0, 15, GL_UNIT_PIXEL);
	else if(ucRange == TYPE_TERMINAL_D3500)
		GL_Widget_SetMargins(title_widget, 0, 50, 0, 15, GL_UNIT_PIXEL);



	T_GL_HWIDGET layoutForPictures = GL_Layout_Create(mainLayout);
	GL_Widget_SetBackColor(window, 0xFFFFFFFF);
	GL_Widget_SetItem(layoutForPictures, 0, 1);
	GL_Widget_SetForeAlign(layoutForPictures, GL_ALIGN_BOTTOM);

	T_GL_HWIDGET leftPictureLayout = GL_Layout_Create(layoutForPictures);
	GL_Widget_SetBackColor(leftPictureLayout, 0xFFFFFFFF);
	GL_Widget_SetItem(leftPictureLayout, 0, 0);
	GL_Widget_SetForeAlign(leftPictureLayout, GL_ALIGN_LEFT);
	GL_Widget_SetGrow(leftPictureLayout, GL_DIRECTION_WIDTH);
	GL_Widget_SetExpand(leftPictureLayout, GL_DIRECTION_WIDTH);

	T_GL_HWIDGET rightPictureLayout = GL_Layout_Create(layoutForPictures);
	GL_Widget_SetBackColor(rightPictureLayout, 0xFFFFFFFF);
	GL_Widget_SetItem(rightPictureLayout, 1, 0);
	GL_Widget_SetForeAlign(rightPictureLayout, GL_ALIGN_RIGHT);
	GL_Widget_SetGrow(rightPictureLayout, GL_DIRECTION_WIDTH);
	GL_Widget_SetExpand(rightPictureLayout, GL_DIRECTION_WIDTH);




    // Confirm
	T_GL_HWIDGET confirm_widget = GL_Picture_Create(rightPictureLayout);
	GL_Widget_SetItem(confirm_widget, 0, 0);
	GL_Widget_SetSource(confirm_widget, "file://flash/HOST/naps-tpe-icons3.png");
	GL_Widget_SetMaxSize(confirm_widget, 50, 50, GL_UNIT_PIXEL);
	GL_Widget_SetTransformation(confirm_widget, GL_TRANSFORMATION_FIT_ALL);

	// Cancel
	T_GL_HWIDGET cancel_widget = GL_Picture_Create(leftPictureLayout);
	GL_Widget_SetItem(cancel_widget, 0, 0);
	GL_Widget_SetSource(cancel_widget, "file://flash/HOST/naps-tpe-icons-4.png");
	GL_Widget_SetMaxSize(cancel_widget, 50, 50, GL_UNIT_PIXEL);
	GL_Widget_SetTransformation(cancel_widget, GL_TRANSFORMATION_FIT_ALL);


    GL_Window_Dispatch(window, 0);

	GL_Widget_Destroy(window);

	return 0;
}

void detectCard(void){
//
//	Telium_File_t *handleCam0;
//	unsigned char cam0_status;
//
//	handleCam0 = Telium_Fopen("CAM0", "r");

//	Telium_Status(handleCam0, &cam0_status);
//	if((cam0_status & CAM_PRESENT) == 0){
//		Os__xprinta("CAM_PRESENT");
//	}
//	if(handleCam0 != NULL)
//		Telium_Fclose(handleCam0);
}


//===========================================================================
int CUMESS_GetAmount (unsigned short currency_code,
					  unsigned long *entered_amount)

{
	// Internal data declaration
	unsigned short i;
	unsigned short found;
	int            merchLang;
	//unsigned char  fmtAmt;
	int            retgetamt;
	int            i_l_numMsg;
	char 		   *pc_l_DisplayAmt;
	char			c_l_DisplayAmt[32];
	int 			Ret;
	// Retreive Terminal merchant language
	merchLang = My_PSQ_Give_Language();



	//fct();

	if(tst_fic_plein() == OK){
		MessageWithDelay("TPE Plein\nLancer la telecollecte", NULL, 2000);
		return 0; // NBR MAX des transactions jrizki 30042019
	}


	if (FicPara.status == KO) {
		CUMORE_CMI_AFFICHE_MESSAGE(27, 138, 1000);
		//Mess_delay(27,138,1,100,1);
		return 0 ;
	}

	if(((ulTransactionType == CUSERV_DEBIT_TRANSACTION) || (ulTransactionType == CUSERV_CASH_TRANSACTION)) && (!memcmp(FicPara.TYPEAPPLICATION,"NAPSLYDC",8)))
		{
			Lydec();//Lydec badr
			if (FicPara.Flag_service !=2)
			{
				FicPara.AutoReversalOK = 4;
				PutAllParam();
				return 0;
			}
		}
	// Search currency in currency definition table
	i= 0;
	found = FALSE;
	while ((!found) && (i < L_NBCURCY))
	{
		found = (ct_confCurrency[i].currency == currency_code);
		if (!found)
		{
			i++;
		}
	}

	/*if (ct_confCurrency[i].unit == 0)
	{
		if (merchLang == C_LG_FRENCH)
		{
			fmtAmt = 3;
		}
		else
		{
			fmtAmt = 4;
		}
	}
	else
	{
		if (merchLang == C_LG_FRENCH)
		{
			fmtAmt = 0;
		}
		else
		{
			fmtAmt = 1;
		}
	};*/

	switch (ulTransactionType)
	{
	case CUSERV_REFUND_TRANSACTION :
		i_l_numMsg = DFT_MESS_NB_REFUND_TRANS;
		pc_l_DisplayAmt = CUMESS_Read_Message(i_l_numMsg, merchLang);
		break;

	case CUSERV_CASH_TRANSACTION :
		i_l_numMsg = DFT_MESS_NB_CASH_TRANS;
		pc_l_DisplayAmt = CUMESS_Read_Message(i_l_numMsg, merchLang);
		break;

	case CUSERV_CASHBACK_TRANSACTION :
		i_l_numMsg = DFT_MESS_NB_CASHBACK_TRANS;
		pc_l_DisplayAmt = CUMESS_Read_Message(i_l_numMsg, merchLang);
		break;

	case CUSERV_PREAUT_TRANSACTION :
		sprintf(c_l_DisplayAmt,"%s","PRE AUTORISATION");
		pc_l_DisplayAmt = c_l_DisplayAmt;
		break;

	case CUSERV_COMPL_TRANSACTION :
		sprintf(c_l_DisplayAmt,"%s","COMPLEMENT PRE AUTO");
		pc_l_DisplayAmt = c_l_DisplayAmt;
		break;

	case CUSERV_PAIEMENT_NFOIS_TRANSACTION :
		sprintf(c_l_DisplayAmt,"%s","PAIEMENT N FOIS");
		pc_l_DisplayAmt = c_l_DisplayAmt;
		break;

	case CUSERV_PAIEMENT_DIFFERE_TRANSACTION :
		sprintf(c_l_DisplayAmt,"%s","PAIEMENT DIFFERE");
		pc_l_DisplayAmt = c_l_DisplayAmt;
		break;

	case CUSERV_PAIEMENT_POURBOIRE_TRANSACTION :
		sprintf(c_l_DisplayAmt,"%s","PAIEMENT POURBOIRE");
		if(StructInfoPayment.AfficheIndication_POURBOIRE_INTERCHANGE == 1)
			sprintf(c_l_DisplayAmt,"%s","MONTANT POURBOIRE");
		pc_l_DisplayAmt = c_l_DisplayAmt;
		break;

	case CUSERV_PAIEMENT_INTERCHANGE_TRANSACTION :
		sprintf(c_l_DisplayAmt,"%s","PAIEMENT INTERCHANGE");
		if(StructInfoPayment.AfficheIndication_POURBOIRE_INTERCHANGE == 2)
			sprintf(c_l_DisplayAmt,"%s","MONTANT SURCOUT");
		pc_l_DisplayAmt = c_l_DisplayAmt;
		break;

	default :

		i_l_numMsg = STD_MESS_AMOUNT;
		pc_l_DisplayAmt = CUMESS_Read_Message(i_l_numMsg, merchLang);
		break;
	}

	FicPara.AutoReversalOK = 4;
	PutAllParam();

	retgetamt = putAmount(pc_l_DisplayAmt,entered_amount);

	if (retgetamt == 1)
		*entered_amount = ul_InputAmount;
	else
		*entered_amount = 0;

	if(((ulTransactionType == CUSERV_DEBIT_TRANSACTION) || (ulTransactionType == CUSERV_CASH_TRANSACTION)) && (!memcmp(FicPara.TYPEAPPLICATION,"NAPSAVIS",8)))
	{
		Ret = Avis();//avis badr
	if (!Ret)
			{
				FicPara.AutoReversalOK = 4;
				PutAllParam();
				return 0;
			}
	}

	return (retgetamt);
}

//===========================================================================
void CUMESS_ConvertBinNUM (unsigned long BinaryValue, unsigned char *NumValue)
{
	// Internal data declaration
	unsigned long Bin;
	unsigned long rest;
	unsigned long i;

	Bin = BinaryValue;

	for (i = 0; i < 6; i++)
	{
		// first nibble
		rest = Bin - ((Bin / 10) * 10);
		NumValue [5 - i] = (unsigned char) rest;
		Bin = (Bin / 10);

		// Second nibble
		rest = Bin - ((Bin / 10) * 10);
		NumValue [5 - i] += (unsigned char) (rest << 4);
		Bin = (Bin / 10);
	}
}

#ifdef OUTPUT_MSG_ON_COM
void CUMESS_SendMsg(char * Msg)
{
    unsigned short sender;
    unsigned short receiver;
    unsigned short type;
	static   S_MESSAGE_IAM message;

    receiver = (TaskIam1*256) + MANAGER_TYPE; /* To TaskIam1           */
                                              /* Application Type M2OS */

    sender   = (TaskApplication*256) + 0x00;  /* 03 ==> M2OS task      */
                                              /* Application Type all  */
    type = 0x0;                               /* For Exemple           */

    /* envoie du message */
    message.receiver = receiver;
    message.sender = sender;
    message.type = type;
    message.length = strlen(Msg);
    memcpy(message.value,(unsigned char *)Msg,message.length);
    Send_Message( &message );

    Telium_Ttestall(0,200);
}
#endif



//===========================================================================
char *CUMESS_ReadMessageReceipt(int number, int language)
{
	char * Msgptr;

	// Table number not used
	Msgptr = CUMESS_Read_Message(number, language);

	return(Msgptr);
}


//===========================================================================
void CUMESS_GiveIso639Code(unsigned char * pc_x_IsoCode, unsigned short *ps_x_IdCode)
{
	int j;

	// Extract the language code associated with the ISO code
	for (j = 0; j < C_NBLANGHOLDER; j++)
	{
		if ((pc_x_IsoCode[0] == lang_table [j].iso639_code [0]) &&
			(pc_x_IsoCode[1] == lang_table [j].iso639_code [1]))
		{
			*ps_x_IdCode = (unsigned short) lang_table [j].table_id;
			break;
			// Language is found. Process can be stopped.
		}
	}
}


//===========================================================================
//! \brief This function returns one message from the message table given
//! as parameter.
//! The message is defined by input parameters (num, language).
//! \param[in] nNum : the index of the message in the table.
//! \param[in] nLanguage : message language.
//! \return
//! - The message, if it exists in the table.
//! - no_mess_found, if the message doesn't exist in the table.
//! - NULL if the language_table is NULL.
//===========================================================================

char * CUMESS_Read_Message(int nNum, int nLanguage)
{
	//return ReadMessage_cmi(nNum,nLanguage);//KT
	
	
	int index;
	char ac_l_iso[] = "xx";		// Unused value

	// if language code is not valid: DefDefaultLang(EN) = English will be used
	for(index=0; index<C_NBLANGHOLDER; index++)
	{
		if (lang_table[index].table_id == (unsigned char)nLanguage)
		{
			strcpy(ac_l_iso, (const char*)lang_table[index].iso639_code);
			break;
		}
	}

	DefCurrentLang(ac_l_iso);

	// Test GetMessageInfos function
	GetMessageInfos(nNum,&CurrentMSG);

	return(CurrentMSG.message);
}


//===========================================================================
void CUMESS_Read_MsgInfos(MSGinfos * ptr_MSGtest, int num, int language)
{
	int index;
	char ac_l_iso[] = "xx"; /* Unused value */

	// if language code is not valid: DefDefaultLang(EN) = English will be used
	for(index=0; index<C_NBLANGHOLDER; index++)
	{
		if (lang_table[index].table_id == (unsigned char)language)
		{
			strcpy(ac_l_iso, (const char*)lang_table[index].iso639_code);
			break;
		}
	}

	DefCurrentLang(ac_l_iso);

	// Test GetMessageInfos function
	GetMessageInfos(num, ptr_MSGtest);
}




// ======================== traitement montant

Pile *initialiser()
{
    Pile *pile = malloc(sizeof(*pile));
    pile->premier = NULL;
}

void empiler(Pile *pile, int nvNombre)
{
	IntBuffer *nouveau = malloc(sizeof(*nouveau));
    if (pile != NULL && nouveau != NULL)
    {
		nouveau->nombre = nvNombre;
		nouveau->suivant = pile->premier;
		pile->premier = nouveau;
    }

}

int depiler(Pile *pile)
{
    if (pile == NULL)
    {
    	return -1;
    }

    int nombreDepile = 0;
    IntBuffer *elementDepile = pile->premier;

    if (pile != NULL && pile->premier != NULL)
    {
        nombreDepile = elementDepile->nombre;
        pile->premier = elementDepile->suivant;
        free(elementDepile);
    }

    return nombreDepile;
}

void reverse_string(char *str)
{
    /* skip null */
    if (str == 0)
    {
        return;
    }

    /* skip empty string */
    if (*str == 0)
    {
        return;
    }

    /* get range */
    char *start = str;
    char *end = start + strlen(str) - 1; /* -1 for \0 */
    char temp;

    /* reverse */
    while (end > start)
    {
        /* swap */
        temp = *start;
        *start = *end;
        *end = temp;

        /* move */
        ++start;
        --end;
    }
}


char *getAmountStr(Pile *pile)
{
    static char amountstr[200];
    static char amountForLong[200];
    char buffer[3];


	if (pile == NULL)
    {
        return NULL;
    }

	IntBuffer *actuel = pile->premier;

	strcpy(amountstr, "");
	strcpy(amountForLong, "");

	int i = 0;
    while (actuel != NULL)
    {
    	if(i == 2) {
    		sprintf(buffer, "%s", ",");
    		strcat(amountstr, buffer);
    	}
        sprintf(buffer, "%d", actuel->nombre);
        strcat(amountstr, buffer);
        strcat(amountForLong, buffer);
        actuel = actuel->suivant;
        i++;
    }

    // get the final amount value
    reverse_string(amountForLong);
    ul_InputAmount = atoi(amountForLong);



    if(i == 2){
    	strcat(amountstr, ",0");
    }else if (i == 1){
    	strcat(amountstr, "0,0");
    }else if (i == 0){
    	strcat(amountstr, "00,0");
    }

    reverse_string(amountstr);


    strcat(amountstr, " MAD");

    return amountstr;
}

int countPileElements(Pile *pile){
	int count = 0;

	IntBuffer *actuel = pile->premier;

	while (actuel != NULL){
		actuel = actuel->suivant;
		count++;
	}

	return count;

}

char *getReaderBuffer(Pile *pile)
{
    static char bufferForLong[200];
    char buffer[3];


	if (pile == NULL)
    {
        return NULL;
    }

	IntBuffer *actuel = pile->premier;


	strcpy(bufferForLong, "");

	int i = 0;
    while (actuel != NULL)
    {

        sprintf(buffer, "%d", actuel->nombre);
        strcat(bufferForLong, buffer);
        actuel = actuel->suivant;
        i++;
    }

    // get the final amount value
    reverse_string(bufferForLong);


    return bufferForLong;
}


void traceInt(int nb, int mode){
	char buff[30];
	Telium_File_t *hSmc=NULL;
	byte ucSta;

	sprintf(buff, "%d", nb);

	hSmc = Telium_Stdperif("PRINTER", NULL);
	if (hSmc == NULL)
	{
		hSmc = Telium_Fopen("PRINTER", "rw*");     // Open the main Cam peripheral
	}

	// Display remove card and check if card removed
	// *********************************************
	Telium_Status (hSmc, &ucSta);
	switch (ucSta)
	{
	case PRINTER_GET_PAPER_OUT_STATE:
		APEMV_UI_MessageDisplayDelay(buff, 1);
		break;
	default:
		if (mode == 1) APEMV_UI_MessageDisplayDelay(buff, 1);
		Os__xprinta(buff);
		break;
	}
	Telium_Fclose(hSmc);                        // Close "cam" channel
//	if(mode == 1)
//		APEMV_UI_MessageDisplayDelay(buff, 1);
//	else
//		Os__xprinta(buff);
}

void traceString(char *mess){
	APEMV_UI_MessageDisplayDelay(mess, 1);
	//Os__xprinta(mess);
}
/*-----------------------------------------------------------------
 * Fonction   : Avis
 * objet      : gestion des avis
 *
 * Parametre  :----
 * Retour     :Service OK/KO
 *-----------------------------------------------------------------
*/
int Avis(void)//avis badr
{

//StructInfoPayment.CodeFunction = OR_FACTURE50;
	T_GL_HGRAPHIC_LIB hGoal = APEMV_UI_GoalHandle();
	T_GL_HGRAPHIC_LIB gGoalGraphicLibInstance = APEMV_UI_GoalHandle();
	int iRet,iRet1;
	bool bContinue = TRUE;
	uchar Affich_ligne1[16+1],Affich_ligne2[16+1];

	const char * codesmotif[] =
	{
	" Estimation location",
	" Journée supplémentaire",
	" Carburant",
	" Franchise d assurance",
	" Frais de collecte",
	" Perte Papiers",
	" Autre",
	0 //<- DO NOT FORGET THIS TO STOP THE LIST
	};

	memset(Affich_ligne1,0,sizeof(Affich_ligne1));
	memset(Affich_ligne2,0,sizeof(Affich_ligne2));

	memset(StructInfoPayment.AV_CodeMotif,0,sizeof(StructInfoPayment.AV_CodeMotif));

	sprintf((PC)Affich_ligne1," Confirmez-vous ");
	sprintf((PC)Affich_ligne2,"le service Avis ? ");
	if(ValiderMess(Affich_ligne1,Affich_ligne2)!=KEY_ENTER)
		return KO;


do
{
	memset(StructInfoPayment.AV_NumContrat,0,sizeof(StructInfoPayment.AV_NumContrat));
	iRet = GL_Dialog_Text(gGoalGraphicLibInstance, NULL,
		"NUMERO DE CONTRAT:", NULL, StructInfoPayment.AV_NumContrat, 12, GL_TIME_MINUTE);

	if ((iRet==GL_KEY_CANCEL) || (iRet==GL_RESULT_INACTIVITY))
	{
		FicPara.Flag_service = 0;
		PutAllParam();
		return 0;
	}
}while((strlen(StructInfoPayment.AV_NumContrat) == 0) || (strlen(StructInfoPayment.AV_NumContrat) > 9));


	do {

		switch (iRet1 = GL_Dialog_Choice(hGoal, "Les codes motif", codesmotif, 0,GL_BUTTON_DEFAULT, GL_KEY_1,GL_TIME_MINUTE)) {
		case 0:
			memcpy(StructInfoPayment.AV_CodeMotif,codesmotif[0],strlen(codesmotif[0]));
			goto lblEnd;
		case 1:
			memcpy(StructInfoPayment.AV_CodeMotif,codesmotif[1],strlen(codesmotif[1]));
			goto lblEnd;
		case 2:
			memcpy(StructInfoPayment.AV_CodeMotif,codesmotif[2],strlen(codesmotif[2]));
			goto lblEnd;
		case 3:
			memcpy(StructInfoPayment.AV_CodeMotif,codesmotif[3],strlen(codesmotif[3]));
			goto lblEnd;
		case 4:
			memcpy(StructInfoPayment.AV_CodeMotif,codesmotif[4],strlen(codesmotif[4]));
			goto lblEnd;
		case 5:
			memcpy(StructInfoPayment.AV_CodeMotif,codesmotif[5],strlen(codesmotif[5]));
			goto lblEnd;
		case 6:
			memcpy(StructInfoPayment.AV_CodeMotif,codesmotif[6],strlen(codesmotif[6]));
			goto lblEnd;
		default:
			bContinue = FALSE;
			goto lblEnd;

		}
	} while (bContinue);


	lblEnd:
	if(strlen(StructInfoPayment.AV_CodeMotif) != 0)
	{
		FicPara.Flag_service = 1;
		PutAllParam();
	}
	else{
		FicPara.Flag_service = 0;
		PutAllParam();
		return 0;
	}

	lblKO:
	return KO;

}


/*-----------------------------------------------------------------
 * Fonction   : Lydec
 * objet      : Lydec collecte info
 *
 * Parametre  :----
 * Retour     :----
 *-----------------------------------------------------------------
*/
void Lydec(void)//Lydec badr
{

	T_GL_HGRAPHIC_LIB hGoal = APEMV_UI_GoalHandle();
	T_GL_HGRAPHIC_LIB gGoalGraphicLibInstance = APEMV_UI_GoalHandle();
	int iRet,iRet1;
	bool bContinue = TRUE;


	do
		{
			memset(StructInfoPayment.LY_RefContra,0,sizeof(StructInfoPayment.LY_RefContra));
			iRet = GL_Dialog_Text(gGoalGraphicLibInstance, NULL,
				"Saisir Référence \ndu contrat :", NULL, StructInfoPayment.LY_RefContra, 12, GL_TIME_MINUTE);

			if ((iRet==GL_KEY_CANCEL) || (iRet==GL_RESULT_INACTIVITY))
				{
				FicPara.Flag_service = 0;
				PutAllParam();
				return KO;
				}

			}while((strlen(StructInfoPayment.LY_RefContra) == 0) || (strlen(StructInfoPayment.LY_RefContra) > 10));

	do
		{
			memset(StructInfoPayment.LY_NumGSM,0,sizeof(StructInfoPayment.LY_NumGSM));
			if(customInputReader_(138, "Saisir N° GSM :", 10, TRUE, &StructInfoPayment.LY_NumGSM[0])!=OK)
				break;
		}while((strlen((PC)StructInfoPayment.LY_NumGSM)!=10) && (strlen((PC)StructInfoPayment.LY_NumGSM)!=0));



	memset(StructInfoPayment.LY_AdrEmail,0,sizeof(StructInfoPayment.LY_AdrEmail));
//	iRet = GL_Dialog_Text(gGoalGraphicLibInstance, NULL,
//		"Saisir @ Email :", NULL, StructInfoPayment.LY_AdrEmail, sizeof(StructInfoPayment.LY_AdrEmail), GL_TIME_MINUTE);
	memcpy(StructInfoPayment.LY_AdrEmail,"M2M@GROUP.com",strlen("M2M@GROUP.com"));

	if((strlen(StructInfoPayment.LY_NumGSM) != 0))//||(strlen(StructInfoPayment.LY_AdrEmail) != 0))
		FicPara.Flag_service = 2;
	else
		FicPara.Flag_service = 0;
	PutAllParam();
	lblKO:
	return KO;

}


