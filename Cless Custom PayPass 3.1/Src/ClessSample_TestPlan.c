//! \file
//! Module that manages the terminal test plan mechanism.
//
/////////////////////////////////////////////////////////////////
//// Includes ///////////////////////////////////////////////////

#include "ClessSample_Implementation.h"
#include "GL_File.h"
#include "GL_Dir.h"

/////////////////////////////////////////////////////////////////
//// Macros & preprocessor definitions //////////////////////////

static const char m_pFileType[] = "file://";
static const char m_pHostPath[] = "flash/HOST/";
static const char m_pTestPlanFileName[] = "TESTPLAN";
static const char m_pCupExtension[] = ".CUP";

static const char m_pKeyWordDefault[] = "DEFAULT";
static const char m_pKeyWordComputed[] = "COMPUTED";

#define TEST_PLAN_SHARED_DATA_MAX_SIZE				4096


/////////////////////////////////////////////////////////////////
//// Types //////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////
//// Global variables ///////////////////////////////////////////

static TLV_TREE_NODE gs_TestPlanTlvTree = NULL;
static int gs_bIsLoaded = FALSE;
static T_SHARED_DATA_STRUCT * gs_pSharedCurrentTestPlanData = NULL;
static int gs_nCurrentMenuIndex = 0;

/////////////////////////////////////////////////////////////////
//// Static function definitions ////////////////////////////////

static int __CS_TestPlan_IsFileInTerminal (void);
static int __CS_TestPlan_LoadFromFile (void);
static int __CS_TestPlan_GetListOfNames (T_NAMES_LIST * pList);
static int __CS_TestPlan_ExtractDataFromIndex (T_NAMES_LIST * pList, unsigned int index, T_SHARED_DATA_STRUCT * pStruct);


/////////////////////////////////////////////////////////////////
//// Functions //////////////////////////////////////////////////
void	Cless_PSQ_Est_money (S_MONEY *p_money);

/**
 * Copy data into provided shared exchange structure.
 * @param pStruct Destination structure.
 * @return
 * - \a TRUE if correctly completed.
 * - \a FALSE if an error occurred.
 */
int CS_TestPlan_CopyToSharedStruct (T_SHARED_DATA_STRUCT * pStruct)
{
	int nResult = FALSE;
	int cr;

	if (pStruct)
	{
		if (gs_pSharedCurrentTestPlanData)
		{
			cr = GTL_SharedExchange_AddSharedBufferContent(pStruct, gs_pSharedCurrentTestPlanData);
			if (cr != STATUS_SHARED_EXCHANGE_OK)
			{
				GTL_Traces_TraceDebug("CS_TestPlan_CopyToSharedStruct: Fail to copy gs_pSharedCurrentTestPlanData to pStruct (cr = %d)", cr);
			}
			else
			{
				nResult = TRUE;
			}
		}
		else
		{
			GTL_Traces_TraceDebug("CS_TestPlan_CopyToSharedStruct: gs_pSharedCurrentTestPlanData = NULL");
		}
	}
	else
	{
		GTL_Traces_TraceDebug("CS_TestPlan_CopyToSharedStruct: pStruct = NULL");
	}

	return (nResult);
}



/**
 * Erase TLV Tree containing the loaded test plan.
 */
void CS_TestPlan_Erase (void)
{
	if (gs_TestPlanTlvTree != NULL)
	{
		TlvTree_Release(gs_TestPlanTlvTree);
		gs_TestPlanTlvTree = NULL;
	}

	gs_bIsLoaded = FALSE;
}


/**
 * Indicates if the test plan file is present in the terminal or not.
 * @return
 * - \a TRUE if present.
 * - \a FALSE if not.
 */
static int __CS_TestPlan_IsFileInTerminal (void)
{
	char pTestPlanPath[256];

	memset (pTestPlanPath, '\0', sizeof(pTestPlanPath));
	sprintf (pTestPlanPath, "%s%s%s%s", m_pFileType, m_pHostPath, m_pTestPlanFileName, m_pCupExtension);

	return (GL_File_Exists(pTestPlanPath) == GL_SUCCESS);
}


/**
 * Load the Test Plan from the file in the terminal HOST directory.
 * @return
 * - \a TRUE if correctly loaded.
 * - \a FALSE if an error occurred (no file, cannot open the file, unable to unserialize the file, etc).
 */
