/**
* \file cu_str.c
* \brief This module contains the functions needed for magnetic stripe management.
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

#include "sdk.h" // IngeDev automatic refactoring - 2014/04/29 14:29:58 - Replace '#include "SDK30.H"' to '#include "sdk.h"'
#include "_emvdctag_.h"
#include "del_lib.h"
#include "cu_term.h"
#include "MyTlvTree.h"
#include "cu_batch.h"
#include "cu_base.h"
#include "convert.h"
#include "gestion_param.h"
#include "cu_serv.h"
#include "cu_param.h"
#include "cu_mess.h"
#include "MessagesDefinitions.h"
#include "cu_term.h"
#include "cu_entry.h"
#include "convert.h"
#include "cu_mess.h"
#include "cu_pinen.h"
#include "cu_more.h"
#include "cu_logos.h"
#include "TlvTreeDel.h"
#include "EngineInterfaceLib.h"
#include "cu_receipt.h"

#include "GL_GraphicLib.h"
#include "sec_interface.h"
#include "GTL_Assert.h"
#include "GTL_Convert.h"

#include "EPSTOOL_Convert.h"
#include "EPSTOOL_TlvTree.h"
#include "EPSTOOL_PinEntry.h"
#include "EPSTOOL_Unicode.h"
#include "cu_UserInterfaceDisplay.h"

//! \addtogroup Group_cu_str
//! @{

/* ======================================================================== */
/* Macros & preprocessor definitions                                        */
/* ======================================================================== */

/* =========================================================================*/
/* Types declarations                                                       */
/* =========================================================================*/

/* ======================================================================== */
/* Constant declarations                                                    */
/* ======================================================================== */

/* ========================================================================	*/
/* Global variables                                                         */
/* ========================================================================	*/

static unsigned long entered_amount;

/* ======================================================================== */
/* Static function definitions                                              */
/* ======================================================================== */

static int Input_Amount(int bAmount, unsigned long amount, TLV_TREE_NODE *phOutputTLVTree);

/* ======================================================================== */
/* Functions                                                                */
/* ======================================================================== */

