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

#ifndef DISABLE_OTHERS_KERNELS_EXPRESS

/////////////////////////////////////////////////////////////////
//// Macros & preprocessor definitions //////////////////////////

const unsigned char ARC_00[]={0x30, 0x30};
const unsigned char ARC_08[]={0x30, 0x38};
const unsigned char ARC_10[]={0x31, 0x30};
const unsigned char ARC_11[]={0x31, 0x31};

/////////////////////////////////////////////////////////////////
//// Global data definition /////////////////////////////////////

word g_TimerTask;										// To store the different tasks ID.
t_topstack * g_tsTimer_task_handle;						// Handle of the timer task.

static word g_wCurrentTask;
static unsigned char g_bTimer_task_to_be_killed = FALSE;	// Global variable used to stop the timer task.
static int g_bTimerTaskRunning = FALSE;						// Indicates if the timer task is still running or if it is waiting to be killed.
static int g_bTimerExpired = FALSE;							// Global variable used to indicate that removal timer expired.
	
static unsigned long gs_ulStartRemovalTimerTime = 0;		// Removal Timer


/////////////////////////////////////////////////////////////////
//// Static functions definition ////////////////////////////////

static int __ClessSample_ExpressPay_AddExpressPaySpecificData (T_SHARED_DATA_STRUCT * pDataStruct);
static int __ClessSample_ExpressPay_RetreiveCvmToApply (T_SHARED_DATA_STRUCT * pResultDataStruct, unsigned char * pCvm);
static T_Bool __ClessSample_ExpressPay_OnlinePinManagement (T_SHARED_DATA_STRUCT * pStructureForOnlineData, int nCardHolderLang);
static int __ClessSample_ExpressPay_ActionCodeDefaultCheck (void);

static void __ClessSample_ExpressPay_KillTimerTask (void);
static int __ClessSample_ExpressPay_LaunchRemovalTimerTask (void);
static void __ClessSample_ExpressPay_InitTimerVariables (void);
static word __ClessSample_ExpressPay_StartTimerTask (void);


/////////////////////////////////////////////////////////////////
//// Functions //////////////////////////////////////////////////



//! \brief Get the ExpressPay CVM to perform.
//! \param[out] pCvm Retreived transaction CVM :
//!		- \a EXPRESSPAY_CVM_NO_CVM No CVM to be performed.
//!		- \a EXPRESSPAY_CVM_SIGNATURE if signature shall be performed.
//!		- \a EXPRESSPAY_CVM_ONLINE_PIN if online PIN shall be performed.
//! \return
//!		- \ref TRUE if correctly retreived.
//!		- \ref FALSE if an error occured.

static int __ClessSample_ExpressPay_RetreiveCvmToApply (T_SHARED_DATA_STRUCT * pResultDataStruct, unsigned char * pCvm)
{
	int nResult = TRUE;
	int nPosition, cr;
	unsigned long ulReadLength;
	const unsigned char * pReadValue;
	
	nPosition = SHARED_EXCHANGE_POSITION_NULL;
	* pCvm = EXPRESSPAY_CVM_NO_CVM; // Default result
	
	cr = GTL_SharedExchange_FindNext(pResultDataStruct, &nPosition, TAG_EXPRESSPAY_TRANSACTION_CVM, &ulReadLength, &pReadValue);
	
	if (cr != STATUS_SHARED_EXCHANGE_OK)
	{
		nResult = FALSE;
		goto End;
	}
	
	// Get the transaction outcome
	* pCvm = pReadValue[0];
	
End:	
	return (nResult);
}


//! \brief Fill buffer with specific ExpressPay for transaction.
//! \param[out] pDataStruct Shared exchange structure filled with the specific ExpressPay data.
//! \return
//!		- \ref TRUE if correctly performed.
//!		- \ref FALSE if an error occured.

static int __ClessSample_ExpressPay_AddExpressPaySpecificData (T_SHARED_DATA_STRUCT * pDataStruct)
{
	int cr, nResult;
	object_info_t ObjectInfo;
	T_KERNEL_TRANSACTION_FLOW_CUSTOM sTransactionFlowCustom;
	unsigned char StepInterruption[KERNEL_PAYMENT_FLOW_STOP_LENGTH];// Bit field to stop payment flow,
																	// if all bit set to 0 => no stop during payment process
									                                // if right bit set to 1 : stop after payment step number 1
	unsigned char StepCustom[KERNEL_PAYMENT_FLOW_CUSTOM_LENGTH]; 	// Bit field to custom payment flow,
																	// if all bit set to 0 => no stop during payment process
									                                // if right bit set to 1 : stop after payment step number 1
	
	if (pDataStruct == NULL)
	{
		GTL_Traces_TraceDebug("__ClessSample_ExpressPay_AddExpressPaySpecificData : Invalid Input data");
		nResult = FALSE;
		goto End;
	}
	
    // Init parameteters
	memset(StepInterruption, 0, sizeof(StepInterruption)); // Default Value : not stop on process
	memset(StepCustom, 0, sizeof(StepCustom)); // Default Value : not stop on process
	ObjectGetInfo(OBJECT_TYPE_APPLI, ApplicationGetCurrent(), &ObjectInfo);
	nResult = TRUE;

	
	// Add a tag for Do_Txn management
	cr = GTL_SharedExchange_AddTag(pDataStruct, TAG_KERNEL_PAYMENT_FLOW_STOP, KERNEL_PAYMENT_FLOW_STOP_LENGTH, (const unsigned char *)StepInterruption);
	if (cr != STATUS_SHARED_EXCHANGE_OK)
	{
		GTL_Traces_TraceDebug("__ClessSample_ExpressPay_AddExpressPaySpecificData : Unable to add TAG_KERNEL_PAYMENT_FLOW_STOP in shared buffer (cr=%02x)", cr);
		nResult = FALSE;
		goto End;
	}
		
	
	// Customize steps
	ADD_STEP_CUSTOM(STEP_EXPRESSPAY_REMOVE_CARD,StepCustom); // To do GUI when ExpressPay card has been read
	ADD_STEP_CUSTOM(STEP_EXPRESSPAY_GET_CERTIFICATE, StepCustom); // To provide the CA key data for ODA
	
	if (ClessSample_IsBlackListPresent())	
		ADD_STEP_CUSTOM(STEP_EXPRESSPAY_EXCEPTION_FILE_GET_DATA, StepCustom); // To check if PAN is in the blacklist
	
	memcpy ((void*)&sTransactionFlowCustom, (void*)StepCustom, KERNEL_PAYMENT_FLOW_CUSTOM_LENGTH);
	sTransactionFlowCustom.usApplicationType = ObjectInfo.application_type; // Kernel will call this application for customisation 
	sTransactionFlowCustom.usServiceId = SERVICE_CUSTOM_KERNEL; // Kernel will call SERVICE_CUSTOM_KERNEL service id for customisation 

    cr = GTL_SharedExchange_AddTag(pDataStruct, TAG_KERNEL_PAYMENT_FLOW_CUSTOM, sizeof(T_KERNEL_TRANSACTION_FLOW_CUSTOM), (const unsigned char *)&sTransactionFlowCustom);
	if (cr != STATUS_SHARED_EXCHANGE_OK)
	{
		GTL_Traces_TraceDebug("__ClessSample_ExpressPay_AddExpressPaySpecificData : Unable to add TAG_KERNEL_PAYMENT_FLOW_CUSTOM in shared buffer (cr=%02x)", cr);
		nResult = FALSE;
		goto End;
	}
	
End:
	return (nResult);
}


//! \brief Perform the Online PIN input and encipher PIN.
//! \param[out] pStructureForOnlineData Data returned by the kernel in which the enciphered online PIN would be added.
//! \return
//!		- \ref TRUE if correctly performed.
//!		- \ref FALSE if an error occured.

