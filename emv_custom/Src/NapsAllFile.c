/* ======================================================================== */
/* Include Files                                                            */
/* ======================================================================== */

#include "SDK_Naps.h"
#include "_emvdctag_.h"
#include "TlvTree.h"
#include "del_lib.h"
#include "convert.h"
#include "cu_base.h"
#include "def_tag.h"
#include "cu_mess.h"
#include "cu_entry.h"
#include "gestion_param.h"
#include "cu_term.h"
#include "cu_param.h"
#include "cu_disk.h"


#define LABEL_FILE_CMI_PAR	 "CMI_PAR"

void Crypto__3DesEnc(unsigned char *Input_Data,unsigned char *Output_Data,unsigned char *bkey); //KT
void Crypto__3DesDec(unsigned char *Input_Data,unsigned char *Output_Data,unsigned char *bkey); //KT
/* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
Fichier_Parametre	Fic_Parametre;
/* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
/*-----------------------------------------------------------------
 * Fonction   : ParFile
 * objet      : Ecris tous tous les paras
 * Parametre  : Aucun
 * Retour     : Aucun
 *-----------------------------------------------------------------
*/

L	ParFile()
{
	T_Bool  ret;
	char FilePath_L[33];
	int iret;

	ret = B_TRUE;

	iret = CUDISK_CreateDisk();
	if (iret == FS_OK)
	{
		memclr(FilePath_L, sizeof(FilePath_L));
		sprintf(FilePath_L, "/%s%02x/%s",LABEL_DISK,CustApplicationNumber,LABEL_FILE_CMI_PAR);

		////APEMV_UI_MessageDisplayDelay (FilePath_L, 5); 		//Test17

		iret = FS_exist (FilePath_L);
		if (iret == FS_OK)
		{
			memclr(FilePath_L,sizeof(FilePath_L));
			sprintf(FilePath_L, "/%s%02x",LABEL_DISK,CustApplicationNumber);
			FS_unmount(FilePath_L);

			GetAllParam();
		}
		else
		{
			memclr(FilePath_L,sizeof(FilePath_L));
			sprintf(FilePath_L, "/%s%02x",LABEL_DISK,CustApplicationNumber);
			FS_unmount(FilePath_L);

			Init_Param();
		}
	}
    return (ret);
}
/*-----------------------------------------------------------------
 * Fonction   : GetAllParam
 * objet      : Ecris tous tous les paras
 * Parametre  : Aucun
 * Retour     : Aucun
 *-----------------------------------------------------------------
*/
L	GetAllParam()
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
		sprintf(FilePath_L, "/%s%02x/%s",LABEL_DISK,CustApplicationNumber,LABEL_FILE_CMI_PAR);
		file1 = FS_open (FilePath_L, "r");
		if (file1 != NULL)
		{
			FS_read((PUC)&Fic_Parametre, sizeof (Fichier_Parametre), 1, file1);
			FS_close (file1);

			memclr(FilePath_L,sizeof(FilePath_L));
			sprintf(FilePath_L, "/%s%02x",LABEL_DISK,CustApplicationNumber);
			FS_unmount(FilePath_L);

			memcpy(&FicPara			,&Fic_Parametre.FicPara				,sizeof(parametre));
			memcpy(&Fic_compte_dom	,&Fic_Parametre.Fic_compte_dom		,sizeof(Struct_compte_dom));
			memcpy(&FicBin			,&Fic_Parametre.FicBin				,sizeof(Struct_File_Bin));
			memcpy(&FicPrefixe		,&Fic_Parametre.FicPrefixe			,sizeof(Struct_File_Prefixe));
			memcpy(&FicInsReg		,&Fic_Parametre.FicInsReg			,sizeof(Struct_File_Ins_reg));
			memcpy(&Fic_Devise_Acc	,&Fic_Parametre.Fic_Devise_Acc		,sizeof(Struct_Devise_Acc));
			memcpy(&Fic_Devise_reg	,&Fic_Parametre.Fic_Devise_reg		,sizeof(Struct_devise_reg));
			memcpy(&Fic_EMV_AID		,&Fic_Parametre.Fic_EMV_AID			,sizeof(Fichier_EMV_AID));
			memcpy(&Fic_EMV_Keys	,&Fic_Parametre.Fic_EMV_Keys		,sizeof(Fichier_EMV_Keys));
			memcpy(&Fic_EMV_TAC		,&Fic_Parametre.Fic_EMV_TAC			,sizeof(Fichier_EMV_TAC));
			memcpy(&Fic_EMV_TDOL	,&Fic_Parametre.Fic_EMV_TDOL		,sizeof(Fichier_EMV_TDOL));
			memcpy(&Fic_EMV_DDOL	,&Fic_Parametre.Fic_EMV_DDOL		,sizeof(Fichier_EMV_DDOL));
			memcpy(&Fic_EMV_rand	,&Fic_Parametre.Fic_EMV_rand		,sizeof(Fichier_EMV_rand));
			memcpy(&Fic_EMV_TCC		,&Fic_Parametre.Fic_EMV_TCC			,sizeof(Fichier_EMV_TCC));
			memcpy(&Data_Last_TRS_Emv_Rej,&Fic_Parametre.Data_Last_TRS_Emv_Rej,sizeof(Struct_Data_Last_TRS_Emv_Rej));
			memcpy(&StructInfoPayment,&Fic_Parametre.StructInfoPayment ,sizeof(infopayment));//memcpy(&SaveLastInfoCaisse,&Fic_Parametre.SaveLastInfoCaisse,sizeof(MSG_CAISSE));

			return OK;
		}
	}
    return KO;
}

