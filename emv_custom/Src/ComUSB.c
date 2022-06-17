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

static timeout_var = 0;

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
	//MessageWithDelay("piconfig", NULL, 1000);

    piConfig = TlvTree_New(LL_TAG_LINK_LAYER_CONFIG);                 // LinkLayer parameters Root tag of the configuration tree
    CHECK(piConfig!=0, lblKOConfigure);

	// Physical layer parameters  
	// =========================
	//MessageWithDelay("piPhysical", NULL, 1000);

	piPhysicalConfig = TlvTree_AddChild(piConfig,                        
		                                LL_TAG_PHYSICAL_LAYER_CONFIG, // TAG Physical layer parameters 
									    NULL,                         // VALUE (Null)
									    0);                           // LENGTH 0
    CHECK(piPhysicalConfig!=0, lblKOConfigure);

    // Port USB
	// --------
	//MessageWithDelay("AddChild", NULL, 1000);

	TlvTree_AddChildInteger(piPhysicalConfig, 
		                    LL_PHYSICAL_T_LINK,                       // TAG
		                    LL_PHYSICAL_V_USB,                        // VALUE
							LL_PHYSICAL_L_LINK);                      // LENGTH 1 byte
	
	// Link Layer configuration
	// ************************
	//MessageWithDelay("LL_Config", NULL, 1000);

	iRet = LL_Configure(&hSession, piConfig);                         // Initialize the handle of the session
	//MessageWithDelay("end of iRet", NULL, 1000);
	CHECK(iRet==LL_ERROR_OK, lblKOConfigure);
	//MessageWithDelay("end of LL_Config", NULL, 1000);
	goto lblEnd;

	// Errors treatment
    // ****************
lblKOConfigure:                                                       // Configuration failed
    hSession=NULL;
    //MessageWithDelay("hSession=NULL", NULL, 1000);
    goto lblEnd;
lblEnd:
    if (piConfig)
    	TlvTree_Release(piConfig);
    // Release tree to avoid memory leak
	//MessageWithDelay("return hSession", NULL, 1000);


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
    //MessageWithDelay("ConnectUSB", NULL, 1000);

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
    Telium_File_t* keyboard;

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
    keyboard = Telium_Fopen("KEYBOARD", "r");
	do
    {
    	iNbrBytes = LL_Receive(hSession, usLen, pcMsg+iLength, 1*100);  // Check reception
    	if (iNbrBytes != 0)
    	{
    		break;                                                      // Bytes received
    	}
		iRet = Telium_Ttestall(KEYBOARD, 1);                            // Check for a key pressed
		if (iRet & KEYBOARD)                                            // Key detected
		{
			cKey = Telium_Getchar();                                    // Retrieve key pressed
			CHECK(cKey!=T_ANN, lblTimeOut);                             // Exit on cancel key
		}
		if (lSec != LL_INFINITE)
			lTimeOut = TimerGet(0);                                     // Retrieve timer value
	    MessageWithDelay("before tel_auto", NULL, 1000);

		tel_auto();	////APEMV_UI_MessageDisplayDelay("\nCas ou le terminal attend la caisse en USB",2);
	    MessageWithDelay("after tel_auto", NULL, 1000);


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

	if (keyboard != NULL)
		Telium_Fclose (keyboard);

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
//	DisplayMedium(0, 0, "1-UcomOpen..     ");                        // Display start message
	//MessageWithDelay("OpenUSB", NULL, 1000);

    hUSBSession = OpenUSB(); //CHECK(hUSBSession!=NULL, lblKO);                     // ** Open **
//    DisplayMedium(1, 0, "1-UcomOpen OK    ");                        // Display open OK
	//MessageWithDelay("end OpenUSB", NULL, 1000);

	// Connect USB port
	// ================
//	DisplayMedium(2, 0, "2-UcomConnect... ");                        // Display connect message
    iRet = ConnectUSB(hUSBSession); CHECK(iRet>=0, lblComKO);              // ** Connect **
//    DisplayMedium(3, 0, "2-UcomConnect OK ");                        // Display connect OK
   // MessageWithDelay("end of ConnectUSB", NULL, 1000);
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
    //APEMV_UI_MessageDisplayDelay (LL_ErrorMsg(iRet),2);             // Link Layer error
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
    LL_HANDLE *hUSB=NULL;
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
    MessageWithDelay("before ReceiveUSB", NULL, 1000);
	iRet = ReceiveUSB(hUSBSession, tcRsp, sizeof(tcRsp), 0xFF);      // ** Receive data ** // infini timeout
    MessageWithDelay("end ReceiveUSB", NULL, 1000);
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

//	Telium_Reset_buf(hKbd, _receive_id);                            // Reset keyboard FIFO
//	Telium_Ttestall(KEYBOARD, 1*100);                               // Wait until key pressed or timeout
//	Telium_Reset_buf(hKbd, _receive_id);                            // Reset keyboard FIFO to clear key pressed

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
    //APEMV_UI_MessageDisplayDelay (LL_ErrorMsg(iRet), 2);	    	// DisplayMedium(0, 0, LL_ErrorMsg(iRet));                         // Link Layer error
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
		//APEMV_UI_MessageDisplayDelay (LL_ErrorMsg(iRet), 2);	    	// DisplayMedium(1, 0, LL_ErrorMsg(iStatus));                  // Link Layer status
	}
	//Telium_Ttestall(0,2*100);                                       // Delay 5s
	goto lblEnd;
