//****************************************************************************
//       INGENICO                                INGEDEV 7
//============================================================================
//       FILE  COMGPRS.C                           (Copyright INGENICO 2009)
//============================================================================
//  Created :       13-Avril-2009     Kassovic
//  Last modified : 13-Avril-2009     Kassovic
//  Module : TRAINING
//
//  Purpose :
//  GPRS communication management:
//             Transmission by GPRS
//             Test done with Hyper-terminal Tcp/Ip Winsock
//  !!! Link Layer application must be loaded 3628xxxx.SGN inside the terminal
//      Supports PPP, TCP/IP and SSL protocols. Submitted under licence.
//  !!! Link Layer provided under SDK package 3429xxxx.SGN does not support
//      TCP/IP excepted under Wifi and Gprs (So this demo will not work).
//      AVL.lib + LinkLayerExeInterface.lib must be implemented to link properly
//
//  List of routines in file :
//      GprsReport : Provide the state of the GPRS driver.
//      StartGPRS : Attach to the GPRS network.
//      OpenGPRS : Create the GPRS layer.
//      InitGPRS : Configure the GPRS layer.
//      ConnectGPRS : Connect the GPRS layer.
//      SendBufGPRS : Send data through the GPRS layer.
//      RecBufGPRS : Receive data through the GPRS layer.
//      DisconnectGPRS : Disconnect the GPRS layer.
//      CloseGPRS : Delete the GPRS layer.
//      StopGPRS : Break the attachment to the GPRS network.
//      PromptGPRS : Prompt for GPRS's parameters.
//      ComGPRS : GPRS communication demo.
//
//  File history :
//  120508-BK : File created
//
//****************************************************************************

//****************************************************************************
//      INCLUDES
//****************************************************************************
#include "SDK_Naps.h" //#include "sdk.h" // IngeDev automatic refactoring - 2014/04/29 14:29:58 - Replace '#include "SDK30.H"' to '#include "sdk.h"'
#include "sdk.h"
#include "Training.h"

#include "TlvTree.h"
#include "LinkLayer.h"
#include "ExtraGPRS.h"

#include "DNS_.h"


//#include <SDK30.h>
#include <SSL_.h>
#include <X509_.h>
#define __3_SECONDS__ 300
#define __20_SECONDS__ 2000

LL_HANDLE hGPRS = NULL;

/*char g_szProfile[40];
extern short FLAG_Ref_SSL_GPRS;
SSL_HANDLE sslHdl;
SSL_PROFILE_HANDLE ProfileHnd_GPRS;
short FlagSSLPRo_GPRS=0;*/


extern char g_szProfile[40];
extern short FLAG_Ref_SSL;
SSL_HANDLE sslHdl;
SSL_PROFILE_HANDLE ProfileHnd;
short FlagSSLPRo_gprs=0;


SSL_HANDLE hSSL; // Connection handle.
SSL_PROFILE_HANDLE hProfile; // Profile handle.
//****************************************************************************
//      EXTERN
//****************************************************************************

//****************************************************************************
//      EXTERN
//****************************************************************************
static Telium_File_t *hKbdComGPRS;      // Handle of the peripheral Keyboard

//****************************************************************************
//      PRIVATE CONSTANTS
//****************************************************************************
#define MAX_SND  2048	//KT	512
#define MAX_RSP  2048	//KT	512

#define GPRS_TIMEOUT  60*100
#define TCPIP_TIMEOUT 30*100

const char *parseStr(char ucToken, char *pcDst, const char *pcSrc, int iDim);
void APEMVDisplayMedium(byte ucLine, byte ucColumn, char *pcText);
void APEMVDisplaySmall(byte i,byte j,char* mess);

//****************************************************************************
//      PRIVATE TYPES
//****************************************************************************
// Empty

//****************************************************************************
//      PRIVATE DATA
//****************************************************************************
// Empty


void APEMVDisplaySmall(byte i,byte j,char* mess)
{
	APEMV_UI_MessageDisplayDelay(mess,0);
}
void APEMVDisplayMedium(byte i,byte j,char* mess)
{
	APEMV_UI_MessageDisplayDelay(mess,0);
}


//****************************************************************************
//                   int GprsReport (char *pcApn, char *pcUserName)
//  This function provides the state of the GPRS driver.
//  The state of the GPRS driver includes the "start report" and the
//  "sim status", these informations are displayed on screen to show the
//  progress.
//  A low level "fioctl" function should be used (DGPRS_FIOCTL_GET_INFORMATION)
//  to get the information on a structure.
//  A second low level "fioctl" function is also used (DGPRS_FIOCTL_SET_PIN_MODE_FREE)
//  to ask for the pin entry each time the terminal has been switched off.
//       LL_GPRS_Start() : Start GPRS modem and enable facility
//  This function has parameters.
//    pcApn (I-) : Gprs Apn string (max 30 bytes)
//    pcUsername (I-) : Gprs Username string (max 30 bytes)
//  This function has return value
//    >=0 : Report done
//     <0 : Report failed
//****************************************************************************

