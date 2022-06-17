
#include "OSL_Layer.h"


#define CHECK(CND,LBL) {if(!(CND)){goto LBL;}}
#define NUMBER_OF_ITEMS(a) (sizeof(a)/sizeof((a)[0]))
#define NUMBER_OF_LINES(a) (sizeof(a)/sizeof((a)[0]))


#define PARAM_DISK   "PARAMDISK"



// Data base keys
// ==============
enum
{
	appBeg=0,      // Application parameter keys (Record)
};

// Application Parameters keys
// ===========================
enum
{
	appCmpDat=appBeg,    // Compile date Mapapp.c     Oct 24 2011
	appCmpTim,           // Compile time Mapapp.c     01:12:49

	appSerialItem,       // Com Port, Data Bits, Parity, Stop Bits, Baud Rate
	appSerialInit,       //              8         N        1         115200

	appModemItem,        // Mdm Type, Correction,   , Data Bits, Parity,   ,Baud Rate
	appModemInit,        //               ?       T       8         N    1    115200
	appModemPabx,        // Pabx
	appModemPrefix,      // Prefix
	appModemCountry,     // Country code
	appModemPhone,       // Phone number

	appEthIpLocal,       // Local Ip Address
	appEthPort,          // Port number

	appGprsRequirePin,   // Pin required?
	appGprsApn,          // Apn
	appGprsUser,         // User name
	appGprsPass,         // Password
	appGprsIpRemote,     // Remote Ip Address
    appGprsPort,         // Port number

	appPppPabx,          // Pabx
	appPppPrefix,        // Prefix
	appPppCountry,       // Country code
	appPppPhone,         // Phone number
	appPppUser,          // User name
	appPppPass,          // Password
    appPppIpLocal,       // Local Ip Address
	appPppPort,          // Port number

	appEnd
};

// Parameters length
// =================
enum
{
    lenCmpDat = 11,
    lenCmpTim = 8,

	lenSerialItem = 5,
	lenSerialInit = 16,

	lenModemItem = 4,
	lenModemInit = 16,
	lenModemPabx = 4,
	lenModemPrefix = 4,
	lenModemCountry = 3,
	lenModemPhone = 16,

	lenEthIpLocal = 15,
	lenEthPort = 5,

	lenGprsRequirePin = 1,
	lenGprsPin = 8,
	lenGprsPuk = 8,
	lenGprsApn = 30,
	lenGprsUser = 30,
	lenGprsPass = 30,
	lenGprsIpRemote = 15,
	lenGprsPort = 5,

	lenPppPabx = 4,
	lenPppPrefix = 4,
	lenPppCountry = 3,
	lenPppPhone = 16,
	lenPppUser = 30,
	lenPppPass = 30,
	lenPppIpLocal = 15,
	lenPppPort = 5,

	lenAppEnd
};

// Mapapp.c
// ========
int appReset(void);
int appPut(word usKey, void *pvDat, word usLen);
int appGet(word usKey, void *pvDat, word usLen);
const char *getAppCmpDat(void); // See Mapapp.c
const char *getAppCmpTim(void);


// LL GPRS-3G Status
// =================
#define LL_STATUS_GPRS_NO_SIM                 LL_ERRORS+10
#define LL_STATUS_GPRS_ERR_SIM_LOCK           LL_ERRORS+9
#define LL_STATUS_GPRS_ERR_PPP                LL_ERRORS+8
#define LL_STATUS_GPRS_ERR_UNKNOWN            LL_ERRORS+7

// LL ETHERNET Status
// ==================
#define LL_STATUS_ETH_AVAILABLE               LL_ERRORS+20
#define LL_STATUS_ETH_NO_DEFAULT_ROUTE        LL_ERRORS+19
#define LL_STATUS_ETH_NOT_PLUGGED             LL_ERRORS+18
#define LL_STATUS_ETH_BASE_NOT_READY          LL_ERRORS+17
#define LL_STATUS_ETH_OUT_OF_BASE             LL_ERRORS+16

// LL PHYSICAL Status
// ==================
#define LL_STATUS_PHY_PERIPHERAL_BUSY         LL_ERRORS+30
#define LL_STATUS_PHY_PERIPHERAL_OUT_OF_BASE  LL_ERRORS+31
#define LL_STATUS_PHY_CONVERTER_NOT_PLUGGED   LL_ERRORS+32

// LL BLUETOOTH Status
// ===================
#define LL_STATUS_BT_NO_BASE_ASSOCIATED		  LL_ERRORS+40
#define LL_STATUS_BT_PERIPHERAL_NOT_AVAILABLE LL_ERRORS+41
#define LL_STATUS_BT_PERIPHERAL_BUSY		  LL_ERRORS+42
#define LL_STATUS_BT_BASE_NOT_AVAILABLE		  LL_ERRORS+43


void Delay(void);
long TimerStart(byte ucTimerNbr, long lDelay);
long TimerGet(byte ucTimerNbr);
int TimerStop(byte ucTimerNbr);
void Timer(void);

void DisplaySmall (byte ucLine, byte ucColumn, char *pcText);
void DisplayMedium (byte ucLine, byte ucColumn, char *pcText);
void DisplayLarge (byte ucLine, byte ucColumn, char *pcText);
int Display25(byte ucLine, char *pcData , int iLen);
int Print24(char *pcData, int iLen);
int ShowMenu(const char *pcTitle, int iMenuStyle, int iDefaultItem, int iMaxItems, const char *tpcItems[]);
int ShowEntry (const char *pcTitle, const char *pcText, char *pcInput, byte ucMinDigit, byte ucMaxDigit, byte ucEntryMode);

char* FMG_ErrorMsg (int iValue);
char* LL_ErrorMsg (int iValue);
