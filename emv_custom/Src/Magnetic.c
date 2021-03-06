//****************************************************************************
//       INGENICO                                INGEDEV 7
//============================================================================
//       FILE  MAGNETIC.C                        (Copyright INGENICO 2008)
//============================================================================
//  Created :       12-May-2008     Kassovic
//  Last modified : 12-May-2008     Kassovic
//  Module : TRAINING
//
//  Purpose :
//  Magnetic stripe management:
//              Print Iso1/Iso2/Iso3 tracks
//
//  List of routines in file :
//      IsoError : Iso error analysis.
//      Magnetic : Magnetic stripe demo.
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
#include "oem_clemv.h"
#include "Goal.h"


//typedef struct stImgColor
//{
//	T_GL_COLOR ulPen;          // Pen color
//	bool bGrowWidth;           // Background (width) will fill the complete cell
//	ST_FONT xFont;             // Widget font properties
//	T_GL_DIRECTION eExpand;    // Widget to expand
//	ST_BORDER xBorder;         // Widget border properties
//	ST_MARGIN xMargin;         // Widget margin properties
//}ST_IMG_COLOR;

//typedef struct stImgBW
//{
//	bool bGrowWidth;           // Background (width) will fill the complete cell
//	ST_FONT xFont;             // Widget font properties
//	T_GL_DIRECTION eExpand;    // Widget to expand
//	ST_BORDER xBorder;         // Widget border properties
//	ST_MARGIN xMargin;         // Widget margin properties
//}ST_IMG_BW;

// Properties of image (Goal)
// =======================================
static const ST_IMAGE m_xImage480 =
{
	{GL_COLOR_WHITE, TRUE, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_MEDIUM}, GL_DIRECTION_NONE, {0, 0, 0, 0, GL_COLOR_RED}, {70, 70, 70, 70} },
	{TRUE, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_MEDIUM}, GL_DIRECTION_NONE, {1, 1, 1, 1, GL_COLOR_BLACK}, {2, 1, 2, 1} }
};

static const ST_IMAGE m_xImage240 =
{
	{GL_COLOR_BLACK, TRUE, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_MEDIUM}, GL_DIRECTION_NONE, {0, 0, 0, 0, GL_COLOR_RED}, {20, 20, 20, 20} },
	{TRUE, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_MEDIUM}, GL_DIRECTION_NONE, {1, 1, 1, 1, GL_COLOR_BLACK}, {2, 1, 2, 1} }
};

static const ST_IMAGE m_xImage2_480 =
{
	{GL_COLOR_BLACK, NULL, {"LUCIDA", GL_FONT_STYLE_NORMAL, GL_SCALE_MEDIUM}, GL_DIRECTION_NONE, {0, 0, 0, 0, GL_COLOR_RED}, {100, 0, 100, 100} },
	{TRUE, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_MEDIUM}, GL_DIRECTION_NONE, {1, 1, 1, 1, GL_COLOR_BLACK}, {2, 1, 2, 1} }
};

static const ST_IMAGE m_xImage2_240 =
{
	{GL_COLOR_BLACK, NULL, {"LUCIDA", GL_FONT_STYLE_NORMAL, GL_SCALE_MEDIUM}, GL_DIRECTION_NONE, {0, 0, 0, 0, GL_COLOR_RED}, {50, 0, 50, 50} },
	{TRUE, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_MEDIUM}, GL_DIRECTION_NONE, {1, 1, 1, 1, GL_COLOR_BLACK}, {2, 1, 2, 1} }
};


//****************************************************************************
//      EXTERN
//****************************************************************************
extern Telium_File_t *hKbd;      // Handle of the peripheral Keyboard

extern int isFallbackAllowed;

int PinOnlinereboot = 0;//belhadeg

extern unsigned char g_numero_carte[20];

//****************************************************************************
//      PRIVATE CONSTANTS
//****************************************************************************
    /* */

//****************************************************************************
//      PRIVATE TYPES
//****************************************************************************
    /* */

//****************************************************************************
//      PRIVATE DATA
//****************************************************************************
    /* */

void test_rf3(char*);


