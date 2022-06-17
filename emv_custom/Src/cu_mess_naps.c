/**
* \file cu_mess.c
* \brief This module contains the functions used for message management.
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
#include "_emvdctag_.h"
#include "TlvTree.h"
#include "cu_base.h"
#include "cu_term.h"
#include "cu_mess.h"
#include "MSGlib.h"
#include "MessagesDefinitions.h"
#include "cu_serv.h"
#include "def_tag.h"

#include "Goal.h"


#define	VX520_TERMINAL 1
#define	VX675_TERMINAL 0

// Properties of image (Goal)
// =======================================
static const ST_IMAGE cmn_xImage480 =
{
	{GL_COLOR_WHITE, TRUE, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_MEDIUM}, GL_DIRECTION_NONE, {0, 0, 0, 0, GL_COLOR_RED}, {70, 70, 70, 70} },
	{TRUE, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_MEDIUM}, GL_DIRECTION_NONE, {1, 1, 1, 1, GL_COLOR_BLACK}, {2, 1, 2, 1} }
};

static const ST_IMAGE cmn_xImage240 =
{
	{GL_COLOR_BLACK, TRUE, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_MEDIUM}, GL_DIRECTION_NONE, {0, 0, 0, 0, GL_COLOR_RED}, {20, 20, 20, 20} },
	{TRUE, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_MEDIUM}, GL_DIRECTION_NONE, {1, 1, 1, 1, GL_COLOR_BLACK}, {2, 1, 2, 1} }
};

static const ST_IMAGE cmn_xImage2_480 =
{
	{GL_COLOR_BLACK, NULL, {"LUCIDA", GL_FONT_STYLE_NORMAL, GL_SCALE_MEDIUM}, GL_DIRECTION_NONE, {0, 0, 0, 0, GL_COLOR_RED}, {100, 0, 100, 100} },
	{TRUE, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_MEDIUM}, GL_DIRECTION_NONE, {1, 1, 1, 1, GL_COLOR_BLACK}, {2, 1, 2, 1} }
};

static const ST_IMAGE cmn_xImage2_240 =
{
	{GL_COLOR_BLACK, NULL, {"LUCIDA", GL_FONT_STYLE_NORMAL, GL_SCALE_MEDIUM}, GL_DIRECTION_NONE, {0, 0, 0, 0, GL_COLOR_RED}, {50, 0, 50, 50} },
	{TRUE, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_MEDIUM}, GL_DIRECTION_NONE, {1, 1, 1, 1, GL_COLOR_BLACK}, {2, 1, 2, 1} }
};

/* ========================================================================	*/
/* Type declaration                                                         */
/* ========================================================================	*/
typedef struct
{
  int   mess_nb;
  char *english;
  char *french;
} S_TAB_LANG;

static const char no_mess_found       [] = "Mess not found";
const char	gcardNumber	[] = "";
static char* ProductName(void) ;// lkeddouch 07/03/2019
/* ========================================================================	*/
/* Constant declaration                                                     */
/* ========================================================================	*/

