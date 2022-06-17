//! \file
//! Module that manages all the application menus

/////////////////////////////////////////////////////////////////
//// Includes ///////////////////////////////////////////////////

#include "ClessSample_Implementation.h"


/////////////////////////////////////////////////////////////////
//// Macros & preprocessor definitions //////////////////////////

#define DISK_NAME_SIZE			33			/*!< Size of disk name. */


/////////////////////////////////////////////////////////////////
//// Types //////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////
//// Global variables ///////////////////////////////////////////

static int g_TransactionLoop = FALSE;			/*!< Global data indicating if transaction loop is enabled or not (currently not used). */
static int g_DumpTransactionData = TRUE;		/*!< Global data indicating if transaction data dumping is enabled or not. */
static int g_DumpTransactionDatabase = FALSE;	/*!< Global data indicating if transaction database dumping is enabled or not. */
static int g_TracesActivated = FALSE;			/*!< Global data indicating if transaction traces (using trace tool) is enabled or not. */
static int g_VisaEuropeModeEnabled = FALSE;		/*!< Global data indicating if Visa Europe Mode is enabled or not. */
static int g_EnableInputTransactionType = FALSE;/*!< Global data indicating if TransactionType information must be inputed before each transaction. */
static int g_MerchantForcedOnline = FALSE;		/*!< Global data indicating if merchant forced transaction online (PayPass only). */
static int g_DumpApduDataExchange = TRUE;		/*!< Global data indicating if APDU and Data Exchange dumping is enabled or not. */
static int g_AutomaticCleanTorn = FALSE;		/*!< Global data indicating if automatic CLEAN of torn log is enabled or not. */

#ifdef __TELIUM3__
#define RESOURCE_FILE		"822449"
#else
#define RESOURCE_FILE		"844084"
#endif

//! \brief Idle menu.
static const T_CLESS_SAMPLE_HELPER_MENU_ITEM IdleMenu[] = {
	{STD_MESS_MENU_STD_TXN, "file://param/" RESOURCE_FILE "----.TAR/icons/CU_DEBIT.wgu"},
	{STD_MESS_MENU_SPECIFIC_TXN, "file://param/" RESOURCE_FILE "----.TAR/icons/CU_DEBIT.wgu"},
	{STD_MESS_MENU_CLEAN_TORN, "file://param/" RESOURCE_FILE "----.TAR/icons/CL_BATCH.wgu"},
	{STD_MESS_MENU_MENU, "file://param/" RESOURCE_FILE "----.TAR/icons/CU_MERCHANT.wgu"},
};

//! \brief Main menu.
static const T_CLESS_SAMPLE_HELPER_MENU_ITEM MainMenu[] = {
	{STD_MESS_MENU_DO_TRANSACTION, "file://param/" RESOURCE_FILE "----.TAR/icons/CU_DEBIT.wgu"},
	{STD_MESS_MENU_MAIN_TEST, "file://param/" RESOURCE_FILE "----.TAR/icons/CU_TOOLWARE.wgu"},
	{STD_MESS_MENU_MAIN_PARAMETERS, "file://param/" RESOURCE_FILE "----.TAR/icons/CL_PARAMS.wgu"},
	{STD_MESS_MENU_MAIN_MERCHANT, "file://param/" RESOURCE_FILE "----.TAR/icons/CU_MERCHANT.wgu"},
	{STD_MESS_MENU_MAIN_BATCH, "file://param/" RESOURCE_FILE "----.TAR/icons/CL_BATCH.wgu"},
	{STD_MESS_MENU_TORN_LOG, "file://param/" RESOURCE_FILE "----.TAR/icons/CL_BATCH.wgu"},
	{STD_MESS_MENU_DATA_EXCHANGE, "file://param/" RESOURCE_FILE "----.TAR/icons/CL_PARAMS.wgu"}
};

//! \brief Transaction Type menu.
static const T_CLESS_SAMPLE_HELPER_MENU_ITEM TransactionTypeMenu[] = {
	{STD_MESS_MENU_MAIN_DEBIT, "file://param/" RESOURCE_FILE "----.TAR/icons/CU_DEBIT.wgu"},
	{STD_MESS_MENU_MAIN_CASH, "file://param/" RESOURCE_FILE "----.TAR/icons/CU_CREDIT.wgu"},
	{STD_MESS_MENU_MAIN_REFUND, "file://param/" RESOURCE_FILE "----.TAR/icons/CU_CREDIT.wgu"},
	{STD_MESS_MENU_MAIN_CASHBACK, "file://param/" RESOURCE_FILE "----.TAR/icons/CU_CREDIT.wgu"},
	{STD_MESS_MENU_MAIN_MANUAL_CASH, "file://param/" RESOURCE_FILE "----.TAR/icons/CU_CREDIT.wgu"},
	{STD_MESS_MENU_MAIN_CASH_DEPOSIT, "file://param/" RESOURCE_FILE "----.TAR/icons/CU_CREDIT.wgu"},
	{STD_MESS_MENU_MAIN_CASH_DISBURSMENT, "file://param/" RESOURCE_FILE "----.TAR/icons/CU_CREDIT.wgu"},
	{STD_MESS_MENU_MAIN_TTYPE_UNKNOWN, "file://param/" RESOURCE_FILE "----.TAR/icons/CU_CREDIT.wgu"},
};

//! \brief Batch menu.
static const T_CLESS_SAMPLE_HELPER_MENU_ITEM BatchMenu[] = {
	{STD_MESS_MENU_PARAMS_BATCH_TOTALS, "file://param/" RESOURCE_FILE "----.TAR/icons/CU_DEFAULT.wgu"},
	{STD_MESS_MENU_PARAMS_BATCH_PRINT_LOG, "file://param/" RESOURCE_FILE "----.TAR/icons/CU_DEFAULT.wgu"},
	{STD_MESS_MENU_PARAMS_BATCH_CLEAR, "file://param/" RESOURCE_FILE "----.TAR/icons/CU_DEFAULT.wgu"},
	{STD_MESS_MENU_PARAMS_BATCH_LAST_TR, "file://param/" RESOURCE_FILE "----.TAR/icons/CU_DEFAULT.wgu"}
};

//! \brief Torn log menu.
static const T_CLESS_SAMPLE_HELPER_MENU_ITEM TornLogMenu[] = {
	{STD_MESS_MENU_PARAMS_TORN_PRINT_LOG, "file://param/" RESOURCE_FILE "----.TAR/icons/CU_DEFAULT.wgu"},
	{STD_MESS_MENU_PARAMS_TORN_CLEAN_LOG, "file://param/" RESOURCE_FILE "----.TAR/icons/CU_DEFAULT.wgu"},
	{STD_MESS_MENU_PARAMS_TORN_AUTO_CLEAN_LOG, "file://param/" RESOURCE_FILE "----.TAR/icons/CU_DEFAULT.wgu"},
};

