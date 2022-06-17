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
//  Serial communication management:
//             Transmission by serial communication COM0
//             Test done with Hyper-terminal (8-N-1-115200)
//  !!! Link Layer application must be loaded 34290302.SGN inside the terminal
//      AVL.lib + LinkLayerExeInterface.lib must be implemented to link properly
//
//  List of routines in file :
//      GetHandle : Handle of the configuration.
//      OpenSerial : Create the serial port.
//      SetSerial : Configure the serial port.
//      ConnectSerial : Connect the serial port.
//      SendSerial : Send data through the serial port.
//      ReceiveSerial : Receive data through the serial port.
//      DisconnectSerial : Disconnect the serial port.
//      CloseSerial : Delete the serial port.
//      PromptSerial : Prompt for serial's parameters.
//      ComSerial : COM0 communication demo.
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

//****************************************************************************
//      PRIVATE CONSTANTS
//****************************************************************************
#define MAX_SND 1024	//KT 512
#define MAX_RSP	1024	//KT 512

//****************************************************************************
//      PRIVATE TYPES
//****************************************************************************
enum eChn {                     ///< Com channels
    chnBeg,                     ///<start sentinel
    chnCom0,                    ///<Com0 Rs232
    chnCom1,                    ///<Com1 Rs232
    chnCom2,                    ///<Com2 Rs232
    chnComExt,                  ///<ComExt Usb->Rs232
    chnEnd                      ///<end sentinel
};

typedef struct sComChn
{
	byte chn;                   // communication channel used
} tComChn;

//****************************************************************************
//      PRIVATE DATA
//****************************************************************************
static const char *tzComSerial[] = { "RS232  (COM0)", "RS232  (COM1)",
		                              "RS232  (COM2)", "USB->RS232 (COM_EXT)" };
static const char *tzDataBits[] = { "7", "8" };
static const char *tzParity[] = { "NONE", "EVEN", "ODD" };
static const char *tzStopBits[] = { "1", "2" };
static const char *tzBaudRate[] = { "300", "1200", "2400", "4800", "9600",
	                                 "19200", "38400", "57600", "115200" };

//****************************************************************************
//           int SetSerial (const char *pcInit, enum eChn eComType)
//  This function configures the serial port.
//    - LL_Configure() : Create Link Layer configuration
//  This function has parameters.
//    pcInit (I-) :  DPSB com channel(s) (ex: "8N12400")
//           D = Number of data bits (7,8)
//           P = Parity (None, Even, Odd)
//           S = Stop bits (1,2)
//           B = Baud Rate (300,1200,2400,4800,9600,19200,38400,57600,115200)
//    eCommType (I-) : Communication channel used
//  This function has return value
//    !NULL : Handle of the session
//     NULL : Session failed
//****************************************************************************