static int GprsReport(char *pcApn, char *pcUsername) {
	// Local variables
	// ***************
	Telium_File_t *hGprs = NULL;
	int iRequirePin;
	char tcRequirePin[lenGprsRequirePin + 1];   // Pin required ?
	char tcPin[lenGprsPin + 1];
	char tcPin1[lenGprsPin + 1];
	char tcPuk[lenGprsPuk + 1];
	char tcReport[25 + 1], tcStatus[25 + 1];
	T_EGPRS_GET_INFORMATION xInfo;
	int iRet;

	// Get state of GPRS driver
	// ************************
	//APEMV_UI_MessageDisplayDelay((char*) "In GprsReport", 1);

#ifndef __TELIUM3__
	hGprs = Telium_Stdperif((char*)"DGPRS", NULL);
	APEMV_UI_MessageDisplayDelay ((char*)"Telium_Stdperif", 2);
	CHECK(hGprs!=NULL, lblKO);
#endif


	iRet = appGet(appGprsRequirePin, tcRequirePin, lenGprsRequirePin + 1);
	CHECK(iRet >= 0, lblKO);

	iRequirePin = atoi(tcRequirePin); // False => 0 (pin required), True => 1 (pin saved)

	iRet = gprs_SetPinModeFree(hGprs, iRequirePin);

	CHECK(iRet == 0, lblKO);
	iRet = gprs_GetInformation(hGprs, &xInfo, sizeof(xInfo));
	CHECK(iRet == 0, lblKO);


	// GPRS start report
	// =================
	switch (xInfo.start_report) {
	case EGPRS_REPORT_NOT_RECEIVED_YET:
		strcpy(tcReport, "Rpt: NOT_RECEIVED_YET");
		break;
	case EGPRS_REPORT_READY:
		strcpy(tcReport, "Rpt: READY           ");
		break;
	case EGPRS_REPORT_WAITING_CODE:
		strcpy(tcReport, "Rpt: WAITING_CODE    ");
		break;
	case EGPRS_REPORT_SIM_NOT_PRESENT:
		strcpy(tcReport, "Rpt: SIM_NOT_PRESENT ");
		break;
	case EGPRS_REPORT_SIMLOCK_STATE:
		strcpy(tcReport, "Rpt: SIMLOCK_STATE   ");
		break;
	case EGPRS_REPORT_FATAL_ERROR:
		strcpy(tcReport, "Rpt: FATAL_ERROR     ");
		break;
	default:
		strcpy(tcReport, "Rpt: UNKNOWN_ERROR   ");
		break;
	}

	// GPRS sim status
	// ===============
	switch (xInfo.sim_status) {
	case EGPRS_SIM_NOT_RECEIVED_YET:
		strcpy(tcStatus, "Sta: NOT_RECEIVED_YET");

		// *** Start GSM session ***
		iRet = LL_GSM_Start(NULL);
		//APEMV_UI_MessageDisplayDelay ((char*)"GprsReport 5", 2);
		CHECK(iRet==LL_ERROR_OK, lblKO)
		;

		break;
	case EGPRS_SIM_OK:
		strcpy(tcStatus, "Sta: SIM_OK          "); // Sim ready, pin already checked

		// *** Start GPRS session ***
		iRet = LL_GPRS_Start(NULL, pcApn);
		//APEMV_UI_MessageDisplayDelay ((char*)"GprsReport 6", 2);
		CHECK(iRet==LL_ERROR_OK, lblKO)
		;

		APEMVDisplaySmall(7, 0, "CONNECT. IN PROGRESS ");
		break;
	case EGPRS_SIM_KO:
		strcpy(tcStatus, "Sta: SIM_KO          ");
		break;
	case EGPRS_SIM_PIN_REQUIRED:
		strcpy(tcStatus, "Sta: PIN_REQUIRED    ");   // Pin rsequired, enter it

		// *** Enter Pin code ***
		do {
			memset(tcPin, 0, sizeof(tcPin));
			iRet = ShowEntry("ENTER  PIN", "PIN  (8 digits max)", tcPin, 4,
					lenGprsPin, 3);
			if (iRet < 0) {
				APEMVDisplaySmall(6, 4, "Entry exceeded !!!");
				buzzer(10);
				Telium_Ttestall(0, 2 * 100);
			}
		} while (iRet < 0);
		CHECK(iRet != 0, lblEnd)
		;                      // Cancel, Timeout

		// *** Start GSM session ***
		iRet = LL_GSM_Start(tcPin[0] ? tcPin : NULL);
		CHECK(iRet==LL_ERROR_OK, lblKO)
		;

		// *** Start GPRS session ***
		iRet = LL_GPRS_Start(tcPin[0] ? tcPin : NULL, pcApn);
		CHECK(iRet==LL_ERROR_OK, lblKO)
		;

		_clrscr();
		APEMVDisplayMedium(0, 0, "1-comStart...");
		APEMVDisplayMedium(1, 0, pcUsername);

		APEMVDisplaySmall(7, 0, "CONNECT. IN PROGRESS ");
		break;
	case EGPRS_SIM_PIN2_REQUIRED:
		strcpy(tcStatus, "Sta: PIN2 REQUIRED   ");
		break;
	case EGPRS_SIM_PIN_ERRONEOUS:
		strcpy(tcStatus, "Sta: PIN ERRONEOUS   ");   // Wrong Pin, enter it

		// *** Enter Pin code ***
		do {
			memset(tcPin, 0, sizeof(tcPin));
			iRet = ShowEntry("ENTER  PIN", "PIN  (8 digits max)", tcPin, 4,
					lenGprsPin, 3);
			if (iRet < 0) {
				APEMVDisplaySmall(6, 4, "Entry exceeded !!!");
				buzzer(10);
				Telium_Ttestall(0, 2 * 100);
			}
		} while (iRet < 0);
		CHECK(iRet != 0, lblEnd)
		;                      // Cancel, Timeout

		// *** Start GSM session ***
		LL_GSM_Start(tcPin[0] ? tcPin : NULL);

		// *** Start GPRS session ***
		iRet = LL_GPRS_Start(tcPin[0] ? tcPin : NULL, pcApn);
		CHECK(iRet==LL_ERROR_OK, lblKO)
		;

		_clrscr();
		APEMVDisplayMedium(0, 0, "1-comStart...");
		APEMVDisplayMedium(1, 0, pcUsername);

		APEMVDisplaySmall(7, 0, "CONNECT. IN PROGRESS ");
		break;
	case EGPRS_SIM_NOT_INSERTED:
		strcpy(tcStatus, "Sta: SIM_NOT_INSERTED");
		break;
	case EGPRS_SIM_PUK_REQUIRED:
		strcpy(tcStatus, "Sta: PUK_REQUIRED    ");   // Puk required, enter it

		// *** Enter Puk code ***
		do {
			memset(tcPuk, 0, sizeof(tcPuk));
			iRet = ShowEntry("ENTER  PUK", "PUK  (8 digits max)", tcPuk, 4,
					lenGprsPuk, 3);
			if (iRet < 0) {
				APEMVDisplaySmall(6, 4, "Entry exceeded !!!");
				buzzer(10);
				Telium_Ttestall(0, 2 * 100);
			}
		} while (iRet < 0);
		CHECK(iRet != 0, lblEnd)
		;                      // Cancel, Timeout

		// *** Enter new Pin code and confirm ***
		do {
			do {
				memset(tcPin1, 0, sizeof(tcPin1));  // Enter new Pin
				iRet = ShowEntry("ENTER  NEW  PIN", "PIN  (8 digits max)",
						tcPin1, 4, lenGprsPin, 3);
				if (iRet < 0) {
					APEMVDisplaySmall(6, 4, "Entry exceeded !!!");
					buzzer(10);
					Telium_Ttestall(0, 2 * 100);
				}
			} while (iRet < 0);
			CHECK(iRet != 0, lblEnd);                  // Cancel, Timeout

			do {
				memset(tcPin, 0, sizeof(tcPin));    // Confirm new Pin
				iRet = ShowEntry("CONFIRM  NEW  PIN", "PIN  (8 digits max)",
						tcPin, 4, lenGprsPin, 3);
				if (iRet < 0) {
					APEMVDisplaySmall(6, 4, "Entry exceeded !!!");
					buzzer(10);
					Telium_Ttestall(0, 2 * 100);
				}
			} while (iRet < 0);
			CHECK(iRet != 0, lblEnd);                  // Cancel, Timeout
		} while (memcmp(tcPin1, tcPin, lenGprsPin) != 0);

		iRet = gprs_SubmitPuk(hGprs, tcPuk, tcPin);
		if (iRet == 0)                               // Puk code OK
				{
			Telium_Ttestall(0, 1 * 100);  // Wait 1s before restarting a session

			// *** Start GSM session ***
			LL_GSM_Start(tcPin[0] ? tcPin : NULL);

			// *** Start GPRS session ***
			iRet = LL_GPRS_Start(tcPin[0] ? tcPin : NULL, pcApn);
			CHECK(iRet==LL_ERROR_OK, lblKO);
		}

		_clrscr();
		APEMVDisplayMedium(0, 0, "1-comStart...");
		APEMVDisplayMedium(1, 0, pcUsername);

		APEMVDisplaySmall(7, 0, "CONNECT. IN PROGRESS ");
		break;
	case EGPRS_SIM_PUK2_REQUIRED:
		strcpy(tcStatus, "Sta: PUK2_REQUIRED   ");
		break;
	case EGPRS_SIM_PIN_PRESENTED:
		strcpy(tcStatus, "Sta: PIN_PRESENTED   ");
		break;
	default:
		strcpy(tcStatus, "Sta: UNKNOWN_ERROR   ");
		break;
	}
	//APEMV_UI_MessageDisplayDelay ((char*)"GprsReport END", 2);
	//APEMVDisplaySmall(5, 0, tcReport);                   // Display start report
	//APEMVDisplaySmall(6, 0, tcStatus);                   // Display sim status

	iRet = 1;
	goto lblEnd;

	// Errors treatment
	// ****************
	lblKO: iRet = -1;
	goto lblEnd;
	lblEnd: return iRet;
}

//****************************************************************************
//                   int StartGPRS (const char *pcInit)
//  This function handles the attachment to the GPRS network.
//    - LL_GPRS_Connect() : Connect the GPRS network
//    - LL_Network_GetStatus() : Return current status of the network
//  This function has parameters.
//    pcInit (I-) :
//           Apn          = a string (max 30 bytes)
//           Username     = a string (max 30 bytes)
//           Password     = a string (max 30 bytes)
//           The '|' is the separator
//           Ex: "1234|internet-entreprise|orange|orange
//  This function has return value
//    >=0 : Attachment done
//     <0 : Attachment failed
//****************************************************************************