static T_Bool __ClessSample_ExpressPay_OnlinePinManagement (T_SHARED_DATA_STRUCT * pStructureForOnlineData, int nCardHolderLang)
{
	T_SHARED_DATA_STRUCT * pDataRequest;
	T_Bool nResult = B_TRUE;
	int cr;
	int nPosition;
	const unsigned char * pPan;
	unsigned long ulPanLength;
	const unsigned char * pAmount;
	unsigned char aucDymmyMsg[] = "";
	unsigned long ulAmountLength;
	unsigned long ulAmount = 0;
	BUFFER_SAISIE buffer_saisie;
	MSGinfos tMsg;
	
	pDataRequest = GTL_SharedExchange_InitShared (128);

	if (pDataRequest != NULL)
	{
		// Clear shared buffer
		GTL_SharedExchange_ClearEx (pDataRequest, FALSE);

		// Indicate tag to be requested
		GTL_SharedExchange_AddTag (pDataRequest, TAG_EMV_APPLI_PAN, 0, NULL);
		GTL_SharedExchange_AddTag (pDataRequest, TAG_EMV_AMOUNT_AUTH_NUM, 0, NULL);

		cr = ExpressPay_GetData (pDataRequest);

		if (cr != KERNEL_STATUS_OK)
		{
			GTL_Traces_TraceDebug("__ClessSample_ExpressPay_OnlinePinManagement : An error occured when getting tags from the ExpressPay kernel (cr=%02x)", cr);
			nResult = B_FALSE;
			goto End;
		}

		// Tags have been got (if present), get the PAN
		nPosition = SHARED_EXCHANGE_POSITION_NULL;
		if (GTL_SharedExchange_FindNext (pDataRequest, &nPosition, TAG_EMV_APPLI_PAN, &ulPanLength, &pPan) != STATUS_SHARED_EXCHANGE_OK)
		{
			GTL_Traces_TraceDebug ("__ClessSample_ExpressPay_OnlinePinManagement : Missing PAN for Online PIN");
			nResult = B_FALSE;
			goto End;
		}

		// Get the transaction amount (numeric)
		nPosition = SHARED_EXCHANGE_POSITION_NULL;
		if (GTL_SharedExchange_FindNext (pDataRequest, &nPosition, TAG_EMV_AMOUNT_AUTH_NUM, &ulAmountLength, &pAmount) != STATUS_SHARED_EXCHANGE_OK)
		{
			GTL_Traces_TraceDebug ("__ClessSample_ExpressPay_OnlinePinManagement : Missing Amount for Online PIN");
			nResult = B_FALSE;
			goto End;
		}
		
		// Convert amount
		GTL_Convert_DcbNumberToUl(pAmount, &ulAmount, ulAmountLength);
		
		// Warning, erase display must be made only if Pin input will be made on customer screen
		if (ClessSample_IsPinpadPresent())
		{
			// Display dummy message to erase display
			tMsg.message = (char*)aucDymmyMsg;
			tMsg.coding = _ISO8859_;
			tMsg.file = GetCurrentFont();

			Helper_DisplayTextCustomer(ERASE, HELPERS_CUSTOMER_LINE_1, &tMsg, CLESSSAMPLE_ALIGN_CENTER, LEDSOFF);
		}
		
		// Request online PIN entry
		cr = ClessSample_PinManagement_OnLinePinManagement ((unsigned char*)pPan, 1, ulAmount, 30000, 10000, nCardHolderLang, &buffer_saisie);

		if (cr == INPUT_PIN_ON)
		{
			cr = GTL_SharedExchange_AddTag(pStructureForOnlineData, TAG_SAMPLE_ENCIPHERED_PIN_CODE, buffer_saisie.nombre , (const unsigned char *)buffer_saisie.donnees);

			if (cr != STATUS_SHARED_EXCHANGE_OK)
			{
				GTL_Traces_TraceDebug ("__ClessSample_ExpressPay_OnlinePinManagement : Unable to add TAG_SAMPLE_ENCIPHERED_PIN_CODE in the shared buffer (cr = %02x)", cr);
				nResult = B_FALSE;
				goto End;
			}
		}
		else if (cr == CANCEL_INPUT)
		{
				GTL_Traces_TraceDebug ("__ClessSample_ExpressPay_OnlinePinManagement : input pin Cancelled");
				nResult = B_NON_INIT;
				goto End;
		}
	}

End:
	return (nResult);
}


//! \brief Manage the debug mode for ExpressPay kernel
//! \param[in] bActivate \a TRUE to activate the debug features. \a FALSE to deactivate features.

void ClessSample_ExpressPay_DebugActivation (int bActivate)
{
	T_SHARED_DATA_STRUCT * pSharedStructure;
	int nResult;
	unsigned char ucDebugMode = 0x00;
	
	if (bActivate)
		ucDebugMode = 0x01;
	
	pSharedStructure = GTL_SharedExchange_InitShared(256);
	
	if (pSharedStructure != NULL)
	{
		nResult = GTL_SharedExchange_AddTag(pSharedStructure, TAG_KERNEL_DEBUG_ACTIVATION, 1, &ucDebugMode);
		
		if (nResult != STATUS_SHARED_EXCHANGE_OK)
		{
			GTL_Traces_TraceDebug("ClessSample_ExpressPay_DebugActivation : Unable to add TAG_KERNEL_DEBUG_ACTIVATION (nResult = %02x)", nResult);
		}
		else
		{
			nResult = ExpressPay_DebugManagement(pSharedStructure);
			
			if (nResult != KERNEL_STATUS_OK)
			{
				GTL_Traces_TraceDebug("ClessSample_ExpressPay_DebugActivation : Error occured during ExpressPay Debug activation (nResult = %02x)", nResult);
			}
		}
		
		// Destroy the shared buffer
		GTL_SharedExchange_DestroyShare(pSharedStructure);
	}
}



//! \brief Perform the ExpressPay kernel customisation.
//! \param[in,out] pSharedData Shared buffer used for customisation.
//! \param[in] ucCustomisationStep Step to be customised.
//! \return
//!		- \a KERNEL_STATUS_CONTINUE always.

int ClessSample_ExpressPay_CustomiseStep (T_SHARED_DATA_STRUCT * pSharedData, const unsigned char ucCustomisationStep)
{
	int nResult = KERNEL_STATUS_CONTINUE;
	unsigned char ucCapkIndex;
	unsigned char ucRid[5];
	unsigned long ulReadLength;
	int nPosition;
	const unsigned char * pReadValue;
	const unsigned char * pPan;
	unsigned long ulPanLength;
	const unsigned char * pPanSeqNumber;
	unsigned char ucVoidPanSeqNumber = C_CLESS_VOID_PAN_SEQ_NUMBER; // Unused value for PanSeqNumber
	unsigned long ulPanSeqNbLength;
	unsigned char bPanInExceptionFile = FALSE;
	
    switch (ucCustomisationStep) // Steps to customise
    {
    case (STEP_EXPRESSPAY_REMOVE_CARD):
		HelperRemoveCardSequence(pSharedData);
		GTL_SharedExchange_ClearEx (pSharedData, FALSE);
		nResult = KERNEL_STATUS_CONTINUE;
		break;
	
	case (STEP_EXPRESSPAY_GET_CERTIFICATE):
		memset (ucRid, 0, sizeof(ucRid));
		nPosition = SHARED_EXCHANGE_POSITION_NULL;
		if (GTL_SharedExchange_FindNext (pSharedData, &nPosition, TAG_EMV_CA_PUBLIC_KEY_INDEX_CARD, &ulReadLength, (const unsigned char **)&pReadValue) == STATUS_SHARED_EXCHANGE_OK)
			ucCapkIndex = pReadValue[0];
		else
			ucCapkIndex = 0;
		
		nPosition = SHARED_EXCHANGE_POSITION_NULL;
		if (GTL_SharedExchange_FindNext (pSharedData, &nPosition, TAG_EMV_DF_NAME, &ulReadLength, (const unsigned char **)&pReadValue) == STATUS_SHARED_EXCHANGE_OK)
			memcpy (ucRid, pReadValue, 5);
		
		GTL_SharedExchange_ClearEx (pSharedData, FALSE);
		
		if (!ClessSample_Parameters_GetCaKeyData (pTreeCurrentParam, ucCapkIndex, ucRid, pSharedData))
		{
			GTL_Traces_TraceDebug ("ClessSample_ExpressPay_CustomiseStep : CUPAR_GetCaKeyData failed");
		}
		
		nResult = KERNEL_STATUS_CONTINUE;
		break;
	
	case (STEP_EXPRESSPAY_EXCEPTION_FILE_GET_DATA):
		// Get the PAN
		nPosition = SHARED_EXCHANGE_POSITION_NULL;
		if (GTL_SharedExchange_FindNext (pSharedData, &nPosition, TAG_EMV_APPLI_PAN, &ulPanLength, &pPan) != STATUS_SHARED_EXCHANGE_OK)
		{
			// Pan parameters is missing, we cannot check BlackList
			GTL_Traces_TraceDebug ("ClessSample_ExpressPay_CustomiseStep : PAN is missing for exception file checking");
			break;
		}
		
		// Get the PAN Sequence Number
		nPosition = SHARED_EXCHANGE_POSITION_NULL;
		if (GTL_SharedExchange_FindNext (pSharedData, &nPosition, TAG_EMV_APPLI_PAN_SEQUENCE_NUMBER, &ulPanSeqNbLength, &pPanSeqNumber) != STATUS_SHARED_EXCHANGE_OK)
		{
			// Pan Sequence Number is missing, we will check BlackList without PanSeqNumber
			pPanSeqNumber = &ucVoidPanSeqNumber;
		}

		// Check if PAN is in the exception file
		bPanInExceptionFile = ClessSample_BlackListIsPan((int)ulPanLength, pPan, (int)(pPanSeqNumber[0]));

		GTL_SharedExchange_ClearEx (pSharedData, FALSE);

		if (bPanInExceptionFile)
		{
			// Add TAG_KERNEL_PAN_IN_BLACK_LIST tag in the exchange buffer to indicate ExpressPay kernel the PAN is in the black list
			if (GTL_SharedExchange_AddTag (pSharedData, TAG_KERNEL_PAN_IN_BLACK_LIST, 1, &bPanInExceptionFile) != STATUS_SHARED_EXCHANGE_OK)
			{
				GTL_SharedExchange_ClearEx (pSharedData, FALSE);
				GTL_Traces_TraceDebug ("ClessSample_ExpressPay_CustomiseStep : Unable to add TAG_KERNEL_PAN_IN_BLACK_LIST in the shared buffer");
			}
		}

		nResult = KERNEL_STATUS_CONTINUE;
		break;

	// Other customisation steps could be defined if necessary
		
    default:
    	GTL_Traces_TraceDebug ("ClessSample_ExpressPay_CustomiseStep : Step to customise (unknown) = %02x\n", ucCustomisationStep);
    	break;
    }
    
    return (nResult);
}