//ATTENTION //ATTENTION //ATTENTION //ATTENTION kamal 120305
//les message de 0 à 183 sont réservés
//les message de 0 à 183 sont réservés
//ATTENTION //ATTENTION //ATTENTION //ATTENTION kamal 120305
//Mouna050405_début
const TAppMsg ct_DefMess_Cmi[C_NB_DEFAULT_MESS_CMI] =
   {
		  /* English				Frensh						Arabic												Spanish					*/
/*0**/   {"AMOUNT",					"MONTANT",					"\xC7\xE4\xE5\xC8\xE4\xDA",							"AMOUNT"				},
/*1**/   {"FRENSH",					"FRANCAIS",					"\xC7\xE4\xE1\xD1\xE6\xD3\xEA\xC9",					"FRENSH"				},
/*2**/   {"CANCELLATION",			"ANNULATION",				"\xC7\xE4\xC5\xE4\xDA\xC7\xC1",						"CANCELLATION"			},
/*3**/   {"NOT ALLOWED",			"NON PERMISE",				"\xDA\xEA\xD1\x20\xE5\xCA\xC7\xCD\xC9",				"NOT ALLOWED"			},
/*4**/   {".CARD NUMBER",			"NUM.CARTE",				"\xD1\xE2\xE5\x20\xC7\xE4\xC8\xD7\xC7\xE2\xC9",		".CARD NUMBER"			},
/*5**/   {"TRANSACTION NUMBER",		"NUM.TRANSACTION",			"\xD1\xE2\xE5\x20\xC7\xE4\xE5\xD9\xC7\xE5\xE4\xC9",	"TRANSACTION NUMBER"	},
/*6**/   {"AUTHORIZATION NUMBER.",	"NUM.AUTORISATION",			"\xD1\xE2\xE5\x20\xC7\xE4\xCA\xD1\xCE\xEA\xD5",		"AUTHORIZATION NUMBER."	},
/*7**/   {"TRANSACTION",			"TRANSACTION",				"\xE5\xD9\xC7\xE5\xE4\xC9",							"TRANSACTION"			},
/*8**/   {"INEXISTENT",				"INEXISTANTE",				"\xDA\xEA\xD1\x20\xE5\xE8\xCC\xE8\xCF\xC9",			"INEXISTENT"			},
/*9**/   {"ALREADY CANCELED",		"DEJA ANNULEE",				"\xC3\xE4\xDA\xEA\xCA\x20\xD3\xC7\xC8\xE2\xC7",		"ALREADY CANCELED"		},
/*10**/  {"ALREADY PRINTED",		"DEJA EDITE",				"\xE5\xCD\xCF\xCB\xC9\x20\xD3\xC7\xC8\xE2\xC7",		"ALREADY PRINTED"		},
/*11**/  {"MINIMUM AMOUNT",			"MONTANT INSUFFISANT",		"\xC7\xE4\xE5\xC8\xE4\xDA\x20\xDA\xEA\xD1\x20\xE3\xC7\xE1\xEA",			"MINIMUM AMOUNT"		},
/*12**/  {"DOUBLE TRANSACTION",		"TRANSACTION EN DOUBLE",	"\xE5\xD9\xC7\xE5\xE4\xC9\x20\xE5\xE3\xD1\xD1\xC9",	"DOUBLE TRANSACTION"	},
/*13**/  {"DO Y WANT TO CONTINUE",	"VOULEZ-VOUS CONTINUER",	"\xE7\xE4\x20\xCA\xD1\xEA\xCF\x20\xC7\xE4\xE5\xCA\xC7\xC8\xD9\xC9",		"DO Y WANT TO CONTINUE"	},
/*14**/  {"BADGE",					"BADGE",					"\xD4\xC7\xD1\xC9",									"BADGE"					},
/*15**/  {"ILLISIBLE",				"ILLISIBLE",				"\xDA\xEA\xD1\x20\xE5\xE2\xD1\xE8\xC1",				"ILLISIBLE"				},
/*16**/  {"INVALID",				"INVALIDE",					"\xDA\xEA\xD1\x20\xD5\xC7\xE4\xCD\xC9",				"INVALID"				},
/*17**/  {"MERCHANT NUMBER",		"NUM.COMMERCANT",			"\xD1\xE2\xE5\x20\xC7\xE4\xCA\xC7\xCC\xD1",			"MERCHANT NUMBER"		},
/*18**/  {"POINT.SALES NUM",		"NUM.POINT.VENTE",			"\xD1\xE2\xE5\x20\xE5\xCD\xD7\xC9\x20\xC7\xE4\xCF\xE1\xD9",				"POINT.SALES NUM"		},
/*19**/  {"TERMINAL.NUM",			"NUM.TERMINAL",				"\xD1\xE2\xE5\x20\xE5\xCD\xD7\xC9\x20\xC7\xE4\xCF\xE1\xD9",				"TERMINAL.NUM"			},
/*20**/  {"PASSWORD",				"MOT DE PASSE",				"\xE3\xE4\xE5\xC9\x20\xC7\xE4\xD3\xD1",				"PASSWORD"				},
/*21**/  {"ARE YOU SURE",			"ETES VOUS SUR",			"\xE7\xE4\x20\xC3\xE6\xCA\x20\xE5\xCA\xC3\xE3\xCF",	"ARE YOU SURE"			},
/*22**/  {"OPERATION",				"OPERATION",				"\xC7\xE4\xD9\xE5\xE4\xEA\xC9",						"OPERATION"				},
/*23**/  {"IN PROGRESS",			"EN COURS",					"\xCC\xC7\xD1\xEA",									"IN PROGRESS"			},
/*24**/  {"CONTACT THE",			"CONTACTER LE",				"\xCA\xE8\xC7\xD5\xE4\x20\xE5\xD9",					"CONTACT THE"			},
/*25**/  {"SERVER CENTER",			"CENTRE SERVEUR",			"\xE5\xD1\xE3\xD2\x20\xC7\xE4\xCE\xC7\xCF\xE5",		"SERVER CENTER"			},
/*26**/  {"SUCCEDED",				"REUSSIE",					"\xE6\xC7\xCC\xCD\xC9",								"SUCCEDED"				},
/*27**/  {"PARAMETERS.PROBLEM",		"PROBLEME.PARAMETRES",		"\xE5\xD4\xE3\xE4\xC9\x20\xC5\xD9\xCF\xC7\xCF\xC7\xCA",					"PARAMETERS.PROBLEM"	},
/*28**/  {"NO TRANS.",				"PAS DE TRANS.",			"\xE4\xC7\x20\xE5\xD9\xC7\xE5\xE4\xC7\xCA",			"NO TRANS."				},
/*29**/  {"TRANSACTION",			"TRANSACTION",				"\xE5\xD9\xC7\xE5\xE4\xC9",							"TRANSACTION"			},
/*30**/  {"NOT ACCEPTED",			"NON ACCEPTEE",				"\xDA\xEA\xD1\x20\xE5\xE2\xC8\xE8\xE4",				"NOT ACCEPTED"			},
/*31**/  {"Kernel Reset",			"INIT. NOYAU",				"\xD6\xC8\xD7\x20\xC7\xE4\xE6\xE8\xC7\xC9",			"Kernel Reset"			},
/*32**/  {"Please Wait...",			"PATIENTEZ SVP",			"\xC7\xE6\xCA\xD8\xD1\x20\xE5\xE6\x20\xE1\xD6\xE4\xE3\...",				"Please Wait..."		},
/*33**/  {"ABANDON !!",				"ABANDON !!",				"\xC5\xE6\xD3\xCD\xC7\xC8\!!",						"ABANDON !!"			},
/*34**/  {"TELEPARAMETRAGE",		"TELEPARAMETRAGE",			"\xCA\xCD\xCF\xEA\xCA\x20\xC7\xE4\xC5\xD9\xCF\xC7\xCF\xC7\xCA",			"TELEPARAMETRAGE"		},
/*35**/  {"BLACK LIST",				"LISTE NOIRE",				"\xC7\xE4\xE2\xC7\xC6\xE5\xC9\x20\xC7\xE4\xD3\xE8\xCF\xC7\xC1",			"BLACK LIST"			},
/*36**/  {"DELAI EXPIRED",			"DELAI EXPIRE",				"\xC5\xE6\xCA\xE7\xC7\xC1\x20\xCA\xC7\xD1\xEA\xCE\x20\xC7\xE4\xD5\xE4\xC7\xCD\xEA\xC9",			"DELAI EXPIRED"			},
/*37**/  {"LOP CHARGED",			"LOP CHARGEE",				"LOP CHARGED",										"LOP CHARGED"			},
/*38**/  {"LOP NOT CHARGED",		"LOP NON CHARGEE",			"LOP NOT CHARGED",									"LOP NOT CHARGED"		},
/*39**/  {"REDRESSEMENT",			"REDRESSEMENT",				"\xCA\xE2\xE8\xEA\xE5",								"REDRESSEMENT"			},
/*40**/  {"PIN DEPASSED",			"PIN DEPASSE",				"\xD1\xE2\xE5\x20\xCA\xD9\xD1\xEA\xE1\x20\xE5\xCA\xCC\xC7\xE8\xD2",		"PIN DEPASSE"			},
/*41**/  {"TRYING NOMBRE",			"NOMBRE ESSAIS",			"\xD9\xCF\xCF\x20\xC7\xE4\xE5\xCD\xC7\xE8\xE4\xC7\xCA",					"TRYING NOMBRE"			},
/*42**/  {"TELEPHONE NUMBER",		"NUMERO DE TELEPHONE",		"\xD1\xE2\xE5\x20\xC7\xE4\xE7\xC7\xCA\xE1",			"TELEPHONE NUMBER"		},
/*43**/  {"SONDAGE",				"SONDAGE",					"SONDAGE",											"SONDAGE"				},
/*44**/  {"VALIDATION",				"VALIDATION",				"\xCA\xC3\xE3\xEA\xCF",								"VALIDATION"			},
/*45**/  {"ECHOUEE",				"ECHOUEE",					"\xE1\xC7\xD4\xE4\xC9",								"ECHOUEE"				},
/*46**/  {"ANNULATION PUCE",		"ANNULATION PUCE",			"\xC5\xE4\xDA\xC7\xC1\x20\xC7\xE4\xD1\xE2\xC7\xE2\xC9",					"ANNULATION PUCE"		},
/*47**/  {"ANNUL- PRE PUCE",		"ANNUL- PRE PUCE",			"ANNUL- PRE PUCE",									"ANNUL- PRE PUCE"		},
/*48**/  {"DUPLICATA",				"DUPLICATA",				"\xC7\xE4\xCA\xE3\xD1\xC7\xD1",						"DUPLICATA"				},
/*49**/  {"PREPAREZ",				"PREPAREZ",					"PREPAREZ",											"PREPAREZ"				},
/*50**/  {"ATTENTE CARTE",			"ATTENTE CARTE",			"\xC5\xE6\xCA\xD8\xC7\xD1\x20\xC7\xE4\xC8\xD7\xC7\xE2\xC9",				"ATTENTE CARTE"			},
/*51**/  {"APPLICATION",			"APPLICATION",				"\xCA\xD7\xC8\xEA\xE2",								"APPLICATION"			},
/*52**/  {"BLOQUEE",				"BLOQUEE",					"\xE5\xE2\xE1\xE8\xE4",								"BLOQUEE"				},
/*53**/  {"CARTE",					"CARTE",					"\xC8\xD7\xC7\xE2\xC9",								"CARTE"					},
/*54**/  {"",						"",							"",													""						},
/*55**/  {"",						"",							"",													""						},
/*56**/  {"",						"",							"",													""						},
/*57**/  {"TRANSPAC PB.",			"PB. TRANSPAC",				"TRANSPAC PB.",				"TRANSPAC PB."			},
/*58**/  {"PWCB Txn",				"PWCB Txn",					"PWCB Txn",					"PWCB Txn"				},
/*59**/  {"SITE PB.",				"PB. SITE",					"SITE PB.",					"SITE PB."				},
/*60**/  {"TAKE TICKET",			"COUPEZ TICKET",			"TAKE TICKET",				"TAKE TICKET"			},
/*61**/  {"VALIDATE TRANSACTION?",	"VALIDEZ TRANSACTION?",		"VALIDATE TRANSACTION?",	"VALIDATE TRANSACTION?"	},
/*62**/  {"CONTACT",				"APPEL",					"CONTACT",					"CONTACT"				},
/*63**/  {"CENTER",					"PHONIQUE",					"CENTER",					"CENTER"				},
/*64**/  {"CONFIRMED",				"CONFIRMEE",				"CONFIRMED",				"CONFIRMED"				},
/*65**/  {"GIVEN UP",				"ABANDONNEE",				"GIVEN UP",					"GIVEN UP"				},
/*66**/  {"DO Y WANT TO FORC THE",	"VOULEZ-VOUS FORCER LA",	"DO Y WANT TO FORC THE",	"DO Y WANT TO FORC THE"	},
/*67**/  {"RAZ RAM",				"RAZ RAM",					"RAZ RAM",					"RAZ RAM"				},
/*68**/  {"ERASE TRANSA.",			"EFFACE TRANSA.",			"ERASE TRANSA.",			"ERASE TRANSA."			},
/*69**/  {"ERASE PRE AUTO",			"EFFACE PRE AUTO",			"ERASE PRE AUTO",			"ERASE PRE AUTO"		},
/*70**/  {"ERASE BLACK LIST",		"EFFACE LISTE NOIRE",		"ERASE BLACK LIST",			"ERASE BLACK LIST"		},
/*71**/  {"Service Barred",			"Service Barre",			"Service Barred",			"Service Barred"		},
/*72**/  {"Issuer Auth",			"Auth Emetteur",			"Issuer Auth",				"Issuer Auth"			},
/*73**/  {"Failed",					"Echec",					"Failed",					"Failed"				},
/*74**/  {"Advice Required",		"Avis Requis",				"Advice Required",			"Advice Required"		},
/*75**/  {"(A)Adding DOLs",			"(A)Ajout DOLs",			"(A)Adding DOLs",			"(A)Adding DOLs"		},
/*76**/  {"Merchant Forces",		"Commercant Force",			"Merchant Forces",			"Merchant Forces"		},
/*77**/  {"Online? [Y/N]",			"Online? [O/N]",			"Online? [Y/N]",			"Online? [Y/N]"			},
/*78**/  {"CASH Amount",			"Mont CASH",				"CASH Amount",				"CASH Amount"			},
/*79**/  {"SALE Amount",			"Mont ACHAT",				"SALE Amount",				"SALE Amount"			},
/*80**/  {"CASHBACK Amt",			"Mnt CASHBACK",				"CASHBACK Amt",				"CASHBACK Amt"			},
/*81**/  {"Adding Keys",			"Ajout cles",				"Adding Keys",				"Adding Keys"			},
/*82**/  {"Error Key",				"Erreur cle",				"Error Key",				"Error Key"				},
/*83**/  {"Issuer Referral",		"Issuer Referral",			"Issuer Referral",			"Issuer Referral"		},
/*84**/  {"Card Referral",			"Card Referral",			"Card Referral",			"Card Referral"			},
/*85**/  {"Approved[Y/N]",			"Approuve[O/N]",			"Approved[Y/N]",			"Approved[Y/N]"			},
/*86**/  {"Online Reversal",		"Annul Online",				"Online Reversal",			"Online Reversal"		},
/*87**/  {"Printing Receipt",		"impress. ticket",			"Printing Receipt",			"Printing Receipt"		},
/*88**/  {"User Cancelled",			"Anul utilisateur",			"User Cancelled",			"User Cancelled"		},
/*89**/  {"Transaction Xfer",		"Transaction Xfer",			"Transaction Xfer",			"Transaction Xfer"		},
/*90**/  {"Detank Txn Data",		"Detank Txn Data",			"Detank Txn Data",			"Detank Txn Data"		},
/*91**/  {"Store     [Y/N]",		"Enregistrer[Y/N]",			"Store     [Y/N]",			"Store     [Y/N]"		},
/*92**/  {"Storing Txn",			"Enregistre Txn",			"Storing Txn",				"Storing Txn"			},
/*93**/  {"Print Txn Data",			"imp Données Txn ",			"Print Txn Data",			"Print Txn Data"		},
/*94**/  {"PRINTING",				"IMPRESSION",				"PRINTING",					"PRINTING"				},
/*95**/  {"Clear Txn Data",			"Clear Txn Data",			"Clear Txn Data",			"Clear Txn Data"		},
/*96**/  {"Confirm Appn ?",			"Confirmer App?",			"Confirm Appn ?",			"Confirm Appn ?"		},
/*97**/  {"Use this Appln ?",		"Util cette Appl?",			"Use this Appln ?",			"Use this Appln ?"		},
/*98**/  {"Set Debug",				"Set Debug",				"Set Debug",				"Set Debug"				},
/*99**/  {"SWIPE MERCHANT",			"PASSEZ BADGE",				"SWIPE MERCHANT",			"SWIPE MERCHANT"		},
/*100*/  {"CARD",					"COMMERCANT",				"CARD",						"CARD"					},
/*101**/ {"SWIPE HOLDER",			"INTRO. LA CARTE",			"SWIPE HOLDER",				"SWIPE HOLDER"			},
/*102**/ {"CARD?",					"DU CLIENT?",				"CARD?",					"CARD?"					},
/*103**/ {"CARD NUMBER",			"NUMERO DE CARTE",			"CARD NUMBER",				"CARD NUMBER"			},
/*104**/ {"EXP. DATE MMYY",			"DATE EXP. MMAA",			"EXP. DATE MMYY",			"EXP. DATE MMYY"		},
/*105**/ {"INVALID DATE",			"DATE INVALIDE",			"INVALID DATE",				"INVALID DATE"			},
/*106**/ {"CONTROL FRAUD",			"CONTROLE FRAUD",			"CONTROL FRAUD",			"CONTROL FRAUD"			},
/*107**/ {"YES->E   NO->C",			"OUI->V  NON->A",			"YES->E   NO->C",			"YES->E   NO->C"		},
/*108**/ {"DIALING",				"NUMEROTATION",				"DIALING",					"DIALING"				},
/*109**/ {"CALL PROBLEM",			"PROBLEME APPEL",			"CALL PROBLEM",				"CALL PROBLEM"			},
/*110**/ {"CONNECTED",				"CONNECTE",					"CONNECTED",				"CONNECTED"				},
/*111**/ {"NO TONE",				"PAS DE TONALITE",			"NO TONE",					"NO TONE"				},
/*112**/ {"CALLED LINE BUSY",		"LIGNE OCCUPEE",			"CALLED LINE BUSY",			"CALLED LINE BUSY"		},
/*113**/ {"MEMORY FULL",			"MEMOIRE PLEINE",			"MEMORY FULL",				"MEMORY FULL"			},
/*114**/ {"FORBIDDEN CALL",			"APPEL interdit",			"FORBIDDEN CALL",			"FORBIDDEN CALL"		},
/*115**/ {"DELAYED DIAL",			"NUMEROT RETARDE",			"DELAYED DIAL",				"DELAYED DIAL"			},
/*116**/ {"LINE BUSY",				"LIGNE OCCUPEE",			"LINE BUSY",				"LINE BUSY"				},
/*117**/ {"NOT CONNECTED",			"NON CONNECTE",				"NOT CONNECTED",			"NOT CONNECTED"			},
/*118**/ {"LINE PROBLEM",			"PROBLEME LIGNE",			"LINE PROBLEM",				"LINE PROBLEM"			},
/*119**/ {"PB. PORTEUSE",			"PB. PORTEUSE",				"PB. PORTEUSE",				"PB. PORTEUSE"			},
/*120**/ {"AUTOR. REQ. ",			"DEM. AUTOR.",				"AUTOR. REQ. ",				"AUTOR. REQ. "			},
/*121**/ {"ADVISE",					"AVIS",						"ADVISE",					"ADVISE"				},
/*122**/ {"CURRENT DAY BATCH",		"BATCH FIN JOURNEE",		"CURRENT DAY BATCH",		"CURRENT DAY BATCH"		},
/*123**/ {"REQUEST",				"DEMANDE",					"REQUEST",					"REQUEST"				},
/*124**/ {"ADVISE TRANSFERT",		"TRANSFERT AVIS",			"ADVISE TRANSFERT",			"ADVISE TRANSFERT"		},
/*125**/ {"RECONCILIATION",			"RECONCILIATION",			"RECONCILIATION",			"RECONCILIATION"		},
/*126**/ {"TELECOLLECTE",			"TELECOLLECTE",				"TELECOLLECTE",				"TELECOLLECTE"			},
/*127**/ {"COMMUNICATION PROBLEM",	"PB COMMUNICATION",			"COMMUNICATION PROBLEM",	"COMMUNICATION PROBLEM"	},
/*128**/ {"AUTORIZATION REFUSED",	"AUTOR. REFUSEE",			"AUTORIZATION REFUSED",		"AUTORIZATION REFUSED"	},
/*129**/ {"IMPOSSIBLE",				"IMPOSSIBLE",				"IMPOSSIBLE",				"IMPOSSIBLE"			},
/*130**/ {"TRANSACTION FILE",		"OPERATION NON PERMISE",	"\xC7\xE4\xD9\xE5\xE4\xEA\x20\xDA\xEA\xD1\x20",			"TRANSACTION FILE"		},
/*131**/ {"FULL",					"LANCER FIN JOUR.",			"FULL",						"FULL"					},
/*132**/ {"LUHN KEY",				"CLE DE LUHN",				"LUHN KEY",					"LUHN KEY"				},
/*133**/ {"INCORRECT",				"INCORRECTE",				"INCORRECT",				"INCORRECT"				},
/*134**/ {"CARD NOT",				"CARTE NON",				"CARD NOT",					"CARD NOT"				},
/*135**/ {"ACCEPTED",				"ACCEPTEE",					"ACCEPTED",					"ACCEPTED"				},
/*136**/ {"INCORRECT KEY",			"CLE",						"INCORRECT KEY",			"INCORRECT KEY"			},
/*137**/ {"ENTERED",				"INCORRECTE",				"ENTERED",					"ENTERED"				},
/*138**/ {"",						"",							"",							""						},
/*139**/ {"PRE-AUTORIZATION FILE",	"FICH PRE-AUTOR.",			"PRE-AUTORIZATION FILE",	"PRE-AUTORIZATION FILE"	},
/*140**/ {"FULL",					"PLEIN",					"FULL",						"FULL"					},
/*141**/ {"CVV2",					"CVV2",						"CVV2",						"CVV2"					},
/*142**/ {"LAST FOUR DIGITS?",		"4 DERNIERS DIGITS:",		"LAST FOUR DIGITS?",		"LAST FOUR DIGITS?"		},
/*143**/ {"READ",					"LUE",						"READ",						"READ"					},
/*144**/ {"DAT JJ/MM/AA HH:mm:SS",	"DAT JJ/MM/AA HH:mm:SS",	"DAT JJ/MM/AA HH:mm:SS",	"DAT JJ/MM/AA HH:mm:SS"	},
/*145**/ {"DAT JJ/MM/AA HH:mm",		"DAT JJ/MM/AA HH:mm",		"DAT JJ/MM/AA HH:mm",		"DAT JJ/MM/AA HH:mm"	},
/*146**/ {"ENTER NEW PWD",			"NOUVEAU PWD",				"ENTER NEW PWD",			"ENTER NEW PWD"			},
/*147**/ {"RENTER NEW PWD",			"CONFIRMER PWD",			"RENTER NEW PWD",			"RENTER NEW PWD"		},
/*148**/ {"DATA ENTERED",			"SAISIE",					"DATA ENTERED",				"DATA ENTERED"			},
/*149**/ {"NOT CORRECT!",			"INCORRECT!",				"NOT CORRECT!",				"NOT CORRECT!"			},
/*150**/ {"PIN PROBLEM",			"PROBLEME PIN",				"PIN PROBLEM",				"PIN PROBLEM"			},
/*151**/ {"DO Y WANT TO CONTINUE",	"VOULEZ-VOUS CONTINUEZ",	"DO Y WANT TO CONTINUE",	"DO Y WANT TO CONTINUE"	},
/*152**/ {"WITH PRE-AUT AMOUNT?",	"AVEC MONTANT PRE-AUT?",	"WITH PRE-AUT AMOUNT?",		"WITH PRE-AUT AMOUNT?"	},
/*153**/ {"ENTER PIN CODE!",		"ENTRER CODE PIN!",			"ENTER PIN CODE!",			"ENTER PIN CODE!"		},
/*154**/ {"DOM ACCOUNT",			"COMPTE DE DOMMIC",			"DOM ACCOUNT",				"DOM ACCOUNT"			},
/*155**/ {" PREAUTOR- PUCE",		" PREAUTOR- PUCE",			" PREAUTOR- PUCE",			" PREAUTOR- PUCE"		},
/*156**/ {"PRE-AUTHORIZATION NUM",	"NUM.PRE-AUTORISATION.",	"PRE-AUTHORIZATION NUM",	"PRE-AUTHORIZATION NUM"	},
/*157**/ {"PRE-AUTHORIZATION",		"PRE-AUTORISATION",			"PRE-AUTHORIZATION",		"PRE-AUTHORIZATION"		},
/*158**/ {"CONFIRM PRE PUCE",		"CONFIRM PRE PUCE",			"CONFIRM PRE PUCE",			"CONFIRM PRE PUCE"		},
/*159**/ {"GOING ONLINE:KO",		"ALLER ONLINE:KO",			"GOING ONLINE:KO",			"GOING ONLINE:KO"		},
/*160**/ {"TO ACCEPT?",				"A ACCEPTER?",				"TO ACCEPT?",				"TO ACCEPT?"			},
/*161**/ {"CHIP CARD",				"CARTE A PUCE",				"CHIP CARD",				"CHIP CARD"				},
/*162**/ {"USE CHIP",				"UTILISER LA PUCE!",		"USE CHIP",					"USE CHIP"				},
/*163**/ {"  CREDIT PUCE   "	,	"  CREDIT PUCE   ",			"  CREDIT PUCE   ",			"  CREDIT PUCE   "		},
/*164**/ {"INVALID CHIP",			"PUCE DEFECTUEUSE",			"INVALID CHIP",				"INVALID CHIP"			},
/*165**/ {"NO PAPER",				"PAS DE PAPIER",			"NO PAPER",					"NO PAPER"				},
/*166**/ {"COUV.OPENED",			"COUV.OUVERT",				"COUV.OPENED",				"COUV.OPENED"			},
/*167**/ {"CARD RETIRED",			"CARTE ABSENTE!",			"CARD RETIRED",				"CARD RETIRED"			},
/*168**/ {"USE ISO2 READER!",		"UTILISER LA PISTE!",		"USE ISO2 READER!",			"USE ISO2 READER!"		},
/*169**/ {"REFUND",					"CREDIT",					"REFUND",					"REFUND"				},
/*170**/ {"TIME OUT",				"TIME OUT",					"TIME OUT",					"TIME OUT"				},
/*171**/ {"TRANSACTION REFUSED",	"TRANSACTION REFUSEE",		"TRANSACTION REFUSED",		"TRANSACTION REFUSED"	},
/*172**/ {"PIN",					"PIN",						"PIN",						"PIN"					},
/*173**/ {"DATE",					"DATE",						"DATE",						"DATE"					},
/*174**/ {"ACHIEVED",				"ACCOMPLIE",				"ACHIEVED",					"ACHIEVED"				},
/*175**/ {"STAN NUMBER",			"NUMERO DE STAN",			"STAN NUMBER",				"STAN NUMBER"			},
/*176**/ {"CARD TO BE CAPTURED",	"CARTE A CAPTURER",			"CARD TO BE CAPTURED",		"CARD TO BE CAPTURED"	},
/*177**/ {"EXPIRED CARD",			"CARTE EXPIREE",			"EXPIRED CARD",				"EXPIRED CARD"			},
/*178**/ {"CARD ONLINE?",			"SONDAGE CARTE?",			"CARD ONLINE?",				"CARD ONLINE?"			},
/*179**/ {"CARD",					"CARTE",					"CARD",						"CARD"					},
/*180**/ {"AMEX"	,				"AMEX",						"AMEX",						"AMEX"					},
/*181**/ {"DINNER",					"DINNER",					"DINNER",					"DINNER"				},
/*182**/ {"JCB",					"JCB",						"JCB",						"JCB"					},
/*183**/ {"INCORRECT PIN",			"PIN INCORRECT",			"INCORRECT PIN",			"INCORRECT PIN"			},
/*184**/ {"DEM. ANNUL.",			"DEM. ANNUL.",				"DEM. ANNUL.",				"DEM. ANNUL."			},
/*185**/ {"NO DIAL TONE",			"PAS DE TONALITE",			"NO DIAL TONE",				"NO DIAL TONE"			},
/*186**/ {"BUSY",					"OCCUPE",					"BUSY",						"BUSY"					},
/*187**/ {"SERVEUR DOWN",			"SERVEUR DOWN",				"SERVEUR DOWN",				"SERVEUR DOWN"			},
/*188**/ {"PROBLEME COM.",			"PROBLEME COM.",			"PROBLEME COM.",			"PROBLEME COM."			},
/*189**/ {"NO CARRIER",				"PAS DE REPONSE",			"NO CARRIER",				"NO CARRIER"			},
/*190**/ {"NOMBRE DE FOIS",			"NOMBRE DE FOIS",			"NOMBRE DE FOIS",			"NOMBRE DE FOIS"		},
/*191**/ {"",						"",							"",							""						},
/*192**/ {"",						"",							"",							""						},
/*193**/ {"",						"",							"",							""						},
/*194**/ {"",						"",							"",							""						},
/*195**/ {"",						"",							"",							""						},
/*196**/ {"",						"",							"",							""						},
/*197**/ {"",						"",							"",							""						},
/*198**/ {"",						"",							"",							""						},
/*199**/ {"",						"",							"",							""						},
//Mouna050405_fin

//C_NB_DEFAULT_MESS_CMI

};

