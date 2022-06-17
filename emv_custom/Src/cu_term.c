/**
* \file cu_term.c
* \brief This module contains the functions that manages the interface with the terminal.
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

#include "sdk.h" // IngeDev automatic refactoring - 2014/04/29 14:29:58 - Replace '#include "SDK30.H"' to '#include "sdk.h"'
#include "hterm.h"
#include "cu_term.h"
// #include "convert.h"
#include "_emvdctag_.h"
#include "cu_pin.h"
#include "cu_file.h"
#include "cu_entry.h"
#include "cu_mess.h"


#ifdef _USE_UCM_
#include "ucmtelium.h"
#include "ucmhostdll.h"
#include "ucmclib.h"
#endif

//! \addtogroup Group_cu_term
//! @{

/* ======================================================================== */
/* Macros & preprocessor definitions                                        */
/* ======================================================================== */

// Constants used to dialog with the peripherals
// #define C_DISPLAY				"DISPLAY"				//!< Name for display peripheral.
#define C_KBD					"KEYBOARD"					//!< Name for keyboard peripheral.
#define C_PRNT					"PRINTER"					//!< Name for printer peripheral.

// #define C_COM1					"COM1"					//!< Name for COM1 peripheral.

// Constants used for the DISPLAY peripheral
// #define C_CLEAR					"\x1b"					//!< Definition of the clear character.
#define C_BLANK					0x20						//!< Definition of the blank character.
#define C_DISPLAYLINE			2							//!< Number of lines on the display.
#define C_DISPLAYCOL			16							//!< Number of comlumns on the display.
#define C_FIRSTCOL				0							//!< Definition of the first comlumn.

// Constants used for the PRINTER peripheral
#define C_NORMAL				"\x14""\x1b""F"				//!< Definition of the normal font.
#define C_CONDENSED				"\x0f"						//!< Definition of the condensed font.
#define C_LARGE					"\x1b""E"					//!< Definition of the large font.
#define C_WAIT_PRINT_CMD		0							//!< Awaiting time for the printer in ticks (1 tick = 10 ms).

// Constants used for the pinpad DISPLAY peripheral
#define C_PINPADLINE			2							//!< Number of lines on the pinpad display.
#define C_PINPADCOL				16							//!< Number of columns on the pinpad display.

// Constants in relation with printf() text font
#define C_TXT_FONT_HEIGHT		32							//!< Height of the text font.

#define C_TXT_X_POS(p1)						(FontWidth*p1)
#define C_GRAPHIC_X_POS(p1)					(p1)
#define C_TXT_Y_POS(p1)						(C_TXT_FONT_HEIGHT/2 - FontHeight/2 + (C_TXT_FONT_HEIGHT * p1))
#define C_GRAPHIC_Y_POS_RECT(p1)			(FontHeight/2 * (3*p1))
#define C_GRAPHIC_Y_POS(p1)					(FontHeight/2 * (1 + 3*p1))
#define C_GRAPHIC_Y_POS_WITHOUT_MARG(p1)	((FontHeight+0) * p1 + 3)

#define C_SMALL_POLICE_REDEFINE				_POLICE4x7_		//!< Redefinition of small police.
#define C_SMALL_POLICE						_dPOLICE4x7_	//!< Definition of small police.
#define C_MIDDLE_POLICE						_dPOLICE5x7_	//!< Definition of middle police.
#define C_CURRENT_POLICE					_dPOLICE8x14_	//!< Definition of current police.

#define PINPAD								1<< 14			//!< Pinpad event.


/* =========================================================================*/
/* Types declarations                                                       */
/* =========================================================================*/

/* ======================================================================== */
/* Constant declarations                                                    */
/* ======================================================================== */

/* ========================================================================	*/
/* Global variables                                                         */
/* ========================================================================	*/

// File stream identifiers
Telium_File_t *lp_display;											//!< Handle definition for display.
Telium_File_t *lp_keybd;												//!< Handle definition for keyboard.
Telium_File_t *lp_printer;											//!< Handle definition for printer.

static int TypePP;											//!< variable for pinpad type.
static int FontHeight;										//!< variable for font height.
static int FontWidth;										//!< variable for font width.

T_Bool isPinpad;											
extern char * ptr_PrinterMode;

/* ======================================================================== */
/* Functions                                                                */
/* ======================================================================== */

extern int  printfMSGInfos (MSGinfos *pMSGinfos);


//===========================================================================




int My_PSQ_Give_Language(void)
{
       const char *lang;

       lang = GetManagerLanguage();
       if (lang != NULL)
       {
           if (strncmp(lang, "fr", 3) == 0)
                  return 1;
           if (strncmp(lang, "FR", 3) == 0)
                  return 1;
       }

       return 0;
}