static LL_HANDLE OpenSerial(const char *pcInit, enum eChn eComType)
{
	// Local variables
    // ***************
    byte ucDatasize;
    byte ucParity;
    byte ucStopbits;
    doubleword uiBps;
	byte ucComNbr;
	TLV_TREE_NODE piConfig=NULL;
	TLV_TREE_NODE piPhysicalConfig=NULL;
	LL_HANDLE hSession = NULL;                                        // Session handle
    int iRet;

	// Create the LinkLayer configuration parameters tree
	// **************************************************
    CHECK(pcInit!=NULL, lblKOConfigure);                              // Configuration error

    // Parsing initialization parameters
    // =================================
    ucDatasize = LL_PHYSICAL_V_8_BITS;                                // *** Number of data bits ***
    switch (*pcInit++)
    {
	case '7': ucDatasize = LL_PHYSICAL_V_7_BITS; break;               // 7 data bits
	case '8': ucDatasize = LL_PHYSICAL_V_8_BITS; break;               // 8 data bits
	default: goto lblKOConfigure;                                     // Configuration error
    }

	ucParity = LL_PHYSICAL_V_NO_PARITY;                               // *** Parity ***
    switch (*pcInit++)
    {
	case 'N': ucParity = LL_PHYSICAL_V_NO_PARITY;   break;            // No parity
	case 'E': ucParity = LL_PHYSICAL_V_EVEN_PARITY; break;            // Even parity
	case 'O': ucParity = LL_PHYSICAL_V_ODD_PARITY; break;             // Odd parity
	default: goto lblKOConfigure;                                     // Configuration error
    }

    ucStopbits = LL_PHYSICAL_V_1_STOP;                                // *** Number of stop bits ***
    switch (*pcInit++)
    {
    case '1': ucStopbits = LL_PHYSICAL_V_1_STOP; break;               // 1 stop bit
    case '2': ucStopbits = LL_PHYSICAL_V_2_STOP; break;               // 2 stop bits
    default: goto lblKOConfigure;                                     // Configuration error
    }

    if(*pcInit == 0) {                                                // *** Baud rate ***
        uiBps = LL_PHYSICAL_V_BAUDRATE_1200; pcInit++;                // 1200 bauds
	} else if(memcmp(pcInit, "300", 3) == 0) {
		uiBps = LL_PHYSICAL_V_BAUDRATE_300; pcInit+=3;                // 300 bauds
	} else if(memcmp(pcInit, "1200", 4) == 0) {
		uiBps = LL_PHYSICAL_V_BAUDRATE_1200; pcInit+=4;               // 1200 bauds
	} else if(memcmp(pcInit, "2400",4) == 0) {
		uiBps = LL_PHYSICAL_V_BAUDRATE_2400; pcInit+=4;               // 2400 bauds
	} else if(memcmp(pcInit, "4800", 4) == 0) {
		uiBps = LL_PHYSICAL_V_BAUDRATE_4800; pcInit+=4;               // 4800 bauds
	} else if(memcmp(pcInit, "9600", 4) == 0) {
		uiBps = LL_PHYSICAL_V_BAUDRATE_9600; pcInit+=4;               // 9600 bauds
	} else if(memcmp(pcInit, "19200", 5) == 0) {
		uiBps = LL_PHYSICAL_V_BAUDRATE_19200; pcInit+=5;              // 19200 bauds
	} else if(memcmp(pcInit, "38400", 5) == 0) {
		uiBps = LL_PHYSICAL_V_BAUDRATE_38400; pcInit+=5;              // 38400 bauds
	} else if(memcmp(pcInit, "57600", 5) == 0) {
		uiBps = LL_PHYSICAL_V_BAUDRATE_57600;  pcInit+=5;             // 57600 bauds
	} else if(memcmp(pcInit, "115200", 6) == 0) {
		uiBps = LL_PHYSICAL_V_BAUDRATE_115200; pcInit+=6;             // 115200 bauds
	} else
		goto lblKOConfigure;                                          // Configuration error

    // Create parameters tree
    // ======================
	piConfig = TlvTree_New(LL_TAG_LINK_LAYER_CONFIG);                 // LinkLayer parameters Root tag of the configuration tree
	CHECK(piConfig!=NULL, lblKOConfigure);

	// Physical layer parameters
	// =========================
	piPhysicalConfig = TlvTree_AddChild(piConfig,
		                                LL_TAG_PHYSICAL_LAYER_CONFIG, // TAG Physical layer parameters
									    NULL,                         // VALUE (Null)
									    0);                           // LENGTH 0
	CHECK(piPhysicalConfig!=NULL, lblKOConfigure);

    // Port Com(s)
	// -----------
    ucComNbr = (byte)(eComType==chnCom0 ? LL_PHYSICAL_V_COM0 :
	                  eComType==chnCom1 ? LL_PHYSICAL_V_COM1 :
					  eComType==chnCom2 ? LL_PHYSICAL_V_COM2 :
	                  LL_PHYSICAL_V_CONVERTER_USB_RS232);
	TlvTree_AddChildInteger(piPhysicalConfig,
		                    LL_PHYSICAL_T_LINK,                       // TAG
							ucComNbr,                                 // VALUE
							LL_PHYSICAL_L_LINK);                      // LENGTH 1 byte

	// Baud Rate
	// ---------
	TlvTree_AddChildInteger(piPhysicalConfig,
		                    LL_PHYSICAL_T_BAUDRATE,                   // TAG
	                        uiBps,                                    // VALUE (Integer)
                            LL_PHYSICAL_L_BAUDRATE);                  // LENGTH 4 bytes

    // Data bits
	// ---------
	TlvTree_AddChildInteger(piPhysicalConfig,
		                    LL_PHYSICAL_T_BITS_PER_BYTE,              // TAG
							ucDatasize,                               // VALUE
							LL_PHYSICAL_L_BITS_PER_BYTE);             // LENGTH 1 byte

    // Stop bits
	// ---------
	TlvTree_AddChildInteger(piPhysicalConfig,
		                    LL_PHYSICAL_T_STOP_BITS,                  // TAG
							ucStopbits,                               // VALUE
							LL_PHYSICAL_L_STOP_BITS);                 // LENGTH 1 byte

    // Parity
	// ------
	TlvTree_AddChildInteger(piPhysicalConfig,
		                    LL_PHYSICAL_T_PARITY,                     // TAG
							ucParity,                                 // VALUE
							LL_PHYSICAL_L_PARITY);                    // LENGTH 1 byte

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
//                    int ConnectSerial (LL_HANDLE hSession)
//  This function connects the serial port.
//    - LL_Connect() : Connect Link Layer
//  This function has no parameters.
//    hSession (I-) : Handle of the session
//  This function has return value
//    >=0 : Connect done
//     <0 : Connect failed
//****************************************************************************

static int ConnectSerial(LL_HANDLE hSession)
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
//     int SendSerial (LL_HANDLE hSession, const char *pcMsg, word usLen)
//  This function sends data through the serial port.
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

static int SendSerial(LL_HANDLE hSession, const char *pcMsg, word usLen)
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
//        int ReceiveSerial (LL_HANDLE hSession, char *pcMsg,
//                           word usLen, byte ucDly)
//  This function receives data through the serial port.
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

static int ReceiveSerial(LL_HANDLE hSession, char *pcMsg, word usLen, byte ucDly)
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
    Telium_Reset_buf(hKbd, _receive_id);                                 // Reset keyboard FIFO
    iRet = TimerStart(0, lSec);                                          // Timer0 starts
    CHECK(iRet>=0, lblTimeOut);
	do
    {
    	iNbrBytes = LL_Receive(hSession, usLen, pcMsg+iLength, 1*100);   // Check reception
    	if (iNbrBytes != 0)
    		break;                                                       // Bytes received
		iRet = Telium_Ttestall(KEYBOARD, 1);                             // Check for a key pressed
		if (iRet & KEYBOARD)                                             // Key detected
		{
			cKey = Telium_Getchar();                                     // Retrieve key pressed
			CHECK(cKey!=T_ANN, lblTimeOut);                              // Exit on cancel key
		}
		if (lSec != LL_INFINITE)
			lTimeOut = TimerGet(0);                                      // Retrieve timer value
    } while (lTimeOut>0);

    CHECK(lTimeOut!=0, lblTimeOut);                                      // Exit on timeout

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
lblTimeOut:                                                              // Timeout expired
	iRet = LL_ERROR_TIMEOUT;
	goto lblEnd;
lblEnd:
	TimerStop(0);
    return iRet;
}

//****************************************************************************
//                  int DisconnectSerial (LL_HANDLE hSession)
//  This function disconnects the serial port.
//    - LL_Disconnect() : Disconnect Link Layer
//  This function has no parameters.
//    hSession (I-) : Handle of the session
//  This function has return value
//    >=0 : Disconnect done
//     <0 : Disconnect failed
//****************************************************************************

static int DisconnectSerial(LL_HANDLE hSession)
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
//                 int CloseSerial (LL_HANDLE hSession)
//  This function deletes the handle of the serial port.
//    - LL_Configure() : Delete Link Layer configuration
//  This function has no parameters.
//    hSession (I-) : Handle of the session
//  This function has return value
//    >=0 : Configuration deleted
//     <0 : Processing failed
//****************************************************************************

static int CloseSerial(LL_HANDLE hSession)
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
//                      void PromptSerial (void)
//  This function asks for the Serial's parameters.
//  This function has no parameters.
//  This function has no return value
//****************************************************************************

