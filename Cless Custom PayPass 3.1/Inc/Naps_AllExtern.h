extern	L					sortir_boucle;
extern	L					reception;
extern	L					timer_fin;
extern	L					g_l_deconnecte;
extern	uchar				g_tompon1[MAX_MESS];
extern	UL					g_size;
extern	UL					g_ul_tag;
extern	L					g_connect_stat;
extern	lop_gest_enreg		lop_gest;              /* Entete */
extern	typ_lop_enr_bcd		lop_fichier[NBMAXOPP]; /* Tableau des lop */
extern	typ_lop_enr_bin		lop_bin[NBMAXOPPBIN];
extern	typ_lop_enr_trn		lop_trn[NBMAXOPPTRN];
extern	parametre	 		FicPara;
extern	Struct_compte_dom	Fic_compte_dom;
extern	Struct_File_Bin		FicBin;
extern	Struct_File_Prefixe	FicPrefixe;
extern	Struct_File_Ins_reg	FicInsReg;
extern	Struct_Devise_Acc   Fic_Devise_Acc;
extern	Struct_devise_reg   Fic_Devise_reg;
//PARAMETRE EMV
extern	Fichier_EMV_AID		Fic_EMV_AID;
extern	Fichier_EMV_Keys    Fic_EMV_Keys;
extern	Fichier_EMV_TAC     Fic_EMV_TAC;
extern	Fichier_EMV_TDOL    Fic_EMV_TDOL;
extern	Fichier_EMV_DDOL    Fic_EMV_DDOL;
extern	Fichier_EMV_rand    Fic_EMV_rand;
extern	Fichier_EMV_TCC     Fic_EMV_TCC;
extern	Struct_Data_Last_TRS_Emv_Rej Data_Last_TRS_Emv_Rej;
extern	Struct_Data_Last_TRS_Emv_Rej Data_Last_TRS_Emv_Rej_tmp;
extern	Ushort				g_Num_Seq_Bloc;
extern	emis				E_Transac8583;
extern	rece				R_Transac8583;
extern  uchar	bmp1200[];
extern  uchar	bitmap_avis[];
extern  uchar	bmp1420[];
extern	infopayment			StructInfoPayment;
extern	lignetra			StrLineTra;
extern	lignePreAut			StrPreAut;
extern	Fichier_Transaction	FicTra;
extern	StructPreAut		FicPreauth;
extern	C					g_trace_emv;
extern	L					Nb_connexion;
extern	L					Nb_deconnexion;
extern	L					g_trace;
extern	L					g_arqc;
extern	Ushort				g_indices_offline[MAX_AVIS+1];
extern	Ushort				g_indice_completion;
extern	C					typehangup;
extern	Ushort				gdevise_num;
extern  int					g_type_GPRS;
extern  int					g_nHandle;
extern	MSG_CAISSE			StructInfoCaisse[3];
extern  MSG_CAISSE			SaveLastInfoCaisse;



extern unsigned short 		CustApplicationNumber;				//!< Custom application type.



