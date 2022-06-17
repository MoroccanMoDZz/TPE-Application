/**
* \file cu_entry.c
* \brief This module contains the entry point functions called by the Manager.
*
* \author Ingenico France
* \author Copyright (c) 2009 Ingenico France, 1 rue claude Chappe,\n
* 07503 Guilherand-Granges, France, All Rights Reserved.
*
* \author Ingenico France has intellectual property rights relating to the technology embodied\n
* in this software. In particular, and without limitation, these intellectual property rights may\n
* include one or more patents.\n
* This software is distributed under licenses restricting its use, copying, distribution, and\n
* and decompilation. No part of this software may be reproduced in any form by any means\n
* without prior written authorization of Ingenico France.
**/

/* ======================================================================== */
/* Include Files                                                            */
/* ======================================================================== */

#include "SDK_Naps.h"

#include "emv_sq.h"
#include "_emvdctag_.h"
#include "TagOS.h"
#include "MyTlvTree.h"
#include "EngineInterfaceLib.h"
//#include "oem_sysfioctl.h"
#include "del_lib.h"
#include "cu_str.h"
#include "EmvEngineInterface.h"
#include "cu_term.h"
#include "cu_batch.h"
#include "cu_black.h"
#include "cu_base.h"
#include "cu_mess.h"
#include "MessagesDefinitions.h"
#include "convert.h"
#include "cu_param.h"
#include "gestion_param.h"
#include "cu_str.h"
#include "cu_serv.h"
#include "param.h"
#include "cu_file.h"
#include "cu_disk.h"
#include "cu_pinen.h"
#include "cu_more.h"
#include "cu_mess.h"
#include "cu_entry.h"
#include "serveng.h"
#include "SEC_interface.h"
#include "Gpinpad.h"
#include "cu_receipt.h"
#include "cu_logos.h"

#include "Training.h"	//Training17
#include "VGE_FMG.H"	//Training17

#ifdef _USE_UCM_
#include "ucmtelium.h"
#include "ucmhostdll.h"
#include "ucmclib.h"
#endif

#include "sec_interface.h"
#include "TlvTreeDel.h"
#include "EngineInterface.h"


#include "TrackIac.h"
#include "GL_GraphicLib.h"
#include "GTL_Assert.h"
#include "GTL_Convert.h"

#include "EPSTOOL_Convert.h"
#include "EPSTOOL_TlvTree.h"
#include "EPSTOOL_PinEntry.h"
#include "EPSTOOL_Unicode.h"
#include "cu_UserInterfaceDisplay.h"

#include "Goal.h"


#ifdef _TRACK_IAC_DC_
#include "spy.h"
#endif

//! \addtogroup Group_cu_entry
//! @{


/* ======================================================================== */
/* Macros & preprocessor definitions                                        */
/* ======================================================================== */

/* =========================================================================*/
/* Types declarations                                                       */
/* =========================================================================*/

typedef int (*T_SERVICE_FUNCTION)(unsigned int nSize, void*Data);

typedef struct{
	unsigned char   priority;
	unsigned char   cardholder;
	unsigned char   codeTableIndex;
	unsigned int    lengthPreferredName;
	unsigned char   preferredName[16];
	unsigned int    lengthApplicationLabel;
	unsigned char   applicationLabel[16];
	unsigned char   lengthLanguagePreference;
	unsigned char   languagePreference[8];
	unsigned int    lengthAID;
	unsigned char   AIDValue[16] ;
	unsigned int    lengthPDOL;
	DataElement     * PDOL;
}AIDCandidate;

typedef struct{
	unsigned char    count;
	AIDCandidate    list[64];
}AIDCandidateList;

/* ======================================================================== */
/* Constant declarations                                                    */
/* ======================================================================== */
Telium_File_t *hPrn=NULL;        // Instance of the peripheral Printer 	//CashRegister17
Telium_File_t *hKbd=NULL;        // Instance of the peripheral Keyboard	//CashRegister17

/* ========================================================================	*/
/* Global variables                                                         */
/* ========================================================================	*/

#ifdef _USE_UCM_
#define TYPE_UCMC		(0x12)
#define SERVICE_UCMC	(0x1201)
int UCM_Present;
#endif

static char g_szAppName[32 + 1];
static NO_SEGMENT CustomNoAppli;
static unsigned short CUENT_IsFirst_Init;


#ifdef _TOOLWARE_
unsigned long			ulBegin_transaction;
extern unsigned long	TMT_Retrieve_Clock(void);
extern unsigned long    ulMMI_selappli;
extern unsigned long    ulMMI_langue;
#endif /* _TOOLWARE_ */

int isFallbackAllowed;

/*&BEGIN FT/09/4316/MA  Multi EMV Application*/
unsigned short CustApplicationNumber;
static  StructProcessStep _DELLocale;
static  DEL                DELLocale;
/*&END FT/09/4316/MA  Multi EMV Application*/

extern int TransactionLoopMode;
extern unsigned long loop_amount;


extern char buffer13[4000];

// Data needed to call the magstripe application. Declared as global
// parameters, because they does not fit in the stack.
static TRACK1_BUFFER bufIso1;
static TRACK2_BUFFER bufIso2;
static TRACK3_BUFFER bufIso3;
static S_TRANSIN trans_in;
static StructPt  EntryPoint;

static AIDCandidateList  outAIDCandidateList;
static unsigned char num_aid_selected ;

/* ========================================================================	*/
/* Static function definitions                                              */
/* ========================================================================	*/

#if 0
static void	Test_DLL_Security_Presence(void);
#endif
static void CUENT_First_Init (void);

#ifdef _TRACK_IAC_
void InitTrackIAC (void);
#endif

static int cu_Short_AID_List(byte* Tab,AIDCandidateList* aAIDCandidateList);
static int cu_AS_AffichageCardHolder(byte* TAB, AIDCandidateList *aAIDCandidateList);
static int cu_is_name(NO_SEGMENT noappli, void *param_in, S_ETATOUT *param_out);
/*&BEGIN FT/09/4316/MA  Multi EMV Application*/
static int IsdedicatedFile (S_FILE *param_in);
/*&END FT/09/4316/MA  Multi EMV Application*/
static void EMVCUST_Menu_Select_TlvTree(TLV_TREE_NODE *phInputTLVTree, TLV_TREE_NODE *phOutputTLVTree);


void keepConnection(void);

T_GL_HGRAPHIC_LIB gGoalGraphicLibInstance = NULL;

Telium_File_t *hDsp=NULL;

#define AMOUNT_ENTRY           TRUE //added 03052018 by jawad

#define SAP_TRACE_0  0xF100        // Initialize tracing with trace tool

//****************************************************************************
// Properties of the Idle screen (Goal)
// ====================================
static const ST_DSP_LINE txIdle[] =
{
	{ {GL_ALIGN_CENTER,   GL_ALIGN_CENTER, GL_COLOR_WHITE,  GL_COLOR_BLACK,   0, FALSE, {0, 0, 0, 0}, {0, 0, 0, 0, GL_COLOR_WHITE}, {0, 0, 0, 0}, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_LARGE}},  // Line1
	  {GL_ALIGN_CENTER,   GL_ALIGN_CENTER, FALSE,   0, FALSE, {0, 0, 0, 0}, {0, 0, 0, 0, GL_COLOR_BLACK}, {0, 0, 0, 0}, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_MEDIUM}} },
	{ {GL_ALIGN_CENTER,   GL_ALIGN_CENTER, GL_COLOR_WHITE,  GL_COLOR_RED,     0, FALSE, {8, 2, 8, 2}, {1, 1, 1, 1, GL_COLOR_WHITE}, {0, 0, 0, 0}, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_XLARGE}}, // Line2
	  {GL_ALIGN_CENTER,   GL_ALIGN_CENTER, TRUE,    0, FALSE, {8, 4, 8, 6}, {0, 0, 0, 0, GL_COLOR_BLACK}, {0, 0, 0, 0}, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_XLARGE}} },
	{ {GL_ALIGN_CENTER,   GL_ALIGN_CENTER, GL_COLOR_WHITE,  GL_COLOR_BLACK,   0, FALSE, {0, 0, 0, 0}, {0, 0, 0, 0, GL_COLOR_WHITE}, {0, 0, 0, 0}, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_XLARGE}}, // Line3
	  {GL_ALIGN_CENTER,   GL_ALIGN_CENTER, FALSE,   0, FALSE, {0, 0, 0, 0}, {0, 0, 0, 0, GL_COLOR_BLACK}, {0, 0, 0, 0}, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_MEDIUM}} }
};

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

// Properties of the default printer (Goal)
// ========================================
static const ST_PRN_LINE xPrinter =
{
	GL_ALIGN_LEFT, GL_ALIGN_CENTER, FALSE, 100, {0, 0, 0, 0}, {0, 0, 0, 0, GL_COLOR_BLACK}, {1, 0, 1, 0}, {"LUCIDA", GL_FONT_STYLE_NORMAL, GL_SCALE_LARGE}
};

// Properties of image (Goal)
// =======================================
static const ST_IMAGE xImage480 =
{
	{GL_COLOR_WHITE, TRUE, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_MEDIUM}, GL_DIRECTION_NONE, {0, 0, 0, 0, GL_COLOR_RED}, {70, 70, 70, 70} },
	{TRUE, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_MEDIUM}, GL_DIRECTION_NONE, {1, 1, 1, 1, GL_COLOR_BLACK}, {2, 1, 2, 1} }
};

static const ST_IMAGE xImage240 =
{
	{GL_COLOR_BLACK, TRUE, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_MEDIUM}, GL_DIRECTION_NONE, {0, 0, 0, 0, GL_COLOR_RED}, {20, 20, 20, 20} },
	{TRUE, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_MEDIUM}, GL_DIRECTION_NONE, {1, 1, 1, 1, GL_COLOR_BLACK}, {2, 1, 2, 1} }
};

static const ST_IMAGE xImage2_480 =
{
	{GL_COLOR_BLACK, NULL, {"LUCIDA", GL_FONT_STYLE_NORMAL, GL_SCALE_MEDIUM}, GL_DIRECTION_NONE, {0, 0, 0, 0, GL_COLOR_RED}, {100, 0, 100, 100} },
	{TRUE, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_MEDIUM}, GL_DIRECTION_NONE, {1, 1, 1, 1, GL_COLOR_BLACK}, {2, 1, 2, 1} }
};

static const ST_IMAGE xImage2_240 =
{
	{GL_COLOR_BLACK, NULL, {"LUCIDA", GL_FONT_STYLE_NORMAL, GL_SCALE_MEDIUM}, GL_DIRECTION_NONE, {0, 0, 0, 0, GL_COLOR_RED}, {50, 0, 50, 50} },
	{TRUE, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_MEDIUM}, GL_DIRECTION_NONE, {1, 1, 1, 1, GL_COLOR_BLACK}, {2, 1, 2, 1} }
};


#define MAX_IMG   48

// Animated image sequence
// =======================
const byte tcIdxImg[MAX_IMG] =
{
	 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	 0,  1,  2,  3,  4,  5,  6,  7,  8,  9,
	10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
	 9,  8,  7,  6,  5,  4,  3,  2,  1, 0
};
static word usIdxImg=0;       // Index of image
static word usTimeout=10;     // Timeout between two images


char dim[4]; // Screen dimension

#define LOGO_IDLE_ID 2
#define TIMER_ALTER_ID 3

#define SERVICES_LOW_PRIORITY       150 + 30 // For all services except idle_message,
#define SERVICES_HIGH_PRIORITY      150 + 10 //      priority => 150 highest and 255 lowest
#define IDLE_LOW_PRIORITY           150 + 10 // For idle_message,
#define IDLE_HIGH_PRIORITY          150 + 30 //      priority => 150 lowest and 255 highest
#define SERVICES_DEFAULT_PRIORITY   150 + 20

// Currency table according to ISO 4217
// ====================================
static const S_MONEY_EXTENDED txCurrency[] =
{
	{{{'M','A','D'}, {'5','0','4'}, 2}, {',', '.', CURRENCY_AFTER_AMOUNT}}, // Euro
	{{{'E','U','R'}, {'9','7','8'}, 2}, {',', '.', CURRENCY_AFTER_AMOUNT}}, // Euro
	{{{'G','B','P'}, {'8','2','6'}, 2}, {'.', ',', CURRENCY_AFTER_AMOUNT}}, // Pound Sterling
	{{{'U','S','D'}, {'8','4','0'}, 2}, {'.', ',', CURRENCY_AFTER_AMOUNT}}, // United State Dollar
	{{{'I','S','K'}, {'3','5','2'}, 0}, {',', '.', CURRENCY_AFTER_AMOUNT}}, // Icelandic Krona
	{{{'T','N','D'}, {'7','8','8'}, 3}, {',', '.', CURRENCY_AFTER_AMOUNT}}  // Tunisian Dinar
};

#ifdef _TRACE_REMOTE_              // Initialize tracing with remote debugger
	#define TRACE_DEBUG(BUF) {Telium_Systemfioctl(SYS_FIOCTL_DEBUG_TRACE, (char*) BUF);}
#else
	#define TRACE_DEBUG(BUF)
#endif

static const ST_BORDER xBorder = {0, 0, 0, 0, GL_COLOR_BLACK};

T_GL_HGRAPHIC_LIB hGoal=NULL;      // Handle of the graphics object library
static word idleTaskNbr;
static word afterResetTaskNbr;
t_topstack *alternateNbr = NULL;
t_topstack *autoConnectionTaskNbr = NULL;

int AlternanceIsOn = 0;
int connectionIsGoingOn = FALSE;
int GPRSDisabled = FALSE;


char* substring(const char* str, size_t begin, size_t len)
{
  if (str == 0 || strlen(str) == 0 || strlen(str) < begin || strlen(str) < (begin+len))
    return NULL;

  return strndup(str + begin, len);
}

/* ======================================================================== */
/* Functions                                                                */
/* ======================================================================== */
 int cu_file_received (NO_SEGMENT no_appli,S_FILE *param_in,void *param_out);



 static bool OnAlternateLogo(T_GL_HMESSAGE hMessage)
 {
	char source[100];
	char* source_logo_com = "file://flash/HOST/Logo-commercant.png";
	char* source_logo_naps = "file://flash/HOST/Logo-Naps.png";
	T_GL_HWIDGET logo_image = GL_Widget_SearchId(GL_Message_GetWindow(hMessage), LOGO_IDLE_ID);
	T_GL_HWIDGET timer_alter = GL_Widget_SearchId(GL_Message_GetWindow(hMessage), TIMER_ALTER_ID);

	// Get the displayed logo
	sprintf(source, "%s", GL_Widget_GetSource(logo_image));

	// Alternate logos
	if(strncmp(source, source_logo_naps, strlen(source_logo_naps)) == 0){
		GL_Widget_SetSource(logo_image, source_logo_com);
	}else{
		GL_Widget_SetSource(logo_image, source_logo_naps);
	}

	// Restart the timer
	GL_Timer_SetInterval(timer_alter, 5000);

	return true;

 }



static word AlternateLogos(){

	byte ucProduct;
	byte ucRange;
	// Return the product name
	ucRange = PSQ_Get_product_type(&ucProduct);

	T_GL_HWIDGET window = GL_Window_Create(APEMV_UI_GoalHandle());
	GL_Widget_SetBackColor(window, GL_COLOR_WHITE);


	tStatus usSta;
	usSta = Telium_SignalEvent(idleTaskNbr, 15);

	// Creates main layout
	T_GL_HWIDGET mainLayout = GL_Layout_Create(window);

	// create body
	T_GL_HWIDGET logoPicture = GL_Picture_Create(mainLayout);
	GL_Widget_SetSource(logoPicture, "file://flash/HOST/Logo-Naps.png");
	if (ucRange == TYPE_TERMINAL_M3500 || ucRange == TYPE_TERMINAL_M5000)
		GL_Widget_SetMargins(logoPicture, 0, 50, 0, 0, GL_UNIT_PIXEL); /// 50
	else if (ucRange == TYPE_TERMINAL_D3500)
		GL_Widget_SetMargins(logoPicture, 0, 20, 0, 0, GL_UNIT_PIXEL);
	GL_Widget_SetId(logoPicture, LOGO_IDLE_ID);
	GL_Widget_SetItem(logoPicture, 0, 0);

	////////////////
	T_GL_HWIDGET layoutForPicture = GL_Layout_Create(mainLayout);
	GL_Widget_SetItem(layoutForPicture, 0, 1);

	// Image
	T_GL_HWIDGET image_widget = GL_Picture_Create(layoutForPicture);
	GL_Widget_SetItem(image_widget, 0, 0);
	GL_Widget_SetSource(image_widget, "file://flash/HOST/Attentecarte.bmp");
	if (ucRange == TYPE_TERMINAL_M3500 || ucRange == TYPE_TERMINAL_M5000)
		GL_Widget_SetMaxSize(image_widget, 200, 200, GL_UNIT_PIXEL); //200
	else if (ucRange == TYPE_TERMINAL_D3500)
		GL_Widget_SetMaxSize(image_widget, 200, 200, GL_UNIT_PIXEL);
	GL_Widget_SetTransformation(image_widget, GL_TRANSFORMATION_FIT_ALL);
	GL_Widget_SetMargins(image_widget, 0, 10, 0, 13, GL_UNIT_PIXEL);


	// create footer
	T_GL_HWIDGET footer = GL_Layout_Create(mainLayout);
	GL_Widget_SetItem(footer, 0, 2);
	GL_Widget_SetShrink(footer, GL_DIRECTION_ALL);
	GL_Widget_SetExpand(footer, GL_DIRECTION_ALL);

	T_GL_HWIDGET menu = GL_Label_Create(footer);
	GL_Widget_SetItem(menu, 0, 0);
	GL_Widget_SetForeColor(menu, GL_COLOR_BLACK);
	GL_Widget_SetText(menu, "NAPS");
	GL_Widget_SetTextAlign(menu, GL_ALIGN_BOTTOM_LEFT);
	GL_Widget_SetShrink(menu, GL_DIRECTION_ALL);
	GL_Widget_SetExpand(menu, GL_DIRECTION_ALL);
	GL_Widget_SetMargins(menu, 0, 0, 0, 0, GL_UNIT_PIXEL);
	GL_Widget_SetFontSize(menu, 15, 15, GL_UNIT_PIXEL);

	T_GL_HWIDGET network = GL_Label_Create(footer);
	GL_Widget_SetItem(network, 2, 0);
	GL_Widget_SetForeColor(network, GL_COLOR_BLACK);
	GL_Widget_SetText(network, "ADMIN");
	GL_Widget_SetFontSize(network, 15, 15, GL_UNIT_PIXEL);
	GL_Widget_SetTextAlign(network, GL_ALIGN_BOTTOM_RIGHT);
	GL_Widget_SetMargins(network, 0, 0, 0, 0, GL_UNIT_PIXEL);
	GL_Widget_SetShrink(network, GL_DIRECTION_ALL);
	GL_Widget_SetExpand(network, GL_DIRECTION_ALL);

	if(!memcmp(FicPara.TYPEAPPLICATION,"NAPSSNTL",8))
	{
		T_GL_HWIDGET sntl = GL_Label_Create(footer);
		GL_Widget_SetItem(sntl, 1, 0);
		GL_Widget_SetForeColor(sntl, GL_COLOR_BLACK);
		GL_Widget_SetText(sntl, "SNTL");
		GL_Widget_SetTextAlign(sntl, GL_ALIGN_BOTTOM | GL_ALIGN_CENTER);
		GL_Widget_SetShrink(sntl, GL_DIRECTION_ALL);
		GL_Widget_SetExpand(sntl, GL_DIRECTION_ALL);
		GL_Widget_SetMargins(sntl, 0, 0, 0, 0, GL_UNIT_PIXEL);
		GL_Widget_SetFontSize(sntl, 15, 15, GL_UNIT_PIXEL);
	}

	T_GL_HWIDGET timer = GL_Timer_Create(window);

	GL_Timer_SetInterval(timer, 3000);

	// Registering a callback. This callback will be called every second
	GL_Widget_RegisterCallback(timer, GL_EVENT_TIMER_OUT, OnAlternateLogo);

	GL_Window_MainLoop(window);

	GL_Widget_Destroy(window);

	return 0;

}


static word autoConnectionTask() {

	tStatus usSta;
	usSta = Telium_SignalEvent(afterResetTaskNbr, 15);

	while(1) {
		if(!connectionIsGoingOn && !GPRSDisabled)
			keepConnection();
	}

//	unsigned char heure[4];
//	unsigned char heur[10];
//	DATE date;
//	uchar status;
//	uchar date_appel[7];
//
//
//	tStatus usSta;
//	usSta = Telium_SignalEvent(afterResetTaskNbr, 15);
//
////	Os__xprinta("Starting task");
//	while (1) {
////		Os__xprinta("   ");
//		Telium_Read_date(&date);
//		sprintf(heur, "%.2s:%.2s:%.2s", date.hour, date.minute, date.second);
//		sprintf(heure, "%.2s%.2s", date.hour, date.minute);
//		if (!memcmp(heure, FicPara.Heure_appel, 4)) {
//		Os__xprinta("Heure_appel");
//			if (AlternanceIsOn) {
//				Telium_SignalEvent(Telium_GiveNoTask(alternateNbr), 15);
//
//				Telium_Ttestall(0, 30);
//
//				Telium_Kill(alternateNbr, "");
//
//				AlternanceIsOn = 0;
//			}
//
//			MessageWithDelay("Lancement Telecollect ...", NULL, 2000);
//			status = Fin_jour(1, 0);
//			if (status == RECO_KO) {
//				PutAllTra();
//				PutAllParam();
//				hang_up(typehangup, g_trace);
//				Os__xprinta("Lancer la telecollecte");
//			} else {
//				ImprimeTelecollecte(0, status);
//				Efface_Trs();
//				gest_param();
//				Telium_Read_date(&date);
//				sprintf(date_appel, "%.2s%.2s%.2s", date.day, date.month,
//						date.year);
//				Os__saved_copy(&date_appel[0], &FicPara.Date_appel[0], 6);
//				set_ushort_secourue(&FicPara.compteur_appel, 0);
//			}
//			PutAllTra();
//			PutAllParam();
////			break;
//
//			Telium_Ttestall(00, 30000);
//		}
//	}
//	Os__xprinta("End task");
	return 0;
}





  //===========================================================================
  //! \brief This function displays the idle message. It is called on manager
  //! entry point IDLE_MESSAGE.
  //! \param[in] no : application number.
  //! \param[in] null1 : data received from manager. (not used).
  //! \param[out] null2 : data sent to manager. (not used).
  //===========================================================================