//Mouna310305_début
/*  Messages en francais */
static const char m_cmi_fra					    [] = "C.M.I"			;
static const char m_administration_cmi_fra      [] = "Administration"	;
static const char m_trasamanuel_cmi_fra         [] = "Transaction Manuelle";
static const char m_annulation_cmi_fra          [] = "Annulation"		;
static const char m_sondageautori_cmi_fra       [] = "Sondage Autori"	;
static const char m_credit_cmi_fra				[] = "Credit"			;
static const char m_duplicata_cmi_fra			[] = "Duplicata"		;
static const char m_batch_cmi_fra				[] = "Batch"			;
static const char m_report_cmi_fra				[] = "Rapport"			;
static const char m_maintenance_cmi_fra			[] = "Maintenance"		;
static const char m_preauto_cmi_fra				[] = "Pre autorisation"	;
static const char m_preautocomp_cmi_fra			[] = "Complement Pre autorisation"	;
static const char m_annulpreauto_cmi_fra		[] = "Annulation Pre autorisation"		;
static const char m_dernieretrs_cmi_fra			[] = "Derniere Trx"		;
static const char m_autretrs_cmi_fra			[] = "Autre Trx"		;
static const char m_dernierepreaut_cmi_fra	   	[] = "Derniere Pre autorisation"	;
static const char m_autrepreauto_cmi_fra	   	[] = "Autre Pre Auto"	;
static const char m_initparam_cmi_fra		    [] = "Init.Param"		;
static const char m_editparam_cmi_fra		    [] = "Edition Parametres"		;
static const char m_motpassecmr_cmi_fra		    [] = "Mot.Passe.Cmr"	;
static const char m_initialisation_cmi_fra	    [] = "Initialisation"	;
static const char m_findejournee_cmi_fra		[] = "Telecollecte"	;
static const char m_listenoire_cmi_fra			[] = "Liste Noire"		;
static const char m_totaux_cmi_fra				[] = "Totaux"			;
static const char m_journal_cmi_fra				[] = "Journal"			;
static const char m_journalpreauto_cmi_fra		[] = "Journal Pre autorisation"	;
static const char m_razram_cmi_fra				[] = "Raz Ram"			;
static const char m_effacetransa_cmi_fra		[] = "Efface Transa"	;
static const char m_effacepreauto_cmi_fra		[] = "Efface Pre Auto"	;
static const char m_razlop_cmi_fra				[] = "Raz Lop"			;
static const char m_edittrsemv_cmi_fra			[] = "Edit Trs Emv"		;
static const char m_totale_cmi_fra			   	[] = "Totale"			;
static const char m_partielle_cmi_fra		   	[] = "Partielle"		;
static const char m_NumCmr_cmi_fra		    	[] = "Num. Commercant"	;
static const char m_NumTrm_cmi_fra	    		[] = "Num. Terminal"	;
static const char m_ligneautopri_cmi_fra	    [] = "Ligne.Auto.Pri"	;
static const char m_lignetelesec_cmi_fra		[] = "Ligne.Tele.Sec"	;
static const char m_ligneautosec_cmi_fra		[] = "Ligne.Auto.Sec"	;
static const char m_abonntelepri_cmi_fra		[] = "Abonn.Tele.Pri"	;
static const char m_abonnautopri_cmi_fra		[] = "Abonn.Auto.Pri"	;
static const char m_abonntelesec_cmi_fra		[] = "Abonn.Tele.Sec"	;
static const char m_abonnautosec_cmi_fra		[] = "Abonn.Auto.Sec"	;
static const char m_delaipreautor_cmi_fra		[] = "Delai Pre-Autor"	;
static const char m_parametres_cmi_fra			[] = "Parametres"		;
static const char m_devise_cmi_fra				[] = "Devise"			;
static const char m_bin_cmi_fra					[] = "Bin"				;
static const char m_prefixes_cmi_fra		   	[] = "Prefixes"			;
static const char m_comptes_cmi_fra			   	[] = "Comptes"			;
static const char m_appli_naps_fra			   	[] = "NAPS"				;
static const char m_appli_emv_fra			   	[] = "EMV"				;
static const char m_changecompdom_fra		   	[] = "Chang. compte dom";
static const char m_comm_fra					[] = "Communication"	;