void PromptSerial(void)
{
	// Local variables
    // ***************
	char tcSerialItem[lenSerialItem];
	char tcSerialInit[lenSerialInit+1];
    int iDefItemP, iDefItemN, iRet;

    // Retrieve serial parameters
    // **************************
	iRet = appGet(appSerialItem, tcSerialItem, lenSerialItem); CHECK(iRet>=0, lblDbaErr);
	iRet = appGet(appSerialInit, tcSerialInit, lenSerialInit+1); CHECK(iRet>=0, lblDbaErr);

	// Select com serial
	// *****************
	iDefItemP = tcSerialItem[0];                                                               // Retrieve previous item selected
	iDefItemN = ShowMenu("SERIAL", 2, iDefItemP, NUMBER_OF_ITEMS(tzComSerial), tzComSerial);
	CHECK(iDefItemN>=0, lblEnd);                                                               // Cancel, Timeout
    if (iDefItemN != iDefItemP)
    {
    	tcSerialItem[0] = iDefItemN;                                                           // Save new item selected
    	iRet = appPut(appSerialItem, tcSerialItem, lenSerialItem); CHECK(iRet>=0, lblDbaErr);  // Save serial parameters
    }

    // Select data bits
    // ****************
	iDefItemP = tcSerialItem[1];                                                               // Retrieve previous item selected
	iDefItemN = ShowMenu("DATA  BITS", 2, iDefItemP, NUMBER_OF_ITEMS(tzDataBits), tzDataBits);
	CHECK(iDefItemN>=0, lblEnd);                                                               // Cancel, Timeout
	if (iDefItemN != iDefItemP)
	{
		tcSerialInit[0] = *tzDataBits[iDefItemN];                                              // Save data bits
		tcSerialItem[1] = iDefItemN;                                                           // Save new item selected
		iRet = appPut(appSerialItem, tcSerialItem, lenSerialItem); CHECK(iRet>=0, lblDbaErr);  // Save serial parameters
		iRet = appPut(appSerialInit, tcSerialInit, lenSerialInit); CHECK(iRet>=0, lblDbaErr);
	}

    // Select parity
    // *************
	iDefItemP = tcSerialItem[2];                                                               // Retrieve previous item selected
	iDefItemN = ShowMenu("PARITY", 2, iDefItemP, NUMBER_OF_ITEMS(tzParity), tzParity);
	CHECK(iDefItemN>=0, lblEnd);                                                               // Cancel, Timeout
    if (iDefItemN != iDefItemP)
    {
    	tcSerialInit[1] = *tzParity[iDefItemN];                                                // Save parity
    	tcSerialItem[2] = iDefItemN;                                                           // Save new item selected
    	iRet = appPut(appSerialItem, tcSerialItem, lenSerialItem); CHECK(iRet>=0, lblDbaErr);  // Save serial parameters
    	iRet = appPut(appSerialInit, tcSerialInit, lenSerialInit); CHECK(iRet>=0, lblDbaErr);
    }

    // Select stop bits
    // ****************
	iDefItemP = tcSerialItem[3];                                                               // Retrieve previous item selected
	iDefItemN = ShowMenu("STOP  BITS", 2, iDefItemP, NUMBER_OF_ITEMS(tzStopBits), tzStopBits);
	CHECK(iDefItemN>=0, lblEnd);                                                               // Cancel, Timeout
	if (iDefItemN != iDefItemP)
	{
		tcSerialInit[2] = *tzStopBits[iDefItemN];                                              // Save stop bits
		tcSerialItem[3] = iDefItemN;                                                           // Save new item selected
		iRet = appPut(appSerialItem, tcSerialItem, lenSerialItem); CHECK(iRet>=0, lblDbaErr);  // Save serial parameters
		iRet = appPut(appSerialInit, tcSerialInit, lenSerialInit); CHECK(iRet>=0, lblDbaErr);
	}

    // Select baud rate
    // ****************
	iDefItemP = tcSerialItem[4];                                                               // Retrieve previous item selected
	iDefItemN = ShowMenu("BAUD  RATE", 2, iDefItemP, NUMBER_OF_ITEMS(tzBaudRate), tzBaudRate);
	CHECK(iDefItemN>=0, lblEnd);                                                               // Cancel, Timeout
	if (iDefItemN != iDefItemP)
	{
		strcpy (&tcSerialInit[3], tzBaudRate[iDefItemN]);                                      // Save baud rate
		tcSerialItem[4] = iDefItemN;                                                           // Save new item selected
		iRet = appPut(appSerialItem, tcSerialItem, lenSerialItem); CHECK(iRet>=0, lblDbaErr);  // Save serial parameters
		iRet = appPut(appSerialInit, tcSerialInit, lenSerialInit); CHECK(iRet>=0, lblDbaErr);
	}

	goto lblEnd;

	// Errors treatment
	// ****************
lblDbaErr:
	_clrscr();                                                                                 // Clear screen
	DisplayMedium(0, 0, FMG_ErrorMsg(iRet));                                                   // FMG error
	DisplayMedium(1, 0, "Reset Needed");                                                       // Rebuild data base
	Telium_Ttestall(0, 5*100);                                                                 // Delay 5s
	goto lblEnd;
lblEnd:
	return;
}

//****************************************************************************
//                        void ComSerial(void)
//  This function communicates through COM0 port at 8-N-1-115200.
//  This function has no parameters.
//  This function has no return value
//****************************************************************************