//! \brief Test menu.
static const T_CLESS_SAMPLE_HELPER_MENU_ITEM TestMenu[] = {
	{STD_MESS_MENU_TEST_DUMP_TR, "file://param/" RESOURCE_FILE "----.TAR/icons/CU_DEFAULT.wgu"},
	{STD_MESS_MENU_TEST_DUMP_DB, "file://param/" RESOURCE_FILE "----.TAR/icons/CU_DEFAULT.wgu"},
	{STD_MESS_MENU_TEST_LOOP, "file://param/" RESOURCE_FILE "----.TAR/icons/CU_DEFAULT.wgu"},
	{STD_MESS_MENU_TEST_CHANGE_OUTPUT, "file://param/" RESOURCE_FILE "----.TAR/icons/CU_DEFAULT.wgu"},
	{STD_MESS_MENU_TEST_TRACES, "file://param/" RESOURCE_FILE "----.TAR/icons/CU_DEFAULT.wgu"},
	{STD_MESS_MENU_TEST_INPUT_TRANSACTION_TYPE, "file://param/" RESOURCE_FILE "----.TAR/icons/CU_DEFAULT.wgu"},
	{STD_MESS_MENU_TEST_DUMP_APDU_DE, "file://param/" RESOURCE_FILE "----.TAR/icons/CU_DEFAULT.wgu"},
};

//! \brief Menu for output data change (with printer).
static const T_CLESS_SAMPLE_HELPER_MENU_ITEM TestMenuWithPrinter[] = {
	{STD_MESS_MENU_OUTPUT_COM0, "file://param/" RESOURCE_FILE "----.TAR/icons/CU_DEFAULT.wgu"},
	{STD_MESS_MENU_OUTPUT_USB, "file://param/" RESOURCE_FILE "----.TAR/icons/CU_DEFAULT.wgu"},
	{STD_MESS_MENU_OUTPUT_NONE, "file://param/" RESOURCE_FILE "----.TAR/icons/CU_DEFAULT.wgu"},
	{STD_MESS_MENU_OUTPUT_PRINTER, "file://param/" RESOURCE_FILE "----.TAR/icons/CU_DEFAULT.wgu"}
};

//! \brief Menu for output data change (without printer).
static const T_CLESS_SAMPLE_HELPER_MENU_ITEM TestMenuWithoutPrinter[] = {
	{STD_MESS_MENU_OUTPUT_COM0, "file://param/" RESOURCE_FILE "----.TAR/icons/CU_DEFAULT.wgu"},
	{STD_MESS_MENU_OUTPUT_USB, "file://param/" RESOURCE_FILE "----.TAR/icons/CU_DEFAULT.wgu"},
	{STD_MESS_MENU_OUTPUT_NONE, "file://param/" RESOURCE_FILE "----.TAR/icons/CU_DEFAULT.wgu"}
};

//! \brief Parameters menu.
static const T_CLESS_SAMPLE_HELPER_MENU_ITEM ParametersMenu[] = {
	{STD_MESS_MENU_PARAMS_DUMP, "file://param/" RESOURCE_FILE "----.TAR/icons/CU_DUMP.wgu"},
	{STD_MESS_MENU_PARAMS_DEFAULT, "file://param/" RESOURCE_FILE "----.TAR/icons/CU_DEF_CNF.wgu"},
	{STD_MESS_MENU_PARAMS_ERASE_ALL, "file://param/" RESOURCE_FILE "----.TAR/icons/CL_PARAMDEL.wgu"},
	{STD_MESS_MENU_PARAMS_SELECT_FILE, "file://param/" RESOURCE_FILE "----.TAR/icons/CU_DEFAULT.wgu"}
};

//! \brief Parameters dump menu.
static const T_CLESS_SAMPLE_HELPER_MENU_ITEM ParametersMenuDump[] = {
	{STD_MESS_MENU_PARAMS_DUMP_ID, "file://param/" RESOURCE_FILE "----.TAR/icons/CU_DEFAULT.wgu"},
	{STD_MESS_MENU_PARAMS_DUMP_AIDS, "file://param/" RESOURCE_FILE "----.TAR/icons/CU_DEFAULT.wgu"},
	{STD_MESS_MENU_PARAMS_DUMP_CA, "file://param/" RESOURCE_FILE "----.TAR/icons/CU_DEFAULT.wgu"},
	{STD_MESS_MENU_PARAMS_DUMP_GLOBALS, "file://param/" RESOURCE_FILE "----.TAR/icons/CU_DEFAULT.wgu"},
	{STD_MESS_MENU_PARAMS_DUMP_KERNEL_SPECIFIC, "file://param/" RESOURCE_FILE "----.TAR/icons/CU_DEFAULT.wgu"},
	{STD_MESS_MENU_PARAMS_DUMP_ALL, "file://param/" RESOURCE_FILE "----.TAR/icons/CU_DEFAULT.wgu"}
};

//! \brief Merchant menu.
static const T_CLESS_SAMPLE_HELPER_MENU_ITEM MerchantMenu[] = {
	{STD_MESS_MENU_PARAMS_MERCHANT_FORCE_ONLINE, "file://param/" RESOURCE_FILE "----.TAR/icons/CU_DEFAULT.wgu"},
	{STD_MESS_MENU_PARAMS_MERCHANT_RESET_TSC, "file://param/" RESOURCE_FILE "----.TAR/icons/CU_DEFAULT.wgu"}
};

//! \brief Parameters dump menu.
static const T_CLESS_SAMPLE_HELPER_MENU_ITEM DataExchangeMenu[] = {
	{STD_MESS_MENU_PARAMS_DUMP, "file://param/" RESOURCE_FILE "----.TAR/icons/CU_DUMP.wgu"},
	{STD_MESS_MENU_PARAMS_ERASE_ALL, "file://param/" RESOURCE_FILE "----.TAR/icons/CL_PARAMDEL.wgu"},
	{STD_MESS_MENU_PARAMS_DUMP_ID, "file://param/" RESOURCE_FILE "----.TAR/icons/CU_DEFAULT.wgu"},
	{STD_MESS_MENU_PARAMS_SELECT_FILE, "file://param/" RESOURCE_FILE "----.TAR/icons/CU_DEFAULT.wgu"}
};


/////////////////////////////////////////////////////////////////
//// Static function definitions ////////////////////////////////

static int __ClessSample_Menu_DoExplicitTransaction (int bUseDefaultAmount);
static int __ClessSample_Menu_BatchTotals(void);
static int __ClessSample_Menu_BatchPrintLog(void);
static int __ClessSample_Menu_BatchClear(void);
static int __ClessSample_Menu_BatchPrintLastTransaction(void);
static int __ClessSample_Menu_Batch(void);
static int __ClessSample_Menu_Test (void);
static int __ClessSample_Menu_Parameters (void);
static int __ClessSample_Menu_DefaultParameters(void);
static int __ClessSample_Menu_EraseParameters (void);
static int __ClessSample_Menu_ParametersDump (void);
static int __ClessSample_Menu_TestChooseOutput (void);
static int __ClessSample_Menu_MerchantForcedOnline(void);
static int __ClessSample_Menu_Merchant (void);
static int __ClessSample_Menu_DumpDataExchangeParams (void);
static int __ClessSample_Menu_DumpDataExchangeParamsInfos (void);
static int __ClessSample_Menu_DataExchange(void);