//! \brief Calls the ExpressPay kernel to perform the transaction.
//! \param[in] pDataStruct Data buffer to be filled and used for ExpressPay transaction.
//!	\return
//!		- ExpressPay kernel result.

int ClessSample_ExpressPay_PerformTransaction (T_SHARED_DATA_STRUCT * pDataStruct)
{
	unsigned char bLoopMode;
	int nResult = CLESS_CR_MANAGER_END;
	int cr, ret, nPosition;
	unsigned long ulReadLength;
	const unsigned char * pReadValue;
	unsigned char ucCvm;
	unsigned char auCustomerDisplayAvailable;
	unsigned char * pInfo;
	int merchLang, nCardHolderLang;
	T_Bool bContinueWithOnlineAuthorisation = B_TRUE;
	MSGinfos tMsg;
	unsigned char bUnableToGoOnline = FALSE;
	unsigned char bARCOnlineApproved = FALSE;
	unsigned char bCommunicationResult;
	// int nMsgNumber;

	// Loop mode information
	bLoopMode = ClessSample_Menu_IsTransactionLoopOn();
	
	merchLang = PSQ_Give_Language();
	auCustomerDisplayAvailable = Helper_IsClessCustomerDisplayAvailable();
	
	// Indicate ExpressPay kernel is going to be used (for customisation purposes)
	ClessSample_Customisation_SetUsedPaymentScheme (CLESS_SAMPLE_CUST_EXPRESSPAY);
	
	if (!__ClessSample_ExpressPay_AddExpressPaySpecificData(pDataStruct))
	{
	    GTL_Traces_TraceDebug("ClessSample_ExpressPay_PerformTransaction : ClessSample_Fill_ExpressPayTransaction failed\n");
	}
	else
	{		
		// Debug purpose : if you need to dump the data provided to the kernel
		///ClessSample_DumpData_DumpSharedBuffer(pDataStruct, 0);
		
		cr = ExpressPay_DoTransaction(pDataStruct);
		
		// Debug purpose : if you need to dump the data provided by the kernel
		if (ClessSample_Menu_IsTransactionDataDumpingOn())
			ClessSample_DumpData_DumpSharedBufferWithTitle (pDataStruct, "DATA RECORD");
		
		// Get prefered card language (defaulty returned by the ExpressPay kernel (if present in the kernel database)
		nCardHolderLang = merchLang; // By default, cardholder language is set to default language
		if (ClessSample_Common_RetrieveInfo (pDataStruct, TAG_KERNEL_SELECTED_PREFERED_LANGUAGE, &pInfo))
			nCardHolderLang = ClessSample_Term_GiveLangNumber(pInfo);

		// Get the TAG_EXPRESSPAY_TRANSACTION_CVM to identify the CVM to be performed :
		// 	- EXPRESSPAY_CVM_NO_CVM (0x01) : "No CVM" method has been applied.
		// 	- EXPRESSPAY_CVM_SIGNATURE (0x02) : "Signature" method has been applied.
		// 	- EXPRESSPAY_CVM_ONLINE_PIN (0x04) : "Online PIN" method has been applied.
		if (!__ClessSample_ExpressPay_RetreiveCvmToApply (pDataStruct, &ucCvm))
		{
			ucCvm = EXPRESSPAY_CVM_NO_CVM;
		}
		
		if ((ucCvm == EXPRESSPAY_CVM_ONLINE_PIN) && (cr == KERNEL_STATUS_OFFLINE_APPROVED))
			cr = EXPRESSPAY_STATUS_MAGSTRIPE_ONLINE_AUTHORISATION;

Restart:
		switch (cr)
		{
		case (KERNEL_STATUS_OFFLINE_APPROVED):
		case (EXPRESSPAY_STATUS_EMV_FULL_ONLINE_APPROVED):

			if (ucCvm == EXPRESSPAY_CVM_SIGNATURE)
			{
	    		/* if (auCustomerDisplayAvailable) // If a customer display is available
	    		{
					ClessSample_Term_Read_Message(STD_MESS_SIGNATURE_CHECK_L1, merchLang, &tMsg);
	    			Helper_DisplayTextMerchant(ERASE, HELPERS_MERCHANT_LINE_3, &tMsg, LEDSOFF);
					ClessSample_Term_Read_Message(STD_MESS_SIGNATURE_CHECK_L2, merchLang, &tMsg);
	    			Helper_DisplayTextMerchant(NO_ERASE, HELPERS_MERCHANT_LINE_4, &tMsg, NOLEDSOFF);
					Helper_RefreshScreen(NOWAIT, HELPERS_MERCHANT_SCREEN);
	    		}
	    		ClessSample_Term_Read_Message(STD_MESS_SIGNATURE_CHECK_L1, nCardHolderLang, &tMsg);
				Helper_DisplayTextCustomer(ERASE, HELPERS_CUSTOMER_LINE_2, &tMsg, CLESSSAMPLE_ALIGN_CENTER, LEDSOFF);
				ClessSample_Term_Read_Message(STD_MESS_SIGNATURE_CHECK_L2, nCardHolderLang, &tMsg);
				Helper_DisplayTextCustomer(NO_ERASE, HELPERS_CUSTOMER_LINE_3, &tMsg, CLESSSAMPLE_ALIGN_CENTER, NOLEDSOFF);
				Helper_RefreshScreen(WAIT, HELPERS_CUSTOMER_SCREEN); */
				
				ClessSample_GuiState_DisplayScreen (CLESS_SAMPLE_PAYPASS_SCREEN_SIGNATURE_REQUIRED, merchLang, nCardHolderLang);
			}
			else
			{
			    if(cr == KERNEL_STATUS_OFFLINE_APPROVED)
				    //nMsgNumber = STD_MESS_OFFLINE_APPROVED;
    				ClessSample_GuiState_DisplayScreen (CLESS_SAMPLE_EXPRESSPAY_SCREEN_OFFLINE_APPROVED, merchLang, nCardHolderLang);
			    else
				    //nMsgNumber = STD_MESS_ONLINE_APPROVED;
    				ClessSample_GuiState_DisplayScreen (CLESS_SAMPLE_EXPRESSPAY_SCREEN_ONLINE_APPROVED, merchLang, nCardHolderLang);
				    
				/* if (auCustomerDisplayAvailable) // If a customer display is available
				{
					ClessSample_Term_Read_Message(nMsgNumber, merchLang, &tMsg);
					Helper_DisplayTextMerchant(ERASE, HELPERS_MERCHANT_LINE_3, &tMsg, LEDSOFF);
					Helper_RefreshScreen(NOWAIT, HELPERS_MERCHANT_SCREEN);
				}
				ClessSample_Term_Read_Message(nMsgNumber, nCardHolderLang, &tMsg);
				Helper_DisplayTextCustomer(ERASE, HELPERS_CUSTOMER_LINE_2, &tMsg, CLESSSAMPLE_ALIGN_CENTER, LEDSOFF);
				Helper_RefreshScreen(WAIT, HELPERS_CUSTOMER_SCREEN);*/
			}

			ExpressPay_GetAllData(pDataStruct); // Get all the kernel data to print the receipt
			
			ClessSample_Term_Read_Message(STD_MESS_PRINT_RECEIPT, merchLang, &tMsg);
            if (bLoopMode == 0)
            {
               if ((ClessSample_DumpData_DumpGetOutputId() != CUSTOM_OUTPUT_NONE) && ((ucCvm == EXPRESSPAY_CVM_SIGNATURE) || (HelperQuestionYesNo (&tMsg, 30, merchLang))))
               {
                 if (ucCvm == EXPRESSPAY_CVM_SIGNATURE)
            		ClessSample_Receipt_PrintTransaction(pDataStruct, BATCH_TICKET_AMEX, APPROVED_TICKED, WITH_SIGNATURE, nCardHolderLang, WITH_AOSA);
            	 else
            		ClessSample_Receipt_PrintTransaction(pDataStruct, BATCH_TICKET_AMEX, APPROVED_TICKED, WITHOUT_SIGNATURE, nCardHolderLang, WITH_AOSA);
               }
            }

			if (ucCvm == EXPRESSPAY_CVM_SIGNATURE)
			{
				ClessSample_Term_Read_Message(STD_MESS_SIGNATURE_OK, merchLang, &tMsg);
				if (HelperQuestionYesNo (&tMsg, 30, merchLang))
				{
		    		/* if (auCustomerDisplayAvailable) // If a customer display is available
		    		{
						ClessSample_Term_Read_Message(STD_MESS_SIGNATURE_CORRECT, merchLang, &tMsg);
		    			Helper_DisplayTextMerchant(ERASE, HELPERS_MERCHANT_LINE_3, &tMsg, LEDSOFF);
						Helper_RefreshScreen(NOWAIT, HELPERS_MERCHANT_SCREEN);
		    		}
					
		    		ClessSample_Term_Read_Message(STD_MESS_APPROVED, nCardHolderLang, &tMsg);
					Helper_DisplayTextCustomer(ERASE, HELPERS_CUSTOMER_LINE_2, &tMsg, CLESSSAMPLE_ALIGN_CENTER, LEDSOFF);
					Helper_RefreshScreen(WAIT, HELPERS_CUSTOMER_SCREEN);*/
					
    				ClessSample_GuiState_DisplayScreen (CLESS_SAMPLE_EXPRESSPAY_SCREEN_SIGNATURE_OK, merchLang, nCardHolderLang);
				}
				else
				{
		    		/* if (auCustomerDisplayAvailable) // If a customer display is available
		    		{
						ClessSample_Term_Read_Message(STD_MESS_SIGNATURE_INCORRECT, merchLang, &tMsg);
		    			Helper_DisplayTextMerchant(ERASE, HELPERS_MERCHANT_LINE_3, &tMsg, LEDSOFF);
						Helper_RefreshScreen(NOWAIT, HELPERS_MERCHANT_SCREEN);
		    		}
		    		ClessSample_Term_Read_Message(STD_MESS_OFFLINE_DECLINED, nCardHolderLang, &tMsg);
					Helper_DisplayTextCustomer (ERASE, HELPERS_CUSTOMER_LINE_2, &tMsg, CLESSSAMPLE_ALIGN_CENTER, LEDSOFF);
					Helper_RefreshScreen(WAIT, HELPERS_CUSTOMER_SCREEN); */
					
    				ClessSample_GuiState_DisplayScreen (CLESS_SAMPLE_EXPRESSPAY_SCREEN_SIGNATURE_KO, merchLang, nCardHolderLang);
				}
			}
			break;

 		case (KERNEL_STATUS_OFFLINE_DECLINED):
			ClessSample_GuiState_DisplayScreen (CLESS_SAMPLE_EXPRESSPAY_SCREEN_OFFLINE_DECLINED, merchLang, nCardHolderLang);
			HelperErrorSequence (WITHBEEP);
			Helper_WaitUserDisplay();
			break;
			
		case (EXPRESSPAY_STATUS_EMV_FULL_ONLINE_DECLINED):
			ClessSample_GuiState_DisplayScreen (CLESS_SAMPLE_EXPRESSPAY_SCREEN_ONLINE_DECLINED, merchLang, nCardHolderLang);
			break;

/* 		case (KERNEL_STATUS_OFFLINE_DECLINED):
		case (EXPRESSPAY_STATUS_EMV_FULL_ONLINE_DECLINED):

			if(cr == KERNEL_STATUS_OFFLINE_DECLINED)
				// nMsgNumber = STD_MESS_OFFLINE_DECLINED;
			else
				// nMsgNumber = STD_MESS_ONLINE_DECLINED;
				
			if (auCustomerDisplayAvailable) // If a customer display is available
			{
				ClessSample_Term_Read_Message(nMsgNumber, merchLang, &tMsg);
				Helper_DisplayTextMerchant(ERASE, HELPERS_MERCHANT_LINE_3, &tMsg, LEDSOFF);
				Helper_RefreshScreen(NOWAIT, HELPERS_MERCHANT_SCREEN);
			}
			ClessSample_Term_Read_Message(nMsgNumber, nCardHolderLang, &tMsg);
			Helper_DisplayTextCustomer (ERASE, HELPERS_CUSTOMER_LINE_2, &tMsg, CLESSSAMPLE_ALIGN_CENTER, LEDSOFF);
			Helper_RefreshScreen(NOWAIT, HELPERS_CUSTOMER_SCREEN);
			break;*/
		
		case (EXPRESSPAY_STATUS_CV_NOT_SUCCESSFUL):

			/* if (auCustomerDisplayAvailable) // If a customer display is available
			{
				ClessSample_Term_Read_Message(EXPRESSPAY_MESS_CLESS_TRANS_NOT_PERMITTED_L1, merchLang, &tMsg);
				Helper_DisplayTextMerchant(ERASE, HELPERS_MERCHANT_LINE_2, &tMsg, LEDSOFF);
				ClessSample_Term_Read_Message(EXPRESSPAY_MESS_CLESS_TRANS_NOT_PERMITTED_L2, merchLang, &tMsg);
				Helper_DisplayTextMerchant(NO_ERASE, HELPERS_MERCHANT_LINE_3, &tMsg, NOLEDSOFF);
				Helper_RefreshScreen(NOWAIT, HELPERS_MERCHANT_SCREEN);
			}
			ClessSample_Term_Read_Message(EXPRESSPAY_MESS_CLESS_TRANS_NOT_PERMITTED_L1, nCardHolderLang, &tMsg);
			Helper_DisplayTextCustomer (ERASE, HELPERS_CUSTOMER_LINE_2, &tMsg, CLESSSAMPLE_ALIGN_CENTER, LEDSOFF);
			ClessSample_Term_Read_Message(EXPRESSPAY_MESS_CLESS_TRANS_NOT_PERMITTED_L2, nCardHolderLang, &tMsg);
			Helper_DisplayTextCustomer (NO_ERASE, HELPERS_CUSTOMER_LINE_3, &tMsg, CLESSSAMPLE_ALIGN_CENTER, NOLEDSOFF);
			Helper_RefreshScreen(NOWAIT, HELPERS_CUSTOMER_SCREEN);
			HelperErrorSequence (WITHBEEP);
			Helper_WaitUserDisplay(); */
			
			ClessSample_GuiState_DisplayScreen (CLESS_SAMPLE_EXPRESSPAY_SCREEN_NOT_PERMITTED, merchLang, nCardHolderLang);
			break;

		case (EXPRESSPAY_STATUS_MAGSTRIPE_ONLINE_AUTHORISATION):
		case (EXPRESSPAY_STATUS_EMV_PARTIAL_ONLINE_AUTHORISATION):
		case (EXPRESSPAY_STATUS_EMV_FULL_ONLINE_AUTHORISATION):
			if (ucCvm == EXPRESSPAY_CVM_ONLINE_PIN)
			{
				bContinueWithOnlineAuthorisation = __ClessSample_ExpressPay_OnlinePinManagement (pDataStruct, nCardHolderLang);
			}

			if (bContinueWithOnlineAuthorisation == B_TRUE)
			{
				if(cr == EXPRESSPAY_STATUS_EMV_FULL_ONLINE_AUTHORISATION)
				{
					/* if (auCustomerDisplayAvailable) // If a customer display is available
					{
						ClessSample_Term_Read_Message(EXPRESSPAY_MESS_PROCESSING, merchLang, &tMsg);
						Helper_DisplayTextMerchant(ERASE, HELPERS_MERCHANT_LINE_3, &tMsg, NOLEDSOFF);
						ClessSample_Term_Read_Message(EXPRESSPAY_MESS_PLEASE_WAIT, merchLang, &tMsg);
						Helper_DisplayTextMerchant(NO_ERASE, HELPERS_MERCHANT_LINE_4, &tMsg, NOLEDSOFF);
						Helper_RefreshScreen(NOWAIT, HELPERS_MERCHANT_SCREEN);
					}
					ClessSample_Term_Read_Message(EXPRESSPAY_MESS_PROCESSING, nCardHolderLang, &tMsg);
					Helper_DisplayTextCustomer (ERASE, HELPERS_CUSTOMER_LINE_2, &tMsg, CLESSSAMPLE_ALIGN_CENTER, NOLEDSOFF);
					ClessSample_Term_Read_Message(EXPRESSPAY_MESS_PLEASE_WAIT, nCardHolderLang, &tMsg);
					Helper_DisplayTextCustomer (NO_ERASE, HELPERS_CUSTOMER_LINE_3, &tMsg, CLESSSAMPLE_ALIGN_CENTER, NOLEDSOFF);
					Helper_RefreshScreen(NOWAIT, HELPERS_CUSTOMER_SCREEN);*/
					
        			ClessSample_GuiState_DisplayScreen (CLESS_SAMPLE_EXPRESSPAY_SCREEN_FULL_ONLINE_PROCESSING, merchLang, nCardHolderLang);
				}
				else
				{
					/* if (auCustomerDisplayAvailable) // If a customer display is available
					{
						ClessSample_Term_Read_Message(STD_MESS_ONLINE_REQUEST, merchLang, &tMsg);
						Helper_DisplayTextMerchant(ERASE, HELPERS_MERCHANT_LINE_3, &tMsg, LEDSOFF);
						Helper_RefreshScreen(NOWAIT, HELPERS_MERCHANT_SCREEN);
					}
					ClessSample_Term_Read_Message(STD_MESS_ONLINE_REQUEST, nCardHolderLang, &tMsg);
					Helper_DisplayTextCustomer (ERASE, HELPERS_CUSTOMER_LINE_2, &tMsg, CLESSSAMPLE_ALIGN_CENTER, LEDSOFF);
					Helper_RefreshScreen(NOWAIT, HELPERS_CUSTOMER_SCREEN);*/
					
        			ClessSample_GuiState_DisplayScreen (CLESS_SAMPLE_EXPRESSPAY_SCREEN_FULL_ONLINE_PROCESSING, merchLang, nCardHolderLang);
				}

				if(cr == EXPRESSPAY_STATUS_EMV_FULL_ONLINE_AUTHORISATION)
					__ClessSample_ExpressPay_LaunchRemovalTimerTask ();
		
				bCommunicationResult = ClessSample_FinancialCommunication_ManageAuthorisation (pDataStruct);
				
				if(cr == EXPRESSPAY_STATUS_EMV_FULL_ONLINE_AUTHORISATION)
				{
					// Kill the timer task
					__ClessSample_ExpressPay_KillTimerTask();

					// Removal timer expired, transaction is completed as partial online transaction
					if (g_bTimerExpired)
						cr = EXPRESSPAY_STATUS_EMV_PARTIAL_ONLINE_AUTHORISATION;
				}

				if (bCommunicationResult)
				{
					if(cr == EXPRESSPAY_STATUS_EMV_FULL_ONLINE_AUTHORISATION)
					{
						// Full Online Transaction is performed						
						// Get some data from the response to sent to the kernel
						nPosition = SHARED_EXCHANGE_POSITION_NULL;
						if (GTL_SharedExchange_FindNext(pDataStruct, &nPosition, TAG_EMV_AUTHORISATION_RESPONSE_CODE, &ulReadLength, &pReadValue) == STATUS_SHARED_EXCHANGE_OK)
						{						
							// ARC values of “00”, “08”, “10”, or “11”  indicate Approval.
							if((memcmp(pReadValue, ARC_00, 2) == 0) ||
							   (memcmp(pReadValue, ARC_08, 2) == 0) ||
							   (memcmp(pReadValue, ARC_10, 2) == 0) ||
							   (memcmp(pReadValue, ARC_11, 2) == 0))
		 					{
								bARCOnlineApproved = TRUE;
								ret = GTL_SharedExchange_AddTag(pDataStruct, TAG_EXPRESSPAY_ARC_ONLINE_APPROVED, 1, &bARCOnlineApproved);
								if (ret != STATUS_SHARED_EXCHANGE_OK)
									GTL_Traces_TraceDebug("ClessSample_ExpressPay_PerformTransaction: Unable to add TAG_EXPRESSPAY_ARC_ONLINE_APPROVED in shared buffer (ret=%02x)", ret);

							}
						}
						else
						{
							bUnableToGoOnline = TRUE;
							ret = GTL_SharedExchange_AddTag(pDataStruct, TAG_EXPRESSPAY_UNABLE_TO_GO_ONLINE, 1, &bUnableToGoOnline);
							if (ret != STATUS_SHARED_EXCHANGE_OK)
								GTL_Traces_TraceDebug("ClessSample_ExpressPay_PerformTransaction: Unable to add TAG_EXPRESSPAY_UNABLE_TO_GO_ONLINE in shared buffer (ret=%02x)", ret);
						}
					}
					else // Magstripe Online or EMV Partial Online
					{
						nPosition = SHARED_EXCHANGE_POSITION_NULL;

						if (GTL_SharedExchange_FindNext(pDataStruct, &nPosition, TAG_EMV_AUTHORISATION_RESPONSE_CODE, &ulReadLength, &pReadValue) == STATUS_SHARED_EXCHANGE_OK)
						{
							if((memcmp(pReadValue, ARC_00, 2) == 0) ||
							   (memcmp(pReadValue, ARC_08, 2) == 0) ||
							   (memcmp(pReadValue, ARC_10, 2) == 0) ||
							   (memcmp(pReadValue, ARC_11, 2) == 0))
		 					{
								if (ucCvm == EXPRESSPAY_CVM_SIGNATURE)
								{
									/* if (auCustomerDisplayAvailable) // If a customer display is available
									{
										ClessSample_Term_Read_Message(STD_MESS_SIGNATURE_CHECK_L1, merchLang, &tMsg);
										Helper_DisplayTextMerchant(ERASE, HELPERS_MERCHANT_LINE_3, &tMsg, LEDSOFF);
										ClessSample_Term_Read_Message(STD_MESS_SIGNATURE_CHECK_L2, merchLang, &tMsg);
										Helper_DisplayTextMerchant(NO_ERASE, HELPERS_MERCHANT_LINE_4, &tMsg, NOLEDSOFF);
										Helper_RefreshScreen(NOWAIT, HELPERS_MERCHANT_SCREEN);
									}
									ClessSample_Term_Read_Message(STD_MESS_SIGNATURE_CHECK_L1, nCardHolderLang, &tMsg);
									Helper_DisplayTextCustomer (ERASE, HELPERS_CUSTOMER_LINE_2, &tMsg, CLESSSAMPLE_ALIGN_CENTER, LEDSOFF);
									ClessSample_Term_Read_Message(STD_MESS_SIGNATURE_CHECK_L2, nCardHolderLang, &tMsg);
									Helper_DisplayTextCustomer (NO_ERASE, HELPERS_CUSTOMER_LINE_3, &tMsg, CLESSSAMPLE_ALIGN_CENTER, NOLEDSOFF);
									Helper_RefreshScreen(WAIT, HELPERS_CUSTOMER_SCREEN);*/
									
                        			ClessSample_GuiState_DisplayScreen (CLESS_SAMPLE_EXPRESSPAY_SCREEN_SIGNATURE_REQUIRED, merchLang, nCardHolderLang);
								}
								else
								{
									/* if (auCustomerDisplayAvailable) // If a customer display is available
									{
										ClessSample_Term_Read_Message(STD_MESS_ONLINE_APPROVED, merchLang, &tMsg);
										Helper_DisplayTextMerchant(ERASE, HELPERS_MERCHANT_LINE_3, &tMsg, LEDSOFF);
										Helper_RefreshScreen(NOWAIT, HELPERS_MERCHANT_SCREEN);
									}
									ClessSample_Term_Read_Message(STD_MESS_ONLINE_APPROVED, nCardHolderLang, &tMsg);
									Helper_DisplayTextCustomer (ERASE, HELPERS_CUSTOMER_LINE_2, &tMsg, CLESSSAMPLE_ALIGN_CENTER, LEDSOFF);
									Helper_RefreshScreen(WAIT, HELPERS_CUSTOMER_SCREEN);*/
									
                        			ClessSample_GuiState_DisplayScreen (CLESS_SAMPLE_EXPRESSPAY_SCREEN_ONLINE_APPROVED, merchLang, nCardHolderLang);
								}

								// Get all the kernel data to print the receipt
								ExpressPay_GetAllData(pDataStruct);

								ClessSample_Term_Read_Message(STD_MESS_PRINT_RECEIPT, merchLang, &tMsg);
								if (bLoopMode == 0)
								{
									if ((ClessSample_DumpData_DumpGetOutputId() != CUSTOM_OUTPUT_NONE) && ((ucCvm == EXPRESSPAY_CVM_SIGNATURE) || (HelperQuestionYesNo (&tMsg, 30, merchLang))))
									{
									    if (ucCvm == EXPRESSPAY_CVM_SIGNATURE)
										    ClessSample_Receipt_PrintTransaction(pDataStruct, BATCH_TICKET_AMEX, APPROVED_TICKED, WITH_SIGNATURE, nCardHolderLang, WITH_AOSA);
										else
										    ClessSample_Receipt_PrintTransaction(pDataStruct, BATCH_TICKET_AMEX, APPROVED_TICKED, WITHOUT_SIGNATURE, nCardHolderLang, WITH_AOSA);
									}
								}

								if (ucCvm == EXPRESSPAY_CVM_SIGNATURE)
								{
									ClessSample_Term_Read_Message(STD_MESS_SIGNATURE_OK, merchLang, &tMsg);
									if ((bLoopMode) || (HelperQuestionYesNo (&tMsg, 30, merchLang)))
									{
										/* if (auCustomerDisplayAvailable) // If a customer display is available
										{
											ClessSample_Term_Read_Message(STD_MESS_SIGNATURE_CORRECT, merchLang, &tMsg);
											Helper_DisplayTextMerchant(ERASE, HELPERS_MERCHANT_LINE_3, &tMsg, LEDSOFF);
											Helper_RefreshScreen(NOWAIT, HELPERS_MERCHANT_SCREEN);
										}
										ClessSample_Term_Read_Message(STD_MESS_ONLINE_APPROVED, nCardHolderLang, &tMsg);
										Helper_DisplayTextCustomer (ERASE, HELPERS_CUSTOMER_LINE_2, &tMsg, CLESSSAMPLE_ALIGN_CENTER, LEDSOFF);
										Helper_RefreshScreen(WAIT, HELPERS_CUSTOMER_SCREEN);*/
										
	                        			ClessSample_GuiState_DisplayScreen (CLESS_SAMPLE_EXPRESSPAY_SCREEN_ONLINE_SIGNATURE_OK, merchLang, nCardHolderLang);
									}
									else
									{
										/* if (auCustomerDisplayAvailable) // If a customer display is available
										{
											ClessSample_Term_Read_Message(STD_MESS_SIGNATURE_INCORRECT, merchLang, &tMsg);
											Helper_DisplayTextMerchant(ERASE, HELPERS_MERCHANT_LINE_3, &tMsg, LEDSOFF);
											Helper_RefreshScreen(NOWAIT, HELPERS_MERCHANT_SCREEN);
										}
										ClessSample_Term_Read_Message(STD_MESS_OFFLINE_DECLINED, nCardHolderLang, &tMsg);
										Helper_DisplayTextCustomer (ERASE, HELPERS_CUSTOMER_LINE_2, &tMsg, CLESSSAMPLE_ALIGN_CENTER, LEDSOFF);
										Helper_RefreshScreen(WAIT, HELPERS_CUSTOMER_SCREEN);*/
										
	                        			ClessSample_GuiState_DisplayScreen (CLESS_SAMPLE_EXPRESSPAY_SCREEN_SIGNATURE_KO, merchLang, nCardHolderLang);
									}
								}
							}
							else
							{
								// Authorisation response code do not indicates an Approved transaction
								/* if (auCustomerDisplayAvailable) // If a customer display is available
								{
									ClessSample_Term_Read_Message(STD_MESS_ONLINE_DECLINED, merchLang, &tMsg);
									Helper_DisplayTextMerchant(ERASE, HELPERS_MERCHANT_LINE_3, &tMsg, LEDSOFF);
									Helper_RefreshScreen(NOWAIT, HELPERS_MERCHANT_SCREEN);
								}
								ClessSample_Term_Read_Message(STD_MESS_ONLINE_DECLINED, nCardHolderLang, &tMsg);
								Helper_DisplayTextCustomer (ERASE, HELPERS_CUSTOMER_LINE_2, &tMsg, CLESSSAMPLE_ALIGN_CENTER, LEDSOFF);
								Helper_RefreshScreen(WAIT, HELPERS_CUSTOMER_SCREEN); */
								
                       			ClessSample_GuiState_DisplayScreen (CLESS_SAMPLE_EXPRESSPAY_SCREEN_ONLINE_DECLINED, merchLang, nCardHolderLang);
							}
						}
						else
						{
							bUnableToGoOnline = TRUE;
						}
					}
				}
				else
				{
					if(cr == EXPRESSPAY_STATUS_EMV_FULL_ONLINE_AUTHORISATION)
					{
						bUnableToGoOnline = TRUE;
						ret = GTL_SharedExchange_AddTag(pDataStruct, TAG_EXPRESSPAY_UNABLE_TO_GO_ONLINE, 1, &bUnableToGoOnline);
						if (ret != STATUS_SHARED_EXCHANGE_OK)
							GTL_Traces_TraceDebug("ClessSample_ExpressPay_PerformTransaction: Unable to add TAG_EXPRESSPAY_UNABLE_TO_GO_ONLINE in shared buffer (ret=%02x)", ret);
					}
					else // Magstripe Online or EMV Partial Online
					{
						bUnableToGoOnline = TRUE;
					}
				}

				if(cr == EXPRESSPAY_STATUS_EMV_FULL_ONLINE_AUTHORISATION)
				{
					// EMV Full Online transaction is performed
					cr = ExpressPay_DoTransactionPostProcessing(pDataStruct);
					goto Restart;
				}
				else // Magstripe Online or EMV Partial Online
				{
					if(bUnableToGoOnline)
					{
						if((cr == EXPRESSPAY_STATUS_MAGSTRIPE_ONLINE_AUTHORISATION) ||
						   (cr == EXPRESSPAY_STATUS_EMV_PARTIAL_ONLINE_AUTHORISATION && __ClessSample_ExpressPay_ActionCodeDefaultCheck()))
						{
							// Default Action Code match, transaction is decline
							/* if (auCustomerDisplayAvailable) // If a customer display is available
							{
								ClessSample_Term_Read_Message(STD_MESS_OFFLINE_DECLINED, merchLang, &tMsg);
								Helper_DisplayTextMerchant(ERASE, HELPERS_MERCHANT_LINE_3, &tMsg, LEDSOFF);
								Helper_RefreshScreen(NOWAIT, HELPERS_MERCHANT_SCREEN);
							}
							ClessSample_Term_Read_Message(STD_MESS_OFFLINE_DECLINED, nCardHolderLang, &tMsg);
							Helper_DisplayTextCustomer (ERASE, HELPERS_CUSTOMER_LINE_2, &tMsg, CLESSSAMPLE_ALIGN_CENTER, LEDSOFF);
							Helper_RefreshScreen(WAIT, HELPERS_CUSTOMER_SCREEN);*/
							
                       		ClessSample_GuiState_DisplayScreen (CLESS_SAMPLE_EXPRESSPAY_SCREEN_OFFLINE_DECLINED, merchLang, nCardHolderLang);
						}
						else
						{
							// Default Action Code does not match, transaction is approved offline
							if (ucCvm == EXPRESSPAY_CVM_SIGNATURE)
							{
								/* if (auCustomerDisplayAvailable) // If a customer display is available
								{
									ClessSample_Term_Read_Message(STD_MESS_SIGNATURE_CHECK_L1, merchLang, &tMsg);
									Helper_DisplayTextMerchant(ERASE, HELPERS_MERCHANT_LINE_3, &tMsg, LEDSOFF);
									ClessSample_Term_Read_Message(STD_MESS_SIGNATURE_CHECK_L2, merchLang, &tMsg);
									Helper_DisplayTextMerchant(NO_ERASE, HELPERS_MERCHANT_LINE_4, &tMsg, NOLEDSOFF);
									Helper_RefreshScreen(NOWAIT, HELPERS_MERCHANT_SCREEN);
								}
								ClessSample_Term_Read_Message(STD_MESS_SIGNATURE_CHECK_L1, nCardHolderLang, &tMsg);
								Helper_DisplayTextCustomer(ERASE, HELPERS_CUSTOMER_LINE_2, &tMsg, CLESSSAMPLE_ALIGN_CENTER, LEDSOFF);
								ClessSample_Term_Read_Message(STD_MESS_SIGNATURE_CHECK_L2, nCardHolderLang, &tMsg);
								Helper_DisplayTextCustomer(NO_ERASE, HELPERS_CUSTOMER_LINE_3, &tMsg, CLESSSAMPLE_ALIGN_CENTER, NOLEDSOFF);
								Helper_RefreshScreen(WAIT, HELPERS_CUSTOMER_SCREEN);*/
								
                           		ClessSample_GuiState_DisplayScreen (CLESS_SAMPLE_EXPRESSPAY_SCREEN_SIGNATURE_REQUIRED, merchLang, nCardHolderLang);
							}
							else
							{
								/* if (auCustomerDisplayAvailable) // If a customer display is available
								{
									ClessSample_Term_Read_Message(STD_MESS_OFFLINE_APPROVED, merchLang, &tMsg);
									Helper_DisplayTextMerchant(ERASE, HELPERS_MERCHANT_LINE_3, &tMsg, LEDSOFF);
									Helper_RefreshScreen(NOWAIT, HELPERS_MERCHANT_SCREEN);
								}
								ClessSample_Term_Read_Message(STD_MESS_OFFLINE_APPROVED, nCardHolderLang, &tMsg);
								Helper_DisplayTextCustomer(ERASE, HELPERS_CUSTOMER_LINE_2, &tMsg, CLESSSAMPLE_ALIGN_CENTER, LEDSOFF);
								Helper_RefreshScreen(WAIT, HELPERS_CUSTOMER_SCREEN);*/
								
                           		ClessSample_GuiState_DisplayScreen (CLESS_SAMPLE_EXPRESSPAY_SCREEN_OFFLINE_APPROVED, merchLang, nCardHolderLang);
							}

							ExpressPay_GetAllData(pDataStruct); // Get all the kernel data to print the receipt

							ClessSample_Term_Read_Message(STD_MESS_PRINT_RECEIPT, merchLang, &tMsg);
							if (bLoopMode == 0)
							{
								if ((ClessSample_DumpData_DumpGetOutputId() != CUSTOM_OUTPUT_NONE) && ((ucCvm == EXPRESSPAY_CVM_SIGNATURE) || (HelperQuestionYesNo (&tMsg, 30, merchLang))))
								{
								    if (ucCvm == EXPRESSPAY_CVM_SIGNATURE)
									    ClessSample_Receipt_PrintTransaction(pDataStruct, BATCH_TICKET_AMEX, APPROVED_TICKED, WITH_SIGNATURE, nCardHolderLang, WITH_AOSA);
									else
								        ClessSample_Receipt_PrintTransaction(pDataStruct, BATCH_TICKET_AMEX, APPROVED_TICKED, WITHOUT_SIGNATURE, nCardHolderLang, WITH_AOSA);
								}
							}

							if (ucCvm == EXPRESSPAY_CVM_SIGNATURE)
							{
								ClessSample_Term_Read_Message(STD_MESS_SIGNATURE_OK, merchLang, &tMsg);
								if (HelperQuestionYesNo (&tMsg, 30, merchLang))
								{
									/* if (auCustomerDisplayAvailable) // If a customer display is available
									{
										ClessSample_Term_Read_Message(STD_MESS_SIGNATURE_CORRECT, merchLang, &tMsg);
										Helper_DisplayTextMerchant(ERASE, HELPERS_MERCHANT_LINE_3, &tMsg, LEDSOFF);
										Helper_RefreshScreen(NOWAIT, HELPERS_MERCHANT_SCREEN);
									}

									ClessSample_Term_Read_Message(STD_MESS_APPROVED, nCardHolderLang, &tMsg);
									Helper_DisplayTextCustomer(ERASE, HELPERS_CUSTOMER_LINE_2, &tMsg, CLESSSAMPLE_ALIGN_CENTER, LEDSOFF);
									Helper_RefreshScreen(WAIT, HELPERS_CUSTOMER_SCREEN);*/
									
                               		ClessSample_GuiState_DisplayScreen (CLESS_SAMPLE_EXPRESSPAY_SCREEN_SIGNATURE_OK, merchLang, nCardHolderLang);
								}
								else
								{
									/* if (auCustomerDisplayAvailable) // If a customer display is available
									{
										ClessSample_Term_Read_Message(STD_MESS_SIGNATURE_INCORRECT, merchLang, &tMsg);
										Helper_DisplayTextMerchant(ERASE, HELPERS_MERCHANT_LINE_3, &tMsg, LEDSOFF);
										Helper_RefreshScreen(NOWAIT, HELPERS_MERCHANT_SCREEN);
									}
									ClessSample_Term_Read_Message(STD_MESS_OFFLINE_DECLINED, nCardHolderLang, &tMsg);
									Helper_DisplayTextCustomer (ERASE, HELPERS_CUSTOMER_LINE_2, &tMsg, CLESSSAMPLE_ALIGN_CENTER, LEDSOFF);
									Helper_RefreshScreen(WAIT, HELPERS_CUSTOMER_SCREEN);*/
									
                               		ClessSample_GuiState_DisplayScreen (CLESS_SAMPLE_EXPRESSPAY_SCREEN_SIGNATURE_KO, merchLang, nCardHolderLang);
								}
							}
						}
					}
				}
			}
			else
			{
				/* if (auCustomerDisplayAvailable) // If a customer display is available
				{
					if (bContinueWithOnlineAuthorisation == B_NON_INIT)
						ClessSample_Term_Read_Message(STD_MESS_PIN_CANCEL, merchLang, &tMsg);
					else
						ClessSample_Term_Read_Message(STD_MESS_ONLINE_PIN_ERROR, merchLang, &tMsg);
					Helper_DisplayTextMerchant(ERASE, HELPERS_MERCHANT_LINE_3, &tMsg, LEDSOFF);
					Helper_RefreshScreen(NOWAIT, HELPERS_MERCHANT_SCREEN);
				}
				ClessSample_Term_Read_Message(STD_MESS_OFFLINE_DECLINED, nCardHolderLang, &tMsg);
				Helper_DisplayTextCustomer (ERASE, HELPERS_CUSTOMER_LINE_2, &tMsg, CLESSSAMPLE_ALIGN_CENTER, LEDSOFF);
				Helper_RefreshScreen(WAIT, HELPERS_CUSTOMER_SCREEN);*/

				if (bContinueWithOnlineAuthorisation == B_NON_INIT)
               		ClessSample_GuiState_DisplayScreen (CLESS_SAMPLE_EXPRESSPAY_SCREEN_PIN_CANCEL, merchLang, nCardHolderLang);
                else
               		ClessSample_GuiState_DisplayScreen (CLESS_SAMPLE_EXPRESSPAY_SCREEN_PIN_ERROR, merchLang, nCardHolderLang);
			}
			break;
		
		case (KERNEL_STATUS_USE_CONTACT_INTERFACE):
			nResult = CLESS_CR_MANAGER_RESTART_WO_CLESS;
			break;
		
		case (KERNEL_STATUS_COMMUNICATION_ERROR):
			nResult = CLESS_CR_MANAGER_RESTART;
			break;
			
		default: // Error case
			GTL_Traces_TraceDebug ("ExpressPay_DoTransaction result = %02x", cr);
			/* if (auCustomerDisplayAvailable) // If a customer display is available
			{
				ClessSample_Term_Read_Message(STD_MESS_ERROR, merchLang, &tMsg);
				Helper_DisplayTextMerchant(ERASE, HELPERS_MERCHANT_LINE_3, &tMsg, LEDSOFF);
				Helper_RefreshScreen(NOWAIT, HELPERS_MERCHANT_SCREEN);
			}
			ClessSample_Term_Read_Message(STD_MESS_ERROR, nCardHolderLang, &tMsg);
			Helper_DisplayTextCustomer (ERASE, HELPERS_CUSTOMER_LINE_2, &tMsg, CLESSSAMPLE_ALIGN_CENTER, LEDSOFF);
			Helper_RefreshScreen(NOWAIT, HELPERS_CUSTOMER_SCREEN);*/
			
       		ClessSample_GuiState_DisplayScreen (CLESS_SAMPLE_SCREEN_ERROR, merchLang, nCardHolderLang);
			HelperErrorSequence(WITHBEEP);
			Helper_WaitUserDisplay();
			break;
		}

		// Deselect the card
		ClessEmv_DeselectCard(0, TRUE, FALSE);

		// If the transaction does not restart from the begining, set the LEDs into the idle state
		if (nResult != CLESS_CR_MANAGER_RESTART)
		{
			// Increment 
			ClessSample_Batch_IncrementTransactionSeqCounter();

			// If activated, dump all the kernel database
			if (ClessSample_Menu_IsTransactionDatabaseDumpingOn())
			{
				ExpressPay_GetAllData(pDataStruct); // Get all the kernel data to print the receipt
				ClessSample_DumpData_DumpSharedBufferWithTitle (pDataStruct, "DATABASE DUMP");
			}
		}
	}
	
	// Transaction is completed, clear ExpressPay kernel transaction data
	ExpressPay_Clear ();
	
	// Return result
	return (nResult);
}