int Transaction_Magnetique(char *t1,char *t2,char *t3)
{
	uchar	PtIso[MAXPTISO];
	uchar	status;


	memset(&E_Transac8583,0x00,sizeof(E_Transac8583));
	memset(&R_Transac8583,0x00,sizeof(R_Transac8583));
	memset(&StrLineTra ,0x00,sizeof(StrLineTra));

	if(isFallbackAllowed)
	{
		StructInfoPayment.Fallback = 1;
		isFallbackAllowed = FALSE;
	}

	StructInfoPayment.puce_out = 1;

	StructInfoPayment.Entry_Mode = TRS_MAG;

	memcpy(&PtIso[0],t1,50);
	memcpy(&PtIso[51],t2,49); //[50] sentinelle

	//APEMV_UI_MessageDisplayDelay ("Recup_Info_Card.avant", 1);

	status = Recup_Info_Card(PtIso);

	//APEMV_UI_MessageDisplayDelay ("Recup_Info_Card.apres", 1);


	if(StructInfoPayment.CodeFunction == VOIDCODE)
	{
		return OK;
	}
	else if (StructInfoPayment.CodeFunction == PREAUTHCOMPCODE)
	{
		if(memcmp(g_numero_carte+1,StructInfoPayment.Holder_Card+1,StructInfoPayment.Holder_Card[0]))
		{
			CUMORE_CMI_AFFICHE_MESSAGE(179,16,1000);
			return OK;
		}
	}

	if(status == OK){
		//Os__xprintaa("01");
		status = transa_client();
		//Os__xprintaa("02");
		 PinOnlinereboot = 1;
	}else{
		//retourner ? la caisse l'erreur
	}
	return status;
}

//****************************************************************************
//                 void IsoError (int iSta, char *pcTrk)
//  This function analyzes the Iso error.
//  This function has parameters:
//     iSta (I-) : Iso error
//     pcTrk (I-) : Track buffer error
//  This function has no return value.
//****************************************************************************

void IsoError(int iSta, char *pcTrk)
{
	switch (iSta)
	{
		case DEF_SEP: strcpy(pcTrk, "Separator fault");  break;
		case DEF_PAR: strcpy(pcTrk, "Parity fault");     break;
		case DEF_LRC: strcpy(pcTrk, "LRC fault");        break;
		case DEF_LUH: strcpy(pcTrk, "Luhn fault");       break;
		case DEF_NUM: strcpy(pcTrk, "Numerical fault");  break;
		case NO_DATA: strcpy(pcTrk, "No readable data"); break;
		default:      strcpy(pcTrk, "Unknown error");    break;
	}
}

//****************************************************************************
//                          void Magnetic (void)
//  This function returns the contain of the magnetic stripe ISO1-ISO2-ISO3.
//    - fopen() : open Iso13, Iso2, Iso3 channels
//    - ttestall() : Wait for card swiped from mag peripheral
//    - is_iso1-2-3() : Retrieve Iso track in ascii format
//    - fclose() : close Iso13, Iso2, Iso3 channels
//  This function has no parameters.
//  This function has no return value.
//****************************************************************************