static int __CS_TestPlan_LoadFromFile (void)
{
	char pTestPlanPath[256];
	unsigned long fileLength = 0;
	T_GL_HFILE pFileToLoad = NULL;
	int nResult = FALSE;
	char * pBuffer = NULL;
	long readLength = 0;
	int cr;


	// Erase any previously loaded file
	CS_TestPlan_Erase ();

	// Check if there is a file loaded in the terminal
	if (__CS_TestPlan_IsFileInTerminal())
	{
		// Create the file path
		memset (pTestPlanPath, '\0', sizeof(pTestPlanPath));
		sprintf (pTestPlanPath, "%s%s%s%s", m_pFileType, m_pHostPath, m_pTestPlanFileName, m_pCupExtension);

	    // Open the file
	    pFileToLoad = GL_File_Open(pTestPlanPath, GL_FILE_OPEN_EXISTING, GL_FILE_ACCESS_READ);
	    if (pFileToLoad == NULL)
	    {
	    	GTL_Traces_TraceDebug ("__CS_TestPlan_LoadFromFile: The file <%s> can't be opened.", pTestPlanPath);
	        goto end;
	    }

	    // Get the file length
	    fileLength = GL_File_GetSize(pFileToLoad);
	    if (fileLength==0)
	    {
	    	GTL_Traces_TraceDebug ("__CS_TestPlan_LoadFromFile: The file <%s> is empty.", pTestPlanPath);
	        goto end;
	    }

	    // Allocate the buffer to read the file
	    pBuffer = (char*)umalloc(fileLength);
	    if (pBuffer == NULL)
	    {
	    	GTL_Traces_TraceDebug ("__CS_TestPlan_LoadFromFile: Unable to allocate <%ld> bytes.", fileLength);
	        goto end;
	    }

	    // Read the file
	    readLength = GL_File_Read(pFileToLoad, pBuffer, fileLength);
	    if (readLength != fileLength)
	    {
	    	GTL_Traces_TraceDebug ("__CS_TestPlan_LoadFromFile: The file <%s> can't be read correctly. Only %ld read instead of %ld", pTestPlanPath, readLength, fileLength);
	        goto end;
	    }

	    // Decode the read buffer
	    cr = TlvTree_Unserialize(&gs_TestPlanTlvTree, TLV_TREE_SERIALIZER_XML, (const unsigned char*)pBuffer, readLength);
	    if (cr != TLV_TREE_OK)
	    {
	    	GTL_Traces_TraceDebug ("__CS_TestPlan_LoadFromFile: Cannot unserialize the TLV Tree buffer (cr = %d)", cr);
	        goto end;
	    }

		nResult = TRUE;
		gs_bIsLoaded = TRUE;
	}
	else
	{
    	GTL_Traces_TraceDebug ("__CS_TestPlan_LoadFromFile: No Test Plan file loaded in the terminal");
        goto end;
	}


end:
	if (pFileToLoad)
	{
		GL_File_Close(pFileToLoad);
		pFileToLoad = NULL;
	}

	if (pBuffer != NULL)
	{
		ufree (pBuffer);
		pBuffer = NULL;
	}

	return (nResult);
}



/**
 * Get the list of names to be displayed in the menu for choice.
 * @param pList List of name.
 * @return
 * - \a TRUE if list of names successfully filled.
 * - \a FALSE if an error occurred.
 */
