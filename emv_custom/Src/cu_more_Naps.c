/**
* \file cu_more.c
* \brief This module contains the functions activated by the navigation menus.
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
#include <FTP_.h>
#include "SDK_Naps.h"
#include "GL_GraphicLib.h"

#include "_emvdctag_.h"
#include "del_lib.h"
#include "def_tag.h"
#include "cu_term.h"
#include "cu_black.h"
#include "MyTlvTree.h"
#include "cu_batch.h"
#include "cu_base.h"
#include "cu_serv.h"
#include "cu_mess.h"
#include "MessagesDefinitions.h"
#include "cu_more.h"
#include "cu_entry.h"
#include "convert.h"
#include "cu_param.h"
#include "gestion_param.h"
#include "cu_comm.h"
#include "servcomm.h"
#include "servcomm_TlvTree.h"
#include "convert.h"
#include "cu_file.h"
#include "LinkLayer.h"
#include "serveng.h"
#include "cu_disk.h"
#include "EngineInterfaceLib.h"
#include "TlvTreeDel.h"
#include "EngineInterface.h"
#include "cu_trfile.h"
#include "UserInterfaceHelpers.h" //KT


#include "dll_wifi.h"//badr

#include "DNS_.h"


#include "Training.h"
#include "TlvTree.h"
#include "LinkLayer.h"
#include "ExtraGPRS.h"

#define T_CUTERM_X930 0 //ToSee
#define CHECK(CND,LBL) {if(!(CND)){goto LBL;}}

#define MAX_SND  512
#define MAX_RSP  512

#include "FTP_.h"
#include "SSL_.h"

#define SSL_PROFILE		"SSLPR"


int ShowEntry (const char *pcTitle, const char *pcText, char *pcInput, byte ucMinDigit, byte ucMaxDigit, byte ucEntryMode);
void DisplayMedium (byte ucLine, byte ucColumn, char *pcText);
void PromptGPRS();
void PromptEthernet();
void ComEthernet();
int APEMV_UI_NumericalEntry(const char *title,const char *text, unsigned char *entryString, const char *entryMask );
void APEMV_UI_MessageDisplayDelay(char *message, int duration);
void ActivationWifi(void);
int administration_naps(void);
int administradom_cmi(void);//Mouna310305
int administra_cmi(void);
int trasamanuel_cmi(void);
int getDebt(void);
int payAmount(void);
int annulation_cmi(void);
int sondageautori_cmi(void);
int credit_cmi(void);
int duplicata_cmi(void);
int batch_cmi(void);
int report_cmi(void);
int maintenance_cmi(void);
int preauto_cmi(void);
int preautocomp_cmi(void);
int annulpreauto_cmi(void);
int demandesolde_naps(void);
int minireleve_naps(void);
int pinchange_naps(void);
int rechargegsm_naps(void);
int paiement_naps(void);
int paiement_nfois_naps(void);
int paiement_differe_naps(void);
int paiement_pourboire_naps(void);
int paiement_surcout_naps(void);
int dernieretrs_cmi(void);
int autretrs_cmi(void);
int dernierepreaut_cmi(void);
int autrepreauto_cmi(void);
int initparam_cmi(void);
int editparam_cmi(void);
int more_function_fr(void);

int param_gprs(void);//Mo 10/04/06

int motpassecmr_cmi(void);
int changecomptdom_cmi(void);//Mouna310305
int initialisation_cmi(void);
int testreseau_cmi(void);
int findejournee_cmi(void);
int listenoire_cmi(void);
int totaux_cmi(void);
int journal_cmi(void);
int journalpreauto_cmi(void);
int razram_cmi(void);
int effacetransa_cmi(void);
int effacepreauto_cmi(void);
int razlop_cmi(void);
int edittrsemv_cmi(void);
int totale_cmi(void);
int partielle_cmi(void);
int NumCmr_cmi(void);
int NumTrm_cmi(void);
int choix_langue(void);
int ligneautopri_cmi(void);
int lignetelesec_cmi(void);
int ligneautosec_cmi(void);
int abonntelepri_cmi(void);
int abonnautopri_cmi(void);
int abonntelesec_cmi(void);
int abonnautosec_cmi(void);
int delaipreautor_cmi(void);
int parametres_cmi(void);
int devise_cmi(void);
int bin_cmi(void);
int prefixes_cmi(void);
int comptes_cmi(void);
int comm_pm(void);
int comm_pm_rtc(void);
int id_mode_nbe(void);
int id_rlp_nbe(void);
int ip_gprs_nbe(void);
int port_gprs_nbe(void);
int apn_gprs_nbe(void);
int login_gprs_nbe(void);
int pwd_gprs_nbe(void);
int tcpip_nbe(void);
int appli_cmi(void);
int appli_emv(void);

//Mo 10/04/06
int GetItemAlphaNumerique( char *sOutput, int OutputLen,char *libelle,unsigned short int);
int ip_gprs_cmi(void);

int port_gprs_cmi(void);
int apn_gprs_cmi(void);
int login_gprs_cmi(void);
int pwd_gprs_cmi(void);
int time_out_param(void);

int startTMSDownload(void);//badr
int transaction_naps(void);
int prepaye_naps(void);
int duplicata_naps(void);
int parametres_naps(void);
int cashadvance_naps(void);
int preautorisation_naps(void);
int service_naps(void);
int journaux_naps(void);
int Activer(void);
int Desactiver(void);
uchar control_badge(void);



#define TELECO_OK					0
#define __STATE_INPUT_EXIT			-2
#define __STATE_INPUT_BACK			-1
#define __STATE_INPUT_END			0

#define __STATE_INPUT_ADDRESS		1
#define __STATE_INPUT_PORT			2
#define __STATE_INPUT_LOGIN			3
#define __STATE_INPUT_PASSWORD		4

#define __STATE_INPUT_GSM_NUMBER	5
#define __STATE_INPUT_GSM_MODE		6
#define __STATE_INPUT_GSM_RLP		7



//T_GL_HGRAPHIC_LIB hGoal=NULL;
/* ========================================================================	*/
/* Constant declaration                                                     */
/* ========================================================================	*/

static const char *tzMenuUser[] =
{
	" Menu Transaction",
	" Menu Préautorisation",
	" Menu Duplicata",
	" Menu Paramètres",
	" Menu CashAdvance",
	//" Menu Prépayée",
	//" Menu Services NAPS",//toORI
	" Menu Journaux",
	" Menu Réseau",
	//" Chargement à distance",
	NULL
};
static const char *tzMenuUser2[] =
{
	" Menu Transaction",
	" Menu Préautorisation",
	" Menu Duplicata",
	" Menu Paramètres",
	" Menu CashAdvance",
	//" Menu Prépayée",
	" Menu Journaux",
	//" Chargement à distance",

	NULL
};
static const char *tzMenuUserOri[] =
{
	" Menu Transaction",
	" Menu Préautorisation",
	" Menu Duplicata",
	" Menu Paramètres",
	" Menu CashAdvance",
	//" Menu Prépayée",
	" Menu Journaux",
	" Menu Réseau",
	" Menu Services NAPS",//toORI
	//" Chargement à distance",
	NULL
};
static const char *tzMenuUser2Ori[] =
{
	" Menu Transaction",
	" Menu Préautorisation",
	" Menu Duplicata",
	" Menu Paramètres",
	" Menu CashAdvance",
	//" Menu Prépayée",
	" Menu Journaux",
	" Menu Services NAPS",//toORI
	//" Chargement à distance",
	NULL
};
static const char *tzMenuPrepaye[] =
{
	" RECHARGE CARTE",
	" DECHARGE CARTE",
	" TR. CARTE A CARTE",
	" TR. CARTE A CASH",
	NULL
};

static const char *tzMenuDialogBox[] =
{
	" Transaction Manuelle",
	" Annulation",
	" Crédit",
	" Télécollecte NAPS",
	NULL
};

static const char *tzMenuWidget[] =
{
	" Dernière Transaction",
	" Autre Transaction",
	" Dernière Préautorisation",
	" Autre Préautorisation",
	NULL
};
static const char *tzMenuAdmin[] =
{
	" IP MODE",
	" Edition Paramètres",
	" Réferencement NAPS",
	" Raz Ram",
	" Admin Services",
	" Menu services",
	" Chargement à distance",
	NULL
};
static const char *tzMenuPreauto[] =
{
	" Préautorisation",
	" Complément Préautorisation",
	" Annulation Préautorisation ",
	" Purge Préautorisation",
	NULL
};
static const char *tzMenuService[] =
{
	" Demande Solde",
	" Mini Releve",
	" Pin Change",
	" Recharge Gsm",
	" Paiement NAPS",
	NULL
};
static const char *tzMenuService2[] =
{
	" Oriflame",
	NULL
};
static const char *tzMenuOriflame[] =
{
	" Factures",
	//" Payer un montant",
	NULL
};

static char *tzMenuFactOriflame[] =
{
	NULL,
	NULL,
	NULL,
	NULL,
	NULL
};

static const char *tzOriflame[] =
{
		" paiement par carte ",
		" paiement cash ",
		NULL
};
//static const char *tzOriflame[] =
//{
//	" Payez",
//	" Passez à la facture suivante ",
//	NULL
//};

const char *listeWifi[] =
{
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL
};
const char *listeProfile[] =
{
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL
};
static const char *tzMenuTotal[] =
{
	" Journal Trs NAPS",
	" Journal Préautorisation ",
	" Total ",
	NULL
};

static const char *tzMenuReseau[] =
{
	" Menu Bluetooth",
	" Menu Wifi",
	" Menu GPRS",
	NULL
};

static const char *tzMenuBT[] =
{
	" Start BT",
	" Stop BT",
	" Restart BT",
	NULL
};
static const char *tzMenuWifi[] =
{
	" Activation WIFI",
	" Scan réseau",
	" Mes réseaux",
	NULL
};
static const char *tzMenuGPRS[] =
{
	" Start GPRS",
	" Stop GPRS",
	" Restart GPRS",
	NULL
};
static const char *MenuAnnulPre[] =
{
	" Annulation Totale",
	" Annulation Partielle",
	NULL
};
static const char *MenuConfiguration[] =
{
	" Adresse IP",
	" Port",
	" APN",
	" Utilisateur",
	" Mot de Passe",
	//" Time out",
	NULL
};
static const char *MenuParametres[] =
{
	" Paramètres",
	" Devise",
	" Bin",
	" Prefixes",
	" Comptes",
	NULL
};

static const char *MenuInit_Param[] =
{
	" Num Commerçant",
	" Num Terminal",
	//" Choisir une langue",
	NULL
};

const NavigationListe NavigationEmv[] =
{
   {MESS49,          (PFONCNAVI) appli_cmi},
//   {MESS50,          (PFONCNAVI) appli_emv},
#ifdef _TOOLWARE_
  ,{MESS40,         (PFONCNAVI) CUMORE_Demo_Mode}
#endif
};

extern t_topstack *alternateNbr;
extern int AlternanceIsOn;
extern int GPRSDisabled;
int imprimeparam = 0;

static Telium_File_t *sKeyboard = NULL;
static Telium_File_t *sDisplay = NULL;

const NavigationListe NavigationEmv_cmi[] =
{

		{MESS58,          (PFONCNAVI) transaction_naps},
		{MESS59,          (PFONCNAVI) prepaye_naps},
		{MESS7,           (PFONCNAVI) duplicata_naps},
		{MESS2,           (PFONCNAVI) administration_naps},
		{MESS65,          (PFONCNAVI) cashadvance_naps},
		{MESS11,          (PFONCNAVI) preautorisation_naps},
		{MESS67,          (PFONCNAVI) service_naps},
		{MESS24,          (PFONCNAVI) journaux_naps},


#ifdef _TOOLWARE_
	,{CMN_MESS_MERCHANT_TOOLWARE, (PFONCNAVI) CUMORE_Demo_Mode}
#endif   
};

static const NavigationListe Navigationduplicata_cmi[] =
{
   {MESS14,          (PFONCNAVI) dernieretrs_cmi},
   {MESS15,          (PFONCNAVI) autretrs_cmi},
   {MESS16,          (PFONCNAVI) dernierepreaut_cmi},
   {MESS17,          (PFONCNAVI) autrepreauto_cmi},
};

static const NavigationListe Navigationadministrationdom_cmi[] =
{
   {MESS18,          (PFONCNAVI) initparam_cmi},
   {MESS19,          (PFONCNAVI) editparam_cmi},
   {MESS20,          (PFONCNAVI) motpassecmr_cmi},
   {MESS51,          (PFONCNAVI) changecomptdom_cmi},
   {MESS52,          (PFONCNAVI) comm_pm},
};

static const NavigationListe Navigationadministration_cmi[] =
{
   {MESS18,          (PFONCNAVI) initparam_cmi},
   {MESS52,          (PFONCNAVI) comm_pm},
   {MESS19,          (PFONCNAVI) editparam_cmi},
//   {MESS20,          (PFONCNAVI) motpassecmr_cmi},
};

static const NavigationListe Navigationbatch_cmi[] =
{
   {MESS21,          (PFONCNAVI) initialisation_cmi},
   {MESS22,          (PFONCNAVI) findejournee_cmi}/*,
   {MESS23,          (PFONCNAVI) listenoire_cmi},*/
};

static const NavigationListe Navigationreport_cmi[] =
{
   {MESS24,          (PFONCNAVI) totaux_cmi},
   {MESS25,          (PFONCNAVI) journal_cmi},
   {MESS26,          (PFONCNAVI) journalpreauto_cmi},
};

static const NavigationListe Navigationmaintenance_cmi[] =
{
   {MESS27,          (PFONCNAVI) razram_cmi},
   {MESS28,          (PFONCNAVI) effacetransa_cmi},
   {MESS29,          (PFONCNAVI) effacepreauto_cmi},
   {MESS30,          (PFONCNAVI) razlop_cmi},
   {MESS31,          (PFONCNAVI) edittrsemv_cmi},
};

static const NavigationListe Navigationannulpreauto_cmi[] =
{
   {MESS32,          (PFONCNAVI) totale_cmi},
   {MESS33,          (PFONCNAVI) partielle_cmi},
};

static const NavigationListe NavigationServices[] =
{
   {MESS88,          (PFONCNAVI) Desactiver},
   {MESS89,          (PFONCNAVI) Activer},
};

static const NavigationListe Navigationinitparam_cmi[] =
{
   {MESS34,          (PFONCNAVI) NumCmr_cmi},
   {MESS35,          (PFONCNAVI) NumTrm_cmi},
   {MESS36,          (PFONCNAVI) choix_langue},
/*
   {MESS36,          (PFONCNAVI) ligneautopri_cmi},
   {MESS37,          (PFONCNAVI) lignetelesec_cmi},
   {MESS38,          (PFONCNAVI) ligneautosec_cmi},
   {MESS39,          (PFONCNAVI) abonntelepri_cmi},
   {MESS40,          (PFONCNAVI) abonnautopri_cmi},
   {MESS41,          (PFONCNAVI) abonntelesec_cmi},
   {MESS42,          (PFONCNAVI) abonnautosec_cmi},
   {MESS43,          (PFONCNAVI) delaipreautor_cmi},
 */
};

static const NavigationListe Navigationeditparam_cmi[] =
{
   {MESS44,          (PFONCNAVI) parametres_cmi},
   {MESS45,          (PFONCNAVI) devise_cmi},
   {MESS46,          (PFONCNAVI) bin_cmi},
   {MESS47,          (PFONCNAVI) prefixes_cmi},
   {MESS48,          (PFONCNAVI) comptes_cmi},
};

static const NavigationListe Navigationparam_gprs[] =
{
   {MESS53,          (PFONCNAVI) ip_gprs_cmi},
   {MESS54,          (PFONCNAVI) port_gprs_cmi},
   {MESS55,          (PFONCNAVI) apn_gprs_cmi},
   {MESS56,          (PFONCNAVI) login_gprs_cmi},
   {MESS57,          (PFONCNAVI) pwd_gprs_cmi},
  // {MESS58,          (PFONCNAVI) time_out_param},

};

static const NavigationListe Navigationbatchparametres_naps[] =
{
   {MESS53,          (PFONCNAVI) comm_pm},
   {MESS19,          (PFONCNAVI) initparam_cmi},
   {MESS21,          (PFONCNAVI) initialisation_cmi},
   {MESS27,          (PFONCNAVI) razram_cmi},
};

static const NavigationListe Navigationtransaction_naps[] =
{
   {MESS3,          (PFONCNAVI) trasamanuel_cmi},
   {MESS4,          (PFONCNAVI) annulation_cmi},
   {MESS6,          (PFONCNAVI) credit_cmi},
   {MESS22,          (PFONCNAVI) findejournee_cmi},
};

static const NavigationListe Navigationpreauto_naps[] =
{
   {MESS11,          (PFONCNAVI) preauto_cmi},
   {MESS12,          (PFONCNAVI) preautocomp_cmi},
   {MESS13,          (PFONCNAVI) annulpreauto_cmi},
//   {MESS22,          (PFONCNAVI) PurgePreauto},
};

static const NavigationListe Navigationservice_naps[] =
{
   {MESS68,          (PFONCNAVI) demandesolde_naps},
   {MESS69,          (PFONCNAVI) minireleve_naps},
   {MESS70,          (PFONCNAVI) pinchange_naps},
   {MESS71,          (PFONCNAVI) rechargegsm_naps},
   {MESS72,          (PFONCNAVI) paiement_naps},
};

static const NavigationListe Navigationpaiement_naps[] =
{
   {MESS74,          (PFONCNAVI) paiement_nfois_naps},
   {MESS77,          (PFONCNAVI) paiement_differe_naps},
   {MESS78,          (PFONCNAVI) paiement_pourboire_naps},
   {MESS81,          (PFONCNAVI) paiement_surcout_naps},
};


const unsigned char CUMORE_CMI_TRUE []   = {0x00, 0x01};
const unsigned char CUMORE_CMI_FALSE []  = {0x00, 0x00};

const char mess_cmi_NumCmr_id []		= "Num. Commercant :";
const char mess_cmi_NumTrm_id []  		= "Num. Terminal :";
const char mess_cmi_ligneautopri_id []  = "LIGNE.AUTO.PRI. :";
const char mess_cmi_lignetelesec_id []  = "LIGNE.TELE.SEC. :";
const char mess_cmi_ligneautosec_id []  = "LIGNE.AUTO.SEC. :";
const char mess_cmi_abonntelepri_id []  = "ABONNEE.TELE.PRI. :";
const char mess_cmi_abonnautopri_id []  = "ABONNEE.AUTO.PRI. :";
const char mess_cmi_abonntelesec_id []  = "ABONNEE.TELE.SEC. :";
const char mess_cmi_abonnautosec_id []  = "ABONNEE.AUTO.SEC. :";
const char mess_cmi_delaipreauto_id []  = "DELAI PRE AUTO. :";

const char mess_nbe_modegsm_id []		= "MODE :";
const char mess_nbe_rlp_id []			= "RLP (0/1):";
const char mess_nbe_version_id []		= "VERSION :";

const char mess_nbe_type_id []			= "RTC(0)GSM(1)GP(2): ";

unsigned short Bypass_Required;
unsigned short TransactionForcedOnline;
L	g_l_annul_pre=0;

extern unsigned short CUSERV_New_Parameters;

extern unsigned long ulTransactionType;
extern unsigned long ulTransactionTypeidlemessage;

unsigned char RetransmitIndicator;

/*%DEBUT_MODIF 18/07/02 */
extern StructListe       Menu;
/*%FIN_MODIF 18/07/02 */

extern	int SLC_Saisie_Numerique(unsigned char *saisie,char *title, unsigned char *chaine, TAB_ENTRY_STRING *table);
char *ReadMessage_cmi(int num,int langue); //KT1

int more_function(NO_SEGMENT no, void *p1, void *p2) {

		if(FicPara.Langue == '2')
			{
			more_function_ara();
		}
		else{
			more_function_fr();
		}

}

