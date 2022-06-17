/**
 * \author	Ingenico
 * \author	Copyright (c) 2008 Ingenico, rue claude Chappe,\n
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
//#include "ClessSample_Logos.h"
#include "Goal.h"
#include <stdlib.h>
#include <string.h>

// Properties of the default screen (Goal)
// =======================================
static const ST_DSP_LINE thScreen[] =
{
	{ {GL_ALIGN_LEFT, GL_ALIGN_CENTER, GL_COLOR_WHITE, GL_COLOR_BLACK, 100, FALSE, {1, 0, 0, 0}, {0, 0, 0, 0, GL_COLOR_WHITE}, {0, 0, 0, 0}, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_MEDIUM}}, // Line0
	  {GL_ALIGN_LEFT, GL_ALIGN_CENTER, FALSE,  100, FALSE, {2, 0, 0, 0}, {0, 0, 0, 0, GL_COLOR_BLACK}, {0, 0, 0, 0}, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_MEDIUM}} },
	{ {GL_ALIGN_LEFT, GL_ALIGN_CENTER, GL_COLOR_WHITE, GL_COLOR_BLACK, 100, FALSE, {1, 0, 0, 0}, {0, 0, 0, 0, GL_COLOR_WHITE}, {0, 0, 0, 0}, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_MEDIUM}}, // Line1
	  {GL_ALIGN_LEFT, GL_ALIGN_CENTER, FALSE,  100, FALSE, {2, 0, 0, 0}, {0, 0, 0, 0, GL_COLOR_BLACK}, {0, 0, 0, 0}, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_MEDIUM}} },
	{ {GL_ALIGN_LEFT, GL_ALIGN_CENTER, GL_COLOR_WHITE, GL_COLOR_BLACK, 100, FALSE, {1, 0, 0, 0}, {0, 0, 0, 0, GL_COLOR_WHITE}, {0, 0, 0, 0}, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_MEDIUM}}, // Line2
	  {GL_ALIGN_LEFT, GL_ALIGN_CENTER, FALSE,  100, FALSE, {2, 0, 0, 0}, {0, 0, 0, 0, GL_COLOR_BLACK}, {0, 0, 0, 0}, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_MEDIUM}} },
	{ {GL_ALIGN_LEFT, GL_ALIGN_CENTER, GL_COLOR_WHITE, GL_COLOR_BLACK, 100, FALSE, {1, 0, 0, 0}, {0, 0, 0, 0, GL_COLOR_WHITE}, {0, 0, 0, 0}, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_MEDIUM}}, // Line3
	  {GL_ALIGN_LEFT, GL_ALIGN_CENTER, FALSE,  100, FALSE, {2, 0, 0, 0}, {0, 0, 0, 0, GL_COLOR_BLACK}, {0, 0, 0, 0}, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_MEDIUM}} },
	{ {GL_ALIGN_LEFT, GL_ALIGN_CENTER, GL_COLOR_WHITE, GL_COLOR_BLACK, 100, FALSE, {1, 0, 0, 0}, {0, 0, 0, 0, GL_COLOR_WHITE}, {0, 0, 0, 0}, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_MEDIUM}}, // Line4
	  {GL_ALIGN_LEFT, GL_ALIGN_CENTER, FALSE,  100, FALSE, {2, 0, 0, 0}, {0, 0, 0, 0, GL_COLOR_BLACK}, {0, 0, 0, 0}, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_MEDIUM}} }
};


static const ST_IMAGE cs_xImage480 =
{
	{GL_COLOR_WHITE, TRUE, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_MEDIUM}, GL_DIRECTION_NONE, {0, 0, 0, 0, GL_COLOR_RED}, {70, 70, 70, 70} },
	{TRUE, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_MEDIUM}, GL_DIRECTION_NONE, {1, 1, 1, 1, GL_COLOR_BLACK}, {2, 1, 2, 1} }
};

static const ST_IMAGE cs_xImage240 =
{
	{GL_COLOR_BLACK, TRUE, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_MEDIUM}, GL_DIRECTION_NONE, {0, 0, 0, 0, GL_COLOR_RED}, {20, 20, 20, 20} },
	{TRUE, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_MEDIUM}, GL_DIRECTION_NONE, {1, 1, 1, 1, GL_COLOR_BLACK}, {2, 1, 2, 1} }
};
/////////////////////////////////////////////////////////////////
//// Macros & preprocessor definitions //////////////////////////


/////////////////////////////////////////////////////////////////
//// Define the transaction card provider type //////////////////

void Remplir_Infopayment_Cless (T_SHARED_DATA_STRUCT * pTransaction, int nCardHolderLang);
void Remplir_Infopayment_Cless_VISA (T_SHARED_DATA_STRUCT * pTransaction,int nCardHolderLang);
/////////////////////////////////////////////////////////////////
//// Static functions definition ////////////////////////////////

static void __ClessSample_Receipt_FormatAmountNaps(unsigned long ulAmount, int bNegative, int nCurrencyExponent, int nWidth, char* pString);



/////////////////////////////////////////////////////////////////
//// Functions //////////////////////////////////////////////////

//! \brief Format the amount for printing purpose.
//! \param[in] ulAmount Amount to be formatted.
//! \param[in] bNegative Indicates if a minus character shall be displayed or not. 
//! \param[in] nCurrencyExponent Currency exponent (for decimal separator location).
//! \param[in] nWidth 
//! \param[out] pString Formatted string for receipt printing.

static void __ClessSample_Receipt_FormatAmountNaps(unsigned long ulAmount, int bNegative, int nCurrencyExponent, int nWidth, char* pString)
{
	char Amount[10 + 1];
	char String[20];
	int nSrcIndex;
	int nDestIndex;
	int bDigitDetected;
	char* pPtr;

	sprintf(Amount, "%010lu", ulAmount);

	bDigitDetected = FALSE;
	nDestIndex = 0;

	memset(String, 0, sizeof(String));

	for(nSrcIndex = 0; nSrcIndex < 10; nSrcIndex++)
	{
		if (!bDigitDetected)
		{
			if (Amount[nSrcIndex] != '0')
				bDigitDetected = TRUE;
			else if (nSrcIndex == 10 - 1 - nCurrencyExponent)
				bDigitDetected = TRUE;
		}

		if (bDigitDetected)
		{
			String[nDestIndex++] = Amount[nSrcIndex];

			if (nSrcIndex < 10 - 1 - nCurrencyExponent)
			{
				if (((10 - 1 - nCurrencyExponent - nSrcIndex) % 3) == 0)
					String[nDestIndex++] = ' ';
			}
			else if (nSrcIndex == 10 - 1 - nCurrencyExponent)
			{
				// If exponent = 0, do not print a decimal separator
				if (nCurrencyExponent != 0)
					String[nDestIndex++] = '.';
			}
		}
	}

	String[nDestIndex] = '\0';

	nDestIndex = strlen(String);
	if (bNegative)
		nDestIndex++;

	pPtr = pString;
	while(nDestIndex < nWidth)
	{
		*(pPtr++) = ' ';
		nDestIndex++;
	}

	if (bNegative)
		*(pPtr++) = '-';

	strcpy(pPtr, String);
}

//! \brief Prints the receipt for C2 kernel transactions.
//! \param[in] pTransaction Structure containing the data returned by the C2 kernel.
//! \param[in] bMobileTransaction Indicates if the transaction has bee performed with a mobile or not.
//! \param[in] nRequiredCvm Indicates the required CVM.
//! \param[in] nCardHolderLang Indicates the cardholder language.

//! \brief Print a transaction ticket (for cardholder or merchant) at the end of a transaction or print transaction batch data, according to input parameter (nTicketType).
//! \param[in] pTransaction Shared exchange buffer containing all the transaction data.
//! \param[in] nTicketType Ticket type.
//! \param[in] nTransactionResult APPROVED_TICKED or DECLINED_TICKED
//! \param[in] bSignatureRequested Indicates if signature is requested or not (\a WITH_SIGNATURE if requested, \a WITHOUT_SIGNATURE else).
//! \param[in] nCardHolderLang Indicates Cardholder lang to use.
//! \param[in] nAOSA : with Available Offline Spending Amount : WITH_AOSA or WITHOUT_AOSA
//! \note Make sure the output driver is already opened before calling this function.

void ClessSample_Receipt_PrintTransaction (T_SHARED_DATA_STRUCT * pTransaction, int nTicketType, int nTransactionResult, const int bSignatureRequested, int nCardHolderLang, int nAOSA)
{
	int nResult, nPosition;
	unsigned long ulTag;
	unsigned long ulReadLength;
	const unsigned char * pValue;
	const unsigned char* pValueStatusCode;
	const unsigned char* pValueTsc;
	unsigned int LengthTsc;
	const unsigned char* pValueAid;
	unsigned int LengthAid;
	//const unsigned char* pValueCid;
	const unsigned char* pValueCvrResult;
	const unsigned char* pValueTvr;
	const unsigned char* pValueAmountBin;
	const unsigned char* pValueAmountNum;
	const unsigned char* pValueAmountOtherBin;
	const unsigned char* pValueAmountOtherNum;
	const unsigned char* pValueExpirationDate;
	const unsigned char* pValuePan;
	unsigned int LengthPan;
	const unsigned char* pValuePanSeq;
	const unsigned char* pValueAuthorCode;
	const unsigned char* pValueAuthorRespCode;
	const unsigned char* pValueCurrencyCode;
	const unsigned char* pValueCurrencyExp;
	const unsigned char* pValueDate;
	const unsigned char* pValueTime;
	const unsigned char* pValueType;
	//const unsigned char* pValueForcedOnline;
	const unsigned char* pAvailableOfflineSpendingAmount;
	unsigned int length_spending_amount;
	const unsigned char* pVisaWaveAvailableOfflineSpendingAmount;
	unsigned int length_VisaWave_spending_amount;
	//const unsigned char* pValueCardType;
	unsigned char temp;
	const unsigned char* pValueApplicationLabel;
	unsigned int length_application_label;
	const unsigned char* pValueAppliPreferedName;
	unsigned int length_appli_prefered_name;
	unsigned long ulStatusCode;
	unsigned long ulTsc;
	unsigned char Aid[20];
	//unsigned char Cid;
	unsigned char CvrResult[3];
	unsigned char Tvr[5];
	unsigned long ulAmount;
	unsigned long ulAmountOther;
	char ExpirationDate[6 + 1];
	char Pan[19 + 1];
	unsigned long ulPanSeq;
	char AuthorCode[6 + 1];
	char AuthorRespCode[2 + 1];
	unsigned long ulCurrencyCode;
	unsigned long ulCurrencyExp;
	char Date[6 + 1];
	char Time[6 + 1];
	unsigned long ulType;
	//int bForcedOnline;
	char szAmount[20];
	char szSpendingAmount[20];
	char szVisaWaveSpendingAmount[20];
	//char transactionKind;
	//unsigned long ulAvailableOfflineSpendingAmount;
	//unsigned short usCardType;
	unsigned char ApplicationLabel [16 + 1]; // Application label + '\0'
	unsigned char AppliPreferedName [16 + 1]; // Application prefered name + '\0'
	MSGinfos tDisplayMsg;



	GL_GraphicLib_SetCharset(APEMV_UI_GoalHandle(), GL_ENCODING_ISO_8859_1);
	T_GL_HWIDGET hPicture=NULL;
//
	hPicture = GoalCreatePicture(APEMV_UI_GoalHandle()); // Create and clear screen
	GL_Widget_SetBackColor(hPicture, GL_COLOR_WHITE);
//	GoalDspPicture(hPicture, "Transaction acceptée", "file://flash/HOST/Approuvee.png",&cs_xImage240, (GL_TIME_SECOND) * 2, true);
	GoalDestroyPicture(&hPicture);
	//if(StructInfoPayment.CodeFunction == SALECODE)
		//MessageWithDelay("Transaction acceptée", "file://flash/HOST/Approuvee.png", 2000);
	//MessageWithoutConfirmation("Transaction validée", "file://flash/HOST/Approuvee.png");

	if(nTransactionResult == APPROVED_TICKED){
		//Os__xprinta("Here 11");
		Retour_Caisse("000");
	}

	else{
		//Os__xprinta("Here 12");
		Retour_Caisse("999"); //Cless_ A développer
	}
	return;


	// no receipt if loop mode
	if (ClessSample_Menu_IsTransactionLoopOn())
		return;

	if (!ClessSample_DumpData_DumpOpenOutputDriver())
		return;

	pValueStatusCode = NULL;
	pValueTsc = NULL;
	LengthTsc = 0;
	pValueAid = NULL;
	LengthAid = 0;
	//pValueCid = NULL;
	pValueCvrResult = NULL;
	pValueTvr = NULL;
	pValueAmountBin = NULL;
	pValueAmountNum = NULL;
	pValueAmountOtherBin = NULL;
	pValueAmountOtherNum = NULL;
	pValueExpirationDate = NULL;
	pValuePan = NULL;
	LengthPan = 0;
	pValuePanSeq = NULL;
	pValueAuthorCode = NULL;
	pValueAuthorRespCode = NULL;
	pValueCurrencyCode = NULL;
	pValueCurrencyExp = NULL;
	pValueDate = NULL;
	pValueTime = NULL;
	pValueType = NULL;
	//pValueForcedOnline = NULL;
	pAvailableOfflineSpendingAmount = NULL;
	length_spending_amount = 0;
	pVisaWaveAvailableOfflineSpendingAmount = NULL;
	length_VisaWave_spending_amount = 0;
	//transactionKind =0;
	//pValueCardType = NULL;
	//usCardType = 0;
	pValueApplicationLabel = NULL;
	length_application_label = 0;
	pValueAppliPreferedName = NULL;
	length_appli_prefered_name = 0;

	//badr problème d'affichage des accents
		T_GL_HGRAPHIC_LIB hGoal_fr=APEMV_UI_GoalHandle();
		T_GL_HWIDGET hScreen_fr=NULL;

		hScreen_fr = GoalCreateScreen(hGoal_fr, thScreen, NUMBER_OF_LINES(thScreen), GL_ENCODING_ISO_8859_1);
		if (hScreen_fr)
			GoalDestroyScreen(&hScreen_fr);
	Remplir_Infopayment_Cless (pTransaction, nCardHolderLang);
	//valide_carte(); //KamalCless

	if(nTransactionResult == APPROVED_TICKED){
		//Os__xprinta("Here 12");
		Retour_Caisse("000");
	}

	else{
		//Os__xprinta("Here 13");
		Retour_Caisse("999"); //Cless_ A développer
	}


	return;



	//Telium_DefprinterpatternHR(0, 0, 0xF1, (char *) Line_part_1);
	//Telium_DefprinterpatternHR (0, 0, 0xF2, (char *) Line_part_2);
	//Telium_DefprinterpatternHR (0, 0, 0xF3, (char *) Visa_payWave_part_1);
	//Telium_DefprinterpatternHR (0, 0, 0xF4, (char *) Visa_payWave_part_2);
	//Telium_DefprinterpatternHR (0, 0, 0xF5, (char *) Visa_1);
	//Telium_DefprinterpatternHR (0, 0, 0xF6, (char *) Visa_2);
	//Telium_DefprinterpatternHR (0, 0, 0xF7, (char *) Ingenico_part_1);
	//Telium_DefprinterpatternHR (0, 0, 0xF8, (char *) Ingenico_part_2);
	//Telium_DefprinterpatternHR (0, 0, 0xF9, (char *) Signature_part_1);
	//Telium_DefprinterpatternHR (0, 0, 0xFA, (char *) Signature_part_2);
	//Telium_DefprinterpatternHR (0, 0, 0xFB, (char *) ClessWavesCenterOnly_Part1);
	//Telium_DefprinterpatternHR (0, 0, 0xFC, (char *) ClessWavesCenterOnly_Part2);
	//Telium_DefprinterpatternHR (0, 0, 0xFD, (char *) MasterCardPayPass_Part1);
	//Telium_DefprinterpatternHR (0, 0, 0xFE, (char *) MasterCardPayPass_Part2);
	//Telium_DefprinterpatternHR (0, 0, 0xEE, (char *) AmericanExpress_part_1);
	//Telium_DefprinterpatternHR (0, 0, 0xEF, (char *) AmericanExpress_part_2);
	//Telium_DefprinterpatternHR (0, 0, 0xEC, (char *) Discover_part_1);
	//Telium_DefprinterpatternHR (0, 0, 0xED, (char *) Discover_part_2);
	//Telium_DefprinterpatternHR (0, 0, 0xEA, (char *) Interac_only_part_1);
	//Telium_DefprinterpatternHR (0, 0, 0xEB, (char *) Interac_only_part_2);
	//Telium_DefprinterpatternHR (0, 0, 0xE8, (char *) Cartes2010_Logo_Part_1);
	//Telium_DefprinterpatternHR (0, 0, 0xE9, (char *) Cartes2010_Logo_Part_2);

	nPosition = SHARED_EXCHANGE_POSITION_NULL;

	do{
    	// Try to following tag.
    	nResult = GTL_SharedExchange_GetNext (pTransaction, &nPosition, &ulTag, &ulReadLength, &pValue);

        if (nResult == STATUS_SHARED_EXCHANGE_OK)  // If tag found
        {
			switch(ulTag)
			{
			case TAG_KERNEL_STATUS_CODE:					pValueStatusCode = pValue; break;
    		case TAG_EMV_TRANSACTION_SEQUENCE_COUNTER:		pValueTsc = pValue;	LengthTsc = ulReadLength; break;
    		case TAG_EMV_AID_CARD:							pValueAid = pValue;	LengthAid = ulReadLength; break;
    		case TAG_EMV_DF_NAME:							pValueAid = pValue;	LengthAid = ulReadLength; break;
    		//case TAG_EMV_CRYPTOGRAM_INFO_DATA:				pValueCid = pValue; break;
    		case TAG_EMV_CVM_RESULTS:						pValueCvrResult = pValue; break;
    		case TAG_EMV_TVR:								pValueTvr = pValue; break;
    		case TAG_EMV_AMOUNT_AUTH_BIN:					pValueAmountBin = pValue; break;
    		case TAG_EMV_AMOUNT_AUTH_NUM:					pValueAmountNum = pValue; break;
    		case TAG_EMV_AMOUNT_OTHER_BIN:					pValueAmountOtherBin = pValue; break;
    		case TAG_EMV_AMOUNT_OTHER_NUM:					pValueAmountOtherNum = pValue; break;
    		case TAG_EMV_APPLI_EXPIRATION_DATE:				pValueExpirationDate = pValue; break;
    		case TAG_EMV_APPLI_PAN:							pValuePan = pValue;	LengthPan = ulReadLength; break;
    		case TAG_EMV_APPLI_PAN_SEQUENCE_NUMBER:			pValuePanSeq = pValue; break;
    		case TAG_EMV_AUTHORISATION_CODE:				pValueAuthorCode = pValue; break;
    		case TAG_EMV_AUTHORISATION_RESPONSE_CODE:		pValueAuthorRespCode = pValue; break;
    		case TAG_EMV_TRANSACTION_CURRENCY_CODE:			pValueCurrencyCode = pValue; break;
    		case TAG_EMV_TRANSACTION_CURRENCY_EXPONENT:		pValueCurrencyExp = pValue; break;
    		case TAG_EMV_TRANSACTION_DATE:					pValueDate = pValue; break;
    		case TAG_EMV_TRANSACTION_TIME:					pValueTime = pValue; break;
    		case TAG_EMV_TRANSACTION_TYPE:					pValueType = pValue; break;
    		//case TAG_KERNEL_CARD_TYPE:						pValueCardType=pValue; break;
    		case TAG_EMV_APPLICATION_LABEL:					pValueApplicationLabel=pValue; length_application_label = ulReadLength; break;
    		case TAG_EMV_APPLI_PREFERED_NAME:				pValueAppliPreferedName=pValue; length_appli_prefered_name = ulReadLength; break;
#ifndef DISABLE_OTHERS_KERNELS
    		case TAG_PAYWAVE_AVAILABLE_OFFLINE_SPENDING_AMOUNT: pAvailableOfflineSpendingAmount=pValue; length_spending_amount = ulReadLength; break;
			case TAG_VISAWAVE_VLP_AVAILABLE_FUNDS:			pVisaWaveAvailableOfflineSpendingAmount=pValue; length_VisaWave_spending_amount = ulReadLength; break;
#endif
			default:
				break;
			}
        }
        else if (nResult == STATUS_SHARED_EXCHANGE_END)  // Buffer end reached
        {
        }
        else // error or end or ...
        {
        	GTL_Traces_TraceDebug("Display tags error:%x\n", nResult);
        }
    }while (nResult == STATUS_SHARED_EXCHANGE_OK);



	ulStatusCode = 0;
	ulTsc = 0;
	memset(Aid, 0, sizeof(Aid));
	//Cid = 0;
	memset(CvrResult, 0, sizeof(CvrResult));
	memset(Tvr, 0, sizeof(Tvr));
	ulAmount = 0;
	ulAmountOther = 0;
	memset(ExpirationDate, '?', sizeof(ExpirationDate));
	memset(Pan, '?', sizeof(Pan));
	ulPanSeq = 0;
	memset(AuthorCode, 0, sizeof(AuthorCode));
	memset(AuthorRespCode, 0, sizeof(AuthorRespCode));
	ulCurrencyCode = 0;
	ulCurrencyExp = 0;
	memset(Date, '?', sizeof(Date));
	memset(Time, '?', sizeof(Time));
	ulType = 0;
	//bForcedOnline = FALSE;
	//ulAvailableOfflineSpendingAmount = 0;
	memset (szSpendingAmount, '\0', sizeof(szSpendingAmount));
	memset (szVisaWaveSpendingAmount, '\0', sizeof(szVisaWaveSpendingAmount));
	memset (ApplicationLabel, '\0', sizeof(ApplicationLabel));
	memset (AppliPreferedName, '\0', sizeof(AppliPreferedName));

	if (pValueStatusCode != NULL)
		GTL_Convert_BinNumberToUl(pValueStatusCode, &ulStatusCode, 2);
	if (pValueTsc != NULL)
		GTL_Convert_DcbNumberToUl(pValueTsc, &ulTsc, LengthTsc);
	if (pValueAid != NULL)
		memcpy(Aid, pValueAid, LengthAid);
	//if (pValueCid != NULL)
	//	Cid = *pValueCid;
	if (pValueCvrResult != NULL)
		memcpy(CvrResult, pValueCvrResult, sizeof(CvrResult));
	if (pValueTvr != NULL)
		memcpy(Tvr, pValueTvr, sizeof(Tvr));
	if (pValueAmountBin != NULL)
		GTL_Convert_BinNumberToUl(pValueAmountBin, &ulAmount, 4);
	if (pValueAmountOtherBin != NULL)
		GTL_Convert_BinNumberToUl(pValueAmountOtherBin, &ulAmountOther, 4);
	if (pValueAmountNum != NULL)
		GTL_Convert_DcbNumberToUl(pValueAmountNum, &ulAmount, 6);
	if (pValueAmountOtherNum != NULL)
		GTL_Convert_DcbNumberToUl(pValueAmountOtherNum, &ulAmountOther, 6);
	if (pValueExpirationDate != NULL)
		GTL_Convert_DcbNumberToAscii(pValueExpirationDate, ExpirationDate, 3, 6);
	if (pValuePan != NULL)
		GTL_Convert_DcbPaddedToAscii(pValuePan, Pan, LengthPan);
	if (pValuePanSeq != NULL)
		GTL_Convert_DcbNumberToUl(pValuePanSeq, &ulPanSeq, 1);
	if (pValueAuthorCode != NULL)
		memcpy(AuthorCode, pValueAuthorCode, 6);
	if (pValueAuthorRespCode != NULL)
		memcpy(AuthorRespCode, pValueAuthorRespCode, 2);
	if (pValueCurrencyCode != NULL)
		GTL_Convert_DcbNumberToUl(pValueCurrencyCode, &ulCurrencyCode, 2);
	if (pValueCurrencyExp != NULL)
		GTL_Convert_DcbNumberToUl(pValueCurrencyExp, &ulCurrencyExp, 1);
	if (pValueDate != NULL)
		GTL_Convert_DcbNumberToAscii(pValueDate, Date, 3, 6);
	if (pValueTime != NULL)
		GTL_Convert_DcbNumberToAscii(pValueTime, Time, 3, 6);
	if (pValueType != NULL)
		GTL_Convert_DcbNumberToUl(pValueType, &ulType, 1);
	//if (pValueForcedOnline != NULL)
	//{
	//	if (*pValueForcedOnline)
	//		bForcedOnline = TRUE;
	//}

	if (pAvailableOfflineSpendingAmount != NULL)
	{
		ClessSampleFormatSpendingAmount(pAvailableOfflineSpendingAmount, length_spending_amount, szSpendingAmount);
	}

	if (pVisaWaveAvailableOfflineSpendingAmount != NULL)
	{
		ClessSampleFormatSpendingAmount(pVisaWaveAvailableOfflineSpendingAmount, length_VisaWave_spending_amount, szVisaWaveSpendingAmount);
	}

	//transactionKind = UNKNOWN_TRANSACTION;
	//if (pValueCardType != NULL)
	//	usCardType = (pValueCardType[0] << 8) + pValueCardType[1];

	if (pValueApplicationLabel != NULL)
		memcpy (ApplicationLabel, pValueApplicationLabel, length_application_label);

	if (pValueAppliPreferedName != NULL)
		memcpy (AppliPreferedName, pValueAppliPreferedName, length_appli_prefered_name);

	// Print a VISA ticket
	if (nTicketType == BATCH_TICKET_VISA)
	{
		__ClessSample_Receipt_FormatAmountNaps(ulAmount, FALSE, ulCurrencyExp, 0, szAmount);

		memset(Pan, '-', 6);
		Pan[strlen(Pan) - 1] = '-';


#ifndef DISABLE_OTHERS_KERNELS
		if(VisaWave_isVisaWaveProcessing())
		{
			if (ClessSample_DumpData_DumpGetOutputId() == CUSTOM_OUTPUT_PRINTER) // Bitmaps only works with a printer
			{
				ClessSample_DumpData("\xF7\xF8");
			}
			else
			{
				ClessSample_DumpData("#       #           #         #                    )");ClessSample_DumpData_DumpNewLine();
				ClessSample_DumpData(" #     #   ###       #   #   #                   )  )");ClessSample_DumpData_DumpNewLine();
				ClessSample_DumpData("  #   #  # #     ###  # # # # ### #   # ##     )  )  )");ClessSample_DumpData_DumpNewLine();
				ClessSample_DumpData("   # #   #   #  #  #   #   # #  #  # # ## #  )  )  ) )");ClessSample_DumpData_DumpNewLine();
				ClessSample_DumpData("    #    # ###   ####  #   #  ####  #   ##     )  )  )");ClessSample_DumpData_DumpNewLine();
				ClessSample_DumpData("                                                 )  )");ClessSample_DumpData_DumpNewLine();
				ClessSample_DumpData("                                                   ) ");ClessSample_DumpData_DumpNewLine();
			}
		}
		else
#endif
		{
			if (ClessSample_DumpData_DumpGetOutputId() == CUSTOM_OUTPUT_PRINTER) // Bitmaps only works with a printer
			{
				ClessSample_DumpData("\xF3\xF4");
			}
			else
			{
				ClessSample_DumpData("               #         #                    )");ClessSample_DumpData_DumpNewLine();
				ClessSample_DumpData("                #   #   #                   )  )");ClessSample_DumpData_DumpNewLine();
				ClessSample_DumpData(" ###   ### #   # # # # # ### #   # ##     )  )  )");ClessSample_DumpData_DumpNewLine();
				ClessSample_DumpData(" #  # #  #  # #   #   # #  #  # # ## #  )  )  ) )");ClessSample_DumpData_DumpNewLine();
				ClessSample_DumpData(" ###   ####  #    #   #  ####  #   ##     )  )  )");ClessSample_DumpData_DumpNewLine();
				ClessSample_DumpData(" #          #                               )  )");ClessSample_DumpData_DumpNewLine();
				ClessSample_DumpData(" #         #                                  ) ");ClessSample_DumpData_DumpNewLine();
			}
		}

		ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData_DumpNewLine();

		if (ClessSample_DumpData_DumpGetOutputId() == CUSTOM_OUTPUT_PRINTER) // Bitmaps only works with a printer
			ClessSample_DumpData ("\xF1\xF2");
		else
			ClessSample_DumpData ("---------------------------");

		ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData_DumpNewLine();

		// Display the application label if available
		if (pValueApplicationLabel != NULL)
		{
			for (temp=0;temp<(unsigned char)length_application_label;temp++)
			{
				if (!(((ApplicationLabel[temp] >= 0x30) && (ApplicationLabel[temp] <= 0x39))
					|| ((ApplicationLabel[temp] >= 0x41) && (ApplicationLabel[temp] <= 0x5A))
					|| ((ApplicationLabel[temp] >= 0x61) && (ApplicationLabel[temp] <= 0x7A))))
				{
					ApplicationLabel[temp] = 0x20;
				}
			}

			ClessSample_DumpData("\x1b" "@");
			ClessSample_DumpData("%s", ApplicationLabel);
			ClessSample_DumpData_DumpNewLine();
		}

		for (temp=0;temp<(unsigned char)LengthAid;temp++)
			ClessSample_DumpData("%02x", Aid[temp]);

		ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData_DumpNewLine();

		ClessSample_DumpData("\x1b" "@\x1b" "E");


		if (ulType == CLESS_SAMPLE_TRANSACTION_TYPE_DEBIT)       // 0x00
		{
			ClessSample_Term_Read_Message(STD_MESS_DEBIT, nCardHolderLang, &tDisplayMsg);
			ClessSample_DumpData(tDisplayMsg.message);
		}
		else if (ulType == CLESS_SAMPLE_TRANSACTION_TYPE_CASH)   // 0x01
		{
			ClessSample_Term_Read_Message(STD_MESS_CASH, nCardHolderLang, &tDisplayMsg);
			ClessSample_DumpData(tDisplayMsg.message);
		}
		else if (ulType == CLESS_SAMPLE_TRANSACTION_TYPE_REFUND) // 0x20
		{
			ClessSample_Term_Read_Message(STD_MESS_REFUND, nCardHolderLang, &tDisplayMsg);
			ClessSample_DumpData(tDisplayMsg.message);
		}
		else
		{
			ClessSample_Term_Read_Message(STD_MESS_TRANSAC_TYPE, nCardHolderLang, &tDisplayMsg);
			ClessSample_DumpData("%s %lX", tDisplayMsg.message, ulType);
		}

		ClessSample_DumpData("\x1b" "@");
		ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData ("%c%c/%c%c/%c%c %c%c:%c%c:%c%c", Date[4], Date[5], Date[2], Date[3], Date[0], Date[1], Time[0], Time[1], Time[2], Time[3], Time[4], Time[5]);
		ClessSample_DumpData_DumpNewLine();

		if (pValuePan != NULL)
		{
			ClessSample_DumpData ("\x1b" "@");
			ClessSample_DumpData ("%s", Pan);
			ClessSample_DumpData_DumpNewLine();
		}
		else
		{
			ClessSample_DumpData_DumpNewLine();
		}

		if (pValueExpirationDate != NULL)
		{
			ClessSample_DumpData ("\x1b" "@");
			ClessSample_DumpData ("%c%c/%c%c", ExpirationDate[2], ExpirationDate[3], ExpirationDate[0], ExpirationDate[1]);
			ClessSample_DumpData_DumpNewLine();
			ClessSample_DumpData_DumpNewLine();
		}
		else
		{
			ClessSample_DumpData_DumpNewLine();
		}

		if (ClessSample_DumpData_DumpGetOutputId() == CUSTOM_OUTPUT_PRINTER)
			ClessSample_DumpData ("\x1b" "@" "\x1b" "E");

        if (nTransactionResult == APPROVED_TICKED)
		    ClessSample_Term_Read_Message(STD_MESS_APPROVED_TICKET, nCardHolderLang, &tDisplayMsg);
		else // (nTransactionResult == DECLINED_TICKED)
		    ClessSample_Term_Read_Message(STD_MESS_DECLINED_TICKET, nCardHolderLang, &tDisplayMsg);
		ClessSample_DumpData (tDisplayMsg.message);

		ClessSample_DumpData ("\x1b" "@");
		ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData_DumpNewLine();
		ClessSample_Term_Read_Message(STD_MESS_AMOUNT, nCardHolderLang, &tDisplayMsg);
		ClessSample_DumpData (tDisplayMsg.message);

		if (ClessSample_DumpData_DumpGetOutputId() == CUSTOM_OUTPUT_PRINTER)
			ClessSample_DumpData ("\x1b" "E");

		ClessSample_DumpData ("%s %s", ClessSample_Fill_GiveMoneyLabel(), szAmount);


		if (ClessSample_DumpData_DumpGetOutputId() == CUSTOM_OUTPUT_PRINTER)
			ClessSample_DumpData ("\x1b" "@");
		ClessSample_DumpData_DumpNewLine();

		if (nAOSA == WITH_AOSA)
		{
#ifndef DISABLE_OTHERS_KERNELS
			if(VisaWave_isVisaWaveProcessing())
			{
				if (pVisaWaveAvailableOfflineSpendingAmount != NULL)
				{
					if (ClessSample_DumpData_DumpGetOutputId() == CUSTOM_OUTPUT_PRINTER)
						ClessSample_DumpData ("\x0F");

				    ClessSample_Term_Read_Message(STD_MESS_AVAILABLE, nCardHolderLang, &tDisplayMsg);
				    ClessSample_DumpData (tDisplayMsg.message);
				    if (ClessSample_DumpData_DumpGetOutputId() == CUSTOM_OUTPUT_PRINTER)
					    ClessSample_DumpData ("\x1b" "@");

				    ClessSample_DumpData ("%s", szVisaWaveSpendingAmount);

				    if (ClessSample_DumpData_DumpGetOutputId() == CUSTOM_OUTPUT_PRINTER)
					    ClessSample_DumpData ("\x1b" "@");
				    ClessSample_DumpData_DumpNewLine();
				}
			}
			else
#endif
			{
				if (pAvailableOfflineSpendingAmount != NULL)
				{
					if (ClessSample_DumpData_DumpGetOutputId() == CUSTOM_OUTPUT_PRINTER)
						ClessSample_DumpData ("\x0F");

					ClessSample_Term_Read_Message(STD_MESS_AVAILABLE, nCardHolderLang, &tDisplayMsg);
					ClessSample_DumpData (tDisplayMsg.message);
					if (ClessSample_DumpData_DumpGetOutputId() == CUSTOM_OUTPUT_PRINTER)
						ClessSample_DumpData ("\x1b" "@");

					ClessSample_DumpData ("%s", szSpendingAmount);

					if (ClessSample_DumpData_DumpGetOutputId() == CUSTOM_OUTPUT_PRINTER)
						ClessSample_DumpData ("\x1b" "@");
					ClessSample_DumpData_DumpNewLine();
				}
			}
		} // else (nAOSA == WITHOUT_AOSA) => no action

		if (bSignatureRequested == WITH_SIGNATURE)
		{
			ClessSample_DumpData_DumpNewLine();

			if (ClessSample_DumpData_DumpGetOutputId() == CUSTOM_OUTPUT_PRINTER) // Bitmaps only works with a printer
			{
				ClessSample_DumpData ("\xF9\xFA");
			}
			else
			{
				ClessSample_DumpData (" *** Signature ******************* ");ClessSample_DumpData_DumpNewLine();
				ClessSample_DumpData ("*                                 *");ClessSample_DumpData_DumpNewLine();
				ClessSample_DumpData ("*                                 *");ClessSample_DumpData_DumpNewLine();
				ClessSample_DumpData ("*                                 *");ClessSample_DumpData_DumpNewLine();
				ClessSample_DumpData (" ********************************* ");
			}
		}

		ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData("Love Every Quick Payment");
		ClessSample_DumpData_DumpNewLine();

		if (ClessSample_DumpData_DumpGetOutputId() == CUSTOM_OUTPUT_PRINTER) // Bitmaps only works with a printer
			ClessSample_DumpData ("\xF1\xF2");
		else
			ClessSample_DumpData ("-------------------------");

		ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData("  And keep in touch !");
		ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData_DumpNewLine();

		if (ClessSample_DumpData_DumpGetOutputId() == CUSTOM_OUTPUT_PRINTER) // Different presentation if on a printer or in text mode
		{
			ClessSample_DumpData("      Ingenico");
			ClessSample_DumpData_DumpNewLine();
			ClessSample_DumpData("\x0F");
			ClessSample_DumpData("           1, rue Claude Chappe BP 346");
			ClessSample_DumpData_DumpNewLine();
			ClessSample_DumpData("\x0F");
			ClessSample_DumpData("      07 503  Guilherand-Granges - FRANCE");
			ClessSample_DumpData_DumpNewLine();
			ClessSample_DumpData("\x0F");
			ClessSample_DumpData("Tel:+33 475 81 40 40        Fax:+33 475 81 43 00");
			ClessSample_DumpData("\x1b" "@");
			ClessSample_DumpData_DumpNewLine();
			ClessSample_DumpData_DumpNewLine();
			ClessSample_DumpData ("\xF1\xF2");
		}
		else
		{
			ClessSample_DumpData("      Ingenico");ClessSample_DumpData_DumpNewLine();
			ClessSample_DumpData("1, rue Claude Chappe BP 346");ClessSample_DumpData_DumpNewLine();
			ClessSample_DumpData(" 07 503 Guilherand-Granges");ClessSample_DumpData_DumpNewLine();
			ClessSample_DumpData("    Tel:+33 475 81 40 40");ClessSample_DumpData_DumpNewLine();
			ClessSample_DumpData("    Fax:+33 475 81 43 00");
			ClessSample_DumpData_DumpNewLine();
			ClessSample_DumpData_DumpNewLine();
			ClessSample_DumpData ("-------------------------");
			Telium_Ttestall (0,5);
		}

		ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData_DumpNewLine();
	}

	// Print a Mastercard ticket
	else if (nTicketType == BATCH_TICKET_MASTERCARD)
	{
		__ClessSample_Receipt_FormatAmountNaps(ulAmount, FALSE, ulCurrencyExp, 0, szAmount);

		memset(Pan, '-', 6);
		Pan[strlen(Pan) - 1] = '-';

		if (ClessSample_DumpData_DumpGetOutputId() == CUSTOM_OUTPUT_PRINTER) // Bitmaps only works with a printer
		{
			ClessSample_DumpData("\xFD\xFE");
		}
		else
		{
			ClessSample_DumpData(" ####            #### ");ClessSample_DumpData_DumpNewLine();
			ClessSample_DumpData(" #   #           #   #");ClessSample_DumpData_DumpNewLine();
			ClessSample_DumpData(" ####  ### #   # ####  ###   ###  ###");ClessSample_DumpData_DumpNewLine();
			ClessSample_DumpData(" #    #  #  # #  #    #  #  #### ####");ClessSample_DumpData_DumpNewLine();
			ClessSample_DumpData(" #     ####  #   #     #### ###  ###");ClessSample_DumpData_DumpNewLine();
			ClessSample_DumpData("            #");ClessSample_DumpData_DumpNewLine();
			ClessSample_DumpData("           #");ClessSample_DumpData_DumpNewLine();
		}

		ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData_DumpNewLine();

		if (ClessSample_DumpData_DumpGetOutputId() == CUSTOM_OUTPUT_PRINTER) // Bitmaps only works with a printer
			ClessSample_DumpData ("\xF1\xF2");
		else
			ClessSample_DumpData ("-------------------------");

		ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData_DumpNewLine();

		ClessSample_DumpData ("\x1b" "@\x1b" "E");

		// Display the application label if available
		if (pValueApplicationLabel != NULL)
		{
			for (temp=0;temp<(unsigned char)length_application_label;temp++)
			{
				if (!(((ApplicationLabel[temp] >= 0x30) && (ApplicationLabel[temp] <= 0x39))
					|| ((ApplicationLabel[temp] >= 0x41) && (ApplicationLabel[temp] <= 0x5A))
					|| ((ApplicationLabel[temp] >= 0x61) && (ApplicationLabel[temp] <= 0x7A))))
				{
					ApplicationLabel[temp] = 0x20;
				}
			}

			ClessSample_DumpData("\x1b" "@");
			ClessSample_DumpData("%s", ApplicationLabel);
			ClessSample_DumpData_DumpNewLine();
		}

		// Print the Application Prefered Name
		ClessSample_DumpData("\x1b" "@");
		ClessSample_DumpData("%s", AppliPreferedName);
		ClessSample_DumpData_DumpNewLine();

		for (temp=0;temp<(unsigned char)LengthAid;temp++)
			ClessSample_DumpData("%02x", Aid[temp]);

		ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData_DumpNewLine();

		ClessSample_DumpData("\x1b" "@\x1b" "E");

		if (ulType == 0) // Debit
		{
			ClessSample_Term_Read_Message(STD_MESS_DEBIT, nCardHolderLang, &tDisplayMsg);
			ClessSample_DumpData(tDisplayMsg.message);
		}
		else if (ulType == 20) // Refund
		{
			ClessSample_Term_Read_Message(STD_MESS_MENU_MAIN_REFUND, nCardHolderLang, &tDisplayMsg);
			ClessSample_DumpData(tDisplayMsg.message);
		}
		else
		{
			ClessSample_Term_Read_Message(STD_MESS_TRANSAC_TYPE, nCardHolderLang, &tDisplayMsg);
			ClessSample_DumpData("%s %lX", tDisplayMsg.message, ulType);
		}

		ClessSample_DumpData ("\x1b" "@");
		ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData ("%c%c/%c%c/%c%c %c%c:%c%c:%c%c", Date[4], Date[5], Date[2], Date[3], Date[0], Date[1], Time[0], Time[1], Time[2], Time[3], Time[4], Time[5]);
		ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData ("%s", Pan);
		ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData ("%c%c/%c%c", ExpirationDate[2], ExpirationDate[3], ExpirationDate[0], ExpirationDate[1]);
		ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData ("\x1b" "E");

		ClessSample_Term_Read_Message(STD_MESS_APPROVED_TICKET, nCardHolderLang, &tDisplayMsg);
		ClessSample_DumpData (tDisplayMsg.message);

		ClessSample_DumpData ("\x1b" "@");
		ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData_DumpNewLine();

		ClessSample_Term_Read_Message(STD_MESS_AMOUNT, nCardHolderLang, &tDisplayMsg);
		ClessSample_DumpData (tDisplayMsg.message);

		if (ClessSample_DumpData_DumpGetOutputId() == CUSTOM_OUTPUT_PRINTER)
			ClessSample_DumpData ("\x1b" "E");

		ClessSample_DumpData ("%s %s", ClessSample_Fill_GiveMoneyLabel(), szAmount);
		ClessSample_DumpData ("\x1b" "@");
		ClessSample_DumpData_DumpNewLine();


		if (bSignatureRequested == WITH_SIGNATURE)
		{
			ClessSample_DumpData_DumpNewLine();

			if (ClessSample_DumpData_DumpGetOutputId() == CUSTOM_OUTPUT_PRINTER) // Bitmaps only works with a printer
			{
				ClessSample_DumpData ("\xF9\xFA");
			}
			else
			{
				ClessSample_DumpData (" *** Signature ******************* ");ClessSample_DumpData_DumpNewLine();
				ClessSample_DumpData ("*                                 *");ClessSample_DumpData_DumpNewLine();
				ClessSample_DumpData ("*                                 *");ClessSample_DumpData_DumpNewLine();
				ClessSample_DumpData ("*                                 *");ClessSample_DumpData_DumpNewLine();
				ClessSample_DumpData (" ********************************* ");
			}
		}

		ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData("        Tap & Go");
		ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData_DumpNewLine();

		if (ClessSample_DumpData_DumpGetOutputId() == CUSTOM_OUTPUT_PRINTER) // Bitmaps only works with a printer
			ClessSample_DumpData ("\xF1\xF2");
		else
			ClessSample_DumpData ("---------------------------");

		ClessSample_DumpData("  Let's keep in touch !");

		ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData_DumpNewLine();

		if (ClessSample_DumpData_DumpGetOutputId() == CUSTOM_OUTPUT_PRINTER) // Different presentation if on a printer or in text mode
		{
			ClessSample_DumpData("       INGENICO");
			ClessSample_DumpData_DumpNewLine();
			ClessSample_DumpData("\x0F");
			ClessSample_DumpData("      190-192, avenue Charles de Gaulle");
			ClessSample_DumpData_DumpNewLine();
			ClessSample_DumpData("\x0F");
			ClessSample_DumpData("       92000 Neuilly-sur-Seine - FRANCE");
			ClessSample_DumpData_DumpNewLine();
			ClessSample_DumpData("\x0F");
			ClessSample_DumpData("Tel:+33 146 25 82 00        Fax:+33 147 72 56 95");
			ClessSample_DumpData("\x1b" "@");
			ClessSample_DumpData_DumpNewLine();
			ClessSample_DumpData_DumpNewLine();
			ClessSample_DumpData ("\xF1\xF2");
		}
		else
		{
			ClessSample_DumpData("       INGENICO");ClessSample_DumpData_DumpNewLine();
			ClessSample_DumpData("190-192, avenue Charles de Gaulle");ClessSample_DumpData_DumpNewLine();
			ClessSample_DumpData(" 92000 Neuilly-sur-Seine");ClessSample_DumpData_DumpNewLine();
			ClessSample_DumpData("   Tel:+33 146 25 82 00");ClessSample_DumpData_DumpNewLine();
			ClessSample_DumpData("   Fax:+33 147 72 56 95");ClessSample_DumpData_DumpNewLine();
			ClessSample_DumpData_DumpNewLine();
			ClessSample_DumpData("---------------------------");
			Telium_Ttestall (0,5);
		}

		ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData_DumpNewLine();
	}
		// Print an ExpressPay ticket
	else if (nTicketType == BATCH_TICKET_AMEX)
	{
		__ClessSample_Receipt_FormatAmountNaps(ulAmount, FALSE, ulCurrencyExp, 0, szAmount);

		memset(Pan, '-', 6);
		Pan[strlen(Pan) - 1] = '-';

		if (ClessSample_DumpData_DumpGetOutputId() == CUSTOM_OUTPUT_PRINTER) // Bitmaps only works with a printer
		{
			ClessSample_DumpData("\xEE\xEF");
		}
		else
		{
			ClessSample_DumpData("  ###             ");ClessSample_DumpData_DumpNewLine();
			ClessSample_DumpData(" #   #              ");ClessSample_DumpData_DumpNewLine();
			ClessSample_DumpData(" #####  ### ### ###  # # ");ClessSample_DumpData_DumpNewLine();
			ClessSample_DumpData(" #   # #  ##  # # #   # ");ClessSample_DumpData_DumpNewLine();
			ClessSample_DumpData(" #   # #  #   #  ### # #");ClessSample_DumpData_DumpNewLine();
		}

		ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData_DumpNewLine();

		if (ClessSample_DumpData_DumpGetOutputId() == CUSTOM_OUTPUT_PRINTER) // Bitmaps only works with a printer
			ClessSample_DumpData ("\xF1\xF2");
		else
			ClessSample_DumpData ("-------------------------");

		ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData_DumpNewLine();

		ClessSample_DumpData ("\x1b" "@\x1b" "E");

		// Display the application label if available
		if (pValueApplicationLabel != NULL)
		{
			for (temp=0;temp<(unsigned char)length_application_label;temp++)
			{
				if (!(((ApplicationLabel[temp] >= 0x30) && (ApplicationLabel[temp] <= 0x39))
					|| ((ApplicationLabel[temp] >= 0x41) && (ApplicationLabel[temp] <= 0x5A))
					|| ((ApplicationLabel[temp] >= 0x61) && (ApplicationLabel[temp] <= 0x7A))))
				{
					ApplicationLabel[temp] = 0x20;
				}
			}

			ClessSample_DumpData("\x1b" "@");
			ClessSample_DumpData("%s", ApplicationLabel);
			ClessSample_DumpData_DumpNewLine();
		}

		for (temp=0;temp<(unsigned char)LengthAid;temp++)
			ClessSample_DumpData("%02x", Aid[temp]);

		ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData_DumpNewLine();

		ClessSample_DumpData("\x1b" "@\x1b" "E");

		if (ulType == 0)
		{
			ClessSample_Term_Read_Message(STD_MESS_DEBIT, nCardHolderLang, &tDisplayMsg);
			ClessSample_DumpData(tDisplayMsg.message);
		}
		else
		{
			ClessSample_Term_Read_Message(STD_MESS_TRANSAC_TYPE, nCardHolderLang, &tDisplayMsg);
			ClessSample_DumpData("%s %lX", tDisplayMsg.message, ulType);
		}

		ClessSample_DumpData ("\x1b" "@");
		ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData ("%c%c/%c%c/%c%c %c%c:%c%c:%c%c", Date[4], Date[5], Date[2], Date[3], Date[0], Date[1], Time[0], Time[1], Time[2], Time[3], Time[4], Time[5]);
		ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData ("%s", Pan);
		ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData ("%c%c/%c%c", ExpirationDate[2], ExpirationDate[3], ExpirationDate[0], ExpirationDate[1]);
		ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData ("\x1b" "E");

		ClessSample_Term_Read_Message(STD_MESS_APPROVED_TICKET, nCardHolderLang, &tDisplayMsg);
		ClessSample_DumpData (tDisplayMsg.message);

		ClessSample_DumpData ("\x1b" "@");
		ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData_DumpNewLine();

		ClessSample_Term_Read_Message(STD_MESS_AMOUNT, nCardHolderLang, &tDisplayMsg);
		ClessSample_DumpData (tDisplayMsg.message);

		if (ClessSample_DumpData_DumpGetOutputId() == CUSTOM_OUTPUT_PRINTER)
			ClessSample_DumpData ("\x1b" "E");

		ClessSample_DumpData ("%s %s", ClessSample_Fill_GiveMoneyLabel(), szAmount);

		ClessSample_DumpData ("\x1b" "@");
		ClessSample_DumpData_DumpNewLine();


		if (bSignatureRequested == WITH_SIGNATURE)
		{
			ClessSample_DumpData_DumpNewLine();

			if (ClessSample_DumpData_DumpGetOutputId() == CUSTOM_OUTPUT_PRINTER) // Bitmaps only works with a printer
			{
				ClessSample_DumpData ("\xF9\xFA");
			}
			else
			{
				ClessSample_DumpData (" *** Signature ******************* ");ClessSample_DumpData_DumpNewLine();
				ClessSample_DumpData ("*                                 *");ClessSample_DumpData_DumpNewLine();
				ClessSample_DumpData ("*                                 *");ClessSample_DumpData_DumpNewLine();
				ClessSample_DumpData ("*                                 *");ClessSample_DumpData_DumpNewLine();
				ClessSample_DumpData (" ********************************* ");
			}
		}

		ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData("  Let's keep in touch !");
		ClessSample_DumpData_DumpNewLine();

		if (ClessSample_DumpData_DumpGetOutputId() == CUSTOM_OUTPUT_PRINTER) // Bitmaps only works with a printer
			ClessSample_DumpData ("\xF1\xF2");
		else
			ClessSample_DumpData ("---------------------------");

		ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData_DumpNewLine();
		if (ClessSample_DumpData_DumpGetOutputId() == CUSTOM_OUTPUT_PRINTER) // Different presentation if on a printer or in text mode
		{
			ClessSample_DumpData("       INGENICO");
			ClessSample_DumpData_DumpNewLine();
			ClessSample_DumpData("\x0F");
			ClessSample_DumpData("      190-192, avenue Charles de Gaulle");
			ClessSample_DumpData_DumpNewLine();
			ClessSample_DumpData("\x0F");
			ClessSample_DumpData("       92000 Neuilly-sur-Seine - FRANCE");
			ClessSample_DumpData_DumpNewLine();
			ClessSample_DumpData("\x0F");
			ClessSample_DumpData("Tel:+33 146 25 82 00        Fax:+33 147 72 56 95");
			ClessSample_DumpData("\x1b" "@");
			ClessSample_DumpData_DumpNewLine();
			ClessSample_DumpData_DumpNewLine();
			ClessSample_DumpData ("\xF1\xF2");
		}
		else
		{
			ClessSample_DumpData("       INGENICO");ClessSample_DumpData_DumpNewLine();
			ClessSample_DumpData("190-192, avenue Charles de Gaulle");ClessSample_DumpData_DumpNewLine();
			ClessSample_DumpData(" 92000 Neuilly-sur-Seine");ClessSample_DumpData_DumpNewLine();
			ClessSample_DumpData("   Tel:+33 146 25 82 00");ClessSample_DumpData_DumpNewLine();
			ClessSample_DumpData("   Fax:+33 147 72 56 95");ClessSample_DumpData_DumpNewLine();
			ClessSample_DumpData_DumpNewLine();
			ClessSample_DumpData("---------------------------");
			Telium_Ttestall (0,5);
		}

		ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData_DumpNewLine();
	}

	// Print a Discover ticket
	else if (nTicketType == BATCH_TICKET_DISCOVER)
	{
		__ClessSample_Receipt_FormatAmountNaps(ulAmount, FALSE, ulCurrencyExp, 0, szAmount);

		memset(Pan, '-', 6);
		Pan[strlen(Pan) - 1] = '-';

		if (ClessSample_DumpData_DumpGetOutputId() == CUSTOM_OUTPUT_PRINTER) // Bitmaps only works with a printer
		{
			ClessSample_DumpData("\xEC\xED");
		}
		else
		{
			ClessSample_DumpData(" #### ");ClessSample_DumpData_DumpNewLine();
			ClessSample_DumpData(" #   #  #");ClessSample_DumpData_DumpNewLine();
			ClessSample_DumpData(" #    #     ##  ##  ##  #  #  ##   ##");ClessSample_DumpData_DumpNewLine();
			ClessSample_DumpData(" #    # #  #   #   #  # #  # #  # #");ClessSample_DumpData_DumpNewLine();
			ClessSample_DumpData(" #    # #  ##  #   #  # #  # ###  #");ClessSample_DumpData_DumpNewLine();
			ClessSample_DumpData(" #   #  #    # #   #  #  ##  #    #");ClessSample_DumpData_DumpNewLine();
			ClessSample_DumpData(" ####   #  ##   ##  ##   ##   ##  #");ClessSample_DumpData_DumpNewLine();
		}

		ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData_DumpNewLine();

		if (ClessSample_DumpData_DumpGetOutputId() == CUSTOM_OUTPUT_PRINTER) // Bitmaps only works with a printer
			ClessSample_DumpData ("\xF1\xF2");
		else
			ClessSample_DumpData ("-------------------------");

		ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData_DumpNewLine();

		ClessSample_DumpData ("\x1b" "@\x1b" "E");

		// Display the application label if available
		if (pValueApplicationLabel != NULL)
		{
			for (temp=0;temp<(unsigned char)length_application_label;temp++)
			{
				if (!(((ApplicationLabel[temp] >= 0x30) && (ApplicationLabel[temp] <= 0x39))
					|| ((ApplicationLabel[temp] >= 0x41) && (ApplicationLabel[temp] <= 0x5A))
					|| ((ApplicationLabel[temp] >= 0x61) && (ApplicationLabel[temp] <= 0x7A))))
				{
					ApplicationLabel[temp] = 0x20;
				}
			}

			ClessSample_DumpData("\x1b" "@");
			ClessSample_DumpData("%s", ApplicationLabel);
			ClessSample_DumpData_DumpNewLine();
		}

		// Print the Application Prefered Name
		ClessSample_DumpData("\x1b" "@");
		ClessSample_DumpData("%s", AppliPreferedName);
		ClessSample_DumpData_DumpNewLine();

		for (temp=0;temp<(unsigned char)LengthAid;temp++)
			ClessSample_DumpData("%02x", Aid[temp]);

		ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData_DumpNewLine();

		ClessSample_DumpData("\x1b" "@\x1b" "E");

		if (ulType == 0)
		{
			ClessSample_Term_Read_Message(STD_MESS_DEBIT, nCardHolderLang, &tDisplayMsg);
			ClessSample_DumpData(tDisplayMsg.message);
		}
		else
		{
			ClessSample_Term_Read_Message(STD_MESS_TRANSAC_TYPE, nCardHolderLang, &tDisplayMsg);
			ClessSample_DumpData("%s %lX", tDisplayMsg.message, ulType);
		}

		ClessSample_DumpData ("\x1b" "@");
		ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData ("%c%c/%c%c/%c%c %c%c:%c%c:%c%c", Date[4], Date[5], Date[2], Date[3], Date[0], Date[1], Time[0], Time[1], Time[2], Time[3], Time[4], Time[5]);
		ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData ("%s", Pan);
		ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData ("%c%c/%c%c", ExpirationDate[2], ExpirationDate[3], ExpirationDate[0], ExpirationDate[1]);
		ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData ("\x1b" "E");

		ClessSample_Term_Read_Message(STD_MESS_APPROVED_TICKET, nCardHolderLang, &tDisplayMsg);
		ClessSample_DumpData (tDisplayMsg.message);

		ClessSample_DumpData ("\x1b" "@");
		ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData_DumpNewLine();

		ClessSample_Term_Read_Message(STD_MESS_AMOUNT, nCardHolderLang, &tDisplayMsg);
		ClessSample_DumpData (tDisplayMsg.message);

		if (ClessSample_DumpData_DumpGetOutputId() == CUSTOM_OUTPUT_PRINTER)
			ClessSample_DumpData ("\x1b" "E");

		ClessSample_DumpData ("%s %s", ClessSample_Fill_GiveMoneyLabel(), szAmount);
		ClessSample_DumpData ("\x1b" "@");
		ClessSample_DumpData_DumpNewLine();


		if (bSignatureRequested)
		{
			ClessSample_DumpData_DumpNewLine();

			if (ClessSample_DumpData_DumpGetOutputId() == CUSTOM_OUTPUT_PRINTER) // Bitmaps only works with a printer
			{
				ClessSample_DumpData ("\xF9\xFA");
			}
			else
			{
				ClessSample_DumpData (" *** Signature ******************* ");ClessSample_DumpData_DumpNewLine();
				ClessSample_DumpData ("*                                 *");ClessSample_DumpData_DumpNewLine();
				ClessSample_DumpData ("*                                 *");ClessSample_DumpData_DumpNewLine();
				ClessSample_DumpData ("*                                 *");ClessSample_DumpData_DumpNewLine();
				ClessSample_DumpData (" ********************************* ");
			}
		}

		ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData("    Have a nice day !");
		ClessSample_DumpData_DumpNewLine();

		if (ClessSample_DumpData_DumpGetOutputId() == CUSTOM_OUTPUT_PRINTER) // Bitmaps only works with a printer
			ClessSample_DumpData ("\xF1\xF2");
		else
			ClessSample_DumpData ("---------------------------");

		ClessSample_DumpData_DumpNewLine();

		if (ClessSample_DumpData_DumpGetOutputId() == CUSTOM_OUTPUT_PRINTER) // Different presentation if on a printer or in text mode
		{
			ClessSample_DumpData("  Let's keep in touch !");
			ClessSample_DumpData_DumpNewLine();
			ClessSample_DumpData_DumpNewLine();
			ClessSample_DumpData("       INGENICO");
			ClessSample_DumpData_DumpNewLine();
			ClessSample_DumpData("\x0F");
			ClessSample_DumpData("      190-192, avenue Charles de Gaulle");
			ClessSample_DumpData_DumpNewLine();
			ClessSample_DumpData("\x0F");
			ClessSample_DumpData("       92000 Neuilly-sur-Seine - FRANCE");
			ClessSample_DumpData_DumpNewLine();
			ClessSample_DumpData("\x0F");
			ClessSample_DumpData("Tel:+33 146 25 82 00        Fax:+33 147 72 56 95");
			ClessSample_DumpData("\x1b" "@");
			ClessSample_DumpData_DumpNewLine();
			ClessSample_DumpData_DumpNewLine();
			ClessSample_DumpData ("\xF1\xF2");
		}
		else
		{
			ClessSample_DumpData("       INGENICO");ClessSample_DumpData_DumpNewLine();
			ClessSample_DumpData("190-192, avenue Charles de Gaulle");ClessSample_DumpData_DumpNewLine();
			ClessSample_DumpData(" 92000 Neuilly-sur-Seine");ClessSample_DumpData_DumpNewLine();
			ClessSample_DumpData("   Tel:+33 146 25 82 00");ClessSample_DumpData_DumpNewLine();
			ClessSample_DumpData("   Fax:+33 147 72 56 95");ClessSample_DumpData_DumpNewLine();
			ClessSample_DumpData_DumpNewLine();
			ClessSample_DumpData("---------------------------");
			Telium_Ttestall (0,5);
		}

		ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData_DumpNewLine();
	}

	// Print a Interac ticket
	else if (nTicketType == BATCH_TICKET_INTERAC)
	{
		__ClessSample_Receipt_FormatAmountNaps(ulAmount, FALSE, ulCurrencyExp, 0, szAmount);

		memset(Pan, '-', 6);
		Pan[strlen(Pan) - 1] = '-';

		if (ClessSample_DumpData_DumpGetOutputId() == CUSTOM_OUTPUT_PRINTER) // Bitmaps only works with a printer
		{
			ClessSample_DumpData("       \xEA\xEB");
		}
		else
		{
			ClessSample_DumpData(" #                            ");ClessSample_DumpData_DumpNewLine();
			ClessSample_DumpData(" #       #                    ");ClessSample_DumpData_DumpNewLine();
			ClessSample_DumpData(" #      ###                   ");ClessSample_DumpData_DumpNewLine();
			ClessSample_DumpData(" # ###   #    ##  ##  ##   ## ");ClessSample_DumpData_DumpNewLine();
			ClessSample_DumpData(" # #  #  #   # # #   #  # #   ");ClessSample_DumpData_DumpNewLine();
			ClessSample_DumpData(" # #  #  # # ##  #   #  # #   ");ClessSample_DumpData_DumpNewLine();
			ClessSample_DumpData(" # #  #  ##   ## #    ###  ## ");ClessSample_DumpData_DumpNewLine();
		}

		ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData_DumpNewLine();

		if (ClessSample_DumpData_DumpGetOutputId() == CUSTOM_OUTPUT_PRINTER) // Bitmaps only works with a printer
			ClessSample_DumpData ("\xF1\xF2");
		else
			ClessSample_DumpData ("-------------------------");

		ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData_DumpNewLine();

		ClessSample_DumpData ("\x1b" "@\x1b" "E");

		// Display the application label if available
		if (pValueApplicationLabel != NULL)
		{
			for (temp=0;temp<(unsigned char)length_application_label;temp++)
			{
				if (!(((ApplicationLabel[temp] >= 0x30) && (ApplicationLabel[temp] <= 0x39))
					|| ((ApplicationLabel[temp] >= 0x41) && (ApplicationLabel[temp] <= 0x5A))
					|| ((ApplicationLabel[temp] >= 0x61) && (ApplicationLabel[temp] <= 0x7A))))
				{
					ApplicationLabel[temp] = 0x20;
				}
			}

			ClessSample_DumpData("\x1b" "@");
			ClessSample_DumpData("%s", ApplicationLabel);
			ClessSample_DumpData_DumpNewLine();
		}

		// Print the Application Prefered Name
		ClessSample_DumpData("\x1b" "@");
		ClessSample_DumpData("%s", AppliPreferedName);
		ClessSample_DumpData_DumpNewLine();

		for (temp=0;temp<(unsigned char)LengthAid;temp++)
			ClessSample_DumpData("%02x", Aid[temp]);

		ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData_DumpNewLine();

		ClessSample_DumpData("\x1b" "@\x1b" "E");

		if (ulType == 0)
		{
			ClessSample_Term_Read_Message(STD_MESS_DEBIT, nCardHolderLang, &tDisplayMsg);
			ClessSample_DumpData(tDisplayMsg.message);
		}
		else
		{
			ClessSample_Term_Read_Message(STD_MESS_TRANSAC_TYPE, nCardHolderLang, &tDisplayMsg);
			ClessSample_DumpData("%s %lX", tDisplayMsg.message, ulType);
		}

		ClessSample_DumpData ("\x1b" "@");
		ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData ("%c%c/%c%c/%c%c %c%c:%c%c:%c%c", Date[4], Date[5], Date[2], Date[3], Date[0], Date[1], Time[0], Time[1], Time[2], Time[3], Time[4], Time[5]);
		ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData ("%s", Pan);
		ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData ("%c%c/%c%c", ExpirationDate[2], ExpirationDate[3], ExpirationDate[0], ExpirationDate[1]);
		ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData ("\x1b" "E");

		ClessSample_Term_Read_Message(STD_MESS_APPROVED_TICKET, nCardHolderLang, &tDisplayMsg);
		ClessSample_DumpData (tDisplayMsg.message);

		ClessSample_DumpData ("\x1b" "@");
		ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData_DumpNewLine();

		ClessSample_Term_Read_Message(STD_MESS_AMOUNT, nCardHolderLang, &tDisplayMsg);
		ClessSample_DumpData (tDisplayMsg.message);

		if (ClessSample_DumpData_DumpGetOutputId() == CUSTOM_OUTPUT_PRINTER)
			ClessSample_DumpData ("\x1b" "E");

		ClessSample_DumpData ("%s %s", ClessSample_Fill_GiveMoneyLabel(), szAmount);
		ClessSample_DumpData ("\x1b" "@");
		ClessSample_DumpData_DumpNewLine();


		if (bSignatureRequested)
		{
			ClessSample_DumpData_DumpNewLine();

			if (ClessSample_DumpData_DumpGetOutputId() == CUSTOM_OUTPUT_PRINTER) // Bitmaps only works with a printer
			{
				ClessSample_DumpData ("\xF9\xFA");
			}
			else
			{
				ClessSample_DumpData (" *** Signature ******************* ");ClessSample_DumpData_DumpNewLine();
				ClessSample_DumpData ("*                                 *");ClessSample_DumpData_DumpNewLine();
				ClessSample_DumpData ("*                                 *");ClessSample_DumpData_DumpNewLine();
				ClessSample_DumpData ("*                                 *");ClessSample_DumpData_DumpNewLine();
				ClessSample_DumpData (" ********************************* ");
			}
		}

		ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData("Contactless in a Flash !");
		ClessSample_DumpData_DumpNewLine();

		if (ClessSample_DumpData_DumpGetOutputId() == CUSTOM_OUTPUT_PRINTER) // Bitmaps only works with a printer
			ClessSample_DumpData ("\xF1\xF2");
		else
			ClessSample_DumpData ("---------------------------");

		ClessSample_DumpData_DumpNewLine();

		if (ClessSample_DumpData_DumpGetOutputId() == CUSTOM_OUTPUT_PRINTER) // Different presentation if on a printer or in text mode
		{
			ClessSample_DumpData("  Let's keep in touch !");
			ClessSample_DumpData_DumpNewLine();
			ClessSample_DumpData_DumpNewLine();
    		ClessSample_DumpData("       INGENICO");
			ClessSample_DumpData_DumpNewLine();
			ClessSample_DumpData("\x0F");
			ClessSample_DumpData("     190-192, avenue Charles de Gaulle");
			ClessSample_DumpData_DumpNewLine();
			ClessSample_DumpData("\x0F");
			ClessSample_DumpData("      92000 Neuilly-sur-Seine - FRANCE");
			ClessSample_DumpData_DumpNewLine();
			ClessSample_DumpData("\x0F");
			ClessSample_DumpData("Tel:+33 146 25 82 00        Fax:+33 147 72 56 95");
			ClessSample_DumpData("\x1b" "@");
			ClessSample_DumpData_DumpNewLine();
			ClessSample_DumpData_DumpNewLine();
			ClessSample_DumpData ("\xF1\xF2");
		}
		else
		{
			ClessSample_DumpData("       INGENICO");ClessSample_DumpData_DumpNewLine();
			ClessSample_DumpData("190-192, avenue Charles de Gaulle");ClessSample_DumpData_DumpNewLine();
			ClessSample_DumpData(" 92000 Neuilly-sur-Seine");ClessSample_DumpData_DumpNewLine();
			ClessSample_DumpData("   Tel:+33 146 25 82 00");ClessSample_DumpData_DumpNewLine();
			ClessSample_DumpData("   Fax:+33 147 72 56 95");ClessSample_DumpData_DumpNewLine();
			ClessSample_DumpData_DumpNewLine();
			ClessSample_DumpData("---------------------------");
			Telium_Ttestall (0,5);
		}

		ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData_DumpNewLine();

		// output the TVR
		/*ClessSample_DumpData("TVR: ");

		for (temp=0;temp<5;temp++)
			ClessSample_DumpData("%02x", Tvr[temp]);*/

		ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData_DumpNewLine();
	}

	ClessSample_DumpData_DumpCloseOutputDriver();
}