//===========================================================================
//! \brief This function loads a font file, if necessary, and inits font 
//! size and position line.
//! \param[in] police : font to manage.
//===========================================================================
static void CUTERM_ManageFont(char *police)
{
	static char FontLoaded = 0;

	if (FontLoaded == 0)
	{
		CurrentMSG.file = LoadFont("/LOCAL/MANAGER.SGN");
		CurrentMSG.coding = _ISO8859_;
		// CurrentMSG.message = affected later
		FontLoaded = 1;

		if (CurrentMSG.file != NULL)
		{
			DefCurrentFont(CurrentMSG.file);
		}
		// default printer mode
		ptr_PrinterMode = _pNORMAL_;
	}

	// If Font file not found
	if (CurrentMSG.file == NULL)
	{
		// Default font size
		FontHeight = 14;
		FontWidth = 8;
	}
	else
	{
		FontHeight = GetExtendedPoliceSize(police);
		if (memcmp(police, C_CURRENT_POLICE, strlen(C_CURRENT_POLICE)) == 0)
		{
			FontWidth = SizeOfISO8859String((unsigned char*)"O", police, _FIXED_WIDTH_);
		}
		else if (memcmp(police,C_MIDDLE_POLICE, strlen(C_MIDDLE_POLICE)) == 0)
		{
			FontWidth = SizeOfISO8859String((unsigned char*)"O", police, _PROP_WIDTH_);
		}
		else /* if (memcmp(police,C_SMALL_POLICE, strlen(C_SMALL_POLICE)) == 0) */
		{
			FontWidth = SizeOfISO8859String((unsigned char*)"O", police, _PROP_WIDTH_);
		}
	}
}

//===========================================================================
void CUTERM_DefineNewChar(void)
{
	char bitmap_I[] =		// Redefine the "|" character as a "l" without spaces
	{
		// Line 1 :
		0x00,				// Number of blank columns prior to first column
		0x01,				// useful column count
		0x7F,
		0x1,				// Following a new line 0xff : yes, other values : no
		0x00,				// Number of blank columns after last column
	};

	char bitmap_P[] =		// Redefine the "`" character as a "'" without spaces
	{
		// Line 1 :
		0x00,				// Number of blank columns prior to first column
		0x01,				// useful column count
		0x03,
		0x1,				// Following a new line 0xff : yes, other values : no
		0x00,				// Number of blank columns after last column
	};


	DefGraphicPattern('|', bitmap_I, C_SMALL_POLICE_REDEFINE);
	DefGraphicPattern('`', bitmap_P, C_SMALL_POLICE_REDEFINE);
}

//===========================================================================
void CUTERM_RestoreChar(void)
{
	char bitmap_I[] =		// Restore the "|" character
	{
		// Line 1 :
		0x02,				// Number of blank columns prior to first column
		0x01,				// Useful column count
		0x11,
		0x1,				// Following a new line 0xff : yes, other values : no
		0x00				// Number of blank columns after last column
	};

	char bitmap_P[] =		// Redefine the "`" character as a "'" without spaces
	{
		// Line 1 :
		0x00,				// Number of blank columns prior to first column
		0x02,				// Usefel column count
		0x01,0x02,
		0x1,				// Following a new line 0xff : yes, other values : no
		0x02				// Number of blank columns after last column
	};


	DefGraphicPattern('|', bitmap_I, C_SMALL_POLICE_REDEFINE);
	DefGraphicPattern('`', bitmap_P, C_SMALL_POLICE_REDEFINE);
}


