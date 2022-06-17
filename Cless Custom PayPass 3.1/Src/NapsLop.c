/* ======================================================================== */
/* Include Files                                                            */
/* ======================================================================== */

#include "SDK_Naps.h"
#include "TlvTree.h"
#include "del_lib.h"
#include "convert.h"
#include "cu_disk.h"


Fichier_Lop		Fic_Lop;
#define LABEL_FILE_CMI_LOP	 "CMI_LOP"

//void	CUMORE_CMI_PHONE_NUMBER(PC pPhoneNumberScreen);
//void	CUMORE_CMI_AFFICHE_MESSAGE_SD(int ligne1,int ligne2,int delay);
/* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
#define LIRE_BIN		1
#define LIRE_CARD		2

uchar		g_Bin_lu[7];
uchar		g_Data_lu[14];
Ushort		g_nbr_Data;
uchar		g_Status_data_lu;

uchar		g_tranche_deb[20];
uchar		g_tranche_fin[20];
Ushort		g_Data_trch;
#define NBR_MAX_CARD	200

/*-----------------------------------------------------------------
 * Fonction   : LopFile
 * objet      : Ecris tous tous les lop
 * Parametre  : Aucun
 * Retour     : Aucun
 *-----------------------------------------------------------------
*/
L	LopFile()
{
	T_Bool  ret;

	char FilePath_L[33];
	int iret;

	ret = B_TRUE;

	iret = CUDISK_CreateDisk();
	if (iret == FS_OK)
	{
		memclr(FilePath_L, sizeof(FilePath_L));
		sprintf(FilePath_L, "/%s%02x/%s",LABEL_DISK,CustApplicationNumber,LABEL_FILE_CMI_LOP);

		iret = FS_exist (FilePath_L);
		if (iret == FS_OK)
		{
			memclr(FilePath_L,sizeof(FilePath_L));
			sprintf(FilePath_L, "/%s%02x",LABEL_DISK,CustApplicationNumber);
			FS_unmount(FilePath_L);

			GetAllLop();
		}
		else
		{
			memclr(FilePath_L,sizeof(FilePath_L));
			sprintf(FilePath_L, "/%s%02x",LABEL_DISK,CustApplicationNumber);
			FS_unmount(FilePath_L);

			Init_Lop();
		}
	}
    return (ret);
}
/*-----------------------------------------------------------------
 * Fonction   : GetAllLop
 * objet      : Remplir tous les lops
 * Parametre  : Aucun
 * Retour     : Aucun
 *-----------------------------------------------------------------
*/
L	GetAllLop()
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
		sprintf(FilePath_L, "/%s%02x/%s",LABEL_DISK,CustApplicationNumber,LABEL_FILE_CMI_LOP);
		file1 = FS_open (FilePath_L, "r");
		if (file1 != NULL)
		{
			FS_read((PUC)&Fic_Lop, sizeof (Fichier_Lop), 1, file1);
			FS_close (file1);

			memclr(FilePath_L,sizeof(FilePath_L));
			sprintf(FilePath_L, "/%s%02x",LABEL_DISK,CustApplicationNumber);
			FS_unmount(FilePath_L);

			memcpy(&lop_gest	,&Fic_Lop.lop_gest		,sizeof(lop_gest_enreg));
			memcpy(&lop_fichier	,&Fic_Lop.lop_fichier	,sizeof(typ_lop_enr_bcd));
			memcpy(&lop_bin		,&Fic_Lop.lop_bin		,sizeof(typ_lop_enr_bin));
			memcpy(&lop_trn		,&Fic_Lop.lop_trn		,sizeof(typ_lop_enr_trn));

			return OK;
		}
	}
    return KO;
}

