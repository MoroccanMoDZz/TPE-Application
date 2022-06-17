/////////////////////////////////////////////////////////////////
//// Includes ///////////////////////////////////////////////////
#include "SDK_Naps.h"	//#include "sdk.h" // IngeDev automatic refactoring - 2014/04/29 14:29:58 - Replace '#include "SDK30.H"' to '#include "sdk.h"'

#include "UserInterfaceHelpers.h"

#include "cu_term.h"


#include "_emvdctag_.h"
#include "TlvTree.h"
#include "cu_base.h"

#include "cu_mess.h"
#include "MSGlib.h"
#include "MessagesDefinitions.h"
#include "cu_serv.h"
#include "def_tag.h"

#include "sec_interface.h"
#include "GL_GraphicLib.h"
#include "GTL_Assert.h"
#include "GTL_Convert.h"

#include "EPSTOOL_Convert.h"
#include "EPSTOOL_TlvTree.h"
#include "EPSTOOL_PinEntry.h"
#include "EPSTOOL_Unicode.h"
#include "cu_UserInterfaceDisplay.h"

//// Macros & preprocessor definitions //////////////////////////
#define NUMBER_OF_ITEMS(a)			(sizeof(a)/sizeof((a)[0]))

//// Types //////////////////////////////////////////////////////

//// Global variables ///////////////////////////////////////////

//// Static function definitions ////////////////////////////////
static int HelperDisplayMenuInternal( const char* szTitle, int bRadioButtons, int nDefaultChoice,
									  int nItems, const char* Items[],
									  int nBanners );
int ShowEntry (const char *pcTitle, const char *pcText, char *pcInput, byte ucMinDigit, byte ucMaxDigit, byte ucEntryMode);
//// Functions //////////////////////////////////////////////////
#define STATE_INPUT_END				0
#define STATE_INPUT_WAIT_KEY		1
#define STATE_INPUT_KEY_STEP		2


#define __GETKEY(tab) \
	if( (tab)[*pnIndex] == '\0' ) *pnIndex = 0; *pcCharacter = (tab)[*pnIndex]; (*pnIndex)++;


static int GetNextCharacter( int nKey, int *pnIndex, char *pcCharacter )
{
	const char tKey0[] = { '0', '-', ' ', '\0' };
	const char tKey1[] = { '1', 'q', 'z', 'Q', 'Z', '\0' };
	const char tKey2[] = { '2', 'a', 'b', 'c', 'A', 'B', 'C', '\0' };
	const char tKey3[] = { '3', 'd', 'e', 'f', 'D', 'E', 'F', '\0' };
	const char tKey4[] = { '4', 'g', 'h', 'i', 'G', 'H', 'I', '\0' };
	const char tKey5[] = { '5', 'j', 'k', 'l', 'J', 'K', 'L', '\0' };
	const char tKey6[] = { '6', 'm', 'n', 'o', 'M', 'N', 'O', '\0' };
	const char tKey7[] = { '7', 'p', 'r', 's', 'P', 'R', 'S', '\0' };
	const char tKey8[] = { '8', 't', 'u', 'v', 'T', 'U', 'V', '\0' };
	const char tKey9[] = { '9', 'w', 'x', 'y', 'W', 'X', 'Y', '\0' };
	const char tKeyDot[] = { '.', ',', '\'', ';', ':', '@', '/', '\\', '\0' };

	switch( nKey )
	{
	case T_NUM0:	__GETKEY(tKey0);	break;
	case T_NUM1:	__GETKEY(tKey1);	break;
	case T_NUM2:	__GETKEY(tKey2);	break;
	case T_NUM3:	__GETKEY(tKey3);	break;
	case T_NUM4:	__GETKEY(tKey4);	break;
	case T_NUM5:	__GETKEY(tKey5);	break;
	case T_NUM6:	__GETKEY(tKey6);	break;
	case T_NUM7:	__GETKEY(tKey7);	break;
	case T_NUM8:	__GETKEY(tKey8);	break;
	case T_NUM9:	__GETKEY(tKey9);	break;
	case T_POINT:	__GETKEY(tKeyDot);	break;

	case T_APAP:
	case T_VAL:
	case T_ANN:
	case T_CORR:
	case T_SK1:
	case T_SK2:
	case T_SK3:
	case T_SK4:
	case T_SKHAUT:
	case T_SKBAS:
	case T_SKVAL:
	case T_SKCLEAR:
	case T_SK10:
	default:
		return '\0';
	}

	return 1;
}

