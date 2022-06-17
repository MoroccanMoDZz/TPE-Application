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

#include "DNS_.h"
#include "Goal.h"


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
#define PIXEL_BOTTOM   40

int ShowEntry (const char *pcTitle, const char *pcText, char *pcInput, byte ucMinDigit, byte ucMaxDigit, byte ucEntryMode);
void DisplayMedium (byte ucLine, byte ucColumn, char *pcText);
void PromptGPRS();
void PromptEthernet();
void ComEthernet();
int APEMV_UI_NumericalEntry(const char *title,const char *text, unsigned char *entryString, const char *entryMask );
void APEMV_UI_MessageDisplayDelay(char *message, int duration);
int administration_naps_ara(void);
int administradom_cmi_ara(void);//Mouna310305
int administra_cmi_ara(void);
int trasamanuel_cmi_ara(void);
int annulation_cmi_ara(void);
int credit_cmi_ara(void);
int duplicata_cmi_ara(void);
int batch_cmi_ara(void);
int report_cmi_ara(void);
int maintenance_cmi_ara(void);
int preauto_cmi_ara(void);
int preautocomp_cmi_ara(void);
int annulpreauto_cmi_ara(void);
int demandesolde_naps_ara(void);
int minireleve_naps_ara(void);
int pinchange_naps_ara(void);
int rechargegsm_naps_ara(void);
int paiement_naps_ara(void);
int paiement_nfois_naps_ara(void);
int paiement_differe_naps_ara(void);
int paiement_pourboire_naps_ara(void);
int paiement_surcout_naps_ara(void);
int dernieretrs_cmi_ara(void);
int autretrs_cmi_ara(void);
int dernierepreaut_cmi_ara(void);
int autrepreauto_cmi_ara(void);
int initparam_cmi_ara(void);
int editparam_cmi_ara(void);

int param_gprs_ara(void);//Mo 10/04/06

int motpassecmr_cmi_ara(void);
int changecomptdom_cmi_ara(void);//Mouna310305
int initialisation_cmi_ara(void);
int testreseau_cmi_ara(void);
int findejournee_cmi_ara(void);
int listenoire_cmi_ara(void);
int totaux_cmi_ara(void);
int journal_cmi_ara(void);
int journalpreauto_cmi_ara(void);
int razram_cmi_ara(void);
int effacetransa_cmi_ara(void);
int effacepreauto_cmi_ara(void);
int razlop_cmi_ara(void);
int edittrsemv_cmi_ara(void);
int totale_cmi_ara_ara(void);
int partielle_cmi_ara(void);
int totale_cmi_ara(void);

int NumCmr_cmi_ara(void);
int NumTrm_cmi_ara(void);
int choix_langue_ara(void);
int ligneautopri_cmi_ara(void);
int lignetelesec_cmi_ara(void);
int ligneautosec_cmi_ara(void);
int abonntelepri_cmi_ara(void);
int abonnautopri_cmi_ara(void);
int abonntelesec_cmi_ara(void);
int abonnautosec_cmi_ara(void);
int delaipreautor_cmi_ara(void);
int parametres_cmi_ara(void);
int devise_cmi_ara(void);
int bin_cmi_ara(void);
int prefixes_cmi_ara(void);
int comptes_cmi_ara(void);
int comm_pm_ara(void);
int comm_pm_rtc_ara(void);
int id_mode_nbe_ara(void);
int id_rlp_nbe_ara(void);
int ip_gprs_nbe_ara(void);
int port_gprs_nbe_ara(void);
int apn_gprs_nbe_ara(void);
int login_gprs_nbe_ara(void);
int pwd_gprs_nbe_ara(void);
int tcpip_nbe_ara(void);
int appli_cmi_ara(void);
int more_function_ara(void);
int appli_emv_ara(void);
//Mo 10/04/06
int GetItemAlphaNumerique_ara( char *sOutput, int OutputLen,char *libelle,unsigned short int);
int ip_gprs_cmi_ara(void);

int port_gprs_cmi_ara(void);
int apn_gprs_cmi_ara(void);
int login_gprs_cmi_ara(void);
int pwd_gprs_cmi_ara(void);
int time_out_param_ara(void);

int transaction_naps_ara(void);
int prepaye_naps_ara(void);
int duplicata_naps_ara_ara(void);
int parametres_naps_ara(void);
int duplicata_naps_ara(void);
int cashadvance_naps_ara(void);
int preautorisation_naps_ara(void);
int service_naps_ara(void);
int journaux_naps_ara(void);
int Activer_ara(void);
int Desactiver_ara(void);
int arabe(void);
int francais(void);
int anglais(void);

void Arabic_ara();

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

static const ST_PRN_LINE xPrinter =
{
	GL_ALIGN_CENTER, GL_ALIGN_CENTER, FALSE, 0, {0, 0, 0, 0}, {0, 0, 0, 0, GL_COLOR_BLACK}, {1, 0, 1, 0}, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_XXLARGE}
};
static const ST_DSP_LINE txGoalFont[] =
{
	{ {GL_ALIGN_CENTER, GL_ALIGN_CENTER, GL_COLOR_WHITE, GL_COLOR_TRANSPARENT,  0,  TRUE, {0, 0, 0, 0}, {0, 0, 0, 0, GL_COLOR_WHITE}, {0, 0, 0, 0}, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_LARGE}}, // Line0
	  {GL_ALIGN_CENTER, GL_ALIGN_CENTER, FALSE,  0,  TRUE, {0, 0, 0, 0}, {0, 0, 0, 0, GL_COLOR_BLACK}, {0, 0, 0, 0}, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_XLARGE}} },
	{ {GL_ALIGN_CENTER, GL_ALIGN_CENTER, GL_COLOR_WHITE, GL_COLOR_TRANSPARENT,  0,  TRUE, {0, 0, 0, 0}, {0, 0, 0, 0, GL_COLOR_WHITE}, {0, 0, 0, 0}, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_LARGE}}, // Line1
	  {GL_ALIGN_CENTER, GL_ALIGN_CENTER, FALSE,  0,  TRUE, {0, 0, 0, 0}, {0, 0, 0, 0, GL_COLOR_BLACK}, {0, 0, 0, 0}, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_MEDIUM}} },
	{ {GL_ALIGN_LEFT,   GL_ALIGN_CENTER, GL_COLOR_WHITE, GL_COLOR_BLUE, 100, FALSE, {1, 3, 1, 3}, {1, 1, 1, 1, GL_COLOR_WHITE}, {3, 0, 3, 0}, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_LARGE}}, // Line2
	  {GL_ALIGN_LEFT,   GL_ALIGN_CENTER, TRUE, 100, FALSE, {2, 2, 2, 2}, {0, 0, 0, 0, GL_COLOR_BLACK}, {0, 0, 0, 0}, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_MEDIUM}} }
};
//T_GL_HGRAPHIC_LIB hGoal=NULL;
/* ========================================================================	*/
/* Constant declaration                                                     */
/* ========================================================================	*/

