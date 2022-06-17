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


/////////////////////////////////////////////////////////////////
//// Macros & preprocessor definitions //////////////////////////


/////////////////////////////////////////////////////////////////
//// Global data definition /////////////////////////////////////

Telium_File_t* hSwipe2 = NULL;
Telium_File_t* hCam0 = NULL;
int bSwipe2DriverOpened;
int bCam0DriverOpened;

typedef struct
{
    unsigned short type_code;
	unsigned short service;
	unsigned long m_ulAmount;
	int m_nCurrencyCode;
} T_PERFORM_PAYMENT;


word g_ScanningTask;										// To store the different tasks ID.
t_topstack * g_tsScanning_task_handle;						// Handle of the scanning task.
int g_ListOfEvent;											// Global variable used by the scanning task to provide the event.
int g_ListOfInterfaceToScan;								// Before starting the task, it is initialised with the list of interface to be scanned for cancel

static word g_wCurrentTask;
static unsigned char g_bScanning_task_to_be_killed = FALSE;	// Global variable used to stop the scanning task.
static int g_ScanningTaskRunning = FALSE;					// Indicates if the scanning task is still running or if it is waiting to be killed.

static int g_ScanningTaskCancel = FALSE;

/////////////////////////////////////////////////////////////////
//// Static functions definition ////////////////////////////////

static void __ClessSample_Scan_ContactlessCancel (void);

// Data for task management (transparent mode)
static word __ClessSample_Scan_StartScanningTask (void);
static void __ClessSample_Scan_InitScanningVariables (const int nScannedInterfaces);


/////////////////////////////////////////////////////////////////
//// Functions //////////////////////////////////////////////////
//! \brief Call the kernel to cancel the transaction

static void __ClessSample_Scan_ContactlessCancel (void)
{
	if (ClessSample_Common_GetTransactionMode() == CLESS_SAMPLE_TRANSACTION_MODE_EXPLICIT)
	{
		g_ScanningTaskCancel = TRUE;
		if (Telium_SignalEvent (g_wCurrentTask, (tEvent)E_USER_EVENT_CANCEL) != cOK)
			GTL_Traces_DiagnosticText("Scanning task 2: Error when setting the event\n");
	}

	// Depending on the kernel used, send the cancel command
	switch (ClessSample_Customisation_GetUsedPaymentScheme())
	{
	case (CLESS_SAMPLE_CUST_PAYPASS):
		PayPass3_Cancel ();
		break;
#ifndef DISABLE_OTHERS_KERNELS
	case (CLESS_SAMPLE_CUST_PAYWAVE):
		payWave_Cancel ();
		break;
#endif
		default:
		break;
	}
}



//! \brief Open the CAM0 and SWIPE if not already opened.

void ClessSample_Scan_TransacOpenDrivers(void)
{	
	// Open SWIPE2 if not already opened
	hSwipe2 = Telium_Stdperif("SWIPE2",NULL);
	if (hSwipe2 == NULL)
	{
		bSwipe2DriverOpened = TRUE;
		hSwipe2 = Telium_Fopen("SWIPE2", "r");
		GTL_Traces_TraceDebug("Swipe 2 opened");
	}
	else
	{
		bSwipe2DriverOpened = FALSE;
		GTL_Traces_TraceDebug("Swipe 2 already opened");
	}

	// Open CAM0 if not already opened
	hCam0 = Telium_Stdperif("CAM0",NULL);
	if (hCam0 == NULL)
	{
		bCam0DriverOpened = TRUE;
		hCam0 = Telium_Fopen("CAM0", "rw");
		GTL_Traces_TraceDebug("Cam0 opened");
	}
	else
	{
		bCam0DriverOpened = FALSE;
		GTL_Traces_TraceDebug("Cam0 already opened");
	}
}



//! \brief Close the CAM0 driver.

void ClessSample_Scan_TransacCloseDriversExceptSwipe(void)
{
    if (hCam0 != NULL)
    {
        Telium_Fclose(hCam0);
        hCam0 = NULL;
    }
}



//! \brief Close the drivers opened by the application.