//===========================================================================
int CUSTR_Mag_Stripe_Debit (int bValidAmount, unsigned long amount, int isFallbackAllowed,
							char *track2_buffer)
{
	// Internal data declaration
	unsigned char pan[19 + 1];
	unsigned char pan_dcb[10];
	unsigned int i,j;
	int        endPAN;
	char       ExpirationDate [5];
	char       ExpirationDate_dcb [3];
	char       ServiceCode [4];
	unsigned short lg_code;
	int rejection;
	int ICCPresent;
	int resdeb;
	unsigned char chaine [40];
	DataElement elt;
	int record_POSEM;
	unsigned short usFound;
	unsigned short currency_code = 0;
	int            g_language;
	int            unit = 0;
	int            found;

	unsigned char  currency[4];
	unsigned char  buffer[50];
	unsigned char  text[50];
	int previousUiState;
	char  buffer0[50];
	char  buffer1[50];
	char  buffer2[50];	
	TLV_TREE_NODE  hTree;

	CUDB_Get_cardholder_language (&lg_code);
	resdeb = FALSE;
	hTree = NULL;


	// Begin

	// Extract PAN
	i= 0;
	endPAN = FALSE;
	rejection = FALSE;

	memset (pan , 0, 19 + 1);
	memset (ExpirationDate , 0, 5);
	memset (ServiceCode , 0, 4);
	memset (currency , 0, 4);

	// Get the PAN (skip the first char that is the start flag of the track)
	do
	{
		if (track2_buffer [i + 1] == 'D')
		{
			endPAN = TRUE;
		}
		else
		{
			pan [i] = track2_buffer [i + 1];
			i++;
		}
	}
	while (!endPAN && ( i < 20));

	if (endPAN)
	{
		pan [i] = 0;
		i += 2;


		j = strlen((char*)pan);
		Aschex(&pan_dcb[0],pan,j);
		if     ((j & 0x01) == 0x01)
		{
			// pad an F at the right of data
			pan_dcb[j/2]=pan_dcb[j/2]|0x0F;
			j++;
		}

		// Hide the PAN
		memset ((void*)&pan[6], '*', strlen((char*)pan) - 6 - 4);

		// Extract DATE and Service Code
		ExpirationDate [0] = track2_buffer [i++];
		ExpirationDate [1] = track2_buffer [i++];
		ExpirationDate [2] = track2_buffer [i++];
		ExpirationDate [3] = track2_buffer [i++];

		ServiceCode [0] = track2_buffer [i++];
		ServiceCode [1] = track2_buffer [i++];
		ServiceCode [2] = track2_buffer [i++];

		// Is it an ISO2 card (Service Code begins with a 2 or a 6)
		ICCPresent = FALSE;
		
		// Fallback receipt and data capture POS entry mode
		if ((ServiceCode [0] == '2') || (ServiceCode [0] == '6'))
		{
			ICCPresent = TRUE;

			if (!isFallbackAllowed)
			{
			    // More than ...., between last IC introduction and magnetic stripe : reject the magnetic introduction.
				strcpy ((char* ) text, CUMESS_Read_Message(MAGS_MESS_USE_ICC, lg_code));

				APEMV_UI_MessageDisplay ((char *)text);

				rejection = TRUE;
			}
			else
			{
				resdeb = TRUE;
			}
		}

		PAR_SelectMark(C_TAG_ICS0, C_TREE_ALL);

		// Fallback receipt and data capture POS entry mode
		if (rejection == FALSE)
		{
			hTree = TlvTree_New(0); 
			if (hTree != NULL)
			{
				if (Input_Amount(bValidAmount, amount, &hTree))
				{
#ifdef _USE_UCM_
					if(!UCM_Present)
#endif
					{
					     TlvTree_AddChild(hTree, TAG_APPLI_PRIM_ACCOUNT_NB, ( unsigned char *) pan_dcb, j / 2);

						Aschex((unsigned char*)&ExpirationDate_dcb[0],(unsigned char*)&ExpirationDate[0],4);
						ExpirationDate_dcb[2]=0x30;
						TlvTree_AddChild(hTree, TAG_APPLI_EXPIRATION_DATE, ( unsigned char *) ExpirationDate_dcb, 3);

						PAR_ReadParameter(TAG_TRANSACTION_CURRENCY_CODE, (DataElementExt *)&elt);
						TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length);

						PAR_ReadParameter(TAG_TERMINAL_COUNTRY_CODE, (DataElementExt *)&elt);
						TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length);

						/*&BEGIN FT/06/1188/MA : improvement magstripe management      */
						// DEL_ConcatDEL (&CUMORE_outDEL, &Common_outputDEL);
						/*&END FT/06/1188/MA : improvement magstripe management        */
					}
				}
				else rejection = TRUE;
			}
			else rejection = TRUE;
		}

		// Fallback receipt and data capture POS entry mode
		if (rejection == FALSE)
		{	
			if (!IsPrinter())
			{
				previousUiState = APEMV_UI_TransactionBegin();
				sprintf ((char *) buffer, "\n%.40s\n",track2_buffer);	
				//APEMV_UI_MessageDisplayDelay ((char *)buffer,5);		
				APEMV_UI_TransactionEnd(previousUiState);	
			}
			else
			{
				// Print ticket with PAN, Expiration Date, and Service Code status
				// Trailer Ingenico
				CUPRINT_Open_Printer();
				CULOGO_Print_Ingenico();
				CUPRINT_Boundary_Line(0, 24);
				CUPRINT_Blanks_Lines(1);
				CUPRINT_String_XT((const unsigned char *)CUMESS_Read_Message(MAGS_MESS_MAG_STRIPE_READ, lg_code), ALIGN_LEFT, L1);
				
				memset(buffer, 0, sizeof(buffer));					
				memset(buffer0, 0, sizeof(buffer0));					
				memset(buffer1, 0, sizeof(buffer1));					
				memset(buffer2, 0, sizeof(buffer2));					
				memcpy ((char *) buffer0, track2_buffer, 15);	
				memcpy ((char *) buffer1, &(track2_buffer[15]), 15);
				memcpy ((char *) buffer2, &(track2_buffer[30]), 10);
				sprintf ((char *) buffer, "%.15s\n", buffer0);
				CUPRINT_String_XT(buffer, ALIGN_LEFT, L1);
				memset(buffer, 0, sizeof(buffer));					

				sprintf ((char *) buffer, "%.15s\n", buffer1);
				CUPRINT_String_XT(buffer, ALIGN_LEFT, L1);
				memset(buffer, 0, sizeof(buffer));					

				sprintf ((char *) buffer, "%.15s\n", buffer2);
				CUPRINT_String_XT(buffer, ALIGN_LEFT, L1);

				
				sprintf ((char *) buffer, "\n%.30s\n",pan);
				CUPRINT_String_XT(buffer, ALIGN_LEFT, L1);
				CUPRINT_String_XT((const unsigned char *)CUMESS_Read_Message(MAGS_MESS_EXP_DATE, lg_code), ALIGN_LEFT, L1);
				sprintf ((char *) buffer, "%s\n",ExpirationDate);
				CUPRINT_String_XT(buffer, ALIGN_LEFT, L1);
				CUPRINT_String_XT((const unsigned char *)CUMESS_Read_Message(MAGS_MESS_SERV_CODE, lg_code), ALIGN_LEFT, L1);
				sprintf ((char *) buffer, "%3s\n",ServiceCode);
				CUPRINT_String_XT(buffer, ALIGN_LEFT, L1);

				found = PAR_ReadParameter(TAG_TRANSACTION_CURRENCY_CODE, (DataElementExt *)&elt);
				if (found)
				{
					currency_code = (elt.ptValue[0] << 8) + elt.ptValue[1];

					i = 0;
					usFound = FALSE;
					while ((!usFound) && (i < L_NBCURCY))
					{
						usFound = (ct_confCurrency[i].currency == currency_code);
						if (!usFound)
						{
							i++;
						}
						else
						{
							strcpy((char *) currency, ct_confCurrency[i].label);
							unit = ct_confCurrency[i].unit;
						}
					}
				}

				g_language = My_PSQ_Give_Language();

				sprintf ((char*)buffer, "%s\n", (char*)CUMESS_Read_Message(CU_MESS_AMOUNT, g_language));
				CUPRINT_String_XT(buffer, ALIGN_LEFT, L1);

				if (unit == 0)
				{
					sprintf((char *) buffer, "%8lu %s",entered_amount,currency);
				}
				else
				{
					sprintf((char *) buffer, "%6lu.%02lu %s",entered_amount/100,entered_amount%100,currency);
				}
				CUPRINT_String_XT(buffer, ALIGN_LEFT, L1);

				if (ICCPresent)
				{
					CUPRINT_String_XT((const unsigned char *)CUMESS_Read_Message(MAGS_MESS_CHIP_PRESENT, lg_code), ALIGN_LEFT, L1);

					if ((!rejection))
					{
						CUPRINT_String_XT((const unsigned char *)CUMESS_Read_Message(MAGS_MESS_LAST_CHIP_1, lg_code), ALIGN_LEFT, L1);
						CUPRINT_String_XT((const unsigned char *)CUMESS_Read_Message(MAGS_MESS_LAST_CHIP_2, lg_code), ALIGN_LEFT, L1);
					}
				}
				else
				{
					CUPRINT_String_XT((const unsigned char *)CUMESS_Read_Message(MAGS_MESS_CHIP_ABSENT, lg_code), ALIGN_LEFT, L1);
				}

				// add calls to EMV Tool functionnalities
#ifdef _USE_UCM_
				// reimplementation of Dumptags and traces
				if(UCM_Present)
				{
					SetOutputCom0();
					DumpTags();
				}
				else
#endif
				{
					// Fallback receipt and data capture POS entry mode
					// Magstripe transaction shall not set POS_entry_mode
					if ((!rejection) && (isFallbackAllowed))
					{
						int found;
						int PEMValue;

						found = PAR_ReadParameter(TAG_POS_ENTRY_MODE_CODE, (DataElementExt *)&elt);

						if (found)
						{
							PEMValue = (elt.ptValue[1] << 8) + elt.ptValue[0];
							sprintf ((char* ) chaine, "POS EM %x\n\n\n", PEMValue);
						}
						else
							sprintf ((char* ) chaine, "POS EM %s\n\n\n", NONE);

						CUPRINT_String_XT(chaine, ALIGN_LEFT, L1);
					}

					record_POSEM = FALSE;
					if ((!rejection) && (isFallbackAllowed))
					{
						record_POSEM = TRUE;
					}
					CUBATCH_Create_MAGFinancial_Record (&hTree, record_POSEM);

					// Release local TLV tree
					TlvTree_ReleaseEx(&hTree);
				}
				CUPRINT_Close_Printer();
			}
		}
	}

	return (resdeb);
}