void ClessSample_Receipt_PrintPayPass (T_SHARED_DATA_STRUCT * pTransaction, const int bMobileTransaction, const int nRequiredCvm, int nCardHolderLang)
{
	int nResult, nPosition;
	unsigned long ulTag;
	unsigned long ulReadLength;
	const unsigned char * pValue;
	const unsigned char* pValueAid;
	unsigned int LengthAid;
	const unsigned char* pValueTvr;
	const unsigned char* pValueAmountNum;
	const unsigned char* pValueAmountOtherNum;
	const unsigned char* pValueExpirationDate;
	const unsigned char* pValuePan;
	unsigned int LengthPan;
	const unsigned char* pValuePanSeq;
	const unsigned char* pValueCurrencyCode;
	const unsigned char* pValueCurrencyExp;
	const unsigned char* pValueDate;
	const unsigned char* pValueType;
	//const unsigned char* pValueCardType;
	const unsigned char* pValueApplicationLabel;
	unsigned int length_application_label;
	const unsigned char* pValueAppliPreferedName;
	unsigned int length_appli_prefered_name;

	unsigned char Aid[20];
	unsigned char Tvr[5];
	unsigned long ulAmount;
	unsigned long ulAmountOther;
	char ExpirationDate[6 + 1];
	char Pan[19 + 1];
	unsigned long ulPanSeq;
	unsigned long ulCurrencyCode;
	unsigned long ulCurrencyExp;
	char Date[6 + 1];
	unsigned long ulType;
	char szAmount[20];
	//unsigned short usCardType;
	unsigned char ApplicationLabel [16 + 1];  // Application label + '\0'
	unsigned char AppliPreferedName [16 + 1]; // Application prefered name + '\0'
	int temp;
	MSGinfos tDisplayMsg;
	


	Remplir_Infopayment_Cless (pTransaction, nCardHolderLang);
	//valide_carte(); //KamalCless
	//MessageWithDelay("before Retour_Caisse", NULL, 1000);

	Retour_Caisse("000");
	return;


	// Open output device
	if (!ClessSample_DumpData_DumpOpenOutputDriver())
	{
		APEMV_UI_MessageDisplayDelay ("\nOUTINO\nOUTINO\nOUTINO", 5);
		return;
	}

	pValueAid = NULL;
	LengthAid = 0;
	pValueTvr = NULL;
	pValueAmountNum = NULL;
	pValueAmountOtherNum = NULL;
	pValueExpirationDate = NULL;
	pValuePan = NULL;
	LengthPan = 0;
	pValuePanSeq = NULL;
	pValueCurrencyCode = NULL;
	pValueCurrencyExp = NULL;
	pValueDate = NULL;
	pValueType = NULL;
	//pValueCardType = NULL;
	//usCardType = 0;
	pValueApplicationLabel = NULL;
	length_application_label = 0;
	pValueAppliPreferedName = NULL;
	length_appli_prefered_name = 0;

	//Telium_DefprinterpatternHR (0, 0, 0xF1, (char *) Line_part_1);
	//Telium_DefprinterpatternHR (0, 0, 0xF2, (char *) Line_part_2);
	//Telium_DefprinterpatternHR (0, 0, 0xF9, (char *) Signature_part_1);
	//Telium_DefprinterpatternHR (0, 0, 0xFA, (char *) Signature_part_2);
	//Telium_DefprinterpatternHR (0, 0, 0xE8, (char *) MasterCardPayPass_Part1);
	//Telium_DefprinterpatternHR (0, 0, 0xE9, (char *) MasterCardPayPass_Part2);

	nPosition = SHARED_EXCHANGE_POSITION_NULL;
	
	do{
    	// Try to following tag.
    	nResult = GTL_SharedExchange_GetNext (pTransaction, &nPosition, &ulTag, &ulReadLength, &pValue);

        if (nResult == STATUS_SHARED_EXCHANGE_OK)  // If tag found
        {
			switch(ulTag)
			{
    		case TAG_EMV_DF_NAME:							pValueAid = pValue;	LengthAid = ulReadLength; break;
    		case TAG_EMV_TVR:								pValueTvr = pValue; break;
    		case TAG_EMV_AMOUNT_AUTH_NUM:					pValueAmountNum = pValue; break;
    		case TAG_EMV_AMOUNT_OTHER_NUM:					pValueAmountOtherNum = pValue; break;
    		case TAG_EMV_APPLI_EXPIRATION_DATE:				pValueExpirationDate = pValue; break;
    		case TAG_EMV_APPLI_PAN:							pValuePan = pValue;	LengthPan = ulReadLength; break;
    		case TAG_EMV_APPLI_PAN_SEQUENCE_NUMBER:			pValuePanSeq = pValue; break;
    		case TAG_EMV_TRANSACTION_CURRENCY_CODE:			pValueCurrencyCode = pValue; break;
    		case TAG_EMV_TRANSACTION_CURRENCY_EXPONENT:		pValueCurrencyExp = pValue; break;
    		case TAG_EMV_TRANSACTION_DATE:					pValueDate = pValue; break;
    		case TAG_EMV_TRANSACTION_TYPE:					pValueType = pValue; break;
    		//case TAG_KERNEL_CARD_TYPE:						pValueCardType=pValue; break;
    		case TAG_EMV_APPLICATION_LABEL:					pValueApplicationLabel=pValue; length_application_label = ulReadLength; break;
    		case TAG_EMV_APPLI_PREFERED_NAME:				pValueAppliPreferedName=pValue; length_appli_prefered_name = ulReadLength; break;
			default:
				break;
			}
        }
        else if (nResult != STATUS_SHARED_EXCHANGE_END)  // Error
        {
        	GTL_Traces_TraceDebug("Display tags error:%x\n", nResult);
        }
    }while (nResult == STATUS_SHARED_EXCHANGE_OK);


	memset(Aid, 0, sizeof(Aid));
	memset(Tvr, 0, sizeof(Tvr));
	ulAmount = 0;
	ulAmountOther = 0;
	memset(ExpirationDate, '?', sizeof(ExpirationDate));
	memset(Pan, '?', sizeof(Pan));
	Pan[sizeof(Pan) - 1] = '\0';
	ulPanSeq = 0;
	ulCurrencyCode = 0;
	ulCurrencyExp = 0;
	memset(Date, '?', sizeof(Date));
	ulType = 0;
	memset (ApplicationLabel, '\0', sizeof(ApplicationLabel));
	memset (AppliPreferedName, '\0', sizeof(AppliPreferedName));

	if (pValueAid != NULL)
		memcpy(Aid, pValueAid, LengthAid);
	if (pValueTvr != NULL)
		memcpy(Tvr, pValueTvr, sizeof(Tvr));
	if (pValueAmountNum != NULL)
		GTL_Convert_DcbNumberToUl(pValueAmountNum, &ulAmount, 6);
	if (pValueAmountOtherNum != NULL)
		GTL_Convert_DcbNumberToUl(pValueAmountOtherNum, &ulAmountOther, 6);
	if (pValueExpirationDate != NULL)
		GTL_Convert_DcbNumberToAscii(pValueExpirationDate, ExpirationDate, 3, 6);
	if (pValuePan != NULL)
		GTL_Convert_DcbPaddedToAscii(pValuePan, Pan, LengthPan);
	if (pValuePanSeq != NULL)
		GTL_Convert_DcbNumberToUl(pValuePanSeq, &ulPanSeq, 1);
	if (pValueCurrencyCode != NULL)
		GTL_Convert_DcbNumberToUl(pValueCurrencyCode, &ulCurrencyCode, 2);
	if (pValueCurrencyExp != NULL)
		GTL_Convert_DcbNumberToUl(pValueCurrencyExp, &ulCurrencyExp, 1);
	if (pValueDate != NULL)
		GTL_Convert_DcbNumberToAscii(pValueDate, Date, 3, 6);
	if (pValueType != NULL)
		ulType = pValueType[0];
	//if (pValueCardType != NULL)
	//	usCardType = (pValueCardType[0] << 8) + pValueCardType[1];
	if (pValueApplicationLabel != NULL)
		memcpy (ApplicationLabel, pValueApplicationLabel, length_application_label);
	if (pValueAppliPreferedName != NULL)
		memcpy (AppliPreferedName, pValueAppliPreferedName, length_appli_prefered_name);

	// Print the receipt
	__ClessSample_Receipt_FormatAmountNaps(ulAmount, FALSE, ulCurrencyExp, 0, szAmount);

	// Format the PAN
	{
		int nLen;

		nLen = strlen(Pan);
		if (nLen)
		{
			Pan[nLen - 1] = '-';
			if (nLen > 6)
				memset(Pan, '-', 6);
			else
				memset(Pan, '-', nLen-1);
		}
	}

	// Print the logo
	if (ClessSample_DumpData_DumpGetOutputId() == CUSTOM_OUTPUT_PRINTER) // Bitmaps only works with a printer
	{
		ClessSample_DumpData("\xE8\xE9");
	}
	else
	{
		ClessSample_DumpData(" ####            #### ");ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData(" #   #           #   #");ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData(" ####  ### #   # ####  ###   ###  ###");ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData(" #    #  #  # #  #    #  #  #### ####");ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData(" #     ####  #   #     #### ###  ###");ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData("            #");ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData("           #");ClessSample_DumpData_DumpNewLine();
	}
			
	ClessSample_DumpData_DumpNewLine();
	ClessSample_DumpData_DumpNewLine();

	// Print separator line
	ClessSample_DumpData_DumpNewLine();
	if (ClessSample_DumpData_DumpGetOutputId() == CUSTOM_OUTPUT_PRINTER) // Bitmaps only works with a printer
		ClessSample_DumpData ("\xF1\xF2");
	else
		ClessSample_DumpData ("-------------------------");
	ClessSample_DumpData_DumpNewLine();
	ClessSample_DumpData_DumpNewLine();

	// Print receipt depending on the action
	ClessSample_DumpData ("\x1b" "@\x1b" "E");

	// Display the application label if available
	if (pValueApplicationLabel != NULL)
	{
		for (temp=0;temp<(unsigned char)length_application_label;temp++)
		{
			if (!(((ApplicationLabel[temp] >= 0x30) && (ApplicationLabel[temp] <= 0x39))
				|| ((ApplicationLabel[temp] >= 0x41) && (ApplicationLabel[temp] <= 0x5A))
				|| ((ApplicationLabel[temp] >= 0x61) && (ApplicationLabel[temp] <= 0x7A))))
			{
				ApplicationLabel[temp] = 0x20;
			}
		}

		ClessSample_DumpData("\x1b" "@");
		ClessSample_DumpData("%s", ApplicationLabel);
		ClessSample_DumpData_DumpNewLine();
	}

	// Display the application preferred name if available
	if (pValueAppliPreferedName != NULL)
	{
		for (temp=0;temp<(unsigned char)length_appli_prefered_name;temp++)
		{
			if (!(((AppliPreferedName[temp] >= 0x30) && (AppliPreferedName[temp] <= 0x39))
				|| ((AppliPreferedName[temp] >= 0x41) && (AppliPreferedName[temp] <= 0x5A))
				|| ((AppliPreferedName[temp] >= 0x61) && (AppliPreferedName[temp] <= 0x7A))))
			{
				AppliPreferedName[temp] = 0x20;
			}
		}

		ClessSample_DumpData("\x1b" "@");
		ClessSample_DumpData("%s", AppliPreferedName);
		ClessSample_DumpData_DumpNewLine();
	}

	ClessSample_DumpData_DumpNewLine();

	ClessSample_DumpData("\x1b" "@\x1b" "E");

	if (ulType == CLESS_SAMPLE_TRANSACTION_TYPE_DEBIT)
	{
		ClessSample_Term_Read_Message(STD_MESS_DEBIT, nCardHolderLang, &tDisplayMsg);
		ClessSample_DumpData(tDisplayMsg.message);
	}
	else if (ulType == CLESS_SAMPLE_TRANSACTION_TYPE_REFUND)
	{
		ClessSample_Term_Read_Message(STD_MESS_REFUND, nCardHolderLang, &tDisplayMsg);
		ClessSample_DumpData(tDisplayMsg.message);
	}
	else
	{
		ClessSample_Term_Read_Message(STD_MESS_TRANSAC_TYPE, nCardHolderLang, &tDisplayMsg);
		ClessSample_DumpData("%s %lX", tDisplayMsg.message, ulType);
	}

	ClessSample_DumpData_DumpNewLine();
	ClessSample_Term_Read_Message(STD_MESS_APPROVED_TICKET, nCardHolderLang, &tDisplayMsg);
	ClessSample_DumpData (tDisplayMsg.message);
	ClessSample_DumpData ("\x1b" "@");

	ClessSample_DumpData_DumpNewLine();
	ClessSample_DumpData_DumpNewLine();

	ClessSample_DumpData ("%c%c/%c%c/%c%c", Date[4], Date[5], Date[2], Date[3], Date[0], Date[1]);
	ClessSample_DumpData_DumpNewLine();

	// If the PAN is present, print it (with hidden parts)
	if (pValuePan != NULL)
	{
		ClessSample_DumpData ("%s", Pan);
		ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData_DumpNewLine();
	}
		
	ClessSample_Term_Read_Message(STD_MESS_AMOUNT, nCardHolderLang, &tDisplayMsg);
	ClessSample_DumpData (tDisplayMsg.message);
		
	if (ClessSample_DumpData_DumpGetOutputId() == CUSTOM_OUTPUT_PRINTER)
		ClessSample_DumpData ("\x1b" "E");

	ClessSample_DumpData ("%s %s", ClessSample_Fill_GiveMoneyLabel(), szAmount);
	ClessSample_DumpData ("\x1b" "@");
	ClessSample_DumpData_DumpNewLine();

	// Check if signature is required
	if (nRequiredCvm == PAYPASS_OPS_CVM_SIGNATURE)
	{
		ClessSample_DumpData_DumpNewLine();

		if (ClessSample_DumpData_DumpGetOutputId() == CUSTOM_OUTPUT_PRINTER) // Bitmaps only works with a printer
		{
			ClessSample_DumpData ("\xF9\xFA");
			ClessSample_DumpData_DumpNewLine();
		}
		else
		{
			ClessSample_DumpData (" *** Signature ******************* ");ClessSample_DumpData_DumpNewLine();
			ClessSample_DumpData ("*                                 *");ClessSample_DumpData_DumpNewLine();
			ClessSample_DumpData ("*                                 *");ClessSample_DumpData_DumpNewLine();
			ClessSample_DumpData ("*                                 *");ClessSample_DumpData_DumpNewLine();
			ClessSample_DumpData (" ********************************* ");ClessSample_DumpData_DumpNewLine();
		}
	}

	ClessSample_DumpData_DumpNewLine();

	if (bMobileTransaction)
	{
		ClessSample_Term_Read_Message(STD_MESS_RECEIPT_TXN_WITH_MOBILE, nCardHolderLang, &tDisplayMsg);
		ClessSample_DumpData (tDisplayMsg.message);

		ClessSample_DumpData("\x0F");

		if (nRequiredCvm == PAYPASS_OPS_CVM_CONFIRMATION_CODE_VERIFIED)
			ClessSample_Term_Read_Message(STD_MESS_RECEIPT_PIN_VERIFIED, nCardHolderLang, &tDisplayMsg);
		else
			ClessSample_Term_Read_Message(STD_MESS_RECEIPT_NO_CARDHOLDER_VERIFICATION, nCardHolderLang, &tDisplayMsg);

		ClessSample_DumpData (tDisplayMsg.message);

		ClessSample_DumpData("\x1b" "@");
	}
	else
	{
		ClessSample_Term_Read_Message(STD_MESS_RECEIPT_TXN_WITH_CARD, nCardHolderLang, &tDisplayMsg);
		ClessSample_DumpData (tDisplayMsg.message);
	}


	// Print separator line
	ClessSample_DumpData_DumpNewLine();
	ClessSample_DumpData_DumpNewLine();
	if (ClessSample_DumpData_DumpGetOutputId() == CUSTOM_OUTPUT_PRINTER) // Bitmaps only works with a printer
		ClessSample_DumpData ("\xF1\xF2");
	else
		ClessSample_DumpData ("-------------------------");
	ClessSample_DumpData_DumpNewLine();
	ClessSample_DumpData_DumpNewLine();

	ClessSample_DumpData("  Let's keep in touch !");

	ClessSample_DumpData_DumpNewLine();
	ClessSample_DumpData_DumpNewLine();

	if (ClessSample_DumpData_DumpGetOutputId() == CUSTOM_OUTPUT_PRINTER) // Different presentation if on a printer or in text mode
	{
		ClessSample_DumpData("       INGENICO");
		ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData("\x0F");
		ClessSample_DumpData("      190-192, avenue Charles de Gaulle");
		ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData("\x0F");
		ClessSample_DumpData("       92000 Neuilly-sur-Seine - FRANCE");
		ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData("\x0F");
		ClessSample_DumpData("Tel:+33 146 25 82 00        Fax:+33 147 72 56 95");
		ClessSample_DumpData("\x1b" "@");
		ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData ("\xF1\xF2");
	}
	else
	{
		ClessSample_DumpData("       INGENICO");ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData("190-192, avenue Charles de Gaulle");ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData(" 92000 Neuilly-sur-Seine");ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData("   Tel:+33 146 25 82 00");ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData("   Fax:+33 147 72 56 95");ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData_DumpNewLine();
		ClessSample_DumpData("---------------------------");
		Telium_Ttestall (0,5);
	}

	ClessSample_DumpData_DumpNewLine();
	ClessSample_DumpData_DumpNewLine();
	ClessSample_DumpData_DumpNewLine();
	ClessSample_DumpData_DumpNewLine();
	ClessSample_DumpData_DumpNewLine();
	ClessSample_DumpData_DumpNewLine();

	ClessSample_DumpData_DumpCloseOutputDriver();

}

