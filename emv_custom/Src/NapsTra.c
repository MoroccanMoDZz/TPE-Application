/* ======================================================================== */
/* Include Files                                                            */
/* ======================================================================== */

#include "SDK_Naps.h"
#include "_emvdctag_.h"
#include "TlvTree.h"
#include "del_lib.h"
#include "convert.h"
#include "cu_base.h"
#include "cu_serv.h"
#include "def_tag.h"
#include "cu_mess.h"
#include "cu_entry.h"
#include "gestion_param.h"
#include "cu_term.h"
#include "cu_param.h"
#include "cu_disk.h"

#define LABEL_FILE_CMI_TRA	 "CMI_TRA"
#define LABEL_FILE_CMI_PRE	 "CMI_PRE"

uchar			g_Type_Annul;
uchar			g_numero_carte[20];
Ushort   		gindice;
uchar	GLB_STAN_ANN[7]={'\0'};

int				GlLine;
int				DebutLine;


extern unsigned long ulTransactionType;
extern unsigned long ulTransactionTypeidlemessage;
extern unsigned short CUSERV_IsInBlackList;
uchar	ImprimeTraComp(uchar langue,uchar source,uchar dup,lignetra		*transac);
int		receive_caisseUSB_SansKeyBoard(unsigned char *buffer, int len, char fermport);
extern void CUTERMclearDisplayT1(void); //KT1

extern char buffer13[4000];

/* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
/*-----------------------------------------------------------------
 * Fonction   : TraFile
 * objet      : Ecris tous tous les transactions
 * Parametre  : Aucun
 * Retour     : Aucun
 *-----------------------------------------------------------------
*/
L	TraFile()
{
	T_Bool  ret;
	char FilePath_L[33];
	int iret;

	ret = B_TRUE;

	iret = CUDISK_CreateDisk();
	if (iret == FS_OK)
	{
		memclr(FilePath_L, sizeof(FilePath_L));
		sprintf(FilePath_L, "/%s%02x/%s",LABEL_DISK,CustApplicationNumber,LABEL_FILE_CMI_TRA);
		iret = FS_exist (FilePath_L);
		if (iret == FS_OK)
		{
			memclr(FilePath_L,sizeof(FilePath_L));
			sprintf(FilePath_L, "/%s%02x",LABEL_DISK,CustApplicationNumber);
			FS_unmount(FilePath_L);

			GetAllTra();
		}
		else
		{
			memclr(FilePath_L,sizeof(FilePath_L));
			sprintf(FilePath_L, "/%s%02x",LABEL_DISK,CustApplicationNumber);
			FS_unmount(FilePath_L);

			Init_Tra();
		}
	}
    return (ret);
}

/*-----------------------------------------------------------------
 * Fonction   : GetAllTra
 * objet      : Remplir tous les transactions
 * Parametre  : Aucun
 * Retour     : Aucun
 *-----------------------------------------------------------------
*/
L	GetAllTra()
{
	T_Bool  ret;
	S_FS_FILE *file1;
	char FilePath_L[33];
	int iret;

	ret = B_TRUE;

	iret = CUDISK_CreateDisk();
	if (iret == FS_OK)
	{
		memclr(FilePath_L, sizeof(FilePath_L));
		sprintf(FilePath_L, "/%s%02x/%s",LABEL_DISK,CustApplicationNumber,LABEL_FILE_CMI_TRA);
		file1 = FS_open (FilePath_L, "r");
		if (file1 != NULL)
		{
			FS_read((PUC)&FicTra, sizeof (FicTra), 1, file1);
			FS_close (file1);

			memclr(FilePath_L,sizeof(FilePath_L));
			sprintf(FilePath_L, "/%s%02x",LABEL_DISK,CustApplicationNumber);
			FS_unmount(FilePath_L);

			return OK;
		}
	}
    return KO;
}

/*-----------------------------------------------------------------
 * Fonction   : PutAllTra
 * objet      : Ecris tous les transactions
 * Parametre  : Aucun
 * Retour     : Aucun
 *-----------------------------------------------------------------
*/
L	PutAllTra()
{
	T_Bool  ret;
	S_FS_FILE *file1;
	char FilePath_L[33];
	int iret;

	ret = B_TRUE;

	iret = CUDISK_CreateDisk();
	if (iret == FS_OK)
	{
		memclr(FilePath_L, sizeof(FilePath_L));
		sprintf(FilePath_L, "/%s%02x/%s",LABEL_DISK,CustApplicationNumber,LABEL_FILE_CMI_TRA);
		iret = FS_exist (FilePath_L);
		if (iret == FS_OK)
			iret = FS_unlink (FilePath_L);

		file1 = FS_open (FilePath_L, "a");
		if (file1 != NULL)
		{
			FS_write((PUC)&FicTra, sizeof (FicTra), 1, file1);
			FS_close (file1);

			memclr(FilePath_L,sizeof(FilePath_L));
			sprintf(FilePath_L, "/%s%02x",LABEL_DISK,CustApplicationNumber);
			FS_unmount(FilePath_L);
			return OK;
		}
	}
    return KO;
}
/*-----------------------------------------------------------------
 * Fonction   : Init_Tra
 * objet      : Init du fichier Liste Noire
 * Parametre  : Aucun
 * Retour     : Aucun
 *-----------------------------------------------------------------
*/
void	Init_Tra(void)
{
	// Initialisation du tableau des Transactions
	Efface_Trs();

	PutAllTra();
}

/* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
/*-----------------------------------------------------------------
 * Fonction   : preFile
 * objet      : Ecris tous tous les pre autorisation
 * Parametre  : Aucun
 * Retour     : Aucun
 *-----------------------------------------------------------------
*/
L	PreFile()
{
	T_Bool  ret;
	char FilePath_L[33];
	int iret;

	ret = B_TRUE;

	iret = CUDISK_CreateDisk();
	if (iret == FS_OK)
	{
		memclr(FilePath_L, sizeof(FilePath_L));
		sprintf(FilePath_L, "/%s%02x/%s",LABEL_DISK,CustApplicationNumber,LABEL_FILE_CMI_PRE);
		iret = FS_exist (FilePath_L);
		if (iret == FS_OK)
		{
			memclr(FilePath_L,sizeof(FilePath_L));
			sprintf(FilePath_L, "/%s%02x",LABEL_DISK,CustApplicationNumber);
			FS_unmount(FilePath_L);

			GetAllPre();
		}
		else
		{
			memclr(FilePath_L,sizeof(FilePath_L));
			sprintf(FilePath_L, "/%s%02x",LABEL_DISK,CustApplicationNumber);
			FS_unmount(FilePath_L);

			Init_Pre();
		}
	}
    return (ret);
}

/*-----------------------------------------------------------------
 * Fonction   : GetAllPre
 * objet      : Remplir tous les pre autorisation
 * Parametre  : Aucun
 * Retour     : Aucun
 *-----------------------------------------------------------------
*/
L	GetAllPre()
{
	T_Bool  ret;
	S_FS_FILE *file1;
	char FilePath_L[33];
	int iret;

	ret = B_TRUE;

	iret = CUDISK_CreateDisk();
	if (iret == FS_OK)
	{
		memclr(FilePath_L, sizeof(FilePath_L));
		sprintf(FilePath_L, "/%s%02x/%s",LABEL_DISK,CustApplicationNumber,LABEL_FILE_CMI_PRE);
		file1 = FS_open (FilePath_L, "r");
		if (file1 != NULL)
		{
			FS_read((PUC)&FicPreauth, sizeof (FicPreauth), 1, file1);
			FS_close (file1);

			memclr(FilePath_L,sizeof(FilePath_L));
			sprintf(FilePath_L, "/%s%02x",LABEL_DISK,CustApplicationNumber);
			FS_unmount(FilePath_L);

			return OK;
		}
	}
    return KO;
}


/*-----------------------------------------------------------------
 * Fonction   : PutAllPre
 * objet      : Ecris tous les pre autorisation
 * Parametre  : Aucun
 * Retour     : Aucun
 *-----------------------------------------------------------------
*/
L	PutAllPre()
{
	T_Bool  ret;
	S_FS_FILE *file1;
	char FilePath_L[33];
	int iret;

	ret = B_TRUE;

	iret = CUDISK_CreateDisk();
	if (iret == FS_OK)
	{
		memclr(FilePath_L, sizeof(FilePath_L));
		sprintf(FilePath_L, "/%s%02x/%s",LABEL_DISK,CustApplicationNumber,LABEL_FILE_CMI_PRE);
		iret = FS_exist (FilePath_L);
		if (iret == FS_OK)
			iret = FS_unlink (FilePath_L);

		file1 = FS_open (FilePath_L, "a");
		if (file1 != NULL)
		{

			FS_write((PUC)&FicPreauth, sizeof (FicPreauth), 1, file1);
			FS_close (file1);

			memclr(FilePath_L,sizeof(FilePath_L));
			sprintf(FilePath_L, "/%s%02x",LABEL_DISK,CustApplicationNumber);
			FS_unmount(FilePath_L);
			return OK;
		}
	}
    return KO;
}

L	PutMemoryPreauth(char * EMETTEUR) //NAPS_CMI_INTEGREE
{
	if(!memcmp(EMETTEUR,"CMI",3))
	{
		memcpy((PC)&FicPreauthCMI,(PC)&FicPreauth, sizeof (FicPreauth));
	}
	else if(!memcmp(EMETTEUR,"NAPS",4))
	{
		memcpy((PC)&FicPreauthNAPS,(PC)&FicPreauth, sizeof (FicPreauth));
	}
  return OK;
}



L	PutAllPreEmetteur()
{
	PutMemoryPreauth(FicPreauth.EMETTEUR);
	PutAllPre();
}
/*-----------------------------------------------------------------
 * Fonction   : Init_Pre
 * objet      : Init du fichier Liste Noire
 * Parametre  : Aucun
 * Retour     : Aucun
 *-----------------------------------------------------------------
*/
void	Init_Pre(void)
{
	// Initialisation du tableau des pre autorisation
	Efface_File_Preauto();
	PutAllPre();
}





/****************************************************************************
*                                                                           *
*      SOUS-PROGRAMME :   Prepare_Transaction                               *
*                                                                           *
*****************************************************************************
*                                                                           *
*      FONCTION :     Preparation de la transaction      					*
*                                                                           *
*      ENTREE   :  RIEN                                          			*
*                                                                           *
*      SORTIE   :  status                                                   *
*                                                                           *
****************************************************************************/
uchar	Prepare_Transaction()
{
	uchar		bitmap[8];
	uchar		bitmapPI[16] = {'\0'};
	ulong		Num_Seq;

	memset(bitmap,0,8);

	if(
		(StructInfoPayment.CodeFunction!=PREAUTHCODE) &&
		(StructInfoPayment.CodeFunction!=VOID_PREAUTO_PARTIEL_CODE) &&
		(StructInfoPayment.CodeFunction!=VOID_PREAUTO_TOTAL_CODE)
	)
	{
		Num_Seq = FicTra.nombre_tra+1 ;
		long_asc(StructInfoPayment.Num_sequence,6,(Ulong *)&Num_Seq);
	}
	else
	{
		Num_Seq = FicPreauth.Indice_fin+1 ;
		long_asc(StructInfoPayment.Num_sequence,6,(Ulong *)&Num_Seq);
	}

	switch (StructInfoPayment.CodeFunction)
	{
		case    CASHADVANCESCODE  :
			memcpy(StructInfoPayment.Proc_Code,"170000",6);
			memcpy(bitmap,bmp1200,8) ;
			memcpy(StructInfoPayment.Message_TypeisoEmi,"1200",4);
			memcpy(StructInfoPayment.Message_TypeisoRec,"1210",4);
			break;

		case	SALECODE	:
		case	PREAUTHCOMPCODE :
		case    REMISE  :
		case    PAIEMENT_NFOIS  :
		case    PAIEMENT_DIFFERE  :
		case    PAIEMENT_POURBOIRE  :
		case    PAIEMENT_INTERCHANGE  :
//			ShowTrace("PT0");
			memcpy(StructInfoPayment.Proc_Code,"000000",6);
			memcpy(bitmap,bmp1200,8) ;
			memcpy(StructInfoPayment.Message_TypeisoEmi,"1200",4);
			memcpy(StructInfoPayment.Message_TypeisoRec,"1210",4);
			break;

		case	PINCHANGE	:
			if (StructInfoPayment.Etat_PIN == 0){
				memcpy(StructInfoPayment.Proc_Code,"PI0000",6); /* OURBIAA POUR AMSSF 121115 */
			}else if (StructInfoPayment.Etat_PIN == 1){
				memcpy(StructInfoPayment.Proc_Code,"PI0000",6);
			}

			memcpy(bitmapPI,bitmapPINChange,16) ;
			memcpy(StructInfoPayment.Message_TypeisoEmi,"0200",4);
			memcpy(StructInfoPayment.Message_TypeisoRec,"0210",4);
		   if (StructInfoPayment.Etat_PIN == 1) // Version PREPAYE 1.2
		   {
				//eft30printhexKamal(bitmapPI,8);
				bitdel(bitmapPI,5);
				//eft30printhexKamal(bitmapPI,8);
		   }
			break;
		case 	MICRO_CREDIT	:
			memcpy(StructInfoPayment.Proc_Code,"CN0000",6);
			memcpy(bitmapPI,bitmapMicroCredit,16) ;
			memcpy(StructInfoPayment.Message_TypeisoEmi,"0300",4);
			memcpy(StructInfoPayment.Message_TypeisoRec,"0310",4);
			break;
		case 	CREDIT_PARTIEL	:
			memcpy(StructInfoPayment.Proc_Code,"RM0000",6);
			memcpy(bitmapPI,bitmapCredit,16) ;
			memcpy(StructInfoPayment.Message_TypeisoEmi,"0300",4);
			memcpy(StructInfoPayment.Message_TypeisoRec,"0310",4);
			break;
		case	CREDIT_TOTAL	:
			memcpy(StructInfoPayment.Proc_Code,"RM0000",6);
			memcpy(bitmapPI,bitmapCredit,16) ;
			memcpy(StructInfoPayment.Message_TypeisoEmi,"0300",4);
			memcpy(StructInfoPayment.Message_TypeisoRec,"0310",4);
			break;
		case	RECH_PREPAYE	:
			memcpy(StructInfoPayment.Proc_Code,"900000",6);
			memcpy(bitmap,bmp1200,8) ;
			memcpy(StructInfoPayment.Message_TypeisoEmi,"0200",4);
			memcpy(StructInfoPayment.Message_TypeisoRec,"0210",4);
			break;
		case	DECH_PREPAYE	:
			memcpy(StructInfoPayment.Proc_Code,"910000",6);
			memcpy(bitmap,bmp1200,8) ;
			memcpy(StructInfoPayment.Message_TypeisoEmi,"0200",4);
			memcpy(StructInfoPayment.Message_TypeisoRec,"0210",4);
			break;
		case	CAC_PREPAYE	:
			memcpy(StructInfoPayment.Proc_Code,"920000",6);
			memcpy(bitmap,bmp1200,8) ;
			memcpy(StructInfoPayment.Message_TypeisoEmi,"0200",4);
			memcpy(StructInfoPayment.Message_TypeisoRec,"0210",4);
			break;
		case	CASHIN_PREPAYE	:
			memcpy(StructInfoPayment.Proc_Code,"930000",6);
			memcpy(bitmap,bmp1200,8) ;
			memcpy(StructInfoPayment.Message_TypeisoEmi,"0200",4);
			memcpy(StructInfoPayment.Message_TypeisoRec,"0210",4);
			break;
		case	CASHOUT_PREPAYE	:
			memcpy(StructInfoPayment.Proc_Code,"940000",6);
			memcpy(bitmap,bmp1200,8) ;
			memcpy(StructInfoPayment.Message_TypeisoEmi,"0200",4);
			memcpy(StructInfoPayment.Message_TypeisoRec,"0210",4);
			break;
		case	DEM_SOLDE	:
			memcpy(StructInfoPayment.Proc_Code,"960000",6);
			memcpy(bitmap,bmp1200,8) ;
			memcpy(StructInfoPayment.Message_TypeisoEmi,"0200",4);
			memcpy(StructInfoPayment.Message_TypeisoRec,"0210",4);
			break;
		case	MINI_RELEVE	:
			memcpy(StructInfoPayment.Proc_Code,"970000",6);
			memcpy(bitmapPI,bitmapMiniReleve,16) ;
			memcpy(StructInfoPayment.Message_TypeisoEmi,"0200",4);
			memcpy(StructInfoPayment.Message_TypeisoRec,"0210",4);
			break;

		case    PREAUTHCODE  :
			memcpy(StructInfoPayment.Proc_Code,"000000",6);
			memcpy(bitmap,bmp1200,8) ;
			memcpy(StructInfoPayment.Message_TypeisoEmi,"1100",4);
			memcpy(StructInfoPayment.Message_TypeisoRec,"1110",4);
			break;

		case	DE_RECHARGE	: // Demande recharge GSM
			memcpy(StructInfoPayment.Proc_Code,"980000",6);
			memcpy(bitmap,bmp1200,8) ;
			memcpy(StructInfoPayment.Message_TypeisoEmi,"0200",4);
			memcpy(StructInfoPayment.Message_TypeisoRec,"0210",4);
			break;
		case	E_RECHARGE	: // Recharge GSM
			memcpy(StructInfoPayment.Proc_Code,"990000",6);
			memcpy(bitmap,bmp1200,8) ;
			memcpy(StructInfoPayment.Message_TypeisoEmi,"0200",4);
			memcpy(StructInfoPayment.Message_TypeisoRec,"0210",4);
			break;
		case	OR_FACTURE50	: // Oriflame factures
			//memcpy(StructInfoPayment.Proc_Code,"OR5000",6);
			memcpy(StructInfoPayment.Proc_Code,"OR5000",6);
			memcpy(bitmap,bmp1200,8) ;
			memcpy(StructInfoPayment.Message_TypeisoEmi,"1200",4);
			memcpy(StructInfoPayment.Message_TypeisoRec,"1210",4);
			break;
		case	OR_FACTURE51	: // Oriflame factures
			memcpy(StructInfoPayment.Proc_Code,"OR5100",6);
			memcpy(bitmap,bmp1200,8) ;
			memcpy(StructInfoPayment.Message_TypeisoEmi,"1200",4);
			memcpy(StructInfoPayment.Message_TypeisoRec,"1210",4);
			break;
		case	DE_REMBOURSSEMENT	: // AMANASUR
			memcpy(StructInfoPayment.Proc_Code,"330000",6);
			memcpy(bitmap,bmp1200,8) ;
			memcpy(StructInfoPayment.Message_TypeisoEmi,"0200",4);
			memcpy(StructInfoPayment.Message_TypeisoRec,"0210",4);
			break;
		case 	NOTATION			: // notation
			memcpy(StructInfoPayment.Proc_Code,"340000",6);
			memcpy(bitmap,bmp1200,8) ;
			memcpy(StructInfoPayment.Message_TypeisoEmi,"0200",4);
			memcpy(StructInfoPayment.Message_TypeisoRec,"0210",4);
			break;

		case VOIDCODE:
		case VOID_PREAUTO_TOTAL_CODE:
		case VOID_PREAUTO_PARTIEL_CODE:
			memcpy(StructInfoPayment.Bit_Map,bmp1420,8);
			if(StructInfoPayment.Entry_Mode==TRS_EMV)
				bitset(StructInfoPayment.Bit_Map, 23);
			if(StructInfoPayment.CodeFunction==VOID_PREAUTO_PARTIEL_CODE)
				bitset(StructInfoPayment.Bit_Map, 30);
			memcpy(StructInfoPayment.Message_TypeisoEmi,"1420",4);
			memcpy(StructInfoPayment.Message_TypeisoRec,"1430",4);
			break;

		case REFUNDCODE:
			memcpy(StructInfoPayment.Proc_Code,"200000",6);
			memcpy(StructInfoPayment.Bit_Map,bmp1200,8);
			memcpy(StructInfoPayment.Message_TypeisoEmi,"1200",4);
			memcpy(StructInfoPayment.Message_TypeisoRec,"1210",4);
			break;

		default	:
			return KO;
	}

	if(
		(StructInfoPayment.CodeFunction==CASHADVANCESCODE)||
		(StructInfoPayment.CodeFunction==SALECODE)||
		(StructInfoPayment.CodeFunction==PREAUTHCOMPCODE)||
		(StructInfoPayment.CodeFunction==PREAUTHCODE) ||
		(StructInfoPayment.CodeFunction == REFUNDCODE) ||
		(StructInfoPayment.CodeFunction == RECH_PREPAYE) ||
		(StructInfoPayment.CodeFunction == DECH_PREPAYE) ||
		(StructInfoPayment.CodeFunction == CASHIN_PREPAYE) ||
		(StructInfoPayment.CodeFunction == CASHOUT_PREPAYE) ||
		(StructInfoPayment.CodeFunction == CAC_PREPAYE) ||
		(StructInfoPayment.CodeFunction == DE_RECHARGE) ||
		(StructInfoPayment.CodeFunction == E_RECHARGE) ||
		(StructInfoPayment.CodeFunction == OR_FACTURE50) ||
		(StructInfoPayment.CodeFunction == OR_FACTURE51) ||
		(StructInfoPayment.CodeFunction == OR_FACTURE52) ||
		(StructInfoPayment.CodeFunction == OR_FACTURE53) ||
		(StructInfoPayment.CodeFunction == PINCHANGE) ||
		(StructInfoPayment.CodeFunction == PREAUTHCODE) ||
		(StructInfoPayment.CodeFunction == DEM_SOLDE) ||
		(StructInfoPayment.CodeFunction == MINI_RELEVE) ||
		(StructInfoPayment.CodeFunction == MICRO_CREDIT) ||
		(StructInfoPayment.CodeFunction == CREDIT_PARTIEL) ||
		(StructInfoPayment.CodeFunction == CREDIT_TOTAL)  ||
		(StructInfoPayment.CodeFunction == DE_REMBOURSSEMENT) ||
		(StructInfoPayment.CodeFunction == REMISE) ||
		(StructInfoPayment.CodeFunction == PAIEMENT_NFOIS) ||
		(StructInfoPayment.CodeFunction == PAIEMENT_DIFFERE) ||
		(StructInfoPayment.CodeFunction == PAIEMENT_POURBOIRE) ||
		(StructInfoPayment.CodeFunction == PAIEMENT_INTERCHANGE) ||
		(StructInfoPayment.CodeFunction == NOTATION)
	  )
	{

		if(StructInfoPayment.Entry_Pin==PIN_ONLINE)
		{
			if ((StructInfoPayment.CodeFunction == PINCHANGE) || (StructInfoPayment.CodeFunction == E_RECHARGE)
			||	(StructInfoPayment.CodeFunction == OR_FACTURE50) ||	(StructInfoPayment.CodeFunction == OR_FACTURE51)
			||	(StructInfoPayment.CodeFunction == OR_FACTURE52) ||	(StructInfoPayment.CodeFunction == OR_FACTURE53)
			|| (StructInfoPayment.CodeFunction == MINI_RELEVE) || (StructInfoPayment.CodeFunction == MICRO_CREDIT)
			|| (StructInfoPayment.CodeFunction == CREDIT_PARTIEL) || (StructInfoPayment.CodeFunction == CREDIT_TOTAL)){
				bitset(bitmapPI, 48);
				bitset(bitmapPI, 52);
			}else{
				bitset(bitmap, 48);
				bitset(bitmap, 52);
			}
		}else if((FicPara.Flag_service == 1)||(FicPara.Flag_service == 2))//services badr
			bitset(bitmap, 48);


		switch(StructInfoPayment.Entry_Mode)
		{
			case TRS_MAG:
				if ((StructInfoPayment.CodeFunction == PINCHANGE) || (StructInfoPayment.CodeFunction == E_RECHARGE)
				||	(StructInfoPayment.CodeFunction == OR_FACTURE50) ||	(StructInfoPayment.CodeFunction == OR_FACTURE51)
				||	(StructInfoPayment.CodeFunction == OR_FACTURE52) ||	(StructInfoPayment.CodeFunction == OR_FACTURE53)
				|| (StructInfoPayment.CodeFunction == MINI_RELEVE) || (StructInfoPayment.CodeFunction == MICRO_CREDIT))
					bitset(bitmapPI, 35);
				else
					bitset(bitmap, 35);
				break ;
			case TRS_EMV :
				if (StructInfoPayment.PanSeqNum5F34[0] != 0){//Ajout? par Hind SAHEL pour certif
					if ((StructInfoPayment.CodeFunction == PINCHANGE) || (StructInfoPayment.CodeFunction == E_RECHARGE)
					||	(StructInfoPayment.CodeFunction == OR_FACTURE50) ||	(StructInfoPayment.CodeFunction == OR_FACTURE51)
					||	(StructInfoPayment.CodeFunction == OR_FACTURE52) ||	(StructInfoPayment.CodeFunction == OR_FACTURE53)
					|| (StructInfoPayment.CodeFunction == MINI_RELEVE) || (StructInfoPayment.CodeFunction == MICRO_CREDIT)
					|| (StructInfoPayment.CodeFunction == CREDIT_PARTIEL) || (StructInfoPayment.CodeFunction == CREDIT_TOTAL))
						bitset(bitmapPI, 23);
					else
						bitset(bitmap, 23);
				}
				if ((StructInfoPayment.CodeFunction == PINCHANGE) || (StructInfoPayment.CodeFunction == E_RECHARGE)
				||	(StructInfoPayment.CodeFunction == OR_FACTURE50) ||	(StructInfoPayment.CodeFunction == OR_FACTURE51)
				||	(StructInfoPayment.CodeFunction == OR_FACTURE52) ||	(StructInfoPayment.CodeFunction == OR_FACTURE53)
				|| (StructInfoPayment.CodeFunction == MINI_RELEVE) || (StructInfoPayment.CodeFunction == MICRO_CREDIT)
				|| (StructInfoPayment.CodeFunction == CREDIT_PARTIEL) ||(StructInfoPayment.CodeFunction == CREDIT_TOTAL)){
					bitset(bitmapPI, 35);
					bitset(bitmapPI, 55);
				}
				if(StructInfoPayment.CodeFunction == DE_RECHARGE || (StructInfoPayment.CodeFunction == E_RECHARGE)
				|| (StructInfoPayment.CodeFunction ==RECH_PREPAYE) || (StructInfoPayment.CodeFunction == NOTATION)
				||	(StructInfoPayment.CodeFunction == OR_FACTURE50) ||	(StructInfoPayment.CodeFunction == OR_FACTURE51)
				||	(StructInfoPayment.CodeFunction == OR_FACTURE52) ||	(StructInfoPayment.CodeFunction == OR_FACTURE53)
				|| (StructInfoPayment.CodeFunction == DE_REMBOURSSEMENT)
				|| (StructInfoPayment.CodeFunction == NOTATION)
				){

				bitset(bitmap,62) ;// STOUARGUI 170616 gsm
				}
				else{
					if ((StructInfoPayment.CodeFunction == PINCHANGE) || (StructInfoPayment.CodeFunction == E_RECHARGE)
					||	(StructInfoPayment.CodeFunction == OR_FACTURE50) ||	(StructInfoPayment.CodeFunction == OR_FACTURE51)
					||	(StructInfoPayment.CodeFunction == OR_FACTURE52) ||	(StructInfoPayment.CodeFunction == OR_FACTURE53)
					|| (StructInfoPayment.CodeFunction == MINI_RELEVE)  || (StructInfoPayment.CodeFunction == MICRO_CREDIT)
					|| (StructInfoPayment.CodeFunction == CREDIT_PARTIEL)|| (StructInfoPayment.CodeFunction == CREDIT_TOTAL)){
						bitset(bitmapPI, 35);
						bitset(bitmapPI, 55);
					}else{
						bitset(bitmap, 35);
						bitset(bitmap, 55);
					}
				}
				break ;
			case TRS_MAN:
				bitset(bitmap, 48);
				break;
			default:
				break;
		}
		if ((StructInfoPayment.CodeFunction == PINCHANGE)
		|| (StructInfoPayment.CodeFunction == MINI_RELEVE)  || (StructInfoPayment.CodeFunction == MICRO_CREDIT) || (StructInfoPayment.CodeFunction == CREDIT_PARTIEL)
		|| (StructInfoPayment.CodeFunction == CREDIT_TOTAL))
			memcpy(StructInfoPayment.Bit_Map,bitmapPI,16);
		else
			memcpy(StructInfoPayment.Bit_Map,bitmap,8);
	}

//	if(StructInfoPayment.STAN[0]==0)
//	{
		memcpy(StructInfoPayment.STAN,FicPara.Message_Number,6);
		IncStan();
		//memcpy(StructInfoPayment.STAN,FicPara.Message_Number,6);
//	}

	return OK;
}