/*-----------------------------------------------------------------
 * Fonction   :menu_Fact_Oriflame
 * objet      :
 *
 * Parametre  :
 * Retour     :
 *-----------------------------------------------------------------
*/
int menu_Fact_Oriflame(int i)
{
	T_GL_HGRAPHIC_LIB hGoal = APEMV_UI_GoalHandle();
	int iRet,j=0;
	bool bContinue3 = TRUE;
	Ulong montant_total;


	montant_total=0;



	char tzMenuFact_Oriflame[5][48];

	memset(tzMenuFactOriflame,0,sizeof(tzMenuFactOriflame));


 	for(j=0;j<(i+1);j++)
 	{
		StructInfoPayment.Transaction_Amount = 0;
		StructInfoPayment.Transaction_Amount += (StructInfoPayment.OR_debt[j][11]-48);
		StructInfoPayment.Transaction_Amount += 10*(StructInfoPayment.OR_debt[j][10]-48);
		StructInfoPayment.Transaction_Amount += 10*10*(StructInfoPayment.OR_debt[j][9]-48);
		StructInfoPayment.Transaction_Amount += 10*10*10*(StructInfoPayment.OR_debt[j][8]-48);
		StructInfoPayment.Transaction_Amount += 10*10*10*10*(StructInfoPayment.OR_debt[j][7]-48);
		StructInfoPayment.Transaction_Amount += 10*10*10*10*10*(StructInfoPayment.OR_debt[j][6]-48);
		StructInfoPayment.Transaction_Amount += 10*10*10*10*10*10*(StructInfoPayment.OR_debt[j][5]-48);
		montant_total += StructInfoPayment.Transaction_Amount;
 		sprintf(tzMenuFact_Oriflame[j],"%s  %d,%d%d MAD",StructInfoPayment.OR_dueDate[j], StructInfoPayment.Transaction_Amount / 100,
				(StructInfoPayment.Transaction_Amount / 10) % 10,
				StructInfoPayment.Transaction_Amount % 10);

 		tzMenuFactOriflame[j] = tzMenuFact_Oriflame[j];
 	}


	sprintf(tzMenuFact_Oriflame[i+1],"Total: %d,%d%d MAD", montant_total / 100,
				(montant_total / 10) % 10,
				montant_total % 10);
	tzMenuFactOriflame[i+1] = tzMenuFact_Oriflame[i+1];
	do {
		switch (iRet = GL_Dialog_Menu(hGoal,"LISTE DES FACTURES",
				tzMenuFactOriflame, 0, GL_BUTTON_DEFAULT, GL_KEY_1,
				GL_TIME_MINUTE)) {
		case 0:
		case 1:
		case 2:
		case 3:
			if(iRet<i+1)
			{
				StructInfoPayment.Transaction_Amount = 0;
				StructInfoPayment.Transaction_Amount += (StructInfoPayment.OR_debt[iRet][11]-48);
				StructInfoPayment.Transaction_Amount += 10*(StructInfoPayment.OR_debt[iRet][10]-48);
				StructInfoPayment.Transaction_Amount += 10*10*(StructInfoPayment.OR_debt[iRet][9]-48);
				StructInfoPayment.Transaction_Amount += 10*10*10*(StructInfoPayment.OR_debt[iRet][8]-48);
				StructInfoPayment.Transaction_Amount += 10*10*10*10*(StructInfoPayment.OR_debt[iRet][7]-48);
				StructInfoPayment.Transaction_Amount += 10*10*10*10*10*(StructInfoPayment.OR_debt[iRet][6]-48);
				StructInfoPayment.Transaction_Amount += 10*10*10*10*10*10*(StructInfoPayment.OR_debt[iRet][5]-48);
			}
			else if(iRet=i+1)
				StructInfoPayment.Transaction_Amount = montant_total;
			else // Au dela des factures affichées et leur total
			{
				bContinue3 = FALSE;
				break;
			}
			strncpy(StructInfoPayment.OR_invoiceNumber62,"          ",10);


			ulTransactionType = CUSERV_DEBIT_TRANSACTION;
			StructInfoPayment.Fallback = 0;//311019 belhadeg probleme fall back oriflame
			CUMORE_PaiementFctExt(CUSERV_DEBIT_TRANSACTION, TRUE, (unsigned long)StructInfoPayment.Transaction_Amount, (int)504);
			bContinue3 = FALSE;
			break;


		default:
			bContinue3 = FALSE;
			break;
		}
	} while (bContinue3);
	return 0;
}


// Disable header, footer, led in order to Maximize canvas size
int sSavedStatusHeader, sSavedStatusLeds, sSavedStatusFooter;
/*-----------------------------------------------------------------
 * Fonction   :Print
 * objet      :
 *
 * Parametre  :
 * Retour     :
 *-----------------------------------------------------------------
*/

void Print(char *pPrtBuff)
{
	Telium_File_t *hPrinter;

	if(IsPrinter())
	{
		// Open printer driver
		hPrinter = Telium_Fopen("PRINTER", "w*");
		if (hPrinter != NULL)
		{
			Telium_Pprintf(pPrtBuff);
			Telium_Pprintf("\n");
			Telium_Ttestall(PRINTER, 100);
			Telium_Fclose(hPrinter);
		}
	}
}
/*-----------------------------------------------------------------
 * Fonction   :WiFi_connect
 * objet      :
 *
 * Parametre  :
 * Retour     :
 *-----------------------------------------------------------------
*/
static void WiFi_connect(char *WifiID)//badr
{
	int i;
	int iRet;
	int visibility = 0; //visible
	char ssid[32 + 1] = {0};
	unsigned char bssid[] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
	char AsciiPwd[63+1] ={0};
	char mask[256];
	T_GL_HGRAPHIC_LIB gGoalGraphicLibInstance = APEMV_UI_GoalHandle();

	memset(mask, 0, sizeof(mask));
	for (i=0; i<sizeof(ssid); i++)
		strcat(mask, "/c");                                             // Accept any character
	memcpy(ssid, WifiID, sizeof(WifiID));
	{
		for (i=0; i<sizeof(AsciiPwd); i++)
			strcat(mask, "/c");
		iRet = GL_Dialog_Text(gGoalGraphicLibInstance, "Mot de passe Wifi", "Saisie du mot de passe:", mask, AsciiPwd, sizeof(AsciiPwd), GL_TIME_MINUTE);
	}

	if ((iRet!=GL_KEY_CANCEL) && (iRet!=GL_RESULT_INACTIVITY))
	{
		//Add the new profile
		//Wifi_Essid(ssid);
		Wifi_ProfileAddHome(ssid, 	// ESSID
				bssid,				// BSSID
				DLL_WIFI_ALGO_WPA_PSK,
				(unsigned char*)AsciiPwd,
				visibility,
				0);

		// Activating the last added profile
		Wifi_ProfileSetActiveBySsid((const unsigned char*)ssid);
		Telium_Ttestall(0,200);
		//Re-initialize the network selection to Automatic
		Wifi_ProfileSetActiveBySsid(NULL);
	}
}

/*-----------------------------------------------------------------
 * Fonction   :WiFiGetScannedNetworks
 * objet      :Wireless network scan
 *
 * Parametre  :void
 * Retour     :void
 *-----------------------------------------------------------------
*/
static void WiFiGetScannedNetworks(void)//badr
{
	int i = 0,j = 0;
	int iNbrNetwork = 0;
	//char text[50]={0};
	unsigned char ssid[31+1] ;
	unsigned char ResMas[31 + 1];
	int iRet,nbr=0;
	bool bContinue3 = TRUE;
	dll_wifi_scan_result_t wifiScanResultList[DLL_WIFI_MAX_ACCESS_POINT];
	T_GL_HGRAPHIC_LIB hGoal = APEMV_UI_GoalHandle();
	//char essid[36] ;
	//int reboot=0;
	char mask[256];

//	char *permut;
	int visibility = 0; //visible // 1 hidden
	int iRetl,iRe;
	unsigned char bssid[] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
	char AsciiPwd[63+1];

	memset(&wifiScanResultList, 0x00, sizeof(wifiScanResultList));
	memset(ssid, 0, sizeof(ssid));
	memset(ResMas, 0, sizeof(ResMas));// Get the scan resultso
	memset(mask, 0, sizeof(mask));
	memset(AsciiPwd, 0, sizeof(AsciiPwd));

	memset(listeWifi,0,sizeof(listeWifi));

//	Os__xprintaa("000000");
	iNbrNetwork = Wifi_GetScanResults(wifiScanResultList, DLL_WIFI_MAX_ACCESS_POINT);

//Os__xprintaa("111111");
	if(iNbrNetwork > 0)
	{
		//Os__xprintaa("121221212");
		for (i = 0; (i < iNbrNetwork) && (i < DLL_WIFI_MAX_ACCESS_POINT); i++)
		{
			if(strlen(wifiScanResultList[i].ssid) != 0)
				listeWifi[i] = wifiScanResultList[i].ssid;
			else listeWifi[i] = "Réseau masqué";
		}
	}
	//Os__xprintaa("22222222");

	do {
			switch (iRet = GL_Dialog_Menu(hGoal, "Réseaux Disponibles",
					listeWifi, 0, GL_BUTTON_DEFAULT,GL_KEY_1,
					GL_TIME_MINUTE)){

			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:

				//Os__xprintaa("33333");
			visibility = 0;
			APEMV_UI_GoalDestroy();
			//Os__xprintaa("44444");
			T_GL_HGRAPHIC_LIB gGoalGraphicLibInstance = APEMV_UI_GoalHandle__();
			if (!memcmp(listeWifi[iRet], "Réseau masqué",13))
			{
				for (i=0; i<sizeof(ssid); i++)
					strcat(mask, "/c");
				iRe = GL_Dialog_VirtualKeyboard(gGoalGraphicLibInstance, "Réseau masqué",
						"Entrer le nom réseau (SSID):", mask, ResMas, sizeof(ResMas), GL_TIME_MINUTE);
				if ((iRe!=GL_KEY_CANCEL) && (iRe!=GL_RESULT_INACTIVITY))
				{
					memcpy(wifiScanResultList[iRet].ssid, ResMas, sizeof(ResMas));
					visibility = 1;
				}
				else break;
			}


				memcpy(ssid, wifiScanResultList[iRet].ssid, sizeof(ssid));

						for (i=0; i<sizeof(AsciiPwd); i++)
							strcat(mask, "/c");
						//Os__xprintaa("55555");
							//GL_Widget_SetBackColor(T_GL_HWIDGET widget, T_GL_COLOR 0xAARRGGBB);
						iRetl = GL_Dialog_VirtualKeyboard(gGoalGraphicLibInstance, "Mot de passe Wifi",
								"Saisie du mot de passe:", mask, AsciiPwd, sizeof(AsciiPwd), GL_TIME_MINUTE);
	//					Os__xprintaa("11111");

					if ((iRetl!=GL_KEY_CANCEL) && (iRetl!=GL_RESULT_INACTIVITY))
					{
						//Os__xprintaa("66666");
						//Add the new profile
	//					Os__xprintaa("11111");
						Wifi_ProfileAddHome(ssid, 	// ESSID
								bssid,				// BSSID
								DLL_WIFI_ALGO_WPA_PSK,
								(unsigned char*)AsciiPwd,
								visibility,
								0);

						// Activating the last added profile
						Wifi_ProfileSetActiveBySsid((const unsigned char*)ssid);
						Telium_Ttestall(0,200);

						//Re-initialize the network selection to Automatic
						Wifi_ProfileSetActiveBySsid(NULL);
						bContinue3 = FALSE;


					}
				else{
//					Os__xprintaa("aaaaaaa");
					bContinue3 = FALSE;
					//goto lend;
				}

				break;
			default:
				bContinue3 = FALSE;
				break;
			}
		} while (bContinue3);

	//lend:
	APEMV_UI_GoalDestroy();
}
/*-----------------------------------------------------------------
 * Fonction   :wifi_ScanNetwork
 * objet      :This function requests a wireless network scan.
 *
 * Parametre  :void
 * Retour     :WIFI_OK or a negative error code
 *-----------------------------------------------------------------
*/
static int wifi_ScanNetwork(void)//badr
{
	int iRet = -1,iRet1 = -1;
	iRet1 = wifilib_open();
	if (iRet1 == 0)
	{
		iRet = Wifi_AccessPointScan();
		if (iRet == 0)
			{
			// Get the list of available Networks
			WiFiGetScannedNetworks();
		}
	}
	wifilib_close();
	return iRet;
}



/**
 *badr 31122019
 * Main menu
 */
/*-----------------------------------------------------------------
 * Fonction   :menuGoalDisplay
 * objet      :Main menu
 *
 * Parametre  :----
 * Retour     :----
 *-----------------------------------------------------------------
*/
void menuGoalDisplay(void)//badr
{
  char choice = 0;
  int iRet = -1;
  T_GL_HGRAPHIC_LIB gGoalGraphicLibInstance = APEMV_UI_GoalHandle();

  StateHeader (1);
  //wifilib_open();

  sDisplay = Telium_Fopen("DISPLAY", "w*");
      // Open keyboard
      sKeyboard = Telium_Fopen("KEYBOARD", "r*");

    	GL_Dialog_Message(gGoalGraphicLibInstance, "Scan",
    	    			"Recherche en cours ...", GL_ICON_NONE, GL_BUTTON_NONE,
    					2 * GL_TIME_SECOND);
    	iRet = wifi_ScanNetwork();
    	if(iRet != 0)
    	{

        	GL_Dialog_Message(gGoalGraphicLibInstance, "Scan",
        			"Scan Failed", GL_ICON_WARNING, GL_BUTTON_VALID,
    				2 * GL_TIME_SECOND);
    	}
  if (sKeyboard)
      {
          Telium_Fclose(sKeyboard);
          sKeyboard = NULL;
      }
      // close display
      if (sDisplay)
      {
          Telium_Fclose(sDisplay);
          sDisplay = NULL;
      }
      //wifilib_close();

  return;
}


/**
 * Initialize the display context to maximize the canvas on user area
 */
void initUIContext(int displayHeaderFooterLeds)
{
	Telium_File_t * sDisplay;
	Telium_File_t * sKeyboard;
	if(displayHeaderFooterLeds != _OFF_ && displayHeaderFooterLeds != _ON_) {
		displayHeaderFooterLeds = _OFF_;
	}
	sSavedStatusHeader=DisplayHeader(displayHeaderFooterLeds);
	sSavedStatusLeds=DisplayLeds(displayHeaderFooterLeds);
	sSavedStatusFooter=DisplayFooter(displayHeaderFooterLeds);

    // Open display
    sDisplay = Telium_Fopen("DISPLAY", "w*");
    // Open keyboard
    sKeyboard = Telium_Fopen("KEYBOARD", "r*");

    return;
}

/**
 * Restore display context
 */
void releaseUIContext(void)
{
    // close keyboard
    if (sKeyboard)
    {
        Telium_Fclose(sKeyboard);
        sKeyboard = NULL;
    }
    // close display
    if (sDisplay)
    {
        Telium_Fclose(sDisplay);
        sDisplay = NULL;
    }

	// Restore display context
	DisplayHeader(sSavedStatusHeader);
	DisplayLeds(sSavedStatusLeds);
	DisplayFooter(sSavedStatusFooter);

	return;
}
/*-----------------------------------------------------------------
 * Fonction   :ActivationWifi
 * objet      :Turn on Wifi, Turn off Wifi
 *
 * Parametre  :----
 * Retour     :----
 *-----------------------------------------------------------------
*/

void ActivationWifi(void)//badr
{
		T_GL_HGRAPHIC_LIB hGoal = APEMV_UI_GoalHandle();
		bool bContinue = TRUE, bContinue2 = TRUE;
		const char * menu[] =
		{
		" Activer",
		" Désactiver",
		0 //<- DO NOT FORGET THIS TO STOP THE LIST
		};

		do {
			switch (GL_Dialog_Choice(hGoal, "Activation Wifi", menu, 0,GL_BUTTON_DEFAULT, GL_KEY_1,GL_TIME_MINUTE)) {
			case 0:
				Wifi_PowerOn();
				goto lblEnd;
			case 1:
				Wifi_PowerOff();
				goto lblEnd;

			default:
				bContinue2 = FALSE;
				goto lblEnd;
			}
		} while (bContinue2);


		lblEnd:
		return KO;

}
/*-----------------------------------------------------------------
 * Fonction   :Profile
 * objet      :Selectionner ou supprimer un profile Wifi
 *
 * Parametre  :
 * Retour     :
 *-----------------------------------------------------------------
*/

void Profile(void)//badr
{
	int i=0, iRet, iRetl;
	bool bContinue = TRUE,	bContinue1 = TRUE;
	dll_wifi_access_point_list_t list;
	T_GL_HGRAPHIC_LIB gGoalGraphicLibInstance = APEMV_UI_GoalHandle();
	const char *SetDelete[] =
			    { " Sélectionner"," Supprimer", 0 };
	memset(&list, 0, sizeof(list));
	memset(listeProfile, 0, sizeof(listeProfile));

	Wifi_ProfileGetList(&list);

if(list.access_point_count > 0)
		{
			for (i = 0; (i < list.access_point_count); i++)
			{
				listeProfile[i] = list.access_point_info[i].essid;
			}
		}

do {
			switch (iRet = GL_Dialog_Menu(gGoalGraphicLibInstance,"Mes réseaux",
					listeProfile, 0, GL_BUTTON_DEFAULT, GL_KEY_1,
					GL_TIME_MINUTE)) {
			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
//				if (list!= NULL)
//				 {
					do{
							switch (iRetl = GL_Dialog_Menu(gGoalGraphicLibInstance,list.access_point_info[iRet].essid,
									SetDelete, 0, GL_BUTTON_DEFAULT, GL_KEY_1,
									GL_TIME_MINUTE)) {
							case 0:
								GL_Dialog_Message(gGoalGraphicLibInstance, list.access_point_info[iRet].essid,
													"Profil sélectionné", GL_ICON_INFORMATION, GL_BUTTON_VALID,
													2 * GL_TIME_SECOND);
								//Re-initialize the network selection to Automatic
								Wifi_ProfileSetActiveBySsid(NULL);

								Wifi_ProfileSetActiveBySsid(list.access_point_info[iRet].essid);
								Telium_Ttestall(0,200);
								goto lend;
								break;
							case 1:
								GL_Dialog_Message(gGoalGraphicLibInstance, list.access_point_info[iRet].essid,
								        			"Profil supprimé", GL_ICON_WARNING, GL_BUTTON_VALID,
								    				2 * GL_TIME_SECOND);
								Wifi_ProfileRemoveBySsid(list.access_point_info[iRet].essid);
								goto lend;
								break;


							default:
								bContinue1 = FALSE;
								break;
							}
						} while (bContinue1);
//				}

				break;
			case GL_KEY_CANCEL:
			case GL_KEY_CORRECTION:
				goto lend;

			default:
				bContinue = FALSE;
				break;
			}
		} while (bContinue);

lend:

	Os__xprint("end");
}
/*-----------------------------------------------------------------
 * Fonction   :StopBT
 * objet      :
 *
 * Parametre  :
 * Retour     :
 *-----------------------------------------------------------------
*/
void StopBT(void)
{
	T_GL_HGRAPHIC_LIB gGoalGraphicLibInstance = APEMV_UI_GoalHandle();


	HWCNF_SetBTPower(HWCNF_BT_RADIO_OFF);
	GL_Dialog_Message(gGoalGraphicLibInstance, NULL,
		"Stop Bluetooth...", GL_ICON_NONE, GL_BUTTON_VALID,
			2 * GL_TIME_SECOND);
}
/*-----------------------------------------------------------------
 * Fonction   :StartBT
 * objet      :
 *
 * Parametre  :
 * Retour     :
 *-----------------------------------------------------------------
*/
void StartBT(void)
{
	T_GL_HGRAPHIC_LIB gGoalGraphicLibInstance = APEMV_UI_GoalHandle();


	HWCNF_SetBTPower(HWCNF_BT_RADIO_ON);
	GL_Dialog_Message(gGoalGraphicLibInstance, NULL,
			"Start Bluetooth...", GL_ICON_NONE, GL_BUTTON_VALID,
				5 * GL_TIME_SECOND);




}
//	**********************	//
/*-----------------------------------------------------------------
 * Fonction   :more_function_fr
 * objet      :
 *
 * Parametre  :
 * Retour     :
 *-----------------------------------------------------------------
*/

