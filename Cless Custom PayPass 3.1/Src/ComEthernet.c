//****************************************************************************
//       INGENICO                                INGEDEV 7
//============================================================================
//       FILE  COMETHERNET.C                       (Copyright INGENICO 2008)
//============================================================================
//  Created :       12-May-2008     Kassovic
//  Last modified : 12-May-2008     Kassovic
//  Module : TRAINING
//
//  Purpose :
//  Ethernet communication management:
//             Transmission by ethernet interface ETH
//             Test done with Hyper-terminal Tcp/Ip Winsock
//  !!! Link Layer application must be loaded 3628xxxx.SGN inside the terminal
//      Supports PPP, TCP/IP and SSL protocols. Submitted under licence.
//  !!! Link Layer provided under SDK package 3429xxxx.SGN does not support
//      TCP/IP excepted under Wifi and Gprs (So this demo will not work).
//      AVL.lib + LinkLayerExeInterface.lib must be implemented to link properly
//
//  List of routines in file :
//      GetHandle : Handle of the configuration.
//      OpenEthernet : Create the ethernet configuration.
//      SetEthernet : Configure the ethernet layer.
//      ConnectEthernet : Connect the ethernet layer.
//      SendEthernet : Send data through the ethernet layer.
//      ReceiveEthernet : Receive data through the ethernet layer.
//      DisconnectEthernet : Disconnect the ethernet layer.
//      CloseEthernet : Delete the ethernet configuration.
//      PromptEthernet : Prompt for ethernet's paramaters.
//      ComEthernet : Ethernet communication demo.
//
//  File history :
//  120508-BK : File created
//
//****************************************************************************

//****************************************************************************
//      INCLUDES
//****************************************************************************
#include "SDK_Naps.h" // IngeDev automatic refactoring - 2014/04/29 14:29:58 - Replace '#include "SDK30.H"' to '#include "sdk.h"'

#include "Training.h"

#include "TlvTree.h"
#include "LinkLayer.h"

#include "Goal.h"


//Kamalssl comEthernet Debut
#include <SSL_.h>
char g_szProfile[40] ="STUNNEL1";
extern short FLAG_Ref_SSL;
SSL_HANDLE sslHdl;
SSL_PROFILE_HANDLE ProfileHnd;
short FlagSSLPRo=0;


//****************************************************************************
//      EXTERN
//****************************************************************************
extern Telium_File_t *hKbd;      // Handle of the peripheral Keyboard

//****************************************************************************
//      PRIVATE CONSTANTS
//****************************************************************************
#define MAX_SND  2048	//KT	512
#define MAX_RSP  2048	//KT	512

#define TCPIP_TIMEOUT 30*100


const char *parseStr(char ucToken, char *pcDst, const char *pcSrc, int iDim);
void DisplayMedium (byte ucLine, byte ucColumn, char *pcText);

// Properties of image (Goal)
// =======================================
static const ST_IMAGE ce_xImage480 =
{
	{GL_COLOR_WHITE, TRUE, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_MEDIUM}, GL_DIRECTION_NONE, {0, 0, 0, 0, GL_COLOR_RED}, {70, 70, 70, 70} },
	{TRUE, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_MEDIUM}, GL_DIRECTION_NONE, {1, 1, 1, 1, GL_COLOR_BLACK}, {2, 1, 2, 1} }
};

static const ST_IMAGE ce_xImage240 =
{
	{GL_COLOR_BLACK, TRUE, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_MEDIUM}, GL_DIRECTION_NONE, {0, 0, 0, 0, GL_COLOR_RED}, {20, 20, 20, 20} },
	{TRUE, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_MEDIUM}, GL_DIRECTION_NONE, {1, 1, 1, 1, GL_COLOR_BLACK}, {2, 1, 2, 1} }
};

static const ST_IMAGE ce_xImage2_480 =
{
	{GL_COLOR_BLACK, NULL, {"LUCIDA", GL_FONT_STYLE_NORMAL, GL_SCALE_MEDIUM}, GL_DIRECTION_NONE, {0, 0, 0, 0, GL_COLOR_RED}, {100, 0, 100, 100} },
	{TRUE, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_MEDIUM}, GL_DIRECTION_NONE, {1, 1, 1, 1, GL_COLOR_BLACK}, {2, 1, 2, 1} }
};

static const ST_IMAGE ce_xImage2_240 =
{
	{GL_COLOR_BLACK, NULL, {"LUCIDA", GL_FONT_STYLE_NORMAL, GL_SCALE_MEDIUM}, GL_DIRECTION_NONE, {0, 0, 0, 0, GL_COLOR_RED}, {50, 0, 50, 50} },
	{TRUE, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_MEDIUM}, GL_DIRECTION_NONE, {1, 1, 1, 1, GL_COLOR_BLACK}, {2, 1, 2, 1} }
};

int MessageWithoutConfirmation(char* title_widget, char* image); // lkeddouch 11/02/2019

//****************************************************************************
//      PRIVATE TYPES
//****************************************************************************
	// Empty

//****************************************************************************
//      PRIVATE DATA
//****************************************************************************
	// Empty

//****************************************************************************
//      LL_HANDLE OpenEthernet (const char *pcInit, const char *pcServer)
//  This function configures the Ethernet layer.
//    - LL_Configure() : Create Link Layer configuration
//  This function has parameters.
//    pcInit (I-) : static IP address unused, DHCP done via Manager menu.
//           LocalAddress = xxx.xxx.xxx.xxx
//           NetMask      = xxx.xxx.xxx.xxx
//           Gateway      = xxx.xxx.xxx.xxx
//           The '|' is the separator
//           Ex: "192.168.1.2|255.255.255.0|192.168.1.1
//    pcServer (I-) : Tcp/Ip address and port number
//           IpAddress = xxx.xxx.xxx.xxx or url
//           PortNumber = a string (max 5 bytes)
//           The '|' is the separator
//           Ex: "192.168.1.3|2000
//  This function has return value
//    !NULL : Pointer to the handle of the session
//     NULL : Session failed
//****************************************************************************