//! \brief Modify several parameters before to use it.
//! \param[in/out] pAidParameters parameters found to manage AID.
//!	\return
//!		- ExpressPay kernel result.
void ClessSample_ExpressPay_AidRelatedData(T_SHARED_DATA_STRUCT * pAidParameters)
{
	// Local variables
	int cr;
	int nPositionElement;
	unsigned long ulTerminalTypeLength, ulTerminalCapabilitiesLength;
	unsigned char *pTerminalType, *pTerminalCapabilities;
	object_descriptor_t Descriptor;
	unsigned char ucContactEMVCapable;
	
	ucContactEMVCapable = 0;

	// Protection against bad parameters
	if (pAidParameters == NULL)
		return;

	// Find the Terminal Type(0x9F35) in pAidParameters structure
	nPositionElement = SHARED_EXCHANGE_POSITION_NULL;
	cr = GTL_SharedExchange_FindNext (pAidParameters, &nPositionElement, TAG_EMV_TERMINAL_TYPE, &ulTerminalTypeLength, (const unsigned char **)&pTerminalType);
    if (cr != STATUS_SHARED_EXCHANGE_OK)
		return;	// Not found

	// Find the ExpressPay Terminal Capabilities (0x9F6D) in pAidParameters structure
	nPositionElement = SHARED_EXCHANGE_POSITION_NULL;
	cr = GTL_SharedExchange_FindNext (pAidParameters, &nPositionElement, TAG_EXPRESSPAY_TERMINAL_CAPABILITIES, &ulTerminalCapabilitiesLength, (const unsigned char **)&pTerminalCapabilities);
    if (cr != STATUS_SHARED_EXCHANGE_OK)
		return;	// Not found
	
	pTerminalType[0] &= 0x37;								// 00XX 0XXX erase bits
    pTerminalType[0] |= pTerminalCapabilities[0] & 0xC8;	// Take ExpressPay Terminal Capabilities bits 4,7 and 8 value

	
	// Check if EMV application is loaded or not
	if (ObjectGetDescriptor(OBJECT_TYPE_APPLI, 0x60, &Descriptor) == 0)
	{
		// EMV DC is loaded
		ucContactEMVCapable = 1;
		// Add tag TAG_EXPRESSPAY_CONTACT_EMV_CAPABLE with information true
		cr = GTL_SharedExchange_AddTag (pAidParameters, TAG_EXPRESSPAY_CONTACT_EMV_CAPABLE, 1, &ucContactEMVCapable);
		if (cr != STATUS_SHARED_EXCHANGE_OK)
			return;
	}
}