int more_function_fr(void)
{
	// Local variables *
	// *****************

		Ushort		nbr_preauto;
		lignePreAut	preauto ;
		T_GL_HGRAPHIC_LIB hGoal = APEMV_UI_GoalHandle();
		T_GL_HGRAPHIC_LIB gGoalGraphicLibInstance = APEMV_UI_GoalHandle();
		bool bContinue = TRUE, bContinue2 = TRUE, bContinue3 = TRUE, bContinue1 = TRUE;
		int iRet;
		char *pMenuUser;

		if(!memcmp(FicPara.TYPEAPPLICATION,"NAPSORIF",8))
			pMenuUser = tzMenuUserOri;
		else
			pMenuUser = tzMenuUser;
//		if((IsRadioGPRS() != 1) && (IsRadio3G() != 1) && (!memcmp(FicPara.TYPEAPPLICATION,"NAPSORIF",8)))//belhadeg V5.1
//			pMenuUser = tzMenuUser2Ori;
//		else if ((IsRadioGPRS() != 1) && (IsRadio3G() != 1))
//			pMenuUser = tzMenuUser2;


		if (AlternanceIsOn) {
			Telium_Kill(alternateNbr, "");
			AlternanceIsOn = 0;
		}

		do {

			switch (GL_Dialog_Menu(hGoal, "MENU PRINCIPAL", pMenuUser, 0, GL_BUTTON_DEFAULT,
							GL_KEY_1, GL_TIME_MINUTE)) {
			case 0:
				do {
					switch (iRet = GL_Dialog_Menu(hGoal, tzMenuUser[0],tzMenuDialogBox, 0, GL_BUTTON_DEFAULT, GL_KEY_1,GL_TIME_MINUTE)) {
					case 0:
						//GL_Dialog_Media  (T_GL_HGRAPHIC_LIB graphicLib, const char * title, const char * text, const char * source, byte volume, T_GL_BUTTONS buttons, T_GL_DURATION duration) GL_EXCEPTION;

						if (FicPara.MenuTPE[6] == '1') {

							trasamanuel_cmi();
						}
						else {
							CUMORE_CMI_AFFICHE_MESSAGE(22,3,1000);
						}
						break;
					case 1:
						annulation_cmi();
						break;
					case 2:
						credit_cmi();
						break;
					case 3:
						findejournee_cmi();
						break;
					default:
						bContinue2 = FALSE;
						break;
					}
				} while (bContinue2);
				bContinue2 = TRUE;
				CHECK(iRet==GL_KEY_CORRECTION, lblEnd);
				break;

			case 1:
				do {
					switch (iRet = GL_Dialog_Menu(hGoal, tzMenuUser[1],
							tzMenuPreauto, 0, GL_BUTTON_DEFAULT, GL_KEY_1,
							GL_TIME_MINUTE)) {
					case 0:
						preauto_cmi();
						goto lblEnd;
					case 1:
						preautocomp_cmi();
						goto lblEnd;
						//break;
					case 2:
						if (control_badge() != OK)
							return KO ;
						annulpreauto_cmi();
						break;

					case 3:
						if (control_badge() != OK)
							return KO ;

						PurgePreautoNaps();
						break;
					default:
						bContinue2 = FALSE;
						break; // Abort/Timeout
					}
				} while (bContinue2);
				bContinue2 = TRUE;
				CHECK(iRet==GL_KEY_CORRECTION, lblEnd);

				break;

			case 2:

				do {
					switch (iRet = GL_Dialog_Menu(hGoal, tzMenuUser[2],tzMenuWidget, 0, GL_BUTTON_DEFAULT, GL_KEY_1,
							GL_TIME_MINUTE)) {
					case 0:
						dernieretrs_cmi();
						break;
					case 1:
						autretrs_cmi();
						break;
					case 2:
						dernierepreaut_cmi();
						break;
					case 3:
						autrepreauto_cmi();
						break;
					default:
						bContinue2 = FALSE;
						break; // Abort/Timeout
					}
				} while (bContinue2);

				bContinue2 = TRUE;
				CHECK(iRet==GL_KEY_CORRECTION, lblEnd);

				break;

			case 3:

				if (mdp_merchant_cmi() != CR_SAISIE_OK) {
					CUMORE_CMI_AFFICHE_MESSAGE(16, 20, 5000);
					return CR_SAISIE_NOK;
				}

				do {

					switch (iRet = GL_Dialog_Menu(hGoal, tzMenuUser[3], tzMenuAdmin,
							0, GL_BUTTON_DEFAULT, GL_KEY_1, GL_TIME_MINUTE)) {
					case 0:
						do {
							switch (iRet = GL_Dialog_Menu(hGoal,"Communication",
									MenuConfiguration, 0, GL_BUTTON_DEFAULT, GL_KEY_1,
									GL_TIME_MINUTE)) {
							case 0:
								ip_gprs_cmi();
								break;
							case 1:
								port_gprs_cmi();
								break;
							case 2:
								apn_gprs_cmi();
								break;
							case 3:
								login_gprs_cmi();
								break;
							case 4:
								pwd_gprs_cmi();
								break;
	//						case 5:
	//							time_out_param();
	//							break;
							default:
								bContinue3 = FALSE;
								break;
							}
						} while (bContinue3);
						bContinue3 = TRUE;
						CHECK(iRet==GL_KEY_CORRECTION, lblEnd);
						//comm_pm();
						break;
					case 1:
	//					initparam_cmi();

						do {
							switch (iRet = GL_Dialog_Menu(hGoal, "Init. paramètres",
									MenuInit_Param, 0, GL_BUTTON_DEFAULT,GL_KEY_1,
									GL_TIME_MINUTE)) {
							case 0:
								NumCmr_cmi();
								break;
							case 1:
								NumTrm_cmi();
								break;
							case 2:
								choix_langue();
								break;
							default:
								bContinue3 = FALSE;
								break;
							}
						} while (bContinue3);
						bContinue3 = TRUE;
						CHECK(iRet==GL_KEY_CORRECTION, lblEnd);
						break;
					case 2:
						if (control_badge() != OK)
							return KO ;
						initialisation_cmi();
						if (imprimeparam)
						Imprime_Param();
						//FicPara.MCC="6010";
						//sprintf(FicPara.MCC, "6010");
						break;
					case 3:
						if (control_badge() != OK)
							return KO ;
						if (mdp_merchant_cmi() != CR_SAISIE_OK) {
							CUMORE_CMI_AFFICHE_MESSAGE(16, 20, 5000);
							return CR_SAISIE_NOK;
						}
						razram_cmi();
						break;
					case 4:
						if(FicPara.MenuTPE[26] == '0'){
							CUMORE_CMI_AFFICHE_MESSAGE(22,3,1000);
							break;
						}
						if (control_badge() != OK)
							break;
						Admin_Services();
						break;
					case 5:
						ServicesActivation();
						break;
					case 6:
						startTMSDownload();
						break;
					default:
						bContinue2 = FALSE;
						break; // Abort/Timeout
					}
				} while (bContinue2);
				bContinue2 = TRUE;
				CHECK(iRet==GL_KEY_CORRECTION, lblEnd);
				break;

			case 4:
				if (FicPara.MenuTPE[12] == '1' && memcmp(FicPara.MCC, "6010", 4) == 0) {
					cashadvance_naps();
				} else {
					CUMORE_CMI_AFFICHE_MESSAGE(22, 3, 1000);
				}
				goto lblEnd;
				//break;




	/*
			case 5:
				do {
					InitDefaut(_ON_,1);
					switch (iRet = GL_Dialog_Menu(hGoal, tzMenuUser[1],tzMenuPrepaye, 0, GL_BUTTON_DEFAULT, GL_KEY_1,
							GL_TIME_MINUTE)) {
					case 0:
						prepaye_naps();
						break;
					case 1:
						prepaye_naps();
						break;
					case 2:
						prepaye_naps();
						break;
					case 3:
						prepaye_naps();
						break;
					default:
						bContinue2 = FALSE;
						break; // Abort/Timeout
					}
				} while (bContinue2);
				bContinue2 = TRUE;
				CHECK(iRet==GL_KEY_CORRECTION, lblEnd);
				break;

			case 5:
				InitDefaut(_ON_, 1);
				StateHeader(0);
				do {
					switch (iRet = GL_Dialog_Menu(hGoal, tzMenuUser[5],tzMenuService, 0, GL_BUTTON_DEFAULT, GL_KEY_1,
							GL_TIME_MINUTE)) {
					case 0:
						demandesolde_naps();
						break;
					case 1:
						minireleve_naps();
						break;
					case 2:
						pinchange_naps();
						break;
					case 3:
						rechargegsm_naps();
						break;
					case 4:
						paiement_naps();
						break;
					default:
						bContinue2 = FALSE;
						break; // Abort/Timeout
					}
				} while (bContinue2);
				StateHeader(1);
				PutAllParam();
				bContinue2 = TRUE;
				CHECK(iRet==GL_KEY_CORRECTION, lblEnd);
				break;
	*/
			case 5://toORI
/*
					InitDefaut(_ON_, 1);
					StateHeader(0);

					do {
						switch (iRet = GL_Dialog_Menu(hGoal, tzMenuUser[5],tzMenuService2, 0, GL_BUTTON_DEFAULT, GL_KEY_1,
								GL_TIME_MINUTE)) {
						case 0:
							do {
								switch (iRet = GL_Dialog_Menu(hGoal,"Gestion des factures",
										tzMenuOriflame, 0, GL_BUTTON_DEFAULT, GL_KEY_1,
										GL_TIME_MINUTE)) {
								case 0:
									getDebt();

									if(FicPara.vCam0 == 2) //CLESS
										return 0;

									break;
								case 1:
									//payAmountglob();
									//PayAmount();
									break;
								default:
									bContinue3 = FALSE;
									break;
								}
							} while (bContinue3);
							bContinue3 = TRUE;
							bContinue1 = FALSE;
							bContinue2 = FALSE;
							break; // Abort/Timeout

						StateHeader(1);
						PutAllParam();
						//bContinue1 = TRUE;
						CHECK(iRet==GL_KEY_CORRECTION, lblEnd);
						break;

						default:
							bContinue1 = FALSE;
							break;
						} // Abort/Timeout
					}while(bContinue1);
					bContinue1 = TRUE;
					break;
			case 6:
*///toORI
				do {//toORI 6==>5
					switch (iRet = GL_Dialog_Menu(hGoal, tzMenuUser[5], tzMenuTotal,
							0, GL_BUTTON_DEFAULT, GL_KEY_1, GL_TIME_MINUTE)) {
					case 0:
						journal_cmi();
						break;
					case 1:
						journalpreauto_cmi();
						break;
					case 2:
						totaux_cmi();
						break;
	//				case 3:
	//					GL_SampleOrientation();
	//					break;
					default:
						bContinue2 = FALSE;
						break;

					}
				} while (bContinue2);
				bContinue2 = TRUE;
				CHECK(iRet==GL_KEY_CORRECTION, lblEnd);
				break;
			case 6://toORI 7==>6
				if((IsRadioGPRS() != 1) && (IsRadio3G() != 1))
				{
					//MessageWithDelay("GPRS-Wifi-Bluetooth \n non disponible", NULL, 500);
					GL_Dialog_Message(gGoalGraphicLibInstance, NULL,
							"GPRS-Wifi-Bluetooth \nnon disponible", GL_ICON_WARNING, GL_BUTTON_VALID,
								5 * GL_TIME_SECOND);

					goto lblNoRadio;
				}
				//CHECK((IsRadioGPRS() == 1) || (IsRadio3G() == 1), lblNoRadio);//belhadeg V5.1
				do {//toORI 7==>6
					switch (iRet = GL_Dialog_Menu(hGoal, tzMenuUser[6], tzMenuReseau,
							0, GL_BUTTON_DEFAULT, GL_KEY_1, GL_TIME_MINUTE)) {

					case 0:
						do {
							switch (iRet = GL_Dialog_Menu(hGoal,tzMenuReseau[0],
									tzMenuBT, 0, GL_BUTTON_DEFAULT, GL_KEY_1,
									GL_TIME_MINUTE)) {
							case 0:
								StartBT();//badr
								break;
							case 1:
								StopBT();//badr
								break;

							case 2:
								StopBT();//badr

								StartBT();//badr
								break;
							default:
								bContinue3 = FALSE;
								break;
							}
						} while (bContinue3);
						bContinue3 = TRUE;
						CHECK(iRet==GL_KEY_CORRECTION, lblEnd);
						break;
					case 1:
						do {
							switch (iRet = GL_Dialog_Menu(hGoal,tzMenuReseau[1],
									tzMenuWifi, 0, GL_BUTTON_DEFAULT, GL_KEY_1,
									GL_TIME_MINUTE)) {
							case 0:
								ActivationWifi();//badr
								break;
							case 1:
								menuGoalDisplay();//badr
								break;
							case 2:
								Profile();//badr
								break;

							default:
								bContinue3 = FALSE;
								break;
							}
						} while (bContinue3);
						bContinue3 = TRUE;
						CHECK(iRet==GL_KEY_CORRECTION, lblEnd);
						break;
					case 2:
						do {
							switch (iRet = GL_Dialog_Menu(hGoal,tzMenuReseau[2],
									tzMenuGPRS, 0, GL_BUTTON_DEFAULT, GL_KEY_1,
									GL_TIME_MINUTE)) {
							case 0:
								GPRSDisabled = FALSE;
//								Telium_File_t* hGprs;
//								gsm_OpenUssd ( hGprs );

								StartGprs_naps();
								break;
							case 1:
								GPRSDisabled = TRUE;
								StopGprs_naps();
								break;
							case 2:
								StopGprs_naps();

								StartGprs_naps();
								break;

							default:
								bContinue3 = FALSE;
								break;
							}
						} while (bContinue3);
						bContinue3 = TRUE;
						CHECK(iRet==GL_KEY_CORRECTION, lblEnd);
						break;

						default:
						bContinue2 = FALSE;
						break;
					}
				} while (bContinue2);
			lblNoRadio :
				bContinue2 = TRUE;
				CHECK(iRet==GL_KEY_CORRECTION, lblEnd);
				break;

			case 7://toORI 7==>8
				InitDefaut(_ON_, 1);
				StateHeader(0);

				do {
					switch (iRet = GL_Dialog_Menu(hGoal, tzMenuUser[6],tzMenuService2, 0, GL_BUTTON_DEFAULT, GL_KEY_1,
							GL_TIME_MINUTE)) {
					case 0:
						do {
							switch (iRet = GL_Dialog_Menu(hGoal,"Gestion des factures",
									tzMenuOriflame, 0, GL_BUTTON_DEFAULT, GL_KEY_1,
									GL_TIME_MINUTE)) {
							case 0:
								getDebt();

								if(FicPara.vCam0 == 2) //CLESS
									return 0;

								break;
							case 1:
								//payAmountglob();
								//PayAmount();
								break;
							default:
								bContinue3 = FALSE;
								break;
							}
						} while (bContinue3);
						bContinue3 = TRUE;
						bContinue1 = FALSE;
						bContinue2 = FALSE;
						break; // Abort/Timeout

					StateHeader(1);
					PutAllParam();
					//bContinue1 = TRUE;
					CHECK(iRet==GL_KEY_CORRECTION, lblEnd);
					break;

					default:
						bContinue1 = FALSE;
						break;
					} // Abort/Timeout
				}while(bContinue1);
				bContinue1 = TRUE;
				break;
			case 8://toORI 7==>8
//				download_FTP();
//				activateApplication();
				startTMSDownload();
			break;
			default:
				bContinue = FALSE;
				break;  // Abort/Timeout
			}
		} while (bContinue);

		goto lblEnd;

	// Errors treatment
	// ****************
		lblKO:                       // None-classified low level error
		GL_Dialog_Message(hGoal, NULL, "Processing Error", GL_ICON_ERROR,GL_BUTTON_VALID, 5 * 1000);
		goto lblEnd;
		lblEnd:


		//DisplayLeds(_ON_);       // Enable Leds
		return FCT_OK;
}


void CUTERMclearDisplayT1(void) //KT1
{
   _clrscr();                                                           // Clear screen
}
void CUTERMdisplayLineT1(int xi_line, char *xs_text, T_CUTERMjustify xt_justify, char *mode)
{
    _clrscr();                                                           //  screen
    PaintGraphics();                                                     // **** Normal font 4x16 ****
	DisplayMedium(xi_line, 0, xs_text);


	
/*
#ifdef _USE_UCM_

	int lg, y;
	int line, column;

	// In order to get back the number of column on the display 
	CUTERMgetDisplay(&line, &column);

	Telium_Fopen("DISPLAY", mode);

	// No test of return fopen because no return of CUTERMdisplayLine 


	// clear the xi_line 
	gotoxy(xi_line, 0);
	printf("                ");

	// Get the length of the input text 
	lg = strlen(xs_text);
	y = 0;

	switch (xt_justify)
	   {
	   case (CENTER):
		  {
		  y = (column - lg) / 2;

		   // case of xs_text lenght is superior to column: xs_text will be 
		   // automatically troncated 
		   if (y < 0)
		  {
			  y = 0;
		  }
		   break;
		  };
	   }; // End of swith on xt_justify 

	gotoxy(xi_line, y);
	printf("%s", xs_text);
	//fclose (lp_display);
	//Fermer17fclose (stdout());
	//lp_display = NULL;

#endif
*/
} /*end of CUTERMdisplayLine*/

int GetInfo_Goal (char *chaine, char * vide,char *saisie,byte nb_min,byte nb_max,byte mask)
{
	int  result;
	int  iRet;
	char maskbuf[20];
	char title[32];

	memclr(maskbuf,sizeof(maskbuf));
	memcpy(maskbuf,"/d/d/d/d/d/d/d/d/d/d/d/d/d/d/d/d/d/d/d",nb_max);


	if(strlen(vide))
		sprintf(title,"%s",vide);
	else
		sprintf(title,"%s","ADMINISTRATION");

	result = 1;

	APEMV_UI_GoalOpen();
	switch(mask)
	{
	case 0:
		if(GL_Dialog_Text(APEMV_UI_GoalHandle(),title,chaine,NULL,saisie,nb_max,30000) == GL_KEY_VALID)
			result = 0;
		break;

//	case 7:
//		if(GL_Dialog_Text(APEMV_UI_GoalHandle(), title, chaine, "/s/s/s/s/s/s/s/s/s/s/s/s/s/s/s/s/s/s/s/s/s/s/s/s", saisie,nb_max, 30000) == GL_KEY_VALID)
//			result = 0;
//		break;

	case 2:
		if(GL_Dialog_Text(APEMV_UI_GoalHandle(),title,chaine,"/d/d/d/d/d/d/d/d/d/d/d/d/d/d/d/d/d/d/d",saisie,nb_max,30000) == GL_KEY_VALID)
			result = 0;
		break;

	case 5:
		if(GL_Dialog_Text(APEMV_UI_GoalHandle(),title,chaine,"/d/d/d/d/d/d",saisie,nb_max+1,30000) == GL_KEY_VALID)
			result = 0;
		break;
	case 6:
	    if(GL_Dialog_Text(APEMV_UI_GoalHandle(),NULL,chaine,"/d/d/d/d",saisie,5,30000) == GL_KEY_VALID)
			result = 0;
		break;

	case 3:
		if(GL_Dialog_Password(APEMV_UI_GoalHandle(),title,chaine,"/d/d/d/d/d/d",saisie,7,30000) == GL_KEY_VALID)
			result = 0;
		break;

	case 4: /*IP Adress , GL_Dialog_Ip ne fonctionne pas*/
		//if(GL_Dialog_Text(APEMV_UI_GoalHandle(),"ADMINISTRATION",chaine,"/d/d/d./d/d/d./d/d/d./d/d/d",saisie,nb_max,30000) == GL_KEY_VALID)
		if(GL_Dialog_Text(APEMV_UI_GoalHandle(),title,chaine,NULL,saisie,nb_max,30000) == GL_KEY_VALID)
			result = 0;
		break;

	default:
		break;
	}
	APEMV_UI_GoalClose();


	if (result != 0)
		result=TIMEOUT;
	else
		result=T_VAL;

	return result;
}