static LL_HANDLE OpenEthernet(const char *pcInit, const char *pcServer)
{
	// Local variables
    // ***************
    char tcLocalAddr[15+1];    // Ip address xxx.xxx.xxx.xxx
    char tcNetmask[15+1];      // Mask address xxx.xxx.xxx.xxx
    char tcGateway[15+1];      // Gateway address xxx.xxx.xxx.xxx
//    doubleword uiLocalAddr=0;
//    doubleword uiNetmask=0;
//    doubleword uiGateway=0;
    TLV_TREE_NODE piConfig=NULL;
    TLV_TREE_NODE piPhysicalConfig=NULL;
    TLV_TREE_NODE piTransportConfig=NULL;
    char tcAddr[lenEthIpLocal+1];
    char tcPort[lenEthPort+1];
	doubleword uiTimeout, uiRemotePort;
	LL_HANDLE hSession = NULL;                                          // Session handle
    int iRet;

    // Configure Static Address (DHCP used instead)
    // ********************************************
    CHECK((pcInit!=NULL) && (pcServer!=NULL), lblKOConfigure);          // Configuration error

    memset(tcLocalAddr, 0, sizeof(tcLocalAddr));                        // Ip address
    pcInit = parseStr('|', tcLocalAddr, pcInit, sizeof(tcLocalAddr));
    CHECK(pcInit!=NULL, lblKOConfigure);

    memset(tcNetmask, 0, sizeof(tcNetmask));                            // Netmask address
    pcInit = parseStr('|', tcNetmask, pcInit, sizeof(tcNetmask));
    CHECK(pcInit!=NULL, lblKOConfigure);

    memset(tcGateway, 0, sizeof(tcGateway));                            // Gateway address
    pcInit = parseStr('|', tcGateway, pcInit, sizeof(tcGateway));
    CHECK(pcInit!=NULL, lblKOConfigure);

/*  // Extra functions from TCP/IP
    if (tcLocalAddr[0]) {
        uiLocalAddr = __inet_addr(tcLocalAddr);                         // Force local address
        EthernetSetOption(ETH_IFO_ADDR, &uiLocalAddr);
    }

    if (tcNetmask[0]) {
        uiNetmask = __inet_addr(tcNetmask);                             // Force mask
        EthernetSetOption(ETH_IFO_NETMASK, &uiNetmask);
    }
    if(tcGateway[0]) {
        uiGateway = __inet_addr(tcGateway);                             // Force gateway
        EthernetSetOption(ETH_IFO_GATEWAY, &uiGateway);
    }
*/

	// Create the LinkLayer configuration parameters tree
	// **************************************************

    // Create parameters tree
    // ======================
    piConfig = TlvTree_New(LL_TAG_LINK_LAYER_CONFIG);                   // LinkLayer parameters Root tag of the configuration tree
    CHECK(piConfig!=NULL, lblKOConfigure);

    // Physical layer parameters
	// =========================
	piPhysicalConfig = TlvTree_AddChild(piConfig,
		                                LL_TAG_PHYSICAL_LAYER_CONFIG,   // TAG Physical layer parameters
									    NULL,                           // VALUE (Null)
									    0);                             // LENGTH 0
	CHECK(piPhysicalConfig!=NULL, lblKOConfigure);

	// Ethernet
	// --------
    TlvTree_AddChildInteger(piPhysicalConfig,
		                    LL_PHYSICAL_T_LINK,                         // TAG
							LL_PHYSICAL_V_ETHERNET,                     // VALUE
							LL_PHYSICAL_L_LINK);                        // LENGTH 1

	// Transport and network layer parameters
	// ======================================
	piTransportConfig = TlvTree_AddChild(piConfig,
		                                 LL_TAG_TRANSPORT_LAYER_CONFIG, // TAG Transport layer parameters
										 NULL,                          // VALUE (Null)
										 0);                            // LENGTH 0
	CHECK(piTransportConfig!=NULL, lblKOConfigure);

	memset(tcAddr, 0, sizeof(tcAddr));
	pcServer = parseStr('|', tcAddr, pcServer, sizeof(tcAddr));         // Parse Tcp/Ip address
    CHECK(pcServer!=NULL, lblKOConfigure);

    memset(tcPort, 0, sizeof(tcPort));
    pcServer = parseStr('|', tcPort, pcServer, sizeof(tcPort));         // Parse Tcp/Ip port
    CHECK(pcServer!=NULL, lblKOConfigure);                              // End string

	// TCP/IP
	// ------
	TlvTree_AddChildInteger(piTransportConfig,
		                    LL_TRANSPORT_T_PROTOCOL,                    // TAG
							LL_TRANSPORT_V_TCPIP,                       // VALUE
							LL_TRANSPORT_L_PROTOCOL);                   // LENGTH 1 byte

	// Host Name
	// ---------
	TlvTree_AddChildString(piTransportConfig,
		                   LL_TCPIP_T_HOST_NAME,                        // TAG
                           tcAddr);                                     // VALUE
                                                                        // LENGTH (strlen addr)
	// Port
	// ----
	uiRemotePort = atoi (tcPort);
	TlvTree_AddChildInteger(piTransportConfig,
		                    LL_TCPIP_T_PORT,                            // TAG
							uiRemotePort,                               // VALUE (Integer)
							LL_TCPIP_L_PORT);                           // LENGTH 4 bytes

	// Connection timeout
	// ------------------
	uiTimeout = TCPIP_TIMEOUT;
    TlvTree_AddChildInteger(piTransportConfig,
                            LL_TCPIP_T_CONNECT_TIMEOUT,                 // TAG
	                        uiTimeout,                                  // Value (Integer)
                            LL_TCPIP_L_CONNECT_TIMEOUT);                // LENGTH 4 bytes

	// Link Layer configuration
	// ************************
    iRet = LL_Configure(&hSession, piConfig);                           // Initialize the handle of the session
    CHECK(iRet==LL_ERROR_OK, lblKOConfigure);

    goto lblEnd;

	// Errors treatment
    // ****************
lblKOConfigure:                                                         // Configuration failed
    hSession=NULL;
    goto lblEnd;
lblEnd:
    if (piConfig)
    	TlvTree_Release(piConfig);                                      // Release tree to avoid memory leak
	return hSession;                                                    // Return the handle of the session
}

//****************************************************************************
//                int ConnectEthernet(LL_HANDLE hSession)
//  This function connects the Ethernet layer.
//    - LL_Connect() : Connect Link Layer
//  This function has no parameters.
//    hSession (I-) : Handle of the session
//  This function has return value
//    >=0 : Connect done
//     <0 : Connect failed
//****************************************************************************

static int ConnectEthernet(LL_HANDLE hSession)
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
//    int SendEthernet (LL_HANDLE hSession, const char *pcMsg, word usLen)
//  This function sends data through the Ethernet layer.
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

