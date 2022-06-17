/**
 * \author	Ingenico
 * \author	Copyright (c) 2009 Ingenico, rue claude Chappe,\n
 *			07503 Guilherand-Granges, France, All Rights Reserved.
 *
 * \author	Ingenico has intellectual property rights relating to the technology embodied \n
 *			in this software. In particular, and without limitation, these intellectual property rights may\n
 *			include one or more patents.\n
 *			This software is distributed under licenses restricting its use, copying, distribution, and\n
 *			and decompilation. No part of this software may be reproduced in any form by any means\n
 *			without prior written authorization of Ingenico.
 */


/////////////////////////////////////////////////////////////////
//// Includes ///////////////////////////////////////////////////

#include "ClessSample_Implementation.h"


/////////////////////////////////////////////////////////////////
//// Macros & preprocessor definitions //////////////////////////

// Macro to add customisation steps
#define ADD_CUST_STEP(step,buffer)	buffer[(step-1)/8] += (1<<((step-1)%8))

/////////////////////////////////////////////////////////////////
//// Static function definition /////////////////////////////////


/////////////////////////////////////////////////////////////////
//// Global data definition /////////////////////////////////////

static unsigned char gs_ucSchemeToUse = CLESS_SAMPLE_CUST_UNKNOWN;
static unsigned char gs_bDoubleTapInProgress = FALSE;


/////////////////////////////////////////////////////////////////
//// Functions //////////////////////////////////////////////////

//! \brief Set the used payment scheme.
//! \param[in] ucUsedScheme Indicates the used scheme.

void ClessSample_Customisation_SetUsedPaymentScheme (const unsigned char ucUsedScheme)
{
	gs_ucSchemeToUse = ucUsedScheme;
}



//! \brief Set the used payment scheme.
//! \return The used scheme.
//!		\a CLESS_SAMPLE_CUST_UNKNOWN Scheme is not yet known.
//!		\a CLESS_SAMPLE_CUST_PAYPASS Scheme is PayPass.
//!		\a CLESS_SAMPLE_CUST_PAYWAVE Scheme is payWave.
//!		\a CLESS_SAMPLE_CUST_VISAWAVE Scheme is VisaWave.
//!		\a CLESS_SAMPLE_CUST_EXPRESSPAY Scheme is ExpressPay.
//!		\a CLESS_SAMPLE_CUST_DISCOVER Scheme is Discover.

unsigned char ClessSample_Customisation_GetUsedPaymentScheme (void)
{
	return (gs_ucSchemeToUse);
}



//! \brief Perform the kernel customisation.
//! \param[in,out] pSharedData Shared buffer used for customisation.
//! \return
//!		- \a KERNEL_STATUS_CONTINUE always.

int ClessSample_Customisation_Process (T_SHARED_DATA_STRUCT * pSharedData)
{
	int cr;
	int nResult = KERNEL_STATUS_CONTINUE;
	int nPosition;
	unsigned long ulReadLength;
	unsigned char * pReadValue = NULL;
	unsigned char ucCustomStep;
    
	// Dump provided parameters
	////ClessSample_DumpData_DumpSharedBuffer(pSharedData, 0);
	
	// Get the step to be customised
	nPosition = SHARED_EXCHANGE_POSITION_NULL;
	cr = GTL_SharedExchange_FindNext (pSharedData, &nPosition, TAG_KERNEL_CUSTOM_STEP, &ulReadLength, (const unsigned char **)&pReadValue);
	
	if (cr == STATUS_SHARED_EXCHANGE_OK) // If tag found
	{
		ucCustomStep = pReadValue[0];//omoubarak 241121
        
    	switch (ClessSample_Customisation_GetUsedPaymentScheme())
    	{
    	case (CLESS_SAMPLE_CUST_PAYPASS):
    		nResult = ClessSample_PayPass_CustomiseStep (pSharedData, ucCustomStep);
    		break;

    	case (CLESS_SAMPLE_CUST_PAYWAVE):
    		nResult = ClessSample_payWave_CustomiseStep (pSharedData, ucCustomStep);
    		break;
    	case (CLESS_SAMPLE_CUST_VISAWAVE):
    		nResult = ClessSample_VisaWave_CustomiseStep (pSharedData, ucCustomStep);
    		break;
#ifndef DISABLE_OTHERS_KERNELS_EXPRESS
    	case (CLESS_SAMPLE_CUST_EXPRESSPAY):
    		nResult = ClessSample_ExpressPay_CustomiseStep (pSharedData, ucCustomStep);
    		break;
		case (CLESS_SAMPLE_CUST_DISCOVER):
    		nResult = ClessSample_Discover_CustomiseStep (pSharedData, ucCustomStep);
    		break;
    	case (CLESS_SAMPLE_CUST_INTERAC):
    		nResult = ClessSample_Interac_CustomiseStep (pSharedData, ucCustomStep);
    		break;
#endif
    	default:
    		break;
    	}
	}
    else
    {
		GTL_Traces_DiagnosticText ("ClessSample_Customisation_Process : Custom step tag not found");
    }
	
	return (nResult);
}