//! \brief Check if the Default Action Codes match or not (TAG_EXPRESSPAY_INT_TAC_IAC_DEFAULT_MATCHED set to 1 by the ExpressPay kernel).
//! \return
//!		- \ref TRUE if Default Action codes match (decline the transaction).
//!		- \ref FALSE else (approve the transaction).

static int __ClessSample_ExpressPay_ActionCodeDefaultCheck (void)
{
	T_SHARED_DATA_STRUCT * pDataRequest;
	int cr;
	int nPosition;
	const unsigned char * pValue;
	unsigned long ulLength;
			
	pDataRequest = GTL_SharedExchange_InitShared (128);

	if (pDataRequest != NULL)
	{
		// Clear shared buffer
		GTL_SharedExchange_ClearEx (pDataRequest, FALSE);

		// Indicate tag to be requested
		GTL_SharedExchange_AddTag (pDataRequest, TAG_EXPRESSPAY_INT_TAC_IAC_DEFAULT_MATCHED, 0, NULL);
		
		cr = ExpressPay_GetData (pDataRequest);

		if (cr != KERNEL_STATUS_OK)
		{
			GTL_Traces_TraceDebug("__ClessSample_ExpressPay_ActionCodeDefaultCheck : An error occured when getting tags from the ExpressPay kernel (cr=%02x)", cr);
			return (FALSE);
		}

		nPosition = SHARED_EXCHANGE_POSITION_NULL;
		if (GTL_SharedExchange_FindNext (pDataRequest, &nPosition, TAG_EXPRESSPAY_INT_TAC_IAC_DEFAULT_MATCHED, &ulLength, &pValue) != STATUS_SHARED_EXCHANGE_OK)
		{
			GTL_Traces_TraceDebug ("__ClessSample_ExpressPay_ActionCodeDefaultCheck : Missing TAG_EXPRESSPAY_INT_TAC_IAC_DEFAULT_MATCHED");
			return (FALSE);
		}

		if(pValue[0])
			return(TRUE); // Default Action Codes match

		
		// Destroy the shared buffer
		GTL_SharedExchange_DestroyShare(pDataRequest);
	}
		
	return (FALSE);
}