static int SendEthernet(LL_HANDLE hSession, const char *pcMsg, word usLen)
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
//        int ReceiveEthernet (LL_HANDLE hSession, char *pcMsg,
//                             word usLen, byte ucDly)
//  This function receives data through the Ethernet layer.
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

static int ReceiveEthernet(LL_HANDLE hSession, char *pcMsg, word usLen, byte ucDly)
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
	//Telium_Reset_buf(hKbd, _receive_id);                                   // Reset peripherals FIFO
    iRet = TimerStart(0, lSec);                                            // Timer0 starts
    CHECK(iRet>=0, lblTimeOut);
	do
    {
    	iNbrBytes = LL_Receive(hSession, usLen, pcMsg+iLength, 1*100);     // Check reception
    	if (iNbrBytes != 0)
    		break;                                                         // Bytes received
		iRet = Telium_Ttestall(KEYBOARD, 1);                               // Check for a key pressed
		if (iRet & KEYBOARD)                                               // Key detected
		{
			cKey = Telium_Getchar();                                       // Retrieve key pressed
			CHECK(cKey!=T_ANN, lblTimeOut);                                // Exit on cancel key
		}
		if (lSec != LL_INFINITE)
			lTimeOut = TimerGet(0);                                        // Retrieve timer value
    } while (lTimeOut>0);

    CHECK(lTimeOut!=0, lblTimeOut);                                        // Exit on timeout

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
lblTimeOut:                                                                // Timeout expired
   	iRet = LL_ERROR_TIMEOUT;
   	goto lblEnd;
lblEnd:
    TimerStop(0);
    return iRet;
}

//****************************************************************************
//                int DisconnectEthernet (LL_HANDLE hSession)
//  This function disconnects the Ethernet layer.
//    - LL_Disconnect() : Disconnect Link Layer
//  This function has no parameters.
//    hSession (I-) : Handle of the session
//  This function has return value
//    >=0 : Disconnect done
//     <0 : Disconnect failed
//****************************************************************************

static int DisconnectEthernet(LL_HANDLE hSession)
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
//                 int CloseEthernet (LL_HANDLE hSession)
//  This function deletes the handle of the Ethernet layer.
//    - LL_Configure() : Delete Link Layer configuration
//  This function has no parameters.
//    hSession (I-) : Handle of the session
//  This function has return value
//    >=0 : Configuration deleted
//     <0 : Processing failed
//****************************************************************************

static int CloseEthernet(LL_HANDLE hSession)
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
//                    void PromptEthernet (void)                            
//  This function asks for the Ethernet's parameters.   
//  This function has no parameters.  
//  This function has no return value
//****************************************************************************

void PromptEthernet(void)
{
	// Local variables 
    // ***************
	char tcIpAddress[lenEthIpLocal+1];
	char tcPort[lenEthPort+1];
    int iRet;

    // Enter Local IP address
    // **********************
	do
	{
		iRet = appGet(appEthIpLocal, tcIpAddress, lenEthIpLocal+1); CHECK(iRet>=0, lblDbaErr);
		iRet = ShowEntry("ENTER LOCAL IP", "", tcIpAddress, 0, lenEthIpLocal, 4);
		if (iRet<0)
		{
			DisplaySmall(6, 4, "Entry exceeded !!!");
			buzzer(10);
			Telium_Ttestall(0, 2*100);
		}
	} while (iRet<0);
	CHECK(iRet!=0, lblEnd);                                     // Cancel, Timeout
	if (iRet==2)                                                // Save Ip address
	{
		iRet = appPut(appEthIpLocal, tcIpAddress, lenEthIpLocal); CHECK(iRet>=0, lblDbaErr);
	}

	// Enter Port number 
	// *****************
	do
	{
		iRet = appGet(appEthPort, tcPort, lenEthPort+1); CHECK(iRet>=0, lblDbaErr);
		iRet = ShowEntry("ENTER  PORT  NUMBER", "", tcPort, 0, lenEthPort, 0); //CHECK(iRet>=0, lblKO);
		if (iRet<0)
		{
			DisplaySmall(5, 4, "Entry exceeded !!!");
			buzzer(10);
			Telium_Ttestall(0, 2*100);
		}
	} while (iRet<0);
	CHECK(iRet!=0, lblEnd);                                    // Cancel, Timeout
	if (iRet==2)                                               // Save port number
	{
		iRet = appPut(appEthPort, tcPort, lenEthPort); CHECK(iRet>=0, lblDbaErr);
	}
		
	goto lblEnd;

	// Errors treatment 
	// ****************
lblDbaErr:
	_clrscr();                                                 // Clear screen
	DisplayMedium(0, 0, FMG_ErrorMsg(iRet));                   // FMG error
	DisplayMedium(1, 0, "Reset Needed");                       // Rebuild data base
	Telium_Ttestall(0, 1*100);                                 // Delay 5s
	goto lblEnd;
lblEnd:
    return;
}

//****************************************************************************
//                      void ComEthernet (void)
//  This function communicates through the Ethernet layer.
//  This function has no parameters.
//  This function has no return value
//****************************************************************************

