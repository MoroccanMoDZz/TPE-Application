//****************************************************************************
//       INGENICO                                INGEDEV 7                  
//============================================================================
//       FILE  COMSERIAL.C                        (Copyright INGENICO 2008)   
//============================================================================
//  Created :       12-May-2008     Kassovic                                 
//  Last modified : 12-May-2008     Kassovic                                
//  Module : TRAINING                                                          
//                                                                          
//  Purpose :                                                               
//  USB communication management:
//             Transmission by USB communication
//             Test done with Hyper-terminal
//  !!! Link Layer application must be loaded 34290302.SGN inside the terminal
//      AVL.lib + LinkLayerExeInterface.lib must be implemented to link properly   
//                                                                            
//  List of routines in file :  
//      GetHandle : Handle of the configuration.
//      OpenUSB : Create the USB port.
//      InitUSB : Configure the USB port.
//      ConnectUSB : Connect the USB port.
//      SendUSB : Send data through the USB port.
//      ReceiveUSB : Receive data through the USB port.
//      DisconnectUSB : Disconnect the USB port.                                           
//      CloseUSB : Delete the USB port.
//      ComUSB : USB communication demo.
//                            
//  File history :
//  120508-BK : File created 
//                                                                           
//****************************************************************************

//****************************************************************************
//      INCLUDES                                                            
//****************************************************************************
#include "SDK_Naps.h"

#include "Training.h"
#include "LinkLayer.h"
#include "TlvTree.h"

#include "Goal.h"
#include <stdlib.h>
#include <string.h>
//****************************************************************************
//      EXTERN                                                              
//****************************************************************************
extern Telium_File_t *hKbd;      // Handle of the peripheral Keyboard

extern char buffer12[1024];
extern char buffer13[4000];


//****************************************************************************
//      PRIVATE CONSTANTS                                                   
//****************************************************************************
#define MAX_SND  512
#define MAX_RSP  512

//****************************************************************************
//      PRIVATE TYPES                                                       
//****************************************************************************
    /* */

//****************************************************************************
//      PRIVATE DATA                                                        
//****************************************************************************
	/* */

//****************************************************************************
//                         LL_HANDLE OpenUSB (void)
//  This function configures the USB port.     
//    - LL_Configure() : Create Link Layer configuration
//  This function has no parameters.  
//  This function has return value
//    !NULL : Handle of the session
//     NULL : Session failed
//****************************************************************************
void	Naps_Approve(void);//badr
Ushort   		gindice;//badr
static int timeout_var = 0;
static int confir_var = 0;

static LL_HANDLE OpenUSB(void)
{
	// Local variables 
    // ***************
	TLV_TREE_NODE piConfig=NULL;
	TLV_TREE_NODE piPhysicalConfig=NULL;
	LL_HANDLE hSession = NULL;                                        // Session handle
    int iRet;

	// Create the LinkLayer configuration parameters tree 
	// **************************************************
    
    // Create parameters tree
    // ======================
	piConfig = TlvTree_New(LL_TAG_LINK_LAYER_CONFIG);                 // LinkLayer parameters Root tag of the configuration tree
    CHECK(piConfig!=0, lblKOConfigure);

	// Physical layer parameters  
	// =========================
	piPhysicalConfig = TlvTree_AddChild(piConfig,                        
		                                LL_TAG_PHYSICAL_LAYER_CONFIG, // TAG Physical layer parameters 
									    NULL,                         // VALUE (Null)
									    0);                           // LENGTH 0
    CHECK(piPhysicalConfig!=0, lblKOConfigure);

    // Port USB
	// --------
	TlvTree_AddChildInteger(piPhysicalConfig, 
		                    LL_PHYSICAL_T_LINK,                       // TAG
		                    LL_PHYSICAL_V_USB,                        // VALUE
							LL_PHYSICAL_L_LINK);                      // LENGTH 1 byte
	
	// Link Layer configuration
	// ************************
	iRet = LL_Configure(&hSession, piConfig);                         // Initialize the handle of the session
	CHECK(iRet==LL_ERROR_OK, lblKOConfigure);

	goto lblEnd;

	// Errors treatment
    // ****************
lblKOConfigure:                                                       // Configuration failed
    hSession=NULL;
    goto lblEnd;
lblEnd:
    if (piConfig)
    	TlvTree_Release(piConfig);                                    // Release tree to avoid memory leak
	return hSession;                                                  // Return the handle of the session
}

//****************************************************************************
//                   int ConnectUSB (LL_HANDLE hSession)
//  This function connects the USB port.     
//    - LL_Connect() : Connect Link Layer
//  This function has no parameters.  
//    hSession (I-) : Handle of the session
//  This function has return value
//    >=0 : Connect done
//     <0 : Connect failed                                        
//****************************************************************************

static int ConnectUSB(LL_HANDLE hSession)
{
	// Local variables 
    // ***************
    int iRet;
    
    // Link Layer connection
	// *********************
    iRet = LL_Connect(hSession);

    return iRet;
}

//****************************************************************************
//     int SendUSB (LL_HANDLE hSession, const char *pcMsg, word usLen)
//  This function sends data through the USB port.      
//    - LL_Send() : Send data
//    - LL_GetLastError() : Retrieve the last error
//  This function has parameters.
//    hSession (I-) : Handle of the session
//    pcMsg (I-) : Data to send
//    usLen (I-) : Number of byte to send
//  This function has return value
//    >=0 : Number of bytes sent
//     <0 : Transmission failed                                       
//****************************************************************************