static int cu_idle_message(NO_SEGMENT no, void *null1, void*null2) {

	// Local variables
	// ***************
	int lg_code;

	byte dum1;
	tStatus usSta;
	tEventList usEve;

	// memclr(mess_on_display, 33);

	// Header activation
	// ??MF InitContexteGraphique(PERIPH_DISPLAY);
	//StateHeader (1);
	//SetPatchAppli(_PERIPH_LEDS,_PERIPH_ON);

	// Reset the selected mark list
	PAR_ResetMarkList();

	// Select Main ICS + ICSx (Maybe unused)
	CUPAR_SelectAdditionnalICS();

	// Get the manager language
	lg_code = My_PSQ_Give_Language();

	//pc_l_line = CUMESS_Read_Message(STANDARD_TABLE, STD_MESS_INSERT_CARD, lg_code);

	//CUTERMdisplayLine(0, pc_l_line, LEFT, (char *)"w");

	GetAllTra();
	GetAllPre();
	GetAllParam();

//	Os__xprinta("IDLE");

	if ((!AlternanceIsOn && FicPara.vCam0 == 0)
			|| (FicPara.vCam0 != 2 && !AlternanceIsOn)) { // prevent Alternance to work until end of CLESS 'FicPara.vCam0 != 2' // jrizki

		//Os__xprinta("Alternance TASK ON");

		idleTaskNbr = Telium_CurrentTask();

		alternateNbr = Telium_Fork(AlternateLogos, &dum1, 0); // Launch task for alternate logos jrizki 030419

		usSta = Telium_WaitEvents(0x00008000, 0, &usEve);
		usSta = Telium_ClearEvents(0x00008000);

		AlternanceIsOn = 1;
	}

	
	return FCT_OK;
}




//===========================================================================
//! \brief This function is used to perform periodic treatments. It is called
//! each minute, on manager entry point TIME_FUNCTION. The code to be executed
//! periodically will be called on manager entry point IS_TIME_FUNCTION.
//! \param[in] no : application number.
//! \param[in] null1 : data received from manager. (not used).
//! \param[out] null2 : data sent to manager. (not used).
//===========================================================================
static void cu_time_function(NO_SEGMENT no, void *null1 ,void*null2)
{
	//Telium_Kill(alternateNbr, "");
	//keepConnection();

	//Os__xprintaa("Time_function");

	TLV_TREE_NODE hOutputTree;
	DATE date;

	hOutputTree = TlvTree_New(0);
	DATE date_c;
	uchar date_appel_c[7];

	Telium_Read_date(&date);
	uchar		Date_YYMMDDHHmmss2[7];
	sprintf(Date_YYMMDDHHmmss2,"%.2s%.2s%.2s",date.day, date.month, date.year);
	//Os__xprinta("telecollecte auto 1");

	if(hOutputTree != NULL)
	{
		//EMVCUST_Periodic_Call_TLVTree(&hOutputTree);

		unsigned char heure[4];
		unsigned char heur[10];
		DATE date;

		uchar status ;
		uchar date_appel[7];

		if(!memcmp(heure,FicPara.Heure_appel,4))
		{
			Telium_Kill(alternateNbr, "");
			Telium_Ttestall(00, 1000);
			AlternanceIsOn = FALSE;

			keepConnection();


			MessageWithoutConfirmation("Lancement Telecollect ...", NULL);
			status = Fin_jour(1,0);
			if((status == TEL_OK) || (status == RECO_OK))
			//if(status != RECO_KO)
			{
				ExploiField7();
				if (FicTra.nombre_tra == 0)
					Imprime_ticket_telezero();
				else
					ImprimeTelecollecte(0,status);

				Efface_Trs();
				gest_param();
				Telium_Read_date(&date);
				sprintf ((char *)date_appel,
					"%.2s%.2s%.2s",
					date.day,
					date.month,
					date.year);
				Os__saved_copy( &date_appel[0], &FicPara.Date_appel[0],6);
				set_ushort_secourue((Ushort *)&FicPara.compteur_appel,0);

				//FicPara.Top_TelAuto = 1;
			}
			else if(status == RECO_KO)
			{
				if (FicTra.nombre_tra == 0)
					Imprime_ticket_telezero();
				else
				{
					Imprime_ticket_teleechou();
					//FicPara.Top_TelAuto = 0;
				}
			}
			else
			{
				Imprime_ticket_teleechou();
				FicPara.Top_TelAuto = 0;
			}


			//initialisation_cmi();//référencement automatique


//			status = Fin_jour(1,0);
//			if(status == RECO_KO)
//			{
//				PutAllTra();
//				PutAllParam();
//				hang_up(typehangup,g_trace);
//			}
//			else
//			{
//				ImprimeTelecollecte(0,status);
//				Efface_Trs();
//				gest_param();
//				Telium_Read_date(&date);
//				sprintf (date_appel,
//					"%.2s%.2s%.2s",
//					date.day,
//					date.month,
//					date.year);
//				Os__saved_copy( &date_appel[0], &FicPara.Date_appel[0],6);
//				set_ushort_secourue(&FicPara.compteur_appel,0);
//			}



			PutAllTra();
			PutAllParam();
		}
//		else if ((memcmp(heure, FicPara.Heure_appel, 4) > 0) && (FicPara.Top_TelAuto == 1))
//		{
//			FicPara.Top_TelAuto = 0;
//		}

		// release local TLV tree
		TlvTree_ReleaseEx(&hOutputTree);
	}



	Telium_Read_date(&date_c);
	sprintf ((char *)date_appel_c,		"%.2s%.2s%.2s",		date_c.day,		date_c.month,		date_c.year);

//	Os__xprinta(FicPara.Date_appel);
//	Os__xprinta(date_appel_c);


//	if (memcmp(FicPara.Date_appel, date_appel_c, 6) < 0)
//		FicPara.Top_TelAuto = 0;

	//Os__xprinta(FicPara.Top_TelAuto);

}

//===========================================================================
//! \brief This function builds a _DEL_ containing the list of the AID managed
//! by the application.
//! \param[in] AppliNum : application number.
//! \param[in] param_in : data received from manager. (not used).
//! \param[out] delout : _DEL_ containing the list of AID managed by the application.
//===========================================================================
static void cu_give_aid(NO_SEGMENT AppliNum, S_TRANSIN *param_in,   _DEL_ *delout)
{
	DataElement	eltExt;
	DataElement Elt;
	unsigned char ucDispList;
	unsigned short cr;
	TLV_TREE_NODE hInputTreeSG;
	TLV_TREE_NODE hInputTree, hOutputTree, hTree;


	hInputTreeSG = TlvTree_New(0);


	if(hInputTreeSG != NULL)
	{
		// Reset the selected mark list
		PAR_ResetMarkList();

		// Select Main ICS + ICSx (Maybe unused)
		CUPAR_SelectAdditionnalICS();

		cr = PAR_ReadParameter(TAG_USE_PSE, (DataElementExt *)&eltExt);
		if (cr)
		{
			TlvTree_AddChild(hInputTreeSG, eltExt.tag, eltExt.ptValue, eltExt.length);
		}
		cr = PAR_ReadParameter(TAG_WAIT_AFTER_ATR, (DataElementExt *)&eltExt);
		if (cr)
		{
			TlvTree_AddChild(hInputTreeSG, eltExt.tag, eltExt.ptValue, eltExt.length);
		}
		cr = PAR_ReadParameter(TAG_COMPATIBILITY_2000, (DataElementExt *)&eltExt);
		if (cr)
		{
			TlvTree_AddChild(hInputTreeSG, eltExt.tag, eltExt.ptValue, eltExt.length);
		}
		cr = PAR_ReadParameter(TAG_WAIT_AFTER_ATR, (DataElementExt *)&eltExt);
		if (cr)
		{
			TlvTree_AddChild(hInputTreeSG, eltExt.tag, eltExt.ptValue, eltExt.length);
		}

		Engine_SetGlobalParameters(hInputTreeSG);

		// release local TLV tree
		TlvTree_ReleaseEx(&hInputTreeSG);

	}

	hOutputTree = TlvTree_New(0);

	if(hOutputTree != NULL)
	{
		EMVCUST_Get_AID_TlvTree(&hOutputTree);

		// Build an input TLV Tree
		hInputTree = TlvTree_New(0);

		if(hInputTree != NULL)
		{
			TlvTree_AddChild(hInputTree, TAG_TERM_APP_NUMBER, (unsigned char*) &AppliNum, 1);

			hTree = TlvTree_GetFirstChild(hOutputTree);

			ucDispList = FALSE;

			while(hTree != NULL)
			{
				Elt.tag = TlvTree_GetTag(hTree);
				Elt.length = TlvTree_GetLength(hTree);
				Elt.ptValue = TlvTree_GetData(hTree);



				if (Elt.tag == TAG_GIVE_AID_NB_AID)
				{
					TlvTree_AddChild(hInputTree, TAG_GIVE_AID_NB_AID, Elt.ptValue, 1);
				}


				else if (Elt.tag ==TAG_GIVE_AID_MANUAL_SELECTION)
				{
					ucDispList = Elt.ptValue [0];
				}

				else if (Elt.tag == TAG_GIVE_AID_AID_VALUE)
				{
					TlvTree_AddChild(hInputTree, Elt.tag, Elt.ptValue, Elt.length);

					// Get the next node
					hTree = TlvTree_GetNext(hTree);
					Elt.tag = TlvTree_GetTag(hTree);
					Elt.length = TlvTree_GetLength(hTree);
					Elt.ptValue = TlvTree_GetData(hTree);

					if (Elt.tag == TAG_GIVE_AID_ASI)
					{
						TlvTree_AddChild(hInputTree, Elt.tag, Elt.ptValue, Elt.length);
					}

					TlvTree_AddChild(hInputTree, TAG_GIVE_AID_MANUAL_SELECTION, (unsigned char*) &ucDispList, 1);
				}

				// Get the next node
				hTree = TlvTree_GetNext(hTree);
			}

			// Copy the input TLV tree to output _DEL_ object
			AddTlvTreeToDelValue(delout, hInputTree);

			// release local TLV tree
			TlvTree_ReleaseEx(&hInputTree);
		}

		// release local TLV tree
		TlvTree_ReleaseEx(&hOutputTree);
	}
}

//===========================================================================
//! \brief This function tests if the EMV transaction can be run.
//! \param[in] AppliNum : application number.
//! \param[in] pParamIn : data received from manager. (not used).
//! \param[out] pParamOut : structure containing the response :
//! - if the field cardappnumber is != from 0 then custom application wants to
//! process the EMV card. In this case, the fields cardapp.priority and cardapp.cardappname
//! have to be fulfilled.
//! - if the field cardappnumber = 0 then custom application doesn't wants to process the EMV card.
//===========================================================================
static void cu_is_card_emv_for_you(NO_SEGMENT AppliNum, const S_AID* pParamIn, S_CARDOUT* pParamOut)
{
	(void)pParamIn;


	// Initialise the output parameter
	memset(&pParamOut->returned_state[pParamOut->response_number], 0,
		sizeof(pParamOut->returned_state[pParamOut->response_number]));

	// Get the application name
	strcpy(pParamOut->returned_state[pParamOut->response_number].appname, g_szAppName);

	pParamOut->returned_state[pParamOut->response_number].no_appli = AppliNum;
	pParamOut->returned_state[pParamOut->response_number].cardappnumber = 1;
	// Set the application name and the priority
	pParamOut->returned_state[pParamOut->response_number].cardapp[0].priority = 30;

	strcpy (pParamOut->returned_state[pParamOut->response_number].cardapp [0].cardappname, g_szAppName);

	pParamOut->response_number++;
}

//===========================================================================
//! \brief This function allows the user to execute custom menu functions.
//! \param[in] AppliNum : application number.
//! \param[in] null1 : data received from manager. (not used).
//! \param[out] null2 : data sent to manager. (not used).
//===========================================================================
static void cu_more_function(NO_SEGMENT AppliNum, void *null1 ,void*null2)
{
	TLV_TREE_NODE hOutputTree;
	NO_SEGMENT No;

	DisplayHeader (_OFF_);

	hOutputTree = NULL;
	//badr problème d'affichage des accents
	T_GL_HGRAPHIC_LIB hGoal_fr=APEMV_UI_GoalHandle();
	T_GL_HWIDGET hScreen_fr=NULL;

	hScreen_fr = GoalCreateScreen(hGoal_fr, thScreen, NUMBER_OF_LINES(thScreen), GL_ENCODING_ISO_8859_1);
	if (hScreen_fr)
		GoalDestroyScreen(&hScreen_fr);
	// EMVCUST_More_Function_TLVTree(&hOutputTree);
	more_function(No, NULL, NULL);
	// release local TLV tree
	TlvTree_ReleaseEx(&hOutputTree);

	DisplayHeader (_ON_);

#ifdef _TLVTREE_DEBUG_
	TlvTree_DumpAllocatedTrees();
#endif
}

//===========================================================================
//! \brief This function allow custom application to capture the key pressed.
//! It is called at each key pressed when manager is idle.
//! \param[in] no : application number.
//! \param[in] key1 : data received from manager. (not used).
//! \param[out] key2 : data sent to manager. (not used).
//===========================================================================
static void cu_keyboard_event(NO_SEGMENT no, S_KEY *key1, S_KEY *key2)
{
	unsigned char OutKey;

	OutKey = (unsigned char)(key1->keycode);
	EMVCUST_KeyManagement_TLVTree(key1->keycode, &OutKey);
	key2->keycode = OutKey;
}

//===========================================================================
//! \brief This function allows custom application to perform a service call
//! to engine application.
//! \param[in] function : engine function number.
//! \param[in] aDEL : input DEL to be transmitted to engine.
//! \param[out] ret_code : service call return code.
//===========================================================================
DEL *ENGINE_ServiceCall( int function,const DEL *aDEL, int* ret_code)
{
	unsigned char priority;

	*ret_code = PAY_KO;
	if (Telium_ServiceGet(I_EMVENG_Type_Appli,function,&priority)==0)
	{
		if (aDEL != NULL)
			CopyEntree((DEL *)aDEL,&_DELLocale.DEL);
		else _DEL_init(&_DELLocale.DEL);
		_DELLocale.type_code = I_EMVENG_Type_Appli;
		_DELLocale.service   = function;
		_DELLocale.function = function;
		Telium_ServiceCall(I_EMVENG_Type_Appli,function,sizeof(_DELLocale),&_DELLocale,ret_code);
		CopySortie(&_DELLocale.DEL,&DELLocale);
		return(&DELLocale);
	}
	else
		return(NULL);
}

//===========================================================================
//! \brief This function performs a debit transaction on an EMV chip card.
//! \param[in] AppliNum : application number.
//! \param[in] param_in : DEL from manager. This DEL contains mandatory parameters
//! selected AID, application label. And may contain optionally parameters
//! application preferred name, language preference, issuer code table index,
//! application priority indicator.
//! \param[out] param_out : structure containing the payment return code :
//! - PAY_OK : if the payment successfully performed.
//! - PAY_KO : otherwise.
//===========================================================================
void cu_debit_emv(NO_SEGMENT AppliNum, S_TRANSIN *param_in,   S_TRANSOUT *param_out)
{
	DataElement	AnElt;
	unsigned char EngAppliSel;
	unsigned char TabAmount [4];
	unsigned char TypeAppli[2];
#ifdef _USE_UCM_
	T_UCMHOST_DEBIT sDebit;
	T_UCMHOST_R_DEBIT sRDeb;
#endif
	TLV_TREE_NODE hInputTree, hTree;
	TLV_TREE_NODE hOutputTree;
	int iret, i;
	char cPinBooster;
	char * pt_PinPeriph;
	int nNbSecurePartsConfig = -1;
	T_SEC_CONFIG tabConfParts[10];
	int bFound;
	int previousUiState;





	cPinBooster = 0;
	pt_PinPeriph=NULL;
#ifdef _USE_UCM_
    if (!UCM_Present)
    {
#endif

	if (param_in->peri != 0)
	{
		// If CAM0 is not used, specify that CARD and PIN are on the terminal (CAM2 management)
		iret = SEC_GetConfig (&nNbSecurePartsConfig, tabConfParts);
		if (iret == OK)
		{
			bFound = FALSE;
			for (i=0; i < nNbSecurePartsConfig; i++)
			{
				if (tabConfParts[i].SecureType == C_SEC_CIPHERING)
				{
					pt_PinPeriph = tabConfParts [i].ptszBoosterPeripheral;
					cPinBooster = tabConfParts [i].cBoosterType;
					bFound = TRUE;
					break;
				}
			}
			if (bFound)
			{
				bFound = FALSE;
				for (i=0; i < nNbSecurePartsConfig; i++)
				{
					if (tabConfParts[i].SecureType == C_SEC_CARD)
					{
						tabConfParts [i].ptszBoosterPeripheral = pt_PinPeriph;
						tabConfParts [i].cBoosterType = cPinBooster;
						bFound = TRUE;
						break; /* to go out the for loop */
					}
				}

				if (bFound)
					iret = SEC_SetConfig (nNbSecurePartsConfig, tabConfParts);
			}
		}
	}
#ifdef _USE_UCM_
	}
#endif

	memset(param_out, 0, sizeof(*param_out));
	param_out->noappli = AppliNum;

	CreateTlvTreeFromDel(&hInputTree, (_DEL_ *)&param_in->del);

	hTree = TlvTree_GetFirstChild(hInputTree);

	while (hTree != NULL)
	{

		AnElt.tag = TlvTree_GetTag(hTree);

		if (AnElt.tag == TAG_GIVE_AID_AID_VALUE)
		{
			TlvTree_SetTag(hTree, TAG_AID_ICC);
		}
		else if (AnElt.tag == TAG_GIVE_AID_APP_NAME)
		{
			TlvTree_SetTag(hTree, TAG_APPLICATION_LABEL);
		}
		else if (AnElt.tag == TAG_GIVE_AID_APP_PRIO)
		{
			TlvTree_SetTag(hTree, TAG_APPLI_PRIOR_IND);
		}
		else if (AnElt.tag == TAG_GIVE_AID_PREF_LANGUAGE)
		{
			TlvTree_SetTag(hTree, TAG_LANGUAGE_PREFERENCE);
		}

		// Get the next node
		hTree = TlvTree_GetNext(hTree);
	}

	ShortToChar(CustApplicationNumber,TypeAppli);

	TlvTree_AddChild(hInputTree, TAG_ENG_CUSTOM_APPLI_TYPE, (unsigned char*) &TypeAppli, 2);

	EngAppliSel =1;
	TlvTree_AddChild(hInputTree, TAG_ENG_APPLI_SEL_ALREADY_DONE, (unsigned char*) & EngAppliSel, 1);

	TlvTree_AddChild(hInputTree, TAG_TRANSACTION_CURRENCY_CODE, (unsigned char*) &param_in->currency.code, 2);

	TlvTree_AddChild(hInputTree, TAG_TRANSACTION_CURRENCY_EXP, (unsigned char*) &param_in->currency.posdec, 1);

	LongToChar (param_in->amount, TabAmount);
	TlvTree_AddChild(hInputTree, TAG_AMOUNT_AUTH_BIN, (unsigned char *)&TabAmount, 4);

//	LongToChar (0, TabAmount);
//	TlvTree_AddChild(hInputTree, TAG_AMOUNT_OTHER_BIN, TabAmount, 4);

	hOutputTree = NULL;
#ifdef _USE_UCM_
	if(UCM_Present)
	{
		memset ( &sRDeb, 0x00, sizeof ( T_UCMHOST_R_DEBIT ) ) ;
		memset ( &sDebit, 0x00, sizeof ( T_UCMHOST_DEBIT ) ) ;

		sDebit.ucFunction = DEBIT_EMV;
		switch(iLIBUCM_Pay_Ready_For_Debit(sizeof(T_UCMHOST_DEBIT), &sDebit))
		{
		case UCMCPAY_SOLV:
			switch(Engine_DoTransaction(hInputTree,&hOutputTree))
			{
			case ERR_ENG_OK:
				param_out->rc_payment = PAY_OK;

				// TODO: Set the correct status (UCMHOST_CR_REFUSED, UCMHOST_CR_CARDFORBIDEN, ...)
				sRDeb.ucCr = UCMHOST_CR_OK;
				break;
			case ERR_ENG_TRY_ANOTHER_AID:
				param_out->rc_payment = PAY_OTHER_AID;
				sRDeb.ucCr = UCMHOST_CR_APPLI_ASK_NEW_SELECTION;
				break;
			default:
				param_out->rc_payment = PAY_KO;
				sRDeb.ucCr = UCMHOST_CR_CARDINVALIDE;
				break;
			}

			sRDeb.ucFunction   = UCMHOST_FCT_SOLV ;
			sRDeb.usAppName    = AppliNum ;
			strcpy ( sRDeb.tAppLibelle, g_szAppName ) ;

			sRDeb.ulAmount = sDebit.ulAmount;
			memcpy( &sRDeb.tCurrency, &sDebit.tCurrency, sizeof( S_MONEY));

			iLIBUCM_Pay_Result_Debit(sizeof(T_UCMHOST_R_DEBIT), &sRDeb);
			iLIBUCM_Pay_End(0, NULL);
			break;

		case UCMCPAY_RECORD:
			// TODO: Store the transaction here !

			param_out->rc_payment = PAY_OK;
			sRDeb.ucCr = UCMHOST_CR_OK;

			sRDeb.ucFunction   = UCMHOST_FCT_ENREG ;
			sRDeb.usAppName    = AppliNum ;
			strcpy ( sRDeb.tAppLibelle, g_szAppName ) ;

			sRDeb.ulAmount = sDebit.ulAmount;
			memcpy( &sRDeb.tCurrency, &sDebit.tCurrency, sizeof( S_MONEY));

			iLIBUCM_Pay_Result_Debit(sizeof(T_UCMHOST_R_DEBIT), &sRDeb);
			iLIBUCM_Pay_End(0, NULL);
			break;

		default:
			param_out->rc_payment = PAY_KO;
			break;
		}
	}
	else
	{
#endif
		if(FicPara.MenuTPE[0]!='1')
			{
			CUMORE_CMI_AFFICHE_MESSAGE(22,3,1000);//operation non permise*/ 110719
			return 0;}

		ulTransactionType = CUSERV_DEBIT_TRANSACTION;   //19062019

		if(memcmp(FicPara.MCC,"6010",4) == 0)   //27062019
		{
			Os__xprinta("CashAdvance 1.0");
			ulTransactionType = CUSERV_CASH_TRANSACTION;//eee
		}
		//ulTransactionType = CUSERV_CASH_TRANSACTION;

		memset(&E_Transac8583,0x00,sizeof(E_Transac8583));
		memset(&R_Transac8583,0x00,sizeof(R_Transac8583));

		memcpy(StructInfoPayment.Devise_acc.code_num,"504",3);
		memcpy(StructInfoPayment.Devise_acc.code_alph,"MAD",3);
		StructInfoPayment.Devise_acc.Exposant = 2;

		// Setup the user interface environment
		previousUiState = APEMV_UI_TransactionBegin();

		switch(Engine_DoTransaction(hInputTree,&hOutputTree))
		{
		case ERR_ENG_OK:
			param_out->rc_payment = PAY_OK;
			break;
		case ERR_ENG_TRY_ANOTHER_AID:
			param_out->rc_payment = PAY_OTHER_AID;
			break;
		default:
			param_out->rc_payment = PAY_KO;
			break;

		}
		// Restore the user interface environment
		APEMV_UI_TransactionEnd(previousUiState);
#ifdef _USE_UCM_
	}
#endif

#ifdef _USE_UCM_
    if (!UCM_Present)
    {
#endif
		SEC_SetConfig (0, 0);
#ifdef _USE_UCM_
	}
#endif

	// release local TLV tree
	TlvTree_ReleaseEx(&hInputTree);
	TlvTree_ReleaseEx(&hOutputTree);

#ifdef _TLVTREE_DEBUG_
	TlvTree_DumpAllocatedTrees();
#endif


}
/*&END FT/09/4316/MA  Multi EMV Application*/