lblEnd:
	return OK;
} 

int receive_caisseUSBAvecAnnulation(unsigned char *buffer, int len, char fermport)
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
	timeout_var = 0;

	// Receive data through USB port
	// =============================
//	DisplayMedium(2, 0, "4-UcomRec...     ");                     // Display receive message
    iRet = ReceiveUSB(hUSBSession, tcRsp, sizeof(tcRsp), 10);      // ** Receive data ** // 10 secondes pour suffisamment large pour le porteur de la carte
	CHECK(iRet>=0, lblComKO);
    if (iRet > MAX_RSP) strcpy (tcRsp, "Buffer overflow Max=512");  // Data overflow
	memcpy(buffer,tcRsp,iRet);

//	Telium_Reset_buf(hKbd, _receive_id);                            // Reset keyboard FIFO
//	Telium_Ttestall(KEYBOARD, 1*100);                               // Wait until key pressed or timeout
//	Telium_Reset_buf(hKbd, _receive_id);                            // Reset keyboard FIFO to clear key pressed

    goto lblEnd;

	// Errors treatment
    // ****************

lblComKO:                                                           // Communication error
    _clrscr();                                                      // Clear screen
    //APEMV_UI_MessageDisplayDelay(LL_ErrorMsg(iRet),2);				//DisplayMedium(0, 0, LL_ErrorMsg(iRet));                         // Link Layer error
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
		//APEMV_UI_MessageDisplayDelay (LL_ErrorMsg(iRet), 2);	    	// DisplayMedium(1, 0, LL_ErrorMsg(iStatus));                  // Link Layer status
	}
	return KO;

lblEnd:
	return OK;
}

int send_receive_caisseUSB(unsigned char *buffer, int len, char fermport)
{
	return OK;
}