//! \brief Prints the receipt for C2 kernel transactions.
//! \param[in] pTransaction Structure containing the data returned by the C2 kernel.
//! \param[in] bMobileTransaction Indicates if the transaction has bee performed with a mobile or not.
//! \param[in] nRequiredCvm Indicates the required CVM.
//! \param[in] nCardHolderLang Indicates the cardholder language.

void Remplir_Infopayment_Cless (T_SHARED_DATA_STRUCT * pTransaction, int nCardHolderLang)
{
	int nResult, nPosition;
	unsigned long ulTag;
	unsigned long ulReadLength;

	const unsigned char * pValue;

	const unsigned char* pValueAid;
	unsigned int LengthAid;

	const unsigned char* pValueTvr;
	const unsigned char* pValueAmountNum;
	const unsigned char* pValueAmountOtherNum;
	const unsigned char* pValueExpirationDate;

	const unsigned char* pValuePan;
	unsigned int LengthPan;

	const unsigned char* pValuePanSeq;
	const unsigned char* pValueCurrencyCode;
	const unsigned char* pValueCurrencyExp;
	const unsigned char* pValueDate;
	const unsigned char* pValueType;
	//const unsigned char* pValueCardType;

	const unsigned char* pValueApplicationLabel;
	unsigned int length_application_label;

	const unsigned char* pValueAppliPreferedName;
	unsigned int length_appli_prefered_name;

	const unsigned char* pValueTime;
	unsigned int length_time;

	const unsigned char* pValueTrack2;
	unsigned int length_Track2;

	unsigned char Aid[20];
	unsigned char Tvr[5];
	unsigned long ulAmount;
	unsigned long ulAmountOther;
	char ExpirationDate[6 + 1];
	char Pan[19 + 1];
	unsigned long ulPanSeq;
	unsigned long ulCurrencyCode;
	unsigned long ulCurrencyExp;
	char Date[6 + 1];
	unsigned long ulType;
	char szAmount[20];
	//unsigned short usCardType;
	unsigned char ApplicationLabel [16 + 1]; // Application label + '\0'
	unsigned char AppliPreferedName [16 + 1]; // Application prefered name + '\0'

	char Time[6 + 1];

	Telium_Date_t Datesys;//sss


//	int temp;
//	MSGinfos tDisplayMsg;


	uint8	l_champs55;
	uint8	champs55[255];

	UC		Lg,i;
	UC		Card_Holder_Nbre_ISO2[21],Expired_Date_ISO2[5];
	UC		l_c_Track2[40];

	//APEMV_UI_MessageDisplayDelay ("\nREMPLIR\nCLESS\nSTRUCTINFOPAYMENT", 1);


	Remplir_Infopayment_Cless_VISA (pTransaction, nCardHolderLang);
	return;

	pValueAid = NULL;
	LengthAid = 0;
	pValueTvr = NULL;
	pValueAmountNum = NULL;
	pValueAmountOtherNum = NULL;
	pValueExpirationDate = NULL;
	pValuePan = NULL;
	LengthPan = 0;
	pValuePanSeq = NULL;
	pValueCurrencyCode = NULL;
	pValueCurrencyExp = NULL;
	pValueDate = NULL;
	pValueType = NULL;
	//pValueCardType = NULL;
	//usCardType = 0;
	pValueApplicationLabel = NULL;
	length_application_label = 0;
	pValueAppliPreferedName = NULL;
	length_appli_prefered_name = 0;

	pValueTime = NULL;
	length_time = 0;

	pValueTrack2 = NULL;
	length_Track2 = 0;

	nPosition = SHARED_EXCHANGE_POSITION_NULL;

	do{
    	// Try to following tag.
    	nResult = GTL_SharedExchange_GetNext (pTransaction, &nPosition, &ulTag, &ulReadLength, &pValue);

        if (nResult == STATUS_SHARED_EXCHANGE_OK)  // If tag found
        {
			switch(ulTag)
			{
    		case TAG_EMV_AID_CARD:							pValueAid = pValue;	LengthAid = ulReadLength; break;
    		case TAG_EMV_TVR:								pValueTvr = pValue; break;
    		case TAG_EMV_AMOUNT_AUTH_NUM:					pValueAmountNum = pValue; break;
    		case TAG_EMV_AMOUNT_OTHER_NUM:					pValueAmountOtherNum = pValue; break;
    		case TAG_EMV_APPLI_EXPIRATION_DATE:				pValueExpirationDate = pValue; break;
    		case TAG_EMV_APPLI_PAN:							pValuePan = pValue;	LengthPan = ulReadLength; break;
    		case TAG_EMV_APPLI_PAN_SEQUENCE_NUMBER:			pValuePanSeq = pValue; break;
    		case TAG_EMV_TRANSACTION_CURRENCY_CODE:			pValueCurrencyCode = pValue; break;
    		case TAG_EMV_TRANSACTION_CURRENCY_EXPONENT:		pValueCurrencyExp = pValue; break;
    		case TAG_EMV_TRANSACTION_DATE:					pValueDate = pValue; break;
    		case TAG_EMV_TRANSACTION_TYPE:					pValueType = pValue; break;
    		//case TAG_KERNEL_CARD_TYPE:						pValueCardType=pValue; break;
    		case TAG_EMV_APPLICATION_LABEL:					pValueApplicationLabel=pValue; length_application_label = ulReadLength; break;
    		case TAG_EMV_CARDHOLDER_NAME:					pValueAppliPreferedName=pValue; length_appli_prefered_name = ulReadLength; break;
    		case TAG_EMV_TRANSACTION_TIME:					pValueTime=pValue; length_time = ulReadLength; break;
    		case TAG_EMV_TRACK_2_EQU_DATA:					pValueTrack2=pValue;length_Track2=ulReadLength; break;
			default:
				break;
			}
        }
        else if (nResult != STATUS_SHARED_EXCHANGE_END)  // Error
        {
        	GTL_Traces_TraceDebug("Display tags error:%x\n", nResult);
        }
    }while (nResult == STATUS_SHARED_EXCHANGE_OK);


	memset(Aid, 0, sizeof(Aid));
	memset(Tvr, 0, sizeof(Tvr));
	ulAmount = 0;
	ulAmountOther = 0;
	memset(ExpirationDate, '?', sizeof(ExpirationDate));
	memset(Pan, '?', sizeof(Pan));
	Pan[sizeof(Pan) - 1] = '\0';
	ulPanSeq = 0;
	ulCurrencyCode = 0;
	ulCurrencyExp = 0;
	memset(Date, '?', sizeof(Date));
	ulType = 0;
	memset (ApplicationLabel, '\0', sizeof(ApplicationLabel));
	memset (AppliPreferedName, '\0', sizeof(AppliPreferedName));

	if (pValueAid != NULL)
	{
		champs55[l_champs55++] =TAG_EMV_DF_NAME;
		champs55[l_champs55++] =(UC)LengthAid;
		memcpy(&champs55[l_champs55],pValueAid,LengthAid);
		l_champs55 += LengthAid;

		GTL_Convert_DcbNumberToAscii(pValueAid, Aid, LengthAid, 2*LengthAid);
		memcpy(StructInfoPayment.Appli_aid, Aid, 2*LengthAid);
//		Os__xprintaa("001");
//	    Os__xprintd(StructInfoPayment.Appli_aid);
	}
	if (pValueTvr != NULL)
	{
		champs55[l_champs55++] =TAG_EMV_TVR;
		champs55[l_champs55++] =(UC)0x05;
		memcpy(&champs55[l_champs55],pValueTvr,0x05);
		l_champs55 += 0x05;

		memcpy(Tvr, pValueTvr, sizeof(Tvr));
		//memcpy(StructInfoPayment.t, pValueTvr, sizeof(Tvr));
	}
	if (pValueAmountNum != NULL)
	{
		champs55[l_champs55++] =0x9F;
		champs55[l_champs55++] =0x02;
		champs55[l_champs55++] =(UC)0x06;
		memcpy(&champs55[l_champs55],pValueAmountNum,0x06);
		l_champs55 += 0x06;

		GTL_Convert_DcbNumberToUl(pValueAmountNum, &ulAmount, 6);
		StructInfoPayment.Transaction_Amount = ulAmount;
	}
	if (pValueAmountOtherNum != NULL)
	{
		champs55[l_champs55++] =0x9F;
		champs55[l_champs55++] =0x03;
		champs55[l_champs55++] =(UC)0x06;
		memcpy(&champs55[l_champs55],pValueAmountOtherNum,0x06);
		l_champs55 += 0x06;

		GTL_Convert_DcbNumberToUl(pValueAmountOtherNum, &ulAmountOther, 6);
		//StructInfoPayment.mo
	}
	if (pValueExpirationDate != NULL)
	{
		champs55[l_champs55++] =0x5F;
		champs55[l_champs55++] =0x24;
		champs55[l_champs55++] =(UC)0x03;
		memcpy(&champs55[l_champs55],pValueExpirationDate,0x03);
		l_champs55 += 0x03;

		GTL_Convert_DcbNumberToAscii(pValueExpirationDate, ExpirationDate, 3, 6);
		memcpy(StructInfoPayment.Expired_Date,ExpirationDate+4,2);//sss
		memcpy(StructInfoPayment.Expired_Date+2,ExpirationDate+2,2);
	}
	if (pValuePan != NULL)
	{
		champs55[l_champs55++] =0x5A;
		champs55[l_champs55++] =(UC)LengthPan;
		memcpy(&champs55[l_champs55],pValuePan,LengthPan);
		l_champs55 += LengthPan;

		GTL_Convert_DcbPaddedToAscii(pValuePan, Pan, LengthPan);
		StructInfoPayment.Holder_Card[0] = 2*LengthPan;
		memcpy(StructInfoPayment.Holder_Card+1,Pan,2*LengthPan);
	}
	if (pValuePanSeq != NULL)
	{
		champs55[l_champs55++] =0x5F;
		champs55[l_champs55++] =0x34;
		champs55[l_champs55++] =(UC)0x01;
		memcpy(&champs55[l_champs55],pValuePanSeq,0x01);
		l_champs55 += 0x01;

    	GTL_Convert_DcbNumberToUl(pValuePanSeq, &ulPanSeq, 1);
		sprintf((char*)StructInfoPayment.PanSeqNumber,"%.03lu",ulPanSeq);
	}
	if (pValueCurrencyCode != NULL)
	{
		champs55[l_champs55++] =0x5F;
		champs55[l_champs55++] =0x2A;
		champs55[l_champs55++] =(UC)0x02;
		memcpy(&champs55[l_champs55],pValueCurrencyCode,0x02);
		l_champs55 += 0x02;

   		GTL_Convert_DcbNumberToUl(pValueCurrencyCode, &ulCurrencyCode, 2);
		sprintf((char*)StructInfoPayment.Devise_acc.code_num,"%.03lu",ulCurrencyCode);//sss
	}
	if (pValueCurrencyExp != NULL)
	{
		champs55[l_champs55++] =0x5F;
		champs55[l_champs55++] =0x36;
		champs55[l_champs55++] =(UC)0x01;
		memcpy(&champs55[l_champs55],pValueCurrencyExp,0x01);
		l_champs55 += 0x01;

		GTL_Convert_DcbNumberToUl(pValueCurrencyExp, &ulCurrencyExp, 1);
		StructInfoPayment.Devise_acc.Exposant = (unsigned char) ulCurrencyExp;//sss
	}
	if (pValueDate != NULL)
	{
		champs55[l_champs55++] =0xFA;
		champs55[l_champs55++] =(UC)0x03;
		memcpy(&champs55[l_champs55],pValueDate,0x03);
		l_champs55 += 0x03;

		GTL_Convert_DcbNumberToAscii(pValueDate, Date, 3, 6);
		memcpy(StructInfoPayment.Transaction_DateTime,Date,6);//sss
	}
	if (pValueType != NULL)
	{
		champs55[l_champs55++] =0x9F;
		champs55[l_champs55++] =(UC)0x01;
		memcpy(&champs55[l_champs55],&pValueType[0],0x01);
		l_champs55 += 0x01;

		ulType = pValueType[0];
	}
	//if (pValueCardType != NULL)
	//	usCardType = (pValueCardType[0] << 8) + pValueCardType[1];
	if (pValueApplicationLabel != NULL)
	{
		champs55[l_champs55++] =0x50;
		champs55[l_champs55++] =(UC)length_application_label;
		memcpy(&champs55[l_champs55],pValueApplicationLabel,length_application_label);
		l_champs55 += length_application_label;

   		memcpy (ApplicationLabel, pValueApplicationLabel, length_application_label);
		memcpy(StructInfoPayment.Appli_label,ApplicationLabel,length_application_label);
	}
	if (pValueAppliPreferedName != NULL)
	{
		champs55[l_champs55++] =0x9F;
		champs55[l_champs55++] =0x12;
		champs55[l_champs55++] =(UC)length_appli_prefered_name;
		memcpy(&champs55[l_champs55],pValueAppliPreferedName,length_appli_prefered_name);
		l_champs55 += length_appli_prefered_name;

   		memcpy (AppliPreferedName, pValueAppliPreferedName, length_appli_prefered_name);
		memcpy(StructInfoPayment.Nom_Prenom,AppliPreferedName,length_appli_prefered_name);
	}
	if (pValueTime != NULL)
	{
		champs55[l_champs55++] =0x9F;
		champs55[l_champs55++] =0x21;
		champs55[l_champs55++] =(UC)0x03;
		memcpy(&champs55[l_champs55],pValueTime,0x03);
		l_champs55 += 0x03;

   		GTL_Convert_DcbNumberToAscii(pValueTime, Time, 3, 6);
		memcpy(StructInfoPayment.Transaction_DateTime+6,Time,6);
	}
	else
	{
		if (Telium_Read_date(&Datesys) == OK)//sss
		{
//				memcpy(StructInfoPayment.Transaction_DateTime	,Datesys.year,2);
//				memcpy(StructInfoPayment.Transaction_DateTime+2	,Datesys.month,2);
//				memcpy(StructInfoPayment.Transaction_DateTime+4	,Datesys.day,2);
			memcpy(StructInfoPayment.Transaction_DateTime+6	,Datesys.hour,2);
			memcpy(StructInfoPayment.Transaction_DateTime+8	,Datesys.minute,2);
			memcpy(StructInfoPayment.Transaction_DateTime+10,Datesys.second,2);
		}
	}


	if(pValueTrack2!=NULL)
	{
		short cara;

		memset(l_c_Track2,'\0',40);

		CONVERT_dcbtoa(pValueTrack2, length_Track2, (PC)l_c_Track2, FALSE);

		Lg = (UC)length_Track2*2;
		for(cara=0;((cara<Lg)&&(cara<37)&&(l_c_Track2[cara] != 'F')&&(l_c_Track2[cara] != 'f')&&(l_c_Track2[cara] != 0)&&(l_c_Track2[cara] != ' '));cara ++);
		Lg=cara;
		StructInfoPayment.Normal_Iso2[0] = Lg;
		memcpy(&StructInfoPayment.Normal_Iso2[1],l_c_Track2,Lg);
		StructInfoPayment.Compressed_Iso2[0]= Lg;

		memcpy(&StructInfoPayment.Compressed_Iso2[1],l_c_Track2,19);
		memcpy(StructInfoPayment.SeviceCode,&StructInfoPayment.Normal_Iso2[StructInfoPayment.Holder_Card[0]+6],3);

		if(StructInfoPayment.Expired_Date[0] == 0)
		{
			memcpy(StructInfoPayment.Expired_Date,&StructInfoPayment.Normal_Iso2[StructInfoPayment.Holder_Card[0]+2],4);//sss
			sprintf((char*)StructInfoPayment.Devise_acc.code_num,"%.03lu",504);//sss
			StructInfoPayment.Devise_acc.Exposant = (unsigned char) 2;//sss

			if (Telium_Read_date(&Datesys) == OK)//sss
			{
				memcpy(StructInfoPayment.Transaction_DateTime	,Datesys.year,2);
				memcpy(StructInfoPayment.Transaction_DateTime+2	,Datesys.month,2);
				memcpy(StructInfoPayment.Transaction_DateTime+4	,Datesys.day,2);
				memcpy(StructInfoPayment.Transaction_DateTime+6	,Datesys.hour,2);
				memcpy(StructInfoPayment.Transaction_DateTime+8	,Datesys.minute,2);
				memcpy(StructInfoPayment.Transaction_DateTime+10,Datesys.second,2);
			}
		}
	}


	for(i=0;((i<20)&&(StructInfoPayment.Normal_Iso2[i+1]!='D')&&(StructInfoPayment.Normal_Iso2[i+1]!='d')&&(StructInfoPayment.Normal_Iso2[i+1]!='='));i++);
	if(i!=20)
	{
		memset(Card_Holder_Nbre_ISO2,0,sizeof(Card_Holder_Nbre_ISO2));
		Card_Holder_Nbre_ISO2[0] = i;
		memcpy(&Card_Holder_Nbre_ISO2[1],&StructInfoPayment.Normal_Iso2[1],i);
		memset(Expired_Date_ISO2,0,sizeof(Expired_Date_ISO2));
		memcpy(Expired_Date_ISO2,&StructInfoPayment.Normal_Iso2[i+2],4);
	}
	if(		(Card_Holder_Nbre_ISO2[0]!=StructInfoPayment.Holder_Card[0])
		||	memcmp(&Card_Holder_Nbre_ISO2[1],&StructInfoPayment.Holder_Card[1],StructInfoPayment.Holder_Card[0])
		||	memcmp(Expired_Date_ISO2,StructInfoPayment.Expired_Date,4)
	)
	{
		Os__xprint("ATTENTION INCONSIS");
	}



	/*
	APEMV_UI_MessageDisplayDelay ("-.Appli_aid", 1);
	APEMV_UI_MessageDisplayDelay (StructInfoPayment.Appli_aid, 1);
	APEMV_UI_MessageDisplayDelay ("-.Expired_Date", 1);
	APEMV_UI_MessageDisplayDelay (StructInfoPayment.Expired_Date, 1);
	APEMV_UI_MessageDisplayDelay ("-.Holder_Card+1", 1);
	APEMV_UI_MessageDisplayDelay (StructInfoPayment.Holder_Card+1, 1);
	APEMV_UI_MessageDisplayDelay ("-.PanSeqNumber", 1);
	APEMV_UI_MessageDisplayDelay (StructInfoPayment.PanSeqNumber, 1);
	APEMV_UI_MessageDisplayDelay ("-.Devise_acc.code_num", 1);
	APEMV_UI_MessageDisplayDelay (StructInfoPayment.Devise_acc.code_num, 1);
	APEMV_UI_MessageDisplayDelay ("-.Devise_acc.Exposant", 1);
	APEMV_UI_MessageDisplayDelay (&StructInfoPayment.Devise_acc.Exposant, 1);
	APEMV_UI_MessageDisplayDelay ("-.Appli_label", 1);
	APEMV_UI_MessageDisplayDelay (StructInfoPayment.Appli_label, 1);
	APEMV_UI_MessageDisplayDelay ("-.Nom_Prenom", 1);
	APEMV_UI_MessageDisplayDelay (StructInfoPayment.Nom_Prenom, 1);
	APEMV_UI_MessageDisplayDelay ("-.Transaction_DateTime", 1);
	APEMV_UI_MessageDisplayDelay (StructInfoPayment.Transaction_DateTime, 1);
	APEMV_UI_MessageDisplayDelay ("-.Transaction_DateTime+6", 1);
	APEMV_UI_MessageDisplayDelay (StructInfoPayment.Transaction_DateTime+6, 1);
	*/


	StructInfoPayment.l_champ55 = l_champs55;
	memcpy(StructInfoPayment.Champ55,champs55,l_champs55);


	// Print the receipt
	__ClessSample_Receipt_FormatAmountNaps(ulAmount, FALSE, ulCurrencyExp, 0, szAmount);
}