//! \brief Perform the kernel customisation.
//! \param[in,out] pSharedData Shared buffer used for customisation.
//! \return
//!		- \a KERNEL_STATUS_CONTINUE always.

int ClessSample_Customisation_Data_Exchange (T_SHARED_DATA_STRUCT * pSharedData)
{
	int nResult = KERNEL_STATUS_CONTINUE;
    
	if (ClessSample_Customisation_GetUsedPaymentScheme() == CLESS_SAMPLE_CUST_PAYPASS)
	{
		if (pSharedData != NULL)
		{
			// For Type Approval testing: Check the DEK Signal with the loaded MCW XML file
			if (!ClessSample_DataExchange_GetDETData (pSharedData))
			{
				// An error occurred when retreiving the DET Signal data in the parameters
				GTL_Traces_TraceDebug ("ClessSample_PayPass_CustomiseStep : ClessSample_Parameters_GetDETData not present");
			}
		}

		nResult = KERNEL_STATUS_CONTINUE;
	}
    else
    {
		GTL_Traces_DiagnosticText ("ClessSample_DE_Process : Other scheme that does not support Data Exchange");
    }
	
	return (nResult);
}



//! \brief Function called for application selection processing customisation.
//! \param[in] pDataStruct Shared exchange structure provided with customisation information.
//! \return
//! 	- CLESS_CUST_DEFAULT to use the default processing. 
//! 	- CLESS_CUST_RETRY_WITH_CLESS if the transaction shall be restarted.
//! 	- CLESS_CUST_RETRY_WITHOUT_CLESS if transaction shall be restarted without cless.
//! 	- CLESS_CUST_STOP if the transaction shall be stopped.
//! 	- CLESS_CUST_NEXT_METHOD if next application selection method shall be used (if present). 
//! 	- CLESS_CUST_CONTINUE_METHOD if next application selection criteria shall be used. 

