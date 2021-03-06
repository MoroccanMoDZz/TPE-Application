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

//#include "sdk.h" // IngeDev automatic refactoring - 2014/04/29 14:29:58 - Replace '#include "SDK30.H"' to '#include "sdk.h"'
#include "SDK_Naps.h"
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
#include "cu_receipt.h"
#include "cu_logos.h"
#include "EngineInterfaceLib.h"
#include "TlvTreeDel.h"
#include "EngineInterface.h"
#include "cu_trfile.h"
#include "GL_GraphicLib.h"
#include "sec_interface.h"
#include "GTL_Assert.h"
#include "GTL_Convert.h"

#include "EPSTOOL_Convert.h"
#include "EPSTOOL_TlvTree.h"
#include "EPSTOOL_PinEntry.h"
#include "EPSTOOL_Unicode.h"
#include "cu_UserInterfaceDisplay.h"

#include "Goal.h"

// Properties of image (Goal)
// =======================================
static const ST_IMAGE cm_xImage480 =
{
	{GL_COLOR_WHITE, TRUE, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_MEDIUM}, GL_DIRECTION_NONE, {0, 0, 0, 0, GL_COLOR_RED}, {70, 70, 70, 70} },
	{TRUE, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_MEDIUM}, GL_DIRECTION_NONE, {1, 1, 1, 1, GL_COLOR_BLACK}, {2, 1, 2, 1} }
};

static const ST_IMAGE cm_xImage240 =
{
	{GL_COLOR_BLACK, TRUE, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_MEDIUM}, GL_DIRECTION_NONE, {0, 0, 0, 0, GL_COLOR_RED}, {20, 20, 20, 20} },
	{TRUE, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_MEDIUM}, GL_DIRECTION_NONE, {1, 1, 1, 1, GL_COLOR_BLACK}, {2, 1, 2, 1} }
};

static const ST_IMAGE cm_xImage2_480 =
{
	{GL_COLOR_BLACK, NULL, {"LUCIDA", GL_FONT_STYLE_NORMAL, GL_SCALE_MEDIUM}, GL_DIRECTION_NONE, {0, 0, 0, 0, GL_COLOR_RED}, {100, 0, 100, 100} },
	{TRUE, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_MEDIUM}, GL_DIRECTION_NONE, {1, 1, 1, 1, GL_COLOR_BLACK}, {2, 1, 2, 1} }
};

static const ST_IMAGE cm_xImage2_240 =
{
	{GL_COLOR_BLACK, NULL, {"LUCIDA", GL_FONT_STYLE_NORMAL, GL_SCALE_MEDIUM}, GL_DIRECTION_NONE, {0, 0, 0, 0, GL_COLOR_RED}, {50, 0, 50, 50} },
	{TRUE, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_MEDIUM}, GL_DIRECTION_NONE, {1, 1, 1, 1, GL_COLOR_BLACK}, {2, 1, 2, 1} }
};


int	GlLine;
int	DebutLine;

char buffer12[1024];
char buffer13[4000];

extern Ushort gindice;

//! \addtogroup Group_cu_more
//! @{

/* ======================================================================== */
/* Macros & preprocessor definitions                                        */
/* ======================================================================== */

#define TO_PARAM    120								//!< Time-out for parameter entry
#define CHECK(CND,LBL) {if(!(CND)){goto LBL;}}
/* ========================================================================	*/
/* Static function definitions                                              */
/* ========================================================================	*/

static void CUMORE_Cash          (void);
static void CUMORE_CashBack      (void);
static void CUMORE_BatchStat     (void);
static int  CUMORE_BatchCaptRaz  (void);
static void CUMORE_BatchRAZ      (void);
void CUMORE_Enter_Terminal_ID (void);
static void CUMORE_Reset         (void);
static void CUMORE_Default       (void);
static void CUMORE_Administrative(void);
static void CUMORE_Inquiry       (void);


static void CUMORE_Dump_GParam   (void);
static void CUMORE_Bypass_PIN (void);

static void CUMORE_ForcedOnline  (void);
static void CUMORE_TransactionLog(void);
static void CUMORE_TransactionReactivate(void);
static void CUMORE_TransactionLoopMode(void);

static int merchant(void);
static int ParamManagement(void);

#ifdef _TOOLWARE_
static void CUMORE_Demo_Mode(void);
#endif /* _TOOLWARE_ */

static void CUMORE_Dump_ICS(void);
static int  CUMORE_Send_Financial_Record (TLV_TREE_NODE hInputTree);
static int  CUMORE_Send_Advice_Record (TLV_TREE_NODE hInputTree);
static void CUMORE_Dump_AID(void);
static void CUMORE_Load_Param(void);

static int  FileCopy(char *in, char *out);
int Ava_pourboire(void);
int CinqP_pourboire(void );
int DixP_pourboire(void);
int QuinzeP_pourboire(void);
int Choix_pourboire(void);
int affichage_pourboire(UL Montant, UL Total);

static int RetourConfirmPourboire;
/* ========================================================================*/
/* Constant declarations                                                   */
/* ========================================================================*/




const NavigationListe NavigationEmvdc[] =
{
	{CMN_MESS_DEBIT,                    (PFONCNAVI) CUMORE_Debit},
	{CMN_MESS_REFUND,                   (PFONCNAVI) CUMORE_Refund},
	{CMN_MESS_EMV_CASH,                 (PFONCNAVI) CUMORE_Cash},
	{CMN_MESS_EMV_CASHBACK,             (PFONCNAVI) CUMORE_CashBack},
	{CMN_MESS_EMV_ADMINISTRATIVE,       (PFONCNAVI) CUMORE_Administrative},
	{CMN_MESS_EMV_INQUIRY		,       (PFONCNAVI) CUMORE_Inquiry},
	{CMN_MESS_MERCHANT,                 (PFONCNAVI) merchant}
#ifdef _TOOLWARE_
	,{CMN_MESS_MERCHANT_TOOLWARE, (PFONCNAVI) CUMORE_Demo_Mode}
#endif /* _TOOLWARE_ */
};

static const NavigationListe NavigationMerchant[] =
{
	{CMN_MESS_MERCHANT_BYPASSPIN,       (PFONCNAVI) CUMORE_Bypass_PIN},
	{CMN_MESS_MERCHANT_BATCH_STAT,      (PFONCNAVI) CUMORE_BatchStat},
	{CMN_MESS_MERCHANT_RUN_BATCH,       (PFONCNAVI) CUMORE_BatchCaptRaz},
	{CMN_MESS_MERCHANT_PARAM_MNGT,      (PFONCNAVI) ParamManagement},
	{CMN_MESS_MERCHANT_FORCE_ONLINE,    (PFONCNAVI) CUMORE_ForcedOnline},
	{CMN_MESS_MERCHANT_TRANSACTIONLOG,  (PFONCNAVI) CUMORE_TransactionLog},
	{CMN_MESS_MERCHANT_TRRECATIVATION,  (PFONCNAVI) CUMORE_TransactionReactivate},
	{CMN_MESS_MERCHANT_TRLOOPMODE,      (PFONCNAVI) CUMORE_TransactionLoopMode}
};

static const NavigationListe NavigationBatchCapt[] =
{
	{CMN_MESS_MERCHANT_RUN_BATCH_RAZ,   (PFONCNAVI) CUMORE_BatchCapt},
	{CMN_MESS_MERCHANT_BATCH_RAZ,       (PFONCNAVI) CUMORE_BatchRAZ}
};

static const NavigationListe NavigationParamMngt[] =
{
	{CMN_MESS_MERCHANT_RESET_ALL,       (PFONCNAVI) CUMORE_Reset},
	{CMN_MESS_MERCHANT_DEFAUT_CONF,     (PFONCNAVI) CUMORE_Default},
	{CMN_MESS_MERCHANT_TERMINAL_ID,		(PFONCNAVI) CUMORE_Enter_Terminal_ID},
	{CMN_MESS_MERCHANT_DUMP_ICS,		(PFONCNAVI) CUMORE_Dump_ICS},
	{CMN_MESS_MERCHANT_DUMP_AID,		(PFONCNAVI) CUMORE_Dump_AID},
	{CMN_MESS_MERCHANT_GLOBAL_PARAM,	(PFONCNAVI) CUMORE_Dump_GParam},
	{CMN_MESS_MERCHANT_LOAD_PARAM,      (PFONCNAVI) CUMORE_Load_Param},

};

const NavigationListe NavigationPourboire[] =
{
		{MESS82,          (PFONCNAVI) CinqP_pourboire},
		{MESS83,          (PFONCNAVI) DixP_pourboire},
		{MESS84,          (PFONCNAVI) QuinzeP_pourboire},
		{MESS87,          (PFONCNAVI) Choix_pourboire},
};

const unsigned char CUMORE_TRUE []   = {0x00, 0x01};
const unsigned char CUMORE_FALSE []  = {0x00, 0x00};

// static const TAB_ENTRY_STRING t_term_id={0x0000,TO_PARAM,1,0,1,8,1};


/* ========================================================================	*/
/* Global variables                                                         */
/* ========================================================================	*/

unsigned short Bypass_Required;

unsigned short TransactionForcedOnline;

//extern unsigned char ucMode2000;

unsigned char RetransmitIndicator;

TLV_TREE_NODE CUMORE_Tree;

unsigned short TransactionReactivateAsked;

extern int TransactionLoopMode;
extern unsigned long loop_amount;
int control_amount;
extern int isFallbackAllowed;

unsigned char * p_status_tr;

extern t_topstack *alternateNbr;
extern int AlternanceIsOn;
//extern Telium_File_t *hKbd;      // Handle of the peripheral Keyboard


/* ======================================================================== */
/* Functions                                                                */
/* ======================================================================== */
extern void EMVCUST_ParamFile_Upload_TlvTree (TLV_TREE_NODE *phOutputTLVTree);
extern int cu_file_received (NO_SEGMENT no_appli,S_FILE *param_in,void *param_out);

//===========================================================================
//! \brief This function manages the application Merchant menu.
//! \return The Get_Entry return value.
//! - CR_ENTRY_OK : Successful entry.
//! \li the field d_entry[0] of Entry_buffer contains the index of the selected
//! item in the list for G_List_Entry.
//! \li the field d_len of Entry_buffer contains the entry length.
//! \li the field d_entry [50]  of Entry_buffer contains the entry for G_Numerical_Entry.
//! - CR_ENTRY_NOK : Correction (if authorised)
//! - CR_ENTRY_TIME_OUT : Timeout elapsed
//! - CR_ENTRY_CANCEL : Entry Cancelled
//! - CR_ENTRY_VAL : Green key (confirmation)
//! - CR_ENTRY_FCT : F key pushed (if authorised)
//! - CR_ENTRY_SK1 : SK1 ('1' on EFT930M or ML30) key pushed (if authorised)
//! - CR_ENTRY_SK2 : SK2 ('2' on EFT930M or ML30) key pushed (if authorised)
//! - CR_ENTRY_SK3 : SK3 ('3' on EFT930M or ML30) key pushed (if authorised)
//! - CR_ENTRY_SK4 : SK4 ('4' on EFT930M or ML30) key pushed (if authorised)
//===========================================================================
static int merchant(void)
{
	unsigned char	cr = 0;
	unsigned char	i;
	unsigned short	lg_code;
	wIconList		IconLst;
    T_GL_WCHAR choice  = 0;

	memclr(&IconLst, sizeof(IconLst));

	// Init table from const

	lg_code = My_PSQ_Give_Language();

	{
		Menu.Fenetre.titre = (unsigned char *) CUMESS_Read_Message(CMN_MESS_MERCHANT, lg_code);

		for (i=0 ; i < ELVEC(NavigationMerchant) ; i++ )
		{
			Menu.tab[i] = (unsigned char *) CUMESS_Read_Message(NavigationMerchant[i].itemlabel, lg_code);
		}

		// Call navigation time-out 30 secs
		Menu.tab[i] = NULL;
	}

	choice = APEMV_UI_MenuSelectTopic((const char*)Menu.Fenetre.titre,  (const char*)Menu.tab, TRUE);

	if (choice != -1)
	{
		NavigationMerchant[choice].function();
		cr = CR_ENTRY_OK ;
	}

	return(cr);
}

//===========================================================================
//! \brief This function manages the application ParamManagement menu.
//! \return The Get_Entry return value.
//! - CR_ENTRY_OK : Successful entry.
//! \li the field d_entry[0] of Entry_buffer contains the index of the selected
//! item in the list for G_List_Entry.
//! \li the field d_len of Entry_buffer contains the entry length.
//! \li the field d_entry [50]  of Entry_buffer contains the entry for G_Numerical_Entry.
//! - CR_ENTRY_NOK : Correction (if authorised)
//! - CR_ENTRY_TIME_OUT : Timeout elapsed
//! - CR_ENTRY_CANCEL : Entry Cancelled
//! - CR_ENTRY_VAL : Green key (confirmation)
//! - CR_ENTRY_FCT : F key pushed (if authorised)
//! - CR_ENTRY_SK1 : SK1 ('1' on EFT930M or ML30) key pushed (if authorised)
//! - CR_ENTRY_SK2 : SK2 ('2' on EFT930M or ML30) key pushed (if authorised)
//! - CR_ENTRY_SK3 : SK3 ('3' on EFT930M or ML30) key pushed (if authorised)
//! - CR_ENTRY_SK4 : SK4 ('4' on EFT930M or ML30) key pushed (if authorised)
//===========================================================================
static int ParamManagement(void)
{
	unsigned char	cr = 0;
	unsigned char	i;
	unsigned short	lg_code;
	wIconList		IconLst;
    T_GL_WCHAR choice  = 0;

	memclr(&IconLst, sizeof(IconLst));

	// Init table from const
	InitDefaut(_ON_,1);

	lg_code = My_PSQ_Give_Language();

	{
		Menu.Fenetre.titre = (unsigned char *) CUMESS_Read_Message(CMN_MESS_MERCHANT_PARAM_MNGT, lg_code);

		for (i=0 ; i < ELVEC(NavigationParamMngt) ; i++ )
		{
			Menu.tab[i] = (unsigned char *) CUMESS_Read_Message(NavigationParamMngt[i].itemlabel, lg_code);
		}

		// Call navigation time-out 30 secs
		Menu.tab[i] = NULL;
	}

	choice = APEMV_UI_MenuSelectTopic((const char*)Menu.Fenetre.titre,  (const char*)Menu.tab, TRUE);
	if (choice != -1)
	{
		NavigationParamMngt[choice].function();
		cr = CR_ENTRY_OK ;
	}
	return(cr);
}
//===========================================================================
//! \brief This function manages Echeance amount for N time payment.
//! \return OK/KO.
//===========================================================================
int GetAmountEcheances(unsigned long *mnt_echeances)
{
	int 	 nbrFois  = 0 ;
	double 	 mntInter = 0;
	long 	 mntTotalAffich = 0L;

	memset(StructInfoPayment.Nbr_Fois,'\0',sizeof(StructInfoPayment.Nbr_Fois));
	memset(StructInfoPayment.ModeReglement,'\0',sizeof(StructInfoPayment.ModeReglement));
	memset(StructInfoPayment.montant_total,'\0',sizeof(StructInfoPayment.montant_total));

	mntInter = * mnt_echeances;
	mntTotalAffich =  mntInter;
	long_asc (StructInfoPayment.montant_total,12, &mntTotalAffich);

	if(ucGetField(190,"PAIEMENT N FOIS",StructInfoPayment.Nbr_Fois,2,1,1,"0",30000,0)!=OK)
		return KO;

	nbrFois = atol(StructInfoPayment.Nbr_Fois);

	mntInter = (double)((double)mntInter/100);
	mntInter = (double)mntInter/nbrFois ;

//	sprintf(montant_C, "%.2f",mntInter);
//	del_char(montant_C,'.');
//	*(mnt_echeances) = atol(montant_C);
	*(mnt_echeances) = mntInter;


	unsigned char   cr;
	BUFFER_SAISIE   buf_saisie;

	InitDefaut(_ON_,1);

	Menu.Fenetre.titre      = (unsigned char *)"PAIEMENT N FOIS";
	Menu.tab[0] 			= "REGLEMENT MENSUEL";
	Menu.tab[1] 			= "REGLEMENT HEBDO";
	Menu.tab[2] = NULL;

	cr = (int)APEMV_UI_MenuSelectTopic((const char*)Menu.Fenetre.titre,  (const char*)Menu.tab, TRUE);

	switch(cr)
	{
		case 0:
			memcpy(StructInfoPayment.ModeReglement,"M",1);
			break;
		case 1:
			memcpy(StructInfoPayment.ModeReglement,"H",1);
			break;
		default:
			return KO;
			break;
	}
	return OK;
}
//===========================================================================
//! \brief This function manages surcout amount validation.
//! \return OK/KO.
//===========================================================================
unsigned short MontantSurcout(unsigned long *lnAmountValue)
{
	unsigned long  ulINT  		= 0L;
	unsigned long  ulSVM  		= 0L;
	unsigned long  ulMN   		= 0L;
	char		   Message_csurcout[112];


	if(StructInfoPayment.AfficheIndication_POURBOIRE_INTERCHANGE == 2)
	{

		if(StructInfoPayment.Origine_carte == CARTE_NATIONALE)
		{
			Os__xprinta("CARTE NATIONALE");

			ulINT = atol(FicPara.COMMISSION_INT[0]);
			//Os__xprinta(FicPara.COMMISSION_INT[0]);
			//Os__xprintd(ulINT);

			ulSVM = atol(FicPara.COMMISSION_SVM[0]);
			//Os__xprinta(FicPara.COMMISSION_SVM[0]);
			//Os__xprintd(ulSVM);

			ulMN  = atol(FicPara.COMMISSION_MN[0]);
			//Os__xprinta(FicPara.COMMISSION_MN[0]);
			//Os__xprintd(ulMN);
		}
		else if((StructInfoPayment.Origine_carte == CARTE_ETRANGERE)||(StructInfoPayment.Origine_carte == CARTE_ETRANGATRS))
		{
			//Os__xprinta("CARTE ETRANGERE");

			ulINT = atol(FicPara.COMMISSION_INT[1]);
			//Os__xprinta(FicPara.COMMISSION_INT[1]);
			//Os__xprintd(ulINT);

			ulSVM = atol(FicPara.COMMISSION_SVM[1]);
			//Os__xprinta(FicPara.COMMISSION_SVM[1]);
			//Os__xprintd(ulSVM);

			ulMN  = atol(FicPara.COMMISSION_MN[1]);
			//Os__xprinta(FicPara.COMMISSION_MN[1]);
			//Os__xprintd(ulMN);
		}
		else
		{
			//Os__xprinta("CARTE DOMESTIQUE");

			ulINT	= 0;
			ulSVM	= 0;
			ulMN	= 0;
		}
		//Os__xprintd(((ulINT + ulSVM)*10000));
		//Os__xprintd((10000 -ulINT -ulSVM -ulMN));

		*lnAmountValue =  ((ulINT + ulSVM)*10000) / (10000 -ulINT -ulSVM -ulMN);

		//Os__xprintd((*lnAmountValue));

		memset(Message_csurcout,0,sizeof(Message_csurcout));

		sprintf(Message_csurcout,"\n\n   Confirmez vous\nle surcout %d,%d%d DH \nde cette operation ?",
		(*lnAmountValue)/100,
		((*lnAmountValue)/10)%10,
		(*lnAmountValue)%10);

		//Os__xprinta(Message_csurcout);

		APEMV_UI_InfoDisplay((char *)Message_csurcout);
		while (1)
		{
			if(Os__xget_key(FicPara.TimeOut*6) != KEY_CLEAR)
				return OK;

			*lnAmountValue = 0;
			return KO;
		}
	}
}