static int SendUSB(LL_HANDLE hSession, const char *pcMsg, word usLen)
{
	// Local variables 
    // ***************
    int iNbrBytes, iRet;

    // Send data
    // *********
    iNbrBytes = LL_Send(hSession, usLen, pcMsg, LL_INFINITE);
    
    // Wait until data sent
    // ********************
    iRet = LL_WaitEvent(hSession, LL_WAIT_SEND_END, LL_INFINITE);
    if (iRet != LL_WAIT_SEND_END)
		iRet = LL_GetLastError(hSession);
    else
    	iRet = iNbrBytes;

    return iRet;
}

static int ReceiveUSB_SansKeyBoard(LL_HANDLE hSession, char *pcMsg, word usLen, byte ucDly)
{
	// Local variables
    // ***************
    //Cless_ char cKey;
    long lSec, lTimeOut=LL_INFINITE;
    int iRet, iLength=0, iNbrBytes;

	// Timeout setting
	// ***************
    if(ucDly != 0xFF)
        lSec = ucDly*1000;
	else
		lSec = LL_INFINITE;

	// Receiving first block
	// *********************

    iRet = TimerStart(0, lSec);                                         // Timer0 starts
    CHECK(iRet>=0, lblTimeOut);
	do
    {
    	iNbrBytes = LL_Receive(hSession, usLen, pcMsg+iLength, 1*100);  // Check reception
    	if (iNbrBytes != 0)
    		break;                                                      // Bytes received
		if (lSec != LL_INFINITE)
			lTimeOut = TimerGet(0);                                     // Retrieve timer value
    } while (lTimeOut>0);

    CHECK(lTimeOut!=0, lblTimeOut);                                     // Exit on timeout


	iLength += iNbrBytes;
	iRet = LL_GetLastError(hSession);
	CHECK (iRet==LL_ERROR_OK, lblEnd);
	if ((iNbrBytes != 0) && (iNbrBytes < usLen))
	{
		// Receiving next block until timeout (Inter block 500ms)
		// ======================================================
		while(1)
		{
			iNbrBytes = LL_Receive(hSession, usLen-iLength, pcMsg+iLength, 50);
			iLength += iNbrBytes;
			iRet = LL_GetLastError(hSession);
			CHECK ((iRet==LL_ERROR_OK) || (iRet==LL_ERROR_TIMEOUT), lblEnd);
			if ((iRet==LL_ERROR_TIMEOUT) || (iNbrBytes==0))
				break;
		}
	}

	iRet = iLength;
    goto lblEnd;

	// Errors treatment
    // ****************
lblTimeOut:                                                             // Configuration failed
   	iRet = LL_ERROR_TIMEOUT;
	timeout_var = 1;
   	goto lblEnd;
lblEnd:
    TimerStop(0);
    return iRet;
}

//****************************************************************************
//        int ReceiveUSB (LL_HANDLE hSession, char *pcMsg,
//                        word usLen, byte ucDly)
//  This function receives data through the USB port.
//    - LL_Receive() : Wait and receive data
//    - LL_GetLastError() : Retrieve the last error
//  This function has parameters.
//    hSession (I-) : Handle of the session
//    pcMsg (-O) : Data to receive
//    usLen (I-) : Maximum number of bytes to receive
//    ucDly (I-) : Timeout reception (in second, 0xFF infinite)
//  This function has return value
//    >=0 : Number of bytes received
//     <0 : Reception failed
//****************************************************************************

static int ReceiveUSB(LL_HANDLE hSession, char *pcMsg, word usLen, byte ucDly)
{
	// Local variables
    // ***************
    char cKey;
    long lSec, lTimeOut=LL_INFINITE;
    int iRet, iLength=0, iNbrBytes;

	// Timeout setting
	// ***************
    if(ucDly != 0xFF)
        lSec = ucDly*1000;
	else
		lSec = LL_INFINITE;

	// Receiving first block
	// *********************
    Telium_Reset_buf(hKbd, _receive_id);                                // Reset keyboard FIFO
    iRet = TimerStart(0, lSec);                                         // Timer0 starts
    CHECK(iRet>=0, lblTimeOut);
	do
    {
    	iNbrBytes = LL_Receive(hSession, usLen, pcMsg+iLength, 1*100);  // Check reception
    	if (iNbrBytes != 0)
    		break;                                                      // Bytes received
		iRet = Telium_Ttestall(KEYBOARD, 1);                            // Check for a key pressed
		if (iRet & KEYBOARD)                                            // Key detected
		{
			cKey = Telium_Getchar();                                    // Retrieve key pressed
			CHECK(cKey!=T_ANN, lblTimeOut);                             // Exit on cancel key
		}
		if (lSec != LL_INFINITE)
			lTimeOut = TimerGet(0);                                     // Retrieve timer value
    } while (lTimeOut>0);

    CHECK(lTimeOut!=0, lblTimeOut);                                     // Exit on timeout

	
	iLength += iNbrBytes;
	iRet = LL_GetLastError(hSession);
	CHECK (iRet==LL_ERROR_OK, lblEnd);
	if ((iNbrBytes != 0) && (iNbrBytes < usLen))
	{
		// Receiving next block until timeout (Inter block 500ms)
		// ======================================================
		while(1)
		{
			iNbrBytes = LL_Receive(hSession, usLen-iLength, pcMsg+iLength, 50);
			iLength += iNbrBytes;
			iRet = LL_GetLastError(hSession);
			CHECK ((iRet==LL_ERROR_OK) || (iRet==LL_ERROR_TIMEOUT), lblEnd);
			if ((iRet==LL_ERROR_TIMEOUT) || (iNbrBytes==0))
				break;
		}
	}

	iRet = iLength;
    goto lblEnd;

	// Errors treatment
    // ****************
lblTimeOut:                                                             // Configuration failed
   	iRet = LL_ERROR_TIMEOUT;
	timeout_var = 1;
   	goto lblEnd;
lblEnd:
    TimerStop(0);
    return iRet;
}