void WaitBothMagneticAndCam0(char *pCam0)
{
	// Local variables
    // ***************
    Telium_File_t *hMag13=NULL, *hMag2=NULL, *hMag3=NULL, *hCam0=NULL, *keyboard=NULL;
	byte ucLen=0;
    int iSta, iNum=0;
    char tcTmp[128];
    char *pcSrc, *pcDst, cKey;
    char tcTrk1[128], tcTrk2[128], tcTrk3[128];
    char tcDisplay[25+1];
	int iRet;
	int  Ret;
	long lTimeOut;
	unsigned int nTimeout;
	unsigned int nNumOfCards;
	int  nResult = CL_KO;


	char AFFMONT[48];
	char DEVALP[4];

	if(*pCam0 == 5) //basculer vers la saisie manuelle
	{
		//Os__xprintaa("0001");
		goto lblEnd;
	}
	//Os__xprintaa("0002");
	PinOnlinereboot = 0;
	// Swipe card in progress
	// **********************
	strcpy(tcTrk1, "No readable data");                                       // By default
	strcpy(tcTrk2, "No readable data");
	strcpy(tcTrk3, "No readable data");

	// Open peripherals
	// ================
    hMag2 = Telium_Fopen("SWIPE2", "r*"); CHECK(hMag2!=NULL, lblKO);          // Open "mag2" channel (must be supported, check return code)
	hMag13 = Telium_Fopen("SWIPE31", "r*");                                   // Open "mag13" channel (Could not be supported)
    hMag3 = Telium_Fopen("SWIPE3", "r*");                                     // Open "mag3" channel (Could not be supported)
	hCam0 = Telium_Fopen("CAM0", "rw");

	keyboard = Telium_Fopen("KEYBOARD", "r");

//    _clrscr();                                                              // Clear screen
//	DisplayMedium(0, 0, "Swipe a card...");                                   // Display message
//    //APEMV_UI_MessageDisplayDelay ("Attente Carte.1..\nMAG ou CHIP", 1); 	  //Test17
	if(!memcmp(StructInfoCaisse[0].DE,"504",3))
		sprintf(DEVALP,"%s","MAD");
	else if(!memcmp(StructInfoCaisse[0].DE,"978",3))
		sprintf(DEVALP,"%s","EUR");
	else if(!memcmp(StructInfoCaisse[0].DE,"840",3))
		sprintf(DEVALP,"%s","USD");
	else
		sprintf(DEVALP,"%s","ERR");


	//Telium_Fclose(hKbd);
	//Telium_Ttestall(0,100);

/*
	if(StructInfoPayment.Fallback == 1)
		sprintf(AFFMONT,"FALL BACK\n/CONTACT\n%s\n%d,%.02d%s","MONTANT :                        ",atol(StructInfoCaisse[0].MT)/100,atol(StructInfoCaisse[0].MT)%100,DEVALP);
	else
		sprintf(AFFMONT,"ATTENTE CARTE\nCONTACT\n%s\n%d,%.02d%s","MONTANT :                        ",atol(StructInfoCaisse[0].MT)/100,atol(StructInfoCaisse[0].MT)%100,DEVALP);

		StructInfoPayment.Transaction_Amount = entered_amount;
		memcpy(StructInfoPayment.Devise_acc.code_num,"504",3);
		memcpy(StructInfoPayment.Devise_acc.code_alph,"MAD",3);
		StructInfoPayment.Devise_acc.Exposant = 2;

*/
	GL_GraphicLib_SetCharset(APEMV_UI_GoalHandle(), GL_ENCODING_ISO_8859_1);

	if(StructInfoPayment.Fallback == 1)
		sprintf(AFFMONT,"FALL BACK\n%s%d,%.02d%s","MONTANT :",StructInfoPayment.Transaction_Amount/100,StructInfoPayment.Transaction_Amount%100,StructInfoPayment.Devise_acc.code_alph);
	else
		//sprintf(AFFMONT,"Ins?rer la carte\n%s%d,%.02d%s","MONTANT :",StructInfoPayment.Transaction_Amount/100,StructInfoPayment.Transaction_Amount%100,StructInfoPayment.Devise_acc.code_alph);
		sprintf(AFFMONT,"Pr?sentez Carte");


//	T_GL_HWIDGET hPicture=NULL;
//
//	hPicture = GoalCreatePicture(APEMV_UI_GoalHandle());                             // Create and clear screen
//	GoalDspPicture(hPicture, AFFMONT, "file://flash/HOST/inserer_la_carte_svp.png", &m_xImage480, (GL_TIME_SECOND) * 0, true);
//	GoalDestroyPicture(&hPicture);

	//MessageWithDelay(AFFMONT, "file://flash/HOST/inserer_la_carte_svp.png", 30000);
	MessageWithoutConfirmation(AFFMONT, "file://flash/HOST/inserer_la_carte_svp.png");
	//APEMV_UI_MessageDisplayDelay (AFFMONT, 0); 		//Test17


	//hKbd = Telium_Fopen("KEYBOARD", "r*");
	//Telium_Ttestall(0,100);


	// Wait for events
	// ===============
	//Telium_Reset_buf(hKbd, _receive_id);                                      // Reset keyboard FIFO

	//*pCam0 = 2;

	FicPara.AutoReversalOK =  4;

	Ret = TimerStart(0, 30*1000);                                            // Timer0 starts
		CHECK(Ret>=0, lblEnd);

	do
	{
		////APEMV_UI_MessageDisplayDelay ("222", 1);
		if(StructInfoPayment.Fallback == 1)
			iSta = Telium_Ttestall(KEYBOARD | SWIPE31 | SWIPE2 | SWIPE3, 100); // Wait for the first event ISO1	/*1F1*/
		else if(StructInfoPayment.CodeFunction == PREAUTHCODE){
			iSta = Telium_Ttestall(KEYBOARD | CLESS | CAM0 | SWIPE31 | SWIPE2 | SWIPE3, 100); // Wait for the first event ISO1	/*1F1*/
		}
		else
			iSta = Telium_Ttestall(KEYBOARD | CLESS | CAM0 | SWIPE31 | SWIPE2 | SWIPE3, 100); // Wait for the first event ISO1	/*1F1*/

		////APEMV_UI_MessageDisplayDelay ("333", 1);
//		iSta |= Telium_Ttestall(iSta ^ (SWIPE31 | SWIPE2 | SWIPE3), 10);      // Wait for the second event ISO2
//		iSta |= Telium_Ttestall(iSta ^ (SWIPE31 | SWIPE2 | SWIPE3), 10);      // Wait for the third event ISO3

        //CHECK(iSta!=0, lblEnd);                                               // Exit if nothing swiped after 30s
        if(iSta & KEYBOARD)                                                   // Key event
		{
        	////APEMV_UI_MessageDisplayDelay ("444", 1);

        	int vint = Telium_Getc(keyboard);

        	//traceInt(vint);

        	//MessageWithDelay("", NULL, 1000);
        	if (vint == T_ANN )
        	{
        		*pCam0 = 4;
        		goto lblEnd;
        	}
        	else if (vint == T_CORR)
        	{
        		//traceString("TRACE 1");
//        		MessageWithDelay("Transaction sans contacte", NULL, 1000);
        		//*pCam0 = 2;
        		goto lblEnd;
        	}
        	else if (	(vint == T_NUM0) ||(vint == T_NUM1) ||(vint == T_NUM2) ||(vint == T_NUM3) ||(vint == T_NUM4)
        			||	(vint == T_NUM5) ||(vint == T_NUM6) ||(vint == T_NUM7) ||(vint == T_NUM8) ||(vint == T_NUM9)
			)
        	{
        		//traceInt(vint);
        		continue;
        	}

		}
        else if(iSta & CAM0)                                                  // Key event
		{
        	////APEMV_UI_MessageDisplayDelay ("555", 1);
        	*pCam0 = 1;
        	goto lblEnd;
		}
        // commented by jrizki 180219
        else if(iSta & CLESS)                                                  // Key event
			{
				//APEMV_UI_MessageDisplayDelay ("666", 1);
				//APEMV_UI_MessageDisplayDelay ("CLESS....\nDetected", 1);
				*pCam0 = 2;
				goto lblEnd;
			}
        else if((iSta & SWIPE31)  || (iSta & SWIPE2) || (iSta & SWIPE3))
			{
				break;
			}else {

				ClessEmv_OpenDriver();
				nTimeout=1;
	        	nResult = ClessEmv_DetectCards(CL_TYPE_AB, &nNumOfCards,nTimeout);

	        	//! ok
	        	if (nResult == CL_OK)
	        	{
	        		*pCam0 = 2;
	        		goto lblEnd;
	        	}
	        	else
	        	{
	        		//APEMV_UI_MessageDisplayDelay ("TASK A KO", 2);
	        	}

	        	ClessEmv_CloseDriver();
			}
        lTimeOut = TimerGet(0);
	}while (lTimeOut>0);

	// Retrieve and analyze ISO1
	// =========================
	if(iSta & SWIPE31)
	{
		memset(tcTmp, 0, sizeof(tcTmp));
	    memset(tcTrk1, 0, sizeof(tcTrk1));
		iRet = Telium_Is_iso1(hMag13, &ucLen, (byte*)tcTmp);                  // *** Read ISO1 to ascii format ***
		if (iRet != ISO_OK)
			IsoError (iRet, tcTrk1);
		else
		{
			CHECK(strlen(tcTmp)<128, lblKO);
			iNum++;
			pcSrc = tcTmp;
			pcDst = tcTrk1;
			while(*pcSrc) {                                                   // Find start sentinel
				if(*pcSrc++ == '%')
					break;
			}
			while(*pcSrc) {                                                   // Copy all data between start and end sentinels
				if(*pcSrc == '?')
					break;
				*pcDst++ = *pcSrc++;
			}
		}
	}

	// Retrieve and analyze ISO2
	// =========================
	if(iSta & SWIPE2)
	{
		memset(tcTmp, 0, sizeof(tcTmp));
	    memset(tcTrk2, 0, sizeof(tcTrk2));
		iRet = Telium_Is_iso2(hMag2, &ucLen, (byte*)tcTmp);                   // *** Read ISO2 to ascii format ***
		if (iRet != ISO_OK)
			IsoError (iRet, tcTrk2);
		else
		{
			CHECK(strlen(tcTmp)<128, lblKO);
			iNum++;
			pcSrc = tcTmp;
			pcDst = tcTrk2;
			while(*pcSrc) {                                                   // Find start sentinel
				if(*pcSrc++ == 'B')
					break;
			}
			while(*pcSrc) {                                                   // Copy all data between start and end sentinels
				if(*pcSrc == 'F')
				{
					*pcDst++ = *pcSrc++;
					break;
				}
				if(*pcSrc == 'D')
					*pcSrc = '=';
				*pcDst++ = *pcSrc++;
			}
		}
		*pCam0 = 0;
	}

	// Retrieve and analyze ISO3
	// =========================
	if(iSta & SWIPE3)
	{
		memset(tcTmp, 0, sizeof(tcTmp));
	    memset(tcTrk3, 0, sizeof(tcTrk3));
		iRet = Telium_Is_iso3(hMag3, &ucLen, (byte*)tcTmp);                   // *** Read ISO3 to ascci format ***
		if (iRet != ISO_OK)
			IsoError (iRet, tcTrk3);
		else
		{
			CHECK(strlen(tcTmp)<128, lblKO);
			iNum++;
			pcSrc = tcTmp;
			pcDst = tcTrk3;
			while(*pcSrc) {                                                   // Find start sentinel
				if(*pcSrc++ == 'B')
					break;
			}
			while(*pcSrc) {                                                   // Copy all data between start and end sentinels
				if(*pcSrc == 'F')
					break;
				if(*pcSrc == 'D')
					*pcSrc = '=';
				*pcDst++ = *pcSrc++;
			}
		}
		*pCam0 = 0;
	}
	if (tcTrk1[0]==0) strcpy(tcTrk1, "Empty");
	if (tcTrk2[0]==0) strcpy(tcTrk2, "Empty");
	if (tcTrk3[0]==0) strcpy(tcTrk3, "Empty");

	// Display tracks
	// ==============
//	_clrscr();                                                                // Clr Screen
//	DisplaySmall(0, 0, "Track1:");
//	sprintf(tcDisplay, "%.25s", tcTrk1);                                      // Display part of track1
//	DisplaySmall(1, 0, tcDisplay);
//  DisplaySmall(2, 0, "Track2:");
//  sprintf(tcDisplay, "%.25s", tcTrk2);                                      // Display part of track2
//    //APEMV_UI_MessageDisplayDelay (tcDisplay, 1); 					  			//Test17
//    DisplaySmall(3, 0, tcDisplay);
//    DisplaySmall(4, 0, "Track3:");
//    sprintf(tcDisplay, "%.25s", tcTrk3);                                      // Display part of track3
//    DisplaySmall(5, 0, tcDisplay);
//    DisplaySmall(7, 6, "Press a key");

	// Print tracks
	// ============
//	iRet = Telium_Pprintf("\x1b""E""       Magnetic Demo\n\n"                 // Print demo in progress
//		           "\x1b""F"); CHECK(iRet>=0, lblKO);
//	iRet = Telium_Pprintf("Track1:\n%s\n", tcTrk1); CHECK(iRet>=0, lblKO);    // Print track1
//  iRet = Telium_Pprintf("Track2:\n%s\n", tcTrk2); CHECK(iRet>=0, lblKO);    // Print track2
//  iRet = Telium_Pprintf("Track3:\n%s\n", tcTrk3); CHECK(iRet>=0, lblKO);    // Print track3
//	iRet = Telium_Pprintf("\n\n\n\n\n\n"); CHECK(iRet>=0, lblKO);

//	Telium_Reset_buf(hKbd, _receive_id);                                      // Reset keyboard FIFO
//	Telium_Ttestall(KEYBOARD, 30*100);                                        // Wait until key pressed or timeout
//	Telium_Reset_buf(hKbd, _receive_id);                                      // Reset keyboard FIFO to clear key pressed

  if(memcmp(tcTrk2,"Empty",5))
  {
	  //Os__xprintaa("1010101");
	  Transaction_Magnetique(tcTrk1,tcTrk2,tcTrk3);
	  //Os__xprintaa("222222");
	  //Os__xprintaa("Transaction_Magnetique");
  }

  goto lblEnd;

	// Errors treatment
    // ****************
lblKO:                                                                        // None-classified low level error
	_clrscr();                                                                // Clear screen
	DisplayMedium(0, 0, "Processing Error");                                  // Oops! error
    Telium_Ttestall(0,5*100);                                                 // Delay 5s
    goto lblEnd;


lblcless:
	//APEMV_UI_MessageDisplayDelay ("CLESS Trx\nStarted....", 1);
	//traceString("CLESS");
	*pCam0 = 2;
	goto lblEnd;

lblEnd:
	if(hMag2)
		Telium_Fclose(hMag2);                                                 // Close "mag2" channel

	if(hMag3)
		Telium_Fclose(hMag3);                                                 // Close "mag3" channel

	if(hMag13)
		Telium_Fclose(hMag13);                                                // Close "mag13" channel

	if(hCam0)
		Telium_Fclose(hCam0);
if(!PinOnlinereboot){		//belhadeg
	//Os__xprintaa("0001");
	if (keyboard != NULL);
		Telium_Fclose(keyboard);
}
}