void ComSerial(void)
{
	// Local variables
    // ***************
    LL_HANDLE *hCOM=NULL;
    byte ucComType;
    int iPhysical=0;
	char tcSerialItem[lenSerialItem];
	char tcSerialInit[lenSerialInit+1];
    char tcSnd[MAX_SND+1];
    char tcRsp[MAX_RSP+1];
	int iRet=0, iStatus=0;;

	// Transmission through serial port in progress
	// ********************************************
    memset(tcSnd, 0, sizeof(tcSnd));
	memset(tcRsp, 0, sizeof(tcRsp));
	_clrscr();                                                           // Clear screen

    // Open COM port
	// =============
	DisplayMedium(0, 0, "1-comOpen...");                                 // Display open message
	iRet = appGet(appSerialItem, tcSerialItem, lenSerialItem);           // Retrieve com serial parameters
	CHECK(iRet>=0, lblDbaErr);
	switch (tcSerialItem[0])
	{
	case 0:
		//APEMV_UI_MessageDisplayDelay ((char *)"COM0\nUSB", 1);
		ucComType=chnCom0;                                               // Rs232 Com0
		iPhysical=LL_PHYSICAL_V_COM0;
		break;
	case 1:
		//APEMV_UI_MessageDisplayDelay ((char *)"COM1\nUSB", 1);
		ucComType=chnCom1;                                               // Rs232 Com1
		iPhysical=LL_PHYSICAL_V_COM1;
		break;
	case 2:
		//APEMV_UI_MessageDisplayDelay ((char *)"COM2\nUSB", 1);
		ucComType=chnCom2;                                               // Rs232 Com2
		iPhysical=LL_PHYSICAL_V_COM2;
		break;
	case 3:
		//APEMV_UI_MessageDisplayDelay ((char *)"USB\nUSB", 1);
		ucComType=chnComExt;                                             // Usb->Rs232 Com6
		iPhysical=LL_PHYSICAL_V_CONVERTER_USB_RS232;
		break;
	default:
		//APEMV_UI_MessageDisplayDelay ((char *)"DEFAUTL\nUSB", 1);
		ucComType=0xFF;
		break;
	}
	iRet = appGet(appSerialInit, tcSerialInit, lenSerialInit+1);         // Retrieve init string
	CHECK(iRet>=0, lblDbaErr);
    hCOM = OpenSerial(tcSerialInit, ucComType);                          // ** Open **
    CHECK(hCOM!=NULL, lblKO);
    DisplayMedium(1, 0, "1-comOpen OK");                                 // Display open OK

	// Connect COM port
	// ================
	DisplayMedium(2, 0, "2-comConnect...");                              // Display connect message
    iRet = ConnectSerial(hCOM); CHECK(iRet>=0, lblComKO);                // ** Connect **
    DisplayMedium(3, 0, "2-comConnect OK");                              // Display connect OK

	// Clear sending/receiving buffers
	// ===============================
	iRet = LL_ClearSendBuffer(hCOM);                                     // Clear sending buffer
    CHECK(iRet==LL_ERROR_OK, lblComKO);
    iRet = LL_ClearReceiveBuffer(hCOM);                                  // Clear receiving buffer
    CHECK(iRet==LL_ERROR_OK, lblComKO);

    // Send data through COM port
	// ==========================
    _clrscr();                                                           // Clear screen
	DisplayMedium(0, 0, "3-comSend...");                                 // Display send message
    strcpy(tcSnd, "Hello, do you hear me?\r\n"                           // Data to send
		          "Hello, do you hear me?\r\n"
			   	  "Hello, do you hear me?\r\n"
				  "Hello, do you hear me?\r\n"
				  "Hello, do you hear me?\r\n");
    iRet = SendSerial(hCOM, tcSnd, (word) strlen(tcSnd));                // ** Send data **
	CHECK(iRet>=0, lblComKO);
	DisplayMedium(1, 0, "3-comSend OK");                                 // Display send OK

	// Receive data through COM port
	// =============================
	DisplayMedium(2, 0, "4-comRec...");                                  // Display receive message
    iRet = ReceiveSerial(hCOM, tcRsp, sizeof(tcRsp), 30);                // ** Receive data **
	CHECK(iRet>=0, lblComKO);
    if (iRet > MAX_RSP) strcpy (tcRsp, "Buffer overflow Max=512");       // Data overflow
	DisplayMedium(3, 0, "4-comRec OK");                                  // Display receive OK

	// Disconnection
	// =============
	iRet = DisconnectSerial(hCOM);                                       // ** Disconnect **
	CHECK(iRet>=0, lblComKO);
	_clrscr();                                                           // Clear screen
	DisplayMedium(0, 0, "5-comDisconn OK");                              // Display disconnect OK
	iRet = CloseSerial(hCOM);                                            // ** Close **
	CHECK(iRet>=0, lblComKO);

	// Display transmission data
	// =========================
	_clrscr();                                                           // Clear screen
	DisplaySmall(0, 0, "RECEIVING:");                                    // Display data to receive
	iRet = Display25(1, tcRsp, strlen(tcRsp)); CHECK(iRet>=0, lblKO);
    DisplaySmall(7, 6, "Press a key");

	// Print transmission data
	// =======================
//	iRet = Telium_Pprintf("\x1b""E""        RS232 Demo\n\n"              // Print demo in progress
//		           "\x1b""F"); CHECK(iRet>=0, lblKO);
//  iRet = Telium_Pprintf("Sending:\n"); CHECK(iRet>=0, lblKO);          // Print data to send
//	iRet = Print24(tcSnd, strlen(tcSnd)); CHECK(iRet>=0, lblKO);
//  iRet = Telium_Pprintf("\nReceiving:\n"); CHECK(iRet>=0, lblKO);      // Print data to receive
//	iRet = Telium_Pprintf(tcRsp, strlen(tcRsp)); CHECK(iRet>=0, lblKO);
//  iRet = Telium_Pprintf("\n\n\n\n\n\n"); CHECK(iRet>=0, lblKO);

	Telium_Reset_buf(hKbd, _receive_id);                                 // Reset keyboard FIFO
	Telium_Ttestall(KEYBOARD, 30*100);                                   // Wait until key pressed or timeout
	Telium_Reset_buf(hKbd, _receive_id);                                 // Reset keyboard FIFO to clear key pressed

    goto lblEnd;

	// Errors treatment
    // ****************
lblKO:                                                                   // None-classified low level error
	_clrscr();                                                           // Clear screen
    DisplayMedium(0, 0, "Processing Error");                             // Oops! error
    Telium_Ttestall(0,5*100);                                            // Delay 5s
    goto lblEnd;
lblComKO:                                                                // Communication error
    _clrscr();                                                           // Clear screen
    DisplayMedium(0, 0, LL_ErrorMsg(iRet));                            	 // Link Layer error
	if (iRet == LL_Network_GetStatus(iPhysical, &iStatus))
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
		DisplayMedium(1, 0, LL_ErrorMsg(iStatus));                 	     // Link Layer status
	}
	Telium_Ttestall(0,5*100);                                            // Delay 5s
    goto lblEnd;
lblDbaErr:                                                               // Data base error
	_clrscr();                                                           // Clear screen
	DisplayMedium(0, 0, FMG_ErrorMsg(iRet));                             // FMG error
	DisplayMedium(1, 0, "Reset Needed");                                 // Rebuild data base
	Telium_Ttestall(0, 5*100);                                           // Delay 5s
	goto lblEnd;
lblEnd:
    if (hCOM) {
		DisconnectSerial(hCOM);                                          // ** Disconnect **
		CloseSerial(hCOM);                                               // ** Close **
	}
}


LL_HANDLE 	Com0hSession;
int 		g_iPhysical=0;

void Function_lblCom0KO()
{
	int iRet=0, iStatus=0;

	_clrscr();                                                           // Clear screen
    DisplayMedium(0, 0, LL_ErrorMsg(iRet));                            	 // Link Layer error
	if (iRet == LL_Network_GetStatus(g_iPhysical, &iStatus))
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
		DisplayMedium(1, 0, LL_ErrorMsg(iStatus));                 	     // Link Layer status
	}
	Telium_Ttestall(0,5*100);                                            // Delay 5s
}