//===========================================================================
void CUTERMdisplayGraphicLine(int xi_line, char *xs_text, T_CUTERMjustify xt_justify, char *mode, char c_l_LongLine)
{

	int lg, y;
	int line, column;

	// For test
	char * police;
	char EmptyScreen[] = "                        ";

	if (c_l_LongLine)
		police = C_SMALL_POLICE;
	else
		police = C_MIDDLE_POLICE;

	// In order to get back the number of column on the display
	CUTERMgetDisplay(&line, &column);
	column *= 8; 

#ifdef _USE_UCM_
	if(UCM_Present)
	{
		// Display 2 lines on the UCM
		iLIBUCM_Display_Open(UCMC_DISPLAY,"w");
		iLIBUCM_Display_Message(UCMC_DISPLAY," ", 0);
	}
	else
#endif
	{
		// Load File font if necessary
		CUTERM_ManageFont(police);

		// For test
		CurrentMSG.message = EmptyScreen;

		if (strcmp(mode, "w") == 0)
		{
			// Clear the screen
			_clrscr();
		}

		if (c_l_LongLine)
			DisplayMSGinfos(C_GRAPHIC_X_POS(0), C_GRAPHIC_Y_POS(xi_line), &CurrentMSG, _OFF_, police, _PROP_WIDTH_);
		else
			DisplayMSGinfos(C_GRAPHIC_X_POS(0), C_GRAPHIC_Y_POS_WITHOUT_MARG(xi_line), &CurrentMSG, _OFF_, police, _FIXED_WIDTH_);
	}

	// Get the length of the input text
	if (c_l_LongLine)
		lg = SizeOfISO8859String((unsigned char *)xs_text, police, _PROP_WIDTH_);
	else
		lg = SizeOfISO8859String((unsigned char *)xs_text, police, _FIXED_WIDTH_);

	y = C_FIRSTCOL;

	switch (xt_justify)
	{
		case (CENTER):
			{
				y = (column - lg) / 2;

				// If xs_text lenght is superior to column: xs_text will be automatically truncated
				if (y < 0)
				{
					y = C_FIRSTCOL;
				}
				break;
			};

		case (LEFT):
			{
				y = C_FIRSTCOL;
				break;
			}
		case (RIGHT):
			{
				y = column - lg;

				// If xs_text lenght is superior to column: xs_text will be automatically truncated
				if (y < 0)
				{
					y = C_FIRSTCOL;
				}
				break;
			}
	} // End of switch on xt_justify


#ifdef _USE_UCM_
	if(UCM_Present)
	{
		gotoxy(xi_line, y);
		iLIBUCM_Display_Message(UCMC_DISPLAY, xs_text, 0);
	}
	else
#endif
	{
		// Display long text with background
		CurrentMSG.message = xs_text;  
		if (c_l_LongLine)
		{
			DrawRect(0, C_GRAPHIC_Y_POS_RECT(xi_line), column, C_GRAPHIC_Y_POS_RECT(xi_line) + FontHeight*2, 0, _OFF_, _ON_);
			DisplayMSGinfos(C_GRAPHIC_X_POS(y), C_GRAPHIC_Y_POS(xi_line), &CurrentMSG, _ON_, police, _PROP_WIDTH_);
		}
		else    
		{
			DisplayMSGinfos(C_GRAPHIC_X_POS(y), C_GRAPHIC_Y_POS_WITHOUT_MARG(xi_line), &CurrentMSG, _OFF_, police, _FIXED_WIDTH_);
		}
	}

#ifdef _USE_UCM_
	if(UCM_Present)
	{
		iLIBUCM_Display_Close(UCMC_DISPLAY);
	}
#endif

	police = C_CURRENT_POLICE;

	// Set old configuration
	CUTERM_ManageFont(police);
}	// end of CUTERMdisplayLine


//===========================================================================
void CUTERMgetDisplay (int *xp_line, int *xp_column)
{
	*xp_line = C_DISPLAYLINE;
	*xp_column = C_DISPLAYCOL;
}	// end of CUTERMgetDisplay

