//typedef	unsigned char	uchar;
typedef	unsigned char	uint8;
typedef	unsigned long	Ulong;
typedef	unsigned short	Ushort; //Mo 07/04/06 Activer par mouad
typedef	unsigned int	Uint;
typedef	unsigned short	uint16;

typedef	unsigned char	UC;
typedef	unsigned char*	PUC;
typedef	char			C;
typedef	char*			PC;
typedef	unsigned char	UINT;
typedef	unsigned long	UL;
typedef	unsigned long*	PUL;
typedef	unsigned short	US;
typedef	unsigned short*	PUS;
typedef	short			S;
typedef	short*			PS;
typedef	long			L;
typedef	long*			PL;

//ORIFLAME
#define MAXINVOICES					5

//LISTE NOIRE
#define NBMAXOPP2					2800
#define NBMAXOPP3					3000
#define NBMAXOPP4					2200
#define NBMAXOPP					NBMAXOPP2+NBMAXOPP3+NBMAXOPP4
#define NBMAXOPPBIN					100
#define NBMAXOPPTRN					100

#define LOP_AVEC_REPRISE			'1'
#define LOP_SANS_REPRISE			'0'

#define LOP_INCOHERENTE				2
#define INS_LN_COMP					0
#define SUPP_LN						1
#define INS_LN						2

#define LOP_COMPLETE				1
#define LOP_DIFF_AJOUT				2
#define LOP_DIFF_SUPP				3
#define PAS_DE_MAJ					4

#define	SIZE___MODE_GSM				2
#define	SIZE___RLP					1
#define	SIZE___VERSION				1

#define	SIZE___TYPE_CONNEXION		1
#define	SIZE___IP_GPRS				15
#define	SIZE___PORT_GPRS			6
#define	SIZE___APN_GPRS				40

#define	SIZE___LOGIN_GPRS			40
#define	SIZE___PWD_GPRS				40

typedef struct {   //tag
	  C TM[4];        //001
	  C MT[13];        //002
	  C NCAI[9];      //003 M.O 8 ===> 9 probl?me de NCAI erron?e
	  C NS[7];        //004
	  C NSA[7];       //005
	  C NHC[7];       //006
	  C NCAR[20];     //007
	  C STAN[7];      //008
	  C NA1[7];       //009
	  C DP[3500];/*bom*/ //010
	  C DE[4];        //012
	  C CRPIN[4];     //013
	  C DA[9];        //014
	  C HE[7];        //015
	  C NPRT[48];     //016
	  C DAEX[5];      //017
	  C DATR[9];      //018
	  C HETR[7];      //019
	  C TIDE[3];      //020
	  C TYPA[2];      //021
	  C RE[256];      //022
	  C RECB[25];     //023
	  C REQU[3];      //024
	  C RESE[26];     //025
	  C RECO[26];     //026
	  C RERA[3];      //027
	  C MDLC[3];      //Cless_MDLC
}MSG_CAISSE ;

typedef struct
{
	Ushort	indexfinln;			/* index fin de liste                   	   */
	Ushort	indexfinln_bin;		/* index fin de liste de bins en opposition  */
	Ushort	indexfinln_trn;		/* index fin de liste de tranches en oppo    */
	UC	NumListe[4];  		/* numero de la Liste d'Oppositions     	   */
	UC	NumListInt[4];      /* numero de liste en cours de reception     */
	UC	NumBloc[3] ;        /* numero de bloc attendu                    */
	UC	typmis ;            /* type mise ? jour :1,2,3						   */
	UC	reprise ;			/* avec reprise =1 ; sans reprise =0		   */
	UC	stat ;				/* status											   */
	Ushort	indexcouln;			/* index courant pour insersion, suppression */
	Ushort	indexinsln;			/* index pour insersion                      */
	Ushort	indexcouln_bin;
	Ushort	indexinsln_bin;
	Ushort	indexcouln_trn;
	Ushort	indexinsln_trn;

	UC	Bin_lu[7];
	UC	Data_lu[14];
	Ushort	Nbr_Data;
	UC	Status_Data_lu;
	UC	Action;
}lop_gest_enreg ;

/* structure de travail d'un numero en liste d'opposition */
typedef struct
{
	UC  numero[10] ;      /* numero porteur interdit compacte      */
	//UC  typopp ;        /* type d'opposition                    */
}typ_lop_enr_bcd;
/* structure de travail d'un BIN en liste d'opposition */
typedef struct
{
	UC  bin[6] ;			 /* BIN interdit */
}typ_lop_enr_bin;
/* structure de travail d'une tranche de cartes liste d'opposition */
typedef struct
{
	UC  trn_debut[10] ;
	UC  trn_fin[10] ;
}typ_lop_enr_trn;


typedef struct
{
	lop_gest_enreg		lop_gest;				/*	Entete */
	typ_lop_enr_bcd		lop_fichier[NBMAXOPP];	/*	Tableau des lop */
	typ_lop_enr_bin		lop_bin[NBMAXOPPBIN];	/*	Tableau des bin */
	typ_lop_enr_trn		lop_trn[NBMAXOPPTRN];	/*	Tableau des tranches */
}Fichier_Lop;

//PARAMETRES
typedef struct
{
	UC		Last_Card_Rejected[20];
	UC		Num_sequence[6+1] ;
	UC		Num_seqAnnul[6+1] ;
	UC		Message_Number[6+1] ;
	UC		Trace_Audit_Number[6+1];
	UC		Num_point_vente[15+1];
	UC		Num_commerce[15+1];
	UC		Num_Terminal[8+1];
	UC		Num_serie_terminal[10+1];
	UC		Type_Terminnal[2+1];
	UC		Type_Appli[2+1];
	UC		Nom_Appli[10+1];
	//UC		Indic_new_appli;
	UC		Merchant_Name[24+1];
	UC		Merchant_Adrees[24+1];

	UC		M_code_postal[10+1] ;
	UC		M_code_pays[3+1] ;

	UC		Num_reg_commerce[9+1];
	UC		MCC[4+1];
	UC		Merchant_City[13+1];
	UC		pass_word[4+1];
	UC		Indic_accept_carte[9+1];
	UC		Langue;
	UC		Accept_puce;
	UC		Accept_Man;
	UC		Indic_annul;
	//UC		Indic_achat;
	UC		Indic_impression;
	UC		Indic_forcage;
	UC		Ctrl_fraude;
	UC		Indic_pinpad;
	/************************/
	UC		Indic_TimeOut_Val;
	UC		Indic_TimeOut_Val_annul;
	UC		Indic_Change_Cpte_Dom;
	UC		Indic_PreAutor;
	UC		Indic_OffShort;
	UC		Indic_Refund;
	UC		Indic_CashBack;
	UC		Indic_TIP;
	UC		Indic_Instalment;
	UC		Indic_Offline;
	Ushort		TimeOut;
	/************************/
	UC		Heure_appel[4+1];
	UC		Date_appel[6+1];
	Ushort		compteur_appel;
	Ushort		Nbr_of_redial;
	Ushort		redial_delay;
	UC		capture_date[4+1];
	Ushort		Max_trs_off;
	Ushort		Max_avis;
	UC		logo[11+1];
	UC		Appli_type;
	UC		type_appel;
	UC		standard[4+1];
	UC		Tel_autor_pri[16+1];
	UC		Tel_telec_pri[16+1];
	UC		X25_autor_pri[15+1];
	UC		X25_telec_pri[15+1];
	UC		Tel_autor_sec[16+1];
	UC		Tel_telec_sec[16+1];
	UC		X25_autor_sec[15+1];
	UC		X25_telec_sec[15+1];
	UC		Tel_telech_pri[16+1];
	UC		Tel_telech_sec[16+1];
	UC		X25_telech_pri[15+1];
	UC		X25_telech_sec[15+1];
	UC		Master_Key[24];
	UC		MK_check_Value[8];
	UC		Index_MK[3];
	UC		Methode_Encry;
	UC		Mac_Key[8];
	UC		MacKey_check_Value[8];
	UC		Index_MacKey[3];
	UC		Version_appli[4+1];//modif	04/04
	UC		Date_version_appli[15] ;
	Ushort		Index_compte;
	UC		Update_param_Emv;
	UC		Update_Appli;
	UC		Update_bin;
	UC		Update_LN;
	UC		Update_mk;
	UC		Update_mac;
	UC		Update_compt_dom;
	UC		Update_pos_param;

	UC		Version_Pos_Param[3];
	UC		Version_Bin_Prefixe[3];
	UC		Version_Tab_Emv[3];
	UC		Version_Compte_Dom[3];
	UC		Version_Opp[4];//par Hind SAHEL
	UC		Bank_Pwd[5];
	UC		Merchant_Pwd[7];
	Ushort		delai_preauto;
	UC		sondage;
	UC		status;
	UC		X_IP[16];  // KI Le 120308
	UC		X_PORT[7];  // KI Le 120308
	UC		Telech_IP[16];  // AOURBIAA 050116
	UC		Telech_PORT[7];  // AOURBIAA 050116
	UC		WanamodeIP;
	UC		AutoCollecteOK;
	UC		AutoCollecteExecute;
	UC		AutoReversalOK;
	UC      CodeReponnseAvantAutoReversal[3];
	UC				var_pourboire;
	UC		RFU[17]; // pour garder les param?tres de param?trage inchang?s lors de changement de l'application , 20032017
	UC		MODELE_TERMINAL;
	Ushort		NbrLParam;
	int		TimeOutReceive;
	Ulong		Montant_Total;
	int		NbrEchChoisi;
	char		EMETTEUR[5];
	UC		Phone_number[10];	//Kamalssl
	UC		Etat_TLS;			//Kamalssl
	UC	 	Access_code[4];  // stouargui 19/04/2016
	char 		COMMISSION_INT[2][6];
	char 		COMMISSION_SVM[2][6];
	char 		COMMISSION_MN[2][6];
	UC		MenuTPE[30]; //Changement de position dans la structure 20032017, RDS Ticket#2017031510000014

//Mo 10/04/06
	C				g_type_connexion;
	UC				ip_gprs[SIZE___IP_GPRS+1];
	UC				port_gprs[SIZE___PORT_GPRS+1];
	UC				apn_gprs[SIZE___APN_GPRS+1];
	UC				MODEGSM[SIZE___MODE_GSM+1];
	UC				RLPGSM[SIZE___RLP+1];
	UC				login_gprs[SIZE___LOGIN_GPRS+1];
	UC				pwd_gprs[SIZE___PWD_GPRS+1];
	char			vCam0;
	UC				nom_domaine[30+1];
	UC				Top_TelAuto;
	int				etat_PIN;

	UC				TYPEAPPLICATION[15];
	UC				Flag_service;//services flag (=0 NAPS NAPS SNTL)(=1NAPS AVIS)(=2 NAPS LYDEC) badr
//Mo 10/04/06
}parametre ;
//DEVISES
#define NB_MAX_DEV_ACC		6
#define NB_MAX_DEV_REG  	6
typedef struct
{
	UC		code_num[3+1];
	UC		code_alph[3+1];
	UC		Exposant;
	Ulong		Montant_min;
}devise_accept;