//===========================================================================
//! \brief This function treats the engine entry points managed
//! by the custom application.
//! \param[in] size : size of the exchanged data.
//! \param[in,out] data : input/output structure used for the exchange between
//! engine and custom.
//! \return 0.
//===========================================================================
int Main_EMVCUST_TlvTree(unsigned int size, I_CUST_TLVTREE_SERVICE *data)
{
	int CR_Tree_Conv = TLV_TREE_OK;
	TLV_TREE_NODE hInputTlvTree=NULL;
	TLV_TREE_NODE hOutputTlvTree;
	int function;

	function = 0;

	// Deserialization of TLV tree
	CR_Tree_Conv = TlvTree_Unserialize(&hInputTlvTree, TLV_TREE_SERIALIZER_DEFAULT, data->m_Buffer, sizeof(data->m_Buffer));
	if((CR_Tree_Conv == TLV_TREE_OK)) //  || (CR_Tree_Conv == TLV_TREE_ERROR_INVALID_BUFFER))
	{
		// Deserialization ok
		// Create the output TlvTree
		hOutputTlvTree=TlvTree_New(0);
		if (hOutputTlvTree != NULL)
		{
			switch(data->m_Header.m_usService)
			{
			case I_EMVCUST_Get_Global_Param_TlvTree :
				CUPAR_UpdateGlobalParam();
				EMVCUST_Get_Global_Param_TlvTree(&hOutputTlvTree);
				break;

			case I_EMVCUST_Get_AID_List_TlvTree :
				EMVCUST_Get_AID_List_TlvTree(&hOutputTlvTree);
				break;

			case I_EMVCUST_Get_AID_DATA_TlvTree :
				EMVCUST_Get_AID_DATA_TlvTree(&hInputTlvTree, &hOutputTlvTree);
				break;

			case I_EMVCUST_Get_AID_Param_TlvTree :
				EMVCUST_Get_AID_Param_TlvTree(&hInputTlvTree, &hOutputTlvTree);
				break;

			case I_EMVCUST_Process_Step_TlvTree :
				function = data->m_Header.m_nFunction;
				EMVCUST_Process_Step_TlvTree (function, &hInputTlvTree, &hOutputTlvTree);
				break;

			case I_EMVCUST_Display_Message_TlvTree :
				EMVCUST_Display_Message_TlvTree(&hInputTlvTree, &hOutputTlvTree);
				break;

			case I_EMVCUST_Get_Last_Transaction_TlvTree :
				EMVCUST_Get_Last_Transaction_TlvTree(&hInputTlvTree, &hOutputTlvTree);
				break;

			case I_EMVCUST_Voice_Referral_TlvTree :
				EMVCUST_Voice_Referral_TlvTree(&hInputTlvTree, &hOutputTlvTree);
				break;

			case I_EMVCUST_Authorization_TlvTree :
				EMVCUST_Authorization_TlvTree (&hOutputTlvTree);
				break;

			case I_EMVCUST_Choose_Language_Menu_TlvTree :
				EMVCUST_Choose_Language_Menu_TlvTree (&hOutputTlvTree);
				break;

			case I_EMVCUST_Menu_Select_TlvTree :
				EMVCUST_Menu_Select_TlvTree (&hInputTlvTree, &hOutputTlvTree);
				break;

			case I_EMVCUST_Choose_Account_Type_Menu_TlvTree :
				EMVCUST_Choose_Account_Type_Menu_TlvTree (&hOutputTlvTree);
				break;

			case I_EMVCUST_Get_AID_ICS_TlvTree :
				EMVCUST_Get_AID_ICS_TlvTree(&hInputTlvTree, &hOutputTlvTree);
				break;

			default:
				break;
			}

			// Debug
			// if (data->m_Header.m_usService == I_EMVCUST_Get_Last_Transaction_TlvTree) // && (function == EMVDC_STOP))
			// if ((data->m_Header.m_usService == I_EMVCUST_Display_Message_TlvTree))  //  || (data->m_Header.m_usService == I_EMVCUST_KeyManagement_TlvTree))
			// TraceTLVTree(data->m_Header.m_usService, &hOutputTlvTree);
			// TraceTLVTreeToDEL(hOutputTlvTree);

			// Serialization of output TLV tree
			CR_Tree_Conv = TlvTree_Serialize(hOutputTlvTree, TLV_TREE_SERIALIZER_DEFAULT, data->m_Buffer, sizeof(data->m_Buffer));
			if(CR_Tree_Conv < TLV_TREE_OK)
			{
				memset(data->m_Buffer, 0, sizeof(data->m_Buffer));
			}

			// Destruction of output TLV tree
		//	a  revoir !!!!!TlvTree_ReleaseEx(&hOutputTlvTree);

		}
		else
		{
			memset(data->m_Buffer, 0, sizeof(data->m_Buffer));
		}

		// Destruction of input TLV tree
		// a revoir !!!!!!TlvTree_ReleaseEx(&hInputTlvTree);
	}
	else
	{
		memset(data->m_Buffer, 0, sizeof(data->m_Buffer));
	}


	TlvTree_ReleaseEx(&hInputTlvTree);

	TlvTree_ReleaseEx(&hOutputTlvTree);


	#ifdef _TLVTREE_DEBUG_
	if ((data->m_Header.m_usService==I_EMVCUST_Process_Step_TlvTree)&&(function==EMVDC_STOP))
		TlvTree_DumpAllocatedTrees();
	#endif



//	isFallbackAllowed = CUSTR_CheckFallback();


	return 0;
}

int WarmReset()
{
	char FilePath_L[80];
	char LabelDiskName[DISK_NAME_SIZE];
	int iRet;

	iRet = CUDISK_CreateDisk();
	if (iRet == FS_OK)
	{
		memclr(LabelDiskName, sizeof(LabelDiskName));
		CUDISK_Get_Disk_Name(LabelDiskName);

		sprintf(FilePath_L, "/%s/%s", LabelDiskName, C_FILE_PARAM);
		return  (FS_exist (FilePath_L)==FS_OK);
	}
	return 0;
}

int ParamReset() //Training17
{
	// Parameter disk configuration
	// ============================
	static S_FS_PARAM_CREATE xCfg;     // Parameter disk configuration
	static unsigned long ulSize;
	char tcAppDat[lenCmpDat+1], tcAppTim[lenCmpTim+1];
	char tcPath[100];
    doubleword uiMode;  // Bug in prototype => 2nd parameter (output) on FS_mount returns Mode (FS_WRITEONCE or FS_WRITEMANY)
                        // and NOT AccessMode (FS_NOFLAGS or FS_RONLYMOD or FS_WRTMOD)
	int iRet;
	
	// ** Parameters Disk used by FMG library ** 
	strcpy(xCfg.Label, PARAM_DISK);              // Disk name
    xCfg.Mode = FS_WRITEONCE;                    // Disk on Flash
	xCfg.AccessMode	= FS_WRTMOD;                 // r/w access
	xCfg.NbFichierMax = 1;                       // Max files number
	xCfg.IdentZone	= FS_WO_ZONE_DATA;           // Zone id
	ulSize = xCfg.NbFichierMax*32768;            // Disk size in bytes
    
	// ** All disks created by FFMS and used by FMG library must be mounted before the call to FMG_init() **
	sprintf(tcPath, "/%s", PARAM_DISK);
	iRet = FS_mount (tcPath, &uiMode);           // Check if disk already activated (!!! second parameter returns (FS_WRITEONCE or FS_WRITEMANY))
    if (iRet != FS_OK)
    {
    	// == Application added ==
    	iRet = FS_dskcreate(&xCfg, &ulSize);     // Create and format the none volatile disk
    	CHECK(iRet==FS_OK, lblKO);
        iRet = FS_mount(tcPath, &uiMode);        // Activate a disk (!!! second parameter returns (FS_WRITEONCE or FS_WRITEMANY))
        CHECK(iRet==FS_OK, lblKO);
        iRet = FMG_Init();                       // Initialize File ManaGement
        CHECK(iRet==FMG_SUCCESS, lblKO);

        iRet = appReset();                       // Reset application parameters (Flash)
        CHECK(iRet>=0, lblKO);

		iRet = appPut(appCmpDat, (char*)getAppCmpDat(), lenCmpDat);  // Store compiler date/time (See Mapapp.c)
		CHECK(iRet>=0, lblKO);
	    iRet = appPut(appCmpTim, (char*)getAppCmpTim(), lenCmpTim);
	    CHECK(iRet>=0, lblKO);

	    _clrscr();
		
		//////APEMV_UI_MessageDisplayDelay ((char *)"New Software.0\nReset Done", 1);
		//Remplacer17_DrawExtendedString(0, 20, (char*) zNewSoft, _OFF_, _XLARGE_, _PROPORTIONNEL_);
		//static const char zNewSoft[] = "New Software\nReset Done";
	    //Telium_Ttestall(0, 3*100);               // Wait 3s
    }
    else
    {

		iRet = FMG_Init();                       // Initialize File ManaGement
    	CHECK(iRet==FMG_INIT_OK, lblKO);

    	iRet = appGet(appCmpDat, tcAppDat, lenCmpDat+1);  // Retrieve compiler date/time (See Mapapp.c)
    	CHECK(iRet>=0, lblKO);
    	iRet = appGet(appCmpTim, tcAppTim, lenCmpTim+1);
    	CHECK(iRet>=0, lblKO);
                                                          // Check compiler date/time
    	if( (strcmp(tcAppDat, getAppCmpDat()) == 0) && (strcmp(tcAppTim, getAppCmpTim()) == 0))
    	{
    		// == Application already upto date ==
    	}
        else
        {
        	// == Application updated ==
            if ((strcmp(tcAppDat, getAppCmpDat()) != 0) || (strcmp(tcAppTim, getAppCmpTim()) != 0))
            {
            	iRet = appReset();                // Reset application parameters (Flash)
            	CHECK(iRet>=0, lblKO);
            }

    		iRet = appPut(appCmpDat, (char*)getAppCmpDat(), lenCmpDat); // Store compiler date/time (See Mapapp.c)
    		CHECK(iRet>=0, lblKO);
    	    iRet = appPut(appCmpTim, (char*)getAppCmpTim(), lenCmpTim);
    	    CHECK(iRet>=0, lblKO);

    	    _clrscr();
    		//////APEMV_UI_MessageDisplayDelay ((char *)"Update Software.2\nUpdate Done", 1);
			//Remplacer17_DrawExtendedString(0, 20, (char*) zUpdateSoft, _OFF_, _XLARGE_, _PROPORTIONNEL_);
			//static const char zUpdateSoft[] = "Update Software\nUpdate Done";
    	    //Telium_Ttestall(0, 3*100);           // Wait 3s
        }
    }
lblKO :
	_clrscr();
	iRet = appReset();                           // Reset one more time
	if (iRet>=0)                                 // Memory OK
	{
		appPut(appCmpDat, (char*)getAppCmpDat(), lenCmpDat); // Store compiler date/time (See Mapapp.c)
	    appPut(appCmpTim, (char*)getAppCmpTim(), lenCmpTim);

		//////APEMV_UI_MessageDisplayDelay ((char *)"Update Software.3\nUpdate Done", 1);
		//Remplacer17_DrawExtendedString(0, 20, (char*) zInitSoft, _OFF_, _XLARGE_, _PROPORTIONNEL_);
		//static const char zUpdateSoft[] = "Update Software\nUpdate Done";
	}
	else                                         // Memory problem => Terminal problem
	{
		//////APEMV_UI_MessageDisplayDelay ((char *)"Processing Error\nMemory Failed", 1);
	}
		//Remplacer17_DrawExtendedString(0, 20, (char*) zMemFailed, _OFF_, _XLARGE_, _PROPORTIONNEL_);
		//static const char zMemFailed[] = "Processing Error\nMemory Failed";
	//Telium_Ttestall(0, 3*100);                   // Wait 3s
	goto lblEnd;
lblEnd:

	return 0;
}

//===========================================================================
//! \brief This function performs the initialisation of the CUSTOM component.
//! \param[in] noappli : application number.
//! \param[in] param_in : not used.
//! \param[out] param_out : structure containing the field noappli updated with
//! the application number.
//! \return Always FCT_OK.
//===========================================================================
static int cu_after_reset (NO_SEGMENT noappli, void *param_in, S_TRANSOUT *param_out)
{
	S_TRANSOUT   paramseq;
#ifndef __TELIUM3__
	unsigned char  chgt;
	unsigned char  type;
#endif
//	Telium_Date_t       date;
	int iRet;
	byte dum1;
	tStatus usSta;
	tEventList usEve;
	byte ucProduct;
	byte ucRange;
	// Return the product name
	ucRange = PSQ_Get_product_type(&ucProduct);
	uchar		Date_YYMMDDHHmmss2[7];
	UC	        heure[4+1];
	DATE date;
	char version[50];

#ifdef _USE_UCM_
	int bIsUcm;
	object_descriptor_t UcmcDescriptor;
	unsigned char ucPriority;
#endif

	//problème d'affichage des accents

	T_GL_HGRAPHIC_LIB hGoal_fr=APEMV_UI_GoalHandle();
	T_GL_HWIDGET hScreen_fr=NULL;

	hScreen_fr = GoalCreateScreen(hGoal_fr, thScreen, NUMBER_OF_LINES(thScreen), GL_ENCODING_ISO_8859_1);
	if (hScreen_fr)
		GoalDestroyScreen(&hScreen_fr);



//	memcpy(data_time, StructInfoPayment.Transaction_DateTime, 12);
//	sprintf(heure,"%.2s%.2s",data_time + 6,data_time + 8);

	Telium_Read_date(&date);
	sprintf(heure,"%.2s%.2s",date.hour,date.minute);
	sprintf(Date_YYMMDDHHmmss2,"%.2s%.2s%.2s",date.day, date.month, date.year);

//	Os__xprinta(heure);
//	Os__xprinta(Date_YYMMDDHHmmss2);
	// DLL INIT IS REQUIRED BEFORE USE.
	SEClib_Open();
	// Restore global variables stored in RAM
	FILE_Save_Restore_global_RAM_data ();

	// Messages Initialisation
	CUMESS_Initialise();
	/*&BEGIN FT/09/4316/MA  Multi EMV Application*/
	CustApplicationNumber = ApplicationGetCurrentAppliType();
	// sprintf(g_szAppName, "APP EMV %04X", CustApplicationNumber);
	sprintf(g_szAppName, "EMV_%04X", CustApplicationNumber);
	/*&END FT/09/4316/MA  Multi EMV Application*/
#ifdef _USE_UCM_

	UCM_Present = FALSE;

	// Check if running in CAD30 configuration
	bIsUcm = PSQ_Is_Ucm_Behaviour();
	if (bIsUcm < 0)
	{
		// PSQ_Is_Ucm_Behaviour is not yet implemented => simply checks the hardware type
		if (PSQ_Get_hardware_type() == TYPE_TERMINAL_CAD30)
		{
			bIsUcm = TRUE;
		}
		else
		{
			bIsUcm = FALSE;
		}
	}

	if (bIsUcm)
	{
		// We are running on a CAD30

		// Check if UCMC is loaded or not
		if (ObjectGetDescriptor(OBJECT_TYPE_APPLI, TYPE_UCMC, &UcmcDescriptor) == 0)
		{
			// A component 0x12 is loaded, but is it UCMC ?

			if (Telium_ServiceGet(TYPE_UCMC, SERVICE_UCMC, &ucPriority) == 0)
			{
				// UCMC is loaded ! (functions iLIBUCM_xxx are available)
				UCM_Present = TRUE;
			}
		}
	}

#endif
	CustomNoAppli = noappli;
	nb_time_select =0;

	Telium_Read_date(&date);
	if (param_out != NULL)
		memcpy (&paramseq, param_out, sizeof (paramseq));
	CUPAR_Update_Kernel_and_System_Infos();
	isPinpad = B_NON_INIT;

	/*&BEGIN FT/08/3682/NP tests presence and level of security DLL */
	// TODO: a remettre
	//?? a remettre Test_DLL_Security_Presence();
	/*&END FT/08/3682/NP tests presence and level of security DLL */

#ifndef __TELIUM3__
	// check for first run : cold or warm reset ?
	////APEMV_UI_MessageDisplayDelay ("#ifndef __TELIUM3__", 2);

	if (first_init (noappli,(unsigned char*)&chgt,(unsigned char*)&type) != FSE_FAILED)
	{
		////APEMV_UI_MessageDisplayDelay ("first_init !=FSE_FAILED", 2);
		if (chgt == (FLAG_CHGT)CHGT_OUI )
		{
			////APEMV_UI_MessageDisplayDelay ("chgt == (FLAG_CHGT)CHGT_OUI", 2);

			// cold reset
			raz_init(noappli);
//			CUMORE_Enter_Terminal_ID();
			CUENT_IsFirst_Init = TRUE;

			// Make general initalisation
			iRet = CUDISK_CreateDisk();
			if (iRet == FS_OK)
			{
				CUENT_First_Init ();
			}

			// This function call the PAR_ReadArray() function
			CUPAR_InitDefFiles (FALSE);

			CUENT_IsFirst_Init = FALSE;

		}
	}
		else
#endif
//#ifdef __TELIUM3__
			////APEMV_UI_MessageDisplayDelay ("#ifdef __TELIUM3__", 2);
		if (!WarmReset())
		{
			////APEMV_UI_MessageDisplayDelay ("!WarmReset", 1);
			// Make general initalisation
			iRet = CUDISK_CreateDisk();
			if (iRet == FS_OK)
			{
				CUENT_First_Init ();
			}

			// This function call the PAR_ReadArray() function
			CUPAR_InitDefFiles (FALSE);
		}
	else
//#endif
		{
			iRet = CUDISK_CreateDisk();
			if (iRet == FS_OK)
			{
				CUENT_First_Init ();
			}

			// This function call the PAR_ReadArray() function
			CUPAR_InitDefFiles (FALSE);

			////APEMV_UI_MessageDisplayDelay ("WarmReset", 2);

			char *ptrDummy = 0;

			// Warm reset
			CUENT_IsFirst_Init = FALSE;
			// Initialise TLVTree
			PAR_InitParam(C_PARAM_INIT_APLI, ptrDummy);
			iRet = CUDISK_CreateDisk();
			if (iRet == FS_OK)
			{
				// CUBATCH_RestoreLog();
				CUPAR_RestoreParam (C_MGT_PARAM_PAR_FILE_FIRST);
			}
			else
			{
				upload_diagnostic_txt((unsigned char *)"DISK CREATE HS\n");
			}
		}

	iRet = CUDISK_CreateDisk();
	if (iRet == FS_OK)
	{
		// Test consistency of the batch file
		CUBATCH_RestoreLog();
		// Unmount the volume
		CUDISK_UnmountDisk();
	}

	paramseq.noappli = noappli;
	if (param_out != NULL)
		memcpy (param_out, &paramseq, sizeof (paramseq));

#ifdef _TOOLWARE_
	// Check if the kernel and the system support TOOLWARE
	CUPAR_Init_ToolWare();
#endif /* _TOOLWARE_ */
/*	after_reset_done = TRUE;
}*/
	
	ParamReset();						//Test17
	
	ParFile();
	PreFile();
	TraFile();
	LopFile();


	Os__saved_copy((PUC)"0729",(uchar *)&FicPara.Version_appli,4);//VersionV
//	FicPara.g_type_connexion = 'I';		//Test17
	memset(version,0,sizeof(version));
	sprintf(version,"Version contact %.04s",FicPara.Version_appli);//badr
	Os__xprintVersion(version);
	//Os__xprintaa(Date_YYMMDDHHmmss2);
	//Os__xprintaa(heure);
	if (ucRange == TYPE_TERMINAL_M3500 ){
		FicPara.g_type_connexion = 'G';
	}

	else if (ucRange == TYPE_TERMINAL_M5000) {
		FicPara.g_type_connexion = 'G';
	}
	else
		FicPara.g_type_connexion = 'I';


	if(FicPara.g_type_connexion == 'G'){
		//Os__xprintaa("111");
		StartGprs_naps();
		//Os__xprintaa("222");
	}

//	FicPara.g_type_connexion = 'G';
//	StartGprs_naps();
	hKbd = Telium_Fopen("KEYBOARD", "r*");

//	afterResetTaskNbr = Telium_CurrentTask();
//
//	autoConnectionTaskNbr = Telium_Fork(autoConnectionTask, &dum1, 0);
//
//	usSta = Telium_WaitEvents(0x00008000, 0, &usEve);
//	usSta = Telium_ClearEvents(0x00008000);

//	Telium_Ttestall(00, 50);



/*
	memcpy(FicPara.Num_point_vente,"7575750",7);	//Test17
	memcpy(FicPara.Num_Terminal,"00000001",8);		//Test17

	memcpy(FicPara.ip_gprs,"192.10.2.53",11);		//Test17
	memcpy(FicPara.port_gprs,"6002",4);				//Test17
*/
	
	//memcpy(FicPara.Version_appli,"0505",4);
	FicPara.vCam0 = 0; //Retour à la normale
	PutAllParam();
	ssllib_open();		//Kamalssl
	AddProfileSLL();	//Kamalssl
/*	Os__xprinta(FicPara.Date_appel);
	Os__xprinta(FicPara.Heure_appel);
	Os__xprinta(FicPara.Top_TelAuto);
//*/
//	if (memcmp(FicPara.Date_appel, Date_YYMMDDHHmmss2, 6) != 0) {
//		Os__xprinta("condition verifiée");
//	}
//	if (FicPara.Top_TelAuto == 0) {
//		Os__xprinta("condition verifiée 2");
//	}
//
//	if (memcmp(heure, FicPara.Heure_appel, 4) > 0) {
//	}
	//FicPara.Top_TelAuto = 0;

//	Os__xprintaa("FicPara.Date_appel");
//	Os__xprintaa(FicPara.Date_appel);
//	Os__xprintaa("Date_YYMMDDHHmmss2");
//	Os__xprintaa(Date_YYMMDDHHmmss2);
//	Os__xprintaa("FicPara.Heure_appel");
//	Os__xprintaa(FicPara.Heure_appel);
//	Os__xprintaa("heure");
//	Os__xprintaa(heure);
	//FicPara.Top_TelAuto = 0;
	if ((memcmp(FicPara.Date_appel, Date_YYMMDDHHmmss2, 6) != 0) && (memcmp(heure, FicPara.Heure_appel, 4) > 0))
	{
		if (FicTra.nombre_tra == 0) {
			Imprime_ticket_telezero();
			memcpy(FicPara.Date_appel,Date_YYMMDDHHmmss2,6);
			PutAllParam();
		}
		else
		{
			Imprime_ticket_teleAutoechou();
			memcpy(FicPara.Date_appel,Date_YYMMDDHHmmss2,6);
		}
	//	FicPara.Top_TelAuto = 2;
	}

	if (FicPara.Top_TelAuto == 1)//badr
		{
		initialisation_cmi();//référencement automatique aprés un chargement a distance
		FicPara.Top_TelAuto = 0;
//		PutAllTra();
		PutAllParam();
		}

	//memcpy(FicPara.Heure_appel,"1030", 4);//belhadeg
	//memcpy(FicPara.Date_appel,"091219", 4);

	return (FCT_OK);
}