void ClessSample_Scan_TransacCloseDrivers(void)
{
	// Close the SWIPE2 driver only if it had been opened by the application
	if (bSwipe2DriverOpened)
	{
		if (hSwipe2 != NULL)
		{
			Telium_Fclose(hSwipe2);
			hSwipe2 = NULL;
		}
	}

	// Close the CAM0 driver only if it had been opened by the application
	if (bCam0DriverOpened)
	{
		if (hCam0 != NULL)
		{
			Telium_Fclose(hCam0);
			hCam0 = NULL;
		}
	}
}



//! \brief Start an EMV contact transaction.
//! \param[in] ulAmount Transaction amount.
//! \param[in] nCurrencyCode Transaction currency code.
//! \return
//!	- \a TRUE if EMV contact component correctly called.
//!	- \a FALSE if EMV contact component cannot be called.

int ClessSample_Scan_TransacCam0 (unsigned long ulAmount, int nCurrencyCode)
{
	int bResult;
	unsigned char priority;
	int  ret_code;
	T_PERFORM_PAYMENT Data;

	bResult = FALSE;

	//TransacStopCless();
	ClessSample_Scan_TransacCloseDrivers();

	if (Telium_ServiceGet(0x5D,120,&priority)==0)
	{
		Data.type_code = 0x5D;
		Data.service = 120;
		Data.m_ulAmount = ulAmount;
		Data.m_nCurrencyCode = nCurrencyCode;
		Telium_ServiceCall(0x5D,120,sizeof(Data),&Data,&ret_code);

		bResult = TRUE;
	}
	else if (Telium_ServiceGet(0x51,120,&priority)==0)
	{
		Data.type_code = 0x51;
		Data.service = 120;
		Data.m_ulAmount = ulAmount;
		Data.m_nCurrencyCode = nCurrencyCode;
		Telium_ServiceCall(0x51,120,sizeof(Data),&Data,&ret_code);

		bResult = TRUE;
	}
#ifdef __TELIUM3__
	else if (Telium_ServiceGet(0x57b3,120,&priority)==0)
	{
		Data.type_code = 0x57b3;
		Data.service = 120;
		Data.m_ulAmount = ulAmount;
		Data.m_nCurrencyCode = nCurrencyCode;
		Telium_ServiceCall(0x57b3,120,sizeof(Data),&Data,&ret_code);

		bResult = TRUE;
	}
#endif

	return bResult;
}



//! \brief Start an SWIPE transaction.
//! \param[in] ulAmount Transaction amount.
//! \param[in] nCurrencyCode Transaction currency code.
//! \return
//!	- \a TRUE if SWIPE component correctly called.
//!	- \a FALSE if SWIPE component cannot be called.

int ClessSample_Scan_TransacSwipe2 (unsigned long ulAmount, int nCurrencyCode)
{
	int bResult;
	unsigned char priority;
	int  ret_code;
	T_PERFORM_PAYMENT Data;

	bResult = FALSE;

	//TransacStopCless();
	ClessSample_Scan_TransacCloseDriversExceptSwipe(); // Close just the CAM0

	if (Telium_ServiceGet(0x68,200,&priority)==0)
	{
		Data.type_code = 0x68;
		Data.service = 200;
		Data.m_ulAmount = ulAmount;
		Data.m_nCurrencyCode = nCurrencyCode;
		Telium_ServiceCall(0x68,200,sizeof(Data),&Data,&ret_code);

		bResult = TRUE;
	}
	ClessSample_Scan_TransacCloseDrivers();

	return bResult;
}



/////////////////////////////////////////////////////////////////////////////////////
//    ####  ####  ###  #   # #   # #  #   #  ####        #####  ###   #### #   #   //
//   #     #     #   # ##  # ##  # #  ##  # #              #   #   # #     #  #    //
//    ###  #     ##### # # # # # # #  # # # #  ##          #   #####  ###  ###     //
//       # #     #   # #  ## #  ## #  #  ## #   #          #   #   #     # #  #    //
//   ####   #### #   # #   # #   # #  #   #  ####          #   #   # ####  #   #   //
/////////////////////////////////////////////////////////////////////////////////////

//! \brief Waits an event on keyboard, swipe or chip.