//===========================================================================
T_Bool CUTERMgetKey(int xi_timeout, T_CUTERMkey *xp_key)
{
	T_Bool        res_key;
	unsigned char state;
	int           cr;
	int           timeout;
	Telium_File_t          *lp_keybd;

	res_key = B_FALSE;
	// Telium_Ttestall in 10th of ms
	timeout = xi_timeout / 10;

	lp_keybd = NULL;

#ifdef _USE_UCM_
	if(UCM_Present)
	{
		ret = iLIBUCM_Pinpad_Open(UCMC_PPAD,"r");
	}
	else
#endif
		////APEMV_UI_MessageDisplayDelay("Topen.01.11",2);
		lp_keybd = Telium_Stdin();
		if(lp_keybd != 0)
			Telium_Fclose(lp_keybd);
		////APEMV_UI_MessageDisplayDelay("Topen.01.22",2);
		lp_keybd = Telium_Fopen(C_KBD, "r");
		////APEMV_UI_MessageDisplayDelay("Topen.01.33",2);

#ifdef _USE_UCM_
	if(UCM_Present)
	{
		if(ret>=0)
		{
			// Test if a key is already pressed
			iLIBUCM_Pinpad_Status(UCMC_PPAD, &state);

			if (state == KEYS_IN_BUFFER)
			// No wait needed, get immediatelly the key pressed
			{
				// Test of the terminal
				*xp_key = (T_CUTERMkey)iLIBUCM_Pinpad_GetChar(UCMC_PPAD);
				res_key = B_TRUE;
			}
			else
			// Keyboard buffer is empty
			{
				if (timeout != 0)
				{
					// Wait for a key pressed during the timeout
					cr = iLIBUCM_Pinpad_Ttestall(UCMC_PPAD, (unsigned short)timeout);
					if(cr>=0)
					{
						// Test of the terminal
						*xp_key = (T_CUTERMkey)iLIBUCM_Pinpad_GetChar(UCMC_PPAD);
						res_key = B_TRUE;
					}

				}// end of if

			}// end of else buffer empty
		}
	}
	else
#endif
	{
		if(lp_keybd != NULL)
		{
			// Test if a key is already pressed
			Telium_Status(lp_keybd, &state);

			if (state == KEYS_IN_BUFFER)
			// No wait needed, get immediatelly the key pressed
			{
				// Test of the terminal
				*xp_key =(T_CUTERMkey) Telium_Getc(lp_keybd);
				res_key = B_TRUE;
			}
			else
			// Keyboard buffer is empty
			{
				if (timeout != 0)
				{
					// Wait for a key pressed during the timeout
					cr = Telium_Ttestall(KEYBOARD, timeout);
					switch(cr)
					{
					case KEYBOARD :
						{
							// Test of the terminal
							*xp_key = (T_CUTERMkey)Telium_Getc(lp_keybd);
							res_key = B_TRUE;
							break;
						}

					}// end of switch
				}// end of if

			}// end of else buffer empty
		}
	}

#ifdef _USE_UCM_
	if(UCM_Present)
	{
		iLIBUCM_Pinpad_Close(UCMC_PPAD);
	}
	else
#endif
		Telium_Fclose(Telium_Stdin());
	return (res_key);

}	// end of CUTERMgetKey


//===========================================================================
T_Bool CUTERM_Wait_function_key(byte xi_timeout, word mask, T_CUTERMkey *xp_key)
{
	T_Bool res_key;

	ENTRY_BUFFER Entry;
	int nResultTmp;

	TAB_ENTRY_FCT  t_annulcd;

	t_annulcd.mask = mask;
	t_annulcd.time_out = xi_timeout;

	// Default value : no key inputed
	res_key = B_FALSE;

	// Key mask
	Fct_Entry ( (TAB_ENTRY_FCT *)&t_annulcd );
	// Wait key or time-out
	Telium_Ttestall(ENTRY, 0);

	nResultTmp = Get_Entry(&Entry);

#if 0    
	{
		char tmp[40];
		sprintf(tmp, "F : nResultTmp : %x", nResultTmp);
		//trace(0, strlen(tmp), tmp);
		sprintf(tmp, "F : key : %x", Entry);
		//trace(0, strlen(tmp), tmp);
	}
#endif // 0

	switch(nResultTmp)
	{
		case CR_ENTRY_OK  :
			res_key = B_TRUE;

			if (Entry.d_len > 0)
			{
				switch (Entry.d_entry[0])
				{
					case TC_SK1 :
						*xp_key = T_SK1;
						break;
					case TC_SK2 :
						*xp_key = T_SK2;
						break;
					case TC_SK3 :
						*xp_key = T_SK3;
						break;
					case TC_SK4 :
						*xp_key = T_SK4;
						break;
					case TC_ANNULATION :
						*xp_key = T_ANN;
						break;
					case TC_CORRECTION :
						*xp_key = T_CORR;
						break;
					case TC_VALIDATION :
					case TC_SKVAL :
						*xp_key = T_VAL;
						break;
					default:
						*xp_key = 0;
						break;
				}              
			}
			break;

		case CR_ENTRY_VALIDATED :
			res_key = B_TRUE;
			*xp_key=T_VAL;
			break;
		case CR_ENTRY_CANCEL :
			res_key = B_TRUE;
			*xp_key=T_ANN;
			break;
		case CR_ENTRY_NOK :
			res_key = B_TRUE;
			*xp_key=T_CORR;
			break;
		case CR_ENTRY_TIME_OUT :
		default:
			*xp_key=TIMEOUT;
			break;
	}

#if 0
	{
		char tmp[40];
		sprintf(tmp, "F : res_key : %x", res_key);
		trace(0, strlen(tmp), tmp);
		sprintf(tmp, "F : *xp_key : %x", *xp_key);
		trace(0, strlen(tmp), tmp);
	}
#endif // 0

	return(res_key);
}


//===========================================================================
T_Bool CUTERIsPrinterPresent(void)
{
	T_Bool isPrinter;

#ifdef _USE_UCM_
	if(UCM_Present)
	{
		ret = iLIBUCM_Print_Exist(UCMC_PRINT);
		if (ret>=0)
			isPrinter = B_TRUE;
		else
			isPrinter = B_FALSE    ;
	}
	else
#endif
	{
		if (IsTwin31()||IsTwin32()||IsTwin33())
		{// different to TWIN30
			isPrinter = B_FALSE;
	    }
		else
		{
			isPrinter = B_TRUE;
		}
	}
	return (isPrinter);

}	// end of CUTERIsPrinterPresent