//****************************************************************************
//                int DisconnectUSB (LL_HANDLE hSession)
//  This function disconnects the USB port.      
//    - LL_Disconnect() : Disconnect Link Layer
//  This function has no parameters.
//    hSession (I-) : Handle of the session
//  This function has return value
//    >=0 : Disconnect done
//     <0 : Disconnect failed                                       
//****************************************************************************

static int DisconnectUSB(LL_HANDLE hSession)
{	
	// Local variables 
    // ***************
	int iRet;

	// Link Layer disconnection
	// ************************
	iRet = LL_Disconnect(hSession);

	return iRet;
}

//****************************************************************************
//                  int CloseUSB(LL_HANDLE hSession)
//  This function deletes the handle of the USB port.   
//    - LL_Configure() : Delete Link Layer configuration
//  This function has parameters.
//    hSession (IO) : Handle of the session
//  This function has return value
//    >=0 : Configuration deleted
//     <0 : Processing failed                                        
//****************************************************************************

static int CloseUSB(LL_HANDLE hSession)
{
	// Local variables 
    // ***************
    int iRet;

	// Delete the configuration
	// ************************
    iRet = LL_Configure(&hSession, NULL);

    return iRet;
}

//****************************************************************************
//                      void ComUSB(void)                            
//  This function communicates through USB port.   
//  This function has parameters.
//  This function has no return value
//****************************************************************************

void ComUSB(void) 
{
	// Local variables 
    // ***************
    LL_HANDLE *hUSB=NULL;
    char tcSnd[MAX_SND+1];
    char tcRsp[MAX_RSP+1];
	int iRet=0, iStatus=0;

	// Transmission through USB port in progress
	// *****************************************
    memset(tcSnd, 0, sizeof(tcSnd));
	memset(tcRsp, 0, sizeof(tcRsp));
	_clrscr();                                                      // Clear screen

    // Open USB port 
	// =============
	DisplayMedium(0, 0, "1-UcomOpen..     ");                        // Display start message
    hUSB = OpenUSB(); CHECK(hUSB!=NULL, lblKO);                     // ** Open **
    DisplayMedium(1, 0, "1-UcomOpen OK    ");                        // Display open OK

	// Connect USB port
	// ================
	DisplayMedium(2, 0, "2-UcomConnect... ");                        // Display connect message
    iRet = ConnectUSB(hUSB); CHECK(iRet>=0, lblComKO);              // ** Connect **
    DisplayMedium(3, 0, "2-UcomConnect OK ");                        // Display connect OK

	// Clear sending/receiving buffers
	// ===============================
	iRet = LL_ClearSendBuffer(hUSB);                                // Clear sending buffer
    CHECK(iRet==LL_ERROR_OK, lblComKO);
    iRet = LL_ClearReceiveBuffer(hUSB);                             // Clear receiving buffer
    CHECK(iRet==LL_ERROR_OK, lblComKO);

    // Send data through USB port
	// ==========================
	_clrscr();                                                      // Clear screen
	DisplayMedium(0, 0, "3-UcomSend...    ");                        // Display send message
    strcpy(tcSnd, "Hello, do you hear me?\r\n"                      // Data to send
		          "Hello, do you hear me?\r\n"
			   	  "Hello, do you hear me?\r\n"
				  "Hello, do you hear me?\r\n"
				  "Hello, do you hear me?\r\n");
    iRet = SendUSB(hUSB, tcSnd, (word) strlen(tcSnd));              // ** Send data **
	CHECK(iRet>=0, lblComKO);
	DisplayMedium(1, 0, "3-UcomSend OK    ");                        // Display send OK

	// Receive data through USB port
	// =============================
	DisplayMedium(2, 0, "4-UcomRec...     ");                        // Display receive message
    iRet = ReceiveUSB(hUSB, tcRsp, sizeof(tcRsp), 30);              // ** Receive data **
	CHECK(iRet>=0, lblComKO);
    if (iRet > MAX_RSP) strcpy (tcRsp, "Buffer overflow Max=512");  // Data overflow
	DisplayMedium(3, 0, "4-UcomRec OK     ");                        // Display receive OK

	// Disconnection
	// =============
	iRet = DisconnectUSB(hUSB);                                     // ** Disconnect **
	CHECK(iRet>=0, lblComKO);
	_clrscr();                                                      // Clear screen
	DisplayMedium(0, 0, "5-UcomDisconn OK");                         // Display disconnect OK
	iRet = CloseUSB(hUSB);                                          // ** Close **
	CHECK(iRet>=0, lblComKO);

	// Display transmission data
	// =========================
	_clrscr();                                                      // Clear screen
	DisplaySmall(0, 0, "RECEIVING:");                               // Display data to receive
	iRet = Display25(1, tcRsp, strlen(tcRsp)); CHECK(iRet>=0, lblKO);
    DisplaySmall(7, 6, "Press a key");

	// Print transmission data
	// =======================
//	iRet = Telium_Pprintf("\x1b""E""          USB Demo\n\n"         // Print demo in progress
//		           "\x1b""F"); CHECK(iRet>=0, lblKO);      
//  iRet = Telium_Pprintf("Sending:\n"); CHECK(iRet>=0, lblKO);     // Print data to send
//	iRet = Print24(tcSnd, strlen(tcSnd)); CHECK(iRet>=0, lblKO);
//  iRet = Telium_Pprintf("\nReceiving:\n"); CHECK(iRet>=0, lblKO); // Print data to receive
//	iRet = Print24(tcRsp, strlen(tcRsp)); CHECK(iRet>=0, lblKO);
//    iRet = Telium_Pprintf("\n\n\n\n\n\n"); CHECK(iRet>=0, lblKO);

	Telium_Reset_buf(hKbd, _receive_id);                            // Reset keyboard FIFO
	Telium_Ttestall(KEYBOARD, 30*100);                              // Wait until key pressed or timeout
	Telium_Reset_buf(hKbd, _receive_id);                            // Reset keyboard FIFO to clear key pressed

    goto lblEnd;

	// Errors treatment 
    // ****************
lblKO:                                                              // None-classified low level error
	_clrscr();                                                      // Clear screen
	DisplayMedium(0, 0, "Processing Error");                        // Oops! error
    Telium_Ttestall(0,5*100);                                       // Delay 5s
    goto lblEnd;
lblComKO:                                                           // Communication error
    _clrscr();                                                      // Clear screen
    DisplayMedium(0, 0, LL_ErrorMsg(iRet));                         // Link Layer error
	if (iRet == LL_Network_GetStatus(LL_PHYSICAL_V_USB, &iStatus))
	{
		switch(iStatus)
		{
		// General physical status
		case LL_STATUS_PERIPHERAL_BUSY:             iStatus=LL_STATUS_PHY_PERIPHERAL_BUSY;         break;
		case LL_STATUS_PERIPHERAL_OUT_OF_BASE:      iStatus=LL_STATUS_PHY_PERIPHERAL_OUT_OF_BASE;  break;
		case LL_STATUS_CONVERTER_NOT_PLUGGED:       iStatus=LL_STATUS_PHY_CONVERTER_NOT_PLUGGED;   break;
		// Bluetooth status
		case LL_BT_STATUS_NO_BASE_ASSOCIATED:       iStatus=LL_STATUS_BT_NO_BASE_ASSOCIATED;       break;
		case LL_BT_STATUS_PERIPHERAL_NOT_AVAILABLE: iStatus=LL_STATUS_BT_PERIPHERAL_NOT_AVAILABLE; break;
		case LL_BT_STATUS_PERIPHERAL_BUSY:          iStatus=LL_STATUS_BT_PERIPHERAL_BUSY;          break;
		case LL_BT_STATUS_BASE_NOT_AVAILABLE:       iStatus=LL_STATUS_BT_BASE_NOT_AVAILABLE;       break;
		default:                                    iStatus=-1;                                    break;
		}
		DisplayMedium(1, 0, LL_ErrorMsg(iStatus));                  // Link Layer status
	}
	Telium_Ttestall(0,2*100);                                       // Delay 5s
	goto lblEnd;
lblEnd:
    if (hUSB) {
		DisconnectUSB(hUSB);                                        // ** Disconnect **
		CloseUSB(hUSB);                                             // ** Close **
	}
}
LL_HANDLE 	hUSBSession;
int 		g_iPhysicalUSB=0;