/****************************************************************************
*                                                                           *
*      SOUS-PROGRAMME :  transa_client                                      *
*                                                                           *
*****************************************************************************
*                                                                           *
*      FONCTION :    Traitement d'une transaction sale ou cash_advance      *
*                                                                           *
*      ENTREE   :  fonction                                      			*
*                                                                           *
*      SORTIE   :  status                                                   *
*                                                                           *
****************************************************************************/
uchar	transa_client()
{
	UL		Tmp_Amount;
	uchar	PtIso[MAXPTISO];
	uchar	update_param[6];
	uchar	update_date[11];
	uchar	Buffer[7];
	uchar	status;
	uchar	fixdevise;
	Ushort	devise_num;
	uchar	Date_sys[7];
	uchar	Time_sys[7];
	Ulong	l_ul_montant;
	Ushort 	Valide_Mont;
	uchar	l_retour,l_retour_seq;
	Ushort	indices_offline[MAX_AVIS+1];
	Ushort	nbr_tra_off_succ;
	uchar	Status_trs;
	Ushort	j;
	DATE date;
	uchar	Date_YYMMDDHHmmss[13];
//	uchar			size_carte[3];


	StructInfoPayment.Author_Status = 0xff;
	memset(indices_offline,0,sizeof(indices_offline));
	// R?cup?ration de la date/heure de la transaction
	memset(Date_sys,'\0',7);
	memset(Time_sys,'\0',5);

	Telium_Read_date(&date);
	sprintf ((char*)Date_YYMMDDHHmmss,
		"%.2s%.2s%.2s%.2s%.2s%.2s",
		date.year,
		date.month,
		date.day,
		date.hour,
		date.minute,
		date.second);
	memcpy(StructInfoPayment.Transaction_DateTime,Date_YYMMDDHHmmss,12);

	/*Os__read_date(Date_sys);
	memcpy(StructInfoPayment.Transaction_DateTime,&Date_sys[4],2);
	memcpy(StructInfoPayment.Transaction_DateTime+2,&Date_sys[2],2);
	memcpy(StructInfoPayment.Transaction_DateTime+4,&Date_sys[0],2);
	Os__read_time_sec (Time_sys) ;
	memcpy(StructInfoPayment.Transaction_DateTime+6,Time_sys,6);*/

//	ShowTrace("TC0");


 	status = valide_carte();
	//ShowTrace("valide_carte");
//	ShowTrace("TC1");

 	if(status != OK)
	{
 		//APEMV_UI_MessageDisplayDelay ("transa_client 1", 1);
		ShowTrace("TC2");
		memset(&E_Transac8583,0x00,sizeof(E_Transac8583));
		memset(&R_Transac8583,0x00,sizeof(R_Transac8583));
		memset(&StructInfoPayment ,0x00,sizeof(StructInfoPayment));
		return status;
	}


//	if(memcmp(FicPara.MCC,"6010",4) == 0)
//		StructInfoPayment.CodeFunction = CASHADVANCESCODE;
//	else
//		StructInfoPayment.CodeFunction = SALECODE;

//	StructInfoPayment.Entry_Mode = TRS_MAG;


	// droit de forcage
	if(
		(!StructInfoPayment.Pin_Required)&&
		(StructInfoPayment.CodeFunction!=PREAUTHCODE)
	  )
	{
		//APEMV_UI_MessageDisplayDelay ("transa_client 2", 1);
	//	ShowTrace("TC21");
		status=forcage();
		//ShowTrace("forcage");
		if(status==OK)
			StructInfoPayment.forcage_possible=1;
	}
//	ShowTrace("TC3");
	if(
		(StructInfoPayment.SeviceCode[0]!='1')
		&&(StructInfoPayment.SeviceCode[0]!='2')
		&&(StructInfoPayment.Entry_Mode != TRS_MAN)
		)
		fixdevise=1 ;

	// Initier la devise par default
	devise_num=0;
//	ShowTrace("TC4");
	switch (StructInfoPayment.CodeFunction)
	{
		case	CASHADVANCESCODE :
		case	SALECODE:
		case	PREAUTHCODE:
		case	REFUNDCODE:
			//cas de refund emv
		//	ShowTrace("TC5");
			//APEMV_UI_MessageDisplayDelay ("transa_client 3", 1);
			if(StructInfoPayment.Entry_Mode == TRS_EMV)
			{
				ShowTrace("TC6");
				break;
			}
			/*
			do{
				memcpy(&StructInfoPayment.Devise_acc,&Fic_Devise_Acc.TabDevise[devise_num],sizeof(devise_accept));

				//ShowTrace("TC7");
				status = GetAmountCmi(SALECODE,&l_ul_montant,fixdevise,&devise_num,12) ;
				if(status == KEY_CLEAR)
				{
					ShowTrace("TC8");
					memset(&E_Transac8583,0x00,sizeof(E_Transac8583));
					memset(&R_Transac8583,0x00,sizeof(R_Transac8583));
					memset(&StructInfoPayment ,0x00,sizeof(StructInfoPayment));
					return ABORTED;
				}

				memcpy(&StructInfoPayment.Devise_acc,&Fic_Devise_Acc.TabDevise[devise_num],sizeof(devise_accept));
				// si la devise n'est pas une devise par d?fault alors Dem autor
				if(devise_num!=0)
				{
					ShowTrace("TC9");
					StructInfoPayment.Flag_Dem_Aut=1;
				}

				if((Valide_Mont=AmountValid(&l_ul_montant,indices_offline))==AMOUNT_MIN)
				{
					ShowTrace("TC10");
					CUMORE_CMI_AFFICHE_MESSAGE(11,138,1000);
					//Mess_delay(11,138,0,100,1);
				}
			}while(Valide_Mont==AMOUNT_MIN);
			*/

//			l_ul_montant = atol(StructInfoCaisse[0].MT);//Cless_
//			sprintf((char*)StructInfoPayment.Devise_acc.code_num,"%s",StructInfoCaisse[0].DE);//Cless_
//
//			StructInfoPayment.Transaction_Amount = l_ul_montant;

			if(StructInfoPayment.Double_tra==1)
			{
				ShowTrace("TC11");
				if(ValiderMessage(12,13,0)!=KEY_ENTER)
				{
					//APEMV_UI_MessageDisplayDelay ("transa_client 4", 1);
					ShowTrace("TC12");
					memset(&E_Transac8583,0x00,sizeof(E_Transac8583));
					memset(&R_Transac8583,0x00,sizeof(R_Transac8583));
					memset(&StructInfoPayment ,0x00,sizeof(StructInfoPayment));
					return ABORTED;
				}
			}
			break;

		case	PREAUTHCOMPCODE:
			//StructInfoPayment.Flag_Dem_Aut=1;     //larbi 10042019 d?commenter instruction
			break;

		case VOIDCODE:
			ShowTrace("TC13");
			break;
		default:
			ShowTrace("TC14");
			break;
	}
	//ShowTrace("GetAmountCmi");
	//APEMV_UI_MessageDisplayDelay ("transa_client 5", 1);
	// Demande d'autorisation pour la saisie manuelle ou une pr?-autorisation
	if((StructInfoPayment.Entry_Mode == TRS_MAN)||(StructInfoPayment.CodeFunction == PREAUTHCODE))
		StructInfoPayment.Flag_Dem_Aut=1;

	if(
		((StructInfoPayment.CodeFunction != REFUNDCODE) &&
		(StructInfoPayment.CodeFunction != PREAUTHCOMPCODE)) &&
		(FicPara.sondage)
	  )
		StructInfoPayment.Flag_Dem_Aut=1;

//	ShowTrace("TC15");
	if(memcmp(&StructInfoPayment.Holder_Card[1],FicPara.Last_Card_Rejected,StructInfoPayment.Holder_Card[0])==0)
		StructInfoPayment.Flag_Dem_Aut=1;
	////////////////////////////////////

	//StructInfoPayment.Pin_Required = 1 ;//larbi

	//if((StructInfoPayment.Pin_Required)&&(StructInfoPayment.CodeFunction != REFUNDCODE)&&(StructInfoPayment.Entry_Mode != TRS_MAN))
	if((StructInfoPayment.CodeFunction != REFUNDCODE)&&(StructInfoPayment.Entry_Mode != TRS_MAN))
	{
		//APEMV_UI_MessageDisplayDelay ("transa_client 6", 1);
		StructInfoPayment.Entry_Pin=PIN_ONLINE;
		StructInfoPayment.Flag_Dem_Aut=1;


		if(traitement_code_conf() != OK)
		{

			//Os__xprintaa("888888");
			memset(&E_Transac8583,0x00,sizeof(E_Transac8583));
			memset(&R_Transac8583,0x00,sizeof(R_Transac8583));
			memset(&StructInfoPayment ,0x00,sizeof(StructInfoPayment));
			//Os__xprintaa("111");
			//return(CANCEL_INPUT);//belhadeg
			return ABORTED;
		}
	}
	////////////////////////////////////
//	ShowTrace("TC17");
	status = Prepare_Transaction();
	//APEMV_UI_MessageDisplayDelay ("transa_client 7", 1);
	//ShowTrace("GetAmountCmi");
//	ShowTrace("TC18");
	l_retour=OK ;
	if((!StructInfoPayment.Flag_Dem_Aut)&&(StructInfoPayment.Entry_Mode != TRS_MAN))
	{
		//verifier si Nbr de tran offline successsives < Nbre max de tran offline
		nbr_tra_off_succ=FicTra.nbr_tra_off_succ;
		if(nbr_tra_off_succ >= FicPara.Max_trs_off)
			StructInfoPayment.Flag_Dem_Aut=1 ;
	}
	memset(update_param,0,6) ;
	memset(update_date,0,11) ;


	StructInfoPayment.Author_Status = 0xff;

	StructInfoPayment.Flag_Dem_Aut = 1;


//	ShowTrace("TC19");
	if(((StructInfoPayment.Flag_Dem_Aut)||(StructInfoPayment.SeviceCode[1]=='2')||(FicPara.Indic_Offline!='1'))
		&&(StructInfoPayment.CodeFunction != REFUNDCODE)
	  )
	{
//		ShowTrace("TC20");
		l_retour=DemAutor8583(update_param,update_date);   // demande aut

		//Cless_ Livraison l_retour = OK;

		ShowTrace("TC21");
		//ShowTrace("DemAutor8583");
		StructInfoPayment.Author_Status = l_retour;
		StructInfoPayment.Flag_Dem_Aut=1;
		FicPara.sondage = 0;
		PutAllParam();
	}
	else
		StructInfoPayment.Flag_Dem_Aut = 0;

	l_retour_seq = l_retour;
	Status_trs = OK;
	switch(l_retour)
	{
		case OK :
			if(StructInfoPayment.Author_Status==OK)
				StructInfoPayment.Top_Forcage = TOP_FOR_ONLINE;

			memcpy(StructInfoPayment.Approval_Code,R_Transac8583.champs38,6);

			Naps_Approve();

			break ;


		case CAPTURE_CARTE :
			ValiderMessage(176,138,1);
			StructInfoPayment.Flag_Dem_Aut = 0;
			ImprimeCapture_carte(StructInfoPayment.Langue,COMMERCANT);
			ImprimeCapture_carte(FicPara.Langue,CLIENT);
			j=FicTra.nbr_card_capture+1;
			set_ushort_secourue(&FicTra.nbr_card_capture,j);//wd
			Status_trs = FORBIDDEN_CARD;
			break ;

		case REFERRAL :
      		StructInfoPayment.Flag_Dem_Aut = 0;
			StructInfoPayment.Top_Forcage = TOP_FOR_REFFERAL;

			if(StructInfoPayment.CodeFunction == PREAUTHCOMPCODE)
			{
				StructInfoPayment.Flag_Dem_Aut=0;
				status=KEY_CLEAR;
				Status_trs = REFUSED;
				CUMORE_CMI_AFFICHE_MESSAGE(138,128,1000);
				break;
			}

			status=ValiderMessage(62,63,0);
			if(status==KEY_ENTER)
			{
				memset(Buffer,0,sizeof(Buffer));

				if(ucGetField(6,"",Buffer,7,1,6,"      ",30000,0)==OK)
				{
					//FillWithZero(Buffer,6); //commenter par larbi 12042019
					memcpy(StructInfoPayment.Approval_Code,Buffer,6);
				}
				else
				{
					status=KEY_CLEAR ;
					Status_trs = ABORTED;
				}
				if(status==KEY_ENTER)
				{
					do
					{
						status = Read_Card(PtIso,CARTE_COM);
					}while(status==SAIS_MAN);
					if(status==OK)
					{
						if(Recup_Info_badge(PtIso,0)==OK)
						{
							StructInfoPayment.Flag_Dem_Aut=0 ;
							Os__saved_copy((PUC)"000000", (uchar*)&FicPara.Num_seqAnnul,6);
							l_retour_seq=OK ;

            				StructInfoPayment.forcage = 1;
							if(ImprimeTra(FicPara.Langue,COMMERCANT,0) != OK)
								break;
							status=Valider_Transac();
							if(status==KEY_ENTER)
							{
								SaveTra() ;
								if(StructInfoPayment.CodeFunction != PREAUTHCODE){
									ImprimeTra(StructInfoPayment.Langue,CLIENT,0);
								}

								Status_trs = OK;
							}
							else
								Status_trs = ABORTED;
						}
						else
						{
							Status_trs = ABORTED;
							CUMORE_CMI_AFFICHE_MESSAGE(14,16,1000);
							//Mess_delay(14,16,0,300,1);
						}
					}
					else if(status==ILLISIBLE)
					{
						Status_trs = ABORTED;
						CUMORE_CMI_AFFICHE_MESSAGE(14,15,1000);
						//Mess_delay(14,15,0,300,1);
					}
					else
						Status_trs = ABORTED;
				}
			}
			else
				Status_trs = ABORTED;
			break;

		case NON_AUTOR :
			StructInfoPayment.Flag_Dem_Aut = 0;
			status=KEY_CLEAR;
			Status_trs = REFUSED;
			CUMORE_CMI_AFFICHE_MESSAGE(138,128,1000);
			//Mess_delay(138,128,1,100,1);
			break ;

		case PIN_FAUX :
			StructInfoPayment.Flag_Dem_Aut = 0;
			status=KEY_CLEAR;
			Status_trs = REFUSED;
			CUMORE_CMI_AFFICHE_MESSAGE(138,183,1000);
			//Mess_delay(138,183,1,100,1);
			break;
		case CONNEXION_KO:
		case FORMAT_KO :
		case EMISSION_KO :
      		StructInfoPayment.Flag_Dem_Aut = 0;
			StructInfoPayment.Top_Forcage = TOP_FOR_FORCED;

			if(StructInfoPayment.CodeFunction == PREAUTHCOMPCODE)
			{
				StructInfoPayment.Flag_Dem_Aut=0;
				status=KEY_CLEAR;
				Status_trs = REFUSED;
				CUMORE_CMI_AFFICHE_MESSAGE(138,128,1000);
				break;
			}

			CUMORE_CMI_AFFICHE_MESSAGE(138,127,1000);
			//Mess_delay(138,127,1,100,1);
			//hang_up(typehangup,g_trace);
			if(StructInfoPayment.forcage_possible==1)
			{
				status=ValiderMessage(66,7,0);
				if(status==KEY_ENTER)
				{
					//Mess_delay(6,138,0,100,0);
					memset(Buffer,0,sizeof(Buffer));

					if(ucGetField(6,"",Buffer,7,1,6,"      ",30000,0)==OK)
					{
						//FillWithZero(Buffer,6); //commenter par larbi 12042019
						memcpy(StructInfoPayment.Approval_Code,Buffer,6) ;
					}
					else
					{
						status=KEY_CLEAR ;
						Status_trs = ABORTED;
					}

					if(status==KEY_ENTER)
					{
						do
						{
							status = Read_Card(PtIso,CARTE_COM);
						}while(status==SAIS_MAN);
						if(status==OK)
						{
							if(Recup_Info_badge(PtIso,0)==OK)
							{
								StructInfoPayment.Flag_Dem_Aut=0;
								Os__saved_copy((PUC)"000000", (uchar*)&FicPara.Num_seqAnnul,6);
								l_retour_seq=OK ;

								StructInfoPayment.forcage = 1;

								if(ImprimeTra(FicPara.Langue,COMMERCANT,0) != OK)
									break;
								status=Valider_Transac();
								if(status==KEY_ENTER)
								{
									SaveTra() ;
									if(StructInfoPayment.CodeFunction != PREAUTHCODE){
										ImprimeTra(StructInfoPayment.Langue,CLIENT,0);
									}

									Status_trs = OK;
								}
								else
									Status_trs = ABORTED;
							}
							else
							{
								Status_trs = ABORTED;
								CUMORE_CMI_AFFICHE_MESSAGE(14,16,1000);
								//Mess_delay(14,16,0,300,1);
							}
						}
						else if(status==ILLISIBLE)
						{
							Status_trs = ABORTED;
							CUMORE_CMI_AFFICHE_MESSAGE(14,15,1000);
							//Mess_delay(14,15,0,300,1);
						}
						else
							Status_trs = ABORTED;
					}
				}
			}
			break;

		case RECEPT_KO :
		case FORMAT_MSG_REC_KO:
			StructInfoPayment.Flag_Dem_Aut = 0;
			CUMORE_CMI_AFFICHE_MESSAGE(138,127,1000);
			//Mess_delay(138,127,1,100,1);
			//hang_up(typehangup,g_trace);
			Status_trs = REFUSED;
			break;
	}

	// Discussion num?ro de s?quence
	Sequence(l_retour_seq,status) ;


	PutAllTra();
	PutAllPre();
//	memset(size_carte,'\0',3);
//	sprintf(size_carte,"%d",StructInfoPayment.Holder_Card[0]);
//	ShowTrace(size_carte);
//	imp_buffer(StructInfoPayment.Holder_Card,20);

	// Gestion des avis


   if((StructInfoPayment.Author_Status == OK)||(StructInfoPayment.Author_Status == CAPTURE_CARTE)||(StructInfoPayment.Author_Status == NON_AUTOR))
   {
	   Os__xprintaa("Gest_avis");
		l_retour = Gest_avis(indices_offline);
   }

/*
	// Modification de fichers de param?tres s'il ya une nouvelle version
	if(l_retour == OK)
	{
		if((StructInfoPayment.Author_Status == OK)||(StructInfoPayment.Author_Status == CAPTURE_CARTE)||(StructInfoPayment.Author_Status == NON_AUTOR))
		{
			gest_param();
			PutAllParam();
			PutAllLop();
		}
		else if(StructInfoPayment.CodeFunction != REFUNDCODE)//Mouna310305
		{
			hang_up(typehangup,g_trace);
		}
	}
	else
	{
	//	hang_up(typehangup,g_trace);//Mouna310305
	}
*/

//   if((StructInfoPayment.Author_Status == OK)||(StructInfoPayment.Author_Status == CAPTURE_CARTE)||(StructInfoPayment.Author_Status == NON_AUTOR))
//	   hang_up(typehangup,g_trace);

	return Status_trs;
}

/****************************************************************************
*                                                                           *
*      SOUS-PROGRAMME :  Gest_avis                                          *
*                                                                           *
*****************************************************************************
*                                                                           *
*      FONCTION :    Gestion des avis                                       *
*                                                                           *
*      ENTREE   :  indice offline                                           *
*                                                                           *
*      SORTIE   :  status                                                   *
*                                                                           *
****************************************************************************/
uchar Gest_avis(Ushort *indices_offline)
{
	Ushort	i;
	uchar	montant[13];

	memset(montant,0,13);
	i=1;
	while(i<=indices_offline[0])
	{
		CUMORE_CMI_AFFICHE_MESSAGE(121,32,1000);
		//Aff_mess(FicPara.Langue,121,0,0,0,0);
		//Aff_mess(FicPara.Langue,32,1,0,0,0) ;

		read_trans_index( indices_offline[i] ,&StrLineTra);
		if(Dem_avis()==OK)
		{
			StrLineTra.avis=1;
			memcpy(montant,R_Transac8583.champs5,12);
			StrLineTra.Consolidation_Amount=atol((PC)(PC)montant);
			memcpy(StrLineTra.NumCurrency_cons,R_Transac8583.champs50,3);
			memcpy(StrLineTra.Message_Type,"1220",4);
			memcpy(StrLineTra.STAN,E_Transac8583.champs11,6);
			write_trans_index( indices_offline[i],&StrLineTra );
		}
		else
		{
			hang_up(typehangup,g_trace);
			return KO;
		}
		i+=1;
	}
	return OK;
}