static const char m_ip_gprs_fra					[] = "Adresse IP"		;
static const char m_port_gprs_fra				[] = "Port"				;
static const char m_apn_gprs_fra				[] = "APN"				;
static const char m_login_gprs_fra				[] = "Utilisateur"		;
static const char m_pwd_gprs_fra				[] = "Mot de passe"		;

static const char m_transaction_fra				[] = "Transaction"		;
static const char m_prepaye_fra					[] = "Prepaye"			;
static const char m_cashadvance_fra				[] = "Cash Advance"		;
static const char m_service_naps_fra			[] = "Service NAPS"		;

static const char m_demandesolde_naps_fra		[] = "Demande Solde"	;
static const char m_minireleve_naps_fra			[] = "Mini Releve"		;
static const char m_pinchange_naps_fra			[] = "Pin Change"		;
static const char m_rechargegsm_naps_fra		[] = "Recharge Gsm"		;
static const char m_paiement_naps_fra			[] = "Paiement NAPS"	;

static const char m_paiement_nfois_naps_fra		[] = "Paiement N fois"	;
static const char m_paiement_differe_naps_fra	[] = "Paiement Differe"	;
static const char m_paiement_pourboire_naps_fra	[] = "Paiement Pourboire";
static const char m_paiement_surcout_naps_fra	[] = "Paiement Surcout"	;
static const char m_pourboire_cinq_pourcent_fra	[] = "1    5%"	;
static const char m_pourboire_dix_pourcent_fra	[] = "2    10%"	;
static const char m_pourboire_quinze_pourcent_fra	[] = "3    15%"	;
static const char m_titre_menu_pourboire_fra    [] ="Selectionner le pourboire :";
static const char m_choix_pourboire_fra			[] = "Autre montant"	;
static const char m_Desactiver_Pourboire_fra    [] = "Desactiver"	;
static const char m_Activer_Pourboire_fra       [] = "Activer"	;