extern int IsPrinter(void);

/////////////////////////////////////////////////////////////////
//// Functions //////////////////////////////////////////////////

//##################################################################
//################### DEBIT MENU ###################################
//##################################################################

//! \brief Starts the payment if transaction is explicitly chosen in the application menu (explicit selection transaction).
//! \return Always -1.

static int __ClessSample_Menu_DoExplicitTransaction (int bUseDefaultAmount)
{
	ClessSample_ExplicitSelection_Process (FALSE, bUseDefaultAmount);
	return -1;
}



//##################################################################
//################### BATCH MENU ###################################
//##################################################################

//! \brief Action to do if TOTALS is selected in the Batch menu
//! \return Always -1.

static int __ClessSample_Menu_BatchTotals(void)
{
	// Print a receipt with the total of the transactions in the batch
	ClessSample_Batch_PrintTotals();
	return -1;
}



//! \brief Action to do if PRINT LOG is selected in the Batch menu
//! \return Always -1.

static int __ClessSample_Menu_BatchPrintLog(void)
{
	// Print the batch content
	ClessSample_Batch_PrintBatchOverview();
	return -1;
}



//! \brief Action to do if PRINT LOG is selected in the Batch menu
//! \return Always -1.

static int __ClessSample_Menu_BatchPrintLastTransaction(void)
{
	// Print the last transaction saved in the batch
	ClessSample_Batch_PrintLastTransaction();
	return -1;
}



//! \brief  Action to do if UPLOAD is selected in the Batch menu
//! \return Always -1.

static int __ClessSample_Menu_BatchClear(void)
{
	// Updload the batch content and empty it
	ClessSample_Batch_Destroy();
	return -1;
}



//! \brief Batch menu management
//! \return Always -1.

static int __ClessSample_Menu_Batch(void)
{
	int nChoice;
	int nResult;

	nChoice = 0;
	do
	{
		nChoice = HelperDisplayMenu("BATCH", nChoice, NUMBER_OF_ITEMS(BatchMenu), BatchMenu, FALSE);
		if (nChoice >= 0)
		{
			switch(nChoice)
			{
			case 0:
				nResult = __ClessSample_Menu_BatchTotals();
				break;
			case 1:
				nResult = __ClessSample_Menu_BatchPrintLog();
				break;
			case 2:
				nResult = __ClessSample_Menu_BatchClear();
				break;
			case 3:
				nResult = __ClessSample_Menu_BatchPrintLastTransaction();
				break;
			default:
				nResult = 0;
				break;
			}

			if (nResult == -1)
				nChoice = -1;
		}
	} while(nChoice >= 0);

	return nChoice;
}



//##################################################################
//#################### TORN MENU ###################################
//##################################################################

//! \brief Action to do if PRINT LOG is selected in the Torn Log menu
//! \return Always -1.

static int __ClessSample_Menu_TornPrintLog(void)
{
	ClessSample_Torn_PrintLog();
	return -1;
}



//! \brief  Action to do if CLEAN is selected in the Torn Log menu
//! \return Always -1.

static int __ClessSample_Menu_TornCleanLog(void)
{
	ClessSample_Torn_CleanLog();
	ClessSample_DumpData_DumpTitle ("Torn log cleaned");

	return -1;
}



//! \brief  Action to do if AUTO CLEAN is selected in the Torn Log menu
//! \return Always -1.

int ClessSample_Menu_TornAutoCleanLog(void)
{
	MSGinfos tMsg;
	int lg_code;

	// Get the manager language (merchant language)
	lg_code = PSQ_Give_Language();

	ClessSample_Term_Read_Message(STD_MESS_MENU_PARAMS_TORN_AUTO_CLEAN_LOG, lg_code, &tMsg);
	Helper_DisplayTextMerchant(ERASE, HELPERS_MERCHANT_LINE_2, &tMsg, LEDSOFF);
		
	if (g_AutomaticCleanTorn == FALSE)
	{
		g_AutomaticCleanTorn = TRUE;
		ClessSample_Term_Read_Message(STD_MESS_ENABLE, lg_code, &tMsg);
		Helper_DisplayTextMerchant(NO_ERASE, HELPERS_MERCHANT_LINE_3, &tMsg, LEDSOFF);
	}
	else
	{
		g_AutomaticCleanTorn = FALSE;
		ClessSample_Term_Read_Message(STD_MESS_DISABLE, lg_code, &tMsg);
		Helper_DisplayTextMerchant(NO_ERASE, HELPERS_MERCHANT_LINE_3, &tMsg, LEDSOFF);
	}
	
#ifndef DISABLE_UNATTENDED
	if (ClessSample_Unattended_IsUnattendedMechanism())
		Helper_RefreshScreen(NOWAIT, HELPERS_MERCHANT_SCREEN);
	else
#endif
		Helper_RefreshScreen(WAIT, HELPERS_MERCHANT_SCREEN);

	return -1;
}



//! \brief Torn log menu management
//! \return Always -1.

static int __ClessSample_Menu_TornLog(void)
{
	int nChoice;
	int nResult;

	nChoice = 0;
	do
	{
		nChoice = HelperDisplayMenu("TORN LOG", nChoice, NUMBER_OF_ITEMS(TornLogMenu), TornLogMenu, FALSE);
		if (nChoice >= 0)
		{
			switch(nChoice)
			{
			case 0:
				nResult = __ClessSample_Menu_TornPrintLog();
				break;
			case 1:
				nResult = __ClessSample_Menu_TornCleanLog();
				break;
			case 2:
				nResult = ClessSample_Menu_TornAutoCleanLog();
				break;
			default:
				nResult = 0;
				break;
			}

			if (nResult == -1)
				nChoice = -1;
		}
	} while(nChoice >= 0);

	return nChoice;
}



//##################################################################
//#################### DATA EXCHANGE MENU ##########################
//##################################################################

//! \brief Action to do if DUMP is selected in the Data Exchange menu
//! \return Always -1.

static int __ClessSample_Menu_DumpDataExchangeParams (void)
{
	ClessSample_DataExchange_DumpFile (pTreeCurrentDataExchange);
	return (-1);
}



//! \brief Action to do if ERASE is selected in the Data Exchange menu
//! \return Always -1.