/*-----------------------------------------------------------------
 * Fonction   :
 * objet      :
 *
 * Parametre  :
 * Retour     :
 *-----------------------------------------------------------------
*/
uchar	Efface_Trs(void)
{
	Ulong	num_batch;
	uchar	Date_sys[7];
	uchar	Date_fic[7];
	DATE date;



//	return 0;




	memset(Date_sys,0,7);
	memset(Date_fic,0,7);

	Telium_Read_date(&date);
	memcpy(Date_fic,date.day,2);
	memcpy(Date_fic+2,date.month,2);
	memcpy(Date_fic+4,date.year,2);

	/*Os__read_date(Date_sys);
	memcpy(Date_fic,&Date_sys[4],2);
	memcpy(Date_fic+2,&Date_sys[2],2);
	memcpy(Date_fic+4,&Date_sys[0],2);*/
	Os__saved_set ((uchar *)&FicTra, 0x00, sizeof(FicTra));
	num_batch=FicTra.num_batch;
	num_batch++;
	Os__saved_set ((uchar *)&FicTra, 0x00, sizeof(FicTra));
	set_ulong_secourue(&FicTra.num_batch,num_batch);
	Os__saved_copy(Date_fic,(uchar *)&FicTra.date_fic[0],6);

	set_ushort_secourue(&FicTra.nombre_tra,0);
	set_ushort_secourue(&FicTra.nbr_tra_off_succ,0);
	set_ushort_secourue(&FicTra.nbr_card_capture,0);
	Os__saved_copy((PUC)"000001",(uchar *)&FicPara.Num_sequence,6);

	return 0;
}
/*-----------------------------------------------------------------
 * Fonction   :
 * objet      :
 *
 * Parametre  :
 * Retour     :
 *-----------------------------------------------------------------
*/
uchar	Efface_File_Preauto(void)
{
	Os__saved_set ((uchar *)&FicPreauth, 0x00, sizeof(FicPreauth));
	set_ushort_secourue(&FicPreauth.nombre_tra,0) ;
	set_ulong_secourue(&FicPreauth.Indice_fin,0) ;

	return 0;
}






/****************************************************************************
*                                                                           *
*      SOUS-PROGRAMME :   IncStan				                            *
*                                                                           *
*****************************************************************************
*                                                                           *
*      FONCTION :     Incrementation du STAN		      					*
*                                                                           *
*      ENTREE   :  RIEN                                          			*
*                                                                           *
*      SORTIE   :  RIEN		                                                *
*                                                                           *
****************************************************************************/
void	IncStan(void)
{
	uchar	chaine[7];
	Ulong 	Mess_Nb;

	//APEMV_UI_MessageDisplayDelay(FicPara.Message_Number,1);

	Mess_Nb = atol((PC)FicPara.Message_Number);
	if(Mess_Nb==999999)
		Mess_Nb = 1;
	else
		Mess_Nb += 1;
	//long_asc(chaine,6,&Mess_Nb);
	sprintf(chaine,"%.06d",Mess_Nb);
	Os__saved_copy(chaine,(uchar *)FicPara.Message_Number,6);

	//APEMV_UI_MessageDisplayDelay(FicPara.Message_Number,1);

	PutAllParam();//CCD
}


/************************************************************************
*                                                                       *
*       SOUS-PROGRAMME : valide_carte                                   *
*                                                                       *
*************************************************************************
*                                                                       *
*       Fonction : test des xxdata de la carte (piste ou puce)          *
*                                                                       *
*       Entree : @ de la structure d'information                        *
*                                                                       *
*       Sortie : status                                                 *
* ***********************************************************************/
uchar    valide_carte(void)
{
	uchar	status;
	uchar	bin[10];
	uchar	Number[20];

	uchar	Card_Nb_Ascci[20];
	uchar	Card_Nb_bcd[10];

	// Controle de fichier de transact s'il est plein
//	ShowTrace("VC0");
	status = tst_fic_plein();
	if(status==OK)
	{
		ShowTrace("VC1");
		CUMORE_CMI_AFFICHE_MESSAGE(130,131,1000);
		return TRANS_FILE_FULL;
	}
	//ShowTrace_Mouna("aa");
	memset(Number,'\0',20);
	Number[0] = StructInfoPayment.Holder_Card[0];
	memcpy(&Number[1],&StructInfoPayment.Holder_Card[1],StructInfoPayment.Holder_Card[0]);
	status = luhn_key_ok(Number);
	if(status != TRUE)
	{
		ShowTrace("VC2");
		CUMORE_CMI_AFFICHE_MESSAGE(132,133,1000);
	}

	// Controle BIN
	memset(bin,0,10);
	memcpy(bin,&StructInfoPayment.Holder_Card[1],6);
	if(BinPrefixControl(StructInfoPayment.Holder_Card,1)!=OK)
	{
		ShowTrace("VC3");
		return REFUSED;
	}
	//ShowTrace_Mouna("bb");
	if(
		memcmp(Fic_Devise_Acc.TabDevise[0].code_num,"504",3)&&
		((StructInfoPayment.SeviceCode[0]!=0)&&(StructInfoPayment.SeviceCode[0]!='1')&&(StructInfoPayment.SeviceCode[0]!='2')&&(StructInfoPayment.Entry_Mode != TRS_MAN))
		)
	{
		ShowTrace("VC4");
		if(StructInfoPayment.Entry_Mode != TRS_EMV){
//			uchar Card_Nb[20];
//			memset(Card_Nb, 0, 20);
//			memcpy(Card_Nb, &StructInfoPayment.Holder_Card[1],
//					StructInfoPayment.Holder_Card[0]);
//			Os__xprinta(Card_Nb);
			CUMORE_CMI_AFFICHE_MESSAGE(134,135,1000);
		}
		return REFUSED;
	}
	//ShowTrace_Mouna("cc");
	if(
		(memcmp(StructInfoPayment.Expired_Date,"7001",4)>0)
		||	(memcmp(StructInfoPayment.Expired_Date,StructInfoPayment.Transaction_DateTime,4)<0)
		)
	{
		ShowTrace("VC5");
		CUMORE_CMI_AFFICHE_MESSAGE(177,138,1000);
		strncpy(StructInfoCaisse[1].CRPIN,"101",3);
		return EXPIRED_CARD;
	}
	//ShowTrace_Mouna("dd");
	//Contr?le des carte Amex, Diner, Jcb
	if(StructInfoPayment.Entry_Mode==TRS_MAN)
	{
//		ShowTrace("VC6");
		if(StructInfoPayment.Code_Prefixe ==30)
		{
			ShowTrace("VC7");
			if(StructInfoPayment.Entry_Mode != TRS_EMV)
				CUMORE_CMI_AFFICHE_MESSAGE(7,3,1000);
			return REFUSED;
		}
		if(StructInfoPayment.Code_Prefixe ==34)
		{
			ShowTrace("VC8");
			if(StructInfoPayment.Entry_Mode != TRS_EMV)
				CUMORE_CMI_AFFICHE_MESSAGE(7,3,1000);
			return REFUSED;
		}
	}
	switch (StructInfoPayment.CodeFunction)
	{
		case    PREAUTHCODE:
			ShowTrace("VC9");
			if(StructInfoPayment.Code_Prefixe ==35)
			{
				ShowTrace("VC10");
				if(StructInfoPayment.Entry_Mode != TRS_EMV)
					CUMORE_CMI_AFFICHE_MESSAGE(7,3,1000);
				return REFUSED;
			}

		case    REFUNDCODE:
			ShowTrace("VC11");
			if(StructInfoPayment.Code_Prefixe ==30)
			{
				ShowTrace("VC12");
				if(StructInfoPayment.Entry_Mode != TRS_EMV)
					CUMORE_CMI_AFFICHE_MESSAGE(7,3,1000);
				return REFUSED;
			}
			if(StructInfoPayment.Code_Prefixe ==34)
			{
				ShowTrace("VC13");
				if(StructInfoPayment.Entry_Mode != TRS_EMV)
					CUMORE_CMI_AFFICHE_MESSAGE(7,3,1000);
				return REFUSED;
			}
			break;

		default	:
//			ShowTrace("VC14");
			break;
	}
	//ShowTrace_Mouna("ff");
	//si l'?tat de la liste noire KO alors Dem autor
	StructInfoPayment.Carte_EnLN=0;
	if(lop_gest.stat==KO)
	{
		StructInfoPayment.Carte_EnLN=1;
		//CCDCUSERV_IsInBlackList=B_TRUE;
		StructInfoPayment.Flag_Dem_Aut=1;
	}
	else
	{
//		ShowTrace("VC16");
		memset(Card_Nb_Ascci,0x0F,20);
		memcpy(Card_Nb_Ascci,&StructInfoPayment.Holder_Card[1],
			StructInfoPayment.Holder_Card[0]);
		asc_bcd(Card_Nb_bcd,10,Card_Nb_Ascci,20);
		status = rechlop_trn(Card_Nb_bcd);
		if(status == KO)
		{
			StructInfoPayment.Carte_EnLN=1;
			CUSERV_IsInBlackList=B_TRUE;
			StructInfoPayment.Flag_Dem_Aut=1;
			//Os__xprint("Carte en LOP TRN");
		}
		else
		{
			status = rechlop_bin(bin);
			if(status == KO)
			{
				StructInfoPayment.Carte_EnLN=1;
				CUSERV_IsInBlackList=B_TRUE;
				StructInfoPayment.Flag_Dem_Aut=1;
				//Os__xprint("Carte en LOP BIN");
			}
			else
			{
				status = rechlop(Card_Nb_bcd);
				if(status == KO)
				{
					StructInfoPayment.Carte_EnLN=1;
					CUSERV_IsInBlackList=B_TRUE;
					StructInfoPayment.Flag_Dem_Aut=1;
					//Os__xprint("Carte en LOP");
				}
			}
		}
	}
	//ShowTrace_Mouna("gg");
/*
	if(	(!StructInfoPayment.Pin_Required)
	&&	(StructInfoPayment.Entry_Mode == TRS_MAG)
	)
	{
			status=Verification_Digits();
			if (status==REFUSED)
			{
				ShowTrace("VC18");
				CUMORE_CMI_AFFICHE_MESSAGE(136,137,1000);
			}
			if(status!=OK)
			{
				ShowTrace("VC19");
				return status;
			}
	}*/

	return(OK);
}
/*-----------------------------------------------------------------
 * Fonction   :
 * objet      :
 *
 * Parametre  :
 * Retour     :
 *-----------------------------------------------------------------
*/
Ushort	SaveTra(void)
{
	Ushort		position ;


	//memset((uchar*)&transact,0,sizeof(lignetra1));

	FicPara.AutoCollecteExecute = 0;

	if(StructInfoPayment.CodeFunction!=PREAUTHCODE)
	{
		RemplirSpecification();
		position=FicTra.nombre_tra ;
		if(position<NBMAXTRA)
		{
			write_trans_index( position,&StrLineTra) ;
			position++ ;
			set_ushort_secourue(&FicTra.nombre_tra,position) ;
		}
	}
	else
	{
		RemplirPreaut();
		position = FicPreauth.nombre_tra ;
		write_preauto_index( position,&StrPreAut) ;
		position++ ;
		set_ushort_secourue(&FicPreauth.nombre_tra,position) ;
		if(FicPreauth.Indice_fin+1 < 999999)
			set_ulong_secourue(&FicPreauth.Indice_fin,FicPreauth.Indice_fin+1) ;
		else
			set_ulong_secourue(&FicPreauth.Indice_fin,0) ;

	}
	//CCD

	PutAllTra();
	PutAllPre();
	PutAllParam();
	return OK ;
}
/*-----------------------------------------------------------------
 * Fonction   :
 * objet      :
 *
 * Parametre  :
 * Retour     :
 *-----------------------------------------------------------------
*/
void RemplirSpecification()
{
	uchar	montant[13];

	memset(montant,0,13);
	memset(&StrLineTra,0,sizeof(StrLineTra));
	memcpy(StrLineTra.Num_point_vente,StructInfoPayment.Num_point_vente,strlen((PC)StructInfoPayment.Num_point_vente));

	if((strlen(StructInfoPayment.Approval_Code) == 0) && (StructInfoPayment.Flag_Dem_Aut == 0)){
			memcpy(StructInfoPayment.Message_TypeisoEmi,"1220",4);
			StructInfoPayment.Flag_Dem_Aut = 0;
		}else{
			memcpy(StructInfoPayment.Message_TypeisoEmi,"1200",4);
			StructInfoPayment.Flag_Dem_Aut = 1;
		}


	StrLineTra.Flag_Dem_Aut =StructInfoPayment.Flag_Dem_Aut;
	StrLineTra.Top_Forcage =StructInfoPayment.Top_Forcage;
	memcpy(StrLineTra.Proc_Code,StructInfoPayment.Proc_Code,6);
	memcpy(StrLineTra.Message_Type,StructInfoPayment.Message_TypeisoEmi,4);
	memcpy(StrLineTra.RRN,FicPara.Num_sequence,6);
	StrLineTra.Entry_Mode =StructInfoPayment.Entry_Mode;
	StrLineTra.Entry_Pin =StructInfoPayment.Entry_Pin;
	StrLineTra.CodeFunction = StructInfoPayment.CodeFunction;
	StrLineTra.Transaction_Amount=StructInfoPayment.Transaction_Amount;
	StrLineTra.MontantPourboireInterchange=StructInfoPayment.MontantPourboireInterchange;	//POURBOIRE
	StrLineTra.Preauto_Amount_Final=StructInfoPayment.Preauto_Amount_Final;
	StrLineTra.Preauto_indic_completion = StructInfoPayment.Preauto_indic_completion;
	memcpy(StrLineTra.NumCurrency,StructInfoPayment.Devise_acc.code_num,3);

	memcpy(StrLineTra.STAN,StructInfoPayment.STAN,6);
	StrLineTra.StatusAnnulAdjust = StructInfoPayment.StatusAnnulAdjust;

	if(StrLineTra.Flag_Dem_Aut)
	{
   		memset(montant,0,13);
		memcpy(montant,R_Transac8583.champs5,12);
		StrLineTra.Consolidation_Amount=atol((PC)montant) ;
		memcpy(StrLineTra.NumCurrency_cons,R_Transac8583.champs50,3) ;
	}
	else
	{
		StrLineTra.Consolidation_Amount=StructInfoPayment.Transaction_Amount ;
		memcpy(StrLineTra.NumCurrency_cons,StrLineTra.NumCurrency,3) ;
		////////////////////////////////////
		if(StrLineTra.CodeFunction==VOIDCODE)
			memcpy(StrLineTra.Message_Type,"1420",4);
		else
			memcpy(StrLineTra.Message_Type,"1220",4);
		////////////////////////////////////
	}
	memcpy(StrLineTra.Transaction_DateTime,StructInfoPayment.Transaction_DateTime,12);
	if(StrLineTra.Entry_Mode == TRS_MAN)
		memcpy(StrLineTra.Compressed_Iso2,StructInfoPayment.Holder_Card,20);
	else
		memcpy(StrLineTra.Compressed_Iso2,StructInfoPayment.Compressed_Iso2,20);

	memcpy(StrLineTra.Expired_Date,StructInfoPayment.Expired_Date,4);
	// date de saisie : champ17
	memcpy(StrLineTra.Num_sequence,StructInfoPayment.Num_sequence,6);
	memcpy(StrLineTra.Approval_Code,StructInfoPayment.Approval_Code,6);
	if(StrLineTra.Approval_Code[0]==0)
		memcpy(StrLineTra.Approval_Code,"      ",6);

	// champ  55 : EMV
	if(StrLineTra.Entry_Mode == TRS_EMV)
	{
		StrLineTra.Lg_EmvData = StructInfoPayment.l_champ55;
		memcpy(StrLineTra.EmvData , StructInfoPayment.Champ55, StrLineTra.Lg_EmvData);
		memcpy(StrLineTra.PanSeqNumber,StructInfoPayment.PanSeqNumber,3);
	}
	memcpy(StrLineTra.Appli_aid,StructInfoPayment.Appli_aid,34);// juste pour la duplicata
	memcpy(StrLineTra.Appli_label,StructInfoPayment.Appli_label,16); // juste pour la duplicata
	memcpy(StrLineTra.Nom_Prenom,StructInfoPayment.Nom_Prenom,27); // juste pour la duplicata
	StrLineTra.Exposant=StructInfoPayment.Devise_acc.Exposant;
	StrLineTra.Type_carte=StructInfoPayment.Type_carte ;
	StrLineTra.Origine_carte=StructInfoPayment.Origine_carte ;//modif 04/04
	StrLineTra.Langue=StructInfoPayment.Langue ;
}

/*-----------------------------------------------------------------
 * Fonction   :
 * objet      :
 *
 * Parametre  :
 * Retour     :
 *-----------------------------------------------------------------
*/
void	RemplirPreaut(void)
{
	uchar	montant[13];
	UC  iso2[38];

	memset(montant,0,13);
	memset(&StrPreAut,0,sizeof(StrPreAut));
	memcpy(StrPreAut.Num_point_vente,StructInfoPayment.Num_point_vente,strlen((PC)StructInfoPayment.Num_point_vente));
	StrPreAut.Flag_Dem_Aut =StructInfoPayment.Flag_Dem_Aut;
	StrPreAut.Top_Forcage =StructInfoPayment.Top_Forcage;
	memcpy(StrPreAut.Proc_Code,StructInfoPayment.Proc_Code,6);
	memcpy(StrPreAut.Message_Type,StructInfoPayment.Message_TypeisoEmi,4);
	memcpy(StrPreAut.RRN,FicPara.Num_sequence,6);
	StrPreAut.Entry_Mode =StructInfoPayment.Entry_Mode ;
	StrPreAut.Entry_Pin =StructInfoPayment.Entry_Pin ;
	StrPreAut.CodeFunction = StructInfoPayment.CodeFunction;
	StrPreAut.Transaction_Amount=StructInfoPayment.Transaction_Amount ;
	memcpy(StrPreAut.NumCurrency,StructInfoPayment.Devise_acc.code_num,3) ;

	memcpy(StrPreAut.STAN,StructInfoPayment.STAN,6);//modif 04/04

	StrPreAut.StatusAnnulAdjust = StructInfoPayment.StatusAnnulAdjust ;

	if(memcmp(StrPreAut.Message_Type,"1200",4)==0 )
		StrLineTra.Flag_Dem_Aut = 1;
	else
		StrLineTra.Flag_Dem_Aut = 0;

	if(StrPreAut.Flag_Dem_Aut)
	{
		memset(montant,0,13);
		memcpy(montant,R_Transac8583.champs5,12);
		StrPreAut.Consolidation_Amount=atol((PC)montant) ;
		memcpy(StrPreAut.NumCurrency_cons,R_Transac8583.champs50,3) ;
	}
	else
	{
		StrPreAut.Consolidation_Amount=StructInfoPayment.Transaction_Amount ;
		memcpy(StrPreAut.NumCurrency_cons,StrPreAut.NumCurrency,3) ;
		/////////////////////////////////////
		if(StrPreAut.CodeFunction==VOIDCODE)
			memcpy(StrPreAut.Message_Type,"1420",4);
		else if((memcmp(StrPreAut.Message_Type,"1200",4)!=0) && (StrPreAut.Message_Type[0] == '1')){
			memcpy(StrPreAut.Message_Type,"1220",4);
			StrPreAut.Flag_Dem_Aut = 0;

			}
		/////////////////////////////////////
	}
	memcpy(StrPreAut.Transaction_DateTime,StructInfoPayment.Transaction_DateTime,12);

	if(StrPreAut.Entry_Mode == TRS_MAN)
		memcpy(StrPreAut.Compressed_Iso2,StructInfoPayment.Holder_Card,20);
	else
		memcpy(StrPreAut.Compressed_Iso2,StructInfoPayment.Compressed_Iso2,20);

	memcpy(StrPreAut.Expired_Date,StructInfoPayment.Expired_Date,4);
	// date de saisie : champ17
	memcpy(StrPreAut.Num_sequence,StructInfoPayment.Num_sequence,6);
	memcpy(StrPreAut.Approval_Code,StructInfoPayment.Approval_Code,6);
	/*if(StrPreAut.Approval_Code[0]==0)
		memcpy(StrPreAut.Approval_Code,"      ",6);*/

	// champ  55 : EMV
	if(StrPreAut.Entry_Mode == TRS_EMV)
	{
		StrPreAut.Lg_EmvData = StructInfoPayment.l_champ55;
		memcpy(StrPreAut.EmvData , StructInfoPayment.Champ55, StrPreAut.Lg_EmvData);
		memcpy(StrPreAut.PanSeqNumber,StructInfoPayment.PanSeqNumber,3);
	}

	StrPreAut.Exposant=StructInfoPayment.Devise_acc.Exposant;
	StrPreAut.Type_carte=StructInfoPayment.Type_carte ;
	StrPreAut.Origine_carte=StructInfoPayment.Origine_carte ;//modif 04/04
	StrPreAut.Langue=StructInfoPayment.Langue ;
	StrPreAut.vie_preauth = 0;
}

/****************************************************************************
*                                                                           *
*      SOUS-PROGRAMME :  GetAmountCmi                                       *
*                                                                           *
*****************************************************************************
*                                                                           *
*      FONCTION :    Gestion de l'ecran du montant                           *
*                                                                           *
*      ENTREE   :  montant,taille                                           *
*                                                                           *
*      SORTIE   :  status                                                   *
*                                                                           *
****************************************************************************/
uchar	GetAmountCmi(C c,Ulong *pTab,uchar fix_devise,Ushort *default_dev,Ushort taille)
{
	uchar	indexfin;//short	indexfin;//wd
	unsigned long entered_amount;

	int merchLang;
	TAB_ENTRY_FCT     Tab;
	unsigned char     fmtAmt;
	int               retgetamt;

	indexfin=(UC)Fic_Devise_Acc.Indice_fin;//wd
	if(indexfin==0)
	{
		//Os__xprint("No Devises");
		return KEY_CLEAR ;
	}
	StructInfoPayment.default_dev=*default_dev;
	StructInfoPayment.fix_devise=fix_devise;
	StructInfoPayment.indexfin=indexfin;


	//affichagedumenu
	if(StructInfoPayment.CodeFunction == PREAUTHCOMPCODE);
	else if(indexfin == 1);
	else if(StructInfoPayment.fix_devise!=1)
		; //Fermer17choix_devise_cmi();

	merchLang = My_PSQ_Give_Language();
	//ShowTrace("GA1");

   if (merchLang == C_LG_FRENCH)
   {
		fmtAmt = 0;
   }
   else
   {
		fmtAmt = 1;
   }

   Tab.time_out = 30;
   Tab.mask = 0;
	//ShowTrace("GA12");

	entered_amount=0;
//	ShowTrace("GA2");
   while(1)
   {
//	   if(c == SALECODE) || (c == VOIDCODE) || (c == REFUNDCODE)
	   {

		//ShowTrace("GA2");
		retgetamt = GetAmount (fmtAmt,
			   (unsigned char *)Fic_Devise_Acc.TabDevise[StructInfoPayment.default_dev].code_alph,
			   1,
			   &entered_amount,
			   (unsigned char *) (ct_DefMess_Cmi [0][merchLang]),
			   &Tab);
	   }
	//ShowTrace("GA3");
	   switch(retgetamt)
	   {
	   case T_F1:
		   if(StructInfoPayment.fix_devise==1)    // FIXDEVISE
			   break;

		   if((--StructInfoPayment.default_dev)==-1)
			   StructInfoPayment.default_dev=StructInfoPayment.indexfin-1;
		   break;
	   case T_F2:
		   if(StructInfoPayment.fix_devise==1)    // FIXDEVISE
			   break;

		   if((++StructInfoPayment.default_dev)==StructInfoPayment.indexfin)
			   StructInfoPayment.default_dev=0 ;
		   break;

	   case T_VAL:
			//ShowTrace("GA4");
		   if(entered_amount != 0)
		   {
				//ShowTrace("GA5");
			   StructInfoPayment.Enter[0]=KEY_ENTER;
			   *pTab=entered_amount;
			   *default_dev=StructInfoPayment.default_dev;
			   return StructInfoPayment.Enter[0];
		   }
		   else
		   {
			   //ShowTrace("GA6");
//			   if(StructInfoPayment.fix_devise==1)    // FIXDEVISE
//				   break;

//			   if((++StructInfoPayment.default_dev)==StructInfoPayment.indexfin)
//				   StructInfoPayment.default_dev=0 ;
		   }
		   break;

	   case T_ANN:
		   StructInfoPayment.Enter[0]=KEY_CLEAR;
		   return StructInfoPayment.Enter[0];


	   default:
		   StructInfoPayment.Enter[0]=ABORTED;
		   break;
	   }
   };
	return StructInfoPayment.Enter[0];
}

/****************************************************************************
*                                                                           *
*      SOUS-PROGRAMME :  AmountValid                                        *
*                                                                           *
*****************************************************************************
*                                                                           *
*      FONCTION :    validation du montant                                  *
*                                                                           *
*      ENTREE   :  montant                                                  *
*                                                                           *
*      SORTIE   :  Resultat de validation                                   *
*                                                                           *
****************************************************************************/
uchar	AmountValid(Ulong* ul_montant,Ushort *indices_offline)
{
	Ulong	l_ul_cumul;

	if(*ul_montant<StructInfoPayment.Devise_acc.Montant_min)
		return AMOUNT_MIN ;

	if(PlafondAtteint(*ul_montant,StructInfoPayment.Holder_Card,indices_offline,&l_ul_cumul)==OK)
	{
		StructInfoPayment.Flag_Dem_Aut	=	0x01;
		return	AMOUNT_OK;
	}
	return AMOUNT_OK ;
}