int appel_caisse(void)
{
	// Local variables
    // ***************
    byte ucComType;
	char tcSerialItem[lenSerialItem];
	char tcSerialInit[lenSerialInit+1];
	int iRet=0;

	_clrscr();                                                           // Clear screen

    // Open COM port
	// =============
	DisplayMedium(0, 0, "1-comOpen...");                                 // Display open message
	iRet = appGet(appSerialItem, tcSerialItem, lenSerialItem);           // Retrieve com serial parameters
	//CHECK(iRet>=0, lblDbaErr);
	if(iRet < 0)				//Remplacer17
	{
		_clrscr();                                                                                 // Clear screen
		DisplayMedium(0, 0, FMG_ErrorMsg(iRet));                                                   // FMG error
		DisplayMedium(1, 0, "Reset Needed");                                                       // Rebuild data base
		Telium_Ttestall(0, 5*100);                                                                 // Delay 5s
		return KO;
	}
	switch (tcSerialItem[0])
	{
	case 0:
		ucComType=chnCom0;                                               // Rs232 Com0
		g_iPhysical=LL_PHYSICAL_V_COM0;
		break;
	case 1:
		ucComType=chnCom1;                                               // Rs232 Com1
		g_iPhysical=LL_PHYSICAL_V_COM1;
		break;
	case 2:
		ucComType=chnCom2;                                               // Rs232 Com2
		g_iPhysical=LL_PHYSICAL_V_COM2;
		break;
	case 3:
		ucComType=chnComExt;                                             // Usb->Rs232 Com6
		g_iPhysical=LL_PHYSICAL_V_CONVERTER_USB_RS232;
		DisplayMedium(1, 0, "USB USB");
		break;
	default:
		ucComType=0xFF;
		break;
	}

	iRet = appGet(appSerialInit, tcSerialInit, lenSerialInit+1);         // Retrieve init string
	//CHECK(iRet>=0, lblDbaErr);
	if(iRet < 0)				//Remplacer17
	{
		_clrscr();                                                                                 // Clear screen
		DisplayMedium(0, 0, FMG_ErrorMsg(iRet));                                                   // FMG error
		DisplayMedium(1, 0, "Reset Needed");                                                       // Rebuild data base
		Telium_Ttestall(0, 5*100);                                                                 // Delay 5s
		return KO;
	}
    Com0hSession = OpenSerial(tcSerialInit, ucComType);                          // ** Open **
    //CHECK(Com0hSession!=NULL, lblKO);
    if(Com0hSession == NULL)			//Remplacer17
    {
		_clrscr();                                                           // Clear screen
		DisplayMedium(0, 0, "Processing Error");                             // Oops! error
		Telium_Ttestall(0,5*100);                                            // Delay 5s
		return KO;
    }
    DisplayMedium(1, 0, "1-comOpen OK");                                 // Display open OK

	// Connect COM port
	// ================
	DisplayMedium(2, 0, "2-comConnect...");                              // Display connect message
    iRet = ConnectSerial(Com0hSession); //CHECK(iRet>=0, lblComKO);                // ** Connect **
    if(iRet < 0)
    {
		Function_lblCom0KO();
    	return KO;
    }
    DisplayMedium(3, 0, "2-comConnect OK");                              // Display connect OK

    return OK;
}

int send_caisse(unsigned char *buffer, int len, char fermport)
{
	return SendSerial(Com0hSession, (PC)buffer, len);
}

int send_caisse_echo(unsigned char *buffer, int len, char fermport)
{
	return send_caisse_echoUSB(buffer,len,fermport);
	
	return SendSerial(Com0hSession, (PC)buffer, len);
} 

int receive_caisse(unsigned char *buffer, int len, char fermport)
{

	return receive_caisseUSB(buffer, len,1);

	// Local variables
    // ***************
    char tcRsp[MAX_RSP+1];
	int iRet=0;

	// Transmission through serial port in progress
	// ********************************************
	memset(tcRsp, 0, sizeof(tcRsp));
	_clrscr();                                                           // Clear screen


	// Clear sending/receiving buffers
	// ===============================
	iRet = LL_ClearSendBuffer(Com0hSession);                                     // Clear sending buffer
    //CHECK(iRet==LL_ERROR_OK, lblComKO);
	if(iRet!=LL_ERROR_OK)			//Remplacer17
	{
		Function_lblCom0KO();
		return KO;
	}
    iRet = LL_ClearReceiveBuffer(Com0hSession);                                  // Clear receiving buffer
    //CHECK(iRet==LL_ERROR_OK, lblComKO);
	if(iRet!=LL_ERROR_OK)			//Remplacer17
	{
		Function_lblCom0KO();
		return KO;
	}

	// Receive data through COM port
	// =============================
	DisplayMedium(2, 0, "4-comRec...");                                  // Display receive message
    iRet = ReceiveSerial(Com0hSession, tcRsp, sizeof(tcRsp), 0xFF);      // ** Receive data ** // infini timeout
	//CHECK(iRet>=0, lblComKO);
	if(iRet < 0)					//Remplacer17
	{
		Function_lblCom0KO();
		return KO;
	}
    if (iRet > MAX_RSP) strcpy (tcRsp, "Buffer overflow Max=512");       // Data overflow
	DisplayMedium(3, 0, "4-comRec OK");                                  // Display receive OK
	memcpy(buffer,tcRsp,iRet);
	return OK;
} 