//===========================================================================
//! \brief This function performs a transaction according to transaction type
//! given as parameter.
//! \param[in] TransactionType : type of the transaction.
//! \param ucParameters  : TRUE if ulAmount and nCurrencyCode parameters must be used.
//! \param ulAmount      : amount entered.
//! \param nCurrencyCode : ISO currency code.
//! \ref CUSERV_DEBIT_TRANSACTION
//! \ref CUSERV_REFUND_TRANSACTION
//! \ref CUSERV_CASH_TRANSACTION
//! \ref CUSERV_CASHBACK_TRANSACTION
//===========================================================================
static void CUMORE_PaiementFct(unsigned long TransactionType, unsigned char ucParameters, unsigned long ulAmount, int nCurrencyCode) {
	//APEMV_UI_MessageDisplayDelay ((char *)"Found it cu_more", 1);
	int previousUiState;
#ifdef _ENABLE_CAM2_
	static const unsigned long ulChipReaders = CAM0 | CAM2;
#else
	static const unsigned long ulChipReaders = CAM0;
#endif
	int result;
	int found;
	unsigned short currency_code = 0;
	unsigned char  TabAmount [4];
	unsigned char  TabAmountC [4];
	unsigned char  NumAmount [6];
	unsigned char  NumAmountC [6];
	unsigned long  entered_amount =0;
	unsigned long  entered_amount_pourboire;
	DataElement eltCurrency,eltCurrencyExp;
	unsigned char TypeAppli[2];
	unsigned char EngAppliSel;
	TLV_TREE_NODE hInputTree;
	TLV_TREE_NODE hOutputTree;
	unsigned char ucTimeout[4];

	T_GL_HWIDGET hPicture=NULL;


	// Reset the selected mark list
	PAR_ResetMarkList();
	// Select Main ICS + ICSx (Maybe unused)
	CUPAR_SelectAdditionnalICS();

	memset(&E_Transac8583,0x00,sizeof(E_Transac8583));
	memset(&R_Transac8583,0x00,sizeof(R_Transac8583));

	if(		(ulTransactionType != CUSERV_VOID_TRANSACTION)
			&&	(ulTransactionType != CUSERV_VOIDP_TRANSACTION)
			&&	(ulTransactionType != CUSERV_COMPL_TRANSACTION)
			&&  (ucParameters == FALSE)
	)
		memset(&StructInfoPayment ,0x00,sizeof(StructInfoPayment));//Larbi


	if(ulTransactionType == CUSERV_PREAUT_TRANSACTION)						StructInfoPayment.CodeFunction = PREAUTHCODE;
	else if(ulTransactionType == CUSERV_CASH_TRANSACTION)					StructInfoPayment.CodeFunction = CASHADVANCESCODE;
	else if(ulTransactionType == CUSERV_COMPL_TRANSACTION)					StructInfoPayment.CodeFunction = PREAUTHCOMPCODE;
	else if(ulTransactionType == CUSERV_PAIEMENT_NFOIS_TRANSACTION)			StructInfoPayment.CodeFunction = PAIEMENT_NFOIS;
	else if(ulTransactionType == CUSERV_PAIEMENT_DIFFERE_TRANSACTION)		StructInfoPayment.CodeFunction = PAIEMENT_DIFFERE;
	else if(ulTransactionType == CUSERV_PAIEMENT_POURBOIRE_TRANSACTION)		StructInfoPayment.CodeFunction = PAIEMENT_POURBOIRE;
	else if(ulTransactionType == CUSERV_PAIEMENT_INTERCHANGE_TRANSACTION)	StructInfoPayment.CodeFunction = PAIEMENT_INTERCHANGE;
	else if(ulTransactionType == CUSERV_REFUND_TRANSACTION)					StructInfoPayment.CodeFunction = REFUNDCODE;
	else if(ulTransactionType == CUSERV_VOID_TRANSACTION)					StructInfoPayment.CodeFunction = VOIDCODE;
	else if(ulTransactionType == CUSERV_VOIDP_TRANSACTION)					StructInfoPayment.CodeFunction = VOIDCODE;
	else if(memcmp(FicPara.MCC,"6010",4) == 0)
		StructInfoPayment.CodeFunction = CASHADVANCESCODE;
	else
		StructInfoPayment.CodeFunction = SALECODE;

	// -----------------------------------------------------------------
	// Amount Entry
	// -----------------------------------------------------------------
    if (ucParameters == TRUE) // If parameters given in interface must be used
    {

        result = T_VAL;
        entered_amount = ulAmount;
        currency_code = nCurrencyCode;

		StructInfoPayment.Transaction_Amount = entered_amount;
		memcpy(StructInfoPayment.Devise_acc.code_num,"504",3);
		memcpy(StructInfoPayment.Devise_acc.code_alph,"MAD",3);
		StructInfoPayment.Devise_acc.Exposant = 2;

	    // No card is present at this moment. The currency used is the Terminal currency
	    found = PAR_ReadParameter(TAG_TRANSACTION_CURRENCY_CODE, (DataElementExt *)&eltCurrency);
	    if (found)
		    currency_code = (eltCurrency.ptValue[0] << 8) + eltCurrency.ptValue[1];

    }
    else // Parameters : amount and currency code must be inputed or found ...
    {

	    // No card is present at this moment. The currency used is the Terminal currency
	    found = PAR_ReadParameter(TAG_TRANSACTION_CURRENCY_CODE, (DataElementExt *)&eltCurrency);
	    if (found)
		    currency_code = (eltCurrency.ptValue[0] << 8) + eltCurrency.ptValue[1];

    	//currency_code = 0x0504;

		if (TransactionLoopMode == TRUE)
		{
			if (loop_amount == 0)
			{
				result = CUMESS_GetAmount (currency_code, &entered_amount);
				if ((result) && (entered_amount!=0))
				{
					loop_amount = entered_amount;
				}
				else
				{
					hInputTree = NULL;
					hOutputTree = NULL;
					goto End;
				}
			}
			else
			{
				entered_amount = loop_amount;
				result = T_VAL;
			}
		}
		else
		{
			////////////////////here/////////////////////////////
			result = CUMESS_GetAmount (currency_code, &entered_amount);

			if ((result) && (entered_amount!=0))
			{

			}
			else
			{
				hInputTree = NULL;
				hOutputTree = NULL;
				goto End;
			}
		}

		if(ulTransactionType == CUSERV_COMPL_TRANSACTION)
		{
			StructInfoPayment.Preauto_Amount_Final = entered_amount;
			StructInfoPayment.Preauto_Amount_Initial = StrPreAut.Transaction_Amount;
			memcpy(StructInfoPayment.Devise_acc.code_num,"504",3);
			memcpy(StructInfoPayment.Devise_acc.code_alph,"MAD",3);
			StructInfoPayment.Devise_acc.Exposant = 2;
		}
		else if(ulTransactionType == CUSERV_PAIEMENT_NFOIS_TRANSACTION)
		{
			StructInfoPayment.Transaction_Amount = entered_amount;
			memcpy(StructInfoPayment.Devise_acc.code_num,"504",3);
			memcpy(StructInfoPayment.Devise_acc.code_alph,"MAD",3);
			StructInfoPayment.Devise_acc.Exposant = 2;

			GetAmountEcheances(&entered_amount);

			if((StructInfoPayment.ModeReglement[0] != 'M') && (StructInfoPayment.ModeReglement[0] != 'H'))
			{
				hInputTree = NULL;
				hOutputTree = NULL;
				goto End;
			}

			StructInfoPayment.Transaction_Amount=entered_amount;

		}
		else if(ulTransactionType == CUSERV_PAIEMENT_DIFFERE_TRANSACTION)
		{
			StructInfoPayment.Transaction_Amount = entered_amount;
			memcpy(StructInfoPayment.Devise_acc.code_num,"504",3);
			memcpy(StructInfoPayment.Devise_acc.code_alph,"MAD",3);
			StructInfoPayment.Devise_acc.Exposant = 2;

//

			StructInfoPayment.MontantDiffere = entered_amount ;
		}
		else if(ulTransactionType == CUSERV_PAIEMENT_POURBOIRE_TRANSACTION)
		{
			StructInfoPayment.Transaction_Amount = entered_amount;
			memcpy(StructInfoPayment.Devise_acc.code_num,"504",3);
			memcpy(StructInfoPayment.Devise_acc.code_alph,"MAD",3);
			StructInfoPayment.Devise_acc.Exposant = 2;

			StructInfoPayment.AfficheIndication_POURBOIRE_INTERCHANGE = 1; // pour saisie du mnt et mnt pourboire

			CUMESS_GetAmount (currency_code, &entered_amount_pourboire);
			StructInfoPayment.MontantPourboireInterchange = entered_amount_pourboire ;

			if(!StructInfoPayment.MontantPourboireInterchange)
			{
				hInputTree = NULL;
				hOutputTree = NULL;
				goto End;
			}
		}
		else if(ulTransactionType == CUSERV_PAIEMENT_INTERCHANGE_TRANSACTION)
		{
			StructInfoPayment.Transaction_Amount = entered_amount;
			memcpy(StructInfoPayment.Devise_acc.code_num,"504",3);
			memcpy(StructInfoPayment.Devise_acc.code_alph,"MAD",3);
			StructInfoPayment.Devise_acc.Exposant = 2;

			StructInfoPayment.AfficheIndication_POURBOIRE_INTERCHANGE = 2; // pour saisie du mnt et mnt pourboire
			MontantSurcout (&entered_amount);
			StructInfoPayment.MontantPourboireInterchange = entered_amount ;

			if(!StructInfoPayment.MontantPourboireInterchange)
			{
				hInputTree = NULL;
				hOutputTree = NULL;
				goto End;
			}
		}
		else
		{
			StructInfoPayment.Transaction_Amount = entered_amount;
			memcpy(StructInfoPayment.Devise_acc.code_num,"504",3);
			memcpy(StructInfoPayment.Devise_acc.code_alph,"MAD",3);
			StructInfoPayment.Devise_acc.Exposant = 2;
//            if(FicPara.MenuTPE[26] == '1')
//            	Ava_pourboire();
		}
    }


    //traceString("Trace 1");
	//Message de Confirmation du montant
    if (ucParameters == FALSE) // If parameters given in interface must be used
		if (MessageConfirmationMontant() != OK)
		{
			hInputTree = NULL;
			hOutputTree = NULL;
			goto End;
		}
	//Message de Confirmation du montant



	if ((result) && (entered_amount!=0))
	{
		// Amount is valid. Convert in num
		CUMESS_ConvertBinNUM (entered_amount, NumAmount);

		LongToChar (entered_amount, TabAmount);

		// Store amounts in current database

		hInputTree = TlvTree_New(0);

		if(hInputTree != NULL)
		{
			ShortToChar(CustApplicationNumber,TypeAppli);
			TlvTree_AddChild(hInputTree, TAG_ENG_CUSTOM_APPLI_TYPE, (unsigned char*) &TypeAppli, 2);

			EngAppliSel =0;
			TlvTree_AddChild(hInputTree, TAG_ENG_APPLI_SEL_ALREADY_DONE, (unsigned char*) & EngAppliSel, 1);

			TlvTree_AddChild(hInputTree, TAG_TRANSACTION_CURRENCY_CODE, (unsigned char*) &eltCurrency.ptValue[0], 2);

			found = PAR_ReadParameter(TAG_TRANSACTION_CURRENCY_CODE, (DataElementExt *)&eltCurrencyExp);
			if (found)
			{
				TlvTree_AddChild(hInputTree, TAG_TRANSACTION_CURRENCY_EXP, (unsigned char*) &eltCurrencyExp.ptValue[0], 1);
			}

			memset (NumAmountC,0,sizeof(NumAmountC));

			if (TransactionType == CUSERV_CASHBACK_TRANSACTION)
			{
				ulTransactionType = CUSERV_CASHBACK_TRANSACTION;

				// Read the amount, using the good currency code

				result = CUMESS_GetAmount (currency_code, &entered_amount);
				LongToChar (0, TabAmountC);
				if ( (result) && (entered_amount!=0) )
				{
					CUMESS_ConvertBinNUM (entered_amount, NumAmountC);

					LongToChar (entered_amount, TabAmountC);
				}
			}
			else if(TransactionType == CUSERV_COMPL_TRANSACTION)
			{
				ulong		Tmp_Amount;
				ushort		Nb_preauto,i,j;
				uchar		status;

				Tmp_Amount = StructInfoPayment.Preauto_Amount_Initial + (StructInfoPayment.Preauto_Amount_Initial*15)/100;
				if(StructInfoPayment.Preauto_Amount_Final <= Tmp_Amount)
				{
					StructInfoPayment.Preauto_indic_completion = PRE_VAL_1_TRS;
					StructInfoPayment.Transaction_Amount = StructInfoPayment.Preauto_Amount_Final;

					StructInfoPayment.Top_Forcage = TOP_FOR_NORMAL;
					memcpy(StructInfoPayment.Approval_Code,StrPreAut.Approval_Code,6);
					Prepare_Transaction();
					memcpy(StructInfoPayment.Message_TypeisoEmi,"1220",4);
					memcpy(StructInfoPayment.Message_TypeisoRec,"1230",4);
					StructInfoPayment.Flag_Dem_Aut = 1;  // larbi 10042019 0==>1
					StrPreAut.Flag_Dem_Aut = StructInfoPayment.Flag_Dem_Aut;
					memcpy(StructInfoPayment.STAN,StrPreAut.STAN,6);

					if(ImprimeTra(FicPara.Langue,COMMERCANT,0) == OK)
					{
						//status=Valider_Transac() ;
						MessageWithWaitForAction("Couper ticket","file://flash/HOST/couper_ticket.png", 10000);
//						if(status==KEY_ENTER)
//						{
							SaveTra() ;
							ImprimeTra(StructInfoPayment.Langue,CLIENT,0);
							if(StructInfoPayment.Preauto_Amount_Final<StructInfoPayment.Preauto_Amount_Initial)
							{
								StrPreAut.Montant_Accomp = StructInfoPayment.Preauto_Amount_Final;

								StrPreAut.Indic_Accomp = 1 ;
								write_preauto_index( gindice ,&StrPreAut );
							}
							else
							{
								memset(&StrPreAut ,0x00,sizeof(StrPreAut));
								write_preauto_index( gindice ,&StrPreAut );
								Nb_preauto = FicPreauth.nombre_tra;
								for(i = gindice+1;(i < Nb_preauto); i++)
								{
									read_preauto_index( i ,&StrPreAut );
									j=i-1;
									write_preauto_index( j ,&StrPreAut );
								}
								j=FicPreauth.nombre_tra-1;
								set_ushort_secourue(&FicPreauth.nombre_tra,j) ;
							}
						//}
					}
					hInputTree = NULL;
					hOutputTree = NULL;
					goto End;
				}
				else
				{
			   		StructInfoPayment.Preauto_indic_completion = PRE_VAL_2_TRS;
					StructInfoPayment.Transaction_Amount = StructInfoPayment.Preauto_Amount_Final - StructInfoPayment.Preauto_Amount_Initial;
					entered_amount = StructInfoPayment.Transaction_Amount;
					LongToChar (entered_amount, TabAmount);
				}

			}
			else
			{
				memclr  ( TabAmountC,sizeof(TabAmountC));
			}
//			TlvTree_AddChild(hInputTree, TAG_AMOUNT_OTHER_BIN, TabAmountC, 4);
//			TlvTree_AddChild(hInputTree, TAG_AMOUNT_OTHER_NUM, NumAmountC, 6);
			TlvTree_AddChild(hInputTree, TAG_AMOUNT_AUTH_BIN, TabAmount, 4);

			TlvTree_AddChild(hInputTree, TAG_ENG_WAIT_CHIP_READERS, &ulChipReaders, sizeof(ulChipReaders));
			TlvTree_AddChild(hInputTree, TAG_ENG_REMOVE_CHIP_READERS, &ulChipReaders, sizeof(ulChipReaders));

			if (TransactionLoopMode == TRUE)
			{
				Telium_File_t* hCam0;
				//Telium_File_t* keyb;
				int merchLang;
				unsigned char	*pc_line1;
				//int uiEvent;
				unsigned char ucStatus;

	        	memclr( ucTimeout,sizeof(ucTimeout));
			    TlvTree_AddChild(hInputTree, TAG_ENG_REMOVE_CARD_TIMEOUT, (unsigned char *)&ucTimeout[0], 4);

                while (1)
				{

//					hPicture = GoalCreatePicture(APEMV_UI_GoalHandle());                             // Create and clear screen
//					GoalDspPicture(hPicture, "Inserer la carte", "file://flash/HOST/inserer_la_carte_svp.png", &cm_xImage480, (GL_TIME_SECOND) * 2, true);
//					GoalDestroyPicture(&hPicture);
                	WaitBothMagneticAndCam0(&FicPara.vCam0);
                	if(FicPara.vCam0 == 1)//CAM0
    				{
                		// Setup the user interface environment
						previousUiState = APEMV_UI_TransactionBegin();
						// Perform the transaction
						hOutputTree = NULL;

						Engine_DoTransaction(hInputTree,&hOutputTree);
						//keyb = NULL;
						hCam0 = NULL;

						ucStatus =0;

						// keyb = Telium_Fopen("KEYBOARD","r");
						hCam0 = Telium_Fopen("CAM0", "rw");

						// Display remove
						merchLang = My_PSQ_Give_Language();
						pc_line1 = (unsigned char *)CUMESS_Read_Message(STD_MESS_REMOVE_CARD, merchLang); //EngineK
//						APEMV_UI_InfoDisplay ((char *)pc_line1);

						MessageWithoutConfirmation(pc_line1, "file://flash/HOST/retirer_la_carte_svp.png");

					/*uiEvent =*/ Telium_Ttestall(CAM0,500);

					/*	if (keyb != NULL)
						{
							// Close the chip reader driver
							Telium_Fclose(keyb);
							keyb = NULL;
						}*/
						//if ((uiEvent & KEYBOARD)!=0)
						//
						// Restore the user interface environment
						//APEMV_UI_TransactionEnd(previousUiState);

							//	break;

						Telium_Status(hCam0, &ucStatus);

						if (hCam0 != NULL)
						{
							Telium_Fclose(hCam0);
							hCam0 = NULL;
						}

						if ((ucStatus & CAM_PRESENT) == 0)
						{
							/*if (hCam0 != NULL)
							{
								Telium_Fclose(hCam0);
								hCam0 = NULL;
							}*/

							// Restore the user interface environment
							APEMV_UI_TransactionEnd(previousUiState);

							// A card has been removed
							break;
						}

						// Restore the user interface environment
						APEMV_UI_TransactionEnd(previousUiState);
    				}
    				else if(FicPara.vCam0 == 2)//CLESS
    				{
						memset(&SaveLastInfoCaisse, 0, sizeof(MSG_CAISSE));
						memcpy(&SaveLastInfoCaisse, &StructInfoCaisse[0],sizeof(MSG_CAISSE));
						PutAllParam();
						Telium_Kill(alternateNbr, "");
						//Telium_Ttestall(00, 1000);
						AlternanceIsOn = 0;
//						Os__xprinta("TEST 1");
						SendToApplication(0x0122, 1, 6, "ABCDEF");
						AlternanceIsOn = 0;
//						Os__xprintd(AlternanceIsOn);
    					hInputTree = NULL;
    					hOutputTree = NULL;
    				}
    				else if(FicPara.vCam0 == 4)//TIME OUT
    				{
    					hInputTree = NULL;
    					hOutputTree = NULL;
    				}
    				else	//(!= 5)			//MAG
    				{
    					hInputTree = NULL;
    					hOutputTree = NULL;
    				}
				}
			}
			else
			{

//				Telium_File_t* hCam0;
//				unsigned char ucStatus;
//
//				hCam0 = NULL;
//				ucStatus =0;
//
//				hCam0 = Telium_Fopen("CAM0", "rw");
//
//				hPicture = GoalCreatePicture(APEMV_UI_GoalHandle());                             // Create and clear screen
//				GoalDspPicture(hPicture, "Inserer la carte", "file://flash/HOST/inserer_la_carte_svp.png", &cm_xImage480, (GL_TIME_SECOND) * 0, true);
//
//				Telium_Status(hCam0, &ucStatus);
//
//				while ((ucStatus & CAM_PRESENT) == 0)
//				{
//					Telium_Status(hCam0, &ucStatus);
//				}
//
//				GoalDestroyPicture(&hPicture);
//
//				if (hCam0 != NULL)
//				{
//					Telium_Fclose(hCam0);
//					hCam0 = NULL;
//				}
				if(FicPara.vCam0 == 1)
				FicPara.vCam0 = NULL;//301019 ecran blanc
				//Os__xprintaa("010101");
				WaitBothMagneticAndCam0(&FicPara.vCam0);
				//Os__xprintaa("010102");
				if(FicPara.vCam0 == 1)//CAM0
				{
					//Os__xprintaa("00051");

					// Setup the user interface environment

					previousUiState = APEMV_UI_TransactionBegin();
					// Perform the transaction

					hOutputTree = NULL;
					//Os__xprintaa("00512");
					Engine_DoTransaction(hInputTree,&hOutputTree);//EngineK
					//Os__xprintaa("00513");
					// Restore the user interface environment
					APEMV_UI_TransactionEnd(previousUiState);
					//Os__xprintaa("00514");
				}
				else if(FicPara.vCam0 == 2)//CLESS
				{


					memset(&SaveLastInfoCaisse, 0, sizeof(MSG_CAISSE));
					memcpy(&SaveLastInfoCaisse, &StructInfoCaisse[0],sizeof(MSG_CAISSE));
					PutAllParam();

					Telium_SignalEvent(Telium_GiveNoTask(alternateNbr), 15);
					Telium_Kill(alternateNbr, "");
//					MessageWithDelay("FicPara.vCam0EMV", NULL, 1000);
//					Os__xprintd(FicPara.vCam0);
					//MessageWithDelay("SendToApplication", NULL, 1000);
					StructInfoPayment.gindice = gindice;
					SendToApplication(0x0122, 1, 6, "ABCDEF");

					AlternanceIsOn = 0;

					hInputTree = NULL;
					hOutputTree = NULL;
				}
				else if(FicPara.vCam0 == 4)//TIME OUT
				{
					//Os__xprintaa("00052");
					//Telium_Ttestall(0,3*100);
					hInputTree = NULL;
					hOutputTree = NULL;

				}
				else	//(!= 5)			//MAG
				{
					//Os__xprintaa("00053");
					hInputTree = NULL;
					hOutputTree = NULL;
				}
			}

End:
			if(StructInfoPayment.Preauto_indic_completion != PRE_VAL_2_TRS)
				FicPara.vCam0 = 0;//301019

			//Os__xprintaa("00053");
			TlvTree_ReleaseEx(&hInputTree);
			TlvTree_ReleaseEx(&hOutputTree);
			//Os__xprintaa("00054");

		}
	}
}