/****************************************************************************
*                                                                           *
*      SOUS-PROGRAMME :   Sequence                                          *
*                                                                           *
*****************************************************************************
*                                                                           *
*      FONCTION :     Manipulation de num?ro de s?quence                    *
*                                                                           *
*      ENTREE   :  Resultat d'autorisation , status de validation           *
*                                                                           *
*      SORTIE   :  Rien                                                     *
*                                                                           *
****************************************************************************/
void Sequence(uchar aut,uchar validate)
{
	uchar		uc_sequence[7] ;
	Ulong		l_sequence=0 ;
	Ushort		nbr_tra_off_succ;

	memset(uc_sequence,0,7) ;
	switch(aut)
	{
	case OK:
		//si la transac offline alors incr?menter le num sequence et nbre offline successives
		if((StructInfoPayment.Author_Status == 0xff)&&(validate==KEY_ENTER))
		{
			memcpy(uc_sequence,FicPara.Num_sequence,6) ;
			l_sequence=atol((PC)(PC)uc_sequence) ;
			l_sequence++ ;
			long_asc (uc_sequence, 6, &l_sequence);
			Os__saved_copy((PUC)uc_sequence, (uchar*)&FicPara.Num_sequence,6) ;
			nbr_tra_off_succ=FicTra.nbr_tra_off_succ ;
			nbr_tra_off_succ++ ;
			set_ushort_secourue(&FicTra.nbr_tra_off_succ,nbr_tra_off_succ) ;
		}
		else
		{
			// si la transac online valider alors on incr?mente le num de seq,
			// et annuler seq d'annulation
            if((StructInfoPayment.Author_Status!=0xff)&&(validate==KEY_ENTER))
			{
				memcpy(uc_sequence,FicPara.Num_sequence,6) ;
				l_sequence=atol((PC)(PC)uc_sequence) ;
				l_sequence++ ;
				long_asc (uc_sequence, 6, &l_sequence);
				Os__saved_copy((PUC)uc_sequence, (uchar*)&FicPara.Num_sequence,6) ;
				Os__saved_copy((PUC)"000000", (uchar*)&FicPara.Num_seqAnnul,6) ;
				//remise ? zero le nbre succ de tra offline
				set_ushort_secourue(&FicTra.nbr_tra_off_succ,0) ;
			}
			// si la transac online non valider seq d'annulation=Seq
			//et on incr?mente le num de seq
			//else if((StructInfoPayment.Flag_Dem_Aut)&&(validate!=KEY_ENTER))
            else if((StructInfoPayment.Author_Status!=0xff)&&(validate!=KEY_ENTER))
			{
				memcpy(uc_sequence,FicPara.Num_sequence,6) ;
				Os__saved_copy((PUC)uc_sequence, (uchar*)&FicPara.Num_seqAnnul,6) ;
				l_sequence=atol((PC)(PC)uc_sequence) ;
				l_sequence++ ;
				long_asc (uc_sequence, 6, &l_sequence);
				Os__saved_copy((PUC)uc_sequence, (uchar*)&FicPara.Num_sequence,6) ;
			}
		}
		break ;
	case REFERRAL :
		// incr?menter le num de seq et annuler seq d'annulation
		if(validate==KEY_ENTER)
		{
			memcpy(uc_sequence,FicPara.Num_sequence,6) ;
			l_sequence=atol((PC)(PC)uc_sequence) ;
			l_sequence++ ;
			long_asc (uc_sequence, 6, &l_sequence);
			Os__saved_copy((PUC)uc_sequence, (uchar*)&FicPara.Num_sequence,6) ;
			Os__saved_copy((PUC)"000000", (uchar*)&FicPara.Num_seqAnnul,6) ;
			//remise ? zero le nbre succ de tra offline
			set_ushort_secourue(&FicTra.nbr_tra_off_succ,0) ;
		}
		break;
	case NON_AUTOR :
	case CAPTURE_CARTE :
	case RECEPT_KO :
	case FORMAT_MSG_REC_KO :
		// seqAnnul=Seq Ne pas incr?menter le nu seq
		memcpy(uc_sequence,FicPara.Num_sequence,6) ;
		Os__saved_copy((PUC)uc_sequence, (uchar*)&FicPara.Num_seqAnnul,6) ;
		break ;
	default:
		// il faut g?rer le cas (KO):timeout par exemple
		break ;
	}
	PutAllParam();
}

/****************************************************************************
*                                                                           *
*      SOUS-PROGRAMME :   AnnulTra                                          *
*                                                                           *
*****************************************************************************
*                                                                           *
*      FONCTION :     Annuler une transaction                               *
*                                                                           *
*      ENTREE   :  Rien                                                     *
*                                                                           *
*      SORTIE   :  Rien                                                     *
*                                                                           *
****************************************************************************/
uchar AnnulTra()
{
	uchar			Buffer[7];
	uchar			num_aut[7];
	uchar			date_time[13];
	uchar			date_tra[13];
	uchar			Date_sys[7];
	uchar			Time_sys[7];
	Ushort   		indice=0;
	uchar			status ;
	uchar			PtIso[MAXPTISO];

	memset(Buffer,0,7);
	memset(num_aut,0,7);

	memset(date_time,0,13);
	memset(date_tra,0,13);
	memset(Date_sys,0,7);
	memset(Time_sys,0,7);

	memset(&E_Transac8583,0x00,sizeof(E_Transac8583));
	memset(&R_Transac8583,0x00,sizeof(R_Transac8583));
	memset(&StructInfoPayment ,0x00,sizeof(StructInfoPayment));

	StructInfoPayment.puce_out = 1;
	memset(&StrLineTra ,0x00,sizeof(StrLineTra));
	StructInfoPayment.CodeFunction = VOIDCODE;

	// Controle de fichier de transact s'il est plein
	status=tst_fic_plein();
	if(status==OK)
	{
		Os__xprint("Fichier Transac Plein");
		CUMORE_CMI_AFFICHE_MESSAGE(130,131,1000);
		memset(&E_Transac8583,0x00,sizeof(E_Transac8583));
		memset(&R_Transac8583,0x00,sizeof(R_Transac8583));
		memset(&StructInfoPayment ,0x00,sizeof(StructInfoPayment));
		return KO;
	}

	if (control_badge() != OK)
		return KO ;

/*
	////////////////////////////////////
	do
	{
		status = Read_Card(PtIso,CARTE_COM);
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
	}*/

	// STAN

	if(ucGetField(175,"ANNULATION TRX",Buffer,7,6,6,"     ",30000,0)!=OK)
	{
		memset(&E_Transac8583,0x00,sizeof(E_Transac8583));
		memset(&R_Transac8583,0x00,sizeof(R_Transac8583));
		memset(&StructInfoPayment ,0x00,sizeof(StructInfoPayment));
		return KO;
	}

	//Os__xprinta(Buffer);
	//FillWithZero(Buffer,6); //commenter par larbi 12042019
//	Os__xprinta(Buffer);
//	memcpy(GLB_STAN_ANN,Buffer,6);
//	Os__xprinta(Buffer);
    status=cherchTraSTAN(Buffer,&indice);
	if(status!=OK)
	{
		memset(&E_Transac8583,0x00,sizeof(E_Transac8583));
		memset(&R_Transac8583,0x00,sizeof(R_Transac8583));
		memset(&StructInfoPayment ,0x00,sizeof(StructInfoPayment));
		return KO;
	}

	if((StructInfoPayment.StatusAnnulAdjust)||(StructInfoPayment.CodeFunction==VOIDCODE)){
		//TRANSACTION ANNULEE
		CUMORE_CMI_AFFICHE_MESSAGE(7,9,1000);
		memset(&E_Transac8583,0x00,sizeof(E_Transac8583));
		memset(&R_Transac8583,0x00,sizeof(R_Transac8583));
		memset(&StructInfoPayment ,0x00,sizeof(StructInfoPayment));
		return KO ;
	}
	if(StructInfoPayment.CodeFunction==REFUNDCODE){
		//ANNULATION IMPOSSIBLE
		CUMORE_CMI_AFFICHE_MESSAGE(2,129,1000);
		memset(&E_Transac8583,0x00,sizeof(E_Transac8583));
		memset(&R_Transac8583,0x00,sizeof(R_Transac8583));
		memset(&StructInfoPayment ,0x00,sizeof(StructInfoPayment));
		return KO ;
	}

	gindice=indice;
	memcpy(g_numero_carte,StructInfoPayment.Holder_Card,StructInfoPayment.Holder_Card[0]+1);

	StructInfoPayment.CodeFunction = VOIDCODE ;

	return OK ;
}