typedef struct
{
	UC		code_num[3+1];
	UC		code_alph[3+1];
	UC		Exposant;
}devise_reg;

typedef struct
{
	Ushort			Indice_fin;
	devise_accept	TabDevise[NB_MAX_DEV_ACC];
}Struct_Devise_Acc ;

typedef struct
{
	Ushort		Indice_fin;
	devise_reg	TabDevise[NB_MAX_DEV_REG];
}Struct_devise_reg ;
//BIN
#define MAXPTISO			200
#define NB_MAX_BIN			500
#define NB_MAX_PRE		  	40
#define NB_MAX_INS_REG	  	10

typedef struct
{
	UC		Bin_Debut[11];
	UC		Bin_Fin[11];
	Ushort		Code_Prefixe;
	UC		LgCard_Type;
	Ushort		LgCard_Min;
	Ushort		LgCard_Max;
	UC		OffSet_Octet36;
	UC		Code_Service[3];
	UC		Langue;
	UC		Gest_Pin;
	Ulong		Plafond ;
}accepted_bin;

typedef struct
{
	Ushort			Indice_fin;
	accepted_bin	Tab_Bin[NB_MAX_BIN]	;
}Struct_File_Bin ;

typedef struct
{
	UC		Pre_Debut[11];
	UC		Pre_Fin[11];
	UC		OffSet_Octet36;
	UC		LgCard_Type;
	Ushort		LgCard_Min;
	Ushort		LgCard_Max;
	UC		Gest_Pin;
	Ushort		Indice_Ins_Reg;
}Accepted_prefixe;
typedef struct
{
	Ushort		  		Indice_fin ;
	Accepted_prefixe	Tab_Prefixe[NB_MAX_PRE];
}Struct_File_Prefixe;

typedef struct
{
	Ushort		Code_Prefixe ;
	UC	  	Libelle[16+1];
	UC	  	Num_point_vente[15+1];
	UC		Message1[64+1] ;
	UC		Message2[64+1] ;
	Ulong		Plafond ;
}Struct_Ins_reg;
typedef struct
{
	Ushort		  		Indice_fin ;
	Struct_Ins_reg		Tab_Ins_reg[NB_MAX_INS_REG];
}Struct_File_Ins_reg;
//COMPTES DE DOMICILIATION
#define	NB_MAX_COMPTE	4
typedef struct
{
	UC		Indice_compte[2+1] ;
	UC		Compte[12+1] ;
}Compte_dom;
typedef struct
{
	Ushort		Indice_fin;
	Compte_dom	tab_compte[NB_MAX_COMPTE];
}Struct_compte_dom;
//EMV AID
#define	NB_MAX_AID	30
typedef struct
{
	UC		RID[5];
	UC		Lg_pix;
	UC		PIX[11];
	UC		version_app_tpe[4];
	UC		priorite;
	UC		Indic_for;
}Struct_EMV_AID;

typedef struct
{
	Ushort			Indice_fin;
	UC			Num_ver_file[4];
	Struct_EMV_AID	tab_aid[NB_MAX_AID];
}Fichier_EMV_AID;
//EMV KEYS
#define NB_MAX_EMV_KEY 60
typedef struct
{
	UC		RID[5];
	UC		Key_Index;
	UC		Lg_modulo;
	UC		Key_mod[248];
	UC		Lg_Exp;
	UC		Key_Exp[3];
}Struct_EMV_Keys;

typedef struct
{
	Ushort		Indice_fin;
	UC		Num_ver_file[4];
	Struct_EMV_Keys	tab_emv_key[NB_MAX_EMV_KEY];
}Fichier_EMV_Keys;
//EMV TAC
#define	NB_MAX_TAC	30
typedef struct
{
	UC		RID[5];
	UC		Lg_PIX;
	UC		PIX[11];
	UC		TAC_default[5];
	UC		TAC_online[5];
	UC		TAC_denial[5];
}Struct_EMV_TAC;

typedef struct
{
	Ushort		Indice_fin;
	UC		Num_ver_file[4];
	Struct_EMV_TAC	tab_emv_tac[NB_MAX_TAC];
}Fichier_EMV_TAC;
//EMV TDOL
#define	NB_MAX_TDOL	20
typedef struct
{
	UC		RID[5];
	UC		Lg_TDOL;
	//UC	TDOL[252];
	UC		TDOL[126];
}Struct_EMV_TDOL;

typedef struct
{
   Ushort		Indice_fin;
   UC		Num_ver_file[4];
   Struct_EMV_TDOL	tab_emv_tdol[NB_MAX_TDOL];
}Fichier_EMV_TDOL;
//EMV DDOL
#define	NB_MAX_DDOL	30
typedef struct
{
	UC		RID[5];
	UC		Lg_PIX;
	UC		PIX[11];
	UC		Lg_DDOL;
	//UC	DDOL[252];
	UC		DDOL[126];
}Struct_EMV_DDOL;

typedef struct
{
	Ushort		Indice_fin;
	UC		Num_ver_file[4];
	Struct_EMV_DDOL	tab_emv_ddol[NB_MAX_DDOL];
}Fichier_EMV_DDOL;
//EMV RAND CALL
#define	NB_MAX_RAND_CALL	60
typedef struct
{
	UC		RID[5];
	UC		Lg_PIX;
	UC		PIX[11];
	UC		devise[3];
	UC		montant_seuil[12];
	UC		Nbr_trs[2];
	UC		Nbr_max_trs[2];
}Struct_EMV_rand;

typedef struct
{
	Ushort		Indice_fin;
	UC		Num_ver_file[4];
	Struct_EMV_rand	tab_emv_rand[NB_MAX_RAND_CALL];
}Fichier_EMV_rand;
//EMV TCC
#define	NB_MAX_TCC	20
typedef struct
{
	UC		RID[5];
	UC		Lg_PIX;
	UC		PIX[11];
	UC		tcc_autor;
}Struct_EMV_TCC;

typedef struct
{
	Ushort		Indice_fin;
	UC		Num_ver_file[4];
	Struct_EMV_TCC	tab_emv_tcc[NB_MAX_TCC];
}Fichier_EMV_TCC;

typedef struct
{
	Ushort	l_champ55;
	UC	Champ55[512];
	Ushort	l_Tag91;
	UC	Tag91[33];
	Ushort	l_Tag71;
	UC	Tag71[255];
	Ushort	l_Tag72;
	UC	Tag72[255];
	UC	Aid[33];
	Ulong	Plafond;
	UC	SDA_DDA_Performed;
	Ushort   Idx_Emv_Key;
	Ushort   Idx_Emv_Tac;
	Ushort   Idx_Emv_TDOL;
	Ushort   Idx_Emv_DDOL;
	Ushort   Idx_Emv_RAND;
}Struct_Data_Last_TRS_Emv_Rej;


//ISO8583 EMISSION
#define	MAX_MESS			1200
#define NUMERIC_		0x01
#define TRACKX			0x02
#define ALPHANUM		0x03
#define ALPHANUMS		0x04
#define BITS			0x05

#define LLVAR	     	0x01
#define LLLVAR		  	0x02
#define YYMMDDhhmmss	0x03
#define YYMMDDhhmm		0x04
#define YYMMDD			0x05
#define YYMM		  	0x06
#define MMDD		  	0x07