static int __CS_TestPlan_GetListOfNames (T_NAMES_LIST * pList)
{
	int nResult = FALSE;
	TLV_TREE_NODE pChild = NULL;
	int counter = 0;
	unsigned int readTag;

	if (pList != NULL)
	{
		if (gs_TestPlanTlvTree != NULL)
		{
			// First, count the number of TEST_PLAN_TAG_NAME in the test plan
			pChild = TlvTree_GetFirstChild(gs_TestPlanTlvTree);

			while (pChild != NULL)
			{
				readTag = TlvTree_GetTag(pChild);
				if (readTag == TEST_PLAN_TAG_NAME)
				{
					counter++;
				}

				pChild = TlvTree_GetNext(pChild);
			}

			// Check if there are entries
			if (counter > 0)
			{
				CS_NameList_Init(pList, counter+1);

				// Parse again the TLV Tree to fill the name list
				pChild = TlvTree_GetFirstChild(gs_TestPlanTlvTree);

				while (pChild != NULL)
				{
					readTag = TlvTree_GetTag(pChild);
					if (readTag == TEST_PLAN_TAG_NAME)
					{
						if (!CS_NameList_AddItem(pList, TlvTree_GetData(pChild)))
						{
							GTL_Traces_TraceDebug ("__CS_TestPlan_GetListOfNames: An error occurred when adding the name in the list.");
							CS_NameList_Init(pList, 0);
							goto end;
						}
					}

					pChild = TlvTree_GetNext(pChild);
				}

				if (pList->numberOfEntries != (pList->maxNumberOfEntries - 1))
				{
			    	GTL_Traces_TraceDebug ("__CS_TestPlan_GetListOfNames: list should be full.");
			    	CS_NameList_Init(pList, 0);
			    	goto end;
				}
			}
			else
			{
		    	GTL_Traces_TraceDebug ("__CS_TestPlan_GetListOfNames: no entry found in the test plan.");
		    	goto end;
			}
		}
		else
		{
	    	GTL_Traces_TraceDebug ("__CS_TestPlan_GetListOfNames: no loaded test plan.");
	        goto end;
		}
	}
	else
	{
    	GTL_Traces_TraceDebug ("__CS_TestPlan_GetListOfNames: pList is NULL.");
        goto end;
	}

	nResult = TRUE;

end:
	return (nResult);
}


/**
 * Extract the test case data which is at a given index in the name list.
 * @param pList Name list filled with the test case names.
 * @param index Index of the chosen test case.
 * @param pStruct Structure to be filled with the test case data.
 * @return
 * - \a TRUE if successfully performed.
 * - \a FALSE if an error occurred.
 */