static word __ClessSample_Scan_StartScanningTask (void)
{
	int nEvent, bSendCancel/*, bWaitToBeKilled*/;
	unsigned char bStartIndication = TRUE;
	Telium_File_t * hKeyboard = NULL;
	Telium_File_t * hCam0Task = NULL;
	Telium_File_t * hSwipeTask = NULL;
	unsigned char StatusSwipe,StatusCam0;
	int nTestallMask = 0;
	
	///bWaitToBeKilled = FALSE;
	bSendCancel = FALSE;
	g_ScanningTaskRunning = TRUE; // Indicates the task is running
	g_ScanningTask = Telium_CurrentTask(); // get the Scanning task ID and store it in a global variable

	// Open the keyboard driver (if required)
	if (g_ListOfInterfaceToScan & SCAN_CANCEL_KEYBOARD)
	{
		//hKeyboard = Telium_Fopen("KEYBOARD", "r*");
		//APEMV_UI_MessageDisplayDelay("BBBB.09",0);
	}


	// List the events to be checked on the ttestall
	if (g_ListOfInterfaceToScan & SCAN_CANCEL_KEYBOARD)
	{
		nTestallMask |= KEYBOARD;

#ifndef DISABLE_UNATTENDED
		if ((ClessSample_Unattended_IsUnattendedMechanism()) && (ClessSample_Unattended_IsUCMCpresent()))
			nTestallMask |= USER_EVENT_CANCEL_UNATTENDED_UCMC;
#endif
	}

	if (g_ListOfInterfaceToScan & SCAN_CANCEL_CAM0)
		nTestallMask |= CAM0;

	if (g_ListOfInterfaceToScan & SCAN_CANCEL_SWIPE)
		nTestallMask |= SWIPE2;

	while (!g_bScanning_task_to_be_killed) // While the task is not to be killed by the custom application
	{
		/*if (bWaitToBeKilled) // A cancel has been sent to the Engine, task is waiting to be killed
		{
			if ((g_ListOfInterfaceToScan & SCAN_CANCEL_KEYBOARD) && (hKeyboard != NULL))
			{
				if (hKeyboard)
				{
					Telium_Fclose (hKeyboard); // Close the keyboard driver
					hKeyboard = NULL;
				}
			}

			g_ScanningTaskRunning = FALSE; // Indicate to the custom application the task is waiting to be killed
			Telium_Ttestall (0,0); // Wait to be killed
		}*/

		if (bStartIndication)
		{
			bStartIndication = FALSE;

			// Allows to 
			if (Telium_SignalEvent (g_wCurrentTask, (tEvent)E_USER_EVENT_START) != cOK)
				GTL_Traces_DiagnosticText("Scanning task 2: Error when setting the event\n");
		}

		// Init the events		
		nEvent = 0;
		
		// Check if there is an event on the CAM0 (if opened and if interface is required)
		if (g_ListOfInterfaceToScan & SCAN_CANCEL_CAM0)
		{
			// CAM0 is opened, check an event has not been detected
			hCam0Task = Telium_Stdperif ("CAM0", NULL);
			if (hCam0Task != NULL)
			{
				StatusCam0 = 0;
				Telium_Status(hCam0Task, &StatusCam0);
				if ((StatusCam0 & CAM_PRESENT) != 0)
					nEvent = CAM0; // Indicate CAM0 event has been detected
			}
		}

		// Check SWIPE only if not event has been detected
		if (nEvent == 0)
		{
			// Check if there is an event on the SWIPE2 (if opened and if interface is required)
			if (g_ListOfInterfaceToScan & SCAN_CANCEL_SWIPE)
			{
				hSwipeTask = Telium_Stdperif ("SWIPE2", NULL);
				if (hSwipeTask != NULL)
				{
					// SWIPE is opened, check an event has not been detected
					StatusSwipe = 0;
					Telium_Status(hSwipeTask, &StatusSwipe);
					if ((StatusSwipe & TRACK_READ) != 0)
						nEvent = SWIPE2; // Indicate SWIPE2 event has been detected
				}
			}
		}

		// No previous events (SWIPE2 or CAM0), make a ttestall during 10ms on the left interfaces
		if (nEvent == 0)
		{
			nEvent = Telium_Ttestall (nTestallMask | USER_EVENT, 600); // Wait a signal indicating the scanning task shall be stopped
		}

		// According to the event that may have occurred
		switch(nEvent)
		{
		case (0): // TTestall timeout, no event, continue scanning
		case USER_EVENT: // User event (from main task), it indicates the task is going to be killed (because g_bScanning_task_to_be_killed has been set to TRUE by the custom application
			break;

		case KEYBOARD: // Keyboard event
			if (Telium_Getchar() == T_ANN) // Cancel only if red key pressed
				bSendCancel = TRUE;
			break;

		default: // Other event (SWIPE2 or CAM0 or Cancel Unattended)
			bSendCancel = TRUE;
			break;
		}

		// If cless transaction shall be cancelled
		if (bSendCancel)
		{
			__ClessSample_Scan_ContactlessCancel (); // Cancel the transaction
			g_ListOfEvent = nEvent; // Update the global variable indicating the detected event (CAM0, SWIPE or KEYBOARD)
			///bWaitToBeKilled = TRUE; // Task is going to wait to be killed
			Telium_Ttestall (USER_EVENT, 0);
		}
	} // End While


	// The task is stopped by the main application, so no event occurred
	g_ListOfEvent = 0; // Clear the list of event (because of timeout)

	if (g_ListOfInterfaceToScan & SCAN_CANCEL_KEYBOARD)
	{
		if (hKeyboard)
		{
			Telium_Fclose (hKeyboard); // Close the keyboard
			hKeyboard = NULL;
		}
	}

	g_ScanningTaskRunning = FALSE; // Update global variable

	// Send the event to the main task (application that calls the TPass DLL)
	if (Telium_SignalEvent (g_wCurrentTask, (tEvent)E_USER_EVENT) != cOK)
		GTL_Traces_DiagnosticText ("Scanning task: Error when setting the event\n");

	///Telium_Ttestall (0,0); // Wait to be killed
	return TRUE;
}