#define TRANSMISSION_REJ	2
#define TRANSMISSION_END	3


#define FILE_TRS 		0x00
#define FILE_KEY   		0x01
#define FILE_MACKEY  	0x02
#define FILE_BIN		0x03
#define FILE_COMPTE		0x04
#define FILE_DOM		0x05
#define FILE_LOP		0x06
#define FILE_PARAM		0x07
#define FILE_EMV_AID	0x08
#define FILE_EMV_KEYS	0x09
#define FILE_EMV_TAC	0x0A
#define FILE_EMV_TDOL	0x0B
#define FILE_EMV_DDOL	0x0C
#define FILE_EMV_RAND	0x0D
#define FILE_EMV_TCC	0x0E
#define FILE_PREFIXES	0x0F
#define FILE_OPERATEUR	0x10//remarjane arevoir
#define FILE_INTERCHANGE	0x11 //Version 1.11.13 AILAL


#define HEADER_OK			0x00
#define HEADER_KO			0x01
#define RECEPT_KO			0x02
#define FORMAT_KO			0x03
#define EMISSION_KO			0x04
#define CHAMP_ERROR			0x05
#define FORMAT_MSG_REC_KO	0x06
#define CONNEXION_KO		0x07

#define MAX_DIGITS_SUPPORTED 13

#define NON_AUTOR			41
#define CAPTURE_CARTE		42
#define REFERRAL			43
#define PIN_FAUX			44

#define FIN_TRANSFER		0x90
#define REJET_RECEPT		0x91

typedef struct
{
	UC 	e_type_mess[4];
	UC 	r_type_mess[4];
	UC 	bitmap[8];
	UC 	bitmap2[8];
	UC 	champs2[19];
	Ushort 	l_champs2;
	UC 	champs3[6];
	UC 	champs4[12];
	UC 	champs5[12];
	UC 	champs7[10];
	UC 	champs11[6];
	UC 	champs12[12];
	UC 	champs14[4];
	UC 	champs17[4];
	UC 	champs22[12];
	UC 	champs23[3];
	UC 	champs24[3];
	UC 	champs25[4];
	UC 	champs26[4];
	UC 	champs30[12];
	UC 	champs32[11];
	Ushort 	l_champs32;
	UC 	champs35[37];
	Ushort 	l_champs35;
	UC 	champs37[12];
	UC 	champs38[6];
	UC 	champs39[3];
	UC 	champs41[8];
	UC 	champs42[15];
	UC 	champs43[99];
	Ushort 	l_champs43;
	//UC 	champs48[500];
	UC 	champs48[500];
	Ushort l_champs48;
	UC 	champs49[3];
	UC 	champs50[3];
	UC 	champs52[8];
	UC 	champs53[99];
	UC 	l_champs53;
	UC 	champs54[120];
	Ushort l_champs54;
	Ushort l_champs55;
	UC 	champs55[255];
	UC 	champs56[35];
	UC 	l_champs56;
	//UC 	champs62[10];
	UC 	champs62[30];
	Ushort l_champs62;
	UC 	champs63[30];
	Ushort l_champs63;
	UC 	champs64[8];
	UC 	champs71[8];
	UC 	champs72[720];
	Ushort l_champs72;
	UC 	champs73[6];
	UC 	champs74[10];
	UC 	champs75[10];
	UC 	champs76[10];
	UC 	champs77[10];
	UC 	champs83[10];
	UC 	champs84[10];
	UC 	champs86[16];
	UC 	champs87[16];
	UC 	champs88[16];
	UC 	champs89[16];
	UC 	champs97[17];
	UC 	champs101[17];
	Ushort 	l_champs101;
	UC 	champs110[600];
	Ushort l_champs110;
	UC 	champs128[8];
}emis;
//ISO8583 RECEPTION
#define	INITA			0
#define	AVIS			1
#define	RECONC			2
#define	TELEC			3
#define	CONFIRM			4
#define	COMPTE			5

typedef struct
{
	UC 	e_type_mess[4];
	UC 	r_type_mess[4];
	UC 	bitmap[8];
	UC 	bitmap2[8];
	UC 	champs2[19];
	Ushort 	l_champs2;
	UC 	champs3[6];
	UC 	champs4[12];
	UC 	champs5[12];
	UC 	champs6[12]; /* OURBIAA POUR AMSSF 281015 */
	UC 	champs7[10];
	UC 	champs11[6];
	UC 	champs12[12];
	UC 	champs15[6];
	UC 	champs17[4];
	UC	champs22[12];
	UC 	champs24[3];
	UC 	champs30[12];
	UC 	champs32[11];
	Ushort 	l_champs32;
	UC 	champs35[37];
	Ushort 	l_champs35;
	UC 	champs37[12];
	UC 	champs38[6];
	UC 	champs39[3];
	UC 	champs41[8];
	UC 	champs42[15];
	//UC 	champs44[999];
	UC 	champs44[400];
	Ushort l_champs44;
	//UC 	champs48[999];
	UC 	champs48[500];
	Ushort 	l_champs48;
	UC 	champs49[3];
	UC 	champs50[3];
	UC 	champs53[99];
	Ushort 	l_champs53;
	UC 	champs54[120];
	Ushort l_champs54;
	Ushort l_champs55;
	UC 	champs55[255];
	UC 	champs56[35];
	Ushort 	l_champs56;
	//UC 	champs63[999];
	UC 	champs62[999]; // STOUARGUI ADD 170616 GSM
	Ushort l_champs62;
	UC 	champs63[30];
	Ushort l_champs63;
	UC 	champs64[8];
	UC 	champs71[8];
	UC 	champs72[999];
	Ushort l_champs72;
	UC 	champs73[6];
	UC 	champs101[17];
	Ushort 	l_champs101;
	UC 	champs110[600];
	Ushort l_champs110;
	UC 	champs128[8];
}rece;
//INFOPAIEMENT
#define PRE_VAL_1_TRS			0
#define PRE_VAL_2_TRS			1

#define ANNUL_PREAUTOR_TOTAL	0
#define ANNUL_PREAUTOR_PARTIEL	1

//XPOS
#define XPOS_N_TRANSACTION	0xAE
#define XPOS_N_CAISSE		0xAF
#define XPOS_N_FICHIER		0xBA
#define XPOS_TRA_DUPL		0xBB
#define XPOS_N_CARTE		0xBC
#define XPOS_D_H_TRA		0xBD
#define XPOS_MNT_TRA		0xBE
#define XPOS_NUM_AUT		0xBF

#define XPOS_ETAT_CMR		0xAA
#define XPOS_ETAT_PARAM		0xAB
#define XPOS_ETAT_LN		0xAC
#define XPOS_CARD_IN_LN		0xAD
#define XPOS_NBR_TR_OFFLINE	0xAE
#define XPOS_DBLE_TRA		0xAF
#define XPOS_ETAT_STAN		0xBA
#define XPOS_CUMUL_AMNT		0xBB
#define XPOS_NUM_LN			0xBC
#define XPOS_KEY_MACKEY		0xBD
#define XPOS_FURUR1			0xBE
#define XPOS_SEQ_TRANSA		0xBF
#define XPOS_SEQ_PRE_AUT	0xCA
#define XPOS_LIBELLE_PORT	0xCB
#define XPOS_TRANREF		0xCC
#define XPOS_CMRALFAIZ		0xCD
#define XPOS_TPEALFAIZ		0xCE




#define	C_NATIONALE			0x04
#define	C_AURORA			0x05
#define	C_ALFAIZ			0x06
#define	C_ATTIJARIWAFA		0x07

//XPOS

#define	CARD_OK					0
#define	CARD_KO					1
#define FIRST_AMOUNT			'1'
#define SECOND_AMOUNT			'2'
#define	PIN_REQUIRED			2
#define	PROTOCOLE_VISA			'1'
#define	PROTOCOLE_ISO8583		'2'


/******
#define CASHWITHDRAWALSCODE		0x00
#define CASHADVANCESCODE		0x01
#define CASHDEPOSITCODE			0x02

#define	SALECODE				0x03
#define SALECASHBACKCODE		0x04
#define REFUNDCODE				0x05
#define PREAUTHCODE				0x06
#define PREAUTHCOMPCODE			0x07
#define MANUALTRACODE			0x08

#define BALANCECODE				0x09
#define CARDVERIFICATIONCODE	0x0a
#define CHECKVERIFICATIONCODE	0x0b

#define ADJUSTDCODE				0x0c
#define ADJUSTCCODE				0x0d

#define	VOIDCODE				0x0e//0x08
#define	VOID_PREAUTO_TOTAL_CODE		  0x0f//0x00
#define	VOID_PREAUTO_PARTIEL_CODE	  0x1f//0x01
******/
/**/
#define	VOID_PREAUTO_TOTAL_CODE	    0x10//0x00 avoir 210905
#define	VOID_PREAUTO_PARTIEL_CODE   0x01
#define CASHADVANCESCODE			0x02
#define	SALECODE					0x03
#define SALECASHBACKCODE			0x04
#define REFUNDCODE					0x05
#define PREAUTHCODE					0x06
#define PREAUTHCOMPCODE				0x07
#define	VOIDCODE					0x08
#define MANUALTRACODE				0x09