/*  Messages en anglais */
static const char m_cmi_eng					    [] = "C.M.I"			;
static const char m_administration_cmi_eng      [] = "Administration"	;
static const char m_trasamanuel_cmi_eng         [] = "Transa.Manuel"	;
static const char m_annulation_cmi_eng          [] = "Annulation"		;
static const char m_sondageautori_cmi_eng       [] = "Sondage Autori"	;
static const char m_credit_cmi_eng				[] = "Credit"			;
static const char m_duplicata_cmi_eng			[] = "Duplicata"		;
static const char m_batch_cmi_eng				[] = "Batch"			;
static const char m_report_cmi_eng				[] = "Report"			;
static const char m_maintenance_cmi_eng			[] = "Maintenance"		;
static const char m_preauto_cmi_eng				[] = "Pre auto."		;
static const char m_preautocomp_cmi_eng			[] = "Pre auto. comp."	;
static const char m_annulpreauto_cmi_eng		[] = "AnnulPreAuto"		;
static const char m_dernieretrs_cmi_eng			[] = "Derniere Trs"		;
static const char m_autretrs_cmi_eng			[] = "Autre Trs"		;
static const char m_dernierepreaut_cmi_eng	   	[] = "Derniere Pre Aut"	;
static const char m_autrepreauto_cmi_eng	   	[] = "Autre Pre Auto"	;
static const char m_initparam_cmi_eng		    [] = "Init.Pram"		;
static const char m_editparam_cmi_eng		    [] = "Edit.Commercant"	;
static const char m_motpassecmr_cmi_eng		    [] = "Mot.Passe.Cmr"	;
static const char m_initialisation_cmi_eng	    [] = "Initialisation"	;
static const char m_findejournee_cmi_eng		[] = "Fin De Journee"	;
static const char m_listenoire_cmi_eng			[] = "Liste Noire"		;
static const char m_totaux_cmi_eng				[] = "Totaux"			;
static const char m_journal_cmi_eng				[] = "Journal"			;
static const char m_journalpreauto_cmi_eng		[] = "Journal Pre-Aut"	;
static const char m_razram_cmi_eng				[] = "Raz Ram"			;
static const char m_effacetransa_cmi_eng		[] = "Efface Transa"	;
static const char m_effacepreauto_cmi_eng		[] = "Efface Pre Auto"	;
static const char m_razlop_cmi_eng				[] = "Raz Lop"			;
static const char m_edittrsemv_cmi_eng			[] = "Edit Trs Emv"		;
static const char m_totale_cmi_eng			   	[] = "Totale"			;
static const char m_partielle_cmi_eng		   	[] = "Partielle"		;
static const char m_NumCmr_cmi_eng		    	[] = "Merchant Number"	;
static const char m_NumTrm_cmi_eng	    		[] = "Num. Terminal"	;
static const char m_ligneautopri_cmi_eng	    [] = "Ligne.Auto.Pri"	;
static const char m_lignetelesec_cmi_eng		[] = "Ligne.Tele.Sec"	;
static const char m_ligneautosec_cmi_eng		[] = "Ligne.Auto.Sec"	;
static const char m_abonntelepri_cmi_eng		[] = "Abonn.Tele.Pri"	;
static const char m_abonnautopri_cmi_eng		[] = "Abonn.Auto.Pri"	;
static const char m_abonntelesec_cmi_eng		[] = "Abonn.Tele.Sec"	;
static const char m_abonnautosec_cmi_eng		[] = "Abonn.Auto.Sec"	;
static const char m_delaipreautor_cmi_eng		[] = "Delai Pre-Autor"	;
static const char m_parametres_cmi_eng			[] = "Parametres"		;
static const char m_devise_cmi_eng				[] = "Devise"			;
static const char m_bin_cmi_eng					[] = "Bin"				;
static const char m_prefixes_cmi_eng		   	[] = "Prefixes"			;
static const char m_comptes_cmi_eng			   	[] = "Comptes"			;
static const char m_appli_naps_eng			   	[] = "NAPS"				;
static const char m_appli_emv_eng			   	[] = "EMV"				;
static const char m_changecompdom_eng		   	[] = "Chang. compte dom";
static const char m_comm_eng		   			[] = "Communication"	;

static const char m_ip_gprs_eng		   		    [] = "IP Address"		;
static const char m_port_gprs_eng		   		[] = "Port"				;
static const char m_apn_gprs_eng		   		[] = "APN"				;
static const char m_login_gprs_eng		   		[] = "Login"			;
static const char m_pwd_gprs_eng		   		[] = "Password"			;

static const char m_transaction_eng				[] = "Transaction"		;
static const char m_prepaye_eng					[] = "Prepaye"			;
static const char m_cashadvance_eng				[] = "Cash Advance"		;
static const char m_service_naps_eng			[] = "Service NAPS"		;

static const char m_demandesolde_naps_eng		[] = "Demande Solde"	;
static const char m_minireleve_naps_eng			[] = "Mini Releve"		;
static const char m_pinchange_naps_eng			[] = "Pin Change"		;
static const char m_rechargegsm_naps_eng		[] = "Recharge Gsm"		;
static const char m_paiement_naps_eng			[] = "Paiement NAPS"	;