//===========================================================================

static const char *MenuPourboire[] =
{
	"  5 %",
	"  10 %",
	"  15 %",
	"  Autre montant",
	NULL
};

int Ava_pourboire(void){                 //larbi
	T_GL_HGRAPHIC_LIB hGoal = APEMV_UI_GoalHandle();
	bool bContinue = TRUE, bContinue2 = TRUE;
	char		   		Message_confmontant[112];

	memset(Message_confmontant,0,sizeof(Message_confmontant));
	sprintf(Message_confmontant,"Montant : %d,%d%d DH \n%s",
	StructInfoPayment.Transaction_Amount/100,
	(StructInfoPayment.Transaction_Amount/10)%10,
	StructInfoPayment.Transaction_Amount%10,
	"Voulez vous verser\nun pourboire ?");

	MessageWithConfirmation(Message_confmontant);
	//APEMV_UI_InfoDisplay((char *)Message_confmontant);

	RetourConfirmPourboire = 0;

	if(Os__xget_key(FicPara.TimeOut*6) != KEY_CLEAR)
		{

		InitDefaut(_ON_, 1);
		StateHeader(0);
		switch (GL_Dialog_Menu(hGoal, "S?lectionner le pourboire :", MenuPourboire, 0, GL_BUTTON_DEFAULT, GL_KEY_1, GL_TIME_MINUTE)) {
		case 0:
			CinqP_pourboire();
			break;
		case 1:
			DixP_pourboire();
			break;
		case 2:
			QuinzeP_pourboire();
			break;
		case 3:
			Choix_pourboire();
			break;
		default:
			bContinue = FALSE;
			break;
		}
		StateHeader(1);
	    PutAllParam();
	    goto lblEnd;
		}
	lblEnd:
	return RetourConfirmPourboire;
}

//===========================================================================
int CinqP_pourboire(void) {
	UL Total, montant = StructInfoPayment.Transaction_Amount;
	Total = montant + montant * 5 / 100;

    affichage_pourboire(montant,Total);
    //StructInfoPayment.Transaction_Amount=Total;
    StructInfoPayment.MontantPourboireInterchange = montant * 5 / 100;
    return OK;
	}
//===========================================================================
int DixP_pourboire(void){
	UL Total, montant=StructInfoPayment.Transaction_Amount;
	Total=montant+montant*10/100;

	affichage_pourboire(montant,Total);
    //StructInfoPayment.Transaction_Amount=Total;
    StructInfoPayment.MontantPourboireInterchange = montant * 10 / 100;
	return OK;
}
//===========================================================================
int QuinzeP_pourboire(void){
	UL Total, montant=StructInfoPayment.Transaction_Amount;
	Total=montant+montant*15/100;

	affichage_pourboire(montant,Total);
    //StructInfoPayment.Transaction_Amount=Total;
    StructInfoPayment.MontantPourboireInterchange = montant * 15 / 100;
    return OK;
}

//===========================================================================
int Choix_pourboire(void) {
	int result;
	unsigned long entered_pourboire_amount = 0;
	unsigned short currency_code = 0;
	UL Total, montant = 0;
	int found;

	DataElement eltCurrency;


	montant = StructInfoPayment.Transaction_Amount;

    found = PAR_ReadParameter(TAG_TRANSACTION_CURRENCY_CODE, (DataElementExt *)&eltCurrency);
    if (found)
	    currency_code = (eltCurrency.ptValue[0] << 8) + eltCurrency.ptValue[1];

	ulTransactionType = CUSERV_PAIEMENT_POURBOIRE_TRANSACTION;
	StructInfoPayment.AfficheIndication_POURBOIRE_INTERCHANGE = 1;

	result = CUMESS_GetAmount(currency_code, &entered_pourboire_amount);
	if ((result) && (entered_pourboire_amount != 0))
	{
		Total=montant+entered_pourboire_amount;
		affichage_pourboire(montant,Total);
		//StructInfoPayment.Transaction_Amount=Total;
		StructInfoPayment.MontantPourboireInterchange = entered_pourboire_amount;
	}
	ulTransactionType = CUSERV_DEBIT_TRANSACTION;
	return OK ;
}

//===========================================================================
int affichage_pourboire(UL montant,UL Total ){   //p_larbi
	char Message_montant[112];
	char Message_total[112];
	memset(Message_montant, 0, sizeof(Message_montant));
	sprintf(Message_montant, "Montant : %d,%d%d MAD ", montant / 100,
			(montant / 10) % 10, montant % 10);
	memset(Message_total, 0, sizeof(Message_total));
	sprintf(Message_total, "%s \nTotal avec pourboire :\n %d,%d%d MAD \n Confirmez vous ?",
			Message_montant, Total / 100, (Total / 10) % 10, Total % 10);


//	if (ValiderMess(Message_montant, Message_total) != KEY_ENTER) {
//		RetourConfirmPourboire = 0;
//		return KO;
//	}
//	RetourConfirmPourboire = 1;
//	return OK;



	MessageWithConfirmation(Message_total);
	if (Os__xget_key(FicPara.TimeOut * 6) != KEY_CLEAR) {
		RetourConfirmPourboire = 1;
		return OK;
	}
	RetourConfirmPourboire = 0;
	return KO;
}

//===========================================================================
void CUMORE_Debit(void)
{
	ulTransactionType = CUSERV_DEBIT_TRANSACTION;
	CUMORE_PaiementFct(CUSERV_DEBIT_TRANSACTION, FALSE, (unsigned long)0, (int)0);
}