//#define	SETTLECODE				0x10
//#define	TOTALSCODE				0x11
//#define	BUSINESSCYCLECODE		0x12
//#define	INITIALIZATIONCODE		0x13
//#define	BLACKLISTCODE			0x14
/**/

#define BALANCEENQUIRYCODE		0x15
#define	DUPLICATECODE			0x6f
#define VIGNETTE				0x80// stouargui
#define MICRO_CREDIT			0x81//Hind SAHEL
#define CREDIT_PARTIEL			0x82//Hind SAHEL
#define CREDIT_TOTAL			0x83//Hind SAHEL
#define RECH_PREPAYE			0x90//Hind SAHEL
#define DECH_PREPAYE			0x91//Hind SAHEL
#define CAC_PREPAYE				0x92//Hind SAHEL
#define CASHIN_PREPAYE			0x93//Hind SAHEL
#define CASHOUT_PREPAYE			0x94//Hind SAHEL
#define DE_RECHARGE				0x95//stouargui
#define E_FACTURE				0x96//Hind SAHEL
#define PINCHANGE				0x97//Hind SAHEL
#define DEM_SOLDE				0x98//Hind SAHEL
#define MINI_RELEVE				0x99//Hind SAHEL
#define E_RECHARGE				0x9A//stouargui
#define DE_REMBOURSSEMENT		0x9B//stouargui
#define REMISE					0x9C//stouargui
#define PAIEMENT_NFOIS			0x9D//stouargui
#define PAIEMENT_DIFFERE		0x9E//stouargui
#define PAIEMENT_POURBOIRE		0x9F//stouargui
#define PAIEMENT_INTERCHANGE	0x20 //Version 1.11.13 AILAL
#define NOTATION				0x84//stouargui
//#define SAISIE_OBLIG			0x7f
//#define PAS_SAISIE			0x8f
//#define SAISIE_FACUL			0x9f
/**************************							 ***************/
#define OR_FACTURE50				0x50
#define OR_FACTURE51				0x51
#define OR_FACTURE52				0x52
#define OR_FACTURE53				0x53


/**************************		Constantes Montants		***********************************/
#define	AMOUNT_OK				0x0A
#define	AMOUNT_KO				0x0B
#define	AMOUNT_MIN				0x0C
/*************************		Constantes	Transaction	************************************/
#define	TRANSACTION_OK	  		0
#define	TRANSACTION_KO			1
#define	COMPLETION_OK			0
#define	COMPLETION_KO			1
#define	AVIS_OK					2
/*************************************************************/
#define SAIS_MAN				0x40
#define NO_ME					0x41
#define CARTE_COM    			0x42
#define CARTE_CLIENT			0x43
#define ILLISIBLE				0x44
#define SAIS_EMV				0x45
#define SAIS_MAG				0x46
#define PUCE_ILLISIBLE			0x47
/*************************		Constantes	Entry_Mode	*********************/
/*
#define PIN						0x50
#define TRS_EMV					0x51
#define PIN_EMV					0x52
#define KEY_ENTRY				0x53
#define KEY_PIN					0x54
*/
#define TRS_MAG					0x00
#define TRS_MAN					0x01
#define TRS_EMV					0x02
#define TRS_SCT					0x03

/*************************		Constantes	Entry_Pin	*********************/
#define NO_PIN 					0x00
#define PIN_ONLINE				0x01
#define PIN_OFFLINE				0x02

/*************************		Constantes	Top For?age	*********************/
#define TOP_FOR_NORMAL			0x00
#define TOP_FOR_ONLINE			0x01
#define TOP_FOR_REFFERAL		0x02
#define TOP_FOR_FORCED			0x03

#define CLIENT					0x55
#define COMMERCANT				0x56
/*************************		Constantes	Type cartes	*********************/
#define CMI						0x60
#define AMEX					0x61
#define DINERZ					0x62
#define CETELEM					0x63
#define CARTE_LOCAL				0x64
/*************************		Constantes	Telecollecte	*********************/
#define NBR_DEB					1
#define NBR_REDR				2
#define MONT_DEB				3
#define MONT_REDR				4
#define NBR_CRE					5
#define MONT_CRE				6

#define NBR_RECH				7
#define MONT_RECH				8
#define NBR_DECH				9
#define MONT_DECH				10
#define NBR_CAC					11
#define MONT_CAC				12
#define NBR_CASHIN				13
#define MONT_CASHIN				14
#define NBR_CASHOUT				15
#define MONT_CASHOUT			16

#define	TEL_AUTO				0
#define	TEL_MAN					1

#define	RECO_OK					0
#define	RECO_KO					1
#define	TEL_OK					2
/*************************		Constantes	Langues			*********************/
/*
#define FR								0
#define ANG								1
#define ESP								2
*/
#define FRAN							0
#define ENG								1
#define ARABE							2

/*************************	 Constantes	Gestion de fichier *****************/
#define AJOUT_ENREG				0
#define MODIF_ENREG				1
#define SUPP_ENREG				2
/*************************	 Constantes	Signature Porteur *****************/
#define REQUIRED				0
#define NOT_REQUIRED			1
/*************************	 Constantes	PassWord *****************/
#define BANK_PWD				0
#define MERCHANT_PWD			1
/**********************	 Constantes	Origine Carte (Nationale ou Etrangere) ***************/
#define CARTE_ETRANGERE			0
#define CARTE_NATIONALE			1
#define CARTE_DOMESTIQUE		2//ajout? par Hind SAHEL pour les cartes NAPS
#define CARTE_ETRANGATRS		3//ajout? par Hind SAHEL pour les cartes NAPS
/*************************	 Constantes	Manipulation *****************/
#define TIMER_TAG				0x00000001
#define	KEY_CLEAR				0x00
#define	ABORTED					0x01
#define	KEY_ENTER				0x02
#define	FORBIDDEN_CARD			0x03
#define	REFUSED					0x04
#define	TRANS_FILE_FULL			0x05
#define	KEY_F1					0x06
#define	KEY_F2					0x07
#define	NOT_TREATED				0x08
#define	EXPIRED_CARD			0x09

#define	EVENTABORTED			0x00
#define	EVENTVAL				0x01
#define	EVENTESC				0x02
#define	EVENTCARD				0x03
#define	EVENTILLISIBLE			0x04
#define	EVENTTIMER				0x05