////////////////////////////////////////////
//// TASK FOR EMV FULL ONLINE TRANSACTION //
////////////////////////////////////////////

//! \brief Get the removal timeout value from the parameter file.
//! \param[out] pRemovalTimeOut the timeout value (in milliseconds).
//! \return
//!		- TRUE if tag is present.
//!		- FALSE else.
static int __ClessSample_ExpressPay_GetRemovalTimeOut (unsigned long *pRemovalTimeOut)
{
	TLV_TREE_NODE pTimeOut;
	unsigned char * pValue;
	unsigned int nDataLength;

	pValue = NULL;
	nDataLength = 0;

	pTimeOut = TlvTree_Find(pTreeCurrentParam, TAG_EXPRESSPAY_FULL_ONLINE_EMV_REMOVAL_TIMEOUT, 0);

	if (pTimeOut != NULL)
	{
		pValue = TlvTree_GetData(pTimeOut);
		nDataLength = TlvTree_GetLength(pTimeOut);
	}

	if ((pValue != NULL) && (nDataLength != 0))
	{
		GTL_Convert_BinNumberToUl(pValue, pRemovalTimeOut, nDataLength);
		return (TRUE);
	}

	return (FALSE);
}


/**
 * function:    __ClessSample_ExpressPay_StartTimerTask
 * @brief: Waits an event from custom (Online Authorization received) during XX secondes.
 * @brief: If the event is not received, ask for card removal.
 */