void ComEthernet(void)
{
	// Local variables
    // ***************
    LL_HANDLE *hETH=NULL;
	char *pcStr, tcStr[128+1];
	char tcIpAddress[lenEthIpLocal+1];
	char tcPort[lenEthPort+1];
    char tcSnd[MAX_SND+1];
    char tcRsp[MAX_RSP+1];
	int iRet=0, iStatus=0;

	// Transmission through Ethernet layer in progress
	// ************************************************
    memset(tcSnd, 0, sizeof(tcSnd));
	memset(tcRsp, 0, sizeof(tcRsp));
	_clrscr();                                                      // Clear screen

    // Open Ethernet layer
	// ===================
	DisplayMedium(0, 0, "1-comOpen...");                            // Display open message
	pcStr = "DHCP";                                                 // No static addess => use DHCP
	pcStr = "10.1.15.98|255.255.255.0|10.1.15.1";
	DisplayMedium(1, 0, pcStr);                                     // Display config
	iRet = appGet(appEthIpLocal, tcIpAddress, lenEthIpLocal+1);     // Retrieve local IP
	CHECK(iRet>=0, lblDbaErr);
	DisplayMedium(2, 0, tcIpAddress);                               // Display IP Address
	iRet = appGet(appEthPort, tcPort, lenEthPort+1);                // Retrieve port number
	CHECK(iRet>=0, lblDbaErr);
    DisplayMedium(3, 0, tcPort);                                    // Display port number
    sprintf (tcStr, "%s|%s", tcIpAddress, tcPort);
    hETH = OpenEthernet(pcStr, tcStr); CHECK(hETH!=NULL, lblKO);    // ** Open **
    _clrscr();                                                      // Clear screen
    DisplayMedium(0, 0, "1-comOpen OK");                            // Display open OK

	// Connect Ethernet layer
    // ======================
	DisplayMedium(1, 0, "2-comConnect...");                         // Display connect message
	iRet = ConnectEthernet(hETH); CHECK(iRet>=0, lblComKO);         // ** Connect **
    DisplayMedium(2, 0, "2-comConnect OK");                         // Display connect OK

	// Clear sending/receiving buffers
	// ===============================
	iRet = LL_ClearSendBuffer(hETH);                                // Clear sending buffer
    CHECK(iRet==LL_ERROR_OK, lblEnd);
    iRet = LL_ClearReceiveBuffer(hETH);                             // Clear receiving buffer
    CHECK(iRet==LL_ERROR_OK, lblEnd);

    // Send data through Ethernet layer
	// ================================
	DisplayMedium(3, 0, "3-comSend...");                            // Display send message
    strcpy(tcSnd, "Hello, do you hear me?\n"                        // Data to send
		          "Hello, do you hear me?\n"
				  "Hello, do you hear me?\n"
				  "Hello, do you hear me?\n"
				  "Hello, do you hear me?\n");
    iRet = SendEthernet(hETH, tcSnd, (word) strlen(tcSnd));         // ** Send data **
	CHECK(iRet>=0, lblComKO);
	_clrscr();                                                      // Clear screen
	DisplayMedium(0, 0, "3-comSend OK");                            // Display send OK

	// Receive data through Ethernet layer
	// ===================================
	DisplayMedium(1, 0, "4-comRec...");                             // Display receive message
    iRet = ReceiveEthernet(hETH, tcRsp, sizeof(tcRsp), 30);         // ** Receive data **
	CHECK(iRet>=0, lblComKO);
    if (iRet > MAX_RSP) strcpy (tcRsp, "Buffer overflow Max=512");  // Data overflow
	DisplayMedium(2, 0, "4-comRec OK");                             // Display receive OK

	// Disconnection
	// =============
	iRet = DisconnectEthernet(hETH);                                // ** Disconnect **
	CHECK(iRet>=0, lblComKO);
	DisplayMedium(3, 0, "5-comDisconn OK");                         // Display disconnect OK
	iRet = CloseEthernet(hETH);                                     // ** Close **
	CHECK(iRet>=0, lblComKO);

	// Display transmission data
	// =========================
	_clrscr();                                                      // Clear screen
	DisplaySmall(0, 0, "RECEIVING:");                               // Display data to receive
	iRet = Display25(1, tcRsp, strlen(tcRsp)); CHECK(iRet>=0, lblKO);
    DisplaySmall(7, 6, "Press a key");

	// Print transmission data
	// =======================
	iRet = Telium_Pprintf("\x1b""E""      ETHERNET Demo\n\n"        // Print demo in progress
		           "\x1b""F"); CHECK(iRet>=0, lblKO);
    iRet = Telium_Pprintf("Sending:\n"); CHECK(iRet>=0, lblKO);     // Print data to send
	iRet = Print24(tcSnd, strlen(tcSnd)); CHECK(iRet>=0, lblKO);
    iRet = Telium_Pprintf("\nReceiving:\n"); CHECK(iRet>=0, lblKO); // Print data to receive
	iRet = Print24(tcRsp, strlen(tcRsp)); CHECK(iRet>=0, lblKO);
    iRet = Telium_Pprintf("\n\n\n\n\n\n"); CHECK(iRet>=0, lblKO);

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
	if (iRet == LL_Network_GetStatus(LL_PHYSICAL_V_ETHERNET, &iStatus))
	{
		switch(iStatus)
		{
		// Ethernet status
		case LL_STATUS_ETHERNET_NO_DEFAULT_ROUTE:   iStatus=LL_STATUS_ETH_NO_DEFAULT_ROUTE;        break;
		case LL_STATUS_ETHERNET_NOT_PLUGGED:        iStatus=LL_STATUS_ETH_NOT_PLUGGED;             break;
		case LL_STATUS_ETHERNET_BASE_NOT_READY:     iStatus=LL_STATUS_ETH_BASE_NOT_READY;          break;
		case LL_STATUS_ETHERNET_OUT_OF_BASE:        iStatus=LL_STATUS_ETH_OUT_OF_BASE;             break;
		// Bluetooth status
		case LL_BT_STATUS_NO_BASE_ASSOCIATED:       iStatus=LL_STATUS_BT_NO_BASE_ASSOCIATED;       break;
		case LL_BT_STATUS_PERIPHERAL_NOT_AVAILABLE: iStatus=LL_STATUS_BT_PERIPHERAL_NOT_AVAILABLE; break;
		case LL_BT_STATUS_PERIPHERAL_BUSY:          iStatus=LL_STATUS_BT_PERIPHERAL_BUSY;          break;
		case LL_BT_STATUS_BASE_NOT_AVAILABLE:       iStatus=LL_STATUS_BT_BASE_NOT_AVAILABLE;       break;
		default:                                    iStatus=-1;                                    break;
		}
		DisplayMedium(1, 0, LL_ErrorMsg(iStatus));                  // Link Layer status
	}
	Telium_Ttestall(0, 1*100);                                      // Delay 5s
	goto lblEnd;
lblDbaErr:                                                          // Data base error
    _clrscr();                                                      // Clear screen
    DisplayMedium(0, 0, FMG_ErrorMsg(iRet));                        // FMG error
    DisplayMedium(1, 0, "Reset Needed");                            // Rebuild data base
    Telium_Ttestall(0, 1*100);                                      // Delay 5s
    goto lblEnd;
lblEnd:
    if (hETH) {
		DisconnectEthernet(hETH);                                   // ** Disconnect **
		CloseEthernet(hETH);                                        // ** Close **
	}
}