int send_receive_caisse(unsigned char *buffer, int len, char fermport)
{
	send_caisse_echo(buffer, len, fermport);

	memset(buffer,'\0', sizeof(buffer));
	len =0;

	return receive_caisseUSBAvecAnnulation(buffer,len,fermport);	//receive_caisse(buffer, len, fermport);

	return OK;
}
int deco_msg_caise2(char *buffer,char trcc)
{   	  
	char tag[4];
	char c_length[4]; 
	short l_length;
	short l_length_msg=0;
	char buff[4];

	int i; 


	memset(buff,'\0',4);
	
	memset(StructInfoCaisse[0].TM,'\0',4);
	memset(StructInfoCaisse[0].NCAI,'\0',9);/*M.O le 09/01/2012 problème NCAI erroné*/
	memset(StructInfoCaisse[1].NCAI,'\0',9);/*M.O le 09/01/2012 problème NCAI erroné*/
		
	
	memset(c_length,0,4);
	Os__xprint("deb");			 	

	i=0;
	while(i<10)
	{
		memset(tag,'\0',4);
		memset(c_length,'\0',4);
		strncpy(tag,&buffer[l_length_msg],3);
		strncpy(c_length,&buffer[l_length_msg+3],3);
  
		l_length=atol(c_length);
		if(!memcmp(tag,"001",3))
		{
			strncpy(StructInfoCaisse[0].TM,&buffer[l_length_msg+6],l_length);
		}
				   	
	 if(!memcmp(tag,"002",3))

					{

					strncpy(StructInfoCaisse[0].MT,&buffer[l_length_msg+6],l_length);	
					strncpy(StructInfoCaisse[1].MT,&buffer[l_length_msg+6],l_length);					 
				   	}
	 			
	 if(!memcmp(tag,"003",3))
						{
						
						strncpy(StructInfoCaisse[0].NCAI,&buffer[l_length_msg+6],l_length);
					 	strncpy(StructInfoCaisse[1].NCAI,&buffer[l_length_msg+6],l_length);
					 	   	Os__xprint(StructInfoCaisse[0].NCAI);
					 	   	Os__xprint("NCAI");
						}
	
	 if(!memcmp(tag,"004",3))
						{strncpy(StructInfoCaisse[0].NS,&buffer[l_length_msg+6],l_length);
						 strncpy(StructInfoCaisse[1].NS,&buffer[l_length_msg+6],l_length);	 						 
	          	 	Os__xprint(StructInfoCaisse[0].NS);
	          	 	Os__xprint("NS");}
	        
	   if(!memcmp(tag,"005",3))
						{strncpy(StructInfoCaisse[0].NSA,&buffer[l_length_msg+6],l_length);  						 
	          	 	Os__xprint(StructInfoCaisse[0].NSA);
	          	 	Os__xprint("NSA");}
	          	 	
	   if(!memcmp(tag,"006",3))
						{strncpy(StructInfoCaisse[0].NHC,&buffer[l_length_msg+6],l_length);  						 
	          	 	Os__xprint(StructInfoCaisse[0].NHC);
	          	 	Os__xprint("NHC");}
	          	 		          	 	
	   if(!memcmp(tag,"007",3))
						{strncpy(StructInfoCaisse[0].NCAR,&buffer[l_length_msg+6],l_length);  						 
	          	 	Os__xprint(StructInfoCaisse[0].NCAR);
	          	 	Os__xprint("NCAR");}     	 	
	          	 	
	   if(!memcmp(tag,"008",3))
						{strncpy(StructInfoCaisse[0].STAN,&buffer[l_length_msg+6],l_length);  						 
	          	 	Os__xprint(StructInfoCaisse[0].STAN);
	          	 	Os__xprint("STAN");}          	 	
	    
	    if(!memcmp(tag,"009",3))
						{strncpy(StructInfoCaisse[0].NA1,&buffer[l_length_msg+6],l_length);  						 
	          	 	Os__xprint(StructInfoCaisse[0].NA1);
	          	 	Os__xprint("NA1");}  
	          	 	
	    if(!memcmp(tag,"010",3))
						{strncpy(StructInfoCaisse[0].DP,&buffer[l_length_msg+6],l_length);  						 
	          	 	Os__xprint(StructInfoCaisse[0].DP);
	          	 	Os__xprint("DP");}        	 		 	

	    if(!memcmp(tag,"011",3))
						{strncpy(StructInfoCaisse[0].STAN,&buffer[l_length_msg+6],l_length);  						 
	          	 	Os__xprint(StructInfoCaisse[0].STAN);
	          	 	Os__xprint("STAN");}  
	          	  
	   if(!memcmp(tag,"012",3))
					{strncpy(StructInfoCaisse[0].DE,&buffer[l_length_msg+6],l_length);
					 strncpy(StructInfoCaisse[1].DE,&buffer[l_length_msg+6],l_length);
					 Os__xprint(StructInfoCaisse[0].DE);
				   	Os__xprint("DE");	}         	 	

	   if(!memcmp(tag,"013",3))
					{strncpy(StructInfoCaisse[0].CRPIN,&buffer[l_length_msg+6],l_length); 
					 Os__xprint(StructInfoCaisse[0].CRPIN);
				   	Os__xprint("CRPIN");	}

	
	   if(!memcmp(tag,"014",3))
						{strncpy(StructInfoCaisse[0].DA,&buffer[l_length_msg+6],l_length);
                         strncpy(StructInfoCaisse[1].DA,&buffer[l_length_msg+6],l_length);						
	           	Os__xprint(StructInfoCaisse[0].DA);
	           	Os__xprint("DA");}
	
	 if(!memcmp(tag,"015",3))
						{strncpy(StructInfoCaisse[0].HE,&buffer[l_length_msg+6],l_length);
						 strncpy(StructInfoCaisse[1].HE,&buffer[l_length_msg+6],l_length);
						 Os__xprint(StructInfoCaisse[0].HE);
						 Os__xprint("HE");}		 
						 
			 if(!memcmp(tag,"016",3))
						{strncpy(StructInfoCaisse[0].NPRT,&buffer[l_length_msg+6],l_length);
						 Os__xprint(StructInfoCaisse[0].NPRT);
						 Os__xprint("NPRT");}		 
						 				 
						 
		if(!memcmp(tag,"017",3))
						{strncpy(StructInfoCaisse[0].DAEX,&buffer[l_length_msg+6],l_length);  						 
	          	 	Os__xprint(StructInfoCaisse[0].DAEX);
	          	 	Os__xprint("DAEX");}  				 
		
		if(!memcmp(tag,"018",3))
						{strncpy(StructInfoCaisse[0].DATR,&buffer[l_length_msg+6],l_length);  						 
	          	 	Os__xprint(StructInfoCaisse[0].DATR);
	          	 	Os__xprint("DATR");}  						 

		if(!memcmp(tag,"019",3))
						{strncpy(StructInfoCaisse[0].HETR,&buffer[l_length_msg+6],l_length);  						 
	          	 	Os__xprint(StructInfoCaisse[0].HETR);
	          	 	Os__xprint("HETR");}  
	          	 	
	   if(!memcmp(tag,"020",3))
						{
						
						strncpy(StructInfoCaisse[0].TIDE,&buffer[l_length_msg+6],l_length);
						strncpy(StructInfoCaisse[1].TIDE,&buffer[l_length_msg+6],l_length);
                       						 
	          	 	}  						 
	
		if(!memcmp(tag,"021",3))
						{strncpy(StructInfoCaisse[0].TYPA,&buffer[l_length_msg+6],l_length);  						 
	          	 	Os__xprint(StructInfoCaisse[0].TYPA);
	          	 	Os__xprint("TYPA");}  						 
						 
	  if(!memcmp(tag,"022",3))
						{strncpy(StructInfoCaisse[0].RE,&buffer[l_length_msg+6],l_length);

					}
	         l_length_msg=l_length_msg+l_length+6; 	
				i++;	
  }					
		
			Os__xprint("fin");
	return 0K;
 }