//===========================================================================
//! \brief This function tests the presence of security DLL. And if it has
//! at least a release 0204.
//===========================================================================
#if 0
static void	Test_DLL_Security_Presence(void)
{
	int sec_ver;
	int ret;
	Telium_File_t * printer;


	ret = ObjectLoad (OBJECT_TYPE_DLL, "SECURITY");
	if ((ret != 0) && (ret != OL_ALREADY_LOADED))
	{
#ifdef _USE_UCM_
		if(!UCM_Present)
		{
#endif
			printer=Telium_Fopen("PRINTER","w-");
			Telium_Pprintf ("\n""Custom \n");
			Telium_Pprintf ("Please LOAD DLL SECURITY\n");

			Telium_Pprintf ("\n\n\n\n\n");
			Telium_Ttestall(PRINTER,0);
			Telium_Fclose(printer);
#ifdef _USE_UCM_
		}
#endif
	}
	else
	{
		sec_ver = SEC_Version();
		if (sec_ver<0x020400)
		{
#ifdef _USE_UCM_
			if(!UCM_Present)
			{
#endif
				printer=Telium_Fopen("PRINTER","w-");
				Telium_Pprintf ("\n""Custom \n");
				Telium_Pprintf ("\n""Please LOAD DLL SECURITY\nWith VERSION >= 0204");

				Telium_Pprintf ("\n\n\n\n\n");
				Telium_Ttestall(PRINTER,0);
				Telium_Fclose(printer);
#ifdef _USE_UCM_
			}
#endif
		}
	}
}
#endif

//===========================================================================
//! \brief This function prints the application info and status.
//! \param[in] noappli : application number.
//! \param[in] param_in : not used.
//! \param[out] param_out : not used.
//! \return Always FCT_OK.
//===========================================================================
static int cu_state (NO_SEGMENT noappli,void *param_in,void *param_out)
{
	Telium_Date_t    date;
	unsigned char text[30];
	object_info_t info;

	CUPRINT_Open_Printer();

	sprintf ((char *) text, "\x1b""EEMV CUSTOM""\x1b""F");
	CUPRINT_String_XT(text, ALIGN_LEFT, L1);
	Telium_Read_date (&date);
	//???
	sprintf ((char *) text, "%.2s/%.2s/%.2s  %.2s:%.2s", date.day, date.month, date.year, date.hour, date.minute);
	CUPRINT_String_XT(text, ALIGN_LEFT, L1);

	ObjectGetInfo(OBJECT_TYPE_APPLI, ApplicationGetCurrent(), 	&info);

	sprintf ((char *) text, "Vers. : %s",info.name);
	CUPRINT_String_XT(text, ALIGN_LEFT, L1);

	sprintf ((char *) text, "CRC   : %04x",info.crc);
	CUPRINT_String_XT(text, ALIGN_LEFT, L1);

	CUPRINT_Paper_Feed();
	CUPRINT_Close_Printer();

	return (FCT_OK);

}

//===========================================================================
//! \brief This function gives the application state.
//! \param[in] noappli : application number.
//! \param[in] param_in : not used.
//! \param[out] param_out : structure S_ETATOUT containing the fields appname
//! and no_appli updated.
//! \return Always FCT_OK.
//===========================================================================
static int cu_is_state (NO_SEGMENT noappli,void *param_in,S_ETATOUT *param_out)
{
	S_ETATOUT etatout;
	int iRet ;

	memcpy (&etatout, param_out, sizeof(etatout));

	etatout.returned_state[etatout.response_number].state.response = REP_OK ;

	memcpy (param_out, &etatout, sizeof (etatout));
	iRet = cu_is_name (noappli, PT_NULL, param_out);

	return (iRet);
}

//===========================================================================
//! \brief This function prints a ticket with the total amount in the batch
//! data file.
//! \param[in] noappli : application number.
//! \param[in] param_in : not used.
//! \param[out] param_out : not used.
//! \return Always FCT_OK.
//===========================================================================
static int cu_consult (NO_SEGMENT noappli,void *param_in, void *param_out)
{
	int merchLang;
	unsigned long nb_record;
	unsigned long total_amount;
	T_Bool    ret;
	Telium_Date_t date;
	char str_line [80];

	// Retreive Terminal merchant language
	merchLang = My_PSQ_Give_Language();

	// Read Batch status
	ret = CUBATCH_GiveStatistics (&nb_record,
		&total_amount);

	if (ret)
	{
		// Print the Consult Receipt, using merchant terminal language
		CUPRINT_Open_Printer();

		CUPRINT_String_XT((const unsigned char*)CUMESS_Read_Message(CNS_MESS_EMV_CONSULT, merchLang), ALIGN_LEFT, L1);

		// Read and print current date
		Telium_Read_date(&date);
		//???
		sprintf (str_line,
			"%.2s/%.2s/%.2s  %.2s:%.2s\n",
			date.day,
			date.month,
			date.year,
			date.hour,
			date.minute);

		CUPRINT_String_XT((const unsigned char*)str_line, ALIGN_LEFT, L1);

		// Print the number of tx
		sprintf (str_line,
			//                 "%s : %d",
			"%s : %lu",
			CUMESS_Read_Message(CNS_MESS_NB_OF_TX, merchLang),
			nb_record);
		CUPRINT_String_XT((const unsigned char*)str_line, ALIGN_LEFT, L1);

		/* Print the total amount                                          */
		sprintf (str_line,
			//                 "%s : %d",
			"%s : %lu",
			CUMESS_Read_Message(CNS_MESS_TOTAL, merchLang),
			total_amount);
		CUPRINT_String_XT((const unsigned char*)str_line, ALIGN_LEFT, L1);

		/* Ending the print receipt correctely */
		CUPRINT_Paper_Feed();
		CUPRINT_Close_Printer();	}

	return (FCT_OK);
}

//===========================================================================
//! \brief This function allows the manager to modify all the parameters of
//! the application.
//! \param[in] noappli : application number.
//! \param[in] param_in : not used.
//! \param[out] param_out : structure S_ETATOUT containing the fields mask, appname
//! and no_appli updated.
//! \return Always FCT_OK.
//===========================================================================
static int cu_is_change_init(NO_SEGMENT noappli, void *param_in, S_ETATOUT *param_out)
{
	// Internal data declaration
	S_ETATOUT     etatout;

	// Allow the manager to modify all the parameters : reset the whole mask
	memcpy(&etatout, param_out, sizeof(etatout));
	etatout.returned_state[etatout.response_number].state.mask = 0;

	memcpy (param_out, &etatout, sizeof (etatout));

	// Call the manager service cu_is_name which will give the application name
	// and number and then increment the number of answers
	cu_is_name(noappli, PT_NULL, param_out);

	return(FCT_OK);
}

//===========================================================================
//! \brief This function informs the manager that the application has a
//! periodic function to be executed.
//! \param[in] noappli : application number.
//! \param[in] param_in : not used.
//! \param[out] param_out : structure S_ETATOUT containing the fields response, appname,
//! no_appli, response updated.
//! \return Always FCT_OK.
//===========================================================================
int is_time_function(NO_SEGMENT noappli, void *param_in, S_ETATOUT *param_out)
{
	//	//Os__xprinta("hello 1111");
	//	// Internal data declaration
	//	S_ETATOUT etatout;
	//
	//	memcpy(&etatout, param_out, sizeof(etatout));
	//
	//	etatout.returned_state[etatout.response_number].state.response = REP_KO;
	//
	//	memcpy (param_out, &etatout, sizeof (etatout));
	//
	//	// Call the manager service cu_is_name which will give the application name
	//	// and number and then increment the number of answers
	//	cu_is_name(noappli, PT_NULL, param_out);
	//
	//	return(FCT_OK);


		//Telium_Kill(alternateNbr, "");
			//keepConnection();


			TLV_TREE_NODE hOutputTree;
			DATE date;

			hOutputTree = TlvTree_New(0);
			DATE date_c;
			uchar date_appel_c[7];

			Telium_Read_date(&date);
			uchar		Date_YYMMDDHHmmss2[7];
			sprintf(Date_YYMMDDHHmmss2,"%.2s%.2s%.2s",date.day, date.month, date.year);
			HWCNF_SetStandbyDelay(86400);

			//badr problème d'affichage des accents
			T_GL_HGRAPHIC_LIB hGoal_fr=APEMV_UI_GoalHandle();
			T_GL_HWIDGET hScreen_fr=NULL;

			hScreen_fr = GoalCreateScreen(hGoal_fr, thScreen, NUMBER_OF_LINES(thScreen), GL_ENCODING_ISO_8859_1);
			if (hScreen_fr)
				GoalDestroyScreen(&hScreen_fr);
			if(hOutputTree != NULL)
			{
				//EMVCUST_Periodic_Call_TLVTree(&hOutputTree);

				unsigned char heure[4];
				unsigned char heur[10];
				DATE date;

				uchar status ;
				uchar date_appel[7];

				Telium_Read_date(&date);
				sprintf(heur,"%.2s:%.2s:%.2s",date.hour,date.minute,date.second);
				//Os__xprinta(heur);
				sprintf(heure,"%.2s%.2s",date.hour,date.minute);
				//Os__xprinta(heure);

				if(!memcmp(heure,FicPara.Heure_appel,4))
				{
					Telium_Kill(alternateNbr, "");
					Telium_Ttestall(00, 1000);
					AlternanceIsOn = FALSE;

					keepConnection();


					MessageWithoutConfirmation("Lancement Telecollect ...", NULL);
					//Os__xprintaa("0000000");

					status = Fin_jour(1,0);
					if((status == TEL_OK) || (status == RECO_OK))
					//if(status != RECO_KO)
					{
						ExploiField7();
						if (FicTra.nombre_tra == 0)
							Imprime_ticket_telezero();
						else
							ImprimeTelecollecte(0,status);

						Efface_Trs();
						gest_param();
						Telium_Read_date(&date);
						sprintf ((char *)date_appel,
							"%.2s%.2s%.2s",
							date.day,
							date.month,
							date.year);
						Os__saved_copy( &date_appel[0], &FicPara.Date_appel[0],6);
						set_ushort_secourue((Ushort *)&FicPara.compteur_appel,0);

						//FicPara.Top_TelAuto = 1;
					}
					else if(status == RECO_KO)
					{
						if (FicTra.nombre_tra == 0)
							Imprime_ticket_telezero();
						else
						{
							Imprime_ticket_teleechou();
							//FicPara.Top_TelAuto = 0;
						}
					}
					else
					{
						Imprime_ticket_teleechou();
						//FicPara.Top_TelAuto = 0;
					}

//
					startTMSDownload();
					Telium_Ttestall(0, 15 * 100);
					initialisation_cmi();//référencement automatique


					PutAllTra();
					PutAllParam();
				}
		//		else if ((memcmp(heure, FicPara.Heure_appel, 4) > 0) && (FicPara.Top_TelAuto == 1))
		//		{
		//			FicPara.Top_TelAuto = 0;
		//		}

				// release local TLV tree
				TlvTree_ReleaseEx(&hOutputTree);
			}



			Telium_Read_date(&date_c);
			sprintf ((char *)date_appel_c,		"%.2s%.2s%.2s",		date_c.day,		date_c.month,		date_c.year);

		//	Os__xprinta(FicPara.Date_appel);
		//	Os__xprinta(date_appel_c);


//			if (memcmp(FicPara.Date_appel, date_appel_c, 6) < 0)
//				FicPara.Top_TelAuto = 0;

			//Os__xprinta(FicPara.Top_TelAuto);


}

/*

 cust_mess5 overload manager treatment for MESS5

 --> DISPLAY is closed.

 --> Current context is PERIPH_DISPLAY

 */

void cust_mess5(InfosMSG_CUST *infoscust)
{

//	APEMV_UI_MessageDisplayDelay ("My Custom message", 1);
	APEMV_UI_InfoDisplay("My Custom message");

}

/*

 ServiceCustom_messages gives an example on how to customize a existing message
 1st case : overload manager
 If MESS5 has to be called by the Manager, then cust_mess5( ) will be called and
 you will see "INITIALIZE THE SAMPLE" on the terminal screen.
 2nd case : disable a message, MESS152 for exemple.
 For all other messages, calling
 infoscust->cr_treatment=NO_CUSTOMIZED_MESSAGE;
 there will be no modifcation.
 All the messages are defined into english.h emebed into the SDK/inc directory of the SDK.
 Regarding REMOVE CARD PLEASE , the message is (MESS6 or MES262)
 */

void ServiceCustom_messages(int size, InfosMSG_CUST *infoscust)

{
	char buff[10];

	//APEMV_UI_InfoDisplay("ServiceCustom_messages");
	switch (infoscust->num) {
		case 257:
			//Os__xprintaa("000");
			infoscust->cr_treatment = CUSTOMIZED_MESSAGE;
			MessageWithoutConfirmation("Veuillez patienter SVP", "file://flash/HOST/attendez_svp.png");
			break;
		case STD_MESS_PLEASE_WAIT:
			//Os__xprintaa("111");
			infoscust->cr_treatment = CUSTOMIZED_MESSAGE;
			MessageWithoutConfirmation("Veuillez patienter SVP", "file://flash/HOST/attendez_svp.png");  //jawad
			break;
		case 261:
			infoscust->cr_treatment = CUSTOMIZED_MESSAGE;
			MessageWithoutConfirmation("Retirer la carte SVP", "file://flash/HOST/retirer_la_carte_svp.png");
			break;
		case 256:
			infoscust->cr_treatment = CUSTOMIZED_MESSAGE;
			MessageWithoutConfirmation("MODE LECTURE \n NON GERE", NULL);
			break;

		default:
			sprintf(buff, "%d", infoscust->num);
//			APEMV_UI_MessageDisplayDelay(buff, 1);
			break;
	}
}


//===========================================================================
//! \brief This function prints the scheduled automatic application calls.
//! ("call" is a reserved manager function).
//! \param[in] noappli : application number.
//! \param[in] param_in : not used.
//! \param[out] param_out : not used.
//! \return Always FCT_OK.
//===========================================================================
static int cu_mcall(NO_SEGMENT noappli, void *param_in, void *param_out)
{
	// Internal data declaration
	int    merchLang;
	char   chaine1[40];
	Telium_Date_t   date_time;

	// Retreive Terminal merchant language
	merchLang = My_PSQ_Give_Language();

	// Print the Call Receipt, using merchant terminal language
	CUPRINT_Open_Printer();

	CUPRINT_String_XT((const unsigned char*)CUMESS_Read_Message(CALLR_MESS_EMV_CALL, merchLang), ALIGN_LEFT, L1);
	CUPRINT_String_XT((const unsigned char*)CUMESS_Read_Message(CALLR_MESS_AUTOMATIC_CALLS, merchLang), ALIGN_LEFT, L1);

	CUBATCH_GetNextTransfDate (&date_time);
	if (PSQ_Donner_format_date() != 0)
	{
		sprintf (chaine1, "%.2s/%.2s/%.2s  %.2s:%.2s\n",
			date_time.month, date_time.day, date_time.year, date_time.hour, date_time.minute);
	}
	else
	{
		sprintf (chaine1, "%.2s/%.2s/%.2s  %.2s:%.2s\n",
			date_time.day, date_time.month, date_time.year, date_time.hour, date_time.minute);
	}

	CUPRINT_String_XT((const unsigned char*)chaine1, ALIGN_LEFT, L1);


	// Ending the print receipt correctely
	CUPRINT_Paper_Feed();
	CUPRINT_Close_Printer();

	// In all cases, always return FCT_OK
	return(FCT_OK);
}

//===========================================================================
//! \brief This function informs the component that the manager parameters
//! have been modified.
//! \param[in] noappli : application number.
//! \param[in] param_in : structure S_MODIF_P containing the fields :
//! - mask : bitfield where each bit corresponds to a parameter.
//! - date_difference : represents the gap in days between the previous date
//! and the new one.
//! - time_difference : represents the gap in minutes between the previous time
//! and the new one.
//! \param[out] param_out : not used.
//! \return Always FCT_OK.
//===========================================================================
static int cu_modif_param(NO_SEGMENT noappli, S_MODIF_P *param_in, void *param_out)
{
	// Internal data declaration
	S_MODIF_P     etatout;

	// Reading mask in param_in
	memcpy(&etatout, param_in, sizeof(etatout));

	// Signal new parameters to engine
	CUSERV_New_Parameters = TRUE;

	return (FCT_OK);
}

//===========================================================================
//! \brief This function authorizes the download of the parameters file.
//! \param[in] no_appli : application number.
//! \param[in] param_in : S_FILE structure containing the fields volume_name, file_name.
//! \param[out] param_out : not used.
//! \return Always FCT_OK.
//===========================================================================
int cu_file_received (NO_SEGMENT no_appli,S_FILE *param_in,void *param_out)
{
	unsigned short   lg_code;
	int              iRet;
	S_FS_PARAM_CREATE ParamCreat;
	char             Dir_File[25]; /*1+3+8+1+3+1, slash disk,name,point,extension,null*/
	char             Dir_Label[25];
	char             LabelDiskName[DISK_NAME_SIZE];

	sprintf (Dir_Label,"/%s",param_in->volume_name);

	if (IsdedicatedFile(param_in)==TRUE)
	{
		ParamCreat.Mode         = FS_WRITEONCE;

		iRet = FS_mount (Dir_Label,&ParamCreat.Mode);

		if (iRet != FS_OK)
		{
			upload_diagnostic_txt((unsigned char *)"PB Mount COM Filen\n");
		}

		{
			memclr(Dir_File,sizeof(Dir_File));
			sprintf (Dir_File,"/%s/%s",param_in->volume_name,param_in->file_name);

			//trace(0, strlen("File received :"), "File received :");
			//trace(0, strlen(Dir_File), Dir_File);

			iRet = FALSE;

			if ((strstr(param_in->file_name, C_AID_NAME) != NULL)		||
				(strstr(param_in->file_name, C_ICS_NAME) != NULL)		||
				(strstr(param_in->file_name, C_KREVOK_NAME) != NULL )	||
				(strstr(param_in->file_name, C_CAKEYS_NAME) != NULL )	||
				(strstr(param_in->file_name, C_FILE_NAME_XML) != NULL) )
			{
				{
					char tmpbuf[30];
					sprintf(tmpbuf, "Load %s file", param_in->file_name);
					//trace(0, strlen(tmpbuf), tmpbuf);
				}

				// CUPAR_FileCopy() is allready called by this function
				iRet = (int)CUPAR_FileRead (Dir_File, param_in->file_name);

				if ((strstr(param_in->file_name, C_ICS_NAME) != NULL) && (iRet != 0))
				{
					iRet = CUPAR_InitICS();
				}
				else if ((strstr (param_in->file_name, C_FILE_NAME_XML) != NULL) && (iRet != 0))
				{
					char FilePath_L[33];

					memclr(LabelDiskName, sizeof(LabelDiskName));
					CUDISK_Get_Disk_Name(LabelDiskName);

					sprintf(FilePath_L, "/%s/%s", LabelDiskName, C_FILE_PARAM);

					// File successfully read. Store it in private disk
					// CUPAR_FileCopy(Dir_File, param_in->file_name);
					//trace(0, strlen("Update PARAM.PAR file"), "Update PARAM.PAR file");
					CUPAR_Update_Param_File(FilePath_L);
				}
				else if (strstr(param_in->file_name, C_CAKEYS_NAME) != NULL)
				{
					// Control and delete bad keys
					iRet = CUPAR_ControlCAKEYS();
				}
			}
			else if (strstr (param_in->file_name, C_BLACKL_NAME) != NULL)  // Black List
			{
				/* CUPAR_FileCopy() is allready called by this function */
				iRet = CUPAR_BlackListInit (Dir_File);
			}
			else
			{
				// This file isn't for us.
				iRet = TRUE;
				//trace(0, strlen("File not managed by Custom EMV"), "File not managed by Custom EMV");
			}

			if (iRet == FALSE)
			{
				char * pc_l_DisplayTxt;
				//trace(0, strlen("File management KO"), "File management KO");
				CUDB_Get_cardholder_language (&lg_code);
				if (lg_code > C_NBLANGHOLDER)
				{
					lg_code = 0;
				}
				pc_l_DisplayTxt = CUMESS_Read_Message(STD_MESS_PROCESSING_ERROR, lg_code);
//				APEMV_UI_MessageDisplay ((char *)pc_l_DisplayTxt);

				MessageWithoutConfirmation((char *)pc_l_DisplayTxt, "file://flash/HOST/naps-tpe-icons-4.png");
				//Os__xprintaa("00001");
			}
			//else
				//trace(0, strlen("File management ok"), "File management ok");
		}
		FS_unmount(Dir_File);
		return (STOP);

	}
	return (REQUEST_NSTOP);

}