//===========================================================================
void CUTERMpaperFeed(void)
{
	Telium_File_t *lp_printer;

	if (CUTERIsPrinterPresent() == TRUE)
	{
#ifdef _USE_UCM_
		if(UCM_Present)
		{
			iLIBUCM_Print_Open(UCMC_PRINT,"w-");
			iLIBUCM_Print_Message(UCMC_PRINT,"\n\n\n\n\n");
			iLIBUCM_Print_Close(UCMC_PRINT);
		}
		else
#endif
		{
			// In case of printer
			lp_printer = Telium_Fopen(C_PRNT, "w-");
			// No test of return fopen because no return of CUTERMpaperFeed

			// print 3 empty lines
			Telium_Fprintf(lp_printer,  "\n\n\n\n\n");

			// wait for 5 seconds
			Telium_Ttestall(PRINTER, C_WAIT_PRINT_CMD);

			Telium_Fclose(Telium_Stdprt());
		}
	}
}	// end of CUTERMpaperFeed


//===========================================================================
void CUTERMprintLine(char *xs_text)
{
	Telium_File_t *lp_printer;

	if (CUTERIsPrinterPresent() == TRUE)
	{
#ifdef _USE_UCM_
		if(UCM_Present)
		{
			iLIBUCM_Print_Open(UCMC_PRINT,"w-");
			iLIBUCM_Print_Message(UCMC_PRINT,xs_text);
			iLIBUCM_Print_Close(UCMC_PRINT);
		}
		else
#endif
		{
			// In case of printer
			lp_printer = Telium_Fopen(C_PRNT, "w-");

			CurrentMSG.file = (char*)lp_printer;
			CurrentMSG.message = xs_text;

			PrintMSGInfos(&CurrentMSG, _OFF_, _pNORMAL_, _FIXED_WIDTH_);
			Telium_Fprintf(lp_printer,"\n");
			// wait for print end
			Telium_Ttestall(PRINTER, C_WAIT_PRINT_CMD);

			Telium_Fclose(Telium_Stdprt());
		}
	}
}	// end of CUTERMprintLine



//===========================================================================
void CUTERMsetPrinter(T_CUTERMprintMode xt_mode)
{
	Telium_File_t *lp_printer;

	lp_printer = NULL;

	if (CUTERIsPrinterPresent() == TRUE)
	{
#ifdef _USE_UCM_
		if(UCM_Present)
		{
			iLIBUCM_Print_Open(UCMC_PRINT,"w-");
		}
		else
#endif
			lp_printer = Telium_Fopen(C_PRNT, "w-");

		switch(xt_mode)
		{
			case PRINT_NORMAL :
				{
					// print in normal mode
#ifdef _USE_UCM_
					if(UCM_Present)
						iLIBUCM_Print_Message(UCMC_PRINT,C_NORMAL);
					else
#endif
						Telium_Fprintf(lp_printer, C_NORMAL);
					break;
				};
			case PRINT_CONDENSED :
				{
					// print in condensed mode
#ifdef _USE_UCM_
					if(UCM_Present)
						iLIBUCM_Print_Message(UCMC_PRINT,C_CONDENSED);
					else
#endif
						Telium_Fprintf(lp_printer, C_CONDENSED);
					break;
				}
			case PRINT_LARGE :
				{
					// print in large mode
#ifdef _USE_UCM_
					if(UCM_Present)
						iLIBUCM_Print_Message(UCMC_PRINT,C_LARGE);
					else
#endif
						Telium_Fprintf(lp_printer, C_LARGE);
					break;
				}
		}// end of switch

#ifdef _USE_UCM_
		if(UCM_Present)
		{
			iLIBUCM_Print_Close(UCMC_PRINT);
		}
		else
#endif
			Telium_Fclose(Telium_Stdprt());
	};

}// end of CUTERMsetPrinter


//===========================================================================
void CUTERMsleep(int xi_delay)
{
	int delay;

	// delay in Telium_Ttestall in 10th of ms
	delay = xi_delay / 10;

	Telium_Ttestall(0, delay);
} // end of CUTERMsleep



//===========================================================================
void CUTERMgetSerialNumber(char *no_serial)
{
	ReadGlobalData();
	strncpy(no_serial ,(char *)g_no_serial, sizeof(g_no_serial)); // Proctect against overflow
}