static const char *tzMenuUser_ara[] =
{
		"\xE2\xC7\xC6\xE5\xC9\x20\xC7\xE4\xE5\xD9\xC7\xE5\xE4\xC7\xCA",					   	//" Menu Transaction",
		"\xE2\xC7\xC6\xE5\xC9\x20\xC7\xE4\xCA\xE1\xE8\xEA\xD6\x20\xC7\xE4\xE5\xD3\xC8\xE2",	//" Menu Préautorisation",
		"\xE2\xC7\xC6\xE5\xC9\x20\xC7\xE4\xCA\xE3\xD1\xC7\xD1",								//" Menu Duplicata",
		"\xE2\xC7\xC6\xE5\xC9\x20\xC7\xE4\xC5\xD9\xCF\xC7\xCF\xC7\xCA",						//" Menu Paramètres",
		"\xE2\xC7\xC6\xE5\xC9\x20\xC7\xE4\xD3\xE4\xE1\x20\xC7\xE4\xE6\xE2\xCF\xEA",			//" Menu CashAdvance",
		"\xE2\xC7\xC6\xE5\xC9\x20\xC7\xE4\xD5\xCD\xE1",										//" Menu Journaux",
		"\xE2\xC7\xC6\xE5\xC9\x20\xC7\xE4\xD4\xC8\xE3\xC9",									//" Menu Réseau",

	NULL
};

static const char *tzMenuUser2_ara[] =
{
		"\xE2\xC7\xC6\xE5\xC9\x20\xC7\xE4\xE5\xD9\xC7\xE5\xE4\xC7\xCA",						//" Menu Transaction",
		"\xE2\xC7\xC6\xE5\xC9\x20\xC7\xE4\xCA\xE1\xE8\xEA\xD6\x20\xC7\xE4\xE5\xD3\xC8\xE2", //" Menu Préautorisation",
		"\xE2\xC7\xC6\xE5\xC9\x20\xC7\xE4\xCA\xE3\xD1\xC7\xD1",							    //" Menu Duplicata",
		"\xE2\xC7\xC6\xE5\xC9\x20\xC7\xE4\xC5\xD9\xCF\xC7\xCF\xC7\xCA",						//" Menu Paramètres",
		"\xE2\xC7\xC6\xE5\xC9\x20\xC7\xE4\xD3\xE4\xE1\x20\xC7\xE4\xE6\xE2\xCF\xEA",			//" Menu CashAdvance",
		"\xE2\xC7\xC6\xE5\xC9\x20\xC7\xE4\xD5\xCD\xE1",										//" Menu Journaux",
	NULL
};
static const char *tzMenuPrepaye_ara[] =
{
	" RECHARGE CARTE",
	" DECHARGE CARTE",
	" TR. CARTE A CARTE",
	" TR. CARTE A CASH",
	NULL
};

static const char *tzMenuDialogBox_ara[] =
{
		"\xE5\xD9\xC7\xE5\xE4\xC9\x20\xEA\xCF\xE8\xEA\xC9",							  		//" Transaction Manuelle",
		"\xC5\xE4\xDA\xC7\xC1",													      		//" Annulation",
		"\xE2\xD1\xD6",																  		//" Crédit",
		"\xCC\xE5\xD9\x20\xE8\xCA\xCD\xE8\xEA\xE4\x20\xC7\xE4\xE5\xD9\xC7\xE5\xE4\xC7\xCA",	//" Télécollecte NAPS",
	NULL
};

static const char *tzMenuWidget_ara[] =
{
		"\xC2\xEE\xCE\xD1\x20\xE5\xD9\xC7\xE5\xE4\xC9",					//" Dernière Transaction",
		"\xE5\xD9\xC7\xE5\xE4\xC9\x20\xC3\xCE\xD1\xE9",					//" Autre Transaction",
		"\xC2\xEE\xCE\xD1\x20\xCA\xE1\xE8\xEA\xD6\x20\xE5\xD3\xC8\xE2",	//" Dernière Préautorisation",
		"\xCA\xE1\xE8\xEA\xD6\x20\xE5\xD3\xC8\xE2\x20\xC2\xEE\xCE\xD1",	//" Autre Préautorisation",
	NULL
};
static const char *tzMenuAdmin_ara[] =
{
		"\xC5\xD9\xCF\xC7\xCF\xC7\xCA\x20\xC8\xD1\xE8\xCA\xE8\xE3\xE8\xE4\x20\xC7\xE4\xC5\xE6\xCA\xD1\xE6\xCA",	//" IP MODE",
	"\xC5\xD9\xCF\xC7\xCF\xC7\xCA\x20\xCE\xC7\xD5\xC9",															//" Edition Paramètres",
	"\xCA\xCD\xCF\xEA\xCA\x20\xC7\xE4\xC5\xD9\xCF\xC7\xCF\xC7\xCA",												//" Réferencement NAPS",
	"\xE5\xD3\xCD\x20\xC7\xE4\xCF\xC7\xE3\xD1\xC9",																//" Raz Ram",
	"\xC7\xE4\xCE\xCF\xE5\xC7\xCA\x20\xC7\xE4\xC5\xCF\xC7\xD1\xEA\xC9",											//" Admin Services",
	NULL
};
static const char *tzMenuPreauto_ara[] =
{
		"\xC7\xE4\xCA\xE1\xE8\xEA\xD6\x20\xC7\xE4\xE5\xD3\xC8\xE2",							//" Préautorisation",
		"\xE2\xE3\xE5\xE4\xC9\x20\xC7\xE4\xCA\xE1\xE8\xEA\xD6\x20\xC7\xE4\xE5\xD3\xC8\xE2",	//" Complément Préautorisation",
		"\xC5\xE4\xDA\xC7\xC1\x20\xC7\xE4\xCA\xE1\xE8\xEA\xD6\x20\xC7\xE4\xE5\xD3\xC8\xE2",	//" Annulation Préautorisation ",
		"\xCA\xD7\xE7\xEA\xD1\x20\xC7\xE4\xCA\xE1\xE8\xEA\xD6\x20\xC7\xE4\xE5\xD3\xC8\xE2",	//" Purge Préautorisation",
	NULL
};
static const char *tzMenuService_ara[] =
{
	"\xD7\xE4\xC8\x20\xC7\xE4\xD1\xD5\xEA\xCF",								//" Demande Solde",
	" Mini Releve",
	"\xCA\xDA\xEA\xEA\xD1\x20\xC7\xE4\xD1\xE2\xE5\x20\xC7\xE4\xD3\xD1\xEA",	//" Pin Change",
	" Recharge Gsm",
	" Paiement NAPS",
	NULL
};
static const char *tzMenuTotal_ara[] =
{
		"\xD5\xCD\xEA\xE1\xC9\x20\xC7\xE4\xE5\xD9\xC7\xE5\xE4\xC7\xCA",						//" Journal Trs NAPS",
		"\xD5\xCD\xEA\xE1\xC9\x20\xC7\xE4\xCA\xE1\xE8\xEA\xD6\x20\xC7\xE4\xE5\xD3\xC8\xE2",	//" Journal Préautorisation ",
		"\xD5\xCD\xEA\xE1\xC9\x20\xC5\xCC\xE5\xC7\xE4\xEA\xC9",								//" Total ",
	NULL
};