//===========================================================================
void CUMORE_Administrative(void)
{
	ulTransactionType = CUSERV_DEBIT_TRANSACTION;
	CUMORE_PaiementFct(CUSERV_DEBIT_TRANSACTION, FALSE, (unsigned long)0, (int)0);
}
//===========================================================================
void CUMORE_Inquiry(void)
{
	ulTransactionType = CUSERV_DEBIT_TRANSACTION;
	CUMORE_PaiementFct(CUSERV_DEBIT_TRANSACTION, FALSE, (unsigned long)0, (int)0);
}

void CUMORE_DebitEx(unsigned long ulAmount, int nCurrencyCode)
{
	ulTransactionType = CUSERV_DEBIT_TRANSACTION;
	CUMORE_PaiementFct(CUSERV_DEBIT_TRANSACTION, TRUE, ulAmount, nCurrencyCode);
}

//===========================================================================
void CUMORE_Refund (void)
{
	ulTransactionType = CUSERV_REFUND_TRANSACTION;
	CUMORE_PaiementFct(CUSERV_REFUND_TRANSACTION, FALSE, (unsigned long)0, (int)0);
}

//===========================================================================
//! \brief This function performs a cash transaction.
//===========================================================================
static void CUMORE_Cash (void)
{
	/* Memorize the transaction type                                       */
	ulTransactionType = CUSERV_CASH_TRANSACTION;
	CUMORE_PaiementFct(CUSERV_CASH_TRANSACTION, FALSE, (unsigned long)0, (int)0);
}

//===========================================================================
//! \brief This function performs a cash back transaction.
//===========================================================================
static void CUMORE_CashBack(void)
{
	CUMORE_PaiementFct(CUSERV_CASHBACK_TRANSACTION, FALSE, (unsigned long)0, (int)0);
}

//===========================================================================
//! \brief This function gives statistics on the batch data file.
//===========================================================================
static void CUMORE_BatchStat(void)
{
	unsigned long nb_record;
	unsigned long total_amount;
	unsigned long result;
	int merchLang;
	unsigned char *pcline1;
	char buffer[30];

	// Get manager language
	merchLang = My_PSQ_Give_Language();
	if (merchLang >= C_NBLANGMERCH)
	{
		merchLang = 0;
	}

	// Display please wait
    pcline1 = (unsigned char *)CUMESS_Read_Message(STD_MESS_PLEASE_WAIT, merchLang);
//    APEMV_UI_InfoDisplay ((char *)pcline1);

	// Read statistics
	result = CUBATCH_GiveStatistics (&nb_record, &total_amount);

	if (result)
	{
		CUPRINT_Open_Printer();

		sprintf((char *) buffer, "\x1b""E%s""\x1b""F", (unsigned char *)CUMESS_Read_Message(STAT_MESS_BATCH_STATISTICS, merchLang));
		CUPRINT_String_XT((const unsigned char*)buffer, ALIGN_LEFT, L1);

		sprintf(buffer,"\n%s   : %lu\n",CUMESS_Read_Message(STAT_MESS_NB_RECORDS, merchLang),	nb_record);
		CUPRINT_String_XT((const unsigned char*)buffer, ALIGN_LEFT, L1);

		sprintf(buffer,"%s   : %lu\n\n",CUMESS_Read_Message(STAT_MESS_TOTAL_AMOUNT, merchLang),	total_amount);
		CUPRINT_String_XT((const unsigned char*)buffer, ALIGN_LEFT, L1);

		CUPRINT_String_XT((const unsigned char*)"\n\n", ALIGN_LEFT, L1);

		CUPRINT_Close_Printer();}
}


//===========================================================================
//! \brief This function manages the Bypass PIN function.
//===========================================================================
static void CUMORE_Bypass_PIN (void)
{
	unsigned short		lg_code;
	unsigned char		msg[100];
	unsigned char		*pcline1, *pcline2;

	// Short message to display the BYPASS PIN function is activated for next transaction
	CUDB_Get_cardholder_language (&lg_code);
	if (lg_code >= C_NBLANGMERCH)
	{
		lg_code = 0;
	}


	pcline1 = (unsigned char *)CUMESS_Read_Message(DFT_MESS_NB_BYPASS_PIN_1, lg_code);
	pcline2 = (unsigned char *)CUMESS_Read_Message(DFT_MESS_NB_BYPASS_PIN_2, lg_code);
	memset(msg, 0, sizeof(msg));
	sprintf((char *)msg, "%s\n%s", pcline1, pcline2);

	APEMV_UI_MessageDisplay ((char *)msg);

	// Set the bypass required flag
	Bypass_Required = (CUMORE_TRUE[0] << 8) + CUMORE_TRUE[1];

}

//===========================================================================
//! \brief This function forces the next transaction to go online.
//===========================================================================
static void CUMORE_ForcedOnline (void)
{
	unsigned short		lg_code;
	unsigned char		msg[100];
	unsigned char		*pcline1, *pcline2;

	// Short message to display that next transaction will go online
	CUDB_Get_cardholder_language (&lg_code);
	if (lg_code >= C_NBLANGMERCH)
	{
		lg_code = 0;
	}

	pcline1 = (unsigned char *)CUMESS_Read_Message(DFT_MESS_NB_FORCED_ONLINE_1, lg_code);
	pcline2 = (unsigned char *)CUMESS_Read_Message(DFT_MESS_NB_FORCED_ONLINE_2, lg_code);
	memset(msg, 0, sizeof(msg));
	sprintf((char *)msg, "%s\n%s", pcline1, pcline2);

	APEMV_UI_MessageDisplay ((char *)msg);

	// Set the Transaction forced online flag
	TransactionForcedOnline = (CUMORE_TRUE[0] << 8) + CUMORE_TRUE[1];

}

static void CUMORE_TransactionLoopMode(void)
{
	int				merchLang;
	unsigned char	msg[100];
	unsigned char	*pc_line1, *pc_line2;

	TransactionLoopMode = FALSE;


	merchLang = My_PSQ_Give_Language();


	pc_line1 = (unsigned char *)CUMESS_Read_Message(CMN_MESS_LOOP_MODE, merchLang);
	pc_line2 = (unsigned char *)CUMESS_Read_Message(CMN_MESS_SELECTED, merchLang);
	memset(msg, 0, sizeof(msg));
	sprintf((char *)msg, "%s\n%s", pc_line1, pc_line2);
	APEMV_UI_MessageDisplay ((char *)msg);

	TransactionLoopMode = TRUE;

	ulTransactionType = CUSERV_DEBIT_TRANSACTION;
	CUMORE_PaiementFct(CUSERV_DEBIT_TRANSACTION, FALSE, (unsigned long)0, (int)0);

	TransactionLoopMode = FALSE;
	loop_amount = 0;



}


//===========================================================================
//! \brief This function sends the batch data file to the host thru ISO8583 protocol.
//===========================================================================
static void CUMORE_BatchCapt_Iso8583 (void)
{
	int cr;
	unsigned long nb_record;
	DataElement elt;
	unsigned short found;  // transfer_ok
	int transfer_ok;
	unsigned short NeedBlackList;
	int merchLang;
	unsigned char text [255];
	Telium_Date_t date;
	unsigned char raz_no_serial [ 9 ]  ;
	int bConnected;
	char *pc_l_DisplayTxt;
	TLV_TREE_NODE hTree, hOutTree,hTreeDis;
	unsigned int Trans_pos = 0;
	unsigned short Trans_index = 0;
	unsigned char *pcline1;


	hTreeDis = NULL;

	// Read number of records
	CUBATCH_InitialiseBatchTransfer (&nb_record);

	merchLang = My_PSQ_Give_Language();

	// Display please wait
	pcline1 = (unsigned char *)CUMESS_Read_Message(STD_MESS_PLEASE_WAIT, merchLang);

//	APEMV_UI_InfoDisplay ((char *)pcline1);


	/*-----------------------------------------------------------------------*/
	/* Batch transfer initialisation                                         */
	/*-----------------------------------------------------------------------*/
	found = FALSE;

	bConnected = Host_Config_Connect();

	if (bConnected)
	{
		// Sequence Counter : 9F41
		hTree = Engine_GetDataElement(TAG_TRANSACTION_SEQ_COUNTER);

		hOutTree = TlvTree_New(0);

		if(hOutTree != NULL)
		{

			found = CUSERV_Extract_Element_TLV_Tree(&hTree, TAG_TRANSACTION_SEQ_COUNTER, &elt);
			if (found)
			{
				TlvTree_AddChild(hOutTree, elt.tag, elt.ptValue, elt.length);
			}

			// release local TLV tree
			TlvTree_ReleaseEx(&hTree);

			// Improvement of IFD_serial number and terminal id management
			CUTERMgetSerialNumber((char*)raz_no_serial);
			TlvTree_AddChild(hOutTree, TAG_IFD_SERIAL_NUMBER, raz_no_serial, 8);

			TlvTree_AddChild(hOutTree, TAG_CUST_NB_RECORD_IN_BATCH, ( unsigned char *) &nb_record, 4);

			// Check Black-list validity
			NeedBlackList = !(CUBLACKL_IsValid ());
			TlvTree_AddChild(hOutTree, TAG_CUST_NEW_BLACK_LIST_NEEDED, ( unsigned char *) &NeedBlackList, 2);

			// Batch transfer initialisation
			found = FALSE;

			// Generate batch TLV tree in the same way than for authorisation
			hOutTree = EMVCUST_Build_start_transac_download_TLVTree(&hOutTree);

			hOutTree = COMM_Send_And_Receive_Message_TlvTree(hOutTree);

			found = CUSERV_Extract_Element_TLV_Tree(&hOutTree, TAG_COMM_STATUS_CODE, &elt);
		}
	}
	else
	{
		found = FALSE;
	}

	transfer_ok = (found && (*(elt.ptValue) == 0));
	if (transfer_ok)
	{
		/*-----------------------------------------------------------------------*/
		/* Batch transfer loop                                                   */
		/*-----------------------------------------------------------------------*/

		cr = CUDISK_CreateDisk();
		if (cr == FS_OK)
		{
			do
			{
				cr = Get_Next_Transaction(&Transac_File, &hTree, &Trans_pos);

				if (cr == TRANS_FILE_OK)
				{
					// Get TAG_CUST_BATCH_RECORD_TYPE
					found = CUSERV_Extract_Element_TLV_Tree(&hTree, TAG_CUST_BATCH_RECORD_TYPE, &elt);
					if (found)
					{
						// Check if record read is a financial record
						if (elt.ptValue[0] == FINANCIAL_RECORD)
						{
							transfer_ok = CUMORE_Send_Financial_Record(hTree);
						}
						else
						{
							transfer_ok = CUMORE_Send_Advice_Record(hTree);
						}

						Trans_index++;

						if (transfer_ok)
						{
							// Erase the record
							RetransmitIndicator = FALSE;
						}
						else
						{
							// Mark the record has been transmitted
							RetransmitIndicator = TRUE;
						}
					}
				}

				TlvTree_ReleaseEx(&hTree);
			}
			while ((Trans_index < nb_record) && (transfer_ok));
			// End of batch transfer loop
			// while (cr == TRANS_FILE_OK);

			if ((Trans_index == nb_record) && transfer_ok)
			{
				// All the transaction records are sent. The file can be deleted
				Del_Transaction_File(&Transac_File);
			}

			// Unmount the volume
			CUDISK_UnmountDisk();
		}

		/*-----------------------------------------------------------------------*/
		/* Batch transfer termination                                            */
		/*-----------------------------------------------------------------------*/

		// Sequence Counter : 9F41
		hTree = Engine_GetDataElement(TAG_TRANSACTION_SEQ_COUNTER);

		// release local TLV tree
		TlvTree_ReleaseEx(&hOutTree);

		hOutTree = TlvTree_New(0);

		if(hOutTree != NULL)
		{
			found = CUSERV_Extract_Element_TLV_Tree(&hTree, TAG_TRANSACTION_SEQ_COUNTER, &elt);
			if (found)
			{
				TlvTree_AddChild(hOutTree, elt.tag, elt.ptValue, elt.length);
			}

			// release local TLV tree
			TlvTree_ReleaseEx(&hTree);

			// Improvement of IFD_serial number and terminal id management
			// CUTERMgetTerminalId ( (char *) card_acceptor_terminal_id ) ;
			CUTERMgetSerialNumber((char*)raz_no_serial);
			TlvTree_AddChild(hOutTree, TAG_IFD_SERIAL_NUMBER, ( unsigned char * ) raz_no_serial, 8);

			// Mixed Communication with ISO8583 and SPDH
			// Send record
			hOutTree = EMVCUST_Build_terminate_transac_download_TLVTree(&hOutTree);

			hOutTree = COMM_Send_And_Receive_Message_TlvTree(hOutTree);

			found = CUSERV_Extract_Element_TLV_Tree(&hOutTree, TAG_COMM_STATUS_CODE, &elt);

			transfer_ok = (found && transfer_ok && (*(elt.ptValue) == 0));

		}
	}
	else
	{
		// else communication KO

		// Mixed Communication with ISO8583 and SPDH
		hOutTree = EMVCUST_Build_terminate_transac_download_TLVTree(&hOutTree);

		hOutTree = COMM_Send_And_Receive_Message_TlvTree(hOutTree);
	}

	// release local TLV tree
	TlvTree_ReleaseEx(&hOutTree);

	// Generate batch TLV tree in the same way than for authorisation
	if (bConnected)
	{
		hTreeDis=COMM_Disconnect_TlvTree (NULL);
		bConnected = FALSE;
		TlvTree_ReleaseEx(&hTreeDis);
	}

	/*-----------------------------------------------------------------------*/
	/* ticket                                                                */
	/*-----------------------------------------------------------------------*/
	CUPRINT_Open_Printer();

	sprintf ((char *) text, "\x1b""E%s""\x1b""F", CUMESS_Read_Message(DFT_MESS_NB_APPLI_NAME, merchLang));
	CUPRINT_String_XT(text, ALIGN_LEFT, L1);

	Telium_Read_date(&date);
	sprintf ((char *) text, "%.2s/%.2s/%.2s  %.2s:%.2s", date.day, date.month, date.year, date.hour, date.minute);
	CUPRINT_String_XT(text, ALIGN_LEFT, L1);

	pc_l_DisplayTxt = CUMESS_Read_Message(CNS_MESS_CAPTURE, merchLang);
	CUPRINT_String_XT((const unsigned char*)pc_l_DisplayTxt, ALIGN_LEFT, L1);

	if (transfer_ok)
	{
		// DATA CAPTURE SUCCESFUL
		pc_l_DisplayTxt = CUMESS_Read_Message(CNS_MESS_SUCCESFUL, merchLang);
		CUPRINT_String_XT((const unsigned char*)pc_l_DisplayTxt, ALIGN_LEFT, L1);
	}
	else
	{
		// DATA CAPTURE ERROR
		pc_l_DisplayTxt = CUMESS_Read_Message(CNS_MESS_ERROR, merchLang);
		CUPRINT_String_XT((const unsigned char*)pc_l_DisplayTxt, ALIGN_LEFT, L1);
	}

	CUPRINT_Paper_Feed();
	CUPRINT_Close_Printer();}