//===========================================================================
void CUTERMsetSerialNumber(char *no_serial)
{
	unsigned char lg;

	lg = strlen(no_serial);
	switch (lg)
	{
		case 0 :
			strcpy ((char *)g_no_serial , "00000000" ) ;
			break ;
		case 1 :
			strcpy ((char *)g_no_serial , "0000000" ) ;
			strcat ((char *)g_no_serial ,  no_serial ) ;
			break ;
		case 2 :
			strcpy ((char *)g_no_serial , "000000" ) ;
			strcat ((char *)g_no_serial ,  no_serial ) ;
			break ;
		case 3 :
			strcpy ((char *)g_no_serial , "00000" ) ;
			strcat ((char *)g_no_serial ,  no_serial ) ;
			break ;
		case 4 :
			strcpy ((char *)g_no_serial , "0000" ) ;
			strcat ((char *)g_no_serial ,  no_serial ) ;
			break ;
		case 5 :
			strcpy ((char *)g_no_serial , "000" ) ;
			strcat ((char *)g_no_serial ,  no_serial ) ;
			break ;
		case 6 :
			strcpy ((char *)g_no_serial , "00" ) ;
			strcat ((char *)g_no_serial ,  no_serial ) ;
			break ;
		case 7 :
			strcpy ((char *)g_no_serial , "0" ) ;
			strcat ((char *)g_no_serial ,  no_serial ) ;
			break ;
		default :
			strcpy ((char *)g_no_serial ,  no_serial ) ;
			break ;
	}

	StoreGlobalData();

}

//===========================================================================
void CUTERMgetTerminalId(char *term_id)
{
	ReadGlobalData();
	strncpy(term_id ,(char *)g_terminal_id, sizeof(g_terminal_id)); // Protect againt overflow
}

//===========================================================================
void CUTERMsetTerminalId(char *term_id)
{
	unsigned char lg;

	lg = strlen(term_id);
	switch (lg)
	{
		case 0 :
			strcpy ((char *)g_terminal_id , "00000000" ) ;
			break ;
		case 1 :
			strcpy ((char *)g_terminal_id , "0000000" ) ;
			strcat ((char *)g_terminal_id ,  term_id ) ;
			break ;
		case 2 :
			strcpy ((char *)g_terminal_id , "000000" ) ;
			strcat ((char *)g_terminal_id ,  term_id ) ;
			break ;
		case 3 :
			strcpy ((char *)g_terminal_id , "00000" ) ;
			strcat ((char *)g_terminal_id ,  term_id ) ;
			break ;
		case 4 :
			strcpy ((char *)g_terminal_id , "0000" ) ;
			strcat ((char *)g_terminal_id ,  term_id ) ;
			break ;
		case 5 :
			strcpy ((char *)g_terminal_id , "0000" ) ;
			strcat ((char *)g_terminal_id ,  term_id ) ;
			break ;
		case 6 :
			strcpy ((char *)g_terminal_id , "00" ) ;
			strcat ((char *)g_terminal_id ,  term_id ) ;
			break ;
		case 7 :
			strcpy ((char *)g_terminal_id , "0" ) ;
			strcat ((char *)g_terminal_id ,  term_id ) ;
			break ;
		default :
			strcpy ((char *)g_terminal_id ,  term_id ) ;
			break ;
	}

	StoreGlobalData();
}