static int StartGPRS(const char *pcInit) {
	// Local variables
	// ***************
	char tcApn[lenGprsApn + 1];                 // Apn
	char tcUsername[lenGprsUser + 1];           // Login
	char tcPassword[lenGprsPass + 1];           // Password
	char cKey;
	word usTimeOut = 0;
	bool bExit = TRUE;
	int iStatus, iRet;
	char buff[30];

	//hKbdComGPRS = Telium_Fopen("KEYBOARD", "r*");

	// Network GPRS parameters
	// ************************
	if (*pcInit) {
		//APEMV_UI_MessageDisplayDelay((char*) "Receive Params", 1);
		memset(tcApn, 0, sizeof(tcApn));
		pcInit = parseStr('|', tcApn, pcInit, sizeof(tcApn));     // Extract apn
		//APEMV_UI_MessageDisplayDelay(tcApn, 1);
		CHECK(pcInit!=NULL, lblKOConfigure);

		memset(tcUsername, 0, sizeof(tcUsername));
		pcInit = parseStr('|', tcUsername, pcInit, sizeof(tcUsername)); // Extract username
		//APEMV_UI_MessageDisplayDelay(tcUsername, 1);
		CHECK(pcInit!=NULL, lblKOConfigure);

		memset(tcPassword, 0, sizeof(tcPassword));
		pcInit = parseStr('|', tcPassword, pcInit, sizeof(tcPassword)); // Extract password
		//APEMV_UI_MessageDisplayDelay(tcPassword, 1);
		CHECK(pcInit!=NULL, lblKOConfigure);
		//APEMV_UI_MessageDisplayDelay((char*) "END Receive Params", 1);
	} else
		goto lblKOConfigure;
	// Configuration error

	// Connect the GPRS network
	// ************************
	Telium_Reset_buf(hKbdComGPRS, _receive_id);
	do {
		iRet = GprsReport(tcApn, tcUsername);
		sprintf(buff, "%d", iRet);
//    	APEMV_UI_MessageDisplayDelay (buff, 1);
//    	APEMV_UI_MessageDisplayDelay ((char*)"Here !", 1);
		CHECK(iRet >= 0, lblKOConfigure);
		//APEMV_UI_MessageDisplayDelay ((char*)"Here 2.1!", 2);
		CHECK(iRet != 0, lblEnd);                             // Cancel, Timeout

//    	APEMV_UI_MessageDisplayDelay ((char*)"Here 2.2 !", 1);
		iRet = LL_GPRS_Connect(tcApn, tcUsername, tcPassword, 10 * 100);
		switch (iRet) {
		//case LL_ERROR_NETWORK_NOT_READY:
		//	bExit = FALSE;
		//	break;
		case LL_ERROR_ALREADY_CONNECTED:
		case LL_ERROR_OK:
			APEMVDisplaySmall(7, 0, "GPRS CONNECTED!!!    ");
			buzzer(10);
			iRet = 1;
			bExit = TRUE;
			break;
		case LL_ERROR_NETWORK_NOT_SUPPORTED:
		case LL_ERROR_SERVICE_CALL_FAILURE:
			bExit = TRUE;
			break;
		default:         // LL_ERROR_NETWORK_NOT_READY or LL_ERROR_NETWORK_ERROR
			iRet = LL_Network_GetStatus(LL_PHYSICAL_V_GPRS, &iStatus);
			switch (iStatus)                       // Check network status
			{
			case LL_STATUS_GPRS_ERROR_SIM_LOCK:    // Pin locked => Wait Puk
			case LL_STATUS_GPRS_ERROR_BAD_PIN:     // Wrong Pin => Wait Pin
			case LL_STATUS_GPRS_ERROR_NO_PIN:      // Pin required => Wait Pin
			case LL_STATUS_GPRS_CONNECTING: // Connection to GPRS Network in progress
			case LL_STATUS_GPRS_AVAILABLE:         // GPRS Network is available
			case LL_STATUS_GPRS_CONNECTING_PPP: // PPP connection is in progress with the GPRS provider
			case LL_STATUS_GPRS_DISCONNECTED: // Disconnection to GPRS Network, let's retry a connection
			case 0x2000600:                        // Wait until network ready
				bExit = FALSE;
				break;
			case LL_STATUS_GPRS_CONNECTED:
				APEMVDisplaySmall(7, 0, "GPRS CONNECTED!!!    ");
				buzzer(10);
				iRet = 1;
				bExit = TRUE;
				break;
			case LL_STATUS_GPRS_ERROR_NO_SIM: // No SIM card is inserted into the terminal
			case LL_STATUS_GPRS_ERROR_PPP: // Error occurred during the PPP link establishment.
			case LL_STATUS_GPRS_ERROR_UNKNOWN:     // Error status unknown
			default:
				bExit = TRUE;
				break;
			}
			break;
		}

		if (!bExit)                        // Exit on cancel or wait for timeout
		{
			iRet = Telium_Ttestall(KEYBOARD, 1 * 100);
			usTimeOut += 1 * 100;
			if (iRet & KEYBOARD) {
				cKey = Telium_Getchar();
				if (cKey == T_ANN)
					usTimeOut = GPRS_TIMEOUT;
			}
		}
	} while (!bExit && (usTimeOut < GPRS_TIMEOUT));

	if (usTimeOut == GPRS_TIMEOUT)                 // Timeout/Cancel
		iRet = 0;

	goto lblEnd;

	// Errors treatment
	// ****************
	lblKOConfigure: iRet = LL_ERROR_INVALID_PARAMETER;
	goto lblEnd;
	lblEnd:
	//if(hKbdComGPRS)
	//	Telium_Fclose(hKbdComGPRS);
	return iRet;
}

//****************************************************************************
//           LL_HANDLE OpenGPRS (const char *pcServer)
//  This function configures the GPRS layer.
//    - LL_Configure() : Create Link Layer configuration
//  This function has no parameter.
//    pcServer (I-) : Tcp/Ip address and port number
//           IpAddress = xxx.xxx.xxx.xxx or url
//           PortNumber = a string (max 5 bytes)
//           The '|' is the separator
//           Ex: "192.168.1.3|2000
//  This function has no return value
//****************************************************************************

static LL_HANDLE OpenGPRS(const char *pcServer) {
	// Local variables
	// ***************
	// Tlv tree nodes
	// ==============
	TLV_TREE_NODE piConfig = NULL;
	TLV_TREE_NODE piPhysicalConfig = NULL;
	TLV_TREE_NODE piTransportConfig = NULL;
	char tcAddr[lenGprsIpRemote + 1];
	char tcPort[lenGprsPort + 1];
	doubleword uiTimeout, uiRemotePort;
	LL_HANDLE hSession = NULL;
	int iRet;

	// Create the LinkLayer configuration parameters tree
	// **************************************************
	CHECK(pcServer!=NULL, lblKOConfigure);               // Configuration failed

	// Create parameters tree
	// ======================
	piConfig = TlvTree_New(LL_TAG_LINK_LAYER_CONFIG); // LinkLayer parameters Root tag of the configuration tree
	CHECK(piConfig!=NULL, lblKOConfigure);

	// Physical layer parameters
	// =========================
	piPhysicalConfig = TlvTree_AddChild(piConfig,
	LL_TAG_PHYSICAL_LAYER_CONFIG,   // TAG Physical layer parameters
			NULL,                           // VALUE (Null)
			0);                             // LENGTH 0
	CHECK(piPhysicalConfig!=NULL, lblKOConfigure);

	// GPRS
	// ----
	TlvTree_AddChildInteger(piPhysicalConfig,
	LL_PHYSICAL_T_LINK,                         // TAG
			LL_PHYSICAL_V_GPRS,                         // VALUE
			LL_PHYSICAL_L_LINK);                        // LENGTH 1

	// Transport and network layer parameters
	// ======================================
	piTransportConfig = TlvTree_AddChild(piConfig,
	LL_TAG_TRANSPORT_LAYER_CONFIG, // TAG Transport layer parameters
			NULL,                          // VALUE (Null)
			0);                            // LENGTH 0
	CHECK(piTransportConfig!=NULL, lblKOConfigure);

	memset(tcAddr, 0, sizeof(tcAddr));
	pcServer = parseStr('|', tcAddr, pcServer, sizeof(tcAddr)); // Parse Tcp/Ip address
	CHECK(pcServer!=NULL, lblKOConfigure);

	memset(tcPort, 0, sizeof(tcPort));
	pcServer = parseStr('|', tcPort, pcServer, sizeof(tcPort)); // Parse Tcp/Ip port
	CHECK(pcServer!=NULL, lblKOConfigure);

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
	uiRemotePort = atoi(tcPort);
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
	iRet = LL_Configure(&hSession, piConfig); // Initialize the handle of the session
	CHECK(iRet==LL_ERROR_OK, lblKOConfigure);

	goto lblEnd;

	// Errors treatment
	// ****************
	lblKOConfigure:                                     // Configuration failed
	hSession = NULL;
	goto lblEnd;
	lblEnd: if (piConfig)
		TlvTree_Release(piConfig);          // Release tree to avoid memory leak
	return hSession;                         // Return the handle of the session
}

//****************************************************************************
//              int ConnectGPRS (LL_HANDLE hSession)
//  This function connects the GPRS layer.
//    - LL_Connect() : Connect Link Layer
//  This function has parameters.
//    hSession (I-) : Handle of the session
//  This function has return value
//    >=0 : Connect done
//     <0 : Connect failed
//****************************************************************************

static int ConnectGPRS(LL_HANDLE hSession) {
	// Local variables
	// ***************
	int iRet;

	// Link Layer connection
	// *********************
	iRet = LL_Connect(hSession);

	return iRet;
}

//****************************************************************************
//       int SendGPRS (LL_HANDLE hSession, const char *pcMsg, word usLen)
//  This function sends data through the GPRS layer.
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