uchar AnnulTraSuite()
{
	uchar			Buffer[7];
	uchar			update_param[6];
	uchar			update_date[11] ;
	uchar			affiche_mont[16],affiche_date[16+1],montant_tmp[13],montant_ppad[13];
	uchar			numero_carte[20];
	uchar			num_aut[7];
	uchar			date_time[13];
	uchar			date_tra[13];
	uchar			Date_sys[7];
	uchar			Time_sys[7];
	Ushort   		indice=0;
	uchar			status ;
	uchar			l_retour;

	uchar			Affich_ligne1[16+1];

	uchar	Date_YYMMDDHHmmss[13];
	int h_clock=0, yr, mth, day, hr, min, sec, wday;
	DATE date;

	memset(Buffer,0,7);
	memset(num_aut,0,7);

	memset(affiche_mont,0,14);
	memset(numero_carte,0,20);
	memset(date_time,0,13);
	memset(date_tra,0,13);
	memset(Date_sys,0,7);
	memset(Time_sys,0,7);




	if(memcmp(g_numero_carte+1,StructInfoPayment.Holder_Card+1,StructInfoPayment.Holder_Card[0]))
	{
		if(g_trace)Os__xprint("Numero Carte Invalide");
		CUMORE_CMI_AFFICHE_MESSAGE(179,16,1000);
		memset(&E_Transac8583,0x00,sizeof(E_Transac8583));
		memset(&R_Transac8583,0x00,sizeof(R_Transac8583));
		memset(&StructInfoPayment ,0x00,sizeof(StructInfoPayment));
		return KO;
	}


	// R?cup?ration de la date/heure de la transaction
	memset(Date_sys,'\0',7);
	memset(Time_sys,'\0',7);

	//affichage sur ecran
	sprintf((PC)affiche_date,"%.2s/%.2s/%.2s   %.2s:%.2s",
		StructInfoPayment.Transaction_DateTime+4,
		StructInfoPayment.Transaction_DateTime+2,
		StructInfoPayment.Transaction_DateTime,
		StructInfoPayment.Transaction_DateTime+6,
		StructInfoPayment.Transaction_DateTime+8);

	memset(Affich_ligne1,0,sizeof(Affich_ligne1));

	if (StructInfoPayment.CodeFunction == PAIEMENT_POURBOIRE
		|| (StructInfoPayment.CodeFunction == VOIDCODE && StructInfoPayment.MontantPourboireInterchange != 0 )
		|| (StructInfoPayment.StatusAnnulAdjust && StructInfoPayment.MontantPourboireInterchange != 0 )
		) //Pourboire
		sprintf((PC)Affich_ligne1,"MNT:%d,%d%d%s",
					(StructInfoPayment.MontantPourboireInterchange
							+ StructInfoPayment.Transaction_Amount) / 100,
					((StructInfoPayment.MontantPourboireInterchange
							+ StructInfoPayment.Transaction_Amount) / 10) % 10,
					(StructInfoPayment.MontantPourboireInterchange
							+ StructInfoPayment.Transaction_Amount) % 10,
					StructInfoPayment.Devise_acc.code_alph);
	else
		sprintf((PC)Affich_ligne1,"MNT:%d,%d%d%s",
				StructInfoPayment.Transaction_Amount/100,
				(StructInfoPayment.Transaction_Amount/10)%10,
				StructInfoPayment.Transaction_Amount%10,
				StructInfoPayment.Devise_acc.code_alph);

	if(ValiderMess(Affich_ligne1,affiche_date)!=KEY_ENTER)
	{
		memset(&E_Transac8583,0x00,sizeof(E_Transac8583));
		memset(&R_Transac8583,0x00,sizeof(R_Transac8583));
		memset(&StructInfoPayment ,0x00,sizeof(StructInfoPayment));
		return KO;
	}
//	char title[200];
//	sprintf(title,"%s\n%s",affiche_date,Affich_ligne1);
	//MessageWithConfirmation(title);
	//ValiderMess_(title);
//	if (ValiderMess_(title) != KEY_ENTER) {
//		memset(&E_Transac8583, 0x00, sizeof(E_Transac8583));
//		memset(&R_Transac8583, 0x00, sizeof(R_Transac8583));
//		memset(&StructInfoPayment, 0x00, sizeof(StructInfoPayment));
//		return KO ;
//	}


//	if (strlen(title) > 0) {
//		//		if(ValiderMess(Message_confmontant,"")!=KEY_ENTER)
//		//			return KO;
//		MessageWithConfirmation(title);
//
//		//APEMV_UI_InfoDisplay((char *)Message_confmontant);
//
//		while (1) {
//			if (Os__xget_key(FicPara.TimeOut * 6) != KEY_CLEAR) {
//				//Os__xprinta("11111");
//				return OK ;
//			}
//			//Os__xprinta("");
//			return KO ;
//		}
//		return KO ;
//	}

	clock2int(h_clock, (unsigned char *)NULL, &yr, &mth, &day, &hr, &min, &sec, &wday);
	sprintf (Date_YYMMDDHHmmss,
		"%.2d%.2d%.2d%.2d%.2d%.2d",
		(yr%100),
		mth,
		day,
		hr,
		min,
		sec);
	memcpy(StructInfoPayment.Transaction_DateTime,Date_YYMMDDHHmmss,12);

	StructInfoPayment.CodeFunction = VOIDCODE;

	//r?cup?ration du message centr?s
	BinPrefixControl(StructInfoPayment.Holder_Card,0);

	read_trans_index( gindice ,&StrLineTra);
	StructInfoPayment.Flag_Dem_Aut=StrLineTra.Flag_Dem_Aut;
	StructInfoPayment.avis=StrLineTra.avis;

	// si la transac offline et non avis?e on ne remonte pas au serveur
	StructInfoPayment.Author_Status = 0xff;
	if((!StructInfoPayment.Flag_Dem_Aut)&&(!StructInfoPayment.avis))
	{
		Prepare_Transaction();

		StrLineTra.StatusAnnulAdjust=1;
		write_trans_index( gindice,&StrLineTra );

		StructInfoPayment.StatusAnnulAdjust =1;
		if(SaveTra() != OK) //FEB06
		{
			strncpy(StructInfoCaisse[1].CRPIN,"481",3);
			return ABORTED;
		}
		strncpy(StructInfoCaisse[1].CRPIN,"000",3);



		ImprimeTra(StructInfoPayment.Langue,COMMERCANT,0);
		DebutLine = GlLine;
		Vprintline_Knife("  ",&DebutLine);
		DebutLine ++;
		ImprimeTra(StructInfoPayment.Langue,CLIENT,0);
	}
	else
	{
		Prepare_Transaction();
		memset(update_param,0,6);
		memset(update_date,0,11);

		//sprintf((char*)StructInfoPayment.Devise_acc.code_num,"%s",StructInfoCaisse[0].DE);//Cless_

		l_retour=DemAutor8583(update_param,update_date);   // demande aut
		StructInfoPayment.Author_Status = l_retour;
		switch(l_retour)
		{
		case OK :
			strncpy(StructInfoCaisse[1].CRPIN,"481",3);
			memcpy(StructInfoPayment.Approval_Code,R_Transac8583.champs38,6);

			MessageWithDelay("Annulation accept?e", "file://flash/HOST/Approuvee.png", 1000); // jrizki 200319

			StrLineTra.StatusAnnulAdjust=1;
			write_trans_index( gindice,&StrLineTra ) ;

			if(SaveTra() != OK)
			{
				strncpy(StructInfoCaisse[1].CRPIN,"481",3);
				return ABORTED;
			}

			strncpy(StructInfoCaisse[1].CRPIN,"000",3);

			StructInfoPayment.StatusAnnulAdjust=1;
			StructInfoPayment.Flag_Dem_Aut =1;
			StructInfoPayment.Top_Forcage = TOP_FOR_ONLINE;

			ImprimeTra(StructInfoPayment.Langue,COMMERCANT,0);
			MessageWithWaitForAction("Couper ticket", "file://flash/HOST/couper_ticket.png", 10000);
			DebutLine = GlLine;
			Vprintline_Knife("  ",&DebutLine);
			DebutLine ++;
			ImprimeTra(StructInfoPayment.Langue,CLIENT,0);
			detectCard();
			break;

		case CAPTURE_CARTE:
		case REFERRAL:
		case NON_AUTOR:
			StructInfoPayment.Flag_Dem_Aut = 0;
			hang_up(typehangup,g_trace);
			strncpy(StructInfoCaisse[1].CRPIN,"481",3);
			break;
		case CONNEXION_KO:
		case FORMAT_KO :
		case EMISSION_KO :
			StructInfoPayment.Flag_Dem_Aut = 0;
			hang_up(typehangup,g_trace);
			strncpy(StructInfoCaisse[1].CRPIN,"481",3);
			break;
		case RECEPT_KO :
		case FORMAT_MSG_REC_KO:
			StructInfoPayment.Flag_Dem_Aut = 0;
			hang_up(typehangup,g_trace);
			strncpy(StructInfoCaisse[1].CRPIN,"481",3);
			break;
		}
	}
	Sequence(l_retour,status);

	memset(&E_Transac8583,0x00,sizeof(E_Transac8583));
	memset(&R_Transac8583,0x00,sizeof(R_Transac8583));
	memset(&StructInfoPayment ,0x00,sizeof(StructInfoPayment));
	//strncpy(StructInfoCaisse[1].CRPIN,"000",3);
	return OK ;
}
/****************************************************************************
*                                                                           *
*      SOUS-PROGRAMME :   AnnulTraPuce                                      *
*                                                                           *
*****************************************************************************
*                                                                           *
*      FONCTION :     Annuler une transaction                               *
*                                                                           *
*      ENTREE   :  Rien                                                     *
*                                                                           *
*      SORTIE   :  Rien                                                     *
*                                                                           *
****************************************************************************/
uchar AnnulTraPuce()
{
	uchar			Buffer[7];
	uchar			update_param[6];
	uchar			update_date[11];
	uchar			affiche_mont[16],affiche_date[16+1],montant_tmp[13];
	uchar			num_aut[7];
	uchar			date_time[13];
	uchar			date_tra[13];
	uchar			Date_sys[7];
	uchar			Time_sys[7];
	uchar			status=0;
	uchar			l_retour;
	uchar			Affich_ligne1[16+1];
	uchar			Exp;
	uchar			Date_YYMMDDHHmmss[13];
	DATE			date;
	
	memset(Buffer,0,7);
	memset(num_aut,0,7);

	memset(affiche_mont,0,14);
	memset(date_time,0,13);
	memset(date_tra,0,13);
	memset(Date_sys,0,7);
	memset(Time_sys,0,7);

	// Coherence de num?ro de carte
	if(memcmp(g_numero_carte,StructInfoPayment.Holder_Card,StructInfoPayment.Holder_Card[0]+1))
	{
		if(g_trace)Os__xprint("Numero Carte Invalide");
		CUMORE_CMI_AFFICHE_MESSAGE(179,16,1000);
		return KO;
	}

	//affichage sur ecran 
	sprintf((PC)affiche_date,"%.2s/%.2s/%.2s   %.2s:%.2s",
		StructInfoPayment.Transaction_DateTime+4,
		StructInfoPayment.Transaction_DateTime+2,
		StructInfoPayment.Transaction_DateTime,
		StructInfoPayment.Transaction_DateTime+6,
		StructInfoPayment.Transaction_DateTime+8);
	
	long_asc (montant_tmp, 12, &StructInfoPayment.Transaction_Amount);
	RemoveLeftZeros(montant_tmp);
	Exp = StructInfoPayment.Devise_acc.Exposant-48;
	FormatAmount(montant_tmp,affiche_mont,Exp);
	
	memset(Affich_ligne1,0,sizeof(Affich_ligne1));
	sprintf((PC)Affich_ligne1,"MNT:%9s%.3s",affiche_mont,StructInfoPayment.Devise_acc.code_alph);
	//if(ValiderMess(Affich_ligne1,affiche_date)==KEY_CLEAR)
	if(ValiderMess(Affich_ligne1,affiche_date)!=KEY_ENTER)
		return KO;

	// R?cup?ration de la date/heure de la transaction
	memset(Date_sys,'\0',7);
	memset(Time_sys,'\0',7);

	Telium_Read_date(&date);
	sprintf ((char*)Date_YYMMDDHHmmss,
		"%.2s%.2s%.2s%.2s%.2s%.2s",
		date.year,
		date.month,
		date.day,
		date.hour,
		date.minute,
		date.second);
	memcpy(StructInfoPayment.Transaction_DateTime,Date_YYMMDDHHmmss,12);

	/*Os__read_date(Date_sys);
	memcpy(StructInfoPayment.Transaction_DateTime,&Date_sys[4],2);
	memcpy(StructInfoPayment.Transaction_DateTime+2,&Date_sys[2],2);
	memcpy(StructInfoPayment.Transaction_DateTime+4,&Date_sys[0],2);
	Os__read_time_sec (Time_sys) ;
	memcpy(StructInfoPayment.Transaction_DateTime+6,Time_sys,6)*/;
	StructInfoPayment.CodeFunction = VOIDCODE;
	
	//r?cup?ration du message centr?s
	BinPrefixControl(StructInfoPayment.Holder_Card,0);
	read_trans_index( gindice ,&StrLineTra);
	StructInfoPayment.Flag_Dem_Aut=StrLineTra.Flag_Dem_Aut;
	StructInfoPayment.avis=StrLineTra.avis;
	// si la transac offline et non avis?e on ne remonte pas au serveur
	StructInfoPayment.Author_Status = 0xff;
	if((!StructInfoPayment.Flag_Dem_Aut)&&(!StructInfoPayment.avis))
	{
		Prepare_Transaction();
		StrLineTra.StatusAnnulAdjust=1 ;
		StructInfoPayment.StatusAnnulAdjust =1;
		if(ImprimeTra(FicPara.Langue,COMMERCANT,0) == OK)
		{
			write_trans_index( gindice,&StrLineTra );
			SaveTra() ;
			ImprimeTra(StructInfoPayment.Langue,CLIENT,0);

		}
	}
	else
	{
		Prepare_Transaction();
		memset(update_param,0,6);
		memset(update_date,0,11);
		l_retour=DemAutor8583(update_param,update_date);   // demande aut
		StructInfoPayment.Author_Status = l_retour;
		switch(l_retour)
		{
		case OK :
			memcpy(StructInfoPayment.Approval_Code,R_Transac8583.champs38,6);
			if(ImprimeTra(FicPara.Langue,COMMERCANT,0) == OK)
			{
				StrLineTra.StatusAnnulAdjust=1 ;
				StructInfoPayment.StatusAnnulAdjust =1;
				StructInfoPayment.Flag_Dem_Aut =1;
				StructInfoPayment.Top_Forcage = TOP_FOR_ONLINE;
				write_trans_index( gindice,&StrLineTra );
				SaveTra();
				ImprimeTra(StructInfoPayment.Langue,CLIENT,0);

				// Demande teleparametrage si n?cessaire
				gest_param();
			}
			break;
		case CAPTURE_CARTE:
		case REFERRAL:
		case NON_AUTOR:
			StructInfoPayment.Flag_Dem_Aut = 0;
			CUMORE_CMI_AFFICHE_MESSAGE(138,127,1000);
			//hang_up(0,g_trace);
			break;
		case CONNEXION_KO:
		case FORMAT_KO :
		case EMISSION_KO :
			StructInfoPayment.Flag_Dem_Aut = 0;
			CUMORE_CMI_AFFICHE_MESSAGE(138,127,1000);
			//hang_up(0,g_trace);
			break;
		case RECEPT_KO :
		case FORMAT_MSG_REC_KO:
			StructInfoPayment.Flag_Dem_Aut = 0;
			CUMORE_CMI_AFFICHE_MESSAGE(138,127,1000);
			//hang_up(0,g_trace);
			break;
		}
	}
	Sequence(l_retour,status);
	return OK ;
}
/****************************************************************************
*                                                                           *
*      SOUS-PROGRAMME :   duplicata                                         *
*                                                                           *
*****************************************************************************
*                                                                           *
*      FONCTION :     Impression de la duplicata                            *
*                                                                           *
*      ENTREE   :  Rien                                                     *
*                                                                           *
*      SORTIE   :  status                                                   *
*                                                                           *
****************************************************************************/
uchar duplicata(uchar LastTrs)
{
	uchar			Buffer[7];
	Ushort   		indice=0;
	uchar			status;
	lignetra		transac;
	
	memset(Buffer,0,7);
	memset(&StructInfoPayment ,0x00,sizeof(StructInfoPayment));
	StructInfoPayment.puce_out = 1;
	memset((uchar*)&transac,0,sizeof(transac));
	StructInfoPayment.CodeFunction = DUPLICATECODE;
	if(LastTrs)
	{
		if(FicTra.nombre_tra>0)
			sprintf((PC)Buffer,"%06d",FicTra.nombre_tra);
		else
			memcpy(Buffer,"000000",6);
		status=cherchTra(Buffer,&indice);
		if(status!=OK)
		{
			strncpy(StructInfoCaisse[1].CRPIN,"381",3);
			return KO;
		}
	}
	else
	{
//		CUMORE_CMI_AFFICHE_MESSAGE(5,138,1000);
//		Aff_mess(FicPara.Langue,5,0,0,0,0) ;

		if(ucGetField(175,"DUPLICATA TRX",Buffer,7,1,6,"     ",30000,0)!=OK)
		{
			strncpy(StructInfoCaisse[1].CRPIN,"381",3);
			return KO ;
		}
		//memcpy(Buffer,StructInfoCaisse[0].STAN,6);
		//FillWithZero(Buffer,6); //commenter par larbi 26022019
		status=cherchTraSTAN(Buffer,&indice);
		if(status!=OK)
		{
			strncpy(StructInfoCaisse[1].CRPIN,"381",3);
			return KO;
		}
	}
	
	read_trans_index( indice ,&transac);
	
	//r?cup?ration du message centr?s
	BinPrefixControl(StructInfoPayment.Holder_Card,0);
	
	if(		(StructInfoPayment.CodeFunction == PREAUTHCOMPCODE)
		&&	(StructInfoPayment.Top_Forcage == TOP_FOR_ONLINE)
	)
	{
		//CUMORE_CMI_AFFICHE_MESSAGE(48,129,1000);

		sprintf((PC)Buffer,"%06d",indice);
		cherchTra(Buffer,&indice);
		read_trans_index( indice+1 ,&transac);
		
		if(ImprimeTraComp(FicPara.Langue,COMMERCANT,1,&transac) != OK)
			return KO;
		Valider_Transac();
		if(ImprimeTraComp(StructInfoPayment.Langue,CLIENT,1,&transac) != OK)
			return KO;
	}
	else if(	(StructInfoPayment.CodeFunction == PREAUTHCOMPCODE) &&
		(StructInfoPayment.Preauto_Amount_Final != StructInfoPayment.Transaction_Amount)
	)
	{

		read_trans_index( indice+1 ,&transac);
		
		if(ImprimeTraComp(FicPara.Langue,COMMERCANT,1,&transac) != OK)
			return KO;
		if(ImprimeTraComp(StructInfoPayment.Langue,CLIENT,1,&transac) != OK)
			return KO;
	}
	else
	{
		/*
		if(ImprimeTra(FicPara.Langue,COMMERCANT,1) != OK)
			return KO;
		Valider_Transac();
		*/
		//if(ImprimeTra(StructInfoPayment.Langue,CLIENT,1) != OK)


		if(ImprimeTra(StructInfoPayment.Langue,COMMERCANT,1) != OK)
		{
			strncpy(StructInfoCaisse[1].CRPIN,"381",3);
			return KO;
		}

		DebutLine = GlLine;
		Vprintline_Knife("  ",&DebutLine);
		DebutLine ++;

		MessageWithWaitForAction("Couper ticket","file://flash/HOST/couper_ticket.png", 10000);



		if(ImprimeTra(StructInfoPayment.Langue,CLIENT,1) != OK)
		{
			strncpy(StructInfoCaisse[1].CRPIN,"381",3);
			return KO;
		}

		detectCard();

		transac.Duplicata=1;
		write_trans_index( indice,&transac );

		if(StructInfoPayment.Entry_Mode == TRS_MAG)
			strncpy(StructInfoCaisse[1].MDLC,"MG",2);
		else if(StructInfoPayment.Entry_Mode == TRS_SCT)
			strncpy(StructInfoCaisse[1].MDLC,"SC",2);
		else if(StructInfoPayment.Entry_Mode == TRS_EMV)
			strncpy(StructInfoCaisse[1].MDLC,"CO",2);
	}

	return OK;
}
uchar duplicata01(uchar LastTrs)
{
	uchar			Buffer[7];
	Ushort   		indice=0;
	uchar			status;
	lignetra		transac;
	
	memset(Buffer,0,7);
	memset(&StructInfoPayment ,0x00,sizeof(StructInfoPayment));
	StructInfoPayment.puce_out = 1;
	memset((uchar*)&transac,0,sizeof(transac));
	StructInfoPayment.CodeFunction = DUPLICATECODE;
	if(LastTrs)
	{
		if(FicTra.nombre_tra>0)
			sprintf((PC)Buffer,"%06d",FicTra.nombre_tra);
		else
			memcpy(Buffer,"000000",6);
		status=cherchTra(Buffer,&indice);
		if(status!=OK)
			return KO;
	}
	else
	{
		CUMORE_CMI_AFFICHE_MESSAGE(5,138,1000);
		//Aff_mess(FicPara.Langue,5,0,0,0,0) ;
		if(ucGetField(175,"",Buffer,7,1,6,"     ",30000,0)!=OK)
			return KO ;

		//FillWithZero(Buffer,6); //commenter par larbi 12042019
		status=cherchTraSTAN(Buffer,&indice);
		if(status!=OK)
			return KO;
	}
	
	read_trans_index( indice ,&transac);
	
	//r?cup?ration du message centr?s
	BinPrefixControl(StructInfoPayment.Holder_Card,0);
	
	if(ImprimeTra(FicPara.Langue,COMMERCANT,1) != OK)
		return KO;
	if(ImprimeTra(StructInfoPayment.Langue,CLIENT,1) != OK)
		return KO;
	transac.Duplicata=1;
	write_trans_index( indice,&transac ) ;
	if(	(StructInfoPayment.CodeFunction == PREAUTHCOMPCODE) &&
		(StructInfoPayment.Preauto_Amount_Final != StructInfoPayment.Transaction_Amount)
	)
	{
		if(StructInfoPayment.Top_Forcage == TOP_FOR_ONLINE)
			sprintf((PC)Buffer,"%06d",indice);
		else
			sprintf((PC)Buffer,"%06d",indice+2);
		
		status=cherchTra(Buffer,&indice);
		if(status!=OK)
			return KO ;
		read_trans_index( indice ,&transac);
		
		if(ImprimeTra(FicPara.Langue,COMMERCANT,1) != OK)
			return KO;
		if(ImprimeTra(StructInfoPayment.Langue,CLIENT,1) != OK)
			return KO;
		transac.Duplicata=1;
		write_trans_index( indice,&transac);
	}
	return OK;
}
/****************************************************************************
*                                                                           *
*      SOUS-PROGRAMME :   duplicPre                                         *
*                                                                           *
*****************************************************************************
*                                                                           *
*      FONCTION :     Impression de la duplicata de la pre autorisation     *
*                                                                           *
*      ENTREE   :  Rien                                                     *
*                                                                           *
*      SORTIE   :  status                                                   *
*                                                                           *
****************************************************************************/
uchar duplicPre(uchar LastTrs)
{
	uchar			Buffer[7];
	Ushort   		indice=0;
	uchar			status;
	lignetra		transac;
	memset(Buffer,0,7);
	memset(&StructInfoPayment ,0x00,sizeof(StructInfoPayment));
		StructInfoPayment.puce_out = 1;
		memset((uchar*)&transac,0,sizeof(transac));
	StructInfoPayment.CodeFunction = DUPLICATECODE;
	if(LastTrs)
	{
		if(FicPreauth.nombre_tra>0)
		{
			indice = FicPreauth.nombre_tra-1;
			read_preauto_index(indice,&StrPreAut);
			memcpy(Buffer,StrPreAut.Num_sequence,6);
		}
		else
			memcpy(Buffer,"000000",6);
		status=CherchPreAuto(Buffer,&indice);
		if(status!=OK)
			return KO;
	}
	else
	{
		//CUMORE_CMI_AFFICHE_MESSAGE(5,138,1000);
		//Aff_mess(FicPara.Langue,5,0,0,0,0) ;
		if(ucGetField(175,"DUPLICATA PRE AUTO",Buffer,7,1,6,"     ",30000,0)!=OK)
			return KO ;
		//FillWithZero(Buffer,6); //commenter par larbi 12042019
		status=CherchPreAuto_STAN(Buffer,&indice);
		if(status!=OK)
			return KO;
	}
	read_preauto_index( indice ,&StrPreAut);
	StructInfoPayment.Transaction_Amount=StrPreAut.Transaction_Amount;
	memcpy(StructInfoPayment.STAN,StrPreAut.STAN,6);
  //r?cup?ration du message centr?s
	BinPrefixControl(StructInfoPayment.Holder_Card,0);
	if(ImprimeTra(FicPara.Langue,COMMERCANT,1) != OK)
		return KO;
	status = Valider_Transac();//belhadeg
	if(status == KEY_ENTER)
	if(ImprimeTra(StructInfoPayment.Langue,CLIENT,1) != OK)
	return KO;
	StrPreAut.Duplicata=1;
	write_preauto_index( indice,&StrPreAut ) ;
	return OK ;
}
/****************************************************************************
*                                                                           *
*      SOUS-PROGRAMME :   cherchTra                                         *
*                                                                           *
*****************************************************************************
*                                                                           *
*      FONCTION :     Se pointer sur la transaction ? annuler               *
*                                                                           *
*      ENTREE   :  la trace de la transaction ,sa position dans             *
*							le fichier(param?tre output)                    *
*                                                                           *
*      SORTIE   :  status(OK si trouver)                                    *
*                                                                           *
****************************************************************************/
uchar cherchTra(uchar *Buffer,Ushort *indice)
{
	Ushort		l_count,nbr_tra	;
	uchar		k;
	uchar    	iso2[38];
	uchar		j;
	lignetra	transac ;
	
	uint8		ConvertBuffer[40],Chaine[7];
	uint16		Offset;
	uint16		Length;
	uint16		Index_trs;
	uint8		dLength;

	l_count=0;
	nbr_tra=FicTra.nombre_tra ;

	memset(Chaine,0,7);
	memcpy(Chaine,Buffer,6);
	Index_trs = atol((PC)Chaine)-1;
	if((nbr_tra==0)||(Index_trs>=nbr_tra))
	{
		//TRANSACTION INEXISTANTE
		CUMORE_CMI_AFFICHE_MESSAGE(7,8,1000);
		//Mess_delay(7,8,0,100,0);
    	return KO ;
	}
	read_trans_index( Index_trs ,&transac) ;
	*indice = Index_trs ;

	if(StructInfoPayment.CodeFunction == VOIDCODE)
	{
		if((transac.StatusAnnulAdjust)||(transac.CodeFunction==VOIDCODE)){
			//TRANSACTION ANNULEE
			CUMORE_CMI_AFFICHE_MESSAGE(7,9,1000);
			//Mess_delay(7,9,0,300,0);
			return KO ;
		}
		if(transac.CodeFunction==REFUNDCODE){
			//ANNULATION IMPOSSIBLE
			CUMORE_CMI_AFFICHE_MESSAGE(2,129,1000);
			//Mess_delay(2,129,0,300,0);
			return KO ;
		}
	}
	//*indice=l_count ;

	memcpy(StructInfoPayment.Num_point_vente,transac.Num_point_vente,strlen((PC)transac.Num_point_vente));
	StructInfoPayment.Preauto_Amount_Final=transac.Preauto_Amount_Final ;
	StructInfoPayment.Preauto_indic_completion=transac.Preauto_indic_completion ;
	StructInfoPayment.Transaction_Amount=transac.Transaction_Amount ;
	StructInfoPayment.MontantPourboireInterchange=transac.MontantPourboireInterchange ;	//POURBOIRE
	StructInfoPayment.Devise_acc.Exposant=transac.Exposant ;
	StructInfoPayment.Type_carte=transac.Type_carte ;
	StructInfoPayment.Origine_carte=transac.Origine_carte ;
	StructInfoPayment.Entry_Mode=transac.Entry_Mode;
	StructInfoPayment.Entry_Pin=transac.Entry_Pin;
	StructInfoPayment.StatusAnnulAdjust=transac.StatusAnnulAdjust ;
	StructInfoPayment.CodeFunction=transac.CodeFunction ;
	StructInfoPayment.Flag_Dem_Aut=transac.Flag_Dem_Aut ;
	StructInfoPayment.Top_Forcage=transac.Top_Forcage ;
	StructInfoPayment.avis=transac.avis ;
	StructInfoPayment.Langue=transac.Langue ;
	StructInfoPayment.Consolidation_Amount=transac.Consolidation_Amount ;
	// Il faut chercher la devise dont son code num==NumCurrency
	for(k=0;k<NB_MAX_DEV_ACC;k++)
    {
		if(!memcmp(Fic_Devise_Acc.TabDevise[k].code_num,transac.NumCurrency,3))
		{
			memcpy((uchar*)&StructInfoPayment.Devise_acc,
			(uchar*)&Fic_Devise_Acc.TabDevise[k],
			sizeof(StructInfoPayment.Devise_acc));
			break;
		}
    }

	//memcpy(StructInfoPayment.Devise_acc.code_num,transac.NumCurrency,3);
	memcpy(StructInfoPayment.Transaction_DateTime,transac.Transaction_DateTime,12);
	memcpy(StructInfoPayment.Compressed_Iso2,transac.Compressed_Iso2,20);
	/*****/
/*	ShowTrace_Mouna("ad6");
	imp_buffer(StructInfoPayment.Compressed_Iso2,StructInfoPayment.Compressed_Iso2[0]);
	bcd_asc(iso2,&StructInfoPayment.Compressed_Iso2[1],20);
	imp_buffer(iso2,20);*/
	/*****/

	if(transac.Entry_Mode == TRS_MAN)
 	{
 		memcpy(StructInfoPayment.Holder_Card,transac.Compressed_Iso2,20);
 	}
 	else if(transac.Entry_Mode == TRS_SCT)
 	{
 		sprintf(iso2,"%s",&transac.Compressed_Iso2[1]);

 		//for(j=0;((j<20)&&(iso2[j] !='d')&&(iso2[j] !='D')&&(iso2[j] !='='));j++);
 		for(j=0;((j<19)&&(iso2[j] !='d')&&(iso2[j] !='D')&&(iso2[j] !='='));j++);//badr PAN CARD = 19
		memcpy(&StructInfoPayment.Holder_Card[1],iso2,j);
		StructInfoPayment.Holder_Card[0] = j;
		if(j==19)
			memcpy(StructInfoPayment.SeviceCode,transac.SeviceCode,3);
		else {
		j += 5 ; // Expired Date
		memcpy(StructInfoPayment.SeviceCode,&iso2[j],3);
		}
 	}
 	else
 	{
		bcd_asc(iso2,&transac.Compressed_Iso2[1],28);

		for(j=0;((j<20)&&(iso2[j] !='d')&&(iso2[j] !='D')&&(iso2[j] !='='));j++);
		memcpy(&StructInfoPayment.Holder_Card[1],iso2,j);
		StructInfoPayment.Holder_Card[0] = j;
		j += 5 ; // Expired Date
		memcpy(StructInfoPayment.SeviceCode,&iso2[j],3);
	}

	/*
 	if(transac.Entry_Mode == TRS_MAN)
 	{
 		memcpy(StructInfoPayment.Holder_Card,transac.Compressed_Iso2,20);
 	}
 	else
 	{
		bcd_asc(iso2,&transac.Compressed_Iso2[1],28);
		for(j=0;((j<20)&&(iso2[j] !='d')&&(iso2[j] !='D')&&(iso2[j] !='='));j++);
		memcpy(&StructInfoPayment.Holder_Card[1],iso2,j);
		StructInfoPayment.Holder_Card[0] = j;
		j += 5 ; // Expired Date 
		memcpy(StructInfoPayment.SeviceCode,&iso2[j],3);
	}
 	*/

	//memcpy(StructInfoPayment.Code_pt_service,transac.Code_pt_service,12);
	memcpy(StructInfoPayment.Expired_Date,transac.Expired_Date,4);
	memcpy(StructInfoPayment.Appli_aid,transac.Appli_aid,34);
	memcpy(StructInfoPayment.Appli_label,transac.Appli_label,16);
	memcpy(StructInfoPayment.Nom_Prenom,transac.Nom_Prenom,27);
	memcpy(StructInfoPayment.Approval_Code,transac.Approval_Code,6);
	memcpy(StructInfoPayment.Proc_Code,transac.Proc_Code,6);
	memcpy(StructInfoPayment.STAN,transac.STAN,6);//modif 04/04
	//Response_Time
	//Response_Date 
	//memcpy(StructInfoPayment.Trace_Audit_Number,transac.Trace_Audit_Number,6);
	memcpy(StructInfoPayment.Num_sequence,transac.Num_sequence,6);
	memcpy(StructInfoPayment.NumCurrency_cons,transac.NumCurrency_cons,3);
	if(transac.Entry_Mode == TRS_EMV)//CCD
	{
		StructInfoPayment.l_champ55 = transac.Lg_EmvData;
		memcpy(StructInfoPayment.Champ55,transac.EmvData,transac.Lg_EmvData);
		if ((FindTag (TAG_APPLI_CRYPTOGRAMME_AC, &Offset, &Length,transac.EmvData, transac.Lg_EmvData) == OK) &&
				(Length == 8))//SEMV_TAG_9F26_APPLI_CRYPTO
		{
			memset (ConvertBuffer, 0, 8);
			if (memcmp (&transac.EmvData[Offset], ConvertBuffer, 8) != 0)
			{
				dLength=(UC)Length * 2;
				hex_str (ConvertBuffer, &transac.EmvData[Offset], dLength);
				memcpy(StructInfoPayment.Certif,ConvertBuffer, dLength);
				ShowTrace("COPIE FAIT");
				//Printtampon(transac.EmvData,transac.Lg_EmvData);
			}
		}
	}
	return OK;
}