//===========================================================================
//! \brief This function .
//! \param[in] param_in : S_FILE structure containing the fields volume_name, file_name.
//! \return
//===========================================================================
static int IsdedicatedFile (S_FILE *param_in)
{
	unsigned char AppliNb[5];
	unsigned short AppliNbLong;

	memclr(AppliNb,sizeof(AppliNb));

	if ((strstr(param_in->file_name, C_AID_NAME) == NULL)		&&
		(strstr(param_in->file_name, C_ICS_NAME) == NULL)		&&
		(strstr(param_in->file_name, C_KREVOK_NAME) == NULL )	&&
		(strstr(param_in->file_name, C_CAKEYS_NAME) == NULL )	&&
		(strstr(param_in->file_name, C_BLACKL_NAME) == NULL )	&&
		(strstr(param_in->file_name, C_FILE_NAME_XML) == NULL) )
	{
		return FALSE;
	}

	if ( (strstr(param_in->file_name, C_AID_NAME) != NULL))
	{
		memcpy (AppliNb,&param_in->file_name[sizeof(C_AID_NAME)+1],4);
	}

	if ((strstr(param_in->file_name, C_ICS_NAME) != NULL) )
	{
		memcpy (AppliNb,&param_in->file_name[sizeof(C_ICS_NAME)],4);
	}

	if ((strstr(param_in->file_name, C_KREVOK_NAME) != NULL ) )
	{
		memcpy (AppliNb,&param_in->file_name[sizeof(C_KREVOK_NAME)],4);
	}

	if ((strstr(param_in->file_name, C_BLACKL_NAME) != NULL ) )
	{
		memcpy (AppliNb,&param_in->file_name[sizeof(C_BLACKL_NAME)],4);
	}

	if ((strstr(param_in->file_name, C_CAKEYS_NAME) != NULL ) )
	{
		memcpy (AppliNb,&param_in->file_name[sizeof(C_CAKEYS_NAME)],4);
	}

	if ((strstr(param_in->file_name, C_FILE_NAME_XML) != NULL) )
	{
		memcpy (AppliNb,&param_in->file_name[sizeof(C_FILE_NAME_XML)-1],4);
	}

	/* if there is no extension for multi custom application and appli type is Standard custom, so file is kept */
	/*  this is to keep compatibility */
	if ((memcmp (AppliNb,"PAR",3)==0)&& (CustApplicationNumber==I_EMVCUST_Type_Appli))
		return TRUE;

	/*&BEGIN FT/09/4668/MA acceptance of ICS_0xxx.par file*/
	if ((AppliNb[0] >= 'A') && (AppliNb[0] <= 'F'))
	   AppliNbLong = (AppliNb[0] - 'A' + 0xa) << 12;
	else AppliNbLong = (AppliNb[0] - '0') << 12;
	if ((AppliNb[1] >= 'A') && (AppliNb[1] <= 'F'))
	   AppliNbLong |= (AppliNb[1] - 'A' + 0xa) << 8;
	else AppliNbLong |= (AppliNb[1] - '0') << 8;
	if ((AppliNb[2] >= 'A') && (AppliNb[2] <= 'F'))
	   AppliNbLong |= (AppliNb[2] - 'A' + 0xa) << 4;
	else AppliNbLong |= (AppliNb[2] - '0') << 4;
	if ((AppliNb[3] >= 'A') && (AppliNb[3] <= 'F'))
	   AppliNbLong |= (AppliNb[3] - 'A' + 0xa);
	else AppliNbLong |= (AppliNb[3] - '0');
	/*&END FT/09/4668/MA acceptance of ICS_0xxx.par file*/

	if (AppliNbLong == CustApplicationNumber  )
		return TRUE;

	return FALSE;

}


//===========================================================================
//! \brief This function returns the name of the application.
//! \param[in] noappli : application number.
//! \param[in] param_in : not used.
//! \param[out] param_out : structure S_ETATOUT containing significant fields
//! appname, and no_appli.
//! \return Always FCT_OK.
//===========================================================================
static int cu_is_name(NO_SEGMENT noappli, void *param_in, S_ETATOUT *param_out)
{
	// Internal data declaration
	S_ETATOUT etatout;

	memcpy(&etatout, param_out, sizeof(etatout));

	strcpy(etatout.returned_state[etatout.response_number].appname, g_szAppName);
	etatout.returned_state[etatout.response_number].no_appli = noappli;
	etatout.response_number++;

	memcpy (param_out, &etatout, sizeof (etatout));

	return(FCT_OK);
}

//===========================================================================
//! \brief This function authorizes or not, the manager, to perform the downloading of
//! the application which the number is given as parameter.
//! \param[in] noappli : application number.
//! \param[in] param_in : not used.
//! \param[out] param_out : structure S_ETATOUT containing the field response appname
//! and no_appli updated.
//! - the field response is set to REP_OK if the download is authorized.
//! - the field response is set to REP_KO if the download is refused.
//! \return Always FCT_OK.
//===========================================================================
static int cu_is_evol_pg (NO_SEGMENT noappli, void *param_in, S_ETATOUT *param_out)
{
	// Internal declaration data
	S_ETATOUT etatout;
	int       isEvolPg;
	unsigned long nb_record;
	unsigned long total_amount;
	T_Bool    ret;

	memcpy(&etatout, param_out, sizeof(etatout));

	// Read Batch status
	ret = CUBATCH_GiveStatistics (&nb_record,
		&total_amount);

	// EMV CUSTOM download allowed if the batch file is void
	if ((!ret) || nb_record == 0)
	{
		etatout.returned_state[etatout.response_number].state.response = REP_OK;
	}
	else
	{
		//etatout.returned_state[etatout.response_number].state.response = REP_KO;
		etatout.returned_state[etatout.response_number].state.response = REP_OK;
	}

	memcpy (param_out, &etatout, sizeof (etatout));

	isEvolPg = cu_is_name(noappli, NULL, param_out);
	// In all cases, always return FCT_OK

	return(isEvolPg);
}

//===========================================================================
//! \brief This function performs the first initialisation.
//===========================================================================
static void CUENT_First_Init (void)
{
	Telium_Date_t tmp_date;
	char FilePath_L[33];
	char *ptrDummy = 0;
	char LabelDiskName[DISK_NAME_SIZE];
	NO_SERIE Tmp;

	// Default mode : emv2000 compatibility
	ucMode2000 = TRUE;

	// Default mode : Euro for currency
	usDefaultCurrency = C_IEP_CURCY_EURO;

	// Default mode : No delay after Atr
	ucDelayAfterAtr = FALSE;

	// Set serial Number
	memset(Tmp, 0, sizeof(Tmp));
	PSQ_Give_Serial_Number (Tmp);
	Tmp[8] = 0;
	CUTERMsetSerialNumber (Tmp);

	// Set terminal ID to 0
	memset(Tmp, 0, sizeof(Tmp));
	CUTERMsetTerminalId(Tmp);

	StoreGlobalData();


#ifdef _TOOLWARE_
	ucAdvancedMode = FALSE;
#endif // _TOOLWARE_

	RetransmitIndicator = FALSE;

	// Retreive Terminal merchant language
	My_PSQ_Give_Language();

	// Create batch log
	// CUBATCH_DeleteLog();
	// CUBATCH_CreateLog(CUBATCH_DEF_LOG_SIZE);

	// Create black-list file
	CUBLACKL_DeleteBlackList();
	CUBLACKL_CreateFile(CUBLACKL_DEF_FILE_SIZE);

	// Initialise default values in databases
	EMVCUST_Initialise_TlvTree (NULL);

	// Initialise date to the next periodic call with default value
	Telium_Read_date(&tmp_date);
	CUBATCH_SetNextTransfDate (&tmp_date);

	memclr(LabelDiskName, sizeof(LabelDiskName));
	CUDISK_Get_Disk_Name(LabelDiskName);

	memclr(FilePath_L,sizeof(FilePath_L));
	sprintf(FilePath_L, "/%s",LabelDiskName);
	FS_unmount(FilePath_L);

	PAR_InitParam(C_PARAM_INIT_APLI, ptrDummy); /* !!! */
	PAR_InitParam(C_PARAM_DEFAULT, ptrDummy);

	CUPAR_ResetAdditionnalICS();
	CUPAR_ResetAdditionnalAID();
	CUPAR_ResetAdditionnalKRevok();
}


void concatenate_string(char *original, char *add)
{
   while(*original)
      original++;

   while(*add)
   {
      *original = *add;
      add++;
      original++;
   }
   *original = '\0';
}

//===========================================================================
//! \brief This function processes a non EMV transaction or a magnetic transaction.
//! \param[in] noappli : application number.
//! \param[in] trans_in : structure S_TRANSIN with fields containing transaction
//! parameters such as : amount, currency, transaction, entry, payment, support...
//! \param[out] trans_out : structure S_TRANSOUT containing the field noappli.
//! \return
//! - FCT_OK : the application has not recognized the card.
//! - STOP : the application has recognized the card and processed the current
//! debit transaction.
//===========================================================================
static int cu_debit_non_emv(NO_SEGMENT noappli, S_TRANSIN *trans_in, S_TRANSOUT *trans_out)
{
	// Internal data declaration
	int        debitExport;
	S_TRANSOUT transaction_out, *param_out;
	unsigned short lg_code;
	int deb_res;
	unsigned char buffer[MAXPTISO];
	unsigned long  entered_amount;
	int result;
	uchar	status;

#ifdef _USE_UCM_
	T_UCMHOST_DEBIT sDebit;
	T_UCMHOST_R_DEBIT sRDeb;
	int fct;
	int RunMSR;
#endif

	memcpy(&transaction_out, trans_out, sizeof(S_TRANSOUT));
	param_out = &transaction_out;

	debitExport = FCT_OK;
	CUDB_Get_cardholder_language (&lg_code);


//	  if(StructInfoPayment.Entry_Mode == TRS_EMV)
//	  {
//		  debitExport = STOP;
//		  return(debitExport);
//	  }


	/*&BEGIN FT/07/2825/MA     inform CAd30 simu of paiement  */
#ifdef _USE_UCM_

	if(UCM_Present)
	{

		sDebit.ulAmount = trans_in->amount;
		sDebit.tCurrency = trans_in->currency;

		sDebit.ucTrsType = trans_in->transaction;
		sDebit.ucTrsEntry = trans_in->entry;
		sDebit.ucTrsMode = trans_in->payment;
		sDebit.ucTrsSupport = trans_in->support;

		sDebit.ucMode = UCMHOST_MODE_REEL;
		sDebit.ucClasse = UCMHOST_CLASSE_2_1;
		sDebit.ucPrint = 1;
		sDebit.ucDisplay = 1;

		sDebit.usToWaitingCard = 0xFF;
		sDebit.usToRemovedCard = 0xFF;
		sDebit.ucAppliNum = noappli;
		sDebit.ucPowerOn = 1;
		sDebit.ucFunction = DEBIT_EMV;


		// EMV application is ready for a debit/record
		fct = iLIBUCM_Pay_Ready_For_Debit(sizeof(T_UCMHOST_DEBIT), &sDebit);


		if(fct==UCMCPAY_SOLV)
		{
			RunMSR = TRUE;
		}
		else RunMSR = FALSE;
	}
	else RunMSR = TRUE;

	if (RunMSR)
	{
#endif
	/*&END FT/07/2825/MA     inform CAd30 simu of paiement  */

		// Begin
		if ((trans_in->support == TRACK2_SUPPORT) || (trans_in->support == TRACK12_SUPPORT) || (trans_in->support == TRACK32_SUPPORT) || (trans_in->support == TRACK123_SUPPORT))
		{
			memset(&StructInfoPayment ,0x00,sizeof(StructInfoPayment));//Larbi
			//APEMV_UI_MessageDisplayDelay("LU",3);
			deb_res = FALSE;
			memcpy(&buffer[0],(char*)trans_in->track1,50);
			memcpy(&buffer[50],(char*)trans_in->track2,50);

			if(Recup_Info_Card((char*)buffer) != OK)
			{
//				uchar		Card_Nb[20];
//				memset(Card_Nb, 0, 20);
//				memcpy(Card_Nb, &StructInfoPayment.Holder_Card[1],StructInfoPayment.Holder_Card[0]);
//				Os__xprinta(Card_Nb);
			  CUMORE_CMI_AFFICHE_MESSAGE(134,135,1000);
			}
			else
			{
			 	if(valide_carte() != OK)
				{
					memset(&E_Transac8583,0x00,sizeof(E_Transac8583));
					memset(&R_Transac8583,0x00,sizeof(R_Transac8583));
					memset(&StructInfoPayment ,0x00,sizeof(StructInfoPayment));
				}
			 	else
			 	{
					////////////////////////////get 4 digits//////////////////////////////
	//				if ((!StructInfoPayment.Pin_Required)
	//						&& (StructInfoPayment.Entry_Mode == TRS_MAG)
	//						&& (StructInfoPayment.CodeFunction != CASHOUT_PREPAYE)
	//				) {
	//					status = Verification_Digits();
	//					if (status == REFUSED) {
	//						ShowTrace("VC18");
	//						CUMORE_CMI_AFFICHE_MESSAGE(136, 137, 1000);
	//					}
	//					if (status != OK) {
	//						ShowTrace("VC19");
	//						return status;
	//					}
	//				}

					////////////////////4 digit//////////////////////////////////////////
					result = CUMESS_GetAmount (0x504, &entered_amount);

					if ((result) && (entered_amount!=0))
					{
						StructInfoPayment.Transaction_Amount = entered_amount;
						memcpy(StructInfoPayment.Devise_acc.code_num,"504",3);
						memcpy(StructInfoPayment.Devise_acc.code_alph,"MAD",3);
						StructInfoPayment.Devise_acc.Exposant = 2;

						if(memcmp(FicPara.MCC,"6010",4) == 0)
						  StructInfoPayment.CodeFunction = CASHADVANCESCODE;
						else
						  StructInfoPayment.CodeFunction = SALECODE;

						//Message de Confirmation du montant
						if (MessageConfirmationMontant() == OK)
						//Message de Confirmation du montant
						{
						StructInfoPayment.Fallback = 0;

						memset(&E_Transac8583,0x00,sizeof(E_Transac8583));
						memset(&R_Transac8583,0x00,sizeof(R_Transac8583));
						//Os__xprintaa("01");

						transa_client();
						PutAllTra();
						PutAllParam();
						}
					}
			 	}
			}

			//deb_res = CUSTR_Mag_Stripe_Debit ((trans_in->entry != NO_ENTRY), trans_in->amount, (int) (trans_in->report_piste), (char*)trans_in->track2);

			if (deb_res == FALSE)
			{
				debitExport = FCT_OK;
			}
			else
			{
				debitExport = STOP;
			}
		}  /* End if Chip Card                           */
		else {
			APEMV_UI_MessageDisplayDelay("MALU",3);
			debitExport = STOP;
		}


		/*&BEGIN FT/07/2825/MA     inform CAd30 simu of paiement  */
#ifdef _USE_UCM_
		if(UCM_Present)
		{
			memclr(&sRDeb, sizeof(sRDeb));

			sRDeb.ucDiag = 0;
			sRDeb.ucUCMDiag = 0;

			sRDeb.ucPrinter = 1;
			sRDeb.ucDisplay = 1;

			iLIBUCM_Icc_Status(UCMC_ICC, &sRDeb.ucCardInside);

			sRDeb.ucMode = sDebit.ucTrsMode;
			sRDeb.ucFunction = sDebit.ucFunction;
			sRDeb.ucTypeCardStruct = UCMHOST_CARD_EMV;
			sRDeb.ucSupport = trans_in->support;

			sRDeb.usAppName  = noappli;
			//strcpy(sRDeb.tAppLibelle, appnamemv);

			sRDeb.ulAmount = sDebit.ulAmount;
			memcpy( &sRDeb.tCurrency, &sDebit.tCurrency, sizeof( S_MONEY));

			sRDeb.ucCardHolderLanguage = 0xFF;
			// manage sRDeb.u

			Telium_Read_date( &sRDeb.u.sCard.uAppli.sApp.sLocalDate ) ;

			sRDeb.u.sCard.uAppli.sApp.ucTypeTrs[ 0 ] = 'T';
			sRDeb.u.sCard.uAppli.sApp.ucTypeTrs[ 1 ] = 0;

			memset( sRDeb.u.sCard.uAppli.sApp.OwnerNumber, '1', sizeof(NO_PORTEUR) );
			memset( sRDeb.u.sCard.uAppli.sApp.OwnerNumber, 'F', 4 );
			memset( sRDeb.u.sCard.uAppli.sApp.ucServiceCode, 'S', 3 );
			sRDeb.u.sCard.uAppli.sApp.iTrsNumber = 1234;
			sRDeb.u.sCard.uAppli.sApp.iRemisNumber = 5678;
			sRDeb.u.sCard.uAppli.sApp.ulAmountCV = 9999;
			memset( &sRDeb.u.sCard.uAppli.sApp.sCurrencyCV, 'V', sizeof(S_MONEY) );
			sRDeb.u.sCard.uAppli.sApp.ulAmountEstimat = 8888;

			memset( &sRDeb.u.sCard.uAppli.sApp.ucAutoNumber[0], 'A', 3 );
			sRDeb.u.sCard.uAppli.sApp.ucForcingCode = 5;

			sRDeb.u.sCard.uAppli.sApp.ucReaderMode = 2;

			sRDeb.u.sCard.uAppli.sApp.usTASA = 9;

			// T_UCMHOST_CARD_ACCEPT
			memset( sRDeb.u.sCard.uAccept.sAcpt.ucNoContrat, 'C', 7 ) ;
			memset( sRDeb.u.sCard.uAccept.sAcpt.ucNoSA, 'S', 3 ) ;
			memset( sRDeb.u.sCard.uAccept.sAcpt.ucNoSiret, 'R', 14 ) ;
			memset( sRDeb.u.sCard.uAccept.sAcpt.ucTypeActivite, 'C', 4 ) ;
			memset( sRDeb.u.sCard.uAccept.sAcpt.ucTypeSite, 'C', 4 ) ;
			memset( sRDeb.u.sCard.uAccept.sAcpt.ucTypePaiement, 'P', 2 ) ;
			memset( sRDeb.u.sCard.uAccept.sAcpt.ucEnseigne, 'E', 60 );
			sRDeb.u.sCard.uAccept.sAcpt.ucEnseigne[ 60 ] = 0 ;
			memset( sRDeb.u.sCard.uAccept.sAcpt.ucEnteteTicket, 'T', 50 );
			sRDeb.u.sCard.uAccept.sAcpt.ucEnteteTicket[ 50 ] = 0 ;
			memset( sRDeb.u.sCard.uAccept.sAcpt.ucPiedTicket, 'P', 50 );
			sRDeb.u.sCard.uAccept.sAcpt.ucPiedTicket[ 50 ] = 0 ;

			iLIBUCM_Pay_Result_Debit(sizeof(T_UCMHOST_R_DEBIT), &sRDeb);

			iLIBUCM_Pay_End(0, NULL);
		}
	}
	else
	{
		debitExport = FCT_OK;
	}
#endif
	/*&END FT/07/2825/MA     inform CAd30 simu of paiement  */

	transaction_out.noappli=noappli;

	memcpy (param_out, &transaction_out, sizeof (transaction_out));


	return(debitExport);
}








//===========================================================================
//! \brief This function returns the priority of EMV selection of chip card
//! application.
//! \param[in] noappli : application number.
//! \param[in] param_in : not used.
//! \param[out] param_out : structure S_ETATOUT containing the fields response, appname
//! and no_appli updated.
//! \return Always FCT_OK.
//===========================================================================
int cu_is_for_you_before  (NO_SEGMENT noappli,S_TRANSIN *param_in, S_ETATOUT *param_out)
{
	//int iRet ;
	S_ETATOUT etatout;

#ifdef _TOOLWARE_
	if (ucAdvancedMode == TRUE)
	{
		ulMMI_langue   = 0;
		ulMMI_selappli = 0;
		ulBegin_transaction = TMT_Retrieve_Clock();
	}
#endif // _TOOLWARE_

	TrackIAC_Init();
	memcpy (&etatout, param_out, sizeof(etatout));

	etatout.returned_state[etatout.response_number].state.response = 0 ;
	memcpy (param_out, &etatout, sizeof (etatout));
	cu_is_name (noappli, PT_NULL, param_out);

	return (FCT_OK);
}