static word __ClessSample_ExpressPay_StartTimerTask (void)
{
	unsigned long ulRemovalTimeOut;	// Card removal timeout (in milliseconds)
	int nTimeout;
	int nLang;
	unsigned char auCustomerDisplayAvailable;
	// MSGinfos tMsg;
	
	g_bTimerTaskRunning = TRUE;		// Indicates the task is running
	g_TimerTask = Telium_CurrentTask();	// Get the Timer task ID and store it in a global variable

	// Start the removal timer
	gs_ulStartRemovalTimerTime = GTL_StdTimer_GetCurrent();

	nLang = PSQ_Give_Language();
	auCustomerDisplayAvailable = Helper_IsClessCustomerDisplayAvailable();

	// First, get the removal timeout value
	if(!__ClessSample_ExpressPay_GetRemovalTimeOut (&ulRemovalTimeOut))
		ulRemovalTimeOut = 0; // Perform a Partial Online transaction
	
	while (!g_bTimer_task_to_be_killed) // While the task is not to be killed by the custom application
	{
		if (g_bTimerExpired)	// Timeout expired, task is waiting to be killed
		{
			g_bTimerTaskRunning = FALSE; // Indicate to the custom application the task is waiting to be killed
			Telium_Ttestall (0,0);		// Wait to be killed
		}

		// Test if removal timer expired
		nTimeout = GTL_StdTimer_GetRemaining(gs_ulStartRemovalTimerTime, ulRemovalTimeOut/10);
		if (nTimeout == 0)
		{
			// Timer expires before a response is received from the Acquirer
			g_bTimerExpired = TRUE;
			// Prompt for the removal of the card
			HelperRemoveCardSequence(NULL);

			// Clear the screen and display "Online request" message
			/* if (auCustomerDisplayAvailable) // If a customer display is available
			{
				ClessSample_Term_Read_Message(STD_MESS_ONLINE_REQUEST, nLang, &tMsg);
				Helper_DisplayTextMerchant(ERASE, HELPERS_MERCHANT_LINE_3, &tMsg, LEDSOFF);
				Helper_RefreshScreen(NOWAIT, HELPERS_MERCHANT_SCREEN);
			}
			ClessSample_Term_Read_Message(STD_MESS_ONLINE_REQUEST, nLang, &tMsg);
			Helper_DisplayTextCustomer (ERASE, HELPERS_CUSTOMER_LINE_2, &tMsg, CLESSSAMPLE_ALIGN_CENTER, LEDSOFF);
			Helper_RefreshScreen(NOWAIT, HELPERS_CUSTOMER_SCREEN);*/
			
       		ClessSample_GuiState_DisplayScreen (CLESS_SAMPLE_EXPRESSPAY_SCREEN_ONLINE_PROCESSING, nLang, nLang);
		}
		
		if(!g_bTimerExpired)
			Telium_Ttestall (USER_EVENT, 1);	// User event (from main task), it indicates the task is going to be killed (because g_bTimer_task_to_be_killed has been set to TRUE by the custom application)

	} // End While

	// The task is stopped by the main application (Online Response received)
	g_bTimerTaskRunning = FALSE; // Update global variable

	// Send the event to the main task
	if (Telium_SignalEvent (g_wCurrentTask, (tEvent)E_USER_EVENT) != cOK)
		GTL_Traces_DiagnosticText ("Timer task: Error when setting the event\n");

	Telium_Ttestall (0,0); // Wait to be killed
	return TRUE;
}