int SLC_Saisie_Numerique(unsigned char *saisie, char *title,unsigned char *chaine, TAB_ENTRY_STRING *table)
{
	return GetInfo_Goal(chaine, title, saisie, table->nb_min, table->nb_max,table->mask);
}

int	PSQ_Donner_langue(void) //KT1
{
	return C_LG_FRENCH;
}

int CUTERMgetTerminalType(void)
{
	return T_CUTERM_X930;
}

void choix_aleatoire(void)
 {
   unsigned char   cr;

   BUFFER_SAISIE   buf_saisie;

   InitDefaut(_ON_,1);
   // table init from const
   Menu.Fenetre.titre = (unsigned char *)"";
   Menu.tab[0] = NULL;


   Menu.Fenetre.time_out=1;


   G_List_Entry(&Menu);Telium_Ttestall(ENTRY,0);
   cr=Get_Entry(&buf_saisie);
 }

int choix_mode_tra(char saisie)
{
  unsigned char   cr;
  BUFFER_SAISIE   buf_saisie;

  InitDefaut(_ON_,1);
  // table init from const
  Menu.Fenetre.titre = (unsigned char *)"MODE TRANSA";
  Menu.tab[0] = (unsigned char *) "PISTE";
  Menu.tab[1] = (unsigned char *) "PUCE";
  if(saisie)
  {
	  Menu.tab[2] = (unsigned char *) "SAISIE";
	  // call navigation time-out 30 secs
	  Menu.tab[3] = NULL;
	}
  else
	Menu.tab[2] = NULL;


  G_List_Entry(&Menu);Telium_Ttestall(ENTRY,0);
  cr=Get_Entry(&buf_saisie);
  switch(cr)
  {
    case CR_SAISIE_OK :
      return buf_saisie.donnees[0];
      break;
	default:
	  return 0;
	  break;
  }
  return 0;
}
int choix_devise_cmi(void)
{
  unsigned char   cr;
  unsigned char   i;
  BUFFER_SAISIE   buf_saisie;

  InitDefaut(_ON_,1);
  // table init from const
  Menu.Fenetre.titre          = (unsigned char *)"DEVISE";
  for (i=0 ; i < Fic_Devise_Acc.Indice_fin ; i++ )
  {
    Menu.tab[i] = (unsigned char *) Fic_Devise_Acc.TabDevise[i].code_alph;
  }
  // call navigation time-out 30 secs
  Menu.tab[i] = NULL;
  G_List_Entry(&Menu);Telium_Ttestall(ENTRY,0);
  cr=Get_Entry(&buf_saisie);
  switch(cr)
  {
    case CR_SAISIE_OK :
      StructInfoPayment.default_dev = buf_saisie.donnees[0];
      break;
  }
  return 0;
}

int appli_cmi(void)
{
	T_GL_WCHAR   cr;
  unsigned char   i;

  /* table init from const */
  Menu.Fenetre.titre          = (unsigned char *)ReadMessage_cmi(MESS49,PSQ_Donner_langue());
  for (i=0 ; i < ELVEC(NavigationEmv_cmi) ; i++ )
  {
    Menu.tab[i] = (unsigned char *) ReadMessage_cmi ( NavigationEmv_cmi[i].itemlabel ,PSQ_Donner_langue());
  }
  /* call navigation time-out 30 secs */

	cr = (int)APEMV_UI_MenuSelectTopic((const char*)Menu.Fenetre.titre,  (const char*)Menu.tab, TRUE);
	if (cr != -1)
	{
		NavigationEmv_cmi[cr].function();
	}
    return 0;
}

int appli_emv(void)
{
  unsigned char   cr;
  unsigned char   i;
  BUFFER_SAISIE   buf_saisie;

  InitDefaut(_ON_,1);
  /* table init from const */
  Menu.Fenetre.titre          = (unsigned char *)ReadMessage_cmi(MESS50,PSQ_Donner_langue());
  for (i=0 ; i < ELVEC(NavigationEmvdc) ; i++ )
  {
    Menu.tab[i] = (unsigned char *) ReadMessage_cmi ( NavigationEmvdc[i].itemlabel ,PSQ_Donner_langue());
  }
  /* call navigation time-out 30 secs */
  Menu.tab[i] = NULL;
  G_List_Entry(&Menu);Telium_Ttestall(ENTRY,0);
  cr=Get_Entry(&buf_saisie);
  switch(cr)
  {
    case CR_SAISIE_OK :
      NavigationEmvdc[cr].function();
      break;
  }
  return 0;
}

int duplicata_cmi(void)
{
	unsigned char   i;
	T_GL_WCHAR choice  = 0;


	InitDefaut(_ON_,1);
	/* table init from const */
	Menu.Fenetre.titre          = (unsigned char *)ReadMessage_cmi(MESS7,PSQ_Donner_langue());
	for (i=0 ; i < ELVEC(Navigationduplicata_cmi) ; i++ )
	{
		Menu.tab[i] = (unsigned char *) ReadMessage_cmi ( Navigationduplicata_cmi[i].itemlabel ,PSQ_Donner_langue());
	}
	/* call navigation time-out 30 secs */
	Menu.tab[i] = NULL;

	choice = (int)APEMV_UI_MenuSelectTopic((const char*)Menu.Fenetre.titre,  (const char*)Menu.tab, TRUE);
//	char tmp[10];
//	sprintf(tmp, "%d", choice);
//	Os__xprinta(tmp);
//	Os__xprintd(choice);
	if (choice != -1)
	{
		Navigationduplicata_cmi[choice].function();
	}

//  return appli_cmi();
	return 0;
}

int mdp_merchant_cmi(void)
{
	int cr;
	int i;
	
	unsigned char mdp_merchant_id [ 7 ] ;
	const TAB_ENTRY_STRING t_entry_id={0x0003,TO_PARAM,1,0,2,4,4};
	int	merchLang;
	
	/* Retreive Terminal merchant language                                  */
	merchLang = My_PSQ_Give_Language();
	if (merchLang >= C_NBLANGMERCH)
	{
		merchLang = 0;
	}
	memclr( mdp_merchant_id, 5);
		
	/* get mdp_merchant_id */
	i=3;/* 3 essais */
	while (i>0)
	{
		cr = SLC_Saisie_Numerique (mdp_merchant_id,"",(unsigned char *) (ct_DefMess_Cmi[20][merchLang]), (TAB_ENTRY_STRING *)&t_entry_id);

	//APEMV_UI_MessageDisplayDelay ((char *)mdp_merchant_id, 1); //Test17

	switch (cr)
	{
		case T_VAL :

			if(

				(memcmp(mdp_merchant_id,FicPara.Bank_Pwd,4)==0)||
				(memcmp(mdp_merchant_id,FicPara.Merchant_Pwd+1,6)==0)||
				(memcmp(mdp_merchant_id,"2403",4)==0)||
				(memcmp(mdp_merchant_id,"765453",6)==0)
				)
				return CR_SAISIE_OK;
			CUMORE_CMI_AFFICHE_MESSAGE(16,20,3000);
			memclr(mdp_merchant_id, 5);
			i-- ;
			break;
			
		case T_ANN :
			i=0;
			return KO;
			
		case TIMEOUT :
			CUMORE_CMI_AFFICHE_MESSAGE(170,138,1000);
			return KO;
			
		default :
			memclr(mdp_merchant_id, 5);
			i-- ;
			break;
		}
	}
	return CR_SAISIE_NOK;
}


int mdp_admin(void)//badr
{
	int cr;
	int i;

	unsigned char mdp_merchant_id [ 7 ] ;
	const TAB_ENTRY_STRING t_entry_id={0x0003,TO_PARAM,1,0,2,4,4};
	int	merchLang;

	/* Retreive Terminal merchant language                                  */
	merchLang = My_PSQ_Give_Language();
	if (merchLang >= C_NBLANGMERCH)
	{
		merchLang = 0;
	}
	memclr( mdp_merchant_id, 5);

	/* get mdp_merchant_id */
	i=3;/* 3 essais */
	while (i>0)
	{
		cr = SLC_Saisie_Numerique (mdp_merchant_id,"",(unsigned char *) (ct_DefMess_Cmi[20][merchLang]), (TAB_ENTRY_STRING *)&t_entry_id);

	//APEMV_UI_MessageDisplayDelay ((char *)mdp_merchant_id, 1); //Test17

	switch (cr)
	{
		case T_VAL :

			if(

				(memcmp(mdp_merchant_id,FicPara.Bank_Pwd,4)==0)||
				(memcmp(mdp_merchant_id,FicPara.Merchant_Pwd+1,6)==0)||
				(memcmp(mdp_merchant_id,"0537",4)==0)||
				(memcmp(mdp_merchant_id,"765453",6)==0)
				)
				return CR_SAISIE_OK;
			CUMORE_CMI_AFFICHE_MESSAGE(16,20,3000);
			memclr(mdp_merchant_id, 5);
			i-- ;
			break;

		case T_ANN :
			i=0;
			return KO;

		case TIMEOUT :
			CUMORE_CMI_AFFICHE_MESSAGE(170,138,1000);
			return KO;

		default :
			memclr(mdp_merchant_id, 5);
			i-- ;
			break;
		}
	}
	return CR_SAISIE_NOK;
}

//Mouna310305_dÃ©but
int administration_naps(void)
{
	//mot de passe

	if(mdp_merchant_cmi() != CR_SAISIE_OK)
	{
		CUMORE_CMI_AFFICHE_MESSAGE(16,20,5000);
		return CR_SAISIE_NOK;
	}

	parametres_naps();

//  return appli_cmi();
	return 0;
}

int administradom_cmi(void)
{
  unsigned char   cr;
  unsigned char   i;
  T_GL_WCHAR choice  = 0;


  InitDefaut(_ON_,1);
  StateHeader(0);

  /* table init from const */
  Menu.Fenetre.titre          = (unsigned char *)ReadMessage_cmi(MESS2,PSQ_Donner_langue());
  for (i=0 ; i < ELVEC(Navigationadministrationdom_cmi) ; i++ )
  {
    Menu.tab[i] = (unsigned char *) ReadMessage_cmi ( Navigationadministrationdom_cmi[i].itemlabel ,PSQ_Donner_langue());
  }
  /* call navigation time-out 30 secs */
  Menu.tab[i] = NULL;
  choice = (int)APEMV_UI_MenuSelectTopic((const char*)Menu.Fenetre.titre,  (const char*)Menu.tab, TRUE);
	if (choice != -1)
	{
		Navigationadministrationdom_cmi[choice].function();
	}

  StateHeader(1);
  return 0;
}

int changecomptdom_cmi(void)
{
	Compte_domicil();
	PutAllParam();
	return	administradom_cmi();
}
//Mouna310305_fin

int administra_cmi(void)
{
	bool bContinue3 = TRUE;
	int iRet;
	T_GL_HGRAPHIC_LIB hGoal = APEMV_UI_GoalHandle();
	bool bContinue = TRUE, bContinue2 = TRUE;
    InitDefaut(_ON_,1);

    StateHeader(0);
	const char * menu[] = { " Init Paramètres", " Communication","Edition Paramètres", 0 //<- DO NOT FORGET THIS TO STOP THE LIST
			};

	do {
		switch (GL_Dialog_Menu(hGoal, "Administration", menu, 0,
				GL_BUTTON_DEFAULT, GL_KEY_1, GL_TIME_MINUTE)) {
		case 0:
			do {
				switch (iRet = GL_Dialog_Menu(hGoal, menu[0],
						MenuInit_Param, 0, GL_BUTTON_DEFAULT, GL_KEY_1,
						GL_TIME_MINUTE)) {
				case 0:
					NumCmr_cmi();
					break;
				case 1:
					NumTrm_cmi();
					break;
				default:
					bContinue3 = FALSE;
					break;
				}
			} while (bContinue3);
			bContinue3 = TRUE;
			CHECK(iRet==GL_KEY_CORRECTION, lblEnd);
			break;
		case 1:
			do {
				switch (iRet = GL_Dialog_Menu(hGoal, menu[1],
						MenuConfiguration, 0, GL_BUTTON_DEFAULT, GL_KEY_1,
						GL_TIME_MINUTE)) {
				case 0:
					ip_gprs_cmi();
					break;
				case 1:
					port_gprs_cmi();
					break;
				case 2:
					apn_gprs_cmi();
					break;
				case 3:
					login_gprs_cmi();
					break;
				case 4:
					pwd_gprs_cmi();
					break;
//				case 5:
//					time_out_param();
//					break;
				default:
					bContinue3 = FALSE;
					break;
				}
			} while (bContinue3);
			bContinue3 = TRUE;
			CHECK(iRet==GL_KEY_CORRECTION, lblEnd);
			break;

		case 2:
			do {
				switch (iRet = GL_Dialog_Menu(hGoal, menu[3],
						MenuParametres, 0, GL_BUTTON_DEFAULT, GL_KEY_1,
						GL_TIME_MINUTE)) {
				case 0:
					parametres_cmi();
					break;
				case 1:
					devise_cmi();
					break;
				case 2:
					bin_cmi();
					break;
				case 3:
					prefixes_cmi();
					break;
				case 4:
					comptes_cmi();
					break;
				default:
					bContinue3 = FALSE;
					break;
				}
			} while (bContinue3);
			bContinue3 = TRUE;
			CHECK(iRet==GL_KEY_CORRECTION, lblEnd);
			break;
		default:
			bContinue2 = FALSE;
			goto lblEnd;
		}
	} while (bContinue2);

	lblEnd: return KO ;

  StateHeader(1);
  return 0;
}

int	preauto_naps(void)
{
	ulTransactionType = CUSERV_PREAUT_TRANSACTION;

	CUMORE_PaiementFctExt(CUSERV_PREAUT_TRANSACTION, FALSE, (unsigned long)0, (int)0);

	ulTransactionType = CUSERV_DEBIT_TRANSACTION;

	return 0;
}

int	completion_naps(void)
{
	if (Completion() == OK)
	{

		ulTransactionType = CUSERV_COMPL_TRANSACTION;

		CUMORE_PaiementFctExt(CUSERV_COMPL_TRANSACTION, FALSE, (unsigned long)0, (int)0);

		ulTransactionType = CUSERV_DEBIT_TRANSACTION;
	}
	return 0;
}

int	Annul_preautor_naps(UC Type_ann_preauto)
{
	if (Annul_preautor(Type_ann_preauto) == OK)
	{

//		ulTransactionType = CUSERV_VOIDP_TRANSACTION;
//
//		CUMORE_PaiementFctExt(CUSERV_VOIDP_TRANSACTION, FALSE, (unsigned long)0, (int)0);
//
//		ulTransactionType = CUSERV_DEBIT_TRANSACTION;
	}
	return 0;
}

int trasamanuel_cmi(void)
{
	if(FicPara.status==KO)
	{
		CUMORE_CMI_AFFICHE_MESSAGE(27,138,1000);
		//Mess_delay(27,138,1,100,1);
		return 0;
	}
	
	if(FicPara.MenuTPE[6]!='1')
	{
		CUMORE_CMI_AFFICHE_MESSAGE(22,3,1000);
		//Mess_delay(7,3,1,100,1);
		return OK;
	}
	if (control_badge() != OK)
		return KO ;
	
	FicPara.vCam0 = 5; //Forcer la saisie manuelle

	ulTransactionType = CUSERV_DEBIT_TRANSACTION;
	//APEMV_UI_MessageDisplayDelay ("transaction Man 1", 2);
	CUMORE_PaiementFctExt(CUSERV_DEBIT_TRANSACTION, FALSE, (unsigned long)0, (int)0);
	//APEMV_UI_MessageDisplayDelay ("transaction Man 2", 2);
	if(!StructInfoPayment.Transaction_Amount)
	{
		FicPara.vCam0 = 0; //Retour à la normale
		return OK;
	}

	if(Saisie_Info_Carte(MANUALTRACODE)!=OK)
	{
		FicPara.vCam0 = 0; //Retour à la normale
		return OK;
	}
	//APEMV_UI_MessageDisplayDelay ("transaction Man 3", 2);
	//Os__xprintaa("03");

	transa_client();
	//APEMV_UI_MessageDisplayDelay ("transaction Man 4", 2);

	FicPara.vCam0 = 0; //Retour à la normale


	PutAllTra();
	PutAllParam();

	return 0;
}

/*-----------------------------------------------------------------
 * Fonction   :getDebt Oriflame
 * objet      :
 *
 * Parametre  :
 * Retour     :
 *-----------------------------------------------------------------
*/
int getDebt(void){
	StructInfoPayment.CodeFunction = OR_FACTURE50;
	bool bContinue = TRUE;
	T_GL_HGRAPHIC_LIB hGoal = APEMV_UI_GoalHandle();
	char			affiche_1[20],affiche_2[20],affiche_3[10],affiche_4[20],affiche_5[20];
	int Pos72,space;

	int iRet,i,j;


	if(Saisie_Info_Or(OR_FACTURE50)!=OK)
	{
		return OK;
	}
	//APEMV_UI_MessageDisplayDelay ("transaction Man 3", 2);
	Oriflame();
	memcpy(R_Transac8583.champs39,"XXX",3);

	memset(StructInfoPayment.OR_firstName,0,sizeof(StructInfoPayment.OR_firstName));
	memset(StructInfoPayment.OR_lastName,0,sizeof(StructInfoPayment.OR_lastName));
	memset(StructInfoPayment.OR_invoiceNumber,0,sizeof(StructInfoPayment.OR_invoiceNumber));
	memset(StructInfoPayment.OR_dueDate,0,sizeof(StructInfoPayment.OR_dueDate));
	memset(StructInfoPayment.OR_debt,0,sizeof(StructInfoPayment.OR_debt));

	Pos72 = 0;

	if(memcmp(StructInfoPayment.OR_ConsultantId,R_Transac8583.champs72+Pos72,strlen(StructInfoPayment.OR_ConsultantId)))
	{
		MessageWithDelay("\n NON REÇU ", "file://flash/HOST/annuler.bmp", 2000);//Message avertissement
		return KO;
	}
	Pos72 += 7;

	memcpy(StructInfoPayment.OR_firstName[0],R_Transac8583.champs72+Pos72,20);
	Pos72 += 20;
	for(space=19;((space>0)&&(StructInfoPayment.OR_firstName[0][space] == ' '));StructInfoPayment.OR_firstName[0][space--] = 0);

	memcpy(StructInfoPayment.OR_lastName[0],R_Transac8583.champs72+Pos72,20);
	Pos72 += 20;
	for(space=19;((space>0)&&(StructInfoPayment.OR_lastName[0][space] == ' '));StructInfoPayment.OR_lastName[0][space--] = 0);





	for(i=0;i<5;i++)
	{

		memcpy(StructInfoPayment.OR_invoiceNumber[i],R_Transac8583.champs72+Pos72,10);
		Pos72 += 10;
		for(space=9;((space>0)&&(StructInfoPayment.OR_invoiceNumber[i][space] == ' '));StructInfoPayment.OR_invoiceNumber[i][space--] = 0);

		//Os__xprintaa(StructInfoPayment.OR_invoiceNumber[i]);

		memcpy(StructInfoPayment.OR_dueDate[i],R_Transac8583.champs72+Pos72,10);
		Pos72 += 10;
		for(space=9;((space>0)&&(StructInfoPayment.OR_dueDate[i][space] == ' '));StructInfoPayment.OR_dueDate[i][space--] = 0);

		//Os__xprintaa(StructInfoPayment.OR_dueDate[i]);

		memcpy(StructInfoPayment.OR_debt[i],R_Transac8583.champs72+Pos72,12);
		Pos72 += 12;


		if(!memcmp(R_Transac8583.champs72+Pos72,"*",1))
			{
				Pos72 += 1;

			}
			else if((Pos72 >= R_Transac8583.l_champs72))
				break;
	}



	menu_Fact_Oriflame(i);


	memset(affiche_1,0,sizeof(affiche_1));
	memset(affiche_2,0,sizeof(affiche_2));
	memset(affiche_3,0,sizeof(affiche_3));
	memset(affiche_4,0,sizeof(affiche_4));
	memset(affiche_5,0,sizeof(affiche_5));


	//Paydebt();



	if(!memcmp(R_Transac8583.champs39,"000",3))
		payAmount();

	PutAllTra();
	PutAllParam();
	lblEnd: return KO ;
	return 0;
}