int appel_caisseUSB(void)
{
	// Local variables 
    // ***************
    //Cless_ LL_HANDLE *hUSB=NULL;
    char tcSnd[MAX_SND+1];
    char tcRsp[MAX_RSP+1];
	int iRet=0, iStatus=0;

	// Transmission through USB port in progress
	// *****************************************
    memset(tcSnd, 0, sizeof(tcSnd));
	memset(tcRsp, 0, sizeof(tcRsp));
	_clrscr();                                                      // Clear screen

    // Open USB port 
	// =============
//	DisplayMedium(0, 0, "1-UcomOpen..     ");                        // Display start message
    hUSBSession = OpenUSB(); CHECK(hUSBSession!=NULL, lblKO);                     // ** Open **
//    DisplayMedium(1, 0, "1-UcomOpen OK    ");                        // Display open OK

	// Connect USB port
	// ================
//	DisplayMedium(2, 0, "2-UcomConnect... ");                        // Display connect message
    iRet = ConnectUSB(hUSBSession); CHECK(iRet>=0, lblComKO);              // ** Connect **
//    DisplayMedium(3, 0, "2-UcomConnect OK ");                        // Display connect OK

	// Clear sending/receiving buffers
	// ===============================
	iRet = LL_ClearSendBuffer(hUSBSession);                                // Clear sending buffer
    CHECK(iRet==LL_ERROR_OK, lblComKO);
    iRet = LL_ClearReceiveBuffer(hUSBSession);                             // Clear receiving buffer
    CHECK(iRet==LL_ERROR_OK, lblComKO);

    goto lblEnd;

	// Errors treatment 
    // ****************
lblKO:                                                              // None-classified low level error
	_clrscr();                                                      // Clear screen
	DisplayMedium(0, 0, "Processing Error");                        // Oops! error
    Telium_Ttestall(0,5*100);                                       // Delay 5s
    goto lblEnd;
lblComKO:                                                           // Communication error
    _clrscr();                                                      // Clear screen
    DisplayMedium(0, 0, LL_ErrorMsg(iRet));                         // Link Layer error
	if (iRet == LL_Network_GetStatus(LL_PHYSICAL_V_USB, &iStatus))
	{
		switch(iStatus)
		{
		// General physical status
		case LL_STATUS_PERIPHERAL_BUSY:             iStatus=LL_STATUS_PHY_PERIPHERAL_BUSY;         break;
		case LL_STATUS_PERIPHERAL_OUT_OF_BASE:      iStatus=LL_STATUS_PHY_PERIPHERAL_OUT_OF_BASE;  break;
		case LL_STATUS_CONVERTER_NOT_PLUGGED:       iStatus=LL_STATUS_PHY_CONVERTER_NOT_PLUGGED;   break;
		// Bluetooth status
		case LL_BT_STATUS_NO_BASE_ASSOCIATED:       iStatus=LL_STATUS_BT_NO_BASE_ASSOCIATED;       break;
		case LL_BT_STATUS_PERIPHERAL_NOT_AVAILABLE: iStatus=LL_STATUS_BT_PERIPHERAL_NOT_AVAILABLE; break;
		case LL_BT_STATUS_PERIPHERAL_BUSY:          iStatus=LL_STATUS_BT_PERIPHERAL_BUSY;          break;
		case LL_BT_STATUS_BASE_NOT_AVAILABLE:       iStatus=LL_STATUS_BT_BASE_NOT_AVAILABLE;       break;
		default:                                    iStatus=-1;                                    break;
		}
		DisplayMedium(1, 0, LL_ErrorMsg(iStatus));                  // Link Layer status
	}
	Telium_Ttestall(0,2*100);                                       // Delay 5s
	goto lblEnd;
lblEnd:
    return OK;
}