void ClearText( char *szInput, int xInput, int yInput )
{
	int nWidth;

	// Erase all text
	nWidth =  StringWidth( szInput ) + 4;

	_SetArea( xInput, yInput, xInput + nWidth, yInput + _SIZE_MEDIUM_, _OFF_ );
}

static int EraseCharacterInText( char *szInput, int xInput, int yInput, int nIndex  )
{
	int i = 0;
	int len = strlen( szInput );

	// Clear current text.
	ClearText( szInput, xInput, yInput );

	//
	for( i = nIndex; i < len; i++ )
	{
		szInput[i] = szInput[i+1];
	}

	szInput[i] = '\0';

	if(( nIndex >= len - 1 ) && ( nIndex != 0 ))
	{
		nIndex--;
	}

	DisplayText( szInput, xInput, yInput, nIndex, 0  );

	return nIndex;
}

void DisplayText( char *szInput, int x, int y, int index, int bErase  )
{
	// Display the string
	char tc[2];

	int xCursor, yCursor;

	int i = 0;
	int len = strlen( szInput );

	tc[1] = '\0';

	if( bErase == 1 )
	{
		ClearText( szInput, x, y );
	}

	xCursor = x;
	yCursor = y + _SIZE_MEDIUM_;

	for( i = 0; i < len; i++ )
	{
		tc[0] = szInput[i];
		if( i == index )
		{

			xCursor = x;
		}

		DrawString( tc, x, y, _OFF_ );

		x += StringWidth( tc );
	}

	if( index >= len )
	{
		xCursor = x;
	}

	if( index != -1 )
	{
		DrawLine( xCursor, yCursor, xCursor + 4, yCursor, _ON_ );
	}

	PaintGraphics();
}

static int SetCharacterInText( char *szInput, int xInput, int yInput,
							   int nIndex, char Character, int nMax )
{
	if( nIndex == nMax - 1 )
	{
		return 0;
	}

	// Clear current text.
	ClearText( szInput, xInput, yInput );


	if( szInput[nIndex] == '\0' )
	{
		szInput[nIndex] = Character;
		szInput[nIndex+1] = '\0';
	}
	else
	{
		szInput[nIndex] = Character;
	}

	DisplayText( szInput, xInput, yInput, nIndex, 0  );

	return 1;
}