//INTEGRATION DE TCP/IP
void Function_lblComKO(int iRet)
{
	int iStatus=0;
	

	if(iRet == LL_ERROR_INVALID_HANDLE)
		return;


	_clrscr();                                                      // Clear screen
	APEMV_UI_MessageDisplayDelay(LL_ErrorMsg(iRet),1);//DisplayMedium(0, 0, LL_ErrorMsg(iRet));                         // Link Layer error
	if (iRet == LL_Network_GetStatus(LL_PHYSICAL_V_ETHERNET, &iStatus))
	{
		switch(iStatus)
		{
		// Ethernet status
		case LL_STATUS_ETHERNET_NO_DEFAULT_ROUTE:   iStatus=LL_STATUS_ETH_NO_DEFAULT_ROUTE;        break;
		case LL_STATUS_ETHERNET_NOT_PLUGGED:        iStatus=LL_STATUS_ETH_NOT_PLUGGED;             break;
		case LL_STATUS_ETHERNET_BASE_NOT_READY:     iStatus=LL_STATUS_ETH_BASE_NOT_READY;          break;
		case LL_STATUS_ETHERNET_OUT_OF_BASE:        iStatus=LL_STATUS_ETH_OUT_OF_BASE;             break;
		// Bluetooth status
		case LL_BT_STATUS_NO_BASE_ASSOCIATED:       iStatus=LL_STATUS_BT_NO_BASE_ASSOCIATED;       break;
		case LL_BT_STATUS_PERIPHERAL_NOT_AVAILABLE: iStatus=LL_STATUS_BT_PERIPHERAL_NOT_AVAILABLE; break;
		case LL_BT_STATUS_PERIPHERAL_BUSY:          iStatus=LL_STATUS_BT_PERIPHERAL_BUSY;          break;
		case LL_BT_STATUS_BASE_NOT_AVAILABLE:       iStatus=LL_STATUS_BT_BASE_NOT_AVAILABLE;       break;
		default:                                    iStatus=-1;                                    break;
		}
		APEMV_UI_MessageDisplayDelay(LL_ErrorMsg(iRet),1);//DisplayMedium(1, 0, LL_ErrorMsg(iStatus));                  // Link Layer status
	}
	Telium_Ttestall(0, 1*100);                                      // Delay 5s
}


LL_HANDLE *hETH=NULL;		// Connected socket descriptor.

int CallPerformConnectionIP( const char *szHostName, unsigned int nPort)
{
	// Local variables 
    // ***************
	char *pcStr, tcStr[128+1];
	char tcIpAddress[lenEthIpLocal+1];
	char tcPort[lenEthPort+1];
	int iRet=0;

	_clrscr();                                                      // Clear screen

    // Open Ethernet layer
	// ===================
	//DisplayMedium(0, 0, "1-comOpen.0.");                            // Display open message
	pcStr = "DHCP";                                                 // No static addess => use DHCP
	pcStr = "10.1.15.98|255.255.255.0|10.1.15.1";
	//DisplayMedium(1, 0, pcStr);                                     // Display config
//	iRet = appGet(appEthIpLocal, tcIpAddress, lenEthIpLocal+1);     // Retrieve local IP
//	CHECK(iRet>=0, lblDbaErr);
//	sprintf(tcIpAddress,"%s",szHostName); //Remplacer17
	//DisplayMedium(2, 0, FicPara.ip_gprs);                          // Display IP Address
//	iRet = appGet(appEthPort, tcPort, lenEthPort+1);                // Retrieve port number
//	CHECK(iRet>=0, lblDbaErr);
//	sprintf(tcPort,"%d",nPort); //Remplacer17
    //DisplayMedium(3, 0, FicPara.port_gprs);                        // Display port number
    //sprintf (tcStr, "%s|%s", tcIpAddress, tcPort);
	sprintf (tcStr, "%s|%s", FicPara.ip_gprs, FicPara.port_gprs);
	hETH = OpenEthernet(pcStr, tcStr); //CHECK(hETH!=NULL, lblKO);    // ** Open **
	if(hETH == NULL)		//Remplacer17
	{
		_clrscr();                                                      // Clear screen
		DisplayMedium(0, 0, "Processing Error");                        // Oops! error
		Telium_Ttestall(0,5*100);                                       // Delay 5s		
		return KO;
	}
    _clrscr();                                                      // Clear screen
    //DisplayMedium(0, 0, "1-comOpen OK");                            // Display open OK

	// Connect Ethernet layer
    // ======================
	//DisplayMedium(1, 0, "2-comConnect...");                         // Display connect message
	iRet = ConnectEthernet(hETH); //CHECK(iRet>=0, lblComKO);         // ** Connect **
	if(iRet < 0)		//Remplacer17
	{
		Function_lblComKO(iRet);
		return KO;
	}
	
    //DisplayMedium(2, 0, "2-comConnect OK");                         // Display connect OK

	return	OK;

lblDbaErr:
	_clrscr();                                                 // Clear screen
	DisplayMedium(0, 0, FMG_ErrorMsg(iRet));                   // FMG error
	DisplayMedium(1, 0, "Reset Needed");                       // Rebuild data base
	Telium_Ttestall(0, 1*100);                                 // Delay 5s
	return KO;
}