//===========================================================================
unsigned char CUTERMgetPPpin (short TO_first_car, short TO_inter_car, ENTRY_BUFFER *xp_pin)
{
	unsigned char         res_PPkey;
	unsigned int          cr;
	TAB_ENTRY_STRING_PPAD entry;
	int                   rc_entry;
	unsigned short device;
#ifdef _USE_UCM_
	unsigned short event;
#endif

	res_PPkey = PP_HS;

	if (CUTERIsPinpadPresent() == B_TRUE)
	{
#ifdef _USE_UCM_
		if(UCM_Present)
		{
			cr = PP30_PPAD;
		}
		else
#endif
		{
			cr = PSQ_Pinpad_Value();
		}

		switch (cr)
		{
		case PP30_PPAD:
			// request scheme execution on EFT30
			//SetPprState(FALSE);
			break;
		default :
			// request scheme execution on PPC30
			//SetPprState(TRUE);
			// disable French message on PPR/PPC
			PPS_ClearFrenchWarningDisplay();
			break;
		}
		
		// Initialisation for the Pinpad entry
		entry.mask = 0x0000;						// Numerics, green, red, yellow keys enabled
		entry.time_out = (TO_first_car / 100);
		entry.line = 2;								// Echo on line 2
		entry.column = 0;
		entry.echo = 2;								// The echo character is star
		entry.nb_max = 12;							// The maximum number of entered characters is 12
		entry.nb_min = 4;							// The minimum number of entered characters is 4
		entry.timeout_inter = (TO_inter_car / 100);
		entry.phase = REQUEST_PHASE_2;
		entry.ruf[0]=0;
		entry.ruf[1]=0;

#ifdef _USE_UCM_
		if(UCM_Present)
		{
			Telium_ClearEvents (UCMC_PPAD);
		}
		else
#endif
		{
			Telium_ClearEvents (PINPAD);
		}


		PPS_Num_Entry_T(&entry);

#ifdef _USE_UCM_
		if(UCM_Present)
		{
			event = UCMMULTI_TTESTALL_ICC | UCMMULTI_TTESTALL_PINPAD;
			rc_entry = iLIBUCM_Device_Ttestall(&event, 0);
		}
		else
#endif
		{
			rc_entry = Telium_Ttestall (((PINPAD)|CAM0), 0);
		}

		// Time out on pincode input
#ifdef _USE_UCM_
		if(UCM_Present)
		{
			device = UCMC_PPAD;
		}
		else
#endif
		{
			device = PINPAD;
		}

		// Time out on pincode input
		if (rc_entry ==00)
		{	
			res_PPkey = TO_INPUT;
		}
		else
			if ((rc_entry & (device)) == (device))
			{
				cr = PPS_Read_Entry((BUFFER_SAISIE*)xp_pin);
				switch (cr)
				{
					case(CR_SAISIE_PINPAD_OK):
						res_PPkey = 0;
						break;

					case(CR_SAISIE_PINPAD_TIME_OUT):
						res_PPkey = TO_INPUT;
						break;

					case(CR_SAISIE_PINPAD_ANNUL):
						res_PPkey = CANCEL_INPUT;
						break;

					case(CR_SAISIE_PINPAD_NOK):
					case(CR_SAISIE_PINPAD_VAL):
						res_PPkey = ERROR_INPUT;
						break;

					default :
						res_PPkey = PP_HS;
						break;
				}
			}
			PPS_Stop_Entry();
	}

	return(res_PPkey);
}

//===========================================================================
T_Bool CUTERIsPinpadPresent()
{
	int cr;

	if ( isPinpad == B_NON_INIT)
	{
		TypePP = pinpad_simple;
#ifdef _USE_UCM_
		if(UCM_Present)
		{
			cr = iLIBUCM_Pinpad_Exist(UCMC_PPAD);
			if (cr>=0)
				isPinpad = B_TRUE;
			else
				isPinpad = B_FALSE;

		}
		else
#endif
		{
			cr = PSQ_Is_pinpad();
			if (cr == 1)
			{
				isPinpad = B_TRUE;
			}
			else
			{
				isPinpad = B_FALSE;
			}
		}
	}

	return(isPinpad);

}	// end of CUTERIsPinpadPresent


//===========================================================================
void CUTERMgetPPdisplay(int *xp_line, int *xp_column)
{
	if (CUTERIsPinpadPresent() == TRUE)
	{
		*xp_line = C_PINPADLINE;
		*xp_column = C_PINPADCOL;
	}
	else
	{	// If no pinpad is connected
		*xp_line = 0;
		*xp_column = 0;
	};
}/*end of CUTERMgetPPdisplay*/