static const char *tzMenuReseau_ara[] =
{
	" Start GPRS",
	" Stop GPRS",
	" Restart GPRS",
	NULL
};
static const char *MenuAnnulPre_ara[] =
{
		"\xC5\xE4\xDA\xC7\xC1\x20\xE3\xE4\xEA",		//" Annulation Totale",
		"\xC5\xE4\xDA\xC7\xC1\x20\xCC\xD2\xC6\xEA",	//" Annulation Partielle",
	NULL
};
static const char *MenuConfiguration_ara[] =
{
	"\xD9\xE6\xE8\xC7\xE6\x20\xC8\xD1\xE8\xCA\xE8\xE3\xE8\xE4\x20\xC7\xE4\xC5\xE6\xCA\xD1\xE6\xCA",	//" Adresse IP",
	"\xC7\xE4\xE5\xCF\xCE\xE4",																		//" Port",
	" APN",
	"\xC7\xE4\xE5\xD3\xCA\xCE\xCF\xE5",																//" Utilisateur",
	"\xE3\xE4\xE5\xC9\x20\xC7\xE4\xD3\xD1",															//" Mot de Passe",
	//" Time out",
	NULL
};
static const char *MenuParametres_ara[] =
{
	" Paramètres",
	" Devise",
	" Bin",
	" Prefixes",
	" Comptes",
	NULL
};

static const char *MenuInit_Param_ara[] =
{
		"\xD1\xE2\xE5\x20\xC7\xE4\xCA\xC7\xCC\xD1",									//" Num Commerçant",
		"\xD1\xE2\xE5\x20\xE5\xCD\xD7\xC9\x20\xC7\xE4\xCF\xE1\xD9",					//" Num Terminal",
		"\xE2\xC7\xC6\xE5\xC9\x20\xC5\xCE\xCA\xEA\xC7\xD1\x20\xC7\xE4\xE4\xDA\xC9",	//" Choisir une langue",
	NULL
};

const NavigationListe NavigationEmv_ara[] =
{
   {MESS49,          (PFONCNAVI) appli_cmi_ara},
//   {MESS50,          (PFONCNAVI) appli_emv_ara},
#ifdef _TOOLWARE_
  ,{MESS40,         (PFONCNAVI) CUMORE_Demo_Mode}
#endif
};

extern t_topstack *alternateNbr;
extern int AlternanceIsOn;
extern int GPRSDisabled;









const NavigationListe NavigationEmv_cmi_ara[] =
{

		{MESS58,          (PFONCNAVI) transaction_naps_ara},
		{MESS59,          (PFONCNAVI) prepaye_naps_ara},
		{MESS7,           (PFONCNAVI) duplicata_naps_ara},
		{MESS2,           (PFONCNAVI) administration_naps_ara},
		{MESS65,          (PFONCNAVI) cashadvance_naps_ara},
		{MESS11,          (PFONCNAVI) preautorisation_naps_ara},
		{MESS67,          (PFONCNAVI) service_naps_ara},
		{MESS24,          (PFONCNAVI) journaux_naps_ara},


#ifdef _TOOLWARE_
	,{CMN_MESS_MERCHANT_TOOLWARE, (PFONCNAVI) CUMORE_Demo_Mode}
#endif   
};

static const NavigationListe Navigationduplicata_cmi_ara[] =
{
   {MESS14,          (PFONCNAVI) dernieretrs_cmi_ara},
   {MESS15,          (PFONCNAVI) autretrs_cmi_ara},
   {MESS16,          (PFONCNAVI) dernierepreaut_cmi_ara},
   {MESS17,          (PFONCNAVI) autrepreauto_cmi_ara},
};

static const NavigationListe Navigationadministrationdom_cmi_ara[] =
{
   {MESS18,          (PFONCNAVI) initparam_cmi_ara},
   {MESS19,          (PFONCNAVI) editparam_cmi_ara},
   {MESS20,          (PFONCNAVI) motpassecmr_cmi_ara},
   {MESS51,          (PFONCNAVI) changecomptdom_cmi_ara},
   {MESS52,          (PFONCNAVI) comm_pm_ara},
};

static const NavigationListe Navigationadministration_cmi_ara[] =
{
   {MESS18,          (PFONCNAVI) initparam_cmi_ara},
   {MESS52,          (PFONCNAVI) comm_pm_ara},
   {MESS19,          (PFONCNAVI) editparam_cmi_ara},
//   {MESS20,          (PFONCNAVI) motpassecmr_cmi_ara},
};

static const NavigationListe Navigationbatch_cmi_ara[] =
{
   {MESS21,          (PFONCNAVI) initialisation_cmi_ara},
   {MESS22,          (PFONCNAVI) findejournee_cmi_ara}/*,
   {MESS23,          (PFONCNAVI) listenoire_cmi_ara},*/
};

static const NavigationListe Navigationreport_cmi_ara[] =
{
   {MESS24,          (PFONCNAVI) totaux_cmi_ara},
   {MESS25,          (PFONCNAVI) journal_cmi_ara},
   {MESS26,          (PFONCNAVI) journalpreauto_cmi_ara},
};

static const NavigationListe Navigationmaintenance_cmi_ara[] =
{
   {MESS27,          (PFONCNAVI) razram_cmi_ara},
   {MESS28,          (PFONCNAVI) effacetransa_cmi_ara},
   {MESS29,          (PFONCNAVI) effacepreauto_cmi_ara},
   {MESS30,          (PFONCNAVI) razlop_cmi_ara},
   {MESS31,          (PFONCNAVI) edittrsemv_cmi_ara},
};

static const NavigationListe Navigationannulpreauto_cmi_ara[] =
{
   {MESS32,          (PFONCNAVI) totale_cmi_ara},
   {MESS33,          (PFONCNAVI) partielle_cmi_ara},
};

static const NavigationListe NavigationServices_ara[] =
{
   {MESS88,          (PFONCNAVI) Desactiver_ara},
   {MESS89,          (PFONCNAVI) Activer_ara},
};

static const NavigationListe Navigationinitparam_cmi_ara[] =
{
   {MESS34,          (PFONCNAVI) NumCmr_cmi_ara},
   {MESS35,          (PFONCNAVI) NumTrm_cmi_ara},
   {MESS36,          (PFONCNAVI) choix_langue_ara},

/*
   {MESS36,          (PFONCNAVI) ligneautopri_cmi_ara},
   {MESS37,          (PFONCNAVI) lignetelesec_cmi_ara},
   {MESS38,          (PFONCNAVI) ligneautosec_cmi_ara},
   {MESS39,          (PFONCNAVI) abonntelepri_cmi_ara},
   {MESS40,          (PFONCNAVI) abonnautopri_cmi_ara},
   {MESS41,          (PFONCNAVI) abonntelesec_cmi_ara},
   {MESS42,          (PFONCNAVI) abonnautosec_cmi_ara},
   {MESS43,          (PFONCNAVI) delaipreautor_cmi_ara},
 */
};

static const NavigationListe Navigationeditparam_cmi_ara[] =
{
   {MESS44,          (PFONCNAVI) parametres_cmi_ara},
   {MESS45,          (PFONCNAVI) devise_cmi_ara},
   {MESS46,          (PFONCNAVI) bin_cmi_ara},
   {MESS47,          (PFONCNAVI) prefixes_cmi_ara},
   {MESS48,          (PFONCNAVI) comptes_cmi_ara},
};