uchar cherchTraSTAN(uchar *Buffer,Ushort *indice)
{
	Ushort		nbr_tra;
	uchar		k;
	uchar    	iso2[38];
	uchar		j;
	lignetra	transac;
	
	uint8		ConvertBuffer[40],Chaine[7];
	uint16		Offset;
	uint16		Length;
	uint16		Index_trs;
	uint8		dLength;
	uint8		kk;

	nbr_tra=FicTra.nombre_tra;

	memset(Chaine,0,7);
	memcpy(Chaine,Buffer,6);
	
	kk=0;
	for(Index_trs = 0;Index_trs < nbr_tra;Index_trs ++)
	{
		read_trans_index( Index_trs ,&transac);
/*
		if(memcmp(Buffer,transac.STAN,6) == 0)
			break;
*/
		if(memcmp(Buffer,transac.STAN,6) == 0)
		{
			//MessageWithDelay("0000", NULL, 1000);

//			if(
//				(kk==0)
//				&&(transac.StatusAnnulAdjust)
//				&&(StructInfoPayment.CodeFunction == DUPLICATECODE)
//			)
//			{
//				MessageWithDelay("1111111", NULL, 1000);
//
//				kk=1;
//			}
//			else
				break;
		}
	}
	if(Index_trs == nbr_tra)
	{
		//MessageWithDelay("222222", NULL, 1000);

		//TRANSACTION INEXISTANTE
		CUMORE_CMI_AFFICHE_MESSAGE(7,8,1000);
		//Mess_delay(7,8,0,100,0);
		strncpy(StructInfoCaisse[0].CRPIN,"481",3);
		strncpy(StructInfoCaisse[1].CRPIN,"481",3);
    	return KO;
	}
	*indice = Index_trs;

//	if(StructInfoPayment.CodeFunction == VOIDCODE)
//	{
//		if((transac.StatusAnnulAdjust)||(transac.CodeFunction==VOIDCODE)){
//			//TRANSACTION ANNULEE
//			CUMORE_CMI_AFFICHE_MESSAGE(7,9,1000);
//			strncpy(StructInfoCaisse[0].CRPIN,"482",3);
//			strncpy(StructInfoCaisse[1].CRPIN,"482",3);
//			//Mess_delay(7,9,0,300,0);
//			return KO ;
//		}
//		if(transac.CodeFunction==REFUNDCODE){
//			//ANNULATION IMPOSSIBLE
//			CUMORE_CMI_AFFICHE_MESSAGE(2,129,1000);
//			strncpy(StructInfoCaisse[0].CRPIN,"481",3);
//			strncpy(StructInfoCaisse[1].CRPIN,"481",3);
//			//Mess_delay(2,129,0,300,0);
//			return KO ;
//		}
//	}
	//*indice=l_count ;

	memcpy(StructInfoPayment.Num_point_vente,transac.Num_point_vente,strlen((PC)transac.Num_point_vente));
	StructInfoPayment.Preauto_Amount_Final=transac.Preauto_Amount_Final ;
	StructInfoPayment.Preauto_indic_completion=transac.Preauto_indic_completion ;
	StructInfoPayment.Transaction_Amount=transac.Transaction_Amount ;
	StructInfoPayment.MontantPourboireInterchange=transac.MontantPourboireInterchange ;	//POURBOIRE
	StructInfoPayment.Devise_acc.Exposant=transac.Exposant ;
	StructInfoPayment.Type_carte=transac.Type_carte ;
	StructInfoPayment.Origine_carte=transac.Origine_carte ;
	StructInfoPayment.Entry_Mode=transac.Entry_Mode;
	StructInfoPayment.Entry_Pin=transac.Entry_Pin;
	StructInfoPayment.StatusAnnulAdjust=transac.StatusAnnulAdjust ;
	StructInfoPayment.CodeFunction=transac.CodeFunction ;
	StructInfoPayment.Flag_Dem_Aut=transac.Flag_Dem_Aut ;
	StructInfoPayment.Top_Forcage=transac.Top_Forcage ;
	StructInfoPayment.avis=transac.avis ;
	StructInfoPayment.Langue=transac.Langue ;
	StructInfoPayment.Consolidation_Amount=transac.Consolidation_Amount ;
	
	// Il faut chercher la devise dont son code num==NumCurrency
	for(k=0;k<NB_MAX_DEV_ACC;k++)
    {
		if(!memcmp(Fic_Devise_Acc.TabDevise[k].code_num,transac.NumCurrency,3))
		{
			memcpy((uchar*)&StructInfoPayment.Devise_acc,
			(uchar*)&Fic_Devise_Acc.TabDevise[k],
			sizeof(StructInfoPayment.Devise_acc));
			break;
		}
    }

	//memcpy(StructInfoPayment.Devise_acc.code_num,transac.NumCurrency,3);
	memcpy(StructInfoPayment.Transaction_DateTime,transac.Transaction_DateTime,12);
	memcpy(StructInfoPayment.Compressed_Iso2,transac.Compressed_Iso2,20);
	/*****/
/*	ShowTrace_Mouna("ad8");
	imp_buffer(StructInfoPayment.Compressed_Iso2,StructInfoPayment.Compressed_Iso2[0]);
	bcd_asc(iso2,&StructInfoPayment.Compressed_Iso2[1],20);
	imp_buffer(iso2,20);*/
	/*****/
 	if(transac.Entry_Mode == TRS_MAN)
 	{
 		memcpy(StructInfoPayment.Holder_Card,transac.Compressed_Iso2,20);
 	}
 	else if(transac.Entry_Mode == TRS_SCT)
 	{
 		sprintf(iso2,"%s",&transac.Compressed_Iso2[1]);

 		//for(j=0;((j<20)&&(iso2[j] !='d')&&(iso2[j] !='D')&&(iso2[j] !='='));j++);
 		for(j=0;((j<19)&&(iso2[j] !='d')&&(iso2[j] !='D')&&(iso2[j] !='='));j++);//badr probl?me d'annulation des transaction avec PAN CARD = 19
		memcpy(&StructInfoPayment.Holder_Card[1],iso2,j);
		StructInfoPayment.Holder_Card[0] = j;
		if(j==19)
			memcpy(StructInfoPayment.SeviceCode,transac.SeviceCode,3);
		else{
			j += 5 ; // Expired Date
			memcpy(StructInfoPayment.SeviceCode,&iso2[j],3);
		}

 	}
 	else
 	{
		bcd_asc(iso2,&transac.Compressed_Iso2[1],28);

		for(j=0;((j<20)&&(iso2[j] !='d')&&(iso2[j] !='D')&&(iso2[j] !='='));j++);
		memcpy(&StructInfoPayment.Holder_Card[1],iso2,j);
		StructInfoPayment.Holder_Card[0] = j;
		j += 5 ; // Expired Date 
		memcpy(StructInfoPayment.SeviceCode,&iso2[j],3);
	}
	//memcpy(StructInfoPayment.Code_pt_service,transac.Code_pt_service,12);
	memcpy(StructInfoPayment.Expired_Date,transac.Expired_Date,4);
	memcpy(StructInfoPayment.Appli_aid,transac.Appli_aid,34);
	memcpy(StructInfoPayment.Appli_label,transac.Appli_label,16);
	memcpy(StructInfoPayment.Nom_Prenom,transac.Nom_Prenom,27);
	memcpy(StructInfoPayment.Approval_Code,transac.Approval_Code,6);
	memcpy(StructInfoPayment.Proc_Code,transac.Proc_Code,6);
	memcpy(StructInfoPayment.STAN,transac.STAN,6);//modif 04/04
	//Response_Time
	//Response_Date
	//memcpy(StructInfoPayment.Trace_Audit_Number,transac.Trace_Audit_Number,6);
	memcpy(StructInfoPayment.Num_sequence,transac.Num_sequence,6);
	memcpy(StructInfoPayment.NumCurrency_cons,transac.NumCurrency_cons,3);
	if(transac.Entry_Mode == TRS_EMV)//CCD
	{
		StructInfoPayment.l_champ55 = transac.Lg_EmvData;
		memcpy(StructInfoPayment.Champ55,transac.EmvData,transac.Lg_EmvData);
		if ((FindTag (TAG_APPLI_CRYPTOGRAMME_AC, &Offset, &Length,transac.EmvData, transac.Lg_EmvData) == OK) &&
				(Length == 8))//SEMV_TAG_9F26_APPLI_CRYPTO
		{
			memset (ConvertBuffer, 0, 8);
			if (memcmp (&transac.EmvData[Offset], ConvertBuffer, 8) != 0)
			{
				dLength=(UC)Length * 2;
				hex_str (ConvertBuffer, &transac.EmvData[Offset], dLength);
				memcpy(StructInfoPayment.Certif,ConvertBuffer, dLength);
				ShowTrace("COPIE FAIT");
			}
		}
	}
	return OK;
}
/****************************************************************************
*                                                                           *
*      SOUS-PROGRAMME :   cherchPreAuto                                     *
*                                                                           *
*****************************************************************************
*                                                                           *
*      FONCTION :     Se pointer sur la pre-autorisation                    *
*                                                                           *
*      ENTREE   :  la trace de la transaction ,sa position dans             *
*							le fichier(param?tre output)                    *
*                                                                           *
*      SORTIE   :  status(OK si trouver)                                    *
*                                                                           *
****************************************************************************/
uchar CherchPreAuto(uchar *Buffer,Ushort *indice)
{
	Ushort		i,l_count,nbr_preauto;
	uchar		k;
	uchar    	iso2[38];
	uchar		j;
	
	uint8		ConvertBuffer[40],Chaine[7];
	uint16		Offset;
	uint16		Length;
	uint8		dLength;
	
	l_count=0;
	nbr_preauto = FicPreauth.nombre_tra ;
	
	memset(Chaine,0,7);
	memcpy(Chaine,Buffer,6);
	if(nbr_preauto==0)
	{
		//TRANSACTION INEXISTANTE
		CUMORE_CMI_AFFICHE_MESSAGE(157,8,1000);
		//Mess_delay(157,8,0,300,0);
		return KO ;
	}
	i=0;
	while(i<nbr_preauto)
	{
		read_preauto_index( i ,&StrPreAut) ;
		if(memcmp(StrPreAut.Num_sequence,Buffer,6)==0)
		{
			*indice = i ;
			break;
		}
		i++;
	}
	if(i==nbr_preauto)
	{
		CUMORE_CMI_AFFICHE_MESSAGE(157,8,1000);
		//Mess_delay(157,8,0,300,0);
		return KO;
	}
	
	memcpy(StructInfoPayment.Num_point_vente,StrPreAut.Num_point_vente,strlen((PC)StrPreAut.Num_point_vente));
	StructInfoPayment.Devise_acc.Exposant=StrPreAut.Exposant;
	StructInfoPayment.Type_carte=StrPreAut.Type_carte ;
	StructInfoPayment.Origine_carte=StrPreAut.Origine_carte ;//modif 04/04
	StructInfoPayment.Entry_Mode=StrPreAut.Entry_Mode;
	StructInfoPayment.Entry_Pin=StrPreAut.Entry_Pin;
	StructInfoPayment.StatusAnnulAdjust=StrPreAut.StatusAnnulAdjust ;
	StructInfoPayment.CodeFunction=StrPreAut.CodeFunction ;
	StructInfoPayment.Flag_Dem_Aut=StrPreAut.Flag_Dem_Aut ;
	StructInfoPayment.Top_Forcage=StrPreAut.Top_Forcage ;
	StructInfoPayment.avis=StrPreAut.avis ;
	StructInfoPayment.Langue=StrPreAut.Langue ;
	StructInfoPayment.Preauto_Amount_Initial=StrPreAut.Transaction_Amount ;
	StructInfoPayment.Consolidation_Amount=StrPreAut.Consolidation_Amount ;
	// Il faut chercher la devise dont son code num==NumCurrency
	for(k=0;k<NB_MAX_DEV_ACC;k++)
	{
		if(!memcmp(Fic_Devise_Acc.TabDevise[k].code_num,StrPreAut.NumCurrency,3))
		{
			memcpy((uchar*)&StructInfoPayment.Devise_acc,(uchar*)&Fic_Devise_Acc.TabDevise[k],
			sizeof(StructInfoPayment.Devise_acc));
			break;
		}
	}
	memcpy(StructInfoPayment.Transaction_DateTime,StrPreAut.Transaction_DateTime,12);
	memcpy(StructInfoPayment.Compressed_Iso2,StrPreAut.Compressed_Iso2,20);
	/*****/
/*	ShowTrace_Mouna("ad10");
	imp_buffer(StructInfoPayment.Compressed_Iso2,StructInfoPayment.Compressed_Iso2[0]);
	bcd_asc(iso2,&StructInfoPayment.Compressed_Iso2[1],20);
	imp_buffer(iso2,20);*/
	/*****/
	if(StrPreAut.Entry_Mode == TRS_MAN)
	{
		memcpy(StructInfoPayment.Holder_Card,StrPreAut.Compressed_Iso2,20);
	}
	else if(StrPreAut.Entry_Mode == TRS_SCT)
	{
		sprintf(iso2,"%s",&StrPreAut.Compressed_Iso2[1]);

		//for(j=0;((j<20)&&(iso2[j] !='d')&&(iso2[j] !='D')&&(iso2[j] !='='));j++);
 		for(j=0;((j<19)&&(iso2[j] !='d')&&(iso2[j] !='D')&&(iso2[j] !='='));j++);//badr probl?me PAN CARD = 19
		memcpy(&StructInfoPayment.Holder_Card[1],iso2,j);
		StructInfoPayment.Holder_Card[0] = j;
		if(j==19)
			memcpy(StructInfoPayment.SeviceCode,StrPreAut.SeviceCode,3);
		else {
			j += 5 ; // Expired Date
			memcpy(StructInfoPayment.SeviceCode,&iso2[j],3);
		}
	}
	else
	{
		/*****/
/*		ShowTrace_Mouna("ad11");
		imp_buffer(StrPreAut.Compressed_Iso2,StrPreAut.Compressed_Iso2[0]);*/
		/*****/
		bcd_asc(iso2,&StrPreAut.Compressed_Iso2[1],28);
		/*****/
/*		imp_buffer(iso2,20);*/
		/*****/
		for(j=0;((j<20)&&(iso2[j] !='d')&&(iso2[j] !='D')&&(iso2[j] !='='));j++);
		memcpy(&StructInfoPayment.Holder_Card[1],iso2,j);
		StructInfoPayment.Holder_Card[0] = j;
		j += 5 ; // Expired Date 
		memcpy(StructInfoPayment.SeviceCode,&iso2[j],3);
	}
	memcpy(StructInfoPayment.Expired_Date,StrPreAut.Expired_Date,4);
	memcpy(StructInfoPayment.Approval_Code,StrPreAut.Approval_Code,6);
	memcpy(StructInfoPayment.Proc_Code,StrPreAut.Proc_Code,6);
	memcpy(StructInfoPayment.Num_sequence,StrPreAut.Num_sequence,6);
	memcpy(StructInfoPayment.NumCurrency_cons,StrPreAut.NumCurrency_cons,3);
	if(StrPreAut.Entry_Mode == TRS_EMV)
	{
		StructInfoPayment.l_champ55 = StrPreAut.Lg_EmvData;
		memcpy(StructInfoPayment.Champ55,StrPreAut.EmvData,StrPreAut.Lg_EmvData);
		if ((FindTag (TAG_APPLI_CRYPTOGRAMME_AC, &Offset, &Length,StrPreAut.EmvData, StrPreAut.Lg_EmvData) == OK) &&
			(Length == 8))//SEMV_TAG_9F26_APPLI_CRYPTO
		{
			memset (ConvertBuffer, 0, 8);
			if (memcmp (&StrPreAut.EmvData[Offset], ConvertBuffer, 8) != 0)
			{
				dLength=(UC)Length * 2;
				hex_str (ConvertBuffer, &StrPreAut.EmvData[Offset], dLength);
				memcpy(StructInfoPayment.Certif,ConvertBuffer, dLength);
			}
		}
	}
	return OK;
}
/*-----------------------------------------------------------------
 * Fonction   :
 * objet      :
 *
 * Parametre  :
 * Retour     :
 *-----------------------------------------------------------------
*/
void	read_trans_index( Ushort Index ,lignetra *Pt_numero )
{
   if (Index<NBMAXTRA)
   {
		memcpy( Pt_numero,(uchar *)&FicTra.enr_tra0[Index], sizeof(lignetra));
   }
}
/*-----------------------------------------------------------------
 * Fonction   :
 * objet      :
 *
 * Parametre  :
 * Retour     :
 *-----------------------------------------------------------------
*/
void	write_trans_index( Ushort Index ,lignetra *Pt_numero )
{
   if (Index<NBMAXTRA)
   {
		Os__saved_copy( (uchar*) Pt_numero, (uchar*)&FicTra.enr_tra0[Index],sizeof(lignetra));
   }
}
/*-----------------------------------------------------------------             
 * Fonction   : 
 * objet      : 
 *              
 * Parametre  : 
 * Retour     : 
 *-----------------------------------------------------------------             
*/
void	read_preauto_index( Ushort Index ,lignePreAut *Pt_numero )
{
   if (Index<NBMAXPREAUT)
   {
		memcpy( Pt_numero,(uchar *)&FicPreauth.enr_pre_aut0[Index], sizeof(lignePreAut));
   }
}
/*-----------------------------------------------------------------             
 * Fonction   : 
 * objet      : 
 *              
 * Parametre  : 
 * Retour     : 
 *-----------------------------------------------------------------             
*/
void	write_preauto_index( Ushort Index ,lignePreAut *Pt_numero )
{
	if (Index<NBMAXPREAUT)
	{
		Os__saved_copy( (uchar*) Pt_numero, (uchar*)&FicPreauth.enr_pre_aut0[Index],sizeof(lignePreAut));
	}
}

/*-----------------------------------------------------------------
 * Fonction   :
 * objet      :
 *
 * Parametre  :
 * Retour     :
 *-----------------------------------------------------------------
*/
uchar	tst_fic_vide(void)
{
	Ushort nbr_tra;

	nbr_tra = FicTra.nombre_tra ;
	if(nbr_tra==0)
		return OK;
	return KO;
}
/*-----------------------------------------------------------------
 * Fonction   :
 * objet      :
 *
 * Parametre  :
 * Retour     :
 *-----------------------------------------------------------------
*/
uchar	tst_fic_plein(void)
{
	Ushort nbr_tra;

	nbr_tra = FicTra.nombre_tra;
	//if(nbr_tra==NBMAXTRA) KI 230106
	if(nbr_tra+1 > NBMAXTRA)
		return OK;
	return KO;
}
/*-----------------------------------------------------------------
 * Fonction   :
 * objet      :
 *
 * Parametre  :
 * Retour     :
 *-----------------------------------------------------------------
*/
uchar	tst_fic_preautor_plein(void)
{
	Ushort nbr_tra;

	nbr_tra = FicPreauth.nombre_tra ;
	//if(nbr_tra==NBMAXPREAUT) KI 230106
	if(nbr_tra+1 > NBMAXPREAUT)
		return OK;
	return KO;
}

/****************************************************************************
*                                                                           *
*      SOUS-PROGRAMME :  Verification_Digits                                *
*                                                                           *
*****************************************************************************
*                                                                           *
*      FONCTION :    Verification des quatres derniers digits               *
*                                                                           *
*      ENTREE   :  Rien                                      				    *
*                                                                           *
*      SORTIE   :  status                                                   *
*                                                                           *
****************************************************************************/
uchar Verification_Digits()
{
/*	uchar	*ptKeyboard;
	uchar	digits[5] ;

	switch (StructInfoPayment.CodeFunction)
	{
	case VOIDCODE:
	case REFUNDCODE:
		ShowTrace("VD0");
		return OK;
	default:
//		ShowTrace("VD1");
		break ;
	}

	if(FicPara.Ctrl_fraude!='1')
	{
		ShowTrace("VD2");
		return OK;
	}
	if(StructInfoPayment.Entry_Mode == TRS_MAN)
	{
//		ShowTrace("VD3");
		return OK;
	}
	ptKeyboard = Os__xget_varkey(142,4);
	if(ptKeyboard[0]==KEY_CLEAR)
	{
		ShowTrace("VD4");
		return ABORTED;
	}
	memcpy(digits,StructInfoPayment.Holder_Card+StructInfoPayment.Holder_Card[0]-3,4);
	if(memcmp(digits,StructInfoPayment.Enter+1,strlen((PC)StructInfoPayment.Enter) -1)!=0)
	{
		ShowTrace("VD5");
		return REFUSED;
	}

//	ShowTrace("VD6");
	return OK;
*/

	uchar *ptKeyboard;
	uchar digits[5];

	switch (StructInfoPayment.CodeFunction) {
	case VOIDCODE:
	case REFUNDCODE:
		ShowTrace("VD0");
		return OK ;
	default:
		//		ShowTrace("VD1");
		break;
	}

	if (FicPara.Ctrl_fraude != '1') {
		ShowTrace("VD2");
		//return OK;//enlev? par Hind SAHEL pour demander les 4 derniers chiffres
	}
	if (StructInfoPayment.Entry_Mode == TRS_MAN) {
		//		ShowTrace("VD3");
		return OK ;
	}
	ptKeyboard = (PUC) Os__xget_varkey(142, 4);
	if (ptKeyboard == KEY_CLEAR) {
		ShowTrace("VD4");
		return ABORTED;
	}
	if (strlen(StructInfoPayment.Enter + 1) == 0) {
		ShowTrace("VDl");
		return ABORTED;
	}

	memcpy(digits,
			StructInfoPayment.Holder_Card + StructInfoPayment.Holder_Card[0]
					- 3, 4);
	if (memcmp(digits, StructInfoPayment.Enter + 1,
			strlen((PC) StructInfoPayment.Enter) - 1) != 0) {
		ShowTrace("VD5");
		return REFUSED;
	}
	//	ShowTrace("VD6");
	return OK ;




















}
/****************************************************************************
*                                                                           *
*      SOUS-PROGRAMME :  PlafondAtteint                                     *
*                                                                           *
*****************************************************************************
*                                                                           *
*      FONCTION :    comparaison du montant avec le cumul                   *
*							verification double transaction                 *
*							chercher l'indice de la premi?re tran offline   *
*										non reconsili?e                     *
*                                                                           *
*      ENTREE   :  montant,num?ro carte                                     *
*                                                                           *
*      SORTIE   :  OK si plafond atteint                                    *
*                                                                           *
****************************************************************************/
uchar	PlafondAtteint(Ulong ul_amount,uchar *Card,Ushort* indices_offline,Ulong *amount_cumul)
{
	Ushort			l_l_count =	0;
	static Ushort	nombre_tra;
	Ushort			k=0;
	long			l_l_amount;
	uchar			chaine[40];
	uchar			Saved_Card_Nb[20];
	uchar			j;
	lignetra		transac;

	l_l_amount = 0;
	nombre_tra= FicTra.nombre_tra ;
	while(l_l_count<nombre_tra)
	{
		memset((uchar*)&transac,0x00,sizeof(transac)) ;
		read_trans_index( l_l_count ,&transac) ;
		if((k<FicPara.Max_avis)&&(k<MAX_AVIS))
		{
			if(	(memcmp(transac.Message_Type,"1200",4) != 0) && //CCD
				(!transac.Flag_Dem_Aut)	&&
				(!transac.avis)	&&
				(!transac.StatusAnnulAdjust)
			)
			{
				indices_offline[k+1]=l_l_count;
				k++;
			}
		}
		if(transac.Entry_Mode != TRS_MAN)
		{
			bcd_asc(chaine,&transac.Compressed_Iso2[1],20);
			for(j=0;((j<20)&&(chaine[j] !='d')&&(chaine[j] !='D')&&(chaine[j] !='='));j++);
			memcpy(Saved_Card_Nb,chaine,j);
		}
		else
			memcpy(Saved_Card_Nb,&transac.Compressed_Iso2[1],transac.Compressed_Iso2[0]);

		if(!memcmp(Card+ 1,Saved_Card_Nb,Card[0]))
		{
			// verification double transaction
			if(
				(transac.Transaction_Amount==ul_amount)&&
				(transac.CodeFunction == StructInfoPayment.CodeFunction)&&
				(memcmp(StructInfoPayment.Devise_acc.code_num,transac.NumCurrency,3)==0)
				)
				StructInfoPayment.Double_tra=1 ;
			switch(transac.CodeFunction)
			{
			case SALECODE :
			case CASHADVANCESCODE :
				l_l_amount = l_l_amount + transac.Transaction_Amount;
				break;

				//case REFUNDCODE :
			case VOIDCODE :
				l_l_amount = l_l_amount - transac.Transaction_Amount;
				break;

			default :
				break;
			}
		}
		l_l_count++;
	}
	indices_offline[0]=k;
	*amount_cumul = l_l_amount;//FF14
	l_l_amount = l_l_amount + ul_amount;
	if(	(l_l_amount > 0) && (l_l_amount > (L)StructInfoPayment.Plafond))//a voir
		return OK;

	return KO;
}
/*-----------------------------------------------------------------             
 * Fonction   : 
 * objet      : 
 *              
 * Parametre  : 
 * Retour     : 
 *-----------------------------------------------------------------             
*/
uchar CherchPreAuto_STAN(uchar *STAN,Ushort *indice)
{
	Ushort		l_count,nbr_preauto	;
	uchar			k;
 	uchar    	iso2[38];
 	uchar			j;
 	lignePreAut preauto;
	uint16  Index_preauto;

	uint8		ConvertBuffer[40];
	uint16		Offset;
	uint16		Length;
	uint8		dLength;
    ////////////////////////////////////

	l_count=0;
	nbr_preauto = FicPreauth.nombre_tra ;

	for(Index_preauto=0;Index_preauto<nbr_preauto;Index_preauto++)
	{
		read_preauto_index( Index_preauto ,&StrPreAut) ;
		if(memcmp(StrPreAut.STAN,STAN,6)==0)
      		break;
	}
	if(Index_preauto==nbr_preauto)
	{
		CUMORE_CMI_AFFICHE_MESSAGE(157,8,1000);
		//Mess_delay(157,8,0,100,1);
    	return KO ;
	}


	//for(i=0;i<nbr_preauto;i++){
	read_preauto_index(nbr_preauto - 1, &preauto);
	if ((StrPreAut.CodeFunction == VOID_PREAUTO_TOTAL_CODE) && (StructInfoPayment.CodeFunction != DUPLICATECODE)) {
		CUMORE_CMI_AFFICHE_MESSAGE(22, 3, 1000);
		return KO ;
	}

//	read_preauto_index(nbr_preauto - 2, &preauto);
//	if (preauto.CodeFunction == VOID_PREAUTO_TOTAL_CODE
//				|| preauto.CodeFunction == VOID_PREAUTO_PARTIEL_CODE) {
//			CUMORE_CMI_AFFICHE_MESSAGE(22, 3, 1000);
//			return KO ;
//		}
	*indice = Index_preauto ;
	////////////////////////////////////

	
	memcpy((PC)StructInfoPayment.Num_point_vente,StrPreAut.Num_point_vente,strlen((PC)StrPreAut.Num_point_vente));
	StructInfoPayment.Devise_acc.Exposant=StrPreAut.Exposant ;
	StructInfoPayment.Type_carte=StrPreAut.Type_carte ;
	StructInfoPayment.Origine_carte=StrPreAut.Origine_carte ;
	StructInfoPayment.Entry_Mode=StrPreAut.Entry_Mode;
	StructInfoPayment.Entry_Pin=StrPreAut.Entry_Pin;
	StructInfoPayment.StatusAnnulAdjust=StrPreAut.StatusAnnulAdjust ;
	StructInfoPayment.CodeFunction=StrPreAut.CodeFunction ;
	StructInfoPayment.Flag_Dem_Aut=StrPreAut.Flag_Dem_Aut ;
	StructInfoPayment.Top_Forcage=StrPreAut.Top_Forcage ;
	StructInfoPayment.avis=StrPreAut.avis ;
	StructInfoPayment.Langue=StrPreAut.Langue ;
	StructInfoPayment.Preauto_Amount_Initial=StrPreAut.Transaction_Amount ;
    StructInfoPayment.Transaction_Amount=StrPreAut.Transaction_Amount ;
	StructInfoPayment.Consolidation_Amount=StrPreAut.Consolidation_Amount ;
	// Il faut chercher la devise dont son code num==NumCurrency
	for(k=0;k<NB_MAX_DEV_ACC;k++)
	{
		if(!memcmp(Fic_Devise_Acc.TabDevise[k].code_num,StrPreAut.NumCurrency,3))
		{
			memcpy((uchar*)&StructInfoPayment.Devise_acc,
			(uchar*)&Fic_Devise_Acc.TabDevise[k],
			sizeof(StructInfoPayment.Devise_acc));
			gdevise_num=k;
			break;
		}
	}
	memcpy(StructInfoPayment.Transaction_DateTime,StrPreAut.Transaction_DateTime,12);
	memcpy(StructInfoPayment.Compressed_Iso2,StrPreAut.Compressed_Iso2,20);
	/*****/
/*	ShowTrace_Mouna("ad14");
	imp_buffer(StructInfoPayment.Compressed_Iso2,StructInfoPayment.Compressed_Iso2[0]);
	bcd_asc(iso2,&StructInfoPayment.Compressed_Iso2[1],20);
	imp_buffer(iso2,20);*/
	/*****/
 	if(StrPreAut.Entry_Mode == TRS_MAN)
 	{
 		memcpy(StructInfoPayment.Holder_Card,StrPreAut.Compressed_Iso2,20);
 	}
	else if(StrPreAut.Entry_Mode == TRS_SCT)
	{
		sprintf(iso2,"%s",&StrPreAut.Compressed_Iso2[1]);

		//for(j=0;((j<20)&&(iso2[j] !='d')&&(iso2[j] !='D')&&(iso2[j] !='='));j++);
 		for(j=0;((j<19)&&(iso2[j] !='d')&&(iso2[j] !='D')&&(iso2[j] !='='));j++);//badr probl?me PAN CARD = 19
		memcpy(&StructInfoPayment.Holder_Card[1],iso2,j);
		StructInfoPayment.Holder_Card[0] = j;
		j += 5 ; // Expired Date
		memcpy(StructInfoPayment.SeviceCode,&iso2[j],3);
	}
	else
 	{
		/*****/
/*		ShowTrace_Mouna("ad15");
		imp_buffer(StrPreAut.Compressed_Iso2,StrPreAut.Compressed_Iso2[0]);*/
		/*****/
		bcd_asc(iso2,&StrPreAut.Compressed_Iso2[1],28);
		/*****/
/*		imp_buffer(iso2,20);*/
		/*****/
		for(j=0;((j<20)&&(iso2[j] !='d')&&(iso2[j] !='D')&&(iso2[j] !='='));j++);
		memcpy(&StructInfoPayment.Holder_Card[1],iso2,j);
		StructInfoPayment.Holder_Card[0] = j;
		j += 5 ; // Expired Date 
		memcpy(StructInfoPayment.SeviceCode,&iso2[j],3);
	}
	memcpy(StructInfoPayment.Expired_Date,StrPreAut.Expired_Date,4);
	memcpy(StructInfoPayment.Approval_Code,StrPreAut.Approval_Code,6);
	memcpy(StructInfoPayment.Proc_Code,StrPreAut.Proc_Code,6);
	memcpy(StructInfoPayment.Num_sequence,StrPreAut.Num_sequence,6);
	memcpy(StructInfoPayment.NumCurrency_cons,StrPreAut.NumCurrency_cons,3);
	if(StrPreAut.Entry_Mode == TRS_EMV)
	{
		StructInfoPayment.l_champ55 = StrPreAut.Lg_EmvData;
		memcpy(StructInfoPayment.Champ55,StrPreAut.EmvData,StrPreAut.Lg_EmvData);
		if ((FindTag (TAG_APPLI_CRYPTOGRAMME_AC, &Offset, &Length,StrPreAut.EmvData, StrPreAut.Lg_EmvData) == OK) &&
				(Length == 8))//SEMV_TAG_9F26_APPLI_CRYPTO
		{
			memset (ConvertBuffer, 0, 8);
			if (memcmp (&StrPreAut.EmvData[Offset], ConvertBuffer, 8) != 0)
			{
				dLength=Length * 2;
				hex_str (ConvertBuffer, &StrPreAut.EmvData[Offset], dLength);
				memcpy(StructInfoPayment.Certif,ConvertBuffer, dLength);
			}
		}
	}
	return OK;
}