//===========================================================================
int CUSTR_CheckFallback()
{
	DataElement Elt;
	int iFound, iEnded;
	int isFallbackAllowed;
	int i;
	unsigned char PSE_CMD[] = {0x00, 0xA4, 0x04, 0x00, 0x0E, 0x31, 0x50, 0x41, 0x59, 0x2E, 0x53, 0x59, 0x53, 0x2E, 0x44, 0x44, 0x46, 0x30, 0x31, 0x00};
	static const unsigned long RequiredTags[] = { TAG_NUMBER_OF_MATCHING_APP, TAG_LAST_APDU_CMD_RESPONSE }; 
	TLV_TREE_NODE hTree,hTreeCom;

	hTreeCom=NULL;
	isFallbackAllowed = TRUE;

	//APEMV_UI_MessageDisplayDelay((char *)"CUSTR_CheckFallback 1", 1);

	// Retrieve RequiredTags from EMVDC Data base
	hTree = Engine_GetDataElements(sizeof(RequiredTags) / sizeof(RequiredTags[0]), RequiredTags);

	if (hTree != NULL)
	{
		//APEMV_UI_MessageDisplayDelay((char *)"CUSTR_CheckFallback 2", 1);
		// --------------------------------------------------------------------
		// Is the card blocked ?
		iFound = CUSERV_Extract_Element_TLV_Tree(&hTree, TAG_LAST_APDU_CMD_RESPONSE, &Elt);
		if (iFound)
		{
			if ((Elt.ptValue [4] == 0x6A) &&
				(Elt.ptValue [5] == 0x81))
			{
				// The card is blocked, fallback not allowed
				isFallbackAllowed = FALSE;
			}
		}
		//APEMV_UI_MessageDisplayDelay((char *)"CUSTR_CheckFallback 2", 1);

		// --------------------------------------------------------------------
		// Read the number of matching application from EMVDC
		iFound = CUSERV_Extract_Element_TLV_Tree(&hTree, TAG_NUMBER_OF_MATCHING_APP, &Elt);
		if ((iFound) && isFallbackAllowed)
		{
			//APEMV_UI_MessageDisplayDelay((char *)"CUSTR_CheckFallback 3", 1);
			isFallbackAllowed = FALSE;
			if (Elt.ptValue[0] == 0)
			{
				//APEMV_UI_MessageDisplayDelay((char *)"CUSTR_CheckFallback 4", 1);
				// No matching application

				// Parse all the SELECT commands sent
				i = 0;
				iEnded = FALSE;
				while (!iEnded)
				{
					hTreeCom = Engine_GetCommand(i);
					iFound = CUSERV_Extract_Element_TLV_Tree(&hTreeCom, TAG_APDU_CMD, &Elt);
					if (iFound)
					{
						// Is it a select command, and not PSE ?
						if ((Elt.ptValue[0] == 0x00) &&
							(Elt.ptValue[1] == 0xA4) &&
							memcmp (Elt.ptValue, PSE_CMD, 20) != 0)
						{
							// It is a select AID command
							// Is the AID blocked ?
							iFound = CUSERV_Extract_Element_TLV_Tree(&hTreeCom, TAG_APDU_RESPONSE, &Elt);
							if (iFound)
							{
								if ((Elt.ptValue[Elt.length-2] == 0x62) &&
									(Elt.ptValue[Elt.length-1] == 0x83))
								{
									// Application blocked, magstripe can not be used
									isFallbackAllowed = FALSE;
									// !! We don't check if the card is a MasterCard
									// A specific test on the AID could be added.
								}
								else
								{
									// Application not blocked, magstripe allowed
									isFallbackAllowed = TRUE;
								}
							}
						}
					}
					else
					{
						iEnded = TRUE;
					}

					i ++;
				}
			}
			else
			{
				//APEMV_UI_MessageDisplayDelay((char *)"CUSTR_CheckFallback 5", 1);
				// One AID is selected : NO fallback
				isFallbackAllowed = FALSE;
			}
		}
		// release local TLV tree
		TlvTree_ReleaseEx(&hTree);
		TlvTree_ReleaseEx(&hTreeCom);
		//APEMV_UI_MessageDisplayDelay((char *)"CUSTR_CheckFallback 6", 1);
	}
	//APEMV_UI_MessageDisplayDelay((char *)"CUSTR_CheckFallback End", 1);

	return isFallbackAllowed;

}