//===========================================================================
//! \brief This function returns the priority of EMV selection of chip card
//! application
//! \param[in] noappli : application number.
//! \param[in] param_in : structure S_TRANSIN containing the fields support,
//! power_on_result, amount.
//! \param[out] param_out : structure S_CARDOUT with the fields appname, and noappli.
//! Optionnally it can contain the fields cardappnumber, cardapp, cardappname.
//! \return Always FCT_OK.
//===========================================================================
static int cu_is_for_you_after  (NO_SEGMENT noappli,S_TRANSIN *param_in, S_CARDOUT *param_out)
{
	S_CARDOUT card_out;
	S_ETATOUT etatout;

	etatout.response_number = 0;
	cu_is_name(noappli, NULL, &etatout);

	memcpy (&card_out, param_out, sizeof(card_out));


	// Card recognised only when mag stripe is used
	if ((param_in->support == TRACK2_SUPPORT) ||
		(param_in->support == TRACK12_SUPPORT) ||
		(param_in->support == TRACK32_SUPPORT))
	{
		strcpy (card_out.returned_state[card_out.response_number].appname , (char*)etatout.returned_state[0].appname) ;
		card_out.returned_state[card_out.response_number].no_appli = noappli ;
		card_out.returned_state[card_out.response_number].cardappnumber = 1 ;
		card_out.returned_state[card_out.response_number].cardapp [0].priority = 30;
		strcpy (card_out.returned_state[card_out.response_number].cardapp [0].cardappname, (char*)etatout.returned_state[0].appname) ;
		card_out.response_number++;
	}
	else
	{
		// This is a Chip card
		card_out.returned_state[card_out.response_number].cardappnumber = 0 ;

		nb_time_select =0;

		if ((param_in->power_on_result) == 0)
		{
			/*&BEGIN FT/08/4015/MA    bad format or forgotten amount */
			ul_InputAmount = param_in->amount;
			/*&END FT/08/4015/MA    bad format or forgotten amount */

			/* The card has been successfully read                          */
			isFallbackAllowed = CUSTR_CheckFallback();
			if (isFallbackAllowed)
			{
#ifdef _USE_UCM_
				if(!UCM_Present)
#endif
					cu_fall_back(noappli, param_in->entry, param_in->amount);
			}
			card_out.returned_state[card_out.response_number].cardappnumber = 0 ;
			card_out.returned_state[card_out.response_number].cardapp [0].priority = 0;

		}

	}

	memcpy (param_out,&card_out, sizeof(card_out));
	return (FCT_OK);
}

//===========================================================================
//! \brief This function allows to define the custom working environnement.
//! TYP_EXPORT in this case.
//! \param[in] no : application number.
//! \param[in] p1 : not used.
//! \param[out] param_out : structure S_INITPARAMOUT containing the fields mask
//! (mask of usefull parameters), and application_type.
//! \return Always FCT_OK.
//===========================================================================
static int cu_give_your_domain(NO_SEGMENT no, void *p1, S_INITPARAMOUT *param_out)
{
	S_INITPARAMOUT etatout;

	memcpy (&etatout, param_out, sizeof(etatout));
	// Parameter mask to be determined.
	etatout.returned_state[etatout.response_number].mask     = MSK_TOUS_PARAM;
	etatout.returned_state[etatout.response_number].application_type = TYP_EXPORT;
	etatout.response_number++;
	memcpy (param_out, &etatout, sizeof(etatout));
	return (FCT_OK);
}

//===========================================================================
//! \brief This function authorises the manager to delete the custom application.
//! \param[in] noappli : application number.
//! \param[in] param_in : not used.
//! \param[out] auth_deleting : structure S_DELETE containing the field response
//! set to REP_OK. The fields appname, and noappli are also fulfilled.
//! \return Always FCT_OK.
//===========================================================================
static int cu_is_delete (NO_SEGMENT noappli,void *param_in, S_DELETE *auth_deleting)
{

	auth_deleting->deleting = DEL_YES ;

	return (FCT_OK);
}

//===========================================================================
int cu_fall_back (NO_SEGMENT noappli, ENTRY_TYPE AmountEntry, unsigned long amount)
{
	unsigned int uiEvent;
	Telium_File_t *swipe2, *swipe13, *keyboard;
	LG_REF_CLI lgIso2;
	unsigned char ucPriority;
	int iResult, iIsClosed;
	unsigned char ucLgIso1, ucLgIso3;

	unsigned short lg_code;
	char *			pc_l_DisplayTxt;
	int previousUiState;

	unsigned char buffer[MAXPTISO];
	unsigned long  entered_amount;
	int result;
	uchar	status;

   	previousUiState = APEMV_UI_TransactionBegin();

	DisplayHeader (_OFF_);

	keyboard = NULL;

    swipe2 = Telium_Stdperif("SWIPE2", NULL);
    if ( swipe2 == NULL)
	/* FT : 2571 swipe */
    	swipe2 = Telium_Fopen("SWIPE2","r");

	swipe13 = Telium_Fopen("SWIPE31","r*");
	iIsClosed = FALSE;

//	if(Valider(164,168) != KEY_ENTER)
//			goto FB_End;

	/* --------------------------------------------------------------------*/
	/* Display a message asking to swipe the card                          */
	CUDB_Get_cardholder_language(&lg_code);
	pc_l_DisplayTxt = CUMESS_Read_Message(STD_MESS_USE_MAG_STRIPE, lg_code);

	//APEMV_UI_MessageDisplayDelay ((char *)"PASSEZ LA CARTE \n EN MODE PISTE", 0);
	MessageWithoutConfirmation("PUCE DEFECTUEUSE \nPASSEZ LA CARTE" , "file://flash/HOST/passer.bmp");  //(char *)

	keyboard = Telium_Fopen("KEYBOARD","r");

	/* --------------------------------------------------------------------*/
	/* Wait for either a time-out, or a card to be swiped                  */
	/* !! 2 testall are necessary (see system documentation)               */
	uiEvent = Telium_Ttestall (KEYBOARD | SWIPE2 | SWIPE31, 2000);
	if ((uiEvent & (SWIPE2 | SWIPE31)) != 0)
	{
		uiEvent |= Telium_Ttestall (uiEvent ^ (SWIPE2 | SWIPE31), 100);

		if ((uiEvent & SWIPE2) != 0)
		{
			 Telium_Is_iso2 (swipe2, &lgIso2, bufIso2);
		}

		if ((uiEvent & SWIPE31) != 0)
		{
			 Telium_Is_iso1 (swipe13, &ucLgIso1, bufIso1);
			 Telium_Is_iso3 (swipe13, &ucLgIso3, bufIso3);
		}

		if (swipe2 != NULL)
		{
			Telium_Fclose (swipe2);
			swipe2 = NULL;
		}
		if (swipe13 != NULL)
		{
			Telium_Fclose (swipe13);
			swipe13 = NULL;
		}
		if (keyboard != NULL)
		{
			Telium_Fclose (keyboard);
			keyboard = NULL;
		}
		iIsClosed = TRUE;

		/* ----------------------------------------------------------------*/
		/* Call the magstripe application, if present                      */
		//!
		//! \todo FALL-BACK implementation :
		//! This piece of code is just a sample of how the EMV
		//! application could communicate with an independent magstripe one
		//! This is not, for sure, the only one possible implementation.
		//! Update this code to fit with your own implementation.
		//!
		if (Telium_ServiceGet (0x68, DEBIT_NON_EMV, &ucPriority) == 0)
		{
			EntryPoint.type_code = 0x68;
			EntryPoint.service = DEBIT_NON_EMV;

			// Fill in trans_in structure
			trans_in.del.count = 0;
			trans_in.support = TRACK2_SUPPORT;
			trans_in.entry = AmountEntry;
			trans_in.payment = CARD_PAYMENT;
			trans_in.transaction = DEBIT_TR;
			// No initial amount
			if (AmountEntry != NO_ENTRY)
				trans_in.amount = amount;
			else trans_in.amount = 0;
			trans_in.peri = 0;
			trans_in.report_piste = TRUE;
			memcpy (trans_in.track1, bufIso1, sizeof (TRACK1_BUFFER));
			memcpy (trans_in.track2, bufIso2, sizeof (TRACK2_BUFFER));
			memcpy (trans_in.track3, bufIso3, sizeof (TRACK3_BUFFER));

			memcpy(&EntryPoint.Param.DebitNonEmv.param_in, &trans_in, sizeof(S_TRANSIN));

			// Call the service
			Telium_ServiceCall(0x68, DEBIT_NON_EMV, sizeof(StructPt), &EntryPoint, &iResult);

		}
		else
		{
			memset(&StructInfoPayment ,0x00,sizeof(StructInfoPayment));//Larbi
			memcpy(&buffer[0],(char*)bufIso1,50);
			memcpy(&buffer[50],(char*)bufIso2,50);

			if(Recup_Info_Card((char*)buffer) != OK)
			{
//				uchar Card_Nb[20];
//				memset(Card_Nb, 0, 20);
//				memcpy(Card_Nb, &StructInfoPayment.Holder_Card[1],
//						StructInfoPayment.Holder_Card[0]);
//				Os__xprinta(Card_Nb);
				CUMORE_CMI_AFFICHE_MESSAGE(134, 135, 1000);
			}
			else
			{
//			 	if(valide_carte() != OK)
//				{
//					memset(&E_Transac8583,0x00,sizeof(E_Transac8583));
//					memset(&R_Transac8583,0x00,sizeof(R_Transac8583));
//					memset(&StructInfoPayment ,0x00,sizeof(StructInfoPayment));
//				}
//			 	else
//			 	{
					////////////////////////////get 4 digits//////////////////////////////
	//				if ((!StructInfoPayment.Pin_Required)
	//						&& (StructInfoPayment.Entry_Mode == TRS_MAG)
	//						&& (StructInfoPayment.CodeFunction != CASHOUT_PREPAYE)
	//				) {
	//					status = Verification_Digits();
	//					if (status == REFUSED) {
	//						ShowTrace("VC18");
	//						CUMORE_CMI_AFFICHE_MESSAGE(136, 137, 1000);
	//					}
	//					if (status != OK) {
	//						ShowTrace("VC19");
	//						return status;
	//					}
	//				}
					////////////////////4 digit//////////////////////////////////////////

					result = CUMESS_GetAmount (0x504, &entered_amount);


					if ((result) && (entered_amount!=0))
					{
						StructInfoPayment.Transaction_Amount = entered_amount;
						memcpy(StructInfoPayment.Devise_acc.code_num,"504",3);
						memcpy(StructInfoPayment.Devise_acc.code_alph,"MAD",3);
						StructInfoPayment.Devise_acc.Exposant = 2;

						if(memcmp(FicPara.MCC,"6010",4) == 0)
						  StructInfoPayment.CodeFunction = CASHADVANCESCODE;
						else
						  StructInfoPayment.CodeFunction = SALECODE;

						//Message de Confirmation du montant
						if (MessageConfirmationMontant() == OK)
						//Message de Confirmation du montant
						{
							StructInfoPayment.Fallback = 1;

							memset(&E_Transac8583,0x00,sizeof(E_Transac8583));
							memset(&R_Transac8583,0x00,sizeof(R_Transac8583));
							//Os__xprintaa("02");

							transa_client();
							PutAllTra();
							PutAllParam();
						}
					}
			 	//}
			}

			// No Magstripe application present, default treatment.
			//CUSTR_Mag_Stripe_Debit ((AmountEntry != NO_ENTRY), amount, TRUE, (char *)bufIso2);
		}
	}

FB_End:
	if (!iIsClosed)
	{
		if (swipe2 != NULL)
		{
			Telium_Fclose (swipe2);
			swipe2 = NULL;
		}
		if (swipe13 != NULL)
		{
			Telium_Fclose (swipe13);
			swipe13 = NULL;
		}
		if (keyboard != NULL)
		{
			Telium_Fclose (keyboard);
			keyboard = NULL;
		}
	}

	DisplayHeader (_ON_);
	// Restore the user interface environment
	APEMV_UI_TransactionEnd(previousUiState);

	return (FCT_OK);
}

//===========================================================================
//! \brief This function builds the candidate list with TLV tree returned
//! by the first application selection.
//! \param[in] phInputTLVTree : TLV tree returned by the first application
//! selection.
//===========================================================================
static void cu_Build_Candidate_List_TlvTree(TLV_TREE_NODE *phInputTLVTree)
{
	int index_AID;
	DataElement Data_Elt;
	TLV_TREE_NODE hTree_ICC, hTree;

	index_AID = 0;

	do{
		// search tag TAG_AID_ICC in input TLV tree
		hTree_ICC = TlvTree_Find(*phInputTLVTree, TAG_AID_ICC, index_AID);
		if (hTree_ICC != NULL)
		{
			// get the length
			Data_Elt.length = TlvTree_GetLength(hTree_ICC);
			// get the value
			Data_Elt.ptValue = TlvTree_GetData(hTree_ICC);
			outAIDCandidateList.count++;
			outAIDCandidateList.list[index_AID].cardholder = FALSE;
			outAIDCandidateList.list[index_AID].lengthAID= Data_Elt.length;
			memcpy(outAIDCandidateList.list[index_AID].AIDValue, Data_Elt.ptValue, Data_Elt.length);
			outAIDCandidateList.list[index_AID].PDOL = PT_NULL ;
			outAIDCandidateList.list[index_AID].lengthPDOL= 0;

			// init hTree
			hTree = TlvTree_GetFirstChild(hTree_ICC);
			do{
				if (hTree != NULL)
				{
					// get the tag
					Data_Elt.tag = TlvTree_GetTag(hTree);
					// get the length
					Data_Elt.length = TlvTree_GetLength(hTree);
					// get the value
					Data_Elt.ptValue = TlvTree_GetData(hTree);

					switch (Data_Elt.tag)
					{
					case TAG_APPLICATION_LABEL :
						outAIDCandidateList.list[index_AID].lengthApplicationLabel = Data_Elt.length;
						memcpy(outAIDCandidateList.list[index_AID].applicationLabel, Data_Elt.ptValue, Data_Elt.length);
						break;

					case TAG_APPLICATION_PREFFERRED_NAME :
						outAIDCandidateList.list[index_AID].lengthPreferredName = Data_Elt.length;
						memcpy(outAIDCandidateList.list[index_AID].preferredName, Data_Elt.ptValue, Data_Elt.length);
						break;

					case TAG_LANGUAGE_PREFERENCE :
						outAIDCandidateList.list[index_AID].lengthLanguagePreference = Data_Elt.length;
						memcpy(outAIDCandidateList.list[index_AID].languagePreference, Data_Elt.ptValue, Data_Elt.length);
						break;

					case TAG_ISSUER_CODE_TABLE_INDEX :
						if (Data_Elt.ptValue != NULL)
						{
							outAIDCandidateList.list[index_AID].codeTableIndex = *(Data_Elt.ptValue);
						}
						else
						{
							outAIDCandidateList.list[index_AID].codeTableIndex = 0;
						}
						break;

					case TAG_APPLI_PRIOR_IND :
						if (Data_Elt.ptValue != NULL)
						{
							outAIDCandidateList.list[index_AID].priority = (Data_Elt.ptValue[0] & 0x0F);
							outAIDCandidateList.list[index_AID].cardholder = ((Data_Elt.ptValue[0] & 0x80) != 0);
						}
						else
						{
							outAIDCandidateList.list[index_AID].priority = 0;
							outAIDCandidateList.list[index_AID].cardholder = 0;
						}
						break;

					case TAG_AID_ICC :
					case TAG_SERVICE_STATUS_CODE:
					case TAG_CARDHOLDER_CONFIRMATION:
						break;
					}

					// get next node
					hTree = TlvTree_GetNext(hTree);

				}
			} while (hTree != NULL);
		}
		index_AID++;
		// j++;
	}while (hTree_ICC != NULL);
}

//===========================================================================
//! \brief This function asks to the cardholder to choose an AID.
//! \param[in] aAIDCandidateList : list of candidate applications.
//! \param[in] CConfimationSupported : if set to 1, indicates that the AID
//! cardholder confirmation is supported by the terminal.
//! \return TRUE if an AID has been selected.
//===========================================================================
static int Select_AID(AIDCandidateList* aAIDCandidateList, unsigned short CConfimationSupported )
{

	int i,DisplayAidsConfirmation, cr_select;
	byte TAB_FinalSelection[10];

#ifdef _TOOLWARE_
	unsigned long ulFin_ihm   = 0;
	unsigned long ulDebut_ihm = 0;
#endif

	if (aAIDCandidateList->count == 0)
		return KO;

	for (i=0;i<10;i++)
		TAB_FinalSelection[i] = 0;

	DisplayAidsConfirmation=0;
	i=0;

	cu_Short_AID_List(TAB_FinalSelection, aAIDCandidateList);
	/*&BEGIN FT/08/2636/MA  Manage Cardholder Confirmation parameter is value is NO */

	// is there a cardHolder Confirmation in the aAIDCandidateList ? if yes DisplayAidsConfirmation = 1
	while (i < aAIDCandidateList->count && !DisplayAidsConfirmation)
	{
		DisplayAidsConfirmation = ((aAIDCandidateList->list[i].cardholder == 1)&& (CConfimationSupported == 1));
		i++;
	}

	// Cardholder confirmation if many Aids and Option Confirmation supported :
	if (DisplayAidsConfirmation == 0)
	{
		if (( aAIDCandidateList->count >= 2 ) && (CConfimationSupported == 1))
		{
			DisplayAidsConfirmation = 1;
		}
		/*&END FT/08/2636/MA  Manage Cardholder Confirmation parameter is value is NO */
	}

	cr_select = OK;

	if (DisplayAidsConfirmation == 1)
	{
#ifdef _TOOLWARE_
		if (ucAdvancedMode == TRUE)
		{
			ulDebut_ihm = TMT_Retrieve_Clock();
		}
#endif
		if ( cu_AS_AffichageCardHolder(TAB_FinalSelection,aAIDCandidateList) != CR_ENTRY_OK)
		{/* CANCEL */
			cr_select = KO;
		}
#ifdef _TOOLWARE_
		if (ucAdvancedMode == TRUE)
		{
			ulFin_ihm = TMT_Retrieve_Clock();
			ulMMI_selappli += ulFin_ihm - ulDebut_ihm;
		}
#endif

	}
	else
	{
		if (CConfimationSupported == 1)
		{
			num_aid_selected = TAB_FinalSelection[0];
		}
		else
		{
			// If Cardholder Confirmation is not required do not run an application that needs to be confirmed
			num_aid_selected = 0xFF;
			i=0;
			while (i < aAIDCandidateList->count && (num_aid_selected == 0xFF))
			{
				if ((aAIDCandidateList->list[TAB_FinalSelection[i]].cardholder == 0))
					num_aid_selected = TAB_FinalSelection[i];
				i++;
			}

			if (num_aid_selected == 0xFF)
				return KO;
		}
	}
	return (cr_select);
}

//===========================================================================
//! \brief This function shorts the AID by decrease priority.
//! \param[out] Tab :
//! \param[in] aAIDCandidateList : list of candidate applications.
//! \return
//! - NO_SAME_PRIOR = 1 if no AID with the same priority is found in the candidate list.
//! - SAME_PRIOR = 0 if at least two AID with the same priority are found in the candidate list.
//===========================================================================
static int cu_Short_AID_List(byte* Tab, AIDCandidateList* aAIDCandidateList)
{
	int cpt,i,j,cr;
	unsigned char flag;
	cpt=j=0;
	flag = FALSE;
#define NO_SAME_PRIOR	1
#define SAME_PRIOR		0
#define MAX_PRIORITY	15

	cr = NO_SAME_PRIOR;

	for (i=1;i<=MAX_PRIORITY;i++)
	{
		while(j<aAIDCandidateList->count)
		{
			if(aAIDCandidateList->list[j++].priority == i)
			{
				// Only the same highest priority
				if ( (flag == TRUE) &&  ( aAIDCandidateList->list[Tab[0]].priority == aAIDCandidateList->list[j-1].priority))
					cr=SAME_PRIOR;
				Tab[cpt++]=j-1;
				flag = TRUE;
			}

		}
		j=0;
		flag = FALSE;
	}
	return cr;
}