/*-----------------------------------------------------------------
 * Fonction   : PutAllParam
 * objet      : Ecris tous tous les paras
 * Parametre  : Aucun
 * Retour     : Aucun
 *-----------------------------------------------------------------
*/
L	PutAllParam()
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
		sprintf(FilePath_L, "/%s%02x/%s",LABEL_DISK,CustApplicationNumber,LABEL_FILE_CMI_PAR);

		iret = FS_exist (FilePath_L);
		if (iret == FS_OK)
			iret = FS_unlink (FilePath_L);

		file1 = FS_open (FilePath_L, "a");
		if (file1 != NULL)
		{
			memcpy(&Fic_Parametre.FicPara				,&FicPara			,sizeof(parametre));
			memcpy(&Fic_Parametre.Fic_compte_dom		,&Fic_compte_dom	,sizeof(Struct_compte_dom));
			memcpy(&Fic_Parametre.FicBin				,&FicBin			,sizeof(Struct_File_Bin));
			memcpy(&Fic_Parametre.FicPrefixe			,&FicPrefixe		,sizeof(Struct_File_Prefixe));
			memcpy(&Fic_Parametre.FicInsReg				,&FicInsReg			,sizeof(Struct_File_Ins_reg));
			memcpy(&Fic_Parametre.Fic_Devise_Acc		,&Fic_Devise_Acc	,sizeof(Struct_Devise_Acc));
			memcpy(&Fic_Parametre.Fic_Devise_reg		,&Fic_Devise_reg	,sizeof(Struct_devise_reg));
			memcpy(&Fic_Parametre.Fic_EMV_AID			,&Fic_EMV_AID		,sizeof(Fichier_EMV_AID));
			memcpy(&Fic_Parametre.Fic_EMV_Keys			,&Fic_EMV_Keys		,sizeof(Fichier_EMV_Keys));
			memcpy(&Fic_Parametre.Fic_EMV_TAC			,&Fic_EMV_TAC		,sizeof(Fichier_EMV_TAC));
			memcpy(&Fic_Parametre.Fic_EMV_TDOL			,&Fic_EMV_TDOL		,sizeof(Fichier_EMV_TDOL));
			memcpy(&Fic_Parametre.Fic_EMV_DDOL			,&Fic_EMV_DDOL		,sizeof(Fichier_EMV_DDOL));
			memcpy(&Fic_Parametre.Fic_EMV_rand			,&Fic_EMV_rand		,sizeof(Fichier_EMV_rand));
			memcpy(&Fic_Parametre.Fic_EMV_TCC			,&Fic_EMV_TCC		,sizeof(Fichier_EMV_TCC));
			memcpy(&Fic_Parametre.Data_Last_TRS_Emv_Rej	,&Data_Last_TRS_Emv_Rej,sizeof(Struct_Data_Last_TRS_Emv_Rej));
			memcpy(&Fic_Parametre.StructInfoPayment		,&StructInfoPayment,sizeof(infopayment));//memcpy(&Fic_Parametre.SaveLastInfoCaisse	,&SaveLastInfoCaisse,sizeof(MSG_CAISSE));

			FS_write((PUC)&Fic_Parametre, sizeof (Fichier_Parametre), 1, file1);
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
 * Fonction   : Init_Param
 * objet      : Init du fichier des parametres
 * Parametre  : Aucun
 * Retour     : Aucun
 *-----------------------------------------------------------------
*/
void	Init_Param(void)
{
	DATE date;
	uchar date_appel[7];

	ShowTrace("Init_Param");
	memset(&FicPara			,0,sizeof(parametre));
	memset(&Fic_compte_dom	,0,sizeof(Struct_compte_dom));
	memset(&FicBin			,0,sizeof(Struct_File_Bin));
	memset(&FicPrefixe		,0,sizeof(Struct_File_Prefixe));
	memset(&FicInsReg		,0,sizeof(Struct_File_Ins_reg));
	memset(&Fic_Devise_Acc	,0,sizeof(Struct_Devise_Acc));
	memset(&Fic_Devise_reg	,0,sizeof(Struct_devise_reg));
	memset(&Fic_EMV_AID		,0,sizeof(Fichier_EMV_AID));
	memset(&Fic_EMV_Keys	,0,sizeof(Fichier_EMV_Keys));
	memset(&Fic_EMV_TAC		,0,sizeof(Fichier_EMV_TAC));
	memset(&Fic_EMV_TDOL	,0,sizeof(Fichier_EMV_TDOL));
	memset(&Fic_EMV_DDOL	,0,sizeof(Fichier_EMV_DDOL));
	memset(&Fic_EMV_rand	,0,sizeof(Fichier_EMV_rand));
	memset(&Fic_EMV_TCC		,0,sizeof(Fichier_EMV_TCC));
	memset(&Data_Last_TRS_Emv_Rej,0,sizeof(Struct_Data_Last_TRS_Emv_Rej));
	memset(&StructInfoPayment,0,sizeof(infopayment));//memset(&SaveLastInfoCaisse,0,sizeof(MSG_CAISSE));

	// Initialisation des paramétres
	Os__saved_set((uchar *)&FicPara, 0x00, sizeof(FicPara));
	Os__saved_set((uchar *)&FicPara.status, KO, 1);
	Os__saved_copy((PUC)"000",(uchar *)&FicPara.Version_Pos_Param,3);
	Os__saved_copy((PUC)"000",(uchar *)&FicPara.Version_Bin_Prefixe,3);
	Os__saved_copy((PUC)"000",(uchar *)&FicPara.Version_Tab_Emv,3);
	Os__saved_copy((PUC)"000",(uchar *)&FicPara.Version_Compte_Dom,3);
	//Os__saved_set((uchar *)&FicPara.Index_compte, '0', 2);
	set_ushort_secourue(&FicPara.Index_compte,0);
	set_ushort_secourue(&FicPara.Nbr_of_redial,6);
	set_ushort_secourue(&FicPara.compteur_appel,0);
    set_ushort_secourue(&FicPara.delai_preauto,30);

	Telium_Read_date(&date);
	sprintf ((char*)date_appel,
		"%.2s%.2s%.2s",
		date.day,
		date.month,
		date.year);
	Os__saved_copy( &date_appel[0], &FicPara.Date_appel[0],6);

	Os__saved_set ((uchar *)&FicPara.Langue, FRAN, 1);
	Os__saved_copy((PUC)"000001",(uchar *)&FicPara.Num_sequence,6);
	Os__saved_copy((PUC)"000000",(uchar *)&FicPara.Num_seqAnnul,6);
	Os__saved_copy((PUC)"000001",(uchar *)&FicPara.Message_Number,6);
	Os__saved_set ((uchar *)&FicPara.Index_MK, '0', 3);
	Os__saved_set ((uchar *)&FicPara.Index_MacKey, '0', 3);
	Os__saved_set ((uchar *)&FicPara.Mac_Key, '\x01', 8);

	Os__saved_copy((PUC)"9267",(uchar *)&FicPara.Bank_Pwd,4);
	Os__saved_copy((PUC)"765453",(uchar *)&FicPara.Merchant_Pwd,6);    // 765453     larbi
	Os__saved_copy((PUC)"0729",(uchar *)&FicPara.Version_appli,4);//VersionV

	// Initialisation du fichier des bins
	set_ushort_secourue(&FicBin.Indice_fin,0);
	Os__saved_set ((uchar *)&FicBin.Tab_Bin, 0x00, sizeof(FicBin));

	Os__saved_set ((uchar *)&FicPrefixe, 0x00, sizeof(FicPrefixe));
	set_ushort_secourue(&FicPrefixe.Indice_fin,0);

	Os__saved_set ((uchar *)&FicInsReg, 0x00, sizeof(FicInsReg));
	set_ushort_secourue(&FicInsReg.Indice_fin,0);

	// Initialisation du fichier devise
	Os__saved_set ((uchar *)&Fic_Devise_Acc, 0x00, sizeof(Fic_Devise_Acc));
	set_ushort_secourue(&Fic_Devise_Acc.Indice_fin,0);

	Os__saved_set ((uchar *)&Fic_Devise_reg, 0x00, sizeof(Fic_Devise_reg));
	set_ushort_secourue(&Fic_Devise_reg.Indice_fin,0);

	// Initialisation des comptes de domiciliation
	Os__saved_set ((uchar *)&Fic_compte_dom, 0x00, sizeof(Fic_compte_dom));
	set_ushort_secourue(&Fic_compte_dom.Indice_fin,0);

	// Initialisation des fichiers des Prefixes
	Os__saved_set ((uchar *)&Fic_EMV_AID, 0x00, sizeof(Fic_EMV_AID));
	set_ushort_secourue(&Fic_EMV_AID.Indice_fin,0);

	Os__saved_set ((uchar *)&Fic_EMV_Keys, 0x00, sizeof(Fic_EMV_Keys));
	set_ushort_secourue(&Fic_EMV_Keys.Indice_fin,0);

	Os__saved_set ((uchar *)&Fic_EMV_TAC, 0x00, sizeof(Fic_EMV_TAC));
	set_ushort_secourue(&Fic_EMV_TAC.Indice_fin,0);

	Os__saved_set ((uchar *)&Fic_EMV_TDOL, 0x00, sizeof(Fic_EMV_TDOL));
	set_ushort_secourue(&Fic_EMV_TDOL.Indice_fin,0);

	Os__saved_set ((uchar *)&Fic_EMV_DDOL, 0x00, sizeof(Fic_EMV_DDOL));
	set_ushort_secourue(&Fic_EMV_DDOL.Indice_fin,0);

	Os__saved_set ((uchar *)&Fic_EMV_rand, 0x00, sizeof(Fic_EMV_rand));
	set_ushort_secourue(&Fic_EMV_rand.Indice_fin,0);

	Os__saved_set ((uchar *)&Fic_EMV_TCC, 0x00, sizeof(Fic_EMV_TCC));
	set_ushort_secourue(&Fic_EMV_TCC.Indice_fin,0);

	FicPara.var_pourboire = '1';

	PutAllParam();
}

//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			NOM			: Trait_transfer_param
//			DESCRIPTION	: Traitement de la recuperation des POS parmeters
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			ENTREE		: Rien
//
//			SORTIE		:resultat de la transmisson
//
//			REMARQUE	:
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
uchar	Trait_transfer_param(uchar Premier_Mess)
{
	char			chaine[5];
	Ushort			i;
	char			Montant_min[5];
	Ushort			Ind_fin;

	devise_accept 	Dev_Acc;
	devise_reg 		Dev_Reg;

	uchar			Tel[17];
	uchar			Id_X25[16];

	Ushort	OffSet;
	parametre  Param_tmp;


	//ShowTrace("TTP0");
	if(R_Transac8583.l_champs72!=500)
		return KO;

	if(memcmp(R_Transac8583.champs24,"300", 3) && memcmp(R_Transac8583.champs24,"308", 3))
   		return KO;

	Os__saved_set((uchar *)&FicPara.status, KO, 1);
	memcpy((uchar *)&Param_tmp,(uchar *)&FicPara,sizeof(FicPara));

	OffSet = (Ushort)12;

	//ShowTrace("TTP1");
	memcpy(Param_tmp.Type_Terminnal,&R_Transac8583.champs72[OffSet],2);
	OffSet += (Ushort)2;
	memcpy(Param_tmp.Type_Appli,&R_Transac8583.champs72[OffSet],2);
	OffSet += (Ushort)2;
	memcpy(Param_tmp.Nom_Appli,&R_Transac8583.champs72[OffSet],10);
	OffSet += (Ushort)10;
	memcpy(Param_tmp.Merchant_Name,&R_Transac8583.champs72[OffSet],24);
	OffSet += (Ushort)24;
	memcpy(Param_tmp.Merchant_Adrees,&R_Transac8583.champs72[OffSet],24);
	OffSet += (Ushort)24;
	memcpy(Param_tmp.MCC,&R_Transac8583.champs72[OffSet],4);
	OffSet += (Ushort)4;
	memcpy(Param_tmp.Merchant_City,&R_Transac8583.champs72[OffSet],13);
	OffSet += (Ushort)13;
	memcpy(Param_tmp.pass_word,&R_Transac8583.champs72[OffSet],4);
	OffSet += (Ushort)4;
	memcpy(Param_tmp.Indic_accept_carte,&R_Transac8583.champs72[OffSet],9);
	OffSet += (Ushort)9;
	memcpy(Param_tmp.Num_serie_terminal,&R_Transac8583.champs72[OffSet],10);
	OffSet += (Ushort)10;
	memcpy(Param_tmp.Num_reg_commerce,&R_Transac8583.champs72[OffSet],9);
	OffSet += (Ushort)9;
	memcpy(Param_tmp.Heure_appel,&R_Transac8583.champs72[OffSet],4);
	OffSet += (Ushort)4;
	memcpy(Param_tmp.logo,&R_Transac8583.champs72[OffSet],11);
	OffSet += (Ushort)11;
	memset(chaine,'\0',5);
	memcpy(chaine,&R_Transac8583.champs72[OffSet],2);
	OffSet += (Ushort)2;
	Param_tmp.Max_trs_off = (Ushort)atol(chaine);
	memset(chaine,'\0',5);
	memcpy(chaine,&R_Transac8583.champs72[OffSet],2);
	OffSet += (Ushort)2;
	Param_tmp.Max_avis = (Ushort)atol(chaine);
	memcpy(Param_tmp.capture_date,&R_Transac8583.champs72[OffSet],4);
	OffSet += (Ushort)4;
	Param_tmp.Indic_pinpad = R_Transac8583.champs72[OffSet];
	OffSet += (Ushort)1;
	Param_tmp.Accept_Man = R_Transac8583.champs72[OffSet];
	OffSet += (Ushort)1;
	Param_tmp.Indic_TimeOut_Val = R_Transac8583.champs72[OffSet];
	OffSet += (Ushort)1;
	Param_tmp.Indic_TimeOut_Val_annul = R_Transac8583.champs72[OffSet];
	OffSet += (Ushort)1;
	Param_tmp.Accept_puce = R_Transac8583.champs72[OffSet];
	OffSet += (Ushort)1;
	Param_tmp.Indic_impression = R_Transac8583.champs72[OffSet];
	OffSet += (Ushort)1;
	Param_tmp.Indic_annul = R_Transac8583.champs72[OffSet];
	OffSet += (Ushort)1;
	Param_tmp.Indic_forcage = R_Transac8583.champs72[OffSet];
	OffSet += (Ushort)1;
	Param_tmp.Indic_Change_Cpte_Dom = R_Transac8583.champs72[OffSet];
	OffSet += (Ushort)1;
	Param_tmp.Indic_PreAutor = R_Transac8583.champs72[OffSet];
	OffSet += (Ushort)1;
	Param_tmp.type_appel = R_Transac8583.champs72[OffSet];
	OffSet += (Ushort)1;
	Param_tmp.Indic_OffShort = R_Transac8583.champs72[OffSet];
	OffSet += (Ushort)1;
	Param_tmp.Indic_Refund = R_Transac8583.champs72[OffSet];
	OffSet += (Ushort)1;
	Param_tmp.Indic_CashBack = R_Transac8583.champs72[OffSet];
	OffSet += (Ushort)1;
	Param_tmp.Indic_TIP = R_Transac8583.champs72[OffSet];
	OffSet += (Ushort)1;
	Param_tmp.Indic_Instalment = R_Transac8583.champs72[OffSet];
	OffSet += (Ushort)1;
	Param_tmp.Indic_Offline = R_Transac8583.champs72[OffSet];
	OffSet += (Ushort)1;
	Param_tmp.Appli_type = R_Transac8583.champs72[OffSet];
	OffSet += (Ushort)1;
	OffSet += (Ushort)12; // Reservé pour utilisation futur

	//ShowTrace("TTP2");
	memset(Param_tmp.Tel_autor_pri,0,sizeof(Param_tmp.Tel_autor_pri));
	memset(Tel,0,17);
	memcpy(Tel,&R_Transac8583.champs72[OffSet],16);
	OffSet += (Ushort)16;
	for(i=0;((i<17)&&((Tel[i]!=' ')&&(Tel[i]!='\0')));i+=(Ushort)1);
	memcpy(Param_tmp.Tel_autor_pri,Tel,i);

	memset(Param_tmp.X25_autor_pri,0,sizeof(Param_tmp.X25_autor_pri));
	memset(Id_X25,0,16);
	memcpy(Id_X25,&R_Transac8583.champs72[OffSet],15);
	OffSet += (Ushort)15;
	for(i=0;((i<16)&&((Id_X25[i]!=' ')&&(Id_X25[i]!='\0')));i+=(Ushort)1);
	memcpy(Param_tmp.X25_autor_pri,Id_X25,i);

	memset(Param_tmp.Tel_telec_pri,0,sizeof(Param_tmp.Tel_telec_pri));
	memset(Tel,0,17);
	memcpy(Tel,&R_Transac8583.champs72[OffSet],16);
	OffSet += (Ushort)16;
	for(i=0;((i<17)&&((Tel[i]!=' ')&&(Tel[i]!='\0')));i+=(Ushort)1);
	memcpy(Param_tmp.Tel_telec_pri,Tel,i);

	memset(Param_tmp.X25_telec_pri,0,sizeof(Param_tmp.X25_telec_pri));
	memset(Id_X25,0,16);
	memcpy(Id_X25,&R_Transac8583.champs72[OffSet],15);
	OffSet += (Ushort)15;
	for(i=0;((i<16)&&((Id_X25[i]!=' ')&&(Id_X25[i]!='\0')));i+=(Ushort)1);
	memcpy(Param_tmp.X25_telec_pri,Id_X25,i);

	memset(Param_tmp.Tel_telech_pri,0,sizeof(Param_tmp.Tel_telech_pri));
	memset(Tel,0,17);
	memcpy(Tel,&R_Transac8583.champs72[OffSet],16);
	OffSet += (Ushort)16;
	for(i=0;((i<17)&&((Tel[i]!=' ')&&(Tel[i]!='\0')));i+=(Ushort)1);
	memcpy(Param_tmp.Tel_telech_pri,Tel,i);

	memset(Param_tmp.X25_telech_pri,0,sizeof(Param_tmp.X25_telech_pri));
	memset(Id_X25,0,16);
	memcpy(Id_X25,&R_Transac8583.champs72[OffSet],15);
	OffSet += (Ushort)15;
	for(i=0;((i<16)&&((Id_X25[i]!=' ')&&(Id_X25[i]!='\0')));i+=(Ushort)1);
	memcpy(Param_tmp.X25_telech_pri,Id_X25,i);

	memset(Param_tmp.Tel_autor_sec,0,sizeof(FicPara.Tel_autor_sec));
	memset(Tel,0,17);
	memcpy(Tel,&R_Transac8583.champs72[OffSet],16);
	OffSet += (Ushort)16;
	for(i=0;((i<17)&&((Tel[i]!=' ')&&(Tel[i]!='\0')));i+=(Ushort)1);
	memcpy(Param_tmp.Tel_autor_sec,Tel,i);

	memset(Param_tmp.X25_autor_sec,0,sizeof(Param_tmp.X25_autor_sec));
	memset(Id_X25,0,16);
	memcpy(Id_X25,&R_Transac8583.champs72[OffSet],15);
	OffSet += (Ushort)15;
	for(i=0;((i<16)&&((Id_X25[i]!=' ')&&(Id_X25[i]!='\0')));i+=(Ushort)1);
	memcpy(Param_tmp.X25_autor_sec,Id_X25,i);

	memset(Param_tmp.Tel_telec_sec,0,sizeof(Param_tmp.Tel_telec_sec));
	memset(Tel,0,17);
	memcpy(Tel,&R_Transac8583.champs72[OffSet],16);
	OffSet += (Ushort)16;
	for(i=0;((i<17)&&((Tel[i]!=' ')&&(Tel[i]!='\0')));i+=(Ushort)1);
	memcpy(Param_tmp.Tel_telec_sec,Tel,i);

	memset(Param_tmp.X25_telec_sec,0,sizeof(Param_tmp.X25_telec_sec));
	memset(Id_X25,0,16);
	memcpy(Id_X25,&R_Transac8583.champs72[OffSet],15);
	OffSet += (Ushort)15;
	for(i=0;((i<16)&&((Id_X25[i]!=' ')&&(Id_X25[i]!='\0')));i+=(Ushort)1);
	memcpy(Param_tmp.X25_telec_sec,Id_X25,i);

	memset(Param_tmp.Tel_telech_sec,0,sizeof(Param_tmp.Tel_telech_sec));
	memset(Tel,0,17);
	memcpy(Tel,&R_Transac8583.champs72[OffSet],16);
	OffSet += (Ushort)16;
	for(i=0;((i<17)&&((Tel[i]!=' ')&&(Tel[i]!='\0')));i+=(Ushort)1);
	memcpy(Param_tmp.Tel_telech_sec,Tel,i);

	memset(Param_tmp.X25_telech_sec,0,sizeof(Param_tmp.X25_telech_sec));
	memset(Id_X25,0,16);
	memcpy(Id_X25,&R_Transac8583.champs72[OffSet],15);
	OffSet += (Ushort)15;
	for(i=0;((i<16)&&((Id_X25[i]!=' ')&&(Id_X25[i]!='\0')));i+=(Ushort)1);
	memcpy(Param_tmp.X25_telech_sec,Id_X25,i);

	//ShowTrace("TTP3");
	Ind_fin = 0 ;
	Os__saved_set((uchar *)&Fic_Devise_Acc,0,sizeof(Fic_Devise_Acc));
	for(i=0;(i<6);i+=(Ushort)1)
	{
   		if(R_Transac8583.champs72[OffSet]==' ')
			OffSet += (Ushort)11;
		else
		{
			memset((char *)&Dev_Acc,'\0',sizeof(Dev_Acc));
			memcpy(Dev_Acc.code_num,&R_Transac8583.champs72[OffSet],3);
			OffSet += (Ushort)3;
			memcpy(Dev_Acc.code_alph,&R_Transac8583.champs72[OffSet],3);
			OffSet += (Ushort)3;
			Dev_Acc.Exposant = R_Transac8583.champs72[OffSet];
			OffSet += (Ushort)1;
			memset(Montant_min,'\0',5);
			memcpy(Montant_min,&R_Transac8583.champs72[OffSet],4);
			OffSet += (Ushort)4;
			Dev_Acc.Montant_min = (Ulong)atol(Montant_min)*100;

			Os__saved_copy((uchar *)&Dev_Acc,(uchar *)&Fic_Devise_Acc.TabDevise[Ind_fin],sizeof(Dev_Acc));
			Ind_fin += (Ushort)1;
			set_ushort_secourue(&Fic_Devise_Acc.Indice_fin,Ind_fin);
		}
	}

	//ShowTrace("TTP4");
	Ind_fin = 0 ;
	Os__saved_set((uchar *)&Fic_Devise_reg,0,sizeof(Fic_Devise_reg));
	for(i=0;(i<5);i+=(Ushort)1)
	{
		if(R_Transac8583.champs72[OffSet]==' ')
			OffSet += (Ushort)7;
		else
		{
			memset((char *)&Dev_Reg,'\0',sizeof(Dev_Reg));
			memcpy(Dev_Reg.code_num,&R_Transac8583.champs72[OffSet],3);
			OffSet += (Ushort)3;
			memcpy(Dev_Reg.code_alph,&R_Transac8583.champs72[OffSet],3);
			OffSet += (Ushort)3;
			Dev_Reg.Exposant = R_Transac8583.champs72[OffSet];
			OffSet += (Ushort)1;

			Os__saved_copy((uchar *)&Dev_Reg,(uchar *)&Fic_Devise_reg.TabDevise[Ind_fin],sizeof(Dev_Reg));
			Ind_fin += (Ushort)1;
			set_ushort_secourue(&Fic_Devise_reg.Indice_fin,Ind_fin);
		}
	}
	OffSet += (Ushort)8; //Reservé pour utilisation futur

	//ShowTrace("TTP5");
	if(R_Transac8583.champs72[OffSet]=='3')
		Param_tmp.Langue = ARABE;
	else if(R_Transac8583.champs72[OffSet]=='2')
		Param_tmp.Langue = ENG;
	else
		Param_tmp.Langue = FRAN;
	OffSet += (Ushort)1;

	//if(Param_tmp.Indic_TimeOut_Val=='1')
	if(Param_tmp.Indic_TimeOut_Val)//garantie de garrag 110505
	{
		memset(chaine,'\0',5);
		memcpy(chaine,&R_Transac8583.champs72[OffSet],3);
		Param_tmp.TimeOut = (Ushort)atol(chaine);
	}
	else
		Param_tmp.TimeOut = 0;
	OffSet += (Ushort)3;

	memset(chaine,'\0',5);
	memcpy(chaine,&R_Transac8583.champs72[OffSet],1);
	OffSet += (Ushort)1;
	Param_tmp.Nbr_of_redial = (Ushort)atol(chaine);

	memset(chaine,'\0',5);
	memcpy(chaine,&R_Transac8583.champs72[OffSet],3);
	OffSet += (Ushort)3;
	Param_tmp.redial_delay = (Ushort)atol(chaine);

	Param_tmp.Ctrl_fraude = R_Transac8583.champs72[OffSet];
	OffSet += (Ushort)1;


	//ShowTrace("TTP6");
	Os__saved_copy((uchar *)&Param_tmp,(uchar *)&FicPara,sizeof(FicPara));

	if(memcmp(R_Transac8583.champs24,"300", 3)==0)
	{
		if(R_Transac8583.l_champs63 > 19)
			Os__saved_copy(&R_Transac8583.champs63[16],(uchar *)&FicPara.Version_Pos_Param,3);
			Os__saved_copy(&R_Transac8583.champs63[12],(uchar *)&FicPara.Version_Opp,4);//badr

		/*AutomaticJob();*/
		return	FIN_TRANSFER;
	}

	return KO;
}
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			NOM			: Trait_transfer_Key
//			DESCRIPTION	: Traitement de la recuperation de la MasterKey
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			ENTREE		: Rien
//
//			SORTIE		:resultat de la transmisson
//
//			REMARQUE	:
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
uchar	Trait_transfer_Key(uchar Premier_Mess)
{
	uchar	MK_Crypt[48];
	uchar	MK_Crypt_asc[24];

	uchar	MK_check_value[16];
	uchar	MK_check_value_asc[8];
	uchar	MK_Index[3];
	uchar	MK_Clair_asc[24];
	uchar	chaine[8],bkey[24+1],Input[8+1];

	uchar	Methode_Encry;

	if(R_Transac8583.l_champs72<68)
		return KO;

	Os__saved_set((uchar *)&FicPara.status, KO, 1);

	memcpy(MK_Crypt,&R_Transac8583.champs72[0],48);

	////APEMV_UI_MessageDisplayDelay(MK_Crypt, 3);

	memcpy(MK_check_value,&R_Transac8583.champs72[48],16);

	////APEMV_UI_MessageDisplayDelay(MK_check_value, 3);

	memcpy(MK_Index,&R_Transac8583.champs72[64],3);

	////APEMV_UI_MessageDisplayDelay(MK_Index, 3);

	Methode_Encry = R_Transac8583.champs72[67];


	if((Methode_Encry!='T')&&(Methode_Encry!='D'))
		return KO;

	asc_hex(MK_Crypt_asc,24,MK_Crypt,48);
	asc_hex(MK_check_value_asc,8,MK_check_value,16);



	//APEMV_UI_MessageDisplayDelayHexa(MK_Crypt_asc,24, 3);
	//APEMV_UI_MessageDisplayDelayHexa(MK_check_value_asc,8, 3);


	memset(bkey,0,sizeof(bkey));
	memcpy(bkey,   "\xD0\xCB\x9D\xF7\x2A\xAD\xAD\x38",8);
	memcpy(bkey+8, "\x37\x3E\x79\xB6\x02\xE0\xEA\x07",8);
	memcpy(bkey+16,"\xD0\xCB\x9D\xF7\x2A\xAD\xAD\x38",8);
	

	Crypto__3DesDec(MK_Crypt_asc,MK_Clair_asc,bkey);
	Crypto__3DesDec(MK_Crypt_asc+8,MK_Clair_asc+8,bkey);
	Crypto__3DesDec(MK_Crypt_asc+16,MK_Clair_asc+16,bkey);

	memset(Input,0,sizeof(Input));
	Crypto__3DesEnc(Input,chaine,MK_Clair_asc);


	//APEMV_UI_MessageDisplayDelayHexa(chaine,8, 3);

	if(memcmp(chaine,MK_check_value_asc,8)==0)
	{
		Os__saved_copy(MK_Clair_asc,FicPara.Master_Key,24);
		Os__saved_copy(MK_check_value_asc,FicPara.MK_check_Value,8);
		Os__saved_copy(MK_Index,FicPara.Index_MK,3);
		Os__saved_set((uchar *)&FicPara.Methode_Encry, Methode_Encry, 1);
	}
	else
	{
		//APEMV_UI_MessageDisplayDelay ("Pb check value MasterKey", 2);
		return KO;
	}

	////APEMV_UI_MessageDisplayDelay ("MasterKey.OK", 2);

	if(memcmp(R_Transac8583.champs24,"300", 3)==0)
	   return	FIN_TRANSFER;

	return KO;

}

//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			NOM			: Trait_transfer_MacKey
//			DESCRIPTION	: Traitement de la recuperation de la MACKEY
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			ENTREE		: Rien
//
//			SORTIE		:resultat de la transmisson
//
//			REMARQUE	:
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
uchar	Trait_transfer_MacKey(uchar Premier_Mess)
{
	uchar	MacKey_Crypt[16];
	uchar	MacKey_Crypt_asc[8];
	uchar	MacKey_check_value[16];
	uchar	MacKey_check_value_asc[8];
	uchar	MacKey_Index[3];
	uchar	MacKey_Clair_asc[8];
	uchar	chaine[8],Input[8+1];

	if(R_Transac8583.l_champs72<35)
		return KO;

	Os__saved_set((uchar *)&FicPara.status, KO, 1);

	memcpy(MacKey_Crypt,&R_Transac8583.champs72[0],16);
	memcpy(MacKey_check_value,&R_Transac8583.champs72[16],16);
	memcpy(MacKey_Index,&R_Transac8583.champs72[32],3);

	asc_hex(MacKey_Crypt_asc,8,MacKey_Crypt,16);
	asc_hex(MacKey_check_value_asc,8,MacKey_check_value,16);

	Crypto__3DesDec(MacKey_Crypt_asc,MacKey_Clair_asc,FicPara.Master_Key);


	memset(Input,0,sizeof(Input));
	des(Input,chaine,MacKey_Clair_asc);

//	return	FIN_TRANSFER;					//Test17
	
	if(memcmp(chaine,MacKey_check_value_asc,8)==0)
	{
		//Os__saved_swap(0);
		Os__saved_copy(MacKey_Clair_asc,FicPara.Mac_Key,8);
		Os__saved_copy(MacKey_check_value_asc,FicPara.MacKey_check_Value,8);
		Os__saved_copy(MacKey_Index,FicPara.Index_MacKey,3);
	}
	else // Probleme check value
	{
		//APEMV_UI_MessageDisplayDelay ("Pb check value MacKey", 2);
		return KO;
	}

   if(memcmp(R_Transac8583.champs24,"300", 3)==0)
	   return	FIN_TRANSFER;

	return KO;
}

//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			NOM			: Maj_bin
//			DESCRIPTION	: Gestion de la table des bin (Ajout,modif et suppression de BINs)
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			ENTREE		: Type_maj :
//
//			SORTIE		:resultat de la transmisson
//
//			REMARQUE	:
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
uchar	Maj_bin(uchar Type_maj)
{
	accepted_bin	BIN;
	Ushort			Ind_fin,i,j,m;
	char			Chaine[16];
	uchar			FS1,FS2;

	FS1 = '*';
	FS2 = '?';

	switch(Type_maj)
	{
		case AJOUT_ENREG:
			i=0;
			while(i<R_Transac8583.l_champs72)
			{
				Ind_fin = FicBin.Indice_fin ;
				memset((uchar *)&BIN,'\0',sizeof(BIN));


				for(j=0;(((i+j)<R_Transac8583.l_champs72)&&(R_Transac8583.champs72[i+j]!=FS1));j+=1);
				if(j>11)
					return KO;
				memset(BIN.Bin_Debut,'0',sizeof(BIN.Bin_Debut));
				memcpy(BIN.Bin_Debut,&R_Transac8583.champs72[i],j);
				i+= (j+1) ;

				for(j=0;(((i+j)<R_Transac8583.l_champs72)&&(R_Transac8583.champs72[i+j]!=FS1));j+=1);
				if(j>11)
					return KO;
				memset(BIN.Bin_Fin,'9',sizeof(BIN.Bin_Fin));
				memcpy(BIN.Bin_Fin,&R_Transac8583.champs72[i],j);
				i+= (j+1) ;

				memset(Chaine,0,sizeof(Chaine));
				memcpy(Chaine,&R_Transac8583.champs72[i],2);
				i+= 2 ;
				BIN.Code_Prefixe = (Ushort)atol(Chaine);
				if(R_Transac8583.champs72[i]!=FS1)
					return KO;
				i+= (Ushort)1;

				BIN.LgCard_Type = R_Transac8583.champs72[i];
				i+= 1;
				if(R_Transac8583.champs72[i]!=FS1)
					return KO;
				i+= (Ushort)1;

				memset(Chaine,0,sizeof(Chaine));
				memcpy(Chaine,&R_Transac8583.champs72[i],2);
				BIN.LgCard_Min = (Ushort)atol(Chaine);
				i+= (Ushort)2;
				if(R_Transac8583.champs72[i]!=FS1)
					return KO;
				i+= (Ushort)1;

				memset(Chaine,0,sizeof(Chaine));
				memcpy(Chaine,&R_Transac8583.champs72[i],2);
				BIN.LgCard_Max = (Ushort)atol(Chaine);
				i+= (Ushort)2;
				if(R_Transac8583.champs72[i]!=FS1)
					return KO;
				i+= (Ushort)1;

				BIN.OffSet_Octet36 = R_Transac8583.champs72[i] - 0x30;
				i+= (Ushort)1;
				if(R_Transac8583.champs72[i]!=FS1)
					return KO;
				i+= (Ushort)1;

				memcpy(BIN.Code_Service,&R_Transac8583.champs72[i],3);
				i+= (Ushort)3;
				if(R_Transac8583.champs72[i]!=FS1)
					return KO;
				i+= (Ushort)1;

				BIN.Langue = R_Transac8583.champs72[i];
				i+= (Ushort)1;
				if(R_Transac8583.champs72[i]!=FS1)
					return KO;
				i+= (Ushort)1;

				BIN.Gest_Pin = R_Transac8583.champs72[i];
				i+= (Ushort)1;
				if(R_Transac8583.champs72[i]!=FS1)
					return KO;
				i+= (Ushort)1;

				memset(Chaine,0,sizeof(Chaine));
				for(j=0;(((i+j)<R_Transac8583.l_champs72)&&(R_Transac8583.champs72[i+j]!=FS2));j+=1);
				memcpy(Chaine,&R_Transac8583.champs72[i],j);
				i+= j ;
				BIN.Plafond = atol(Chaine)*100;
				if(R_Transac8583.champs72[i]!=FS2)
					return KO;
				i+= (Ushort)1;

				if(Ind_fin>=NB_MAX_BIN)
					return KO;
				Os__saved_copy((uchar *)&BIN,(uchar *)&FicBin.Tab_Bin[Ind_fin],sizeof(BIN));
				m=Ind_fin+1;
				set_ushort_secourue(&FicBin.Indice_fin,m);
			}
			break;

		case MODIF_ENREG:
		case SUPP_ENREG:
			break;

	}
	return OK;


}

//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			NOM			: Maj_Prefixe
//			DESCRIPTION	: Gestion des prefixe  (Ajout,modif et suppression)
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			ENTREE		: Type_maj :
//
//			SORTIE		:resultat de la transmisson
//
//			REMARQUE	:
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
uchar	Maj_Prefixe(uchar Type_maj)
{
	Ushort				Ind_fin_Ins_reg,Ind_fin_Pre,i,m;
	char				Chaine[16];
	uchar				*pSep1;

	Struct_Ins_reg		Inst_Reg;
	Accepted_prefixe	St_Prefixe;

	uchar				FS1,FS2;

	FS1 = '*';
	FS2 = '?';

	i = 0;
	switch(Type_maj)
	{
		case AJOUT_ENREG:
			while(i<R_Transac8583.l_champs72)
			{
				Ind_fin_Ins_reg = FicInsReg.Indice_fin ;
				memset((uchar *)&Inst_Reg,'\0',sizeof(Inst_Reg));

				pSep1 = (PUC)strchr((PC)&R_Transac8583.champs72[i],FS1) ;
				if((pSep1)&&((pSep1-&R_Transac8583.champs72[i])<=2))
				{
					memset(Chaine,0,sizeof(Chaine));
					memcpy(Chaine,&R_Transac8583.champs72[i],pSep1-&R_Transac8583.champs72[i]);
					Inst_Reg.Code_Prefixe = (Ushort)atol(Chaine);
				}
				else
					return KO;
				i+=(Ushort)((pSep1-&R_Transac8583.champs72[i])+1);

				pSep1 = (PUC)strchr((PC)&R_Transac8583.champs72[i],FS1) ;
				if((pSep1)&&((pSep1-&R_Transac8583.champs72[i])<=16))
				{
					memcpy(Inst_Reg.Libelle,&R_Transac8583.champs72[i],pSep1-&R_Transac8583.champs72[i]);
				}
				else
					return KO;
				i+=(Ushort)((pSep1-&R_Transac8583.champs72[i])+1);

				pSep1 = (PUC)strchr((PC)&R_Transac8583.champs72[i],FS1) ;
				if((pSep1)&&((pSep1-&R_Transac8583.champs72[i])<=16))
					memcpy(Inst_Reg.Num_point_vente,&R_Transac8583.champs72[i],pSep1-&R_Transac8583.champs72[i]);
				else
					return KO;
				i+=(Ushort)((pSep1-&R_Transac8583.champs72[i])+1);

				////////////////////////////////////////
				while((i<R_Transac8583.l_champs72)&&(R_Transac8583.champs72[i]!=FS2))
				{
					Ind_fin_Pre = FicPrefixe.Indice_fin ;
					memset((uchar *)&St_Prefixe,'\0',sizeof(St_Prefixe));

					pSep1 = (PUC)strchr((PC)&R_Transac8583.champs72[i],FS1);
					if((pSep1)&&((pSep1-&R_Transac8583.champs72[i])<=11))
					{
						memset(St_Prefixe.Pre_Debut,'0',sizeof(St_Prefixe.Pre_Debut));
						memcpy(St_Prefixe.Pre_Debut,&R_Transac8583.champs72[i],pSep1-&R_Transac8583.champs72[i]);
					}
					else
						return KO;
					i+=(Ushort)((pSep1-&R_Transac8583.champs72[i])+1);

					pSep1 = (PUC)strchr((PC)&R_Transac8583.champs72[i],FS1);
					if((pSep1)&&((pSep1-&R_Transac8583.champs72[i])<=11))
					{
						memset(St_Prefixe.Pre_Fin,'9',sizeof(St_Prefixe.Pre_Fin));
						memcpy(St_Prefixe.Pre_Fin,&R_Transac8583.champs72[i],pSep1-&R_Transac8583.champs72[i]);
					}
					else
						return KO;
					i+=(Ushort)((pSep1-&R_Transac8583.champs72[i])+1);

					pSep1 = (PUC)strchr((PC)&R_Transac8583.champs72[i],FS1);
					if((pSep1)&&((pSep1-&R_Transac8583.champs72[i])==1))
						St_Prefixe.OffSet_Octet36 = R_Transac8583.champs72[i] - 0x30;
					else
						return KO;
					i+=(Ushort)((pSep1-&R_Transac8583.champs72[i])+1);

					pSep1 = (PUC)strchr((PC)&R_Transac8583.champs72[i],FS1);
					if((pSep1)&&((pSep1-&R_Transac8583.champs72[i])==1))
						St_Prefixe.LgCard_Type = R_Transac8583.champs72[i];
					else
						return KO;
					i+=(Ushort)((pSep1-&R_Transac8583.champs72[i])+1);

					pSep1 = (PUC)strchr((PC)&R_Transac8583.champs72[i],FS1);
					if((pSep1)&&((pSep1-&R_Transac8583.champs72[i])==2))
					{
						memset(Chaine,0,sizeof(Chaine));
						memcpy(Chaine,&R_Transac8583.champs72[i],2);
						St_Prefixe.LgCard_Min = (Ushort)atol(Chaine);
					}
					else
						return KO;
					i+=(Ushort)((pSep1-&R_Transac8583.champs72[i])+1);

					pSep1 = (PUC)strchr((PC)&R_Transac8583.champs72[i],FS1);
					if((pSep1)&&((pSep1-&R_Transac8583.champs72[i])==2))
					{
						memset(Chaine,0,sizeof(Chaine));
						memcpy(Chaine,&R_Transac8583.champs72[i],2);
						St_Prefixe.LgCard_Max = (Ushort)atol(Chaine);
					}
					else
						return KO;
					i+=(Ushort)((pSep1-&R_Transac8583.champs72[i])+1);

					St_Prefixe.Gest_Pin = R_Transac8583.champs72[i];
					i+=(Ushort)1;

					St_Prefixe.Indice_Ins_Reg = Ind_fin_Ins_reg;

					if(Ind_fin_Pre>=NB_MAX_PRE)
						return KO;

					Os__saved_copy((uchar *)&St_Prefixe,(uchar *)&FicPrefixe.Tab_Prefixe[Ind_fin_Pre],sizeof(St_Prefixe));
					m=Ind_fin_Pre+1;
					set_ushort_secourue(&FicPrefixe.Indice_fin,m);

					if(R_Transac8583.champs72[i]==FS2)
					{
						i+=(Ushort)1;
						break;
					}
					if(R_Transac8583.champs72[i]!=FS1)
						return KO;
					i+=(Ushort)1;
				}
				/////////////////////////////////////////
				pSep1 = (PUC)strchr((PC)&R_Transac8583.champs72[i],FS1) ;
				if((pSep1)&&((pSep1-&R_Transac8583.champs72[i])<=64))
					memcpy(Inst_Reg.Message1,&R_Transac8583.champs72[i],pSep1-&R_Transac8583.champs72[i]);
				else
					return KO;
				i+=(Ushort)((pSep1-&R_Transac8583.champs72[i])+1);

				pSep1 = (PUC)strchr((PC)&R_Transac8583.champs72[i],FS1) ;
				if((pSep1)&&((pSep1-&R_Transac8583.champs72[i])<=64))
					memcpy(Inst_Reg.Message2,&R_Transac8583.champs72[i],pSep1-&R_Transac8583.champs72[i]);
				else
					return KO;
				i+=(Ushort)((pSep1-&R_Transac8583.champs72[i])+1);


				pSep1 = (PUC)strchr((PC)&R_Transac8583.champs72[i],FS2) ;
				if((pSep1)&&((pSep1-&R_Transac8583.champs72[i])<=15))
				{
					memset(Chaine,0,sizeof(Chaine));
					memcpy(Chaine,&R_Transac8583.champs72[i],pSep1-&R_Transac8583.champs72[i]);
					Inst_Reg.Plafond = atol(Chaine)*100;
				}
				else
					return KO;
				i+=(Ushort)((pSep1-&R_Transac8583.champs72[i])+1);

				if(Ind_fin_Ins_reg>=NB_MAX_INS_REG)
					return KO;

				Os__saved_copy((uchar *)&Inst_Reg,(uchar *)&FicInsReg.Tab_Ins_reg[Ind_fin_Ins_reg],sizeof(Inst_Reg));
				m=Ind_fin_Ins_reg+1;
				set_ushort_secourue(&FicInsReg.Indice_fin,m);
			}
			break;

		case MODIF_ENREG:
		case SUPP_ENREG:
			break;
	}
	return OK;
}


//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			NOM			: Trait_transfer_BIN
//			DESCRIPTION	: Traitement de la recuperation de la table des Bins
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			ENTREE		: Rien
//
//			SORTIE		:resultat de la transmisson
//
//			REMARQUE	:
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
uchar	Trait_transfer_BIN(uchar Premier_Mess)
{
	uchar	Status;

	if((Premier_Mess)||(memcmp(R_Transac8583.champs24,"308", 3)==0)||(memcmp(R_Transac8583.champs24,"306", 3)==0))
	{
		set_ushort_secourue(&FicBin.Indice_fin,0);
		Os__saved_set ((uchar *)&FicBin.Tab_Bin, 0x00, sizeof(FicBin));
	}

	Os__saved_set((uchar *)&FicPara.status, KO, 1);

	Status = Maj_bin(AJOUT_ENREG);
	if(Status != OK)
		return KO;

	if(memcmp(R_Transac8583.champs24,"300", 3)==0)
		return	FIN_TRANSFER;

	if((memcmp(R_Transac8583.champs24,"301", 3)==0)||
		(memcmp(R_Transac8583.champs24,"308", 3)==0)||
      (memcmp(R_Transac8583.champs24,"306", 3)==0)
	  )
		return OK;

	return KO;
}

//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			NOM			: Trait_transfer_Prefixes
//			DESCRIPTION	: Traitement de la recuperation des Prefixes
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			ENTREE		: Rien
//
//			SORTIE		:resultat de la transmisson
//
//			REMARQUE	:
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
uchar	Trait_transfer_Prefixes(uchar Premier_Mess)
{
	Ushort	i,OffSet;
	uchar	Status;


	if((Premier_Mess)||(memcmp(R_Transac8583.champs24,"308", 3)==0)||(memcmp(R_Transac8583.champs24,"306", 3)==0))
	{
		Os__saved_set ((uchar *)&FicPrefixe, 0x00, sizeof(FicPrefixe));
		set_ushort_secourue(&FicPrefixe.Indice_fin,0);

		Os__saved_set ((uchar *)&FicInsReg, 0x00, sizeof(FicInsReg));
		set_ushort_secourue(&FicInsReg.Indice_fin,0);

	}

	Os__saved_set((uchar *)&FicPara.status, KO, 1);

	i = 0;
	OffSet = 0;
	Status = OK;

	Status = Maj_Prefixe(AJOUT_ENREG);
	if(Status != OK)
		return KO;

	if(memcmp(R_Transac8583.champs24,"300", 3)==0)
	{
		if(R_Transac8583.l_champs63 > 19)
			Os__saved_copy(&R_Transac8583.champs63[9],(uchar *)&FicPara.Version_Bin_Prefixe,3);
		return	FIN_TRANSFER;
	}

	if((memcmp(R_Transac8583.champs24,"301", 3)==0)||
		(memcmp(R_Transac8583.champs24,"308", 3)==0)||
		(memcmp(R_Transac8583.champs24,"306", 3)==0)
	  )
		return OK;

	return KO;

}

//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			NOM			: Trait_transfer_Compte_Dom
//			DESCRIPTION	: Traitement de la recuperation des comptes de domiciliation
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			ENTREE		: Rien
//
//			SORTIE		:resultat de la transmisson
//
//			REMARQUE	:
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
uchar	Trait_transfer_Compte_Dom(uchar Premier_Mess)
{
	Compte_dom	compte;
	Ushort		Ind_fin,i,m;

	i=0;
	if(((R_Transac8583.l_champs72%14)!=0)&&(R_Transac8583.l_champs72!=1))
		return KO;

	if((Premier_Mess)||(memcmp(R_Transac8583.champs24,"308", 3)==0)||(memcmp(R_Transac8583.champs24,"306", 3)==0))
	{
		Os__saved_set ((uchar *)&Fic_compte_dom, 0x00, sizeof(Fic_compte_dom));
		set_ushort_secourue(&Fic_compte_dom.Indice_fin,0);
	}
	Os__saved_set((uchar *)&FicPara.status, KO, 1);

	/////////////////////////////////////////////////////////////
	if(R_Transac8583.l_champs72==1)
	{
		if((R_Transac8583.champs72[0]!='N')||memcmp(R_Transac8583.champs24,"300", 3))
			return KO;
		if(memcmp(R_Transac8583.champs24,"300", 3)==0)
			return	FIN_TRANSFER;
	}
	/////////////////////////////////////////////////////////////

	while(i<R_Transac8583.l_champs72)
	{
		Ind_fin = Fic_compte_dom.Indice_fin ;
		memset(compte.Indice_compte,'\0',sizeof(compte));
		memcpy(compte.Indice_compte,&R_Transac8583.champs72[i],2);
		i+=2;
		memcpy(compte.Compte,&R_Transac8583.champs72[i],12);
		i+=12;

		if(Ind_fin>=NB_MAX_COMPTE)
			return KO;

		Os__saved_copy(compte.Indice_compte,(uchar *)&Fic_compte_dom.tab_compte[Ind_fin],sizeof(Compte_dom));
		m=Ind_fin+1;
		set_ushort_secourue(&Fic_compte_dom.Indice_fin,m);
	}
	set_ushort_secourue(&FicPara.Index_compte,0);

	if(memcmp(R_Transac8583.champs24,"300", 3)==0)
		return	FIN_TRANSFER;

	if((memcmp(R_Transac8583.champs24,"301", 3)==0)||
		(memcmp(R_Transac8583.champs24,"308", 3)==0)||
      (memcmp(R_Transac8583.champs24,"306", 3)==0)
	  )
		return OK;

	return KO;
}

//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			NOM			: Trait_transfer_modif_compte_Dom
//			DESCRIPTION	: Traitement du transfer de modification du compte de domiciliation
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			ENTREE		: Rien
//
//			SORTIE		:resultat de la transmisson
//
//			REMARQUE	:
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
uchar	Trait_transfer_modif_compte_Dom(void)
{

	if(!memcmp(R_Transac8583.champs24,"300", 3))
		return	FIN_TRANSFER;

	return KO;
}
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			NOM			: Trait_transfer_EMV_AID
//			DESCRIPTION	: Traitement de transfer des AID EMV
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			ENTREE		: Rien
//
//			SORTIE		:resultat de la transmisson
//
//			REMARQUE	:
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
uchar	Trait_transfer_EMV_AID(uchar Premier_Mess)
{
	Struct_EMV_AID	EMV_AID;
	Ushort			Ind_fin,i;
	Ushort			Lg_Pix;
	char			lgpix;
	char			Tmp_Chaine[3];

	if((Premier_Mess)||(memcmp(R_Transac8583.champs24,"308", 3)==0))
	{
		Os__saved_set ((uchar *)&Fic_EMV_AID, 0x00, sizeof(Fic_EMV_AID));
		set_ushort_secourue(&Fic_EMV_AID.Indice_fin,0);
	}
	Os__saved_set((uchar *)&FicPara.status, KO, 1);
	/////////////////////////////////////////////////////////////
	if(R_Transac8583.l_champs72==1)
	{
		if((R_Transac8583.champs72[0]!='N')||memcmp(R_Transac8583.champs24,"300", 3))
			return KO;
		if(memcmp(R_Transac8583.champs24,"300", 3)==0)
			return	FIN_TRANSFER;
	}
	if(R_Transac8583.l_champs72==0)
		return KO;
   /////////////////////////////////////////////////////////////

	Ind_fin = Fic_EMV_AID.Indice_fin;
	i=0;
	while(i<R_Transac8583.l_champs72)
	{
		if((i+ 4)>R_Transac8583.l_champs72)
			return KO;
		Os__saved_copy((uchar *)&R_Transac8583.champs72[i],(uchar *)&Fic_EMV_AID.Num_ver_file,4);
		i+=(Ushort)4;

		memset((uchar *)&EMV_AID,0,sizeof(EMV_AID));

		if((i+ 10)>R_Transac8583.l_champs72)
			return KO;
		asc_hex(EMV_AID.RID,5,&R_Transac8583.champs72[i],10);
		i+=(Ushort)10;

		memset(Tmp_Chaine,0,3);
		if((i+ 2)>R_Transac8583.l_champs72)
			return KO;
		memcpy(Tmp_Chaine,&R_Transac8583.champs72[i],2);
		i+=(Ushort)2;
		Lg_Pix = (Ushort)atol(Tmp_Chaine)/2;
		EMV_AID.Lg_pix = (uchar)Lg_Pix;
		if(EMV_AID.Lg_pix>11)
			return KO;

		if((i+ EMV_AID.Lg_pix)>R_Transac8583.l_champs72)
			return KO;
		lgpix=2*EMV_AID.Lg_pix;
		asc_hex(EMV_AID.PIX,EMV_AID.Lg_pix,&R_Transac8583.champs72[i],lgpix);
		i+= (Ushort)(Lg_Pix *2);

		if((i+ 4)>R_Transac8583.l_champs72)
			return KO;
		memcpy(EMV_AID.version_app_tpe,&R_Transac8583.champs72[i],4);
		i+=4;

		if((i+ 2)>R_Transac8583.l_champs72)
			return KO;
		asc_hex(&EMV_AID.priorite,1,&R_Transac8583.champs72[i],2);
		i+=2;

		if((i+ 1)>R_Transac8583.l_champs72)
			return KO;
		EMV_AID.Indic_for = R_Transac8583.champs72[i] - 48;
		i+=1;

		if(Ind_fin>=NB_MAX_AID)
			return KO;

		Os__saved_copy((uchar *)&EMV_AID,(uchar *)&Fic_EMV_AID.tab_aid[Ind_fin],sizeof(EMV_AID));
		Ind_fin +=(Ushort)1;
		set_ushort_secourue(&Fic_EMV_AID.Indice_fin,Ind_fin);
	}

	if(memcmp(R_Transac8583.champs24,"300", 3)==0)
	{
		if(R_Transac8583.l_champs63 > 19)
			Os__saved_copy(&R_Transac8583.champs63[6],(uchar *)&FicPara.Version_Tab_Emv,3);
		return	FIN_TRANSFER;
	}
	if(
		(memcmp(R_Transac8583.champs24,"301", 3)==0)||
		(memcmp(R_Transac8583.champs24,"308", 3)==0)
	  )
		return OK;

	return KO;
}
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			NOM			: Trait_transfer_EMV_KEYS
//			DESCRIPTION	: Traitement de transfer des Clés EMV (CA pulic Keys)
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			ENTREE		: Rien
//
//			SORTIE		:resultat de la transmisson
//
//			REMARQUE	:
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
uchar	Trait_transfer_EMV_KEYS(uchar Premier_Mess)
{
	Struct_EMV_Keys		EMV_Keys;
	Ushort				Ind_fin,i;
	char				Tmp[5];
	uchar           	Lg_Exp;
	uchar           	lgexp;
	uchar				Key_Exp[6];
	Ushort				Lg_Mod,lgmod;

	if((Premier_Mess)||(memcmp(R_Transac8583.champs24,"308", 3)==0))
	{
		Os__saved_set ((uchar *)&Fic_EMV_Keys, 0x00, sizeof(Fic_EMV_Keys));
		set_ushort_secourue(&Fic_EMV_Keys.Indice_fin,0);
	}
	Os__saved_set((uchar *)&FicPara.status, KO, 1);
	/////////////////////////////////////////////////////////////
	if(R_Transac8583.l_champs72==1)
	{
		if((R_Transac8583.champs72[0]!='N')||memcmp(R_Transac8583.champs24,"300", 3))
			return KO;
		if(memcmp(R_Transac8583.champs24,"300", 3)==0)
			return	FIN_TRANSFER;
	}
	if(R_Transac8583.l_champs72==0)
		return KO;
   /////////////////////////////////////////////////////////////
	Ind_fin = Fic_EMV_Keys.Indice_fin;
	i=0;
	while(i<R_Transac8583.l_champs72)
	{
		if((i+ 4)>R_Transac8583.l_champs72)
			return KO;
		Os__saved_copy((uchar *)&R_Transac8583.champs72[i],(uchar *)&Fic_EMV_Keys.Num_ver_file,4);
		i+=(Ushort)4;

		memset((uchar *)&EMV_Keys,'\0',sizeof(EMV_Keys));

		if((i+ 10)>R_Transac8583.l_champs72)
			return KO;
		asc_hex(EMV_Keys.RID,5,(uchar *)&R_Transac8583.champs72[i],10);
		i+=(Ushort)10;

		if((i+ 2)>R_Transac8583.l_champs72)
			return KO;
		asc_hex(&EMV_Keys.Key_Index,1,&R_Transac8583.champs72[i],2);
		i+=(Ushort)2;

		asc_hex(&Lg_Exp,1,&R_Transac8583.champs72[i],2);
		if(Lg_Exp==5)
			EMV_Keys.Lg_Exp = Lg_Exp/2 +1;
		else
			EMV_Keys.Lg_Exp = Lg_Exp/2;
		i+=(Ushort)2;

		if((i+ 2*Lg_Exp) > R_Transac8583.l_champs72)
      		return KO;

		memset(Key_Exp,'0',6);
		if(Lg_Exp==5)
			memcpy(&Key_Exp[1],(uchar *)&R_Transac8583.champs72[i],5);
		else
			memcpy(&Key_Exp[0],(uchar *)&R_Transac8583.champs72[i],Lg_Exp);
		lgexp=2*EMV_Keys.Lg_Exp;
		asc_hex(EMV_Keys.Key_Exp,EMV_Keys.Lg_Exp,Key_Exp,lgexp);
		i+= Lg_Exp;

		memset(Tmp,'\0',5);
		if((i+ 3)>R_Transac8583.l_champs72)
			return KO;
		memcpy(Tmp,(uchar *)&R_Transac8583.champs72[i],3);
		i+=(Ushort)3;
		Lg_Mod = (Ushort)atol(Tmp)/2;
		EMV_Keys.Lg_modulo = (uchar)Lg_Mod;

		if((i+ 2*Lg_Mod)>R_Transac8583.l_champs72)
      		return KO;

		//asc_hex(EMV_Keys.Key_mod,EMV_Keys.Lg_modulo,(uchar *)&R_Transac8583.champs72[i],2*EMV_Keys.Lg_modulo);
		lgmod=2*Lg_Mod;
		bxdtge_stodcb((uchar *)&R_Transac8583.champs72[i],EMV_Keys.Key_mod,lgmod);
		i+=(EMV_Keys.Lg_modulo*2);

		if(Ind_fin>=NB_MAX_EMV_KEY)
			return KO;

		Os__saved_copy((uchar *)&EMV_Keys,(uchar *)&Fic_EMV_Keys.tab_emv_key[Ind_fin],sizeof(EMV_Keys));
		Ind_fin +=(Ushort)1;
		set_ushort_secourue(&Fic_EMV_Keys.Indice_fin,Ind_fin);
	}

	if(memcmp(R_Transac8583.champs24,"300", 3)==0)
		return	FIN_TRANSFER;

	if(
		(memcmp(R_Transac8583.champs24,"301", 3)==0)||
		(memcmp(R_Transac8583.champs24,"308", 3)==0)
	  )
		return OK;

	return KO;
}
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			NOM			: Trait_transfer_EMV_TAC
//			DESCRIPTION	: Traitement de transfer des TACs EMV
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			ENTREE		: Rien
//
//			SORTIE		:resultat de la transmisson
//
//			REMARQUE	:
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
uchar	Trait_transfer_EMV_TAC(uchar Premier_Mess)
{
	Struct_EMV_TAC		EMV_TAC;
	Ushort			  	Ind_fin,i;
	uchar				Lg_Pix,lgpix;

	if((Premier_Mess)||(memcmp(R_Transac8583.champs24,"308", 3)==0))
	{
		Os__saved_set ((uchar *)&Fic_EMV_TAC, 0x00, sizeof(Fic_EMV_TAC));
		set_ushort_secourue(&Fic_EMV_TAC.Indice_fin,0);
	}
	Os__saved_set((uchar *)&FicPara.status, KO, 1);
	/////////////////////////////////////////////////////////////
	if(R_Transac8583.l_champs72==1)
	{
		if((R_Transac8583.champs72[0]!='N')||memcmp(R_Transac8583.champs24,"300", 3))
			return KO;
		if(memcmp(R_Transac8583.champs24,"300", 3)==0)
			return	FIN_TRANSFER;
	}
	if(R_Transac8583.l_champs72==0)
		return KO;
   /////////////////////////////////////////////////////////////

	Ind_fin = Fic_EMV_TAC.Indice_fin;
	i=0;
	while(i<R_Transac8583.l_champs72)
	{
		if((i+ 4)>R_Transac8583.l_champs72)
			return KO;
		Os__saved_copy((uchar *)&R_Transac8583.champs72[i],(uchar *)&Fic_EMV_TAC.Num_ver_file,4);
		i+=(Ushort)4;

		memset((uchar *)&EMV_TAC,'\0',sizeof(EMV_TAC));

		if((i+ 10)>R_Transac8583.l_champs72)
			return KO;
		asc_hex(EMV_TAC.RID,5,(uchar *)&R_Transac8583.champs72[i],10);
		i+=(Ushort)10;

		if((i+ 2)>R_Transac8583.l_champs72)
			return KO;
		asc_hex(&Lg_Pix,1,(uchar *)&R_Transac8583.champs72[i],2);
		EMV_TAC.Lg_PIX = Lg_Pix/2;
		i+=(Ushort)2;

		if(Lg_Pix>11)
			return KO;

		if((i+ Lg_Pix)>R_Transac8583.l_champs72)
			return KO;
		lgpix=2*EMV_TAC.Lg_PIX;
		asc_hex(EMV_TAC.PIX,EMV_TAC.Lg_PIX,(uchar *)&R_Transac8583.champs72[i],lgpix);
		i+= Lg_Pix;

		if((i+ 10)>R_Transac8583.l_champs72)
			return KO;
		asc_hex(EMV_TAC.TAC_denial,5,(uchar *)&R_Transac8583.champs72[i],10);
		i+=(Ushort)10;

		if((i+ 10)>R_Transac8583.l_champs72)
			return KO;
		asc_hex(EMV_TAC.TAC_online,5,(uchar *)&R_Transac8583.champs72[i],10);
		i+=(Ushort)10;

		if((i+ 10)>R_Transac8583.l_champs72)
			return KO;
		asc_hex(EMV_TAC.TAC_default,5,(uchar *)&R_Transac8583.champs72[i],10);
		i+=(Ushort)10;

		if(Ind_fin>=NB_MAX_TAC)
			return KO;

		Os__saved_copy((uchar *)&EMV_TAC,(uchar *)&Fic_EMV_TAC.tab_emv_tac[Ind_fin],sizeof(EMV_TAC));
		Ind_fin +=(Ushort)1;
		set_ushort_secourue(&Fic_EMV_TAC.Indice_fin,Ind_fin);
	}

	if(memcmp(R_Transac8583.champs24,"300", 3)==0)
		return	FIN_TRANSFER;

	if(
		(memcmp(R_Transac8583.champs24,"301", 3)==0)||
		(memcmp(R_Transac8583.champs24,"308", 3)==0)
	  )
		return OK;

	return KO;
}
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			NOM			: Trait_transfer_EMV_TDOL
//			DESCRIPTION	: Traitement de transfer des TDOL
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			ENTREE		: Rien
//
//			SORTIE		:resultat de la transmisson
//
//			REMARQUE	:
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
uchar	Trait_transfer_EMV_TDOL(uchar Premier_Mess)
{
	Struct_EMV_TDOL		EMV_TDOL;
	Ushort			  	Ind_fin,i,Lg,lg;
	char				Tmp[5];

	if((Premier_Mess)||(memcmp(R_Transac8583.champs24,"308", 3)==0))
	{
		Os__saved_set ((uchar *)&Fic_EMV_TDOL, 0x00, sizeof(Fic_EMV_TDOL));
		set_ushort_secourue(&Fic_EMV_TDOL.Indice_fin,0);
	}
	Os__saved_set((uchar *)&FicPara.status, KO, 1);
	/////////////////////////////////////////////////////////////
	if(R_Transac8583.l_champs72==1)
	{
		if((R_Transac8583.champs72[0]!='N')||memcmp(R_Transac8583.champs24,"300", 3))
			return KO;
		if(memcmp(R_Transac8583.champs24,"300", 3)==0)
			return	FIN_TRANSFER;
	}
	if(R_Transac8583.l_champs72==0)
		return KO;
   //////////////////////////////////////////////////////////////

	Ind_fin = Fic_EMV_TDOL.Indice_fin;
	i=0;
	while(i<R_Transac8583.l_champs72)
	{
		if((i+ 4)>R_Transac8583.l_champs72)
			return KO;
		Os__saved_copy((uchar *)&R_Transac8583.champs72[i],(uchar *)&Fic_EMV_TDOL.Num_ver_file,4);
		i+=(Ushort)4;

		memset((uchar *)&EMV_TDOL,'\0',sizeof(EMV_TDOL));

		if((i+ 10)>R_Transac8583.l_champs72)
			return KO;
		asc_hex(EMV_TDOL.RID,5,(uchar *)&R_Transac8583.champs72[i],10);
		i+=(Ushort)10;

		memset(Tmp,'\0',5);
		if((i+ 3)>R_Transac8583.l_champs72)
			return KO;
		memcpy(Tmp,&R_Transac8583.champs72[i],3);
		Lg = (Ushort)atol(Tmp)/2;
		i+=(Ushort)3;
		EMV_TDOL.Lg_TDOL = (uchar)Lg;

		if((i+ 2*Lg)>R_Transac8583.l_champs72)
			return KO;

		//asc_hex(EMV_TDOL.TDOL,Lg,(uchar *)&R_Transac8583.champs72[i],2*Lg);
		lg=2*Lg;
		bxdtge_stodcb((uchar *)&R_Transac8583.champs72[i],EMV_TDOL.TDOL,lg);
		i+=(2*Lg);

		if(Ind_fin>=NB_MAX_TDOL)
			return KO;

		Os__saved_copy((uchar *)&EMV_TDOL,(uchar *)&Fic_EMV_TDOL.tab_emv_tdol[Ind_fin],sizeof(EMV_TDOL));
		Ind_fin +=(Ushort)1;
		set_ushort_secourue(&Fic_EMV_TDOL.Indice_fin,Ind_fin);
	}

	if(memcmp(R_Transac8583.champs24,"300", 3)==0)
		return	FIN_TRANSFER;

	if(
		(memcmp(R_Transac8583.champs24,"301", 3)==0)||
		(memcmp(R_Transac8583.champs24,"308", 3)==0)
	  )
		return OK;

	return KO;
}

//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			NOM			: Trait_transfer_EMV_DDOL
//			DESCRIPTION	: Traitement de transfer des DDOL
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			ENTREE		: Rien
//
//			SORTIE		:resultat de la transmisson
//
//			REMARQUE	:
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
uchar	Trait_transfer_EMV_DDOL(uchar Premier_Mess)
{

	Struct_EMV_DDOL	EMV_DDOL;
	Ushort			Ind_fin,i,Lg;
	char			lgpix,lg;
	char			Tmp[5];

	if((Premier_Mess)||(memcmp(R_Transac8583.champs24,"308", 3)==0))
	{
		Os__saved_set ((uchar *)&Fic_EMV_DDOL, 0x00, sizeof(Fic_EMV_DDOL));
		set_ushort_secourue(&Fic_EMV_DDOL.Indice_fin,0);
	}
	Os__saved_set((uchar *)&FicPara.status, KO, 1);
	/////////////////////////////////////////////////////////////
	if(R_Transac8583.l_champs72==1)
	{
		if((R_Transac8583.champs72[0]!='N')||memcmp(R_Transac8583.champs24,"300", 3))
			return KO;
		if(memcmp(R_Transac8583.champs24,"300", 3)==0)
			return	FIN_TRANSFER;
	}
	if(R_Transac8583.l_champs72==0)
		return KO;
   //////////////////////////////////////////////////////////////

	Ind_fin = Fic_EMV_DDOL.Indice_fin;
	i=0;
	while(i<R_Transac8583.l_champs72)
	{
		if((i+ 4)>R_Transac8583.l_champs72)
			return KO;
		Os__saved_copy((uchar *)&R_Transac8583.champs72[i],(uchar *)&Fic_EMV_DDOL.Num_ver_file,4);
		i+=(Ushort)4;

		memset((uchar *)&EMV_DDOL,'\0',sizeof(EMV_DDOL));
		if((i+ 10)>R_Transac8583.l_champs72)
			return KO;
		asc_hex(EMV_DDOL.RID,5,(uchar *)&R_Transac8583.champs72[i],10);
		i+=(Ushort)10;

		memset(Tmp,0,5);
		if((i+ 2)>R_Transac8583.l_champs72)
			return KO;
		memcpy(Tmp,&R_Transac8583.champs72[i],2);
		Lg = (Ushort)(atol(Tmp)/2);
		EMV_DDOL.Lg_PIX = (uchar)Lg;
		i+=(Ushort)2;

		if((i+ 2*EMV_DDOL.Lg_PIX)>R_Transac8583.l_champs72)
			return KO;
		lgpix=2*EMV_DDOL.Lg_PIX;
		asc_hex(EMV_DDOL.PIX,EMV_DDOL.Lg_PIX,(uchar *)&R_Transac8583.champs72[i],lgpix);
		i+=(2*EMV_DDOL.Lg_PIX);


		memset(Tmp,0,5);
		if((i+ 3)>R_Transac8583.l_champs72)
			return KO;
		memcpy(Tmp,&R_Transac8583.champs72[i],3);
		Lg = (Ushort)atol(Tmp)/2;
		EMV_DDOL.Lg_DDOL = (uchar)Lg;
		i+=(Ushort)3;

		//asc_hex(EMV_DDOL.DDOL,EMV_DDOL.Lg_DDOL,(uchar *)&R_Transac8583.champs72[i],2*EMV_DDOL.Lg_DDOL);
		if((i+ 2*EMV_DDOL.Lg_DDOL)>R_Transac8583.l_champs72)
			return KO;
		lg=2*Lg;
		bxdtge_stodcb((uchar *)&R_Transac8583.champs72[i],EMV_DDOL.DDOL,lg);
		i+=(2*EMV_DDOL.Lg_DDOL);

		if(Ind_fin>=NB_MAX_DDOL)
			return KO;

		Os__saved_copy((uchar *)&EMV_DDOL,(uchar *)&Fic_EMV_DDOL.tab_emv_ddol[Ind_fin],sizeof(EMV_DDOL));
		Ind_fin +=(Ushort)1;
		set_ushort_secourue(&Fic_EMV_DDOL.Indice_fin,Ind_fin);
	}

	if(memcmp(R_Transac8583.champs24,"300", 3)==0)
		return	FIN_TRANSFER;

	if(
		(memcmp(R_Transac8583.champs24,"301", 3)==0)||
		(memcmp(R_Transac8583.champs24,"308", 3)==0)
	  )
		return OK;

	return KO;
}

//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			NOM			: Trait_transfer_EMV_Rand_Call
//			DESCRIPTION	: Traitement de transfer des parametres d'appel aleatoire EMV
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			ENTREE		: Rien
//
//			SORTIE		:resultat de la transmisson
//
//			REMARQUE	:
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
uchar	Trait_transfer_EMV_Rand_Call(uchar Premier_Mess)
{
	Struct_EMV_rand	EMV_rand;
	Ushort			Ind_fin,i,Lg;
	char			lgpix;
	char			Tmp[5];

	if((Premier_Mess)||(memcmp(R_Transac8583.champs24,"308", 3)==0))
	{
		Os__saved_set ((uchar *)&Fic_EMV_rand, 0x00, sizeof(Fic_EMV_rand));
		set_ushort_secourue(&Fic_EMV_rand.Indice_fin,0);
	}
	Os__saved_set((uchar *)&FicPara.status, KO, 1);
	/////////////////////////////////////////////////////////////
	if(R_Transac8583.l_champs72==1)
	{
		if((R_Transac8583.champs72[0]!='N')||memcmp(R_Transac8583.champs24,"300", 3))
			return KO;
		if(memcmp(R_Transac8583.champs24,"300", 3)==0)
			return	FIN_TRANSFER;
	}
	if(R_Transac8583.l_champs72==0)
		return KO;
   /////////////////////////////////////////////////////////////

	Ind_fin = Fic_EMV_rand.Indice_fin;
	i=0;
	while(i<R_Transac8583.l_champs72)
	{
		if((i+ 4)>R_Transac8583.l_champs72)
			return KO;
		Os__saved_copy((uchar *)&R_Transac8583.champs72[i],(uchar *)&Fic_EMV_rand.Num_ver_file,4);
		i+=(Ushort)4;

		memset((uchar *)&EMV_rand,'\0',sizeof(EMV_rand));
		if((i+ 10)>R_Transac8583.l_champs72)
			return KO;
		asc_hex(EMV_rand.RID,5,(uchar *)&R_Transac8583.champs72[i],10);
		i+=(Ushort)10;

		memset(Tmp,0,5);
		if((i+ 2)>R_Transac8583.l_champs72)
			return KO;
		memcpy(Tmp,&R_Transac8583.champs72[i],2);
		Lg = (Ushort)(atol(Tmp)/2);
		EMV_rand.Lg_PIX = (uchar)Lg;
		i+=(Ushort)2;

		if((i+ 2*EMV_rand.Lg_PIX)>R_Transac8583.l_champs72)
			return KO;
		lgpix=2*EMV_rand.Lg_PIX;
		asc_hex(EMV_rand.PIX,EMV_rand.Lg_PIX,(uchar *)&R_Transac8583.champs72[i],lgpix);
		i+=(2*EMV_rand.Lg_PIX);

		if((i+ 3)>R_Transac8583.l_champs72)
			return KO;
		memcpy(EMV_rand.devise,(uchar *)&R_Transac8583.champs72[i],3);
		i+=(Ushort)3;

		if((i+ 12)>R_Transac8583.l_champs72)
			return KO;
		memcpy(EMV_rand.montant_seuil,(uchar *)&R_Transac8583.champs72[i],12);
		i+=(Ushort)12;

		if((i+ 2)>R_Transac8583.l_champs72)
			return KO;
		memcpy(EMV_rand.Nbr_trs,(uchar *)&R_Transac8583.champs72[i],2);
		i+=(Ushort)2;

		if((i+ 2)>R_Transac8583.l_champs72)
			return KO;
		memcpy(EMV_rand.Nbr_max_trs,(uchar *)&R_Transac8583.champs72[i],2);
		i+=(Ushort)2;

		if(Ind_fin>=NB_MAX_RAND_CALL)
			return KO;

		Os__saved_copy((uchar *)&EMV_rand,(uchar *)&Fic_EMV_rand.tab_emv_rand[Ind_fin],sizeof(EMV_rand));
		Ind_fin +=(Ushort)1;
		set_ushort_secourue(&Fic_EMV_rand.Indice_fin,Ind_fin);
	}

	if(memcmp(R_Transac8583.champs24,"300", 3)==0)
		return	FIN_TRANSFER;

	if(
		(memcmp(R_Transac8583.champs24,"301", 3)==0)||
		(memcmp(R_Transac8583.champs24,"308", 3)==0)
	  )
		return OK;

	return KO;
}

//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			NOM			: Trait_transfer_EMV_TCC
//			DESCRIPTION	: Traitement de transfer des TCC
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			ENTREE		: Rien
//
//			SORTIE		:resultat de la transmisson
//
//			REMARQUE	:
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
uchar	Trait_transfer_EMV_TCC(uchar Premier_Mess)
{
	Struct_EMV_TCC		EMV_TCC;
	Ushort			  	Ind_fin,i,Lg;
	char				lgpix;
	char				Tmp[13];

	if((Premier_Mess)||(memcmp(R_Transac8583.champs24,"308", 3)==0))
	{
		Os__saved_set ((uchar *)&Fic_EMV_TCC, 0x00, sizeof(Fic_EMV_TCC));
		set_ushort_secourue(&Fic_EMV_TCC.Indice_fin,0);
	}
	Os__saved_set((uchar *)&FicPara.status, KO, 1);
	/////////////////////////////////////////////////////////////
	if(R_Transac8583.l_champs72==1)
	{
		if((R_Transac8583.champs72[0]!='N')||memcmp(R_Transac8583.champs24,"300", 3))
			return KO;
		if(memcmp(R_Transac8583.champs24,"300", 3)==0)
			return	FIN_TRANSFER;
	}
	if(R_Transac8583.l_champs72==0)
		return KO;
   /////////////////////////////////////////////////////////////

	Ind_fin = Fic_EMV_TCC.Indice_fin;
	i=0;
	while(i<R_Transac8583.l_champs72)
	{
		if((i+ 4)>R_Transac8583.l_champs72)
			return KO;
		Os__saved_copy((uchar *)&R_Transac8583.champs72[i],(uchar *)&Fic_EMV_TCC.Num_ver_file,4);
		i+=(Ushort)4;

		memset((uchar *)&EMV_TCC,'\0',sizeof(EMV_TCC));

		if((i+ 10)>R_Transac8583.l_champs72)
			return KO;
		asc_hex(EMV_TCC.RID,5,(uchar *)&R_Transac8583.champs72[i],10);
		i+=(Ushort)10;

		memset(Tmp,0,5);
		if((i+ 2)>R_Transac8583.l_champs72)
			return KO;
		memcpy(Tmp,&R_Transac8583.champs72[i],2);
		Lg = (Ushort)(atol(Tmp)/2);
		EMV_TCC.Lg_PIX = (uchar)Lg;
		i+=(Ushort)2;

		if((i+ 2*EMV_TCC.Lg_PIX)>R_Transac8583.l_champs72)
			return KO;
		lgpix=2*EMV_TCC.Lg_PIX;
		asc_hex(EMV_TCC.PIX,EMV_TCC.Lg_PIX,(uchar *)&R_Transac8583.champs72[i],lgpix);
		i+=(2*EMV_TCC.Lg_PIX);

		if((i+ 2)>R_Transac8583.l_champs72)
			return KO;
		asc_hex(&EMV_TCC.tcc_autor,1,&R_Transac8583.champs72[i],2);
		i+=(Ushort)2;

		if(Ind_fin>=NB_MAX_TCC)
			return KO;

		Os__saved_copy((uchar *)&EMV_TCC,(uchar *)&Fic_EMV_TCC.tab_emv_tcc[Ind_fin],sizeof(EMV_TCC));
		Ind_fin +=(Ushort)1;
		set_ushort_secourue(&Fic_EMV_TCC.Indice_fin,Ind_fin);
	}

	if(memcmp(R_Transac8583.champs24,"300", 3)==0)
	{
		if(R_Transac8583.l_champs63 > 19)
			Os__saved_copy(&R_Transac8583.champs63[6],(uchar *)&FicPara.Version_Tab_Emv,3);
		return	FIN_TRANSFER;
	}

	if(
		(memcmp(R_Transac8583.champs24,"301", 3)==0)||
		(memcmp(R_Transac8583.champs24,"308", 3)==0)
	  )
		return OK;

	return KO;
}
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			NOM			: Trait_transfer_INTERCHANGE
//			DESCRIPTION	: Traitement de transfer des commissions INTERCHANGE
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			ENTREE		: Rien
//
//			SORTIE		:resultat de la transmisson
//
//			REMARQUE	:
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
uchar	Trait_transfer_INTERCHANGE(uchar Premier_Mess)
{
	ushort			Ind_fin,i;
	char			Tmp_Chaine[3];

	if((Premier_Mess)||(memcmp(R_Transac8583.champs24,"308", 3)==0))
	{
		Os__saved_set ((uchar *)&FicPara.COMMISSION_INT[0], 0x00, sizeof(FicPara.COMMISSION_INT[0]));
		Os__saved_set ((uchar *)&FicPara.COMMISSION_SVM[0], 0x00, sizeof(FicPara.COMMISSION_SVM[0]));
		Os__saved_set ((uchar *)&FicPara.COMMISSION_MN[0], 0x00, sizeof(FicPara.COMMISSION_MN[0]));
	}
	Os__saved_set((uchar *)&FicPara.status, KO, 1);
	/////////////////////////////////////////////////////////////
	if(R_Transac8583.l_champs72==1)
	{
		if((R_Transac8583.champs72[0]!='N')||memcmp(R_Transac8583.champs24,"300", 3))
			return KO;
		if(memcmp(R_Transac8583.champs24,"300", 3)==0)
			return	FIN_TRANSFER;
	}
	if(R_Transac8583.l_champs72==0)
		return KO;

	while(i<R_Transac8583.l_champs72)
	{
		if((i+ 4)>R_Transac8583.l_champs72)
			return KO;
		//Os__saved_copy((uchar *)&R_Transac8583.champs72[i],(uchar *)&Fic_EMV_AID.Num_ver_file,4);
		i+=(ushort)4;

		Os__saved_set ((uchar *)&FicPara.COMMISSION_INT, 0x00, sizeof(FicPara.COMMISSION_INT));
		Os__saved_set ((uchar *)&FicPara.COMMISSION_SVM, 0x00, sizeof(FicPara.COMMISSION_SVM));
		Os__saved_set ((uchar *)&FicPara.COMMISSION_MN, 0x00, sizeof(FicPara.COMMISSION_MN));

		if((i+ 30)>R_Transac8583.l_champs72)
			return KO;

		//Commission surcout Nationale
		Os__saved_copy((uchar *)&R_Transac8583.champs72[i],(uchar *)&FicPara.COMMISSION_INT[0],5);
		i+=(ushort)5;
		Os__saved_copy((uchar *)&R_Transac8583.champs72[i],(uchar *)&FicPara.COMMISSION_SVM[0],5);
		i+=(ushort)5;
		Os__saved_copy((uchar *)&R_Transac8583.champs72[i],(uchar *)&FicPara.COMMISSION_MN[0],5);
		i+=(ushort)5;

		//Commission surcout InterNationale 1.11.16
		Os__saved_copy((uchar *)&R_Transac8583.champs72[i],(uchar *)&FicPara.COMMISSION_INT[1],5);
		i+=(ushort)5;
		Os__saved_copy((uchar *)&R_Transac8583.champs72[i],(uchar *)&FicPara.COMMISSION_SVM[1],5);
		i+=(ushort)5;
		Os__saved_copy((uchar *)&R_Transac8583.champs72[i],(uchar *)&FicPara.COMMISSION_MN[1],5);
		i+=(ushort)5;

//		Os__xprintaa(FicPara.COMMISSION_INT[0]);
//		Os__xprintaa(FicPara.COMMISSION_SVM[0]);
//		Os__xprintaa(FicPara.COMMISSION_MN[0]);
	}

	if(memcmp(R_Transac8583.champs24,"300", 3)==0)
	{
		return	FIN_TRANSFER;
	}
	if(
		(memcmp(R_Transac8583.champs24,"301", 3)==0)||
		(memcmp(R_Transac8583.champs24,"308", 3)==0)
	)
		return OK;

	return KO;
}
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			NOM			: Trait_transfer_MenuTPE
//			DESCRIPTION	: Traitement de la recuperation des données Menu TPE dynamique
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			ENTREE		: Rien
//
//			SORTIE		:resultat de la transmisson
//
//			REMARQUE	:
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
uchar	Trait_transfer_MenuTPE(uchar Premier_Mess)
{
//	ushort	i,OffSet;
	uchar	Status;


	if((Premier_Mess)||(memcmp(R_Transac8583.champs24,"308", 3)==0)||(memcmp(R_Transac8583.champs24,"306", 3)==0))
	{
		memset(FicPara.MenuTPE,'\0',30);	//Version 1.6
	}

	Os__saved_set((uchar *)&FicPara.status, KO, 1);

	//i = 0;
	//OffSet = 0;
	Status = OK;

	/*Status = Maj_Prefixe(AJOUT_ENREG);
	if(Status != OK)
		return KO;*/

	if(memcmp(R_Transac8583.champs24,"300", 3)==0)
	{
		memset(FicPara.MenuTPE,'\0',30);
		memcpy(FicPara.MenuTPE,R_Transac8583.champs72,29);
		return	FIN_TRANSFER;
	}

	if((memcmp(R_Transac8583.champs24,"301", 3)==0)||
		(memcmp(R_Transac8583.champs24,"308", 3)==0)||
		(memcmp(R_Transac8583.champs24,"306", 3)==0)
	  )
		return OK;

	return KO;

}