//===========================================================================
//! \brief This function erases the batch data file without data capture.
//! \return The Get_Entry return value.
//! - CR_ENTRY_OK : Successful entry.
//! \li the field d_entry[0] of Entry_buffer contains the index of the selected
//! item in the list for G_List_Entry.
//! \li the field d_len of Entry_buffer contains the entry length.
//! \li the field d_entry [50]  of Entry_buffer contains the entry for G_Numerical_Entry.
//! - CR_ENTRY_NOK : Correction (if authorised)
//! - CR_ENTRY_TIME_OUT : Timeout elapsed
//! - CR_ENTRY_CANCEL : Entry Cancelled
//! - CR_ENTRY_VAL : Green key (confirmation)
//! - CR_ENTRY_FCT : F key pushed (if authorised)
//! - CR_ENTRY_SK1 : SK1 ('1' on EFT930M or ML30) key pushed (if authorised)
//! - CR_ENTRY_SK2 : SK2 ('2' on EFT930M or ML30) key pushed (if authorised)
//! - CR_ENTRY_SK3 : SK3 ('3' on EFT930M or ML30) key pushed (if authorised)
//! - CR_ENTRY_SK4 : SK4 ('4' on EFT930M or ML30) key pushed (if authorised)
//===========================================================================
static int CUMORE_BatchCaptRaz(void)
{
	unsigned char	cr = 0;
	unsigned char	i;
	//ENTRY_BUFFER	Entry_buffer;
	unsigned short	lg_code;
	wIconList		IconLst;
    T_GL_WCHAR choice  = 0;

	memclr(&IconLst, sizeof(IconLst));

	// Init table from const

	lg_code = My_PSQ_Give_Language();

	Menu.Fenetre.titre = (unsigned char *) CUMESS_Read_Message(CMN_MESS_MERCHANT_RUN_BATCH, lg_code);

		for (i=0 ; i < ELVEC(NavigationBatchCapt) ; i++ )
		{
			Menu.tab[i] = (unsigned char *) CUMESS_Read_Message(NavigationBatchCapt[i].itemlabel, lg_code);
		}

		// Call navigation time-out 30 secs
	Menu.tab[i] = NULL;

	choice = APEMV_UI_MenuSelectTopic((const char*)Menu.Fenetre.titre,  (const char*)Menu.tab, TRUE);
	if (choice != -1)
	{
		NavigationBatchCapt[choice].function();
		cr = CR_ENTRY_OK ;
	}
	return(cr);
}

//===========================================================================
void CUMORE_BatchCapt()
{
	int Protocol_Type;

	// Mixed Communication with ISO8583 and SPDH
	Protocol_Type = Host_Comm_Protocol();
	if (Protocol_Type != COMM_PROTOCOL_TYPE_SPDH )
	{
		CUMORE_BatchCapt_Iso8583();
	}
	else
	{
		// CUMORE_BatchCapt_Spdh();
	}
}



//===========================================================================
//! \brief This function erases the batch data file.
//===========================================================================
static void CUMORE_BatchRAZ(void)
{
	int  retour;

	retour = CUDISK_CreateDisk();
	if (retour == FS_OK)
	{
		// Erase transaction file
		Del_Transaction_File(&Transac_File);

		// Unmount the volume
		CUDISK_UnmountDisk();

	}
}


//===========================================================================
//! \brief This function initialise the serial number of the terminal.
//===========================================================================
void CUMORE_Enter_Terminal_ID (void)
{
	// int cr;
	TLV_TREE_NODE hTree;
	unsigned char card_acceptor_terminal_id [ 9 ];
	int lg_code;
	StructListe Menu;


	// CUSERV_New_Parameters = TRUE;

	lg_code = My_PSQ_Give_Language();

	memclr( card_acceptor_terminal_id, 9);

	// Get current terminal ID
	CUTERMgetTerminalId((char *)card_acceptor_terminal_id);

	Menu.Fenetre.nblines = 1;

	// Title
	Menu.Fenetre.titre = (unsigned char *) CUMESS_Read_Message(CMN_MESS_MERCHANT_TERMINAL_ID2, lg_code);
	// Label
	// Default value

	APEMV_UI_NumericalEntry((const char *)Menu.Fenetre.titre,(const char *)Menu.Fenetre.titre, card_acceptor_terminal_id,  "/d/d/d/d/d/d/d/d/d");

	/*&BEGIN  FT/07/1692/MA   improvement of IFD_serial number and terminal id management  */
	hTree = TlvTree_New(0);

	if (hTree != NULL)
	{
		TlvTree_AddChild(hTree, TAG_TERMINAL_IDENTIFICATION, (unsigned char *) &card_acceptor_terminal_id, LEN_TERMINAL_IDENTIFICATION);

		CUDB_StoreTLVTree (&hTree);

		// release local TLV tree
		TlvTree_ReleaseEx(&hTree);
	}
	/*&END  FT/07/1692/MA   improvement of IFD_serial number and terminal id management  */


	Bypass_Required = (CUMORE_FALSE[0] << 8) + CUMORE_FALSE[1];

	TransactionForcedOnline = (CUMORE_FALSE[0] << 8) + CUMORE_FALSE[1];

}

//===========================================================================
//! \brief This function resets the database and the keys.
//===========================================================================
static void CUMORE_Reset(void)
{
	char *ptrDummy = 0;

	PAR_InitParam(C_PARAM_INIT_APLI, ptrDummy);
	PAR_InitParam(C_PARAM_INIT_DEFAULT, ptrDummy);

	PAR_ResetMarkList();
	CUPAR_ResetAdditionnalICS();
	CUPAR_ResetAdditionnalAID();
	CUPAR_ResetAdditionnalKRevok();

	CUPAR_InitDefFiles (FALSE);
}

//===========================================================================
//! \brief This function dumps the database from the AID parameters.
//===========================================================================
static void CUMORE_Dump_AID (void)
{
	unsigned int  i_l_NumMark = C_TAG_AID;

	CUPRINT_Open_Printer();
	CUPRINT_String_XT((const unsigned char*)"AID params :\n", ALIGN_LEFT, L1);
	CUPAR_Tag_Dump(i_l_NumMark);
	CUPRINT_Paper_Feed();
	CUPRINT_Close_Printer();
}

//===========================================================================
//! \brief This function dumps the database from the general parameters.
//===========================================================================
static void CUMORE_Dump_GParam     (void)
{
	CUPRINT_Open_Printer();

	CUPRINT_String_XT((const unsigned char*)"General params :\n", ALIGN_LEFT, L1);
	CUPAR_TLVTree_Dump(C_TREE_APPLI);
	CUPRINT_Paper_Feed();
	CUPRINT_Close_Printer();
}

//===========================================================================
//! \brief This function reload the parameters with the default values.
//===========================================================================
static void CUMORE_Default(void)
{
	int found;
	char *ptrDummy = 0;
	DataElement elt;

	CUMORE_Reset();

	CUPRINT_Open_Printer();

	PAR_InitParam(C_PARAM_DEFAULT, ptrDummy);

	CUPAR_InitDefFiles (TRUE);

	// Reset the selected mark list
	PAR_ResetMarkList();

	// Select Main ICS + ICSx (Maybe unused)
	CUPAR_SelectAdditionnalICS();

	found = PAR_ReadParameter(TAG_TRANSACTION_CURRENCY_CODE, (DataElementExt *)&elt);
	if (found)
		usDefaultCurrency  = (elt.ptValue[0] << 8) + elt.ptValue[1];

	// For real card : No delay
	ucDelayAfterAtr = FALSE;
	StoreGlobalData();
	CUPRINT_Close_Printer();

}


//===========================================================================
//! \brief This function reload the parameters from the comm.
//===========================================================================
static void CUMORE_Load_Param(void)
{
	S_FILE loadFile;
	S_FS_FILE* hFile;
	unsigned int uiAccessMode;
	char szVolumeName[1 + FS_DISKNAMESIZE + 1];
	char szFileName[FS_PATHSIZE + 1];
	char new_szFileName[FS_PATHSIZE + 1];
	char newfilename[FS_PATHSIZE + 1];
	unsigned char pFileContent[40];
	int endName,i,j;
	#define MAX_TAG_LINE 35
    TLV_TREE_NODE phOutputTLVTreeBuild;


	EMVCUST_ParamFile_Upload_TlvTree (&phOutputTLVTreeBuild);

	// open /HOST/PARAMFILE loaded by COMM Component
	sprintf(szVolumeName, "/%s", "HOST");
	if (FS_mount(szVolumeName, &uiAccessMode) == FS_OK)
	{
		sprintf(szFileName, "/%s/%s", "HOST", "PARAMFILE");
		hFile = FS_open(szFileName, "r");
		if (hFile != NULL)
		{   // read first ligne that contains the short file name
			if (FS_read(pFileContent, MAX_TAG_LINE, 1, hFile) == 1)
		    { // file name is located in the tag 9FFF00
				if (memcmp(&pFileContent[0], "9FFF01=", sizeof("9FFF01"))==0)
				{
					endName = FALSE;
                    i=0;
                    j=0;
					do
					{ // end of file name is ";"
						if (pFileContent [i + sizeof("9FFF01")] == ';')
							endName = TRUE;
						else
						{ // cancel the spaces between caracters
							if (pFileContent [i + sizeof("9FFF01")] != ' ')
							{
								newfilename[j]=pFileContent [i + sizeof("9FFF01")];
								i++;
								j++;
							}
							else
								i++;
						}
				    }
					while (!endName && ( i < MAX_TAG_LINE));

					if (endName)
					{
/*						memclr(newfilename,sizeof(newfilename));
						ix = Aschex((unsigned char *)newfilename,&asc_file_name[0],j);

						if (memcmp(&asc_file_name[0], "EMVCUST", sizeof("EMVCUST")-1)==0)
						{
							strcat(newfilename, ".PAR");
						}
						else
						{
							strcat(newfilename, "_57A9.PAR");
						}*/
						sprintf(new_szFileName, "/%s/%s", "HOST", newfilename);

						sprintf(loadFile.volume_name, "%s", "HOST");
						sprintf(loadFile.file_name, "%s", newfilename);
			            // mount the  volume
						uiAccessMode = FS_WRTMOD;

						if (FS_mount(szVolumeName, &uiAccessMode) == FS_OK)
						{ // in order to copy PARAMFILE into the "right name" file with the same contents
							FileCopy(szFileName,new_szFileName);
						}
					}
				}
		   }
		}
	FS_close(hFile);
	}
  // do the same is file_received function
  cu_file_received(ApplicationGetCurrent(), &loadFile, NULL);

  FS_mount(new_szFileName, &uiAccessMode);

  // delete the file todo ne marche pas
  FS_unlink(new_szFileName);

  // delete PARAMFILE from HOST todo ne marche pas
  FS_unlink(szFileName);

}
#define COPY_BUFFER_SIZE 16*1024

static int FileCopy(char *in, char *out)
{
  S_FS_FILE     *fd_in;
  S_FS_FILE     *fd_out;
  int            lg;
  unsigned long  w_lg,r_lg;
  unsigned char *buffer;


  fd_in = FS_open(in, "r");
  if (fd_in == NULL)
  {
    return(1);
  }

  FS_unlink(out);
  fd_out = FS_open(out,"a");
  if(fd_out == NULL)
  {
    FS_close(fd_in);
    return(1);
  }

  w_lg = 0;
  r_lg = FS_length(fd_in);
  buffer=umalloc(COPY_BUFFER_SIZE);
  do
  {
    lg = COPY_BUFFER_SIZE ;
    lg = FS_read(buffer, 1, lg, fd_in);
    if (lg>0)
    {
      w_lg += FS_write(buffer, 1, lg, fd_out);
    }
  }

  while(lg>0);
  ufree(buffer);
  FS_close(fd_in);
  FS_close(fd_out);

  FS_unlink(in);

  if(w_lg != r_lg)
  {
    return(1);
  }

  return(0);
}

#ifdef _TOOLWARE_
//===========================================================================
//! \brief This function manages the demo mode selection menu.
//===========================================================================
static void CUMORE_Demo_Mode(void)
{
	T_CUTERMkey		key;
	int				merchLang;
	unsigned short	mask = 0;
	byte			b_l_TimeOut;
	unsigned char	msg[100];
	unsigned char	*pc_line1, *pc_line2;

	CUTERMclearDisplay();
	merchLang = My_PSQ_Give_Language();
	key = CU_VALID; /* init =/= CANCEL */

	if (ct_SystemSupportToolWare == FALSE)
	{
		ucAdvancedMode = FALSE;
		pc_line1 = CUMESS_Read_Message(DEMO_MODE_TABLE, DEMO_MESS_NOT_SUPPORTED, merchLang);
		pc_line2 = CUMESS_Read_Message(DEMO_MODE_TABLE, DEMO_MESS_YOUR_SYSTEM, merchLang);
		memset(msg, 0, sizeof(msg));
		sprintf(msg, "%s\n%s", pc_line1, pc_line2);
		CUTERMdisplayLine(0, msg, LEFT, (char *)"w");

		CUTERM_Wait_function_key(2, mask, &key);

		return;
	}


	// Enter type of ticket required
	pc_line1 = CUMESS_Read_Message(DEMO_MODE_TABLE, DEMO_MESS_USE_DEMO_MODE, merchLang);
	pc_line2 = CUMESS_Read_Message(DEMO_MODE_TABLE, DEMO_MESS_POS_ADVANCED, merchLang);
	memset(msg, 0, sizeof(msg));
	sprintf(msg, "%s\n%s", pc_line1, pc_line2);
	CUTERMdisplayLine(0, msg, LEFT, (char *)"w");


	key = CU_VALID; /* init =/= CANCEL */
	// Init timeout to 10s
	b_l_TimeOut = 10;
	mask = MASK_ANNULATION | MASK_SKCORR | MASK_SK2 | MASK_VALIDATION | MASK_SKVAL |MASK_SK3;

	// Waits 10s with mask
	if (CUTERM_Wait_function_key(b_l_TimeOut, mask, &key) == TRUE)
	{
		//char * PtrMsg;
		unsigned short mask = 0;

		if ((key == T_ANN) ||
			(key == T_CORR) ||
			(key == T_SK2))
		{
			ucAdvancedMode = FALSE;
			CUTERMclearDisplay();
			pc_line1 = CUMESS_Read_Message(DEMO_MODE_TABLE, DEMO_MESS_POS_DEMO, merchLang);
			pc_line2 = CUMESS_Read_Message(DEMO_MODE_TABLE, DEMO_MESS_SELECTED, merchLang);
			memset(msg, 0, sizeof(msg));
			sprintf(msg, "%s\n%s", pc_line1, pc_line2);
			CUTERMdisplayLine(0, msg, LEFT, (char *)"w");
			CUTERM_Wait_function_key(2, mask, &key);
		}
		else if ((key == T_VAL) ||
			(key == T_SK3))
		{
			ucAdvancedMode = TRUE;
			CUTERMclearDisplay();
			pc_line1 = CUMESS_Read_Message(DEMO_MODE_TABLE, DEMO_MESS_ADVANCED_DEMO, merchLang);
			pc_line2 = CUMESS_Read_Message(DEMO_MODE_TABLE, DEMO_MESS_SELECTED, merchLang);
			memset(msg, 0, sizeof(msg));
			sprintf(msg, "%s\n%s", pc_line1, pc_line2);
			CUTERMdisplayLine(0, msg, LEFT, (char *)"w");
			CUTERM_Wait_function_key(2, mask, &key);
		}
		// else time-out

	CUTERMclearDisplay();
	}
}
#endif /* _TOOLWARE_ */

//===========================================================================
//! \brief This function manages the printing of the receipt of the current ICS.
//===========================================================================
static void CUMORE_Dump_ICS(void)
{
	unsigned short lg_code;
	unsigned char msg[100];
	unsigned char *pc_line1, *pc_line2;

	// Retreive language code
	CUDB_Get_cardholder_language (&lg_code);
	if (lg_code > C_NBLANGHOLDER)
	{
		lg_code = 0;
	}


	pc_line1 = (unsigned char *)CUMESS_Read_Message(ICS_MESS_PRINTING_1, lg_code);
	pc_line2 = (unsigned char *)CUMESS_Read_Message(ICS_MESS_IN_PROGRESS, lg_code);
	memset(msg, 0, sizeof(msg));
	sprintf((char *)msg, "%s\n%s", pc_line1, pc_line2);

	APEMV_UI_MessageDisplay ((char *)msg);
	{
		CUPAR_Print_ICS_Config();
	}


}