//===========================================================================
//! \brief This function returns amount tags and transaction date and time.
//! \param[in] bAmount : TRUE if the amount is already available
//! \param[in] amount : the amount if bAmount is TRUE
//! \param[out] phOutputTLVTree : output TLV tree containing amount tags and
//! transaction date and time.
//! \return 
//! - TRUE if output TLV tree built.
//! - FALSE otherwise.
//===========================================================================
static int Input_Amount(int bAmount, unsigned long amount, TLV_TREE_NODE *phOutputTLVTree)
{
	unsigned char NumAmount [6];
	// unsigned long entered_amount;
	int result;
	DataElement elt;
	unsigned char TabAmount [4];
	unsigned char heure_dcb [3];
	unsigned char date_dcb [3];
	Telium_Date_t date;


	entered_amount =0;

	if (bAmount)
	{
		result = T_VAL;
		entered_amount = amount;
	}
	else
	{
		result = CUMESS_GetAmount (0x504, &entered_amount);
	}


	if ((result) && (entered_amount!=0))
	{
		// Amount is valid
		// Convert in num
		CUMESS_ConvertBinNUM (entered_amount, NumAmount);

		// Store amounts in current database
		LongToChar (entered_amount, TabAmount);
		TlvTree_AddChild(*phOutputTLVTree, TAG_AMOUNT_AUTH_BIN, TabAmount, 4);

		TlvTree_AddChild(*phOutputTLVTree, TAG_AMOUNT_AUTH_NUM, NumAmount, 6);

//		LongToChar (0, TabAmount);
//		TlvTree_AddChild(*phOutputTLVTree, TAG_AMOUNT_OTHER_BIN, TabAmount, 4);
//
//		// Convert in num
//		CUMESS_ConvertBinNUM (entered_amount, NumAmount);
//
//		TlvTree_AddChild(*phOutputTLVTree, TAG_AMOUNT_OTHER_NUM, NumAmount, 6);

		PAR_ReadParameter(TAG_TRANSACTION_TYPE, (DataElementExt *)&elt);

		Os__xprinta("cu_str");

		if(elt.ptValue == 0x01)
			Os__xprintaa("0x01");
		else if(elt.ptValue == 0x00)
			Os__xprintaa("0x00");
		else
			Os__xprintaa("wlo");


		TlvTree_AddChild(*phOutputTLVTree, elt.tag, elt.ptValue, elt.length);

		Telium_Read_date(&date);
		// Date of the transaction
		Aschex(&date_dcb[0],date.year,2);
		Aschex(&date_dcb[1],date.month,2);
		Aschex(&date_dcb[2],date.day,2);

		// Time of the transaction
		Aschex(&heure_dcb[0],date.hour,2);
		Aschex(&heure_dcb[1],date.minute,2);
		Aschex(&heure_dcb[2],date.second,2);

		TlvTree_AddChild(*phOutputTLVTree, TAG_TRANSACTION_DATE, ( unsigned char *) date_dcb, 3);

		TlvTree_AddChild(*phOutputTLVTree, TAG_TRANSACTION_TIME, ( unsigned char *) heure_dcb, 3);

		return TRUE;
	}
	else
	{ 
		// amount = 0 or CANCEL
		return FALSE;
	}

	return FALSE;
}
	
//! @}