void TraiterEmv(UC type)
{ 
	DEL				CUMORE_CMI_outDEL;
    unsigned short	DebitAsked;
    unsigned char   buf[11];
    DataElement		elt;
	
	DebitAsked	= TRUE;
	elt.tag		= TAG_CUST_DEBIT_IS_ASKED;
	elt.length  = 2;
	ShortToChar(DebitAsked,buf);
	elt.ptValue  = (unsigned char *)buf;
	DEL_AddDataElement (&CUMORE_CMI_outDEL,&elt) ;
	ulTransactionType = type;
	ulTransactionTypeidlemessage = type;
	CUTERMclearDisplayT1();
}
/****************************************************************************
*                                                                           *
*      SOUS-PROGRAMME :  Credit                             			    *
*                                                                           *
*****************************************************************************
*                                                                           *
*      FONCTION :    Traitement d'une transaction credit				    *
*                                                                           *
*      ENTREE   :  fonction                                      			*
*                                                                           *
*      SORTIE   :  status                                                   *
*                                                                           *
****************************************************************************/
uchar	Credit()
{
	uchar	PtIso[MAXPTISO];
	uchar	status;

	do
	{
		status = Read_Card(PtIso,CARTE_COM);
	}while(status==SAIS_MAN);
	if(status==OK)
	{
		if(Recup_Info_badge(PtIso,0)!=OK)
		{
			CUMORE_CMI_AFFICHE_MESSAGE(14,16,1000);
			return KO;
		}
	}
	else if(status==ILLISIBLE)
	{
		status=KEY_CLEAR ;
		CUMORE_CMI_AFFICHE_MESSAGE(14,15,1000);
		return KO;
	}
	else
		return KO;
   ////////////////////////////////////

//	memset(PtIso,0,sizeof(PtIso));
//
//	status = PresentCard(PtIso,1);
//	switch(status)
//	{
//	case SAIS_EMV:
//		StructInfoPayment.Entry_Mode = TRS_EMV;
//		TraiterEmv(CUSERV_REFUND_TRANSACTION);
//		return OK;
//
//	case SAIS_MAG:
//		status = Recup_Info_Card(PtIso);
//		ShowTrace("SAIS_MAG 0");
//		if(status == OK)
//			status = transa_client();
//		return status;
//		break;
//
//	case SAIS_MAN:
//		StructInfoPayment.Entry_Mode = TRS_MAN;
//		if(Saisie_Info_Carte(REFUNDCODE)!=OK)
//			return KO;
//		return transa_client();
//		break;
//
//	case ILLISIBLE:
//	case PUCE_ILLISIBLE:
//	case ABORTED:
//		break;
//
//	default :
//		return KO;
//	}

	return OK;
}
void PurgePreauto(void)
{
	Ushort	Nb_preauto,delai_preauto,i,j;
	
	delai_preauto = FicPara.delai_preauto;
	
	Nb_preauto = FicPreauth.nombre_tra;
	
	for(i=0,j=0;(i<Nb_preauto); i++)
	{
		read_preauto_index( i ,&StrPreAut );
		StrPreAut.vie_preauth ++;
		if(		(StrPreAut.vie_preauth < delai_preauto)
//			&&	(StrPreAut.Indic_Accomp==0)
		)
		{
			write_preauto_index( j ,&StrPreAut );
			j+= 1;
		}
	}
	set_ushort_secourue(&FicPreauth.nombre_tra,j) ;

	PutAllPre();

}
uchar Annul_preautor_Puce(uchar	Type_Annul, uchar *Buffer,ushort indice)
{

	uchar			update_param[6];
	uchar			update_date[11];
	uchar			affiche_mont[16],affiche_date[16+1],montant_tmp[13];
	uchar			num_aut[7];
	uchar			date_time[13];
	uchar			date_tra[13];
	uchar			Date_sys[7];
	uchar			Time_sys[7];
	uchar			status=0;
	uchar			l_retour;
	uchar			Affich_ligne1[16+1];
	uchar			Exp;
	uchar			Date_YYMMDDHHmmss[13];
	int				h_clock=0, yr, mth, day, hr, min, sec, wday, j = 0;
	ulong			Montant_remplacement;



	memset(num_aut,0,7);

	memset(affiche_mont,0,14);
	memset(date_time,0,13);
	memset(date_tra,0,13);
	memset(Date_sys,0,7);
	memset(Time_sys,0,7);



	// Coherence de numro de carte
	if (memcmp(g_numero_carte,&StructInfoPayment.Holder_Card[1],StructInfoPayment.Holder_Card[0])!=0)//(memcmp(g_numero_carte,StructInfoPayment.Holder_Card,StructInfoPayment.Holder_Card[0]+1))
	{

		//if(g_trace)Os__xprint("Numero Carte Invalide");// STOAURGUI COMMENTE OPTIMISATION 290716
		CUMORE_CMI_AFFICHE_MESSAGE(179,16,1000);
		return KO;
	}

	//affichage sur ecran
	sprintf((PC)affiche_date,"%.2s/%.2s/%.2s   %.2s:%.2s",
		StructInfoPayment.Transaction_DateTime+4,
		StructInfoPayment.Transaction_DateTime+2,
		StructInfoPayment.Transaction_DateTime,
		StructInfoPayment.Transaction_DateTime+6,
		StructInfoPayment.Transaction_DateTime+8);

	if(StrPreAut.Transaction_Amount>StrPreAut.Montant_Accomp)
		Montant_remplacement = StrPreAut.Transaction_Amount - StrPreAut.Montant_Accomp;
	else
	{
		memset(&E_Transac8583,0x00,sizeof(E_Transac8583));
		memset(&R_Transac8583,0x00,sizeof(R_Transac8583));
		memset(&StructInfoPayment ,0x00,sizeof(StructInfoPayment));
		return KO;
	}

	long_asc (montant_tmp, 12, &Montant_remplacement);
	RemoveLeftZeros(montant_tmp);
	Exp = StructInfoPayment.Devise_acc.Exposant-48;
	FormatAmount(montant_tmp,affiche_mont,Exp);
	//Os__xprinta("Annul_preautor_Puce");
	memset(Affich_ligne1,0,sizeof(Affich_ligne1));
	//sprintf((PC)Affich_ligne1,"MNT:%9s%.3s",affiche_mont,StructInfoPayment.Devise_acc.code_alph);
	sprintf((PC)Affich_ligne1,"MNT:%9sDH",affiche_mont);
	//if(ValiderMess(Affich_ligne1,affiche_date)==KEY_CLEAR)
	if(ValiderMess(Affich_ligne1,affiche_date)!=KEY_ENTER)
		return KO;

	status = CherchPreAuto_STAN(Buffer,&indice) ;


	StructInfoPayment.Entry_Pin = 0;		//1.11.62	14122017
	//StructInfoPaymentEntry_PinForcee = 0;		//1.11.62	14122017

	// Rcupration de la date/heure de la transaction
	memset(Date_sys,'\0',7);
	memset(Time_sys,'\0',5);

	clock2int(h_clock, (unsigned char *)NULL, &yr, &mth, &day, &hr, &min, &sec, &wday);
	sprintf (Date_YYMMDDHHmmss,
		"%.2d%.2d%.2d%.2d%.2d%.2d",
		(yr%100),
		mth,
		day,
		hr,
		min,
		sec);
	memcpy(StructInfoPayment.Transaction_DateTime,Date_YYMMDDHHmmss,12);

	/*Os__read_date(Date_sys);
	memcpy(StructInfoPayment.Transaction_DateTime,&Date_sys[4],2);
	memcpy(StructInfoPayment.Transaction_DateTime+2,&Date_sys[2],2);
	memcpy(StructInfoPayment.Transaction_DateTime+4,&Date_sys[0],2);
	Os__read_time_sec (Time_sys) ;
	memcpy(StructInfoPayment.Transaction_DateTime+6,Time_sys,6)*/;
	StructInfoPayment.CodeFunction = VOIDCODE;

	//rcupration du message centrs
	if(BinPrefixControl(StructInfoPayment.Holder_Card,0)!=OK)
		return KO;
	if(Type_Annul == ANNUL_PREAUTOR_TOTAL)
		StructInfoPayment.CodeFunction = VOID_PREAUTO_TOTAL_CODE;
	else
		StructInfoPayment.CodeFunction = VOID_PREAUTO_PARTIEL_CODE;

//	read_trans_index( gindice ,&StrLineTra);                         //
//	StructInfoPayment.Flag_Dem_Aut=StrLineTra.Flag_Dem_Aut;          //larbi 10042019
//	StructInfoPayment.avis=StrLineTra.avis;                          //
	// si la transac offline et non avise on ne remonte pas au serveur
	StructInfoPayment.Author_Status = 0xff;
	Prepare_Transaction();
	memset(update_param,0,6);
	memset(update_date,0,11);
	l_retour=DemAutor8583(update_param,update_date); //Annul_preautor_Puce
	StructInfoPayment.Author_Status = l_retour;
	switch(l_retour)
	{
		case OK :

			MessageWithDelay("Annulation accept?e", "file://flash/HOST/Approuvee.png", 1000); // 200319 jrizki


			memcpy(StructInfoPayment.Approval_Code,R_Transac8583.champs38,6);
			StructInfoPayment.Transaction_Amount = StrPreAut.Transaction_Amount - StrPreAut.Montant_Accomp;

		/*	ImprimeTra(FicPara.Langue, COMMERCANT, 0); //Annul_preautor
			MessageWithWaitForAction("Couper ticket","file://flash/HOST/couper_ticket.png", 10000);
			ImprimeTra(FicPara.Langue, CLIENT, 0); //Annul_preautor */
			read_preauto_index( indice ,&StrPreAut );

			StrPreAut.StatusAnnulAdjust = 1; 				//1.11.61
			write_preauto_index( indice ,&StrPreAut ); 		//1.11.61

			StrPreAut.StatusAnnulAdjust = 0; 				//1.11.61

			memcpy(StrPreAut.STAN,StructInfoPayment.STAN,6);
			memcpy(StrPreAut.Num_sequence,StructInfoPayment.Num_sequence,6);
			StrPreAut.CodeFunction = StructInfoPayment.CodeFunction;
			write_preauto_index( FicPreauth.nombre_tra ,&StrPreAut );
			j=FicPreauth.nombre_tra+1;
			set_ushort_secourue(&FicPreauth.nombre_tra,j);

			if(FicPreauth.Indice_fin+1 < 999999)
				set_ulong_secourue(&FicPreauth.Indice_fin,FicPreauth.Indice_fin+1) ;
			else
				set_ulong_secourue(&FicPreauth.Indice_fin,0) ;

			PutAllPreEmetteur();


			ImprimeTra(FicPara.Langue, COMMERCANT, 0); //Annul_preautor
			MessageWithWaitForAction("Couper ticket","file://flash/HOST/couper_ticket.png", 10000);
			ImprimeTra(FicPara.Langue, CLIENT, 0); //Annul_preautor


			// Demande teleparametrage si ncessaire
			gest_param();
			break;

		case CAPTURE_CARTE:
		case REFERRAL:
		case NON_AUTOR:
			StructInfoPayment.Flag_Dem_Aut = 0;
			MessageWithDelay("\n AUTORISATION REFUSEE ", "file://flash/HOST/annuler.bmp", 2000);
//			MessageWithoutConfirmation("AUTORISATION REFUSEE","file://flash/HOST/annuler.bmp");
//			TimerStart(0, 2 * 1000);
//			while (TimerGet(0));           //wait until the 0th timer stops
//			TimerStop(0);                 //close the 0th timer

			//CUMORE_CMI_AFFICHE_MESSAGE(138,127,1000);
			//Mess_delay(138,127,0,100,1);
			break;

		case CONNEXION_KO:
			StructInfoPayment.Flag_Dem_Aut = 0;
			//CUMORE_CMI_AFFICHE_MESSAGE(138,127,1000);
			//Mess_delay(138,127,0,100,1);
			break;

		case FORMAT_KO :
		case EMISSION_KO :
			StructInfoPayment.Flag_Dem_Aut = 0;
			//CUMORE_CMI_AFFICHE_MESSAGE(138,127,1000);
			//Mess_delay(138,127,0,100,1);
			break;

		case RECEPT_KO :
			StructInfoPayment.Flag_Dem_Aut = 0;
			//CUMORE_CMI_AFFICHE_MESSAGE(170,138,1000);
			//Aff_mess(FicPara.Langue,170,0,1,0,1) ;
			break;

		case FORMAT_MSG_REC_KO:
			StructInfoPayment.Flag_Dem_Aut = 0;
			//CUMORE_CMI_AFFICHE_MESSAGE(138,127,1000);
			//Mess_delay(138,127,0,100,1);
			break;
	}
	//Sequence(l_retour,status) ;
	Sequence(l_retour,KEY_ENTER);

	return OK ;
}




/*-----------------------------------------------------------------
 * Fonction   :
 * objet      :
 *
 * Parametre  :
 * Retour     :
 *-----------------------------------------------------------------
*/
uchar Completion(void)
{
	uchar		Buffer[7];
	uchar		date_time[11];
	uchar		status;
	uchar		ServiceCode[3],ExpiredDate[4],Devise_origine[3];
	Ushort		indice;


	memset(&E_Transac8583,0x00,sizeof(E_Transac8583));
	memset(&R_Transac8583,0x00,sizeof(R_Transac8583));
	memset(&StructInfoPayment ,0x00,sizeof(StructInfoPayment));
	memset(&StrLineTra ,0x00,sizeof(StrLineTra));
	memset(&StrPreAut ,0x00,sizeof(StrPreAut));

	memset(Buffer,0,7);
	memset(date_time,0,11);


	if(ucGetField(175,"COMPLETION PRE AUTO",Buffer,7,1,6,"     ",30000,0)!=OK)
		return KO ;

	//FillWithZero(Buffer,6); //commenter par larbi 26022019

	status = CherchPreAuto_STAN(Buffer,&indice);
	if(status!=OK)
		return KO;

	if(StrPreAut.StatusAnnulAdjust == 1)
		{
			CUMORE_CMI_AFFICHE_MESSAGE(157,9,1000);
			return KO;
		}

	if(StrPreAut.Indic_Accomp)
	{
		CUMORE_CMI_AFFICHE_MESSAGE(157,174,1000);
		return KO;
	}

	gindice = indice;
	memcpy(g_numero_carte,StructInfoPayment.Holder_Card,20);
	memcpy(StructInfoPayment.g_numero_carte,g_numero_carte,20);


	memcpy(Devise_origine,StructInfoPayment.Devise_acc.code_alph,3);
	memcpy(ServiceCode,StructInfoPayment.SeviceCode,3);
	memcpy(ExpiredDate,StructInfoPayment.Expired_Date,4);
	StructInfoPayment.CodeFunction = PREAUTHCOMPCODE;
	PutAllParam();
//	Os__xprintaa("g_numero_carte emv");
//	Os__xprintd(g_numero_carte);
//	Os__xprintaa("StructInfoPayment.g_numero_carte emv");
//	Os__xprintd(StructInfoPayment.g_numero_carte);

	return OK;
}

/*-----------------------------------------------------------------
 * Fonction   :
 * objet      :
 *
 * Parametre  :
 * Retour     :
 *-----------------------------------------------------------------
*/
//uchar Annul_preautor(uchar	Type_Annul)    //here
//{
//	uchar			Buffer[7];
//	uchar			montant[13] ;
//	uchar			update_param[6];
//	uchar			update_date[11] ;
//	uchar			affiche_mont[16],affiche_date[16+1],montant_tmp[13];
//	uchar			numero_carte[20];
//	uchar			num_aut[7];
//	uchar			date_time[13];
//	uchar			date_tra[13];
//	uchar			Date_sys[7];
//	uchar			Time_sys[7];
//	ushort   		indice=0;
//	uchar			status;
//	uchar			l_retour;
//	uchar			PtIso[MAXPTISO];
//	uchar			Affich_ligne1[16+1];
//	ushort			Nb_preauto,i,j;
//	ulong			Montant_remplacement;
//	uchar			Exp;
//	uchar			Date_YYMMDDHHmmss[13];
//  int h_clock=0, yr, mth, day, hr, min, sec, wday;
//
//	memset(Buffer,0,7);
//	memset(num_aut,0,7);
//	memset(montant,0,13);
//	memset(affiche_mont,0,sizeof(affiche_mont));
//	memset(affiche_mont,0,sizeof(affiche_date));
//	memset(montant_tmp,0,sizeof(montant_tmp));
//	memset(numero_carte,0,20);
//	memset(date_time,0,13);
//	memset(date_tra,0,13);
//	memset(Date_sys,0,7);
//	memset(Time_sys,0,7);
//	memset(&E_Transac8583,0x00,sizeof(E_Transac8583));
//	memset(&R_Transac8583,0x00,sizeof(R_Transac8583));
//	memset(&StructInfoPayment ,0x00,sizeof(StructInfoPayment));
//	StructInfoPayment.puce_out = 1;
//	memset(&StrPreAut ,0x00,sizeof(StrPreAut));
//	StructInfoPayment.CodeFunction = VOIDCODE;
//
//
////	status =control_badge();//Annul_preautor	//1.11.61
////	if(status != OK)
////		return KO;
//
//	////////////////////////////////////
//	/*do
//	{
//		status = Read_Card(PtIso,CARTE_COM);
//	}while(status==SAIS_MAN);
//	if(status==OK)
//	{
//		if(Recup_Info_badge(PtIso,0)!=OK)
//		{
//			CUMORE_CMI_AFFICHE_MESSAGE(14,16,1000);
//			memset(&E_Transac8583,0x00,sizeof(E_Transac8583));
//			memset(&R_Transac8583,0x00,sizeof(R_Transac8583));
//			memset(&StructInfoPayment ,0x00,sizeof(StructInfoPayment));
//			return KO;
//		}
//	}
//	else if(status==ILLISIBLE)
//	{
//		status=KEY_CLEAR ;
//		CUMORE_CMI_AFFICHE_MESSAGE(14,15,1000);
//		memset(&E_Transac8583,0x00,sizeof(E_Transac8583));
//		memset(&R_Transac8583,0x00,sizeof(R_Transac8583));
//		memset(&StructInfoPayment ,0x00,sizeof(StructInfoPayment));
//		return KO;
//	}
//	else
//	{
//		memset(&E_Transac8583,0x00,sizeof(E_Transac8583));
//		memset(&R_Transac8583,0x00,sizeof(R_Transac8583));
//		memset(&StructInfoPayment ,0x00,sizeof(StructInfoPayment));
//		return KO;
//	}*/
//	////////////////////////////////////
//
//	// STAN
//	if(ucGetField(175,"ANNULATION PRE AUTO",Buffer,7,1,6,"     ",30000,0)!=OK)
//		{
//			memset(&E_Transac8583,0x00,sizeof(E_Transac8583));
//			memset(&R_Transac8583,0x00,sizeof(R_Transac8583));
//			memset(&StructInfoPayment ,0x00,sizeof(StructInfoPayment));
//			return KO;
//		}
//	FillWithZero(Buffer,6);
//
//    //memset(&StructInfoPayment ,0x00,sizeof(StructInfoPayment));
//
//	/*status=Dem_Transa("000800",Buffer);//Annulation Pre auto
//	if(status!=OK)
//	{
//		memset(&E_Transac8583,0x00,sizeof(E_Transac8583));
//		memset(&R_Transac8583,0x00,sizeof(R_Transac8583));
//		memset(&StructInfoPayment ,0x00,sizeof(StructInfoPayment));
//		return KO;
//	}*/
//	//copie strlinetra ds strlinepre
//	//memcpy(&StrPreAut,&StrLineTra,sizeof(StrPreAut));
//	//StructInfoPayment.Preauto_Amount_Initial=StrPreAut.Transaction_Amount;
////XPOS
//
//	status = CherchPreAuto_STAN(Buffer,&indice);
//	if(status!=OK)
//	{
//		memset(&E_Transac8583,0x00,sizeof(E_Transac8583));
//		memset(&R_Transac8583,0x00,sizeof(R_Transac8583));
//		memset(&StructInfoPayment ,0x00,sizeof(StructInfoPayment));
//		return KO;
//	}
//	StructInfoPayment.puce_out = 1;
//	StructInfoPayment.CodeFunction = VOIDCODE;
//	StructInfoPayment.Entry_Pin = 0;		//1.11.62	14122017
//	//StructInfoPaymentEntry_PinForcee = 0;		//1.11.62	14122017
//	if(
//			((StrPreAut.Indic_Accomp) && (Type_Annul == ANNUL_PREAUTOR_TOTAL))
//		||	((!StrPreAut.Indic_Accomp) && (Type_Annul == ANNUL_PREAUTOR_PARTIEL))			//1.11.66
//		||	(StrPreAut.StatusAnnulAdjust == 1)							//1.11.61
//		||	(StrPreAut.CodeFunction == VOID_PREAUTO_TOTAL_CODE)					//1.11.61
//		||	(StrPreAut.CodeFunction == VOID_PREAUTO_PARTIEL_CODE)					//1.11.61
//	)
//	{
//		CUMORE_CMI_AFFICHE_MESSAGE(2,129,1000);
//		memset(&E_Transac8583,0x00,sizeof(E_Transac8583));
//		memset(&R_Transac8583,0x00,sizeof(R_Transac8583));
//		memset(&StructInfoPayment ,0x00,sizeof(StructInfoPayment));
//		return KO;
//	}
//
//	memcpy(numero_carte,&StructInfoPayment.Holder_Card[1],StructInfoPayment.Holder_Card[0]);
//	memcpy(g_numero_carte,&StructInfoPayment.Holder_Card[1],StructInfoPayment.Holder_Card[0]);
//	g_Type_Annul=Type_Annul;
//	gindice=indice;
//
//	////////////////////////////////////
////******Mouna*******//
//	memset(PtIso,0,sizeof(PtIso));
////******Mouna*******//
//	status = PresentCard(PtIso,1);
//	switch(status)
//	{
//		case SAIS_EMV:
//			StructInfoPayment.Entry_Mode = TRS_EMV;
//			//TraiterEmv(CUSERV_VOIDP_TRANSACTION);
//			Annul_preautor_Puce(Type_Annul,Buffer,indice);
//			return OK;
//
//		case SAIS_MAG:
//			StructInfoPayment.Entry_Mode = TRS_MAG;
//			status = Recup_Info_Card(PtIso);
//			if(status != OK)
//			{
//				memset(&E_Transac8583,0x00,sizeof(E_Transac8583));
//				memset(&R_Transac8583,0x00,sizeof(R_Transac8583));
//				memset(&StructInfoPayment ,0x00,sizeof(StructInfoPayment));
//				return KO;
//			}
//			break;
//
//		case SAIS_MAN :
//			StructInfoPayment.Entry_Mode = TRS_MAN;
//			if(Saisie_Info_Carte(VOIDCODE)!=OK)
//			{
//				memset(&E_Transac8583,0x00,sizeof(E_Transac8583));
//				memset(&R_Transac8583,0x00,sizeof(R_Transac8583));
//				memset(&StructInfoPayment ,0x00,sizeof(StructInfoPayment));
//				return KO;
//			}
//			break;
//
//	  default :
//		memset(&E_Transac8583,0x00,sizeof(E_Transac8583));
//		memset(&R_Transac8583,0x00,sizeof(R_Transac8583));
//		memset(&StructInfoPayment ,0x00,sizeof(StructInfoPayment));
//		return KO;
//	}
//	////////////////////////////////////
//
//	// Coherence de numro de carte
////	memset(size_carte,'\0',3);
////	sprintf(size_carte,"%d",StructInfoPayment.Holder_Card[0]);
////	ShowTrace(size_carte);
////	imp_buffer(numero_carte,20);
////	imp_buffer(StructInfoPayment.Holder_Card,20);
//
//	/*if(memcmp(numero_carte,&StructInfoPayment.Holder_Card[1],StructInfoPayment.Holder_Card[0]))
//	{
//		//Os__print("Numero Carte Invalide");
//		CUMORE_CMI_AFFICHE_MESSAGE(179,16,1000);
//		memset(&E_Transac8583,0x00,sizeof(E_Transac8583));
//		memset(&R_Transac8583,0x00,sizeof(R_Transac8583));
//		memset(&StructInfoPayment ,0x00,sizeof(StructInfoPayment));
//		return KO;
//	}*/
//
//	////////////////////////////////////////////////////////////////////////
//
//	sprintf((PC)affiche_date,"%.2s/%.2s/%.2s   %.2s:%.2s",
//						StructInfoPayment.Transaction_DateTime+4,
//						StructInfoPayment.Transaction_DateTime+2,
//						StructInfoPayment.Transaction_DateTime,
//						StructInfoPayment.Transaction_DateTime+6,
//						StructInfoPayment.Transaction_DateTime+8);
//
//	if(StrPreAut.Transaction_Amount>StrPreAut.Montant_Accomp)
//		Montant_remplacement = StrPreAut.Transaction_Amount - StrPreAut.Montant_Accomp;
//	else
//	{
//		memset(&E_Transac8583,0x00,sizeof(E_Transac8583));
//		memset(&R_Transac8583,0x00,sizeof(R_Transac8583));
//		memset(&StructInfoPayment ,0x00,sizeof(StructInfoPayment));
//		return KO;
//	}
//	//long_asc (montant_tmp, 12, &StructInfoPayment.Transaction_Amount);
//	long_asc (montant_tmp, 12, &Montant_remplacement);
//	RemoveLeftZeros(montant_tmp);
//	Exp=2;
//	FormatAmount(montant_tmp,affiche_mont,Exp);
//
//	//Os__xprintaa("Apreauto");
//	memset(Affich_ligne1,0,sizeof(Affich_ligne1));
//	//sprintf((PC)Affich_ligne1,"MNT:%9s%.3s",affiche_mont,StructInfoPayment.Devise_acc.code_alph);
//	sprintf((PC)Affich_ligne1,"MNT:%9sDH",affiche_mont);
//
//	//if(ValiderMess(Affich_ligne1,affiche_date)==KEY_CLEAR)
//	if(ValiderMess(Affich_ligne1,affiche_date)!=KEY_ENTER)
//	{
//		memset(&E_Transac8583,0x00,sizeof(E_Transac8583));
//		memset(&R_Transac8583,0x00,sizeof(R_Transac8583));
//		memset(&StructInfoPayment ,0x00,sizeof(StructInfoPayment));
//		return KO;
//	}
//
//	status = CherchPreAuto_STAN(Buffer,&indice) ;
//	StructInfoPayment.Entry_Pin = 0;		//1.11.62	14122017
//	//StructInfoPaymentEntry_PinForcee = 0;		//1.11.62	14122017
//	// Rcupration de la date/heure de la transaction
//	memset(Date_sys,'\0',7);
//	memset(Time_sys,'\0',5);
//
//	clock2int(h_clock, (unsigned char *)NULL, &yr, &mth, &day, &hr, &min, &sec, &wday);
//	sprintf (Date_YYMMDDHHmmss,
//		"%.2d%.2d%.2d%.2d%.2d%.2d",
//		(yr%100),
//		mth,
//		day,
//		hr,
//		min,
//		sec);
//	memcpy(StructInfoPayment.Transaction_DateTime,Date_YYMMDDHHmmss,12);
//
//	StructInfoPayment.CodeFunction = VOIDCODE ;
//	StructInfoPayment.Holder_Card[0] = StructInfoPayment.CardNumber[0];
//	memcpy(StructInfoPayment.Holder_Card+1,&StructInfoPayment.CardNumber[1],StructInfoPayment.CardNumber[0]);
//	BinPrefixControl(StructInfoPayment.Holder_Card,0);
//
//	if(Type_Annul == ANNUL_PREAUTOR_TOTAL)
//		StructInfoPayment.CodeFunction = VOID_PREAUTO_TOTAL_CODE;
//	else
//		StructInfoPayment.CodeFunction = VOID_PREAUTO_PARTIEL_CODE;
//
//	//read_trans_index( indice ,&StrLineTra) ;
//	// si la transac offline et non avise on ne remonte pas au serveur
//	StructInfoPayment.Author_Status = 0xff;
//	Prepare_Transaction();
//	memset(update_param,0,6);
//	memset(update_date,0,11);
//	l_retour=DemAutor8583(update_param,update_date); //Annul_preautor
//	StructInfoPayment.Author_Status = l_retour;
//	switch(l_retour)
//	{
//		case OK :
//			memcpy(StructInfoPayment.Approval_Code,R_Transac8583.champs38,6);
//			StructInfoPayment.Transaction_Amount = StrPreAut.Transaction_Amount - StrPreAut.Montant_Accomp;
//			//ImprimeTra(FicPara.Langue,COMMERCANT,0); //Annul_preautor
//			ImprimeTra(FicPara.Langue,CLIENT,0); //Annul_preautor
//			/*memcpy(StrLineTra.Transaction_DateTime,StructInfoPayment.Transaction_DateTime,12);
//			//Suppression
//			if(Save_Tra_StarPos("000500",StrPreAut.STAN) != OK)
//				return ABORTED;*/
//
//			read_preauto_index( indice ,&StrPreAut );
//
//			StrPreAut.StatusAnnulAdjust = 1; 				//1.11.61
//			write_preauto_index( indice ,&StrPreAut ); 		//1.11.61
//
//			StrPreAut.StatusAnnulAdjust = 0; 				//1.11.61
//
//			memcpy(StrPreAut.STAN,StructInfoPayment.STAN,6);
//			memcpy(StrPreAut.Num_sequence,StructInfoPayment.Num_sequence,6);
//			StrPreAut.CodeFunction = StructInfoPayment.CodeFunction;
//			write_preauto_index( FicPreauth.nombre_tra ,&StrPreAut );
//			j=FicPreauth.nombre_tra+1;
//			set_ushort_secourue(&FicPreauth.nombre_tra,j);
//			PutAllPreEmetteur();
//			// Demande teleparametrage si ncessaire
//			gest_param() ;
//			break;
//
//		case CAPTURE_CARTE:
//		case REFERRAL:
//		case NON_AUTOR:
//			StructInfoPayment.Flag_Dem_Aut = 0;
//			//CUMORE_CMI_AFFICHE_MESSAGE(138,127,1000);
//			break;
//
//		case CONNEXION_KO:
//			StructInfoPayment.Flag_Dem_Aut = 0;
//			//CUMORE_CMI_AFFICHE_MESSAGE(138,127,1000);
//			break;
//
//		case FORMAT_KO :
//		case EMISSION_KO :
//			StructInfoPayment.Flag_Dem_Aut = 0;
//			//CUMORE_CMI_AFFICHE_MESSAGE(138,127,1000);
//			break;
//
//		case RECEPT_KO :
//			StructInfoPayment.Flag_Dem_Aut = 0;
//			break;
//
//		case FORMAT_MSG_REC_KO:
//			StructInfoPayment.Flag_Dem_Aut = 0;
//			//CUMORE_CMI_AFFICHE_MESSAGE(138,127,1000);
//			break;
//	}
//	Sequence(l_retour,KEY_ENTER) ;
//
//	memset(&E_Transac8583,0x00,sizeof(E_Transac8583));
//	memset(&R_Transac8583,0x00,sizeof(R_Transac8583));
//	memset(&StructInfoPayment ,0x00,sizeof(StructInfoPayment));
//	return OK;
//}