int ClessSample_Menu_EraseDataExchangeParams (void)
{
	MSGinfos tMsg, tGlobalMsg;
	int lg_code;

	// Get the manager language (merchant language)
	lg_code = PSQ_Give_Language();

	ClessSample_Term_Read_Message(STD_MESS_ERASING_PARAMS, lg_code, &tGlobalMsg);
	Helper_DisplayTextMerchant(ERASE, HELPERS_MERCHANT_LINE_2, &tGlobalMsg, LEDSOFF);
	ClessSample_Term_Read_Message(STD_MESS_IN_PROGRESS, lg_code, &tMsg);
	Helper_DisplayTextMerchant(NO_ERASE, HELPERS_MERCHANT_LINE_3, &tMsg, LEDSOFF);
	Helper_RefreshScreen(NOWAIT, HELPERS_MERCHANT_SCREEN);

	ClessSample_DataExchange_InitTlvTree(&pTreeCurrentDataExchange); // Reset TLVTree parameters
	CS_Settings_DeleteSavedSelectedFileName("MCW_DEKDET");
	if (ClessSample_DataExchange_SaveFile (pTreeCurrentDataExchange) != 0) // Save dummy TLVTree parameters in file
	{
		Helper_DisplayTextMerchant(ERASE, HELPERS_MERCHANT_LINE_2, &tGlobalMsg, LEDSOFF);
		ClessSample_Term_Read_Message(STD_MESS_ERROR, lg_code, &tMsg);
		Helper_DisplayTextMerchant(NO_ERASE, HELPERS_MERCHANT_LINE_3, &tMsg, LEDSOFF);
	}
	else
	{
		Helper_DisplayTextMerchant(ERASE, HELPERS_MERCHANT_LINE_2, &tGlobalMsg, LEDSOFF);
		ClessSample_Term_Read_Message(STD_MESS_COMPLETE, lg_code, &tMsg);
		Helper_DisplayTextMerchant(NO_ERASE, HELPERS_MERCHANT_LINE_3, &tMsg, LEDSOFF);
	}

	Helper_RefreshScreen(WAIT, HELPERS_MERCHANT_SCREEN);
	
	return (-1);
}



//!	\brief Action to do if IDENTIFIER is selected in the Data Exchange menu

static int __ClessSample_Menu_DumpDataExchangeParamsInfos (void)
{
	ClessSample_DataExchange_DumpEvalHeaderInformation (pTreeCurrentDataExchange);
	return (-1);
}


/**
 * Selects and load the DEKDET.PAR file.
 * @return Always (-1).
 */
int ClessSample_Menu_SelectDataExchangeParamsFile (void)
{
	T_NAMES_LIST myList;
	int selectedItem;
	const char * selectedFile;
	int savedItemIndex;

	memset (&myList, 0, sizeof(T_NAMES_LIST));
	CS_NameList_Init(&myList, 0);

	if (CS_Settings_getListOfFiles("MCW_DEKDET", &myList))
	{
		savedItemIndex = CS_Settings_GetIndexOfSavedParameterFile("MCW_DEKDET");

		ClessSample_Goal_Open();
		selectedItem = GL_Dialog_Menu (ClessSample_Goal_Handle(), "MCW_DEKDET", (const char **)myList.pListOfNames, savedItemIndex, GL_BUTTON_DEFAULT, GL_KEY_0, GL_TIME_INFINITE);
		if (selectedItem != GL_KEY_CANCEL)
		{
			selectedFile = myList.pListOfNames[selectedItem];

			if (CS_Settings_loadFromFile("MCW_DEKDET", selectedFile, &pTreeCurrentDataExchange, SettingsDecoderDekDet, ".PAR"))
			{
				CS_Settings_SaveSelectedFileName("MCW_DEKDET", selectedFile);
			}
		}
		ClessSample_Goal_Close();
	}

	CS_NameList_Init(&myList, 0);


	return (-1);
}




//! \brief Torn log menu management
//! \return Always -1.

static int __ClessSample_Menu_DataExchange(void)
{
	int nChoice;
	int nResult;

	nChoice = 0;
	do
	{
		nChoice = HelperDisplayMenu("DATA EXCHANGE", nChoice, NUMBER_OF_ITEMS(DataExchangeMenu), DataExchangeMenu, FALSE);
		if (nChoice >= 0)
		{
			switch(nChoice)
			{
			case 0:
				nResult = __ClessSample_Menu_DumpDataExchangeParams();
				break;
			case 1:
				nResult = ClessSample_Menu_EraseDataExchangeParams();
				break;
			case 2:
				nResult = __ClessSample_Menu_DumpDataExchangeParamsInfos();
				break;
			case 3:
				nResult = ClessSample_Menu_SelectDataExchangeParamsFile();
				break;
			default:
				nResult = 0;
				break;
			}

			if (nResult == -1)
				nChoice = -1;
		}
	} while(nChoice >= 0);

	return nChoice;
}



//##################################################################
//################### TEST MENU ####################################
//##################################################################

//! \brief Action to do if DUMP TR DATA is selected in the TEST menu
//! \return Always -1.

int ClessSample_Menu_TestTransactionData(void)
{
	MSGinfos tMsg;
	int lg_code;

	// Get the manager language (merchant language)
	lg_code = PSQ_Give_Language();

	ClessSample_Term_Read_Message(STD_MESS_MENU_TEST_DUMP_TR, lg_code, &tMsg);
	Helper_DisplayTextMerchant(ERASE, HELPERS_MERCHANT_LINE_2, &tMsg, LEDSOFF);
		
	if (g_DumpTransactionData == FALSE)
	{
		g_DumpTransactionData = TRUE;
		ClessSample_Term_Read_Message(STD_MESS_ENABLE, lg_code, &tMsg);
		Helper_DisplayTextMerchant(NO_ERASE, HELPERS_MERCHANT_LINE_3, &tMsg, LEDSOFF);
	}
	else
	{
		g_DumpTransactionData = FALSE;
		ClessSample_Term_Read_Message(STD_MESS_DISABLE, lg_code, &tMsg);
		Helper_DisplayTextMerchant(NO_ERASE, HELPERS_MERCHANT_LINE_3, &tMsg, LEDSOFF);
	}
	
#ifndef DISABLE_UNATTENDED
	if (ClessSample_Unattended_IsUnattendedMechanism())
		Helper_RefreshScreen(NOWAIT, HELPERS_MERCHANT_SCREEN);
	else
#endif
		Helper_RefreshScreen(WAIT, HELPERS_MERCHANT_SCREEN);

	return -1;
}



//! \brief Action to do if "DUMP DATA DB" is selected in the TEST menu (it enables/diables the dabase dumping).
//! \return Always -1.

int ClessSample_Menu_TestTransactionDumpDatabase(void)
{
	MSGinfos tMsg;
	int lg_code;

	// Get the manager language (merchant language)
	lg_code = PSQ_Give_Language();

	ClessSample_Term_Read_Message(STD_MESS_MENU_TEST_DUMP_DB, lg_code, &tMsg);
	Helper_DisplayTextMerchant(ERASE, HELPERS_MERCHANT_LINE_2, &tMsg, LEDSOFF);
		
	if (g_DumpTransactionDatabase == FALSE)
	{
		g_DumpTransactionDatabase = TRUE;
		ClessSample_Term_Read_Message(STD_MESS_ENABLE, lg_code, &tMsg);
		Helper_DisplayTextMerchant(NO_ERASE, HELPERS_MERCHANT_LINE_3, &tMsg, LEDSOFF);
	}
	else
	{
		g_DumpTransactionDatabase = FALSE;
		ClessSample_Term_Read_Message(STD_MESS_DISABLE, lg_code, &tMsg);
		Helper_DisplayTextMerchant(NO_ERASE, HELPERS_MERCHANT_LINE_3, &tMsg, LEDSOFF);
	}
	
#ifndef DISABLE_UNATTENDED
	if (ClessSample_Unattended_IsUnattendedMechanism())
		Helper_RefreshScreen(NOWAIT, HELPERS_MERCHANT_SCREEN);
	else
#endif
		Helper_RefreshScreen(WAIT, HELPERS_MERCHANT_SCREEN);

	return -1;
}