typedef struct
{
	UC					Event;
	UC					EventCard[200];
	UC					EnterLgPin;
	UC					EnterMin;
	UC					EnterMax;
	UC					Enter[30];
	short				default_dev;
	UC					fix_devise;
	UC					indexfin;
	UC					indexClessMag;		// KamalCless indexcdd;
	UC					waitpuce;
	UC					puce_out;
	/**/
	UC					Entry_Mode;
	UC					Entry_Pin;
	UC					CodeFunction;
	UC					Double_tra;
	UC					Fallback;			//modif 04/04
	UC					forcage;
	UC					forcage_possible;
	UC					StatusAnnulAdjust;
	UC					Transaction_DateTime[12+1];               //  FORMAT YYMMDDHHmmSS
	UC					Normal_Iso2[40];
	UC					Holder_Card[20];
	UC					Compressed_Iso2[20];
	UC					Code_pt_service[12+1];
	Ulong          		Transaction_Amount;
	Ulong				Secondary_Amount;	//modif 04/04
	Ulong				Consolidation_Amount;
    Ulong				Preauto_Amount_Initial;
    Ulong				Preauto_Amount_Final;
	UC					Preauto_indic_completion;
	UC					Response_Code[2+1];
	UC					Trace_Audit_Number[6+1];
	UC					Num_sequence[6+1];
	UC					Num_seq_save_compl_online[6+1];
	UC					Num_seqAnnul[6+1];
	UC					Approval_Code[6+1];
	UC					STAN[6+1];			//modif 04/04
	UC					Message_TypeisoEmi[4+1];
	UC					Message_TypeisoRec[4+1];
	UC					Proc_Code[6+1];
	UC					Bit_Map[16];
	UC					Expired_Date[4+1];
	UC					CVV2[3+1];
	UC					SeviceCode[3+1];
	UC					Flag_Dem_Aut;
	UC					Top_Forcage;
	UC					Carte_EnLN;
	UC					Appli_aid[34];
	UC					Appli_label[17];
	UC					Nom_Prenom[27];
	UC					Type_carte;
	UC					CodeReg[2+1];//XPOS
	UC					Origine_carte;		//modif 04/04
	UC	  				Num_point_vente[15+1];
	UC					Message1[64+1];
	UC					Message2[64+1];
	Ushort				Code_Prefixe;
	UC					Mess_commer[80];
	UC					Mess_porteur[80];
	Ulong				Plafond;
	UC					Langue;
	devise_accept		Devise_acc;
	UC					NumCurrency_cons[3+1];
	UC					pin_code_cryp[8+1];
	UC					cle_travail_cryp[48+1];
	UC					Lg_pin;
	UC					avis;
	UC					SignatureHolder;
	UC					Author_Status;
	UC					Pin_Required;
	UC					Certif[16];
	UC					Crypt_Info;
	UC					AuthRespCode[2];
	UC					EmvForOnline;
	UC					PanSeqNumber[4];
	UC					Status_Emv;			//modif 04/04
	UC					l_champ55;
	UC					Champ55[255];
//For Emv
	UC					TacFF34;
//	UC					AppCryp9F26[8];
	UC					PanSeqNum5F34[1];
	UC					AuthRespCode8A[2];
	UC					HolLanSelected;

	//xpos parti
//	uchar				xpos_pin_code_cryp[16];
//	uchar				xpos_cle_travail_cryp[16];
	uchar				xpos_type_carte;
	uchar				xpos_fin_mois;

	uchar				xpos_etat_ln;
	uchar				xpos_etat_carte_ln;
	uchar				xpos_cumul_mnt[12];
	uchar				xpos_dble_tra;
	uchar				xpos_etat_cmr;
	uchar				xpos_etat_param;
	uchar				xpos_nbr_tr_offlin;
	uchar				xpos_n_transaction[6+1];
	uchar				xpos_n_caisse[2+1];
	uchar				xpos_n_fichier[2+1];
	uchar				xpos_tra_duplique;
	uchar				xpos_num_ln[4+1];
	uchar				xpos_seq_tra[6+1];
	uchar				xpos_seq_pre[6+1];
	uchar				xpos_libelle_port[21+1];
	uchar				xpos_etape;
	uchar				xpos_etat_tran_ref;
	uchar				xpos_cmr_alfaiz[7+1];
	uchar				xpos_tpe_alfaiz[2+1];
	uchar				SIOpr;//remarjane
	uchar				SIProOpr;//remarjane
	uchar				SIValProOpr;//remarjane
	uchar				FctEnCours;//remarjane
	UC					Pin_Block_crypte[8];
	UC					Pin_Change_clair[12+1];
	UC					Lg_Newpin;
	UC					Pin_Change_crypte[8];
	UC					CardNumber[20];
	UC					Etat_PIN;
	char				NumPermis[30];
	char				NumChassis[30];
	char 				Kilometrage[11];
	char 				Litrage[11];
	char				ProduitChoisi[20];
	char				TypeAuthentification[3];
	char				IDConducteur[20];
	char				Immatriculation[15];

	UC				    CardDestin[20]; //stouargui 180516
	char				Operateur[1+1]; //stouargui add pour E_RECHARGE 060516
	char 				NumeroGsm[10+1];//stouargui add pour E_RECHARGE 060516
	char 				Promotion[6+1];//stouargui add pour E_RECHARGE  130616
	char 				Approval_Operateur[6+1];//stouargui add pour E_RECHARGE  200616
	Ulong				MontantRemise;//stouargui add pour AMANSUR  240616
	char 				Remise[4+1];//stouargui add pour AMANSUR  240616
	char 				MTRemise[16];//stouargui add pour AMANSUR  240616
    short				F_remise; //stouargui add pour AMANSUR  270616
	char				Police[12+1];//stouargui add pour CAPFINANCE  270616
	uchar 				flagSinistre;
	char			    Nbr_Fois[1+1];//stouargui add pour  PAIEMENT N FOIS  040716
	char          		montant_total[12+1]; //stouargui add pour  PAIEMENT N FOIS  040716
	char				ModeReglement[1+1];//stouargui add pour  PAIEMENT N FOIS  050716
	Ulong 				MontantDiffere;//stouargui add pour  PAIEMENT N FOIS  050716
	Ulong 				MontantPourboireInterchange;//stouargui add pour  PAIEMENT POURBOIRE  110716
	short				AfficheIndication_POURBOIRE_INTERCHANGE; //
	char				NotationService[20] ; //stouargui add pour  NOTATION  140716

	char				OR_GetDebtResults[256];
	char				OR_ConsultantId[7+1];
//	char				OR_firstName[20+1];
//	char				OR_lastName[20+1];
//	char				OR_invoiceNumber[10+1];
	char				OR_dueDate[MAXINVOICES][10+1];
	char				OR_debt[MAXINVOICES][12+1];

	char				OR_transactionID[6+1];
	char				OR_result[MAXINVOICES][3+1];
	char				OR_financeID[MAXINVOICES][10+1];
	char				OR_branID[MAXINVOICES][3+1];
	char				OR_remainingDebt[MAXINVOICES][12+1];
	char				OR_consultantNumber[MAXINVOICES][7+1];
	char				OR_firstName[MAXINVOICES][20+1];
	char				OR_lastName[MAXINVOICES][20+1];
	char				OR_greeting[MAXINVOICES][20+1];
	char				OR_invoiceNumber[MAXINVOICES][10+1];
	char				OR_paidamount[MAXINVOICES][12+1];

	char				OR_invoiceNumber62[10+1];


	char				AV_NumContrat[9+1];//avis badr
	char				AV_CodeMotif[23+1];//avis badr

	char				LY_RefContra[10+1];//Lydec badr
	char				LY_NumGSM[10+1];//Lydec badr
	char				LY_AdrEmail[30+1];//Lydec badr

	short				gindice;//compliment cless
	UC					g_numero_carte[20];//invalide carte

}infopayment;
// stoaurgui add notation informations
typedef struct
{
		char                DataPrepayee[50] ;
		char                DataPIN[12+1] ;
}infonotation;
//LIGNETRA
#define MAX_AVIS		20

#define NBMAXTRA		300
//#define NBMAXTRA		3

typedef struct
{

	Ulong		Transaction_Amount;
	Ulong		MontantPourboireInterchange;	//POURBOIRE
	Ulong		Preauto_Amount_Final;
	Ulong		Consolidation_Amount;
	Ulong		Montant_Accomp;
	uchar		CodeFunction;
	uchar		Num_point_vente[15+1];
	uchar		STAN[6+1];
	uchar		Message_Type[4+1];
	uchar		Num_sequence[6+1];
	uchar		RRN[6];
	uchar		Preauto_indic_completion;
	uchar	 	NumCurrency[3+1];
	uchar		NumCurrency_cons[3+1];
	uchar		Exposant;
	uchar		Transaction_DateTime[12+1];
	uchar		Compressed_Iso2[20];
	uchar		Expired_Date[4+1];
	uchar		Duplicata;
	uchar		Type_carte;
	uchar		CodeReg[2+1];
	uchar		Origine_carte;
	uchar		Langue;
	uchar	    StatusAnnulAdjust;
	uchar		Entry_Mode;
	uchar		Entry_Pin;
	uchar		avis;
	uchar		SignatureHolder;
	uchar		Flag_Dem_Aut;
	uchar		Top_Forcage;
	uchar		Approval_Code[6+1];
	uchar		Proc_Code[6+1];
	uchar		Lg_pin;
	uchar		PanSeqNumber[3];
	uchar		Lg_EmvData;
	uchar		EmvData[250];
	uchar		Indic_Accomp;
	uchar		Appli_aid[34];
	uchar		Appli_label[17];
	uchar		Nom_Prenom[27];
	uchar		SeviceCode[3+1];//par Hind SAHEL
	char		NumPermis[30];//par Hind SAHEL
	char		NumChassis[30];//par Hind SAHEL
	char 		Litrage[11];//par Hind SAHEL
	char 		Kilometrage[11];//par Hind SAHEL
	char		ProduitChoisi[20];	//par Hind SAHEL
	char		TypeAuthentification[3];//par Hind SAHEL
	char		Immatriculation[15];//par Hind SAHEL
	char		IDConducteur[20];//par Hind SAHEL
}lignetra;
typedef struct				  				/* structure comprenant les transactions plus */
{                                       	/* les param?tres de gestion du fichier */
	UC			date_fic[6+1];             	/* date du fichier transaction (6 carac)*/
	Ulong  		num_batch;	           		/* numero de fichier transaction        */
	Ushort 		nombre_tra;            		/* nombre de transaction enregistr?es   */
	Ushort		nbr_tra_off_succ;			/* nombre de transaction offline successives */
	Ushort		nbr_card_capture;           /* nombre de carte captu?e              */
	UC  		total_deb[12+1];       		/* total debit                          */
	UC  		total_cre[12+1];       		/* total credit                         */
	UC  		noyau_cre[12+1];       		/* total credit  pour noyau             */
	UC  		noyau_deb[12+1];       		/* total debit   pour noyau             */
	Ushort 		noyau_nombre;         		/* nombre de transaction pour le noyau  */
	UC			etat;               		/* etat du fichier de transactions      */
	lignetra	enr_tra0[NBMAXTRA];  		/* table de 100 lignes transactions     */
}Fichier_Transaction;
//PREAUTORISATION
#define	NBMAXPREAUT			170
//#define	NBMAXPREAUT			2