int send_caisseUSB(unsigned char *buffer, int len, char fermport)
{
	return SendUSB(hUSBSession, (PC)buffer, len);
} 

int send_caisse_echoUSB(unsigned char *buffer, int len, char fermport)
{
	return SendUSB(hUSBSession, (PC)buffer, len);
} 

int receive_caisseUSB_SansKeyBoard(unsigned char *buffer, int len, char fermport)
{
	// Local variables
    // ***************
    //Cless_ LL_HANDLE *hUSB=NULL;
    char tcSnd[MAX_SND+1];
    char tcRsp[MAX_RSP+1];
	int iRet=0, iStatus=0;

	// Transmission through USB port in progress
	// *****************************************
    memset(tcSnd, 0, sizeof(tcSnd));
	memset(tcRsp, 0, sizeof(tcRsp));
	_clrscr();                                                      // Clear screen
	timeout_var = 0;

	// Receive data through USB port
	// =============================
    iRet = ReceiveUSB_SansKeyBoard(hUSBSession, tcRsp, sizeof(tcRsp), 10);      // ** Receive data ** // 10 secondes pour suffisamment large pour le porteur de la carte
	CHECK(iRet>=0, lblComKO);
    if (iRet > MAX_RSP) strcpy (tcRsp, "Buffer overflow Max=512");  // Data overflow
	memcpy(buffer,tcRsp,iRet);

	//Telium_Reset_buf(hKbd, _receive_id);                            // Reset keyboard FIFO
	//Telium_Reset_buf(hKbd, _receive_id);                            // Reset keyboard FIFO to clear key pressed

    goto lblEnd;

	// Errors treatment
    // ****************
lblComKO:                                                           // Communication error
    _clrscr();                                                      // Clear screen
    APEMV_UI_MessageDisplayDelay (LL_ErrorMsg(iRet),2);             // Link Layer error
	if (iRet == LL_Network_GetStatus(LL_PHYSICAL_V_USB, &iStatus))
	{
		switch(iStatus)
		{
		// General physical status
		case LL_STATUS_PERIPHERAL_BUSY:             iStatus=LL_STATUS_PHY_PERIPHERAL_BUSY;         break;
		case LL_STATUS_PERIPHERAL_OUT_OF_BASE:      iStatus=LL_STATUS_PHY_PERIPHERAL_OUT_OF_BASE;  break;
		case LL_STATUS_CONVERTER_NOT_PLUGGED:       iStatus=LL_STATUS_PHY_CONVERTER_NOT_PLUGGED;   break;
		// Bluetooth status
		case LL_BT_STATUS_NO_BASE_ASSOCIATED:       iStatus=LL_STATUS_BT_NO_BASE_ASSOCIATED;       break;
		case LL_BT_STATUS_PERIPHERAL_NOT_AVAILABLE: iStatus=LL_STATUS_BT_PERIPHERAL_NOT_AVAILABLE; break;
		case LL_BT_STATUS_PERIPHERAL_BUSY:          iStatus=LL_STATUS_BT_PERIPHERAL_BUSY;          break;
		case LL_BT_STATUS_BASE_NOT_AVAILABLE:       iStatus=LL_STATUS_BT_BASE_NOT_AVAILABLE;       break;
		default:                                    iStatus=-1;                                    break;
		}
		DisplayMedium(1, 0, LL_ErrorMsg(iStatus));                  // Link Layer status
	}
	return KO;
lblEnd:
	return OK;
}