int HelperManageInput( char *szLabel, int xLabel, int yLabel,
					   char *szInput, int xInput, int yInput,
					   int nMaxLen, int bNumerical, int nTimeout )
{	
	FILE* hKeyboard;


	int nResult	= 0;

	int nKey1		= 0;
	int nKey2		= 0;
	int nKeyIndex	= 0;
	char Character	= '\0';

	int n;

	int nState;

	n = 0;
	nResult = 1;
	nState = STATE_INPUT_WAIT_KEY;


    int iRet;

	iRet = ShowEntry(szLabel, "", szInput, 0, nMaxLen, 4);

	if (iRet<0)
		nResult = __EXIT_KEY;
	else
		nResult = __ENTER_KEY;

	return nResult;



	CreateGraphics(_MEDIUM_);


	if( szLabel != NULL )
	{
		DisplayText( szLabel, xLabel, yLabel, -1, 0 );
	}

	n = strlen( szInput );
	DisplayText( szInput, xInput, yInput, n, 0 );

	hKeyboard = fopen("KEYBOARD", "r");

	while( nState !=  STATE_INPUT_END )
	{
		switch( nState )
		{
			case STATE_INPUT_WAIT_KEY:
			{
				// Wait a key
				if( Telium_Ttestall(KEYBOARD, nTimeout ) == KEYBOARD )
				{
					nKey1 = getchar();

					// Monitor key Step.
					switch( nKey1 )
					{
					case T_CORR:
						// Arrow keys.
						n = EraseCharacterInText( szInput, xInput, yInput, n );
						break;

					case T_SK1: // Left
						// Arrow keys.
						if( n != 0 )
						{
							n--;
							DisplayText( szInput, xInput, yInput, n, 1 );
						}
						break;

					case T_SK4: // Right
						// Arrow keys.
						if( szInput[n] != '\0' )
						{
							n++;
						}
						DisplayText( szInput, xInput, yInput, n, 1 );
						break;

					case T_VAL:
						nResult = __ENTER_KEY;
						nState =  STATE_INPUT_END;
						break;

					case T_SK2: // Back
						nResult = __BACK_KEY;
						nState =  STATE_INPUT_END;
						break;

					case T_ANN:
						nResult = __EXIT_KEY;
						nState =  STATE_INPUT_END;
						break;

					case T_NUM0:
					case T_NUM1:
					case T_NUM2:
					case T_NUM3:
					case T_NUM4:
					case T_NUM5:
					case T_NUM6:
					case T_NUM7:
					case T_NUM8:
					case T_NUM9:
					case T_POINT:
						nState = STATE_INPUT_KEY_STEP;
						nKeyIndex = 0;

						if( GetNextCharacter( nKey1, &nKeyIndex, &Character ) != 0 )
						{
							SetCharacterInText( szInput, xInput, yInput, n, Character, nMaxLen );
						}
						break;
					}
				}
				else
				{
						nState = STATE_INPUT_END;
						nResult = 0; // Timeout;
				}
			}
			break;

			case STATE_INPUT_KEY_STEP:
			{
				// Monitor key Step.
				Character = nKey1;

				if(( bNumerical == 0) &&  ( Telium_Ttestall(KEYBOARD, 100 ) == KEYBOARD ))
				{
					nKey2 = getchar();

					if( nKey1 != nKey2 )
					{
						nKey1 = nKey2;
						Character = nKey1;
						n++;

						switch( nKey1 )
						{
						case T_CORR:
							// Arrow keys.
							n = EraseCharacterInText( szInput, xInput, yInput, n );
							nState = STATE_INPUT_WAIT_KEY;
							break;

						case T_SK1: // Left
							// Arrow keys.
							if( n != 0 )
							{
								n--;
								DisplayText( szInput, xInput, yInput, n, 1 );
							}
							nState = STATE_INPUT_WAIT_KEY;
							break;

						case T_SK4: // Right
							// Arrow keys.
							if( szInput[n] != '\0' )
							{
								n++;
								DisplayText( szInput, xInput, yInput, n, 1 );
							}
							nState = STATE_INPUT_WAIT_KEY;
							break;

						case T_VAL:
							nResult = __ENTER_KEY;
							nState =  STATE_INPUT_END;
							break;

						case T_SK2: // Back
							nResult = __BACK_KEY;
							nState =  STATE_INPUT_END;
							break;


						case T_ANN:
							nResult = __EXIT_KEY;
							nState =  STATE_INPUT_END;
							break;

						case T_NUM0:
						case T_NUM1:
						case T_NUM2:
						case T_NUM3:
						case T_NUM4:
						case T_NUM5:
						case T_NUM6:
						case T_NUM7:
						case T_NUM8:
						case T_NUM9:
						case T_POINT:
							nKeyIndex = 0;
							SetCharacterInText( szInput, xInput, yInput, n, Character, nMaxLen );
							break;
						}
					}
					else
					{
						// Rotate on the same key.
						if( GetNextCharacter( nKey1, &nKeyIndex, &Character ) != 0 )
						{
							if( SetCharacterInText( szInput, xInput, yInput, n, Character, nMaxLen ) == 1 )
							{
							}
						}
						else
						{
							// ??
						}
					}
				}
				else
				{
					if( nKeyIndex == 0 )
					{
						SetCharacterInText( szInput, xInput, yInput, n, Character, nMaxLen );
					}

					n++;
					DisplayText( szInput, xInput, yInput, n, 1 );
					nState = STATE_INPUT_WAIT_KEY;
				}
			}
			break;
		}
	}

	fclose(hKeyboard);

	return nResult;
}