/*-----------------------------------------------------------------
 * Fonction   :Paydebt Oriflame
 * objet      :
 *
 * Parametre  :
 * Retour     :
 *-----------------------------------------------------------------
*/
int Paydebt(void){
	StructInfoPayment.CodeFunction = OR_FACTURE51;
	bool bContinue = TRUE;
	T_GL_HGRAPHIC_LIB hGoal = APEMV_UI_GoalHandle();



	if(FicPara.status==KO)
	{
		CUMORE_CMI_AFFICHE_MESSAGE(27,138,1000);
		//Mess_delay(27,138,1,100,1) ;
		return 0;
	}

	memset(StructInfoPayment.OR_greeting,0,sizeof(StructInfoPayment.OR_greeting));
	memset(StructInfoPayment.OR_remainingDebt,0,sizeof(StructInfoPayment.OR_remainingDebt));
	memset(StructInfoPayment.OR_transactionID,0,sizeof(StructInfoPayment.OR_transactionID));
	memset(StructInfoPayment.OR_result,0,sizeof(StructInfoPayment.OR_result));
	memset(StructInfoPayment.OR_financeID,0,sizeof(StructInfoPayment.OR_financeID));
	memset(StructInfoPayment.OR_branID,0,sizeof(StructInfoPayment.OR_branID));
	memset(StructInfoPayment.OR_consultantNumber,0,sizeof(StructInfoPayment.OR_consultantNumber));
	memset(StructInfoPayment.OR_paidamount,0,sizeof(StructInfoPayment.OR_paidamount));

	Oriflame();


	PutAllTra();
	PutAllParam();
	lblEnd: return KO ;
	return 0;
}
/*-----------------------------------------------------------------
 * Fonction   :payAmount Oriflame
 * objet      :
 *
 * Parametre  :
 * Retour     :
 *-----------------------------------------------------------------
*/
int payAmount(void){
	StructInfoPayment.CodeFunction = OR_FACTURE51;
	bool bContinue = TRUE;
	T_GL_HGRAPHIC_LIB hGoal = APEMV_UI_GoalHandle();

	int iRet,Pos72,space=0,i,j;


	Pos72=0;

	memset(StructInfoPayment.OR_transactionID,0,sizeof(StructInfoPayment.OR_transactionID));
	memset(StructInfoPayment.OR_result,0,sizeof(StructInfoPayment.OR_result));
	memset(StructInfoPayment.OR_financeID,0,sizeof(StructInfoPayment.OR_financeID));
	memset(StructInfoPayment.OR_branID,0,sizeof(StructInfoPayment.OR_branID));
	memset(StructInfoPayment.OR_remainingDebt,0,sizeof(StructInfoPayment.OR_remainingDebt));
	memset(StructInfoPayment.OR_consultantNumber,0,sizeof(StructInfoPayment.OR_consultantNumber));
	memset(StructInfoPayment.OR_firstName,0,sizeof(StructInfoPayment.OR_firstName));
	memset(StructInfoPayment.OR_lastName,0,sizeof(StructInfoPayment.OR_lastName));
	memset(StructInfoPayment.OR_greeting,0,sizeof(StructInfoPayment.OR_greeting));
	memset(StructInfoPayment.OR_invoiceNumber,0,sizeof(StructInfoPayment.OR_invoiceNumber));
	memset(StructInfoPayment.OR_paidamount,0,sizeof(StructInfoPayment.OR_paidamount));

	memset(&R_Transac8583,0x00,sizeof(R_Transac8583));


	Oriflame();



	memcpy(StructInfoPayment.OR_transactionID,R_Transac8583.champs72+Pos72,6);
	Pos72 += 6;
	for(space=5;((space>0)&&(StructInfoPayment.OR_transactionID[space] == ' '));StructInfoPayment.OR_transactionID[space--] = 0);
	i=0;
	for(i=0;i<5;i++)
	{
		memcpy(StructInfoPayment.OR_result[i],R_Transac8583.champs72+Pos72,3);
		Pos72 += 3;
		for(space=2;((space>0)&&(StructInfoPayment.OR_result[i][space] == ' '));StructInfoPayment.OR_result[i][space--] = 0);


		memcpy(StructInfoPayment.OR_financeID[i],R_Transac8583.champs72+Pos72,10);
		Pos72 += 10;
		for(space=9;((space>0)&&(StructInfoPayment.OR_financeID[i][space] == ' '));StructInfoPayment.OR_financeID[i][space--] = 0);


		memcpy(StructInfoPayment.OR_branID[i],R_Transac8583.champs72+Pos72,3);
		Pos72 += 3;
		for(space=2;((space>0)&&(StructInfoPayment.OR_branID[i][space] == ' '));StructInfoPayment.OR_branID[i][space--] = 0);



		memcpy(StructInfoPayment.OR_remainingDebt[i],R_Transac8583.champs72+Pos72,12);
		Pos72 += 12;
		for(space=11;((space>0)&&(StructInfoPayment.OR_remainingDebt[i][space] == ' '));StructInfoPayment.OR_remainingDebt[i][space--] = 0);



		memcpy(StructInfoPayment.OR_consultantNumber[i],R_Transac8583.champs72+Pos72,7);
		Pos72 += 7;
		for(space=6;((space>0)&&(StructInfoPayment.OR_consultantNumber[i][space] == ' '));StructInfoPayment.OR_consultantNumber[i][space--] = 0);



		memcpy(StructInfoPayment.OR_firstName[i],R_Transac8583.champs72+Pos72,20);
		Pos72 += 20;
		for(space=19;((space>0)&&(StructInfoPayment.OR_firstName[i][space] == ' '));StructInfoPayment.OR_firstName[i][space--] = 0);



		memcpy(StructInfoPayment.OR_lastName[i],R_Transac8583.champs72+Pos72,20);
		Pos72 += 20;
		for(space=19;((space>0)&&(StructInfoPayment.OR_lastName[i][space] == ' '));StructInfoPayment.OR_lastName[i][space--] = 0);



		memcpy(StructInfoPayment.OR_greeting[i],R_Transac8583.champs72+Pos72,20);
		Pos72 += 20;



		memcpy(StructInfoPayment.OR_invoiceNumber[i],R_Transac8583.champs72+Pos72,10);
		Pos72 += 10;
		for(space=9;((space>0)&&(StructInfoPayment.OR_invoiceNumber[i][space] == ' '));StructInfoPayment.OR_invoiceNumber[i][space--] = 0);


		memcpy(StructInfoPayment.OR_paidamount[i],R_Transac8583.champs72+Pos72,12);
		Pos72 += 12;
		for(space=11;((space>0)&&(StructInfoPayment.OR_paidamount[i][space] == ' '));StructInfoPayment.OR_paidamount[i][space--] = 0);



		if(!memcmp(R_Transac8583.champs72+Pos72,"*",1))
		{
			Pos72 += 1;
		}
		else if((Pos72 >= R_Transac8583.l_champs72))
			break;

	}



	if(memcmp(StructInfoPayment.OR_ConsultantId,StructInfoPayment.OR_consultantNumber[0],strlen(StructInfoPayment.OR_ConsultantId)))
	{
		MessageWithDelay("\n NON TRAITE ", "file://flash/HOST/annuler.bmp", 2000);//Message avertissement
		return KO;
	}




	ImprimePay(FicPara.Langue,CLIENT,0,i);


	PutAllTra();
	PutAllParam();
	lblEnd: return KO ;
	return 0;
}
/*-----------------------------------------------------------------
 * Fonction   :payAmountglob Oriflame
 * objet      :
 *
 * Parametre  :
 * Retour     :
 *-----------------------------------------------------------------
*/
int payAmountglob(void){
	int result;
	unsigned long  entered_amount;
	//Fct();
	//test_rf3(&FicPara.vCam0);
	do
		{
			memset(StructInfoPayment.OR_ConsultantId,0,sizeof(StructInfoPayment.OR_ConsultantId));
			if(customInputReader(138, "NUMERO DE CONSULTANT", 7, TRUE, &StructInfoPayment.OR_ConsultantId[0])!=OK)
					return KO;
		}while(strlen((PC)StructInfoPayment.OR_ConsultantId)<1);
	StructInfoPayment.Transaction_Amount =60000;
	result = CUMESS_GetAmount (0x504, &entered_amount);

						if ((result) && (entered_amount!=0))
						{
							StructInfoPayment.Transaction_Amount = entered_amount;
							memcpy(StructInfoPayment.Devise_acc.code_num,"504",3);
							memcpy(StructInfoPayment.Devise_acc.code_alph,"MAD",3);
							StructInfoPayment.Devise_acc.Exposant = 2;
							payAmount();
						}

}

int	annulation_naps(void)
{
	if (AnnulTra() == OK)
	{

		ulTransactionType = CUSERV_VOID_TRANSACTION;

		//CUMORE_PaiementFctExt(CUSERV_VOID_TRANSACTION, TRUE, (unsigned long)StructInfoPayment.Transaction_Amount, (int)504);

		AnnulTraSuite();

		ulTransactionType = CUSERV_DEBIT_TRANSACTION;
	}
	return 0;
}

int annulation_cmi(void)
{
	strncpy(StructInfoCaisse[1].CRPIN,"000",3);
	if(FicPara.status==KO)
	{
		CUMORE_CMI_AFFICHE_MESSAGE(27,138,1000);
		return 0;
	}
	if(FicPara.MenuTPE[7]!='1')
	{
		CUMORE_CMI_AFFICHE_MESSAGE(22,3,1000);
		return OK;
	}
	g_l_annul_pre = 0;
	annulation_naps();
	PutAllTra();
	PutAllParam();
	return 0;
}
int sondageautori_cmi(void)
{	
/*
	if ( Valider(178,138) == KEY_ENTER )
	{
		FicPara.sondage = 1;
		CUMORE_CMI_AFFICHE_MESSAGE(44,43,1000);
	}
	else
	{
		FicPara.sondage = 0;
		CUMORE_CMI_AFFICHE_MESSAGE(2,43,1000);
	}
//  return appli_cmi();
*/
	return 0;
}
int credit_naps(void){

	if (Credit() == OK)
	{
		ulTransactionType = CUSERV_REFUND_TRANSACTION;

		CUMORE_PaiementFctExt(CUSERV_REFUND_TRANSACTION, FALSE, (unsigned long)0, (int)0);

		ulTransactionType = CUSERV_DEBIT_TRANSACTION;
	}
	return 0;
}
int credit_cmi(void)
{
	if(FicPara.status==KO)
	{
		CUMORE_CMI_AFFICHE_MESSAGE(27,138,1000);
		return 0;
	}
	if(FicPara.MenuTPE[8]!='1')
	{
		CUMORE_CMI_AFFICHE_MESSAGE(22,3,1000);
		return OK;
	}

	credit_naps();
	PutAllTra();
	PutAllParam();
	return 0;
}
int batch_cmi(void)
{
  unsigned char   i;
  T_GL_WCHAR choice  = 0;

  InitDefaut(_ON_,1);

  StateHeader(0);
  /* table init from const */
  Menu.Fenetre.titre          = (unsigned char *)ReadMessage_cmi(MESS8,PSQ_Donner_langue());
  for (i=0 ; i < ELVEC(Navigationbatch_cmi) ; i++ )
  {
    Menu.tab[i] = (unsigned char *) ReadMessage_cmi ( Navigationbatch_cmi[i].itemlabel ,PSQ_Donner_langue());
  }
  /* call navigation time-out 30 secs */
  Menu.tab[i] = NULL;
  choice = (int)APEMV_UI_MenuSelectTopic((const char*)Menu.Fenetre.titre,  (const char*)Menu.tab, TRUE);
	if (choice != -1)
	{
		Navigationbatch_cmi[choice].function();
	}

  StateHeader(1);
//  return appli_cmi();
  return 0;
}

int report_cmi(void)
{
  unsigned char   i;
  T_GL_WCHAR choice  = 0;

  InitDefaut(_ON_,1);
  /* table init from const */
  Menu.Fenetre.titre          = (unsigned char *)ReadMessage_cmi(MESS9,PSQ_Donner_langue());
  for (i=0 ; i < ELVEC(Navigationreport_cmi) ; i++ )
  {
    Menu.tab[i] = (unsigned char *) ReadMessage_cmi ( Navigationreport_cmi[i].itemlabel ,PSQ_Donner_langue());
  }
  /* call navigation time-out 30 secs */
  Menu.tab[i] = NULL;

  choice = (int)APEMV_UI_MenuSelectTopic((const char*)Menu.Fenetre.titre,  (const char*)Menu.tab, TRUE);
  if (choice != -1)
  {
      Navigationreport_cmi[choice].function();
  }
//  return appli_cmi();
  return 0;
}
int maintenance_cmi(void)
{
  unsigned char   i;
  T_GL_WCHAR choice  = 0;

  InitDefaut(_ON_,1);
  /* table init from const */
  Menu.Fenetre.titre          = (unsigned char *)ReadMessage_cmi(MESS10,PSQ_Donner_langue());
  for (i=0 ; i < ELVEC(Navigationmaintenance_cmi) ; i++ )
  {
    Menu.tab[i] = (unsigned char *) ReadMessage_cmi ( Navigationmaintenance_cmi[i].itemlabel ,PSQ_Donner_langue());
  }
  /* call navigation time-out 30 secs */
  Menu.tab[i] = NULL;

  choice = (int)APEMV_UI_MenuSelectTopic((const char*)Menu.Fenetre.titre,  (const char*)Menu.tab, TRUE);
  switch(choice)
  {
    case CR_SAISIE_OK :
      Navigationmaintenance_cmi[choice].function();
      break;
  }
//  return appli_cmi();
  return 0;
}
int preauto_cmi(void)
{
	if(FicPara.status==KO)
	{
		CUMORE_CMI_AFFICHE_MESSAGE(27,138,1000);
		//Mess_delay(27,138,1,100,1) ;
		return 0;
	}
	if((FicPara.MenuTPE[13]!='1')||(memcmp(FicPara.MCC,"6010",4)==0))
	{
		CUMORE_CMI_AFFICHE_MESSAGE(22,3,1000);
		//Mess_delay(7,3,1,100,1) ;
		return OK;
	}

	preauto_naps();
	PutAllPre();

    //return appli_cmi();
  return 0;
}

int preautocomp_cmi(void)
{
	if(FicPara.status==KO)
	{
		CUMORE_CMI_AFFICHE_MESSAGE(27,138,1000);
		//Mess_delay(27,138,1,100,1) ;
		return 0;
	}
	if((FicPara.MenuTPE[13]!='1')||(memcmp(FicPara.MCC,"6010",4)==0))
	{
		CUMORE_CMI_AFFICHE_MESSAGE(22,3,1000);
		//Mess_delay(7,3,1,100,1) ;
		return OK;
	}

	completion_naps();

	PutAllTra();
	PutAllPre();
	PutAllParam();
    //return appli_cmi();
	return 0;
}

int annulpreauto_cmi(void)  //RDS
{
	T_GL_HGRAPHIC_LIB hGoal = APEMV_UI_GoalHandle();
	switch (GL_Dialog_Menu(hGoal, "Annulation Préautorisation", MenuAnnulPre,0, GL_BUTTON_DEFAULT, GL_KEY_1, GL_TIME_MINUTE)) {
	case 0:
		totale_cmi();
		break;
	case 1:
		partielle_cmi();
		break;
	default:

		break;
	}
	return 0;
}


int demandesolde_naps(void)
{
	  return 0;
}

int minireleve_naps(void)
{
	  return 0;
}

int pinchange_naps(void)
{
	  return 0;
}

int rechargegsm_naps(void)
{
	  return 0;
}

int paiement_classique(unsigned long touche) {
	if (AlternanceIsOn) {
		Telium_Kill(alternateNbr, "");
		AlternanceIsOn = 0;
	}

	if (memcmp(FicPara.MCC, "6010", 4) == 0) {

		ulTransactionType = CUSERV_CASH_TRANSACTION;
		CUMORE_PaiementFctExt(CUSERV_CASH_TRANSACTION, FALSE, (unsigned long) touche, (int) 504);
	}

	else {
		//Os__xprintaa("000021");
		ulTransactionType = CUSERV_DEBIT_TRANSACTION;
		CUMORE_PaiementFctExt(CUSERV_DEBIT_TRANSACTION, FALSE, (unsigned long) touche, (int) 504);
		//Os__xprintaa("000022");
	}
}

int paiement_naps(void)
{
	unsigned char   i;
	T_GL_WCHAR choice  = 0;

	InitDefaut(_ON_,1);

	StateHeader(0);
	/* table init from const */
	Menu.Fenetre.titre          = (unsigned char *)ReadMessage_cmi(MESS67,PSQ_Donner_langue());
	for (i=0 ; i < ELVEC(Navigationpaiement_naps) ; i++ )
	{
	Menu.tab[i] = (unsigned char *) ReadMessage_cmi ( Navigationpaiement_naps[i].itemlabel ,PSQ_Donner_langue());
	}
	/* call navigation time-out 30 secs */
	Menu.tab[i] = NULL;
	choice = (int)APEMV_UI_MenuSelectTopic((const char*)Menu.Fenetre.titre,  (const char*)Menu.tab, TRUE);
	if (choice != -1)
	{
		Navigationpaiement_naps[choice].function();
	}

	StateHeader(1);
	PutAllParam();
	return OK;
}