int SendFrameIP( char *sbuffer,unsigned short count )
{
	// Local variables 
    // ***************
    char tcSnd[MAX_SND+1];
	int iRet=0;

	// Transmission through Ethernet layer in progress
	// ************************************************
    memset(tcSnd, 0, sizeof(tcSnd));
	_clrscr();                                                      // Clear screen


	// Clear sending/receiving buffers
	// ===============================
	iRet = LL_ClearSendBuffer(hETH);                                // Clear sending buffer
    //CHECK(iRet==LL_ERROR_OK, lblEnd);
    if (iRet!=LL_ERROR_OK) {
		DisconnectEthernet(hETH);                                   // ** Disconnect **
		CloseEthernet(hETH);                                        // ** Close **
	}	
    iRet = LL_ClearReceiveBuffer(hETH);                             // Clear receiving buffer
    //CHECK(iRet==LL_ERROR_OK, lblEnd);
    if (iRet!=LL_ERROR_OK) {
		DisconnectEthernet(hETH);                                   // ** Disconnect **
		CloseEthernet(hETH);                                        // ** Close **
	}

    // Send data through Ethernet layer
	// ================================
	//DisplayMedium(3, 0, "3-comSend...");                            // Display send message
    //strcpy(tcSnd, "Hello, do you hear me?\n"                        // Data to send
	//			  "Hello, do you hear me?\n");
	memcpy(tcSnd,sbuffer,count);			//Remplacer17
    //iRet = SendEthernet(hETH, tcSnd, (word) strlen(tcSnd));         // ** Send data **
	iRet = SendEthernet(hETH, tcSnd, (word) count);         // ** Send data **
	//CHECK(iRet>=0, lblComKO);
	if(iRet < 0)		//Remplacer17
	{
		Function_lblComKO(iRet);
		return KO;
	}

	_clrscr();                                                      // Clear screen
	//DisplayMedium(0, 0, "3-comSend OK");                            // Display send OK

    return	OK;
}
int RecieveFrameIP(char *rbuffer)
{
	// Local variables 
    // ***************
    char tcRsp[MAX_RSP+1];
	int iRet=0;

	// Transmission through Ethernet layer in progress
	// ************************************************
 	memset(tcRsp, 0, sizeof(tcRsp));
	_clrscr();                                                      // Clear screen


	// Clear sending/receiving buffers
	// ===============================
	iRet = LL_ClearSendBuffer(hETH);                                // Clear sending buffer
    //CHECK(iRet==LL_ERROR_OK, lblEnd);
    if (iRet!=LL_ERROR_OK) {
		DisconnectEthernet(hETH);                                   // ** Disconnect **
		CloseEthernet(hETH);                                        // ** Close **
	}	
    iRet = LL_ClearReceiveBuffer(hETH);                             // Clear receiving buffer
    //CHECK(iRet==LL_ERROR_OK, lblEnd);
    if (iRet!=LL_ERROR_OK) {
		DisconnectEthernet(hETH);                                   // ** Disconnect **
		CloseEthernet(hETH);                                        // ** Close **
	}

	// Receive data through Ethernet layer
	// ===================================
	//DisplayMedium(1, 0, "4-comRec...");                             // Display receive message
    iRet = ReceiveEthernet(hETH, tcRsp, sizeof(tcRsp), 30);         // ** Receive data **
	//CHECK(iRet>=0, lblComKO);
	if(iRet < 0)		//Remplacer17
	{
		Function_lblComKO(iRet);
		return KO;
	}

    if (iRet > MAX_RSP) strcpy (tcRsp, "Buffer overflow Max=512");  // Data overflow
	//DisplayMedium(2, 0, "4-comRec OK");                             // Display receive OK
	memcpy(rbuffer,tcRsp,iRet);					//Remplacer17
	return iRet;

}
void DeconnectionIP(void)
{
	// Local variables
    // ***************
	int iRet=0;

	if(FicPara.Etat_TLS == 1)
	{
		FLAG_Ref_SSL=-1;

		/* realease the SSL socket */
		if(sslHdl)
		{
			SSL_Disconnect(sslHdl);
			SSL_Free( sslHdl );
			//sslHdl = NULL;
		}
		//APEMV_UI_MessageDisplayDelay("DisconnectSSL.2",1);
	}
	else
	{
		// Disconnection
		// =============
		iRet = DisconnectEthernet(hETH);                                // ** Disconnect **
		//CHECK(iRet>=0, lblComKO);
		if(iRet < 0)		//Remplacer17
		{
			Function_lblComKO(iRet);
		}
		else{
			DisplayMedium(3, 0, "5-comDisconn OK");                         // Display disconnect OK
			iRet = CloseEthernet(hETH);                                     // ** Close **
			//CHECK(iRet>=0, lblComKO);
			if(iRet < 0)		//Remplacer17
			{
				Function_lblComKO(iRet);
			}
		}
	}
}

//Kamalssl comEthernet Fin
int CallPerformConnectionIP_TLS( const char *szHostName, unsigned int nPort)
{
	char	tmp[32];
	int		ret;
	int 	nError;
	T_GL_HWIDGET hPicture=NULL;

	MessageWithoutConfirmation("Connexion en cours", "file://flash/HOST/attendez_svp.png");
//	hPicture = GoalCreatePicture(APEMV_UI_GoalHandle());                             // Create and clear screen
//	GoalDspPicture(hPicture, "Connexion en cours", "file://flash/HOST/attendez_svp.png", &ce_xImage480, (GL_TIME_SECOND) * 0, true);
//	GoalDestroyPicture(&hPicture);

//	if(sslHdl != NULL)
//	{
//		SSL_Disconnect(sslHdl);
//		SSL_Free( sslHdl );
//		//sslHdl = NULL;
//		APEMV_UI_MessageDisplayDelay ("\n\nDisconnecting....1", 2);
//	}
//	else
//	{
//		SSL_Disconnect(sslHdl);
//		SSL_Free( sslHdl );
//		//sslHdl = NULL;
//		APEMV_UI_MessageDisplayDelay ("\n\nDisconnecting....2", 2);
//	}

	if(FlagSSLPRo==0)
	{
		ProfileHnd=SSL_LoadProfile(g_szProfile);
		FlagSSLPRo=1;
	}
    /* Tests whether the files linked to the profile are loaded into the terminal.*/
    {
    	int err;
    	//	SSL_HANDLE sslHdl;
        err = SSL_New(&sslHdl, ProfileHnd);

//		memset(tmp, 0, 32);
//		sprintf(tmp, "\nSSL_New=%d\n", err);
//		APEMV_UI_MessageDisplayDelay (tmp, 2);

		if(err != 0)
     	{
     	    //	SSL_UnloadProfile( ProfileHnd );
//        	APEMV_UI_MessageDisplayDelay ("\n\nSSL ERROR", 2);
     	    return KO;
     	}
//        APEMV_UI_MessageDisplayDelay ("\n\nSSL new OK", 2);

        /* connect to the created SSL socket */

//        sprintf(FicPara.ip_gprs,"%s","105.159.251.120");
//        sprintf(FicPara.port_gprs,"%s","8012");

//		APEMV_UI_MessageDisplayDelay (FicPara.ip_gprs, 1);
//		APEMV_UI_MessageDisplayDelay (FicPara.port_gprs, 1);

        ret=SSL_Connect( sslHdl,FicPara.ip_gprs, atoi(FicPara.port_gprs), 2000 );

        SSL_ProfileGetLastError(ProfileHnd,&nError);

        if(nError==18 || nError==SSL_PROFILE_E_SSLV3_ALERT_CERTIFICATE_EXPIRED || nError==SSL_PROFILE_E_SSLV3_ALERT_CERTIFICATE_UNKNOWN) {

				//Os__xprintaa("ici 2");

		 if(FS_exist("HOST/CACERT.PEM") && FS_exist("HOST/CACERT_RES.PEM"))

		 {
				//Os__xprintaa("ici 3");

							  FS_unlink("HOST/CACERT.PEM");

							  FS_rename("HOST/CACERT_RES.PEM", "HOST/CACERT.PEM");

		 }

		 }

//		memset(tmp,0,32);
//		sprintf(tmp,"\nSSL_Connect=%d\n",ret);

//		APEMV_UI_MessageDisplayDelay (tmp, 2);



     	if( ret == 0 )
     	{
//     		APEMV_UI_MessageDisplayDelay ("\n\nSSL Connect", 2);
     		//	hndssl=sslHdl;
     		return OK;
     	}
     	else
     	{
			memset(tmp, 0, 32);
			sprintf(tmp, "\nSSL_Connect=%d\n", ret);
			APEMV_UI_MessageDisplayDelay (tmp, 2);
			//     		APEMV_UI_MessageDisplayDelay ("\n\nSSL Co KO", 2);

			SSL_Disconnect(sslHdl);
     	    SSL_Free( sslHdl );
     	    //sslHdl = NULL;
     	    //	SSL_UnloadProfile( ProfileHnd );
     	    return KO;
     	}
    }
}