void debug_puts(char * t)
{

}

void debug_showframe(char* t,int t1,int t2)
{

}
void test_rf3(char *pCam0)
{
	unsigned int bIsOpened;
	unsigned int nTimeout;
	unsigned int nNumOfCards;
	unsigned timedelay;
	int nResult = OK;//int nResult = KERNEL_STATUS_OK;
	unsigned char rspbuf[256 + 2];

	unsigned char afl_buf[252]; //! variable up to 252.
	int afl_count;

	unsigned char track2_buf[19]; //! variable up to 19. //&&&&& ??????????
	int track2_len; //&&&&& ??????????

	char AFFMONT[48];
	char DEVALP[4];
	int  ik;

	////APEMV_UI_MessageDisplayDelay ("test_rf3()", 1);debug_puts("test_rf3()\r\n");

	//    _clrscr();                                                              // Clear screen
	//	DisplayMedium(0, 0, "Swipe a card...");                                   // Display message
	//    //APEMV_UI_MessageDisplayDelay ("Attente Carte.1..\nMAG ou CHIP", 1); 	  //Test17
		if(!memcmp(StructInfoCaisse[0].DE,"504",3))
			sprintf(DEVALP,"%s","MAD");
		else if(!memcmp(StructInfoCaisse[0].DE,"978",3))
			sprintf(DEVALP,"%s","EUR");
		else if(!memcmp(StructInfoCaisse[0].DE,"840",3))
			sprintf(DEVALP,"%s","USD");
		else
			sprintf(DEVALP,"%s","ERR");


		//Telium_Fclose(hKbd);
		//Telium_Ttestall(0,100);


	//! track2 ?????? ??????

	memset(track2_buf, 0, sizeof(track2_buf));
	track2_len = 0;

	//! close driver
//	ClessEmv_CloseDriver();

	//! open driver
	bIsOpened = ClessEmv_OpenDriver();
	if (bIsOpened == CL_OK) {
		for (;;) {
			//! detect card

			//timedelay = timeout_set();
			for (ik=0;ik<8;ik++) {

				if(StructInfoPayment.Fallback == 1)
				{
					sprintf(AFFMONT,"FALL BACK\nCLESS\n%s\n%d,%.02d%s","MONTANT :                        ",atol(StructInfoCaisse[0].MT)/100,atol(StructInfoCaisse[0].MT)%100,DEVALP);

					for (;;)
					{
						WaitBothMagneticAndCam0(pCam0);
						//Os__xprintaa("0007");

						if(*pCam0 == 0)
							break;
					}
					if(*pCam0 == 0)
						break;
				}
				else
					sprintf(AFFMONT,"ATTENTE CARTE\nCLESS\n%s\n%d,%.02d%s","MONTANT :                        ",atol(StructInfoCaisse[0].MT)/100,atol(StructInfoCaisse[0].MT)%100,DEVALP);

				//APEMV_UI_MessageDisplayDelay (AFFMONT, 0); 		//Test17


				nTimeout = 1; //! 1 second
				nNumOfCards = 1;
				nResult = ClessEmv_DetectCards(CL_TYPE_AB, &nNumOfCards,nTimeout);

				//! ok
				if (nResult == CL_OK)
				{
					////APEMV_UI_MessageDisplayDelay ("nResult == CL_OK", 1);
					*pCam0 = 2;
					break;
				}
				//! timeout
				else if (nResult == CL_NOCARD) {
					////APEMV_UI_MessageDisplayDelay ("CL_NOCARD", 1);
					WaitBothMagneticAndCam0(pCam0);
					//Os__xprintaa("0008");
					if((*pCam0 == 0) ||(*pCam0 == 1))
						break;
				//	if (timeout_elapsed_ms(timedelay) > 30000)
				//		break;
				}
				//! too many cards
				else if (nResult == CL_TOOMANYCARDS) {
					////APEMV_UI_MessageDisplayDelay ("Too many cards", 1);debug_puts("Too many cards\r\n");
				}
				//! error
				else { //! CL_KO
					////APEMV_UI_MessageDisplayDelay ("Invalid Contactless 01", 1);debug_puts("Invalid Contactless 01\r\n");
					break;
				}
			}
			if(ik == 8)
			{
				//APEMV_UI_MessageDisplayDelay ("Time out", 1);
				*pCam0 = 4;
				break;
			}
			if (nResult != CL_OK)
				break;

			if((*pCam0 == 0) ||(*pCam0 == 1))
				break;

			//! activate card

			nResult = ClessEmv_ActiveCard(0, CL_ISO14443_4);
			//! ok
			if (nResult == CL_OK)
				;
			//! invalid card
			else if (nResult == CL_INVALIDCARD) {
				////APEMV_UI_MessageDisplayDelay ("Invalid Contactless 02", 1);debug_puts("Invalid Contactless 02\r\n");
				break;
			}
			//! error
			else { //! CL_KO
				////APEMV_UI_MessageDisplayDelay ("Invalid Contactless 03", 1);debug_puts("Invalid Contactless 03\r\n");
				break;
			}

			//! select PPSE

			if (nResult == CL_OK) {
				//unsigned char cmdbuf[] = "\x00\xCA\x9F\x7F\x2A";
				unsigned char cmdbuf[] =
						"\x00\xA4\x04\x00\x0E\x32\x50\x41\x59\x2E\x53\x59\x53\x2E\x44\x44\x46\x30\x31"; //! ppse
				//unsigned char rspbuf[128];
				int cmdlen = sizeof(cmdbuf) - 1;
				int rsplen;
				int errcode = 0;
				nResult = ClessEmv_ApduEx(0, cmdbuf, cmdlen, rspbuf,
						(unsigned int *) &rsplen, &errcode);
				//! ok
				if (nResult == CL_OK) {
					////APEMV_UI_MessageDisplayDelay ("-select PPSE-OK", 1);debug_puts("-select PPSE-\r\n");
					debug_showframe("send", cmdbuf, cmdlen);
					debug_showframe("recv", rspbuf, rsplen);
					if (rsplen > 2) {
						if ((rspbuf[rsplen - 2] == 0x90)
								&& (rspbuf[rsplen - 1] == 0x00)) {
							;
						} else {
							////APEMV_UI_MessageDisplayDelay ("Invalid Contactless 04", 1);debug_puts("Invalid Contactless 04\r\n");
							nResult = CL_KO;
							break;
						}
					} else {
						////APEMV_UI_MessageDisplayDelay ("Invalid Contactless 05", 1);debug_puts("Invalid Contactless 05\r\n");
						nResult = CL_KO;
						break;
					}
				}
				//! error
				else { //! CL_KO
					char str[32];
					////APEMV_UI_MessageDisplayDelay ("-select PPSE-KO", 1);debug_puts("-select PPSE-\r\n");
					sprintf(str, "nResult = %d, errcode = %d\r\n", nResult,
							errcode);
					debug_puts(str);
					debug_showframe("send1", cmdbuf, cmdlen);
					debug_showframe("recv1", rspbuf, rsplen);
					debug_puts("Invalid Contactless 06\r\n");
					break;
				}
			}

			//! select KonaMoney

			if (nResult == CL_OK) {
				unsigned char cmdbuf[] =
//						"\x00\xA4\x04\x00\x0A\xA0\x00\x00\x02\x51\x40\x25\x26\x00\x10";	//Korean Card
						"\x00\xA4\x04\x00\x07\xA0\x00\x00\x00\x04\x10\x10";
//				unsigned char cmdbuf[] = "\x00\xA4\x04\x00\x0A\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF";
				int cmdlen = sizeof(cmdbuf) - 1;
				int rsplen;
				int errcode = 0;
				nResult = ClessEmv_Apdu(0, cmdbuf, cmdlen, rspbuf,
						(unsigned int *) &rsplen/* &errcode*/);
				//! ok
				if (nResult == CL_OK) {
					////APEMV_UI_MessageDisplayDelay ("-select KonaMoney AID-", 1);debug_puts("-select KonaMoney AID-\r\n");
					debug_showframe("send", cmdbuf, cmdlen);
					debug_showframe("recv", rspbuf, rsplen);
					if (rsplen > 2) {
						if ((rspbuf[rsplen - 2] == 0x90)
								&& (rspbuf[rsplen - 1] == 0x00)) {
							;
						} else {
							////APEMV_UI_MessageDisplayDelay ("Invalid Contactless 07", 1);debug_puts("Invalid Contactless 07\r\n");
							nResult = CL_KO;
							break;
						}
					} else {
						////APEMV_UI_MessageDisplayDelay ("Invalid Contactless 08", 1);debug_puts("Invalid Contactless 08\r\n");
						nResult = CL_KO;
						break;
					}
				}
				//! error
				else { //! CL_KO
					   //&&&&& ???? ???? ???? ????. RF???????? ?????????? ???? ?????? ???? ????.
					char str[32];
					////APEMV_UI_MessageDisplayDelay ("-select KonaMoney AID-", 1);debug_puts("-select KonaMoney AID-\r\n");
					sprintf(str, "nResult = %d, errcode = %d\r\n", nResult,errcode);
					debug_puts(str);
					debug_showframe("send1", cmdbuf, cmdlen);
					debug_showframe("recv1", rspbuf, rsplen);
					debug_puts("Invalid Contactless 09\r\n");
					break;
				}
			}

			//! get processiong option (GPO)

			if (nResult == CL_OK) {
				unsigned char cmdbuf[64] = "\x80\xA0\x00\x00\x00";
				//unsigned char cmdbuf[64] = "\x80\xA8\x00\x00\x1a";
				//unsigned char rspbuf[128];
				int cmdlen = 5;
				int rsplen;
				unsigned char amount[6] = { 0x00, 0x00, 0x00, 0x00, 0x10, 0x00 }; //&&&&& ????????. ???? ???????? ?????? ??.
				unsigned char country_code[2] = { 0x04, 0x10 }; //&&&&& ????????. ???????? ??????????.
				unsigned char currency_code[2] = { 0x04, 0x10 }; //&&&&& ????????. ???????? ??????????.
				unsigned char trdate[3] = { 0x16, 0x06, 0x13 }; //&&&&& ????????
				unsigned char trtype = 0x00; //&&&&& ????????
				unsigned char terminal_random[4] = { 0x11, 0x22, 0x33, 0x44 }; //&&&&& ?????? ????
				{
					memcpy(&cmdbuf[cmdlen], "\x83\x18", 2);
					cmdlen += 2; //! command template
					memcpy(&cmdbuf[cmdlen], amount, 6);
					cmdlen += 6; //! ???????? BCD
					memcpy(&cmdbuf[cmdlen], "\x00\x00\x00\x00\x00\x00", 6);
					cmdlen += 6; //! ?????? BCD
					memcpy(&cmdbuf[cmdlen], country_code, 2);
					cmdlen += 2; //! ????????
					memcpy(&cmdbuf[cmdlen], currency_code, 2);
					cmdlen += 2; //! ????????
					memcpy(&cmdbuf[cmdlen], trdate, 3);
					cmdlen += 3; //! ????????
					memcpy(&cmdbuf[cmdlen], &trtype, 1);
					cmdlen += 1; //! ????????
					memcpy(&cmdbuf[cmdlen], terminal_random, 4);
					cmdlen += 4; //! ????????
				}
				nResult = ClessEmv_Apdu(0, cmdbuf, cmdlen, rspbuf,
						(unsigned int *) &rsplen);
				//! ok
				if (nResult == CL_OK) {
					////APEMV_UI_MessageDisplayDelay ("-get processing option-", 1);debug_puts("-get processing option-\r\n");
					debug_showframe("send", cmdbuf, cmdlen);
					debug_showframe("recv", rspbuf, rsplen);
					if (rsplen > 2) {
						if ((rspbuf[rsplen - 2] == 0x90)
								&& (rspbuf[rsplen - 1] == 0x00)) {
						} else {
							////APEMV_UI_MessageDisplayDelay ("Invalid Contactless 10", 1);debug_puts("Invalid Contactless 10\r\n");
							nResult = CL_KO;
							break;
						}
					} else {
						////APEMV_UI_MessageDisplayDelay ("Invalid Contactless 11", 1);debug_puts("Invalid Contactless 11\r\n");
						nResult = CL_KO;
						break;
					}
				}
				//! error
				else { //! CL_KO
					////APEMV_UI_MessageDisplayDelay ("Invalid Contactless 12", 1);debug_puts("Invalid Contactless 12\r\n");
					break;
				}
			}

			//! read record

		} //! for (;;)
	} else {
		////APEMV_UI_MessageDisplayDelay ("ClessEmv_OpenDriver() failed", 1);debug_puts("ClessEmv_OpenDriver() failed\r\n");
	}

	//! close driver
	ClessEmv_CloseDriver();
}

/*
unsigned char end, cancel = FALSE;
end = FALSE;
cancel = FALSE;

keyboard = Telium_Fopen("KEYBOARD", "r");

do
{
   event = Telium_Ttestall(KEYBOARD | CLESS, 1000);

   if (event & CLESS)
   {
      end = TRUE;
   }
   else   // event  KEYBOARD
   {
      if (Telium_Getc(keyboard) == T_ANN)
      {
         Cless_ExplicitSelection_GlobalCardDetectionCancel();
         cancel = TRUE;
         end = TRUE;
      }
   }
} while(!end);

if (keyboard != NULL)
	Telium_Fclose (keyboard);

// Check if card detection has been canceled
if(cancel == TRUE)
{
   step = __APCLESS_EXPLICIT_STEP_END;
   Retour_Caisse("118");			//Cless_ Carte non enregistr?e
   break;
}
*/