void Remplir_Infopayment_Cless_VISA (T_SHARED_DATA_STRUCT * pTransaction,int nCardHolderLang)
{
	int nResult, nPosition;
	unsigned long ulTag;
	unsigned long ulReadLength;
	const unsigned char * pValue;
	const unsigned char* pValueStatusCode;
	const unsigned char* pValueTsc;
	unsigned int LengthTsc;
	const unsigned char* pValueAid;
	unsigned int LengthAid;
	const unsigned char* pValueCid;
	const unsigned char* pValueCvrResult;
	const unsigned char* pValueTvr;
	const unsigned char* pValueAmountBin;
	const unsigned char* pValueAmountNum;
	const unsigned char* pValueAmountOtherBin;
	const unsigned char* pValueAmountOtherNum;
	const unsigned char* pValueExpirationDate;
	const unsigned char* pValuePan;
	unsigned int LengthPan;
	const unsigned char* pValuePanSeq;
	const unsigned char* pValueAuthorCode;
	const unsigned char* pValueAuthorRespCode;
	const unsigned char* pValueCurrencyCode;
	const unsigned char* pValueCurrencyExp;
	const unsigned char* pValueDate;
	const unsigned char* pValueTime;
	const unsigned char* pValueType;
	const unsigned char* pValueForcedOnline;
	const unsigned char* pAvailableOfflineSpendingAmount;
	unsigned int length_spending_amount;
	const unsigned char* pVisaWaveAvailableOfflineSpendingAmount;
	unsigned int length_VisaWave_spending_amount;
	const unsigned char* pValueCardType;
	unsigned char temp;
	const unsigned char* pValueApplicationLabel;
	unsigned int length_application_label;
	const unsigned char* pValueAppliPreferedName;
	unsigned int length_appli_prefered_name;
	unsigned long ulStatusCode;
	unsigned long ulTsc;
	unsigned char Aid[20];
	unsigned char Cid;
	unsigned char CvrResult[3];
	unsigned char Tvr[5];
	unsigned long ulAmount;
	unsigned long ulAmountOther;
	char ExpirationDate[6 + 1];
	char Pan[19 + 1];
	unsigned long ulPanSeq;
	char AuthorCode[6 + 1];
	char AuthorRespCode[2 + 1];
	unsigned long ulCurrencyCode;
	unsigned long ulCurrencyExp;
	char Date[6 + 1];
	char Time[6 + 1];
	unsigned long ulType;
	int bForcedOnline;
	char szAmount[20];
	char szSpendingAmount[20];
	char szVisaWaveSpendingAmount[20];
	char transactionKind;
	unsigned long ulAvailableOfflineSpendingAmount;
	unsigned short usCardType;
	unsigned char ApplicationLabel [16 + 1]; // Application label + '\0'
	unsigned char AppliPreferedName [16 + 1]; // Application prefered name + '\0'
	MSGinfos tDisplayMsg;



	const unsigned char* pValueDFName;
	unsigned int lenght_DFName;


	const unsigned char* pValueAppVersionNumb;
	unsigned int lenght_AppVersionNumb;
	const unsigned char* pValueIssuerAppData;
	unsigned int lenght_IssuerAppData;
	const unsigned char* pValueTerCountryCD;
	unsigned int lenght_TerCountryCD;
	const unsigned char* pValueIFDSerialNumb;
	unsigned int lenght_IFDSerialNumb;;
	const unsigned char* pValueAppCrypto;
	unsigned int lenght_AppCrypto;
	const unsigned char* pValueTerminalType;
	unsigned int lenght_TerminalType;
	const unsigned char* pValueUnpredictableNumb;
	unsigned int lenght_UnpredictableNumb;;
	const unsigned char* pValueATC;
	unsigned int lenght_ATC;
	const unsigned char* pValueAIP;
	unsigned int lenght_AIP;
	const unsigned char* pValueTerCapabilities;
	unsigned int lenght_TerCapabilities;
	const unsigned char* pValueTrack2;
	unsigned int length_Track2;

	const unsigned char* pValueClessCap;
	unsigned int length_ClessCap;




	uint8	l_champs55;
	uint8	champs55[255];

	UC		Lg,LgCarte,i;
	UC		Card_Holder_Nbre_ISO2[21],Expired_Date_ISO2[5];
	UC		l_c_Track2[40];


	Telium_Date_t Datesys;//sss



		// no receipt if loop mode
//		if (ClessSample_Menu_IsTransactionLoopOn())
//			return;
//
//		if (!ClessSample_DumpData_DumpOpenOutputDriver())
//			return;


		pValueStatusCode = NULL;
		pValueTsc = NULL;
		LengthTsc = 0;
		pValueAid = NULL;
		LengthAid = 0;
		pValueCid = NULL;
		pValueCvrResult = NULL;
		pValueTvr = NULL;
		pValueAmountBin = NULL;
		pValueAmountNum = NULL;
		pValueAmountOtherBin = NULL;
		pValueAmountOtherNum = NULL;
		pValueExpirationDate = NULL;
		pValuePan = NULL;
		LengthPan = 0;
		pValuePanSeq = NULL;
		pValueAuthorCode = NULL;
		pValueAuthorRespCode = NULL;
		pValueCurrencyCode = NULL;
		pValueCurrencyExp = NULL;
		pValueDate = NULL;
		pValueTime = NULL;
		pValueType = NULL;
		pValueForcedOnline = NULL;
		pAvailableOfflineSpendingAmount = NULL;
		length_spending_amount = 0;
		pVisaWaveAvailableOfflineSpendingAmount = NULL;
		length_VisaWave_spending_amount = 0;
		transactionKind =0;
		pValueCardType = NULL;
		usCardType = 0;
		pValueApplicationLabel = NULL;
		length_application_label = 0;
		pValueAppliPreferedName = NULL;
		length_appli_prefered_name = 0;

		pValueDFName=NULL;
		lenght_DFName=0;
		pValueAppVersionNumb=NULL;
		lenght_AppVersionNumb=0;
		pValueIssuerAppData=NULL;
		lenght_IssuerAppData=0;
		pValueTerCountryCD=NULL;
		lenght_TerCountryCD=0;
		pValueIFDSerialNumb=NULL;
		lenght_IFDSerialNumb=0;
		pValueAppCrypto=NULL;
		lenght_AppCrypto=0;
		pValueTerminalType=NULL;
		lenght_TerminalType=0;
		pValueUnpredictableNumb=NULL;
		lenght_UnpredictableNumb=0;
		pValueATC=NULL;
		lenght_ATC=0;
		pValueAIP=NULL;
		lenght_AIP=0;

		pValueTerCapabilities = NULL;
		lenght_TerCapabilities= 0;

		pValueTrack2 = NULL;
		length_Track2 = 0;

		pValueClessCap=NULL;
		length_ClessCap=0;

	/*
		Telium_DefprinterpatternHR (0, 0, 0xF1, (char *) Line_part_1);
		Telium_DefprinterpatternHR (0, 0, 0xF2, (char *) Line_part_2);
		Telium_DefprinterpatternHR (0, 0, 0xF3, (char *) Visa_payWave_part_1);
		Telium_DefprinterpatternHR (0, 0, 0xF4, (char *) Visa_payWave_part_2);
		Telium_DefprinterpatternHR (0, 0, 0xF5, (char *) Visa_1);
		Telium_DefprinterpatternHR (0, 0, 0xF6, (char *) Visa_2);
		Telium_DefprinterpatternHR (0, 0, 0xF7, (char *) Ingenico_part_1);
		Telium_DefprinterpatternHR (0, 0, 0xF8, (char *) Ingenico_part_2);
		Telium_DefprinterpatternHR (0, 0, 0xF9, (char *) Signature_part_1);
		Telium_DefprinterpatternHR (0, 0, 0xFA, (char *) Signature_part_2);
		Telium_DefprinterpatternHR (0, 0, 0xFB, (char *) ClessWavesCenterOnly_Part1);
		Telium_DefprinterpatternHR (0, 0, 0xFC, (char *) ClessWavesCenterOnly_Part2);
		Telium_DefprinterpatternHR (0, 0, 0xFD, (char *) MasterCardPayPass_Part1);
		Telium_DefprinterpatternHR (0, 0, 0xFE, (char *) MasterCardPayPass_Part2);
		Telium_DefprinterpatternHR (0, 0, 0xEE, (char *) AmericanExpress_part_1);
		Telium_DefprinterpatternHR (0, 0, 0xEF, (char *) AmericanExpress_part_2);
		Telium_DefprinterpatternHR (0, 0, 0xEC, (char *) Discover_part_1);
		Telium_DefprinterpatternHR (0, 0, 0xED, (char *) Discover_part_2);
		Telium_DefprinterpatternHR (0, 0, 0xEA, (char *) Interac_only_part_1);
		Telium_DefprinterpatternHR (0, 0, 0xEB, (char *) Interac_only_part_2);
		Telium_DefprinterpatternHR (0, 0, 0xE8, (char *) Cartes2010_Logo_Part_1);
		Telium_DefprinterpatternHR (0, 0, 0xE9, (char *) Cartes2010_Logo_Part_2);
	*/
		nPosition = SHARED_EXCHANGE_POSITION_NULL;

		do{
	    	// Try to following tag.
	    	nResult = GTL_SharedExchange_GetNext (pTransaction, &nPosition, &ulTag, &ulReadLength, &pValue);

	        if (nResult == STATUS_SHARED_EXCHANGE_OK)  // If tag found
	        {
				switch(ulTag)
				{
				case TAG_KERNEL_STATUS_CODE:					pValueStatusCode = pValue; break;
	    		case TAG_EMV_TRANSACTION_SEQUENCE_COUNTER:		pValueTsc = pValue;	LengthTsc = ulReadLength; break;
	    		case TAG_EMV_AID_CARD:							pValueAid = pValue;	LengthAid = ulReadLength; break;
	    		case TAG_EMV_DF_NAME:							pValueDFName = pValue;	lenght_DFName = ulReadLength; break;
	    		case TAG_EMV_CRYPTOGRAM_INFO_DATA:				pValueCid = pValue; break;
	    		case TAG_EMV_CVM_RESULTS:						pValueCvrResult = pValue; break;
	    		case TAG_EMV_TVR:								pValueTvr = pValue; break;
	    		case TAG_EMV_AMOUNT_AUTH_BIN:					pValueAmountBin = pValue; break;
	    		case TAG_EMV_AMOUNT_AUTH_NUM:					pValueAmountNum = pValue; break;
	    		case TAG_EMV_AMOUNT_OTHER_BIN:					pValueAmountOtherBin = pValue; break;
	    		case TAG_EMV_AMOUNT_OTHER_NUM:					pValueAmountOtherNum = pValue; break;
	    		case TAG_EMV_APPLI_EXPIRATION_DATE:				pValueExpirationDate = pValue; break;
	    		case TAG_EMV_APPLI_PAN:							pValuePan = pValue;	LengthPan = ulReadLength; break;
	    		case TAG_EMV_APPLI_PAN_SEQUENCE_NUMBER:			pValuePanSeq = pValue; break;
	    		case TAG_EMV_AUTHORISATION_CODE:				pValueAuthorCode = pValue; break;
	    		case TAG_EMV_AUTHORISATION_RESPONSE_CODE:		pValueAuthorRespCode = pValue; break;
	    		case TAG_EMV_TRANSACTION_CURRENCY_CODE:			pValueCurrencyCode = pValue; break;
	    		case TAG_EMV_TRANSACTION_CURRENCY_EXPONENT:		pValueCurrencyExp = pValue; break;
	    		case TAG_EMV_TRANSACTION_DATE:					pValueDate = pValue; break;
	    		case TAG_EMV_TRANSACTION_TIME:					pValueTime = pValue; break;
	    		case TAG_EMV_TRANSACTION_TYPE:					pValueType = pValue; break;
	    		case TAG_KERNEL_CARD_TYPE:						pValueCardType=pValue; break;
	    		case TAG_EMV_APPLICATION_LABEL:					pValueApplicationLabel=pValue; length_application_label = ulReadLength; break;
	    		//case TAG_EMV_APPLI_PREFERED_NAME:				pValueAppliPreferedName=pValue; length_appli_prefered_name = ulReadLength; break;
	    		//case TAG_EMV_APPLI_PREFERRED_NAME             pValueAppliPreferedName=pValue; length_appli_prefered_name = ulReadLength; break;
	    		case TAG_EMV_CARDHOLDER_NAME:					pValueAppliPreferedName=pValue; length_appli_prefered_name = ulReadLength; break;
	    		//case TAG_EMV_CARDHOLDER_NAME_EXTENDED:		pValueAppliPreferedName=pValue; length_appli_prefered_name = ulReadLength; break;

	    		case TAG_EMV_APPLI_VERSION_NUMBER_TERM:			pValueAppVersionNumb=pValue;lenght_AppVersionNumb=ulReadLength;break;
	    		case TAG_EMV_ISSUER_APPLI_DATA:					pValueIssuerAppData=pValue;lenght_IssuerAppData=ulReadLength;break;
	    		case TAG_EMV_TERMINAL_COUNTRY_CODE:				pValueTerCountryCD=pValue;lenght_TerCountryCD=ulReadLength;break;
	    		case TAG_EMV_IFD_SERIAL_NUMBER:					pValueIFDSerialNumb=pValue;lenght_IFDSerialNumb=ulReadLength;break;
	    		case TAG_EMV_APPLICATION_CRYPTOGRAM:			pValueAppCrypto=pValue;lenght_AppCrypto=ulReadLength;break;
	    		case TAG_EMV_TERMINAL_TYPE:						pValueTerminalType=pValue;lenght_TerminalType=ulReadLength;break;
	    		case TAG_EMV_ATC:								pValueATC=pValue;lenght_ATC=ulReadLength;break;
	    		case TAG_EMV_UNPREDICTABLE_NUMBER:				pValueUnpredictableNumb=pValue;lenght_UnpredictableNumb=ulReadLength;break;
	    		case TAG_EMV_AIP:								pValueAIP=pValue;lenght_AIP=ulReadLength;break;
	    		case TAG_EMV_TERMINAL_CAPABILITIES:				pValueTerCapabilities=pValue;lenght_TerCapabilities=ulReadLength;break;
	    		case TAG_EMV_TRACK_2_EQU_DATA:					pValueTrack2=pValue;length_Track2=ulReadLength; break;
	    		case 0x9F6B:									pValueTrack2=pValue;length_Track2=ulReadLength; break;//sss Track pour Magstripe read
	    		case 0x9F6E :									pValueClessCap=pValue;length_ClessCap=ulReadLength; break;

	#ifndef DISABLE_OTHERS_KERNELS
	    		case TAG_PAYWAVE_AVAILABLE_OFFLINE_SPENDING_AMOUNT: pAvailableOfflineSpendingAmount=pValue; length_spending_amount = ulReadLength; break;
				case TAG_VISAWAVE_VLP_AVAILABLE_FUNDS:			pVisaWaveAvailableOfflineSpendingAmount=pValue; length_VisaWave_spending_amount = ulReadLength; break;
	#endif
				default:
					break;
				}
	        }
	        else if (nResult == STATUS_SHARED_EXCHANGE_END)  // Buffer end reached
	        {
	        }
	        else // error or end or ...
	        {
	        	GTL_Traces_TraceDebug("Display tags error:%x\n", nResult);
	        }
	    }while (nResult == STATUS_SHARED_EXCHANGE_OK);



		ulStatusCode = 0;
		ulTsc = 0;
		memset(Aid, 0, sizeof(Aid));
		Cid = 0;
		memset(CvrResult, 0, sizeof(CvrResult));
		memset(Tvr, 0, sizeof(Tvr));
		ulAmount = 0;
		ulAmountOther = 0;
		memset(ExpirationDate, '?', sizeof(ExpirationDate));
		memset(Pan, '?', sizeof(Pan));
		ulPanSeq = 0;
		memset(AuthorCode, 0, sizeof(AuthorCode));
		memset(AuthorRespCode, 0, sizeof(AuthorRespCode));
		ulCurrencyCode = 0;
		ulCurrencyExp = 0;
		memset(Date, '?', sizeof(Date));
		memset(Time, '?', sizeof(Time));
		ulType = 0;
		bForcedOnline = FALSE;
		ulAvailableOfflineSpendingAmount = 0;
		memset (szSpendingAmount, '\0', sizeof(szSpendingAmount));
		memset (szVisaWaveSpendingAmount, '\0', sizeof(szVisaWaveSpendingAmount));
		memset (ApplicationLabel, '\0', sizeof(ApplicationLabel));
		memset (AppliPreferedName, '\0', sizeof(AppliPreferedName));

		l_champs55 = 0;


		if (pValueStatusCode != NULL)
			GTL_Convert_BinNumberToUl(pValueStatusCode, &ulStatusCode, 2);
		if (pValueTsc != NULL)
		{


			//sTrace("seq num");
			champs55[l_champs55++] =0x9F;
			champs55[l_champs55++] =0x41;
			champs55[l_champs55++] =(UC)LengthTsc;
			memcpy(&champs55[l_champs55],pValueTsc,LengthTsc);
			l_champs55 += LengthTsc;

			GTL_Convert_DcbNumberToUl(pValueTsc, &ulTsc, LengthTsc);


		}
		if (pValueAid != NULL)
		{
			champs55[l_champs55++] =TAG_EMV_DF_NAME;
			champs55[l_champs55++] =(UC)LengthAid;
			memcpy(&champs55[l_champs55],pValueAid,LengthAid);
			l_champs55 += LengthAid;

			memcpy(Aid, pValueAid, LengthAid);
			StructInfoPayment.Appli_aid[0] = LengthAid;
			GTL_Convert_DcbNumberToAscii(Aid, StructInfoPayment.Appli_aid+1, LengthAid, 2*LengthAid);
		}
		else if (pValueDFName != NULL)
		{
			champs55[l_champs55++] =TAG_EMV_DF_NAME;
			champs55[l_champs55++] =(UC)lenght_DFName;
			memcpy(&champs55[l_champs55],pValueDFName,lenght_DFName);
			l_champs55 += lenght_DFName;

			memcpy(Aid, pValueDFName, lenght_DFName);
			StructInfoPayment.Appli_aid[0] = lenght_DFName;
			GTL_Convert_DcbNumberToAscii(Aid, StructInfoPayment.Appli_aid+1, lenght_DFName, 2*lenght_DFName);
		}


		if (pValueCid != NULL)
		{
			champs55[l_champs55++] =0x9F;
			champs55[l_champs55++] =0x27;
			champs55[l_champs55++] =(UC)0x01;
			memcpy(&champs55[l_champs55],pValueCid,0x01);
			l_champs55 += 0x01;

			Cid = *pValueCid;
		}
		if (pValueCvrResult != NULL)
		{
			champs55[l_champs55++] =0x9F;
			champs55[l_champs55++] =0x34;
			champs55[l_champs55++] =(UC)0x03;
			memcpy(&champs55[l_champs55],pValueCvrResult,0x03);
			l_champs55 += 0x03;

			memcpy(CvrResult, pValueCvrResult, sizeof(CvrResult));
		}
		if (pValueTvr != NULL)
		{

			champs55[l_champs55++] =TAG_EMV_TVR;
			champs55[l_champs55++] =(UC)0x05;
			memcpy(&champs55[l_champs55],pValueTvr,0x05);
			l_champs55 += 0x05;


			memcpy(Tvr, pValueTvr, sizeof(Tvr));
		}
		if (pValueAmountBin != NULL)
		{
			GTL_Convert_BinNumberToUl(pValueAmountBin, &ulAmount, 4);

		}


		if (pValueAmountOtherBin != NULL)
		{
			GTL_Convert_BinNumberToUl(pValueAmountOtherBin, &ulAmountOther, 4);
		}
		if (pValueAmountNum != NULL)
		{

			champs55[l_champs55++] =0x9F;
			champs55[l_champs55++] =0x02;
			champs55[l_champs55++] =(UC)0x06;
			memcpy(&champs55[l_champs55],pValueAmountNum,0x06);
			l_champs55 += 0x06;

			GTL_Convert_DcbNumberToUl(pValueAmountNum, &ulAmount, 6);
			StructInfoPayment.Transaction_Amount = ulAmount;
		}
		if (pValueAmountOtherNum != NULL)
		{
			champs55[l_champs55++] =0x9F;
			champs55[l_champs55++] =0x03;
			champs55[l_champs55++] =(UC)0x06;
			memcpy(&champs55[l_champs55],pValueAmountOtherNum,0x06);
			l_champs55 += 0x06;
			GTL_Convert_DcbNumberToUl(pValueAmountOtherNum, &ulAmountOther, 6);
		}
		if (pValueExpirationDate != NULL)
		{

				champs55[l_champs55++] =0x5F;
				champs55[l_champs55++] =0x24;
				champs55[l_champs55++] =(UC)0x03;
				memcpy(&champs55[l_champs55],pValueExpirationDate,0x03);
				l_champs55 += 0x03;

			GTL_Convert_DcbNumberToAscii(pValueExpirationDate, ExpirationDate, 3, 6);
			memcpy(StructInfoPayment.Expired_Date,ExpirationDate+4,2);
			memcpy(StructInfoPayment.Expired_Date+2,ExpirationDate+2,2);
		}
		if (pValuePan != NULL)
		{
		//	sTrace("PAN ok");
				champs55[l_champs55++] =0x5A;
				champs55[l_champs55++] =(UC)LengthPan;
				memcpy(&champs55[l_champs55],pValuePan,LengthPan);
				l_champs55 += LengthPan;


			//APEMV_UI_MessageDisplayDelayHexa(pValuePan,LengthPan,5);

			GTL_Convert_DcbPaddedToAscii(pValuePan, Pan, LengthPan);

			StructInfoPayment.Holder_Card[0] = 2*LengthPan;

			//APEMV_UI_MessageDisplayDelay(Pan,5);

			memcpy(StructInfoPayment.Holder_Card+1,Pan,2*LengthPan);

			if (StructInfoPayment.Holder_Card[0] == 20) //Certif : la carte de longueur 19
				StructInfoPayment.Holder_Card[0] = 19;
		}
		if (pValuePanSeq != NULL)
		{

			champs55[l_champs55++] =0x5F;
			champs55[l_champs55++] =0x34;
			champs55[l_champs55++] =(UC)0x01;
			memcpy(&champs55[l_champs55],pValuePanSeq,0x01);
			l_champs55 += 0x01;

			GTL_Convert_DcbNumberToUl(pValuePanSeq, &ulPanSeq, 1);
			sprintf(StructInfoPayment.PanSeqNumber,"%.03d",ulPanSeq);
		}
		if (pValueAuthorCode != NULL)
			memcpy(AuthorCode, pValueAuthorCode, 6);
		if (pValueAuthorRespCode != NULL)
			memcpy(AuthorRespCode, pValueAuthorRespCode, 2);
		if (pValueCurrencyCode != NULL)
		{
			champs55[l_champs55++] =0x5F;
			champs55[l_champs55++] =0x2A;
			champs55[l_champs55++] =(UC)0x02;
			memcpy(&champs55[l_champs55],pValueCurrencyCode,0x02);
			l_champs55 += 0x02;

			GTL_Convert_DcbNumberToUl(pValueCurrencyCode, &ulCurrencyCode, 2);
			sprintf(StructInfoPayment.Devise_acc.code_num,"%.03d",ulCurrencyCode);
		}
		if (pValueCurrencyExp != NULL)
		{
	//		sTrace("curruncy ok");

			champs55[l_champs55++] =0x5F;
			champs55[l_champs55++] =0x36;
			champs55[l_champs55++] =(UC)0x01;
			memcpy(&champs55[l_champs55],pValueCurrencyExp,0x01);
			l_champs55 += 0x01;

			GTL_Convert_DcbNumberToUl(pValueCurrencyExp, &ulCurrencyExp, 1);
			StructInfoPayment.Devise_acc.Exposant = (unsigned char) ulCurrencyExp;
		}
		if (pValueDate != NULL)
		{
		//	sTrace("date ok");

//			champs55[l_champs55++] =0xFA;
//			champs55[l_champs55++] =(UC)0x03;

			champs55[l_champs55++] =0x9A;
			champs55[l_champs55++] =(UC)0x03;

			memcpy(&champs55[l_champs55],pValueDate,0x03);
			l_champs55 += 0x03;

			GTL_Convert_DcbNumberToAscii(pValueDate, Date, 3, 6);
			memcpy(StructInfoPayment.Transaction_DateTime,Date,6);
		}
		if (pValueTime != NULL)
		{
			champs55[l_champs55++] =0x9F;
			champs55[l_champs55++] =0x21;
			champs55[l_champs55++] =(UC)0x03;
			memcpy(&champs55[l_champs55],pValueTime,0x03);
			l_champs55 += 0x03;
			GTL_Convert_DcbNumberToAscii(pValueTime, Time, 3, 6);
			memcpy(StructInfoPayment.Transaction_DateTime+6,Time,6);
		}
		else
		{
			if (Telium_Read_date(&Datesys) == OK)//sss
			{
//				memcpy(StructInfoPayment.Transaction_DateTime	,Datesys.year,2);
//				memcpy(StructInfoPayment.Transaction_DateTime+2	,Datesys.month,2);
//				memcpy(StructInfoPayment.Transaction_DateTime+4	,Datesys.day,2);
				memcpy(StructInfoPayment.Transaction_DateTime+6	,Datesys.hour,2);
				memcpy(StructInfoPayment.Transaction_DateTime+8	,Datesys.minute,2);
				memcpy(StructInfoPayment.Transaction_DateTime+10,Datesys.second,2);
			}
		}
		if (pValueType != NULL)
		{
//			champs55[l_champs55++] =0x9F;
//			champs55[l_champs55++] =(UC)0x01;

			champs55[l_champs55++] =0x9C;
			champs55[l_champs55++] =(UC)0x01;


			memcpy(&champs55[l_champs55],&pValueType[0],0x01);
			l_champs55 += 0x01;
			GTL_Convert_DcbNumberToUl(pValueType, &ulType, 1);
		}
		if (pValueForcedOnline != NULL)
		{
			if (*pValueForcedOnline)
				bForcedOnline = TRUE;
		}

		if (pAvailableOfflineSpendingAmount != NULL)
		{
			ClessSampleFormatSpendingAmount(pAvailableOfflineSpendingAmount, length_spending_amount, szSpendingAmount);
		}

		if (pVisaWaveAvailableOfflineSpendingAmount != NULL)
		{
			ClessSampleFormatSpendingAmount(pVisaWaveAvailableOfflineSpendingAmount, length_VisaWave_spending_amount, szVisaWaveSpendingAmount);
		}

		//transactionKind = UNKNOWN_TRANSACTION;
		if (pValueCardType != NULL)
			usCardType = (pValueCardType[0] << 8) + pValueCardType[1];

		if (pValueApplicationLabel != NULL)
		{
			champs55[l_champs55++] =0x50;
			champs55[l_champs55++] =(UC)length_application_label;
			memcpy(&champs55[l_champs55],pValueApplicationLabel,length_application_label);
			l_champs55 += length_application_label;
			memcpy (ApplicationLabel, pValueApplicationLabel, length_application_label);
			memcpy(StructInfoPayment.Appli_label,ApplicationLabel,length_application_label);
		}

		if (pValueAppliPreferedName != NULL)
		{

			champs55[l_champs55++] =0x9F;
			champs55[l_champs55++] =0x12;
			champs55[l_champs55++] =(UC)length_appli_prefered_name;
			memcpy(&champs55[l_champs55],pValueAppliPreferedName,length_appli_prefered_name);
			l_champs55 += length_appli_prefered_name;

			//APEMV_UI_MessageDisplayDelay ("-.Nom_Prenom", 1);
			//APEMV_UI_MessageDisplayDelay (pValueAppliPreferedName, 2);

			//memcpy (AppliPreferedName, pValueAppliPreferedName, length_appli_prefered_name);
			GTL_Convert_DcbNumberToAscii(pValueAppliPreferedName, AppliPreferedName, length_appli_prefered_name, 2*length_appli_prefered_name);
			//APEMV_UI_MessageDisplayDelay (AppliPreferedName, 2);

			memcpy(StructInfoPayment.Nom_Prenom,AppliPreferedName,length_appli_prefered_name);
		}



		if(pValueDFName!=NULL)
		{

			champs55[l_champs55++] =0x84;
			champs55[l_champs55++] =(UC)lenght_DFName;
			memcpy(&champs55[l_champs55],pValueDFName,lenght_DFName);
			l_champs55 += lenght_DFName;
		}
		if(pValueTerminalType!=NULL)
		{

			champs55[l_champs55++] =0x9F;
			champs55[l_champs55++] =0x35;
			champs55[l_champs55++] =(UC)lenght_TerminalType;
			memcpy(&champs55[l_champs55],pValueTerminalType,lenght_TerminalType);
			l_champs55 += lenght_TerminalType;
		}
		if(pValueATC!=NULL)
		{

			champs55[l_champs55++] =0x9F;
			champs55[l_champs55++] =0x36;
			champs55[l_champs55++] =(UC)lenght_ATC;
			memcpy(&champs55[l_champs55],pValueATC,lenght_ATC);
			l_champs55 += lenght_ATC;
		}

		if(pValueUnpredictableNumb!=NULL)
		{

			champs55[l_champs55++] =0x9F;
			champs55[l_champs55++] =0x37;
			champs55[l_champs55++] =(UC)lenght_UnpredictableNumb;
			memcpy(&champs55[l_champs55],pValueUnpredictableNumb,lenght_UnpredictableNumb);
			l_champs55 += lenght_UnpredictableNumb;
		}

		if(pValueAppCrypto!=NULL)
		{

			champs55[l_champs55++] =0x9F;
			champs55[l_champs55++] =0x26;
			champs55[l_champs55++] =(UC)lenght_AppCrypto;
			memcpy(&champs55[l_champs55],pValueAppCrypto,lenght_AppCrypto);
			l_champs55 += lenght_AppCrypto;
		}

		if(pValueIFDSerialNumb!=NULL)
		{

			champs55[l_champs55++] =0x9F;
			champs55[l_champs55++] =0x1E;
			champs55[l_champs55++] =(UC)lenght_IFDSerialNumb;
			memcpy(&champs55[l_champs55],pValueIFDSerialNumb,lenght_IFDSerialNumb);
			l_champs55 += lenght_IFDSerialNumb;
		}

		if(pValueIssuerAppData!=NULL)
		{
			//sTrace("issuer appdata");

			champs55[l_champs55++] =0x9F;
			champs55[l_champs55++] =0x10;
			champs55[l_champs55++] =(UC)lenght_IssuerAppData;
			memcpy(&champs55[l_champs55],pValueIssuerAppData,lenght_IssuerAppData);
			l_champs55 += lenght_IssuerAppData;
		}

		if(pValueAppVersionNumb!=NULL)
		{
			//sTrace("app ver numb");

			champs55[l_champs55++] =0x9F;
			champs55[l_champs55++] =0x09;
			champs55[l_champs55++] =(UC)lenght_AppVersionNumb;
			memcpy(&champs55[l_champs55],pValueAppVersionNumb,lenght_AppVersionNumb);
			l_champs55 += lenght_AppVersionNumb;
		}





		if(pValueTerCountryCD!=NULL)
		{

			//sTrace("country code");
				champs55[l_champs55++] =0x9F;
				champs55[l_champs55++] =0x1A;
				champs55[l_champs55++] =(UC)lenght_TerCountryCD;
				memcpy(&champs55[l_champs55],pValueTerCountryCD,lenght_TerCountryCD);
				l_champs55 += lenght_TerCountryCD;
		}

		if(pValueAIP!=NULL)
		{

			//sTrace("country code");
				champs55[l_champs55++] =0x82;
				champs55[l_champs55++] =(UC)lenght_AIP;
				memcpy(&champs55[l_champs55],pValueAIP,lenght_AIP);
				l_champs55 += lenght_AIP;
		}


		if(pValueTerCapabilities!=NULL)
		{

			//sTrace("country code");
				champs55[l_champs55++] =0x9F;
				champs55[l_champs55++] =0x33;
				champs55[l_champs55++] =(UC)lenght_TerCapabilities;
				memcpy(&champs55[l_champs55],pValueTerCapabilities,lenght_TerCapabilities);
				l_champs55 += lenght_TerCapabilities;
		}


		if(pValueTrack2!=NULL)
		{
			short cara;

			memset(l_c_Track2,'\0',40);

			CONVERT_dcbtoa(pValueTrack2, length_Track2, (PC)l_c_Track2, FALSE);

			Lg = (UC)length_Track2*2;

			//APEMV_UI_MessageDisplayDelayHexa(pValueTrack2,length_Track2,5);

			for(cara=0;((cara<Lg)&&(cara<37)&&(l_c_Track2[cara] != '=')&&(l_c_Track2[cara] != 'f')&&(l_c_Track2[cara] != 0)&&(l_c_Track2[cara] != ' ')&&(l_c_Track2[cara] != '?'));cara ++);
			LgCarte = cara;

			if((StructInfoPayment.Holder_Card[0] == 0) || (StructInfoPayment.Holder_Card[0] == 18)) // Lg 18 contourné
			{
//				Os__xprintaa("Holder_Card");
//				Os__xprintaa(StructInfoPayment.Holder_Card[0]);
				StructInfoPayment.Holder_Card[0] = LgCarte;
				memcpy(StructInfoPayment.Holder_Card+1,l_c_Track2,LgCarte);
			}


			for(;((cara<Lg)&&(cara<37)&&(l_c_Track2[cara] != 'F')&&(l_c_Track2[cara] != 'f')&&(l_c_Track2[cara] != 0)&&(l_c_Track2[cara] != ' ')&&(l_c_Track2[cara] != '?'));cara ++);
			Lg=cara;
//			Os__xprintaa("l_c_Track2");
//			Os__xprintaa(l_c_Track2);
			StructInfoPayment.Normal_Iso2[0] = Lg;
			memcpy(&StructInfoPayment.Normal_Iso2[1],l_c_Track2,Lg);
			StructInfoPayment.Compressed_Iso2[0]= Lg;
			memcpy(&StructInfoPayment.Compressed_Iso2[1],l_c_Track2,19);
			memcpy(StructInfoPayment.SeviceCode,&StructInfoPayment.Normal_Iso2[LgCarte+6],3);
//			Os__xprintaa("StructInfoPayment.SeviceCode");
//			Os__xprintaa(StructInfoPayment.SeviceCode);

			if(StructInfoPayment.Expired_Date[0] == 0)
			{
				memcpy(StructInfoPayment.Expired_Date,&StructInfoPayment.Normal_Iso2[LgCarte+2],4);//sss
				sprintf((char*)StructInfoPayment.Devise_acc.code_num,"%s",StructInfoPayment.Devise_acc.code_num);//sss
				StructInfoPayment.Devise_acc.Exposant = (unsigned char) 2;//sss

				if (Telium_Read_date(&Datesys) == OK)//sss
				{
					memcpy(StructInfoPayment.Transaction_DateTime	,Datesys.year,2);
					memcpy(StructInfoPayment.Transaction_DateTime+2	,Datesys.month,2);
					memcpy(StructInfoPayment.Transaction_DateTime+4	,Datesys.day,2);
					memcpy(StructInfoPayment.Transaction_DateTime+6	,Datesys.hour,2);
					memcpy(StructInfoPayment.Transaction_DateTime+8	,Datesys.minute,2);
					memcpy(StructInfoPayment.Transaction_DateTime+10,Datesys.second,2);
				}

				if(StructInfoPayment.Transaction_Amount == 0)
				{
					StructInfoPayment.Transaction_Amount = StructInfoPayment.Transaction_Amount;
				}
				StructInfoPayment.indexClessMag = 1;
			}
			else if(memcmp(StructInfoPayment.Expired_Date,&StructInfoPayment.Normal_Iso2[LgCarte+2],4))
			{
				memcpy(StructInfoPayment.Expired_Date,&StructInfoPayment.Normal_Iso2[LgCarte+2],4);
			}

		}

		if(!strlen(StructInfoPayment.Nom_Prenom))
		{
			sprintf(StructInfoPayment.Nom_Prenom,"%s",StructInfoPayment.Appli_label);
		}


		if(pValueClessCap!=NULL)
		{
			champs55[l_champs55++] =0x9F;
			champs55[l_champs55++] =0x6E;
			champs55[l_champs55++] =(UC)length_ClessCap;
			memcpy(&champs55[l_champs55],pValueClessCap,length_ClessCap);
			l_champs55 += length_ClessCap;
		}


		/*
		APEMV_UI_MessageDisplayDelay ("-.Appli_aid", 1);
		APEMV_UI_MessageDisplayDelay (StructInfoPayment.Appli_aid, 1);
		APEMV_UI_MessageDisplayDelay ("-.Expired_Date", 1);
		APEMV_UI_MessageDisplayDelay (StructInfoPayment.Expired_Date, 1);
		APEMV_UI_MessageDisplayDelay ("-.Holder_Card+1", 1);
		APEMV_UI_MessageDisplayDelay (StructInfoPayment.Holder_Card+1, 1);
		APEMV_UI_MessageDisplayDelay ("-.PanSeqNumber", 1);
		APEMV_UI_MessageDisplayDelay (StructInfoPayment.PanSeqNumber, 1);
		APEMV_UI_MessageDisplayDelay ("-.Devise_acc.code_num", 1);
		APEMV_UI_MessageDisplayDelay (StructInfoPayment.Devise_acc.code_num, 1);
		APEMV_UI_MessageDisplayDelay ("-.Devise_acc.Exposant", 1);
		APEMV_UI_MessageDisplayDelay (&StructInfoPayment.Devise_acc.Exposant, 1);
		APEMV_UI_MessageDisplayDelay ("-.Appli_label", 1);
		APEMV_UI_MessageDisplayDelay (StructInfoPayment.Appli_label, 1);
		APEMV_UI_MessageDisplayDelay ("-.Nom_Prenom", 1);
		APEMV_UI_MessageDisplayDelay (StructInfoPayment.Nom_Prenom, 1);
		APEMV_UI_MessageDisplayDelay ("-.Transaction_DateTime", 1);
		APEMV_UI_MessageDisplayDelay (StructInfoPayment.Transaction_DateTime, 1);
		APEMV_UI_MessageDisplayDelay ("-.Transaction_DateTime+6", 1);
		APEMV_UI_MessageDisplayDelay (StructInfoPayment.Transaction_DateTime+6, 1);
		*/


		StructInfoPayment.l_champ55 = l_champs55;
		memcpy(StructInfoPayment.Champ55,champs55,l_champs55);


/*		char champsascii[513];

		GTL_Convert_DcbNumberToAscii(StructInfoPayment.Champ55, champsascii, 256, 512);
		APEMV_UI_MessageDisplayDelay(champsascii,10);
		APEMV_UI_MessageDisplayDelay(champsascii+128,10);
		APEMV_UI_MessageDisplayDelay(champsascii+256,10);
*/


}