int SendFrameSSL(char *sbuffer,unsigned short count)
{

    char tcSnd[MAX_SND+1];
	int iRet=0,ret=-1;
	int 	nError;


	if(FlagSSLPRo==0)
	{
		ProfileHnd=SSL_LoadProfile(g_szProfile);
		FlagSSLPRo=1;
	}

      /* Tests whether the files linked to the profile are loaded into the terminal.*/
		if(FLAG_Ref_SSL<0)
		{
            int err;

            err = SSL_New(&sslHdl, ProfileHnd);


            if(err != 0)
            {

    			////APEMV_UI_MessageDisplayDelay ("\nSendFrameSSL\nSSL ERROR", 2);
            	//	SSL_UnloadProfile( ProfileHnd );
                return KO;
            }
            ////APEMV_UI_MessageDisplayDelay ("\n\nSSL new OK", 2);
			/* connect to the created SSL socket */

            T_GL_HWIDGET hPicture=NULL;
            MessageWithoutConfirmation("Connexion en cours", "file://flash/HOST/attendez_svp.png");

//        	hPicture = GoalCreatePicture(APEMV_UI_GoalHandle());                             // Create and clear screen
//        	GoalDspPicture(hPicture, "Connexion en cours..", "file://flash/HOST/attendez_svp.png", &ce_xImage480, (GL_TIME_SECOND) * 0, true);
//        	GoalDestroyPicture(&hPicture);


            ret=SSL_Connect( sslHdl,FicPara.ip_gprs, atoi(FicPara.port_gprs), 2000 );

            SSL_ProfileGetLastError(ProfileHnd,&nError);

            if(nError==18 || nError==SSL_PROFILE_E_SSLV3_ALERT_CERTIFICATE_EXPIRED || nError==SSL_PROFILE_E_SSLV3_ALERT_CERTIFICATE_UNKNOWN) {

    				//Os__xprintaa("ici 2");

    		 if(FS_exist("HOST/CACERT.PEM") && FS_exist("HOST/CACERT_RES.PEM"))

    		 {
    				//Os__xprintaa("ici 3");

    							  FS_unlink("HOST/CACERT.PEM");

    							  FS_rename("HOST/CACERT_RES.PEM", "HOST/CACERT.PEM");

    		 }

    		 }



        }
        else
        {
            ret=0;
            ////APEMV_UI_MessageDisplayDelay ("\nSendFrameSSL\nret=0", 2);
        }

		if( ret == 0 )
		{
			////APEMV_UI_MessageDisplayDelay ("\nSendFrameSSL\nSSL Connect", 2);
			memset(tcSnd, 0, sizeof(tcSnd));
			memcpy(tcSnd,sbuffer,count);			//Remplacer17

			iRet= SSL_Write(sslHdl,tcSnd,count,300);
			if(iRet<=0)
			{
				////APEMV_UI_MessageDisplayDelay ("\nSendFrameSSL\nwrite err", 2);
				if(sslHdl)
				{
					SSL_Disconnect(sslHdl);				/* realease the SSL socket */
					SSL_Free( sslHdl );
					//sslHdl = NULL;
				}
				//APEMV_UI_MessageDisplayDelay("DisconnectSSL.3",1);
				//	SSL_UnloadProfile( ProfileHnd );
				////APEMV_UI_MessageDisplayDelay ("\nSendFrameSSL\nafter free", 2);
				return KO;
			}
		}
		else
		{
			////APEMV_UI_MessageDisplayDelay ("\nSendFrameSSL\nSSL Co KO", 2);

			SSL_Disconnect(sslHdl);
			SSL_Free( sslHdl );
			//sslHdl = NULL;

			//	SSL_UnloadProfile( ProfileHnd );
			////APEMV_UI_MessageDisplayDelay ("\nSendFrameSSL\nafter free.2", 2);
			//	SSL_UnloadProfile( ProfileHnd );
			return KO;
		}
		/* Unload the profile to load it after */
		//	SSL_UnloadProfile( hProfile );
	   return OK;
}


int RecieveFrameSSL(char *rbuffer)
{

    char tcRsp[MAX_RSP+1];
    char tmp[10]="\0";
	int ret=0;

	memset(tcRsp, 0, sizeof(tcRsp));

	ret = SSL_Read(sslHdl, tcRsp, sizeof(tcRsp), 6000);
	//ret = SSL_Read(sslHdl, tcRsp, sizeof(tcRsp), FicPara.TimeOutReceive);
//	sprintf(tmp,"%d",ret);
//	sTrace("trace");
//	sTrace(tmp);

	if(ret < 0)		//Remplacer17
	{
//		sTrace("Reception KO");
		Function_lblComKO(ret);

		/* realease the SSL socket */
		if(sslHdl)
		{
			SSL_Disconnect(sslHdl);
			SSL_Free( sslHdl );
			//sslHdl = NULL;

		}
		//APEMV_UI_MessageDisplayDelay("DisconnectSSL.4",1);
		/* unload the SSL profile */
		//	SSL_UnloadProfile(ProfileHnd);

		return KO;
	}

    if (ret > MAX_RSP)
    	strcpy (tcRsp, "Buffer overflow Max=512");  // Data overflow

	//DisplayMedium(2, 0, "4-comRec OK");                             // Display receive OK
	memcpy(rbuffer,tcRsp,ret);					//Remplacer17

	if(FLAG_Ref_SSL==2)
	{
		/* realease the SSL socket */
		if(sslHdl)
		{
			SSL_Disconnect(sslHdl);
			SSL_Free( sslHdl );
			//sslHdl = NULL;
		}
		//APEMV_UI_MessageDisplayDelay("DisconnectSSL.5",1);
	}

	return ret;
}