uchar Annul_preautor(uchar	Type_Annul) { //marche
	uchar			Buffer[7];
	uchar			montant[13] ;
	uchar			update_param[6];
	uchar			update_date[11] ;
	uchar			affiche_mont[16],affiche_date[16+1],montant_tmp[13];
	uchar			numero_carte[20];
	uchar			num_aut[7];
	uchar			date_time[13];
	uchar			date_tra[13];
	uchar			Date_sys[7];
	uchar			Time_sys[7];
	ushort   		indice=0;
	uchar			status;
	uchar			l_retour;
	uchar			PtIso[MAXPTISO];
	uchar			Affich_ligne1[16+1];
	ushort			Nb_preauto,i,j;
	ulong			Montant_remplacement;
	uchar			Exp;
	uchar			Date_YYMMDDHHmmss[13];
  int h_clock=0, yr, mth, day, hr, min, sec, wday;

	memset(Buffer,0,7);
	memset(num_aut,0,7);
	memset(montant,0,13);
	memset(affiche_mont,0,sizeof(affiche_mont));
	memset(affiche_mont,0,sizeof(affiche_date));
	memset(montant_tmp,0,sizeof(montant_tmp));
	memset(numero_carte,0,20);
	memset(date_time,0,13);
	memset(date_tra,0,13);
	memset(Date_sys,0,7);
	memset(Time_sys,0,7);
	memset(&E_Transac8583,0x00,sizeof(E_Transac8583));
	memset(&R_Transac8583,0x00,sizeof(R_Transac8583));
	memset(&StructInfoPayment ,0x00,sizeof(StructInfoPayment));
	StructInfoPayment.puce_out = 1;
	memset(&StrPreAut ,0x00,sizeof(StrPreAut));
	StructInfoPayment.CodeFunction = VOIDCODE;


//	status =control_badge();//Annul_preautor	//1.11.61
//	if(status != OK)
//		return KO;

	////////////////////////////////////
	/*do
	{
		status = Read_Card(PtIso,CARTE_COM);
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
	}*/
	////////////////////////////////////

	// STAN
//	if(ucGetField(175,Buffer,7,1,6,"     ",30000,0)!=OK)
//	{
//		memset(&E_Transac8583,0x00,sizeof(E_Transac8583));
//		memset(&R_Transac8583,0x00,sizeof(R_Transac8583));
//		memset(&StructInfoPayment ,0x00,sizeof(StructInfoPayment));
//		return KO;
//	}

	if(ucGetField(175,"ANNULATION PRE AUTO",Buffer,7,6,6,"     ",30000,0)!=OK)
		{
			memset(&E_Transac8583,0x00,sizeof(E_Transac8583));
			memset(&R_Transac8583,0x00,sizeof(R_Transac8583));
			memset(&StructInfoPayment ,0x00,sizeof(StructInfoPayment));
			return KO;
		}
	//FillWithZero(Buffer,6); //commenter par larbi 12042019

    //memset(&StructInfoPayment ,0x00,sizeof(StructInfoPayment));

	/*status=Dem_Transa("000800",Buffer);//Annulation Pre auto
	if(status!=OK)
	{
		memset(&E_Transac8583,0x00,sizeof(E_Transac8583));
		memset(&R_Transac8583,0x00,sizeof(R_Transac8583));
		memset(&StructInfoPayment ,0x00,sizeof(StructInfoPayment));
		return KO;
	}*/
	//copie strlinetra ds strlinepre
	//memcpy(&StrPreAut,&StrLineTra,sizeof(StrPreAut));
	//StructInfoPayment.Preauto_Amount_Initial=StrPreAut.Transaction_Amount;
//XPOS

	status = CherchPreAuto_STAN(Buffer,&indice);
	if(status!=OK)
	{
		memset(&E_Transac8583,0x00,sizeof(E_Transac8583));
		memset(&R_Transac8583,0x00,sizeof(R_Transac8583));
		memset(&StructInfoPayment ,0x00,sizeof(StructInfoPayment));
		return KO;
	}
	StructInfoPayment.puce_out = 1;
	StructInfoPayment.CodeFunction = VOIDCODE;
	StructInfoPayment.Entry_Pin = 0;		//1.11.62	14122017
	//StructInfoPaymentEntry_PinForcee = 0;		//1.11.62	14122017
	if(
			((StrPreAut.Indic_Accomp) && (Type_Annul == ANNUL_PREAUTOR_TOTAL))
		||	((!StrPreAut.Indic_Accomp) && (Type_Annul == ANNUL_PREAUTOR_PARTIEL))			//1.11.66
		||	(StrPreAut.StatusAnnulAdjust == 1)							//1.11.61
		||	(StrPreAut.CodeFunction == VOID_PREAUTO_TOTAL_CODE)					//1.11.61
		||	(StrPreAut.CodeFunction == VOID_PREAUTO_PARTIEL_CODE)					//1.11.61
	)
	{
		CUMORE_CMI_AFFICHE_MESSAGE(2,129,1000);
		memset(&E_Transac8583,0x00,sizeof(E_Transac8583));
		memset(&R_Transac8583,0x00,sizeof(R_Transac8583));
		memset(&StructInfoPayment ,0x00,sizeof(StructInfoPayment));
		return KO;
	}

	memcpy(numero_carte,&StructInfoPayment.Holder_Card[1],StructInfoPayment.Holder_Card[0]);
	memcpy(g_numero_carte,&StructInfoPayment.Holder_Card[1],StructInfoPayment.Holder_Card[0]);
	g_Type_Annul=Type_Annul;
	gindice=indice;

	//Os__xprinta("trace 1");
	////////////////////////////////////
//******Mouna*******//
	memset(PtIso,0,sizeof(PtIso));
	Annul_preautor_Puce(Type_Annul,Buffer,indice);
    return OK;
//******Mouna*******//
//	status = PresentCard(PtIso,1);
//	switch(status)
//	{
//		case SAIS_EMV:
//			StructInfoPayment.Entry_Mode = TRS_EMV;
//		    //TraiterEmv(CUSERV_VOIDP_TRANSACTION);
//			Annul_preautor_Puce(Type_Annul,Buffer,indice);
//			return OK;
//
//		case SAIS_MAG:
//			StructInfoPayment.Entry_Mode = TRS_MAG;
//			status = Recup_Info_Card(PtIso);
//			if(status != OK)
//			{
//				memset(&E_Transac8583,0x00,sizeof(E_Transac8583));
//				memset(&R_Transac8583,0x00,sizeof(R_Transac8583));
//				memset(&StructInfoPayment ,0x00,sizeof(StructInfoPayment));
//				return KO;
//			}
//			break;
//
//		case SAIS_MAN :
//			StructInfoPayment.Entry_Mode = TRS_MAN;
//			if(Saisie_Info_Carte(VOIDCODE)!=OK)
//			{
//				memset(&E_Transac8583,0x00,sizeof(E_Transac8583));
//				memset(&R_Transac8583,0x00,sizeof(R_Transac8583));
//				memset(&StructInfoPayment ,0x00,sizeof(StructInfoPayment));
//				return KO;
//			}
//			break;
//
//	  default :
//		memset(&E_Transac8583,0x00,sizeof(E_Transac8583));
//		memset(&R_Transac8583,0x00,sizeof(R_Transac8583));
//		memset(&StructInfoPayment ,0x00,sizeof(StructInfoPayment));
//		return KO;
//	}
	////////////////////////////////////

	// Coherence de num?ro de carte
//	memset(size_carte,'\0',3);
//	sprintf(size_carte,"%d",StructInfoPayment.Holder_Card[0]);
//	ShowTrace(size_carte);
//	imp_buffer(numero_carte,20);
//	imp_buffer(StructInfoPayment.Holder_Card,20);

	/*if(memcmp(numero_carte,&StructInfoPayment.Holder_Card[1],StructInfoPayment.Holder_Card[0]))
	{
		//Os__print("Numero Carte Invalide");
		CUMORE_CMI_AFFICHE_MESSAGE(179,16,1000);
		memset(&E_Transac8583,0x00,sizeof(E_Transac8583));
		memset(&R_Transac8583,0x00,sizeof(R_Transac8583));
		memset(&StructInfoPayment ,0x00,sizeof(StructInfoPayment));
		return KO;
	}*/

	////////////////////////////////////////////////////////////////////////

	sprintf((PC)affiche_date,"%.2s/%.2s/%.2s   %.2s:%.2s",
						StructInfoPayment.Transaction_DateTime+4,
						StructInfoPayment.Transaction_DateTime+2,
						StructInfoPayment.Transaction_DateTime,
						StructInfoPayment.Transaction_DateTime+6,
						StructInfoPayment.Transaction_DateTime+8);

	if(StrPreAut.Transaction_Amount>StrPreAut.Montant_Accomp)
		Montant_remplacement = StrPreAut.Transaction_Amount - StrPreAut.Montant_Accomp;
	else
	{
		memset(&E_Transac8583,0x00,sizeof(E_Transac8583));
		memset(&R_Transac8583,0x00,sizeof(R_Transac8583));
		memset(&StructInfoPayment ,0x00,sizeof(StructInfoPayment));
		return KO;
	}
	//long_asc (montant_tmp, 12, &StructInfoPayment.Transaction_Amount);
	long_asc (montant_tmp, 12, &Montant_remplacement);
	RemoveLeftZeros(montant_tmp);
	Exp=2;
	FormatAmount(montant_tmp,affiche_mont,Exp);

	//Os__xprinta("Apreauto");
	memset(Affich_ligne1,0,sizeof(Affich_ligne1));
	//sprintf((PC)Affich_ligne1,"MNT:%9s%.3s",affiche_mont,StructInfoPayment.Devise_acc.code_alph);
	sprintf((PC)Affich_ligne1,"MNT:%9sDH",affiche_mont);

	//if(ValiderMess(Affich_ligne1,affiche_date)==KEY_CLEAR)
	if(ValiderMess(Affich_ligne1,affiche_date)!=KEY_ENTER)
	{
		memset(&E_Transac8583,0x00,sizeof(E_Transac8583));
		memset(&R_Transac8583,0x00,sizeof(R_Transac8583));
		memset(&StructInfoPayment ,0x00,sizeof(StructInfoPayment));
		return KO;
	}

	status = CherchPreAuto_STAN(Buffer,&indice) ;
	StructInfoPayment.Entry_Pin = 0;		//1.11.62	14122017
	//StructInfoPaymentEntry_PinForcee = 0;		//1.11.62	14122017
	// Rcupration de la date/heure de la transaction
	memset(Date_sys,'\0',7);
	memset(Time_sys,'\0',5);

	clock2int(h_clock, (unsigned char *)NULL, &yr, &mth, &day, &hr, &min, &sec, &wday);
	sprintf (Date_YYMMDDHHmmss,
		"%.2d%.2d%.2d%.2d%.2d%.2d",
		(yr%100),
		mth,
		day,
		hr,
		min,
		sec);
	memcpy(StructInfoPayment.Transaction_DateTime,Date_YYMMDDHHmmss,12);

	StructInfoPayment.CodeFunction = VOIDCODE ;
	StructInfoPayment.Holder_Card[0] = StructInfoPayment.CardNumber[0];
	memcpy(StructInfoPayment.Holder_Card+1,&StructInfoPayment.CardNumber[1],StructInfoPayment.CardNumber[0]);
	BinPrefixControl(StructInfoPayment.Holder_Card,0);

	if(Type_Annul == ANNUL_PREAUTOR_TOTAL)
		StructInfoPayment.CodeFunction = VOID_PREAUTO_TOTAL_CODE;
	else
		StructInfoPayment.CodeFunction = VOID_PREAUTO_PARTIEL_CODE;

	//read_trans_index( indice ,&StrLineTra) ;
	// si la transac offline et non avise on ne remonte pas au serveur
	StructInfoPayment.Author_Status = 0xff;
	Prepare_Transaction();
	memset(update_param,0,6);
	memset(update_date,0,11);
	l_retour=DemAutor8583(update_param,update_date); //Annul_preautor
	StructInfoPayment.Author_Status = l_retour;
	switch(l_retour)
	{
		case OK :

			MessageWithDelay("Annulation accept?e", "file://flash/HOST/Approuvee.png", 1000); // jrizki 200319


			memcpy(StructInfoPayment.Approval_Code,R_Transac8583.champs38,6);
			StructInfoPayment.Transaction_Amount = StrPreAut.Transaction_Amount - StrPreAut.Montant_Accomp;
			ImprimeTra(FicPara.Langue,COMMERCANT,0); //Annul_preautor
			MessageWithWaitForAction("Couper ticket","file://flash/HOST/couper_ticket.png", 10000);
			ImprimeTra(FicPara.Langue,CLIENT,0); //Annul_preautor

			detectCard();

			/*memcpy(StrLineTra.Transaction_DateTime,StructInfoPayment.Transaction_DateTime,12);
			//Suppression
			if(Save_Tra_StarPos("000500",StrPreAut.STAN) != OK)
				return ABORTED;*/

			read_preauto_index( indice ,&StrPreAut );

			StrPreAut.StatusAnnulAdjust = 1; 				//1.11.61
			write_preauto_index( indice ,&StrPreAut ); 		//1.11.61

			StrPreAut.StatusAnnulAdjust = 0; 				//1.11.61

			memcpy(StrPreAut.STAN,StructInfoPayment.STAN,6);
			memcpy(StrPreAut.Num_sequence,StructInfoPayment.Num_sequence,6);
			StrPreAut.CodeFunction = StructInfoPayment.CodeFunction;
			write_preauto_index( FicPreauth.nombre_tra ,&StrPreAut );
			j=FicPreauth.nombre_tra+1;
			set_ushort_secourue(&FicPreauth.nombre_tra,j);
			PutAllPreEmetteur();
			// Demande teleparametrage si ncessaire
			gest_param() ;
			break;

		case CAPTURE_CARTE:
		case REFERRAL:
		case NON_AUTOR:
			StructInfoPayment.Flag_Dem_Aut = 0;
			//CUMORE_CMI_AFFICHE_MESSAGE(138,127,1000);

			break;

		case CONNEXION_KO:
			StructInfoPayment.Flag_Dem_Aut = 0;
			//CUMORE_CMI_AFFICHE_MESSAGE(138,127,1000);
			break;

		case FORMAT_KO :
		case EMISSION_KO :
			StructInfoPayment.Flag_Dem_Aut = 0;
			//CUMORE_CMI_AFFICHE_MESSAGE(138,127,1000);
			break;

		case RECEPT_KO :
			StructInfoPayment.Flag_Dem_Aut = 0;
			break;

		case FORMAT_MSG_REC_KO:
			StructInfoPayment.Flag_Dem_Aut = 0;
			//CUMORE_CMI_AFFICHE_MESSAGE(138,127,1000);
			break;
	}
	Sequence(l_retour,KEY_ENTER) ;

	memset(&E_Transac8583,0x00,sizeof(E_Transac8583));
	memset(&R_Transac8583,0x00,sizeof(R_Transac8583));
	memset(&StructInfoPayment ,0x00,sizeof(StructInfoPayment));
	return OK;
}


/*-----------------------------------------------------------------
 * Fonction   :
 * objet      :
 *
 * Parametre  :
 * Retour     :
 *-----------------------------------------------------------------
*/

void PurgePreautoNaps(void)                                  //1.11.61
{
	ushort Nb_preauto, delai_preauto, i, j;
	char tracepurge[24];

	delai_preauto = FicPara.delai_preauto;

	JournalEditPre();
	MessageWithWaitForAction("Couper ticket","file://flash/HOST/couper_ticket.png", 10000);
	Nb_preauto = FicPreauth.nombre_tra;

	for (i = 0, j = 0; (i < Nb_preauto); i++) {
		read_preauto_index(i, &StrPreAut);

		if (StrPreAut.CodeFunction == VOID_PREAUTO_TOTAL_CODE) {
			//sprintf(tracepurge,"ANN TOT : %s effac?",StrPreAut.STAN);
		} else if (StrPreAut.CodeFunction == VOID_PREAUTO_PARTIEL_CODE) {
			//sprintf(tracepurge,"ANN PAR : %s effac?",StrPreAut.STAN);
		} else if (StrPreAut.StatusAnnulAdjust == 1) {
			//sprintf(tracepurge,"PRE ANN : %s effac?",StrPreAut.STAN);
		} else {
			write_preauto_index(j, &StrPreAut);
			j += 1;

			//sprintf(tracepurge,"PRE AUT : %s gard?",StrPreAut.STAN);
		}

		//Os__xprintaa(tracepurge);
	}
	set_ushort_secourue(&FicPreauth.nombre_tra, j);

	//PutAllPreEmetteur();
	PutAllPre();
	JournalEditPre();
}
/****************************************************************************
*                                                                           *
*      SOUS-PROGRAMME :  Oriflame                                      *
*                                                                           *
*****************************************************************************
*                                                                           *
*      FONCTION :
*                                                                           *
*      ENTREE   :  fonction                                      			*
*                                                                           *
*      SORTIE   :  status                                                   *
*                                                                           *
****************************************************************************/
uchar	Oriflame()
{
	UL		Tmp_Amount;
	uchar	PtIso[MAXPTISO];
	uchar	update_param[6];
	uchar	update_date[11];
	uchar	Buffer[7];
	uchar	status;
	uchar	fixdevise;
	Ushort	devise_num;
	uchar	Date_sys[7];
	uchar	Time_sys[7];
	Ulong	l_ul_montant;
	Ushort 	Valide_Mont;
	uchar	l_retour,l_retour_seq;
	Ushort	indices_offline[MAX_AVIS+1];
	Ushort	nbr_tra_off_succ;
	uchar	Status_trs;
	Ushort	j;
	DATE date;
	uchar	Date_YYMMDDHHmmss[13];
//	uchar			size_carte[3];


	StructInfoPayment.Author_Status = 0xff;
	memset(indices_offline,0,sizeof(indices_offline));
	// R?cup?ration de la date/heure de la transaction
	memset(Date_sys,'\0',7);
	memset(Time_sys,'\0',5);

	Telium_Read_date(&date);
	sprintf ((char*)Date_YYMMDDHHmmss,
		"%.2s%.2s%.2s%.2s%.2s%.2s",
		date.year,
		date.month,
		date.day,
		date.hour,
		date.minute,
		date.second);
	memcpy(StructInfoPayment.Transaction_DateTime,Date_YYMMDDHHmmss,12);


	status = Prepare_Transaction();
	l_retour=DemAutor8583(update_param,update_date);   // demande aut
	switch(l_retour)
	{
		case OK :
			memcpy(StructInfoPayment.Approval_Code,R_Transac8583.champs38,6);
			memcpy(StructInfoPayment.OR_GetDebtResults,R_Transac8583.champs72,R_Transac8583.l_champs72);

			//Os__xprintaa(StructInfoPayment.OR_GetDebtResults);

			//
			//
			break ;



		case NON_AUTOR :
		case CONNEXION_KO:
		case FORMAT_KO :
		case EMISSION_KO :
		case RECEPT_KO :
		case FORMAT_MSG_REC_KO:
		default:
			break;
	}


	PutAllTra();
	PutAllPre();

	return l_retour;
}