//===========================================================================
//! \brief This function sends one financial record.
//! \param[in] hInputTree : Input TLV tree containing the financial transaction
//! read in the batch file.
//! \return
//! - TRUE : The transmission is successful.
//! - FALSE : in case of error.
//===========================================================================
static int CUMORE_Send_Financial_Record (TLV_TREE_NODE hInputTree)
{
	DataElement elt;
	unsigned short transfer_ok, found;
	TLV_TREE_NODE hTree,hTreeSR;
	unsigned char ISC_len;

	transfer_ok = 0;
	hTreeSR = NULL;
	hTree = TlvTree_New(0);

	if ((hTree != NULL) && (hInputTree != NULL))
	{
		found = CUSERV_Extract_Element_TLV_Tree(&hInputTree, TAG_AIP, &elt);
		if (found)
		{
			TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length);
		}

		found = CUSERV_Extract_Element_TLV_Tree(&hInputTree, TAG_ATC, &elt);
		if (found)
		{
			TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length);
		}

		/*& BEGIN SMO/ETU/05/20028/MA : Remove unused tags because DEL is too small ! */
		/* found = CUSERV_Extract_Element_TLV_Tree(&hInputTree, TAG_APPLI_USAGE_CONTROL, &elt);
		if (found)
		{
			TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length);
		}*/
		/*& END SMO/ETU/05/20028/MA */

		found = CUSERV_Extract_Element_TLV_Tree(&hInputTree, TAG_CRYPTOGRAM_INFORMATION_DATA, &elt);
		if (found)
		{
			TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length);
		}

		/*& BEGIN SMO/ETU/05/20028/MA : Remove unused tags because DEL is too small ! */
		/* found = CUSERV_Extract_Element_TLV_Tree(&hInputTree, TAG_CVM_LIST, &elt);
		if (found)
		{
			TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length);
		}*/
		/*& END SMO/ETU/05/20028/MA */

		found = CUSERV_Extract_Element_TLV_Tree(&hInputTree, TAG_CVR_RESULT, &elt);
		if (found)
		{
			TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length);
		}

		found = CUSERV_Extract_Element_TLV_Tree(&hInputTree, TAG_IFD_SERIAL_NUMBER, &elt);
		if (found)
		{
			TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length);
		}

		/*& BEGIN SMO/ETU/05/20028/MA : Remove unused tags because DEL is too small ! */
		/* found = CUSERV_Extract_Element_TLV_Tree(&hInputTree, TAG_ISSUER_ACTION_CODE_DEFAULT, &elt);
		if (found)
		{
			TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length);
		}*/

		/* found = CUSERV_Extract_Element_TLV_Tree(&hInputTree, TAG_ISSUER_ACTION_CODE_DENIAL, &elt);
		if (found)
		{
			TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length);
		} */

		/* found = CUSERV_Extract_Element_TLV_Tree(&hInputTree, TAG_ISSUER_ACTION_CODE_ONLINE, &elt);
		if (found)
		{
			TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length);
		}*/
		/*& END SMO/ETU/05/20028/MA */

		found = CUSERV_Extract_Element_TLV_Tree(&hInputTree, TAG_ISSUER_SCRIPT_RESULT_LIST_LEN, &elt);
		if (found)
		{
			ISC_len = elt.ptValue[0];
			// Bad storage of Issuer Script results
			if (ISC_len > 0)
			{
				/* elt.tag     = TAG_ISSUER_SCRIPT_RESULT_LIST_LEN;
				elt.length  = 1;
				elt.ptValue = &(batch_record.record.financial_record.new_data.ISC_len);*/
				TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length);

				/* elt.tag     = TAG_ISSUER_SCRIPT_RESULT_LIST;
				elt.length  = batch_record.record.financial_record.new_data.ISC_len;
				elt.ptValue = batch_record.record.financial_record.new_data.ISC;*/
				found = CUSERV_Extract_Element_TLV_Tree(&hInputTree, TAG_ISSUER_SCRIPT_RESULT_LIST, &elt);
				if (found)
				{
					elt.length = ISC_len;
					TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length);
				}
			}
		}

		found = CUSERV_Extract_Element_TLV_Tree(&hInputTree, TAG_TERMINAL_CAPABILITIES, &elt);
		if (found)
		{
			TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length);
		}

		found = CUSERV_Extract_Element_TLV_Tree(&hInputTree, TAG_TERMINAL_TYPE, &elt);
		if (found)
		{
			TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length);
		}

		found = CUSERV_Extract_Element_TLV_Tree(&hInputTree, TAG_TVR, &elt);
		if (found)
		{
			TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length);
		}

		// Add TSI to transaction file
		found = CUSERV_Extract_Element_TLV_Tree(&hInputTree, TAG_TSI, &elt);
		if (found)
		{
			TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length);
		}

		found = CUSERV_Extract_Element_TLV_Tree(&hInputTree, TAG_APPLI_CRYPTOGRAMME_AC, &elt);
		if (found)
		{
			TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length);
		}

		found = CUSERV_Extract_Element_TLV_Tree(&hInputTree, TAG_UNPREDICTABLE_NUMBER, &elt);
		if (found)
		{
			TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length);
		}

		found = CUSERV_Extract_Element_TLV_Tree(&hInputTree, TAG_ACQUIRER_IDENTIFIER, &elt);
		if (found)
		{
			TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length);
		}

		/* elt.tag     = TAG_AMOUNT_AUTH_BIN;
		elt.length  = 4;
		LongToChar (batch_record.record.financial_record.exist_data.AmAuth, TabAmount);
		elt.ptValue = TabAmount; */
		found = CUSERV_Extract_Element_TLV_Tree(&hInputTree, TAG_AMOUNT_AUTH_BIN, &elt);
		if (found)
		{
			TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length);
		}

		/* elt.tag     = TAG_AMOUNT_OTHER_BIN;
		elt.length  = 4;
		LongToChar (batch_record.record.financial_record.exist_data.AmOth, TabAmount1);
		elt.ptValue = TabAmount1;*/
		found = CUSERV_Extract_Element_TLV_Tree(&hInputTree, TAG_AMOUNT_OTHER_BIN, &elt);
		if (found)
		{
			TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length);
		}

		found = CUSERV_Extract_Element_TLV_Tree(&hInputTree, TAG_APPLI_EFFECTIVE_DATE, &elt);
		if (found)
		{
			TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length);
		}

		found = CUSERV_Extract_Element_TLV_Tree(&hInputTree, TAG_APPLI_EXPIRATION_DATE, &elt);
		if (found)
		{
			TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length);
		}

		found = CUSERV_Extract_Element_TLV_Tree(&hInputTree, TAG_APPLI_PRIM_ACCOUNT_NB, &elt);
		if (found)
		{
			TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length);
		}

		found = CUSERV_Extract_Element_TLV_Tree(&hInputTree, TAG_APPLI_PRIM_ACCOUNT_NB_SEQ_NB, &elt);
		if (found)
		{
			TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length);
		}

		found = CUSERV_Extract_Element_TLV_Tree(&hInputTree, TAG_AUTHORISATION_CODE, &elt);
		if (found)
		{
			TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length);
		}

		found = CUSERV_Extract_Element_TLV_Tree(&hInputTree, TAG_AUTHORISATION_RESPONSE_CODE, &elt);
		if (found)
		{
			TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length);
		}

		found = CUSERV_Extract_Element_TLV_Tree(&hInputTree, TAG_ISSUER_COUNTRY_CODE, &elt);
		if (found)
		{
			TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length);
		}

		found = CUSERV_Extract_Element_TLV_Tree(&hInputTree, TAG_MERCHANT_CATEGORY_CODE, &elt);
		if (found)
		{
			TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length);
		}

		found = CUSERV_Extract_Element_TLV_Tree(&hInputTree, TAG_POS_ENTRY_MODE_CODE, &elt);
		if (found)
		{
			TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length);
		}

		// Point of Service Entry Mode
		found = CUSERV_Extract_Element_TLV_Tree(&hInputTree, TAG_CUST_POS_ISO8583_93, &elt);
		if (found)
		{
			TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length);
		}

		found = CUSERV_Extract_Element_TLV_Tree(&hInputTree, TAG_TERMINAL_COUNTRY_CODE, &elt);
		if (found)
		{
			TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length);
		}

		/*& BEGIN SMO/ETU/05/20028/MA : Remove unused tags because DEL is too small ! */
		/* found = CUSERV_Extract_Element_TLV_Tree(&hInputTree, TAG_TERMINAL_IDENTIFICATION, &elt);
		if (found)
		{
			TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length);
		} */
		/*& END SMO/ETU/05/20028/MA */

		found = CUSERV_Extract_Element_TLV_Tree(&hInputTree, TAG_TRANSACTION_CURRENCY_CODE, &elt);
		if (found)
		{
			TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length);
		}

		found = CUSERV_Extract_Element_TLV_Tree(&hInputTree, TAG_TRANSACTION_DATE, &elt);
		if (found)
		{
			TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length);
		}

		found = CUSERV_Extract_Element_TLV_Tree(&hInputTree, TAG_TRANSACTION_TIME, &elt);
		if (found)
		{
			TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length);
		}

		found = CUSERV_Extract_Element_TLV_Tree(&hInputTree, TAG_TRANSACTION_TYPE, &elt);
		if (found)
		{
			TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length);
		}

		// Issuer Application Data 0x9F10
		found = CUSERV_Extract_Element_TLV_Tree(&hInputTree, TAG_ISSUER_APPLICATION_DATA, &elt);
		if (found)
		{
			if (elt.length > 0)
			{
				TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length);
			}
		}

		// Amount, Authorised 0x9F02 (numeric)
		found = CUSERV_Extract_Element_TLV_Tree(&hInputTree, TAG_AMOUNT_AUTH_NUM, &elt);
		if (found)
		{
			TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length);
		}

		// Amount, Other 0x9F03 (numeric)
		found = CUSERV_Extract_Element_TLV_Tree(&hInputTree, TAG_AMOUNT_OTHER_NUM, &elt);
		if (found)
		{
			TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length);
		}

		// AID : 4F
		found = CUSERV_Extract_Element_TLV_Tree(&hInputTree, TAG_AID_ICC, &elt);
		if (found)
		{
			if (elt.length > 0)
			{
				TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length);
			}
		}

		// Service Code : 5F30
		found = CUSERV_Extract_Element_TLV_Tree(&hInputTree, TAG_SERVICE_CODE, &elt);
		if (found)
		{
			TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length);
		}

		// Sequence Counter : 9F41
		found = CUSERV_Extract_Element_TLV_Tree(&hInputTree, TAG_TRANSACTION_SEQ_COUNTER, &elt);
		if (found)
		{
			TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length);
		}

		// Track 2 : 57
		found = CUSERV_Extract_Element_TLV_Tree(&hInputTree, TAG_TRACK2_EQU_DATA, &elt);
		if (found)
		{
			if (elt.length > 0)
			{
				TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length);
			}
		}

		// Transaction Currency Exponent : 5F36
		found = CUSERV_Extract_Element_TLV_Tree(&hInputTree, TAG_TRANSACTION_CURRENCY_EXP, &elt);
		if (found)
		{
			TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length);
		}

		// Application Version Number : 9F09
		found = CUSERV_Extract_Element_TLV_Tree(&hInputTree, TAG_VERSION_NUMBER_TERMINAL, &elt);
		if (found)
		{
			TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length);
		}

		/*& BEGIN SMO/ETU/05/20028/MA : Remove unused tags because DEL is too small ! */
		/*
		// Add Application Identifier : 9F06
		found = CUSERV_Extract_Element_TLV_Tree(&hInputTree, TAG_AID_TERMINAL, &elt);
		if (found)
		{
			if (elt.length > 0)
			{
				TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length);
			}
		}*/
		/*& END SMO/ETU/05/20028/MA */

		// TAG_TRANSACTION_FORCED_ACCEPTANCE is tested in Script CM.082
		found = CUSERV_Extract_Element_TLV_Tree(&hInputTree, TAG_TRANSACTION_FORCED_ACCEPTANCE, &elt);
		if (found)
		{
			TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length);
		}

		/*& BEGIN SMO/ETU/05/20028/MA : Remove unused tags because DEL is too small ! */
		/* elt.tag     = TAG_COMM_MESSAGE_RETRANSMISSION;
		elt.length  = 1;
		elt.ptValue = &RetransmitIndicator;
		TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length); */
		/*& END SMO/ETU/05/20028/MA */

		// Add ICC dynamic number in batch file
		found = CUSERV_Extract_Element_TLV_Tree(&hInputTree, TAG_ICC_DYNAMIC_NUMBER, &elt);
		if (found)
		{
			TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length);
		}

		//Add Payement Account Reference
		found = CUSERV_Extract_Element_TLV_Tree(&hInputTree, TAG_PAYMENT_ACCOUNT_REFERENCE, &elt);
		if (found)
		{
			TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length);
		}

		// Mixed Communication with ISO8583 and SPDH
		// Send record
		hTree = EMVCUST_Build_transac_download_TLVTree(&hTree);

		hTreeSR = COMM_Send_And_Receive_Message_TlvTree(hTree);

		found = CUSERV_Extract_Element_TLV_Tree(&hTreeSR, TAG_COMM_STATUS_CODE, &elt);

		transfer_ok = (found && (*(elt.ptValue) == 0));

		// release local TLV tree
		TlvTree_ReleaseEx(&hTree);
		TlvTree_ReleaseEx(&hTreeSR);

	}

	return (transfer_ok);

}