//===========================================================================
int CUTERMdisplayPPline(int xi_line, char *xs_text, T_CUTERMjustify xt_justify)
{
	int lg, line, column;
	int leftblanknb, rightblanknb;
	char tempo[C_PINPADCOL+1], message[C_PINPADCOL+1];
	int pinpadok;

	memclr (tempo, C_PINPADCOL+1);
	memclr (message, C_PINPADCOL+1);

	if (CUTERIsPinpadPresent() == TRUE)
	{	
		// If a pinpad is connected.
		{
			// In order to get back the number of column on the display
			CUTERMgetPPdisplay(&line, &column);

			// clear the xi_line
			if (xi_line != 0)
			{
#ifdef _USE_UCM_
				if(UCM_Present)
					iLIBUCM_Display_NewLine(UCMC_DISPLAY);
				else
#endif
					PPS_newline();
			}

			// Get the length of the input text
			lg = strlen(xs_text);
			if (lg >= column)
			{
				// text will be truncated automatically
				strcpy (message, xs_text);
			}
			else
			{
				switch (xt_justify)
				{
				case (CENTER):
					{
						// Calculate the right position in order to center the displayed text
						leftblanknb = (column - lg) / 2;
						rightblanknb = column - lg - leftblanknb;

						// Create the n right blank of the finished message
						memset(tempo, C_BLANK, rightblanknb);

						// Create the n left blank of the finished message
						memset(message, C_BLANK, leftblanknb);

						// Concat the left blank with the input text
						strcat(message, xs_text);


						break;
					};

				case (LEFT):
					{
						rightblanknb = column - lg;

						// Create the n right blank of the finished message
						memset(tempo, C_BLANK, rightblanknb);

						// Concat the right blank with the input text
						strcpy(message, xs_text);
						break;
					};

				case (RIGHT):
					{
						leftblanknb = column - lg;
						tempo[0] = 0;

						// Create the n right blank of the finished message
						memset(message, C_BLANK, leftblanknb);

						// Concat the right blank with the input text
						strcat(message, xs_text);
						break;
					};
				};// End switch xt_justify
			};

			// Concat the 'leftblank+text' with the right blank
			strcat(message, tempo);
			message[C_PINPADCOL] = 0;
			// Display the message on pinpad display
#ifdef _USE_UCM_
			if(UCM_Present)
			{
				if(iLIBUCM_Display_Message(UCMC_DISPLAY,message, 0)==0)
					pinpadok = 1;
				else
					pinpadok = 0;
			}
			else
#endif
			{
				if (PP10_Display((char *) message) == 0)
				{
					pinpadok = 1;
				}
				else
				{
					pinpadok = 0;
				}
			}
		}	// not SPED
	}
	else
	{
		pinpadok = 1;
	}

	return(pinpadok);
}	// end of CUTERMdisplayPPline

//===========================================================================
unsigned char CUTERIsDeclaredPPPresent(char *xs_text)
{
	int retour;
	unsigned char isPPok;
#ifndef __TELIUM3__
	Telium_File_t *handle;
#endif

	isPPok = TRUE;

	if (CUTERIsPinpadPresent())
	{
		{
			if (CUTERMIsGraphicPP())
			{
				// If handle is valid,the PPR or PPC connected
#ifdef _USE_UCM_
				if(UCM_Present)
				{
					ret = iLIBUCM_Display_Open(UCMC_DISPLAY,"a*");
					if(ret>=0)
						iLIBUCM_Display_Close(UCMC_DISPLAY);
					else
						isPPok = B_FALSE;
				}
				else
#endif
				{
#ifdef __TELIUM3__
					isPPok = B_TRUE;
#else
					if ((handle=Telium_Fopen("DISPLAY_PPR","a*"))!=NULL)
					{
						Telium_Fclose(handle);
					}
					else
					{
						isPPok = B_FALSE;
					}
#endif
				}

			}
			else
			{
#ifdef _USE_UCM_
				if(UCM_Present)
				{
					retour = iLIBUCM_Display_Message(UCMC_DISPLAY,xs_text, 0);
					// JLM
					isPPok =  (retour >= 0);
				}
				else
#endif
				{
					retour = PPS_Display((char *)xs_text);
					isPPok =  (retour != 1);
				}
			}
		}
	}

	return(isPPok);
}

//===========================================================================
void CUTERMclearPPdisplay(void)
{
	if (CUTERIsPinpadPresent() == TRUE)
	{
		{
#ifdef _USE_UCM_
			if(UCM_Present)
				iLIBUCM_Display_Clear(UCMC_DISPLAY);
			else
#endif
				PPS_firstline();
		}
	}
}


//===========================================================================
//! \brief This function tests if graphical functions can be used on PPAD.
//! \return
//! - TRUE : graphical functions can be used on PPAD.
//! - FALSE : graphical functions cannot be used on PPAD.
//===========================================================================
char CUTERMIsGraphicPP(void)
{
	// On terminal, we can use graphical function with long text
	unsigned int cr;
	// Default value : no use of pinpad
	char ret = FALSE;

#ifdef _USE_UCM_
	if(UCM_Present)
	{
		cr = PP30_PPAD;
	}
	else
#endif
	{
		cr = PSQ_Pinpad_Value();
	}

	switch (cr)
	{
	// 2 lines display
	case PP30_PPAD:
	case PP30S_PPAD:
	case IPP220_PPAD:
	case IPP250_PPAD:
		ret = FALSE;
		break;

	// Graphic display
	case PPC30_PPAD:
	case PPR30_PPAD:
	case P30_PPAD:
	case IPP280_PPAD:
	case IPP320_PPAD:
	case IPP350_PPAD:
	case IPP285_PPAD:
	case IPP310_PPAD:
	case IPP315_PPAD:
		ret = TRUE;
		break;

	default:
		// By default, use a 2 lines display
		ret = FALSE;
		break;
	}
	return (ret);
}

//! @}