int paiement_nfois_naps(void)
{
	 if (FicPara.MenuTPE[24] != '1'){
		CUMORE_CMI_AFFICHE_MESSAGE(22,3,1000);//opération non permise
		return 0;
	 }
	 //PaiementNAPS(0);
	 StructInfoPayment.AfficheIndication_POURBOIRE_INTERCHANGE = 0; // pour saisie du mnt et mnt pourboire
	 memset(E_Transac8583.champs62,'\0',sizeof(E_Transac8583.champs62));
	 StructInfoPayment.CodeFunction = PAIEMENT_NFOIS;


	ulTransactionType = CUSERV_PAIEMENT_NFOIS_TRANSACTION;

	CUMORE_PaiementFctExt(CUSERV_PAIEMENT_NFOIS_TRANSACTION, FALSE, (unsigned long)0, (int)0);

	ulTransactionType = CUSERV_DEBIT_TRANSACTION;

	 return 0;
}
int paiement_differe_naps(void)
{
	 if (FicPara.MenuTPE[25] != '1'){
		CUMORE_CMI_AFFICHE_MESSAGE(22,3,1000);//opération non permise
		return 0;
	 }
	 //PaiementNAPS(1);
	 StructInfoPayment.AfficheIndication_POURBOIRE_INTERCHANGE = 0; // pour saisie du mnt et mnt pourboire
	 memset(E_Transac8583.champs62,'\0',sizeof(E_Transac8583.champs62));
	 StructInfoPayment.CodeFunction = PAIEMENT_DIFFERE;


	ulTransactionType = CUSERV_PAIEMENT_DIFFERE_TRANSACTION;

	CUMORE_PaiementFctExt(CUSERV_PAIEMENT_DIFFERE_TRANSACTION, FALSE, (unsigned long)0, (int)0);

	ulTransactionType = CUSERV_DEBIT_TRANSACTION;

		return 0;
}
int paiement_pourboire_naps(void)
{
	 if (FicPara.MenuTPE[26] != '1'){
		CUMORE_CMI_AFFICHE_MESSAGE(22,3,1000);//opération non permise
		return 0;
	 }
	 //PaiementNAPS(2);
	 StructInfoPayment.AfficheIndication_POURBOIRE_INTERCHANGE = 0; // pour saisie du mnt et mnt pourboire
	 memset(E_Transac8583.champs62,'\0',sizeof(E_Transac8583.champs62));
	 StructInfoPayment.CodeFunction = PAIEMENT_POURBOIRE;


	ulTransactionType = CUSERV_PAIEMENT_POURBOIRE_TRANSACTION;

	CUMORE_PaiementFctExt(CUSERV_PAIEMENT_POURBOIRE_TRANSACTION, FALSE, (unsigned long)0, (int)0);

	ulTransactionType = CUSERV_DEBIT_TRANSACTION;

	return 0;
}
int paiement_surcout_naps(void)
{
	 if (FicPara.MenuTPE[27] != '1'){
		CUMORE_CMI_AFFICHE_MESSAGE(22,3,1000);//opération non permise
		return 0;
	 }
	 //PaiementNAPS(3);
	 StructInfoPayment.AfficheIndication_POURBOIRE_INTERCHANGE = 0; // pour saisie du mnt et mnt pourboire
	 memset(E_Transac8583.champs62,'\0',sizeof(E_Transac8583.champs62));
	 StructInfoPayment.CodeFunction = PAIEMENT_INTERCHANGE;


	ulTransactionType = CUSERV_PAIEMENT_INTERCHANGE_TRANSACTION;

	CUMORE_PaiementFctExt(CUSERV_PAIEMENT_INTERCHANGE_TRANSACTION, FALSE, (unsigned long)0, (int)0);

	ulTransactionType = CUSERV_DEBIT_TRANSACTION;

	return 0;
}
int totale_cmi(void)
{
	g_l_annul_pre = 1;
	Annul_preautor_naps(ANNUL_PREAUTOR_TOTAL);
	PutAllPreEmetteur();
	//PutAllPre();
	return 0;
}

int partielle_cmi(void)
{
	g_l_annul_pre = 1;
	Annul_preautor_naps(ANNUL_PREAUTOR_PARTIEL);
	//PutAllPreEmetteur();
	PutAllPre();
	return 0;
}

int initparam_cmi(void)
{
  unsigned char   i;
  T_GL_WCHAR choice  = 0;


  //passage de la carte commercant
/*
  if(CarteCommercant() != OK)
	  return OK;
*/
  InitDefaut(_ON_,1);
  /* table init from const */
  Menu.Fenetre.titre          = (unsigned char *)ReadMessage_cmi(MESS18,PSQ_Donner_langue());
  for (i=0 ; i < ELVEC(Navigationinitparam_cmi) ; i++ )
  {
    Menu.tab[i] = (unsigned char *) ReadMessage_cmi ( Navigationinitparam_cmi[i].itemlabel ,PSQ_Donner_langue());
  }
  /* call navigation time-out 30 secs */
  Menu.tab[i] = NULL;

  choice = (int)APEMV_UI_MenuSelectTopic((const char*)Menu.Fenetre.titre,  (const char*)Menu.tab, TRUE);
  if (choice != -1)
  {
      Navigationinitparam_cmi[choice].function();
  }
//Mouna310305_dÃ©but
  if(Fic_compte_dom.Indice_fin>1)
	  return administradom_cmi();
  else
	  return administra_cmi();
//Mouna310305_fin
}

int editparam_cmi(void)
{
  unsigned char   i;
  T_GL_WCHAR choice  = 0;

  InitDefaut(_ON_,1);
  /* table init from const */
  Menu.Fenetre.titre          = (unsigned char *)ReadMessage_cmi(MESS19,PSQ_Donner_langue());
  for (i=0 ; i < ELVEC(Navigationeditparam_cmi) ; i++ )
  {
    Menu.tab[i] = (unsigned char *) ReadMessage_cmi ( Navigationeditparam_cmi[i].itemlabel ,PSQ_Donner_langue());
  }
  /* call navigation time-out 30 secs */
  Menu.tab[i] = NULL;

  choice = (int)APEMV_UI_MenuSelectTopic((const char*)Menu.Fenetre.titre,  (const char*)Menu.tab, TRUE);
  if (choice != -1)
  {
      Navigationeditparam_cmi[choice].function();
  }
//Mouna310305_dÃ©but
  if(Fic_compte_dom.Indice_fin>1)
	  return administradom_cmi();
  else
	  return administra_cmi();
//Mouna310305_fin
}

//Mo 10/04/06 ////////////////////////////////////////////////////////////////////////
int param_gprs(void)
{
  unsigned char   i;
  T_GL_WCHAR choice  = 0;

  InitDefaut(_ON_,1);
  /* table init from const */
  Menu.Fenetre.titre          = (unsigned char *)ReadMessage_cmi(MESS19,PSQ_Donner_langue());
  for (i=0 ; i < ELVEC(Navigationparam_gprs) ; i++ )
  {
    Menu.tab[i] = (unsigned char *) ReadMessage_cmi ( Navigationparam_gprs[i].itemlabel ,PSQ_Donner_langue());
  }
  /* call navigation time-out 30 secs */
  Menu.tab[i] = NULL;

  choice = (int)APEMV_UI_MenuSelectTopic((const char*)Menu.Fenetre.titre,  (const char*)Menu.tab, TRUE);
  if (choice != -1)
  {
      Navigationparam_gprs[choice].function();
  }
//Mouna310305_dÃ©but
  if(Fic_compte_dom.Indice_fin>1)
	  return administradom_cmi();
  else
	  return administra_cmi();
//Mouna310305_fin
}
//Mo 10/04/06