static const NavigationListe Navigationparam_gprs_ara[] =
{
   {MESS53,          (PFONCNAVI) ip_gprs_cmi_ara},
   {MESS54,          (PFONCNAVI) port_gprs_cmi_ara},
   {MESS55,          (PFONCNAVI) apn_gprs_cmi_ara},
   {MESS56,          (PFONCNAVI) login_gprs_cmi_ara},
   {MESS57,          (PFONCNAVI) pwd_gprs_cmi_ara},
  // {MESS58,          (PFONCNAVI) time_out_param_ara},

};

static const NavigationListe Navigationbatchparametres_naps_ara[] =
{
   {MESS53,          (PFONCNAVI) comm_pm_ara},
   {MESS19,          (PFONCNAVI) initparam_cmi_ara},
   {MESS21,          (PFONCNAVI) initialisation_cmi_ara},
   {MESS27,          (PFONCNAVI) razram_cmi_ara},
};

static const NavigationListe Navigationtransaction_naps_ara[] =
{
   {MESS3,          (PFONCNAVI) trasamanuel_cmi_ara},
   //{MESS3,          (PFONCNAVI) Arabic_ara},
   {MESS4,          (PFONCNAVI) annulation_cmi_ara},
   {MESS6,          (PFONCNAVI) credit_cmi_ara},
   {MESS22,          (PFONCNAVI) findejournee_cmi_ara},
};

static const NavigationListe Navigationpreauto_naps_ara[] =
{
   {MESS11,          (PFONCNAVI) preauto_cmi_ara},
   {MESS12,          (PFONCNAVI) preautocomp_cmi_ara},
   {MESS13,          (PFONCNAVI) annulpreauto_cmi_ara},
//   {MESS22,          (PFONCNAVI) PurgePreauto_ara},
};

static const NavigationListe Navigationservice_naps_ara[] =
{
   {MESS68,          (PFONCNAVI) demandesolde_naps_ara},
   {MESS69,          (PFONCNAVI) minireleve_naps_ara},
   {MESS70,          (PFONCNAVI) pinchange_naps_ara},
   {MESS71,          (PFONCNAVI) rechargegsm_naps_ara},
   {MESS72,          (PFONCNAVI) paiement_naps_ara},
};

static const NavigationListe Navigationpaiement_naps_ara[] =
{
   {MESS74,          (PFONCNAVI) paiement_nfois_naps_ara},
   {MESS77,          (PFONCNAVI) paiement_differe_naps_ara},
   {MESS78,          (PFONCNAVI) paiement_pourboire_naps_ara},
   {MESS81,          (PFONCNAVI) paiement_surcout_naps_ara},
};


const unsigned char CUMORE_CMI_TRUE_ara []   = {0x00, 0x01};
const unsigned char CUMORE_CMI_FALSE_ara []  = {0x00, 0x00};

const char mess_cmi_NumCmr_id_ara []		= "Num. Commercant :";
const char mess_cmi_NumTrm_id_ara []  		= "Num. Terminal :";
const char mess_cmi_ligneautopri_id_ara []  = "LIGNE.AUTO.PRI. :";
const char mess_cmi_lignetelesec_id_ara []  = "LIGNE.TELE.SEC. :";
const char mess_cmi_ligneautosec_id_ara []  = "LIGNE.AUTO.SEC. :";
const char mess_cmi_abonntelepri_id_ara []  = "ABONNEE.TELE.PRI. :";
const char mess_cmi_abonnautopri_id_ara []  = "ABONNEE.AUTO.PRI. :";
const char mess_cmi_abonntelesec_id_ara []  = "ABONNEE.TELE.SEC. :";
const char mess_cmi_abonnautosec_id_ara []  = "ABONNEE.AUTO.SEC. :";
const char mess_cmi_delaipreauto_id_ara []  = "DELAI PRE AUTO. :";

const char mess_nbe_modegsm_id_ara []		= "MODE :";
const char mess_nbe_rlp_id_ara []			= "RLP (0/1):";
const char mess_nbe_version_id_ara []		= "VERSION :";

const char mess_nbe_type_id_ara []			= "RTC(0)GSM(1)GP(2): ";

unsigned short Bypass_Required;
unsigned short TransactionForcedOnline;
L	g_l_annul_pre_ara=0;

extern unsigned short CUSERV_New_Parameters;

extern unsigned long ulTransactionType;
extern unsigned long ulTransactionTypeidlemessage;

unsigned char RetransmitIndicator;

/*%DEBUT_MODIF 18/07/02 */
extern StructListe       Menu;
/*%FIN_MODIF 18/07/02 */

extern	int SLC_Saisie_Numerique(unsigned char *saisie,char *title, unsigned char *chaine, TAB_ENTRY_STRING *table);
char *ReadMessage_cmi(int num,int langue); //KT1