static int HelperDisplayMenuInternal(const char* szTitle, int bRadioButtons, int nDefaultChoice, int nItems, const char* Items[], int nBanners )
{
	int nSizeX;
	int nSizeY;
	StructList Menu;
	int i;

	int nInput;
	int nReturn;

	int nBannerHeight = 0;


	ENTRY_BUFFER Entry;

	if ((nDefaultChoice < 0) || (nDefaultChoice >= nItems))
		nDefaultChoice = 0;

	GetScreenSize(&nSizeY, &nSizeX);

	CreateGraphics(_MEDIUM_);

	switch( nBanners )
	{
	case __ONE_BANNER:
		nBannerHeight = 12;
		break;

	case __ALL_BANNERS:
		nBannerHeight = 19;
		break;

	case __NO_BANNER:
	default:
		nBannerHeight = 0;
		break;
	}

	memset(&Menu, 0, sizeof(Menu));
	Menu.MyWindow.left = 0;
	Menu.MyWindow.top = nBannerHeight;
	Menu.MyWindow.rigth = nSizeX - 1;
	Menu.MyWindow.bottom = nSizeY - 1;
	if (nSizeY == 128)
	{
		Menu.MyWindow.nblines = 10;
	}
	else
	{
		Menu.MyWindow.nblines = 5;
	}

	Menu.MyWindow.fontsize = _MEDIUM_;
	Menu.MyWindow.type = _PROPORTIONNEL_;
	Menu.MyWindow.font = 0;
	Menu.MyWindow.correct = _ON_;
	Menu.MyWindow.offset = 0;
	Menu.MyWindow.shortcommand = _ON_;
	if (bRadioButtons)
		Menu.MyWindow.selected = _ON_;
	else Menu.MyWindow.selected = _OFF_;
	Menu.MyWindow.thickness = 2;
	Menu.MyWindow.border = _ON_;
	Menu.MyWindow.popup = _NOPOPUP_;
	Menu.MyWindow.first = nDefaultChoice;
	Menu.MyWindow.current = nDefaultChoice;
	Menu.MyWindow.time_out = 60;
	Menu.MyWindow.title = (unsigned char*)szTitle;

	for(i = 0; i < nItems; i++)
		Menu.tab[i] = (unsigned char*)Items[i];

	G_List_Entry((void*)&Menu);
	Telium_Ttestall(ENTRY, 0);
	nInput = Get_Entry((void*)&Entry);

	switch( nInput )
	{
	case CR_ENTRY_OK:
		nReturn = Entry.d_entry[0];
		break;

	case CR_ENTRY_NOK:
		nReturn = __EXIT_KEY;
		break;

	default:
		nReturn = __BACK_KEY;
		break;
	}

	return nReturn;
}

int HelperDisplayMenu( const char* szTitle, int nDefaultChoice,
					   int nItems, const char* Items[],
					   int nBanners )
{
	return HelperDisplayMenuInternal( szTitle, FALSE, nDefaultChoice,
									  nItems, Items, nBanners );
}