static int SendGPRS(LL_HANDLE hSession, const char *pcMsg, word usLen) {
	// Local variables
	// ***************
	int iNbrBytes, iRet;
	char buff[10];
	// Send data
	// *********
	iNbrBytes = LL_Send(hSession, usLen, pcMsg, LL_INFINITE);
	memset(buff, 0, 10);
	sprintf(buff, "lg:%d", iNbrBytes);
	APEMV_UI_MessageDisplayDelay(buff, 3);
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
//            int ReceiveGPRS (LL_HANDLE hSession, char *pcMsg,
//                             word usLen, byte ucDly)
//  This function receives data through the GPRS layer.
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

static int ReceiveGPRS(LL_HANDLE hSession, char *pcMsg, word usLen, byte ucDly) {
	// Local variables
	// ***************
	char cKey, buff[10];
	long lSec, lTimeOut = LL_INFINITE;
	int iRet, iLength = 0, iNbrBytes;


	// Timeout setting
	// ***************
	if (ucDly != 0xFF)
		lSec = ucDly * 1000;
	else
		lSec = LL_INFINITE;

	hKbdComGPRS = Telium_Fopen("KEYBOARD", "r*");

	// Receiving first block
	// *********************
	//APEMV_UI_MessageDisplayDelay((char*) "trace 2", 2);

	Telium_Reset_buf(hKbdComGPRS, _receive_id);           // Reset keyboard FIFO
	//APEMV_UI_MessageDisplayDelay((char*) "trace 3", 2);

	iRet = TimerStart(0, lSec);                                 // Timer0 starts
	//APEMV_UI_MessageDisplayDelay((char*) "trace 4", 2);

	CHECK(iRet >= 0, lblTimeOut);
	do {
		iNbrBytes = LL_Receive(hSession, usLen, pcMsg + iLength, 1 * 100); // Check reception
		//APEMV_UI_MessageDisplayDelay((char*) "trace 5", 2);

		/*memset(buff, 0, 32);
		sprintf(buff,"%d", iNbrBytes);
	    APEMV_UI_MessageDisplayDelay (buff, 3);*/

		if (iNbrBytes != 0)
			break;                                             // Bytes received
		iRet = Telium_Ttestall(KEYBOARD, 1);          // Check for a key pressed
		//APEMV_UI_MessageDisplayDelay((char*) "trace 6", 2);

		if (iRet & KEYBOARD)                                     // Key detected
		{
			cKey = Telium_Getchar();                     // Retrieve key pressed
			//APEMV_UI_MessageDisplayDelay((char*) "trace 7", 2);
			CHECK(cKey!=T_ANN, lblTimeOut);                // Exit on cancel key
		}
		if (lSec != LL_INFINITE)
			lTimeOut = TimerGet(0);                      // Retrieve timer value
		APEMV_UI_MessageDisplayDelay((char*) "trace Reciv 1", 2);
	} while (lTimeOut > 0);
	APEMV_UI_MessageDisplayDelay((char*) "trace Reciv 2", 2);

	CHECK(lTimeOut != 0, lblTimeOut);                         // Exit on timeout

	iLength += iNbrBytes;
	iRet = LL_GetLastError(hSession);
	APEMV_UI_MessageDisplayDelay((char*) "trace 10", 2);
	CHECK(iRet==LL_ERROR_OK, lblEnd);
	if ((iNbrBytes != 0) && (iNbrBytes < usLen)) {
		// Receiving next block until timeout (Inter block 500ms)
		// ======================================================
		APEMV_UI_MessageDisplayDelay((char*) "trace IF 1", 2);

		while (1) {
			APEMV_UI_MessageDisplayDelay((char*) "tWHILE 1", 2);
			iNbrBytes = LL_Receive(hSession, usLen - iLength, pcMsg + iLength, 50);
			APEMV_UI_MessageDisplayDelay((char*) "tWHILE 2", 2);
			iLength += iNbrBytes;
			iRet = LL_GetLastError(hSession);
			APEMV_UI_MessageDisplayDelay((char*) "tWHILE 3", 2);

			CHECK((iRet==LL_ERROR_OK) || (iRet==LL_ERROR_TIMEOUT), lblEnd);
			APEMV_UI_MessageDisplayDelay((char*) "erreur 1", 2);
			if ((iRet == LL_ERROR_TIMEOUT) || (iNbrBytes == 0)){
				APEMV_UI_MessageDisplayDelay((char*) "erreur 2", 2);
				break;
			}
		}
		APEMV_UI_MessageDisplayDelay((char*) "trace IF fin", 2);
	}
	APEMV_UI_MessageDisplayDelay((char*) "tWHILE 4", 2);

	iRet = iLength;
	goto lblEnd;

	// Errors treatment
	// ****************
	lblTimeOut: iRet = LL_ERROR_TIMEOUT;
	goto lblEnd;
	lblEnd: TimerStop(0);
	if (hKbdComGPRS)
		Telium_Fclose(hKbdComGPRS);
	return iRet;
}

//****************************************************************************
//                   int DisconnectGPRS (LL_HANDLE hSession)
//  This function disconnects the GPRS layer.
//    - LL_Disconnect() : Disconnect Link Layer
//  This function has no parameters.
//    hSession (I-) : Handle of the session
//  This function has return value
//    >=0 : Disconnect done
//     <0 : Disconnect failed
//****************************************************************************

static int DisconnectGPRS(LL_HANDLE hSession) {
	// Local variables
	// ***************
	int iRet;

	// Link Layer disconnection
	// ************************
	iRet = LL_Disconnect(hSession);

	return iRet;
}

//****************************************************************************
//                 int CloseGPRS (LL_HANDLE hSession)
//  This function deletes the handle of the GPRS layer.
//    - LL_Configure() : Delete Link Layer configuration
//  This function has no parameters.
//    hSession (I-) : Handle of the session
//  This function has return value
//    >=0 : Configuration deleted
//     <0 : Processing failed
//****************************************************************************

static int CloseGPRS(LL_HANDLE hSession) {
	// Local variables
	// ***************
	int iRet;

	// Delete the configuration
	// ************************
	iRet = LL_Configure(&hSession, NULL);

	return iRet;
}

//****************************************************************************
//                        int StopGPRS (void)
//  This function breaks the attachment to the GPRS network.
//    - LL_GPRS_Stop() : Disconnect the GPRS network
//  This function has no parameters.
//  This function has return value
//    >=0 : Break attachment done
//     <0 : Break attachment failed
//****************************************************************************

static int StopGPRS(void) {
	// Local variables
	// ***************
	int iRet;

	// GPRS disconnection
	// ******************
//	iRet = LL_GPRS_Stop();

	return iRet;
}

//****************************************************************************
//                      void PromptGPRS (void)
//  This function asks for the GPRS's parameters.
//  This function has no parameters.
//  This function has no return value
//****************************************************************************

void PromptGPRS(void) {
	// Local variables
	// ***************
	char tcRequirePin[lenGprsRequirePin + 1];
	char tcAPN[lenGprsApn + 1];
	char tcUser[lenGprsUser + 1];
	char tcPass[lenGprsPass + 1];
	char tcIpAddress[lenGprsIpRemote + 1];
	char tcPort[lenGprsPort + 1];
	int iRet;

	//APEMV_UI_MessageDisplayDelay((char *) "In PromptGPRS", 2);

	// Pin Required ?
	// ***************
	do {
		iRet = appGet(appGprsRequirePin, tcRequirePin, lenGprsRequirePin + 1);
		CHECK(iRet >= 0, lblDbaErr);
		iRet = ShowEntry("PIN  REQUIRED?", "Yes=0/No=1  (1 digit)",
				tcRequirePin, 0, lenGprsRequirePin, 2);
		if (iRet < 0) {
			APEMVDisplaySmall(6, 4, "Entry exceeded !!!");
			buzzer(10);
			Telium_Ttestall(0, 2 * 100);
		}
	} while (iRet < 0);
	CHECK(iRet != 0, lblEnd);                                 // Cancel, Timeout
	if (iRet == 2)                                                   // Save pin
			{
		iRet = appPut(appGprsRequirePin, tcRequirePin, lenGprsRequirePin);
		CHECK(iRet >= 0, lblDbaErr);
	}

	// Enter APN
	// *********
	do {
		//iRet = appGet(appGprsApn, tcAPN, lenGprsApn + 1);
		//CHECK(iRet >= 0, lblDbaErr);

		iRet = ShowEntry("ENTER  APN", "", tcAPN, 0, lenGprsApn, 0);
		if (iRet < 0) {
			APEMVDisplaySmall(5, 4, "Entry exceeded !!!");
			buzzer(10);
			Telium_Ttestall(0, 2 * 100);
		}
	} while (iRet < 0);
	CHECK(iRet != 0, lblEnd);                                 // Cancel, Timeout
	if (iRet == 2)                                                   // Save apn
			{
		iRet = appPut(appGprsApn, tcAPN, lenGprsApn);
		CHECK(iRet >= 0, lblDbaErr);
	}

	// Enter User Name
	// ***************
	do {
		iRet = appGet(appGprsUser, tcUser, lenGprsUser + 1);
		CHECK(iRet >= 0, lblDbaErr);
		iRet = ShowEntry("ENTER  USERNAME", "", tcUser, 0, lenGprsUser, 0);
		if (iRet < 0) {
			APEMVDisplaySmall(5, 4, "Entry exceeded !!!");
			buzzer(10);
			Telium_Ttestall(0, 2 * 100);
		}
	} while (iRet < 0);
	CHECK(iRet != 0, lblEnd);                                 // Cancel, Timeout
	if (iRet == 2)                                              // Save username
			{
		iRet = appPut(appGprsUser, tcUser, lenGprsUser);
		CHECK(iRet >= 0, lblDbaErr);
	}

	// Enter Password
	// **************
	do {
		//memset(tcPass, '*', 8);
		iRet = appGet(appGprsPass, tcPass, lenGprsPass + 1);
		CHECK(iRet >= 0, lblDbaErr);
		iRet = ShowEntry("ENTER  PASSWORD", "", tcPass, 0, lenGprsPass, 0);
		if (iRet < 0) {
			APEMVDisplaySmall(5, 4, "Entry exceeded !!!");
			buzzer(10);
			Telium_Ttestall(0, 2 * 100);
		}
	} while (iRet < 0);
	CHECK(iRet != 0, lblEnd);                                 // Cancel, Timeout
	if (iRet == 2)                                              // Save password
			{
		iRet = appPut(appGprsPass, tcPass, lenGprsPass);
		CHECK(iRet >= 0, lblDbaErr);
	}

	// Enter Remote IP address
	// ***********************
	do {
		iRet = appGet(appGprsIpRemote, tcIpAddress, lenGprsIpRemote + 1);
		CHECK(iRet >= 0, lblDbaErr);
		iRet = ShowEntry("ENTER REMOTE IP", "", tcIpAddress, 0, lenGprsIpRemote,
				4);
		if (iRet < 0) {
			APEMVDisplaySmall(6, 4, "Entry exceeded !!!");
			buzzer(10);
			Telium_Ttestall(0, 2 * 100);
		}
	} while (iRet < 0);
	CHECK(iRet != 0, lblEnd);                                 // Cancel, Timeout
	if (iRet == 2)                                            // Save Ip address
			{
		iRet = appPut(appGprsIpRemote, tcIpAddress, lenGprsIpRemote);
		CHECK(iRet >= 0, lblDbaErr);
	}

	// Enter Port number
	// *****************
	do {
		iRet = appGet(appGprsPort, tcPort, lenGprsPort + 1);
		CHECK(iRet >= 0, lblDbaErr);
		iRet = ShowEntry("ENTER  PORT  NUMBER", "", tcPort, 0, lenGprsPort, 0);
		if (iRet < 0) {
			APEMVDisplaySmall(5, 4, "Entry exceeded !!!");
			buzzer(10);
			Telium_Ttestall(0, 2 * 100);
		}
	} while (iRet < 0);
	CHECK(iRet != 0, lblEnd);                                 // Cancel, Timeout
	if (iRet == 2)                                                  // Save port
			{
		iRet = appPut(appGprsPort, tcPort, lenGprsPort);
		CHECK(iRet >= 0, lblDbaErr);
	}

	PutAllParam();

	goto lblEnd;

	// Errors treatment
	// ****************
	lblDbaErr: _clrscr();                                        // Clear screen
	APEMVDisplayMedium(0, 0, FMG_ErrorMsg(iRet));                        // FMG error
	APEMVDisplayMedium(1, 0, "Reset Needed");                    // Rebuild data base
	Telium_Ttestall(0, 5 * 100);                                     // Delay 5s
	goto lblEnd;
	lblEnd: return;
}



//****************************************************************************
//                      void ComGPRS (void)
//  This function communicates through the GPRS layer.
//  This function has no parameters.
//  This function has no return value
//****************************************************************************

void ComGPRS(void) {
	// Local variables
	// ***************

	char *pcStr, tcStr[128 + 1];
	char tcUser[lenGprsUser + 1];
	char tcIpAddress[lenGprsIpRemote + 1];
	char tcPort[5 + 1];
	int iRet = 0;
	char tcSnd[MAX_SND + 1];
	char tcRsp[MAX_RSP + 1];
	int iStatus = 0;
	char buff[10];

	APEMV_UI_MessageDisplayDelay((char *) "In ComGPRS", 2);

	CHECK((IsRadioGPRS() == 1) || (IsRadio3G() == 1), lblNoRadio); // Check if radio device

	hKbdComGPRS = Telium_Fopen("KEYBOARD", "r*");

	// Attachment to the GPRS network in progress
	// ******************************************
	_clrscr();                                                   // Clear screen
	APEMVDisplayMedium(0, 0, "1-comStart first...");         // Display start message
	pcStr = tcStr;                                         // Build param string
	iRet = appGet(appGprsApn, pcStr, lenGprsApn + 1);            // Retrieve apn
	CHECK(iRet >= 0, lblDbaErr);
	pcStr += strlen(pcStr);
	*pcStr = '|';
	pcStr++;
	APEMV_UI_MessageDisplayDelay(pcStr, 2);
	iRet = appGet(appGprsUser, pcStr, lenGprsUser + 1);     // Retrieve username
	CHECK(iRet >= 0, lblDbaErr);
	pcStr += strlen(pcStr);
	*pcStr = '|';
	pcStr++;
	APEMV_UI_MessageDisplayDelay(pcStr, 2);
	iRet = appGet(appGprsPass, pcStr, lenGprsPass + 1);     // Retrieve password
	CHECK(iRet >= 0, lblDbaErr);
	pcStr += strlen(pcStr);
	*pcStr = '|';
	APEMV_UI_MessageDisplayDelay(pcStr, 2);
	iRet = appGet(appGprsUser, tcUser, lenGprsUser + 1); // Retrieve username to display
	CHECK(iRet >= 0, lblDbaErr);
	APEMVDisplayMedium(1, 0, tcUser);                             // Display username
	iRet = StartGPRS(pcStr);
	CHECK(iRet >= 0, lblComKO);
//    APEMV_UI_MessageDisplayDelay ((char*)"StartGPRS END", 1);
//      sprintf(buff,"%d", iRet);
//    APEMV_UI_MessageDisplayDelay ((char*)"Return", 1);
//    APEMV_UI_MessageDisplayDelay (buff, 2);
	CHECK(iRet != 0, lblEnd);                                 // Cancel, Timeout
	APEMVDisplayMedium(2, 0, "1-comStart OK");                    // Display start OK

	// Transmission through GPRS layer in progress
	// *******************************************
	memset(tcSnd, 0, sizeof(tcSnd));
	memset(tcRsp, 0, sizeof(tcRsp));

	// Open GPRS layer
	// ===============
	APEMVDisplayMedium(3, 0, "2-comOpen...");                 // Display open message
	iRet = appGet(appGprsIpRemote, tcIpAddress, lenGprsIpRemote + 1); // Retrieve remote IP
	CHECK(iRet >= 0, lblDbaErr);
	_clrscr();                                                   // Clear screen
	APEMVDisplayMedium(0, 0, tcIpAddress);                      // Display IP Address
	iRet = appGet(appGprsPort, tcPort, lenGprsPort + 1); // Retrieve port number
	CHECK(iRet >= 0, lblDbaErr);
	APEMVDisplayMedium(1, 0, tcPort);                          // Display port number
	sprintf(tcStr, "%s|%s", tcIpAddress, tcPort);
	hGPRS = OpenGPRS(tcStr);
	CHECK(hGPRS!=NULL, lblKO);               // ** Open **
	APEMVDisplayMedium(2, 0, "2-comOpen OK");                      // Display open OK

	// Connect GPRS layer
	// ==================
	APEMVDisplayMedium(3, 0, "3-comConnect...");           // Display connect message
	iRet = ConnectGPRS(hGPRS);
	CHECK(iRet >= 0, lblComKO);              // ** Connect **
	_clrscr();                                                   // Clear screen
	APEMVDisplayMedium(0, 0, "3-comConnect OK");                // Display connect OK

	// Clear sending/receiving buffers
	// ===============================
	iRet = LL_ClearSendBuffer(hGPRS);                    // Clear sending buffer
	CHECK(iRet==LL_ERROR_OK, lblComKO);
	iRet = LL_ClearReceiveBuffer(hGPRS);                 // Clear receiving buffer
	CHECK(iRet==LL_ERROR_OK, lblComKO);

	// Send data through GPRS layer
	// ============================
	APEMVDisplayMedium(1, 0, "4-comSend...");                 // Display send message
	strcpy(tcSnd, "Hello, do you hear me?\n"                     // Data to send
					"Hello, do you hear me?\n"
					"Hello, do you hear me?\n"
					"Hello, do you hear me?\n"
					"Hello, do you hear me?\n");
	iRet = SendGPRS(hGPRS, tcSnd, (word) strlen(tcSnd));      // ** Send data **
	CHECK(iRet >= 0, lblComKO);
	APEMVDisplayMedium(2, 0, "4-comSend OK");                      // Display send OK

	// Receive data through GPRS layer
	// ===============================
	APEMVDisplayMedium(3, 0, "5-comRec...");               // Display receive message
	iRet = ReceiveGPRS(hGPRS, tcRsp, sizeof(tcRsp), 30);   // ** Receive data **
	CHECK(iRet >= 0, lblComKO);
	if (iRet > MAX_RSP)
		strcpy(tcRsp, "Buffer overflow Max=512");    // Data overflow
	_clrscr();                                                   // Clear screen
	DisplayMedium(0, 0, "5-comRec OK");                    // Display receive OK

	// Disconnection
	// =============
	iRet = DisconnectGPRS(hGPRS);                            // ** Disconnect **
	CHECK(iRet >= 0, lblComKO);
	APEMVDisplayMedium(1, 0, "6-comDisconn OK");             // Display disconnect OK
	iRet = CloseGPRS(hGPRS);                                      // ** Close **
	CHECK(iRet >= 0, lblComKO);

	// Display transmission data
	// =========================
	_clrscr();                                                   // Clear screen
	APEMVDisplaySmall(0, 0, "RECEIVING:");                 // Display data to receive
	iRet = Display25(1, tcRsp, strlen(tcRsp));
	CHECK(iRet >= 0, lblKO);
	APEMVDisplaySmall(7, 6, "Press a key");

	// Print transmission data
	// =======================
	iRet = Telium_Pprintf("\x1b" "E" "        GPRS Demo\n\n" // Print demo in progress
					"\x1b" "F");
	CHECK(iRet >= 0, lblKO);
	iRet = Telium_Pprintf("Sending:\n");
	CHECK(iRet >= 0, lblKO);       // Print data to send
	iRet = Print24(tcSnd, strlen(tcSnd));
	CHECK(iRet >= 0, lblKO);
	iRet = Telium_Pprintf("\nReceiving:\n");
	CHECK(iRet >= 0, lblKO);   // Print data to receive
	iRet = Print24(tcRsp, strlen(tcRsp));
	CHECK(iRet >= 0, lblKO);
	iRet = Telium_Pprintf("\n\n\n\n\n\n");
	CHECK(iRet >= 0, lblKO);

	Telium_Reset_buf(hKbdComGPRS, _receive_id);           // Reset keyboard FIFO
	Telium_Ttestall(KEYBOARD, 30 * 100);    // Wait until key pressed or timeout
	Telium_Reset_buf(hKbdComGPRS, _receive_id); // Reset keyboard FIFO to clear key pressed

	goto lblEnd;

	// Errors treatment
	// ****************
	lblKO:                                   // None-classified low level error
		_clrscr();                                                   // Clear screen
		APEMVDisplayMedium(0, 0, "Processing Error");                      // Oops! error
		Telium_Ttestall(0, 5 * 100);                                     // Delay 5s
		goto lblEnd;
		lblNoRadio:                                            // No GPRS-3G module
		_clrscr();                                                   // Clear screen
		APEMVDisplayMedium(0, 0, "No Radio Device");                       // Oops! error
		Telium_Ttestall(0, 5 * 100);                                     // Delay 5s
		goto lblEnd;
	lblComKO:                                            // Communication error
		_clrscr();                                                   // Clear screen
		APEMVDisplayMedium(0, 0, LL_ErrorMsg(iRet));                  // Link Layer error
		if (iRet == LL_Network_GetStatus(LL_PHYSICAL_V_GPRS, &iStatus)) {
			switch (iStatus) {
			case LL_STATUS_GPRS_ERROR_NO_SIM:
				iStatus = LL_STATUS_GPRS_NO_SIM;
				break;
			case LL_STATUS_GPRS_ERROR_PPP:
				iStatus = LL_STATUS_GPRS_ERR_PPP;
				break;
			case LL_STATUS_GPRS_ERROR_UNKNOWN:
				iStatus = LL_STATUS_GPRS_ERR_UNKNOWN;
				break;
			default:
				iStatus = -1;
				break;
			}
			APEMVDisplayMedium(1, 0, LL_ErrorMsg(iStatus));          // Link Layer status
		}
		Telium_Ttestall(0, 5 * 100);                                     // Delay 5s
		goto lblEnd;
	lblDbaErr:                                               // Data base error
		_clrscr();                                                   // Clear screen
		APEMVDisplayMedium(0, 0, FMG_ErrorMsg(iRet));                        // FMG error
		APEMVDisplayMedium(1, 0, "Reset Needed");                    // Rebuild data base
		Telium_Ttestall(0, 5 * 100);                                     // Delay 5s
		goto lblEnd;
	lblEnd:
		if (hGPRS) {
			DisconnectGPRS(hGPRS);                               // ** Disconnect **
			CloseGPRS(hGPRS);                                         // ** Close **
		}
		if (hKbdComGPRS)
			Telium_Fclose(hKbdComGPRS);
		StopGPRS();                                                    // ** Stop **
}

int connexion_GPRS(void) {
	int g_nPort, i_res;
	char *g_szHostName;

	//strcpy(FicPara.login_gprs,"MEDINET");
	//strcpy(FicPara.pwd_gprs,"MEDINET");

	// Connexion GPRS
	//memcpy(g_szHostName, FicPara.ip_gprs, 15);
	//g_nPort = atol((PC) FicPara.port_gprs);

	i_res = CallPerformTCP_GPRS(g_szHostName, g_nPort);

	if (i_res == KO)
		return KO ;
	else
		return OK ;

	return OK ;
}

int CallPerformTCP_GPRS(char *szHostname, int nPort) {
	// Local variables
	// ***************
	char *pcStr, tcStr[128 + 1];
	char tcUser[lenGprsUser + 1];
	char tcIpAddress[lenGprsIpRemote + 1];
	char tcPort[lenGprsPort + 1];
	int iRet = 0, iStatus = 0;
	char buff[40];

	CHECK((IsRadioGPRS() == 1) || (IsRadio3G() == 1), lblNoRadio); // Check if radio device

	//strcpy(FicPara.ip_gprs, "105.159.251.120");
//	strcpy(FicPara.ip_gprs, "197.230.14.34");
//	//strcpy(FicPara.port_gprs, "6002");
//	strcpy(FicPara.port_gprs, "8012");
//	strcpy(FicPara.apn_gprs, "internet.orange.ma");
//	strcpy(FicPara.login_gprs, "MEDINET");
//	strcpy(FicPara.pwd_gprs, "MEDINET");

	// Attachment to the GPRS network in progress
	// ******************************************
	_clrscr();                                                   // Clear screen
	//APEMVDisplayMedium(0, 0, "1-comStart...");               // Display start message
	pcStr = tcStr;                                         // Build param string

	//iRet = appGet(appGprsApn, pcStr, lenGprsApn + 1);            // Retrieve apn
	//CHECK(iRet >= 0, lblDbaErr);

	/*
	 sprintf(buff, "%s",FicPara.apn_gprs);
	 memcpy(pcStr, buff, strlen(buff) + 1);
	 pcStr += strlen(pcStr);
	 *pcStr = '|';
	 pcStr++;
	 //	iRet = appGet(appGprsUser, pcStr, lenGprsUser + 1);     // Retrieve username
	 //	CHECK(iRet >= 0, lblDbaErr);


	 sprintf(buff, "%s", FicPara.login_gprs);
	 memcpy(pcStr, buff, strlen(buff) + 1);
	 pcStr += strlen(pcStr);
	 *pcStr = '|';
	 pcStr++;
	 //	iRet = appGet(appGprsPass, pcStr, lenGprsPass + 1);     // Retrieve password
	 //	CHECK(iRet >= 0, lblDbaErr);

	 sprintf(buff, "%s", FicPara.pwd_gprs);
	 memcpy(pcStr, buff, strlen(buff) + 1);
	 pcStr += strlen(pcStr);
	 *pcStr = '|';
	 pcStr++;
	 */
//	iRet = appGet(appGprsUser, tcUser, lenGprsUser + 1); // Retrieve username to display
//	CHECK(iRet >= 0, lblDbaErr);
	//APEMVDisplayMedium(1, 0, tcUser);                             // Display username
	sprintf(pcStr, "%s|%s|%s", FicPara.apn_gprs, FicPara.login_gprs, FicPara.pwd_gprs);
	_clrscr();
	//APEMVDisplayMedium(1, 0, pcStr);
	iRet = StartGPRS(pcStr);
	CHECK(iRet >= 0, lblComKO);                // ** Start **
	CHECK(iRet != 0, lblEnd);                                 // Cancel, Timeout
	//APEMVDisplayMedium(2, 0, "1-comStart OK");                    // Display start OK
	// Open GPRS layer
	// ===============
	//APEMVDisplayMedium(3, 0, "2-comOpen...");                 // Display open message
//	iRet = appGet(appGprsIpRemote, tcIpAddress, lenGprsIpRemote + 1); // Retrieve remote IP
//	CHECK(iRet >= 0, lblDbaErr);
	_clrscr();                                                   // Clear screen
	//APEMVDisplayMedium(0, 0, FicPara.ip_gprs);                  // Display IP Address
//	iRet = appGet(appGprsPort, tcPort, lenGprsPort + 1); // Retrieve port number
//	CHECK(iRet >= 0, lblDbaErr);
	//APEMVDisplayMedium(1, 0, FicPara.port_gprs);    // Display port number

	//     FicPara
	sprintf(tcStr, "%s|%s", FicPara.ip_gprs, FicPara.port_gprs);

	hGPRS = OpenGPRS(tcStr);
	CHECK(hGPRS!=NULL, lblKO);               // ** Open **
	//APEMVDisplayMedium(2, 0, "2-comOpen OK");                      // Display open OK

	// Connect GPRS layer
	// ==================
//	APEMVDisplayMedium(3, 0, "3-comConnect...");           // Display connect message
//	iRet = ConnectGPRS(hGPRS);
//	CHECK(iRet >= 0, lblComKO);              // ** Connect **
//	_clrscr();                                                   // Clear screen
//	APEMVDisplayMedium(0, 0, "3-comConnect OK");                // Display connect OK
//
//	// Clear sending/receiving buffers
//	// ===============================
//	iRet = LL_ClearSendBuffer(hGPRS);                    // Clear sending buffer
//	CHECK(iRet==LL_ERROR_OK, lblComKO);
//	iRet = LL_ClearReceiveBuffer(hGPRS);               // Clear receiving buffer
//	CHECK(iRet==LL_ERROR_OK, lblComKO);

	return OK ;

	// Errors treatment
	// ****************
	lblKO:                                   // None-classified low level error
	_clrscr();                                                   // Clear screen
	APEMVDisplayMedium(0, 0, "Processing Error");                      // Oops! error
	Telium_Ttestall(0, 5 * 100);                                     // Delay 5s
	goto lblEnd;
	lblNoRadio:                                            // No GPRS-3G module
	_clrscr();                                                   // Clear screen
	APEMVDisplayMedium(0, 0, "No Radio Device");                       // Oops! error
	Telium_Ttestall(0, 5 * 100);                                     // Delay 5s
	goto lblEnd;
	lblComKO:                                            // Communication error
	_clrscr();                                                   // Clear screen
	APEMVDisplayMedium(0, 0, LL_ErrorMsg(iRet));                  // Link Layer error
	if (iRet == LL_Network_GetStatus(LL_PHYSICAL_V_GPRS, &iStatus)) {
		switch (iStatus) {
		case LL_STATUS_GPRS_ERROR_NO_SIM:
			iStatus = LL_STATUS_GPRS_NO_SIM;
			break;
		case LL_STATUS_GPRS_ERROR_PPP:
			iStatus = LL_STATUS_GPRS_ERR_PPP;
			break;
		case LL_STATUS_GPRS_ERROR_UNKNOWN:
			iStatus = LL_STATUS_GPRS_ERR_UNKNOWN;
			break;
		default:
			iStatus = -1;
			break;
		}
		APEMVDisplayMedium(1, 0, LL_ErrorMsg(iStatus));          // Link Layer status
	}
	Telium_Ttestall(0, 5 * 100);                                     // Delay 5s
	goto lblEnd;
	lblDbaErr:                                               // Data base error
	_clrscr();                                                   // Clear screen
	APEMVDisplayMedium(0, 0, FMG_ErrorMsg(iRet));                        // FMG error
	APEMVDisplayMedium(1, 0, "Reset Needed");                    // Rebuild data base
	Telium_Ttestall(0, 5 * 100);                                     // Delay 5s
	goto lblEnd;
	lblEnd: if (hGPRS) {
		DisconnectGPRS(hGPRS);                               // ** Disconnect **
		CloseGPRS(hGPRS);                                         // ** Close **
	}
	StopGPRS();                                                    // ** Stop **
	return KO ;
}

int StopGprs_naps(){

	APEMV_UI_MessageDisplay("DECONNEXION EN COURS ...");
	int iRet = LL_GPRS_Stop();
	if(iRet == LL_ERROR_OK)
		APEMV_UI_MessageDisplayDelay("DECONNECTEE", 2);
	else
		APEMV_UI_MessageDisplayDelay("ERREUR", 2);
	return iRet;
}

int StartGprs_naps() {
	// Local variables
	// ***************
	char *pcStr, tcStr[128 + 1];
	char tcUser[lenGprsUser + 1];
	char tcIpAddress[lenGprsIpRemote + 1];
	char tcPort[lenGprsPort + 1];
	int iRet = 0, iStatus = 0;
	char buff[40];

	CHECK((IsRadioGPRS() == 1) || (IsRadio3G() == 1), lblNoRadio); // Check if radio device

	// Attachment to the GPRS network in progress
	// ******************************************
	_clrscr();                                                   // Clear screen
	//APEMVDisplayMedium(0, 0, "1-comStart...");               // Display start message
	pcStr = tcStr;                                         // Build param string

	//APEMVDisplayMedium(1, 0, tcUser);                             // Display username
	sprintf(pcStr, "%s|%s|%s", FicPara.apn_gprs, FicPara.login_gprs, FicPara.pwd_gprs);
	_clrscr();

	iRet = StartGPRS(pcStr);
	CHECK(iRet >= 0, lblComKO);                // ** Start **
	CHECK(iRet != 0, lblEnd);                                 // Cancel, Timeout
	//APEMVDisplayMedium(2, 0, "1-comStart OK");                    // Display start OK
	// Open GPRS layer
	// ===============
	//APEMVDisplayMedium(3, 0, "2-comOpen...");                 // Display open message

	//     FicPara
	sprintf(tcStr, "%s|%s", FicPara.ip_gprs, FicPara.port_gprs);
//	Os__xprinta(tcStr);

	hGPRS = OpenGPRS(tcStr);
	CHECK(hGPRS!=NULL, lblKO);               // ** Open **
	//APEMVDisplayMedium(2, 0, "2-comOpen OK");                      // Display open OK
	return OK ;

	// Errors treatment
	// ****************
lblKO:                                   // None-classified low level error
	_clrscr();                                                   // Clear screen
	APEMVDisplayMedium(0, 0, "Processing Error");                      // Oops! error
	Telium_Ttestall(0, 5 * 100);                                     // Delay 5s
	goto lblEnd;
lblNoRadio:                                            // No GPRS-3G module
	_clrscr();                                                   // Clear screen
	APEMVDisplayMedium(0, 0, "No Radio Device");                       // Oops! error
	Telium_Ttestall(0, 5 * 100);                                     // Delay 5s
	goto lblEnd;
lblComKO:                                            // Communication error
	_clrscr();                                                   // Clear screen
	APEMVDisplayMedium(0, 0, LL_ErrorMsg(iRet));                  // Link Layer error
	if (iRet == LL_Network_GetStatus(LL_PHYSICAL_V_GPRS, &iStatus)) {
		switch (iStatus) {
		case LL_STATUS_GPRS_ERROR_NO_SIM:
			iStatus = LL_STATUS_GPRS_NO_SIM;
			break;
		case LL_STATUS_GPRS_ERROR_PPP:
			iStatus = LL_STATUS_GPRS_ERR_PPP;
			break;
		case LL_STATUS_GPRS_ERROR_UNKNOWN:
			iStatus = LL_STATUS_GPRS_ERR_UNKNOWN;
			break;
		default:
			iStatus = -1;
			break;
		}
		APEMVDisplayMedium(1, 0, LL_ErrorMsg(iStatus));          // Link Layer status
	}
	Telium_Ttestall(0, 5 * 100);                                     // Delay 5s
	goto lblEnd;
lblDbaErr:                                               // Data base error
	_clrscr();                                                   // Clear screen
	APEMVDisplayMedium(0, 0, FMG_ErrorMsg(iRet));                        // FMG error
	APEMVDisplayMedium(1, 0, "Reset Needed");                    // Rebuild data base
	Telium_Ttestall(0, 5 * 100);                                     // Delay 5s
	goto lblEnd;
lblEnd:
	if (hGPRS) {
		DisconnectGPRS(hGPRS);                               // ** Disconnect **
		CloseGPRS(hGPRS);                                         // ** Close **
	}
	StopGPRS();                                                    // ** Stop **
	return KO ;
}

int Emettre_Frame_GPRS(char *sbuffer, unsigned short count) {
	char tcSnd[MAX_SND+1];
		int iRet=0,ret=-1;
		int nError;

		if(FlagSSLPRo_gprs==0)
		{
			ProfileHnd=SSL_LoadProfile(g_szProfile);
			FlagSSLPRo_gprs=1;
		}

	      /* Tests whether the files linked to the profile are loaded into the terminal.*/
			if(FLAG_Ref_SSL<0)
			{
		            int err;
		            err = SSL_New(&hSSL, ProfileHnd);
		            if(err != 0)
		            {
		            	if(FlagSSLPRo_gprs == 1)
		            		SSL_UnloadProfile( ProfileHnd );
		                return KO;
		            }
					/* connect to the created SSL socket */

		            ret=SSL_Connect(hSSL,FicPara.ip_gprs, atoi(FicPara.port_gprs), 2000 );

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
	            FLAG_Ref_SSL = 1;
	        }

			if( ret == 0 )
			{
				memset(tcSnd, 0, sizeof(tcSnd));
				memcpy(tcSnd,sbuffer,count);			//Remplacer17

				iRet= SSL_Write(hSSL,tcSnd,count,300);
				if(iRet<=0)
				{
					SSL_Disconnect(hSSL);				/* realease the SSL socket */
					SSL_UnloadProfile( ProfileHnd );
					SSL_Free( hSSL );
					FlagSSLPRo_gprs = 0;
					FLAG_Ref_SSL = -1;
					return KO;
				}
			}
			else
			{
	            ResolverSetOption ( RSLVO_SERV2_IPADDR , "8.8.8.8" ) ;
	            DNS_GetIpAddress (FicPara.nom_domaine,FicPara.ip_gprs, sizeof(FicPara.ip_gprs));

	            SSL_Disconnect(hSSL);
				SSL_Free(hSSL);
				FLAG_Ref_SSL = -1;

				return KO;
			}

		   return OK;
}

int Receive_Frame_GPRS(unsigned char *rbuffer) {


    char tcRsp[MAX_RSP+1];
    char tmp[10]="\0";
	int ret=0;


	memset(tcRsp, 0, sizeof(tcRsp));

	ret = SSL_Read(hSSL, tcRsp, sizeof(tcRsp), 6000);
	//ret = SSL_Read(hSSL, tcRsp, sizeof(tcRsp), FicPara.TimeOutReceive);
	if(ret < 0)		//Remplacer17
	{
//		Function_lblComKO(ret);


		SSL_Disconnect(hSSL);
		SSL_Free(hSSL);
		FLAG_Ref_SSL = -1;
		//Telium_Ttestall(0,200);
		return KO;
	}

    if (ret > MAX_RSP) strcpy (tcRsp, "Buffer overflow Max=512");  // Data overflow
	memcpy(rbuffer,tcRsp,ret);					//Remplacer17



	if(FLAG_Ref_SSL==2)
	{
		SSL_Disconnect(hSSL);
		SSL_Free(hSSL);
		FLAG_Ref_SSL = -1;
	}

	return ret;
}
void Deconnection_GPRS(void) {

	if (hSSL && FLAG_Ref_SSL > 0)
	{
//		DisconnectGPRS(hGPRS);                               // ** Disconnect **
		SSL_Disconnect(hSSL);
		SSL_Free(hSSL);
		FLAG_Ref_SSL = -1;
		hSSL = NULL;
//		CloseGPRS(hGPRS);                                         // ** Close **
	}

//	StopGPRS();                                                    // ** Stop **

}



//ajouter par lkeddouch
/*int PerformConnectionSSL(const char *szHostName, unsigned int nPort)

{
	char szTemp[100];
	// Load the profile used for the connection.

	hProfile = SSL_LoadProfile(g_szProfile);
	if (hProfile != NULL)
	{
		// The profile is loaded.
		//...
		// Init the connection handle from the loaded profile.
		//APEMV_UI_MessageDisplayDelay ((char*)"SSL 1", 2);
		if (SSL_New(&hSSL, hProfile) == 0)
		{
			// The connection handle is ready to be used.
			//...
			//APEMV_UI_MessageDisplayDelay ((char*)"SSL 2", 2);
			if (SSL_Connect(hSSL, szHostName, nPort,__20_SECONDS__) == 0)
			{
				// Connection success.
				//...
				// Send data
				//APEMV_UI_MessageDisplayDelay ((char*)"SSL 3", 2);
				SSL_Write(hSSL, "HELLO!", 6, __3_SECONDS__);
				// Receive data
				SSL_Read(hSSL, szTemp, 100, __3_SECONDS__);
				// Disconnect.
				SSL_Disconnect(hSSL);
				APEMV_UI_MessageDisplayDelay ((char*)"SSL 4", 2);
			}
			else
			{
				// Display the connection error.
				int nError;
				SSL_ProfileGetLastError(hProfile, &nError);
				//...
			}
			// Release the connection handle.
			SSL_Free(hSSL);
			APEMV_UI_MessageDisplayDelay ((char*)"SSL 5", 2);
			return 0;
		}
	}

	return -1;
}

*/
int CallPerformConnectionGPRS_TLS( const char *szHostName, unsigned int nPort)
 {
	char tmp[32];
	int ret;
	int nError;
	MessageWithoutConfirmation("Connexion en cours",
			"file://flash/HOST/attendez_svp.png");

	keepConnection(); // jrizki 290419

	if (FlagSSLPRo_gprs == 0) {
		ProfileHnd = SSL_LoadProfile(g_szProfile);
		FlagSSLPRo_gprs = 1;
//		Os__xprinta("Trace 1");
	}
	/* Tests whether the files linked to the profile are loaded into the terminal.*/
	{
		int err;
		err = SSL_New(&hSSL, ProfileHnd);

		if (err != 0) {
//			Os__xprinta("Trace 2");
			SSL_UnloadProfile(ProfileHnd);
			FlagSSLPRo_gprs = 0;
			return KO ;
		}


		ret = SSL_Connect(hSSL, FicPara.ip_gprs, atoi(FicPara.port_gprs), 2000);
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
//
		Os__xprintd(FreeSpace());
//		traceInt(ret, 0);
		if (ret == 0) {
//			Os__xprinta("Trace 3");
			FLAG_Ref_SSL = 1;
			return OK ;
		} else {
//			Os__xprinta("Trace 4");
			ResolverSetOption( RSLVO_SERV2_IPADDR, "8.8.8.8");
			DNS_GetIpAddress(FicPara.nom_domaine, FicPara.ip_gprs,
					sizeof(FicPara.ip_gprs));

			SSL_Disconnect(hSSL);
			SSL_UnloadProfile(ProfileHnd);
			SSL_Free(hSSL);

			FlagSSLPRo_gprs = 0;
			FLAG_Ref_SSL = -1;
			return KO ;
		}
	}
}

void keepConnection(void){

	char tcApn[lenGprsApn + 1];                 // Apn
	char tcUsername[lenGprsUser + 1];           // Login

	char tcPassword[lenGprsPass + 1];           // Password

	sprintf(tcApn, "%s", FicPara.apn_gprs);
	sprintf(tcUsername, "%s", FicPara.login_gprs);
	sprintf(tcPassword, "%s", FicPara.pwd_gprs);

	int connStatus = LL_GetStatus(hGPRS);

	if(connStatus ==  LL_STATUS_DISCONNECTED)
		LL_GPRS_Connect(tcApn, tcUsername, tcPassword, 10 * 100);

}