static const char m_paiement_nfois_naps_eng		[] = "Paiement N fois"	;
static const char m_paiement_differe_naps_eng	[] = "Paiement Differe"	;
static const char m_paiement_pourboire_naps_eng	[] = "Paiement Pourboire";
static const char m_paiement_surcout_naps_eng	[] = "Paiement Surcout"	;
static const char m_pourboire_cinq_pourcent_eng	[] = "1    5%"	;
static const char m_pourboire_dix_pourcent_eng	[] = "2    10%"	;
static const char m_pourboire_quinze_pourcent_eng	[] = "3    15%"	;
static const char m_titre_menu_pourboire_eng    [] ="Selectionner le pourboire :";
static const char m_choix_pourboire_eng			[] = "Autre montant"	;
static const char m_Desactiver_Pourboire_eng    [] = "Desactiver"	;
static const char m_Activer_Pourboire_eng       [] = "Activer"	;
static const S_TAB_LANG table_message_cmi[]=
{
 {MESS1 , (char *)m_cmi_eng					,(char *)m_cmi_fra					},
 {MESS2 , (char *)m_administration_cmi_eng		,(char *)m_administration_cmi_fra	},
 {MESS3 , (char *)m_trasamanuel_cmi_eng			,(char *)m_trasamanuel_cmi_fra		},
 {MESS4 , (char *)m_annulation_cmi_eng			,(char *)m_annulation_cmi_fra		},
 {MESS5 , (char *)m_sondageautori_cmi_eng		,(char *)m_sondageautori_cmi_fra	},
 {MESS6 , (char *)m_credit_cmi_eng				,(char *)m_credit_cmi_fra			},
 {MESS7 , (char *)m_duplicata_cmi_eng			,(char *)m_duplicata_cmi_fra		},
 {MESS8 , (char *)m_batch_cmi_eng				,(char *)m_batch_cmi_fra			},
 {MESS9 , (char *)m_report_cmi_eng				,(char *)m_report_cmi_fra			},
 {MESS10, (char *)m_maintenance_cmi_eng			,(char *)m_maintenance_cmi_fra		},
 {MESS11, (char *)m_preauto_cmi_eng			    ,(char *)m_preauto_cmi_fra			},
 {MESS12, (char *)m_preautocomp_cmi_eng			,(char *)m_preautocomp_cmi_fra		},
 {MESS13, (char *)m_annulpreauto_cmi_eng	    ,(char *)m_annulpreauto_cmi_fra		},
 {MESS14, (char *)m_dernieretrs_cmi_eng			,(char *)m_dernieretrs_cmi_fra		},       
 {MESS15, (char *)m_autretrs_cmi_eng			,(char *)m_autretrs_cmi_fra			},          
 {MESS16, (char *)m_dernierepreaut_cmi_eng	    ,(char *)m_dernierepreaut_cmi_fra	},   
 {MESS17, (char *)m_autrepreauto_cmi_eng	    ,(char *)m_autrepreauto_cmi_fra		},      
 {MESS18, (char *)m_initparam_cmi_eng			,(char *)m_initparam_cmi_fra		},      
 {MESS19, (char *)m_editparam_cmi_eng			,(char *)m_editparam_cmi_fra		},      
 {MESS20, (char *)m_motpassecmr_cmi_eng			,(char *)m_motpassecmr_cmi_fra		},    
 {MESS21, (char *)m_initialisation_cmi_eng	    ,(char *)m_initialisation_cmi_fra	}, 
 {MESS22, (char *)m_findejournee_cmi_eng	    ,(char *)m_findejournee_cmi_fra		},   
 {MESS23, (char *)m_listenoire_cmi_eng			,(char *)m_listenoire_cmi_fra		},     
 {MESS24, (char *)m_totaux_cmi_eng			    ,(char *)m_totaux_cmi_fra			},         
 {MESS25, (char *)m_journal_cmi_eng				,(char *)m_journal_cmi_fra			},
 {MESS26, (char *)m_journalpreauto_cmi_eng	    ,(char *)m_journalpreauto_cmi_fra	},
 {MESS27, (char *)m_razram_cmi_eng				,(char *)m_razram_cmi_fra			},
 {MESS28, (char *)m_effacetransa_cmi_eng	    ,(char *)m_effacetransa_cmi_fra		},
 {MESS29, (char *)m_effacepreauto_cmi_eng	    ,(char *)m_effacepreauto_cmi_fra	},
 {MESS30, (char *)m_razlop_cmi_eng			    ,(char *)m_razlop_cmi_fra			},
 {MESS31, (char *)m_edittrsemv_cmi_eng			,(char *)m_edittrsemv_cmi_fra		},
 {MESS32, (char *)m_totale_cmi_eng				,(char *)m_totale_cmi_fra			},
 {MESS33, (char *)m_partielle_cmi_eng			,(char *)m_partielle_cmi_fra		},
 {MESS34, (char *)m_NumCmr_cmi_eng			,(char *)m_NumCmr_cmi_fra			},
 {MESS35, (char *)m_NumTrm_cmi_eng		,(char *)m_NumTrm_cmi_fra		},
 {MESS36, (char *)m_ligneautopri_cmi_eng		,(char *)m_ligneautopri_cmi_fra		},
 {MESS37, (char *)m_lignetelesec_cmi_eng		,(char *)m_lignetelesec_cmi_fra		},
 {MESS38, (char *)m_ligneautosec_cmi_eng		,(char *)m_ligneautosec_cmi_fra		},
 {MESS39, (char *)m_abonntelepri_cmi_eng		,(char *)m_abonntelepri_cmi_fra		},
 {MESS40, (char *)m_abonnautopri_cmi_eng		,(char *)m_abonnautopri_cmi_fra		},
 {MESS41, (char *)m_abonntelesec_cmi_eng		,(char *)m_abonntelesec_cmi_fra		},
 {MESS42, (char *)m_abonnautosec_cmi_eng		,(char *)m_abonnautosec_cmi_fra		},
 {MESS43, (char *)m_delaipreautor_cmi_eng		,(char *)m_delaipreautor_cmi_fra	},
 {MESS44, (char *)m_parametres_cmi_eng			,(char *)m_parametres_cmi_fra		},
 {MESS45, (char *)m_devise_cmi_eng				,(char *)m_devise_cmi_fra			},
 {MESS46, (char *)m_bin_cmi_eng					,(char *)m_bin_cmi_fra				},
 {MESS47, (char *)m_prefixes_cmi_eng			,(char *)m_prefixes_cmi_fra			},
 {MESS48, (char *)m_comptes_cmi_eng				,(char *)m_comptes_cmi_fra			},
 {MESS49, (char *)m_appli_naps_eng				,(char *)m_appli_naps_fra			},
 {MESS50, (char *)m_appli_emv_eng				,(char *)m_appli_emv_fra			},
 {MESS51, (char *)m_changecompdom_eng			,(char *)m_changecompdom_fra		},
 {MESS52, (char *)m_comm_eng					,(char *)m_comm_fra					},
 
 {MESS53, (char *)m_ip_gprs_eng					,(char *)m_ip_gprs_fra				},
 {MESS54, (char *)m_port_gprs_eng				,(char *)m_port_gprs_fra			},
 {MESS55, (char *)m_apn_gprs_eng				,(char *)m_apn_gprs_fra				},
 {MESS56, (char *)m_login_gprs_eng				,(char *)m_login_gprs_fra			},
 {MESS57, (char *)m_pwd_gprs_eng				,(char *)m_pwd_gprs_fra				},

 {MESS58, (char *)m_transaction_eng				,(char *)m_transaction_fra			},
 {MESS59, (char *)m_prepaye_eng					,(char *)m_prepaye_fra				},
 {MESS65, (char *)m_cashadvance_eng				,(char *)m_cashadvance_fra			},
 {MESS67, (char *)m_service_naps_eng			,(char *)m_service_naps_fra			},

 {MESS68, (char *)m_demandesolde_naps_eng		,(char *)m_demandesolde_naps_fra	},
 {MESS69, (char *)m_minireleve_naps_eng			,(char *)m_minireleve_naps_fra		},
 {MESS70, (char *)m_pinchange_naps_eng			,(char *)m_pinchange_naps_fra		},
 {MESS71, (char *)m_rechargegsm_naps_eng		,(char *)m_rechargegsm_naps_fra		},
 {MESS72, (char *)m_paiement_naps_eng			,(char *)m_paiement_naps_fra		},

 {MESS74, (char *)m_paiement_nfois_naps_eng		,(char *)m_paiement_nfois_naps_fra	},
 {MESS77, (char *)m_paiement_differe_naps_eng	,(char *)m_paiement_differe_naps_fra},
 {MESS78, (char *)m_paiement_pourboire_naps_eng	,(char *)m_paiement_pourboire_naps_fra},
 {MESS81, (char *)m_paiement_surcout_naps_eng	,(char *)m_paiement_surcout_naps_fra},
 {MESS82, (char *)m_pourboire_cinq_pourcent_eng	,(char *)m_pourboire_cinq_pourcent_fra},
 {MESS83, (char *)m_pourboire_dix_pourcent_eng	,(char *)m_pourboire_dix_pourcent_fra},
 {MESS84, (char *)m_pourboire_quinze_pourcent_eng	,(char *)m_pourboire_quinze_pourcent_fra},
 {MESS85, (char *)m_titre_menu_pourboire_eng	,(char *)m_titre_menu_pourboire_fra},
 {MESS87, (char *)m_choix_pourboire_eng	,(char *)m_choix_pourboire_fra},
 {MESS88, (char *)m_Desactiver_Pourboire_eng	,(char *)m_Desactiver_Pourboire_fra},
 {MESS89, (char *)m_Activer_Pourboire_eng	,(char *)m_Activer_Pourboire_fra},
 {0,0,0}
};
//Mouna310305_fin

extern void CUTERMclearDisplayT1(void); //KT1
extern void CUTERMdisplayLineT1(int xi_line, char *xs_text, T_CUTERMjustify xt_justify, char *mode); //KT1
void APEMV_UI_MessageDisplayDelay(char *message, int duration);

char message[40];

char *ReadMessage_cmi(int num,int langue) //KT1
{
	int i;

	if (langue==C_LG_ENGLISH || langue==C_LG_FRENCH)
	{
		for (i=0;table_message_cmi[i].english!=NULL;i++)
		{
			if (table_message_cmi[i].mess_nb == num)
			{
				if (langue==C_LG_FRENCH)
				{
					return((char *)table_message_cmi[i].french);//return((char *)ACCES(table_message_cmi[i].french));
				}
				if (langue==C_LG_ENGLISH)
				{
					return((char *)table_message_cmi[i].english);//return((char *)ACCES(table_message_cmi[i].english));
				}
			}
		}
		return((char *)no_mess_found);
	}
	else
	{
		sprintf(message,"Langue: %i - Mess: %i", langue, num);
		return(message);
	}
}