int ManageEnterPinMsg_Naps(int nb_try, int *PtrUsePinpad, int nCardHolderLang, char* pGoalText, char* pGoalHelp,unsigned char* PinClair, unsigned char *LgPin)
{
	//int ii=0;
	int  interfaceok;
	char buffer[40];
	char  PIN[8]="\0";
	int a=0;
	int nResult;
	int event;
	//int iRet;
	char test[3]="\0";
	char PINC[8]="\0";
	T_CUTERMkey key=-1;
	int PinLengh=0;

	char AFFMONT[248];
	char DEVALP[4];

	//Os__xprintaa("1223");

	//*PtrUsePinpad = FALSE;


	if(!memcmp(StructInfoCaisse[0].DE,"504",3))
		sprintf(DEVALP,"%s","MAD");
	else if(!memcmp(StructInfoCaisse[0].DE,"978",3))
		sprintf(DEVALP,"%s","EUR");
	else if(!memcmp(StructInfoCaisse[0].DE,"840",3))
		sprintf(DEVALP,"%s","USD");
	else
		sprintf(DEVALP,"%s","ERR");

//	sprintf(AFFMONT,"%s%d,%.02d%s\nPIN :\n%s","MONTANT :",atol(StructInfoCaisse[0].MT)/100,atol(StructInfoCaisse[0].MT)%100,DEVALP,PIN);
	if(StructInfoPayment.CodeFunction == PAIEMENT_POURBOIRE)
		sprintf(AFFMONT,"Montant total :\n%d,%.02d%s\nTotal avec pourboire :\n%d,%.02d%s\n\nSaisir le PIN pour confirmer:\n%s",
				StructInfoPayment.Transaction_Amount/100,
				StructInfoPayment.Transaction_Amount%100,
				"MAD",
				(StructInfoPayment.Transaction_Amount + StructInfoPayment.MontantPourboireInterchange)/100,
				(StructInfoPayment.Transaction_Amount + StructInfoPayment.MontantPourboireInterchange)%100,
				"MAD",
				PIN
				);
	else
		sprintf(AFFMONT,"Saisir code PIN\n pour :%d,%.02d%s\n\n%s",
				StructInfoPayment.Transaction_Amount/100,
				StructInfoPayment.Transaction_Amount%100,"MAD",PIN);
	//Os__xprintaa("002");
	APEMV_UI_MessageDisplayDelay (AFFMONT, 0); 		//Test17
	//CHECK(iRet>=0, lblKO);
	//iRet = TimerStart(0, 15*1000);
	while(key!=T_VAL || key!=T_ANN)
	{
		nResult=CUTERMgetKey(15000, &key);
		//lTimeOut = TimerGet(0);

		switch (key)
		{
			case T_NUM0:
			case T_NUM1:
			case T_NUM2:
			case T_NUM3:
			case T_NUM4:
			case T_NUM5:
			case T_NUM6:
			case T_NUM7:
			case T_NUM8:
			case T_NUM9:
				//Os__xprintaa("002");
			//ii++;
			if(PinLengh<8)
			{
				strcat(PIN,"*");
				sprintf(&PINC[PinLengh],"%c",(char)key);
				PinLengh++;
			}
			if(PinLengh==8)
            memcpy(PIN,"********",8);

			////APEMV_UI_MessageDisplayDelay(PINC,0);
			//sprintf(AFFMONT,"%s%d,%.02d%s\nPIN :\n%s","MONTANT :",atol(StructInfoCaisse[0].MT)/100,atol(StructInfoCaisse[0].MT)%100,DEVALP,PIN);
			if(StructInfoPayment.CodeFunction == PAIEMENT_POURBOIRE){

				sprintf(AFFMONT,"Montant total :\n%d,%.02d%s\nTotal avec pourboire :\n%d,%.02d%s\n\nSaisir le PIN pour confirmer:\n%s",
				StructInfoPayment.Transaction_Amount/100,
				StructInfoPayment.Transaction_Amount%100,
				"MAD",
				(StructInfoPayment.Transaction_Amount + StructInfoPayment.MontantPourboireInterchange)/100,
				(StructInfoPayment.Transaction_Amount + StructInfoPayment.MontantPourboireInterchange)%100,
				"MAD",
				PIN
				);
			}

			else{
				sprintf(AFFMONT,"Saisir code PIN\n pour :%d,%.02d%s\n\n%s",
				StructInfoPayment.Transaction_Amount/100,
				StructInfoPayment.Transaction_Amount%100,"MAD",PIN);
				//Os__xprintaa("003");
			}

			APEMV_UI_InfoDisplay (AFFMONT);
			a=0;
			break;


			case T_CORR:
				memset(PIN,0,strlen(PIN));
				memset(PINC,0,strlen(PINC));
				PinLengh=0;
				////APEMV_UI_MessageDisplayDelay(PIN,0);
				//sprintf(AFFMONT,"%s%d,%.02d%s\nPIN :\n%s","MONTANT :",atol(StructInfoCaisse[0].MT)/100,atol(StructInfoCaisse[0].MT)%100,DEVALP,PIN);
				if(StructInfoPayment.CodeFunction == PAIEMENT_POURBOIRE)
					sprintf(AFFMONT,"Montant total :\n%d,%.02d%s\nTotal avec pourboire :\n%d,%.02d%s\n\nSaisir le PIN pour confirmer:\n%s",
							StructInfoPayment.Transaction_Amount/100,
							StructInfoPayment.Transaction_Amount%100,
							"MAD",
							(StructInfoPayment.Transaction_Amount + StructInfoPayment.MontantPourboireInterchange)/100,
							(StructInfoPayment.Transaction_Amount + StructInfoPayment.MontantPourboireInterchange)%100,
							"MAD",
							PIN
							);
				else{

					sprintf(AFFMONT,"Saisir code PIN\n pour :%d,%.02d%s\n\n%s",
					StructInfoPayment.Transaction_Amount/100,
					StructInfoPayment.Transaction_Amount%100,"MAD",PIN);
					//Os__xprintaa("004");
				}

				APEMV_UI_InfoDisplay (AFFMONT);
				a++;
			break;

			case T_VAL:
				//if(ii >0 && ii < 4) return KO;
				StructInfoPayment.Lg_pin =PinLengh;
				//encryp_code_conf(PINC,StructInfoPayment.Lg_pin);
				*LgPin = StructInfoPayment.Lg_pin;
				memcpy(PinClair,PINC,*LgPin);
				return OK;
			break;

			case T_ANN:
				memset(PINC,0,8);
				//Telium_Ttestall(0,3*100);
				//StructInfoPayment.Event=KEY_CLEAR;
				//CUTERMclearDisplayT1();
				//Os__xprintaa("T_ANNS");
				return KO;
			break;


			default:



				//sprintf(AFFMONT,"%s%d,%.02d%s\nPIN :\n%s","MONTANT :",atol(StructInfoCaisse[0].MT)/100,atol(StructInfoCaisse[0].MT)%100,DEVALP,PIN);
				if(StructInfoPayment.CodeFunction == PAIEMENT_POURBOIRE)
					sprintf(AFFMONT,"Montant total :\n%d,%.02d%s\nTotal avec pourboire :\n%d,%.02d%s\n\nSaisir le PIN pour confirmer:\n%s",
							StructInfoPayment.Transaction_Amount/100,
							StructInfoPayment.Transaction_Amount%100,
							"MAD",
							(StructInfoPayment.Transaction_Amount + StructInfoPayment.MontantPourboireInterchange)/100,
							(StructInfoPayment.Transaction_Amount + StructInfoPayment.MontantPourboireInterchange)%100,
							"MAD",
							PIN
							);
				else
				{
					sprintf(AFFMONT,"Saisir code PIN\n pour :%d,%.02d%s\n\n%s",
							StructInfoPayment.Transaction_Amount/100,
							StructInfoPayment.Transaction_Amount%100,"MAD",PIN);
					//Os__xprintaa("005");
				}
				APEMV_UI_InfoDisplay (AFFMONT); 		//Test17
				//a=CUTERMgetKey(3000, &key);
				a++;
			break;
			//return KO;
		}
		key = 0;
		if(a==2){
			//CUTERMclearDisplayT1();
			return KO;}
//		if(lTimeOut>0)
//	    TimerStop(0);
//			return KO;

	}
	//CUTERMclearDisplayT1();
	return KO;
}