//===========================================================================
//! \brief This function displays the candidate list to the cardholder.
//! \param TAB : table of AID number.
//! \param[in] aAIDCandidateList : list of candidate applications.
//! \return
//! - CR_ENTRY_OK : Successful entry.
//! \li the field d_entry[0] of entry_buffer contains the index of the selected
//! item in the list for G_List_Entry.
//! \li the field d_len of entry_buffer contains the entry length.
//! \li the field d_entry [50]  of entry_buffer contains the entry for G_Numerical_Entry.
//! - CR_ENTRY_NOK : Correction (if authorised )
//! - CR_ENTRY_TIME_OUT : Timeout elapsed
//! - CR_ENTRY_CANCEL : Entry Cancelled
//! - CR_ENTRY_VAL : Green key (confirmation)
//! - CR_ENTRY_FCT : F key pushed (if authorised )
//! - CR_ENTRY_SK1 : SK1 ('1' on EFT930M or ML30) key pushed (if authorised )
//! - CR_ENTRY_SK2 : SK2 ('2' on EFT930M or ML30) key pushed (if authorised )
//! - CR_ENTRY_SK3 : SK3 ('3' on EFT930M or ML30) key pushed (if authorised )
//! - CR_ENTRY_SK4 : SK4 ('4' on EFT930M or ML30) key pushed (if authorised )
//===========================================================================
static int cu_AS_AffichageCardHolder(byte* TAB, AIDCandidateList *aAIDCandidateList)
{
	byte				i, cr;
	unsigned char		codeTableIndex;
	unsigned short		found;
	unsigned short		IndexManaged;
	DataElement			elt;
	ENTRY_BUFFER		entry_buffer;
	APEMV_UI_SelectAidItem_t *AidUI;
	char 				*title;
	//int previousUiState;


#ifdef _USE_UCM_
	byte index;
	unsigned char		Second_line_name[18];
	unsigned char		NoEnd;
	char				ctMessage[200];
	int					iKey;
#endif

	unsigned short		lg_code;

	cr = CR_ENTRY_OK;

	// Reset the selected mark list
	PAR_ResetMarkList();

	// Select Main ICS + ICSx (Maybe unused)
	CUPAR_SelectAdditionnalICS();

	// Get data from TLVTree
	found = PAR_ReadParameter(TAG_ADD_TERMINAL_CAPABILITIES, (DataElementExt *)&elt);

	// Get the cardholder language
	CUDB_Get_cardholder_language(&lg_code);
	// Get the manager language
	// lg_code = PSQ_Give_Language();


	// init table from const
	title = CUMESS_Read_Message(DFT_MESS_NB_CHOOSE_AN_AID, lg_code);

	AidUI = umalloc(aAIDCandidateList->count * sizeof(APEMV_UI_SelectAidItem_t));
	if (AidUI != NULL)
	{
			// init table from const
	for (i=0 ; i < aAIDCandidateList->count ; i++ )
	{
		// Check in Additional Terminal Capabilites the bit corresponding to the code table index
		IndexManaged = FALSE;
		codeTableIndex = aAIDCandidateList->list[i].codeTableIndex;
		AidUI[i].issuerCodeTableIndex = codeTableIndex;
		AidUI[i].aidLength = aAIDCandidateList->list[i].lengthAID;
		memcpy (&AidUI[i].aid[0],(unsigned char *)aAIDCandidateList->list[i].AIDValue,AidUI[i].aidLength) ;

		if ((codeTableIndex >= 1) && (codeTableIndex < 9))
		{
			// check in byte 5
			if (found)
				IndexManaged = ((elt.ptValue [4] & (0x01 << (codeTableIndex - 1))) != 0);
		}
		if ((codeTableIndex == 9) ||
			(codeTableIndex == 10))
		{
			// check in byte 4
			if (found)
				IndexManaged = ((elt.ptValue [3] & (0x01 << (codeTableIndex - 9))) != 0);
		}

		if ((aAIDCandidateList->list[i].lengthPreferredName != 0) && IndexManaged)
		{
			memcpy (&AidUI[i].preferredName[0],(unsigned char *)aAIDCandidateList->list[i].preferredName,aAIDCandidateList->list[i].lengthPreferredName) ;
			AidUI[i].preferredName[aAIDCandidateList->list[i].lengthPreferredName] = 0;
		}
		else AidUI[i].preferredName[0] = 0;

		memcpy (&AidUI[i].applicationLabel[0],(unsigned char *)aAIDCandidateList->list[i].applicationLabel,aAIDCandidateList->list[i].lengthApplicationLabel) ;
		AidUI[i].applicationLabel[aAIDCandidateList->list[i].lengthApplicationLabel] = 0;
	}

#ifdef _USE_UCM_
	if(UCM_Present)
	{
		index=0;
		NoEnd=0;

		iLIBUCM_Display_Open(UCMC_DISPLAY, "w");
		iLIBUCM_Pinpad_Open(UCMC_PPAD, "r*");

		if (aAIDCandidateList->count == 1)
		{
			sprintf(ctMessage, "%.16s\n%s",Menu.tab[index], CUMESS_Read_Message(DFT_MESS_NB_YESV_NOA, lg_code));

			iLIBUCM_Display_Message( UCMC_DISPLAY, ctMessage,0) ;

			do
			{
				iKey = iLIBUCM_Pinpad_GetChar( UCMC_PPAD );
			}
			while( (iKey!=T_ANN) && (iKey!=T_VAL) );

			if (iKey==T_VAL)
			{
				num_aid_selected = 0;
			}
		}
		else
		{
			memclr( &Second_line_name[0] , sizeof(Second_line_name) );
			strcpy((char *)Second_line_name,"OK(V)   AUTRE(C)");

			do
			{
				sprintf(ctMessage, "%.16s\n%.16s",Menu.tab[index], Second_line_name);

				iLIBUCM_Display_Message( UCMC_DISPLAY, ctMessage,0);

				switch( iLIBUCM_Pinpad_GetChar( UCMC_PPAD ) )
				{
				case T_VAL:
					num_aid_selected = index;
					NoEnd = 1;
					break;

				case T_CORR:
					index++;
					if (index==aAIDCandidateList->count)
					{
						index=0;
					}
					break;

				case T_ANN:
					NoEnd = 2;
					break;

				default:
					break;
				}

			}
			while(NoEnd==0);
		}

		iLIBUCM_Display_Close(UCMC_DISPLAY);
		iLIBUCM_Pinpad_Close(UCMC_PPAD);

		return (CR_ENTRY_OK);
	}
	else
#endif
	{

		if ((TransactionLoopMode == TRUE)&&(loop_amount != 0))
		{
			cr = CR_ENTRY_OK;
			entry_buffer.d_entry[0] = 00;
		}
		else
		{
			//previousUiState = APEMV_UI_TransactionBegin();

			entry_buffer.d_entry[0] = APEMV_UI_MenuSelectAid(title, aAIDCandidateList->count,  (const APEMV_UI_SelectAidItem_t *)AidUI );
			if (entry_buffer.d_entry[0])
				cr = CR_ENTRY_OK ;

			// Restore the user interface environment
			//APEMV_UI_TransactionEnd(previousUiState);


		}

		if (cr == CR_ENTRY_OK)
		{
			num_aid_selected = entry_buffer.d_entry[0];
		}
	}
	}
	// Free the memory
	if (AidUI != NULL)
	{
		ufree(AidUI);
		AidUI = NULL;
	}
	return cr;
}

/*&BEGIN FT/08/2636/MA  Manage Cardholder Confirmation parameter is value is NO */
//===========================================================================
//! \brief This function tests if the cardholder confirmation is possible.
//! \param[in] aAIDCandidateList : list of candidate applications.
//! \return
//! - OK
//! - KO
//===========================================================================
static int Cardholder_Conf_Possible(AIDCandidateList* aAIDCandidateList)
{
	/* 	To ensure that if only one AID is mutually supported, b8 of Application
	priority indicator equals 1, and terminal does not provide for cardholder
	confirmation, the terminal terminates the transaction. */


	if ((aAIDCandidateList->count == 1)&&(aAIDCandidateList->list[0].cardholder == 1))
		return KO;

	return OK;
}
/*&END FT/08/2636/MA  Manage Cardholder Confirmation parameter is value is NO */


//===========================================================================
//! \brief This function tests if the terminal allows AID confirmation by the
//! cardholder.
//! \return
//! - FALSE if the TAG_CUST_IS_CARDHOLDER_CONFIRMATION value is not set.
//! - TRUE otherwise.
//===========================================================================
static int Is_Cardholder_Confirmation_Supported (void)
{
	int found;
	DataElement elt;

	// Reset the selected mark list
	PAR_ResetMarkList();
	// Select Main ICS + ICSx (Maybe unused)
	CUPAR_SelectAdditionnalICS();

	// Get data from TLVTree
	found = PAR_ReadParameter(TAG_CUST_IS_CARDHOLDER_CONFIRMATION, (DataElementExt *)&elt);

	// Search the database number associated with this AID
	if (found)
	{
		if  (elt.ptValue[0]==0x01)
			return TRUE;
		else
			return FALSE;
	}
	return TRUE;
}

//===========================================================================
//! \brief This function allows to select one AID in the candidate list.
//! \param[in] noappli : application number.
//! \param[in] param_in : input DEL containing a list of AID.
//! \param[out] param_out : structure S_AID_OUT containing the fields aid_value,
//! noappli.
//! \return
//! - OK if an AID was successfully selected.
//! - KO otherwise.
//===========================================================================
static int cu_select_aid_extended(NO_SEGMENT noappli,S_SELECT_AID_EXTENDED *param_in,S_AID_OUT *param_out)
{
	int             cr_int;
	int             i,found_aid;
	unsigned short  lg_code;
	char *			pc_l_DisplayTxt;
	unsigned short	iIsCCSupported;
	TLV_TREE_NODE   hTree;


	memclr(&outAIDCandidateList, sizeof(outAIDCandidateList));

	// Copy the filled local _DEL_ object into a local TLV Tree object
	CreateCandidateListTlvTreefromDel((_DEL_ * )&param_in->del, &hTree);

	cu_Build_Candidate_List_TlvTree(&hTree);

	// Release the local TlvTree
	TlvTree_ReleaseEx(&hTree);

	iIsCCSupported = (Is_Cardholder_Confirmation_Supported() == TRUE);

	if (!iIsCCSupported)
	{
		if (Cardholder_Conf_Possible(&outAIDCandidateList)==KO)
		{
			param_out->aid_value= (unsigned char)-1;
			param_out->noappli =noappli;

			return KO ;
		}
	}

	if (iIsCCSupported)
	{
		if (outAIDCandidateList.count != 0)
		{
#ifdef _USE_UCM_
			if(!UCM_Present)
#endif
			{
				if (nb_time_select != 0)
				{
					// display message TRY AGAIN for Aids reselection
					CUDB_Get_cardholder_language (&lg_code);
					pc_l_DisplayTxt = CUMESS_Read_Message(STD_MESS_TRY_AGAIN, lg_code);
					APEMV_UI_MessageDisplay ((char *)pc_l_DisplayTxt);

				}
			}
		}
	}


	// Performs the choose of an AID

	cr_int = Select_AID(&outAIDCandidateList,iIsCCSupported);

	if (cr_int==OK)
	{
		param_out->aid_value= 0;
		found_aid = FALSE;
		i=0;;
		while ((i<NB_MAX_AID_PER_APPLI)&&(found_aid == FALSE))
		{
			if ((param_in->elt[i].lg_libelle == outAIDCandidateList.list[num_aid_selected].lengthAID) &&
				(memcmp(param_in->elt[i].aid,outAIDCandidateList.list[num_aid_selected].AIDValue, outAIDCandidateList.list[num_aid_selected].lengthAID)==0))
				found_aid = TRUE;
			else
				i++;
		}

		param_out->aid_value= i;
	}
	else
	{
		param_out->aid_value= (unsigned char)-1;
		param_out->noappli =noappli;
		return KO ;
	}

	nb_time_select = nb_time_select +1;

	param_out->noappli =noappli;

	return OK ;

}

//===========================================================================
//! \brief This function allows the user to choose an AID.
//! \param[in] phInputTLVTree : input TLV tree containing a list of AID.
//! \param[out] phOutputTLVTree : output TLV tree containing the tag according to
//! the user choice.
//===========================================================================
static void EMVCUST_Menu_Select_TlvTree(TLV_TREE_NODE *phInputTLVTree, TLV_TREE_NODE *phOutputTLVTree)
{
	unsigned short	iIsCCSupported;
	unsigned char StatusCode[2];

	memclr(&outAIDCandidateList, sizeof(outAIDCandidateList));

	cu_Build_Candidate_List_TlvTree(phInputTLVTree);

	if (outAIDCandidateList.count != 0)
	{
		num_aid_selected = 0xff;

		iIsCCSupported = (Is_Cardholder_Confirmation_Supported() == TRUE);

		Select_AID(&outAIDCandidateList,iIsCCSupported);
		if ( num_aid_selected != 0xff)
		{
			// Build an output TLV tree
			TlvTree_AddChild(*phOutputTLVTree, TAG_AID_ICC, (unsigned char *)&outAIDCandidateList.list[num_aid_selected].AIDValue, outAIDCandidateList.list[num_aid_selected].lengthAID);

			// Add tag status code
			ShortToChar(TAG_CUST_PROCESS_COMPLETED, StatusCode);
			TlvTree_AddChild(*phOutputTLVTree, TAG_CUST_STATUS_CODE, &StatusCode, 2);
		}
		else
		{
			// Build an output TLV tree

			// Add tag status code
			ShortToChar(TAG_CUST_TRANSACTION_CANCELLED, StatusCode);
			TlvTree_AddChild(*phOutputTLVTree, TAG_CUST_STATUS_CODE, &StatusCode, 2);

		}

	}

}

//===========================================================================
//! \brief This function allows to select one AID in the candidate list given,
//! as input parameter.
//! \param[in] no : application number.
//! \param[in] param_in : structure S_SELECT_AID containing the candidate list
//! \param[out] param_out : structure S_AID_OUT containing the fields aid_value,
//! noappli.
//! \return
//! - KO if cardholder confirmation is not supported by the terminal.
//! - FCT_OK otherwise.
//===========================================================================
static int cu_select_aid(NO_SEGMENT no,S_SELECT_AID *param_in,S_AID_OUT *param_out)
{
	S_AID_OUT		paramout;
	unsigned short	lg_code;
	char			*pc_l_DisplayTxt;
	unsigned short	iIsCCSupported;
	char			msg[100];

#ifdef _USE_UCM_
	byte			index;
	unsigned char	NoEnd;
	char			ctMessage[200];
	int				UCMKey;
#endif


	memclr(&paramout, sizeof(paramout));

	iIsCCSupported = (Is_Cardholder_Confirmation_Supported() == TRUE);

	if (!iIsCCSupported)
	{
		param_out->aid_value= (unsigned char)-1;
		param_out->noappli =no;
		return KO ;
	}

	paramout.noappli = no;
	if (param_in->nb_appli==1)
	{

		/* &BEGIN FT/07/1755/MA confirmation AID for UCM */

#ifdef _USE_UCM_
		if(UCM_Present)
		{
			unsigned short lg_code;

			index=0;
			NoEnd=0;
			paramout.aid_value = 1;

			iLIBUCM_Display_Open(UCMC_DISPLAY, "w");
			iLIBUCM_Pinpad_Open(UCMC_PPAD, "r*");

			lg_code = PSQ_Give_Language();
			sprintf(ctMessage, "%.16s\n%s",param_in->AID_list[0], CUMESS_Read_Message(DFT_MESS_NB_YESV_NOA, lg_code));
			iLIBUCM_Display_Message( UCMC_DISPLAY, ctMessage,0) ;

			do
			{
				UCMKey = iLIBUCM_Pinpad_GetChar( UCMC_PPAD );
			}
			while( (UCMKey!=T_ANN) && (UCMKey!=T_VAL) );

			if (UCMKey==T_VAL)
			{
				paramout.aid_value = 0;
			}

			iLIBUCM_Display_Close(UCMC_DISPLAY);
			iLIBUCM_Pinpad_Close(UCMC_PPAD);
		}
		else
#endif
		{
			/* &END FT/07/1755/MA confirmation AID for UCM */
			if (nb_time_select != 0)
			{
				// display message TRY AGAIN for Aids reselection
				CUDB_Get_cardholder_language (&lg_code);

				pc_l_DisplayTxt = CUMESS_Read_Message(STD_MESS_TRY_AGAIN, lg_code);

				APEMV_UI_MessageDisplay ((char *)pc_l_DisplayTxt);

			}


				memset(msg, 0, sizeof(msg));
				sprintf(msg, "%s", param_in->AID_list[0]);

				// Default value = no
				paramout.aid_value = 1;

				Menu.tab[0] = (unsigned char *) CUMESS_Read_Message(DFT_MESS_NB_YES ,lg_code);
				Menu.tab[1] = (unsigned char *) CUMESS_Read_Message(DFT_MESS_NB_NO ,lg_code);
				Menu.tab[2] = NULL;

				if ((TransactionLoopMode == TRUE)&&(loop_amount != 0))
				{
					paramout.aid_value = 0;
				}
				else
				{

					if ( APEMV_UI_MenuAcceptRefuse(msg,(const char *) Menu.tab))
					{
							// Approved
						paramout.aid_value = 0;
					}

					// 200s with mask
					/*if (CUTERM_Wait_function_key(b_l_TimeOut, mask, &key) == TRUE)
					{
						if ( (key == T_VAL) ||
							(key == T_SK2))
						{
							paramout.aid_value = 0;
						}
					}*/
				}

		}

	}

	param_out->noappli = no;
	param_out->aid_value = paramout.aid_value;

	return (FCT_OK);
}

typedef struct
{
	unsigned short type_code;
	unsigned short service;
	unsigned long m_ulAmount;
	int m_nCurrencyCode;
} T_PERFORM_PAYMENT;

//===========================================================================
//! \brief This function starts an EMV transaction. This function is called
//! on the engine entry point I_EMVCUST_Perform_Transaction.
//! \param[in] size : size of the data received from engine.
//! \param[in] pPaymentData : T_PERFORM_PAYMENT structure contraining the
//! amount and the currency code to perform the transaction.
//! \return Always FCT_OK.
//===========================================================================
int PerformPayment(unsigned int size, T_PERFORM_PAYMENT* pPaymentData)
{
	CUMORE_DebitEx(pPaymentData->m_ulAmount, pPaymentData->m_nCurrencyCode);
	return FCT_OK;
}

//===========================================================================
//! \brief This function allows to customise the Telium Manager at idle state.
//! It is used to enable usage of CAM2 (CAM2 is the terminal chip reader when
//! a pinpad is connected).
//! \param[in] no : application number.
//! \param[in] p1 : NULL.
//! \param[out] param_out : structure S_STATEOUT containing the fields CAM mask.
//! \return Always FCT_OK.
//===========================================================================
/*& BEGIN FT/08/3144/MA  CAM2 Management*/
int cu_get_idle_state_parameter(NO_SEGMENT no, void *p1,   S_STATEOUT *param_out)
{
	S_STATEOUT        etatout;

	memcpy ((char *)&etatout, (char *)param_out, sizeof(etatout));
	etatout.returned_state[etatout.response_number].no_appli = no;

#ifdef _ENABLE_CAM2_
	etatout.returned_state[etatout.response_number].mask     = CAM0 | CAM2 | SWIPE2;
#else
	etatout.returned_state[etatout.response_number].mask     = CAM0 | SWIPE2;
#endif

	etatout.returned_state[etatout.response_number].key      = 0;
	etatout.response_number++;
	memcpy (param_out, &etatout, sizeof(etatout));
	return (FCT_OK);
}
/*& END FT/08/3144/MA  CAM2 Management*/

//===========================================================================
//! \brief This function allows to customise the Telium Manager after entering an amount.
//! It is used to enable usage of CAM2 (CAM2 is the terminal chip reader when
//! a pinpad is connected).
//! \param[in] no : application number.
//! \param[in] param_in : structure S_TRANSIN.
//! \param[out] param_out : structure S_STATEOUT containing the fields CAM mask.
//! \return Always FCT_OK.
//===========================================================================
/*& BEGIN FT/08/3144/MA  CAM2 Management*/
int cu_get_amount_state_parameter(NO_SEGMENT no, S_TRANSIN *param_in,   S_STATEOUT *param_out)
{
	S_STATEOUT        etatout;

	memcpy ((char *)&etatout, (char *)param_out, sizeof(etatout));
	etatout.returned_state[etatout.response_number].no_appli = no;

#ifdef _ENABLE_CAM2_
	etatout.returned_state[etatout.response_number].mask     = CAM0 | CAM2 | SWIPE2;
#else
	etatout.returned_state[etatout.response_number].mask     = CAM0 | SWIPE2;
#endif

	etatout.returned_state[etatout.response_number].key      = 0;
	etatout.response_number++;
	memcpy (param_out, &etatout, sizeof(etatout));
	return (FCT_OK);
}
/*& END FT/08/3144/MA  CAM2 Management*/

int cu_give_your_specific_context(NO_SEGMENT No, S_SPECIFIC_CONTEXT *param_out)
{
	// Initialise the output parameter
	memset(&param_out->returned_state[param_out->response_number], 0,
		sizeof(param_out->returned_state[param_out->response_number]));

	// The name of the resource file
	strcpy(param_out->returned_state[param_out->response_number].appname, _ING_APPLI_DATA_FILE_BINARY_NAME);
	param_out->returned_state[param_out->response_number].no_appli = No;
	param_out->returned_state[param_out->response_number].mask = 0;
	param_out->returned_state[param_out->response_number].type = 0;
	param_out->returned_state[param_out->response_number].support = 0;
	param_out->returned_state[param_out->response_number].cam = 0;
	param_out->returned_state[param_out->response_number].cgui = CGUI_MASK;

	param_out->response_number++;

	return FCT_OK;
}

