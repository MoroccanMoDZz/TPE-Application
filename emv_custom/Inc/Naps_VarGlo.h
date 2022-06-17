L					sortir_boucle;
L					reception;
L					timer_fin;
L					g_l_deconnecte;
uchar				g_tompon1[MAX_MESS];
UL					g_size;
//HANDLE				g_h_handle;
//HANDLE				h_timer;
UL					g_ul_tag;
L					g_connect_stat;

lop_gest_enreg		lop_gest;				/*	Entete */
typ_lop_enr_bcd		lop_fichier[NBMAXOPP];	/*	Tableau des lop */
typ_lop_enr_bin		lop_bin[NBMAXOPPBIN];	/*	Tableau des bin */
typ_lop_enr_trn		lop_trn[NBMAXOPPTRN];	/*	Tableau des tranches */


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
Struct_Data_Last_TRS_Emv_Rej Data_Last_TRS_Emv_Rej_tmp;
Ushort				g_Num_Seq_Bloc;
emis				E_Transac8583;
rece				R_Transac8583;

infopayment			StructInfoPayment ;
infonotation		StructInfoNotation;
infoEcheance		StructinfoEcheance[MAXECHEANCES];
lignetra			StrLineTra ;
lignePreAut			StrPreAut;
Fichier_Transaction	FicTra;
StructPreAut		FicPreauth;
StructPreAut		FicPreauthCMI;
StructPreAut		FicPreauthNAPS;

//PT__MT_TYPE_W_SELECTED_AID	g_p_selected_aid;
C					g_trace_emv;

L					Nb_connexion;
L					Nb_deconnexion;
L					g_trace;
L					g_arqc;
Ushort				g_indices_offline[MAX_AVIS+1];
Ushort				g_indice_completion;
//char	typehangup=1; declare ds cmiiso8583
Ushort				gdevise_num;

int					g_type_GPRS;
int					g_nHandle;
MSG_CAISSE			StructInfoCaisse[2];
MSG_CAISSE			SaveLastInfoCaisse;