static int __CS_TestPlan_ExtractDataFromIndex (T_NAMES_LIST * pList, unsigned int index, T_SHARED_DATA_STRUCT * pStruct)
{
	int nResult = FALSE;
	char * pTestCaseName = NULL;
	TLV_TREE_NODE pChild = NULL;
	TLV_TREE_NODE pContent = NULL;
	unsigned int readTag;
	unsigned int readTagContent;
	int cr;

	if (gs_TestPlanTlvTree != NULL)
	{
		if ((pList != NULL) && (pStruct != NULL))
		{
			if (index < pList->numberOfEntries)
			{
				// Get the pointer on the test case name
				pTestCaseName = pList->pListOfNames[index];

				// Search the test case name in the TLV Tree
				pChild = TlvTree_GetFirstChild(gs_TestPlanTlvTree);

				while (pChild != NULL)
				{
					readTag = TlvTree_GetTag(pChild);
					if (readTag == TEST_PLAN_TAG_NAME)
					{
						if (strcmp(TlvTree_GetData(pChild), pTestCaseName) == 0)
						{
							// Load the content of the test plan data into the pNode
							pContent = TlvTree_GetFirstChild(pChild);
							while (pContent != NULL)
							{
								readTagContent = TlvTree_GetTag(pContent);
								switch (readTagContent)
								{

								// ////////////////////////////////////////////////////////////////////////////////////////////////////////////
								case (TEST_PLAN_TAG_TERMINAL_SETTING):
									if (CS_Settings_IsFileExisting("AllConfGzip", TlvTree_GetData(pContent)))
									{
										if (CS_Settings_loadFromFile("AllConfGzip", TlvTree_GetData(pContent), &pTreeCurrentParam, SettingsDecoderTeliumXml, "") == FALSE)
										{
											GTL_Traces_TraceDebug ("__CS_TestPlan_ExtractDataFromIndex: Unable to load the associated terminal setting <%s>.", (char*)TlvTree_GetData(pContent));
											goto end;
										}
										else
										{
											CS_Settings_SaveSelectedFileName ("AllConfGzip", TlvTree_GetData(pContent));
										}
									}
									else
									{
										GTL_Traces_TraceDebug ("__CS_TestPlan_ExtractDataFromIndex: Associated terminal setting cannot be found <%s>.", (char*)TlvTree_GetData(pContent));
										goto end;
									}
									break;

								// ////////////////////////////////////////////////////////////////////////////////////////////////////////////
								case (TEST_PLAN_TAG_KERNEL_SETTINGS):
									if (CS_Settings_IsFileExisting("MCW_DEKDET", TlvTree_GetData(pContent)))
									{
										if (CS_Settings_loadFromFile("MCW_DEKDET", TlvTree_GetData(pContent), &pTreeCurrentDataExchange, SettingsDecoderDekDet, ".PAR") == FALSE)
										{
											GTL_Traces_TraceDebug ("__CS_TestPlan_ExtractDataFromIndex: Unable to load the associated kernel setting <%s>.", (char*)TlvTree_GetData(pContent));
											goto end;
										}
										else
										{
											CS_Settings_SaveSelectedFileName ("MCW_DEKDET", TlvTree_GetData(pContent));
										}
									}
									else
									{
										GTL_Traces_TraceDebug ("__CS_TestPlan_ExtractDataFromIndex: Associated kernel setting cannot be found <%s>.", (char*)TlvTree_GetData(pContent));
										goto end;
									}
									break;

								// ////////////////////////////////////////////////////////////////////////////////////////////////////////////
								case (TAG_EMV_IFD_SERIAL_NUMBER):
									if (strcmp (TlvTree_GetData(pContent), m_pKeyWordComputed) == 0)
									{
										// If the keyword is COMPUTED, it means that we shall retrieve the terminal serial number
										NO_SERIE serial;
										memset (serial, '\0', sizeof(NO_SERIE));

										// Extract SN and initialize the structure
										PSQ_Give_Serial_Number(serial);

										// Store the last 8 digits
										///if (TlvTree_AddChild(*pNode, TAG_EMV_IFD_SERIAL_NUMBER, &serial[sizeof(serial) - 1 - 8], 8) == NULL)
										//cr = GTL_SharedExchange_AddTag(pStruct, TAG_EMV_IFD_SERIAL_NUMBER, 8, (const unsigned char *) &(serial[sizeof(serial) - 1 - 8]));
										cr = GTL_SharedExchange_AddTag(pStruct, TAG_EMV_IFD_SERIAL_NUMBER, 8, (const unsigned char *) &serial);
										if (cr != STATUS_SHARED_EXCHANGE_OK)
										{
											GTL_Traces_TraceDebug ("__CS_TestPlan_ExtractDataFromIndex: Cannot add the TAG_EMV_IFD_SERIAL_NUMBER in pNode (cr = %d).", cr);
											goto end;
										}
									}
									else
									{
										// Store the provided value
										///if (TlvTree_AddChild(*pNode, TAG_EMV_IFD_SERIAL_NUMBER, TlvTree_GetData(pContent), TlvTree_GetLength(pContent)) == NULL)
										cr = GTL_SharedExchange_AddTag(pStruct, TAG_EMV_IFD_SERIAL_NUMBER, TlvTree_GetLength(pContent), TlvTree_GetData(pContent));
										if (cr != STATUS_SHARED_EXCHANGE_OK)
										{
											GTL_Traces_TraceDebug ("__CS_TestPlan_ExtractDataFromIndex: Cannot add the TAG_EMV_IFD_SERIAL_NUMBER in pNode (cr = %d).", cr);
											goto end;
										}
									}
									break;

								// ////////////////////////////////////////////////////////////////////////////////////////////////////////////
								case (TAG_EMV_TRANSACTION_CURRENCY_CODE):
									if (strcmp (TlvTree_GetData(pContent), m_pKeyWordComputed) == 0)
									{
										S_MONEY money;
										unsigned char ucTransactionCurrencyCode[2];

										// Get the currency configured in the manager
										memset (&money, 0x00, sizeof(S_MONEY));
										Cless_PSQ_Est_money (&money);

										// Convert the transaction currency code from the manager to 2 byte numeric
										memset (ucTransactionCurrencyCode, 0x00, sizeof(ucTransactionCurrencyCode));
										ucTransactionCurrencyCode[0] = (unsigned char)(money.code[0] - '0');
										ucTransactionCurrencyCode[1] = (unsigned char)(((money.code[1] - '0') << 4) | (money.code[2] - '0'));


										// Store the tag
										///if (TlvTree_AddChild(*pNode, TAG_EMV_TRANSACTION_CURRENCY_CODE, ucTransactionCurrencyCode, 2) == NULL)
										cr = GTL_SharedExchange_AddTag(pStruct, TAG_EMV_TRANSACTION_CURRENCY_CODE, 2, ucTransactionCurrencyCode);
										if (cr != STATUS_SHARED_EXCHANGE_OK)
										{
											GTL_Traces_TraceDebug ("__CS_TestPlan_ExtractDataFromIndex: Cannot add the TAG_EMV_TRANSACTION_CURRENCY_CODE in pNode (cr = %d).", cr);
											goto end;
										}
									}
									else
									{
										// Store the provided value
										///if (TlvTree_AddChild(*pNode, TAG_EMV_TRANSACTION_CURRENCY_CODE, TlvTree_GetData(pContent), TlvTree_GetLength(pContent)) == NULL)
										cr = GTL_SharedExchange_AddTag(pStruct, TAG_EMV_TRANSACTION_CURRENCY_CODE, TlvTree_GetLength(pContent), TlvTree_GetData(pContent));
										if (cr != STATUS_SHARED_EXCHANGE_OK)
										{
											GTL_Traces_TraceDebug ("__CS_TestPlan_ExtractDataFromIndex: Cannot add the TAG_EMV_TRANSACTION_CURRENCY_CODE in pNode (cr = %d).", cr);
											goto end;
										}
									}
									break;

								// ////////////////////////////////////////////////////////////////////////////////////////////////////////////
								case (TAG_EMV_TRANSACTION_CURRENCY_EXPONENT):
									if (strcmp (TlvTree_GetData(pContent), m_pKeyWordComputed) == 0)
									{
										S_MONEY money;
										unsigned char ucTransactionCurrencyExponent;

										// Get the currency configured in the manager
										memset (&money, 0x00, sizeof(S_MONEY));
										Cless_PSQ_Est_money (&money);

										// Get the transaction currency exponent
										ucTransactionCurrencyExponent = money.posdec;

										// Store the tag
										///if (TlvTree_AddChild(*pNode, TAG_EMV_TRANSACTION_CURRENCY_EXPONENT, &ucTransactionCurrencyExponent, 1) == NULL)
										cr = GTL_SharedExchange_AddTag(pStruct, TAG_EMV_TRANSACTION_CURRENCY_EXPONENT, 1, &ucTransactionCurrencyExponent);
										if (cr != STATUS_SHARED_EXCHANGE_OK)
										{
											GTL_Traces_TraceDebug ("__CS_TestPlan_ExtractDataFromIndex: Cannot add the TAG_EMV_TRANSACTION_CURRENCY_EXPONENT in pNode (cr = %d).", cr);
											goto end;
										}
									}
									else
									{
										// Store the provided value
										///if (TlvTree_AddChild(*pNode, TAG_EMV_TRANSACTION_CURRENCY_EXPONENT, TlvTree_GetData(pContent), TlvTree_GetLength(pContent)) == NULL)
										cr = GTL_SharedExchange_AddTag(pStruct, TAG_EMV_TRANSACTION_CURRENCY_EXPONENT, TlvTree_GetLength(pContent), TlvTree_GetData(pContent));
										if (cr != STATUS_SHARED_EXCHANGE_OK)
										{
											GTL_Traces_TraceDebug ("__CS_TestPlan_ExtractDataFromIndex: Cannot add the TAG_EMV_TRANSACTION_CURRENCY_EXPONENT in pNode (cr = %d).", cr);
											goto end;
										}
									}
									break;

								// ////////////////////////////////////////////////////////////////////////////////////////////////////////////
								case (TAG_EMV_TRANSACTION_DATE):
									if (strcmp (TlvTree_GetData(pContent), m_pKeyWordComputed) == 0)
									{
										Telium_Date_t Date;
										unsigned char ucDate[3];

										if (Telium_Read_date(&Date) == OK)
										{
											ucDate[0] = (unsigned char)(((Date.year[0] - '0') << 4) | (Date.year[1] - '0'));
											ucDate[1] = (unsigned char)(((Date.month[0] - '0') << 4) | (Date.month[1] - '0'));
											ucDate[2] = (unsigned char)(((Date.day[0] - '0') << 4) | (Date.day[1] - '0'));
										}
										else
										{
											GTL_Traces_TraceDebug ("__CS_TestPlan_ExtractDataFromIndex: Cannot read the date.");
											goto end;
										}

										// Store the tag
										///if (TlvTree_AddChild(*pNode, TAG_EMV_TRANSACTION_DATE, &ucDate, 3) == NULL)
										cr = GTL_SharedExchange_AddTag(pStruct, TAG_EMV_TRANSACTION_DATE, 3, ucDate);
										if (cr != STATUS_SHARED_EXCHANGE_OK)
										{
											GTL_Traces_TraceDebug ("__CS_TestPlan_ExtractDataFromIndex: Cannot add the TAG_EMV_TRANSACTION_DATE in pNode (cr = %d).", cr);
											goto end;
										}
									}
									else
									{
										// Store the provided value
										///if (TlvTree_AddChild(*pNode, TAG_EMV_TRANSACTION_DATE, TlvTree_GetData(pContent), TlvTree_GetLength(pContent)) == NULL)
										cr = GTL_SharedExchange_AddTag(pStruct, TAG_EMV_TRANSACTION_DATE, TlvTree_GetLength(pContent), TlvTree_GetData(pContent));
										if (cr != STATUS_SHARED_EXCHANGE_OK)
										{
											GTL_Traces_TraceDebug ("__CS_TestPlan_ExtractDataFromIndex: Cannot add the TAG_EMV_TRANSACTION_DATE in pNode (cr = %d).", cr);
											goto end;
										}
									}
									break;

								// ////////////////////////////////////////////////////////////////////////////////////////////////////////////
								case (TAG_EMV_TRANSACTION_TIME):
									if (strcmp (TlvTree_GetData(pContent), m_pKeyWordComputed) == 0)
									{
										Telium_Date_t Date;
										unsigned char ucTime[3];

										if (Telium_Read_date(&Date) == OK)
										{
											ucTime[0] = (unsigned char)(((Date.hour[0] - '0') << 4) | (Date.hour[1] - '0'));
											ucTime[1] = (unsigned char)(((Date.minute[0] - '0') << 4) | (Date.minute[1] - '0'));
											ucTime[2] = (unsigned char)(((Date.second[0] - '0') << 4) | (Date.second[1] - '0'));
										}
										else
										{
											GTL_Traces_TraceDebug ("__CS_TestPlan_ExtractDataFromIndex: Cannot read the date.");
											goto end;
										}

										// Store the tag
										///if (TlvTree_AddChild(*pNode, TAG_EMV_TRANSACTION_TIME, &ucTime, 3) == NULL)
										cr = GTL_SharedExchange_AddTag(pStruct, TAG_EMV_TRANSACTION_TIME, 3, ucTime);
										if (cr != STATUS_SHARED_EXCHANGE_OK)
										{
											GTL_Traces_TraceDebug ("__CS_TestPlan_ExtractDataFromIndex: Cannot add the TAG_EMV_TRANSACTION_TIME in pNode (cr = %d).", cr);
											goto end;
										}
									}
									else
									{
										// Store the provided value
										///if (TlvTree_AddChild(*pNode, TAG_EMV_TRANSACTION_TIME, TlvTree_GetData(pContent), TlvTree_GetLength(pContent)) == NULL)
										cr = GTL_SharedExchange_AddTag(pStruct, TAG_EMV_TRANSACTION_TIME, TlvTree_GetLength(pContent), TlvTree_GetData(pContent));
										if (cr != STATUS_SHARED_EXCHANGE_OK)
										{
											GTL_Traces_TraceDebug ("__CS_TestPlan_ExtractDataFromIndex: Cannot add the TAG_EMV_TRANSACTION_TIME in pNode (cr = %d).", cr);
											goto end;
										}
									}
									break;

								// ////////////////////////////////////////////////////////////////////////////////////////////////////////////
								default:
									// Store the provided value
									///if (TlvTree_AddChild(*pNode, readTagContent, TlvTree_GetData(pContent), TlvTree_GetLength(pContent)) == NULL)
									cr = GTL_SharedExchange_AddTag(pStruct, readTagContent, TlvTree_GetLength(pContent), TlvTree_GetData(pContent));
									if (cr != STATUS_SHARED_EXCHANGE_OK)
									{
										GTL_Traces_TraceDebug ("__CS_TestPlan_ExtractDataFromIndex: Cannot add the %02X L=%02X in pNode (cr = %d).", cr, readTagContent, TlvTree_GetLength(pContent));
										goto end;
									}
									break;
								}

								// Get the next element in the node
								pContent = TlvTree_GetNext(pContent);
							}
						}
					}

					pChild = TlvTree_GetNext(pChild);
				}
			}
			else
			{
				GTL_Traces_TraceDebug ("__CS_TestPlan_ExtractDataFromIndex: index is out of range (index=%d, pList->numberOfEntries = %d).", index, pList->numberOfEntries);
				goto end;
			}

		}
		else
		{
			if (pList == NULL)
				GTL_Traces_TraceDebug ("__CS_TestPlan_ExtractDataFromIndex: pList is NULL.");
			if (pStruct == NULL)
				GTL_Traces_TraceDebug ("__CS_TestPlan_ExtractDataFromIndex: pNode is NULL.");
			goto end;
		}
	}
	else
	{
		GTL_Traces_TraceDebug ("__CS_TestPlan_ExtractDataFromIndex: gs_TestPlanTlvTree is NULL (no test plan loaded).");
		goto end;
	}

	nResult = TRUE;

end:

	if (nResult == FALSE) // An error occurred
	{
		if (pStruct != NULL)
		{
			GTL_SharedExchange_Clear(pStruct);
		}
	}

	return(nResult);
}