int HelperDisplayChoices( const char* szTitle, int nDefaultChoice,
						  int nItems, const char* Items[],
					      int nBanners )

{
	return HelperDisplayMenuInternal( szTitle, TRUE, nDefaultChoice,
									  nItems, Items,
									  nBanners );
}

int HelperNumericInput(const char* szTitle, const char* szLabel, int nMinLength, int nMaxLength, const char* szDefault, char* szInput)
{
	int nSizeX;
	int nSizeY;
	StructList Input;
	int nInput;
	int nReturn;

	TAB_ENTRY_STRING EntryNumeric;
	ENTRY_BUFFER Entry;

	if (nMaxLength >= 20)
		nMaxLength = 20;
	if (nMinLength > nMaxLength)
		nMinLength = nMaxLength;

	GetScreenSize(&nSizeY, &nSizeX);

	memset(&Input, 0, sizeof(Input));
	Input.MyWindow.left = 0;
	Input.MyWindow.top = 0;
	Input.MyWindow.rigth = nSizeX - 1;
	Input.MyWindow.bottom = nSizeY - 1;
	if (nSizeY == 128)
		Input.MyWindow.nblines = 10;
	else Input.MyWindow.nblines = 5;
	Input.MyWindow.fontsize = _MEDIUM_;
	Input.MyWindow.type = _PROPORTIONNEL_;
	Input.MyWindow.font = 0;
	Input.MyWindow.correct = _ON_;
	Input.MyWindow.offset = 0;
	Input.MyWindow.shortcommand = _ON_;
	Input.MyWindow.selected = _OFF_;
	Input.MyWindow.thickness = 2;
	Input.MyWindow.border = _ON_;
	Input.MyWindow.popup = _NOPOPUP_;
	Input.MyWindow.first = 0;
	Input.MyWindow.current = 0;
	Input.MyWindow.time_out = 120;
	Input.MyWindow.title = (unsigned char*)szTitle;

	Input.tab[0] = (unsigned char*)szLabel;
	Input.tab[1] = (unsigned char*)szDefault;

	EntryNumeric.mask = 0;
	EntryNumeric.time_out = 120;
	EntryNumeric.line = 32;
	if (nMaxLength <= 18)
		EntryNumeric.column = 10;
	else if (nMaxLength == 19)
		EntryNumeric.column = 6;
	else EntryNumeric.column = 4;
	EntryNumeric.echo = ECHO_NORMAL;
	EntryNumeric.nb_max = nMaxLength;
	EntryNumeric.nb_min = nMinLength;

	G_Numerical_Entry((void*)&Input, (void*)&EntryNumeric);
	Telium_Ttestall(ENTRY, 0);
	nInput = Get_Entry((void*)&Entry);

	szInput[0] = '\0';

	switch( nInput )
	{
	case  CR_ENTRY_OK:
		strncpy(szInput, (char*)Entry.d_entry, Entry.d_len);
		szInput[Entry.d_len] = '\0';
		nReturn = __ENTER_KEY;
		break;

	case CR_ENTRY_VALIDATED:
		strcpy(szInput, szDefault);
		nReturn = __ENTER_KEY;
		break;

	case CR_ENTRY_NOK:
		nReturn = __BACK_KEY;
		break;

	default:
		nReturn = __EXIT_KEY;
		break;
	}

	return nReturn;
}

int PrintText( char *szText, int bClose )
{
	static FILE* hPrinter = NULL;

	if( hPrinter == NULL )
	{
		hPrinter = fopen( "PRINTER", "w-" );
	}

	if( hPrinter != NULL )
	{
		fprintf( hPrinter, szText );

		if( bClose == 1 )
		{
			Telium_Ttestall ( PRINTER , 0 );
			fclose( hPrinter );
			hPrinter = NULL;
		}

		return 1;
	}

	return 0;
}