typedef struct
{
	UC		CodeFunction;

	UC		Num_point_vente[15+1];

	UC		STAN[6+1];
	UC		Message_Type[4+1];
	UC		Num_sequence[6+1];
	UC		RRN[6];

	Ulong		Transaction_Amount;
	UC	 	NumCurrency[3+1];

	Ulong		Consolidation_Amount ;
	UC		NumCurrency_cons[3+1];
	UC		Exposant;

	UC		Transaction_DateTime[12+1];
	UC		Compressed_Iso2[20];
	UC		Expired_Date[4+1];
	UC		SeviceCode[3+1];//badr prob PAN 19
	UC		Duplicata;
	UC		Type_carte;
	UC		Origine_carte;
	UC		Langue;
	UC	    StatusAnnulAdjust;
	UC		Entry_Mode;
	UC		Entry_Pin;
	UC		avis ;
	UC		SignatureHolder;

	UC		Flag_Dem_Aut;
	UC		Top_Forcage;
	UC		Approval_Code[6+1];
	UC		Proc_Code[6+1];
	UC		Lg_pin;

	UC		PanSeqNumber[3];
	UC		Lg_EmvData;
	UC		EmvData[255];
	UC		Indic_Accomp;
	Ulong		Montant_Accomp;
	Ushort		vie_preauth;
}lignePreAut;
typedef struct
{
	UC			EMETTEUR[5+1];             	/* Propri?taire du fichier NAPS ou CMI	*/ //NAPS_CMI_INTEGREE
	ushort		nombre_tra;
	ulong		Indice_fin;
	UC 			etat;
	lignePreAut enr_pre_aut0[NBMAXPREAUT];
}StructPreAut;


typedef struct
{
	parametre	 		FicPara;
	Struct_compte_dom	Fic_compte_dom;
	Struct_File_Bin		FicBin;
	Struct_File_Prefixe	FicPrefixe;
	Struct_File_Ins_reg	FicInsReg;
	Struct_Devise_Acc   Fic_Devise_Acc;
	Struct_devise_reg   Fic_Devise_reg;
	//PARAMETRE EMV
	Fichier_EMV_AID		Fic_EMV_AID;
	Fichier_EMV_Keys    Fic_EMV_Keys;
	Fichier_EMV_TAC     Fic_EMV_TAC;
	Fichier_EMV_TDOL    Fic_EMV_TDOL;
	Fichier_EMV_DDOL    Fic_EMV_DDOL;
	Fichier_EMV_rand    Fic_EMV_rand;
	Fichier_EMV_TCC     Fic_EMV_TCC;
	Struct_Data_Last_TRS_Emv_Rej Data_Last_TRS_Emv_Rej;
	infopayment			StructInfoPayment;
}Fichier_Parametre;



/*
** TRANSACTION RESULT
*/
#define TRANSACTION_NO_RESULT				0
#define TRANSACTION_ACCEPTED				1
#define TRANSACTION_DECLINED				2
/*
** CVM CODES
*/
#define CVM_CODE_MASK						0x3F
#define CVM_NEXT_IF_UNSUCCESS				0x40
#define CVM_FAIL_PROCESSING					0x00
#define CVM_PLAINTEXT_PIN_VERIF				0x01
#define CVM_ENCIPH_PIN_VERIF_ONLINE			0x02
#define CVM_PLAINTEXT_PIN_VERIF_SIGN		0x03
#define CVM_ENCIPH_PIN_VERIF				0x04
#define CVM_ENCIPH_PIN_VERIF_SIGN			0x05
#define CVM_SIGN_ONLY						0x0E
#define CVM_NO_CVM_REQ						0x1F
#define CVM_NO_CVM_PERFORMED				0x3F

#define	PINOFFLINE							2
#define	FALLBACKR							3
#define	ANNULEMV							4

#define TAG_TCC								0x9F53

/*
typedef struct
{
    uint8 Length;
    uint8 Aid[16];
    uint8 PartialMatchAllowed;

}Aid_t;
*/
//cmiLop
L		GetAllLop(void);
L		PutAllLop(void);
L		LopFile(void);
L		DeleteLop(void);
L		function(void);
void	Init_Lop(void);
UC	ListeNoire(UC Numeroter,UC InitModem);
UC	Trait_Lop_Complete(UC Premier_Mess);
UC	Trait_Lop_Supp(UC Premier_Mess);
UC	Trait_Lop_Ins(UC Premier_Mess);
UC	InsNumero(UC *Buffer);
UC	SupprimerNumero(UC *Buffer);
void 	read_oppos_index(Ushort Index , typ_lop_enr_bcd *Pt_numero );
void 	write_oppos_index(Uint Index , typ_lop_enr_bcd *Pt_numero );
UC	rechlop(UC *numero);
UC	rechlop_index(UC *Buffer,Ushort *index);
UC	Recup_LOP_BIN(UC *buffer_mess, Ushort Lg_mess, Ushort *OffSet,UC *Buf_BIN,Ushort *Nb_bin);
UC	Recup_LOP_Tranche(UC *buffer_mess, Ushort Lg_mess, Ushort *OffSet,UC *Buf_tranche,Ushort *Nb_tranche);
UC	Recup_LOP_Card(UC *buffer_mess, Ushort Lg_mess, Ushort *OffSet,UC *Buf_Card,Ushort *Nb_card);
UC	InsBinLop(UC *Buffer);
UC	InsTrnLop(UC *Buffer);
UC	SupprimerLnBin(UC *Buffer);
UC	SupprimerLnTrn(UC *Buffer);
void	read_oppos_bin_index(Ushort Index , typ_lop_enr_bin *Pt_numero );
void	read_oppos_trn_index(Ushort Index , typ_lop_enr_trn *Pt_numero );
void	write_oppos_bin_index(Uint Index , typ_lop_enr_bin *Pt_numero );
void	write_oppos_trn_index(Uint Index , typ_lop_enr_trn *Pt_numero );
UC	rechlop_bin(UC *numero);
UC	rechlop_trn(UC *numero);
UC	rechlop_bin_index(UC *Buffer,Ushort *index);
UC	rechlop_trn_index(UC *Buffer,Ushort *index);
UC	Lire_Bin_Carte(UC *buffer_mess, Ushort Lg, Ushort *OffSet,UC Action, UC *Buf_Card,Ushort *Nb_card);
UC	Trait_transfer_Lop(UC Premier_Mess);

//cmiProtocole
L		Gerer_Event_Recu(PUC i_puc_buffer);
//L		Dial(PC a,UC b,UC c);
UC		connect_host(PC number_phone,PC site);
//UC		Receive(UL delay);
void	SessionDisconnect(UL Delay);
void	Sortir_App(void);
L		First_Connect(PC);
void	PrintReception(L l_indice);
void	Printtampon(PUC tampon,L	l_indice);
//cmiMessage
void	message_communication(UC,UC,L);
UC	Aff_mess(UC langue,Ushort id,UC Line,UC type_mess, Ushort Delay,UC Beep);
UC	Aff_indicNO_OUI(UC);
UC	Aff_indicRET_VAL(UC);
UC	Aff_indicMont(UC);
UC	Aff_indicOffPin(UC);
//cmiEdition
L		Os__xprinta(PC message);
L		Os__xprintaa(PC message);
L		Os__xprint(PC message);
L		Os__xprintd(L lond);
L		ShowTrace(PC message);
L		ShowTrace_Mouna(PC message);
L 		Print_Text_Sdgmnt (PC pBuffer, size_t ulsize);
L		Print_Text_Ctl(PC pBuffer, size_t Taille);
UL		GetFreeRamSize(void);
void	PrintTrce(L l_l_SECU_CipherPIN,C c);
L		EmvShowTrace(PC message);
//cmiISO8583
UC	Format(UC *typemessage,UC *bitmap,UC file_name);
UC	Emission(UC *typemessage,UC *bitmap);
UC	Reception(void);
void   	init_tpdu(void);
void   	init_pack( UC *msg_type, UC *buf);
UC  	*pack(UC bitn, UC *buf, Ushort len);
UC  	*unpack(Ushort a, UC *fbuf);
void   	format_Recept(Ushort a);
UC	ProcRecept(void);
Ushort 	format_Emis(Ushort a, UC *result);
UC	ProcRequest(UC *bitmp );
UC  	check41(void);
UC  	check42(void);
UC  	check11(void);
UC  	check_header(UC *typemess);
long	WaitRxMsg(UC *Buff);

UC	Format_comptes(UC ucindex);
UC	Format_dem_fj(void);
UC	Fin_jour(UC Numeroter,UC InitModem);
//UC	Format_avis(Ushort* Tab_avis,Ushort start,Ushort last_avis) ;
UC	Format_avis(Ushort* Tab_avis,Ushort last_avis_tr,
															Ushort last_avis_pre,Ushort start);