/**
 * Display a menu in order the operator can choose the test case to be used to perform the transaction. It also load all the test case related data
 * into the shared exchange that can be used later to get data to perform the transaction.
 * @return
 * - \a TRUE if successfully performed.
 * - \a FALSE if an error occurred.
 */
int CS_TestPlan_ChooseAndLoad (void)
{
	T_NAMES_LIST myList;
	int selectedItem;
	int nResult = FALSE;

	// Before loading any test case, erase previously loaded parameters
	if (gs_pSharedCurrentTestPlanData != NULL)
	{
		GTL_SharedExchange_DestroyShare(gs_pSharedCurrentTestPlanData);
		gs_pSharedCurrentTestPlanData = NULL;
	}

	gs_pSharedCurrentTestPlanData = GTL_SharedExchange_InitShared(TEST_PLAN_SHARED_DATA_MAX_SIZE);
	if (gs_pSharedCurrentTestPlanData)
	{
		memset (&myList, 0, sizeof(T_NAMES_LIST));
		CS_NameList_Init(&myList, 0);
		myList.isInitialized = FALSE;

		// Load the test plan from file in the terminal
		if (__CS_TestPlan_LoadFromFile())
		{
			// Get the list of test cases (for in order to display the menu)
			if (__CS_TestPlan_GetListOfNames(&myList))
			{
				// Open GOAL resource
				ClessSample_Goal_Open();

				// Display the menu in order to choose the test case
				selectedItem = GL_Dialog_Menu (ClessSample_Goal_Handle(), "TEST PLAN", (const char **)myList.pListOfNames, gs_nCurrentMenuIndex, GL_BUTTON_DEFAULT, GL_KEY_0, GL_TIME_INFINITE);

				// Close GOAL resource
				ClessSample_Goal_Close();
				if (selectedItem >= 0)
				{
					// Extract the data from the selected test case
					if (!__CS_TestPlan_ExtractDataFromIndex (&myList, selectedItem, gs_pSharedCurrentTestPlanData))
					{
						GTL_Traces_TraceDebug ("CS_TestPlan_ChooseAndLoad: Test Plan data extraction failed.");
						goto end;
					}
					else
					{
						// Save the menu in the index
						gs_nCurrentMenuIndex = selectedItem;
					}
				}
				else
				{
					GTL_Traces_TraceDebug ("CS_TestPlan_ChooseAndLoad: No selected item.");
					goto end;
				}
			}
			else
			{
				GTL_Traces_TraceDebug ("CS_TestPlan_ChooseAndLoad: Unable to retrieve the list of namesNo selected item.");
				goto end;
			}
		}
		else
		{
			GTL_Traces_TraceDebug ("CS_TestPlan_ChooseAndLoad: Unable to load the test plan file.");
			goto end;
		}
	}
	else
	{
		GTL_Traces_TraceDebug ("CS_TestPlan_ChooseAndLoad: Unable to allocate memory to create the shared exchange structure.");
		goto end;
	}

	nResult = TRUE;

end:

	// Deallocate memory of name list
	CS_NameList_Init(&myList, 0);

	// In the case of an error occurred only...
	if (nResult == FALSE)
	{
		if (gs_pSharedCurrentTestPlanData != NULL)
		{
			GTL_SharedExchange_DestroyShare(gs_pSharedCurrentTestPlanData);
			gs_pSharedCurrentTestPlanData = NULL;
		}
	}

	return (nResult);
}