//! \brief Launch the scanning task.
//! \param[in] nScannedInterfaces Indicates the interfaces to be used for scanning.
//! \return
//!	- \a TRUE if task correctly launch
//!	- \a FALSE else.

int ClessSample_Scan_LaunchScanningTask (const int nScannedInterfaces)
{
	// Init global variables
	__ClessSample_Scan_InitScanningVariables(nScannedInterfaces);

	// Get the main task id
	g_wCurrentTask = Telium_CurrentTask();

	// Launch the scanning task
	g_tsScanning_task_handle = Telium_Fork (&__ClessSample_Scan_StartScanningTask, NULL, -1);

	// The task cannot be created
	if (g_tsScanning_task_handle == NULL)
		return (FALSE);
	
	return (TRUE);
}



//! \brief Kill the scanning task.

void ClessSample_Scan_KillScanningTask (void)
{
	if (g_tsScanning_task_handle != NULL) // If the task is lunched
	{
		g_bScanning_task_to_be_killed = TRUE; // To Stop the task (if not already waiting to be killed because of an event)

		// Send an event to make the task ready to be killed
		if (Telium_SignalEvent (g_ScanningTask, (tEvent)E_USER_EVENT) != cOK)
			GTL_Traces_DiagnosticText ("Main task: Error when setting the event\n");

		while (g_ScanningTaskRunning == TRUE) // While the task has not terminated processing
		{
			// Waits a little
			Telium_Ttestall (USER_EVENT,1);
		}

		// The task is ready to be killed, kill it
		//Telium_Kill(g_tsScanning_task_handle, "-*");
		g_tsScanning_task_handle = NULL;
	}
}


int ClessSample_Scan_IsCancel(void)
{
	return g_ScanningTaskCancel;
}

//! \brief Initialise scanning global variables
//! \param[in] nScannedInterfaces Indicates the interfaces to be used for scanning.

static void __ClessSample_Scan_InitScanningVariables (const int nScannedInterfaces)
{
	g_wCurrentTask = 0xFF; // Init the custom application task number
	g_ScanningTask = 0xFF; // Init the scanning task number
	g_tsScanning_task_handle = NULL; // init the scanning task handle
	g_bScanning_task_to_be_killed = FALSE; // Task has not to be killed
	g_ScanningTaskRunning = FALSE; //Task is not running
	g_ListOfInterfaceToScan = nScannedInterfaces;
	g_ScanningTaskCancel = FALSE;
}