void CUMORE_CMI_AFFICHE_MESSAGE(int ligne1,int ligne2,int delay)
{
    unsigned char chaine [40];
	int merchLang;
	T_GL_HWIDGET hPicture=NULL;
	byte ucProduct;
	byte ucRange;
	// Return the product name
	ucRange = PSQ_Get_product_type(&ucProduct);


	if (ligne1 == 99 && ligne2 == 100) {
		if (ucRange == TYPE_TERMINAL_M3500 || ucRange == TYPE_TERMINAL_M5000)
			MessageWithoutConfirmation("PASSEZ BADGE\nCOMMERCANT", "file://flash/HOST/passer.bmp");
		else if (ucRange == TYPE_TERMINAL_D3500)
			MessageWithoutConfirmation("PASSEZ BADGE COMMERCANT", "file://flash/HOST/passer.bmp");
	} else if ((ligne1 == 94) && (ligne2 == 32)) {
//		hPicture = GoalCreatePicture(APEMV_UI_GoalHandle());                             // Create and clear screen
//		GoalDspPicture(hPicture, "Impression en cours", "file://flash/HOST/impression_encours.png", &cmn_xImage480, (GL_TIME_SECOND) * 0, true);
//		GoalDestroyPicture(&hPicture);

		MessageWithoutConfirmation("Impression... Patientez SVP","file://flash/HOST/impression_encours.png");
	}

	else if (ligne1 == 138 && ligne2 == 128) {

		MessageWithoutConfirmation("AUTOR. REFUSEE","file://flash/HOST/naps-tpe-icons-4.png");
		if(StructInfoPayment.Entry_Mode != TRS_MAG){
					MessageWithoutConfirmation("Retirer la carte SVP", "file://flash/HOST/retirer_la_carte_svp.png");//belhadeg 281019
							   }

	}
	else {

		/* Retreive Terminal merchant language  */
		merchLang = My_PSQ_Give_Language();

		if (merchLang >= C_NBLANGMERCH)
		{
			merchLang = 0;
		}
		if(FicPara.Langue == '2')
		{
			merchLang = 2;
		}
		sprintf ((char* ) chaine,"%s\n%s", (char *)(ct_DefMess_Cmi[ligne1][merchLang]),(char *)(ct_DefMess_Cmi[ligne2][merchLang]));

//		hPicture = GoalCreatePicture(APEMV_UI_GoalHandle());                             // Create and clear screen
//		GoalDspPicture(hPicture, chaine, NULL, &cmn_xImage480, (GL_TIME_SECOND) * 0, true);
//		GoalDestroyPicture(&hPicture);

		//APEMV_UI_MessageDisplayDelay ((char *)chaine, 0);
		//APEMV_UI_InfoDisplay((char *)chaine);
		MessageWithoutConfirmation((char *)chaine , NULL);
		
		CUTERMsleep (delay);
	}
}
void CUMORE_CMI_AFFICHE_MESSAGE_SD(int ligne1,int ligne2,int delay)
{
    unsigned char chaine [40];
	int merchLang;
	/* Retreive Terminal merchant language  */
	merchLang = My_PSQ_Give_Language();
	if (merchLang >= C_NBLANGMERCH)
	{
		merchLang = 0;
	}
	SetFont(_LARGE_);SetType(_NORMALE_);//font_size(7,13);
	CUTERMclearDisplayT1();
    strcpy ((char* ) chaine, (char *)(ct_DefMess_Cmi[ligne1][merchLang]));
    CUTERMdisplayLineT1(0, (char *)chaine, CENTER, (char *)"w");
    strcpy ((char* ) chaine, (char *)(ct_DefMess_Cmi[ligne2][merchLang]));
    CUTERMdisplayLineT1(1, (char *)chaine, CENTER, (char *)"a");
}
void	CUMORE_CMI_PHONE_NUMBER(PC pPhoneNumberScreen)
{
	int merchLang;
	/* Retreive Terminal merchant language  */
	merchLang = My_PSQ_Give_Language();
	if (merchLang >= C_NBLANGMERCH)
	{
		merchLang = 0;
	}

	strcpy((char *)&ct_DefMess_Cmi[42][merchLang],pPhoneNumberScreen);
}
void	CUMORE_CMI_CARD_NUMBER(PC pcardNumber,char etape)
{
	int merchLang;
	/* Retreive Terminal merchant language  */
	merchLang = My_PSQ_Give_Language();
	if (merchLang >= C_NBLANGMERCH)
	{
		merchLang = 0;
	}

	if(etape ==1)
	{
		memclr((char *)gcardNumber,0);
		strcpy((char *)gcardNumber,(char *)&ct_DefMess_Cmi[103][merchLang]);
		strcpy((char *)&ct_DefMess_Cmi[103][merchLang],(char *)pcardNumber);
	}
	else
		strcpy((char *)&ct_DefMess_Cmi[103][merchLang],(char *)gcardNumber);
}
/****************************************************************************/
uchar	ValiderMess(PUC Mess0,PUC Mess1)
{
	uchar	tch;
    unsigned char chaine [40];
	int merchLang;
	T_GL_HWIDGET hPicture=NULL;
	int retGL;

	sprintf ((char* ) chaine,"%s\n\n%s", (char *)Mess0,(char *)Mess1);
	//hPicture = GoalCreatePicture(APEMV_UI_GoalHandle());                             // Create and clear screen
//	GL_Widget_SetForeColor(Mess0, GL_COLOR_BLACK);
//	GL_Widget_SetForeColor(Mess1, GL_COLOR_BLACK);
//	GL_Widget_SetBackColor(hPicture, GL_COLOR_WHITE);
//	GL_Widget_SetForeColor(hPicture, GL_COLOR_BLACK);
	//retGL = GoalDspPicture(hPicture, chaine, NULL, &cmn_xImage480, (GL_TIME_SECOND) * 20, true);
	//GoalDestroyPicture(&hPicture);

	//APEMV_UI_InfoDisplay((char *)chaine);


//	uchar title[200];
//	sprintf(title, "%s\n%s", Mess0, Mess1);
	MessageWithConfirmation(chaine);


	//tch = KEY_CLEAR;
	tch = Os__xget_key(3600);

//	switch(retGL)
//	{
//	case GL_KEY_VALID:
//		tch = KEY_ENTER;
//		break;
//
//	case GL_KEY_CANCEL:
//		tch = KEY_CLEAR;
//		break;
//
//	default:
//		break;
//	}

	return(tch);
}

uchar	ValiderMess_(char title)
{
	uchar tch;
	int retGL;

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
	GL_Widget_SetMargins(title_widget, 0, 50, 0, 15, GL_UNIT_PIXEL);

	T_GL_HWIDGET layoutForPictures = GL_Layout_Create(mainLayout);
	GL_Widget_SetBackColor(window, 0xFFFFFFFF);
	GL_Widget_SetItem(layoutForPictures, 0, 1);
	GL_Widget_SetForeAlign(layoutForPictures, GL_ALIGN_BOTTOM);

	T_GL_HWIDGET leftPictureLayout = GL_Layout_Create(layoutForPictures);
	GL_Widget_SetBackColor(leftPictureLayout, 0xFFFFFFFF);
	GL_Widget_SetItem(leftPictureLayout, 0, 0);
	GL_Widget_SetForeAlign(leftPictureLayout, GL_ALIGN_LEFT);
	GL_Widget_SetGrow(leftPictureLayout, GL_DIRECTION_WIDTH);
	GL_Widget_SetExpand(leftPictureLayout, GL_DIRECTION_WIDTH);

	T_GL_HWIDGET rightPictureLayout = GL_Layout_Create(layoutForPictures);
	GL_Widget_SetBackColor(rightPictureLayout, 0xFFFFFFFF);
	GL_Widget_SetItem(rightPictureLayout, 1, 0);
	GL_Widget_SetForeAlign(rightPictureLayout, GL_ALIGN_RIGHT);
	GL_Widget_SetGrow(rightPictureLayout, GL_DIRECTION_WIDTH);
	GL_Widget_SetExpand(rightPictureLayout, GL_DIRECTION_WIDTH);

	// Confirm
	T_GL_HWIDGET confirm_widget = GL_Picture_Create(rightPictureLayout);
	GL_Widget_SetItem(confirm_widget, 0, 0);
	GL_Widget_SetSource(confirm_widget,
			"file://flash/HOST/naps-tpe-icons3.png");
	GL_Widget_SetMaxSize(confirm_widget, 50, 50, GL_UNIT_PIXEL);
	GL_Widget_SetTransformation(confirm_widget, GL_TRANSFORMATION_FIT_ALL);

	// Cancel
	T_GL_HWIDGET cancel_widget = GL_Picture_Create(leftPictureLayout);
	GL_Widget_SetItem(cancel_widget, 0, 0);
	GL_Widget_SetSource(cancel_widget,
			"file://flash/HOST/naps-tpe-icons-4.png");
	GL_Widget_SetMaxSize(cancel_widget, 50, 50, GL_UNIT_PIXEL);
	GL_Widget_SetTransformation(cancel_widget, GL_TRANSFORMATION_FIT_ALL);

	//GL_Window_Dispatch(window, 0);
	//tch = Os__xget_key(3600);
	tch = KEY_CLEAR;
		switch (retGL) {
		case GL_KEY_VALID:
			tch = KEY_ENTER;
			break;

		case GL_KEY_CANCEL:
			tch = KEY_CLEAR;
			break;

		default:
			break;
		}
	//GL_Widget_Destroy(window);

	//APEMV_UI_InfoDisplay((char *)chaine);

	//tch = Os__xget_key(3600);



	return (tch);
}