int receive_caisseUSB(unsigned char *buffer, int len, char fermport)
{
	// Local variables 
    // ***************
    //Cless_ LL_HANDLE *hUSB=NULL;
    char tcSnd[MAX_SND+1];
    char tcRsp[MAX_RSP+1];
	int iRet=0, iStatus=0;

	// Transmission through USB port in progress
	// *****************************************
    memset(tcSnd, 0, sizeof(tcSnd));
	memset(tcRsp, 0, sizeof(tcRsp));
	_clrscr();                                                      // Clear screen
	timeout_var = 0;

	// Receive data through USB port
	// =============================
//	DisplayMedium(2, 0, "4-UcomRec...     ");                        // Display receive message
    iRet = ReceiveUSB(hUSBSession, tcRsp, sizeof(tcRsp), 0xFF);      // ** Receive data ** // infini timeout
	CHECK(iRet>=0, lblComKO);
    if (iRet > MAX_RSP) strcpy (tcRsp, "Buffer overflow Max=512");  // Data overflow
	memcpy(buffer,tcRsp,iRet);

//	DisplayMedium(3, 0, "4-UcomRec OK     ");                        // Display receive OK

	// Display transmission data
	// =========================
//	_clrscr();                                                      // Clear screen
//	DisplaySmall(0, 0, "RECEIVING:");                               // Display data to receive
//	iRet = Display25(1, tcRsp, strlen(tcRsp)); CHECK(iRet>=0, lblKO);
//  DisplaySmall(7, 6, "Press a key");

	// Print transmission data
	// =======================
//	iRet = Telium_Pprintf("\x1b""E""          USB Demo\n\n"         // Print demo in progress
//		           "\x1b""F"); CHECK(iRet>=0, lblKO);      
//  iRet = Telium_Pprintf("Sending:\n"); CHECK(iRet>=0, lblKO);     // Print data to send
//	iRet = Print24(tcSnd, strlen(tcSnd)); CHECK(iRet>=0, lblKO);
//  iRet = Telium_Pprintf("\nReceiving:\n"); CHECK(iRet>=0, lblKO); // Print data to receive
//	iRet = Print24(tcRsp, strlen(tcRsp)); CHECK(iRet>=0, lblKO);
//    iRet = Telium_Pprintf("\n\n\n\n\n\n"); CHECK(iRet>=0, lblKO);

	Telium_Reset_buf(hKbd, _receive_id);                            // Reset keyboard FIFO
	Telium_Ttestall(KEYBOARD, 1*100);                               // Wait until key pressed or timeout
	Telium_Reset_buf(hKbd, _receive_id);                            // Reset keyboard FIFO to clear key pressed

    goto lblEnd;

	// Errors treatment 
    // ****************
lblComKO:                                                           // Communication error
    _clrscr();                                                      // Clear screen
    DisplayMedium(0, 0, LL_ErrorMsg(iRet));                         // Link Layer error
	if (iRet == LL_Network_GetStatus(LL_PHYSICAL_V_USB, &iStatus))
	{
		switch(iStatus)
		{
		// General physical status
		case LL_STATUS_PERIPHERAL_BUSY:             iStatus=LL_STATUS_PHY_PERIPHERAL_BUSY;         break;
		case LL_STATUS_PERIPHERAL_OUT_OF_BASE:      iStatus=LL_STATUS_PHY_PERIPHERAL_OUT_OF_BASE;  break;
		case LL_STATUS_CONVERTER_NOT_PLUGGED:       iStatus=LL_STATUS_PHY_CONVERTER_NOT_PLUGGED;   break;
		// Bluetooth status
		case LL_BT_STATUS_NO_BASE_ASSOCIATED:       iStatus=LL_STATUS_BT_NO_BASE_ASSOCIATED;       break;
		case LL_BT_STATUS_PERIPHERAL_NOT_AVAILABLE: iStatus=LL_STATUS_BT_PERIPHERAL_NOT_AVAILABLE; break;
		case LL_BT_STATUS_PERIPHERAL_BUSY:          iStatus=LL_STATUS_BT_PERIPHERAL_BUSY;          break;
		case LL_BT_STATUS_BASE_NOT_AVAILABLE:       iStatus=LL_STATUS_BT_BASE_NOT_AVAILABLE;       break;
		default:                                    iStatus=-1;                                    break;
		}
		DisplayMedium(1, 0, LL_ErrorMsg(iStatus));                  // Link Layer status
	}
	Telium_Ttestall(0,2*100);                                       // Delay 5s
	goto lblEnd;
lblEnd:
	return OK;
} 

int send_receive_caisseUSB(unsigned char *buffer, int len, char fermport)
{
	return OK;
}

int	Wait_AmountUSB(void)
{


	return 0;
}