int ClessSample_Customisation_CustAsProc (T_SHARED_DATA_STRUCT * pDataStruct)
{
	int nResult = CLESS_CUST_DEFAULT;
	int nGtlResult;
    int nPosition;
	unsigned long LengthTag;
	unsigned char * pReadValue = NULL;
	unsigned short usCustomStep;
	unsigned char ucStepsToCustomise[] = {0x44,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	
	///ClessSample_DumpData_DumpSharedBufferWithTitle (pDataStruct, "AS CUST PROC");
	
	perflog_withid(CLOCK_PROCESS_CPUTIME_ID, "MG\tPP_CUST\tClessSample_Customisation_CustAsProc");
	perflog("MG\tPP_CUST\tClessSample_Customisation_CustAsProc");
	nPosition = SHARED_EXCHANGE_POSITION_NULL;
    
	nGtlResult = GTL_SharedExchange_FindNext (pDataStruct, &nPosition, TAG_GENERIC_CUST_STEP_ID, &LengthTag, (const unsigned char **)&pReadValue);
	
	usCustomStep = pReadValue[0];
	
	if (nGtlResult == STATUS_SHARED_EXCHANGE_OK) // If tag found
	{
        switch (usCustomStep) // Steps to customise
        {
		case (CLESS_CUST_AS_PROC_REGISTRATION_STEP_ID):
			GTL_SharedExchange_ClearEx (pDataStruct, FALSE);
			GTL_SharedExchange_AddTag (pDataStruct, TAG_GENERIC_CUST_STEPS_TO_CUSTOMISE, 8, ucStepsToCustomise);
			nResult = CLESS_CUST_CONTINUE;
			break;

		case (CLESS_CUST_AS_PROC_STEP_PPSE_STEP_ID): // Step called when PPSE method is completed
			if (ClessSample_Fill_IsThereAmexAidOnly())
			{
				// Get the EP status code
				nGtlResult = GTL_SharedExchange_FindNext (pDataStruct, &nPosition, TAG_GENERIC_APPLICATION_SELECTION_RESULT, &LengthTag, (const unsigned char **)&pReadValue);
				if (nGtlResult == STATUS_SHARED_EXCHANGE_OK) // If tag found
				{
					if((pReadValue[0] == 0x0A) && (pReadValue[1] == 0x26))
					{	// CLESS_STATUS_EP_CLESS_SELECTION_NOT_ALLOWED
						GTL_Traces_TraceDebug ("CLESS_STATUS_EP_CLESS_SELECTION_NOT_ALLOWED\n");
						nResult = CLESS_CUST_RETRY_WITHOUT_CLESS;
						break;
					}
				}
			}
			else if (ClessSample_Fill_IsThereInteracAidOnly())
			{	
				// Get the EP status code
				nGtlResult = GTL_SharedExchange_FindNext (pDataStruct, &nPosition, TAG_GENERIC_APPLICATION_SELECTION_RESULT, &LengthTag, (const unsigned char **)&pReadValue);
				if (nGtlResult == STATUS_SHARED_EXCHANGE_OK) // If tag found
				{
					// Interac (Section 4.9) requires abort the transaction if there 
					// is an error - the service will exit with a suitable error 
					// Do not perform AID List selection method
					if((pReadValue[0] == 0x0A) && (pReadValue[1] == 0x09))
					{	// CLESS_STATUS_COMMUNICATION_ERROR 
						nResult = CLESS_CUST_STOP;
						GTL_Traces_DiagnosticText ("ClessSample_Customisation_CustAsProc : EP Comms Error 0x%02x%02x)",pReadValue[0], pReadValue[1]);
						break;
					}
					else if((pReadValue[0] == 0x0A) && (pReadValue[1] == 0x0B))
					{	// CLESS_STATUS_MISSING_ICC_DATA
						nResult = CLESS_CUST_STOP;
						GTL_Traces_DiagnosticText ("ClessSample_Customisation_CustAsProc : EP Missing ICC Data 0x%02x%02x)",pReadValue[0], pReadValue[1]);
						break;
					}
					else if((pReadValue[0] == 0x0A) && (pReadValue[1] == 0x0D))
					{	// CLESS_STATUS_ICC_FORMAT_ERROR
						nResult = CLESS_CUST_STOP;
						GTL_Traces_DiagnosticText ("ClessSample_Customisation_CustAsProc : EP ICC Data format Error 0x%02x%02x)",pReadValue[0], pReadValue[1]);
						break;
					}
					else if((pReadValue[0] == 0x0A) && (pReadValue[1] == 0x04))
					{	// CLESS_STATUS_CARD_BLOCKED
						nResult = CLESS_CUST_STOP;
						GTL_Traces_DiagnosticText ("ClessSample_Customisation_CustAsProc : EP Card Blocked Error 0x%02x%02x)",pReadValue[0], pReadValue[1]);
						break;
					}
					else
					{
						GTL_Traces_DiagnosticText ("ClessSample_Customisation_CustAsProc : EP Response 0x%02x%02x)",pReadValue[0], pReadValue[1]);
					}
				}
			}

			nResult = CLESS_CUST_DEFAULT;
			break;
		
		case (CLESS_CUST_AS_PROC_STEP_END_STEP_ID): // Step called when application selection is completed and no method has matched (card is not supported)
			nResult = CLESS_CUST_DEFAULT;
#ifndef DISABLE_OTHERS_KERNELS
			if (ClessSample_Fill_IsThereVisaAid())
			{
				if ((gs_ucSchemeToUse == CLESS_SAMPLE_CUST_PAYWAVE) || (gs_ucSchemeToUse == CLESS_SAMPLE_CUST_UNKNOWN))
				{
					nResult = CLESS_CUST_RETRY_WITHOUT_CLESS;
				}
			}
#endif
			ClessSample_Customisation_SetDoubleTapInProgress(FALSE);
			break;

		default: // Use default behaviour on other steps
			nResult = CLESS_CUST_DEFAULT;
			break;
        }
    }
    else
    {
    	GTL_Traces_TraceDebug ("ClessSample_Customisation_CustAsProc : Custom step tag not found\n");
		nResult = CLESS_CUST_DEFAULT;
    }
	
	perflog("MG\tPP_CUST\tExit ClessSample_Customisation_CustAsProc");
	return (nResult);
}



//! \brief This function is called to customise the GUI during the DLL processing for Visa Asia.
//! \brief Customisation is different according to the GUI mode used (specific GUI for Visa Asia)
//! \brief Customisation is different according to the selection mode (implicit or explicit)
//! \param[in] pDataStruct Shared buffer containing data necessary to customise the step.
//! \return
//!	- \ref CLESS_CUST_DEFAULT if default DLL GUI shall be executed.
//!	- \ref CLESS_CUST_CONTINUE if application made customisation and default DLL GUI shall not be executed.

int ClessSample_Customisation_VisaAsia_SelectionGui (T_SHARED_DATA_STRUCT * pDataStruct)
{
	int nResult = CLESS_CUST_DEFAULT;
	int nGtlResult;
    int nPosition;
	unsigned long LengthTag;
	unsigned char * pReadValue = NULL;
	unsigned short usCustomStep;
	// Steps to customise are identified by a sequence of bits
	// Example 0x28 : the steps to customised are CLESS_CUST_GUI_PRESENT_CARD_AMOUNT_STEP_ID (bit 4)
	// and CLESS_CUST_GUI_RETRY_AMOUNT_STEP_ID (bit 6)
	unsigned char ucStepsToCustomise[] = {0x28,0x58,0x00,0x00,0x00,0x00,0x00,0x00};
		
	// Get the step to be customised
	nPosition = SHARED_EXCHANGE_POSITION_NULL;
	nGtlResult = GTL_SharedExchange_FindNext (pDataStruct, &nPosition, TAG_GENERIC_CUST_STEP_ID, &LengthTag, (const unsigned char **)&pReadValue);
	
	usCustomStep = pReadValue[0];

	if (nGtlResult == STATUS_SHARED_EXCHANGE_OK) // If tag found
	{
        switch (usCustomStep) // Steps to customise
        {
		case (CLESS_CUST_GUI_REGISTRATION_STEP_ID):
			///GTL_Traces_TraceDebug ("Custom GUI : CLESS_CUST_GUI_REGISTRATION_STEP_ID\n");
			GTL_SharedExchange_ClearEx (pDataStruct, FALSE);
			GTL_SharedExchange_AddTag (pDataStruct, TAG_GENERIC_CUST_STEPS_TO_CUSTOMISE, 8, ucStepsToCustomise);
			nResult = CLESS_CUST_CONTINUE;
			break;
		
		case (CLESS_CUST_GUI_PRESENT_CARD_AMOUNT_STEP_ID):
			ClessSample_GuiState_DisplayScreen (CLESS_SAMPLE_SCREEN_PRESENT_CARD, -1, -1);
			nResult = CLESS_CUST_DEFAULT;
			break;

		case (CLESS_CUST_GUI_RETRY_AMOUNT_STEP_ID):
			ClessSample_GuiState_DisplayScreen (CLESS_SAMPLE_VISAWAVE_SCREEN_TRY_AGAIN, -1, -1);

			TPass_LedsOff(TPASS_LED_RED);
			TPass_LedsBlink(TPASS_LED_BLUE, VISA_ASIA_LED_BLINK_ON, VISA_ASIA_LED_BLINK_OFF);

			// Display the 'PRESENT CARD' message
			ClessSample_GuiState_DisplayScreen (CLESS_SAMPLE_SCREEN_PRESENT_CARD, -1, -1);

			nResult = CLESS_CUST_CONTINUE;
			break;

		case (CLESS_CUST_GUI_COLLISION_STEP_ID):
			ClessSample_GuiState_DisplayScreen(CLESS_SAMPLE_VISAWAVE_SCREEN_COLLISION, -1, -1);

			nResult = CLESS_CUST_CONTINUE;
			
			break;

		case (CLESS_CUST_GUI_CARD_NOT_SUPPORTED_STEP_ID):
			ClessSample_GuiState_DisplayScreen(CLESS_SAMPLE_SCREEN_CARD_NOT_SUPPORTED, -1, -1);
			nResult = CLESS_CUST_DEFAULT;
			break;
		
		case (CLESS_CUST_GUI_USE_CONTACT_STEP_ID):
			
			ClessSample_GuiState_DisplayScreen(CLESS_SAMPLE_VISAWAVE_SCREEN_USE_CONTACT, -1, -1);
			nResult = CLESS_CUST_CONTINUE;
			break;
		
		default:
			GTL_Traces_TraceDebug ("ClessSample_Customisation_SelectionGui : Unknown step (id = %02x)\n", usCustomStep);
			nResult = CLESS_CUST_DEFAULT;
			break;
        }
    }
    else
    {
		nResult = CLESS_CUST_DEFAULT;
		GTL_Traces_TraceDebug ("ClessSample_Customisation_SelectionGui : Custom step tag not found\n");
    }
    
	return (nResult);
}



//! \brief This function is called to customise the GUI during the DLL processing of Interac transaction.
//! \param[in] pDataStruct Shared buffer containing data necessary to customise the step.
//! \return
//!	- \ref CLESS_CUST_DEFAULT if default DLL GUI shall be executed.
//!	- \ref CLESS_CUST_CONTINUE if application made customisation and default DLL GUI shall not be executed.

int ClessSample_Customisation_Interac_SelectionGui (T_SHARED_DATA_STRUCT * pDataStruct)
{
	int nResult = CLESS_CUST_DEFAULT;
	int nGtlResult;
    int nPosition;
	unsigned long LengthTag;
	unsigned char * pReadValue = NULL;
	unsigned short usCustomStep;
	unsigned char ucStepsToCustomise[8];
	MSGinfos tMsg;
	int merchLang, nCardHolderLang;
	unsigned char ucTransactionType;
	unsigned long ulTransactionAmount;
	MSGinfos tMsgAmount;
	char buffer[40];
	char *pLogoFont;
	int nDeviceType;

	// Init customisation mask buffer
	memset (ucStepsToCustomise, 0, sizeof(ucStepsToCustomise));

	// Add steps to be customised
	ADD_CUST_STEP (CLESS_CUST_GUI_COLLISION_STEP_ID, ucStepsToCustomise); // If you need to customise the "PRESENT 1 CARD" screen.
	ADD_CUST_STEP (CLESS_CUST_GUI_PRESENT_CARD_AMOUNT_STEP_ID, ucStepsToCustomise); // If you need to customise the "PRESENT CARD" screen.
	ADD_CUST_STEP (CLESS_CUST_GUI_CARD_NOT_SUPPORTED_STEP_ID, ucStepsToCustomise); // If you need to customise the "NOT SUPPORTED" screen.
	ADD_CUST_STEP (CLESS_CUST_GUI_USE_CONTACT_STEP_ID, ucStepsToCustomise); // If you need to customise the "INSERT CARD" screen.
	
	nPosition = SHARED_EXCHANGE_POSITION_NULL;

	// Find the customisation step
	nGtlResult = GTL_SharedExchange_FindNext (pDataStruct, &nPosition, TAG_GENERIC_CUST_STEP_ID, &LengthTag, (const unsigned char **)&pReadValue);
	usCustomStep = pReadValue[0];

	if (nGtlResult == STATUS_SHARED_EXCHANGE_OK) // If tag found
	{
        switch (usCustomStep) // Steps to customise
        {
		case (CLESS_CUST_GUI_REGISTRATION_STEP_ID):
			GTL_SharedExchange_ClearEx (pDataStruct, FALSE);
			GTL_SharedExchange_AddTag (pDataStruct, TAG_GENERIC_CUST_STEPS_TO_CUSTOMISE, 8, ucStepsToCustomise);
			nResult = CLESS_CUST_CONTINUE;
			break;

		case (CLESS_CUST_GUI_COLLISION_STEP_ID):
			merchLang = PSQ_Give_Language();
			nCardHolderLang = merchLang;

			/* ClessSample_Term_Read_Message(INTERAC_MESS_SELECT_ONE_CARD_L1, nCardHolderLang, &tMsg);
			Helper_DisplayTextCustomer(ERASE, HELPERS_CUSTOMER_LINE_2, &tMsg, CLESSSAMPLE_ALIGN_CENTER, NOLEDSOFF);
			ClessSample_Term_Read_Message(INTERAC_MESS_SELECT_ONE_CARD_L2, nCardHolderLang, &tMsg);
			Helper_DisplayTextCustomer(NO_ERASE, HELPERS_CUSTOMER_LINE_3, &tMsg, CLESSSAMPLE_ALIGN_CENTER, NOLEDSOFF);
			Helper_RefreshScreen(NOWAIT, HELPERS_CUSTOMER_SCREEN);*/
			
    		ClessSample_GuiState_DisplayScreen (CLESS_SAMPLE_INTERAC_SCREEN_COLLISION, merchLang, nCardHolderLang);
			
			HelperErrorSequence (WITHBEEP);

#ifndef DISABLE_OTHERS_KERNELS
			// Re-Initialize the timer because card detection restarts
			Interac_StartDetectionTimer();
#endif
			nResult = CLESS_CUST_CONTINUE;
			//GTL_Traces_TraceDebug ("Custom GUI : CLESS_CUST_GUI_COLLISION_STEP_ID\n");
			break;
			
		case (CLESS_CUST_GUI_PRESENT_CARD_AMOUNT_STEP_ID): // Present card customisation
			merchLang = PSQ_Give_Language();
			nCardHolderLang = merchLang;

			// Turn on the first LED
			TPass_LedsOff(TPASS_LED_2 | TPASS_LED_3 | TPASS_LED_4);
			TPass_LedsOn (TPASS_LED_1);

			// Get the INTERAC_MESS_PRESENT_CARD_L1 message and display it
			ClessSample_Term_Read_Message(INTERAC_MESS_PRESENT_CARD, nCardHolderLang, &tMsg);
			Helper_DisplayTextCustomer(ERASE, HELPERS_CUSTOMER_LINE_1, &tMsg, CLESSSAMPLE_ALIGN_CENTER, NOLEDSOFF);

			// Get the data required to display the Amount and label with the correct transaction type
			ulTransactionAmount = ClessSample_Fill_GiveAmount();
			ucTransactionType = ClessSample_Fill_GiveTransactionType();

			if (ucTransactionType == CLESS_SAMPLE_TRANSACTION_TYPE_REFUND)
				ClessSample_Term_Read_Message(INTERAC_MESS_REFUND, nCardHolderLang, &tMsg);
			else
				ClessSample_Term_Read_Message(INTERAC_MESS_DEBIT, nCardHolderLang, &tMsg);

			// prepare the message with transaction type and amount
			sprintf(buffer, "%s$%lu.%02lu\n", tMsg.message, ulTransactionAmount/100, ulTransactionAmount%100);

			// Fill the Message structure with the formatted message and display it
			tMsgAmount.coding = tMsg.coding;
			tMsgAmount.file = tMsg.file;
			tMsgAmount.message = buffer;

			// Display the buffer message
			Helper_DisplayTextCustomer(NO_ERASE, HELPERS_CUSTOMER_LINE_2, &tMsgAmount, CLESSSAMPLE_ALIGN_CENTER, NOLEDSOFF);

			// Display the Contact Less logo
			nDeviceType = TPass_GetDeviceType();
			if((nDeviceType != TPASS_CLESS_TYPE_TELIUM_PASS) && (nDeviceType != TPASS_CLESS_TYPE_VENDING_PASS))
			{
				pLogoFont = LoadFont("/SYSTEM/LOGOCLESS.SGN");
				if(pLogoFont != NULL)
				{
					DefCurrentFont(pLogoFont);
					DrawExtendedString8859(40,36,(unsigned char *)" ",_OFF_,_dLARGE_,_PROP_WIDTH_);
				}
				else
				{
					// if the loading of the M3x font file fail try the M4x font file
					pLogoFont = LoadFont("/SYSTEM/LOGOCLESS.PGN");
					if(pLogoFont != NULL)
					{
						DefCurrentFont(pLogoFont);
						DrawExtendedString8859(40,36,(unsigned char *)" ",_OFF_,_dLARGE_,_PROP_WIDTH_);
					}
					else
					{
						GTL_Traces_TraceDebug("LOGOCLESS font file not found.");		
					}
				}
			}
	
			nResult = CLESS_CUST_CONTINUE; // Mandatory return if you don't want to use the default GUI (Manager message)
			break;

		case (CLESS_CUST_GUI_CARD_NOT_SUPPORTED_STEP_ID):
			merchLang = PSQ_Give_Language();
			nCardHolderLang = merchLang;

			/*ClessSample_Term_Read_Message(INTERAC_MESS_NOT_SUPPORTED_L1, nCardHolderLang, &tMsg);
			Helper_DisplayTextCustomer(ERASE, HELPERS_CUSTOMER_LINE_2, &tMsg, CLESSSAMPLE_ALIGN_CENTER, NOLEDSOFF);
			ClessSample_Term_Read_Message(INTERAC_MESS_NOT_SUPPORTED_L2, nCardHolderLang, &tMsg);
			Helper_DisplayTextCustomer(NO_ERASE, HELPERS_CUSTOMER_LINE_3, &tMsg, CLESSSAMPLE_ALIGN_CENTER, NOLEDSOFF);
			Helper_RefreshScreen(NOWAIT, HELPERS_CUSTOMER_SCREEN);*/
			
    		ClessSample_GuiState_DisplayScreen (CLESS_SAMPLE_INTERAC_SCREEN_NOT_SUPPORTED, merchLang, nCardHolderLang);
			
			HelperErrorSequence (WITHBEEP);

			nResult = CLESS_CUST_CONTINUE;
			break;

		case (CLESS_CUST_GUI_USE_CONTACT_STEP_ID):
			merchLang = PSQ_Give_Language();
			nCardHolderLang = merchLang;

			/* ClessSample_Term_Read_Message(INTERAC_MESS_INSERT_CARD_L1, nCardHolderLang, &tMsg);
			Helper_DisplayTextCustomer(ERASE, HELPERS_CUSTOMER_LINE_2, &tMsg, CLESSSAMPLE_ALIGN_CENTER, NOLEDSOFF);
			ClessSample_Term_Read_Message(INTERAC_MESS_INSERT_CARD_L2, nCardHolderLang, &tMsg);
			Helper_DisplayTextCustomer(NO_ERASE, HELPERS_CUSTOMER_LINE_3, &tMsg, CLESSSAMPLE_ALIGN_CENTER, NOLEDSOFF);
			Helper_RefreshScreen(NOWAIT, HELPERS_CUSTOMER_SCREEN);*/
			
			ClessSample_GuiState_DisplayScreen (CLESS_SAMPLE_INTERAC_SCREEN_CONTACT, merchLang, nCardHolderLang);
			
			HelperErrorSequence (WITHBEEP);

			nResult = CLESS_CUST_CONTINUE;
			break;

		//case (CLESS_CUST_GUI_RETRY_AMOUNT_STEP_ID):
		default:
			GTL_Traces_TraceDebug ("Custom GUI : Unknown step (id = %02x)\n", usCustomStep);
			nResult = CLESS_CUST_DEFAULT;
			break;
        }
    }
    else
    {
		nResult = CLESS_CUST_DEFAULT;
		GTL_Traces_TraceDebug ("Custom step tag not found\n");
    }
    
	return (nResult);
}



//! \brief Set the global data indicating double tap is in progress.
//! \param[in] bDoubleTapInProgress \ref TRUE if double tap is in progress, \ref FALSE else.

void ClessSample_Customisation_SetDoubleTapInProgress (const unsigned char bDoubleTapInProgress)
{
	gs_bDoubleTapInProgress = bDoubleTapInProgress;
}



//! \brief Indicates if double tap is in progress or not.
//! \return
//!	- \ref TRUE if double tap is in progress.
//!	- \ref FALSE else.

unsigned char ClessSample_Customisation_GetDoubleTapInProgress (void)
{
	return (gs_bDoubleTapInProgress);
}



//! \brief This function is called to customise the GUI during the DLL processing.
//! \brief Customisation is different according to the GUI mode used (specific GUI for Visa Asia)
//! \brief Customisation is different according to the selection mode (implicit or explicit)
//! \param[in] pDataStruct Shared buffer containing data necessary to customise the step.
//! \return
//!	- \ref CLESS_CUST_DEFAULT if default DLL GUI shall be executed.
//!	- \ref CLESS_CUST_CONTINUE if application made customisation and default DLL GUI shall not be executed.

int ClessSample_Customisation_Generic_SelectionGui (T_SHARED_DATA_STRUCT * pDataStruct)
{
	int nResult = CLESS_CUST_DEFAULT;
	int nGtlResult;
    int nPosition;
	unsigned long LengthTag;
	unsigned char * pReadValue = NULL;
	unsigned short usCustomStep;
	// Steps to customise are identified by a sequence of bits
	// Example 0x28 : the steps to customised are CLESS_CUST_GUI_PRESENT_CARD_AMOUNT_STEP_ID (bit 4)
	// and CLESS_CUST_GUI_RETRY_AMOUNT_STEP_ID (bit 6)
	///unsigned char ucStepsToCustomise[] = {0x28,0x58,0x00,0x00,0x00,0x00,0x00,0x00};
	unsigned char ucStepsToCustomise[] = {0x78,0x58,0x04,0x00,0x00,0x00,0x00,0x00};
	///const T_PAYMENT_DATA_STRUCT * pTransactionData;
		

	perflog("MG\tPP_CUST\tClessSample_Customisation_Generic_SelectionGui");

	// Init position
	nPosition = SHARED_EXCHANGE_POSITION_NULL;

	// Get the step identifier to be customized
	nGtlResult = GTL_SharedExchange_FindNext (pDataStruct, &nPosition, TAG_GENERIC_CUST_STEP_ID, &LengthTag, (const unsigned char **)&pReadValue);
	
	///pTransactionData = ClessSample_Fill_GetTransactionDataStructure();

	if (nGtlResult == STATUS_SHARED_EXCHANGE_OK) // If tag found
	{
		// Save the step identifier
		usCustomStep = pReadValue[0];

		// Steps to customize
		switch (usCustomStep)
		{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		case (CLESS_CUST_GUI_REGISTRATION_STEP_ID):
			// Clear the output structure
			GTL_SharedExchange_ClearEx (pDataStruct, FALSE);

			// Indicate the steps to be customized
			GTL_SharedExchange_AddTag (pDataStruct, TAG_GENERIC_CUST_STEPS_TO_CUSTOMISE, 8, ucStepsToCustomise);

			nResult = CLESS_CUST_CONTINUE;
			break;

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		case (CLESS_CUST_GUI_PRESENT_CARD_AMOUNT_STEP_ID):
		case (CLESS_CUST_GUI_PRESENT_CARD_NO_AMOUNT_STEP_ID):
			// Note the following specific processing is only there to display "see phone for instructions" when the card collision is detected, to avoid displaying "present card" again after collision is solved
			// Check if a double tap is in progress ("see phone for instructions" is displayed)
			if (ClessSample_Customisation_GetDoubleTapInProgress())
			{
				ClessSample_GuiState_DisplayScreen (CLESS_SAMPLE_SCREEN_PHONE_INSTRUCTIONS, -1, -1);

				// Turn on the first LED only
				HelperCardWait();
				nResult = CLESS_CUST_CONTINUE;
			}
			else
			{
				// Use the default behavior (standard message "CLIENT CARD" with the cless logo if applicable)
				if (ClessSample_Common_GetTransactionMode() == CLESS_SAMPLE_TRANSACTION_MODE_EXPLICIT)
					ClessSample_GuiState_DisplayScreen (CLESS_SAMPLE_SCREEN_PRESENT_CARD, -1, -1);
				nResult = CLESS_CUST_DEFAULT;
			}
			break;

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		case (CLESS_CUST_GUI_DOUBLE_TAP_STEP_ID):

			if (ClessSample_Common_GetTransactionMode() != CLESS_SAMPLE_TRANSACTION_MODE_EXPLICIT)
			{
				// Display the message
				ClessSample_GuiState_DisplayScreen (CLESS_SAMPLE_SCREEN_PHONE_INSTRUCTIONS, -1, -1);
	
				// Turn on the fist LED only
				HelperCardWait();
	
				nResult = CLESS_CUST_CONTINUE;
			}
			else
			{
				nResult = CLESS_CUST_DEFAULT;
			}
			break;

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		case (CLESS_CUST_GUI_RETRY_AMOUNT_STEP_ID):
		case (CLESS_CUST_GUI_RETRY_NO_AMOUNT_STEP_ID):

			// Turn on the fist LED only
			HelperCardWait();

			// Customized to avoid that the TPass DLL performs a beep
			nResult = CLESS_CUST_CONTINUE;
			break;

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		case (CLESS_CUST_GUI_COLLISION_STEP_ID):
			// Display the message
			if (ClessSample_Common_GetTransactionMode() == CLESS_SAMPLE_TRANSACTION_MODE_EXPLICIT)
			{
				ClessSample_GuiState_DisplayScreen (CLESS_SAMPLE_SCREEN_COLLISION, -1, -1);

				nResult = CLESS_CUST_CONTINUE;
			}
			
			break;

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		case (CLESS_CUST_GUI_CARD_NOT_SUPPORTED_STEP_ID):
			if (ClessSample_Common_GetTransactionMode() == CLESS_SAMPLE_TRANSACTION_MODE_EXPLICIT)
			{
				ClessSample_GuiState_DisplayScreen (CLESS_SAMPLE_SCREEN_ERROR, -1, -1);
				HelperErrorSequence(WITHBEEP);

				nResult = CLESS_CUST_CONTINUE;
			}
			break;
		
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		case (CLESS_CUST_GUI_USE_CONTACT_STEP_ID):
			if (ClessSample_Common_GetTransactionMode() == CLESS_SAMPLE_TRANSACTION_MODE_EXPLICIT)
			{
				ClessSample_GuiState_DisplayScreen (CLESS_SAMPLE_VISAWAVE_SCREEN_USE_CONTACT, -1, -1);
				HelperErrorSequence(WITHBEEP);

				nResult = CLESS_CUST_CONTINUE;
			}
			break;
		
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		default:
			GTL_Traces_TraceDebug ("ClessSample_Customisation_Generic_SelectionGui : Unknown step (id = %02x)\n", usCustomStep);
			nResult = CLESS_CUST_DEFAULT;
			break;
        }
    }
    else
    {
		nResult = CLESS_CUST_DEFAULT;
		GTL_Traces_TraceDebug ("ClessSample_Customisation_Generic_SelectionGui : Custom step tag not found\n");
    }
    
	perflog("MG\tPP_CUST\tExit ClessSample_Customisation_Generic_SelectionGui");
	return (nResult);
}



//! \brief Analyse the application selection result and set the transaction result.
//! \param[in] nApplicationSelectionResult Application selection result.
//! \return The transaction result.

int ClessSample_Customisation_AnalyseApplicationSelectionResult (int nApplicationSelectionResult)
{
	int nResult = CLESS_CR_MANAGER_END;

	//APEMV_UI_MessageDisplayDelay ("result.111", 2);
	switch (nApplicationSelectionResult) // Steps to customise
    {
    case (CLESS_STATUS_COMMUNICATION_ERROR):
		//APEMV_UI_MessageDisplayDelay ("result.222", 2);
		nResult = CLESS_CR_MANAGER_RESTART;
    	break;

	case (CLESS_STATUS_NO_MATCHING_APDU):
		//APEMV_UI_MessageDisplayDelay ("result.333", 2);
		nResult = CLESS_CR_MANAGER_END;
		
		// Visa specific requirement: use contact interface
#ifndef DISABLE_OTHERS_KERNELS
		//APEMV_UI_MessageDisplayDelay ("result.444", 2);
		if (ClessSample_Fill_IsThereAidWithKernel(DEFAULT_EP_KERNEL_VISA))
		{
			//APEMV_UI_MessageDisplayDelay ("result.555", 2);
			if ((ClessSample_Customisation_GetUsedPaymentScheme() == CLESS_SAMPLE_CUST_PAYWAVE) || (ClessSample_Customisation_GetUsedPaymentScheme() == CLESS_SAMPLE_CUST_UNKNOWN))
			{
				//APEMV_UI_MessageDisplayDelay ("result.666", 2);
				nResult = CLESS_CR_MANAGER_RESTART_WO_CLESS;
			}
		}
#endif
		//APEMV_UI_MessageDisplayDelay ("result.777", 2);
		ClessSample_Customisation_SetDoubleTapInProgress(FALSE);
		break;

	case (CLESS_STATUS_EP_CLESS_SELECTION_NOT_ALLOWED):
		//APEMV_UI_MessageDisplayDelay ("result.888", 2);
		if (ClessSample_Fill_IsThereAmexAidOnly())
			nResult = CLESS_CR_MANAGER_RESTART_WO_CLESS;
		break;

	default:
		//APEMV_UI_MessageDisplayDelay ("result.999", 2);
    	GTL_Traces_TraceDebug("ClessSample_Customisation_AnalyseApplicationSelectionResult : Unknown application selection result (nApplicationSelectionResult=%02x)", nApplicationSelectionResult);
		nResult = CLESS_CR_MANAGER_END;
		break;
    }

	 return (nResult);
}