int AddProfileSLL(void)
{
	SSL_PROFILE_HANDLE hProfile;
	char szName[80],tmp[32];
	int pnError=1;


//	APEMV_UI_MessageDisplayDelay ("Profile SSL", 2);


	//memcpy( szName, g_szProfile,strlen(g_szProfile));
	sprintf(szName,"STUNNEL1");
	SSL_DeleteProfile(szName);

	hProfile = SSL_NewProfile( szName, &pnError );
	//sprintf(tmp,"\nNew=%d\n",pnError);

	//APEMV_UI_MessageDisplayDelay (tmp, 2);

	if( hProfile != NULL )
	{
//		APEMV_UI_MessageDisplayDelay ("Profile OK 1", 2);

		sprintf(tmp+strlen(tmp),"SSL\n");
		/* set SSLV3 protocol for the STUNNEL profile */
		SSL_ProfileSetProtocol( hProfile, TLSv1_2 );
		/* set Cipher protocol for the STUNNEL profile */
		//		SSL_ProfileSetCipher( hProfile,
		//							  SSL_RSA | SSL_DES| SSL_3DES | SSL_RC4 | SSL_RC2 |SSL_MD5| SSL_SHA1, SSL_HIGH | SSL_NOT_EXP );
		SSL_ProfileSetCipher( hProfile,SSL_RSA | SSL_DES| SSL_3DES| SSL_AES| SSL_SHA1| SSL_SHA384| SSL_aECDSA| SSL_RC4 | SSL_RC2| SSL_MD5,  SSL_HIGH| SSL_NOT_EXP );


//		?	client.crt (Client Certificat) : cacert.pem charger au niveau de TPE.
//		?	client_key.pem (Client RSA Key) : key.pem RSA PRIVATE KEY
//		?	ca.crt (CA Certificats) : stunnel.pem (Client Certificat + Client RSA Key) charger au niveau de serveur.

				pnError = SSL_ProfileSetCertificateFile( hProfile, "/HOST/CACERT.PEM" );

//				memset(tmp, 0, 32);
//				sprintf(tmp, "\nOther0=%d\n", pnError);
//				APEMV_UI_MessageDisplayDelay (tmp, 2);

				pnError = SSL_ProfileAddCertificateCA( hProfile, "/HOST/CACERT.PEM" );


//				memset(tmp, 0, 32);
//				sprintf(tmp, "\nOther1=%d\n", pnError);
//				APEMV_UI_MessageDisplayDelay (tmp, 2);

//				pnError = SSL_ProfileTestFile(hProfile);
//				memset(tmp, 0, 32);
//				sprintf(tmp, "\nOther2=%d\n", pnError);
//				APEMV_UI_MessageDisplayDelay (tmp, 2);


		/* Use client private key, client certificate, root certificate, */
//				SSL_ProfileSetKeyFile( hProfile, "/SYSTEM/rootcert.PEM", FALSE );
//				pnError=SSL_ProfileAddCertificateCA( hProfile, "/SYSTEM/rootcert.PEM" );


		// Save the profile.
		SSL_SaveProfile( hProfile );
		SSL_UnloadProfile( hProfile );



		/* Tests whether the files linked to the profile are loaded into the terminal.*/
//		{
//			int err;
//			SSL_HANDLE sslHdl;
//
//
//			hProfile = SSL_LoadProfile(szName);
//
//			err = SSL_New(&sslHdl, hProfile);
//			if(err != 0)
//			{
//				memset(tmp, 0, 32);
//				sprintf(tmp, "\nOther3=%d\n", err);
//				APEMV_UI_MessageDisplayDelay (tmp, 2);
//
////				UI_ErrMsgDisplay(ERR_FUN_SSL_CERTS_KEYS_NOT_LOADED);
////				UI_ErrMsgDisplay(ERR_FUN_SSL_PROFILE_CREATION_FAILED);
//				SSL_UnloadProfile( hProfile );
//				SSL_DeleteProfile(szName);
//			}
//			else
//				APEMV_UI_MessageDisplayDelay ("SSL_NEW OK", 2);
//
//
//			SSL_Free(sslHdl);
//		}

	   // SSL_UnloadProfile( hProfile );

	}
}
int DisconnectSSL()
{
	if(FicPara.Etat_TLS == 1)
	{
		FLAG_Ref_SSL=-1;

		/* realease the SSL socket */
		SSL_Disconnect(sslHdl);
		SSL_Free( sslHdl );
	}
}

int MessageWithoutConfirmation(char* title, char* image) {


//	APEMV_UI_MessageDisplayDelay(title,NULL);
	T_GL_HWIDGET window = GL_Window_Create(APEMV_UI_GoalHandle());

	// Creates main layout
	T_GL_HWIDGET mainLayout = GL_Layout_Create(window);
	GL_Widget_SetBackColor(window, 0xFFFFFFFF);
	GL_Widget_SetItem(mainLayout, 0, 0);

	// afficher le header par default
	DisplayHeader(_ON_);

	// Title
	T_GL_HWIDGET title_widget = GL_Label_Create(mainLayout);
	GL_Widget_SetItem(title_widget, 0, 0);
	GL_Widget_SetText(title_widget, title);
	GL_Widget_SetForeColor(title_widget, GL_COLOR_BLACK);
	GL_Widget_SetExpand(title_widget, GL_DIRECTION_WIDTH);
	GL_Widget_SetGrow(title_widget, GL_DIRECTION_WIDTH);
	GL_Widget_SetFontSize(title_widget, 18, 18, GL_UNIT_PIXEL);
	GL_Widget_SetMargins(title_widget, 0, 10, 0, 5, GL_UNIT_PIXEL);

	if (image != NULL) {

		T_GL_HWIDGET layoutForPicture = GL_Layout_Create(mainLayout);
		GL_Widget_SetBackColor(window, 0xFFFFFFFF);
		GL_Widget_SetItem(layoutForPicture, 0, 1);

		// Image
		T_GL_HWIDGET image_widget = GL_Picture_Create(layoutForPicture);
		GL_Widget_SetItem(image_widget, 0, 0);
		GL_Widget_SetSource(image_widget, image);
		GL_Widget_SetMaxSize(image_widget, 100, 100, GL_UNIT_PIXEL);
		GL_Widget_SetTransformation(image_widget, GL_TRANSFORMATION_FIT_ALL);

	}

	GL_Window_Dispatch(window, 0);

	GL_Widget_Destroy(window);
//	//APEMV_UI_MessageDisplayDelay("",0);

}