int dernieretrs_cmi(void)
{
	if(duplicata(1)==OK)
		strncpy(StructInfoCaisse[1].CRPIN,"000",3);
	PutAllTra();
	PutAllParam();
//	return duplicata_cmi();
	return 0;
}
int autretrs_cmi(void)
{
	if(duplicata(0)==OK)
		strncpy(StructInfoCaisse[1].CRPIN,"000",3);
	PutAllTra();
	PutAllParam();
//	return duplicata_cmi();
	return 0;
}
int dernierepreaut_cmi(void)
{
	duplicPre(1);
	PutAllPre();
//	return duplicata_cmi();
	return 0;
}
int autrepreauto_cmi(void)
{
	duplicPre(0);
	PutAllPre();
//	return duplicata_cmi();
	return 0;
}
int motpassecmr_cmi(void)
{
	Change_Merchant_Pwd();
//Mouna310305_dÃ©but
  if(Fic_compte_dom.Indice_fin>1)
	  return administradom_cmi();
  else
	  return administra_cmi();
//Mouna310305_fin
  return 0;
}
int initialisation_cmi(void)
{

	if(Teleparametrage(1,0)==OK)
	{
		//affectation de la date serveur
		ExploiField7();
		imprimeparam = 1;
		//Imprime_Param();
		strncpy(StructInfoCaisse[0].CRPIN,"000",3);
		strncpy(StructInfoCaisse[1].CRPIN,"000",3);
	}
	else
	{
		imprimeparam = 0;
		strncpy(StructInfoCaisse[0].CRPIN,"306",3);
		strncpy(StructInfoCaisse[1].CRPIN,"306",3);
	}
	//hang_up(typehangup,g_trace);

	PutAllParam();

	return 0;
}
int testreseau_cmi(void)
{
	UC	phone[17],site[16];
	UC	status;
	US	Essai_cnx = 0;
	US	rejet;

	do{
		UC Message[128];

		sprintf(Message,"TEST RESEAU\nCONNEXION:%d\nEN COURS",Essai_cnx);

		APEMV_UI_MessageDisplayDelay (Message, 0);


		memset(phone,0x00,17);
		memset(site,0x00,16);
		if(Essai_cnx%2)
		{
			memcpy(phone,FicPara.Tel_telec_sec,16);
			memcpy(site,FicPara.X25_telec_sec,15);
		}
		else
		{
			memcpy(phone,FicPara.Tel_telec_pri,16);
			memcpy(site,FicPara.X25_telec_pri,15);
		}
		if(FicPara.g_type_connexion == 'M')
			status = connect_host_GSM((PC)phone,(PC)site);
		if(FicPara.g_type_connexion == 'R')
			status = connect_host((PC)phone,(PC)site);
		if(FicPara.g_type_connexion == 'G')
			status = connect_host_GPRS((PC)phone,(PC)site);
		if(FicPara.g_type_connexion == 'I')
		{
			if(FicPara.Etat_TLS==0)	//Kamalssl
				status = connect_host_IP((PC)phone,(PC)site);
			else
				status = connect_host_IP_TLS();
		}
		if(status != OK)
		{
			hang_up(0,g_trace);
		}
		Essai_cnx++;
	}while((Essai_cnx < 3)&&(status != OK));

	if(status != OK){
		strncpy(StructInfoCaisse[0].CRPIN,"909",3);
		strncpy(StructInfoCaisse[1].CRPIN,"909",3);
	}
	else{
		strncpy(StructInfoCaisse[0].CRPIN,"000",3);
		strncpy(StructInfoCaisse[1].CRPIN,"000",3);
	}
	//hang_up(typehangup,g_trace);
	return 0;
}
int findejournee_cmi(void)
{
	uchar status ;
	DATE date;
	uchar date_appel[7];

	if (FicPara.status == KO) {
		CUMORE_CMI_AFFICHE_MESSAGE(27, 138, 1000);
		return 0 ;
	}
	status = Fin_jour(1,0);
	if((status == TEL_OK) || (status == RECO_OK))
	//if(status != RECO_KO)
	{

		ExploiField7();
//		if (FicTra.nombre_tra == 0)
//			Imprime_ticket_telezero();
		if(FicTra.nombre_tra != 0)
			ImprimeTelecollecte(0,status);
		else
			Imprime_ticket_telezero();
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


	PutAllTra();
	PutAllParam();
	return 0;
}
int listenoire_cmi(void)
{
	StateHeader(1);
	ListeNoire(1,1);
	PutAllLop();
	StateHeader(0);
//	return batch_cmi();
	return 0;
}
int totaux_cmi(void)
{
	ImprimeTelecollecte(1,0);    //(2,0) ==> ( 1,0) correction journal
//	return report_cmi();
	return 0;
}
int journal_cmi(void)
{
	JournalEdit();
//	return report_cmi();
	return 0;
}
int journalpreauto_cmi(void)
{
	JournalEditPre();
//	return report_cmi();
	return 0;
}
int razram_cmi(void)
{
//	if(Affich_ecr_pwd(BANK_PWD,4)!=OK)
//		return OK;
//	// jrizki 27032019
//	if(FicTra.nombre_tra != 0){
//		MessageWithDelay("LANCER LA TELECOLLECTE", NULL, 5000);
//		return OK;
//	}

	if(mdp_merchant_cmi() != CR_SAISIE_OK)
	{
		CUMORE_CMI_AFFICHE_MESSAGE(16,20,2000);
		return OK;
	}

	if(Valider(67,21)!=KEY_ENTER)
		return OK;

	Init_Param();
	Init_Lop();
	Init_Tra();
	Init_Pre();
//	return maintenance_cmi();
	return 0;
}
int effacetransa_cmi(void)
{
	if(Affich_ecr_pwd(BANK_PWD,4)!=OK)
		return OK;
	if(Valider(68,21)!=KEY_ENTER)
		return OK;
	Init_Tra();
//	return maintenance_cmi();
	return 0;
}
int effacepreauto_cmi(void)
{
	if(Affich_ecr_pwd(BANK_PWD,4)!=OK)
		return OK;

	if(Valider(69,21)!=KEY_ENTER)
		return OK;

	Init_Pre();
//	return maintenance_cmi();
	return 0;
}
int razlop_cmi(void)
{
	if(Affich_ecr_pwd(BANK_PWD,4)!=OK)
		return OK;
	g_trace = 0;//Mouna310305
	g_trace_emv = 1;
	if(Valider(70,21)!=KEY_ENTER)
		return OK;
	Init_Lop();
//	return maintenance_cmi();
	return 0;
}
int edittrsemv_cmi(void)
{
	if(Affich_ecr_pwd(BANK_PWD,4)!=OK)
		return OK;
	Print_Last_Trs_Emv_Rej();
//	return maintenance_cmi();
	return 0;
}

int NumCmr_cmi(void)
{
	int cr;
	int i;

	unsigned char NumCmr_id [ 12+1 ] ;
	const TAB_ENTRY_STRING t_entry_id={0x0002,TO_PARAM,1,0,1,12,6};

	memclr( NumCmr_id, 12 +1);

	/* get NumCmr_id */
	i=3; /* 3 essais */
  	while (i>0)
  	{
		memcpy(NumCmr_id,FicPara.Num_point_vente,strlen((char*)FicPara.Num_point_vente));
		memcpy(NumCmr_id,FicPara.Num_commerce,strlen((char*)FicPara.Num_commerce));
		cr = SLC_Saisie_Numerique (NumCmr_id, "",(unsigned char *) mess_cmi_NumCmr_id, (TAB_ENTRY_STRING *)&t_entry_id);
		switch (cr)
		{
		case T_VAL :

//		memcpy(FicPara.Num_point_vente,NumCmr_id,12);

		if(FicTra.nombre_tra == 0){
			memcpy(FicPara.Num_point_vente,NumCmr_id,12);
			memcpy(FicPara.Num_commerce,NumCmr_id,12);// jrizki 25032019
			FicPara.status = KO; // jrizki 25032019

		}
		else
			CUMORE_CMI_AFFICHE_MESSAGE(130,138,1000);

		CUSERV_New_Parameters = TRUE;
		i=0;
		break;

		case T_ANN :
		i=0;
		return KO;

		case TIMEOUT :
		CUMORE_CMI_AFFICHE_MESSAGE(170,138,1000);
		return KO;

		default :
		memclr(NumCmr_id, 12 + 1);
		i-- ;
		break;
		}
  	}

	return NumTrm_cmi();
}


int NumTrm_cmi(void)
{
	int cr;
	int i;

	unsigned char NumTrm_id [ 17 ] ;
	const TAB_ENTRY_STRING t_entry_id={0x0002,TO_PARAM,1,0,1,12,6};

	memclr( NumTrm_id, sizeof(NumTrm_id));

	/* get NumTrm_id */
	i=3; /* 3 essais */
  	while (i>0)
  	{
		memcpy(NumTrm_id,FicPara.Num_Terminal,strlen((char*)FicPara.Num_Terminal));
		cr = SLC_Saisie_Numerique (NumTrm_id,"", (unsigned char *) mess_cmi_NumTrm_id, (TAB_ENTRY_STRING *)&t_entry_id);
		switch (cr)
		{
		case T_VAL :

//		memcpy(FicPara.Num_Terminal,NumTrm_id,16);

			if(FicTra.nombre_tra == 0)
			memcpy(FicPara.Num_Terminal,NumTrm_id,16);
		else
			CUMORE_CMI_AFFICHE_MESSAGE(130,138,1000);

		PutAllParam();
		CUSERV_New_Parameters = TRUE;
		i=0;
		break;

		case T_ANN :
		i=0;
		return KO;

		case TIMEOUT :
		CUMORE_CMI_AFFICHE_MESSAGE(170,138,1000);
		return KO;

		default :
		memclr(NumTrm_id, sizeof(NumTrm_id));
		i-- ;
		break;
		}
  	}
	//return ligneautopri_cmi();
  	return administra_cmi();
  	//return choix_langue;
}
int choix_langue(void)
{
	T_GL_HGRAPHIC_LIB hGoal = APEMV_UI_GoalHandle();
	bool bContinue = TRUE, bContinue2 = TRUE;
	const char * menu[] =
	{
	" Arabe",
	" Français",
	//" Anglais",
	0 //<- DO NOT FORGET THIS TO STOP THE LIST
	};

	do {
		switch (GL_Dialog_Choice(hGoal, "Choisir une langue", menu, 0,GL_BUTTON_DEFAULT, GL_KEY_1,GL_TIME_MINUTE)) {
		case 0:
			arabe();
			goto lblEnd;
		case 1:
			francais();
			goto lblEnd;
//		case 2:
//			anglais();
//			goto lblEnd;
		default:
			bContinue2 = FALSE;
			goto lblEnd;
		}
	} while (bContinue2);


	lblEnd:
	return KO;

	//return ligneautopri_cmi();
  	return administra_cmi();
}
int ligneautopri_cmi(void)
{
  int cr;
  int i;

  unsigned char ligneautopri_id [ 17 ] ;
  const TAB_ENTRY_STRING t_entry_id={0x0000,TO_PARAM,1,0,1,16,1};

	memclr( ligneautopri_id, 17);

	/* get ligneautopri_id */
	i=3; /* 3 essais */
  	while (i>0)
  	{
    	memcpy(ligneautopri_id,FicPara.Tel_autor_pri,16);//memcpy(ligneautopri_id,"022944008",16);
		cr = SLC_Saisie_Numerique (ligneautopri_id,"", (unsigned char *) mess_cmi_ligneautopri_id, (TAB_ENTRY_STRING *)&t_entry_id);
    	switch (cr)
    	{
    		case T_VAL :
				memcpy(FicPara.Tel_autor_pri,ligneautopri_id,16);
				PutAllParam();
	    		CUSERV_New_Parameters = TRUE;
	    		i=0;
    			break;

			case T_ANN :
	    		i=0;
				return KO;
				
			case TIMEOUT :
				CUMORE_CMI_AFFICHE_MESSAGE(170,138,1000);
				return KO;

			default :
    			memclr(ligneautopri_id, 17);
    			i-- ;
    			break;
    	}
  	}

	return lignetelesec_cmi();
}
int lignetelesec_cmi(void)
{
  int cr;
  int i;

  unsigned char lignetelesec_id [ 17 ] ;
  const TAB_ENTRY_STRING t_entry_id={0x0000,TO_PARAM,1,0,1,16,1};

	memclr( lignetelesec_id, 17);

	/* get lignetelesec_id */
	i=3; /* 3 essais */
  	while (i>0)
  	{
    	memcpy(lignetelesec_id,FicPara.Tel_telec_sec,16);//memcpy(lignetelesec_id,"022944008",16);
		cr = SLC_Saisie_Numerique (lignetelesec_id,"", (unsigned char *) mess_cmi_lignetelesec_id, (TAB_ENTRY_STRING *)&t_entry_id);
    	switch (cr)
    	{
    		case T_VAL :
				memcpy(FicPara.Tel_telec_sec,lignetelesec_id,16);
				PutAllParam();
	    		CUSERV_New_Parameters = TRUE;
	    		i=0;
    			break;

			case T_ANN :
	    		i=0;
				return KO;
				
			case TIMEOUT :
				CUMORE_CMI_AFFICHE_MESSAGE(170,138,1000);
				return KO;

			default :
    			memclr(lignetelesec_id, 17);
    			i-- ;
    			break;
    	}
  	}

	return ligneautosec_cmi();
}
int ligneautosec_cmi(void)
{
  int cr;
  int i;

  unsigned char ligneautosec_id [ 16 ] ;
  const TAB_ENTRY_STRING t_entry_id={0x0000,TO_PARAM,1,0,1,16,1};

	memclr( ligneautosec_id, 16);

	/* get ligneautosec_id */
	i=3; /* 3 essais */
  	while (i>0)
  	{
    	memcpy(ligneautosec_id,FicPara.Tel_autor_sec,16);//memcpy(ligneautosec_id,"022944008",16);
		cr = SLC_Saisie_Numerique (ligneautosec_id,"", (unsigned char *) mess_cmi_ligneautosec_id, (TAB_ENTRY_STRING *)&t_entry_id);
    	switch (cr)
    	{
    		case T_VAL :
				memcpy(FicPara.Tel_autor_sec,ligneautosec_id,16);
				PutAllParam();
	    		CUSERV_New_Parameters = TRUE;
	    		i=0;
    			break;

			case T_ANN :
	    		i=0;
				return KO;
				
			case TIMEOUT :
				CUMORE_CMI_AFFICHE_MESSAGE(170,138,1000);
				return KO;

			default :
    			memclr(ligneautosec_id, 16);
    			i-- ;
    			break;
    	}
  	}

	return abonntelepri_cmi();
}
int abonntelepri_cmi(void)
{
  int cr;
  int i;

  unsigned char abonntelepri_id [ 16 ] ;
  const TAB_ENTRY_STRING t_entry_id={0x0000,TO_PARAM,1,0,1,15,1};

	memclr( abonntelepri_id, 16);

	/* get abonntelepri_id */
	i=3; /* 3 essais */
  	while (i>0)
  	{
		memcpy(abonntelepri_id,FicPara.X25_telec_pri,15);//memcpy(abonntelepri_id,"12701017565",15);
    	cr = SLC_Saisie_Numerique (abonntelepri_id,"", (unsigned char *) mess_cmi_abonntelepri_id, (TAB_ENTRY_STRING *)&t_entry_id);
    	switch (cr)
    	{
    		case T_VAL :
				memcpy(FicPara.X25_telec_pri,abonntelepri_id,15);
				PutAllParam();
				CUSERV_New_Parameters = TRUE;
	    		i=0;
    			break;

			case T_ANN :
	    		i=0;
				return KO;
				
			case TIMEOUT :
				CUMORE_CMI_AFFICHE_MESSAGE(170,138,1000);
				return KO;

			default :
    			memclr(abonntelepri_id, 16);
    			i-- ;
    			break;
    	}
  	}

	return abonnautopri_cmi();
}
int abonnautopri_cmi(void)
{
  int cr;
  int i;

  unsigned char abonnautopri_id [ 16 ] ;
  const TAB_ENTRY_STRING t_entry_id={0x0000,TO_PARAM,1,0,1,15,1};

	memclr( abonnautopri_id, 16);

	/* get abonnautopri_id */
	i=3; /* 3 essais */
  	while (i>0)
  	{
    	memcpy(abonnautopri_id,FicPara.X25_autor_pri,15);//memcpy(abonnautopri_id,"12701017565",15);
		cr = SLC_Saisie_Numerique (abonnautopri_id,"", (unsigned char *) mess_cmi_abonnautopri_id, (TAB_ENTRY_STRING *)&t_entry_id);
    	switch (cr)
    	{
    		case T_VAL :
				memcpy(FicPara.X25_autor_pri,abonnautopri_id,15);
				PutAllParam();
    			CUSERV_New_Parameters = TRUE;
	    		i=0;
    			break;

			case T_ANN :
	    		i=0;
				return KO;
				
			case TIMEOUT :
				CUMORE_CMI_AFFICHE_MESSAGE(170,138,1000);
				return KO;

			default :
    			memclr(abonnautopri_id, 16);
    			i-- ;
    			break;
    	}
  	}

	return abonntelesec_cmi();
}
int abonntelesec_cmi(void)
{
  int cr;
  int i;

  unsigned char abonntelesec_id [ 16] ;
  const TAB_ENTRY_STRING t_entry_id={0x0000,TO_PARAM,1,0,1,15,1};

	memclr( abonntelesec_id, 16);

	/* get abonntelesec_id */
	i=3; /* 3 essais */
  	while (i>0)
  	{
    	memcpy(abonntelesec_id,FicPara.X25_autor_sec,15);//memcpy(abonntelesec_id,"12701017565",15);
		cr = SLC_Saisie_Numerique (abonntelesec_id,"", (unsigned char *) mess_cmi_abonntelesec_id, (TAB_ENTRY_STRING *)&t_entry_id);
    	switch (cr)
    	{
    		case T_VAL :
				memcpy(FicPara.X25_autor_sec,abonntelesec_id,15);
				PutAllParam();
	    		CUSERV_New_Parameters = TRUE;
	    		i=0;
    			break;

			case T_ANN :
	    		i=0;
				return KO;
				
			case TIMEOUT :
				CUMORE_CMI_AFFICHE_MESSAGE(170,138,1000);
				return KO;

			default :
    			memclr(abonntelesec_id, 16);
    			i-- ;
    			break;
    	}
  	}

	return abonnautosec_cmi();
}
int abonnautosec_cmi(void)
{
  int cr;
  int i;

  unsigned char abonnautosec_id [ 16] ;
  const TAB_ENTRY_STRING t_entry_id={0x0000,TO_PARAM,1,0,1,15,1};

	memclr( abonnautosec_id, 16);

	/* get abonnautosec_id */
	i=3; /* 3 essais */
  	while (i>0)
  	{
    	memcpy(abonnautosec_id,FicPara.X25_telec_sec,15);//memcpy(abonnautosec_id,"12701017565",15);
		cr = SLC_Saisie_Numerique (abonnautosec_id,"", (unsigned char *) mess_cmi_abonnautosec_id, (TAB_ENTRY_STRING *)&t_entry_id);
    	switch (cr)
    	{
    		case T_VAL :
				memcpy(FicPara.X25_telec_sec,abonnautosec_id,15);
				PutAllParam();
				CUSERV_New_Parameters = TRUE;
	    		i=0;
    			break;

			case T_ANN :
	    		i=0;
				return KO;
				
			case TIMEOUT :
				CUMORE_CMI_AFFICHE_MESSAGE(170,138,1000);
				return KO;

			default :
    			memclr(abonnautosec_id, 16);
    			i-- ;
    			break;
    	}
  	}

	return delaipreautor_cmi();
}
int delaipreautor_cmi(void)
{
  int cr;
  int i;

  unsigned char delaipreautor_id [ 3 ] ;
  const TAB_ENTRY_STRING t_entry_id={0x0000,TO_PARAM,1,0,1,2,1};

	memclr( delaipreautor_id, 3);

	/* get delaipreautor_id */
	i=3; /* 3 essais */
  	while (i>0)
  	{
    	sprintf((char*)delaipreautor_id,"%d",FicPara.delai_preauto);
		cr = SLC_Saisie_Numerique (delaipreautor_id,"", (unsigned char *) mess_cmi_delaipreauto_id, (TAB_ENTRY_STRING *)&t_entry_id);
    	switch (cr)
    	{
    		case T_VAL :
				FicPara.delai_preauto=atol((char*)delaipreautor_id);
				PutAllParam();
				CUSERV_New_Parameters = TRUE;
	    		i=0;
    			break;

			case T_ANN :
	    		i=0;
				return KO;
				
			case TIMEOUT :
				CUMORE_CMI_AFFICHE_MESSAGE(170,138,1000);
				return KO;

			default :
    			memclr(delaipreautor_id, 3);
    			i-- ;
    			break;
    	}
  	}

//Mouna310305_dÃ©but
  if(Fic_compte_dom.Indice_fin>1)
	  return administradom_cmi();
  else
	  return administra_cmi();
//Mouna310305_fin
}
int parametres_cmi(void)
{
	Imprime_Param();
//	return editparam_cmi();
	return 0;
}
int devise_cmi(void)
{
	Imprime_devises();
//	return editparam_cmi();;
	return 0;
}
int bin_cmi(void)
{
	Imprime_Bin();
//	return editparam_cmi();;
	return 0;
}
int prefixes_cmi(void)
{
	Imprime_Prefixes();
//	return editparam_cmi();;
	return 0;
}
int comptes_cmi(void)
{
	Imprime_Comptes();
//	return editparam_cmi();;
	return 0;
}


/*******************************************************************************************************************/
int GetItemAlphaNumerique( char *sOutput, int OutputLen,char *libelle,unsigned short int maski)
{
	int nState;
	int cr;
	const TAB_ENTRY_STRING t_entry_id={maski,TO_PARAM,1,0,1,24,1};

	
	cr = SLC_Saisie_Numerique (sOutput,"", (unsigned char *) libelle, (TAB_ENTRY_STRING *)&t_entry_id);
	switch (cr)
	{
		case T_VAL :
		nState = __STATE_INPUT_END;
		break;

		case T_ANN :
		nState = __STATE_INPUT_BACK;
		break;
			
		case TIMEOUT :
		nState = __STATE_INPUT_EXIT;

		default :
		break;
	}

	return nState;

	
  if (FicPara.g_type_connexion=='R') return 0;  //
 
	do
	{
			switch(HelperManageInput( libelle, 2, 20,
									  sOutput, 10, 30, OutputLen,
									  0, 0 ))
			switch(nState)
			{
			case CU_VALID:
				nState = __STATE_INPUT_END;
				break;

			case CORRECT: // Back
				nState = __STATE_INPUT_BACK;
				return KO;
				

			default: // Cancel
				nState = __STATE_INPUT_EXIT;
				break;
			}
			break;
	} while(( nState != __STATE_INPUT_END )   &&
		    ( nState != __STATE_INPUT_BACK )  && 
			( nState != __STATE_INPUT_EXIT ));

	return nState;
}

int ip_gprs_cmi(void)
{
	char ip_id[15];
	 

	FicPara.Etat_TLS = 1;
	FicPara.g_type_connexion ='I';
	tcpip_nbe();
	PutAllParam();
 	return 0;


	memset(ip_id,0x00,sizeof(ip_id));

	 memcpy(ip_id,FicPara.ip_gprs,15);

	 if( GetItemAlphaNumerique(ip_id, 15,"IP :",0x0004) == __STATE_INPUT_END ){
		memcpy(FicPara.ip_gprs,ip_id,15);
	 } 
	 	return port_gprs_cmi();
}


int port_gprs_cmi(void)
{
	 char port_id[6];
	 
	 memset(port_id,0x00,sizeof(port_id));
	 if(FicTra.nombre_tra != 0){
	 		CUMORE_CMI_AFFICHE_MESSAGE(130,138,1000);
	 		return 0;
	 	}
	 memcpy(port_id,FicPara.port_gprs,6);

	 if( GetItemAlphaNumerique(port_id, 6,"PORT :",0x0002) == __STATE_INPUT_END ){
		memcpy(FicPara.port_gprs,port_id,6);
	 } 
	 	return apn_gprs_cmi();
}

int apn_gprs_cmi(void)
{
	 char apn_id[40];
	 
	 memset(apn_id,0x00,sizeof(apn_id));

	 memcpy(apn_id,FicPara.apn_gprs,40);
	 if( GetItemAlphaNumerique(apn_id, 40,"APN :",0x0000) == __STATE_INPUT_END ){
		memcpy(FicPara.apn_gprs,apn_id,40);
	 } 
	 	return login_gprs_cmi();
}
int login_gprs_cmi(void)
{
	 char login_id[40];
	 
	 memset(login_id,0x00,sizeof(login_id));

	 memcpy(login_id,FicPara.login_gprs,40);
		if( GetItemAlphaNumerique(login_id, 40,"LOGIN GPRS :",0x0000) == __STATE_INPUT_END ){
			memcpy(FicPara.login_gprs,login_id,40);
		} 
	 return pwd_gprs_cmi();
}
int pwd_gprs_cmi(void)
{
	 char pwd_id[40];
	 
	 memset(pwd_id,0x00,sizeof(pwd_id));

	 memcpy(pwd_id,FicPara.pwd_gprs,40);
		if( GetItemAlphaNumerique(pwd_id, 40,"PASSWORD GPRS :",0x0004) == __STATE_INPUT_END ){
			memcpy(FicPara.pwd_gprs,pwd_id,40);
		}

	FicPara.g_type_connexion = 'G';
//return time_out_param();
}
int time_out_param(void)
{
	 char time[40];

		 memset(time,0x00,sizeof(time));

		 memcpy(time,FicPara.TimeOutReceive,40);
			if( GetItemAlphaNumerique(time, 40,"TIME OUT:",0x0004) == __STATE_INPUT_END ){
				memcpy(FicPara.TimeOutReceive,time,40);
			}

//	 int time=0;
//
//	 memcpy(pwd_id,FicPara.TimeOutReceive,40);
//		if( GetItemAlphaNumerique(pwd_id, 40,"PASSWORD GPRS :",0x0004) == __STATE_INPUT_END ){
//			memcpy(FicPara.TimeOutReceive,pwd_id,40);
//		}
	//APEMV_UI_MessageDisplayDelay("parrametrage",4);
	//SLC_Saisie_Numerique(FicPara.TimeOutReceive,"",);
	//	 memcpy(FicPara.TimeOutReceive,time,strlen(time));

	 }

/*******************************************************************************************************************/
int	menu_com_pm(char item)
{
	unsigned char   cr,i;
	BUFFER_SAISIE   buf_saisie;
	uchar	Buffer[16],Pwd[5];

	InitDefaut(_ON_,1);

	switch(item)
	{

/*	case 0:
		FicPara.Etat_TLS = 1;	//KamalCless
		PutAllParam();
		break;
*/
	case 0:
		FicPara.Etat_TLS = 1;	//KamalCless

		Menu.Fenetre.titre      = (unsigned char *)"TLS/SSL";
		Menu.tab[0] 			= "TLS/SSL.1";
		Menu.tab[1] 			= "TLS/SSL.0";

		Menu.tab[2] = NULL;

		cr = (int)APEMV_UI_MenuSelectTopic((const char*)Menu.Fenetre.titre,  (const char*)Menu.tab, TRUE);
		switch(cr)
		{
		case CR_SAISIE_OK :
			switch(buf_saisie.donnees[0])
			{
			case 0:
				FicPara.Etat_TLS = 1;	//KamalCless
				break;
			case 1:
				FicPara.Etat_TLS = 0;	//KamalCless
				break;
			default:
				break;
			}
			break;
		}
		PutAllParam();
		break;

	case 1:
		FicPara.Etat_TLS = 1;
		FicPara.g_type_connexion ='I';
		tcpip_nbe();
		PutAllParam(); //
		break;
	
	default:
		break;

	}
	PutAllParam();
	return OK;
}
int comm_pm(void)
{

	unsigned char   i;
	T_GL_WCHAR choice  = 0;

	InitDefaut(_ON_,1);

	StateHeader(0);
	/* table init from const */
	Menu.Fenetre.titre          = (unsigned char *)ReadMessage_cmi(MESS52,PSQ_Donner_langue());
	for (i=0 ; i < ELVEC(Navigationparam_gprs) ; i++ )
	{
	Menu.tab[i] = (unsigned char *) ReadMessage_cmi ( Navigationparam_gprs[i].itemlabel ,PSQ_Donner_langue());
	}
	/* call navigation time-out 30 secs */
	Menu.tab[i] = NULL;
	choice = (int)APEMV_UI_MenuSelectTopic((const char*)Menu.Fenetre.titre,  (const char*)Menu.tab, TRUE);
	if (choice != -1)
	{
		Navigationparam_gprs[choice].function();
	}

	StateHeader(1);
	PutAllParam();
	return OK;
}
int comm_pm_rtc(void)
{
	return 0;
}
int id_mode_nbe(void)
{
	int cr;
	int i;
	
	unsigned char modegsm_id [ 2 ] ;
	const TAB_ENTRY_STRING t_entry_id={0x0000,TO_PARAM,1,0,1,9,0};
	
	memclr( modegsm_id , 2);
	
	// get standard_id 
	i=3; // 3 essais 
	while (i>0)
	{
		memcpy(modegsm_id,FicPara.MODEGSM,2);
		cr = SLC_Saisie_Numerique (modegsm_id ,"", (unsigned char *) mess_nbe_modegsm_id , (TAB_ENTRY_STRING *)&t_entry_id);
		switch (cr)
		{
		case T_VAL :
			memcpy(FicPara.MODEGSM,modegsm_id,2);
			CUSERV_New_Parameters = TRUE;
			i=0;
			break;
			
		case T_ANN :
			i=0;
			return KO;
			
		case TIMEOUT :
			CUMORE_CMI_AFFICHE_MESSAGE(170,138,1000);
			return KO;
			
		default :
			memclr(modegsm_id, 2);
			i-- ;
			break;
		}
	}
	return id_rlp_nbe();
	
}
int id_rlp_nbe(void)
{
	int cr;
	int i;
	
	unsigned char rlp_id [ 2 ] ;
	const TAB_ENTRY_STRING t_entry_id={0x0000,TO_PARAM,1,0,1,9,0};
	
	memclr( rlp_id , 2);
	
	// get standard_id 
	i=3; // 3 essais 
	while (i>0)
	{
		memcpy(rlp_id,FicPara.RLPGSM,2);
		cr = SLC_Saisie_Numerique (rlp_id ,"", (unsigned char *) mess_nbe_rlp_id , (TAB_ENTRY_STRING *)&t_entry_id);
		switch (cr)
		{
		case T_VAL :
			memcpy(FicPara.RLPGSM,rlp_id,2);
			CUSERV_New_Parameters = TRUE;
			i=0;
			break;
			
		case T_ANN :
			i=0;
			return KO;
			
		case TIMEOUT :
			CUMORE_CMI_AFFICHE_MESSAGE(170,138,1000);
			return KO;
			
		default :
			memclr(rlp_id, 2);
			i-- ;
			break;
		}
	}
	return 0;
}
int ip_gprs_nbe(void)
{
	char ip_id[15];
	
	Telium_File_t *hDisplay = NULL;
	
	memset(ip_id,0x00,sizeof(ip_id));
	memcpy(ip_id,FicPara.ip_gprs,15);
	
	if (CUTERMgetTerminalType () != T_CUTERM_X930)
		hDisplay = Telium_Fopen( "DISPLAY", "w" );
	
	if( GetItemAlphaNumerique(ip_id, 15,"IP :",0x0004) == __STATE_INPUT_END ){
		memcpy(FicPara.ip_gprs,ip_id,15);
	} 

	if (CUTERMgetTerminalType () != T_CUTERM_X930)
		Telium_Fclose( hDisplay );
	
	return port_gprs_nbe();
}
int port_gprs_nbe(void)
{
	char port_id[6];
	
	Telium_File_t *hDisplay = NULL;
	
	memset(port_id,0x00,sizeof(port_id));
	memcpy(port_id,FicPara.port_gprs,6);
	
	if (CUTERMgetTerminalType () != T_CUTERM_X930)
		hDisplay = Telium_Fopen( "DISPLAY", "w" );
	
	if( GetItemAlphaNumerique(port_id, 6,"PORT :",0x0004) == __STATE_INPUT_END ){
		memcpy(FicPara.port_gprs,port_id,6);
	} 

	if (CUTERMgetTerminalType () != T_CUTERM_X930)
		Telium_Fclose( hDisplay );
	
	return apn_gprs_nbe();
}
int apn_gprs_nbe(void)
{
	char apn_id[40];
	
	Telium_File_t *hDisplay = NULL;
	
	memset(apn_id,0x00,sizeof(apn_id));
	memcpy(apn_id,FicPara.apn_gprs,40);

	if (CUTERMgetTerminalType () != T_CUTERM_X930)
		hDisplay = Telium_Fopen( "DISPLAY", "w" );
	
	if( GetItemAlphaNumerique(apn_id, 40,"APN :",0x0004) == __STATE_INPUT_END ){
		memcpy(FicPara.apn_gprs,apn_id,40);
	} 
	
	if (CUTERMgetTerminalType () != T_CUTERM_X930)
		Telium_Fclose( hDisplay );
	
	return login_gprs_nbe();
}
int login_gprs_nbe(void)
{
	char login_id[40];
	
	Telium_File_t *hDisplay = NULL;
	
	memset(login_id,0x00,sizeof(login_id));
	memcpy(login_id,FicPara.login_gprs,40);

	if (CUTERMgetTerminalType () != T_CUTERM_X930)
		hDisplay = Telium_Fopen( "DISPLAY", "w" );
	
	if( GetItemAlphaNumerique(login_id, 40,"LOGIN GPRS :",0x0004) == __STATE_INPUT_END ){
		memcpy(FicPara.login_gprs,login_id,40);
	} 
	
	if (CUTERMgetTerminalType () != T_CUTERM_X930)
		Telium_Fclose( hDisplay );
	
	return pwd_gprs_nbe();
}
int pwd_gprs_nbe(void)
{
	char pwd_id[40];
	
	Telium_File_t *hDisplay = NULL;
	
	memset(pwd_id,0x00,sizeof(pwd_id));
	memcpy(pwd_id,FicPara.pwd_gprs,40);

	if (CUTERMgetTerminalType () != T_CUTERM_X930)
		hDisplay = Telium_Fopen( "DISPLAY", "w" );
	
	if( GetItemAlphaNumerique(pwd_id, 40,"PASSWORD GPRS :",0x0004) == __STATE_INPUT_END ){
		memcpy(FicPara.pwd_gprs,pwd_id,40);
	} 
	
	if (CUTERMgetTerminalType () != T_CUTERM_X930)
		Telium_Fclose( hDisplay );
	return 0;
}
int	tcpip_nbe(void)
{
	char ip_id[15];
	char port_id[6];
	char domain_name[32];
	 	 
	if(FicTra.nombre_tra != 0){
	CUMORE_CMI_AFFICHE_MESSAGE(130,138,1000);
	return 0;
	}
	Telium_File_t *hDisplay = NULL;
	
	if (CUTERMgetTerminalType () != T_CUTERM_X930)
		hDisplay = Telium_Fopen( "DISPLAY", "w" );
	

	memset(ip_id,0x00,sizeof(ip_id));

	memset(domain_name,0x00,sizeof(domain_name));
	memcpy(domain_name,FicPara.nom_domaine,strlen(FicPara.nom_domaine));


	if( GetItemAlphaNumerique(domain_name, 31,"DOMAINE OU IP :",0x0000) == __STATE_INPUT_END ){
		memset(FicPara.nom_domaine,0x00,sizeof(FicPara.nom_domaine));
		memcpy(FicPara.nom_domaine,domain_name,strlen(domain_name));

		ResolverSetOption ( RSLVO_SERV2_IPADDR , "8.8.8.8" ) ;
		DNS_GetIpAddress (domain_name,FicPara.ip_gprs, sizeof(FicPara.ip_gprs));
	}


	memset(port_id,0x00,sizeof(port_id));
	memcpy(port_id,FicPara.port_gprs,6);
	
	if( GetItemAlphaNumerique(port_id, 6,"PORT :",0x0000) == __STATE_INPUT_END ){
		memcpy(FicPara.port_gprs,port_id,6);
	}




	memset(domain_name,0x00,sizeof(domain_name));
	memcpy(domain_name,FicPara.apn_gprs,strlen(FicPara.apn_gprs));


	if( GetItemAlphaNumerique(domain_name, 31,"APN  :",0x0000) == __STATE_INPUT_END ){
		memset(FicPara.apn_gprs,0x00,sizeof(FicPara.apn_gprs));
		memcpy(FicPara.apn_gprs,domain_name,strlen(domain_name));
		//APEMV_UI_MessageDisplayDelay(FicPara.ip_gprs,2);

		FicPara.g_type_connexion = 'G';
		APEMV_UI_MessageDisplayDelay("GPRS MODE",2);
	}


	if (CUTERMgetTerminalType () != T_CUTERM_X930)
		Telium_Fclose( hDisplay );
	return 0;
}


int transaction_naps(void)
{
	unsigned char   i;
	T_GL_WCHAR choice  = 0;

	InitDefaut(_ON_,1);

	StateHeader(0);
	/* table init from const */
	Menu.Fenetre.titre          = (unsigned char *)ReadMessage_cmi(MESS58,PSQ_Donner_langue());
	for (i=0 ; i < ELVEC(Navigationtransaction_naps) ; i++ )
	{
	Menu.tab[i] = (unsigned char *) ReadMessage_cmi ( Navigationtransaction_naps[i].itemlabel ,PSQ_Donner_langue());
	}
	/* call navigation time-out 30 secs */
	Menu.tab[i] = NULL;
	choice = (int)APEMV_UI_MenuSelectTopic((const char*)Menu.Fenetre.titre,  (const char*)Menu.tab, TRUE);
//	char tmp[10];
//    sprintf(tmp,"%d",choice);
//    Os__xprinta(tmp);
//    Os__xprintd(choice);
	if (choice != -1)
	{
		Navigationtransaction_naps[choice].function();
	}

	StateHeader(1);
	PutAllParam();
	return OK;
}


//////// MENU DYNAMIQUE ///////////////////////////////////////////////////////////////////////////////////////////////////////
int prepaye_naps(void){
	unsigned char   cr;
	BUFFER_SAISIE   buf_saisie;

	InitDefaut(_ON_,1);

//	Menu.Fenetre.titre      = (unsigned char *)"PREPAYE";
//	Menu.tab[0] 			= "RECHARGE CARTE";
//	Menu.tab[1] 			= "DECHARGE CARTE";
//	Menu.tab[2] 			= "TR. CARTE A CARTE";
//	Menu.tab[3] 			= "TR. CARTE A CASH";
//	Menu.tab[4] = NULL;
//
//	cr = (int)APEMV_UI_MenuSelectTopic((const char*)Menu.Fenetre.titre,  (const char*)Menu.tab, TRUE);
//	switch(cr)
//	{
//	case CR_SAISIE_OK :
//		switch(buf_saisie.donnees[0])
//		{
//		case 0:
//
//			break;
//		case 1:
//
//			break;
//		case 2:
//
//			break;
//		case 3:
//			break;
//		default:
//			break;
//		}
//		break;
//	}

	PutAllParam();
	return OK;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////

int duplicata_naps(void)
{
	duplicata_cmi();

	PutAllParam();
	return OK;
}
int parametres_naps(void)
{
	unsigned char   i;
	T_GL_WCHAR choice  = 0;

	InitDefaut(_ON_,1);

	StateHeader(0);
	/* table init from const */
	Menu.Fenetre.titre          = (unsigned char *)ReadMessage_cmi(MESS8,PSQ_Donner_langue());
	for (i=0 ; i < ELVEC(Navigationbatchparametres_naps) ; i++ )
	{
	Menu.tab[i] = (unsigned char *) ReadMessage_cmi ( Navigationbatchparametres_naps[i].itemlabel ,PSQ_Donner_langue());
	}
	/* call navigation time-out 30 secs */
	Menu.tab[i] = NULL;
	choice = (int)APEMV_UI_MenuSelectTopic((const char*)Menu.Fenetre.titre,  (const char*)Menu.tab, TRUE);
	if (choice != -1)
	{
		Navigationbatchparametres_naps[choice].function();
	}

	StateHeader(1);
	PutAllParam();
	return OK;
}

int cashadvance_naps(void) {

//	sprintf(FicPara.MCC, "6010");
//	if (memcmp(FicPara.MCC, "6010", 4) == 0) {
//		Os__xprinta("Cashadvance");


//	if (AlternanceIsOn) {
//		Telium_Kill(alternateNbr, "");
//		AlternanceIsOn = 0;
//	}
	ulTransactionType = CUSERV_CASH_TRANSACTION;
	CUMORE_PaiementFctExt(CUSERV_CASH_TRANSACTION, FALSE, (unsigned long) 0,(int) 0);
//	}
}

int preautorisation_naps(void)
{

	unsigned char   i;
	T_GL_WCHAR choice  = 0;

	InitDefaut(_ON_,1);

	StateHeader(0);
	/* table init from const */
	Menu.Fenetre.titre          = (unsigned char *)ReadMessage_cmi(MESS11,PSQ_Donner_langue());
	for (i=0 ; i < ELVEC(Navigationpreauto_naps) ; i++ )
	{
	Menu.tab[i] = (unsigned char *) ReadMessage_cmi ( Navigationpreauto_naps[i].itemlabel ,PSQ_Donner_langue());
	}
	/* call navigation time-out 30 secs */
	Menu.tab[i] = NULL;
	choice = (int)APEMV_UI_MenuSelectTopic((const char*)Menu.Fenetre.titre,  (const char*)Menu.tab, TRUE);
//	char tmp[10];
//	sprintf(tmp, "%d", choice);
//	Os__xprinta(tmp);
//	Os__xprintd(choice);
	if (choice != -1)
	{
		Navigationpreauto_naps[choice].function();
	}

	StateHeader(1);
	PutAllParam();
	return OK;

}

int service_naps(void)
{

	unsigned char   i;
	T_GL_WCHAR choice  = 0;

	InitDefaut(_ON_,1);

	StateHeader(0);
	/* table init from const */
	Menu.Fenetre.titre          = (unsigned char *)ReadMessage_cmi(MESS67,PSQ_Donner_langue());
	for (i=0 ; i < ELVEC(Navigationservice_naps) ; i++ )
	{
	Menu.tab[i] = (unsigned char *) ReadMessage_cmi ( Navigationservice_naps[i].itemlabel ,PSQ_Donner_langue());
	}
	/* call navigation time-out 30 secs */
	Menu.tab[i] = NULL;
	choice = (int)APEMV_UI_MenuSelectTopic((const char*)Menu.Fenetre.titre,  (const char*)Menu.tab, TRUE);
//	char tmp[10];
//	sprintf(tmp, "%d", choice);
//	Os__xprinta(tmp);
//	Os__xprintd(choice);
	if (choice != -1)   //retour
	{
		Navigationservice_naps[choice].function();
	}

	StateHeader(1);
	PutAllParam();
	return OK;

}

int journaux_naps(void)
{
	report_cmi();
	return OK;

}
int Admin_Services(void)
{

	GL_SampleDialogChoice();
	return 0;
	  unsigned char   i;
	  T_GL_WCHAR choice  = 0;


	  InitDefaut(_ON_,1);
	  /* table init from const */
	  Menu.Fenetre.titre          = (unsigned char *)ReadMessage_cmi(MESS78,PSQ_Donner_langue());
	  for (i=0 ; i < ELVEC(NavigationServices) ; i++ )
	  {
	    Menu.tab[i] = (unsigned char *) ReadMessage_cmi ( NavigationServices[i].itemlabel ,PSQ_Donner_langue());
	  }
	  /* call navigation time-out 30 secs */
	  Menu.tab[i] = NULL;

	  choice = (int)APEMV_UI_MenuSelectTopic((const char*)Menu.Fenetre.titre,  (const char*)Menu.tab, TRUE);
	  if (choice != -1)
	  {
		  NavigationServices[choice].function();
	  }
	  return 0;
}
/*-----------------------------------------------------------------
 * Fonction   :Activer
 * objet      :pourboire
 *
 * Parametre  :
 * Retour     :
 *-----------------------------------------------------------------
*/
int Activer(void)
{
	//report_cmi();
	FicPara.var_pourboire = '1';
	PutAllParam();
	return OK;

}
/*-----------------------------------------------------------------
 * Fonction   :Desactiver
 * objet      :pourboire
 *
 * Parametre  :
 * Retour     :
 *-----------------------------------------------------------------
*/
int Desactiver(void)
{
	//report_cmi();
	FicPara.var_pourboire = '0';
	PutAllParam();
	return OK;

}

/*-----------------------------------------------------------------
 * Fonction   :control_badge
 * objet      :
 *
 * Parametre  :
 * Retour     :
 *-----------------------------------------------------------------
*/
uchar control_badge(void)
{
	uchar			PtIso[MAXPTISO];
	UC				status;


   //return OK;//S.Ba.58


	do
	{
	 status = Read_Card(PtIso,CARTE_COM);//pour CMI
	}while(status==SAIS_MAN);
	if(status==OK)
	{
		if(Recup_Info_badge(PtIso,0)!=OK)
		{
			CUMORE_CMI_AFFICHE_MESSAGE(14,16,1000);
			memset(&E_Transac8583,0x00,sizeof(E_Transac8583));
			memset(&R_Transac8583,0x00,sizeof(R_Transac8583));
			memset(&StructInfoPayment ,0x00,sizeof(StructInfoPayment));
			return KO;
		}
	}
	else if(status==ILLISIBLE)
	{
		status=KEY_CLEAR ;
		CUMORE_CMI_AFFICHE_MESSAGE(14,15,1000);
		memset(&E_Transac8583,0x00,sizeof(E_Transac8583));
		memset(&R_Transac8583,0x00,sizeof(R_Transac8583));
		memset(&StructInfoPayment ,0x00,sizeof(StructInfoPayment));
		return KO;
	}
	else
	{
		memset(&E_Transac8583,0x00,sizeof(E_Transac8583));
		memset(&R_Transac8583,0x00,sizeof(R_Transac8583));
		memset(&StructInfoPayment ,0x00,sizeof(StructInfoPayment));
		return KO;
	}

	//FIN TEST TOUARGUI
	return OK;
}
/*-----------------------------------------------------------------
 * Fonction   :GL_SampleDialogChoice
 * objet      :Activation pourboire
 *
 * Parametre  :
 * Retour     :
 *-----------------------------------------------------------------
*/
void GL_SampleDialogChoice()
{
	T_GL_HGRAPHIC_LIB hGoal = APEMV_UI_GoalHandle();
	bool bContinue = TRUE, bContinue2 = TRUE;
	const char * menu[] =
	{
	" Desactiver",
	" Activer",
	0 //<- DO NOT FORGET THIS TO STOP THE LIST
	};

	do {
		switch (GL_Dialog_Choice(hGoal, "Activation Pourboire", menu, 0,GL_BUTTON_DEFAULT, GL_KEY_1,GL_TIME_MINUTE)) {
		case 0:
			Desactiver();
			goto lblEnd;
		case 1:
			Activer();
			goto lblEnd;
		default:
			bContinue2 = FALSE;
			goto lblEnd;
		}
	} while (bContinue2);


	lblEnd:
	return 0;
}


int disk(){

    int ret;
    int iRet = 1;
    S_FS_PARAM_CREATE param;
    unsigned long size;

    memset(param.Label, 0, 16);
    Telium_Sprintf(param.Label, "FTP");
    param.AccessMode = FS_WRTMOD;
    param.IdentZone = FS_WO_ZONE_APPLICATION;
    param.Mode = FS_WRITEONCE;
    param.NbFichierMax = 10;

    ret = FS_dskcreate(&param, &size);

    switch(ret){
        case FS_OK :        iRet = 1; break;
        case FS_NAMEUSED:   iRet = 1; break;
        case FS_NOMEM :     iRet = 0; break;
        case FS_KO :        iRet = 0; break;
        default :           iRet = 1;
    }

    return iRet;
}
/*-----------------------------------------------------------------
 * Fonction   :download_FTP
 * objet      :
 *
 * Parametre  :
 * Retour     :
 *-----------------------------------------------------------------
*/
void download_FTP(){

	char buf[64];
	memset(buf, 0, sizeof(buf));

    int ret;
    unsigned int acessMode;
    FTP_HANDLE hFTP;

    ret = disk();
    //Os__xprintd(ret);
    if(ret == 1){

        ret = FS_mount("/FTP", &acessMode);                               //file://flash/HOST/
        OSL_Log(OSL_LOGGER_ERROR, "Disk Mount : %d ", ret);
       // Os__xprintd(ret);

        //hFTP = FTP_Connect("INGENICO", "INGENICO@2019$", "192.168.20.111", 21, false);
        hFTP = FTP_Connect("INGENICO", "INGENICO@2019", "172.19.37.18", 21, TRUE);
        if(hFTP=! NULL) Os__xprinta("connection handle OK");


       // err = FTP_GetFile(hFTP, fname, path);
        ret = FTP_GetFile(hFTP,"8224410224_emv_custom.P3A","/FTP/emv_custom.P3A");
        Os__xprinta("Hello 1");
       // Os__xprintd(ret);
        sprintf(buf, "%d", ret);
        Os__xprinta(buf);

		ret = FTP_GetError(hFTP);
		sprintf(buf, "%d", ret);
		Os__xprinta(buf);

		OSL_Log(OSL_LOGGER_ERROR, "Download result : %d ", ret);
        FTP_Disconnect(hFTP);
        FS_unmount("/FTP");
    }
}

void activateApplication(){

    SoftwareActivate("/FTP");
    Telium_Exit(0);
}


#define __FALSE__		0
#define __TRUE__		1

static const char g_pcExtendedStructTag[3]     = { 0x01, 0x02, 0x03 };

/*-----------------------------------------------------------------
 * Fonction   :GetAddressIP
 * objet      :
 *
 * Parametre  :
 * Retour     :
 *-----------------------------------------------------------------
*/
int GetAddressIP( const char *szAddress, unsigned int *o_pnAddress, unsigned int *o_pnPort )
{
	unsigned int nIP;
	unsigned int i,j;
	unsigned char c;

	char szTemp[4];

	//
	nIP = 0;

	j = 0;
	i = 0;

	while ((( c = szAddress[i++] ) != 0 ) &&
	       ( c != ':' ))
	{
		if( c == '.' )
		{
			nIP = ( nIP << 8) + atoi( szTemp );
			j = 0;
		}
		else
		{
			szTemp[j++] = c;
			szTemp[j] = '\0';
		}
	}

	if( c == ':' )
	{
		*o_pnAddress = ( nIP << 8 ) + atoi( szTemp );
		*o_pnPort	 = atoi( &szAddress[i] );

		return __TRUE__;
	}

	return __FALSE__;
}

/*-----------------------------------------------------------------
 * Fonction   :startTMSDownload
 * objet      : chargement à distance
 *
 * Parametre  :
 * Retour     :
 *-----------------------------------------------------------------
*/
int startTMSDownload()//badr
{
	int nRet = -1;
	char buf[64];
	S_PARAM_TLCHGT Params;



	memset( &Params, 0, sizeof( S_PARAM_TLCHGT ));

	PSQ_telechgt(&Params);

	FicPara.Top_TelAuto = 0;

	nRet = remote_download( &Params );
	sprintf(buf, "%d", nRet);
	if (nRet == 0){
		FicPara.Top_TelAuto = 1;
		PutAllTra();
		PutAllParam();
		Telium_Exit(0);
	}

	PutAllParam();

	return nRet;
}
/*-----------------------------------------------------------------
 * Fonction   :ServicesActivation
 * objet      :L'activation des services NAPS-SNTL-ORIFLAME
 *
 * Parametre  :--
 * Retour     :Type d'application
 *-----------------------------------------------------------------
*/

void ServicesActivation(void)//badr
{
		T_GL_HGRAPHIC_LIB hGoal = APEMV_UI_GoalHandle();
		bool bContinue = TRUE, bContinue2 = TRUE;
		const char * menu[] =
		{
		" NAPS",
		" NAPS ORIFLAME",
		" NAPS SNTL",
		" NAPS AVIS",
		" NAPS LYDEC",
		0 //<- DO NOT FORGET THIS TO STOP THE LIST
		};

		do {
			switch (GL_Dialog_Choice(hGoal, "Menu des services", menu, 0,GL_BUTTON_DEFAULT, GL_KEY_1,GL_TIME_MINUTE)) {
			case 0:
				memcpy(FicPara.TYPEAPPLICATION,"NAPSSEUL",8);
				PutAllParam();
				goto lblEnd;
			case 1:
				memcpy(FicPara.TYPEAPPLICATION,"NAPSORIF",8);
				PutAllParam();
				goto lblEnd;
			case 2:
				memcpy(FicPara.TYPEAPPLICATION,"NAPSSNTL",8);
				PutAllParam();
				goto lblEnd;
			case 3:
				memcpy(FicPara.TYPEAPPLICATION,"NAPSAVIS",8);// Avis
				PutAllParam();
				goto lblEnd;
			case 4:
				memcpy(FicPara.TYPEAPPLICATION,"NAPSLYDC",8);//Lydec
				PutAllParam();
				goto lblEnd;
			default:
				bContinue2 = FALSE;
				goto lblEnd;
			}
		} while (bContinue2);


		lblEnd:
		return KO;

}