/**
 * function:    __ClessSample_ExpressPay_InitTimerVariables
 * @brief: Initialise removal timer global variables
 * @param	void
 * @return	void
 */

static void __ClessSample_ExpressPay_InitTimerVariables (void)
{
	g_wCurrentTask = 0xFF;				// Init the custom application task number
	g_TimerTask = 0xFF;					// Init the scanning task number
	g_tsTimer_task_handle = NULL;		// Init the scanning task handle
	g_bTimer_task_to_be_killed = FALSE; // Task has not to be killed
	g_bTimerTaskRunning = FALSE;		//Task is not running
	g_bTimerExpired = FALSE;			// Removal timer has not expired
}

/**
 * function:    __ClessSample_ExpressPay_LaunchRemovalTimerTask
 * @brief: Launch the removal timer task.
 * @param	void
 * @return	OK if task correctly launch, KO else.
 */

static int __ClessSample_ExpressPay_LaunchRemovalTimerTask (void)
{
	// Init global variables
	__ClessSample_ExpressPay_InitTimerVariables();

	// Get the main task id
	g_wCurrentTask = Telium_CurrentTask();

	// Launch the timer task
	g_tsTimer_task_handle = Telium_Fork (&__ClessSample_ExpressPay_StartTimerTask, NULL, -1);

	// The task cannot be created
	if (g_tsScanning_task_handle == NULL)
		return KO;
	
	return OK;
}


/**
 * function:    __ClessSample_ExpressPay_KillTimerTask
 * @brief: Kill the removal timer task.
 * @param	void
 * @return	void
 */
static void __ClessSample_ExpressPay_KillTimerTask (void)
{
	if (g_tsTimer_task_handle != NULL) // If the task is launched
	{
		g_bTimer_task_to_be_killed = TRUE; // To Stop the task (if not already waiting to be killed because of timeout expired)

		// Send an event to make the task ready to be killed
		if (Telium_SignalEvent (g_TimerTask, (tEvent)E_USER_EVENT) != cOK)
			GTL_Traces_DiagnosticText ("Main task: Error when setting the event\n");

		while (g_bTimerTaskRunning == TRUE) // While the task has not terminated processing
		{
			// Waits a little
			Telium_Ttestall (USER_EVENT,1);
		}

		// The task is ready to be killed, kill it
		//Telium_Kill(g_tsTimer_task_handle, "-*");
		g_tsTimer_task_handle = NULL;
	}
}

#endif