UC	Format_telec(Ushort start,Ushort last_avis);
UC	Format_confirmation(void) ;
UC	Format_rec(UC last_devise,UC pos_devise,long *Tab_mont);
UC	Format_Transfer_Trans(Ushort Index_trs,UC *BufferTrs,Ushort *LgBuff,UC *DataEmv,Ushort *LgDataEmv);
UC	Teleparametrage(UC Numeroter,UC InitModem);
UC	Transmission_file(UC file_name);
UC	Transmission(UC *typemessage,UC *bitmap,UC file_name);
UC	tel_auto(void);
void	AutomaticJob(void);
//cmiAllFile
L		ParFile(void);
L		GetAllParam(void);
L		PutAllParam(void);
void	Init_Param(void);
UC	Trait_transfer_param(UC Premier_Mess);
UC	Trait_transfer_Key(UC Premier_Mess);
UC	Trait_transfer_MacKey(UC Premier_Mess);
UC	Maj_bin(UC Type_maj);
UC	Maj_Prefixe(UC Type_maj);
UC	Trait_transfer_BIN(UC Premier_Mess);
UC	Trait_transfer_Prefixes(UC Premier_Mess);
UC	Trait_transfer_Compte_Dom(UC Premier_Mess);
UC	Trait_transfer_modif_compte_Dom(void);
UC	Trait_transfer_EMV_AID(UC Premier_Mess);
UC	Trait_transfer_EMV_KEYS(UC Premier_Mess);
UC	Trait_transfer_EMV_TAC(UC Premier_Mess);
UC	Trait_transfer_EMV_TDOL(UC Premier_Mess);
UC	Trait_transfer_EMV_DDOL(UC Premier_Mess);
UC	Trait_transfer_EMV_Rand_Call(UC Premier_Mess);
UC	Trait_transfer_EMV_TCC(UC Premier_Mess);
L		DeleteParam(void);
uint8	Rech_Aid(Struct_EMV_AID *St_Aid);
uint8	Rech_TAC(Struct_EMV_TAC *St_TAC);
uint8	Rech_TDOL(Struct_EMV_TDOL *St_TDOL);
uint8	Rech_DDOL(Struct_EMV_DDOL *St_DDOL);
uint8	Rech_Rand_Call(uint8 *pTraCurrCod,Struct_EMV_rand *St_EMV_rand);
uint8	Rech_TCC(Struct_EMV_TCC *St_EMV_TCC);
//cmiIngenico
void	Os__beep(void);
PC		dh_systeme(void);
L		Os__read_date(PUC);
L		Os__read_time(PUC);
L		Os__write_date(PUC);
L		Os__write_time(PUC);
L		long_asc(PUC a,L b,PUL c);
L		Os__saved_copy(PUC a,UC * b,L c);
L		bcd_asc(PUC a,PUC b,C c);
L		char_asc(PUC Dest,UC LgDest,PUC ptSrc);
UC		Receptionner(US a);
void	hex_str (PUC a,PUC b,C c);
void	Calcul_Mac(unsigned char *binput, UL Len_Mess, unsigned char *boutput, unsigned char *bkey);
void	Os__read_time_sec(UC *PtTime);
void	Os__hang_up(void);
void	Uti__display1_clr2(PC);
void	Os__display(C,C,PC);
void	Os__saved_set(PUC,C,unsigned int);
void	asc_hex(PUC,C,PUC,C);
void	desm1(PUC,PUC,PUC);
void	des(PUC,PUC,PUC);
void	hex_asc(PUC,PUC,C);
Ushort	bxdtge_stodcb( UC *p, UC *tab, Ushort nb_car);
void	bxdtge_dcbtos( UC *tab, UC *p, Ushort nb_car);
void	bintobcd(Ulong val, UC *buf,Ushort len);
UC	bittest(UC *bitmap, UC pbitno);
void	bitset(UC *bmap, UC bitnum);
void    set_ushort_secourue(Ushort *pt,Ushort val);
void    set_ulong_secourue(Ulong *pt,Ulong val);
void	Os__xdelay(L delay);
void	Uti__display2(PC);
void	asc_bcd(PUC,UC,PUC,UC);
int		subtract( UC *a, UC *b, UC *res );
UC	Mess_delay(UC id0,UC	id1,UC typmess,L delay,C beep);
UC	ValiderMessage(UC id0,UC id1,UC timeout);
UC	Valider(UC id0,UC id1);
UC	Os__get_key(void);
UC	Os__xget_key(Ushort Timeout);
UC	ValiderMess(PUC,PUC);
UC	OR_ValiderMess(PUC,PUC,PUC,PUC,PUC);

uchar	ValiderMess_(char);
UC	ValiderMessCompte(PUC,PUC);
UC	Valider_Transac(void);
void eft30printhex(char* buffer,Ushort n_car);
void Eft30PrintHex(char* buffer,Ushort n_car);
void eft30printhexGPRS(char* buffer,Ushort n_car);
void eft30printhexKamal(char* buffer,Ushort n_car);
L	ShowTrac_GPRS(PC message);
Ulong	LuhnKey(UC *card_nr);
//UC	Get_Text_Input(UC *Buffer,UC taille);
void	RemoveLeftZeros(UC *Ptr);
UC	Manip_Pwd(UC *Ptr,Ushort taille);
UC	goScrPwd(UC fct);
void	FormatAmount(UC *Ptr,UC *Tab,UC exposant);
void	GetDeviceCode(UC StatusCode,UC *Buffer);
UC	check_time(UC *time);
UC	check_date(UC *date);
void	vDisplayField( char* pcText,char* tcLine ) ;
UC	GetField_pin(UC id, UC* PinClair, UC *LgPin);
UC	FillWithZero(UC* buffer,C size);
UC	ucGetField(UC id, UC* buffer, C size,C min, C max,char* init, Ushort delay,UC pwd);
//UC	ChoiceListe(MENU *txList1,UC *ucIndex) ;
UC	luhn_key_ok(PUC	Number);
UC	*Os__xget_varkey(UC,C);
L		Os__timer_start(Ushort* delay);
L		Os__timer_stop(Ushort *TimeOut);
void	smon_to_lmon(PUL l_mont,PC p_pc_montant);
void	SaisirParam(UC status);
UC	Affich_ecr_pwd(UC Indic_pwd,UC Lg_password);
UC	Verif_pwd(UC Indic_pwd,UC Lg_password);
void	Change_Merchant_Pwd(void);
void	Code_PointService(UC *Code_pt_service,UC Entry_Mode,UC Entry_Pin,UC Lg_pin);
UC	Compte_domicil(void);
//cmiTra
Ushort	SaveTra(void);
void	RemplirSpecification(void);
//

void	IncStan(void);
UC	Prepare_Transaction(void);
UC	transa_client(void);
UC	Gest_avis(Ushort *indices_offline);
UC	Sale(void);
UC	Credit(void);
UC	Trait_Preaut(void);
UC	Completion(void);
UC	CompletionEmvOffLine(Ushort	indice);
UC	CompletionEmvOnLine(Ushort	indice);
UC	Verification_Digits(void);
UC	PushAmountScreen(UC *Tab,UC fix_devise,Ushort taille);
UC	GetAmountCmi(C c,Ulong *Tab,UC fix_devise,Ushort *default_dev,Ushort taille);
UC	AmountValid(Ulong* montant,Ushort *indices_offline);
UC	PlafondAtteint(Ulong amount,UC *Card,Ushort* indices_offline,Ulong*);
void	Sequence(UC aut,UC validate);
UC	gest_date(UC* update_date);
UC	AnnulTra(void);
UC	AnnulTraPuce(void);
UC	duplicata(UC);
UC	duplicPre(UC);
UC	cherchTra(UC *Buffer,Ushort *indice);
UC	cherchTraSTAN(UC *Buffer,Ushort *indice);
UC	Efface_Trs(void);
UC	tst_fic_vide(void);
UC	tst_fic_plein(void);
UC	valide_carte(void);
//cmiCard
uint8	PresentCard(UC *Buffer,char saisie);
UC  	Read_Card(UC *buffer,UC type_carte) ;
UC  	Recup_Info_Card(UC	*PtIso) ;
UC  	Recup_Info_badge(UC *PtIso,UC init) ;
UC	BinControl(UC*	bin) ;
UC	PrefixControl(UC*	bin) ;
UC	BinPrefixControl(UC *Card,C message);
UC	BinPrefixControlbis(uchar *Card);
UC	Saisie_Info_Carte(char codefunction) ;
UC  Saisie_Info_Or(char codefunction);