//===========================================================================
//! \brief This function sends one advice record.
//! \param[in] hInputTree : Input TLV tree containing the advice transaction
//! read in the batch file.
//! \return
//! - TRUE : The transmission is successful.
//! - FALSE : in case of error.
//===========================================================================
static int CUMORE_Send_Advice_Record (TLV_TREE_NODE hInputTree)
{
	DataElement elt;
	unsigned short transfer_ok, found;
	TLV_TREE_NODE hTree,hTreeSR;
	unsigned char ISC_len;

	transfer_ok = 0;
	hTree = TlvTree_New(0);
	hTreeSR = NULL;

	if ((hTree != NULL) && (hInputTree != NULL))
	{
		found = CUSERV_Extract_Element_TLV_Tree(&hInputTree, TAG_AIP, &elt);
		if (found)
		{
			TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length);
		}

		found = CUSERV_Extract_Element_TLV_Tree(&hInputTree, TAG_ATC, &elt);
		if (found)
		{
			TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length);
		}

		/*& BEGIN SMO/ETU/05/20028/MA : Remove unused tags because DEL is too small ! */
		/* found = CUSERV_Extract_Element_TLV_Tree(&hInputTree, TAG_APPLI_USAGE_CONTROL, &elt);
		if (found)
		{
			TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length);
		}*/
		/*& END SMO/ETU/05/20028/MA */

		found = CUSERV_Extract_Element_TLV_Tree(&hInputTree, TAG_CRYPTOGRAM_INFORMATION_DATA, &elt);
		if (found)
		{
			TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length);
		}

		/*& BEGIN SMO/ETU/05/20028/MA : Remove unused tags because DEL is too small ! */
		/* found = CUSERV_Extract_Element_TLV_Tree(&hInputTree, TAG_CVM_LIST, &elt);
		if (found)
		{
			TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length);
		}*/
		/*& END SMO/ETU/05/20028/MA */

		found = CUSERV_Extract_Element_TLV_Tree(&hInputTree, TAG_CVR_RESULT, &elt);
		if (found)
		{
			TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length);
		}

		found = CUSERV_Extract_Element_TLV_Tree(&hInputTree, TAG_IFD_SERIAL_NUMBER, &elt);
		if (found)
		{
			TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length);
		}

		/*& BEGIN SMO/ETU/05/20028/MA : Remove unused tags because DEL is too small ! */
		/* found = CUSERV_Extract_Element_TLV_Tree(&hInputTree, TAG_ISSUER_ACTION_CODE_DEFAULT, &elt);
		if (found)
		{
			TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length);
		}*/

		/* found = CUSERV_Extract_Element_TLV_Tree(&hInputTree, TAG_ISSUER_ACTION_CODE_DENIAL, &elt);
		if (found)
		{
			TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length);
		}*/

		/* found = CUSERV_Extract_Element_TLV_Tree(&hInputTree, TAG_ISSUER_ACTION_CODE_ONLINE, &elt);
		if (found)
		{
			TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length);
		}*/
		/*& END SMO/ETU/05/20028/MA */

		found = CUSERV_Extract_Element_TLV_Tree(&hInputTree, TAG_ISSUER_SCRIPT_RESULT_LIST_LEN, &elt);
		if (found)
		{
			ISC_len = elt.ptValue[0];
			// Bad storage of Issuer Script results
			if (ISC_len > 0)
			{
				TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length);

				found = CUSERV_Extract_Element_TLV_Tree(&hInputTree, TAG_ISSUER_SCRIPT_RESULT_LIST, &elt);
				if (found)
				{
					elt.length = ISC_len;
					TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length);
				}
			}
		}

		/* if (batch_record.record.advice_record.adv_new_data.ISC_len > 0)
		{
			elt.tag     = TAG_ISSUER_SCRIPT_RESULT_LIST_LEN;
			elt.length  = 1;
			elt.ptValue = &(batch_record.record.advice_record.adv_new_data.ISC_len);
			TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length);

			elt.tag     = TAG_ISSUER_SCRIPT_RESULT_LIST;
			elt.length  = batch_record.record.advice_record.adv_new_data.ISC_len;
			elt.ptValue = batch_record.record.advice_record.adv_new_data.ISC;
			TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length);
		} */

		found = CUSERV_Extract_Element_TLV_Tree(&hInputTree, TAG_TERMINAL_CAPABILITIES, &elt);
		if (found)
		{
			TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length);
		}

		found = CUSERV_Extract_Element_TLV_Tree(&hInputTree, TAG_TERMINAL_TYPE, &elt);
		if (found)
		{
			TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length);
		}

		found = CUSERV_Extract_Element_TLV_Tree(&hInputTree, TAG_TVR, &elt);
		if (found)
		{
			TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length);
		}

		found = CUSERV_Extract_Element_TLV_Tree(&hInputTree, TAG_APPLI_CRYPTOGRAMME_AC, &elt);
		if (found)
		{
			TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length);
		}

		found = CUSERV_Extract_Element_TLV_Tree(&hInputTree, TAG_UNPREDICTABLE_NUMBER, &elt);
		if (found)
		{
			TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length);
		}

		found = CUSERV_Extract_Element_TLV_Tree(&hInputTree, TAG_ACQUIRER_IDENTIFIER, &elt);
		if (found)
		{
			TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length);
		}

		/* elt.tag     = TAG_AMOUNT_AUTH_BIN;
		elt.length  = 4;
		LongToChar (batch_record.record.advice_record.adv_exist_data.AmAuth, TabAmount);
		elt.ptValue = TabAmount; */
		found = CUSERV_Extract_Element_TLV_Tree(&hInputTree, TAG_AMOUNT_AUTH_BIN, &elt);
		if (found)
		{
			TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length);
		}

		/* elt.tag     = TAG_AMOUNT_OTHER_BIN;
		elt.length  = 4;
		LongToChar (batch_record.record.advice_record.adv_exist_data.AmOth, TabAmount1);
		elt.ptValue = TabAmount1; */
		found = CUSERV_Extract_Element_TLV_Tree(&hInputTree, TAG_AMOUNT_OTHER_BIN, &elt);
		if (found)
		{
			TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length);
		}

		found = CUSERV_Extract_Element_TLV_Tree(&hInputTree, TAG_APPLI_EFFECTIVE_DATE, &elt);
		if (found)
		{
			TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length);
		}

		found = CUSERV_Extract_Element_TLV_Tree(&hInputTree, TAG_APPLI_EXPIRATION_DATE, &elt);
		if (found)
		{
			TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length);
		}

		found = CUSERV_Extract_Element_TLV_Tree(&hInputTree, TAG_APPLI_PRIM_ACCOUNT_NB, &elt);
		if (found)
		{
			TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length);
		}

		found = CUSERV_Extract_Element_TLV_Tree(&hInputTree, TAG_APPLI_PRIM_ACCOUNT_NB_SEQ_NB, &elt);
		if (found)
		{
			TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length);
		}

		found = CUSERV_Extract_Element_TLV_Tree(&hInputTree, TAG_AUTHORISATION_RESPONSE_CODE, &elt);
		if (found)
		{
			TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length);
		}

		found = CUSERV_Extract_Element_TLV_Tree(&hInputTree, TAG_MERCHANT_CATEGORY_CODE, &elt);
		if (found)
		{
			TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length);
		}

		found = CUSERV_Extract_Element_TLV_Tree(&hInputTree, TAG_POS_ENTRY_MODE_CODE, &elt);
		if (found)
		{
			TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length);
		}

		found = CUSERV_Extract_Element_TLV_Tree(&hInputTree, TAG_TERMINAL_COUNTRY_CODE, &elt);
		if (found)
		{
			TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length);
		}

		/*& BEGIN SMO/ETU/05/20028/MA : Remove unused tags because DEL is too small ! */
		/* found = CUSERV_Extract_Element_TLV_Tree(&hInputTree, TAG_TERMINAL_IDENTIFICATION, &elt);
		if (found)
		{
			TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length);
		}*/
		/*& END SMO/ETU/05/20028/MA */

		found = CUSERV_Extract_Element_TLV_Tree(&hInputTree, TAG_TRANSACTION_CURRENCY_CODE, &elt);
		if (found)
		{
			TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length);
		}

		found = CUSERV_Extract_Element_TLV_Tree(&hInputTree, TAG_TRANSACTION_DATE, &elt);
		if (found)
		{
			TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length);
		}

		found = CUSERV_Extract_Element_TLV_Tree(&hInputTree, TAG_TRANSACTION_TIME, &elt);
		if (found)
		{
			TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length);
		}

		found = CUSERV_Extract_Element_TLV_Tree(&hInputTree, TAG_TRANSACTION_TYPE, &elt);
		if (found)
		{
			TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length);
		}

		// Issuer Application Data 0x9F10
		found = CUSERV_Extract_Element_TLV_Tree(&hInputTree, TAG_ISSUER_APPLICATION_DATA, &elt);
		if (found)
		{
			if (elt.length > 0)
			{
				TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length);
			}
		}

		// Track 2 : 57
		found = CUSERV_Extract_Element_TLV_Tree(&hInputTree, TAG_TRACK2_EQU_DATA, &elt);
		if (found)
		{
			if (elt.length > 0)
			{
				TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length);
			}
		}
		
		//Add Payement Account Reference
		found = CUSERV_Extract_Element_TLV_Tree(&hInputTree, TAG_PAYMENT_ACCOUNT_REFERENCE, &elt);
		if (found)
		{
			TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length);
		}
		/*& BEGIN SMO/ETU/05/20028/MA : Remove unused tags because DEL is too small ! */
		/* found = CUSERV_Extract_Element_TLV_Tree(&hInputTree, TAG_TRANSACTION_FORCED_ACCEPTANCE, &elt);
		if (found)
		{
			TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length);
		}*/
		/*& END SMO/ETU/05/20028/MA */

		/*& BEGIN SMO/ETU/05/20028/MA : Remove unused tags because DEL is too small ! */
		/* elt.tag = TAG_COMM_MESSAGE_RETRANSMISSION;
		elt.length  = 1;
		elt.ptValue = &RetransmitIndicator;
		TlvTree_AddChild(hTree, elt.tag, elt.ptValue, elt.length); */
		/*& END SMO/ETU/05/20028/MA */

		// Mixed Communication with ISO8583 and SPDH
		// Send record
		hTree = EMVCUST_Build_transac_advice_TLVTree(&hTree);

		hTreeSR = COMM_Send_And_Receive_Message_TlvTree(hTree);

		found = CUSERV_Extract_Element_TLV_Tree(&hTreeSR, TAG_COMM_STATUS_CODE, &elt);

		transfer_ok = (found && (*(elt.ptValue) == 0));

		// release local TLV tree
		TlvTree_ReleaseEx(&hTree);
		TlvTree_ReleaseEx(&hTreeSR);
	}

	return (transfer_ok);

}

//===========================================================================
//! \brief This function manages the printing of the transaction log of the ICC.
//===========================================================================
static void CUMORE_TransactionLog(void)
{
	unsigned short TransactionLogAsked;
	unsigned char    buf[11];
	TLV_TREE_NODE hInputTree;
	TLV_TREE_NODE hOutputTree;
	unsigned char TypeAppli[2];
	unsigned char EngAppliSel;

	// release global TLV tree
	TlvTree_ReleaseEx(&CUMORE_Tree);

	CUMORE_Tree = TlvTree_New(0);

	if(CUMORE_Tree != NULL)
	{
		TransactionLogAsked = TRUE;
		ShortToChar(TransactionLogAsked,buf);
		TlvTree_AddChild(CUMORE_Tree, TAG_CUST_TRLOG_IS_ASKED, (unsigned char *)buf, 2);

		hInputTree = TlvTree_New(0);
		if(hInputTree != NULL)
		{
			ShortToChar(CustApplicationNumber,TypeAppli);
			TlvTree_AddChild(hInputTree, TAG_ENG_CUSTOM_APPLI_TYPE, (unsigned char*) &TypeAppli, 2);

			EngAppliSel =0;
			TlvTree_AddChild(hInputTree, TAG_ENG_APPLI_SEL_ALREADY_DONE, (unsigned char*) & EngAppliSel, 1);

			hOutputTree = NULL;
			Engine_DoReadTransactionLog(hInputTree,&hOutputTree);

			// Release local TLV tree
			TlvTree_ReleaseEx(&hInputTree);
			TlvTree_ReleaseEx(&hOutputTree);
		}
	}
}


//===========================================================================
//! \brief This function manages the printing of the transaction log of the ICC.
//===========================================================================
 static void CUMORE_TransactionReactivate(void)
{
   TransactionReactivateAsked = TRUE;

	ulTransactionType = CUSERV_DEBIT_TRANSACTION;
	CUMORE_PaiementFct(CUSERV_DEBIT_TRANSACTION, FALSE, (unsigned long)0, (int)0);

    TransactionReactivateAsked = FALSE;
}


