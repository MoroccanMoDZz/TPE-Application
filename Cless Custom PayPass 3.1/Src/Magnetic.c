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

//****************************************************************************
//      EXTERN
//****************************************************************************
extern Telium_File_t *hKbd;      // Handle of the peripheral Keyboard

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
void WaitBothMagneticAndCam0(void)
{
	// Local variables
    // ***************
    Telium_File_t *hMag13=NULL, *hMag2=NULL, *hMag3=NULL,*hCam0=NULL;
	byte ucLen=0;
    int iSta, iNum=0;
    char tcTmp[128];
    char *pcSrc, *pcDst, cKey;
    char tcTrk1[128], tcTrk2[128], tcTrk3[128];
    char tcDisplay[25+1];
	int iRet;

	char AFFMONT[48];
	char DEVALP[4];


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



//    _clrscr();                                                              // Clear screen
//	DisplayMedium(0, 0, "Swipe a card...");                                   // Display message
//    APEMV_UI_MessageDisplayDelay ("Attente Carte.1..\nMAG ou CHIP", 1); 	  //Test17
	if(!memcmp(StructInfoCaisse[0].DE,"504",3))
		sprintf(DEVALP,"%s","MAD");
	else if(!memcmp(StructInfoCaisse[0].DE,"978",3))
		sprintf(DEVALP,"%s","EUR");
	else if(!memcmp(StructInfoCaisse[0].DE,"840",3))
		sprintf(DEVALP,"%s","USD");
	else
		sprintf(DEVALP,"%s","ERR");

	sprintf(AFFMONT,"CARTE MAG CHIP CLESS\n\n%s\n%d,%.02d%s","MONTANT :                        ",atol(StructInfoCaisse[0].MT)/100,atol(StructInfoCaisse[0].MT)%100,DEVALP);
	APEMV_UI_MessageDisplayDelay (AFFMONT, 0); 		//Test17


	// Wait for events
	// ===============
	Telium_Reset_buf(hKbd, _receive_id);                                      // Reset keyboard FIFO

	FicPara.vCam0 = 0;

	while(1)
	{
		iSta = Telium_Ttestall(KEYBOARD | CLESS | CAM0 | SWIPE31 | SWIPE2 | SWIPE3, 30*100); // Wait for the first event ISO1
		iSta |= Telium_Ttestall(iSta ^ (SWIPE31 | SWIPE2 | SWIPE3), 10);      // Wait for the second event ISO2
		iSta |= Telium_Ttestall(iSta ^ (SWIPE31 | SWIPE2 | SWIPE3), 10);      // Wait for the third event ISO3

        CHECK(iSta!=0, lblEnd);                                               // Exit if nothing swiped after 30s

        if(iSta & KEYBOARD)                                                   // Key event
		{
			cKey = Telium_Getchar();
        	CHECK(cKey!=T_ANN, lblEnd);                                       // Exit on red key
        	CHECK(cKey!=T_SK1, lblcless);                                       // Exit on red key
			continue;
		}
        else if(iSta & CAM0)                                                  // Key event
		{
        	FicPara.vCam0 = 1;
        	goto lblEnd;
		}
        else if(iSta & CLESS)                                                  // Key event
		{
        	APEMV_UI_MessageDisplayDelay ("CLESS....\nDetected", 1);
        	FicPara.vCam0 = 2;
        	goto lblEnd;
		}

		else
			break;                                                            // Swipe event
	}

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
					break;
				if(*pcSrc == 'D')
					*pcSrc = '=';
				*pcDst++ = *pcSrc++;
			}
		}
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
	}

	if (tcTrk1[0]==0) strcpy(tcTrk1, "Empty");
	if (tcTrk2[0]==0) strcpy(tcTrk2, "Empty");
	if (tcTrk3[0]==0) strcpy(tcTrk3, "Empty");

	// Display tracks
	// ==============
	_clrscr();                                                                // Clr Screen
//	DisplaySmall(0, 0, "Track1:");
//	sprintf(tcDisplay, "%.25s", tcTrk1);                                      // Display part of track1
//	DisplaySmall(1, 0, tcDisplay);
//  DisplaySmall(2, 0, "Track2:");
//  sprintf(tcDisplay, "%.25s", tcTrk2);                                      // Display part of track2
//    APEMV_UI_MessageDisplayDelay (tcDisplay, 1); 					  			//Test17
//  DisplaySmall(3, 0, tcDisplay);
//  DisplaySmall(4, 0, "Track3:");
//  sprintf(tcDisplay, "%.25s", tcTrk3);                                      // Display part of track3
//  DisplaySmall(5, 0, tcDisplay);
//  DisplaySmall(7, 6, "Press a key");

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

    goto lblEnd;

	// Errors treatment
    // ****************
lblKO:                                                                        // None-classified low level error
	_clrscr();                                                                // Clear screen
	DisplayMedium(0, 0, "Processing Error");                                  // Oops! error
    Telium_Ttestall(0,5*100);                                                 // Delay 5s
    goto lblEnd;


lblcless:
	APEMV_UI_MessageDisplayDelay ("CLESS....\nselected", 0);
	FicPara.vCam0 = 2;
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
}