int Retour_Caisse(char * REPCODE)
{





	//APEMV_UI_MessageDisplayDelay ("RETOUR CAISSE\nUSB.CLESS", 1); 		//Test17
	//appel_caisseUSB();					//Test17

	////hKbd = Telium_Fopen("KEYBOARD", "r*");
	//APEMV_UI_MessageDisplayDelay("BBBB.02",0);

	if(!strlen((char *)REPCODE))
		memcpy(REPCODE,"880",3);

	if(memcmp(REPCODE,"000",3))
	{
		memcpy(StructInfoCaisse[1].STAN,StructInfoPayment.STAN,6);
		memcpy(StructInfoCaisse[1].NPRT,StructInfoPayment.Nom_Prenom,strlen((char*)StructInfoPayment.Nom_Prenom));
		memset(StructInfoCaisse[1].NCAR,'0',StructInfoPayment.Holder_Card[0]);
		memcpy(StructInfoCaisse[1].NCAR,&StructInfoPayment.Holder_Card[1],StructInfoPayment.Holder_Card[0]);
		memset(StructInfoCaisse[1].DAEX,'0',4);
		memcpy(StructInfoCaisse[1].DAEX,StructInfoPayment.Expired_Date,4);
		strncpy(StructInfoCaisse[1].MDLC,"SC",2);		//Cless_MDLC
		strncpy(&StructInfoCaisse[1].TM[0],"1",1);
		strncpy(&StructInfoCaisse[1].TM[1],&StructInfoCaisse[0].TM[1],2);
		strncpy(StructInfoCaisse[1].CRPIN,REPCODE,3);

		strncpy(&StructInfoCaisse[1].DA,&StructInfoCaisse[0].DA,9);
		strncpy(&StructInfoCaisse[1].HE,&StructInfoCaisse[0].HE,7);


		memset(&E_Transac8583,0x00,sizeof(E_Transac8583));
		memset(&R_Transac8583,0x00,sizeof(R_Transac8583));
		memset(&StructInfoPayment ,0x00,sizeof(StructInfoPayment));
		Sequence(OK,KEY_CLEAR);

		memset(buffer13,'\0',sizeof(buffer13));

		format_msg_caise2(buffer13,1);
		strcat(buffer13,"!");

		//send_caisseUSB((unsigned char*)buffer13, strlen(buffer13), 1);

		goto EndRC;
	}
	memcpy(StructInfoPayment.Approval_Code,R_Transac8583.champs38,6);


	Naps_Approve();//badr
	goto EndRC;
	//return 0;//badr


	if(SaveTra() != OK)
	{
		memcpy(StructInfoCaisse[1].STAN,StructInfoPayment.STAN,6);
		memcpy(StructInfoCaisse[1].NPRT,StructInfoPayment.Nom_Prenom,strlen((char *)StructInfoPayment.Nom_Prenom));
		memset(StructInfoCaisse[1].NCAR,'0',StructInfoPayment.Holder_Card[0]);
		memcpy(StructInfoCaisse[1].NCAR,&StructInfoPayment.Holder_Card[1],StructInfoPayment.Holder_Card[0]);
		memset(StructInfoCaisse[1].DAEX,'0',4);
		memcpy(StructInfoCaisse[1].DAEX,StructInfoPayment.Expired_Date,4);
		strncpy(&StructInfoCaisse[1].TM[0],"1",1);
		strncpy(&StructInfoCaisse[1].TM[1],&StructInfoCaisse[0].TM[1],2);
		strncpy(StructInfoCaisse[1].CRPIN,"306",3);

		memset(&E_Transac8583,0x00,sizeof(E_Transac8583));
		memset(&R_Transac8583,0x00,sizeof(R_Transac8583));
		memset(&StructInfoPayment ,0x00,sizeof(StructInfoPayment));
		Sequence(OK,KEY_CLEAR);

		memset(buffer13,'\0',sizeof(buffer13));

		format_msg_caise2(buffer13,1);
		strcat(buffer13,"!");

		//send_caisseUSB((unsigned char*)buffer13, strlen(buffer13), 1);
		goto EndRC;
	}
	else
	{
		strncpy(&StructInfoCaisse[1].TM[0],"1",1);
		strncpy(&StructInfoCaisse[1].TM[1],&StructInfoCaisse[0].TM[1],2);
		strncpy(&StructInfoCaisse[1].DA,&StructInfoCaisse[0].DA,9);
		strncpy(&StructInfoCaisse[1].HE,&StructInfoCaisse[0].HE,7);

		strncpy(&StructInfoCaisse[1].NCAI,&StructInfoCaisse[0].NCAI,9);
		strncpy(&StructInfoCaisse[1].NS,&StructInfoCaisse[0].NS,7);
		strncpy(&StructInfoCaisse[1].DE,&StructInfoCaisse[0].DE,4);

		strncpy(StructInfoCaisse[1].CRPIN,"000",3);
		memcpy(StructInfoCaisse[1].STAN,StructInfoPayment.STAN,6);
		memcpy(StructInfoCaisse[1].NPRT,StructInfoPayment.Nom_Prenom,strlen((char *)StructInfoPayment.Nom_Prenom));
		memset(StructInfoCaisse[1].NCAR,'0',StructInfoPayment.Holder_Card[0]);
		memcpy(StructInfoCaisse[1].NCAR,&StructInfoPayment.Holder_Card[1],StructInfoPayment.Holder_Card[0]);
		memset(StructInfoCaisse[1].DAEX,'0',4);
		memcpy(StructInfoCaisse[1].DAEX,StructInfoPayment.Expired_Date,4);

		strncpy(StructInfoCaisse[1].MDLC,"SC",2);		//Cless_MDLC

		PutAllTra();
		//Os__xprinta("Here 1");
//		GL_GraphicLib_SetCharset(APEMV_UI_GoalHandle(), GL_ENCODING_ISO_8859_1);
//		MessageWithoutConfirmation("Transaction validée", "file://flash/HOST/Approuvee.png");
		ImprimeTra(StructInfoPayment.Langue,COMMERCANT,0);
//		Os__xprinta("Here 2");
//		if(StructInfoPayment.CodeFunction == PREAUTHCODE)
//			MessageWithoutConfirmation("Retirer la carte SVP", "file://flash/HOST/retirer_la_carte_svp.png");
//		else
	    MessageWithWaitForAction("Couper ticket", "file://flash/HOST/couper_ticket.png", 20000);
		memset(buffer13,'\0',sizeof(buffer13));

		format_msg_caise2(buffer13,0);

		memset(&SaveLastInfoCaisse,0,sizeof(MSG_CAISSE));
		memcpy(&SaveLastInfoCaisse,&StructInfoCaisse[1],sizeof(MSG_CAISSE));
		strcat(buffer13,"!");
//		Telium_Ttestall(0,200);

			ImprimeTra(StructInfoPayment.Langue,CLIENT,0);
			strncpy(&StructInfoCaisse[1].TM[0],"1",1);
			strncpy(&StructInfoCaisse[1].TM[1],&StructInfoCaisse[0].TM[1],2);
			strncpy(StructInfoCaisse[1].CRPIN,"000",3);
			Sequence(OK,KEY_ENTER);
	}

EndRC :
	Telium_Ttestall(0,200);
//	if (hUSBSession)
//	{
//		DisconnectUSB(hUSBSession);
//		CloseUSB(hUSBSession);
//
//		FicPara.vCam0 = 3;
//		PutAllParam();
//	}
//	else
	{
		FicPara.vCam0 = 3;
		PutAllParam();
	}
	return 0;
}