/*-----------------------------------------------------------------
 * Fonction   :more_function_ara
 * objet      :Menu globale
 *
 * Parametre  :
 * Retour     :
 *-----------------------------------------------------------------
*/
int more_function_ara(void) {
	// Local variables *
	// *****************

		Ushort		nbr_preauto;
		lignePreAut	preauto ;
		T_GL_HGRAPHIC_LIB hGoal = APEMV_UI_GoalHandle();
		bool bContinue = TRUE, bContinue2 = TRUE, bContinue3 = TRUE;
		int iRet;
		char *pMenuUser;
		pMenuUser = tzMenuUser_ara;

		if((IsRadioGPRS() != 1) && (IsRadio3G() != 1))//belhadeg V5.1
			pMenuUser = tzMenuUser2_ara;

		if (AlternanceIsOn) {
			Telium_Kill(alternateNbr, "");
			AlternanceIsOn = 0;
		}

	// Menu management
	// ***************
		do {

			switch (GL_Dialog_Menu(hGoal, "\xC7\xE4\xE2\xC7\xC6\xE5\xC9\x20\xC7\xE4\xD1\xC6\xEA\xD3\xEA\xC9", pMenuUser, 0, GL_BUTTON_DEFAULT,
							GL_KEY_1, GL_TIME_MINUTE)) {
			case 0:
				do {
					switch (iRet = GL_Dialog_Menu(hGoal, tzMenuUser_ara[0],tzMenuDialogBox_ara, 0, GL_BUTTON_DEFAULT, GL_KEY_1,GL_TIME_MINUTE)) {
					case 0:
						if (FicPara.MenuTPE[6] == '1') {
							trasamanuel_cmi_ara();
							//Arabic_ara();


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
					switch (iRet = GL_Dialog_Menu(hGoal, tzMenuUser_ara[1],
							tzMenuPreauto_ara, 0, GL_BUTTON_DEFAULT, GL_KEY_1,
							GL_TIME_MINUTE)) {
					case 0:
						preauto_cmi_ara();
						goto lblEnd;
					case 1:
						preautocomp_cmi_ara();
						goto lblEnd;
						//break;
					case 2:
						if (control_badge() != OK)
							return KO ;
						annulpreauto_cmi_ara();
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
					switch (iRet = GL_Dialog_Menu(hGoal, tzMenuUser_ara[2],tzMenuWidget_ara, 0, GL_BUTTON_DEFAULT, GL_KEY_1,
							GL_TIME_MINUTE)) {
					case 0:
						dernieretrs_cmi_ara();
						break;
					case 1:
						autretrs_cmi_ara();
						break;
					case 2:
						dernierepreaut_cmi_ara();
						break;
					case 3:
						autrepreauto_cmi_ara();
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

					switch (iRet = GL_Dialog_Menu(hGoal, tzMenuUser_ara[3], tzMenuAdmin_ara,
							0, GL_BUTTON_DEFAULT, GL_KEY_1, GL_TIME_MINUTE)) {
					case 0:
						do {
							switch (iRet = GL_Dialog_Menu(hGoal,"Communication",
									MenuConfiguration_ara, 0, GL_BUTTON_DEFAULT, GL_KEY_1,
									GL_TIME_MINUTE)) {
							case 0:
								ip_gprs_cmi_ara();
								break;
							case 1:
								port_gprs_cmi_ara();
								break;
							case 2:
								apn_gprs_cmi_ara();
								break;
							case 3:
								login_gprs_cmi_ara();
								break;
							case 4:
								pwd_gprs_cmi_ara();
								break;
	//						case 5:
	//							time_out_param_ara();
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
							switch (iRet = GL_Dialog_Menu(hGoal, "\xD6\xC8\xD7\x20\xC7\xE4\xC5\xD9\xCF\xC7\xCF\xC7\xCA\x20\xC7\xE4\xC5\xCF\xC7\xD1\xEA\xC9",
									MenuInit_Param_ara, 0, GL_BUTTON_DEFAULT,GL_KEY_1,
									GL_TIME_MINUTE)) {
							case 0:
								NumCmr_cmi_ara();
								break;
							case 1:
								NumTrm_cmi_ara();
								break;
							case 2:
								choix_langue_ara();
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
						initialisation_cmi_ara();
						//FicPara.MCC="6010";
						//sprintf(FicPara.MCC, "6010");
						break;
					case 3:
						if (control_badge() != OK)
							return KO ;
						if (mdp_merchant_cmi_ara() != CR_SAISIE_OK) {
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
	//					if (control_badge() != OK)
	//						break;
						//Admin_Services();
						GL_SampleDialogChoice_ara();
						goto lblEnd;
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
					cashadvance_naps_ara();
				} else {
					CUMORE_CMI_AFFICHE_MESSAGE(22, 3, 1000);
				}
				goto lblEnd;
				//break;

			case 5:
				do {
					switch (iRet = GL_Dialog_Menu(hGoal, tzMenuUser_ara[5], tzMenuTotal_ara,
							0, GL_BUTTON_DEFAULT, GL_KEY_1, GL_TIME_MINUTE)) {
					case 0:
						journal_cmi_ara();
						break;
					case 1:
						journalpreauto_cmi_ara();
						break;
					case 2:
						totaux_cmi_ara();
						break;
	//				case 3:
	//					GL_SampleOrientation_ara();
	//					break;
					default:
						bContinue2 = FALSE;
						break;

					}
				} while (bContinue2);
				bContinue2 = TRUE;
				CHECK(iRet==GL_KEY_CORRECTION, lblEnd);
				break;
			case 6:
				CHECK((IsRadioGPRS() == 1) || (IsRadio3G() == 1), lblNoRadio);//belhadeg V5.1
				do {
					switch (iRet = GL_Dialog_Menu(hGoal, tzMenuUser_ara[6], tzMenuReseau_ara,
							0, GL_BUTTON_DEFAULT, GL_KEY_1, GL_TIME_MINUTE)) {

					case 0:
						GPRSDisabled = FALSE;
						StartGprs_naps();
						break;
					case 1:
						// Disconnect GPRS jrizki 21032019
						GPRSDisabled = TRUE;
						StopGprs_naps();
						break;
					case 2:
						// Restart GPRS jrizki 21032019
						StopGprs_naps();

						StartGprs_naps();
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
int appli_cmi_ara(void)
{

    return 0;
}

int appli_emv_ara(void)
{
  return 0;
}

int duplicata_cmi_ara(void)
{
	return 0;
}

int mdp_merchant_cmi_ara(void)
{

	return CR_SAISIE_NOK;
}
/*-----------------------------------------------------------------
 * Fonction   :Arabic_ara
 * objet      :Switch to arabic
 *
 * Parametre  :
 * Retour     :
 *-----------------------------------------------------------------
*/
void Arabic_ara(void)
{
	// Local variables
    // ***************
	T_GL_HGRAPHIC_LIB hGoal_ara=APEMV_UI_GoalHandle();
	T_GL_HWIDGET hScreen_ara=NULL;
	ST_DSP_LINE xDspLine;
	T_GL_HWIDGET hDocument=NULL;
	ST_PRN_LINE xPrnLine;
	byte p; // Printer line index
	bool bFontOk;
	int iRet;
		                      // *********************
	                           //      ISO8859-6      *
//	                           // *********************
//	bFontOk = IngenicoFontExist(FONT_GOAL_ORIENTA_NO, "GOAL_ORIENTA_NO"); // Check if Arabic/Hebrew (ISO 8859-6/8 No) font loaded?
//	CHECK(bFontOk==true, lblFontMissing);
//
	hScreen_ara = GoalCreateScreen(hGoal_ara, txGoalFont, NUMBER_OF_LINES(txGoalFont), GL_ENCODING_ISO_8859_6);
	CHECK(hScreen_ara!=NULL, lblKO);                                   // Create screen to ISO_8859_6 and clear it

	iRet = GoalClrScreen(hScreen_ara, GL_COLOR_TRANSPARENT, KEY_VALID, false);
	CHECK(iRet>=0, lblKO);
	iRet = GoalDspLine(hScreen_ara, 0, "FONT ISO8859-6", &txGoalFont[0], 0, false);
	CHECK(iRet>=0, lblKO);
    iRet = GoalDspLine(hScreen_ara, 1, "Arabic message", &txGoalFont[1], 0, false);
	CHECK(iRet>=0, lblKO);
	// Display Arabic "Welcome" ISO8859 coded
	// **************************************
	xDspLine = txGoalFont[1];
	xDspLine.xColor.eTextAlign=GL_ALIGN_RIGHT;
	xDspLine.xBW.eTextAlign=GL_ALIGN_RIGHT;
	xDspLine.xBW.xFont.eScale=GL_SCALE_XLARGE;
	iRet = GoalDspLine(hScreen_ara, 2, 		"\x20\xE2\xC7\xC6\xE5\xC9\x20\xC7\xE4\xE5\xD9\xC7\xE5\xE4\xC7\xCA", &xDspLine, 0, true);
	CHECK(iRet>=0, lblKO);


	GoalGetKey(hScreen_ara, hGoal_ara, true, 30*1000, true);               // Wait for key pressed/touched (shortcut)
	//APEMV_UI_MessageDisplayDelay("\xC7\xE4\xD3\xE4\xC7\xE5\x20\xD9\xE4\xEA\xE3\xE5",5);
	goto lblEnd;

	// Errors treatment
    // ****************
lblKO:                                                             // None-classified low level error
	GL_Dialog_Message(hGoal_ara, NULL, "Processing Error", GL_ICON_ERROR, GL_BUTTON_VALID, 5*1000);
	goto lblEnd;

lblEnd:
	if (hScreen_ara)
		GoalDestroyScreen(&hScreen_ara);                               // Destroy screen
                       // Destroy document
}

int trasamanuel_cmi_ara(void)
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

	transa_client();
	//APEMV_UI_MessageDisplayDelay ("transaction Man 4", 2);

	FicPara.vCam0 = 0; //Retour à la normale


	PutAllTra();
	PutAllParam();

	return 0;
}


int administradom_cmi_ara(void)
{

  return 0;
}


//Mouna310305_fin

int administra_cmi_ara(void)
{
  return 0;
}



int	annulation_naps_ara(void)
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

int annulation_cmi_ara(void)
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
	g_l_annul_pre_ara = 0;
	annulation_naps_ara();
	PutAllTra();
	PutAllParam();
	return 0;
}
int credit_naps_ara(void){

	if (Credit() == OK)
	{
		ulTransactionType = CUSERV_REFUND_TRANSACTION;

		CUMORE_PaiementFctExt(CUSERV_REFUND_TRANSACTION, FALSE, (unsigned long)0, (int)0);

		ulTransactionType = CUSERV_DEBIT_TRANSACTION;
	}
	return 0;
}
int credit_cmi_ara(void)
{
	return 0;
}
int batch_cmi_ara(void)
{

  return 0;
}

int report_cmi_ara(void)
{
  return 0;
}
int maintenance_cmi_ara(void)
{
  return 0;
}
int preauto_cmi_ara(void)
{
  return 0;
}

int preautocomp_cmi_ara(void)
{

	return 0;
}
int	PSQ_Donner_langue_ara(void) //KT1
{
	return C_LG_FRENCH;
}

int annulpreauto_cmi_ara(void)  //RDS
{
	T_GL_HGRAPHIC_LIB hGoal = APEMV_UI_GoalHandle();
	switch (GL_Dialog_Menu(hGoal, "Annulation Préautorisation", MenuAnnulPre_ara,0, GL_BUTTON_DEFAULT, GL_KEY_1, GL_TIME_MINUTE)) {
	case 0:
		totale_cmi_ara();
		break;
	case 1:
		partielle_cmi_ara();
		break;
	default:

		break;
	}
	return 0;
}


int demandesolde_naps_ara(void)
{
	  return 0;
}

int minireleve_naps_ara(void)
{
	  return 0;
}

int pinchange_naps_ara(void)
{
	  return 0;
}

int rechargegsm_naps_ara(void)
{
	  return 0;
}

int paiement_classique_ara()
{

}

int paiement_naps_ara(void)
{
	unsigned char   i;
	T_GL_WCHAR choice  = 0;

	InitDefaut(_ON_,1);

	StateHeader(0);
	/* table init from const */
	Menu.Fenetre.titre          = (unsigned char *)ReadMessage_cmi(MESS67,PSQ_Donner_langue_ara());
	for (i=0 ; i < ELVEC(Navigationpaiement_naps_ara) ; i++ )
	{
	Menu.tab[i] = (unsigned char *) ReadMessage_cmi ( Navigationpaiement_naps_ara[i].itemlabel ,PSQ_Donner_langue_ara());
	}
	/* call navigation time-out 30 secs */
	Menu.tab[i] = NULL;
	choice = (int)APEMV_UI_MenuSelectTopic((const char*)Menu.Fenetre.titre,  (const char*)Menu.tab, TRUE);
	if (choice != -1)
	{
		Navigationpaiement_naps_ara[choice].function();
	}

	StateHeader(1);
	PutAllParam();
	return OK;
}
int	Annul_preautor_naps_ara(UC Type_ann_preauto)
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
int paiement_nfois_naps_ara(void)
{
	 return 0;
}
int paiement_differe_naps_ara(void)
{
		return 0;
}
int paiement_pourboire_naps_ara(void)
{
	return 0;
}
int paiement_surcout_naps_ara(void)
{

	return 0;
}
int totale_cmi_ara(void)
{
	g_l_annul_pre_ara = 1;
	Annul_preautor_naps_ara(ANNUL_PREAUTOR_TOTAL);
	PutAllPreEmetteur();
	//PutAllPre();
	return 0;
}

int partielle_cmi_ara(void)
{
	g_l_annul_pre_ara = 1;
	Annul_preautor_naps_ara(ANNUL_PREAUTOR_PARTIEL);
	//PutAllPreEmetteur();
	PutAllPre();
	return 0;
}

int initparam_cmi_ara(void)
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
  Menu.Fenetre.titre          = (unsigned char *)ReadMessage_cmi(MESS18,PSQ_Donner_langue_ara());
  for (i=0 ; i < ELVEC(Navigationinitparam_cmi_ara) ; i++ )
  {
    Menu.tab[i] = (unsigned char *) ReadMessage_cmi ( Navigationinitparam_cmi_ara[i].itemlabel ,PSQ_Donner_langue_ara());
  }
  /* call navigation time-out 30 secs */
  Menu.tab[i] = NULL;

  choice = (int)APEMV_UI_MenuSelectTopic((const char*)Menu.Fenetre.titre,  (const char*)Menu.tab, TRUE);
  if (choice != -1)
  {
      Navigationinitparam_cmi_ara[choice].function();
  }
//Mouna310305_dÃ©but
  if(Fic_compte_dom.Indice_fin>1)
	  return administradom_cmi_ara();
  else
	  return administra_cmi_ara();
//Mouna310305_fin
}

int editparam_cmi_ara(void)
{
  unsigned char   i;
  T_GL_WCHAR choice  = 0;

  InitDefaut(_ON_,1);
  /* table init from const */
  Menu.Fenetre.titre          = (unsigned char *)ReadMessage_cmi(MESS19,PSQ_Donner_langue_ara());
  for (i=0 ; i < ELVEC(Navigationeditparam_cmi_ara) ; i++ )
  {
    Menu.tab[i] = (unsigned char *) ReadMessage_cmi ( Navigationeditparam_cmi_ara[i].itemlabel ,PSQ_Donner_langue_ara());
  }
  /* call navigation time-out 30 secs */
  Menu.tab[i] = NULL;

  choice = (int)APEMV_UI_MenuSelectTopic((const char*)Menu.Fenetre.titre,  (const char*)Menu.tab, TRUE);
  if (choice != -1)
  {
      Navigationeditparam_cmi_ara[choice].function();
  }
//Mouna310305_dÃ©but
  if(Fic_compte_dom.Indice_fin>1)
	  return administradom_cmi_ara();
  else
	  return administra_cmi_ara();
//Mouna310305_fin
}

//Mo 10/04/06 ////////////////////////////////////////////////////////////////////////
int param_gprs_ara(void)
{
  unsigned char   i;
  T_GL_WCHAR choice  = 0;

  InitDefaut(_ON_,1);
  /* table init from const */
  Menu.Fenetre.titre          = (unsigned char *)ReadMessage_cmi(MESS19,PSQ_Donner_langue_ara());
  for (i=0 ; i < ELVEC(Navigationparam_gprs_ara) ; i++ )
  {
    Menu.tab[i] = (unsigned char *) ReadMessage_cmi ( Navigationparam_gprs_ara[i].itemlabel ,PSQ_Donner_langue_ara());
  }
  /* call navigation time-out 30 secs */
  Menu.tab[i] = NULL;

  choice = (int)APEMV_UI_MenuSelectTopic((const char*)Menu.Fenetre.titre,  (const char*)Menu.tab, TRUE);
  if (choice != -1)
  {
      Navigationparam_gprs_ara[choice].function();
  }
//Mouna310305_dÃ©but
  if(Fic_compte_dom.Indice_fin>1)
	  return administradom_cmi_ara();
  else
	  return administra_cmi_ara();
//Mouna310305_fin
}
//Mo 10/04/06

int dernieretrs_cmi_ara(void)
{
	if(duplicata(1)==OK)
		strncpy(StructInfoCaisse[1].CRPIN,"000",3);
	PutAllTra();
	PutAllParam();
//	return duplicata_cmi();
	return 0;
}
int autretrs_cmi_ara(void)
{
	if(duplicata(0)==OK)
		strncpy(StructInfoCaisse[1].CRPIN,"000",3);
	PutAllTra();
	PutAllParam();
//	return duplicata_cmi();
	return 0;
}
int dernierepreaut_cmi_ara(void)
{
	duplicPre(1);
	PutAllPre();
//	return duplicata_cmi();
	return 0;
}
int autrepreauto_cmi_ara(void)
{
	duplicPre(0);
	PutAllPre();
//	return duplicata_cmi();
	return 0;
}
int motpassecmr_cmi_ara(void)
{
	Change_Merchant_Pwd();
//Mouna310305_dÃ©but
  if(Fic_compte_dom.Indice_fin>1)
	  return administradom_cmi_ara();
  else
	  return administra_cmi_ara();
//Mouna310305_fin
  return 0;
}
int initialisation_cmi_ara(void)
{

	if(Teleparametrage(1,0)==OK)
	{
		//affectation de la date serveur
		ExploiField7();

		Imprime_Param();
		strncpy(StructInfoCaisse[0].CRPIN,"000",3);
		strncpy(StructInfoCaisse[1].CRPIN,"000",3);
	}
	else
	{
		strncpy(StructInfoCaisse[0].CRPIN,"306",3);
		strncpy(StructInfoCaisse[1].CRPIN,"306",3);
	}
	//hang_up(typehangup,g_trace);

	PutAllParam();

	return 0;
}
int testreseau_cmi_ara(void)
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
int findejournee_cmi_ara(void)
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

	else {
		if (FicTra.nombre_tra == 0)
			Imprime_ticket_telezero();
		else {
			Imprime_ticket_teleechou();
			//FicPara.Top_TelAuto = 0;
		}
	}


	PutAllTra();
	PutAllParam();
	return 0;
}
int listenoire_cmi_ara(void)
{
	return 0;
}
int totaux_cmi_ara(void)
{
	ImprimeTelecollecte(1,0);    //(2,0) ==> ( 1,0) correction journal
//	return report_cmi();
	return 0;
}
int journal_cmi_ara(void)
{
	JournalEdit();
//	return report_cmi();
	return 0;
}
int journalpreauto_cmi_ara(void)
{
	JournalEditPre();
//	return report_cmi();
	return 0;
}
int razram_cmi_ara(void)
{
	return 0;
}
int effacetransa_cmi_ara(void)
{
	if(Affich_ecr_pwd(BANK_PWD,4)!=OK)
		return OK;
	if(Valider(68,21)!=KEY_ENTER)
		return OK;
	Init_Tra();
//	return maintenance_cmi();
	return 0;
}
int effacepreauto_cmi_ara(void)
{

	return 0;
}
int razlop_cmi_ara(void)
{
	return 0;
}
int edittrsemv_cmi_ara(void)
{
	if(Affich_ecr_pwd(BANK_PWD,4)!=OK)
		return OK;
	Print_Last_Trs_Emv_Rej();
//	return maintenance_cmi();
	return 0;
}
int GetInfo_Goal_ara (char *chaine, char * vide,char *saisie,byte nb_min,byte nb_max,byte mask)
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

int SLC_Saisie_Numerique_ara(unsigned char *saisie, char *title,unsigned char *chaine, TAB_ENTRY_STRING *table)
{
	return GetInfo_Goal_ara(chaine, title, saisie, table->nb_min, table->nb_max,table->mask);
}
int NumCmr_cmi_ara(void)
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
		cr = SLC_Saisie_Numerique_ara (NumCmr_id, "",(unsigned char *) mess_cmi_NumCmr_id_ara, (TAB_ENTRY_STRING *)&t_entry_id);
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

	return NumTrm_cmi_ara();
}


int NumTrm_cmi_ara(void)
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
		cr = SLC_Saisie_Numerique_ara (NumTrm_id,"", (unsigned char *) mess_cmi_NumTrm_id_ara, (TAB_ENTRY_STRING *)&t_entry_id);
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
  	return choix_langue_ara();
}
/*-----------------------------------------------------------------
 * Fonction   :choix_langue_ara
 * objet      :
 *
 * Parametre  :
 * Retour     :
 *-----------------------------------------------------------------
*/
int choix_langue_ara(void)
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
  	return administra_cmi_ara();
}
int ligneautopri_cmi_ara(void)
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
		cr = SLC_Saisie_Numerique_ara (ligneautopri_id,"", (unsigned char *) mess_cmi_ligneautopri_id_ara, (TAB_ENTRY_STRING *)&t_entry_id);
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

	return lignetelesec_cmi_ara();
}
int lignetelesec_cmi_ara(void)
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
		cr = SLC_Saisie_Numerique_ara (lignetelesec_id,"", (unsigned char *) mess_cmi_lignetelesec_id_ara, (TAB_ENTRY_STRING *)&t_entry_id);
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

	return ligneautosec_cmi_ara();
}
int ligneautosec_cmi_ara(void)
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
		cr = SLC_Saisie_Numerique_ara (ligneautosec_id,"", (unsigned char *) mess_cmi_ligneautosec_id_ara, (TAB_ENTRY_STRING *)&t_entry_id);
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

	return abonntelepri_cmi_ara();
}
int abonntelepri_cmi_ara(void)
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
    	cr = SLC_Saisie_Numerique_ara (abonntelepri_id,"", (unsigned char *) mess_cmi_abonntelepri_id_ara, (TAB_ENTRY_STRING *)&t_entry_id);
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

	return abonnautopri_cmi_ara();
}
int abonnautopri_cmi_ara(void)
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
		cr = SLC_Saisie_Numerique_ara (abonnautopri_id,"", (unsigned char *) mess_cmi_abonnautopri_id_ara, (TAB_ENTRY_STRING *)&t_entry_id);
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

	return abonntelesec_cmi_ara();
}
int abonntelesec_cmi_ara(void)
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
		cr = SLC_Saisie_Numerique_ara (abonntelesec_id,"", (unsigned char *) mess_cmi_abonntelesec_id_ara, (TAB_ENTRY_STRING *)&t_entry_id);
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

	return abonnautosec_cmi_ara();
}
int abonnautosec_cmi_ara(void)
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
		cr = SLC_Saisie_Numerique_ara (abonnautosec_id,"", (unsigned char *) mess_cmi_abonnautosec_id_ara, (TAB_ENTRY_STRING *)&t_entry_id);
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

	return delaipreautor_cmi_ara();
}
int delaipreautor_cmi_ara(void)
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
		cr = SLC_Saisie_Numerique_ara (delaipreautor_id,"", (unsigned char *) mess_cmi_delaipreauto_id_ara, (TAB_ENTRY_STRING *)&t_entry_id);
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
	  return administradom_cmi_ara();
  else
	  return administra_cmi_ara();