//! \brief Action to do if "DUMP APDU DE" is selected in the TEST menu (it enables/diables the APDU and DE dumping).
//! \return Always -1.

int ClessSample_Menu_TestTransactionDumpApduDataExchange(void)
{
	MSGinfos tMsg;
	int lg_code;

	// Get the manager language (merchant language)
	lg_code = PSQ_Give_Language();

	ClessSample_Term_Read_Message(STD_MESS_MENU_TEST_DUMP_APDU_DE, lg_code, &tMsg);
	Helper_DisplayTextMerchant(ERASE, HELPERS_MERCHANT_LINE_2, &tMsg, LEDSOFF);
		
	if (g_DumpApduDataExchange == FALSE)
	{
		g_DumpApduDataExchange = TRUE;
		ClessSample_Term_Read_Message(STD_MESS_ENABLE, lg_code, &tMsg);
		Helper_DisplayTextMerchant(NO_ERASE, HELPERS_MERCHANT_LINE_3, &tMsg, LEDSOFF);
	}
	else
	{
		g_DumpApduDataExchange = FALSE;
		ClessSample_Term_Read_Message(STD_MESS_DISABLE, lg_code, &tMsg);
		Helper_DisplayTextMerchant(NO_ERASE, HELPERS_MERCHANT_LINE_3, &tMsg, LEDSOFF);
	}
	
#ifndef DISABLE_UNATTENDED
	if (ClessSample_Unattended_IsUnattendedMechanism())
		Helper_RefreshScreen(NOWAIT, HELPERS_MERCHANT_SCREEN);
	else
#endif
		Helper_RefreshScreen(WAIT, HELPERS_MERCHANT_SCREEN);

	return -1;
}



//! \brief Choose the output peripheral to get the dumps (parameters or transaction data).
//! \return Always -1.

static int __ClessSample_Menu_TestChooseOutput (void)
{
	int nChoice;
	int nResult;

	nChoice = 0;
	nResult = 0;

	do
	{
		if (!IsPrinter()) // IsPrinter() is not yet provided by the simulator
			nChoice = HelperDisplayMenu("OUTPUT", nChoice, NUMBER_OF_ITEMS(TestMenuWithoutPrinter), TestMenuWithoutPrinter, FALSE);
		else
			nChoice = HelperDisplayMenu("OUTPUT", nChoice, NUMBER_OF_ITEMS(TestMenuWithPrinter), TestMenuWithPrinter, FALSE);

		if (nChoice >= 0)
		{
			switch(nChoice)
			{
			case 0: // COM0
				ClessSample_DumpData_DumpSetOutputId (CUSTOM_OUTPUT_COM0);
				nResult = -1;
				break;

			case 1: // USB
				ClessSample_DumpData_DumpSetOutputId (CUSTOM_OUTPUT_USB);
				nResult = -1;
				break;

			case 2: // NONE
				ClessSample_DumpData_DumpSetOutputId (CUSTOM_OUTPUT_NONE);
				nResult = -1;
				break;

			case 3: //Printer (if present)
				if (IsPrinter())
				{
					ClessSample_DumpData_DumpSetOutputId (CUSTOM_OUTPUT_PRINTER);
					nResult = -1;
				}					
				else
				{
					nResult = 0;
				}
				break;

			default:
				nResult = 0;
				break;
			}

			if (nResult == -1)
				nChoice = -1;
		}
	} while(nChoice >= 0);

	return nChoice;
}



//! \brief Action to do if TRANSAC LOOP is selected in the TEST menu
//! \return Always -1.

int ClessSample_Menu_TestTransactionLoop(void)
{
	MSGinfos tMsg;
	int lg_code;

	// Get the manager language (merchant language)
	lg_code = PSQ_Give_Language();

	ClessSample_Term_Read_Message(STD_MESS_MENU_TEST_LOOP, lg_code, &tMsg);
	Helper_DisplayTextMerchant(ERASE, HELPERS_MERCHANT_LINE_2, &tMsg, LEDSOFF);
		
	if (g_TransactionLoop == FALSE)
	{
		g_TransactionLoop = TRUE;
		ClessSample_Term_Read_Message(STD_MESS_ENABLE, lg_code, &tMsg);
		Helper_DisplayTextMerchant(NO_ERASE, HELPERS_MERCHANT_LINE_3, &tMsg, LEDSOFF);
	}
	else
	{
		g_TransactionLoop = FALSE;
		ClessSample_Term_Read_Message(STD_MESS_DISABLE, lg_code, &tMsg);
		Helper_DisplayTextMerchant(NO_ERASE, HELPERS_MERCHANT_LINE_3, &tMsg, LEDSOFF);
	}

#ifndef DISABLE_UNATTENDED
	if (ClessSample_Unattended_IsUnattendedMechanism())
		Helper_RefreshScreen(NOWAIT, HELPERS_MERCHANT_SCREEN);
	else
#endif
		Helper_RefreshScreen(WAIT, HELPERS_MERCHANT_SCREEN);

	return -1;
}



//! \brief Action to do if TRACES is selected in the TEST menu
//! \return Always -1.

int ClessSample_Menu_TestTraces(void)
{
	MSGinfos tMsg;
	int lg_code;

	// Get the manager language (merchant language)
	lg_code = PSQ_Give_Language();

	ClessSample_Term_Read_Message(STD_MESS_MENU_TEST_TRACES, lg_code, &tMsg);
	Helper_DisplayTextMerchant(ERASE, HELPERS_MERCHANT_LINE_2, &tMsg, LEDSOFF);

	if (g_TracesActivated == FALSE)
	{
		g_TracesActivated = TRUE;
		ClessSample_Term_Read_Message(STD_MESS_ENABLE, lg_code, &tMsg);
		Helper_DisplayTextMerchant(NO_ERASE, HELPERS_MERCHANT_LINE_3, &tMsg, LEDSOFF);
	}
	else
	{
		g_TracesActivated = FALSE;
		ClessSample_Term_Read_Message(STD_MESS_DISABLE, lg_code, &tMsg);
		Helper_DisplayTextMerchant(NO_ERASE, HELPERS_MERCHANT_LINE_3, &tMsg, LEDSOFF);
	}
	
#ifndef DISABLE_UNATTENDED
	if (ClessSample_Unattended_IsUnattendedMechanism())
		Helper_RefreshScreen(NOWAIT, HELPERS_MERCHANT_SCREEN);
	else
#endif
		Helper_RefreshScreen(WAIT, HELPERS_MERCHANT_SCREEN);
	
	// Debug features management
	ClessSample_PayPass_DebugActivation (g_TracesActivated);

	ClessSample_payWave_DebugActivation (g_TracesActivated);
	ClessSample_VisaWave_DebugActivation (g_TracesActivated);
#ifndef DISABLE_OTHERS_KERNELS_EXPRESS
	ClessSample_ExpressPay_DebugActivation (g_TracesActivated);
	ClessSample_Discover_DebugActivation (g_TracesActivated);
    ClessSample_Interac_DebugActivation (g_TracesActivated);
#endif

	return -1;
}