/*-----------------------------------------------------------------
 * Fonction   : PutAllLop
 * objet      : Ecris tous les lop
 * Parametre  : Aucun
 * Retour     : Aucun
 *-----------------------------------------------------------------
*/
L	PutAllLop()
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
		sprintf(FilePath_L, "/%s%02x/%s",LABEL_DISK,CustApplicationNumber,LABEL_FILE_CMI_LOP);
		iret = FS_exist (FilePath_L);
		if (iret == FS_OK)
			iret = FS_unlink (FilePath_L);

		file1 = FS_open (FilePath_L, "a");
		if (file1 != NULL)
		{
			memcpy(&Fic_Lop.lop_gest	,&lop_gest		,sizeof(lop_gest_enreg));
			memcpy(&Fic_Lop.lop_fichier	,&lop_fichier	,sizeof(typ_lop_enr_bcd));
			memcpy(&Fic_Lop.lop_bin		,&lop_bin		,sizeof(typ_lop_enr_bin));
			memcpy(&Fic_Lop.lop_trn		,&lop_trn		,sizeof(typ_lop_enr_trn));

			FS_write((PUC)&Fic_Lop, sizeof (Fichier_Lop), 1, file1);
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
 * Fonction   : Init_Lop
 * objet      : Init du fichier Liste Noire
 * Parametre  : Aucun
 * Retour     : Aucun
 *-----------------------------------------------------------------
*/
void	Init_Lop(void)
{
	Os__saved_set ((uchar *)&lop_gest, 0x00, sizeof(lop_gest));

	set_ushort_secourue(&lop_gest.indexfinln,0);
	set_ushort_secourue(&lop_gest.indexfinln_bin,0);
	set_ushort_secourue(&lop_gest.indexfinln_trn,0);

	Os__saved_set ((uchar *)lop_gest.NumListe, '0', sizeof(lop_gest.NumListe));
	Os__saved_set ((uchar *)lop_gest.NumListInt, '0', sizeof(lop_gest.NumListInt));
	Os__saved_copy ((uchar *)"001",(uchar *)lop_gest.NumBloc, sizeof(lop_gest.NumBloc));
	Os__saved_set ((uchar *)&lop_gest.typmis, PAS_DE_MAJ, 1);
	Os__saved_set ((uchar *)&lop_gest.reprise, KO, 1);
	Os__saved_set ((uchar *)&lop_gest.stat, KO, 1);

	Os__saved_set ((uchar *)&lop_gest.Bin_lu, 0, sizeof(lop_gest.Bin_lu));
	Os__saved_copy ((uchar *)"000000",(uchar *)lop_gest.Bin_lu, sizeof(lop_gest.Bin_lu)-1);
	Os__saved_set ((uchar *)&lop_gest.Data_lu, 0, sizeof(lop_gest.Data_lu));
	Os__saved_copy ((uchar *)"0000000000000",(uchar *)lop_gest.Data_lu, sizeof(lop_gest.Data_lu)-1);
	set_ushort_secourue(&lop_gest.Nbr_Data,0);
	Os__saved_set ((uchar *)&lop_gest.Status_Data_lu, 'P', sizeof(lop_gest.Status_Data_lu));

	Os__saved_set ((uchar *)&lop_fichier, '\0', sizeof(lop_fichier));
	Os__saved_set ((uchar *)&lop_bin, '\0', sizeof(lop_bin));
	Os__saved_set ((uchar *)&lop_bin, '9',6);//si bin zone vide insertion possible
	set_ushort_secourue(&lop_gest.indexfinln_bin,1);//si bin zone vide insertion possible
	Os__saved_set ((uchar *)&lop_trn, '\0', sizeof(lop_trn));

	PutAllLop();
}

//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			NOM			:	ListeNoire
//			DESCRIPTION	: Remplir la liste noire
//
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			ENTREE		:rien
//
//			SORTIE		:status
//
//			FCT APP		:
//
//			REMARQUE	:
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
uchar ListeNoire(uchar Numeroter,uchar InitModem)
{
	uchar	phone[17],site[16];
	uchar	status;
	Ushort	Essai_cnx = 0;

//Mouna050405_début
	if(Numeroter)
	{
		do{
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
				;//Fermer17status = connect_host_GSM((PC)phone,(PC)site);
			if(FicPara.g_type_connexion == 'R')
				;//Fermer17status = connect_host((PC)phone,(PC)site);
			if(FicPara.g_type_connexion == 'G')
				;//Fermer17status = connect_host_GPRS((PC)phone,(PC)site);
			if(FicPara.g_type_connexion == 'I')
				;//Fermer17status = connect_host_IP((PC)phone,(PC)site);
			if(status != OK)
			{
				hang_up(0,g_trace);
			}
			Essai_cnx++;
		}while((Essai_cnx < 3)&&(status != OK));

		if(status != OK)
			return CONNEXION_KO;
	}
//Mouna050405_fin

	CUMORE_CMI_AFFICHE_MESSAGE(35,138,1000);
	//Uti__display1_clr2("LISTE NOIRE");
	Os__xdelay(50);

	status = Transmission_file(FILE_LOP);

	//affectation de la date serveur
	ExploiField7();

	if(status == OK)
	{
		CUMORE_CMI_AFFICHE_MESSAGE(37,138,1000);
		//Uti__display1_clr2("LOP CHARGEE");
		Os__xdelay(100);
	}
	else
	{
		CUMORE_CMI_AFFICHE_MESSAGE(38,138,1000);
		//Uti__display1_clr2("LOP NON CHARGEE");
		Os__xdelay(100);
	}

	if(InitModem)
		hang_up(typehangup,g_trace);

	return	OK;
}

//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			NOM			: Trait_Lop_Complete
//			DESCRIPTION	: Traitement de la liste noire compléte dans les fichiers concernées
//
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			ENTREE		:rien
//
//			SORTIE		:rien
//
//			FCT APP		:
//
//			REMARQUE	:
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
uchar Trait_Lop_Complete(uchar Premier_Mess)
{
	Ushort i,j,OffSet,Nb_Card;
	uchar	 status;
	uchar	 Buf_Card_Lop[NBR_MAX_CARD*10];
	Ushort position,k ;
	typ_lop_enr_bcd Oppos;
	typ_lop_enr_bin Bin_Oppos;
	typ_lop_enr_trn Trn_Oppos;

	uchar		Num_Bloc[4];
	uchar		Num_Bloc_Rec[17];
	Ulong		l_num_bloc;
	uchar		Action;
	uchar		Tmp_chain[24];


	if((memcmp(R_Transac8583.champs24,"300",3)==0)&&(R_Transac8583.champs72[0]=='N'))
		return FIN_TRANSFER;

	memset(Num_Bloc_Rec,0,17);
	memset(Num_Bloc,0,4);

	memcpy(Num_Bloc,&lop_gest.NumBloc[0],3);
	Action = lop_gest.Action;
	sprintf((PC)Num_Bloc_Rec,"  BLOC : %.03s",Num_Bloc);
	CUMORE_CMI_PHONE_NUMBER((char*)Num_Bloc_Rec);
	CUMORE_CMI_AFFICHE_MESSAGE_SD(35,42,0);
	/*Uti__display2((PC)Num_Bloc_Rec);*/

	i = 1;

	if((R_Transac8583.champs72[1]=='B')||(R_Transac8583.champs72[1]=='R')||(R_Transac8583.champs72[1]=='C'))
	{
		if(R_Transac8583.champs72[1]=='C')
		{
			Os__saved_set ((uchar *)&lop_gest.Bin_lu, 0, sizeof(lop_gest.Bin_lu));
			Os__saved_copy ((uchar *)"000000",(uchar *)lop_gest.Bin_lu, sizeof(lop_gest.Bin_lu)-1);
			Os__saved_set ((uchar *)&lop_gest.Data_lu, 0, sizeof(lop_gest.Data_lu));
			Os__saved_copy ((uchar *)"0000000000000",(uchar *)lop_gest.Data_lu, sizeof(lop_gest.Data_lu)-1);
			set_ushort_secourue(&lop_gest.Nbr_Data,0);
			Os__saved_set ((uchar *)&lop_gest.Status_Data_lu, 'P', sizeof(lop_gest.Status_Data_lu));

		}
		Action = R_Transac8583.champs72[1];
		i = 2;
	}

	status = OK;
	while((i<R_Transac8583.l_champs72)&&(status ==OK))
	{
		if((R_Transac8583.champs72[i]=='B')||(R_Transac8583.champs72[i]=='R')||(R_Transac8583.champs72[i]=='C'))
		{
			if(R_Transac8583.champs72[i]=='C')
			{
				Os__saved_set ((uchar *)&lop_gest.Bin_lu, 0, sizeof(lop_gest.Bin_lu));
				Os__saved_copy ((uchar *)"000000",(uchar *)lop_gest.Bin_lu, sizeof(lop_gest.Bin_lu)-1);
				Os__saved_set ((uchar *)&lop_gest.Data_lu, 0, sizeof(lop_gest.Data_lu));
				Os__saved_copy ((uchar *)"0000000000000",(uchar *)lop_gest.Data_lu, sizeof(lop_gest.Data_lu)-1);
				set_ushort_secourue(&lop_gest.Nbr_Data,0);
				Os__saved_set ((uchar *)&lop_gest.Status_Data_lu, 'P', sizeof(lop_gest.Status_Data_lu));

			}
			Action = R_Transac8583.champs72[i];
			i += 1;
		}
		switch(Action)
		{
			case 'B':
				memset(Buf_Card_Lop,0,NBR_MAX_CARD*10);
				OffSet = 0;
				Nb_Card = 0;
				j=R_Transac8583.l_champs72-i;
				status = Recup_LOP_BIN(&R_Transac8583.champs72[i],j,&OffSet,Buf_Card_Lop,&Nb_Card);
				i+=OffSet;
				////////////////////////////////////////////////////////
				k=0;
				while(k<Nb_Card)
   				{
      				position = lop_gest.indexfinln_bin;
      				if(position != 0)
      				{
      					if(position==NBMAXOPPBIN)
         				{
         					Os__xprint("LOP_INCOHERENTE");
      						return LOP_INCOHERENTE;
         				}
						j=position-1;
      					read_oppos_bin_index(j,&Bin_Oppos );
						//if(memcmp(&Buf_Card_Lop[k*6],Bin_Oppos.bin,6)<0)
						if(memcmp(&Buf_Card_Lop[k*6],Bin_Oppos.bin,6)>0)
         				{
         					Os__xprint("LOP_INCOHERENTE");
      						return LOP_INCOHERENTE;
         				}
      				}
      				memcpy((char *)&Bin_Oppos,&Buf_Card_Lop[k*6],6);
      				write_oppos_bin_index(position ,&Bin_Oppos );
      				position ++ ;
      				set_ushort_secourue(&lop_gest.indexfinln_bin,position);
      				k+=(Ushort)1 ;
				}
				////////////////////////////////////////////////////////
				break;
			case 'R':
				memset(Buf_Card_Lop,0,NBR_MAX_CARD*10);
				OffSet = 0;
				Nb_Card = 0;
				j=R_Transac8583.l_champs72-i;
				status = Recup_LOP_Tranche(&R_Transac8583.champs72[i],j,&OffSet,Buf_Card_Lop,&Nb_Card);
				i+=OffSet;
				////////////////////////////////////////////////////////
				k=0;
				while(k<Nb_Card)
   				{
      				position = lop_gest.indexfinln_trn;
      				if(position != 0)
      				{
      					if(position==NBMAXOPPTRN)
         				{
         					Os__xprint("LOP_INCOHERENTE");
      						return LOP_INCOHERENTE;
         				}
						j=position-1;
      					read_oppos_trn_index(j,&Trn_Oppos );
						//if(memcmp(&Buf_Card_Lop[k*20],Trn_Oppos.trn_fin,10)<0)
						if(memcmp(&Buf_Card_Lop[k*20],Trn_Oppos.trn_fin,10)>0)
         				{
         					Os__xprint("LOP_INCOHERENTE");
      						return LOP_INCOHERENTE;
         				}
      				}
      				memcpy((char *)&Trn_Oppos,&Buf_Card_Lop[k*20],20);
      				write_oppos_trn_index(position ,&Trn_Oppos )	;
      				position ++ ;
      				set_ushort_secourue(&lop_gest.indexfinln_trn,position);
      				k+=(Ushort)1 ;
				}
				////////////////////////////////////////////////////////
				break;
			case 'C':
				memset(Buf_Card_Lop,0,NBR_MAX_CARD*10);
				OffSet = 0;
				Nb_Card = 0;
				j=R_Transac8583.l_champs72-i;
				status = Recup_LOP_Card(&R_Transac8583.champs72[i],j,&OffSet,Buf_Card_Lop,&Nb_Card);
				if(status!=OK)
					return KO;
				i+= OffSet;
				////////////////////////////////////////////////////////
				k=0;
				while(k<Nb_Card)
				{
      				position = lop_gest.indexfinln;
      				if(position != 0)
      				{
      					if(position==NBMAXOPP)
         				{
         					Os__xprint("LOP_INCOHERENTE");
      						return LOP_INCOHERENTE;
         				}
						j=position-1;
      					read_oppos_index(j,&Oppos );
					  	if(memcmp(&Buf_Card_Lop[k*10],Oppos.numero,10)>=0)
         				{
							//Os__xprint(&Buf_Card_Lop[k*10]);
							memset(Tmp_chain,0,sizeof(Tmp_chain));
							sprintf((PC)Tmp_chain,"pos = %d ",k);
							Os__xprint((PC)Tmp_chain);
         					Os__xprint("LOP_INCOHERENTE");
      						return LOP_INCOHERENTE;
         				}
      				}
      				memcpy((char *)&Oppos,&Buf_Card_Lop[k*10],10);
      				write_oppos_index(position ,&Oppos )	;
      				position ++ ;
      				set_ushort_secourue(&lop_gest.indexfinln,position);
      				k+=(Ushort)1 ;
				}
				////////////////////////////////////////////////////////
				break;
			default:
         		return KO;
		}
	}

	//lop_gest.Action = Action;
	Os__saved_set((uchar*)&lop_gest.Action,Action,1);
	memset(Num_Bloc,0,4);
	memcpy(Num_Bloc,&lop_gest.NumBloc[0],3);
	l_num_bloc = atol((PC)Num_Bloc)+1;
	sprintf((PC)Num_Bloc,"%03lu",l_num_bloc);
	Os__saved_copy(Num_Bloc,(uchar *)&lop_gest.NumBloc[0],3) ;

   return OK ;
}
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			NOM			: Trait_Lop_Supp
//			DESCRIPTION	: Enregistrement de la liste noire les fichiers concernées
//
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			ENTREE		:rien
//
//			SORTIE		:status
//
//			FCT APP		:
//
//			REMARQUE	:
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
uchar Trait_Lop_Supp(uchar Premier_Mess)
{
	Ushort i,j,OffSet,Nb_Card;
	uchar	 status;
	uchar	 Buf_Card_Lop[NBR_MAX_CARD*10];
	Ushort k ;
	Ushort index_fin_ln ;
	uchar  Buffer[21] ;
	typ_lop_enr_bcd Oppos;
	typ_lop_enr_bin OpposBin;
	typ_lop_enr_trn OpposTrn;


	uchar		Num_Bloc[4];
	uchar		Num_Bloc_Rec[17];
	Ulong		l_num_bloc;
	uchar		Action;


	memset(Num_Bloc_Rec,0,17);
	memset(Num_Bloc,0,4);

	memcpy(Num_Bloc,&lop_gest.NumBloc[0],3);
	Action = lop_gest.Action;
	sprintf((PC)Num_Bloc_Rec,"   SUP : %.03s",Num_Bloc);
	CUMORE_CMI_PHONE_NUMBER((char*)Num_Bloc_Rec);
	CUMORE_CMI_AFFICHE_MESSAGE_SD(35,42,0);

	/*Uti__display2((PC)Num_Bloc_Rec);*/

	i = 1;
	if((R_Transac8583.champs72[1]=='B')||(R_Transac8583.champs72[1]=='R')||(R_Transac8583.champs72[1]=='C'))
	{
		if(R_Transac8583.champs72[1]=='C')
		{
			Os__saved_set ((uchar *)&lop_gest.Bin_lu, 0, sizeof(lop_gest.Bin_lu));
			Os__saved_copy ((uchar *)"000000",(uchar *)lop_gest.Bin_lu, sizeof(lop_gest.Bin_lu)-1);
			Os__saved_set ((uchar *)&lop_gest.Data_lu, 0, sizeof(lop_gest.Data_lu));
			Os__saved_copy ((uchar *)"0000000000000",(uchar *)lop_gest.Data_lu, sizeof(lop_gest.Data_lu)-1);
			set_ushort_secourue(&lop_gest.Nbr_Data,0);
			Os__saved_set ((uchar *)&lop_gest.Status_Data_lu, 'P', sizeof(lop_gest.Status_Data_lu));

		}
		Action = R_Transac8583.champs72[1];
		i = 2;
	}

	if(Premier_Mess)
	{
		////////////////////////////////////////////////////////
   		set_ushort_secourue(&lop_gest.indexcouln,0);
		set_ushort_secourue(&lop_gest.indexcouln_bin,0);
		set_ushort_secourue(&lop_gest.indexcouln_trn,0);
		////////////////////////////////////////////////////////
		Os__saved_set((uchar *)&lop_gest.Bin_lu, 0, sizeof(lop_gest.Bin_lu));
		Os__saved_copy((uchar *)"000000",(uchar *)lop_gest.Bin_lu, sizeof(lop_gest.Bin_lu)-1);
		Os__saved_set((uchar *)&lop_gest.Data_lu, 0, sizeof(lop_gest.Data_lu));
		Os__saved_copy((uchar *)"0000000000000",(uchar *)lop_gest.Data_lu, sizeof(lop_gest.Data_lu)-1);
		set_ushort_secourue(&lop_gest.Nbr_Data,0);
		Os__saved_set((uchar *)&lop_gest.Status_Data_lu, 'P', sizeof(lop_gest.Status_Data_lu));
	}

	status = OK;
	while((i<R_Transac8583.l_champs72)&&(status ==OK))
	{
		if((R_Transac8583.champs72[i]=='B')||(R_Transac8583.champs72[i]=='R')||(R_Transac8583.champs72[i]=='C'))
		{
			if(R_Transac8583.champs72[i]=='C')
			{
				Os__saved_set ((uchar *)&lop_gest.Bin_lu, 0, sizeof(lop_gest.Bin_lu));
				Os__saved_copy ((uchar *)"000000",(uchar *)lop_gest.Bin_lu, sizeof(lop_gest.Bin_lu)-1);
				Os__saved_set ((uchar *)&lop_gest.Data_lu, 0, sizeof(lop_gest.Data_lu));
				Os__saved_copy ((uchar *)"0000000000000",(uchar *)lop_gest.Data_lu, sizeof(lop_gest.Data_lu)-1);
				set_ushort_secourue(&lop_gest.Nbr_Data,0);
				Os__saved_set ((uchar *)&lop_gest.Status_Data_lu, 'P', sizeof(lop_gest.Status_Data_lu));

			}
			Action = R_Transac8583.champs72[i];
			i += 1;
		}
		switch(Action)
		{
			case 'B':
				memset(Buf_Card_Lop,0,NBR_MAX_CARD*10);
				OffSet = 0;
				Nb_Card = 0;
				j=R_Transac8583.l_champs72-i;
				status = Recup_LOP_BIN(&R_Transac8583.champs72[i],j,&OffSet,Buf_Card_Lop,&Nb_Card);
				if(status!=OK)
            		return KO;
				i+=OffSet;
				k=0;
				while(k<Nb_Card)
   				{
    				memcpy(Buffer,&Buf_Card_Lop[k*6],6) ;
      				if(SupprimerLnBin(Buffer)!=OK)
      				{
      					Os__xprint("LOP_INCOHERENTE");
      					return LOP_INCOHERENTE;
      				}
      				k+=(Ushort)1;
				}
				break;
			case 'R':
				memset(Buf_Card_Lop,0,NBR_MAX_CARD*10);
				OffSet = 0;
				Nb_Card = 0;
				j=R_Transac8583.l_champs72-i;
				status = Recup_LOP_Tranche(&R_Transac8583.champs72[i],j,&OffSet,Buf_Card_Lop,&Nb_Card);
				if(status!=OK)
            		return KO;
				i+=OffSet;
				k=0;
				while(k<Nb_Card)
   				{
    				memcpy(Buffer,&Buf_Card_Lop[k*20],20) ;
      				if(SupprimerLnTrn(Buffer)!=OK)
      				{
      					Os__xprint("LOP_INCOHERENTE");
      					return LOP_INCOHERENTE;
      				}
      				k+=(Ushort)1;
				}
				break;
			case 'C':
				memset(Buf_Card_Lop,0,NBR_MAX_CARD*10);
				OffSet = 0;
				Nb_Card = 0;
				j=R_Transac8583.l_champs72-i;
				status = Recup_LOP_Card(&R_Transac8583.champs72[i],j,&OffSet,Buf_Card_Lop,&Nb_Card);
				if(status!=OK)
            		return KO;
				i+=OffSet;
				k=0;
				while(k<Nb_Card)
   				{
    				memcpy(Buffer,&Buf_Card_Lop[k*10],10) ;
      				if(SupprimerNumero(Buffer)!=OK)
      				{
      					Os__xprint("LOP_INCOHERENTE");
      					return LOP_INCOHERENTE;
      				}
      				k+=(Ushort)1;
				}
				break;
			default:
				return KO;
		}
	}

	if(memcmp(R_Transac8583.champs24,"300",3)==0)
   	{
   		index_fin_ln = lop_gest.indexfinln;
   		for ( i=0, j=0 ; i < index_fin_ln; i++)
   		{
      		read_oppos_index(i, &Oppos ) ;
      		if(memcmp(Oppos.numero,"0000000000",10) != 0)
      		{
         		write_oppos_index(j,&Oppos );
         		j++;
      		}
   		}
   		set_ushort_secourue(&lop_gest.indexfinln,j);
		/////////////////////// BIN EN LOP ///////////////////////
		index_fin_ln = lop_gest.indexfinln_bin;
   		for ( i=0, j=0 ; i < index_fin_ln; i++)
   		{
      		read_oppos_bin_index(i, &OpposBin ) ;
      		if(memcmp(OpposBin.bin,"000000",6) != 0)
      		{
         		write_oppos_bin_index(j,&OpposBin );
         		j++;
      		}
   		}
   		set_ushort_secourue(&lop_gest.indexfinln_bin,j);
		/////////////////// TRANCHE DE CARTES EN LOP //////////////////
		index_fin_ln = lop_gest.indexfinln_trn;
   		for ( i=0, j=0 ; i < index_fin_ln; i++)
   		{
      		read_oppos_trn_index(i, &OpposTrn ) ;
      		if(memcmp(OpposTrn.trn_debut,"00000000000000000000",20) != 0)
      		{
         		write_oppos_trn_index(j,&OpposTrn );
         		j++;
      		}
   		}
   		set_ushort_secourue(&lop_gest.indexfinln_trn,j);
	}

	lop_gest.Action = Action;
	memset(Num_Bloc,0,4);
	memcpy(Num_Bloc,&lop_gest.NumBloc[0],3);
	l_num_bloc = atol((PC)Num_Bloc)+1;
	sprintf((PC)Num_Bloc,"%03lu",l_num_bloc);
	Os__saved_copy(Num_Bloc,(uchar *)&lop_gest.NumBloc[0],3) ;
   return OK ;
}

//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			NOM			: Trait_Lop_Ins
//			DESCRIPTION	: Traitement de l'insertion en Liste Noire
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			ENTREE		:rien
//
//			SORTIE		:status
//
//			FCT APP		:
//
//			REMARQUE	:
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
uchar Trait_Lop_Ins(uchar Premier_Mess)
{
	Ushort	i,j,l,m,OffSet,Nb_Card;
	uchar	status;
	uchar	Buf_Card_Lop[NBR_MAX_CARD*10];
	Ushort	k ;
	Ushort	index_fin_ln ;
	uchar	Buffer[21] ;
	typ_lop_enr_bcd Oppos;
	typ_lop_enr_bin OpposBin;
	typ_lop_enr_trn OpposTrn;
	Ushort 	indexinsln,indexcouln;

	uchar		Num_Bloc[4];
	uchar		Num_Bloc_Rec[17];
	Ulong		l_num_bloc;
	uchar		Action;


	memset(Num_Bloc_Rec,0,17);
	memset(Num_Bloc,0,4);

	memcpy(Num_Bloc,&lop_gest.NumBloc[0],3);
	Action = lop_gest.Action;
	sprintf((PC)Num_Bloc_Rec,"   INS : %.03s",Num_Bloc);
	CUMORE_CMI_PHONE_NUMBER((char*)Num_Bloc_Rec);
	CUMORE_CMI_AFFICHE_MESSAGE_SD(35,42,0);

	//Uti__display2((PC)Num_Bloc_Rec);

	i = 1;
	if((R_Transac8583.champs72[1]=='B')||(R_Transac8583.champs72[1]=='R')||(R_Transac8583.champs72[1]=='C'))
	{
		if(R_Transac8583.champs72[1]=='C')
		{
			Os__saved_set ((uchar *)&lop_gest.Bin_lu, 0, sizeof(lop_gest.Bin_lu));
			Os__saved_copy ((uchar *)"000000",(uchar *)lop_gest.Bin_lu, sizeof(lop_gest.Bin_lu)-1);
			Os__saved_set ((uchar *)&lop_gest.Data_lu, 0, sizeof(lop_gest.Data_lu));
			Os__saved_copy ((uchar *)"0000000000000",(uchar *)lop_gest.Data_lu, sizeof(lop_gest.Data_lu)-1);
			set_ushort_secourue(&lop_gest.Nbr_Data,0);
			Os__saved_set ((uchar *)&lop_gest.Status_Data_lu, 'P', sizeof(lop_gest.Status_Data_lu));

		}
		Action = R_Transac8583.champs72[1];
		i = 2;
	}

	if(Premier_Mess)
	{
		Os__saved_set((uchar *)&lop_gest.Bin_lu, 0, sizeof(lop_gest.Bin_lu));
		Os__saved_copy((uchar *)"000000",(uchar *)lop_gest.Bin_lu, sizeof(lop_gest.Bin_lu)-1);
		Os__saved_set((uchar *)&lop_gest.Data_lu, 0, sizeof(lop_gest.Data_lu));
		Os__saved_copy((uchar *)"0000000000000",(uchar *)lop_gest.Data_lu, sizeof(lop_gest.Data_lu)-1);
		set_ushort_secourue(&lop_gest.Nbr_Data,0);
		Os__saved_set((uchar *)&lop_gest.Status_Data_lu, 'P', sizeof(lop_gest.Status_Data_lu));

		set_ushort_secourue(&lop_gest.indexcouln,0);
		set_ushort_secourue(&lop_gest.indexinsln,0);
		index_fin_ln = lop_gest.indexfinln;
		// Decalage de la liste
		for (l=NBMAXOPP,j=index_fin_ln ; j > 0 ; l--, j--)
		{
			// Deplacement des porteurs
			m=j-1;
			read_oppos_index(m, &Oppos);
			m=l-1;
			write_oppos_index(m, &Oppos);
		}
		set_ushort_secourue(&lop_gest.indexinsln,l);

		///////////////////////// BIN EN LOP /////////////////////////////
		set_ushort_secourue(&lop_gest.indexcouln_bin,0);
		set_ushort_secourue(&lop_gest.indexinsln_bin,0);
		index_fin_ln = lop_gest.indexfinln_bin;
		// Decalage de la liste
		for (l=NBMAXOPPBIN,j=index_fin_ln ; j > 0 ; l--, j--)
		{
			// Deplacement des porteurs
			m=j-1;
			read_oppos_bin_index(m, &OpposBin);
			m=l-1;
			write_oppos_bin_index(m, &OpposBin);
		}
		set_ushort_secourue(&lop_gest.indexinsln_bin,l);

		/////////////////////TRANCHE DE CARTES EN LOP //////////////////////////
		set_ushort_secourue(&lop_gest.indexcouln_trn,0);
		set_ushort_secourue(&lop_gest.indexinsln_trn,0);
		index_fin_ln = lop_gest.indexfinln_trn;
		// Decalage de la liste
		for (l=NBMAXOPPTRN,j=index_fin_ln ; j > 0 ; l--, j--)
		{
			// Deplacement des porteurs
			m=j-1;
			read_oppos_trn_index(m, &OpposTrn);
			m=l-1;
			write_oppos_trn_index(m, &OpposTrn);
		}
		set_ushort_secourue(&lop_gest.indexinsln_trn,l);
	}

	status = OK;
	while((i<R_Transac8583.l_champs72)&&(status ==OK))
	{
		if((R_Transac8583.champs72[i]=='B')||(R_Transac8583.champs72[i]=='R')||(R_Transac8583.champs72[i]=='C'))
		{
			if(R_Transac8583.champs72[i]=='C')
			{
				Os__saved_set ((uchar *)&lop_gest.Bin_lu, 0, sizeof(lop_gest.Bin_lu));
				Os__saved_copy ((uchar *)"000000",(uchar *)lop_gest.Bin_lu, sizeof(lop_gest.Bin_lu)-1);
				Os__saved_set ((uchar *)&lop_gest.Data_lu, 0, sizeof(lop_gest.Data_lu));
				Os__saved_copy ((uchar *)"0000000000000",(uchar *)lop_gest.Data_lu, sizeof(lop_gest.Data_lu)-1);
				set_ushort_secourue(&lop_gest.Nbr_Data,0);
				Os__saved_set ((uchar *)&lop_gest.Status_Data_lu, 'P', sizeof(lop_gest.Status_Data_lu));

			}
			Action = R_Transac8583.champs72[i];
			i += 1;
		}
		switch(Action)
		{
		case 'B':
			memset(Buf_Card_Lop,0,NBR_MAX_CARD*10);
			OffSet = 0;
			Nb_Card = 0;
			m=R_Transac8583.l_champs72-i;
			status = Recup_LOP_BIN(&R_Transac8583.champs72[i],m,&OffSet,Buf_Card_Lop,&Nb_Card);
			i+=OffSet;
			if(status!=OK)
				return KO;
			//i+=OffSet;
			k=0;
			while(k<Nb_Card)
			{
				memcpy(Buffer,&Buf_Card_Lop[k*6],6);
				if(InsBinLop(Buffer)!= OK)
				{
					Os__xprint("LOP_INCOHERENTE");
					return LOP_INCOHERENTE;
				}
				k+=(Ushort)1;
			}
			break;
		case 'R':
			memset(Buf_Card_Lop,0,NBR_MAX_CARD*10);
			OffSet = 0;
			Nb_Card = 0;
			m=R_Transac8583.l_champs72-i;
			status = Recup_LOP_Tranche(&R_Transac8583.champs72[i],m,&OffSet,Buf_Card_Lop,&Nb_Card);
			if(status!=OK)
				return KO;
			i+=OffSet;
			k=0;
			while(k<Nb_Card)
			{
				memcpy(Buffer,&Buf_Card_Lop[k*20],20) ;
				if(InsTrnLop(Buffer)!= OK)
				{
					Os__xprint("LOP_INCOHERENTE");
					return LOP_INCOHERENTE;
				}
				k+=(Ushort)1;
			}
			break;
		case 'C':
			memset(Buf_Card_Lop,0,NBR_MAX_CARD*10);
			OffSet = 0;
			Nb_Card = 0;
			m=R_Transac8583.l_champs72-i;
			status = Recup_LOP_Card(&R_Transac8583.champs72[i],m,&OffSet,Buf_Card_Lop,&Nb_Card);
			if(status!=OK)
				return KO;
			i+=OffSet;
			k=0;
			while(k<Nb_Card)
			{
				memcpy(Buffer,&Buf_Card_Lop[k*10],10) ;
				if(InsNumero(Buffer)!= OK)
				{
					Os__xprint("LOP_INCOHERENTE");
					return LOP_INCOHERENTE;
				}
				k+=(Ushort)1;
			}
			break;
		default:
			return KO;
		}
	}

	// remise en place de la fin de la liste
	if(memcmp(R_Transac8583.champs24,"300",3)==0)
	{
		indexinsln = lop_gest.indexinsln;
		indexcouln = lop_gest.indexcouln;
		for (i=indexcouln,j=indexinsln;j<NBMAXOPP;i++,j++)
		{
			// Deplacement des porteurs
			read_oppos_index(j, &Oppos);
			write_oppos_index(i, &Oppos);
		}
		set_ushort_secourue(&lop_gest.indexfinln,i);
		/////////////////////// BIN EN LOP /////////////////////////
		indexinsln = lop_gest.indexinsln_bin;
		indexcouln = lop_gest.indexcouln_bin;
		for (i=indexcouln,j=indexinsln;j<NBMAXOPPBIN;i++,j++)
		{
			// Deplacement des porteurs
			read_oppos_bin_index(j, &OpposBin);
			write_oppos_bin_index(i, &OpposBin);
		}
		set_ushort_secourue(&lop_gest.indexfinln_bin,i);
		/////////////////////// TRANCHE DE CARTE EN LOP ///////////////
		indexinsln = lop_gest.indexinsln_trn;
		indexcouln = lop_gest.indexcouln_trn;
		for (i=indexcouln,j=indexinsln;j<NBMAXOPPTRN;i++,j++)
		{
			// Deplacement des porteurs
			read_oppos_trn_index(j, &OpposTrn);
			write_oppos_trn_index(i, &OpposTrn);
		}
		set_ushort_secourue(&lop_gest.indexfinln_trn,i);
	}
   ////////////////////////////////////////////////////////
	lop_gest.Action = Action;
	memset(Num_Bloc,0,4);
	memcpy(Num_Bloc,&lop_gest.NumBloc[0],3);
	l_num_bloc = atol((PC)Num_Bloc)+1;
	sprintf((PC)Num_Bloc,"%03lu",l_num_bloc);
	Os__saved_copy(Num_Bloc,(uchar *)&lop_gest.NumBloc[0],3) ;
   /////////////////////////////////////////////////////////
	return OK ;
}

//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			NOM			:InsNumero
//			DESCRIPTION	: Insertion d'un numéro dans la listes noire
//
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			ENTREE		: numéro à insérer , sa position
//
//			SORTIE		:Rien
//
//			FCT APP		:
//
//			REMARQUE	:
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/

uchar InsNumero(uchar *Buffer)
{
	Ushort i,j,m,indexinsln,indexcouln;
	typ_lop_enr_bcd Oppos;
	int	status;


	indexinsln = lop_gest.indexinsln;
	indexcouln = lop_gest.indexcouln;


	if(indexcouln >= indexinsln)
	{
		Os__xprint("Debordement");
		return KO;
	}

	for ( j=indexinsln,i=indexcouln;j<NBMAXOPP;i++,j++)
	{
		read_oppos_index(j, &Oppos);
		status = memcmp(Buffer, Oppos.numero,10);
		if(status==0)
			//return KO;Ajouter le 09/03/2005
			return OK;
		if(status>0)
			break;
		write_oppos_index(i, &Oppos);
	}

	if ( i != 0 )
	{
		m=i-1;
		read_oppos_index(m, &Oppos);
		status = memcmp( Oppos.numero, Buffer,10);
	}

	if (status > 0 )
	{
		memcpy(&Oppos.numero[0],Buffer,10);
		write_oppos_index(i,&Oppos);
		i++;

		set_ushort_secourue(&lop_gest.indexcouln,i);
		set_ushort_secourue(&lop_gest.indexinsln,j);
	}
	return OK;
}

//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			NOM			:InsBinLop
//			DESCRIPTION	: Insertion d'un numéro dans la listes noire
//
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			ENTREE		: numéro à insérer , sa position
//
//			SORTIE		:Rien
//
//			FCT APP		:
//
//			REMARQUE	:
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
uchar InsBinLop(uchar *Buffer)
{
	Ushort i,j,m,indexinsln,indexcouln;
	typ_lop_enr_bin Bin_Oppos;
	int	status;

	indexinsln = lop_gest.indexinsln_bin;
	indexcouln = lop_gest.indexcouln_bin;

	if(indexinsln == indexcouln)
	{
		Os__xprint("Debordement");
		//return KO;Ajouter le 09/03/2005
		return OK;
	}

	for ( j=indexinsln,i=indexcouln;j<NBMAXOPPBIN;i++,j++)
	{
		read_oppos_bin_index(j, &Bin_Oppos);
		status = memcmp(Buffer, Bin_Oppos.bin,6);
		if(status == 0)
      		return OK;

		if(status > 0)
			break;
		write_oppos_bin_index(i, &Bin_Oppos);
	}

	if ( i != 0 )
	{
		m=i-1;
		read_oppos_bin_index(m, &Bin_Oppos);
		status = memcmp( Bin_Oppos.bin, Buffer,6);
	}

	if (status > 0 )
	{
		memcpy(&Bin_Oppos.bin[0],Buffer,6);
		write_oppos_bin_index(i,&Bin_Oppos);
		i++;
		set_ushort_secourue(&lop_gest.indexcouln_bin,i);
		set_ushort_secourue(&lop_gest.indexinsln_bin,j);
	}
	return OK ;
}

//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			NOM			:InsTrnLop
//			DESCRIPTION	: Insertion d'un numéro dans la listes noire
//
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			ENTREE		: numéro à insérer , sa position
//
//			SORTIE		:Rien
//
//			FCT APP		:
//
//			REMARQUE	:
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/

uchar InsTrnLop(uchar *Buffer)
{
	Ushort i,j,m,indexinsln,indexcouln;
	typ_lop_enr_trn Trn_Oppos;
	int	status;


	indexinsln = lop_gest.indexinsln_trn;
	indexcouln = lop_gest.indexcouln_trn;

	if(indexinsln == indexcouln)
	{
		Os__xprint("Debordement");
		return KO;
	}

	for ( j=indexinsln,i=indexcouln;j<NBMAXOPPTRN;i++,j++)
	{
		read_oppos_trn_index(j, &Trn_Oppos);
		if ( (status = memcmp(Buffer, Trn_Oppos.trn_debut,10)) > 0 )
			break;
		write_oppos_trn_index(i, &Trn_Oppos);
	}

	if ( i != 0 )
	{
		m=i-1;
		read_oppos_trn_index(m, &Trn_Oppos);
		status = memcmp( Trn_Oppos.trn_debut, Buffer,10);
	}

	if (status > 0 )
	{
		memcpy(&Trn_Oppos.trn_debut[0],Buffer,10);
		memcpy(&Trn_Oppos.trn_fin[0],Buffer+10,10);
		write_oppos_trn_index(i,&Trn_Oppos);
		i++;
		set_ushort_secourue(&lop_gest.indexcouln_trn,i);
		set_ushort_secourue(&lop_gest.indexinsln_trn,j);
	}

	return OK ;
}

//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			NOM			:SupprimerNumero
//			DESCRIPTION	: Suppression d'un numéro de la listes noire
//
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			ENTREE		: numéro à insérer , sa position
//
//			SORTIE		:Rien
//
//			FCT APP		:
//
//			REMARQUE	:
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
uchar SupprimerNumero(uchar *Buffer)
{
	Ushort index,indexcouln,indexfinln,j;
	typ_lop_enr_bcd Oppos;
	short	Status;


	////////////////////////////////////////////////////////

	indexcouln = lop_gest.indexcouln;
	indexfinln = lop_gest.indexfinln;
	for (index=indexcouln; index < indexfinln; index++)
	{
		read_oppos_index(index, &Oppos);
		if ((Status = memcmp(Buffer, Oppos.numero,10)) == 0)
		{
			memset(&Oppos.numero[0],'0',sizeof(typ_lop_enr_bcd));
			write_oppos_index(index,&Oppos );
			j = index+1;

			set_ushort_secourue(&lop_gest.indexcouln,j);
			return OK;
		}
		if(Status > 0)
			break;
	}
	return OK;
	////////////////////////////////////////////////////////
}

//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			NOM			:SupprimerLnBin
//			DESCRIPTION	: Suppression d'un numéro de la listes noire
//
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			ENTREE		: numéro à insérer , sa position
//
//			SORTIE		:Rien
//
//			FCT APP		:
//
//			REMARQUE	:
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/

uchar SupprimerLnBin(uchar *Buffer)
{
	Ushort index,indexcouln,indexfinln,j;
	short  Status;
	typ_lop_enr_bin Bin_Oppos;



	////////////////////////////////////////////////////////
	indexcouln = lop_gest.indexcouln_bin;
	indexfinln = lop_gest.indexfinln_bin;
	for (index=indexcouln; index < indexfinln; index++)
	{
		read_oppos_bin_index(index, &Bin_Oppos);
		if ((Status = memcmp(Buffer, Bin_Oppos.bin,6)) == 0)
		{
			memset(&Bin_Oppos.bin[0],'0',sizeof(typ_lop_enr_bin));
			write_oppos_bin_index(index,&Bin_Oppos );
			j = index+1;
			set_ushort_secourue(&lop_gest.indexcouln_bin,j);
			return OK;
		}
		if(Status > 0)
			break;
	}
	return OK;
	/////////////////////////////////////////////////////////
}

//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			NOM			:SupprimerLnTrn
//			DESCRIPTION	: Suppression d'un numéro de la listes noire
//
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			ENTREE		: numéro à insérer , sa position
//
//			SORTIE		:Rien
//
//			FCT APP		:
//
//			REMARQUE	:
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/

uchar SupprimerLnTrn(uchar *Buffer)
{
	Ushort index,indexcouln,indexfinln,j;
	short  Status;
	typ_lop_enr_trn Trn_Oppos;

	////////////////////////////////////////////////////////
	indexcouln = lop_gest.indexcouln_trn;
	indexfinln = lop_gest.indexfinln_trn;
	for (index=indexcouln; index < indexfinln; index++)
	{
		read_oppos_trn_index(index, &Trn_Oppos);
		if ((Status = memcmp(Buffer, Trn_Oppos.trn_debut,10)) == 0)
		{
			memset(&Trn_Oppos.trn_debut[0],'0',sizeof(typ_lop_enr_trn));
			write_oppos_trn_index(index,&Trn_Oppos );
			j = index+1;
			set_ushort_secourue(&lop_gest.indexcouln_trn,j);
			return OK;
		}
	}
	return OK;
	////////////////////////////////////////////////////////
}

/********************************************************************************************/
/************************************************************************
*                                                                       *
*       SOUS-PROGRAMME : read_oppos_index( Index, Pt_numero )           *
*                                                                       *
*************************************************************************
*                                                                       *
*       Fonction : Lecture d'un enregistrement d'oppositions dans la    *
*                  liste sur le premier,le deuxieme ou le troisieme     *
*									tableau                             *
*                                                                       *
*       Entree : index dans la liste                                    *
*                pointeur du numero … remplir                           *
*                                                                       *
*       Sortie : neant                                                  *
*                                                                       *
************************************************************************/
void read_oppos_index(Ushort Index , typ_lop_enr_bcd *Pt_numero )
{
	// Tableau page 0 : Remplissage du champ de retour
	if (Index<NBMAXOPP)
	{
		memcpy( Pt_numero, &lop_fichier[Index], sizeof( typ_lop_enr_bcd));
	}
}

/************************************************************************
*                                                                       *
*       SOUS-PROGRAMME : read_oppos_index( Index, Pt_numero )           *
*                                                                       *
*************************************************************************
*                                                                       *
*       Fonction : Lecture d'un enregistrement d'oppositions dans la    *
*                  liste sur le premier,le deuxieme ou le troisieme     *
*									tableau                                      *
*                                                                       *
*       Entree : index dans la liste                                    *
*                pointeur du numero … remplir                           *
*                                                                       *
*       Sortie : neant                                                  *
*                                                                       *
************************************************************************/
void read_oppos_bin_index(Ushort Index , typ_lop_enr_bin *Pt_numero )
{

	memcpy( Pt_numero, &lop_bin[Index], sizeof( typ_lop_enr_bin));
}

/************************************************************************
*                                                                       *
*       SOUS-PROGRAMME : read_oppos_index( Index, Pt_numero )           *
*                                                                       *
*************************************************************************
*                                                                       *
*       Fonction : Lecture d'un enregistrement d'oppositions dans la    *
*                  liste sur le premier,le deuxieme ou le troisieme     *
*									tableau                                      *
*                                                                       *
*       Entree : index dans la liste                                    *
*                pointeur du numero … remplir                           *
*                                                                       *
*       Sortie : neant                                                  *
*                                                                       *
************************************************************************/
void read_oppos_trn_index(Ushort Index , typ_lop_enr_trn *Pt_numero )
{

	memcpy( Pt_numero, &lop_trn[Index], sizeof( typ_lop_enr_trn));
}

/************************************************************************
*                                                                       *
*       SOUS-PROGRAMME : write_oppos_index( Index, Pt_numero )          *
*                                                                       *
*************************************************************************
*                                                                       *
*       Fonction : Ecriture d'un enregistrement d'oppositions dans la   *
*                  liste sur le premier ou le deuxieme tableau          *
*                  mise … jour du checksum de la liste                  *
*                                                                       *
*       Entree : index dans la liste                                    *
*                pointeur du numero … stocker                           *
*                                                                       *
*       Sortie : neant                                                  *
*                                                                       *
************************************************************************/
void write_oppos_index(Uint Index , typ_lop_enr_bcd *Pt_numero )
{

	// Tableau page 2 : Remplissage du champ de retour
	if (Index<NBMAXOPP)
	{
		Os__saved_copy( (uchar*) Pt_numero, (uchar*)&lop_fichier[Index],sizeof( typ_lop_enr_bcd));
	}
}

/************************************************************************
*                                                                       *
*       SOUS-PROGRAMME : write_oppos_index( Index, Pt_numero )          *
*                                                                       *
*************************************************************************
*                                                                       *
*       Fonction : Ecriture d'un enregistrement d'oppositions dans la   *
*                  liste sur le premier ou le deuxieme tableau          *
*                  mise … jour du checksum de la liste                  *
*                                                                       *
*       Entree : index dans la liste                                    *
*                pointeur du numero … stocker                           *
*                                                                       *
*       Sortie : neant                                                  *
*                                                                       *
************************************************************************/
void write_oppos_bin_index(Uint Index , typ_lop_enr_bin *Pt_numero )
{

	Os__saved_copy( (uchar*) Pt_numero, (uchar*)&lop_bin[Index],
   			sizeof( typ_lop_enr_bin));
}
/************************************************************************
*                                                                       *
*       SOUS-PROGRAMME : write_oppos_index( Index, Pt_numero )          *
*                                                                       *
*************************************************************************
*                                                                       *
*       Fonction : Ecriture d'un enregistrement d'oppositions dans la   *
*                  liste sur le premier ou le deuxieme tableau          *
*                  mise … jour du checksum de la liste                  *
*                                                                       *
*       Entree : index dans la liste                                    *
*                pointeur du numero … stocker                           *
*                                                                       *
*       Sortie : neant                                                  *
*                                                                       *
************************************************************************/
void write_oppos_trn_index(Uint Index , typ_lop_enr_trn *Pt_numero )
{

	Os__saved_copy( (uchar*) Pt_numero, (uchar*)&lop_trn[Index],
   			sizeof( typ_lop_enr_trn));
}

/************************************************************************
*                                                                       *
*       SOUS-PROGRAMME :  rechlop(  numcarte )                          *
*                                                                       *
*************************************************************************
*                                                                       *
*       Fonction : recherche d'un numero dans la liste d'oppositions  	*
*                                                                       *
*       Entree :   piste iso2                                    			*
*                                                                       *
*       Sortie :   status  ( KO = trouve )                  				*
*                            OK = non trouve                            *
*                            FCB_LOPER erreur lop                       *
*                            FCB_LOPPE lop perimee                      *
************************************************************************/
uchar	rechlop(uchar *numero)
{
	Ushort	i,indexinf,indexsup;
	short   	stat;
	typ_lop_enr_bcd Oppos;  // Numero lu ou ecrit

	indexinf = 0;
	if(lop_gest.indexfinln==0)
   		return OK;
	indexsup = lop_gest.indexfinln-1;

	read_oppos_index(indexinf, &Oppos);
	if ( memcmp(numero, Oppos.numero,10 ) == 0 )
		return KO;

	read_oppos_index(indexsup, &Oppos);
	if ( memcmp(numero, Oppos.numero,10 ) == 0 )
		return KO;

	while((i = (indexinf + indexsup) / 2) != indexinf)
	{
		read_oppos_index(i, &Oppos);
		if ( (stat=(short)memcmp(numero, Oppos.numero,10 )) == 0 )
			return KO;
		//if (stat > 0)
		if (stat < 0)
			indexinf = i;
		else
			indexsup = i;
	}
	return OK;
}

/************************************************************************
*                                                                       *
*       SOUS-PROGRAMME :  rechlop(  numcarte )                          *
*                                                                       *
*************************************************************************
*                                                                       *
*       Fonction : recherche d'un numero dans la liste d'oppositions  	*
*                                                                       *
*       Entree :   bin                                         			*
*                                                                       *
*       Sortie :   status  ( KO = trouve )                  				*
*                            OK = non trouve                            *
*                            FCB_LOPER erreur lop                       *
*                            FCB_LOPPE lop perimee                      *
************************************************************************/
uchar	rechlop_bin(uchar *numero)
{
	Ushort	i,indexinf,indexsup;
	short   	stat;
	typ_lop_enr_bin Bin_Oppos;  // Numero lu ou ecrit

	indexinf = 0;
	if(lop_gest.indexfinln_bin==0)
   		return OK;
	indexsup = lop_gest.indexfinln_bin-1;
	read_oppos_bin_index(indexinf, &Bin_Oppos);
	if ( memcmp(numero, Bin_Oppos.bin,6 ) == 0 )
		return KO;

	read_oppos_bin_index(indexsup, &Bin_Oppos);
	if ( memcmp(numero, Bin_Oppos.bin,6 ) == 0 )
		return KO;

	while((i = (indexinf + indexsup) / 2) != indexinf)
	{
		read_oppos_bin_index(i, &Bin_Oppos);
		if ( (stat=(short)memcmp(numero, Bin_Oppos.bin,6 )) == 0 )
			return KO;
		//if (stat > 0)
		if (stat < 0)
			indexinf = i;
		else
			indexsup = i;
	}
	return OK;
}

/************************************************************************
*                                                                       *
*       SOUS-PROGRAMME :  rechlop(  numcarte )                          *
*                                                                       *
*************************************************************************
*                                                                       *
*       Fonction : recherche d'un numero dans la liste d'oppositions  	*
*                                                                       *
*       Entree :   piste iso2                                    			*
*                                                                       *
*       Sortie :   status  ( KO = trouve )                  				*
*                            OK = non trouve                            *
*                            FCB_LOPER erreur lop                       *
*                            FCB_LOPPE lop perimee                      *
************************************************************************/
uchar	rechlop_trn(uchar *numero)
{
	Ushort	i,indexinf,indexsup;
	short   	stat;
	typ_lop_enr_trn Trn_Oppos;  // Numero lu ou ecrit

	indexinf = 0;
	if(lop_gest.indexfinln_trn==0)
   		return OK;
	indexsup = lop_gest.indexfinln_trn-1;
	read_oppos_trn_index(indexinf, &Trn_Oppos);
	if (
		(memcmp(numero, Trn_Oppos.trn_debut,10 ) >= 0) &&
		(memcmp(numero, Trn_Oppos.trn_fin,10 ) <= 0)
	)
		return KO;

	read_oppos_trn_index(indexsup, &Trn_Oppos);
	if (
		(memcmp(numero, Trn_Oppos.trn_debut,10 ) >= 0) &&
		(memcmp(numero, Trn_Oppos.trn_fin,10 ) <= 0)
	)
		return KO;

	while((i = (indexinf + indexsup) / 2) != indexinf)
	{
		read_oppos_trn_index(i, &Trn_Oppos);
		if (
			(memcmp(numero, Trn_Oppos.trn_debut,10 ) >= 0) &&
			(memcmp(numero, Trn_Oppos.trn_fin,10 ) <= 0)
		)
			return KO;
		stat=(short)memcmp(numero, Trn_Oppos.trn_debut,10 );
		//if (stat > 0)
		if (stat < 0)
			indexinf = i;
		else
			indexsup = i;
	}
	return OK;
}

/******************************************************************************************/

//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			NOM			:	rechlop_index
//			DESCRIPTION	: 	Cherche un emplacement pour un numéro dans la liste LN2
//
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			ENTREE		:Numéro à chercher
//
//			SORTIE		:Resultat de recherche
//
//			FCT APP		:
//
//			REMARQUE	:
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
/*uchar rechlop_index(uchar *Buffer,Ushort *index)
{
	Ushort		indexinf ;
	Ushort		indexsup ;
	Ushort		i ;
	short			stat;

	typ_lop_enr_bcd Oppos;  // Numero lu ou ecrit

	indexinf =	0 ;
	indexsup = lop_gest.indexfinln-1;

	read_oppos_index(indexinf, &Oppos);
	if ((stat = (short)memcmp(Buffer, Oppos.numero,10 )) == 0 )
	{
		*index = indexinf;
		return KO;
	}
	else
	{
		//if(stat > 0)
		if((stat > 0) && memcmp("0000000000", Oppos.numero,10 ))
		{
			*index = indexinf;
			return OK;
		}
	}

	read_oppos_index(indexsup, &Oppos);
	if ((stat = (short)memcmp(Buffer, Oppos.numero,10 )) == 0 )
	{
		*index = indexsup;
		return KO;
	}
	else
	{
		//if(stat < 0)
		if((stat < 0) && memcmp("0000000000", Oppos.numero,10 ))
		{
			*index = indexsup +1;
			return OK;
		}
	}

	while((i = (indexinf + indexsup) / 2) != indexinf)
	{
		read_oppos_index(i, &Oppos);
		if ( (stat = (short)memcmp(Buffer, Oppos.numero,10 )) == 0 )
		{
			*index = i;
			return KO;
		}
		//if (stat > 0)
		if (stat < 0)
			indexinf = i;
		else
			indexsup = i;
	}
	*index = i+1;
	return OK;
}*/


//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			NOM			:	rechlop_index
//			DESCRIPTION	: 	Cherche un emplacement pour un numéro dans la liste LN2
//
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			ENTREE		:Numéro à chercher
//
//			SORTIE		:Resultat de recherche
//
//			FCT APP		:
//
//			REMARQUE	:
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
/*uchar rechlop_bin_index(uchar *Buffer,Ushort *index)
{
	Ushort		indexinf ;
	Ushort		indexsup ;
	Ushort		i ;
	short			stat;

	typ_lop_enr_bin Bin_Oppos;  // Numero lu ou ecrit

	indexinf =	0 ;
	indexsup = lop_gest.indexfinln_bin-1;

	read_oppos_bin_index(indexinf, &Bin_Oppos);
	if ((stat = (short)memcmp(Buffer, Bin_Oppos.bin,6 )) == 0 )
	{
		*index = indexinf;
		return KO;
	}
	else
	{
		//if(stat < 0)
		if(stat > 0)
		{
			*index = indexinf;
			return OK;
		}
	}

	read_oppos_bin_index(indexsup, &Bin_Oppos);
	if ( (stat = (short)memcmp(Buffer, Bin_Oppos.bin,6 )) == 0 )
	{
		*index = indexsup;
		return KO;
	}
	else
	{
		//if(stat > 0)
		if(stat < 0)
		{
			*index = indexsup +1;
			return OK;
		}
	}

	while((i = (indexinf + indexsup) / 2) != indexinf)
	{
		read_oppos_bin_index(i, &Bin_Oppos);
		if ( (stat = (short)memcmp(Buffer, Bin_Oppos.bin,6 )) == 0 )
		{
			*index = i;
			return KO;
		}
		//if (stat > 0)
		if (stat < 0)
			indexinf = i;
		else
			indexsup = i;
	}
	*index = i+1;
	return OK;
}*/

//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			NOM			:	rechlop_index
//			DESCRIPTION	: 	Cherche un emplacement pour un numéro dans la liste LN2
//
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			ENTREE		:Numéro à chercher
//
//			SORTIE		:Resultat de recherche
//
//			FCT APP		:
//
//			REMARQUE	:
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
/*uchar rechlop_trn_index(uchar *Buffer,Ushort *index)
{
	Ushort		indexinf ;
	Ushort		indexsup ;
	Ushort		i ;
	short			stat;

	typ_lop_enr_trn Trn_Oppos;  // Numero lu ou ecrit

	indexinf =	0 ;
	indexsup = lop_gest.indexfinln_trn-1;

	read_oppos_trn_index(indexinf, &Trn_Oppos);
	if (
		(memcmp(Buffer, Trn_Oppos.trn_debut,10 ) >= 0) &&
		(memcmp(Buffer+10, Trn_Oppos.trn_fin,10 ) <= 0)
	)
	{
		*index = indexinf;
		return KO;
	}
	else
	{
		stat = (short)memcmp(Buffer+10, Trn_Oppos.trn_debut,10 );
		//if(stat < 0)
		if(stat > 0)
		{
			*index = indexinf;
			return OK;
		}
	}

	read_oppos_trn_index(indexsup, &Trn_Oppos);
	if (
		(memcmp(Buffer, Trn_Oppos.trn_debut,10 ) >= 0) &&
		(memcmp(Buffer+10, Trn_Oppos.trn_fin,10 ) <= 0)
	)
	{
		*index = indexsup;
		return KO;
	}
	else
	{
		stat = (short)memcmp(Buffer, Trn_Oppos.trn_fin,10 );
		//if(stat < 0)
		if(stat < 0)
		{
			*index = indexsup +1;
			return OK;
		}
	}

	while((i = (indexinf + indexsup) / 2) != indexinf)
	{
		read_oppos_trn_index(i, &Trn_Oppos);
		if (
			(memcmp(Buffer, Trn_Oppos.trn_debut,10 ) >= 0) &&
			(memcmp(Buffer+10, Trn_Oppos.trn_fin,10 ) <= 0)
		)
		{
			*index = i;
			return KO;
		}
		stat = (short)memcmp(Buffer, Trn_Oppos.trn_debut,10 );
		//if (stat > 0)
		if (stat < 0)
			indexinf = i;
		else
			indexsup = i;
	}
	*index = i+1;
	return OK;
}*/

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
uchar	Recup_LOP_BIN(uchar *buffer_mess, Ushort Lg_mess, Ushort *OffSet,uchar *Buf_BIN,Ushort *Nb_bin)
{
	Ushort	i,k,Lg;

	for(i=0;((i<Lg_mess)&&(*(buffer_mess + *OffSet +i)!='B')&&(*(buffer_mess + *OffSet +i)!='R')&&(*(buffer_mess + *OffSet +i)!='C'));i+=1);

	Lg = i;

	if(*(buffer_mess + *OffSet)=='*')
	   *OffSet +=1;

	while(*OffSet<Lg)
	{
		k = (*Nb_bin)*6;
		memcpy(Buf_BIN+k,buffer_mess + *OffSet,6);
		*OffSet +=6;
		*Nb_bin +=1;

		//if((*(buffer_mess + *OffSet)!='*')&&(*OffSet!=Lg))
		if(*OffSet!=Lg)
			*OffSet +=1;
	}
	return OK;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
uchar	Recup_LOP_Tranche(uchar *buffer_mess, Ushort Lg_mess, Ushort *OffSet,uchar *Buf_tranche,Ushort *Nb_tranche)
{
	Ushort	i,k=0,Lg;

	for(i=0;((i<Lg_mess)&&(*(buffer_mess+i)!='B')&&(*(buffer_mess+i)!='R')&&(*(buffer_mess+i)!='C'));i+=1);
	Lg = i;
	if(*(buffer_mess + *OffSet)=='*')
	   *OffSet +=1;

	if(*(buffer_mess + *OffSet)=='?')
	{
		for(i=0;((i<Lg)&&(*(buffer_mess + *OffSet +i)!='*'));i+=1);
		if(i>=20)
			return KO;
		memset(g_tranche_fin,0x0f,20);
		memcpy(g_tranche_fin,g_tranche_deb,g_Data_trch);
		memcpy(&g_tranche_fin[g_Data_trch-i],buffer_mess + *OffSet,i);
		*OffSet += i;
		if(*OffSet!=Lg)
			*OffSet +=1;
		asc_bcd(Buf_tranche+k,10,g_tranche_fin,20);
		k +=10;
		*Nb_tranche +=1;
	}

	k = (*Nb_tranche)*20;

	while(*OffSet<Lg)
	{
		for(i=0;(((*OffSet+i)<Lg)&&(*(buffer_mess + *OffSet +i)!='?'));i+=1);
		if(i>=20)
			return KO;
		g_Data_trch = i;
		memset(g_tranche_deb,0x0f,20);
		memcpy(g_tranche_deb,buffer_mess + *OffSet,g_Data_trch);
		*OffSet += (g_Data_trch+1);
		asc_bcd(Buf_tranche+k,10,g_tranche_deb,20);
		k +=10;

		if(*OffSet<Lg)
		{
			for(i=0;(((*OffSet+i)<Lg)&&(*(buffer_mess + *OffSet +i)!='*'));i+=1);
			if(i>=20)
				return KO;
			memset(g_tranche_fin,0x0f,20);
			memcpy(g_tranche_fin,g_tranche_deb,g_Data_trch);
			memcpy(&g_tranche_fin[g_Data_trch-i],buffer_mess + *OffSet,i);
			*OffSet += i;
			if(*OffSet!=Lg)
				*OffSet +=1;
			asc_bcd(Buf_tranche+k,10,g_tranche_fin,20);
			k +=10;
			*Nb_tranche +=1;
		}
	}
	return OK;
}

/* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
/* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
uchar	Recup_LOP_Card(uchar *buffer_mess, Ushort Lg_mess, Ushort *OffSet,uchar *Buf_Card,Ushort *Nb_card)
{
	Ushort 		i,Lg;
	uchar		Action,Status;


	memcpy(g_Bin_lu,(uchar *)lop_gest.Bin_lu, sizeof(lop_gest.Bin_lu)-1);
	memcpy(g_Data_lu,(uchar *)lop_gest.Data_lu, sizeof(lop_gest.Data_lu)-1);
	g_nbr_Data = lop_gest.Nbr_Data;
	g_Status_data_lu = lop_gest.Status_Data_lu;


	for(i=0;((i<Lg_mess)&&(*(buffer_mess+i)!='B')&&(*(buffer_mess+i)!='R')&&(*(buffer_mess+i)!='C'));i+=1);
	Lg = i;
	*OffSet = 0;

	while(*OffSet<Lg)
	{
		switch(*(buffer_mess+*OffSet))
		{
			case '*':
				if(g_Status_data_lu=='P')
					return KO;
				else
					if(g_Status_data_lu=='B')
						Action = LIRE_CARD;
					else
						Action = LIRE_BIN;
				*OffSet +=1;
				break;

			case '?':
				if(g_Status_data_lu=='P')
					return KO;
				Action = LIRE_CARD;
				*OffSet +=1;
				break;

			default :
				Action = LIRE_BIN;
				break;
		}
		Status = Lire_Bin_Carte(buffer_mess, Lg, OffSet, Action, Buf_Card, Nb_card);
		if(Status != OK)
			return KO;
	}

	Os__saved_copy ((uchar *)g_Bin_lu,(uchar *)lop_gest.Bin_lu, sizeof(lop_gest.Bin_lu)-1);
	Os__saved_copy ((uchar *)g_Data_lu,(uchar *)lop_gest.Data_lu, sizeof(lop_gest.Data_lu)-1);
	set_ushort_secourue(&lop_gest.Nbr_Data,g_nbr_Data);
	Os__saved_set ((uchar *)&lop_gest.Status_Data_lu, g_Status_data_lu, sizeof(lop_gest.Status_Data_lu));

	return OK;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
uchar	Lire_Bin_Carte(uchar *buffer_mess, Ushort Lg, Ushort *OffSet,uchar Action, uchar *Buf_Card,Ushort *Nb_card)
{
	Ushort		i,j,k,l;
	Ulong 		ul_Bin_tmp;
	uchar		Bin_tmp[7];
	uchar		Holder_Card[20],Data_tmp[14],Data_Res[14];
	uchar		Holder_Card_bcd[10];
	int			Res;


	memset(Bin_tmp,0,7);

	if(Action== LIRE_BIN)
	{
		for(i=0;((i<Lg)&&(*(buffer_mess + *OffSet +i)!='*'));i+=1);
		if(i!=0)
		{
			if(i>6)
				return KO;
			memcpy(Bin_tmp,buffer_mess + *OffSet,i);
			g_Status_data_lu = 'B';
		}
		else
			return KO;
		*OffSet+=(i+1);
		if(memcmp(g_Bin_lu,"000000",6))
			ul_Bin_tmp = atol((PC)g_Bin_lu) - atol((PC)Bin_tmp);
		else
			ul_Bin_tmp = atol((PC)Bin_tmp) - atol((PC)g_Bin_lu);
        long_asc(g_Bin_lu,6,&ul_Bin_tmp);
		memcpy(g_Data_lu, "0000000000000",sizeof(g_Data_lu)-1);
		g_nbr_Data = 0;
	}

	j=0;
	l = *OffSet;
	while(*OffSet<Lg)
	{
		if((*OffSet==Lg-1)||(*(buffer_mess + *OffSet)=='?')||(*(buffer_mess + *OffSet)=='*'))
		{
			if(*OffSet==Lg-1)
			{
				j+=1;
				*OffSet+=1;
			}

      	if(g_nbr_Data==0)
         	g_nbr_Data = j;

      	memset(Data_tmp,'0',sizeof(Data_tmp));
			Data_tmp[sizeof(Data_tmp)-1]=0;
			if(g_nbr_Data<j)
				return KO;
			memcpy(&Data_tmp[g_nbr_Data-j],buffer_mess+l,j);
			Res = subtract ( g_Data_lu, Data_tmp, Data_Res);
			memcpy(g_Data_lu, Data_Res,g_nbr_Data);

			memset(Holder_Card,0x0f,sizeof(Holder_Card));
			memcpy(Holder_Card,g_Bin_lu,6);
			memcpy(&Holder_Card[6],g_Data_lu,g_nbr_Data);
			asc_bcd(Holder_Card_bcd,10,Holder_Card,20);
			k = (*Nb_card)*10;
			memcpy(Buf_Card+k,Holder_Card_bcd,10);
			*Nb_card +=1;
			if(*Nb_card > NBR_MAX_CARD)
				return KO;
			j=0;
			l = *OffSet+1;
			g_Status_data_lu = 'C';
			if((*(buffer_mess + *OffSet)!='*')&&(*OffSet!=Lg))
				*OffSet+=1;
			else
				break;
		}
      if(*OffSet!=(Lg-1))
		{
			j+=1;
			*OffSet+=1;
		}
	}
	return OK;
}

//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			NOM			: Trait_transfer_Lop
//			DESCRIPTION	: Traitement de transfer de la liste noire
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			ENTREE		: Rien
//
//			SORTIE		:resultat de la transmisson
//
//			REMARQUE	:
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
uchar	Trait_transfer_Lop(uchar Premier_Mess)
{
	uchar	TypMaj,trait_status;
	Ushort	l_Champ24;
	uchar	c_Champ24[4];
	uchar	Premier_Mess_Ajout=0;
	Ushort 	i,j,index_fin_ln ;
	typ_lop_enr_bcd Oppos;
	uchar	Reprise;

	memset(c_Champ24,0,4);
	memcpy(c_Champ24,R_Transac8583.champs24,3);
	l_Champ24 = (Ushort)atol((PC)c_Champ24);
	//eft30printhexKamal(R_Transac8583.champs24,3);
	//eft30printhexKamal(R_Transac8583.champs72,1);
	if(l_Champ24==308)
	{
		Init_Lop();
		TypMaj = LOP_COMPLETE;
	}
	Os__saved_set ((uchar *)&lop_gest.stat, KO, 1);
	if(memcmp(lop_gest.NumBloc,"001",3)==0)
		Os__saved_set ((uchar *)&lop_gest.reprise, KO, 1);
	else
	{
		if(memcmp(lop_gest.NumBloc,&R_Transac8583.champs71[5],3)==0)
		{
			if(memcmp(lop_gest.NumListInt,&R_Transac8583.champs63[12],4))
			{
				Init_Lop();
				return KO;
			}
		}
		else
			return KO;
	}

	Reprise = lop_gest.reprise;
	//if(Premier_Mess)
	if((Premier_Mess)&&(Reprise!=OK))
	{
   		switch(l_Champ24)
		{
		case 301:
        	Premier_Mess_Ajout = 1;
			TypMaj = LOP_DIFF_AJOUT;
         	break;

		case 303:
			TypMaj = LOP_DIFF_SUPP;
        	break;

		case 308:
        	Init_Lop();
			TypMaj = LOP_COMPLETE;
        	break;

		case 300:
        	if(R_Transac8583.champs72[0]=='A')
			{
				Premier_Mess_Ajout = 1;
				TypMaj = LOP_DIFF_AJOUT;
			}
			else
			{
				if(R_Transac8583.champs72[0]=='D')
            		TypMaj = LOP_DIFF_SUPP;
				else
					if(R_Transac8583.champs72[0]=='N')
						TypMaj = PAS_DE_MAJ;
					else
            			return KO;
			}
			break;

		// Ajouter le 09/03/2005
		case 309:
			if(R_Transac8583.champs72[0]=='N')
				TypMaj = PAS_DE_MAJ;
			else
         		return KO;
			break;

		default:
        	return KO;
		}
		Os__saved_set ((uchar *)&lop_gest.typmis, TypMaj, 1);
	}
	else
	{
   		TypMaj = lop_gest.typmis;
		if(l_Champ24==308)
		{
			Init_Lop();
			TypMaj = LOP_COMPLETE;
		}
		if(TypMaj!=LOP_COMPLETE)
		{
      		switch(l_Champ24)
      		{
         		case 301:
						if(TypMaj==LOP_DIFF_SUPP)
						{
   							index_fin_ln = lop_gest.indexfinln;
   							for ( i=0, j=0 ; i < index_fin_ln; i++)
   							{
      							read_oppos_index(i, &Oppos ) ;
      							if(memcmp(Oppos.numero,"0000000000",10) != 0)
      							{
         							write_oppos_index(j,&Oppos );
         							j++;
      							}
   							}
   							set_ushort_secourue(&lop_gest.indexfinln,j);
							Premier_Mess_Ajout = 1;
						}
						TypMaj = LOP_DIFF_AJOUT;
         			break;

         		case 303:
						TypMaj = LOP_DIFF_SUPP;
         			break;

				case 300:
						if(R_Transac8583.champs72[0]=='A')
						{
							if(TypMaj==LOP_DIFF_SUPP)
							{
   								index_fin_ln = lop_gest.indexfinln;
   								for ( i=0, j=0 ; i < index_fin_ln; i++)
   								{
      								read_oppos_index(i, &Oppos ) ;
      								if(memcmp(Oppos.numero,"0000000000",10) != 0)
      								{
         								write_oppos_index(j,&Oppos );
         								j++;
      								}
	   							}
   								set_ushort_secourue(&lop_gest.indexfinln,j);
								Premier_Mess_Ajout = 1;
							}
							TypMaj = LOP_DIFF_AJOUT;
						}
            			else if(R_Transac8583.champs72[0]=='D')
               				TypMaj = LOP_DIFF_SUPP;
         			break;

				case 308:
            		Init_Lop();
					TypMaj = LOP_COMPLETE;
         			break;

         		default:
         			return KO;
      		}
		}

		Os__saved_set ((uchar *)&lop_gest.typmis, TypMaj, 1);
	}

	switch(TypMaj)
	{
		case LOP_COMPLETE: // Liste Noire Complete
			trait_status = Trait_Lop_Complete(Premier_Mess);
			if(trait_status == LOP_INCOHERENTE)
			{
				Init_Lop();
				return KO;
			}
			if(trait_status !=OK)
				return KO;
			break;

		case LOP_DIFF_SUPP: // Liste Noire Dif : Suppression
			trait_status = Trait_Lop_Supp(Premier_Mess);
			if(trait_status == LOP_INCOHERENTE)
			{
				Init_Lop();
				return KO;
			}
			if(trait_status !=OK)
				return KO;
			break;

		case LOP_DIFF_AJOUT: // Liste Noire Dif : Insertion
			trait_status = Trait_Lop_Ins(Premier_Mess_Ajout);
			if(trait_status == LOP_INCOHERENTE)
			{
				Init_Lop();
				return KO;
			}
			if(trait_status !=OK)
				return KO;
			break;

		case PAS_DE_MAJ:
      	// Ajouter le 09/03/2005
			Os__saved_copy((uchar *)&R_Transac8583.champs63[12],(uchar *)lop_gest.NumListe,sizeof(lop_gest.NumListe));
			Os__saved_set ((uchar *)lop_gest.NumListInt, '0', sizeof(lop_gest.NumListInt));
			Os__saved_copy((uchar *)"001",(uchar *)lop_gest.NumBloc, sizeof(lop_gest.NumBloc));
			Os__saved_set((uchar *)&lop_gest.stat, OK, 1);
			Os__saved_set ((uchar *)&lop_gest.reprise, KO, 1);
			Os__saved_set((uchar *)&lop_gest.typmis, PAS_DE_MAJ, 1);
			Os__saved_set((uchar *)&lop_gest.Bin_lu, 0, sizeof(lop_gest.Bin_lu));
			Os__saved_copy((uchar *)"000000",(uchar *)lop_gest.Bin_lu, sizeof(lop_gest.Bin_lu)-1);
			Os__saved_set((uchar *)&lop_gest.Data_lu, 0, sizeof(lop_gest.Data_lu));
			Os__saved_copy((uchar *)"0000000000000",(uchar *)lop_gest.Data_lu, sizeof(lop_gest.Data_lu)-1);
			set_ushort_secourue(&lop_gest.Nbr_Data,0);
			Os__saved_set((uchar *)&lop_gest.Status_Data_lu, 'P', sizeof(lop_gest.Status_Data_lu));
			return FIN_TRANSFER;

		default:
			break;
	}
	if(l_Champ24==300)
	{
		Os__saved_copy((uchar *)&R_Transac8583.champs63[12],(uchar *)lop_gest.NumListe,sizeof(lop_gest.NumListe));
		Os__saved_set ((uchar *)lop_gest.NumListInt, '0', sizeof(lop_gest.NumListInt));
		Os__saved_copy((uchar *)"001",(uchar *)lop_gest.NumBloc, sizeof(lop_gest.NumBloc));
		Os__saved_set((uchar *)&lop_gest.stat, OK, 1);
		Os__saved_set ((uchar *)&lop_gest.reprise, KO, 1);
		Os__saved_set((uchar *)&lop_gest.typmis, PAS_DE_MAJ, 1);
		Os__saved_set((uchar *)&lop_gest.Bin_lu, 0, sizeof(lop_gest.Bin_lu));
		Os__saved_copy((uchar *)"000000",(uchar *)lop_gest.Bin_lu, sizeof(lop_gest.Bin_lu)-1);
		Os__saved_set((uchar *)&lop_gest.Data_lu, 0, sizeof(lop_gest.Data_lu));
		Os__saved_copy((uchar *)"0000000000000",(uchar *)lop_gest.Data_lu, sizeof(lop_gest.Data_lu)-1);
		set_ushort_secourue(&lop_gest.Nbr_Data,0);
		Os__saved_set((uchar *)&lop_gest.Status_Data_lu, 'P', sizeof(lop_gest.Status_Data_lu));
		return FIN_TRANSFER;
	}
	else
	{
		Os__saved_copy((uchar *)&R_Transac8583.champs63[12],(uchar *)lop_gest.NumListInt,sizeof(lop_gest.NumListInt));
		Os__saved_set ((uchar *)&lop_gest.reprise, OK, 1);
	}
	return OK;
}