//===========================================================================
//! \brief This is the main function. This function treats the manager entry
//! points managed by the custom application.
//! \param[in] size : size of the exchanged data.
//! \param[in,out] data : input/output structure used for exchange between
//! manager and custom.
//! \return 0.
//===========================================================================
int Main(unsigned int size, StructPt *data)
{
	NO_SEGMENT No;


	No=ApplicationGetCurrent();

	switch(data->service)
	{
//	case MORE_FUNCTION :                  // Activated on "F" key and dedicated to navigation menus
//			//trace((trace_sap_id_t) SAP_TRACE_0, strlen(tzMsgTrace[6]), tzMsgTrace[6]); // Trace with trace tool
//			//TRACE_DEBUG(tzMsgTrace[6]);                                                // Trace with remote debugger
//			more_function(No, NULL, NULL);
//	        break;
	case AFTER_RESET :
		cu_after_reset((unsigned char)No, NULL, &data->Param.AfterReset.param_out);
		EMVCUST_Initialise_TlvTree(NULL);
		break;

	case CONSULT :
		//Os__xprinta("CONSULT 3");
		cu_consult((unsigned char)No, NULL, NULL);
		break;

	case STATE :
		//Os__xprinta("STATE 3");
		cu_state((unsigned char)No, NULL, NULL);
		break;

	case IS_CHANGE_INIT :
		cu_is_change_init((unsigned char)No, NULL, &data->Param.IsChangeInit.param_out);
		break;

	case IS_EVOL_PG :
		cu_is_evol_pg((unsigned char)No, NULL, &data->Param.IsEvolPg.param_out);
		break;

	case IS_STATE :
		//Os__xprinta("IS_STATE auto 3");
		cu_is_state((unsigned char)No, NULL, &data->Param.IsState.param_out);
		break;

	case IS_TIME_FUNCTION :
		is_time_function((unsigned char)No, NULL, &data->Param.IsTimeFunction.param_out);
		break;

	case MCALL :
		cu_mcall((unsigned char)No, NULL, NULL);
		break;

	case MODIF_PARAM :
		cu_modif_param((unsigned char)No, &data->Param.ModifParam.param_in, NULL);
		break;

	case IS_FOR_YOU_BEFORE :
		cu_is_for_you_before((unsigned char)No, NULL, &data->Param.IsForYouBefore.param_out);
		break;

	case DEBIT_NON_EMV :
		cu_debit_non_emv((unsigned char)No, &data->Param.DebitNonEmv.param_in, &data->Param.DebitNonEmv.param_out);
		break;
		
	case SELECT_FUNCTION :
		cu_debit_non_emv((unsigned char)No, &data->Param.SelectFunction.param_in, &data->Param.SelectFunction.param_out);
		break;
		
	case FILE_RECEIVED :
        return (cu_file_received((unsigned char)No, &data->Param.FileReceived.param_in, NULL));
		break;

	case GIVE_YOUR_DOMAIN :
		cu_give_your_domain((unsigned char)No, NULL, &data->Param.GiveYourType.param_out);
		break;

	case IS_FOR_YOU_AFTER :
		cu_is_for_you_after((unsigned char)No, &data->Param.IsForYouAfter.param_in, &data->Param.IsForYouAfter.param_out);
		break;

	case IS_DELETE :
		cu_is_delete((unsigned char)No,NULL, &data->Param.IsDelete.param_out);
		break;

	case FALL_BACK :
#ifdef _USE_UCM_
		if(!UCM_Present)
#endif
			cu_fall_back((unsigned char)No, data->Param.FallBack.param_in.entry, data->Param.FallBack.param_in.amount);
		break;

	case SELECT_AID_EXTENDED :
		cu_select_aid_extended((unsigned char)No,&data->Param.SelectAidExtended.param_in,&data->Param.SelectAidExtended.param_out);
		break;

	case SELECT_AID :
		cu_select_aid((unsigned char)No,&data->Param.SelectAid.param_in,&data->Param.SelectAid.param_out);
		break;

	case TRACK_STREAM :
		upload_diagnostic_txt((unsigned char *)"track_stream \n");

    case IDLE_MESSAGE:
		cu_idle_message((unsigned char)No, NULL, NULL);
		break;
		break;

    case TIME_FUNCTION :
		cu_time_function((unsigned char)No, NULL, NULL);
		break;

    case GIVE_AID :
		cu_give_aid((unsigned char)No,NULL,&data->Param.GiveAid.param_out);
		break;

    case IS_CARD_EMV_FOR_YOU:
		cu_is_card_emv_for_you((unsigned char)No, &data->Param.IsCardEmvForYou.param_in,&data->Param.IsCardEmvForYou.param_out) ;
		break;

	case MORE_FUNCTION :
		cu_more_function((unsigned char)No, NULL, NULL);
		break;

	case KEYBOARD_EVENT :
		cu_keyboard_event((unsigned char)No,&data->Param.KeyboardEvent.param_in,&data->Param.KeyboardEvent.param_out);  ;
		break;

	case DEBIT_EMV :
		//Os__xprinta("transaction __Debit");

		cu_debit_emv((unsigned char)No,&data->Param.DebitEmv.param_in,&data->Param.DebitEmv.param_out);
		break;

	case SELECT_FUNCTION_EMV :
		cu_debit_emv((unsigned char)No,&data->Param.SelectFunctionEmv.param_in,&data->Param.SelectFunctionEmv.param_out);
		break;

    case IS_NAME :
        cu_is_name((unsigned char)No,NULL, &data->Param.IsName.param_out);
		break;
	/*&END FT/09/4316/MA  Multi EMV Application*/

	case GET_IDLE_STATE_PARAMETER :
		cu_get_idle_state_parameter((unsigned char)No,NULL,&data->Param.GetIdleStateParameter.param_out);

		break;

	case GET_AMOUNT_STATE_PARAMETER :
		cu_get_amount_state_parameter((unsigned char)No,&data->Param.GetAmountStateParameter.param_in,
			&data->Param.GetAmountStateParameter.param_out);
		break;


	case GIVE_YOUR_SPECIFIC_CONTEXT:
		cu_give_your_specific_context((unsigned char)No,&data->Param.GiveYourSpecificContext.param_out);
		break;
	}

	return (0);
}


#ifdef _USE_UCM_
//===========================================================================
//! \brief
//! \param pdata_p
//! \return
//===========================================================================
static int iAPP_Consult( T_UCMC_IAC_SERVICE *pdata_p )
{
	T_UCMHOST_APP_CONSULT *pConsult;


	pdata_p->uiSize = sizeof( T_UCMHOST_APP_CONSULT );

	pConsult = (void*)&pdata_p->Par.ucBuffer[0];

	ObjectGetInfo(OBJECT_TYPE_APPLI, ApplicationGetCurrent(),&pConsult->sInfo );

	// State of the application
	pConsult->ucAppStatus = UCMHOST_APP_STATUS_INIT_ACTIVE;

	// State of the file
	pConsult->ucFileStatus = UCMHOST_APP_FILE_NOT_EMPTY;

	return( FCT_OK );
}

//===========================================================================
//! \brief
//! \param pdata_p
//! \return
//===========================================================================
static int iAPP_TLC_Status( T_UCMC_IAC_SERVICE *pdata_p )
{

	T_UCMHOST_APP_TLC_STATE *pState;


	pdata_p->uiSize = sizeof( T_UCMHOST_APP_TLC_STATE );

	pState = (void*)&pdata_p->Par.ucBuffer[0];

	ObjectGetInfo( OBJECT_TYPE_APPLI, ApplicationGetCurrent(),&pState->sInfo );

	pState->ucState = UCMHOST_TLC_STATE_OK;
	pState->ucTypeCardStruct = UCMHOST_CARD_EMV;

	return (FCT_OK);
}

//===========================================================================
//! \brief
//! \param pdata_p
//! \return
//===========================================================================
static int iAPP_TLP_Status( T_UCMC_IAC_SERVICE *pdata_p )
{
	int iRet;
	T_UCMHOST_APP_TLP_STATE *pState;

	iRet = FCT_OK;

	pdata_p->uiSize = sizeof( T_UCMHOST_APP_TLC_STATE );

	pState = (void*)&pdata_p->Par.ucBuffer[0];

	ObjectGetInfo( OBJECT_TYPE_APPLI, ApplicationGetCurrent(),&pState->sInfo );

	pState->ucState = UCMHOST_TLC_STATE_OK;
	pState->ucRuf = 0;

	return (FCT_OK);
}

//===========================================================================
//! \brief
//! \param pdata_p
//! \return
//===========================================================================
static int iAPP_TLC_Start( T_UCMC_IAC_SERVICE *pdata_p )
{
	int iRet;
	T_UCMHOST_APP_TLC_START *pState;


	iRet = FCT_OK;

	pdata_p->uiSize = sizeof( T_UCMHOST_APP_TLC_START );
	pState = (void*)&pdata_p->Par.ucBuffer[0];

	ObjectGetInfo( OBJECT_TYPE_APPLI, ApplicationGetCurrent(),&pState->sInfo );

	CUMORE_BatchCapt();

	pState->ucState = UCMHOST_TLC_STATE_OK;
	pState->ucRuf = 0;

	return(FCT_OK);
}

//===========================================================================
//! \brief
//! \param usAppli_p
//! \param pdata_p
//! \return
//===========================================================================
static int iAPPLIUCM_IAC( unsigned short usAppli_p , T_UCMC_IAC_SERVICE *pdata_p )
{
	int iRet;

	iRet = FCT_OK;

	switch( pdata_p->usFct )
	{
	case UCMHOST_FCTAPP_CONSULT:
		iRet = iAPP_Consult( pdata_p );
		break;

	case UCMHOST_FCTAPP_TLP_STATUS:
		iRet = iAPP_TLP_Status( pdata_p ) ;
		break;

	case UCMHOST_FCTAPP_TLC_STATUS:
		iRet = iAPP_TLC_Status( pdata_p ) ;
		break;

	case UCMHOST_FCTAPP_TLC_START:
		iRet = iAPP_TLC_Start( pdata_p ) ;
		break;

	default:
		iRet = UCMERR_IAC_NO_FCT;
		break;
	}
	return( iRet ) ;
}

//===========================================================================
//! \brief
//! \param iSize_p
//! \param pdata_p
//! \return
//===========================================================================
int MainUCMC_IAC(unsigned int iSize_p, T_UCMC_IAC_SERVICE *pdata_p)
{
	int          iRet;
	object_info_t info;


	ObjectGetInfo( OBJECT_TYPE_APPLI, pdata_p->usType_code, &info ) ;

	iRet = FCT_OK;

	switch( pdata_p->usService )
	{
	case SERVICE_APPLI_UCMC:
		switch( pdata_p->usModule )
		{
		case UCMAPPLIMODULE_FCTAPP:
			iRet = iAPPLIUCM_IAC( pdata_p->usType_code, pdata_p ) ;
			break;

		default:
			iRet = UCMERR_IAC_NO_MODULE;
			break;
		}
		break;

	default:
		iRet = UCMERR_IAC_NO_SERVICE;
		break;
	}

	return( iRet );
}
#endif

//===========================================================================
//! \brief This function defines the services managed by the application.
//! \param[in] no : application type.
//! \param p1 : not used.
//! \param p2 : not used.
//! \return Always FCT_OK.
//===========================================================================
int give_interface(unsigned short no,void *p1,void *p2)
{
	/*& BEGIN SMO/ETU/04/20286/MA : Do not use pinpad(PPC30 and PPR30) as secret area key storage */
	Telium_service_desc_t MesServices[60];
	int i;

	memset(g_szAppName, 0, sizeof(g_szAppName));

	i = 0;
	//{ 0, MORE_FUNCTION,                 (Telium_callback)Main, SERVICES_HIGH_PRIORITY }
//	MesServices[i].appli_id = no;
//	MesServices[i].serv_id = MORE_FUNCTION;
//	MesServices[i].sap = (Telium_callback) Main;
//	MesServices[i++].priority = 30; /* 25 30*/

	MesServices[i].appli_id  = no;
	MesServices[i].serv_id   = AFTER_RESET;
	MesServices[i].sap       = (Telium_callback) Main;
	MesServices[i++].priority  = 30; /* 25 */

	MesServices[i].appli_id  = no;
	MesServices[i].serv_id   = CONSULT;
	MesServices[i].sap       = (Telium_callback) Main;
	MesServices[i++].priority  = 30;

	MesServices[i].appli_id  = no;
	MesServices[i].serv_id   = STATE;
	MesServices[i].sap       = (Telium_callback) Main;
	MesServices[i++].priority  = 30;

	MesServices[i].appli_id  = no;
	MesServices[i].serv_id   = IS_CHANGE_INIT;
	MesServices[i].sap       = (Telium_callback) Main;
	MesServices[i++].priority  = 30;

	MesServices[i].appli_id  = no;
	MesServices[i].serv_id   = IS_EVOL_PG;
	MesServices[i].sap       = (Telium_callback) Main;
	MesServices[i++].priority  = 30;

	MesServices[i].appli_id  = no;
	MesServices[i].serv_id   = IS_STATE;
	MesServices[i].sap       = (Telium_callback) Main;
	MesServices[i++].priority  = 30;

	MesServices[i].appli_id  = no;
	MesServices[i].serv_id   = IS_TIME_FUNCTION;
	MesServices[i].sap       = (Telium_callback) Main;
	MesServices[i++].priority  = 30;

	MesServices[i].appli_id  = no;
	MesServices[i].serv_id   = MCALL;
	MesServices[i].sap       = (Telium_callback) Main;
	MesServices[i++].priority  = 30;

	MesServices[i].appli_id  = no;
	MesServices[i].serv_id   = MODIF_PARAM;
	MesServices[i].sap       = (Telium_callback) Main;
	MesServices[i++].priority  = 30;

	MesServices[i].appli_id  = no;
	MesServices[i].serv_id   = IS_FOR_YOU_BEFORE;
	MesServices[i].sap       = (Telium_callback) Main;
	MesServices[i++].priority  = 30;

	MesServices[i].appli_id  = no;
	MesServices[i].serv_id   = DEBIT_NON_EMV;
	MesServices[i].sap       = (Telium_callback) Main;
	MesServices[i++].priority  = 30;

	MesServices[i].appli_id  = no;
	MesServices[i].serv_id   = FILE_RECEIVED;
	MesServices[i].sap       = (Telium_callback) Main;
	MesServices[i++].priority  = 30;

	MesServices[i].appli_id  = no;
	MesServices[i].serv_id   = GIVE_YOUR_DOMAIN;
	MesServices[i].sap       = (Telium_callback) Main;
	MesServices[i++].priority  = 30;

	MesServices[i].appli_id  = no;
	MesServices[i].serv_id   = IS_FOR_YOU_AFTER;
	MesServices[i].sap       = (Telium_callback) Main;
	MesServices[i++].priority  = 30;

	/*& BEGIN FT/1393/06/MG : Add a service to ENGINE to start an EMV transaction */
	MesServices[i].appli_id  = no;
	MesServices[i].serv_id   = I_EMVCUST_Perform_Transaction;
	MesServices[i].sap       = (Telium_callback) PerformPayment;
	MesServices[i++].priority  = 30;
	/*& END FT/1393/06/MG */

	/* MesServices[i].appli_id  = no;
	MesServices[i].serv_id   = I_EMVCUST_Initialise_TlvTree;
	MesServices[i].sap       = (SAP) Main_EMVCUST_TlvTree;
	MesServices[i++].priority  = 30;*/

	MesServices[i].appli_id  = no;
	MesServices[i].serv_id   = I_EMVCUST_Get_Global_Param_TlvTree;
	MesServices[i].sap       = (Telium_callback) Main_EMVCUST_TlvTree;
	MesServices[i++].priority  = 30;

	MesServices[i].appli_id  = no;
	MesServices[i].serv_id   = I_EMVCUST_Get_AID_List_TlvTree;
	MesServices[i].sap       = (Telium_callback) Main_EMVCUST_TlvTree;
	MesServices[i++].priority  = 30;

	MesServices[i].appli_id  = no;
	MesServices[i].serv_id   = I_EMVCUST_Get_AID_DATA_TlvTree;
	MesServices[i].sap       = (Telium_callback) Main_EMVCUST_TlvTree;
	MesServices[i++].priority  = 30;

	MesServices[i].appli_id  = no;
	MesServices[i].serv_id   = I_EMVCUST_Get_AID_Param_TlvTree;
	MesServices[i].sap       = (Telium_callback) Main_EMVCUST_TlvTree;
	MesServices[i++].priority  = 30;

	/* MesServices[i].appli_id  = no;
	MesServices[i].serv_id   = I_EMVCUST_More_Function_TlvTree;
	MesServices[i].sap       = (SAP) Main_EMVCUST_TlvTree;
	MesServices[i++].priority  = 30;

	MesServices[i].appli_id  = no;
	MesServices[i].serv_id   = I_EMVCUST_Periodic_Call_TlvTree;
	MesServices[i].sap       = (SAP) Main_EMVCUST_TlvTree;
	MesServices[i++].priority  = 30;*/

	MesServices[i].appli_id  = no;
	MesServices[i].serv_id   = I_EMVCUST_Process_Step_TlvTree;
	MesServices[i].sap       = (Telium_callback) Main_EMVCUST_TlvTree;
	MesServices[i++].priority  = 30;

//	MesServices[i].appli_id  = no;                 //larbi1
//	MesServices[i].serv_id   = I_EMVCUST_Display_Message_TlvTree;
//	MesServices[i].sap       = (Telium_callback) Main_EMVCUST_TlvTree;
//	MesServices[i++].priority  = 30;

	MesServices[i].appli_id  = no;
	MesServices[i].serv_id   = I_EMVCUST_Get_Last_Transaction_TlvTree;
	MesServices[i].sap       = (Telium_callback) Main_EMVCUST_TlvTree;
	MesServices[i++].priority  = 30;

	MesServices[i].appli_id  = no;
	MesServices[i].serv_id   = I_EMVCUST_Voice_Referral_TlvTree;
	MesServices[i].sap       = (Telium_callback) Main_EMVCUST_TlvTree;
	MesServices[i++].priority  = 30;

	/* MesServices[i].appli_id  = no;
	MesServices[i].serv_id   = I_EMVCUST_Get_AID_TlvTree;
	MesServices[i].sap       = (SAP) Main_EMVCUST_TlvTree;
	MesServices[i++].priority  = 30;

	MesServices[i].appli_id  = no;
	MesServices[i].serv_id   = I_EMVCUST_KeyManagement_TlvTree;
	MesServices[i].sap       = (SAP) Main_EMVCUST_TlvTree;
	MesServices[i++].priority  = 30;*/

	MesServices[i].appli_id  = no;
	MesServices[i].serv_id   = I_EMVCUST_Authorization_TlvTree;
	MesServices[i].sap       = (Telium_callback) Main_EMVCUST_TlvTree;
	MesServices[i++].priority  = 30;

	MesServices[i].appli_id  = no;
	MesServices[i].serv_id   = I_EMVCUST_Choose_Language_Menu_TlvTree;
	MesServices[i].sap       = (Telium_callback) Main_EMVCUST_TlvTree;
	MesServices[i++].priority  = 30;

	// FT/06/473/MA : selection menu to be done in Custom
	MesServices[i].appli_id  = no;
	MesServices[i].serv_id   = I_EMVCUST_Menu_Select_TlvTree;
	MesServices[i].sap       = (Telium_callback) Main_EMVCUST_TlvTree;
	MesServices[i++].priority  = 30;

	// FT/1126/06/MA : Account Type Selection
	MesServices[i].appli_id  = no;
	MesServices[i].serv_id   = I_EMVCUST_Choose_Account_Type_Menu_TlvTree;
	MesServices[i].sap       = (Telium_callback) Main_EMVCUST_TlvTree;
	MesServices[i++].priority  = 30;

	// FT/2543/07/MA : link ICSx.par file to AIDx.par file
	MesServices[i].appli_id  = no;
	MesServices[i].serv_id   = I_EMVCUST_Get_AID_ICS_TlvTree;
	MesServices[i].sap       = (Telium_callback) Main_EMVCUST_TlvTree;
	MesServices[i++].priority  = 30;

	// SMO/ETU/04/20286/MA : Do not use pinpad(PPC30 and PPR30) as secret area key storage
	MesServices[i].appli_id  = no;
	MesServices[i].serv_id   = IS_DELETE;
	MesServices[i].sap       = (Telium_callback) Main;
	MesServices[i++].priority  = 30;

	// SMO/ETU/05/20090/LB
	MesServices[i].appli_id  = no;
	MesServices[i].serv_id   = FALL_BACK;
	MesServices[i].sap       = (Telium_callback) Main;
	MesServices[i++].priority  = 30;

	// SMO/ETU/05/20383/LB management of select_aid_extended
	MesServices[i].appli_id  = no;
	MesServices[i].serv_id   = SELECT_AID_EXTENDED;
	MesServices[i].sap       = (Telium_callback)Main;
	MesServices[i++].priority  = 30;

	MesServices[i].appli_id  = no;
	MesServices[i].serv_id   = TRACK_STREAM;
	MesServices[i].sap       = (Telium_callback) Main;
	MesServices[i++].priority  = 30;

	// FT/732/06  management of select_aid for 1 aid confirmation
	MesServices[i].appli_id  = no;
	MesServices[i].serv_id   = SELECT_AID;
	MesServices[i].sap       = (Telium_callback)Main;
	MesServices[i++].priority  = 30;

#ifdef _USE_UCM_
	MesServices[i].appli_id  = no  ;
	MesServices[i].serv_id   = SERVICE_APPLI_UCMC;
	MesServices[i].sap       = (T_SERVICE_FUNCTION)MainUCMC_IAC;
	MesServices[i++].priority  = 30;
#endif

	MesServices[i].appli_id  = no;
	MesServices[i].serv_id   = TIME_FUNCTION;
	MesServices[i].sap       = (Telium_callback) Main;
	MesServices[i++].priority  = 30;

	MesServices[i].appli_id  = no;
	MesServices[i].serv_id   = GIVE_AID;
	MesServices[i].sap       = (Telium_callback) Main;
	MesServices[i++].priority  = 30;

	MesServices[i].appli_id  = no;
	MesServices[i].serv_id   =IS_CARD_EMV_FOR_YOU ;
	MesServices[i].sap       = (Telium_callback) Main;
	MesServices[i++].priority  = 30;

	MesServices[i].appli_id  = no;
	MesServices[i].serv_id   = MORE_FUNCTION;
	MesServices[i].sap       = (Telium_callback) Main;
	MesServices[i++].priority  = 30;


	MesServices[i].appli_id  = no;
	MesServices[i].serv_id   =KEYBOARD_EVENT ;
	MesServices[i].sap       = (Telium_callback) Main;
	MesServices[i++].priority  = 30;

	MesServices[i].appli_id  = no;
	MesServices[i].serv_id   =DEBIT_EMV ;
	MesServices[i].sap       = (Telium_callback) Main;
	MesServices[i++].priority  = 30;

	MesServices[i].appli_id  = no;
	MesServices[i].serv_id   =SELECT_FUNCTION_EMV ;
	MesServices[i].sap       = (Telium_callback) Main;
	MesServices[i++].priority  = 30;

	MesServices[i].appli_id  = no;
	MesServices[i].serv_id   =SELECT_FUNCTION ;
	MesServices[i].sap       = (Telium_callback) Main;
	MesServices[i++].priority  = 30;
	
	MesServices[i].appli_id  = no;
	MesServices[i].serv_id   =IS_NAME ;
	MesServices[i].sap       = (Telium_callback) Main;
	MesServices[i++].priority  = 20;
	/*&END FT/09/4316/MA  Multi EMV Application*/

	MesServices[i].appli_id  = no;
	MesServices[i].serv_id   = GET_IDLE_STATE_PARAMETER;
	MesServices[i].sap       = (Telium_callback)Main;
	MesServices[i++].priority  = 30;

	MesServices[i].appli_id  = no;
	MesServices[i].serv_id   = GET_AMOUNT_STATE_PARAMETER;
	MesServices[i].sap       = (Telium_callback)Main;
	MesServices[i++].priority  = 30;

	MesServices[i].appli_id  = no;
	MesServices[i].serv_id   = GIVE_YOUR_SPECIFIC_CONTEXT;
	MesServices[i].sap       = (Telium_callback)Main;
	MesServices[i++].priority  = 30;

    MesServices[i].appli_id  = no;
    MesServices[i].serv_id   = IDLE_MESSAGE;
	MesServices[i].sap       = (Telium_callback)Main;
	MesServices[i++].priority  = 30;

	MesServices[i].appli_id  = no;
	MesServices[i].serv_id   = CUSTOMIZE_MESSAGE;
	MesServices[i].sap       = (Telium_callback)ServiceCustom_messages;
	MesServices[i++].priority  = 20;


	Telium_ServiceRegister(i,MesServices);

	return (FCT_OK);
}

//===========================================================================
//! \brief This function is the entry point for the interface with the manager.
//===========================================================================
void entry(void)
{
	object_info_t info;

	ObjectGetInfo(OBJECT_TYPE_APPLI, ApplicationGetCurrent(),&info);
	give_interface(info.application_type, NULL, NULL);

}

//! @}