L format_msg_caise2(char *buffer,int trcc)
{   	  
	short l_length_msg=0;


	
	uchar  montant[13];//M.O le 26/03/2012 problème tag 002

	memset(buffer,'\0',sizeof(buffer));
		
	//M.O le 26/03/2012 problème tag 002 		 : Debut 
	memset(montant,0x00,sizeof(montant));
    long_asc (montant, 12, &StructInfoPayment.Transaction_Amount);	
	//M.O le 26/03/2012 problème tag 002 		 : Fin 
	
    if(strlen(StructInfoCaisse[1].TM) > 0){ 
		strncpy(&buffer[l_length_msg],"001",3);
		sprintf (&buffer[l_length_msg+3],"%.3d\n",(int)strlen(StructInfoCaisse[1].TM));
		//strcpy(&buffer[l_length_msg+6],StructInfoCaisse[1].TM,strlen(StructInfoCaisse[1].TM));
		strcpy(&buffer[l_length_msg+6],StructInfoCaisse[1].TM);
		l_length_msg=l_length_msg+strlen(StructInfoCaisse[1].TM)+6;
	}
	if(strlen(StructInfoCaisse[1].NCAI) > 0){ 
		strncpy(&buffer[l_length_msg],"003",3);
		sprintf (&buffer[l_length_msg+3],"%.3d\n",(int)strlen(StructInfoCaisse[1].NCAI));
		//strncpy(&buffer[l_length_msg+6],StructInfoCaisse[1].NCAI,strlen(StructInfoCaisse[1].NCAI));
		strcpy(&buffer[l_length_msg+6],StructInfoCaisse[1].NCAI);
		l_length_msg=l_length_msg+strlen(StructInfoCaisse[1].NCAI)+6;
			
	}
	if(strlen(StructInfoCaisse[1].NS) > 0){ 
		strncpy(&buffer[l_length_msg],"004",3);
		sprintf (&buffer[l_length_msg+3],"%.3d\n",(int)strlen(StructInfoCaisse[1].NS));
		strncpy(&buffer[l_length_msg+6],StructInfoCaisse[1].NS,strlen(StructInfoCaisse[1].NS));
		l_length_msg=l_length_msg+strlen(StructInfoCaisse[1].NS)+6;
	}
	if(strlen(StructInfoCaisse[1].TIDE) > 0){ 
		strncpy(&buffer[l_length_msg],"020",3);
		sprintf (&buffer[l_length_msg+3],"%.3d\n",(int)strlen(StructInfoCaisse[1].TIDE));
		strncpy(&buffer[l_length_msg+6],StructInfoCaisse[1].TIDE,strlen(StructInfoCaisse[1].TIDE));
		l_length_msg=l_length_msg+strlen(StructInfoCaisse[1].TIDE)+6;
	}
	
	
	//l_length_msg=l_length_msg+1;
	StructInfoCaisse[1].CRPIN[3] =0;
    if(strlen(StructInfoCaisse[1].CRPIN) > 0){ 
		strncpy(&buffer[l_length_msg],"013",3);
		sprintf (&buffer[l_length_msg+3],"%03d",(int)strlen(StructInfoCaisse[1].CRPIN));
		strncpy(&buffer[l_length_msg+6],StructInfoCaisse[1].CRPIN,strlen(StructInfoCaisse[1].CRPIN));
		l_length_msg=l_length_msg+strlen(StructInfoCaisse[1].CRPIN)+6;
	}
	if(strlen(StructInfoCaisse[1].DA) > 0){ 			 						 			
		strncpy(&buffer[l_length_msg],"014",3);
		sprintf (&buffer[l_length_msg+3],"%.3d\n",(int)strlen(StructInfoCaisse[1].DA));
		strncpy(&buffer[l_length_msg+6],StructInfoCaisse[1].DA,strlen(StructInfoCaisse[1].DA));
		l_length_msg=l_length_msg+strlen(StructInfoCaisse[1].DA)+6;
	}
	if(strlen(StructInfoCaisse[1].HE) > 0){ 			 			
		strncpy(&buffer[l_length_msg],"015",3);
		sprintf (&buffer[l_length_msg+3],"%.3d\n",(int)strlen(StructInfoCaisse[1].HE));
		strncpy(&buffer[l_length_msg+6],StructInfoCaisse[1].HE,strlen(StructInfoCaisse[1].HE));
		l_length_msg=l_length_msg+strlen(StructInfoCaisse[1].HE)+6;
    }
	//M.O le 26/03/2012 problème tag 002 : Debut 
	//if(strlen(StructInfoCaisse[1].MT) > 0){ 
	if(
		   memcmp(StructInfoCaisse[1].TM,"101",3)==0 
		|| memcmp(StructInfoCaisse[1].TM,"102",3)==0 
		|| memcmp(StructInfoCaisse[1].TM,"103",3)==0 
		|| memcmp(StructInfoCaisse[1].TM,"104",3)==0
	)
	{
	    strncpy(&buffer[l_length_msg],"002",3);
		//sprintf (&buffer[l_length_msg+3],"%.3d\n",(int)strlen(StructInfoCaisse[1].MT)); M.O le 26/03/2012
		sprintf (&buffer[l_length_msg+3],"%.3d\n",(int)strlen((PC)montant));
		//strncpy(&buffer[l_length_msg+6],StructInfoCaisse[1].MT,strlen(StructInfoCaisse[1].MT)); M.O le 26/03/2012
		strncpy((PC)&buffer[l_length_msg+6],(PC)montant,strlen((PC)montant));
				
		if (atoi((PC)montant)==0)
		    strncpy(&buffer[l_length_msg+6],StructInfoCaisse[0].MT,strlen(StructInfoCaisse[0].MT));
			l_length_msg=l_length_msg+strlen((PC)montant)+6;

	}
	//M.O le 26/03/2012 problème tag 002 : Fin 
	
	if(strlen(StructInfoCaisse[1].DE) > 0){ 
	    
		strncpy(&buffer[l_length_msg],"012",3);
		sprintf (&buffer[l_length_msg+3],"%.3d\n",3);
		strncpy(&buffer[l_length_msg+6],StructInfoCaisse[1].DE,3);
		l_length_msg=l_length_msg+3+6;
	}
	
	if(strlen(StructInfoCaisse[1].STAN) > 0){ 
		
		strncpy(&buffer[l_length_msg],"008",3);
		sprintf (&buffer[l_length_msg+3],"%.3d\n",(int)strlen(StructInfoCaisse[1].STAN));
		strncpy(&buffer[l_length_msg+6],StructInfoCaisse[1].STAN,strlen(StructInfoCaisse[1].STAN));
		l_length_msg=l_length_msg+strlen(StructInfoCaisse[1].STAN)+6;
	}
	
	if(strlen(StructInfoCaisse[1].NCAR) > 0){ 
		
		strncpy(&buffer[l_length_msg],"007",3);
		sprintf (&buffer[l_length_msg+3],"%.3d\n",(int)strlen(StructInfoCaisse[1].NCAR));
		strncpy(&buffer[l_length_msg+6],StructInfoCaisse[1].NCAR,strlen(StructInfoCaisse[1].NCAR));
		l_length_msg=l_length_msg+strlen(StructInfoCaisse[1].NCAR)+6;
	}

	if(strlen(StructInfoCaisse[1].DAEX) > 0){ 
		strncpy(&buffer[l_length_msg],"017",3);
		sprintf (&buffer[l_length_msg+3],"%.3d\n",(int)strlen(StructInfoCaisse[1].DAEX));
		strncpy(&buffer[l_length_msg+6],StructInfoCaisse[1].DAEX,strlen(StructInfoCaisse[1].DAEX));
		l_length_msg=l_length_msg+strlen(StructInfoCaisse[1].DAEX)+6;
	}
	
	if(strlen(StructInfoCaisse[1].DATR) > 0){ 
		
		strncpy(&buffer[l_length_msg],"018",3);
		sprintf (&buffer[l_length_msg+3],"%.3d\n",(int)strlen(StructInfoCaisse[1].DATR));
		strncpy(&buffer[l_length_msg+6],StructInfoCaisse[1].DATR,strlen(StructInfoCaisse[1].DATR));
		l_length_msg=l_length_msg+strlen(StructInfoCaisse[1].DATR)+6;
		
	}
						
	if(strlen(StructInfoCaisse[1].HETR) > 0){ 
		strncpy(&buffer[l_length_msg],"019",3);
		sprintf (&buffer[l_length_msg+3],"%.3d\n",(int)strlen(StructInfoCaisse[1].HETR));
		strncpy(&buffer[l_length_msg+6],StructInfoCaisse[1].HETR,strlen(StructInfoCaisse[1].HETR));
		l_length_msg=l_length_msg+strlen(StructInfoCaisse[1].HETR)+6;
	}

	if(strlen(StructInfoCaisse[1].NPRT) > 0){ 
		strncpy(&buffer[l_length_msg],"016",3);
		sprintf (&buffer[l_length_msg+3],"%.3d\n",(int)strlen(StructInfoCaisse[1].NPRT));
		strncpy(&buffer[l_length_msg+6],StructInfoCaisse[1].NPRT,strlen(StructInfoCaisse[1].NPRT));
		l_length_msg=l_length_msg+strlen(StructInfoCaisse[1].NPRT)+6;
	}
	else {
		strncpy(&buffer[l_length_msg],"016",3);
		sprintf (&buffer[l_length_msg+3],"%.3d\n",4);
		strncpy(&buffer[l_length_msg+6],"NONE",4);
		l_length_msg=l_length_msg+4+6;
	}

	if(strlen(StructInfoCaisse[1].MDLC) > 0){
		strncpy(&buffer[l_length_msg],"040",3);
		sprintf (&buffer[l_length_msg+3],"%.3d\n",(int)strlen(StructInfoCaisse[1].MDLC));
		strncpy(&buffer[l_length_msg+6],StructInfoCaisse[1].MDLC,strlen(StructInfoCaisse[1].MDLC));
		l_length_msg=l_length_msg+strlen(StructInfoCaisse[1].MDLC)+6;
	}

	if(strlen(StructInfoCaisse[1].DP) > 0 && strncmp(StructInfoCaisse[1].CRPIN,"000",3) == 0 ){ 
							
	strncpy(&buffer[l_length_msg],"010",3);
	// M.O le 09/03/2012 , correction RDS ACS ; ajouter la condition dans le cas ou la taille de ticket est > 998
	
	if (strlen(StructInfoCaisse[1].DP)>998)
			strcpy(&buffer[l_length_msg+3],"999");
	else		
	    sprintf (&buffer[l_length_msg+3],"%.3d\n",(int)strlen(StructInfoCaisse[1].DP));
	
	strncpy(&buffer[l_length_msg+6],StructInfoCaisse[1].DP,strlen(StructInfoCaisse[1].DP));
	l_length_msg=l_length_msg+strlen(StructInfoCaisse[1].DP)+6;
	buffer[l_length_msg-1]='?';
	}	
	return 0K;
}