//! \brief Action enable/disable "TransactionType" information to input.
//! \return Always -1.

int ClessSample_Menu_TestInputTransactionType(void)
{
	MSGinfos tMsg;
	int lg_code;

	// Get the manager language (merchant language)
	lg_code = PSQ_Give_Language();

	ClessSample_Term_Read_Message(STD_MESS_MENU_TEST_INPUT_TRANSACTION_TYPE, lg_code, &tMsg);
	Helper_DisplayTextMerchant(ERASE, HELPERS_MERCHANT_LINE_2, &tMsg, LEDSOFF);


    if (g_EnableInputTransactionType == FALSE)
    {
        g_EnableInputTransactionType = TRUE;
		ClessSample_Term_Read_Message(STD_MESS_ENABLE, lg_code, &tMsg);
		Helper_DisplayTextMerchant(NO_ERASE, HELPERS_MERCHANT_LINE_3, &tMsg, LEDSOFF);
    }
    else
    {
        g_EnableInputTransactionType = FALSE;
		ClessSample_Term_Read_Message(STD_MESS_DISABLE, lg_code, &tMsg);
		Helper_DisplayTextMerchant(NO_ERASE, HELPERS_MERCHANT_LINE_3, &tMsg, LEDSOFF);
    }
    
#ifndef DISABLE_UNATTENDED
	if (ClessSample_Unattended_IsUnattendedMechanism())
		Helper_RefreshScreen(NOWAIT, HELPERS_MERCHANT_SCREEN);
	else
#endif
		Helper_RefreshScreen(WAIT, HELPERS_MERCHANT_SCREEN);
		
	return -1;
}



//! \brief Test if EnableTransactionType is on.
//! \return
//!		- \ref TRUE if loop is activated.
//!		- \ref FALSE if not.
unsigned char ClessSample_Menu_IsEnableInputTransactionType (void)
{
	return (unsigned char)g_EnableInputTransactionType;
}	



//! \brief Test if transaction loop mode has been activated.
//! \return
//!		- \ref TRUE if loop is activated.
//!		- \ref FALSE if not.

unsigned char ClessSample_Menu_IsTransactionLoopOn (void)
{
	return (unsigned char)g_TransactionLoop;
}	



//! \brief Test if transaction data dumping is TRUE or FALSE.
//! \return
//!		- \ref TRUE if dump mode is activated.
//!		- \ref FALSE if not.

unsigned char ClessSample_Menu_IsTransactionDataDumpingOn (void)
{
	return (unsigned char)g_DumpTransactionData;
}	



//! \brief Test if transaction database dumping is TRUE or FALSE.
//! \return
//!		- \ref TRUE if database dumping is activated.
//!		- \ref FALSE if not.

unsigned char ClessSample_Menu_IsTransactionDatabaseDumpingOn (void)
{
	return (unsigned char)g_DumpTransactionDatabase;
}	



//! \brief Test if transaction APDU and Data Exchange dumping is TRUE or FALSE.
//! \return
//!		- \ref TRUE if dumping is activated.
//!		- \ref FALSE if not.

unsigned char ClessSample_Menu_IsApduDataExchangeDumpingOn (void)
{
	return (unsigned char)g_DumpApduDataExchange;
}	



//! \brief Test if Visa Europe Mode is activated or not.
//! \return
//!		- \ref TRUE if database dumping is activated.
//!		- \ref FALSE if not.

unsigned char ClessSample_Menu_IsVisaEuropeModeOn (void)
{
	return (unsigned char)g_VisaEuropeModeEnabled;
}	


//! \brief Test if automatic CLEAN of torn log has been activated.
//! \return
//!		- \ref TRUE if automatic CLEAN of torn log  is activated.
//!		- \ref FALSE if not.

unsigned char ClessSample_Menu_IsAutomaticCleanTornOn (void)
{
	return (unsigned char)g_AutomaticCleanTorn;
}	



//! \brief Test menu management
//! \return Always -1.

static int __ClessSample_Menu_Test(void)
{
	int nChoice;
	int nResult;

	nChoice = 0;
	do
	{
		nChoice = HelperDisplayMenu("TEST MENU", nChoice, NUMBER_OF_ITEMS(TestMenu), TestMenu, FALSE);
		if (nChoice >= 0)
		{
			switch(nChoice)
			{
			case 0:
				nResult = ClessSample_Menu_TestTransactionData();
				break;

			case 1:
				nResult = ClessSample_Menu_TestTransactionDumpDatabase();
				break;

			case 2:
				nResult = ClessSample_Menu_TestTransactionLoop();
				break;
				
			case 3:
				nResult = __ClessSample_Menu_TestChooseOutput();
				break;

			case 4:
				nResult = ClessSample_Menu_TestTraces();
				break;
				
			case 5:
				nResult = ClessSample_Menu_TestInputTransactionType();
				break;
				
			case 6:
				nResult = ClessSample_Menu_TestTransactionDumpApduDataExchange();
				break;

			default:
				nResult = 0;
				break;
			}

			if (nResult == -1)
				nChoice = -1;
		}
	} while(nChoice >= 0);

	return nChoice;
}



//##################################################################
//################### PARAMETERS MENU ##############################
//##################################################################

//! \brief Action read default parameters and update .SAV file with it
//! \return Always -1.

static int __ClessSample_Menu_DefaultParameters(void)
{

	MSGinfos tMsg, tGlobalMsg;
	int lg_code;

	// Get the manager language (merchant language)
	lg_code = PSQ_Give_Language();

	ClessSample_Term_Read_Message(STD_MESS_UPDATING_PARAMS, lg_code, &tGlobalMsg);
	Helper_DisplayTextMerchant(ERASE, HELPERS_MERCHANT_LINE_2, &tGlobalMsg, LEDSOFF);
	ClessSample_Term_Read_Message(STD_MESS_IN_PROGRESS, lg_code, &tMsg);
	Helper_DisplayTextMerchant(NO_ERASE, HELPERS_MERCHANT_LINE_3, &tMsg, LEDSOFF);
	Helper_RefreshScreen(NOWAIT, HELPERS_MERCHANT_SCREEN);

	// Read default parameters
	ClessSample_Parameters_ReadDefaultParameters(&pTreeCurrentParam);

	if (ClessSample_Parameters_SaveParam (pTreeCurrentParam) != 0) // Save dummy TLVTree parameters in file
	{
		Helper_DisplayTextMerchant(ERASE, HELPERS_MERCHANT_LINE_2, &tGlobalMsg, LEDSOFF);
		ClessSample_Term_Read_Message(STD_MESS_ERROR, lg_code, &tMsg);
		Helper_DisplayTextMerchant(NO_ERASE, HELPERS_MERCHANT_LINE_3, &tMsg, LEDSOFF);
	}
	else
	{
		Helper_DisplayTextMerchant(ERASE, HELPERS_MERCHANT_LINE_2, &tGlobalMsg, LEDSOFF);
		ClessSample_Term_Read_Message(STD_MESS_COMPLETE, lg_code, &tMsg);
		Helper_DisplayTextMerchant(NO_ERASE, HELPERS_MERCHANT_LINE_3, &tMsg, LEDSOFF);
	}

	Helper_RefreshScreen(WAIT, HELPERS_MERCHANT_SCREEN);
	
	// Check parameters CA keys
	ClessSample_Parameters_CheckCaKeysCrc(pTreeCurrentParam);

	return -1;
}