int	Wait_AmountUSB(int cam0)
{
	GetAllParam();	//Cless_ Get last update if Cless
	GetAllTra();	//Cless_ Get last update if Cless

	//APEMV_UI_MessageDisplayDelay ("BONJOUR", 0); 			//Test17
	
	char buf[128];
	int ln;
	//char AFFMONT[48];
	//char DEVALP[4];
    int iRet;
    char cKey;

	Ushort	Type_Message;
	int h_clock=0, yr, mth, day, hr, min, sec, wday;
	
	memset(buffer12,'\0',sizeof(buffer12));
	memset(buffer13,'\0',sizeof(buffer13));
	
	memset(&StructInfoCaisse[0],0,sizeof(MSG_CAISSE));
	memset(&StructInfoCaisse[1],0,sizeof(MSG_CAISSE));
	
	memset(&StructInfoPayment ,0x00,sizeof(StructInfoPayment));
	StructInfoPayment.Author_Status = 0xff;
    MessageWithDelay("before receive_caisseUSB", NULL, 1000);
	receive_caisseUSB((unsigned char*)buf, ln,1);
    MessageWithDelay("end receive_caisseUSB", NULL, 1000);

	deco_msg_caise2((char *)buf,1);	
	MessageWithDelay("end deco_msg_caise2", NULL, 1000);
	Type_Message = (Ushort)atol((PC)StructInfoCaisse[0].TM);
	if (atoi(StructInfoCaisse[0].NS)==0){
		strncpy(&StructInfoCaisse[1].TM[0],"1",1);
		strncpy(&StructInfoCaisse[1].TM[1],&StructInfoCaisse[0].TM[1],2);
		strncpy(&StructInfoCaisse[1].CRPIN[1],"750",3);
		format_msg_caise2(buffer13,0);
		Type_Message=0;
	}
	if(
			strlen(StructInfoCaisse[0].NS)<=0 
		||	strlen(StructInfoCaisse[0].NCAI)<=0 
		||	strlen(StructInfoCaisse[0].TM)<=0 
		||	strlen(StructInfoCaisse[0].DA)<=0  
		||	strlen(StructInfoCaisse[0].HE)<=0
	){
		Type_Message=0;
    }
	
	memcpy(&StructInfoCaisse[1].TM[0],"1",1);


	if((FicPara.status != OK) && (Type_Message != 13))
	{

		//APEMV_UI_MessageDisplayDelay ("TPE NON\nREFERENCE", 1);
		strncpy(&StructInfoCaisse[1].TM[0],"1",1);
		strncpy(&StructInfoCaisse[1].TM[1],&StructInfoCaisse[0].TM[1],2);
		strncpy(StructInfoCaisse[1].CRPIN,"305",3);
		format_msg_caise2(buffer13,1);
		strcat(buffer13,"!");

		if(FicPara.vCam0 != 2)
			send_caisseUSB((unsigned char*)buffer13, strlen(buffer13), 1);


		if((!timeout_var)&&(FicPara.vCam0 != 2))
		{
	/*
			if (hUSBSession)
			{
				DisconnectUSB(hUSBSession);
				CloseUSB(hUSBSession);
				hUSBSession = NULL;
			}
	*/
			Wait_AmountUSB(0);
		}

	/**/
		if(FicPara.vCam0 == 2)
		{
			if (hUSBSession)
			{
				DisconnectUSB(hUSBSession);
				CloseUSB(hUSBSession);
				hUSBSession = NULL;
			}
		}
	/**/

		return 0;
	}


	switch(Type_Message)
	{		 
		case  1:	//Demande de paiement
/*
 			if(!memcmp(StructInfoCaisse[0].DE,"504",3))
				sprintf(DEVALP,"%s","MAD");
			else if(!memcmp(StructInfoCaisse[0].DE,"978",3))
				sprintf(DEVALP,"%s","EUR");
			else if(!memcmp(StructInfoCaisse[0].DE,"840",3))
				sprintf(DEVALP,"%s","USD");
			else
				sprintf(DEVALP,"%s","ERR");

			sprintf(AFFMONT,"%s\n%d,%.02d%s","MONTANT :                        ",atol(StructInfoCaisse[0].MT)/100,atol(StructInfoCaisse[0].MT)%100,DEVALP);
			//APEMV_UI_MessageDisplayDelay (AFFMONT, 2); 		//Test17
*/
			strncpy(StructInfoCaisse[1].TM,"101",3);			
			strncpy(StructInfoCaisse[1].MDLC,"CO",2);		//Cless_MDLC
			if(strlen(StructInfoCaisse[0].MT)>0){ 
				strncpy(StructInfoCaisse[1].CRPIN,"100",3);//Paiement KO par defaut
                
				//emv_read();
				// CUSERV_DEBIT_TRANSACTION == 0
				CUMORE_PaiementFctTrame(0, TRUE, atol(StructInfoCaisse[0].MT), 504);

				memset(buffer13,'\0',sizeof(buffer13));
				format_msg_caise2(buffer13,1);

				//memset(buffer13,'\0',sizeof(buffer13));
				clock2int(h_clock, (unsigned char *)NULL, &yr, &mth, &day, &hr, &min, &sec, &wday);
			}else{//paiement sans montant
				if(strlen(buffer12)>0){ 
					memset(buffer13,'\0',sizeof(buffer13));
					clock2int(h_clock, (unsigned char *)NULL, &yr, &mth, &day, &hr, &min, &sec, &wday);
					sprintf(buffer13,"013003307014008%.2d%.2d%.4d015006%.2d%.2d%.2d",day,mth,yr,hr,min,sec);
					clrscr();
					//APEMV_UI_MessageDisplayDelay ("Error Format Trame\nUSB", 1);
					}
			}
			strcat(buffer13,"!");
			//send_caisse(buffer13,strlen(buffer13),1);
			break;
			
		case  2:	//Demande de confirmation
			break;

		case  3:	//Demande d'annulation
			  //APEMV_UI_MessageDisplayDelay ("ANNULATION\nUSB", 2); 		//Test17
			  strncpy(StructInfoCaisse[1].TM,"103",3);
			  if(strlen(StructInfoCaisse[0].STAN)>0){
				  annulation_cmi();
				  format_msg_caise2(buffer13,1);
			  }
			  else{
				//Confirmation sans STAN
				if(strlen(buffer12)>0){
						memset(buffer13,'\0',sizeof(buffer13));
						clock2int(h_clock, (unsigned char *)NULL, &yr, &mth, &day, &hr, &min, &sec, &wday);
						sprintf(buffer13,"013003307014008%.2d%.2d%.4d015006%.2d%.2d%.2d",day,mth,yr,hr,min,sec);// M.O le 24/01/2012 bug recette client
						clrscr();
						//APEMV_UI_MessageDisplayDelay ("Error Format Trame\nUSB", 1);
					}
			  }

			strcat(buffer13,"!");
	        //send_caisse(buffer13,strlen(buffer13),1);
			break;

		case  4:	//Demande de confirmation d'annulation
			break;

		case  8:	//Demande de duplicata
			  //APEMV_UI_MessageDisplayDelay ("DUPLICATA\nUSB", 2);
			  strncpy(StructInfoCaisse[1].CRPIN,"111",3);

			  strncpy(StructInfoCaisse[1].STAN,StructInfoCaisse[0].STAN,6);

			  if((!memcmp(StructInfoCaisse[0].STAN,"000000",6)) || (StructInfoCaisse[0].STAN[0] == NULL))
		    	  dernieretrs_cmi();
		      else
		    	  autretrs_cmi();

		      strncpy(StructInfoCaisse[1].TM,"108",3);
		      format_msg_caise2(buffer13,1);
			  strcat(buffer13,"!");
			break;

		case  9:	//Demande de test réseau
			//APEMV_UI_MessageDisplayDelay ("TEST RESEAU\nUSB", 1);
			strncpy(StructInfoCaisse[1].TM,"109",3);
			testreseau_cmi();
			if (strncmp(StructInfoCaisse[1].CRPIN,"000",3) ==0){
				//APEMV_UI_MessageDisplayDelay ("RESEAU OK\nUSB", 1);
				ImpRese_ppad(1);
			}else{
				//APEMV_UI_MessageDisplayDelay ("RESEAU KO\nUSB", 1);
				ImpRese_ppad(0);
			}
			format_msg_caise2(buffer13,1);
			strcat(buffer13,"!");
			break;


		case  11:	//Demande d'impression
			//APEMV_UI_MessageDisplayDelay ("IMPRESSION DE TICKETS\nUSB", 1);
			strncpy(StructInfoCaisse[1].TM,"111",3);
			Imprime_Param_caisse();
			format_msg_caise2(buffer13,1);
			strcat(buffer13,"!");
			break;

		case  12:	//Demande de RAZ RAM
			//APEMV_UI_MessageDisplayDelay ("RAZ DU PINPAD\nUSB", 1);
			maintenance_ppad(StructInfoCaisse[1].CRPIN);
			strncpy(StructInfoCaisse[1].TM,"112",3);
			format_msg_caise2(buffer13,1);
			strcat(buffer13,"!");
			break;

		case  13:	//Demande de référencement
			//APEMV_UI_MessageDisplayDelay ("ORDRE DE REFERENCEMENT\nUSB", 1);
			initialisation_cmi();
			Imprime_Param();
			strncpy(StructInfoCaisse[1].TM,"113",3);
			format_msg_caise2(buffer13,1);
			strcat(buffer13,"!");
			break;

		default :
			if(!timeout_var)
			{
				memset(buffer13,'\0',sizeof(buffer13));
				clock2int(h_clock, (unsigned char *)NULL, &yr, &mth, &day, &hr, &min, &sec, &wday);
				sprintf(buffer13,"113003307014008%.2d%.2d%.4d015006%.2d%.2d%.2d",day,mth,yr,hr,min,sec);
				strcat(buffer13,"!");
				//APEMV_UI_MessageDisplayDelay ("Error Format Trame\nUSB", 1);
				//APEMV_UI_MessageDisplayDelay (buf, 1);
			}
			break;
	}	


	if((FicPara.vCam0 != 2) && (FicPara.AutoReversalOK != 3))
		send_caisseUSB((unsigned char*)buffer13, strlen(buffer13), 1);
	

	if(FicPara.AutoReversalOK == 3)
	{
		FicPara.AutoReversalOK =  4;
		PutAllParam();
	}


	if((!timeout_var)&&(FicPara.vCam0 != 2))
	{
/*
		if (hUSBSession)
		{
			DisconnectUSB(hUSBSession);
			CloseUSB(hUSBSession);
			hUSBSession = NULL;
		}
*/
		Wait_AmountUSB(0);
	}

/**/
	if(FicPara.vCam0 == 2)
	{
		if (hUSBSession)
		{
			DisconnectUSB(hUSBSession);
			CloseUSB(hUSBSession);
			hUSBSession = NULL;
		}
	}
/**/

	return 0;
}

LL_HANDLE 	f_hUSBSession()
{
	MessageWithDelay("f_hUSBSession", NULL, 1000);

	return hUSBSession;
}