int MessageConfirmationMontant(void)
{
	char		   		Message_confmontant[112];
	byte ucProduct;
	byte ucRange;
	// Return the product name
	// ***********************
	ucRange = PSQ_Get_product_type(&ucProduct);    //modifié par lkeddouch 070319

	memset(Message_confmontant,0,sizeof(Message_confmontant));
	if(StructInfoPayment.CodeFunction == PAIEMENT_INTERCHANGE)
	{
		if(FicPara.MODELE_TERMINAL == VX520_TERMINAL)
		{
			sprintf(Message_confmontant,"\n\n   Confirmez vous\n   le paiement de :\n    %d,%d%d MAD  ?",
			(StructInfoPayment.Transaction_Amount+StructInfoPayment.MontantPourboireInterchange)/100,
			((StructInfoPayment.Transaction_Amount+StructInfoPayment.MontantPourboireInterchange)/10)%10,
			(StructInfoPayment.Transaction_Amount+StructInfoPayment.MontantPourboireInterchange)%10);
		}
		else if(FicPara.MODELE_TERMINAL == VX675_TERMINAL)
		{
			sprintf(Message_confmontant,"Confirmez vous le paiement \nde %d,%d%d MAD  ?",
			(StructInfoPayment.Transaction_Amount+StructInfoPayment.MontantPourboireInterchange)/100,
			((StructInfoPayment.Transaction_Amount+StructInfoPayment.MontantPourboireInterchange)/10)%10,
			(StructInfoPayment.Transaction_Amount+StructInfoPayment.MontantPourboireInterchange)%10);
		}

	}else if(	(StructInfoPayment.CodeFunction == SALECODE)
			||	(StructInfoPayment.CodeFunction == PAIEMENT_POURBOIRE)
			||	(StructInfoPayment.CodeFunction == PAIEMENT_NFOIS)
			||	(StructInfoPayment.CodeFunction == PAIEMENT_DIFFERE)
			||	(StructInfoPayment.CodeFunction == CASHADVANCESCODE)
		)
	{

		if(StructInfoPayment.CodeFunction == SALECODE)
		{
			if (FicPara.MenuTPE[26] == '1' && FicPara.var_pourboire == '1') {
				if (Ava_pourboire()) // commenter par jrizki 21/01/2019
				{
					StructInfoPayment.CodeFunction = PAIEMENT_POURBOIRE;
				}
				else
					StructInfoPayment.MontantPourboireInterchange = 0;
			}
		}
		if(StructInfoPayment.CodeFunction != PAIEMENT_POURBOIRE)
		{

			if (FicPara.MenuTPE[26] == '1' && FicPara.var_pourboire == '1') {
				if (ucRange == TYPE_TERMINAL_M3500 || ucRange == TYPE_TERMINAL_M5000)                            //modifié par lkeddouch 070319
				sprintf(Message_confmontant,"Confirmez vous\n   le paiement de :\n    %d,%d%d MAD  ?",
				StructInfoPayment.Transaction_Amount/100,
				(StructInfoPayment.Transaction_Amount/10)%10,
				StructInfoPayment.Transaction_Amount%10);
			else if(ucRange == TYPE_TERMINAL_D3500)                       //modifié par lkeddouch 070319
				sprintf(Message_confmontant,"Confirmez vous le paiement \nde %d,%d%d MAD  ?",
				StructInfoPayment.Transaction_Amount/100,
				(StructInfoPayment.Transaction_Amount/10)%10,
				StructInfoPayment.Transaction_Amount%10);
			}
		}
	}else if(	(StructInfoPayment.CodeFunction == RECH_PREPAYE)
		)
	{
		if(FicPara.MODELE_TERMINAL == VX520_TERMINAL)
			sprintf(Message_confmontant,"\n\n   Confirmez vous\n le montant recharge\nde : %d,%d%d MAD  ?",
			StructInfoPayment.Transaction_Amount/100,
			(StructInfoPayment.Transaction_Amount/10)%10,
			StructInfoPayment.Transaction_Amount%10);
		else if(FicPara.MODELE_TERMINAL == VX675_TERMINAL)
			sprintf(Message_confmontant,"Confirmez vous le montant recharge \nde %d,%d%d MAD  ?",
			StructInfoPayment.Transaction_Amount/100,
			(StructInfoPayment.Transaction_Amount/10)%10,
			StructInfoPayment.Transaction_Amount%10);

	}else if(	(StructInfoPayment.CodeFunction == DECH_PREPAYE)
		)
	{
		if(FicPara.MODELE_TERMINAL == VX520_TERMINAL)
			sprintf(Message_confmontant,"\n\n   Confirmez vous\n le montant decharge\nde : %d,%d%d MAD  ?",
			StructInfoPayment.Transaction_Amount/100,
			(StructInfoPayment.Transaction_Amount/10)%10,
			StructInfoPayment.Transaction_Amount%10);
		else if(FicPara.MODELE_TERMINAL == VX675_TERMINAL)
			sprintf(Message_confmontant,"Confirmez vous le montant decharge \nde %d,%d%d MAD  ?",
			StructInfoPayment.Transaction_Amount/100,
			(StructInfoPayment.Transaction_Amount/10)%10,
			StructInfoPayment.Transaction_Amount%10);

	}else if(	(StructInfoPayment.CodeFunction == CAC_PREPAYE)
			||	(StructInfoPayment.CodeFunction == CASHIN_PREPAYE)
			||	(StructInfoPayment.CodeFunction == CASHOUT_PREPAYE)
		)
	{
		if(FicPara.MODELE_TERMINAL == VX520_TERMINAL)
			sprintf(Message_confmontant,"\n\n   Confirmez vous\nle montant transfert\nde : %d,%d%d MAD  ?",
			StructInfoPayment.Transaction_Amount/100,
			(StructInfoPayment.Transaction_Amount/10)%10,
			StructInfoPayment.Transaction_Amount%10);
		else if(FicPara.MODELE_TERMINAL == VX675_TERMINAL)
			sprintf(Message_confmontant,"Confirmez vous le montant transfert \nde %d,%d%d MAD  ?",
			StructInfoPayment.Transaction_Amount/100,
			(StructInfoPayment.Transaction_Amount/10)%10,
			StructInfoPayment.Transaction_Amount%10);

	}else if(	(StructInfoPayment.CodeFunction == CASHADVANCESCODE)
		)
	{
		if(FicPara.MODELE_TERMINAL == VX520_TERMINAL)
			sprintf(Message_confmontant,"\n\n   Confirmez vous\n   l'avance cash de :\n    %d,%d%d MAD  ?",
			StructInfoPayment.Transaction_Amount/100,
			(StructInfoPayment.Transaction_Amount/10)%10,
			StructInfoPayment.Transaction_Amount%10);
		else if(FicPara.MODELE_TERMINAL == VX675_TERMINAL)
			sprintf(Message_confmontant,"Confirmez vous l'avance cash \nde %d,%d%d MAD  ?",
			StructInfoPayment.Transaction_Amount/100,
			(StructInfoPayment.Transaction_Amount/10)%10,
			StructInfoPayment.Transaction_Amount%10);

	}

//	if(MessageWithConfirmation((char *) Message_confmontant)!=KEY_ENTER)
//			return KO;

	if (strlen(Message_confmontant) > 0) {
//		if(ValiderMess(Message_confmontant,"")!=KEY_ENTER)
//			return KO;
		MessageWithConfirmation((char *) Message_confmontant);

		//APEMV_UI_InfoDisplay((char *)Message_confmontant);

		while (1) {
			if (Os__xget_key(FicPara.TimeOut * 6) != KEY_CLEAR) {
				//Os__xprinta("11111");
				return OK ;
			}
			//Os__xprinta("");
			return KO ;
		}
		return KO ;
	}
	return OK ;
}


static char* ProductName(void)   //ajouter par lkeddouch le 07/03/2019
{
	// Local variables
    // ***************
	byte ucProduct;
	byte ucRange;
	char tcName[8];
	// Return the product name
	// ***********************
	ucRange=PSQ_Get_product_type(&ucProduct);
	switch(ucProduct)
	{
	case TYPE_TERMINAL_SMART :      strcpy(tcName, "EFTSMART"); break;  // 2 EFTsmart
	case TYPE_TERMINAL_EFT30F:      strcpy(tcName, "EFT30F");   break;  // 3 EFT30F
	case TYPE_TERMINAL_EFT30P:      strcpy(tcName, "EFT30P");   break;  // 4 EFT30plus
	case TYPE_TERMINAL_TWIN30:                                          // 5
		switch(ucRange)
		{
		case TYPE_TERMINAL_TWIN30:  strcpy(tcName, "TWIN30");   break;  // 55 TWIN30
		case TYPE_TERMINAL_TWIN33:  strcpy(tcName, "TWIN33");   break;  // 56 TWIN33
		case TYPE_TERMINAL_TWIN32:  strcpy(tcName, "TWIN32");   break;  // 57 TWIN32
		default:                    strcpy(tcName, "UNKNOWN");  break;
		}
		break;
	case TYPE_TERMINAL_EFT930:                                          // 6
		switch(ucRange)
		{
		case TYPE_TERMINAL_EFT930G: strcpy(tcName, "EFT930G");  break;  // 67 EFT930G
		case TYPE_TERMINAL_EFT930B: strcpy(tcName, "EFT930B");  break;  // 68 EFT930B
		case TYPE_TERMINAL_EFT930F: strcpy(tcName, "EFT930F");  break;  // 69 EFT930F
		case TYPE_TERMINAL_EFT930W:	strcpy(tcName, "EFT930W");  break;  // 65 EFT930W
		case TYPE_TERMINAL_EFT930P: strcpy(tcName, "EFT930P");  break;  // 66 EFT930P
		default:                    strcpy(tcName, "UNKNOWN");  break;
		}
		break;
	case TYPE_TERMINAL_ML30:                                            // 1
		switch(ucRange)
		{
		case TYPE_TERMINAL_ML30:    strcpy(tcName, "ML30");     break;  // 11 ML30
		case TYPE_TERMINAL_SPM:     strcpy(tcName, "SPM");      break;  // 12 SPM
		case TYPE_TERMINAL_IPP320:  strcpy(tcName, "IPP320");   break;  // 13 IPP320
		case TYPE_TERMINAL_IPP350:  strcpy(tcName, "IPP350");   break;  // 14 IPP350
		case TYPE_TERMINAL_IPP480:  strcpy(tcName, "IPP480");   break;  // 16 IPP480
		default:                    strcpy(tcName, "UNKNOWN");  break;
		}
		break;
	case TYPE_TERMINAL_CAD30:       strcpy(tcName, "CAD30");    break;  // 7 CAD30
	case TYPE_TERMINAL_MR40:        strcpy(tcName, "MR40");     break;  // 9 MR40
	case TYPE_TERMINAL_X07:                                             // 8
		switch(ucRange)
		{
		case TYPE_TERMINAL_IWL220:  strcpy(tcName, "IWL220");   break;  // 83 IWL220
		case TYPE_TERMINAL_IWL250:  strcpy(tcName, "IWL250");   break;  // 82 IWL250
		case TYPE_TERMINAL_ICT220:  strcpy(tcName, "ICT220");   break;  // 88 ICT220
		case TYPE_TERMINAL_ICT250:  strcpy(tcName, "ICT250");   break;  // 87 ICT250
		case TYPE_TERMINAL_ICT280:  strcpy(tcName, "ICT280");   break;  // 86 ICT280
		case TYPE_TERMINAL_ISC350:  strcpy(tcName, "ISC350");   break;  // 85 ISC350
		case TYPE_TERMINAL_ISC250:  strcpy(tcName, "ISC250");   break;  // 84 ISC250
		default:                    strcpy(tcName, "UNKNOWN");  break;
		}
		break;
#ifdef __TELIUM3__
	case TYPE_TERMINAL_DESK:
		switch(ucRange)
		{
		case TYPE_TERMINAL_D5000: strcpy(tcName, "DESK5000"); break;
		case TYPE_TERMINAL_D3500: strcpy(tcName, "DESK3500"); break;
		case TYPE_TERMINAL_D3200: strcpy(tcName, "DESK3200"); break;
		default:                  strcpy(tcName, "UNKNOWN");  break;
		}
		break;
	case TYPE_TERMINAL_MOVE:
		switch(ucRange)
		{
		case TYPE_TERMINAL_M5000: strcpy(tcName, "MOVE5000"); break;
		case TYPE_TERMINAL_M3500: strcpy(tcName, "MOVE3500"); break;
		case TYPE_TERMINAL_M2500: strcpy(tcName, "MOVE2500"); break;
		default:                  strcpy(tcName, "UNKNOWN");  break;
		}
		break;
	case TYPE_TERMINAL_LANE:
		switch(ucRange)
		{
		case TYPE_TERMINAL_L5000: strcpy(tcName, "LANE5000"); break;
		default:                  strcpy(tcName, "UNKNOWN");  break;
		}
		break;
#endif
	default:                        strcpy(tcName, "UNKNOWN");  break;
	}

	return tcName;
}