//! \brief Action to do if ERASE ALL is selected in the PARAMETERS menu. It erase all the application parameters.
//! \return Always -1.

static int __ClessSample_Menu_EraseParameters (void)
{
	MSGinfos tMsg, tGlobalMsg;
	int lg_code;

	// Get the manager language (merchant language)
	lg_code = PSQ_Give_Language();

	ClessSample_Term_Read_Message(STD_MESS_ERASING_PARAMS, lg_code, &tGlobalMsg);
	Helper_DisplayTextMerchant(ERASE, HELPERS_MERCHANT_LINE_2, &tGlobalMsg, LEDSOFF);
	ClessSample_Term_Read_Message(STD_MESS_IN_PROGRESS, lg_code, &tMsg);
	Helper_DisplayTextMerchant(NO_ERASE, HELPERS_MERCHANT_LINE_3, &tMsg, LEDSOFF);
	Helper_RefreshScreen(NOWAIT, HELPERS_MERCHANT_SCREEN);

	ClessSample_Parameters_InitParameters(&pTreeCurrentParam); // Reset TLVTree parameters
	CS_Settings_DeleteSavedSelectedFileName("AllConfGzip");

	pTreeCurrentParam = TlvTree_New(0);
	//if (ClessSample_Parameters_SaveParam (pTreeCurrentParam) != 0) // Save dummy TLVTree parameters in file
	//{
	//	Helper_DisplayTextMerchant(ERASE, HELPERS_MERCHANT_LINE_2, &tGlobalMsg, LEDSOFF);
	//	ClessSample_Term_Read_Message(STD_MESS_ERROR, lg_code, &tMsg);
	//	Helper_DisplayTextMerchant(NO_ERASE, HELPERS_MERCHANT_LINE_3, &tMsg, LEDSOFF);
	//}
	//else
	{
		Helper_DisplayTextMerchant(ERASE, HELPERS_MERCHANT_LINE_2, &tGlobalMsg, LEDSOFF);
		ClessSample_Term_Read_Message(STD_MESS_COMPLETE, lg_code, &tMsg);
		Helper_DisplayTextMerchant(NO_ERASE, HELPERS_MERCHANT_LINE_3, &tMsg, LEDSOFF);
	}

	Helper_RefreshScreen(WAIT, HELPERS_MERCHANT_SCREEN);
	
	return -1;
}



/**
 * Selects and load the .PAR file.
 * @return Always (-1).
 */
int ClessSample_Menu_SelectParameterFile (void)
{
	T_NAMES_LIST myList;
	int selectedItem;
	const char * selectedFile;
	int savedItemIndex = 0;

	memset (&myList, 0, sizeof(T_NAMES_LIST));
	CS_NameList_Init(&myList, 0);

	if (CS_Settings_getListOfFiles("AllConfGzip", &myList))
	{
		savedItemIndex = CS_Settings_GetIndexOfSavedParameterFile("AllConfGzip");

		ClessSample_Goal_Open();
		selectedItem = GL_Dialog_Menu (ClessSample_Goal_Handle(), "AllConfGzip", (const char **)myList.pListOfNames, savedItemIndex, GL_BUTTON_DEFAULT, GL_KEY_0, GL_TIME_INFINITE);
		if (selectedItem != GL_KEY_CANCEL)
		{
			selectedFile = myList.pListOfNames[selectedItem];

			if (CS_Settings_loadFromFile("AllConfGzip", selectedFile, &pTreeCurrentParam, SettingsDecoderTeliumXml, ""))
			{
				CS_Settings_SaveSelectedFileName("AllConfGzip", selectedFile);
			}
		}
		ClessSample_Goal_Close();
	}

	CS_NameList_Init(&myList, 0);


	return (-1);
}



//! \brief Parameters menu management
//! \return Always -1.

static int __ClessSample_Menu_Parameters (void)
{
	int nChoice;
	int nResult;


	nChoice = 0;
	do
	{
		nChoice = HelperDisplayMenu("PARAMETERS", nChoice, NUMBER_OF_ITEMS(ParametersMenu), ParametersMenu, FALSE);
		if (nChoice >= 0)
		{
			switch(nChoice)
			{
			case 0:			// Trace parameters on printer
				nResult = __ClessSample_Menu_ParametersDump();
				break;
			case 1:			// Read default parameters and save it in file (for next start)
				nResult = __ClessSample_Menu_DefaultParameters();
				break;
			case 2:			// Scratch parameters 
				nResult = __ClessSample_Menu_EraseParameters();
				break;
			case 3:
				nResult = ClessSample_Menu_SelectParameterFile();
				break;
			default:		// ?
				nResult = 0;
				break;
			}

			if (nResult == -1)
				nChoice = -1;
		}
	} while(nChoice >= 0);

	return nChoice;
}



//! \brief Parameters menu management for data dumping
//! \return Always -1.

static int __ClessSample_Menu_ParametersDump (void)
{
	int nChoice;
	int nResult;


	nChoice = 0;
	do
	{
		nChoice = HelperDisplayMenu("PARAMETERS DUMP", nChoice, NUMBER_OF_ITEMS(ParametersMenuDump), ParametersMenuDump, FALSE);
		if (nChoice >= 0)
		{
			switch(nChoice)
			{
			case 0:			// Identifier
				ClessSample_Parameters_DumpIdentificationLabel(pTreeCurrentParam);
				nResult = 0;
				break;
			case 1:			// AIDs
				ClessSample_Parameters_DumpAids (pTreeCurrentParam);
				nResult = 0;
				break;
			case 2:			// CA Keys
				ClessSample_Parameters_DumpCaKeys (pTreeCurrentParam);
				nResult = 0;
				break;
			case 3:			// Globals
				ClessSample_Parameters_DumpGlobals (pTreeCurrentParam);
				nResult = 0;
				break;
			case 4:			// Kernel specific
				ClessSample_Parameters_DumpKernelSpecific (pTreeCurrentParam);
				nResult = 0;
				break;
			case 5:			// All
				ClessSample_Parameters_DumpIdentificationLabel(pTreeCurrentParam);
				ClessSample_Parameters_DumpAids (pTreeCurrentParam);
				ClessSample_Parameters_DumpCaKeys (pTreeCurrentParam);
				ClessSample_Parameters_DumpGlobals (pTreeCurrentParam);
				ClessSample_Parameters_DumpKernelSpecific (pTreeCurrentParam);
				nResult = 0;
				break;
			default:		// ?
				nResult = 0;
				break;
			}

			if (nResult == -1)
				nChoice = -1;
		}
	} while(nChoice >= 0);

	return nChoice;
}