void	Naps_Approve(void)
{
	ushort		Nb_preauto,i,j;
	ulong		Num_Seq;
	uchar		Date_YYMMDDHHmmss2[7];
	UC	        heure[4+1];
	DATE date;
	//	if((!memcmp(R_Transac8583.champs39,"000",3)) && (*p_status_tr == 0))


	Telium_Read_date(&date);
	sprintf(heure,"%.2s%.2s",date.hour,date.minute);
	sprintf(Date_YYMMDDHHmmss2,"%.2s%.2s%.2s",date.day, date.month, date.year);


	if((StructInfoPayment.CodeFunction == PREAUTHCOMPCODE) && (StructInfoPayment.Preauto_indic_completion == PRE_VAL_2_TRS))
	{


		StructInfoPayment.Top_Forcage = TOP_FOR_ONLINE;
		memcpy(StructInfoPayment.Approval_Code,R_Transac8583.champs38,6);
		StructInfoPayment.Transaction_Amount = StructInfoPayment.Preauto_Amount_Final;
		memcpy(StructInfoPayment.Num_seq_save_compl_online,StructInfoPayment.Num_sequence,6);
		if(ImprimeTra(FicPara.Langue,COMMERCANT,0) == OK)
		{
			/////////////////// Enregistrement de  pre-autorisation ////////////////////
			StructInfoPayment.Transaction_Amount = StructInfoPayment.Preauto_Amount_Initial;
			StructInfoPayment.Flag_Dem_Aut = 1; // larbi 10042019 0==>1
			StructInfoPayment.Top_Forcage = TOP_FOR_NORMAL;
			memcpy(StructInfoPayment.Approval_Code,StrPreAut.Approval_Code,6);
			memcpy(StructInfoPayment.STAN,StrPreAut.STAN,6);
			SaveTra();

			//////////////////////Enregistrement du  reliquat ///////////////
			Num_Seq = FicTra.nombre_tra+1;
			long_asc(StructInfoPayment.Num_sequence,6,(ulong *)&Num_Seq);
			StructInfoPayment.Transaction_Amount = StructInfoPayment.Preauto_Amount_Final - StructInfoPayment.Preauto_Amount_Initial;
			StructInfoPayment.Flag_Dem_Aut = 1;
			memcpy(StructInfoPayment.Approval_Code,R_Transac8583.champs38,6);
			StructInfoPayment.Top_Forcage = TOP_FOR_ONLINE;
			memcpy(StructInfoPayment.STAN,E_Transac8583.champs11,6);
			SaveTra();

			StructInfoPayment.Transaction_Amount = StructInfoPayment.Preauto_Amount_Final;
			MessageWithWaitForAction("Couper ticket", "file://flash/HOST/couper_ticket.png", 10000);
			ImprimeTra(StructInfoPayment.Langue,CLIENT,0); //Completion
			////////////////////////////////////
			PutAllTra();
			PutAllPre();
			PutAllParam();
			memset(&StrPreAut ,0x00,sizeof(StrPreAut));
			gindice = StructInfoPayment.gindice;//badr

			write_preauto_index( gindice ,&StrPreAut );
			Nb_preauto = FicPreauth.nombre_tra;//RDRT
			for(i = gindice+1;(i < Nb_preauto); i++)
			{
				read_preauto_index( i ,&StrPreAut );
				j=i-1;
				write_preauto_index( j ,&StrPreAut );
			}
			j=FicPreauth.nombre_tra-1;
			set_ushort_secourue(&FicPreauth.nombre_tra,j) ;
			////////////////////////////////////
		}
		//MessageWithoutConfirmation("Retirer la carte", "file://flash/HOST/retirer_la_carte_svp.png");
	}
	else
	{
		if(SaveTra() != OK)
		{
			memset(&E_Transac8583,0x00,sizeof(E_Transac8583));
			memset(&R_Transac8583,0x00,sizeof(R_Transac8583));
			memset(&StructInfoPayment ,0x00,sizeof(StructInfoPayment));
			Sequence(OK,KEY_CLEAR);
		}
		else
		{
			T_GL_HWIDGET hPicture=NULL;

//			hPicture = GoalCreatePicture(APEMV_UI_GoalHandle());                             // Create and clear screen
//			GoalDspPicture(hPicture, "Approuvee", "file://flash/HOST/Approuvee.png", &cs_xImage480, (GL_TIME_SECOND) * 2, true);
//			GoalDestroyPicture(&hPicture);
			if(StructInfoPayment.CodeFunction == PREAUTHCODE)
				MessageWithDelay("Préauto acceptée", "file://flash/HOST/Approuvee.png", 1000);
			else if (StructInfoPayment.CodeFunction == VOIDCODE)
				MessageWithDelay("Annulation acceptée", "file://flash/HOST/Approuvee.png", 1000);
			else
				MessageWithDelay("Transaction acceptée", "file://flash/HOST/Approuvee.png", 1000);




			ImprimeTra(StructInfoPayment.Langue,COMMERCANT,0);
			PutAllTra();
			PutAllPre();
			PutAllParam();
//			// jrizki 180319
//			if(StructInfoPayment.CodeFunction == PREAUTHCODE)
//			   MessageWithoutConfirmation("Retirer la carte", "file://flash/HOST/retirer_la_carte_svp.png");
//			else

			   MessageWithWaitForAction("Couper ticket", "file://flash/HOST/couper_ticket.png", 10000); // timeout 20 seconds || click on Valid key

			   ImprimeTra(StructInfoPayment.Langue,CLIENT,0);
//			if(StructInfoPayment.CodeFunction = PREAUTHCODE)
// 			{

//			   if((StructInfoPayment.Entry_Mode != TRS_MAG) && (StructInfoPayment.Entry_Mode != TRS_MAN)){ //belhadeg 141119
//				MessageWithoutConfirmation("Retirer la carte", "file://flash/HOST/retirer_la_carte_svp.png");
//			   }
				strncpy(&StructInfoCaisse[1].TM[0],"1",1);
				strncpy(&StructInfoCaisse[1].TM[1],&StructInfoCaisse[0].TM[1],2);
				strncpy(StructInfoCaisse[1].CRPIN,"000",3);
 				Sequence(OK,KEY_ENTER);
// 			}

		}
	}
}