L format_msg_caise(char *buffer)
{   	  
	char tag[4];
  char c_length[4]; 
  short l_length;
  short l_length_msg;
  char buff[4]; 
  //TM
  memset(buff,'\0',4);
	memset(StructInfoCaisse[0].TM,'\0',4);
	memset(c_length,0,4);
  strncpy(tag,&buffer[0],3);
  Os__xprint("TM");
  Os__xprint(buffer);
    Os__xprint(tag);
  strncpy(c_length,&buffer[3],3);
  Os__xprinta(c_length);
   
  l_length=atol(c_length);
  
  Os__xprint("ssdf");
  					strncpy(buff,&buffer[6],3);
  					Os__xprint("buff1");
            Os__xprint(buff);
            Os__xprint("buff2");
            
		if(memcmp(tag,"001",3))
					strncpy(StructInfoCaisse[0].TM,&buffer[6],3);
					  Os__xprint("STM");
					 Os__xprint(StructInfoCaisse[0].TM);
					l_length_msg=l_length_msg+l_length;	 						 
	 //NCAI					 						 
	   Os__xprint("NCAI");
	strncpy(tag,&buffer[l_length_msg+6],3);
  strncpy(c_length,&buffer[l_length_msg+9],3);
  l_length=atol(c_length);
  
   
		if(memcmp(tag,"003",3))
					strncpy(StructInfoCaisse[0].NCAI,&buffer[l_length_msg+12],l_length)		;
						l_length_msg=l_length_msg+l_length;		 				
								 				
		 //NS						 				 
	strncpy(tag,&buffer[l_length_msg+12],3);
  strncpy(c_length,&buffer[l_length_msg+15],3);
  l_length=atol(c_length); 
  
		if(memcmp(tag,"004",3))
					strncpy(StructInfoCaisse[0].NS,&buffer[l_length+18],l_length)		;					 						 
	         l_length_msg=l_length_msg+l_length; 
	
	 //DA
		strncpy(tag,&buffer[l_length_msg+18],3);
  strncpy(c_length,&buffer[l_length_msg+21],3);
  l_length=atol(c_length); 
  
		if(memcmp(tag,"014",3))
					strncpy(StructInfoCaisse[0].NCAI,&buffer[l_length+24],l_length)	;						 						 
	         l_length_msg=l_length_msg+l_length; 
	         
	    //HE      
 strncpy(tag,&buffer[l_length_msg+24],3);
  strncpy(c_length,&buffer[l_length_msg+27],3);
  l_length=atol(c_length); 
  
		if(memcmp(tag,"015",3))
					strncpy(StructInfoCaisse[0].HE,&buffer[l_length+30],l_length)			;				 						 
	         l_length_msg=l_length_msg+l_length; 

	         return 0K;
	
}

int	Wait_Amount(void)
{
	//APEMV_UI_MessageDisplayDelay ("WAITING FOR AMOUNT\nCOM0", 1); 		//Test17

//	Telium_Reset_buf(hKbd, _receive_id);                            // Reset keyboard FIFO
//	Telium_Ttestall(KEYBOARD, 30*100);                              // Wait until key pressed or timeout
//	Telium_Reset_buf(hKbd, _receive_id);                            // Reset keyboard FIFO to clear key pressed


	char buf[128];
	int ln;
	
	receive_caisse((unsigned char*)buf, ln,1);
	deco_msg_caise2((char *)buf,1);
	
	//APEMV_UI_MessageDisplayDelay (buf, 1); //Test17
	send_caisse((unsigned char*)"abcedefghijklmno", 16, 1);
	
	
	Wait_Amount();
	return 0;
}