//===========================================================================
//! \brief This function performs a transaction according to transaction type
//! given as parameter.
//! \param[in] TransactionType : type of the transaction.
//! \param ucParameters  : TRUE if ulAmount and nCurrencyCode parameters must be used.
//! \param ulAmount      : amount entered.
//! \param nCurrencyCode : ISO currency code.
//! \ref CUSERV_DEBIT_TRANSACTION
//! \ref CUSERV_REFUND_TRANSACTION
//! \ref CUSERV_CASH_TRANSACTION
//! \ref CUSERV_CASHBACK_TRANSACTION
//===========================================================================
void CUMORE_PaiementFctTrame(unsigned long TransactionType, unsigned char ucParameters, unsigned long ulAmount, int nCurrencyCode)
{
	int previousUiState;
#ifdef _ENABLE_CAM2_
	static const unsigned long ulChipReaders = CAM0 | CAM2;
#else
	static const unsigned long ulChipReaders = CAM0;
#endif
	int result;
	int found;
	unsigned short currency_code = 0;
	unsigned char  TabAmount [4];
	unsigned char  TabAmountC [4];
	unsigned char  NumAmount [6];
	unsigned char  NumAmountC [6];
	unsigned long  entered_amount;
	DataElement eltCurrency,eltCurrencyExp;
	unsigned char TypeAppli[2];
	unsigned char EngAppliSel;
	TLV_TREE_NODE hInputTree;
	TLV_TREE_NODE hOutputTree;
	unsigned char ucTimeout[4];

	if(memcmp(FicPara.MCC,"6010",4) == 0)
	{
		//??
		StructInfoPayment.CodeFunction = CASHADVANCESCODE;
	}
	else
		StructInfoPayment.CodeFunction = SALECODE;



	// Reset the selected mark list
	PAR_ResetMarkList();

	// Select Main ICS + ICSx (Maybe unused)
	CUPAR_SelectAdditionnalICS();

	// -----------------------------------------------------------------
	// Amount Entry
	// -----------------------------------------------------------------

    if (ucParameters == TRUE) // If parameters given in interface must be used
    {
        result = T_VAL;
        entered_amount = ulAmount;
        currency_code = nCurrencyCode;
    }
    else // Parameters : amount and currency code must be inputed or found ...
    {
	    // No card is present at this moment. The currency used is the Terminal currency
	    found = PAR_ReadParameter(TAG_TRANSACTION_CURRENCY_CODE, (DataElementExt *)&eltCurrency);
	    if (found)
		    currency_code = (eltCurrency.ptValue[0] << 8) + eltCurrency.ptValue[1];

		if (TransactionLoopMode == TRUE)
		{
			if (loop_amount == 0)
			{

				result = CUMESS_GetAmount (currency_code, &entered_amount);
				if ((result) && (entered_amount!=0))
				{
					loop_amount = entered_amount;
				}
			}
			else
			{
				entered_amount = loop_amount;
				result = T_VAL;
			}
		}
		else
			result = CUMESS_GetAmount (currency_code, &entered_amount);


    }

	if ((result) && (entered_amount!=0))
	{
		// Amount is valid. Convert in num
		CUMESS_ConvertBinNUM (entered_amount, NumAmount);

		LongToChar (entered_amount, TabAmount);

		// Store amounts in current database

		hInputTree = TlvTree_New(0);

		if(hInputTree != NULL)
		{
			ShortToChar(CustApplicationNumber,TypeAppli);
			TlvTree_AddChild(hInputTree, TAG_ENG_CUSTOM_APPLI_TYPE, (unsigned char*) &TypeAppli, 2);

			EngAppliSel =0;
			TlvTree_AddChild(hInputTree, TAG_ENG_APPLI_SEL_ALREADY_DONE, (unsigned char*) & EngAppliSel, 1);

			TlvTree_AddChild(hInputTree, TAG_TRANSACTION_CURRENCY_CODE, (unsigned char*) &eltCurrency.ptValue[0], 2);

			found = PAR_ReadParameter(TAG_TRANSACTION_CURRENCY_CODE, (DataElementExt *)&eltCurrencyExp);
			if (found)
			{
				TlvTree_AddChild(hInputTree, TAG_TRANSACTION_CURRENCY_EXP, (unsigned char*) &eltCurrencyExp.ptValue[0], 1);
			}

			memset (NumAmountC,0,sizeof(NumAmountC));

			if (TransactionType == CUSERV_CASHBACK_TRANSACTION)
			{
				ulTransactionType = CUSERV_CASHBACK_TRANSACTION;

				// Read the amount, using the good currency code

				result = CUMESS_GetAmount (currency_code, &entered_amount);
				LongToChar (0, TabAmountC);
				if ( (result) && (entered_amount!=0) )
				{
					CUMESS_ConvertBinNUM (entered_amount, NumAmountC);

					LongToChar (entered_amount, TabAmountC);
				}
			}
			else
			{
				memclr  ( TabAmountC,sizeof(TabAmountC));
			}
//			TlvTree_AddChild(hInputTree, TAG_AMOUNT_OTHER_BIN, TabAmountC, 4);
//			TlvTree_AddChild(hInputTree, TAG_AMOUNT_OTHER_NUM, NumAmountC, 6);
			TlvTree_AddChild(hInputTree, TAG_AMOUNT_AUTH_BIN, TabAmount, 4);

			TlvTree_AddChild(hInputTree, TAG_ENG_WAIT_CHIP_READERS, &ulChipReaders, sizeof(ulChipReaders));
			TlvTree_AddChild(hInputTree, TAG_ENG_REMOVE_CHIP_READERS, &ulChipReaders, sizeof(ulChipReaders));

			if (TransactionLoopMode == TRUE)
			{
				Telium_File_t* hCam0;
				//Telium_File_t* keyb;
				int merchLang;
				unsigned char	*pc_line1;
				//int uiEvent;
				unsigned char ucStatus;

	        	memclr( ucTimeout,sizeof(ucTimeout));
			    TlvTree_AddChild(hInputTree, TAG_ENG_REMOVE_CARD_TIMEOUT, (unsigned char *)&ucTimeout[0], 4);

                while (1)
				{
					// Setup the user interface environment
            		previousUiState = APEMV_UI_TransactionBegin();

					// Perform the transaction
					hOutputTree = NULL;

					//Engine_DoTransaction(hInputTree,&hOutputTree);
					switch(Engine_DoTransaction(hInputTree,&hOutputTree))//EngineK
					{
					case ERR_ENG_OK:
						break;
					default:
						EchoMessage_ppad(7);// XEROX  : For display on DDT screen , i send you :	011	006	TM,105	Probl?me lecture carte, Veuillez retirer et ins?rer ? nouveau votre carte bancaire
						Telium_Ttestall(0, 100);
						goto EndXerox;
						break;
					}

					//keyb = NULL;
					hCam0 = NULL;

					ucStatus =0;

					// keyb = Telium_Fopen("KEYBOARD","r");
					hCam0 = Telium_Fopen("CAM0", "rw");
					

					// Display remove
					merchLang = My_PSQ_Give_Language();
					pc_line1 = (unsigned char *)CUMESS_Read_Message(STD_MESS_REMOVE_CARD, merchLang); //EngineK Trame
//					APEMV_UI_InfoDisplay ((char *)pc_line1);

					MessageWithoutConfirmation(pc_line1, "file://flash/HOST/retirer_la_carte_svp.png");

				/*uiEvent =*/ Telium_Ttestall(CAM0,500);

				/*	if (keyb != NULL)
					{
						// Close the chip reader driver
						Telium_Fclose(keyb);
						keyb = NULL;
					}*/
					//if ((uiEvent & KEYBOARD)!=0)
					//
					// Restore the user interface environment
					//APEMV_UI_TransactionEnd(previousUiState);

						//	break;

					Telium_Status(hCam0, &ucStatus);

					if (hCam0 != NULL)
					{
						Telium_Fclose(hCam0);
						hCam0 = NULL;
					}

					if ((ucStatus & CAM_PRESENT) == 0)
					{
						/*if (hCam0 != NULL)
						{
							Telium_Fclose(hCam0);
							hCam0 = NULL;
						}*/

						EchoMessage_ppad(5);// XEROX  : For display on DDT screen , i send you :	011	005	TM,105	Veuillez Retirer Votre Carte
						Telium_Ttestall(0, 100);

						// Restore the user interface environment
						APEMV_UI_TransactionEnd(previousUiState);

						// A card has been removed
						break;
					}

					EchoMessage_ppad(5);// XEROX  : For display on DDT screen , i send you :	011	005	TM,105	Veuillez Retirer Votre Carte
					Telium_Ttestall(0, 100);

					// Restore the user interface environment
					APEMV_UI_TransactionEnd(previousUiState);

				}
			}
			else
			{
				//Telium_Fclose(hKbd);

MagXerox :
				WaitBothMagneticAndCam0(&FicPara.vCam0);
				//test_rf3(&FicPara.vCam0);/*1F1*/
				//Os__xprintaa("0006");




				if(FicPara.vCam0 == 1)//CAM0
				{
					// Setup the user interface environment
					previousUiState = APEMV_UI_TransactionBegin();
					//Os__xprintaa("0002");

					// Perform the transaction
					hOutputTree = NULL;

					//Engine_DoTransaction(hInputTree,&hOutputTree);
					switch(Engine_DoTransaction(hInputTree,&hOutputTree))//EngineK
					{
					case ERR_ENG_OK:
						break;
					default:
						EchoMessage_ppad(7);// XEROX  : For display on DDT screen , i send you :	011	006	TM,105	Probl?me lecture carte, Veuillez retirer et ins?rer ? nouveau votre carte bancaire
						Telium_Ttestall(0, 100);


						if (isFallbackAllowed)
						{
							////APEMV_UI_MessageDisplayDelay ("\nFallback\nIntroduire la carte\nMagn?tique", 2);

							StructInfoPayment.Fallback = 1;
							goto MagXerox;
							//transa_client();
						}
						else
						{
							goto EndXerox;
						}
						break;
					}

					isFallbackAllowed = FALSE;


					EchoMessage_ppad(5);// XEROX  : For display on DDT screen , i send you :	011	005	TM,105	Veuillez Retirer Votre Carte
					Telium_Ttestall(0, 100);

					// Restore the user interface environment
					APEMV_UI_TransactionEnd(previousUiState);
				}
				else if(FicPara.vCam0 == 2)//CLESS
				{
					memset(&SaveLastInfoCaisse,0,sizeof(MSG_CAISSE));
					memcpy(&SaveLastInfoCaisse,&StructInfoCaisse[0],sizeof(MSG_CAISSE));
					PutAllParam();
					Telium_Kill(alternateNbr, "");
					//Telium_Ttestall(00, 1000);
					AlternanceIsOn = 0;
//					Os__xprinta("TEST 2");
					SendToApplication(0x0122,1,6,"ABCDEF");
					AlternanceIsOn = 0;
//					Os__xprintd(AlternanceIsOn);
					goto EndXerox;
				}
				else if(FicPara.vCam0 == 4)//TIME OUT
				{
					strncpy(StructInfoCaisse[1].CRPIN,"118",3);
					PutAllParam();
					goto EndXerox;
				}
				else						//MAG
				{
					if(FicPara.AutoReversalOK == 2){

						if(!strlen((char *)FicPara.CodeReponnseAvantAutoReversal))
							memcpy(FicPara.CodeReponnseAvantAutoReversal,"880",3);

						strncpy(StructInfoCaisse[1].CRPIN,FicPara.CodeReponnseAvantAutoReversal,3);

						FicPara.AutoReversalOK = 4;
						PutAllParam();
					}

					// Perform the transaction
					hOutputTree = NULL;

					EchoMessage_ppad(5);// XEROX  : For display on DDT screen , i send you :	011	006	TM,105	Probl?me lecture carte, Veuillez retirer et ins?rer ? nouveau votre carte bancaire
					Telium_Ttestall(0, 100);

					// Restore the user interface environment
					APEMV_UI_TransactionEnd(previousUiState);

					strncpy(StructInfoCaisse[1].MDLC,"MG",2);		//Cless_MDLC

					goto EndXerox;
				}
			}


			//Simuler Transaction accepter avec XEROX
			//Cless_ Livraison memcpy(R_Transac8583.champs39,"000",3); //Test
			if(FicPara.AutoReversalOK == 2){

				if(!strlen((char *)FicPara.CodeReponnseAvantAutoReversal))
					memcpy(FicPara.CodeReponnseAvantAutoReversal,"880",3);

				memcpy(StructInfoCaisse[1].STAN,StructInfoPayment.STAN,6);
				memcpy(StructInfoCaisse[1].NPRT,StructInfoPayment.Nom_Prenom,strlen(StructInfoPayment.Nom_Prenom));
				memset(StructInfoCaisse[1].NCAR,'0',StructInfoPayment.Holder_Card[0]);
				memcpy(StructInfoCaisse[1].NCAR,&StructInfoPayment.Holder_Card[1],StructInfoPayment.Holder_Card[0]);
				memset(StructInfoCaisse[1].DAEX,'0',4);
				memcpy(StructInfoCaisse[1].DAEX,StructInfoPayment.Expired_Date,4);
				strncpy(&StructInfoCaisse[1].TM[0],"1",1);
				strncpy(&StructInfoCaisse[1].TM[1],&StructInfoCaisse[0].TM[1],2);
				strncpy(StructInfoCaisse[1].CRPIN,FicPara.CodeReponnseAvantAutoReversal,3);

				memset(&E_Transac8583,0x00,sizeof(E_Transac8583));
				memset(&R_Transac8583,0x00,sizeof(R_Transac8583));
				memset(&StructInfoPayment ,0x00,sizeof(StructInfoPayment));
				Sequence(OK,KEY_CLEAR);

				FicPara.AutoReversalOK = 4;
				PutAllParam();
			}
			else if((!memcmp(R_Transac8583.champs39,"000",3)) && (*p_status_tr == 0))
			{
				if(SaveTra() != OK)
				{
					memcpy(StructInfoCaisse[1].STAN,StructInfoPayment.STAN,6);
					memcpy(StructInfoCaisse[1].NPRT,StructInfoPayment.Nom_Prenom,strlen(StructInfoPayment.Nom_Prenom));
					memset(StructInfoCaisse[1].NCAR,'0',StructInfoPayment.Holder_Card[0]);
					memcpy(StructInfoCaisse[1].NCAR,&StructInfoPayment.Holder_Card[1],StructInfoPayment.Holder_Card[0]);
					memset(StructInfoCaisse[1].DAEX,'0',4);
					memcpy(StructInfoCaisse[1].DAEX,StructInfoPayment.Expired_Date,4);
					strncpy(&StructInfoCaisse[1].TM[0],"1",1);
					strncpy(&StructInfoCaisse[1].TM[1],&StructInfoCaisse[0].TM[1],2);
					strncpy(StructInfoCaisse[1].CRPIN,"306",3);

					memset(&E_Transac8583,0x00,sizeof(E_Transac8583));
					memset(&R_Transac8583,0x00,sizeof(R_Transac8583));
					memset(&StructInfoPayment ,0x00,sizeof(StructInfoPayment));
					Sequence(OK,KEY_CLEAR);
				}
				else
				{

					strncpy(StructInfoCaisse[1].CRPIN,"000",3);
					memcpy(StructInfoCaisse[1].STAN,StructInfoPayment.STAN,6);
					memcpy(StructInfoCaisse[1].NPRT,StructInfoPayment.Nom_Prenom,strlen(StructInfoPayment.Nom_Prenom));
					memset(StructInfoCaisse[1].NCAR,'0',StructInfoPayment.Holder_Card[0]);
					memcpy(StructInfoCaisse[1].NCAR,&StructInfoPayment.Holder_Card[1],StructInfoPayment.Holder_Card[0]);
					memset(StructInfoCaisse[1].DAEX,'0',4);
					memcpy(StructInfoCaisse[1].DAEX,StructInfoPayment.Expired_Date,4);

					ImprimeTra_ppad(StructInfoPayment.Langue,COMMERCANT,0);

					format_msg_caise2(buffer13,0);

					memset(&SaveLastInfoCaisse,0,sizeof(MSG_CAISSE));
					memcpy(&SaveLastInfoCaisse,&StructInfoCaisse[1],sizeof(MSG_CAISSE));
					strcat(buffer13,"!");

					//APEMV_UI_MessageDisplayDelay ("ATTENTE CONFIRMATION\nCAISSE", 1);

					if(send_receive_caisse(buffer13, strlen(buffer13),1) != OK)
					{
					    AnnulationAutomatique();

						//Supprimer la transaction du fichier FicTra
						FicTra.nombre_tra--;
						memset((uchar*)&FicTra.enr_tra0[FicTra.nombre_tra],0x00,sizeof(lignetra));
						PutAllTra();

						FicPara.AutoReversalOK = 3;
						PutAllParam();

						goto EndXerox;
					}

		            memset(&StructInfoCaisse[0],0,sizeof(MSG_CAISSE));
		            memset(&StructInfoCaisse[1],0,sizeof(MSG_CAISSE));

		 		   if(strlen(buffer13)==0){
		 				strncpy(StructInfoCaisse[1].TM,"102",3);
		 				strncpy(StructInfoCaisse[1].CRPIN,"306",3);
		 				//write_at("Probleme de la caisse",21,30,150);
		 				//SVC_WAIT(4000);
		 				goto EndXerox;
		 			}
		 			deco_msg_caise2(buffer13,0);

					//msg_display("  ____________ 002");
					//devoir controler les donn?es de TM002 avec TM101
					//Si KO Remplir CRPIN par code # de 000 et sortir par break et en g?n?re une annulation
		 			if(controle_paiement("002")==0)
					{
						strncpy(&StructInfoCaisse[1].TM[0],"1",1);
						strncpy(&StructInfoCaisse[1].TM[1],&StructInfoCaisse[0].TM[1],2);
						strncpy(StructInfoCaisse[1].CRPIN,"115",3);

						memcpy(StructInfoCaisse[1].STAN,StructInfoPayment.STAN,6);
						memcpy(StructInfoCaisse[1].NPRT,StructInfoPayment.Nom_Prenom,strlen(StructInfoPayment.Nom_Prenom));
						memset(StructInfoCaisse[1].NCAR,'0',StructInfoPayment.Holder_Card[0]);
						memcpy(StructInfoCaisse[1].NCAR,&StructInfoPayment.Holder_Card[1],StructInfoPayment.Holder_Card[0]);
						memset(StructInfoCaisse[1].DAEX,'0',4);
						memcpy(StructInfoCaisse[1].DAEX,StructInfoPayment.Expired_Date,4);

						//write_at("Probleme de la caisse",21,30,150);
						//SVC_WAIT(4000);
						goto EndXerox;
					}

		 			//devoir controler les donn?es de TM002 avec TM101
		 			//Si KO Remplir CRPIN par code # de 000 et sortir par break et en g?n?re une annulation

		 			memcpy(StructInfoCaisse[1].STAN,StructInfoPayment.STAN,6);
		 			memcpy(StructInfoCaisse[1].NPRT,StructInfoPayment.Nom_Prenom,strlen(StructInfoPayment.Nom_Prenom));
		 			memset(StructInfoCaisse[1].NCAR,'0',StructInfoPayment.Holder_Card[0]);
		 			memcpy(StructInfoCaisse[1].NCAR,&StructInfoPayment.Holder_Card[1],StructInfoPayment.Holder_Card[0]);
		 			memset(StructInfoCaisse[1].DAEX,'0',4);
		 			strncpy(StructInfoCaisse[1].TM,"102",3);
		 			strncpy(StructInfoCaisse[1].MDLC,"CO",2);		//Cless_MDLC
		 			memcpy(StructInfoCaisse[1].DAEX,StructInfoPayment.Expired_Date,4);

		 			if(StructInfoPayment.CodeFunction != PREAUTHCODE)
		 			{

						ImprimeTra_ppad(StructInfoPayment.Langue,CLIENT,0);
						strncpy(&StructInfoCaisse[1].TM[0],"1",1);
						strncpy(&StructInfoCaisse[1].TM[1],&StructInfoCaisse[0].TM[1],2);
						strncpy(StructInfoCaisse[1].CRPIN,"000",3);
		 				Sequence(OK,KEY_ENTER);
		 			}
				}
			}
			else
			{
				if(!strlen((char *)R_Transac8583.champs39))
					memcpy(R_Transac8583.champs39,"880",3);

				memcpy(StructInfoCaisse[1].STAN,StructInfoPayment.STAN,6);
				memcpy(StructInfoCaisse[1].NPRT,StructInfoPayment.Nom_Prenom,strlen(StructInfoPayment.Nom_Prenom));
				memset(StructInfoCaisse[1].NCAR,'0',StructInfoPayment.Holder_Card[0]);
				memcpy(StructInfoCaisse[1].NCAR,&StructInfoPayment.Holder_Card[1],StructInfoPayment.Holder_Card[0]);
				memset(StructInfoCaisse[1].DAEX,'0',4);
				memcpy(StructInfoCaisse[1].DAEX,StructInfoPayment.Expired_Date,4);
				strncpy(&StructInfoCaisse[1].TM[0],"1",1);
				strncpy(&StructInfoCaisse[1].TM[1],&StructInfoCaisse[0].TM[1],2);
				strncpy(StructInfoCaisse[1].CRPIN,R_Transac8583.champs39,3);

				memset(&E_Transac8583,0x00,sizeof(E_Transac8583));
				memset(&R_Transac8583,0x00,sizeof(R_Transac8583));
				memset(&StructInfoPayment ,0x00,sizeof(StructInfoPayment));
				Sequence(OK,KEY_CLEAR);
			}
EndXerox :
			TlvTree_ReleaseEx(&hInputTree);
			TlvTree_ReleaseEx(&hOutputTree);
		}
	}
}




//! @}

void CUMORE_PaiementFctExt(unsigned long TransactionType, unsigned char ucParameters, unsigned long ulAmount, int nCurrencyCode)
{
	//Os__xprintaa("000031");
	control_amount=	TRUE;
	CUMORE_PaiementFct(TransactionType,ucParameters,ulAmount,nCurrencyCode);
	control_amount=	FALSE;

	//Os__xprintaa("000032");

}