UC	Saisie_cvv(void);
UC	ident_maestro(UC *num_carte,UC type_carte);
UC	forcage(void);
UC 	encryp_code_conf(UC* pin,UC lg_pin) ;
UC  	traitement_code_conf(void);
//cmiAutor
UC	Mess39Screen(PC);
UC	DemAutor8583(UC* update_param,UC* update_date);
UC	Dem_avis(void);
UC	gest_param(void);
//cmiEdition
UC	imp_buffer(PC	Buffer,int i);
UC	Os__xlinefeed(C c);
UC	Imprime_Param(void) ;
UC	Imprime_Lop(void);
UC	Imprime_devises(void);
UC	Imprime_Prefixes(void);
UC	Imprime_Comptes(void);
UC	Imprime_Bin(void);
UC	ImprimeTra(UC langue,UC source,UC dup);
UC	ImprimeCapture_carte(UC langue,UC source) ;
//UC	ImprimeTelecollecte(UC ind_rec);
UC	ImprimeTelecollecte(UC ind_rec,UC tel_etat);
//UC	Calcul_total(Ushort* j,char* pPtr,UC ind_rec);
UC	Calcul_total(UC ind_rec);
UC	JournalEdit(void);
UC	JournalEditPre(void);
UC	ucDetectPinPad(void);
UC	Centrer_mess(UC* buffer,UC* dest);
void	captureMonth(Ushort l_mois,UC* mois);
void	AffErr(L numerr,C c);
L		DispErr(L numerr,C c);
L		Print_Last_Trs_Emv_Rej(void);
L		Print_Key_Emv(void);
//cmiTra
L		TraFile(void);
L		GetAllTra(void);
L		PutAllTra(void);
void	Init_Tra(void);
L		PreFile(void);
L		GetAllPre(void);
L		PutAllPre(void);
void	Init_Pre(void);
Ushort	SaveTra(void);
void	RemplirSpecification(void);
void	RemplirPreaut(void);
void	read_trans_index( Ushort Index ,lignetra *Pt_numero );
void	write_trans_index( Ushort Index ,lignetra *Pt_numero );
void	read_preauto_index( Ushort Index ,lignePreAut *Pt_numero );
void	write_preauto_index( Ushort Index ,lignePreAut *Pt_numero );
UC	cherchTra(UC *Buffer,Ushort *indice) ;
UC	CherchPreAuto(UC *Buffer,Ushort *indice);
UC	Efface_Trs(void);
UC	Efface_File_Preauto(void);
UC	tst_fic_vide(void);
UC	tst_fic_plein(void);
UC	tst_fic_preautor_plein(void);
UC		GetAmountEmv(C c);
void	FallBackTrait(void);
void	GestAvisParam(Ushort *indices_offline);
UC	Annul_preautor(UC );
UC	Annul_preautoremv(void);
void	PurgePreauto(void);
UC	CherchPreAuto_STAN(UC *STAN,Ushort *indice);
void TraiterEmv(UC );
//cmiUtil
void	compression(PUC numero_comp,PUC numero_dec,UC taille);
void	CONVERT_dcbtoa( UC *a_dcb , US us_longueur , C *a_ascii, int bZero);
US		CONVERT_atodcb( C *a_ascii , UC *a_dcb);
void	CONVERT_ultoa ( UL valeur, C *string,  US nb_chiffre );
UL		CONVERT_atoul ( C *i_pc_Source );
void	CONVERT_Ascii_To_Ebcdic(US Size, UC *Ascii, UC *Ebcdic);
void	CONVERT_Ebcdic_To_Ascii (US Size, UC *Ebcdic, UC *Ascii );
US		CONVERT_atoh( C *a_ascii , UC *a_hexa);
US		CONVERT_atoh2( C *a_ascii , UC *a_hexa,US l_us_Lng);
void	CONVERT_htoa ( UC *Hexa, US Lng, UC *Ascii );
US		CONVERT_DcbToInt ( US i_us_Lng, UC * i_uc_Dcb );
US		CONVERT_DataToInt(PUC i_puc_data);
void	CONVERT_IntToData(US i_us_int, PUC o_puc_data);
UL		CONVERT_DataToLong(PUC i_puc_data);
void	CONVERT_LongToData(UL i_ul_long, PUC o_puc_data);
void	UTIL_PaddingDataLeft(PUC io_puc_data, US i_us_size, US i_us_size_required);
void	CONVERT__hex_to_dec(char *chaine_hex, char *chaine_car, S lg);
US		CONVERT_asctodcb( C *a_ascii ,US us_longueur , UC *a_dcb);
void	recup_date_systeme(PC date,L format);
void	heure_systeme(PC heure,L format);
PC		addition(PC chaine1,PC chaine2,C taille);
PC		soustraction(PC chaine1,PC chaine2,C taille);
//cmiEmvSrv
L		EmvOpen(void);
void	EmvClose(void);
L		EmvService(UL i_ul_service);
UC		EmvGetData(void);
L		EmvReadIccData(US i_us_tag, PUS o_pus_length, PUC o_puc_data);
L		EmvReadAllIccData(PUC o_puc_data,C i_c);
L		ManagePinInput(void);
//cmiEmvDat
L		TagListInit(void);
void	TagListFree(void);
L		TagListGrow(UL i_ul_grow);
L		TagListIsPresent(US i_us_tag);
L		TagListAdd(US i_us_tag, US i_us_length, const UC* i_puc_value);
L		TagListErase(US i_us_tag);
L		TagListRead(US i_us_tag, PUS o_pus_length, PUC o_puc_value);
US		ReadTag(PUC Buffer, PL Tag);
US		ReadDataLength(PUC Buffer, PUS Length);
C		FindTag(L srcTag, PUS pOffset, PUS pDataLength, PUC pData, US Length);
C		ProcessScriptData(uint8 *MessageData, uint16 DataLength,uint8 *Offset71,uint8 *Script71,uint8 *Offset72,uint8 *Script72);
//cmiEmvPay
L		EmvGestionPin(void);
L		EmvGestionPin_Online(void);
L		Remplir_Infopayment(void);
UL		EmvGetFloor(void);
UL		EmvCumulMontant(PUC,US);
L		EmvUnableToGoOnline(void);
L		EmvUnableToGoOnline2(C);
L		EmvReferralProcessing(C);
L		EmvReferralProcessing2(C);
L		EmvAuthorisationProcessing(void);
L		EmvSecondAction(void);
UC		EmvGetNextAid(PUC,PUC);
UC		EmvRecupInfoLibrary(void);
L		EmvSaveTransa(void);
UL		EmvPrintTransa(void);

void	ReportReferralReceipt(uint8 Initiator);
//L		FinalPreAuth(void);


int		CarteCommercant(void);
int		CarteClient(void);
void	CUMORE_CMI_AFFICHE_MESSAGE(int ligne1,int ligne2,int delay);
void	CUMORE_CMI_AFFICHE_MESSAGE_SD(int ligne1,int ligne2,int delay);
void	CUMORE_CMI_PHONE_NUMBER(PC pPhoneNumberScreen);
void	CUMORE_CMI_CARD_NUMBER(PC pcardNumber,char);
UC		Teleparametrage(UC Numeroter,UC InitModem);
long	hang_up(char type,char trace);
char	conn_etabli(void);
//long connect_host(char *phone,char trace);
long	Dial(char *phone,char trace);
long	rechmem1(unsigned char *source,long lgs,unsigned char *ref,unsigned short lgr);
L		ReceptParam(UL delay, PC mot,UL Lg);
L		imadrech(PC source,UL lgs,PC ref,UL lgr);
long	receive(unsigned char *buffer,char trace);
L		FixationParam(void);
L		ValidAbonne(PC site);
long	Emettre(char *txbuf,unsigned short count);
void	ExploiField7(void);
struct BITD {
	unsigned char bitn;				/* bit number */
	unsigned char format;			/* bit format */
	unsigned char attrib;			/* attribute */
	unsigned char max;				/* max width */
};

#define BON_CON				0
#define PAS_CON				1
#define	NO_DIALTONE			2

#define delayA				250
#define delayB				2000
#define delayC				6000
#define delayI			    200
#define delayJ			    50
#define delayK			    400

#define RECEPTION_OK					0
#define RECEPTION_KO					1


/*
#define NUL     '\0'
#define SOH     0x01
#define STX     0x02
#define ETX     0x03
#define EOT     0x04
#define ACK     0x06
#define BEL     0x07
#define HT      0x09
#define LF      0x0A
#define CR      0x0D
#define DLE     0x10
#define NAK     0x15
#define ETB     0x17
#define FS	0x1C
*/

uchar	CheckMacEft30(void);

#define	_OFFLINE	0
#define	_ONLINE		1
#define	ANNUL_		2
#define	CREDIT		3

void	choix_aleatoire(void);
int	choix_devise_cmi(void);
int	choix_mode_tra(char saisie);
uint8	construire_champ55(void);


#define	SCRIPT_DATA_BUFFER_SIZE	255 //CCD

void	AjusterPariteImpaire(uchar *Str,Ushort lg);
uchar	Check_Message(void);
uchar	Emis_batch(uchar type);


int appel_caisse(void);
int send_caisse(unsigned char *buffer, int len, char fermport);
int receive_caisse(unsigned char *buffer, int len, char fermport);
int deco_msg_caise2(char *buffer,char trcc);

void	clock2int(int h_clock, unsigned char * nou, int * pi_yr, int * pi_mth, int * pi_day, int * pi_hr, int * pi_min, int * pi_sec, int * pi_wday);
uchar	ImpRese_ppad(char etareso);
uchar	EchoMessage_ppad(char etareso);


void	APEMV_UI_MessageDisplayDelay(char *message, int duration);
int		ParamReset();
int		Wait_AmountUSB(void);
int		send_caisse_echoUSB(unsigned char *buffer, int len, char fermport);
int		receive_caisseUSB(unsigned char *buffer, int len, char fermport);
L 		format_msg_caise2(char *buffer,int trcc);
void	CUMORE_PaiementFctTrame(unsigned long TransactionType, unsigned char ucParameters, unsigned long ulAmount, int nCurrencyCode);
int		controle_paiement(char*);
uchar	ImprimeTra_ppad(uchar langue,uchar source,uchar dup);
int		annulation_cmi(void);
int		dernieretrs_cmi(void);
int		autretrs_cmi(void);
int		testreseau_cmi(void);
uchar	Imprime_Param_caisse(void);
short   maintenance_ppad(char state[4]);
int		initialisation_cmi(void);
int		annulation_cmi(void);
int		Retour_Caisse(char * REPCODE);