//Mouna310305_fin
}
int prepaye_naps_ara(void){

	return OK;
}

int parametres_cmi_ara(void)
{
	Imprime_Param();
//	return editparam_cmi();
	return 0;
}
int devise_cmi_ara(void)
{
	Imprime_devises();
//	return editparam_cmi();;
	return 0;
}
int administration_naps_ara(void)
{
	//mot de passe

	if(mdp_merchant_cmi_ara() != CR_SAISIE_OK)
	{
		CUMORE_CMI_AFFICHE_MESSAGE(16,20,5000);
		return CR_SAISIE_NOK;
	}

	parametres_naps_ara();

//  return appli_cmi();
	return 0;
}
int changecomptdom_cmi_ara(void)
{
	Compte_domicil();
	PutAllParam();
	return	administradom_cmi_ara();
}
int bin_cmi_ara(void)
{
	Imprime_Bin();
//	return editparam_cmi();;
	return 0;
}
int prefixes_cmi_ara(void)
{
	Imprime_Prefixes();
//	return editparam_cmi();;
	return 0;
}
int comptes_cmi_ara(void)
{
	Imprime_Comptes();
//	return editparam_cmi();;
	return 0;
}


/*******************************************************************************************************************/
int GetItemAlphaNumerique_ara( char *sOutput, int OutputLen,char *libelle,unsigned short int maski)
{
	int nState;
	int cr;
	const TAB_ENTRY_STRING t_entry_id={maski,TO_PARAM,1,0,1,24,1};

	
	cr = SLC_Saisie_Numerique_ara (sOutput,"", (unsigned char *) libelle, (TAB_ENTRY_STRING *)&t_entry_id);
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

int ip_gprs_cmi_ara(void)
{

	 	return port_gprs_cmi_ara();
}


int port_gprs_cmi_ara(void)
{
	 char port_id[6];
	 
	 memset(port_id,0x00,sizeof(port_id));

	 memcpy(port_id,FicPara.port_gprs,6);

	 if( GetItemAlphaNumerique_ara(port_id, 6,"PORT :",0x0002) == __STATE_INPUT_END ){
		memcpy(FicPara.port_gprs,port_id,6);
	 } 
	 	return apn_gprs_cmi_ara();
}

int apn_gprs_cmi_ara(void)
{
	 	return login_gprs_cmi_ara();
}
int login_gprs_cmi_ara(void)
{
	 return pwd_gprs_cmi_ara();
}
int pwd_gprs_cmi_ara(void)
{
return time_out_param_ara();
}
int time_out_param_ara(void)
{

	 }

/*******************************************************************************************************************/
int	menu_com_pm_ara(char item)
{
	return OK;
}
int comm_pm_ara(void)
{

	return OK;
}
int comm_pm_rtc_ara(void)
{
	return 0;
}
int id_mode_nbe_ara(void)
{
	return id_rlp_nbe_ara();
	
}
int id_rlp_nbe_ara(void)
{
	return 0;
}
int ip_gprs_nbe_ara(void)
{
	
	return port_gprs_nbe_ara();
}
int port_gprs_nbe_ara(void)
{
	return apn_gprs_nbe_ara();
}
int apn_gprs_nbe_ara(void)
{
	
	return login_gprs_nbe_ara();
}
int login_gprs_nbe_ara(void)
{
	return pwd_gprs_nbe_ara();
}
int pwd_gprs_nbe_ara(void)
{	return 0;
}
int	tcpip_nbe_ara(void)
{	return 0;
}


int transaction_naps_ara(void)
{	return OK;
}



int duplicata_naps_ara(void)
{
	duplicata_cmi_ara();

	PutAllParam();
	return OK;
}
int parametres_naps_ara(void)
{
	return OK;
}

int cashadvance_naps_ara(void) {

}

int preautorisation_naps_ara(void)
{

	return OK;

}

int service_naps_ara(void)
{
	return OK;

}

int journaux_naps_ara(void)
{
	report_cmi_ara();
	return OK;

}
int Admin_Services_ara(void)
{
	  return 0;
}
/*-----------------------------------------------------------------
 * Fonction   :Activer_ara
 * objet      :
 *
 * Parametre  :
 * Retour     :
 *-----------------------------------------------------------------
*/
int Activer_ara(void)
{
	//report_cmi();
	FicPara.var_pourboire = '1';
	PutAllParam();
	return OK;

}
/*-----------------------------------------------------------------
 * Fonction   :Desactiver_ara
 * objet      :
 *
 * Parametre  :
 * Retour     :
 *-----------------------------------------------------------------
*/
int Desactiver_ara(void)
{
	//report_cmi();
	FicPara.var_pourboire = '0';
	PutAllParam();
	return OK;

}
/*-----------------------------------------------------------------
 * Fonction   :arabe
 * objet      :
 *
 * Parametre  :
 * Retour     :
 *-----------------------------------------------------------------
*/
int arabe(void)
{
	//report_cmi();
	FicPara.Langue = '2';
	PutAllParam();
	T_GL_HGRAPHIC_LIB hGoal_ara=APEMV_UI_GoalHandle();
	T_GL_HWIDGET hScreen_ara=NULL;
	hScreen_ara = GoalCreateScreen(hGoal_ara, txGoalFont, NUMBER_OF_LINES(txGoalFont), GL_ENCODING_ISO_8859_6);
	if (hScreen_ara)
			GoalDestroyScreen(&hScreen_ara);
	PutAllParam();
	return OK;

}
/*-----------------------------------------------------------------
 * Fonction   :francais
 * objet      :
 *
 * Parametre  :
 * Retour     :
 *-----------------------------------------------------------------
*/
int francais(void)
{
	//report_cmi();
	FicPara.Langue = '0';
	T_GL_HGRAPHIC_LIB hGoal_fr=APEMV_UI_GoalHandle();
	T_GL_HWIDGET hScreen_fr=NULL;
		hScreen_fr = GoalCreateScreen(hGoal_fr, txGoalFont, NUMBER_OF_LINES(txGoalFont), GL_ENCODING_ISO_8859_1);
		if (hScreen_fr)
				GoalDestroyScreen(&hScreen_fr);
	PutAllParam();
	return OK;

}
/*-----------------------------------------------------------------
 * Fonction   :anglais
 * objet      :
 *
 * Parametre  :
 * Retour     :
 *-----------------------------------------------------------------
*/
int anglais(void)
{
	//report_cmi();
	FicPara.Langue = '1';
	PutAllParam();
	return OK;

}
/*-----------------------------------------------------------------
 * Fonction   :GL_SampleDialogChoice_ara
 * objet      :activation pourboire arabe
 *
 * Parametre  :
 * Retour     :
 *-----------------------------------------------------------------
*/
void GL_SampleDialogChoice_ara()
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
			Desactiver_ara();
			goto lblEnd;
		case 1:
			Activer_ara();
			goto lblEnd;
		default:
			bContinue2 = FALSE;
			goto lblEnd;
		}
	} while (bContinue2);


	lblEnd:
	return KO;
}