//##################################################################
//################### GLOBAL MENU ##################################
//##################################################################

//! \brief Action to do if "Forced Online" is selected in the MERCHANT menu
//! \return Always -1.

static int __ClessSample_Menu_MerchantForcedOnline(void)
{
	MSGinfos tMsg;
	int lg_code;

	// Get the manager language (merchant language)
	lg_code = PSQ_Give_Language();

	ClessSample_Term_Read_Message(STD_MESS_MENU_PARAMS_MERCHANT_FORCE_ONLINE, lg_code, &tMsg);
	Helper_DisplayTextMerchant(ERASE, HELPERS_MERCHANT_LINE_2, &tMsg, LEDSOFF);

	if (g_MerchantForcedOnline == FALSE)
	{
		g_MerchantForcedOnline = TRUE;
		ClessSample_Term_Read_Message(STD_MESS_ENABLE, lg_code, &tMsg);
		Helper_DisplayTextMerchant(NO_ERASE, HELPERS_MERCHANT_LINE_3, &tMsg, LEDSOFF);
	}
	else
	{
		g_MerchantForcedOnline = FALSE;
		ClessSample_Term_Read_Message(STD_MESS_DISABLE, lg_code, &tMsg);
		Helper_DisplayTextMerchant(NO_ERASE, HELPERS_MERCHANT_LINE_3, &tMsg, LEDSOFF);
	}
	
	Helper_RefreshScreen(WAIT, HELPERS_MERCHANT_SCREEN);
	
	return -1;
}



//! \brief Merchant menu management for data dumping
//! \return Always -1.

static int __ClessSample_Menu_Merchant (void)
{
	int nChoice;
	int nResult;

	nChoice = 0;
	do
	{
		nChoice = HelperDisplayMenu("MERCHANT", nChoice, NUMBER_OF_ITEMS(MerchantMenu), MerchantMenu, FALSE);
		if (nChoice >= 0)
		{
			switch(nChoice)
			{
			case 0:			// Force online
				nResult = __ClessSample_Menu_MerchantForcedOnline();
				break;
			case 1:			// Reset TSC
				ClessSample_Batch_InitTransactionSeqCounter (TRUE);
				nResult = -1;
				break;
			default:		// ?
				nResult = 0;
				break;
			}

			if (nResult == -1)
				nChoice = -1;
		}
	} while(nChoice >= 0);

	return nChoice;
}



//! \brief Test if merchant forced transaction online or not.
//! \return
//!		- \ref TRUE if merchant forced transaction online.
//!		- \ref FALSE if not.

unsigned char ClessSample_Menu_IsMerchantForcedOnline (void)
{
	return (unsigned char)g_MerchantForcedOnline;
}	



//##################################################################
//################### GLOBAL MENU ##################################
//##################################################################

//! \brief Global menu management
void ClessSample_Menu_IdleMenuDisplay(void)
{
	int nChoice;
	int nOldStateHeader;

	nChoice = 0;
	do
	{
		nChoice = HelperDisplayMenu("CONTACTLESS", nChoice, NUMBER_OF_ITEMS(IdleMenu), IdleMenu, FALSE);
		if (nChoice >= 0)
		{
			switch(nChoice)
			{
			case 0:
			case 1:
				nOldStateHeader = StateHeader(0);
				ClessSample_ExplicitSelection_Process(nChoice == 1, FALSE);
				StateHeader(nOldStateHeader);
				HelperLedsIdleState();
				break;
			case 2:
				if (!ClessSample_Menu_IsAutomaticCleanTornOn())
					if (ClessSample_Torn_IsRecoverySupported())
						ClessSample_Torn_CleanLog();
				break;
			case 3:
				ClessSample_Menu_MainMenuDisplay();
				nChoice = -1;
				break;
			default:
				break;
			}
		}
	} while(nChoice >= 0);
}

//! \brief Global menu management
void ClessSample_Menu_MainMenuDisplay(void)
{
	int nChoice;
	int nResult;

	nChoice = 0;
	do
	{
		nChoice = HelperDisplayMenu("CONTACTLESS", nChoice, NUMBER_OF_ITEMS(MainMenu), MainMenu, FALSE);
		if (nChoice >= 0)
		{
			switch(nChoice)
			{
			case 0:
				nResult = __ClessSample_Menu_DoExplicitTransaction(FALSE);
				break;
			case 1:
				nResult = __ClessSample_Menu_Test();
				break;
			case 2:
				nResult = __ClessSample_Menu_Parameters();
				break;
			case 3:
				nResult = __ClessSample_Menu_Merchant();
				break;
			case 4:
				nResult = __ClessSample_Menu_Batch();
				break;
			case 5:
				nResult = __ClessSample_Menu_TornLog();
				break;
			case 6:
				nResult = __ClessSample_Menu_DataExchange();
				break;
			default:
				nResult = 0;
				break;
			}

			if (nResult == -1)
				nChoice = -1;
		}
	} while(nChoice >= 0);
}



//! \brief Transaction Type menu management

void ClessSample_Menu_TransactionTypeMenuDisplay (int * pnTransactionType)
{
	int nChoice;

    *pnTransactionType = -1;

	nChoice = 0;

    do {
    	nChoice = HelperDisplayMenu("CONTACTLESS", nChoice, NUMBER_OF_ITEMS(TransactionTypeMenu), TransactionTypeMenu, FALSE);

	    if (nChoice >= 0)
	    {
		    switch(nChoice)
		    {
		    case 0:
		        *pnTransactionType = CLESS_SAMPLE_TRANSACTION_TYPE_DEBIT;
			    break;
		    case 1:
			    *pnTransactionType = CLESS_SAMPLE_TRANSACTION_TYPE_CASH;
			    break;
		    case 2:
			    *pnTransactionType = CLESS_SAMPLE_TRANSACTION_TYPE_REFUND;
			    break;
		    case 3:
			    *pnTransactionType = CLESS_SAMPLE_TRANSACTION_TYPE_PURSH_WITH_CASHBACK;
			    break;
		    case 4:
			    *pnTransactionType = CLESS_SAMPLE_TRANSACTION_TYPE_MANUAL_CASH;
			    break;
		    case 5:
			    *pnTransactionType = CLESS_SAMPLE_TRANSACTION_TYPE_CASH_DEPOSIT;
			    break;
		    case 6:
			    *pnTransactionType = CLESS_SAMPLE_TRANSACTION_TYPE_CASH_DISBURSEMENT;
			    break;
		    case 7:
			    *pnTransactionType = CLESS_SAMPLE_TRANSACTION_TYPE_UNKNOWN;
			    break;
		    default:
			    *pnTransactionType = -1;
			    break;
    	    }
   	    }
    } while (*pnTransactionType == -1);
}



//! \brief Allows to known trace state

int ClessSample_Menu_IsTraceActivated(void)
{
   return (g_TracesActivated);
}
