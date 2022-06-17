/* ======================================================================== */
/* Include Files                                                            */
/* ======================================================================== */

#include "SDK_Naps.h"

UC		Buffer[1500];
int		ii;

//extern MSG_CAISSE  StructInfoCaisse[2];
extern	int	GlLine;
extern	int	DebutLine;
#define	C_JUSTIFY1	1


L	ShowTrace(PC message)
{
	return 0;

	Telium_File_t *printer ;
	printer = Telium_Fopen("PRINTER","w-");

	/* ##1 print the contents in condensed mode  */
	Telium_Fprintf(printer,"\x0f");
	Telium_Fprintf(printer,"\x0f""%s \n",message);
	/* ##1 print the reading status in bold */
	Telium_Fprintf(printer,"\x12""\n");
	/* ##1 wait for print end */
	Telium_Ttestall ( PRINTER , 300 );
	Telium_Fclose(printer);
	return 0;
}


L	Os__xprint(PC message)
{
	Telium_File_t *printer=NULL;

	return 0;

	printer = Telium_Fopen("PRINTER","w-");

	/* ##1 print the contents in condensed mode  */
	Telium_Fprintf(printer,"\x0f""%s \n",message);
	/* ##1 print the reading status in bold */
	Telium_Fprintf(printer,"\x12""\n");
	/* ##1 wait for print end */
	Telium_Ttestall ( PRINTER , 300 );
	Telium_Fclose(printer);
	return 0;
}


uchar imp_buffer(char* buffer,int n_car)
{
	Telium_File_t *printer1;
	STATUS	st;
	int c;

	printer1 = Telium_Fopen("PRINTER","w-");
	if ( printer1 != NULL )
	{
		for(c =0;c<n_car;c++)
		{
			Telium_Fprintf(printer1,"%c",buffer[c]);
			Telium_Status(printer1 ,&st);
			if(
				( st & PAPER_OUT)
	//			||( st & PRINT_END)
				||( st & PRINT_ERROR)
			)
			{
				Telium_Fclose(printer1);
				//avertissement
				printf("\x1B"" PAPIER ABSENT\n");
				printf (" NO MORE PAPER");
				switch (Telium_Ttestall( KEYBOARD , 500 ))
				{
				case KEYBOARD :
					break;
				case TIMEOUT :
					break;
				}

				return KO;
			}
		}
		if(n_car)
		{
			Telium_Fprintf(printer1,"\n");
			Telium_Ttestall ( PRINTER , 300 );
		}
		Telium_Fclose(printer1);
	}
	return OK;
}

/*-----------------------------------------------------------------             
 * Fonction   :Calcul_total
 * objet      :
 *             
 * Parametre  :
 * Retour     :
 *-----------------------------------------------------------------             
*/
uchar	Calcul_total(Ushort* j,char* pPtr,uchar ind_rec)
{
	// le 1er dim pour type carte,2éme di pour la devise,3éme dim pour type transac
	struct Totaux
	{
		long		Total[NB_MAX_INS_REG][NB_MAX_DEV_ACC][4] ;
		Ulong		Nbre_tra[NB_MAX_INS_REG][NB_MAX_DEV_ACC][4];
	}StrReport;
	Ulong			Nbre_tra;
	lignetra		transac ;
	uchar			type_carte,trouve_type;
	uchar			devise,k;
	uchar			status;
	Ushort   		nombre_tra,saisi_man=0;
	uchar			montant[13]  ;
	uchar			affiche_mont[14];
	uchar			monnai_alpha[4];
	uchar			c_nbr_tra[5];
	long			l_total=0;
	uchar			Exp;	
	int		i=0;

	memset(&StrReport,0,sizeof(StrReport));
	Nbre_tra = 0;
	memset(montant,0,13);
	memset(affiche_mont,0,14);
	memset(monnai_alpha,0,4);
	
	nombre_tra=FicTra.nombre_tra ;
	
	if(nombre_tra==0)
		return KO ;
	
	i=0 ;
	while(i<nombre_tra)
	{
		read_trans_index( i ,&transac) ;

		type_carte = transac.Type_carte;
		
		for(k=0;k<NB_MAX_DEV_ACC;k++)            //l'indice de la 2éme dim
		{
			if(!memcmp(Fic_Devise_Acc.TabDevise[k].code_num,transac.NumCurrency,3))
			{
				devise=k;
				break;
			}
		}
		
		if(transac.CodeFunction!=VOIDCODE)
		{
			
			if(transac.CodeFunction==REFUNDCODE)
			{
				StrReport.Total[type_carte][devise][CREDIT]+=transac.Transaction_Amount;
				StrReport.Nbre_tra[type_carte][devise][CREDIT]+=1;
			}
			else
			{
				if(ind_rec == 2)//TOTAUX
				{
					StrReport.Total[type_carte][devise][_ONLINE]+=transac.Transaction_Amount;
					StrReport.Nbre_tra[type_carte][devise][_ONLINE]+=1;
				}
				else
				{
					if(!transac.Flag_Dem_Aut)
					{
						StrReport.Total[type_carte][devise][_OFFLINE]+=transac.Transaction_Amount;
						StrReport.Nbre_tra[type_carte][devise][_OFFLINE]+=1;
					}
					else
					{
						StrReport.Total[type_carte][devise][_ONLINE]+=transac.Transaction_Amount;
						StrReport.Nbre_tra[type_carte][devise][_ONLINE]+=1;
					}
				}
			}
		}
		else
		{
			StrReport.Total[type_carte][devise][ANNUL_]+=transac.Transaction_Amount;
			StrReport.Nbre_tra[type_carte][devise][ANNUL_]+=1;
		}
		if(transac.Entry_Mode==TRS_MAN)                                        //Nbre saisie manuelle
			saisi_man+=1;
		
		Nbre_tra	+= 1;

		i++;
	}
	
	for(i=0;i<NB_MAX_INS_REG;i++)
	{      
		trouve_type=0 ;
		for(k=0;k<NB_MAX_DEV_ACC;k++)
		{
			if( StrReport.Total[i][k][_OFFLINE]
				+StrReport.Total[i][k][_ONLINE]
				+StrReport.Total[i][k][CREDIT]
				+StrReport.Total[i][k][ANNUL_]!=0
				)
			{
				/*(*j) += sprintf(pPtr+*j, "<#f=%08lx#>",(UL)SYSTEM_FONT_8x14_ID);//Font Gras*/
				(*j) += sprintf(pPtr+*j, "\x1B""@");//Font Normal
				(*j) += sprintf(pPtr+*j, "\x1B""E");//Font Gras
				if(trouve_type==0)
				{            		
					(*j) += sprintf(pPtr+*j,"\n\n					%s",FicInsReg.Tab_Ins_reg[i].Libelle);
					trouve_type=1 ;
				}
				if((ind_rec == 0)||(ind_rec == 1))
				{
					memcpy(monnai_alpha,Fic_Devise_Acc.TabDevise[k].code_alph,3);
					(*j) += sprintf(pPtr+*j, "\n%.3s-",monnai_alpha);
					//(*j) += sprintf(pPtr+*j, "<#f=%08lx#>",(UL)SYSTEM_FONT_5x7_ID);//Font Normal
					(*j) += sprintf(pPtr+*j, "\x1B""F");//Font Normal

					//OFFLINE
					memset(montant,0,13);
					memset(affiche_mont,0,14);
					memset(c_nbr_tra,0,4);
					long_asc (montant, 12, (Ulong *)&(StrReport.Total[i][k][_OFFLINE]));
					long_asc (c_nbr_tra, 4, (Ulong *)&(StrReport.Nbre_tra[i][k][_OFFLINE]));
					RemoveLeftZeros(montant) ;
					Exp=Fic_Devise_Acc.TabDevise[k].Exposant-48;
					FormatAmount(montant,affiche_mont,Exp);
					(*j) += sprintf(pPtr+*j, "\nOffline:%.4s%12s",c_nbr_tra,affiche_mont);
					
					//ONLINE
					memset(montant,0,13);
					memset(affiche_mont,0,14);
					memset(c_nbr_tra,0,4);
					long_asc (montant, 12, (Ulong *)&(StrReport.Total[i][k][_ONLINE]));
					long_asc (c_nbr_tra, 4, (Ulong *)&(StrReport.Nbre_tra[i][k][_ONLINE]));
					RemoveLeftZeros(montant) ;
					Exp=Fic_Devise_Acc.TabDevise[k].Exposant-48;
					FormatAmount(montant,affiche_mont,Exp);
					(*j) += sprintf(pPtr+*j, "\nOnline :%.4s%12s",c_nbr_tra,affiche_mont);

					//ANNULEE
					memset(montant,0,13);
					memset(affiche_mont,0,14);
					memset(c_nbr_tra,0,4);
					long_asc (montant, 12, (Ulong *)&(StrReport.Total[i][k][ANNUL_]));
					long_asc (c_nbr_tra, 4, (Ulong *)&(StrReport.Nbre_tra[i][k][ANNUL_]));
					RemoveLeftZeros(montant) ;
					Exp=Fic_Devise_Acc.TabDevise[k].Exposant-48;
					FormatAmount(montant,affiche_mont,Exp);
					(*j) += sprintf(pPtr+*j, "\nAnnulee:%.4s%12s",c_nbr_tra,affiche_mont);
					
					//CREDIT
					memset(montant,0,13);
					memset(affiche_mont,0,14);
					memset(c_nbr_tra,0,4);
					long_asc (montant, 12, (Ulong *)&(StrReport.Total[i][k][CREDIT]));
					long_asc (c_nbr_tra, 4, (Ulong *)&(StrReport.Nbre_tra[i][k][CREDIT]));
					RemoveLeftZeros(montant) ;
					Exp=Fic_Devise_Acc.TabDevise[k].Exposant-48;
					FormatAmount(montant,affiche_mont,Exp);
					(*j) += sprintf(pPtr+*j, "\nCredit :%.4s%12s",c_nbr_tra,affiche_mont);

					//TOTAL
					if((StrReport.Total[i][k][_OFFLINE]+StrReport.Total[i][k][_ONLINE])>(StrReport.Total[i][k][ANNUL_]+StrReport.Total[i][k][CREDIT]))
						l_total=(StrReport.Total[i][k][_OFFLINE]+StrReport.Total[i][k][_ONLINE]) - (StrReport.Total[i][k][ANNUL_]+StrReport.Total[i][k][CREDIT]);
					else
						l_total=(StrReport.Total[i][k][ANNUL_]+StrReport.Total[i][k][CREDIT]) - (StrReport.Total[i][k][_OFFLINE]+StrReport.Total[i][k][_ONLINE]);
					memset(montant,0,13);
					memset(affiche_mont,0,14);
					long_asc (montant, 12, (Ulong *)&l_total);
					RemoveLeftZeros(montant) ;
					Exp=Fic_Devise_Acc.TabDevise[k].Exposant-48;
					FormatAmount(montant,affiche_mont,Exp);
					if((StrReport.Total[i][k][_OFFLINE]+StrReport.Total[i][k][_ONLINE])<(StrReport.Total[i][k][ANNUL_]+StrReport.Total[i][k][CREDIT]))
						(*j) += sprintf(pPtr+*j, "\nTotal  :\n   -%20s\n",affiche_mont);
					else
						(*j) += sprintf(pPtr+*j, "\nTotal  :\n   +%20s\n",affiche_mont);
				}
				else
				{
					memcpy(monnai_alpha,Fic_Devise_Acc.TabDevise[k].code_alph,3);
					(*j) += sprintf(pPtr+*j, "\n%.3s-",monnai_alpha);              // monnaie alpha
					//(*j) += sprintf(pPtr+*j, "<#f=%08lx#>",(UL)SYSTEM_FONT_5x7_ID);//Font Normal
					(*j) += sprintf(pPtr+*j, "\x1B""F");//Font Normal
					
					//NOMBRE
					Nbre_tra = StrReport.Nbre_tra[i][k][_ONLINE] + StrReport.Nbre_tra[i][k][CREDIT] + StrReport.Nbre_tra[i][k][ANNUL_];
					memset(c_nbr_tra,0,4);
					long_asc (c_nbr_tra, 4, (Ulong *)&Nbre_tra);
					(*j) += sprintf(pPtr+*j, "\nNbr.Trans :         %.4s",c_nbr_tra);
					
					//DEBIT
					memset(montant,0,13);
					memset(affiche_mont,0,14);
					memset(c_nbr_tra,0,4);
					long_asc (montant, 12, (Ulong *)&(StrReport.Total[i][k][_ONLINE]));
					long_asc (c_nbr_tra, 4, (Ulong *)&(StrReport.Nbre_tra[i][k][_ONLINE]));
					RemoveLeftZeros(montant) ;
					Exp=Fic_Devise_Acc.TabDevise[k].Exposant-48;
					FormatAmount(montant,affiche_mont,Exp);
					(*j) += sprintf(pPtr+*j, "\nDebit     :	%12s",affiche_mont);
					
					//CREDIT
					memset(montant,0,13);
					memset(affiche_mont,0,14);
					memset(c_nbr_tra,0,4);
					long_asc (montant, 12, (Ulong *)&(StrReport.Total[i][k][CREDIT]));
					long_asc (c_nbr_tra, 4, (Ulong *)&(StrReport.Nbre_tra[i][k][CREDIT]));
					RemoveLeftZeros(montant) ;
					Exp=Fic_Devise_Acc.TabDevise[k].Exposant-48;
					FormatAmount(montant,affiche_mont,Exp);
					(*j) += sprintf(pPtr+*j, "\nCredit    :	%12s",affiche_mont);
					
					//ANNUL_
					memset(montant,0,13);
					memset(affiche_mont,0,14);
					memset(c_nbr_tra,0,4);
					long_asc (montant, 12, (Ulong *)&(StrReport.Total[i][k][ANNUL_]));
					long_asc (c_nbr_tra, 4, (Ulong *)&(StrReport.Nbre_tra[i][k][ANNUL_]));
					RemoveLeftZeros(montant) ;
					Exp=Fic_Devise_Acc.TabDevise[k].Exposant-48;
					FormatAmount(montant,affiche_mont,Exp);
					(*j) += sprintf(pPtr+*j, "\nAnnulation:	%12s",affiche_mont);
					
					//TOTAL
					if((StrReport.Total[i][k][_ONLINE])>(StrReport.Total[i][k][ANNUL_]+StrReport.Total[i][k][CREDIT]))
						l_total=StrReport.Total[i][k][_ONLINE] - (StrReport.Total[i][k][ANNUL_]+StrReport.Total[i][k][CREDIT]);
					else
						l_total=(StrReport.Total[i][k][ANNUL_]+StrReport.Total[i][k][CREDIT]) - StrReport.Total[i][k][_ONLINE];
					memset(montant,0,13);
					memset(affiche_mont,0,14);
					long_asc (montant, 12, (Ulong *)&l_total);
					RemoveLeftZeros(montant) ;
					Exp=Fic_Devise_Acc.TabDevise[k].Exposant-48;
					FormatAmount(montant,affiche_mont,Exp);
					if(StrReport.Total[i][k][_ONLINE]<(StrReport.Total[i][k][ANNUL_]+StrReport.Total[i][k][CREDIT]))
						(*j) += sprintf(pPtr+*j, "\nTotal:\n   -%20s\n",affiche_mont);
					else
						(*j) += sprintf(pPtr+*j, "\nTotal:\n   +%20s\n",affiche_mont);
				}
			}
			if (*j >= 400)
			{
				status = imp_buffer(pPtr,*j);
				if(status != OK)
					return KO;
				*j = 0;
				//memset(pPtr,0,sizeof(pPtr));
				memset(pPtr,0,600);
			}
		}
	}
	if((ind_rec == 0)||(ind_rec == 1))
	{
		/*(*j) += sprintf(pPtr+*j, "<#f=%08lx#>",(UL)SYSTEM_FONT_5x7_ID);//Font Normal*/
		(*j) += sprintf(pPtr+*j, "\x1B""F");//Font Normal
		(*j) += sprintf(pPtr+*j, "\n\n%02d",FicTra.nbr_card_capture);
		(*j) += sprintf(pPtr+*j, "\n%02d",saisi_man);
	}
	return OK;
}

uchar	ImprimeTra(uchar langue,uchar source,uchar dup)
{
	char		pPtr[1000];

	int			j=0;
	uchar		data_time[13];
	char		affich_Cs[11];
	uchar		montant[13]  ;
	uchar		affiche_mont[14];
	uchar		Card_Nb[20];
	uchar		Exp;
	uchar		status;
	Ulong		MontComp;
	char		NumSeqReliquat[6];

	StructInfoPayment.waitpuce = 0;
	/*Aff_indicNO_OUI(FicPara.Langue);*/
	CUMORE_CMI_AFFICHE_MESSAGE(94,32,1000);
	//Aff_mess(FicPara.Langue,94,0,0,0,0);
	//Aff_mess(FicPara.Langue,32,1,0,0,0);

	memset(pPtr,0x00,sizeof(pPtr)) ;
	memset(affich_Cs,0x00,11) ;
	memset(montant,0x00,13) ;
	memset(affiche_mont,0x00,14) ;

	//CMI Gras
	j=0 ;
	//j += sprintf(pPtr+j, "<#f=%08lx#>",(UL)SYSTEM_FONT_8x14_ID);//Font Gra
	j += sprintf(pPtr+j, "\x1B""@");//Font Normal
//	j += sprintf(pPtr+j, "\x1B""E");//Font Gra
	j += sprintf(pPtr+j, "\x1B""E            CENTRE\n");
	j += sprintf(pPtr+j, "\x1B""E          MONETIQUE\n");
	j += sprintf(pPtr+j, "\x1B""E       INTERBANCAIRE\n");

	//DUPLICATA Gras	||	PRE-AUTORISATION	||	VAL.TRX.PRE-AUTO	||
	if(dup)
   		j += sprintf(pPtr+j, "\n         DUPLICATA");

	if(StructInfoPayment.CodeFunction == PREAUTHCODE)
		j += sprintf(pPtr+j, "\n      PRE-AUTORISATION");
	else if(StructInfoPayment.CodeFunction == PREAUTHCOMPCODE)
		j += sprintf(pPtr+j, "\n      VAL.TRX.PRE-AUTO");
	else
		j += sprintf(pPtr+j, "\n");

	//JJ/MM/AA	HH:MM:SS	+	N pos	+	N terminal	+	Ensei Com	+	Ville
	//j += sprintf(pPtr+j, "<#f=%08lx#>",(UL)SYSTEM_FONT_5x7_ID);//Font Normal
	j += sprintf(pPtr+j, "\x1B""F");//Font Normal
	memcpy(data_time,StructInfoPayment.Transaction_DateTime,12) ;
	j += sprintf(pPtr+j, "\n%.2s/%.2s/%.2s						%.2s:%.2s:%.2s",
		data_time+4,data_time+2,data_time,data_time+6,data_time+8,data_time+10);

	j += sprintf(pPtr+j, "\n%.15s",StructInfoPayment.Num_point_vente);
	j += sprintf(pPtr+j, "\n%.8s",FicPara.Num_Terminal);
	j += sprintf(pPtr+j, "\n%.24s",FicPara.Merchant_Name);
	j += sprintf(pPtr+j, "\n%.13s",FicPara.Merchant_City);

	//Nom du porteur
	if(StructInfoPayment.Appli_label[0]!=0)
	{
		j += sprintf(pPtr+j, "\n\n%s",StructInfoPayment.Appli_aid+1);
		j += sprintf(pPtr+j, "\n%.16s",StructInfoPayment.Appli_label);
		j += sprintf(pPtr+j, "\n%.24s",StructInfoPayment.Nom_Prenom);
	}
	else
		j += sprintf(pPtr+j, "\n\n%.24s",StructInfoPayment.Nom_Prenom);

	//Numéro de la carte
	memset(Card_Nb,0,20);
	memcpy(Card_Nb,&StructInfoPayment.Holder_Card[1],StructInfoPayment.Holder_Card[0]);

//****************************//
	status = imp_buffer(pPtr,j);
	if(status!=OK)
	{
		Os__display(0,0,"Pb. Impression");
		CUTERMpaperFeed();
		return status;
	}
	j=0;
	memset(pPtr,0,sizeof(pPtr));
//****************************//
	if(source==CLIENT)
	{
//		Card_Nb[strlen(Card_Nb)-1] = '*';
//		Card_Nb[strlen(Card_Nb)-2] = '*';
//		Card_Nb[strlen(Card_Nb)-3] = '*';
//		Card_Nb[strlen(Card_Nb)-4] = '*';
		Ushort ik;

		for(ik=0;ik<strlen((char*)Card_Nb)-4;ik++)
			Card_Nb[ik] = '*';
	}

	j += sprintf(pPtr+j, "\n%s",Card_Nb);
	//MM/AA	+	CARTE ETRANGERE||CARTE NATIONALE
	j += sprintf(pPtr+j, "\n%.2s/%.2s\t\t",StructInfoPayment.Expired_Date+2,StructInfoPayment.Expired_Date);
	if(StructInfoPayment.Origine_carte == CARTE_NATIONALE)
		j += sprintf(pPtr+j, "CARTE NATIONALE");
	else
		j += sprintf(pPtr+j, "CARTE ETRANGERE");
	//CERTIF EMV
	if(StructInfoPayment.Entry_Mode==TRS_EMV)
		j += sprintf(pPtr+j, "\n%.16s",StructInfoPayment.Certif);
	//XXX-C-YYY-L-Z
	memcpy(affich_Cs,StructInfoPayment.SeviceCode,3) ;
	//if(StructInfoPayment.forcage)
	if((StructInfoPayment.forcage)||(StructInfoPayment.Top_Forcage == TOP_FOR_REFFERAL))
   		affich_Cs[3]='1';
	else
   		affich_Cs[3]='0';
	memcpy(affich_Cs+4,&lop_gest.NumListe[0],4) ;
	if(StructInfoPayment.Entry_Mode==TRS_EMV)
      	affich_Cs[8]='1';
	else if(StructInfoPayment.Entry_Mode==TRS_MAN)
		affich_Cs[8]='2';
	else
   	affich_Cs[8]='0';
	sprintf(affich_Cs+9,"%.02d",FicInsReg.Tab_Ins_reg[StructInfoPayment.Type_carte].Code_Prefixe);
	j += sprintf(pPtr+j, "\n%.3s-%.1s-%.4s-%.1s-%.2s",affich_Cs,affich_Cs+3,affich_Cs+4,affich_Cs+8,affich_Cs+9);
	//Message1
	/*
	//if(!dup)
		j += sprintf(pPtr+j, "\n\n		%.24s",StructInfoPayment.Message1);
	*/
//****************************//
	status = imp_buffer(pPtr,j);
	if(status!=OK)
	{
		Os__display(0,0,"Pb. Impression");
		CUTERMpaperFeed();
		return status;
	}
	j=0;
	memset(pPtr,0,sizeof(pPtr));
//****************************//

	//pre autorisation
	if(
			(StructInfoPayment.CodeFunction==PREAUTHCOMPCODE)
		&&	(!dup)
		&&	(StructInfoPayment.Preauto_Amount_Final > StructInfoPayment.Preauto_Amount_Initial +
														(StructInfoPayment.Preauto_Amount_Initial*15)/100)
	)
	{
		//Pre-autorisation
		//j += sprintf(pPtr+j, "<#f=%08lx#>",(UL)SYSTEM_FONT_8x14_ID);//Font Gra
		j += sprintf(pPtr+j, "\x1B""E");//Font Gra
		j += sprintf(pPtr+j, "\n\n		Pre-autorisation");
		//j += sprintf(pPtr+j, "<#f=%08lx#>",(UL)SYSTEM_FONT_5x7_ID);//Font Normal
		j += sprintf(pPtr+j, "\x1B""F");//Font Normal
		//JJ/MM/AA	HH:MM:SS
		memcpy(data_time,StrPreAut.Transaction_DateTime,12);
		j += sprintf(pPtr+j, "\n%.2s/%.2s/%.2s						%.2s:%.2s:%.2s",
			data_time+4,data_time+2,data_time,data_time+6,data_time+8,data_time+10);
		//Montant
		memset(affiche_mont,0x00,14) ;
		long_asc (montant, 12, &StrPreAut.Transaction_Amount);
		RemoveLeftZeros(montant) ;
		Exp=StructInfoPayment.Devise_acc.Exposant-48;
		FormatAmount(montant,affiche_mont,Exp);
		j += sprintf(pPtr+j, "\nMnt :		%.13s%.3s",affiche_mont,StructInfoPayment.Devise_acc.code_alph);
		//Num transaction
		j += sprintf(pPtr+j, "\nNum.Transaction : %.6s",StructInfoPayment.Num_seq_save_compl_online);
		//Num autorisation
		j += sprintf(pPtr+j, "\nNum.Autorisation: %.6s",StrPreAut.Approval_Code);
		//Stan
		j += sprintf(pPtr+j, "\nSTAN            : %.6s",StrPreAut.STAN);

		//Complement
		//j += sprintf(pPtr+j, "<#f=%08lx#>",(UL)SYSTEM_FONT_8x14_ID);//Font Gra
		j += sprintf(pPtr+j, "\x1B""E");//Font Gra
		j += sprintf(pPtr+j, "\n\n		Complement");
		//j += sprintf(pPtr+j, "<#f=%08lx#>",(UL)SYSTEM_FONT_5x7_ID);//Font Normal
		j += sprintf(pPtr+j, "\x1B""F");//Font Normal
		//JJ/MM/AA	HH:MM:SS
		memcpy(data_time,StructInfoPayment.Transaction_DateTime,12) ;
		j += sprintf(pPtr+j, "\n%.2s/%.2s/%.2s						%.2s:%.2s:%.2s",
			data_time+4,data_time+2,data_time,data_time+6,data_time+8,data_time+10);
		//Montant
		memset(affiche_mont,0x00,14) ;
		MontComp = StructInfoPayment.Transaction_Amount - StrPreAut.Transaction_Amount;
		long_asc (montant, 12, &MontComp);
		RemoveLeftZeros(montant) ;
		Exp=StructInfoPayment.Devise_acc.Exposant-48;
		FormatAmount(montant,affiche_mont,Exp);
		j += sprintf(pPtr+j, "\nMnt :		%.13s%.3s",affiche_mont,StructInfoPayment.Devise_acc.code_alph);
		//Num transaction
		memcpy(NumSeqReliquat,StructInfoPayment.Num_seq_save_compl_online,6);
		addition(NumSeqReliquat,"000001",6);
		j += sprintf(pPtr+j, "\nNum.Transaction : %.6s",NumSeqReliquat);
		//Num autorisation
		j += sprintf(pPtr+j, "\nNum.Autorisation: %.6s",StructInfoPayment.Approval_Code);
		//Stan
		j += sprintf(pPtr+j, "\nSTAN            : %.6s",StructInfoPayment.STAN);
		//Montant total
		memset(affiche_mont,0x00,14);
		long_asc (montant, 12, &StructInfoPayment.Transaction_Amount);
		RemoveLeftZeros(montant);
		Exp=StructInfoPayment.Devise_acc.Exposant-48;
		FormatAmount(montant,affiche_mont,Exp);
		j += sprintf(pPtr+j, "\n\nMnt Total:		%.13s%.3s",affiche_mont,StructInfoPayment.Devise_acc.code_alph);

//****************************//
	status = imp_buffer(pPtr,j);
	if(status!=OK)
	{
		Os__display(0,0,"Pb. Impression");
		CUTERMpaperFeed();
		return status;
	}
	j=0;
	memset(pPtr,0,sizeof(pPtr));
//****************************//

	}
	else
	{
		//Montant
		long_asc (montant, 12, &StructInfoPayment.Transaction_Amount);
		RemoveLeftZeros(montant) ;
		Exp=StructInfoPayment.Devise_acc.Exposant-48;
		FormatAmount(montant,affiche_mont,Exp);
		j += sprintf(pPtr+j, "\nMnt :		%.13s%.3s",affiche_mont,StructInfoPayment.Devise_acc.code_alph);
		//Num transaction//Num Pre autorisation
		if(StructInfoPayment.CodeFunction==PREAUTHCODE)
			j += sprintf(pPtr+j, "\nNum.Pre-autori  :%.6s",StructInfoPayment.Num_sequence);
		else
			j += sprintf(pPtr+j, "\nNum.Transaction :%.6s",StructInfoPayment.Num_sequence);
		//Num autorisation
		j += sprintf(pPtr+j, "\nNum.Autorisation: %.6s",StructInfoPayment.Approval_Code);
		//Stan
		j += sprintf(pPtr+j, "\nSTAN            : %.6s",StructInfoPayment.STAN);
//****************************//
		status = imp_buffer(pPtr,j);
		if(status!=OK)
		{
			Os__display(0,0,"Pb. Impression");
			CUTERMpaperFeed();
			return status;
		}
		j=0;
		memset(pPtr,0,sizeof(pPtr));
//****************************//
	}
	//DEBIT	||	ANNULATION	||	CREDIT
	//j += sprintf(pPtr+j, "<#f=%08lx#>",(UL)SYSTEM_FONT_8x14_ID);//Font Gra
	j += sprintf(pPtr+j, "\x1B""E");//Font Gra
	switch(StructInfoPayment.CodeFunction)
	{
   	case VOIDCODE:
		j += sprintf(pPtr+j, "\n           ANNULATION");
		break;
	case REFUNDCODE:
		j += sprintf(pPtr+j, "\n                CREDIT");
		break;
	case PREAUTHCODE:
		//j += sprintf(pPtr+j, "\n"); //KI Le 190106
		break;
	case VOID_PREAUTO_TOTAL_CODE:
	case VOID_PREAUTO_PARTIEL_CODE:
		j += sprintf(pPtr+j, "\nANNULATION PRE-AUTO.");//Mouna310305
		break;
	default:
		if(StructInfoPayment.StatusAnnulAdjust)
			j += sprintf(pPtr+j, "\n           ANNULATION");
		else
			j += sprintf(pPtr+j, "\n                 DEBIT");
		break;
	}
	//j += sprintf(pPtr+j, "<#f=%08lx#>",(UL)SYSTEM_FONT_5x7_ID);//Font normal
	j += sprintf(pPtr+j, "\x1B""F");//Font Normal
	//Signature client
	switch(StructInfoPayment.CodeFunction)
	{
	case VOIDCODE://231205
	case VOID_PREAUTO_TOTAL_CODE:
	case VOID_PREAUTO_PARTIEL_CODE:
		break;

	default:
		if(StructInfoPayment.SignatureHolder==REQUIRED)
		{
   			if(source==CLIENT)//kamal251105
				j += sprintf(pPtr+j, "\n");
			else if(StructInfoPayment.CodeFunction == PREAUTHCODE) //CCD
				j += sprintf(pPtr+j, "\n     signature client\n\n");
			else
				j += sprintf(pPtr+j, "\n\n     signature client\n\n");
		}
		break;
	}
	//if(!dup)
	{
		j += sprintf(pPtr+j, "\n		%.24s",StructInfoPayment.Message2);
	}
	j += sprintf(pPtr+j, "\n------------------------");
	if(langue==ENG)
	{
		j += sprintf(pPtr+j, "\n		TICKET TO SAVE");
		if(source==CLIENT)
			j += sprintf(pPtr+j, "\n			Client Copy");
		else
			j += sprintf(pPtr+j, "\n	  	Merchant Copy");
	}
	else
	{
		j += sprintf(pPtr+j, "\n		TICKET A CONSERVER");
		if(source==CLIENT)
			j += sprintf(pPtr+j, "\n			Copie client");
		else
			j += sprintf(pPtr+j, "\n	  	Copie commercant");
	}
	status = imp_buffer(pPtr,j);
	if(status != OK)
	{
		return KO;
	}
	CUTERMpaperFeed();
/*	Os__xlinefeed(4);*/
	return OK;
}

/*-----------------------------------------------------------------
 * Fonction   :ImprimeCapture_carte
 * objet      :
 *
 * Parametre  :
 * Retour     :
 *-----------------------------------------------------------------
*/
uchar	ImprimeCapture_carte(uchar langue,uchar source)
{
	char		pPtr[1000];
	int		j=0;
	uchar		data_time[13];
	char		affich_Cs[11];
	uchar		Card_Nb[20];
	uchar		status;

	StructInfoPayment.waitpuce = 0;
	/*Aff_indicNO_OUI(FicPara.Langue);*/
	CUMORE_CMI_AFFICHE_MESSAGE(94,32,1000);
	//Aff_mess(FicPara.Langue,94,0,0,0,0);
	//Aff_mess(FicPara.Langue,32,1,0,0,0);

	memset(pPtr,0x00,sizeof(pPtr)) ;
	memset(affich_Cs,0x00,11) ;
	j=0 ;
	CUTERMpaperFeed();
	/*Os__xlinefeed(1);*/
	//j += sprintf(pPtr+j, "<#f=%08lx#><#c#>C.M.I",(UL)SYSTEM_FONT_5x7_ID);	//logo
	j += sprintf(pPtr+j, "\x1B""@");//Font Gras

	j += sprintf(pPtr+j, "\x1B""E            CENTRE\n");
	j += sprintf(pPtr+j, "\x1B""E          MONETIQUE\n");
	j += sprintf(pPtr+j, "\x1B""E       INTERBANCAIRE\n""\x1B""F");


	/*************************************************************************/
	memcpy(data_time,StructInfoPayment.Transaction_DateTime,12) ;			// date/heure
	j += sprintf(pPtr+j, "\n%.2s/%.2s/%.2s						%.2s:%.2s",data_time+4,data_time+2,data_time,data_time+6,data_time+8);
	/************************************************************************/
	j += sprintf(pPtr+j, "\n%.10s",FicPara.Num_point_vente);				//Num point vente
	j += sprintf(pPtr+j, "\n\n%.8s",FicPara.Num_Terminal);					//Num Terminal
	j += sprintf(pPtr+j, "\n%.24s",FicPara.Merchant_Name);					//Enseigne comerçant
	j += sprintf(pPtr+j, "\n%.13s",FicPara.Merchant_City);					//ville
	//j += sprintf(pPtr+j, "\n%.24s",StructInfoPayment.Nom_Prenom);    		//nom client
	if(StructInfoPayment.Appli_label[0]!=0)
	{
		j += sprintf(pPtr+j, "\n\n%s",StructInfoPayment.Appli_aid+1);
		j += sprintf(pPtr+j, "\n%.16s",StructInfoPayment.Appli_label);
		j += sprintf(pPtr+j, "\n%.24s",StructInfoPayment.Nom_Prenom);
	}
	else
		j += sprintf(pPtr+j, "\n\n%.24s",StructInfoPayment.Nom_Prenom);

	//Numéro de la carte
	memset(Card_Nb,0,20);
	memcpy(Card_Nb,&StructInfoPayment.Holder_Card[1],StructInfoPayment.Holder_Card[0]);
	if(source==CLIENT)//231205
	{
		Ushort ik;

		for(ik=0;ik<strlen((char*)Card_Nb)-4;ik++)
			Card_Nb[ik] = '*';
	}
	j += sprintf(pPtr+j, "\n%s",Card_Nb);    								//Num carte
	j += sprintf(pPtr+j, "\n%.2s/%.2s",StructInfoPayment.Expired_Date+2,StructInfoPayment.Expired_Date);    //Date validité

	memcpy(affich_Cs,StructInfoPayment.SeviceCode,3) ;
	//if(StructInfoPayment.forcage)
	if((StructInfoPayment.forcage)||(StructInfoPayment.Top_Forcage == TOP_FOR_REFFERAL))
		affich_Cs[3]='1';
	else
		affich_Cs[3]='0';
	/*********************************************************/
	memcpy(affich_Cs+4,&lop_gest.NumListe[0],4) ;

	if(StructInfoPayment.Entry_Mode==TRS_EMV)
		affich_Cs[8]='1';
	else if(StructInfoPayment.Entry_Mode==TRS_MAN)
		affich_Cs[8]='2';
	else
		affich_Cs[8]='0';


	sprintf(affich_Cs+9,"%.02d",FicInsReg.Tab_Ins_reg[StructInfoPayment.Type_carte].Code_Prefixe);
	j += sprintf(pPtr+j, "\n%.3s-%.1s-%.4s-%.1s-%.2s",affich_Cs,
		affich_Cs+3,affich_Cs+4,affich_Cs+8,affich_Cs+9);  //Code Service
	/*************************************************************/
	if(langue==ENG)
	{
		j += sprintf(pPtr+j, "\n\n\n		PICK UP CARD");
		j += sprintf(pPtr+j, "\n		THE BANK WANTED TO");
		j += sprintf(pPtr+j, "\n		RETIRE THE CARD FROM");
		j += sprintf(pPtr+j, "\n		HOLDER");
	}
	else
	{
		j += sprintf(pPtr+j, "\n\n\n	ATTENTION CARTE OPPOSEE");
		j += sprintf(pPtr+j, "\n		LA BANQUE EMETTRICE DE");
		j += sprintf(pPtr+j, "\n		LA CARTE A DEMANDE QU'");
		j += sprintf(pPtr+j, "\n		ELLE SOIT RETIREE AU");
		j += sprintf(pPtr+j, "\n		PORTEUR");
	}
	j += sprintf(pPtr+j, "\n------------------------");
	j += sprintf(pPtr+j, "\n		TICKET A CONSERVER");
	if(source==CLIENT)
		j += sprintf(pPtr+j, "\n		Copie client");
	else
		j += sprintf(pPtr+j, "\n		Copie commercant");

	//imp_buffer(pPtr,j) ;
	status = imp_buffer(pPtr,j);
	if(status != OK)
	{
		return KO;
	}
	CUTERMpaperFeed();
/*	Os__xlinefeed(4);*/
	return OK;
}

L	ShowTrace_TRC_TELEC(PC message)
{
return 0;
	Telium_File_t *printer ;
	printer = Telium_Fopen("PRINTER","w-");

	/* ##1 print the contents in condensed mode  */
	Telium_Fprintf(printer,"\x0f");
	Telium_Fprintf(printer,"\x0f""%s \n",message);
	/* ##1 print the reading status in bold */
	Telium_Fprintf(printer,"\x12""\n");
	/* ##1 wait for print end */
	Telium_Ttestall ( PRINTER , 300 );
	Telium_Fclose(printer);
	return 0;
}
L	ShowTrac_GPRS(PC message)
{
return 0;
	Telium_File_t *printer ;
	printer = Telium_Fopen("PRINTER","w-");

	/* ##1 print the contents in condensed mode  */
	Telium_Fprintf(printer,"\x0f");
	Telium_Fprintf(printer,"\x0f""%s \n",message);
	/* ##1 print the reading status in bold */
	Telium_Fprintf(printer,"\x12""\n");
	/* ##1 wait for print end */
	Telium_Ttestall ( PRINTER , 300 );
	Telium_Fclose(printer);
	return 0;
}
L	ShowTrac_GP(PC message)
{
return 0;
	Telium_File_t *printer ;
	printer = Telium_Fopen("PRINTER","w-");

	/* ##1 print the contents in condensed mode  */
	Telium_Fprintf(printer,"\x0f");
	Telium_Fprintf(printer,"\x0f""%s \n",message);
	/* ##1 print the reading status in bold */
	Telium_Fprintf(printer,"\x12""\n");
	/* ##1 wait for print end */
	Telium_Ttestall ( PRINTER , 300 );
	Telium_Fclose(printer);
	return 0;
}

/*****************/
L	ShowTrace_Mo(PC message)
{
return 0;
	Telium_File_t *printer ;
	printer = Telium_Fopen("PRINTER","w-");

	/* ##1 print the contents in condensed mode  */
	Telium_Fprintf(printer,"\x0f");
	Telium_Fprintf(printer,"\x0f""%s \n",message);
	/* ##1 print the reading status in bold */
	Telium_Fprintf(printer,"\x12""\n");
	/* ##1 wait for print end */
	Telium_Ttestall ( PRINTER , 300 );
	Telium_Fclose(printer);
	return 0;
}
/*****************/
L	ShowTrace_Mouad(PC message)
{
return 0;
	Telium_File_t *printer ;
	printer = Telium_Fopen("PRINTER","w-");

	/* ##1 print the contents in condensed mode  */
	Telium_Fprintf(printer,"\x0f");
	Telium_Fprintf(printer,"\x0f""%s \n",message);
	/* ##1 print the reading status in bold */
	Telium_Fprintf(printer,"\x12""\n");
	/* ##1 wait for print end */
	Telium_Ttestall ( PRINTER , 300 );
	Telium_Fclose(printer);
	return 0;
}

/*****************/
L	ShowTrace_ib(PC message)
{
return 0;
	Telium_File_t *printer ;
	printer = Telium_Fopen("PRINTER","w-");

	/* ##1 print the contents in condensed mode  */
	Telium_Fprintf(printer,"\x0f");
	Telium_Fprintf(printer,"\x0f""%s \n",message);
	/* ##1 print the reading status in bold */
	Telium_Fprintf(printer,"\x12""\n");
	/* ##1 wait for print end */
	Telium_Ttestall ( PRINTER , 300 );
	Telium_Fclose(printer);
	return 0;
}

void eft30printhexMouad(char* buffer,Ushort n_car)
{
}

void eft30printhexGPRS(char* buffer,Ushort n_car)
{
}

void eft30printhex_TELEC(char* buffer,Ushort n_car)
{

}

void eft30printhexGP(char* buffer,Ushort n_car)
{
}
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			NOM			:eft30printhex
//			DESCRIPTION	:impression ticket en hexa.
//
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
void eft30printhex(char* buffer,Ushort n_car)
{
/*	Telium_File_t *printer1;
	ushort c;

	printer1 = Telium_Fopen("PRINTER","w-");
	if ( printer1 != NULL )
	{
		Telium_Fprintf(printer1,"\x0f");
		for(c =0;c<n_car;c++)
		{
			Telium_Fprintf(printer1,"%02X",buffer[c]);
			if(c!=0)
			{
				if(!(c%20))
					Telium_Fprintf(printer1,"\n");
			}
		}
		Telium_Fprintf(printer1,"\x1B""E""\n");
		Telium_Ttestall ( PRINTER , 300 );
		Telium_Fclose(printer1);
	}
*/
}
void eft30printhexKamal(char* buffer,Ushort n_car)
{
/*
	Telium_File_t *printer1;
	Ushort c;

	printer1 = Telium_Fopen("PRINTER","w-");
	if ( printer1 != NULL )
	{
		Telium_Fprintf(printer1,"\x0f");
		for(c =0;c<n_car;c++)
		{
			Telium_Fprintf(printer1,"%02X",buffer[c]);
			if(c!=0)
			{
				if(!(c%20))
					Telium_Fprintf(printer1,"\n");
			}
		}
		Telium_Fprintf(printer1,"\x1B""E""\n");
		Telium_Ttestall ( PRINTER , 300 );
		Telium_Fclose(printer1);
	}
*/
}

L	ShowTrace_Mouna(PC message)
{
return 0;
	Telium_File_t *printer ;
	printer = Telium_Fopen("PRINTER","w-");

	/* ##1 print the contents in condensed mode  */
	Telium_Fprintf(printer,"\x0f");
	Telium_Fprintf(printer,"\x0f""%s \n",message);
	/* ##1 print the reading status in bold */
	Telium_Fprintf(printer,"\x12""\n");
	/* ##1 wait for print end */
	Telium_Ttestall ( PRINTER , 300 );
	Telium_Fclose(printer);
	return 0;
}

/*-----------------------------------------------------------------             
 * Fonction   :ImprimeTelecollecte
 * objet      :
 *             
 * Parametre  :
 * Retour     :
 *-----------------------------------------------------------------             
*/
uchar	ImprimeTelecollecte(uchar ind_rec,uchar tel_etat)
{
	char	pPtr[600];
	
	Ushort		j=0;

	uchar	montant[13]  ;
	uchar	affiche_mont[14];
	uchar	status;
	DATE date;
	
	StructInfoPayment.waitpuce = 0;
	/*Aff_indicNO_OUI(FicPara.Langue);*/
	CUMORE_CMI_AFFICHE_MESSAGE(94,32,1000);
	//Aff_mess(FicPara.Langue,94,0,0,0,0);
	//Aff_mess(FicPara.Langue,32,1,0,0,0);
	
	memset(pPtr,0x00,sizeof(pPtr)) ;
	memset(montant,0x00,13) ;
	memset(affiche_mont,0x00,14) ;
	j=0 ;

	Telium_Read_date(&date);
	/*Os__read_date(data_time);
	Os__read_time_sec (data_time+6) ;*/

	CUTERMpaperFeed();
	/*Os__xlinefeed(1);*/
	/*j += sprintf(pPtr+j, "<#f=%08lx#><#c#>C.M.I",(UL)SYSTEM_FONT_8x14_ID);//Font Gras*/
	j += sprintf(pPtr+j, "\x1B""@");//Font Normal

	j += sprintf(pPtr+j, "\x1B""E            CENTRE\n");
	j += sprintf(pPtr+j, "\x1B""E          MONETIQUE\n");
	j += sprintf(pPtr+j, "\x1B""E       INTERBANCAIRE\n");

	if((ind_rec == 0)||(ind_rec == 1))
		j += sprintf(pPtr+j, "\n			FIN DE JOURNEE");
	else
		j += sprintf(pPtr+j, "\n                 TOTAL");
	/*j += sprintf(pPtr+j, "<#f=%08lx#>",(UL)SYSTEM_FONT_5x7_ID);//Font Normal*/
	j += sprintf(pPtr+j, "\x1B""F");//Font Normal
	j += sprintf(pPtr+j,"\n%.2s/%.2s/%.2s						%.2s:%.2s:%.2s",
		date.day,
		date.month,
		date.year,
		date.hour,
		date.minute,
		date.second);
	/*j += sprintf(pPtr+j,"\n%.2s/%.2s/%.2s<#r#>%.2s:%.2s:%.2s",
		data_time,data_time+2,data_time+4,data_time+6,data_time+8,data_time+10);*/
	j += sprintf(pPtr+j, "\n%.10s",FicPara.Num_point_vente);   //Num point vente
	j += sprintf(pPtr+j, "\n%.8s",FicPara.Num_Terminal);       //Num Terminal
	j += sprintf(pPtr+j, "\n%.24s",FicPara.Merchant_Name);     //Enseigne commercial
	j += sprintf(pPtr+j, "\n%.13s",FicPara.Merchant_City);     //Ville
	Calcul_total(&j,pPtr,ind_rec);

	if((ind_rec == 0)||(ind_rec == 1))
	{
		if(ind_rec == 0)
			j += sprintf(pPtr+j, "\n\nReconciliation Manuelle");
		else
			j += sprintf(pPtr+j, "\n\nReconciliation Auto");

		if(tel_etat == RECO_OK)
			j += sprintf(pPtr+j, "\nSucces Reconciliation");
		else if(tel_etat == RECO_KO)
			j += sprintf(pPtr+j, "\nEchec Reconciliation");
		else if(tel_etat == TEL_OK)
		{
			j += sprintf(pPtr+j, "\nEchec Reconciliation");
			j += sprintf(pPtr+j, "\nSucces Telecollecte");
		}

		j += sprintf(pPtr+j, "\n\n		%.24s",StructInfoPayment.Message2);
		j += sprintf(pPtr+j, "\n\n------------------------");
	}

	/*j += sprintf(pPtr+j, "<#f=%08lx#>",(UL)SYSTEM_FONT_5x7_ID);//Font Normal*/
	j += sprintf(pPtr+j, "\x1B""F");//Font Normal
	if(FicPara.Langue==ENG)
	{
		j += sprintf(pPtr+j, "\n		TICKET TO SAVE");
		j += sprintf(pPtr+j, "\n		 Merchant Copy");
	}
	else
	{
		j += sprintf(pPtr+j, "\n		TICKET A CONSERVER");
		j += sprintf(pPtr+j, "\n		 Copie commercant");
	}

	status = imp_buffer(pPtr,j);
	if(status != OK)
	{
		return KO;
	}

	CUTERMpaperFeed();
/*	Os__xlinefeed(4);*/
	return OK;
}

/*-----------------------------------------------------------------             
 * Fonction   :JournalEdit
 * objet      :
 *             
 * Parametre  :
 * Retour     :
 *-----------------------------------------------------------------             
*/
uchar	JournalEdit(void)
{
	char		pPtr[1000];
	Ushort 		i,k;
	Ushort		nombre_tra;
	int		j=0;
	uchar		trouve = KO;
	uchar		data_time[13];
	uchar	  	montant[13];
	uchar		affiche_mont[13];
	uchar		Holder_Card[20];
	uchar    	iso2[38];
	uchar		l;
	lignetra	transac ;
	uchar		Exp;
	DATE date;


	StructInfoPayment.waitpuce = 0;
	/*Aff_indicNO_OUI(FicPara.Langue);*/
	CUMORE_CMI_AFFICHE_MESSAGE(94,32,1000);
	//Aff_mess(FicPara.Langue,94,0,0,0,0);
	//Aff_mess(FicPara.Langue,32,1,0,0,0);

	memset(pPtr,0x00,sizeof(pPtr)) ;
	memset(data_time,0,13);
	memset(iso2,0,38);

	Telium_Read_date(&date);
	/*Os__read_date(data_time);
	Os__read_time_sec (data_time+6) ;*/

	//	Header  //
	j=0;
	/*j += sprintf(pPtr+j, "<#f=%08lx#>",(UL)SYSTEM_FONT_8x14_ID);//Font Gras*/
	j += sprintf(pPtr+j, "\x1B""@");//Font Normal
	j += sprintf(pPtr+j, "\x1B""E");//Font Gras

	j += sprintf(pPtr+j, "\x1B""E            CENTRE\n");
	j += sprintf(pPtr+j, "\x1B""E          MONETIQUE\n");
	j += sprintf(pPtr+j, "\x1B""E       INTERBANCAIRE\n""\x1B""F");

	j += sprintf(pPtr+j, "\n           JOURNAL");
	/*j += sprintf(pPtr+j, "<#f=%08lx#>",(UL)SYSTEM_FONT_5x7_ID);//Font Normal*/
	j += sprintf(pPtr+j, "\x1B""F");//Font Normal
	/*j += sprintf(pPtr+j,"\n%.2s/%.2s/%.2s<#r#>%.2s:%.2s:%.2s",data_time,data_time+2,data_time+4,data_time+6,data_time+8,data_time+10);*/
	j += sprintf(pPtr+j,"\n%.2s/%.2s/%.2s						%.2s:%.2s:%.2s",
		date.day,
		date.month,
		date.year,
		date.hour,
		date.minute,
		date.second);
	j += sprintf(pPtr+j, "\n%.15s",FicPara.Num_point_vente);
	j += sprintf(pPtr+j, "\n%.8s",FicPara.Num_Terminal);
	j += sprintf(pPtr+j, "\n%.24s",FicPara.Merchant_Name);
	j += sprintf(pPtr+j, "\n%.13s",FicPara.Merchant_City);
	

	nombre_tra = FicTra.nombre_tra;
	if(nombre_tra==0)
	{
		j += sprintf(pPtr+j, "\n		NO TRANSACTIONS");
		if(imp_buffer(pPtr,j)!=OK)
		{
			return KO ;
		}
		CUTERMpaperFeed();
		/*Os__xlinefeed(4);*/
		return OK ;
	}

//****************************//
	if(imp_buffer(pPtr,j)!=OK)
	{
		Os__display(0,0,"Pb. Impression");
		Os__xget_key(5);
		CUTERMpaperFeed();
		return KO;
	}
	j=0;
	memset(pPtr,0,sizeof(pPtr));
//****************************//

	i=0 ;
	//j += sprintf(pPtr+j, "\x0f");
	while(i<nombre_tra)
	{
		trouve=OK ;
		read_trans_index( i ,&transac) ;
		eft30printhex((char*)&transac,sizeof(transac)-250);
		memset(Holder_Card,0,20);      
		/*j += sprintf(pPtr+j, "<#f=%08lx#>",(UL)SYSTEM_FONT_5x7_ID);*/
		//j += sprintf(pPtr+j, "\x1B""F");//Font Normal
 		//Num Carte
		if(transac.Entry_Mode == TRS_MAN)
 		{
 			memcpy(Holder_Card,&transac.Compressed_Iso2[1],19);
 		}
 		else
 		{
			/******/
/*			ShowTrace_Mouna("ad1");
			imp_buffer(transac.Compressed_Iso2,transac.Compressed_Iso2[0]);*/
			/******/
			bcd_asc(iso2,&transac.Compressed_Iso2[1],20);
			/******/
/*			imp_buffer(iso2,20);*/
			/******/
   			for(l=0;((l<20)&&(iso2[l] !='d')&&(iso2[l] !='D')&&(iso2[l] !='='));l++);
   			memcpy(Holder_Card,iso2,l);
		}
		j += sprintf(pPtr+j, "\n\n%.19s",Holder_Card);
		//JJ/MM/AA	HH:MM:SS	STAN
		memcpy(data_time,transac.Transaction_DateTime,12);
		j += sprintf(pPtr+j, "\n%.2s/%.2s/%.2s						%.2s:%.2s:%.2s\n%.6s",data_time+4,data_time+2,data_time,data_time+6,data_time+8,data_time+10,transac.STAN);
		//MONTANT
		memset(montant,0,13);
		memset(affiche_mont,0,13);
		long_asc (montant, 12, &transac.Transaction_Amount);
		RemoveLeftZeros(montant) ;
		Exp=transac.Exposant-48;
		FormatAmount(montant,affiche_mont,Exp);
		for(k=0;k<NB_MAX_DEV_ACC;k++)            //l'indice de la 2éme dim
		{
		if(!memcmp(Fic_Devise_Acc.TabDevise[k].code_num,transac.NumCurrency,3))
			break;
		}
		j += sprintf(pPtr+j, "    %.6s    ",transac.Num_sequence);
		switch(transac.CodeFunction)
		{
       	case SALECODE:            
        case CASHADVANCESCODE:            
			if(transac.Flag_Dem_Aut)
				j += sprintf(pPtr+j, "DO\n");
			else
				j += sprintf(pPtr+j, "DF\n");
            break;
			
		case REFUNDCODE:            
            j += sprintf(pPtr+j, "CF\n");
            break;
			
		case VOIDCODE:
			if(transac.Flag_Dem_Aut)
				j += sprintf(pPtr+j, "AO\n");
			else
				j += sprintf(pPtr+j, "AF\n");
            break;
			
		case PREAUTHCOMPCODE:
            if(transac.Preauto_indic_completion == PRE_VAL_1_TRS)
				j += sprintf(pPtr+j, "PF\n");
            else
				if(transac.Top_Forcage == TOP_FOR_ONLINE)
					j += sprintf(pPtr+j, "PO\n");
				else
					j += sprintf(pPtr+j, "PF\n");
			  break;
		}
		j += sprintf(pPtr+j, "%13s\t%.3s",affiche_mont,Fic_Devise_Acc.TabDevise[k].code_alph);

//	memset(size_carte,'\0',3);
//	sprintf(size_carte,"%d",j);
//	ShowTrace_Mouna(size_carte);
   	//	if (j >= 700)
		{
			if(imp_buffer(pPtr,j)!=OK)
			{
				//j += sprintf(pPtr+j, "\x12");
      			return KO ;
			}
      		trouve = KO;
      		j = 0;
      		memset(pPtr,0,sizeof(pPtr));
		}
  		i++ ;
	}
	//j += sprintf(pPtr+j, "\x12");
	if (trouve==OK)
	{
		if(imp_buffer(pPtr,j)!=OK)
		{
			return KO ;
		}
	}
	CUTERMpaperFeed();
/*    Os__xlinefeed(4);*/
    return OK ;
}

/*-----------------------------------------------------------------             
 * Fonction   :JournalEditPre
 * objet      :
 *             
 * Parametre  :
 * Retour     :
 *-----------------------------------------------------------------             
*/
uchar	JournalEditPre(void)
{
	char		pPtr[1000];
	Ushort 		i,k;
	Ushort		nbr_preauto;
	int		j=0;
	uchar		trouve = KO;
	uchar		data_time[13];
	uchar	  	montant[13];
	uchar		affiche_mont[13];
	uchar		affiche_mont_accomp[13];
	uchar		Holder_Card[20];
	uchar    	iso2[38];
	uchar		l;
	lignePreAut	preauto ;
	uchar		Exp;
	DATE date;

	StructInfoPayment.waitpuce = 0;
	/*Aff_indicNO_OUI(FicPara.Langue);*/
	CUMORE_CMI_AFFICHE_MESSAGE(94,32,1000);
	//Aff_mess(FicPara.Langue,94,0,0,0,0);
	//Aff_mess(FicPara.Langue,32,1,0,0,0);

	memset(pPtr,0x00,sizeof(pPtr)) ;
	memset(data_time,0,13);
	memset(iso2,0,38);

	Telium_Read_date(&date);
	/*Os__read_date(data_time);
	Os__read_time_sec (data_time+6) ;*/

	//	Header  //
	j=0;
	/*j += sprintf(pPtr+j, "<#f=%08lx#>",(UL)SYSTEM_FONT_8x14_ID);//Font Gras*/
	j += sprintf(pPtr+j, "\x1B""@");//Font Normal
	j += sprintf(pPtr+j, "\x1B""E");//Font Gras

	j += sprintf(pPtr+j, "\x1B""E            CENTRE\n");
	j += sprintf(pPtr+j, "\x1B""E          MONETIQUE\n");
	j += sprintf(pPtr+j, "\x1B""E       INTERBANCAIRE\n");

	j += sprintf(pPtr+j, "\n   JOURNAL PRE AUTO.");//Mouna310305
	/*j += sprintf(pPtr+j, "<#f=%08lx#>",(UL)SYSTEM_FONT_5x7_ID);//Font Normal*/
	j += sprintf(pPtr+j, "\x1B""F");//Font Normal
	/*j += sprintf(pPtr+j,"\n%.2s/%.2s/%.2s<#r#>%.2s:%.2s:%.2s",data_time,data_time+2,data_time+4,data_time+6,data_time+8,data_time+10);*/
	j += sprintf(pPtr+j,"\n%.2s/%.2s/%.2s						%.2s:%.2s:%.2s",
		date.day,
		date.month,
		date.year,
		date.hour,
		date.minute,
		date.second);
	j += sprintf(pPtr+j, "\n%.15s",FicPara.Num_point_vente);
	j += sprintf(pPtr+j, "\n%.8s",FicPara.Num_Terminal);
	j += sprintf(pPtr+j, "\n%.24s",FicPara.Merchant_Name);
	j += sprintf(pPtr+j, "\n%.13s",FicPara.Merchant_City);
	

	nbr_preauto = FicPreauth.nombre_tra ;
	if(nbr_preauto==0)
	{
		j += sprintf(pPtr+j, "\n\n\n\n\n		NO PRE-AUTORISATION");
		if(imp_buffer(pPtr,j)!=OK)
		{
			return KO ;
		}
		CUTERMpaperFeed();
		/*Os__xlinefeed(4);*/
		return OK ;
	}
	i=0 ;
	//j += sprintf(pPtr+j, "\x0f");
	while(i<nbr_preauto)
	{
		trouve=OK ;
		read_preauto_index( i ,&preauto) ;
		memset(Holder_Card,0,20);      
		/*j += sprintf(pPtr+j, "<#f=%08lx#>",(UL)SYSTEM_FONT_5x7_ID);*/
		//j += sprintf(pPtr+j, "\x1B""F");//Font Normal
 		//Num Carte			Num Sequ
		if(preauto.Entry_Mode == TRS_MAN)
 		{
 			memcpy(Holder_Card,&preauto.Compressed_Iso2[1],19);
 		}
 		else
 		{
			/******/
/*			ShowTrace_Mouna("ad2");
			imp_buffer(preauto.Compressed_Iso2,preauto.Compressed_Iso2[0]);*/
			/******/
			bcd_asc(iso2,&preauto.Compressed_Iso2[1],20);
		 	/******/
/*			imp_buffer(iso2,20);*/
			/******/
  			for(l=0;((l<20)&&(iso2[l] !='d')&&(iso2[l] !='D')&&(iso2[l] !='='));l++);
   			memcpy(Holder_Card,iso2,l);
		}
		//j += sprintf(pPtr+j, "\n\n%.19s\t%.6s		 ",Holder_Card,preauto.Num_sequence);
		j += sprintf(pPtr+j, "\n\n%.19s",Holder_Card);
		//JJ/MM/AA	HH:MM:SS	STAN
		memcpy(data_time,preauto.Transaction_DateTime,12);
		j += sprintf(pPtr+j, "\n%.2s/%.2s/%.2s						%.2s:%.2s:%.2s\n",data_time+4,data_time+2,data_time,data_time+6,data_time+8,data_time+10);
		j += sprintf(pPtr+j, "%.6s    %.6s    ",preauto.STAN,preauto.Num_sequence);
		//MONTANT PRE AUTORI			MONTANT ACCOMPLI
		memset(montant,0,13);
		
		memset(affiche_mont,0,13);
		long_asc (montant, 12, &preauto.Transaction_Amount);
		RemoveLeftZeros(montant) ;
		Exp=preauto.Exposant-48;
		FormatAmount(montant,affiche_mont,Exp);

		memset(montant,0,13);
		memset(affiche_mont_accomp,0,13);
		long_asc (montant, 12, &preauto.Montant_Accomp);
		RemoveLeftZeros(montant) ;
		Exp=preauto.Exposant-48;
		FormatAmount(montant,affiche_mont_accomp,Exp);

		for(k=0;k<NB_MAX_DEV_ACC;k++)            //l'indice de la 2éme dim
		{
		if(!memcmp(Fic_Devise_Acc.TabDevise[k].code_num,preauto.NumCurrency,3))
			break;
		}
		j += sprintf(pPtr+j, "\n%13s\t%.3s",affiche_mont,Fic_Devise_Acc.TabDevise[k].code_alph);
		j += sprintf(pPtr+j, "\n%13s\t%.3s",affiche_mont_accomp,Fic_Devise_Acc.TabDevise[k].code_alph);

   		//if (j >= 800)
		{
			if(imp_buffer(pPtr,j)!=OK)
			{
			 	//j += sprintf(pPtr+j, "\x12");
     			return KO ;
			}
      		trouve = KO;
      		j = 0;
      		memset(pPtr,0,sizeof(pPtr));
		}
  		i++ ;
	}
	//j += sprintf(pPtr+j, "\x12");
	if (trouve==OK)
	{
		if(imp_buffer(pPtr,j)!=OK)
		{
			return KO ;
		}
	}
	CUTERMpaperFeed();
/*    Os__xlinefeed(4);*/
    return OK ;
}

/*-----------------------------------------------------------------             
 * Fonction   :Print_Last_Trs_Emv_Rej
 * objet      :
 *             
 * Parametre  :
 * Retour     :
 *-----------------------------------------------------------------             
*/
//Os__xprint
L	Print_Last_Trs_Emv_Rej()
{
	int		tprint=0;
	C		Buffer[2048],para;
	uchar	status;

	
	uint16  i;

	uint8  BufferPrint[50],BufferScript[255];
	uint16 Index_Tab;
	uint8	j,chaine[25];
	
	StructInfoPayment.waitpuce = 0;
	/*Aff_indicNO_OUI(FicPara.Langue);*/
	CUMORE_CMI_AFFICHE_MESSAGE(94,32,1000);
	//Aff_mess(FicPara.Langue,94,0,0,0,0);
	//Aff_mess(FicPara.Langue,32,1,0,0,0);

	
	
	CUTERMpaperFeed();
/*	Os__xlinefeed(1);*/
	memset(Buffer,0,sizeof(Buffer));tprint=0;
	
	tprint += sprintf(Buffer+tprint,"\x1B""@");//Font Normal

	tprint += sprintf(Buffer+tprint, "\x1B""E            CENTRE\n");
	tprint += sprintf(Buffer+tprint, "\x1B""E          MONETIQUE\n");
	tprint += sprintf(Buffer+tprint, "\x1B""E       INTERBANCAIRE\n");

	tprint += sprintf(Buffer+tprint, "\nDATA LAST TRS. REJECTED""\x1B""F");
	tprint += sprintf(Buffer+tprint, "\n");

	/* //Fermer17
	if ((FindTag (SEMV_TAG_0082_AIP, &Offset, &Length, Data_Last_TRS_Emv_Rej.Champ55, Data_Last_TRS_Emv_Rej.l_champ55) == OK) &&
		(Length == 2))
	{
		memset(BufferPrint,0,sizeof(BufferPrint));
		memcpy(BufferPrint,"\n0082: ",6);
		para=2*Length;
		hex_asc(&BufferPrint[6],&Data_Last_TRS_Emv_Rej.Champ55[Offset],para);
		tprint += sprintf(Buffer+tprint,(PC)BufferPrint);
		
	}
	else
		tprint += sprintf(Buffer+tprint,"\nMissing TAG_AppIntPro");

	
	if(FindTag (SEMV_TAG_0084_AID, &Offset, &Length, Data_Last_TRS_Emv_Rej.Champ55, Data_Last_TRS_Emv_Rej.l_champ55) == OK)
	{
		memset(BufferPrint,0,sizeof(BufferPrint));
		memcpy(BufferPrint,"\n0084: ",6);
		para=2*Length;
		hex_asc(&BufferPrint[6],&Data_Last_TRS_Emv_Rej.Champ55[Offset],para);
		tprint += sprintf(Buffer+tprint,(PC)BufferPrint);
	}
	else
		tprint += sprintf(Buffer+tprint,"\nMissing TAG_DFName");
	
	if(FindTag (SEMV_TAG_0095_TVR, &Offset, &Length, Data_Last_TRS_Emv_Rej.Champ55, Data_Last_TRS_Emv_Rej.l_champ55) == OK)
	{
		memset(BufferPrint,0,sizeof(BufferPrint));
		memcpy(BufferPrint,"\n0095: ",6);
		para=2*Length;
		hex_asc(&BufferPrint[6],&Data_Last_TRS_Emv_Rej.Champ55[Offset],para);
		tprint += sprintf(Buffer+tprint,(PC)BufferPrint);
	}
	else
		tprint += sprintf(Buffer+tprint,"\nMissing TAG_TerVerRes");
	
	if(FindTag (SEMV_TAG_9F0D_IAC_DEFAULT, &Offset, &Length, Data_Last_TRS_Emv_Rej.Champ55, Data_Last_TRS_Emv_Rej.l_champ55) == OK)
	{
		memset(BufferPrint,0,sizeof(BufferPrint));
		memcpy(BufferPrint,"\n9F0D: ",6);
		para=2*Length;
		hex_asc(&BufferPrint[6],&Data_Last_TRS_Emv_Rej.Champ55[Offset],para);
		tprint += sprintf(Buffer+tprint,(PC)BufferPrint);
	}
	
	if(FindTag (SEMV_TAG_9F0E_IAC_DENIAL, &Offset, &Length, Data_Last_TRS_Emv_Rej.Champ55, Data_Last_TRS_Emv_Rej.l_champ55) == OK)
	{
		memset(BufferPrint,0,sizeof(BufferPrint));
		memcpy(BufferPrint,"\n9F0E: ",6);
		para=2*Length;
		hex_asc(&BufferPrint[6],&Data_Last_TRS_Emv_Rej.Champ55[Offset],para);
		tprint += sprintf(Buffer+tprint,(PC)BufferPrint);
	}
	
	if(FindTag (SEMV_TAG_9F0F_IAC_ONLINE, &Offset, &Length, Data_Last_TRS_Emv_Rej.Champ55, Data_Last_TRS_Emv_Rej.l_champ55) == OK)
	{
		memset(BufferPrint,0,sizeof(BufferPrint));
		memcpy(BufferPrint,"\n9F0F: ",6);
		para=2*Length;
		hex_asc(&BufferPrint[6],&Data_Last_TRS_Emv_Rej.Champ55[Offset],para);
		tprint += sprintf(Buffer+tprint,(PC)BufferPrint);
	}
	
	
	if(FindTag (SEMV_TAG_009A_TRANSAC_DATE, &Offset, &Length, Data_Last_TRS_Emv_Rej.Champ55, Data_Last_TRS_Emv_Rej.l_champ55) == OK)
	{
		memset(BufferPrint,0,sizeof(BufferPrint));
		memcpy(BufferPrint,"\n009A: ",6);
		para=2*Length;
		hex_asc(&BufferPrint[6],&Data_Last_TRS_Emv_Rej.Champ55[Offset],para);
		tprint += sprintf(Buffer+tprint,(PC)BufferPrint);
	}
	

	if(FindTag (SEMV_TAG_9F21_TRANSAC_TIME, &Offset, &Length, Data_Last_TRS_Emv_Rej.Champ55, Data_Last_TRS_Emv_Rej.l_champ55) == OK)
	{
   	memset(BufferPrint,0,sizeof(BufferPrint));
	memcpy(BufferPrint,"\n9F21: ",6);
	para=2*Length;
   	hex_asc(&BufferPrint[6],&Data_Last_TRS_Emv_Rej.Champ55[Offset],para);
	tprint += sprintf(Buffer+tprint,(PC)BufferPrint);
	}

	if(FindTag (SEMV_TAG_009C_TRANSAC_TYPE, &Offset, &Length, Data_Last_TRS_Emv_Rej.Champ55, Data_Last_TRS_Emv_Rej.l_champ55) == OK)
	{
		memset(BufferPrint,0,sizeof(BufferPrint));
		memcpy(BufferPrint,"\n009C: ",6);
		para=2*Length;
		hex_asc(&BufferPrint[6],&Data_Last_TRS_Emv_Rej.Champ55[Offset],para);
		tprint += sprintf(Buffer+tprint,(PC)BufferPrint);
	}
	
	if(FindTag (SEMV_TAG_9F02_AMOUNT_AUTH_NUMERIC, &Offset, &Length, Data_Last_TRS_Emv_Rej.Champ55, Data_Last_TRS_Emv_Rej.l_champ55) == OK)
	{
		memset(BufferPrint,0,sizeof(BufferPrint));
		memcpy(BufferPrint,"\n9F02: ",6);
		para=2*Length;
		hex_asc(&BufferPrint[6],&Data_Last_TRS_Emv_Rej.Champ55[Offset],para);
		tprint += sprintf(Buffer+tprint,(PC)BufferPrint);
	}
	
	if(FindTag (SEMV_TAG_5F2A_TRANSAC_CUR_CODE, &Offset, &Length, Data_Last_TRS_Emv_Rej.Champ55, Data_Last_TRS_Emv_Rej.l_champ55) == OK)
	{
		memset(BufferPrint,0,sizeof(BufferPrint));
		memcpy(BufferPrint,"\n5F2A: ",6);
		para=2*Length;
		hex_asc(&BufferPrint[6],&Data_Last_TRS_Emv_Rej.Champ55[Offset],para);
		tprint += sprintf(Buffer+tprint,(PC)BufferPrint);
	}
	
	if(FindTag (SEMV_TAG_9F09_TERM_APPLI_VERS_NUMB, &Offset, &Length, Data_Last_TRS_Emv_Rej.Champ55, Data_Last_TRS_Emv_Rej.l_champ55) == OK)
	{
		memset(BufferPrint,0,sizeof(BufferPrint));
		memcpy(BufferPrint,"\n9F09: ",6);
		para=2*Length;
		hex_asc(&BufferPrint[6],&Data_Last_TRS_Emv_Rej.Champ55[Offset],para);
		tprint += sprintf(Buffer+tprint,(PC)BufferPrint);
	}
	
	if(FindTag (SEMV_TAG_9F10_ISSUER_APPLI_DATA, &Offset, &Length, Data_Last_TRS_Emv_Rej.Champ55, Data_Last_TRS_Emv_Rej.l_champ55) == OK)
	{
		memset(BufferPrint,0,sizeof(BufferPrint));
		memcpy(BufferPrint,"\n9F10: ",6);
		para=2*Length;
		hex_asc(&BufferPrint[6],&Data_Last_TRS_Emv_Rej.Champ55[Offset],para);
		tprint += sprintf(Buffer+tprint,(PC)BufferPrint);
	}
	
	if(FindTag (SEMV_TAG_9F1A_TERM_COUNTRY_CODE, &Offset, &Length, Data_Last_TRS_Emv_Rej.Champ55, Data_Last_TRS_Emv_Rej.l_champ55) == OK)
	{
		memset(BufferPrint,0,sizeof(BufferPrint));
		memcpy(BufferPrint,"\n9F1A: ",6);
		para=2*Length;
		hex_asc(&BufferPrint[6],&Data_Last_TRS_Emv_Rej.Champ55[Offset],para);
		tprint += sprintf(Buffer+tprint,(PC)BufferPrint);
	}
	
	if(FindTag (SEMV_TAG_0057_TRACK_2_EQU_DATA, &Offset, &Length, Data_Last_TRS_Emv_Rej.Champ55, Data_Last_TRS_Emv_Rej.l_champ55) == OK)
	{
		memset(BufferPrint,0,sizeof(BufferPrint));
		memcpy(BufferPrint,"\n0057: ",6);
		para=2*Length;
		hex_asc(&BufferPrint[6],&Data_Last_TRS_Emv_Rej.Champ55[Offset],para);
		tprint += sprintf(Buffer+tprint,(PC)BufferPrint);
	}
	

	if(FindTag (SEMV_TAG_005A_APPLICATION_PAN, &Offset, &Length, Data_Last_TRS_Emv_Rej.Champ55, Data_Last_TRS_Emv_Rej.l_champ55) == OK)
	{
   	memset(BufferPrint,0,sizeof(BufferPrint));
	memcpy(BufferPrint,"\n005A: ",6);
	para=2*Length;
   	hex_asc(&BufferPrint[6],&Data_Last_TRS_Emv_Rej.Champ55[Offset],para);
	tprint += sprintf(Buffer+tprint,(PC)BufferPrint);
	}
	
	if(FindTag (SEMV_TAG_5F34_APPLI_PAN_SEQ_NUMBER, &Offset, &Length, Data_Last_TRS_Emv_Rej.Champ55, Data_Last_TRS_Emv_Rej.l_champ55) == OK)
	{
		memset(BufferPrint,0,sizeof(BufferPrint));
		memcpy(BufferPrint,"\n5F34: ",6);
		para=2*Length;
		hex_asc(&BufferPrint[6],&Data_Last_TRS_Emv_Rej.Champ55[Offset],para);
		tprint += sprintf(Buffer+tprint,(PC)BufferPrint);
	}
	
	if(FindTag (SEMV_TAG_5F24_APPLI_EXPIR_DATE, &Offset, &Length, Data_Last_TRS_Emv_Rej.Champ55, Data_Last_TRS_Emv_Rej.l_champ55) == OK)
	{
		memset(BufferPrint,0,sizeof(BufferPrint));
		memcpy(BufferPrint,"\n5F24: ",6);
		para=2*Length;
		hex_asc(&BufferPrint[6],&Data_Last_TRS_Emv_Rej.Champ55[Offset],para);
		tprint += sprintf(Buffer+tprint,(PC)BufferPrint);
	}
	
	if(FindTag (SEMV_TAG_5F30_SERVICE_CODE, &Offset, &Length, Data_Last_TRS_Emv_Rej.Champ55, Data_Last_TRS_Emv_Rej.l_champ55) == OK)
	{
		memset(BufferPrint,0,sizeof(BufferPrint));
		memcpy(BufferPrint,"\n5F30: ",6);
		para=2*Length;
		hex_asc(&BufferPrint[6],&Data_Last_TRS_Emv_Rej.Champ55[Offset],para);
		tprint += sprintf(Buffer+tprint,(PC)BufferPrint);
	}

	if(FindTag (SEMV_TAG_9F26_APPLI_CRYPTO, &Offset, &Length, Data_Last_TRS_Emv_Rej.Champ55, Data_Last_TRS_Emv_Rej.l_champ55) == OK)
	{
		memset(BufferPrint,0,sizeof(BufferPrint));
		memcpy(BufferPrint,"\n9F26: ",6);
		para=2*Length;
		hex_asc(&BufferPrint[6],&Data_Last_TRS_Emv_Rej.Champ55[Offset],para);
		tprint += sprintf(Buffer+tprint,(PC)BufferPrint);
	}
	
	if(FindTag (SEMV_TAG_9F27_CRYPTO_INFO_DATA, &Offset, &Length, Data_Last_TRS_Emv_Rej.Champ55, Data_Last_TRS_Emv_Rej.l_champ55) == OK)
	{
		memset(BufferPrint,0,sizeof(BufferPrint));
		memcpy(BufferPrint,"\n9F27: ",6);
		para=2*Length;
		hex_asc(&BufferPrint[6],&Data_Last_TRS_Emv_Rej.Champ55[Offset],para);
		tprint += sprintf(Buffer+tprint,(PC)BufferPrint);
	}
	
	if(FindTag (SEMV_TAG_9F33_TERM_CAPABILITIES, &Offset, &Length, Data_Last_TRS_Emv_Rej.Champ55, Data_Last_TRS_Emv_Rej.l_champ55) == OK)
	{
		memset(BufferPrint,0,sizeof(BufferPrint));
		memcpy(BufferPrint,"\n9F33: ",6);
		para=2*Length;
		hex_asc(&BufferPrint[6],&Data_Last_TRS_Emv_Rej.Champ55[Offset],para);
		tprint += sprintf(Buffer+tprint,(PC)BufferPrint);
	}
	
	if(FindTag (SEMV_TAG_9F34_CVM_RESULT, &Offset, &Length, Data_Last_TRS_Emv_Rej.Champ55, Data_Last_TRS_Emv_Rej.l_champ55) == OK)
	{
		memset(BufferPrint,0,sizeof(BufferPrint));
		memcpy(BufferPrint,"\n9F34: ",6);
		para=2*Length;
		hex_asc(&BufferPrint[6],&Data_Last_TRS_Emv_Rej.Champ55[Offset],para);
		tprint += sprintf(Buffer+tprint,(PC)BufferPrint);
	}
	
	if(FindTag (SEMV_TAG_9F35_TERM_TYPE, &Offset, &Length, Data_Last_TRS_Emv_Rej.Champ55, Data_Last_TRS_Emv_Rej.l_champ55) == OK)
	{
		memset(BufferPrint,0,sizeof(BufferPrint));
		memcpy(BufferPrint,"\n9F35: ",6);
		para=2*Length;
		hex_asc(&BufferPrint[6],&Data_Last_TRS_Emv_Rej.Champ55[Offset],para);
		tprint += sprintf(Buffer+tprint,(PC)BufferPrint);
	}
	
	if(FindTag (SEMV_TAG_9F36_ATC, &Offset, &Length, Data_Last_TRS_Emv_Rej.Champ55, Data_Last_TRS_Emv_Rej.l_champ55) == OK)
	{
		memset(BufferPrint,0,sizeof(BufferPrint));
		memcpy(BufferPrint,"\n9F36: ",6);
		para=2*Length;
		hex_asc(&BufferPrint[6],&Data_Last_TRS_Emv_Rej.Champ55[Offset],para);
		tprint += sprintf(Buffer+tprint,(PC)BufferPrint);
	}
	
	if(FindTag (SEMV_TAG_9F37_UNPREDICT_NUMBER, &Offset, &Length, Data_Last_TRS_Emv_Rej.Champ55, Data_Last_TRS_Emv_Rej.l_champ55) == OK)
	{
		memset(BufferPrint,0,sizeof(BufferPrint));
		memcpy(BufferPrint,"\n9F37: ",6);
		para=2*Length;		hex_asc(&BufferPrint[6],&Data_Last_TRS_Emv_Rej.Champ55[Offset],para);
		tprint += sprintf(Buffer+tprint,(PC)BufferPrint);
	}
	
	if(FindTag (SEMV_TAG_9F41_TRANSAC_SEQ_COUNTER, &Offset, &Length, Data_Last_TRS_Emv_Rej.Champ55, Data_Last_TRS_Emv_Rej.l_champ55) == OK)
	{
		memset(BufferPrint,0,sizeof(BufferPrint));
		memcpy(BufferPrint,"\n9F41: ",6);
		memcpy(&BufferPrint[6],&Data_Last_TRS_Emv_Rej.Champ55[Offset],Length);
		tprint += sprintf(Buffer+tprint,(PC)BufferPrint);
	}
	
//	TAG_TCC = 0x9F53;
	if(FindTag (TAG_TCC, &Offset, &Length, Data_Last_TRS_Emv_Rej.Champ55, Data_Last_TRS_Emv_Rej.l_champ55) == OK)
	{
		memset(BufferPrint,0,sizeof(BufferPrint));
		memcpy(BufferPrint,"\n9F53: ",6);
		para=2*Length;
		hex_asc(&BufferPrint[6],&Data_Last_TRS_Emv_Rej.Champ55[Offset],para);
		tprint += sprintf(Buffer+tprint,(PC)BufferPrint);
	}
	*/
	if(Data_Last_TRS_Emv_Rej.l_Tag91)
	{
		memset(BufferPrint,0,sizeof(BufferPrint));
		memcpy(BufferPrint,"\n0091: ",6);
		para=2*Data_Last_TRS_Emv_Rej.l_Tag91;
		hex_asc(BufferScript,Data_Last_TRS_Emv_Rej.Tag91,para);
		tprint += sprintf(Buffer+tprint,(PC)BufferPrint);
	}
	
	if(Data_Last_TRS_Emv_Rej.l_Tag71)
	{
		tprint += sprintf(Buffer+tprint,(PC)"Issuer Script Template1:");
		memset(BufferScript,0,sizeof(BufferScript));
		para=2*Data_Last_TRS_Emv_Rej.l_Tag71;
		hex_asc(BufferScript,Data_Last_TRS_Emv_Rej.Tag71,para);
		for(i=0;(i<(2*Data_Last_TRS_Emv_Rej.l_Tag71));i+=24)
		{
			//tprint += sprintf(Buffer+tprint,&BufferScript[i]);
			memset(chaine,0,sizeof(chaine));
			memcpy(chaine,&BufferScript[i],24);
			tprint += sprintf(Buffer+tprint,(PC)chaine);
		}
	}
	if(Data_Last_TRS_Emv_Rej.l_Tag72)
	{
		tprint += sprintf(Buffer+tprint,(PC)"Issuer Script Template1:");
		memset(BufferScript,0,sizeof(BufferScript));
		para=2*Data_Last_TRS_Emv_Rej.l_Tag72;
		hex_asc(BufferScript,Data_Last_TRS_Emv_Rej.Tag72,para);
		for(i=0;(i<(2*Data_Last_TRS_Emv_Rej.l_Tag72));i+=24)
		{
			//tprint += sprintf(Buffer+tprint,&BufferScript[i]);
			memset(chaine,0,sizeof(chaine));
			memcpy(chaine,&BufferScript[i],24);
			tprint += sprintf(Buffer+tprint,(PC)chaine);
		}
	}
	status = imp_buffer(Buffer,tprint);
	if(status!=OK)
	{
		Os__display(0,0,"Pb. Impression");
		Os__xget_key(5);
	}
	CUTERMpaperFeed();
/*	Os__xlinefeed(4);*/
	
	memset(Buffer,0,sizeof(Buffer));tprint=0;
	tprint += sprintf(Buffer+tprint,"		PARAMETRES TERMINAL""\x1B""F");
	tprint += sprintf(Buffer+tprint,"\n");
	
	memset(BufferPrint,0,sizeof(BufferPrint));
	memcpy(BufferPrint,"\nAID : ",6);
	para=2*Data_Last_TRS_Emv_Rej_tmp.Aid[0];
	hex_asc(&BufferPrint[6],Data_Last_TRS_Emv_Rej.Aid+1,para);

	tprint += sprintf(Buffer+tprint,(PC)BufferPrint);

	
	memset(BufferPrint,0,sizeof(BufferPrint));
	memcpy(BufferPrint,"\nMCC : ",6);
	memcpy(&BufferPrint[6],FicPara.MCC,4);
	tprint += sprintf(Buffer+tprint,(PC)BufferPrint);
	
	memset(BufferPrint,0,sizeof(BufferPrint));
	sprintf((PC)BufferPrint,"\nPlafond : %lu,%02lu",Data_Last_TRS_Emv_Rej.Plafond/100,Data_Last_TRS_Emv_Rej.Plafond%100);
	tprint += sprintf(Buffer+tprint,(PC)BufferPrint);
	
	
	if(Data_Last_TRS_Emv_Rej.SDA_DDA_Performed)
	{
		memset(BufferPrint,0,sizeof(BufferPrint));
		Index_Tab = Data_Last_TRS_Emv_Rej.Idx_Emv_Key;
		
		sprintf((PC)BufferPrint,"\nIndex Cle: %02X",(int)Fic_EMV_Keys.tab_emv_key[Index_Tab].Key_Index);
		tprint += sprintf(Buffer+tprint,(PC)BufferPrint);
		
		memset(BufferPrint,0,sizeof(BufferPrint));
		strcpy((PC)BufferPrint,"\nExposant Cle: ");
		para=2*Fic_EMV_Keys.tab_emv_key[Index_Tab].Lg_Exp;
		hex_asc(&BufferPrint[14],Fic_EMV_Keys.tab_emv_key[Index_Tab].Key_Exp,para);
		tprint += sprintf(Buffer+tprint,(PC)BufferPrint);
		
		tprint += sprintf(Buffer+tprint,"\nDonnees cle :");
		for(j=0;(j<Fic_EMV_Keys.tab_emv_key[Index_Tab].Lg_modulo);j+=8)
		{
			memset(BufferPrint,0,sizeof(BufferPrint));
			hex_asc(BufferPrint,&Fic_EMV_Keys.tab_emv_key[Index_Tab].Key_mod[j],16);
			tprint += sprintf(Buffer+tprint,(PC)BufferPrint);
		}
	}
	
	Index_Tab = Data_Last_TRS_Emv_Rej.Idx_Emv_Tac;
	if(Index_Tab)
	{
		Index_Tab -= 1;
		memset(BufferPrint,0,sizeof(BufferPrint));
		strcpy((PC)BufferPrint,"\nTAC Denial : ");
		hex_asc(&BufferPrint[13],Fic_EMV_TAC.tab_emv_tac[Index_Tab].TAC_denial,10);
		tprint += sprintf(Buffer+tprint,(PC)BufferPrint);
		
		memset(BufferPrint,0,sizeof(BufferPrint));
		strcpy((PC)BufferPrint,"\nTAC Online : ");
		hex_asc(&BufferPrint[13],Fic_EMV_TAC.tab_emv_tac[Index_Tab].TAC_online,10);
		tprint += sprintf(Buffer+tprint,(PC)BufferPrint);
		
		memset(BufferPrint,0,sizeof(BufferPrint));
		strcpy((PC)BufferPrint,"\nTAC Default: ");
		hex_asc(&BufferPrint[13],Fic_EMV_TAC.tab_emv_tac[Index_Tab].TAC_default,10);
		tprint += sprintf(Buffer+tprint,(PC)BufferPrint);
	}
	
	Index_Tab = Data_Last_TRS_Emv_Rej.Idx_Emv_TDOL;
	if(Index_Tab)
	{
		tprint += sprintf(Buffer+tprint,"\nDefault TDOL :");
		memset(BufferScript,0,sizeof(BufferScript));
		para=2*Fic_EMV_TDOL.tab_emv_tdol[Index_Tab].Lg_TDOL;
		hex_asc(BufferScript,Fic_EMV_TDOL.tab_emv_tdol[Index_Tab].TDOL,para);
		for(j=0;(j<strlen((PC)BufferScript));j+=24)
			tprint += sprintf((PC)Buffer+tprint,(PC)&BufferScript[j]);
	}
	
	Index_Tab = Data_Last_TRS_Emv_Rej.Idx_Emv_DDOL;
	if(Index_Tab)
	{
		tprint += sprintf(Buffer+tprint,"\nDefault DDOL :");
		memset(BufferScript,0,sizeof(BufferScript));
		para=2*Fic_EMV_DDOL.tab_emv_ddol[Index_Tab].Lg_DDOL;
		hex_asc(BufferScript,Fic_EMV_DDOL.tab_emv_ddol[Index_Tab].DDOL,para);
		for(j=0;(j<strlen((PC)BufferScript));j+=24)
			tprint += sprintf((PC)Buffer+tprint,(PC)&BufferScript[j]);
	}
	
	Index_Tab = Data_Last_TRS_Emv_Rej.Idx_Emv_RAND;
	if(Index_Tab)
	{
		tprint += sprintf(Buffer+tprint,"\nThreshold Amount :");
		memset(BufferPrint,0,sizeof(BufferPrint));
		memcpy(BufferPrint,Fic_EMV_rand.tab_emv_rand[Index_Tab].montant_seuil,12);
		tprint += sprintf(Buffer+tprint,(PC)BufferPrint);
		
		memset(BufferPrint,0,sizeof(BufferPrint));
		sprintf((PC)BufferPrint,"\nThreshold percentage :%.2s",Fic_EMV_rand.tab_emv_rand[Index_Tab].Nbr_trs);
		tprint += sprintf(Buffer+tprint,(PC)BufferPrint);
		
		memset(BufferPrint,0,sizeof(BufferPrint));
		sprintf((PC)BufferPrint,"\nThreshold percentage :%.2s",Fic_EMV_rand.tab_emv_rand[Index_Tab].Nbr_max_trs);
		tprint += sprintf(Buffer+tprint,(PC)BufferPrint);
	}
	
	tprint += sprintf(Buffer+tprint,"\n\n\n\n");
	status = imp_buffer(Buffer,tprint);
	if(status!=OK)
	{
		Os__display(0,0,"Pb. Impression");
		Os__xget_key(5);
	}
	return OK;
}

/*-----------------------------------------------------------------             
 * Fonction   :Imprime_devises
 * objet      :
 *             
 * Parametre  :
 * Retour     :
 *-----------------------------------------------------------------             
*/
uchar	Imprime_devises(void)
{
	Ushort		j,Indice_fin;
	int		i=0;
	char		Buffer[400];
	uchar		montant[13];
	uchar		affiche_mont[14];
	uchar		status;
	uchar		trouve = KO;

	uchar		Exp;
	DATE date;
	
	StructInfoPayment.waitpuce = 0;
	/*Aff_indicNO_OUI(FicPara.Langue);*/
	CUMORE_CMI_AFFICHE_MESSAGE(94,32,1000);
	//Aff_mess(FicPara.Langue,94,0,0,0,0);
	//Aff_mess(FicPara.Langue,32,1,0,0,0);
	
	CUTERMpaperFeed();
/*	Os__xlinefeed(1);*/
    memset(Buffer,0,sizeof(Buffer));
	/*i += sprintf(Buffer+i, "<#f=%08lx#><#c#>C.M.I",(UL)SYSTEM_FONT_5x7_ID);//logo*/
	i += sprintf(Buffer+i, "\x1B""@");

	i += sprintf(Buffer+i, "\x1B""E            CENTRE\n");
	i += sprintf(Buffer+i, "\x1B""E          MONETIQUE\n");
	i += sprintf(Buffer+i, "\x1B""E       INTERBANCAIRE\n");

	i += sprintf(Buffer+i,"\n             DEVISES""\x1B""F");
	
	Telium_Read_date(&date);
	/*Os__read_date(data_time);
	Os__read_time_sec (data_time+6) ;
	i += sprintf(Buffer+i,"\n%.2s/%.2s/%.2s<#r#>%.2s:%.2s",
		data_time,data_time+2,
		data_time+4,data_time+6,data_time+8);*/
	i += sprintf(Buffer+i,"\n%.2s/%.2s/%.2s						%.2s:%.2s:%.2s",
		date.day,
		date.month,
		date.year,
		date.hour,
		date.minute,
		date.second);
	
	i += sprintf(Buffer+i, "\n\n%.15s ",FicPara.Num_commerce);
	i += sprintf(Buffer+i, "\n%.8s",FicPara.Num_Terminal);
	i += sprintf(Buffer+i, "\n%.15s",FicPara.Num_point_vente);
	i += sprintf(Buffer+i, "\n%.24s",FicPara.Merchant_Name);
	i += sprintf(Buffer+i, "\n%.13s",FicPara.Merchant_City);
	i += sprintf(Buffer+i, "\n%.24s",FicPara.Merchant_Adrees);
	
	i += sprintf(Buffer+i,"\n\n				DEVISES ACC");
	Indice_fin=Fic_Devise_Acc.Indice_fin;
	if(Indice_fin==0)
	{
		i += sprintf(Buffer+i,"\n\n			NO DEVISES ACC");
	}
	//i += sprintf(Buffer+i, "\x0f");
	for(j=0;j<Indice_fin;j++)
	{
		trouve = OK;
		memset(montant,0,13);
		memset(affiche_mont,0,14);
		i += sprintf(Buffer+i, "\n\nCode num   :        %.3s ",Fic_Devise_Acc.TabDevise[j].code_num);
		i += sprintf(Buffer+i, "\nCode alpha :        %.3s ",Fic_Devise_Acc.TabDevise[j].code_alph);
		long_asc (montant, 12, &Fic_Devise_Acc.TabDevise[j].Montant_min);
		RemoveLeftZeros(montant) ;
		Exp=Fic_Devise_Acc.TabDevise[j].Exposant-48;
		FormatAmount(montant,affiche_mont,Exp);
		i += sprintf(Buffer+i, "\nMont min   :       %.14s ",affiche_mont);
		//if (i >= 300)
		{
			status = imp_buffer(Buffer,i);
			if(status != OK)
			{
				//i += sprintf(Buffer+i, "\x12");
				return KO;
			}
			trouve = KO;
			i = 0;
			memset(Buffer,0,sizeof(Buffer));
		}
	}
	//i += sprintf(Buffer+i, "\x12");
	/*i += sprintf(Buffer+i, "<#f=%08lx#>",(UL)SYSTEM_FONT_5x7_ID);//Font normal*/
	i += sprintf(Buffer+i, "\x1B""F");//logo
	i += sprintf(Buffer+i,"\n\n				DEVISES REG");
	Indice_fin=Fic_Devise_reg.Indice_fin;
	if(Indice_fin==0)
	{
		i += sprintf(Buffer+i,"\n\n			NO DEVISES REG");
	}
	//i += sprintf(Buffer+i, "\x0f");
	for(j=0;j<Indice_fin;j++)
	{
		trouve = OK;
		i += sprintf(Buffer+i, "\n\nCode num   :       %.3s ",Fic_Devise_reg.TabDevise[j].code_num);
		i += sprintf(Buffer+i, "\nCode alpha :       %.3s ",Fic_Devise_reg.TabDevise[j].code_alph);
		//if (i >= 400)
		{
			status = imp_buffer(Buffer,i);
			if(status != OK)
			{
				//i += sprintf(Buffer+i, "\x12");
				return KO;
			}

			trouve = KO;
			i = 0;
			memset(Buffer,0,sizeof(Buffer));
		}
	}
	//i += sprintf(Buffer+i, "\x12");
	if ((trouve==OK)&&(i>0))
	{
		status = imp_buffer(Buffer,i);
		if(status != OK)
		{
			return KO;
		}

	}
	CUTERMpaperFeed();
/*	Os__xlinefeed(4);*/
	return OK;
}
/*-----------------------------------------------------------------             
 * Fonction   :Imprime_Bin
 * objet      :
 *             
 * Parametre  :
 * Retour     :
 *-----------------------------------------------------------------             
*/
uchar	Imprime_Bin(void)
{
	Ushort	i;
	int		j=0;
	Ushort	indexfin ;
	char	Buffer[500];
	uchar	montant[13];
	uchar	affiche_mont[14];
	uchar	status;
	uchar	trouve = KO;

	DATE date;

	StructInfoPayment.waitpuce = 0;
	/*Aff_indicNO_OUI(FicPara.Langue);*/
	CUMORE_CMI_AFFICHE_MESSAGE(94,32,1000);
	//Aff_mess(FicPara.Langue,94,0,0,0,0);
	//Aff_mess(FicPara.Langue,32,1,0,0,0);

	memset(Buffer,0,sizeof(Buffer));

	j = 0;
	CUTERMpaperFeed();
/*	Os__xlinefeed(1);*/
	/*j += sprintf(Buffer+j, "<#f=%08lx#><#c#>C.M.I",(UL)SYSTEM_FONT_5x7_ID);//logo*/
	j += sprintf(Buffer+j, "\x1B""@");

	j += sprintf(Buffer+j, "\x1B""E            CENTRE\n");
	j += sprintf(Buffer+j, "\x1B""E          MONETIQUE\n");
	j += sprintf(Buffer+j, "\x1B""E       INTERBANCAIRE\n");

	j += sprintf(Buffer+j,"\n               BIN""\x1B""F");

	Telium_Read_date(&date);
	j += sprintf(Buffer+j,"\n%.2s/%.2s/%.2s						%.2s:%.2s:%.2s",
		date.day,
		date.month,
		date.year,
		date.hour,
		date.minute,
		date.second);
	/*Os__read_date(data_time);
	Os__read_time_sec (data_time+6) ;
	j += sprintf(Buffer+j,"\n%.2s/%.2s/%.2s<#r#>%.2s:%.2s",
   			data_time,data_time+2,
   			data_time+4,data_time+6,data_time+8);*/

	j += sprintf(Buffer+j, "\n\n%.15s ",FicPara.Num_commerce);
	j += sprintf(Buffer+j, "\n%.8s",FicPara.Num_Terminal);
	j += sprintf(Buffer+j, "\n%.15s",FicPara.Num_point_vente);
	j += sprintf(Buffer+j, "\n%.24s",FicPara.Merchant_Name);
	j += sprintf(Buffer+j, "\n%.13s",FicPara.Merchant_City);
	j += sprintf(Buffer+j, "\n%.24s\n\n",FicPara.Merchant_Adrees);

	if((indexfin=FicBin.Indice_fin)==0)
	{
		imp_buffer(Buffer,j);
		Os__xprinta("\n	PAS DE BIN LIST");
		CUTERMpaperFeed();
/*		Os__xlinefeed(4);*/
		return KO ;
	}

	j += sprintf(Buffer+j, "\x0f");
	i=0 ;
	while(i<indexfin)
	{
		trouve = OK;
		memset(montant,0,13);
		memset(affiche_mont,0,14);
		j += sprintf(Buffer+j,"\n%.11s			 ""\x1B""F",FicBin.Tab_Bin[i].Bin_Debut) ;
		j += sprintf(Buffer+j,"%.11s",FicBin.Tab_Bin[i].Bin_Fin) ;
		j += sprintf(Buffer+j,"\n%.3s ",FicBin.Tab_Bin[i].Code_Service);
		j += sprintf(Buffer+j,"	 %c ",FicBin.Tab_Bin[i].LgCard_Type);		

		j += sprintf(Buffer+j,"	 %02d ",FicBin.Tab_Bin[i].LgCard_Min);
		j += sprintf(Buffer+j,"	 %02d ",FicBin.Tab_Bin[i].LgCard_Max);

		j += sprintf(Buffer+j,"		%c ",FicBin.Tab_Bin[i].Gest_Pin);
		j += sprintf(Buffer+j,"	 %c ",FicBin.Tab_Bin[i].OffSet_Octet36 + 0x30);

		long_asc (montant, 12, &FicBin.Tab_Bin[i].Plafond);
		RemoveLeftZeros(montant) ;
		FormatAmount(montant,affiche_mont,2);
		j += sprintf(Buffer+j, "\nPlafond	:		%.14s \n",affiche_mont);

		if (j >= 400)
		{
			status = imp_buffer(Buffer,j);
			if(status != OK)
			{
			j += sprintf(Buffer+j, "\x12");
				return KO;
			}
			trouve = KO;
			j = 0;
			memset(Buffer,0,sizeof(Buffer));
		}
		i++ ;
	}
	j += sprintf(Buffer+j, "\x12");

	if ((trouve==OK)&&(j>0))
	{
		status = imp_buffer(Buffer,j);
		if(status != OK)
		{
			return KO;
		}
	}
	CUTERMpaperFeed();
/*	Os__xlinefeed(4);*/
	return OK;
}
/*-----------------------------------------------------------------             
 * Fonction   :Imprime_Comptes
 * objet      :
 *             
 * Parametre  :
 * Retour     :
 *-----------------------------------------------------------------             
*/
uchar	Imprime_Comptes(void)
{
	Ushort	i;
	int		j=0;
	Ushort	indexfin ;
	char	Buffer[300];
	uchar	status;

	DATE date;
	
	StructInfoPayment.waitpuce = 0;
	/*Aff_indicNO_OUI(FicPara.Langue);*/
	CUMORE_CMI_AFFICHE_MESSAGE(94,32,1000);
	//Aff_mess(FicPara.Langue,94,0,0,0,0);
	//Aff_mess(FicPara.Langue,32,1,0,0,0);
	
	memset(Buffer,0,sizeof(Buffer));
	j=0;
	CUTERMpaperFeed();
/*	Os__xlinefeed(1);*/
	/*j += sprintf(Buffer+j, "<#f=%08lx#><#c#>C.M.I",(UL)SYSTEM_FONT_5x7_ID);//logo*/
	j += sprintf(Buffer+j, "\x1B""@");

	j += sprintf(Buffer+j, "\x1B""E            CENTRE\n");
	j += sprintf(Buffer+j, "\x1B""E          MONETIQUE\n");
	j += sprintf(Buffer+j, "\x1B""E       INTERBANCAIRE\n");

	j += sprintf(Buffer+j,"\n              COMPTES""\x1B""F");
	
	Telium_Read_date(&date);
	j += sprintf(Buffer+j,"\n%.2s/%.2s/%.2s						%.2s:%.2s:%.2s",
		date.day,
		date.month,
		date.year,
		date.hour,
		date.minute,
		date.second);
	/*Os__read_date(data_time);
	Os__read_time_sec (data_time+6) ;
	j += sprintf(Buffer+j,"\n%.2s/%.2s/%.2s<#r#>%.2s:%.2s",data_time,data_time+2,data_time+4,data_time+6,data_time+8);*/
	
	j += sprintf(Buffer+j, "\n\n%.15s ",FicPara.Num_commerce);
	j += sprintf(Buffer+j, "\n%.8s",FicPara.Num_Terminal);
	j += sprintf(Buffer+j, "\n%.15s",FicPara.Num_point_vente);
	j += sprintf(Buffer+j, "\n%.24s",FicPara.Merchant_Name);
	j += sprintf(Buffer+j, "\n%.13s",FicPara.Merchant_City);
	j += sprintf(Buffer+j, "\n%.24s\n\n",FicPara.Merchant_Adrees);
	
	//Os__xprinta("\x1c\x1d      COMPTES LIST");
	if((indexfin=Fic_compte_dom.Indice_fin)==0)
	{
		imp_buffer(Buffer,j);
		Os__xprinta("\n\n		PAS DE COMPTES LIST");
		CUTERMpaperFeed();
/*		Os__xlinefeed(4);*/
		return KO ;
	}
	//j += sprintf(Buffer+j, "\x0f");
	i=0 ;
	while(i<indexfin)
	{
		j += sprintf(Buffer+j, "\n%.10s",Fic_compte_dom.tab_compte[i].Compte) ;
		i++;
	}
	//j += sprintf(Buffer+j, "\x12");
	status = imp_buffer(Buffer,j);
	if(status != OK)
	{
		return KO;
	}
	CUTERMpaperFeed();
/*	Os__xlinefeed(4);*/
	return OK;
}
/*-----------------------------------------------------------------             
 * Fonction   :Imprime_Prefixes
 * objet      :
 *             
 * Parametre  :
 * Retour     :
 *-----------------------------------------------------------------             
*/
uchar	Imprime_Prefixes(void)
{
	Ushort		i;
	int		j=0;
	Ushort		indexfin ;
	char		Buffer[500];
	uchar		montant[13];
	uchar		affiche_mont[14];
	uchar		status;
	uchar		trouve = KO;

	DATE date;

	StructInfoPayment.waitpuce = 0;
	/*Aff_indicNO_OUI(FicPara.Langue);*/
	CUMORE_CMI_AFFICHE_MESSAGE(94,32,1000);
	//Aff_mess(FicPara.Langue,94,0,0,0,0);
	//Aff_mess(FicPara.Langue,32,1,0,0,0);

	memset(Buffer,0,sizeof(Buffer));

	j=0;
	CUTERMpaperFeed();
/*	Os__xlinefeed(1);*/
	/*j += sprintf(Buffer+j, "<#f=%08lx#><#c#>C.M.I",(UL)SYSTEM_FONT_5x7_ID);//logo*/
	j += sprintf(Buffer+j, "\x1B""@");

	j += sprintf(Buffer+j, "\x1B""E            CENTRE\n");
	j += sprintf(Buffer+j, "\x1B""E          MONETIQUE\n");
	j += sprintf(Buffer+j, "\x1B""E       INTERBANCAIRE\n");

	j += sprintf(Buffer+j,"\n                PREFIXES""\x1B""F");

	Telium_Read_date(&date);
	j += sprintf(Buffer+j,"\n%.2s/%.2s/%.2s						%.2s:%.2s:%.2s",
		date.day,
		date.month,
		date.year,
		date.hour,
		date.minute,
		date.second);
	/*Os__read_date(data_time);
	Os__read_time_sec (data_time+6) ;
	j += sprintf(Buffer+j,"\n%.2s/%.2s/%.2s<#r#>%.2s:%.2s",
   		data_time,data_time+2,
   		data_time+4,data_time+6,data_time+8);*/

	j += sprintf(Buffer+j, "\n\n%.15s ",FicPara.Num_commerce);
	j += sprintf(Buffer+j, "\n%.8s",FicPara.Num_Terminal);
	j += sprintf(Buffer+j, "\n%.15s",FicPara.Num_point_vente);
	j += sprintf(Buffer+j, "\n%.24s",FicPara.Merchant_Name);
	j += sprintf(Buffer+j, "\n%.13s",FicPara.Merchant_City);
	j += sprintf(Buffer+j, "\n%.24s",FicPara.Merchant_Adrees);

	indexfin = FicInsReg.Indice_fin;
	if(indexfin==0)
	{
		imp_buffer(Buffer,j);
		Os__xprinta("  \n	PAS DE PREFIXES LIST");
		CUTERMpaperFeed();
/*		Os__xlinefeed(4);*/
		return KO ;
	}

	j += sprintf(Buffer+j, "\n\n\n------------------------\n");
	//j += sprintf(Buffer+j, "\x0f");
	i=0 ;
	while(i<indexfin)
	{
		trouve = OK;
		j += sprintf(Buffer+j, "\n%.02d ""\x1B""F",FicInsReg.Tab_Ins_reg[i].Code_Prefixe);
		j += sprintf(Buffer+j, "\n%.16s",FicInsReg.Tab_Ins_reg[i].Libelle);
		j += sprintf(Buffer+j, "\n%s",FicInsReg.Tab_Ins_reg[i].Num_point_vente);
		memset(montant,0,13);
		memset(affiche_mont,0,13);
		long_asc (montant, 12, &FicInsReg.Tab_Ins_reg[i].Plafond);
		RemoveLeftZeros(montant) ;
		FormatAmount(montant,affiche_mont,2);
		j += sprintf(Buffer+j, "\n%.14s\n",affiche_mont);
		j += sprintf(Buffer+j, "\n------------------------\n");
		if (j >= 400)
		{
			status = imp_buffer(Buffer,j);
			if(status != OK)
			{
				//j += sprintf(Buffer+j, "\x12");
				return KO;
			}
			trouve = KO;
			j = 0;
			memset(Buffer,0,sizeof(Buffer));
		}
		i++;
	}
	//j += sprintf(Buffer+j, "\x12");

	if ((trouve==OK)&&(j>0))
	{
		status = imp_buffer(Buffer,j);
		if(status != OK)
		{
			return KO;
		}
	}
	CUTERMpaperFeed();
/*	Os__xlinefeed(4);*/
	return OK;
}
/*-----------------------------------------------------------------             
 * Fonction   :ImprimeTra
 * objet      :
 *             
 * Parametre  :
 * Retour     :
 *-----------------------------------------------------------------             
*/
uchar	ImprimeTraComp(uchar langue,uchar source,uchar dup,lignetra		*transac)
{
	char		pPtr[1000];

	int		j=0;
	uchar		data_time[13];
	char		affich_Cs[11];
	uchar		montant[13]  ;
	uchar		affiche_mont[14];
	uchar		Card_Nb[20];
	uchar		Exp;
	uchar		status;
	Ulong		MontComp;
	char		NumSeqReliquat[6];

	StructInfoPayment.waitpuce = 0;
	/*Aff_indicNO_OUI(FicPara.Langue);*/
	CUMORE_CMI_AFFICHE_MESSAGE(94,32,1000);
	//Aff_mess(FicPara.Langue,94,0,0,0,0);
	//Aff_mess(FicPara.Langue,32,1,0,0,0);

	memset(pPtr,0x00,sizeof(pPtr)) ;
	memset(affich_Cs,0x00,11) ;
	memset(montant,0x00,13) ;
	memset(affiche_mont,0x00,14) ;

	//CMI Gras
	j=0 ;
	//j += sprintf(pPtr+j, "<#f=%08lx#>",(UL)SYSTEM_FONT_8x14_ID);//Font Gra
	j += sprintf(pPtr+j, "\x1B""@");//Font Normal
//	j += sprintf(pPtr+j, "\x1B""E");//Font Gra
	j += sprintf(pPtr+j, "\x1B""E            CENTRE\n");
	j += sprintf(pPtr+j, "\x1B""E          MONETIQUE\n");
	j += sprintf(pPtr+j, "\x1B""E       INTERBANCAIRE\n");
	
	//DUPLICATA Gras	||	PRE-AUTORISATION	||	VAL.TRX.PRE-AUTO	||
	if(dup)
   		j += sprintf(pPtr+j, "\n         DUPLICATA");
	
	if(StructInfoPayment.CodeFunction == PREAUTHCODE)
		j += sprintf(pPtr+j, "\n      PRE-AUTORISATION");
	else if(StructInfoPayment.CodeFunction == PREAUTHCOMPCODE)
		j += sprintf(pPtr+j, "\n      VAL.TRX.PRE-AUTO");
	else
		j += sprintf(pPtr+j, "\n");
	
	//JJ/MM/AA	HH:MM:SS	+	N pos	+	N terminal	+	Ensei Com	+	Ville
	//j += sprintf(pPtr+j, "<#f=%08lx#>",(UL)SYSTEM_FONT_5x7_ID);//Font Normal
	j += sprintf(pPtr+j, "\x1B""F");//Font Normal
	memcpy(data_time,StructInfoPayment.Transaction_DateTime,12) ;
	j += sprintf(pPtr+j, "\n%.2s/%.2s/%.2s						%.2s:%.2s:%.2s",
		data_time+4,data_time+2,data_time,data_time+6,data_time+8,data_time+10);
	
	j += sprintf(pPtr+j, "\n%.15s",StructInfoPayment.Num_point_vente);
	j += sprintf(pPtr+j, "\n%.8s",FicPara.Num_Terminal);
	j += sprintf(pPtr+j, "\n%.24s",FicPara.Merchant_Name);
	j += sprintf(pPtr+j, "\n%.13s",FicPara.Merchant_City);

	//Nom du porteur
	if(StructInfoPayment.Appli_label[0]!=0)
	{
		j += sprintf(pPtr+j, "\n\n%s",StructInfoPayment.Appli_aid+1);
		j += sprintf(pPtr+j, "\n%.16s",StructInfoPayment.Appli_label);
		j += sprintf(pPtr+j, "\n%.24s",StructInfoPayment.Nom_Prenom);
	}
	else
		j += sprintf(pPtr+j, "\n\n%.24s",StructInfoPayment.Nom_Prenom);

	//Numéro de la carte
	memset(Card_Nb,0,20);
	memcpy(Card_Nb,&StructInfoPayment.Holder_Card[1],StructInfoPayment.Holder_Card[0]);

//****************************//
	status = imp_buffer(pPtr,j);
	if(status!=OK)
	{
		Os__display(0,0,"Pb. Impression");
		CUTERMpaperFeed();
		return status;
	}
	j=0;
	memset(pPtr,0,sizeof(pPtr));
//****************************//
	if(source==CLIENT)
	{
//		Card_Nb[strlen(Card_Nb)-1] = '*';
//		Card_Nb[strlen(Card_Nb)-2] = '*';
//		Card_Nb[strlen(Card_Nb)-3] = '*';
//		Card_Nb[strlen(Card_Nb)-4] = '*';
		Ushort ik;

		for(ik=0;ik<strlen((char*)Card_Nb)-4;ik++)
			Card_Nb[ik] = '*';
	}

	j += sprintf(pPtr+j, "\n%s",Card_Nb);
	//MM/AA	+	CARTE ETRANGERE||CARTE NATIONALE
	j += sprintf(pPtr+j, "\n%.2s/%.2s\t\t",StructInfoPayment.Expired_Date+2,StructInfoPayment.Expired_Date);
	if(StructInfoPayment.Origine_carte == CARTE_NATIONALE)
		j += sprintf(pPtr+j, "CARTE NATIONALE");
	else
		j += sprintf(pPtr+j, "CARTE ETRANGERE");
	//CERTIF EMV
	if(StructInfoPayment.Entry_Mode==TRS_EMV)
		j += sprintf(pPtr+j, "\n%.16s",StructInfoPayment.Certif);
	//XXX-C-YYY-L-Z
	memcpy(affich_Cs,StructInfoPayment.SeviceCode,3) ;
	//if(StructInfoPayment.forcage)
	if((StructInfoPayment.forcage)||(StructInfoPayment.Top_Forcage == TOP_FOR_REFFERAL))
   		affich_Cs[3]='1';
	else
   		affich_Cs[3]='0';
	memcpy(affich_Cs+4,&lop_gest.NumListe[0],4) ;
	if(StructInfoPayment.Entry_Mode==TRS_EMV)
      	affich_Cs[8]='1';
	else if(StructInfoPayment.Entry_Mode==TRS_MAN)
		affich_Cs[8]='2';
	else
   	affich_Cs[8]='0'; 
	sprintf(affich_Cs+9,"%.02d",FicInsReg.Tab_Ins_reg[StructInfoPayment.Type_carte].Code_Prefixe);
	j += sprintf(pPtr+j, "\n%.3s-%.1s-%.4s-%.1s-%.2s",affich_Cs,affich_Cs+3,affich_Cs+4,affich_Cs+8,affich_Cs+9);
	//Message1
	//if(!dup)
		j += sprintf(pPtr+j, "\n\n		%.24s",StructInfoPayment.Message1);

//****************************//
	status = imp_buffer(pPtr,j);
	if(status!=OK)
	{
		Os__display(0,0,"Pb. Impression");
		CUTERMpaperFeed();
		return status;
	}
	j=0;
	memset(pPtr,0,sizeof(pPtr));
//****************************//

	//pre autorisation
	if(
			(StructInfoPayment.CodeFunction==PREAUTHCOMPCODE)
		&&	(StructInfoPayment.Preauto_Amount_Final > StructInfoPayment.Preauto_Amount_Initial + 
														(StructInfoPayment.Preauto_Amount_Initial*15)/100)
	)
	{
		//Pre-autorisation
		//j += sprintf(pPtr+j, "<#f=%08lx#>",(UL)SYSTEM_FONT_8x14_ID);//Font Gra
		j += sprintf(pPtr+j, "\x1B""E");//Font Gra
		j += sprintf(pPtr+j, "\n\n		Pre-autorisation");
		//j += sprintf(pPtr+j, "<#f=%08lx#>",(UL)SYSTEM_FONT_5x7_ID);//Font Normal
		j += sprintf(pPtr+j, "\x1B""F");//Font Normal
		//JJ/MM/AA	HH:MM:SS
		memcpy(data_time,(*transac).Transaction_DateTime,12);
		j += sprintf(pPtr+j, "\n%.2s/%.2s/%.2s						%.2s:%.2s:%.2s",
			data_time+4,data_time+2,data_time,data_time+6,data_time+8,data_time+10);
		//Montant
		memset(affiche_mont,0x00,14) ;
		long_asc (montant, 12,&StructInfoPayment.Transaction_Amount);
		RemoveLeftZeros(montant) ;
		Exp=StructInfoPayment.Devise_acc.Exposant-48;
		FormatAmount(montant,affiche_mont,Exp);	
		j += sprintf(pPtr+j, "\nMnt :		%.13s%.3s",affiche_mont,StructInfoPayment.Devise_acc.code_alph);
		//Num transaction
		j += sprintf(pPtr+j, "\nNum.Transaction : %.6s",StructInfoPayment.Num_sequence);
		//Num autorisation
		j += sprintf(pPtr+j, "\nNum.Autorisation: %.6s",StructInfoPayment.Approval_Code);
		//Stan
		j += sprintf(pPtr+j, "\nSTAN            : %.6s",StructInfoPayment.STAN);
		
		//Complement
		//j += sprintf(pPtr+j, "<#f=%08lx#>",(UL)SYSTEM_FONT_8x14_ID);//Font Gra
		j += sprintf(pPtr+j, "\x1B""E");//Font Gra
		j += sprintf(pPtr+j, "\n\n		Complement");
		//j += sprintf(pPtr+j, "<#f=%08lx#>",(UL)SYSTEM_FONT_5x7_ID);//Font Normal
		j += sprintf(pPtr+j, "\x1B""F");//Font Normal
		//JJ/MM/AA	HH:MM:SS
		memcpy(data_time,StructInfoPayment.Transaction_DateTime,12) ;
		j += sprintf(pPtr+j, "\n%.2s/%.2s/%.2s						%.2s:%.2s:%.2s",
			data_time+4,data_time+2,data_time,data_time+6,data_time+8,data_time+10);
		//Montant
		memset(affiche_mont,0x00,14) ;
		MontComp = (*transac).Transaction_Amount;
		long_asc (montant, 12, &MontComp);
		RemoveLeftZeros(montant) ;
		Exp=StructInfoPayment.Devise_acc.Exposant-48;
		FormatAmount(montant,affiche_mont,Exp);
		j += sprintf(pPtr+j, "\nMnt :		%.13s%.3s",affiche_mont,StructInfoPayment.Devise_acc.code_alph);
		//Num transaction
		memcpy(NumSeqReliquat,(*transac).Num_sequence,6);
		//addition(NumSeqReliquat,"000001",6);
		j += sprintf(pPtr+j, "\nNum.Transaction : %.6s",NumSeqReliquat);
		//Num autorisation
		j += sprintf(pPtr+j, "\nNum.Autorisation: %.6s",(*transac).Approval_Code);
		//Stan
		j += sprintf(pPtr+j, "\nSTAN            : %.6s",(*transac).STAN);
		//Montant total
		memset(affiche_mont,0x00,14);
		MontComp = StructInfoPayment.Transaction_Amount+(*transac).Transaction_Amount;
		long_asc (montant, 12, &MontComp);
		RemoveLeftZeros(montant);
		Exp=StructInfoPayment.Devise_acc.Exposant-48;
		FormatAmount(montant,affiche_mont,Exp);
		j += sprintf(pPtr+j, "\n\nMnt Total:		%.13s%.3s",affiche_mont,StructInfoPayment.Devise_acc.code_alph);

//****************************//
	status = imp_buffer(pPtr,j);
	if(status!=OK)
	{
		Os__display(0,0,"Pb. Impression");
		CUTERMpaperFeed();
		return status;
	}
	j=0;
	memset(pPtr,0,sizeof(pPtr));
//****************************//

	}
	else
	{
		//Montant
		long_asc (montant, 12, &StructInfoPayment.Transaction_Amount);
		RemoveLeftZeros(montant) ;
		Exp=StructInfoPayment.Devise_acc.Exposant-48;
		FormatAmount(montant,affiche_mont,Exp);
		j += sprintf(pPtr+j, "\n\n\nMnt :		%.13s%.3s",affiche_mont,StructInfoPayment.Devise_acc.code_alph);
		//Num transaction//Num Pre autorisation
		if(StructInfoPayment.CodeFunction==PREAUTHCODE)
			j += sprintf(pPtr+j, "\nNum.Pre-autori  :%.6s",StructInfoPayment.Num_sequence);
		else
			j += sprintf(pPtr+j, "\nNum.Transaction :%.6s",StructInfoPayment.Num_sequence);
		//Num autorisation
		j += sprintf(pPtr+j, "\nNum.Autorisation: %.6s",StructInfoPayment.Approval_Code);
		//Stan
		j += sprintf(pPtr+j, "\nSTAN            : %.6s",StructInfoPayment.STAN);
//****************************//
		status = imp_buffer(pPtr,j);
		if(status!=OK)
		{
			Os__display(0,0,"Pb. Impression");
			CUTERMpaperFeed();
			return status;
		}
		j=0;
		memset(pPtr,0,sizeof(pPtr));
//****************************//
	}
	//DEBIT	||	ANNULATION	||	CREDIT
	//j += sprintf(pPtr+j, "<#f=%08lx#>",(UL)SYSTEM_FONT_8x14_ID);//Font Gra
	j += sprintf(pPtr+j, "\x1B""E");//Font Gra
	switch(StructInfoPayment.CodeFunction)
	{
   	case VOIDCODE:
		j += sprintf(pPtr+j, "\n\n           ANNULATION");
		break;
	case REFUNDCODE:
		j += sprintf(pPtr+j, "\n\n                CREDIT");
		break;
	case PREAUTHCODE:
		//j += sprintf(pPtr+j, "\n\n");    //KI Le 190106
		break;
	case VOID_PREAUTO_TOTAL_CODE:
	case VOID_PREAUTO_PARTIEL_CODE:
		j += sprintf(pPtr+j, "\n\nANNULATION PRE-AUTO.");//Mouna310305
		break;
	default:
		j += sprintf(pPtr+j, "\n\n                 DEBIT");
		break;
	}
	//j += sprintf(pPtr+j, "<#f=%08lx#>",(UL)SYSTEM_FONT_5x7_ID);//Font normal
	j += sprintf(pPtr+j, "\x1B""F");//Font Normal
	//Signature client
	switch(StructInfoPayment.CodeFunction)
	{
	case VOIDCODE://231205
	case VOID_PREAUTO_TOTAL_CODE:
	case VOID_PREAUTO_PARTIEL_CODE:
		break;

	default:
		if(StructInfoPayment.SignatureHolder==REQUIRED)
		{
   			if(source==CLIENT)//kamal251105
				j += sprintf(pPtr+j, "\n");
			else if(StructInfoPayment.CodeFunction == PREAUTHCODE) //CCD
				j += sprintf(pPtr+j, "\n     signature client\n\n");
			else
				j += sprintf(pPtr+j, "\n\n     signature client\n\n");
		}
		break;
	}
	//if(!dup)
	{
		j += sprintf(pPtr+j, "\n		%.24s",StructInfoPayment.Message2);
	}
	j += sprintf(pPtr+j, "\n------------------------");
	if(langue==ENG)
	{
		j += sprintf(pPtr+j, "\n		TICKET TO SAVE");
		if(source==CLIENT)
			j += sprintf(pPtr+j, "\n			Client Copy");
		else
			j += sprintf(pPtr+j, "\n	  	Merchant Copy");
	}
	else
	{
		j += sprintf(pPtr+j, "\n		TICKET A CONSERVER");
		if(source==CLIENT)
			j += sprintf(pPtr+j, "\n			Copie client");
		else
			j += sprintf(pPtr+j, "\n	  	Copie commercant");
	}
	status = imp_buffer(pPtr,j);
	if(status != OK)
	{
		return KO;
	}
	CUTERMpaperFeed();
/*	Os__xlinefeed(4);*/
	return OK;
}
/*-----------------------------------------------------------------             
 * Fonction   :Imprime_Param
 * objet      :
 *             
 * Parametre  :
 * Retour     :
 *-----------------------------------------------------------------             
*/
uchar	Imprime_Param(void)
{
	int		i=0,h,j;
	US	k;
	C		Buffer[1500];
	uchar	status;
	Ushort	Index_Cmpte,Indice_fin;
	uchar	Reseau_accept[25];
	DATE	date;
	
	StructInfoPayment.waitpuce = 0;
	/*Aff_indicNO_OUI(FicPara.Langue);*/
	CUMORE_CMI_AFFICHE_MESSAGE(94,32,1000);
	//Aff_mess(FicPara.Langue,94,0,0,0,0);
	//Aff_mess(FicPara.Langue,32,1,0,0,0);

	memset(Buffer,0,sizeof(Buffer));
	//CMI

/*	i += sprintf(Buffer+i, "<#f=%08lx#>",(UL)SYSTEM_FONT_8x14_ID);//Font Gras
	i += sprintf(Buffer+i, "<#c#>C.M.I");*/
	i += sprintf(Buffer+i, "\x1B""@");//Font Normal

	i += sprintf(Buffer+i, "\x1B""E            CENTRE\n");
	i += sprintf(Buffer+i, "\x1B""E          MONETIQUE\n");
	i += sprintf(Buffer+i, "\x1B""E       INTERBANCAIRE\n");

	//PARAMETRES
	i += sprintf(Buffer+i,"\n         PARAMETRES""\x1B""F");
	//JJ/MM/AA	HH:MM:SS
/*	i += sprintf(Buffer+i, "<#f=%08lx#>",(UL)SYSTEM_FONT_5x7_ID);//Font normal*/

	
	Telium_Read_date(&date);
	i += sprintf(Buffer+i,"\n%.2s/%.2s/%.2s						%.2s:%.2s:%.2s",
		date.day,
		date.month,
		date.year,
		date.hour,
		date.minute,
		date.second);

/*	Os__read_date(data_time);
	Os__read_time_sec (data_time+6) ;
	i += sprintf(Buffer+i,"\n%.2s/%.2s/%.2s<#r#>%.2s:%.2s:%.2s",data_time,data_time+2,data_time+4,data_time+6,data_time+8,data_time+10);*/
	//N Commercant
	i += sprintf(Buffer+i, "\n\n%.15s ",FicPara.Num_commerce);
	//N point de vente
	i += sprintf(Buffer+i, "\n%.15s",FicPara.Num_point_vente);
	//N Terminal
	i += sprintf(Buffer+i, "\n%.8s",FicPara.Num_Terminal);
	//Enseigne
	i += sprintf(Buffer+i, "\n%.24s",FicPara.Merchant_Name);
	//MCC
	i += sprintf(Buffer+i, "\nMCC :%.4s",FicPara.MCC);
	//Compte Domiciliation
	if(Fic_compte_dom.Indice_fin)
	{
		Index_Cmpte = FicPara.Index_compte;
		i += sprintf(Buffer+i, "\nCompte dom.:%.12s",Fic_compte_dom.tab_compte[Index_Cmpte].Compte);
	}
	//Adresse
	i += sprintf(Buffer+i, "\n%.24s",FicPara.Merchant_Adrees);
	//Ville
	i += sprintf(Buffer+i, "\n%.13s",FicPara.Merchant_City);	
	//Reseaux 
	i += sprintf(Buffer+i, "\n\nReseaux Acceptes :		");

   h = 0;
   memset(Reseau_accept,0,sizeof(Reseau_accept));
   if(FicPara.Indic_accept_carte[0]=='1')
   	h += sprintf((char*)Reseau_accept+h,"-1");
   if(FicPara.Indic_accept_carte[1]=='1')
   	h += sprintf((char*)Reseau_accept+h,"-2");
   if(FicPara.Indic_accept_carte[2]=='1')
   	h += sprintf((char*)Reseau_accept+h,"-3");
   if(FicPara.Indic_accept_carte[3]=='1')
   	h += sprintf((char*)Reseau_accept+h,"-4");
   if(FicPara.Indic_accept_carte[4]=='1')
   	h += sprintf((char*)Reseau_accept+h,"-5");
   if(FicPara.Indic_accept_carte[5]=='1')
   	h += sprintf((char*)Reseau_accept+h,"-6");
   if(FicPara.Indic_accept_carte[6]=='1')
   	h += sprintf((char*)Reseau_accept+h,"-7");
   if(FicPara.Indic_accept_carte[7]=='1')
   	h += sprintf((char*)Reseau_accept+h,"-8");
   if(FicPara.Indic_accept_carte[8]=='1')
   	h += sprintf((char*)Reseau_accept+h,"-9");
   h += sprintf((char*)Reseau_accept+h,"-");
   i += sprintf(Buffer+i, "\n%s",Reseau_accept);


   //****************************//
	status = imp_buffer(Buffer,i);
	if(status!=OK)
	{
		Os__display(0,0,"Pb. Impression");
		Os__xget_key(5);
		CUTERMpaperFeed();
		return status;
	}
	i=0;
	memset(Buffer,0,sizeof(Buffer));
//****************************//

	Indice_fin = FicInsReg.Indice_fin;
	//i += sprintf(Buffer+i, "\x0f");
	if(Indice_fin!=0)
	{
		C	TmpBuffer[24];

		for(k=0;k<Indice_fin;k++)
		{
			memset(TmpBuffer,0,24);
			memcpy(TmpBuffer,FicInsReg.Tab_Ins_reg[k].Libelle,strlen((char*)FicInsReg.Tab_Ins_reg[k].Libelle));
			i += (Ushort)sprintf(Buffer+i, "\n%s\n             %s",TmpBuffer,FicInsReg.Tab_Ins_reg[k].Num_point_vente);
		}
	}
	//i += sprintf(Buffer+i, "\x12");
//****************************//
	status = imp_buffer(Buffer,i);
	if(status!=OK)
	{
		Os__display(0,0,"Pb. Impression");
		Os__xget_key(5);
		CUTERMpaperFeed();
		return status;
	}
	i=0;
	memset(Buffer,0,sizeof(Buffer));
//****************************//
	//Acceptance
	i += sprintf(Buffer+i, "\n\nAcceptances :		");
	//i += sprintf(Buffer+i, "\x0f");
	i += sprintf(Buffer+i, "\ntrs puce           :%.3s",((FicPara.Accept_puce=='1') ? "OUI" : "NON"));
	i += sprintf(Buffer+i, "\ntrs man            :%.3s",((FicPara.Accept_Man=='1') ? "OUI" : "NON"));
	i += sprintf(Buffer+i, "\ntrs annul.         :%.3s",((FicPara.Indic_annul=='1') ? "OUI" : "NON"));
	i += sprintf(Buffer+i, "\ntrs credit.        :%.3s",((FicPara.Indic_Refund=='1') ? "OUI" : "NON"));
	i += sprintf(Buffer+i, "\npre-autor.         :%.3s",((FicPara.Indic_PreAutor=='1') ? "OUI" : "NON"));
	i += sprintf(Buffer+i, "\ntrs offline        :%.3s",((FicPara.Indic_Offline=='1') ? "OUI" : "NON"));
	i += sprintf(Buffer+i, "\nForcage            :%.3s",((FicPara.Indic_forcage=='1') ? "OUI" : "NON"));
	i += sprintf(Buffer+i, "\nControle fraude    :%.3s",((FicPara.Ctrl_fraude=='1') ? "OUI" : "NON"));
	i += sprintf(Buffer+i, "\nChgmnt compte      :%.3s",((FicPara.Indic_Change_Cpte_Dom=='1') ? "OUI" : "NON"));
	i += sprintf(Buffer+i, "\nCmmrc offshore     :%.3s",((FicPara.Indic_OffShort=='1') ? "OUI" : "NON"));   
	switch(FicPara.Langue)
	{
   	case FRAN:
		i += sprintf(Buffer+i, "\n\nLangue       :FRANCAIS");
		break;
	case ENG:
		i += sprintf(Buffer+i, "\n\nLangue       :ANGLAIS");
		break;
	case ARABE:
		i += sprintf(Buffer+i, "\n\nLangue       :ARABE");
		break;
	default:
		i += sprintf(Buffer+i, "\n\nLangue       :FRANCAIS");
		break;
	}	
	i += sprintf(Buffer+i, "\nNbr Max Trs Off-line:%02d",FicPara.Max_trs_off);
	i += sprintf(Buffer+i, "\nNbr Max Avis Trs    :%02d",FicPara.Max_avis);

	i += sprintf(Buffer+i, "\n\nMonnaie acceptation:");
	Indice_fin=Fic_Devise_Acc.Indice_fin;
	if(Indice_fin!=0)
	{
		for(j=0;j<Indice_fin;j++)
			i += (Ushort)sprintf(Buffer+i, "\n%.3s                  %.3s",
         			Fic_Devise_Acc.TabDevise[j].code_num,Fic_Devise_Acc.TabDevise[j].code_alph);
	}
	
	//type appli
	switch(FicPara.Appli_type)
	{
   	case '0':
		i += sprintf(Buffer+i, "\nType appli:Achat");
		break;
	case '1':
		i += sprintf(Buffer+i, "\nType appli:Restaurant");
		break;
	case '2':
		i += sprintf(Buffer+i, "\nType appli:Supermarche");
		break;
	case '3':
		i += sprintf(Buffer+i, "\nType appli:Hotel");
		break;
	case '4':
		i += sprintf(Buffer+i, "\nType appli:Loc. voiture");
		break;
	case '5':
		i += sprintf(Buffer+i, "\nType appli:Cash advance");
		break;
	case '6':
		i += sprintf(Buffer+i, "\nType appli:Stat service");
		break;
	default:
		i += sprintf(Buffer+i, "\nType appli:Autre");
		break;
	}
	//i += sprintf(Buffer+i, "\x12");

//****************************//
	status = imp_buffer(Buffer,i);
	if(status!=OK)
	{
		Os__display(0,0,"Pb. Impression");
		Os__xget_key(5);
		CUTERMpaperFeed();
		return status;
	}
	i=0;
	memset(Buffer,0,sizeof(Buffer));
//****************************//

	//i += sprintf(Buffer+i, "\x0f");
	//Heure d'appel
	i += sprintf(Buffer+i, "\n\nHeure appel       :%.2sH%.2s",FicPara.Heure_appel,&FicPara.Heure_appel[2]);
	//Type Numerotation
	//i += sprintf(Buffer+i, "\nType num. :<#r#>%.9s",((FicPara.type_appel=='1') ? "Freq " : "Dec"));
	//Standard
	i += sprintf(Buffer+i, "\nStandard            :%.4s",FicPara.standard);
	//Nombre d'essai connexion
	i += sprintf(Buffer+i, "\nN Essai Connex      :%d",FicPara.Nbr_of_redial);
	//Délai interconnexion
	i += sprintf(Buffer+i, "\nDelai interconnex   :%d",FicPara.redial_delay);
	//Valeur Time out
	i += sprintf(Buffer+i, "\nTime Out            :%d",FicPara.TimeOut);
	//Tel auto Prod
	i += sprintf(Buffer+i, "\nTel.aut.prod:%.24s",FicPara.Tel_autor_pri);
	//X25 auto Prod
	i += sprintf(Buffer+i, "\nX25.aut.Prod:%.24s",FicPara.X25_autor_pri);
	//Tel auto Sec
	i += sprintf(Buffer+i, "\nTel.aut.sec.:%.24s",FicPara.Tel_autor_sec);
	//X25 auto Prod
	i += sprintf(Buffer+i, "\nX25.aut.sec.:%.24s",FicPara.X25_autor_sec);
	//Tel tlc Prod
	i += sprintf(Buffer+i, "\nTel.tlc.prod:%.24s",FicPara.Tel_telec_pri);
	//X25 tlc Prod
	i += sprintf(Buffer+i, "\nX25.tlc.Prod:%.24s",FicPara.X25_telec_pri);
	//Tel tlc Sec
	i += sprintf(Buffer+i, "\nTel.tlc.sec.:%.24s",FicPara.Tel_telec_sec);
	//X25 tlc Sec
	i += sprintf(Buffer+i, "\nX25.tlc.sec.:%.24s",FicPara.X25_telec_sec);
	//Tel serv App prim
//****************************//
	status = imp_buffer(Buffer,i);
	if(status!=OK)
	{
		Os__display(0,0,"Pb. Impression");
		Os__xget_key(5);
		CUTERMpaperFeed();
		return status;
	}
	i=0;
	memset(Buffer,0,sizeof(Buffer));
//****************************//
	i += sprintf(Buffer+i, "\nTel.Tlch.pri:%.24s",FicPara.Tel_telech_pri);
	//X25 serv App prim
	i += sprintf(Buffer+i, "\nX25.Tlch.pri:%.24s",FicPara.X25_telech_pri);
	//Tel serv App sec
	i += sprintf(Buffer+i, "\nTel.Tlch.sec:%.24s",FicPara.Tel_telech_sec);
	//X25 serv App sec
	i += sprintf(Buffer+i, "\nX25.Tlch.sec:%.24s",FicPara.X25_telech_sec);
	//Index clé maitresse
	i += sprintf(Buffer+i, "\n\nIndex MasterKey    :%.3s",FicPara.Index_MK);
	//Index clé mac
	i += sprintf(Buffer+i, "\nIndex MacKey       :%.3s",FicPara.Index_MacKey);
	//Index liste d'opposition
	i += sprintf(Buffer+i, "\nIndex liste Opp.   :%.4s",lop_gest.NumListe);
	//Version para tpe
	i += sprintf(Buffer+i, "\n\nVersion Pos param. :%.3s",FicPara.Version_Pos_Param);
	//Version table bin
	i += sprintf(Buffer+i, "\nVersion Table Bins :%.3s",FicPara.Version_Bin_Prefixe);
	//Version table emv
	i += sprintf(Buffer+i, "\nVersion Tables EMV :%.3s",FicPara.Version_Tab_Emv);
	//Version compte
	i += sprintf(Buffer+i, "\nVersion Compte     :%.3s",FicPara.Version_Compte_Dom);
	//Version logiciel
	i += sprintf(Buffer+i, "\nVersion Logiciel   :%.04s",FicPara.Version_appli);
	//Staus para
	i += sprintf(Buffer+i, "\n\nStatus parametres  :%.2s",((FicPara.status==OK) ? "OK" : "KO"));
	//Status liste d'opposition
	i += sprintf(Buffer+i, "\nStatus LOP         :%.2s",((lop_gest.stat==OK) ? "OK" : "KO"));
	//i += sprintf(Buffer+i, "\x12");


	//Mo 13/04/06
	if (FicPara.g_type_connexion=='R')
		i += sprintf(Buffer+i, "\nTYPE CONNEXION :%.3s","RTC");
	if (FicPara.g_type_connexion=='M')
		i += sprintf(Buffer+i, "\nTYPE CONNEXION :%.3s","GSM");
	if (FicPara.g_type_connexion=='G')
		i += sprintf(Buffer+i, "\nTYPE CONNEXION :%.4s","GPRS");
	if (FicPara.g_type_connexion=='I')
		i += sprintf(Buffer+i, "\nTYPE CONNEXION :%.4s","TCPIP");
	
	i += sprintf(Buffer+i, "\nIP :%.15s",FicPara.ip_gprs);
	i += sprintf(Buffer+i, "\nPort :%.6s",FicPara.port_gprs);
	i += sprintf(Buffer+i, "\nAPN :%.40s",FicPara.apn_gprs);

	i += sprintf(Buffer+i, "\nLOGIN GPRS :%.40s",FicPara.login_gprs);
	i += sprintf(Buffer+i, "\nPASSWORD GPRS:%.40s",FicPara.pwd_gprs);
	//Mo 13/04/06
	


	status = imp_buffer(Buffer,i);
	if(status!=OK)
	{
		Os__display(0,0,"Pb. Impression");
		Os__xget_key(5);
		CUTERMpaperFeed();
		return status;
	}

	CUTERMpaperFeed();
/*	Os__xlinefeed(4);*/
	return status;
}
L	Os__xprinta(PC message)
{
	return 0;
	Telium_File_t *printer ;

	printer = Telium_Fopen("PRINTER","w-");
	
	/* ##1 print the contents in condensed mode  */
	Telium_Fprintf(printer,"\x0f""%s \n",message);
	/* ##1 print the reading status in bold */
	Telium_Fprintf(printer,"\x12""\n");
	/* ##1 wait for print end */
	Telium_Ttestall ( PRINTER , 300 );
	Telium_Fclose(printer);
	return 0;
}



int Vprintline_Knife(PUC buffer,int *line)
{
	char buffer1[100];
	char c_line[4];

	memset(c_line,'\0',4);

	strcpy(buffer1,"030");
	strcat(buffer1,"002");
	sprintf(c_line,"%02d",(*line));
	strcat(buffer1,c_line);

	strcat(buffer1,"031");
	strcat(buffer1,"001");
	strcat(buffer1,"S");

	strcat(buffer1,"032");
	strcat(buffer1,"001");

	strcat(buffer1,"K"); //Knife

	memset(c_line,'\0',4);

	strcat(buffer1,"033");
	sprintf(c_line,"%03d",strlen(buffer));
	strcat(buffer1,c_line);
	strcat(buffer1,buffer);
	strcat(buffer1,"*");


	strcat(StructInfoCaisse[1].DP,buffer1);

  return OK;
}
int Vprintline_ajuste(char dp2,char dp3,PUC buffer,int line)
{
	char buffer1[100];
	char c_line[4];
	int	cmpt;


	cmpt=0;

	memset(c_line,'\0',4);

	strcpy(buffer1,"030");			cmpt += 3;
	strcat(buffer1,"002");			cmpt += 3;
	sprintf(c_line,"%02d",line);
	strcat(buffer1,c_line);			cmpt += 2;

	strcat(buffer1,"031");			cmpt += 3;
	strcat(buffer1,"001");			cmpt += 3;
	buffer1[cmpt]=dp2;				cmpt += 1;

	strcat(buffer1,"032");			cmpt += 3;
	strcat(buffer1,"001");			cmpt += 3;
	buffer1[cmpt]=dp3;				cmpt += 1;

	memset(c_line,'\0',4);

	strcat(buffer1,"033");
	sprintf(c_line,"%03d",strlen(buffer));
	strcat(buffer1,c_line);
	strcat(buffer1,buffer);
	strcat(buffer1,"*");


	strcat(StructInfoCaisse[1].DP,buffer1);


	Os__xprint(buffer1);
	Os__xprint(StructInfoCaisse[1].DP);

	return OK;
}
int Vprintline_caisse_gc(char w_font[4],char format[4],PUC buffer,int line)
{
	char buffer1[100];
	char c_line[4];

	memset(c_line,'\0',4);

	strcpy(buffer1,"030");
	strcat(buffer1,"002");
	sprintf(c_line,"%02d",line);
	strcat(buffer1,c_line);

	strcat(buffer1,"031");
	strcat(buffer1,"001");
	strcat(buffer1,"G");

	strcat(buffer1,"032");
	strcat(buffer1,"001");
	strcat(buffer1,"C");

	memset(c_line,'\0',4);

	strcat(buffer1,"033");
	sprintf(c_line,"%03d",strlen(buffer));
	strcat(buffer1,c_line);
	strcat(buffer1,buffer);
	strcat(buffer1,"*");


	strcat(StructInfoCaisse[1].DP,buffer1);


	Os__xprint(buffer1);
	Os__xprint(StructInfoCaisse[1].DP);

	return OK;
}
int Vprintline_caisse(char w_font[4],char format[4],PUC buffer,int line)
{
	char buffer1[100];
	char c_line[4];

	memset(c_line,'\0',4);

	strcpy(buffer1,"030");
	strcat(buffer1,"002");
	sprintf(c_line,"%02d",line);
	strcat(buffer1,c_line);

	strcat(buffer1,"031");
	strcat(buffer1,"001");
	strcat(buffer1,"S");

	strcat(buffer1,"032");
	strcat(buffer1,"001");
	strcat(buffer1,"G");

	memset(c_line,'\0',4);

	strcat(buffer1,"033");
	sprintf(c_line,"%03d",strlen(buffer));
	strcat(buffer1,c_line);
	strcat(buffer1,buffer);
	strcat(buffer1,"*");


	strcat(StructInfoCaisse[1].DP,buffer1);


	Os__xprint(buffer1);
	Os__xprint(StructInfoCaisse[1].DP);

	return OK;
}


uchar	ImprimeTra_ppad(uchar langue,uchar source,uchar dup)
{
	char handle[5];
	char C_JUSTIFY12[4],PRINT_NORM2[4],C_JUSTIFY2[4];

	uchar			data_time[13];
	char			affich_Cs[11];
	uchar			montant[13]  ;
	uchar			affiche_mont[14];
	uchar			Card_Nb[20];
	uchar			Exp;
	unsigned long	MontComp;
	char			NumSeqReliquat[6];
	char			Expp[5];
	Ushort ik;


	int l =0;

	if(source==COMMERCANT)
	{
		DebutLine = 0;
	}
	else if(source==CLIENT)
	{
		if(DebutLine >0)
			l = DebutLine;
	}


	memset(Expp,0,5);
	memcpy(Expp,"XXXX",4);


	memcpy(Expp,StructInfoPayment.Expired_Date,4);

	StructInfoPayment.waitpuce = 0;
	/*Aff_indicNO_OUI(FicPara.Langue);*/
//	CUMORE_CMI_AFFICHE_MESSAGE(94,32,1000);
	//Aff_mess(FicPara.Langue,94,0,0,0,0);
	//Aff_mess(FicPara.Langue,32,1,0,0,0);
	memset(affich_Cs,0x00,11) ;
	memset(montant,0x00,13) ;
	memset(affiche_mont,0x00,14) ;
   // Vopen_file(handle);
	//CMI Gras
	Os__xprint("ImprimeTra_ppa55");

	Vprintline_caisse_gc(C_JUSTIFY12,handle,"NAPS",l++);

	//Vprintlogosam("cmi.lgo"); samsam
	//DUPLICATA Gras	||	PRE-AUTORISATION	||	VAL.TRX.PRE-AUTO	||
	if(dup)
	{
		l++;
		Vprintline_caisse_gc(C_JUSTIFY12,handle,"DUPLICATA",l++);
	}


	if(StructInfoPayment.CodeFunction == PREAUTHCODE)
		Vprintline_caisse(C_JUSTIFY2,handle,"PRE-AUTORISATION",l++);
	else if(StructInfoPayment.CodeFunction == PREAUTHCOMPCODE)
		Vprintline_caisse(C_JUSTIFY2,handle,"VAL.TRX.PRE-AUTO",l++);
	else
		l++;

	//JJ/MM/AA	HH:MM:SS	+	N pos	+	N terminal	+	Ensei Com	+	Ville

	memcpy(data_time,StructInfoPayment.Transaction_DateTime,12) ;
	sprintf((PC)Buffer, "%.2s/%.2s/%.2s     %.2s:%.2s:%.2s",
		data_time+4,data_time+2,data_time,data_time+6,data_time+8,data_time+10);
	Vprintline_caisse(PRINT_NORM2,handle,Buffer,l++);


	sprintf((PC)Buffer, "%.15s",StructInfoPayment.Num_point_vente);
	Vprintline_caisse(PRINT_NORM2,handle,Buffer,l++);

	sprintf((PC)Buffer, "%.8s",FicPara.Num_Terminal);
	Vprintline_caisse(PRINT_NORM2,handle,Buffer,l++);

	sprintf((PC)Buffer, "%.21s",FicPara.Merchant_Name);
	Vprintline_caisse(PRINT_NORM2,handle,Buffer,l++);

	sprintf((PC)Buffer, "%.13s",FicPara.Merchant_City);
	l++;
	Vprintline_caisse(PRINT_NORM2,handle,Buffer,l++);


	//Nom du porteur
	//if(StructInfoPayment.Appli_label[0]!=0)
	if((StructInfoPayment.Appli_label[0]!=0)&&(StructInfoPayment.Appli_label[0]!=0x20))
	{
		sprintf((PC)Buffer, "%.24s",StructInfoPayment.Appli_aid+1);
		Vprintline_caisse(PRINT_NORM2,handle,Buffer,l++);
		sprintf((PC)Buffer, "%.16s",StructInfoPayment.Appli_label);
		Vprintline_caisse(PRINT_NORM2,handle,Buffer,l++);
		sprintf((PC)Buffer, "%.24s",StructInfoPayment.Nom_Prenom);
		Vprintline_caisse(PRINT_NORM2,handle,Buffer,l++);
	}
	else
	{
		sprintf((PC)Buffer, "%.24s",StructInfoPayment.Nom_Prenom);
		Vprintline_caisse(PRINT_NORM2,handle,Buffer,l++);
	}
	//Numéro de la carte
	memset(Card_Nb,0,20);
	memcpy(Card_Nb,&StructInfoPayment.Holder_Card[1],StructInfoPayment.Holder_Card[0]);


//****************************//
	for(ik=6;ik<strlen((PC)Card_Nb)-4;ik++)
		Card_Nb[ik] = '*';

	if(source==CLIENT)
	{
//		Card_Nb[strlen(Card_Nb)-1] = '*';
//		Card_Nb[strlen(Card_Nb)-2] = '*';
//		Card_Nb[strlen(Card_Nb)-3] = '*';
//		Card_Nb[strlen(Card_Nb)-4] = '*';

		for(ik=0;ik<strlen((PC)Card_Nb)-4;ik++)
			Card_Nb[ik] = '*';
	}

	l++;
	sprintf((PC)Buffer, "%s",Card_Nb);
	Vprintline_caisse(PRINT_NORM2,handle,Buffer,l++);
	//MM/AA	+	CARTE ETRANGERE||CARTE NATIONALE
  if(StructInfoPayment.Origine_carte == CARTE_NATIONALE)
		sprintf((PC)Buffer, "%.2s/%.2s CARTE NATIONALE",Expp+2,Expp);
	else
		sprintf((PC)Buffer, "%.2s/%.2s CARTE ETRANGERE",Expp+2,Expp);
	Vprintline_caisse(PRINT_NORM2,handle,Buffer,l++);

/*	if(StructInfoPayment.Origine_carte == CARTE_NATIONALE)
		 VprintlineCMI(PRINT_NORM,handle,"NATIONALE");
	else
		VprintlineCMI(PRINT_NORM,handle,"CARTE ETRANGERE");
*/
	//CERTIF EMV
	if((StructInfoPayment.Entry_Mode==TRS_EMV) ||(StructInfoPayment.Entry_Mode==TRS_SCT))
	{
		sprintf((PC)Buffer, "%.16s",StructInfoPayment.Certif);
		Vprintline_caisse(PRINT_NORM2,handle,Buffer,l++);
	}
	//XXX-C-YYY-L-Z
	memcpy(affich_Cs,StructInfoPayment.SeviceCode,3);
	//if(StructInfoPayment.forcage)
	if((StructInfoPayment.forcage)||(StructInfoPayment.Top_Forcage == TOP_FOR_REFFERAL))
   		affich_Cs[3]='1';
	else
   		affich_Cs[3]='0';
	//memcpy(affich_Cs+4,&lop_gest.NumListe[0],4) ;
	memcpy(affich_Cs+4,&StructInfoPayment.xpos_num_ln[0],4) ;
	if((StructInfoPayment.Entry_Mode==TRS_EMV) || (StructInfoPayment.Entry_Mode==TRS_SCT))
      	affich_Cs[8]='1';
	else if(StructInfoPayment.Entry_Mode==TRS_MAN)
		affich_Cs[8]='2';
	else
   	affich_Cs[8]='0';
	sprintf(affich_Cs+9,"%.02d",FicInsReg.Tab_Ins_reg[StructInfoPayment.Type_carte].Code_Prefixe);
	sprintf((PC)Buffer, "%.3s-%.1s-%.4s-%.1s-%.2s",affich_Cs,affich_Cs+3,affich_Cs+4,affich_Cs+8,affich_Cs+9);
	Vprintline_caisse(PRINT_NORM2,handle,Buffer,l++);

	//return OK;
	//Montant
	long_asc (montant, 12, &StructInfoPayment.Transaction_Amount);
	RemoveLeftZeros(montant) ;
	Exp=2;
	FormatAmount(montant,affiche_mont,Exp);
	l++;
	sprintf((PC)Buffer, "Montant: %.13s %.3s",affiche_mont,StructInfoPayment.Devise_acc.code_alph);
	l++;
	Vprintline_caisse(PRINT_NORM2,handle,Buffer,l++);
	//Num transaction//Num Pre autorisation
	if(StructInfoPayment.CodeFunction==PREAUTHCODE)
	{
		sprintf((PC)Buffer, "Num.Pre-autori  :%.6s",StructInfoPayment.Num_sequence);
	Vprintline_caisse(PRINT_NORM2,handle,Buffer,l++);}
	else
		{sprintf((PC)Buffer, "N.Transaction :%.6s",StructInfoPayment.Num_sequence);
		Vprintline_caisse(PRINT_NORM2,handle,Buffer,l++);
	}

	//Num autorisation
	sprintf((PC)Buffer, "N.Autorisation:%.6s",StructInfoPayment.Approval_Code);
	Vprintline_caisse(PRINT_NORM2,handle,Buffer,l++);
	//Stan

	sprintf((PC)Buffer, "STAN          :%.6s",StructInfoPayment.STAN);
	Vprintline_caisse(PRINT_NORM2,handle,Buffer,l++);

	if(!dup)
	{
		if(StructInfoPayment.xpos_fin_mois == '0')
			Vprintline_caisse(PRINT_NORM2,handle,"CREDIT",l++);
		else if(StructInfoPayment.xpos_fin_mois == '1')
			Vprintline_caisse(PRINT_NORM2,handle,"FIN DE MOIS",l++);
	}
	l++;

	sprintf((PC)Buffer,"%s",StructInfoPayment.xpos_libelle_port+1);
	Vprintline_caisse(PRINT_NORM2,handle,Buffer,l++);
	//DEBIT	||	ANNULATION	||	CREDIT
	sprintf((PC)Buffer, "%.80s",StructInfoPayment.Mess_porteur);
	Vprintline_caisse(PRINT_NORM2,handle,Buffer,l++);

	switch(StructInfoPayment.CodeFunction)
	{
   	case VOIDCODE:
		Vprintline_caisse_gc(C_JUSTIFY12,handle,"ANNULATION",l++);
		break;
	case REFUNDCODE:
		Vprintline_caisse_gc(C_JUSTIFY12,handle,"CREDIT",l++);
		break;
	case PREAUTHCODE:
		Vprintline_caisse_gc(C_JUSTIFY2,handle,"",l++);
		break;
	case VOID_PREAUTO_TOTAL_CODE:
	case VOID_PREAUTO_PARTIEL_CODE:
		Vprintline_caisse_gc(C_JUSTIFY12,handle,"ANNULATION PRE-AUTO",l++);
		break;
	default:
		if(StructInfoPayment.StatusAnnulAdjust)
			Vprintline_caisse_gc(C_JUSTIFY12,handle,"ANNULATION",l++);
		else
			Vprintline_caisse_gc(C_JUSTIFY12,handle,"DEBIT",l++);
		break;
	}

	//Signature client
	switch(StructInfoPayment.CodeFunction)
	{
	case VOID_PREAUTO_TOTAL_CODE:
	case VOID_PREAUTO_PARTIEL_CODE:
		break;

	default:
		if(StructInfoPayment.SignatureHolder==REQUIRED)
		{
   			if(source==CLIENT);
			else
			{
				Vprintline_caisse_gc(C_JUSTIFY2,handle,"SIGNATURE CLIENT",l++);
				//Vprintline_caisse(C_JUSTIFY2,handle,"\n\n\n",l++);
				l++;l++;l++;
			}
		}
		break;
	}


	//if(!dup)
	{
		for(ii = 63; (ii>4)&&(StructInfoPayment.Message2[ii]==' ');ii--);StructInfoPayment.Message2[ii]=0;
		sprintf((PC)Buffer, " %s",StructInfoPayment.Message2);
		Vprintline_caisse(PRINT_NORM2,handle,Buffer,l++);
	}
	Vprintline_caisse_gc(C_JUSTIFY2,handle,"-------------------",l++);
	if(langue==ENG)
	{
		Vprintline_caisse_gc(C_JUSTIFY2,handle,"TICKET TO SAVE",l++);
		if(source==CLIENT)
			Vprintline_caisse_gc(C_JUSTIFY2,handle,"Client Copy",l++);
		else
			Vprintline_caisse_gc(C_JUSTIFY2,handle,"Merchant Copy",l++);
	}
	else
	{
		Vprintline_caisse_gc(C_JUSTIFY2,handle,"TICKET A CONSERVER",l++);
		if(source==CLIENT)
			Vprintline_caisse_gc(C_JUSTIFY2,handle,"Copie Client",l++);
		else
			Vprintline_caisse_gc(C_JUSTIFY2,handle,"Copie Commercant",l++);
	}


//	Vprintline_caisse("K",handle,"",l++);
	if(source==COMMERCANT)
	{
		GlLine = l;
	}

	return OK;
}

uchar	ImpRese_ppad(char etareso)
{
	char handle[5];
	char C_JUSTIFY12[4],PRINT_NORM2[4],C_JUSTIFY2[4];
	int l=0;
	int	j=0;


	sprintf((PC)Buffer, "%s",&StructInfoPayment.xpos_libelle_port[1]);

	Vprintline_caisse(PRINT_NORM2,handle,Buffer,l++);
	Vprintline_caisse(C_JUSTIFY2,handle,"**************",l++);

	if(etareso)
		Vprintline_caisse(C_JUSTIFY2,handle,"RESEAU OK",l++);
	else
		Vprintline_caisse(C_JUSTIFY2,handle,"RESEAU KO",l++);

	Vprintline_caisse(C_JUSTIFY2,handle,"**************",l++);
	Vprintline_caisse(PRINT_NORM2,handle, "**",l++);

	return 0;
}

uchar	EchoMessage_ppad(char etareso)
{
/*
	char handle[5];
	char PRINT_NORM2[4],C_JUSTIFY2[4];
	int  l=0;
	char buffer_echo[80];
	int h_clock=0, yr, mth, day, hr, min, sec, wday;

	//return 0;
	
	memset(buffer_echo,'\0',sizeof(buffer_echo));
	clock2int(h_clock, (unsigned char *)NULL, &yr, &mth, &day, &hr, &min, &sec, &wday);
					
	if (etareso>1){

		memset(buffer_echo,'\0',sizeof(buffer_echo));
		clock2int(h_clock, (unsigned char *)NULL, &yr, &mth, &day, &hr, &min, &sec, &wday);
	
		if (etareso==2){//Veuillez introduire votre Carte Bancaire 001
		
			sprintf(buffer_echo,"001003105003%.3d%s004%.3d%s011003001014008%.2d%.2d%.4d015006%.2d%.2d%.2d",
			strlen(StructInfoCaisse[1].NCAI),StructInfoCaisse[1].NCAI,strlen(StructInfoCaisse[1].NS),StructInfoCaisse[1].NS,day,mth,yr,hr,min,sec);	
			strcat(buffer_echo,"!");
			send_caisse_echo(buffer_echo,strlen(buffer_echo),1);
			return 0;
		
		}else if(etareso==3){//Veuillez saisir² votre Code PIN sur le PINPAD 002
		
			sprintf(buffer_echo,"001003105003%.3d%s004%.3d%s011003002014008%.2d%.2d%.4d015006%.2d%.2d%.2d",
			strlen(StructInfoCaisse[1].NCAI),StructInfoCaisse[1].NCAI,strlen(StructInfoCaisse[1].NS),StructInfoCaisse[1].NS,day,mth,yr,hr,min,sec);	
			strcat(buffer_echo,"!");
			send_caisse_echo(buffer_echo,strlen(buffer_echo),1);
			return 0;
		
		}else if(etareso==4){//PIN incorrect, Veuillez réessayer la saisie de votre Code PIN sur le PINPAD 003

			sprintf(buffer_echo,"001003105003%.3d%s004%.3d%s011003003014008%.2d%.2d%.4d015006%.2d%.2d%.2d",
			strlen(StructInfoCaisse[1].NCAI),StructInfoCaisse[1].NCAI,strlen(StructInfoCaisse[1].NS),StructInfoCaisse[1].NS,day,mth,yr,hr,min,sec);	
			strcat(buffer_echo,"!");
			send_caisse_echo(buffer_echo,strlen(buffer_echo),1);
			return 0;
		}else if(etareso==6){//PIN incorrect, Attention, troisième tentative de saisie de votre Code PIN 004
			
			sprintf(buffer_echo,"001003105003%.3d%s004%.3d%s011003004014008%.2d%.2d%.4d015006%.2d%.2d%.2d",
			strlen(StructInfoCaisse[1].NCAI),StructInfoCaisse[1].NCAI,strlen(StructInfoCaisse[1].NS),StructInfoCaisse[1].NS,day,mth,yr,hr,min,sec);	
			strcat(buffer_echo,"!");
			send_caisse_echo(buffer_echo,strlen(buffer_echo),1);
			return 0;
		}else if(etareso==7){//Problème lecture carte, Veuillez retirer et insérer à nouveau votre carte bancaire 006

			sprintf(buffer_echo,"001003105003%.3d%s004%.3d%s011003006014008%.2d%.2d%.4d015006%.2d%.2d%.2d",
			strlen(StructInfoCaisse[1].NCAI),StructInfoCaisse[1].NCAI,strlen(StructInfoCaisse[1].NS),StructInfoCaisse[1].NS,day,mth,yr,hr,min,sec);	
			strcat(buffer_echo,"!");
			send_caisse_echo(buffer_echo,strlen(buffer_echo),1);
			return 0;							
		}else if(etareso==8){//Patienter SVP 008	// Y.E 29.08.2012 Ajout Echo message Patienter SVP

			sprintf(buffer_echo,"001003105003%.3d%s004%.3d%s011003007014008%.2d%.2d%.4d015006%.2d%.2d%.2d",
			strlen(StructInfoCaisse[1].NCAI),StructInfoCaisse[1].NCAI,strlen(StructInfoCaisse[1].NS),StructInfoCaisse[1].NS,day,mth,yr,hr,min,sec);	
			strcat(buffer_echo,"!");
			send_caisse_echo(buffer_echo,strlen(buffer_echo),1);
			return 0;							
		}else{//Veuillez Retirer Votre Carte  005
			
			sprintf(buffer_echo,"001003105003%.3d%s004%.3d%s011003005014008%.2d%.2d%.4d015006%.2d%.2d%.2d",
			strlen(StructInfoCaisse[0].NCAI),StructInfoCaisse[0].NCAI,strlen(StructInfoCaisse[0].NS),StructInfoCaisse[0].NS,day,mth,yr,hr,min,sec);	
			strcat(buffer_echo,"!");
			send_caisse_echo(buffer_echo,strlen(buffer_echo),1);
			return 0;
		}
	}
*/
	return 0;
}


uchar	Imprime_Param_caisse(void)
{
	char handle[5];
	char C_JUSTIFY12[4],PRINT_NORM2[4],C_JUSTIFY2[4];
	int l=0;
	int		h,j;
	US	k;
	uchar	status;

	Ushort	Index_Cmpte,Indice_fin;
	char	Reseau_accept[25];
	int h_clock=0, yr, mth, day, hr, min, sec, wday;
	StructInfoPayment.waitpuce = 0;
	/*Aff_indicNO_OUI(FicPara.Langue);*/
	CUMORE_CMI_AFFICHE_MESSAGE(94,32,1000);
	//Aff_mess(FicPara.Langue,94,0,0,0,0);
	//Aff_mess(FicPara.Langue,32,1,0,0,0);

	//CMI



	Vprintline_caisse(C_JUSTIFY12,handle,"NAPS",l++);
    strncpy(StructInfoCaisse[1].CRPIN,"000",3);


	l++;//Vprintline_caisse(C_JUSTIFY12,handle,"\n",l);

	Vprintline_caisse(C_JUSTIFY12,handle,"PARAMETRES",l++);


		clock2int(h_clock, (unsigned char *)NULL, &yr, &mth, &day, &hr, &min, &sec, &wday);
		sprintf((PC)Buffer,"%.2d/%.2d/%.4d      %.2d:%.2d:%.2d",
		day,
		mth,
		yr,
		hr,
		min,
		sec);
		l++;

	Vprintline_caisse(PRINT_NORM2,handle,Buffer,l++);

	//N Commercant
	Vprintline_caisse(PRINT_NORM2,handle,FicPara.Num_commerce,l++);

	//N point de vente
	Vprintline_caisse(PRINT_NORM2,handle,FicPara.Num_point_vente,l++);

	//N Terminal
	Vprintline_caisse(PRINT_NORM2,handle,FicPara.Num_Terminal,l++);

	//Enseigne
	Vprintline_caisse(PRINT_NORM2,handle,FicPara.Merchant_Name,l++);

	//MCC
	sprintf((PC)Buffer,"MCC :%.4s",FicPara.MCC);
	Vprintline_caisse(PRINT_NORM2,handle,Buffer,l++);

	//Compte Domiciliation
	if(Fic_compte_dom.Indice_fin)
	{
	Index_Cmpte = FicPara.Index_compte;
	sprintf((PC)Buffer,"Compte dom.:%.12s",Fic_compte_dom.tab_compte[Index_Cmpte].Compte);
	Vprintline_caisse(PRINT_NORM2,handle,Buffer,l++);
	}

	//Adresse
	Vprintline_caisse(PRINT_NORM2,handle,FicPara.Merchant_Adrees,l++);

	//Ville
	Vprintline_caisse(PRINT_NORM2,handle,FicPara.Merchant_City,l++);

	//Reseaux
	l++;
	sprintf((PC)Buffer,"Reseaux Acceptes :");
	Vprintline_caisse(PRINT_NORM2,handle,Buffer,l++);

   h = 0;
   memset(Reseau_accept,0,sizeof(Reseau_accept));
   if(FicPara.Indic_accept_carte[0]=='1')
   	h += sprintf(Reseau_accept+h,"-1");
   if(FicPara.Indic_accept_carte[1]=='1')
   	h += sprintf(Reseau_accept+h,"-2");
   if(FicPara.Indic_accept_carte[2]=='1')
   	h += sprintf(Reseau_accept+h,"-3");
   if(FicPara.Indic_accept_carte[3]=='1')
   	h += sprintf(Reseau_accept+h,"-4");
   if(FicPara.Indic_accept_carte[4]=='1')
   	h += sprintf(Reseau_accept+h,"-5");
   if(FicPara.Indic_accept_carte[5]=='1')
   	h += sprintf(Reseau_accept+h,"-6");
   if(FicPara.Indic_accept_carte[6]=='1')
   	h += sprintf(Reseau_accept+h,"-7");
   if(FicPara.Indic_accept_carte[7]=='1')
   	h += sprintf(Reseau_accept+h,"-8");
   if(FicPara.Indic_accept_carte[8]=='1')
   	h += sprintf(Reseau_accept+h,"-9");
   h += sprintf(Reseau_accept+h,"-");
   l++;
   sprintf((PC)Buffer, "%s",Reseau_accept);
   Vprintline_caisse(PRINT_NORM2,handle,Buffer,l++);

//****************************//

	Indice_fin = FicInsReg.Indice_fin;
	if(Indice_fin!=0)
	{
		C	TmpBuffer[24];
		for(k=0;k<Indice_fin;k++)
		{
			memset(Buffer,0,sizeof(Buffer));
			memset(TmpBuffer,0,24);
			memcpy(TmpBuffer,FicInsReg.Tab_Ins_reg[k].Libelle,strlen((PC)FicInsReg.Tab_Ins_reg[k].Libelle));
			l++;
			sprintf((PC)Buffer, "%s             %s",TmpBuffer,FicInsReg.Tab_Ins_reg[k].Num_point_vente);
		    Vprintline_caisse(PRINT_NORM2,handle,Buffer,l++);
		}
	}


//****************************//
	//Acceptance
	l++;
	 Vprintline_caisse(PRINT_NORM2,handle,"Acceptances :",l++);

    sprintf((PC)Buffer, "trs puce           :%.3s",((FicPara.Accept_puce=='1') ? "OUI" : "NON"));
	  Vprintline_caisse(PRINT_NORM2,handle,Buffer,l++);

	  sprintf((PC)Buffer, "trs man            :%.3s",((FicPara.Accept_Man=='1') ? "OUI" : "NON"));
	  Vprintline_caisse(PRINT_NORM2,handle,Buffer,l++);

	  sprintf((PC)Buffer, "trs annul.         :%.3s",((FicPara.Indic_annul=='1') ? "OUI" : "NON"));
	  Vprintline_caisse(PRINT_NORM2,handle,Buffer,l++);

	  sprintf((PC)Buffer, "trs credit.        :%.3s",((FicPara.Indic_Refund=='1') ? "OUI" : "NON"));
	  Vprintline_caisse(PRINT_NORM2,handle,Buffer,l++);

	  sprintf((PC)Buffer, "pre-autor.         :%.3s",((FicPara.Indic_PreAutor=='1') ? "OUI" : "NON"));
	  Vprintline_caisse(PRINT_NORM2,handle,Buffer,l++);

	  sprintf((PC)Buffer, "trs offline        :%.3s",((FicPara.Indic_Offline=='1') ? "OUI" : "NON"));
	  Vprintline_caisse(PRINT_NORM2,handle,Buffer,l++);

	  sprintf((PC)Buffer, "Forcage            :%.3s",((FicPara.Indic_forcage=='1') ? "OUI" : "NON"));
	  Vprintline_caisse(PRINT_NORM2,handle,Buffer,l++);

	  sprintf((PC)Buffer, "Controle fraude    :%.3s",((FicPara.Ctrl_fraude=='1') ? "OUI" : "NON"));
	  Vprintline_caisse(PRINT_NORM2,handle,Buffer,l++);

	  sprintf((PC)Buffer, "Chgmnt compte      :%.3s",((FicPara.Indic_Change_Cpte_Dom=='1') ? "OUI" : "NON"));
	  Vprintline_caisse(PRINT_NORM2,handle,Buffer,l++);

	  sprintf((PC)Buffer, "Cmmrc offshore     :%.3s",((FicPara.Indic_OffShort=='1') ? "OUI" : "NON"));
	  Vprintline_caisse(PRINT_NORM2,handle,Buffer,l++);

	switch(FicPara.Langue)
	{
   	case FRAN:
   		l++;
		sprintf((PC)Buffer, "Langue       :FRANCAIS");
		Vprintline_caisse(PRINT_NORM2,handle,Buffer,l++);
		break;
	case ENG:
		l++;
		sprintf((PC)Buffer, "Langue       :ANGLAIS");
		Vprintline_caisse(PRINT_NORM2,handle,Buffer,l++);
		break;
	case ARABE:;
		l++;
		sprintf((PC)Buffer, "Langue       :ARABE");
		Vprintline_caisse(PRINT_NORM2,handle,Buffer,l++);
		break;
	default:
		l++;
		sprintf((PC)Buffer, "Langue       :FRANCAIS");
		Vprintline_caisse(PRINT_NORM2,handle,Buffer,l++);
		break;
	}

	sprintf((PC)Buffer, "Nbr Max Trs Off-line:%02d",FicPara.Max_trs_off);
	Vprintline_caisse(PRINT_NORM2,handle,Buffer,l++);

	sprintf((PC)Buffer, "Nbr Max Avis Trs    :%02d",FicPara.Max_avis);
	Vprintline_caisse(PRINT_NORM2,handle,Buffer,l++);

l++;
	Vprintline_caisse(PRINT_NORM2,handle,"Monnaie acceptation:",l++);

	Indice_fin=Fic_Devise_Acc.Indice_fin;
	if(Indice_fin!=0)
	{
		for(j=0;j<Indice_fin;j++)
		sprintf((PC)Buffer, "%.3s                  %.3s",
         			Fic_Devise_Acc.TabDevise[j].code_num,Fic_Devise_Acc.TabDevise[j].code_alph);
	  Vprintline_caisse(PRINT_NORM2,handle,Buffer,l++);
	}

	//type appli
	switch(FicPara.Appli_type)
	{
   	case '0':
		Vprintline_caisse(PRINT_NORM2,handle,"Type appli:Achat",l++);
		break;
	case '1':
		Vprintline_caisse(PRINT_NORM2,handle,"Type appli:Restaurant",l++);
		break;
	case '2':
		Vprintline_caisse(PRINT_NORM2,handle,"Type appli:Supermarche",l++);
		break;
	case '3':
		Vprintline_caisse(PRINT_NORM2,handle,"Type appli:Hotel",l++);
		break;
	case '4':
		Vprintline_caisse(PRINT_NORM2,handle,"Type appli:Loc. voiture",l++);
		break;
	case '5':
		Vprintline_caisse(PRINT_NORM2,handle,"Type appli:Cash advance",l++);
		break;
	case '6':
		Vprintline_caisse(PRINT_NORM2,handle,"Type appli:Stat service",l++);
		break;
	default:
		Vprintline_caisse(PRINT_NORM2,handle,"Type appli:Autre",l++);
		break;
	}


//****************************//

	//Heure d'appel
	l++;
	sprintf((PC)Buffer, "Heure appel       :%.2sH%.2s",FicPara.Heure_appel,&FicPara.Heure_appel[2]);
	Vprintline_caisse(PRINT_NORM2,handle,Buffer,l++);

	//Nombre d'essai connexion
	sprintf((PC)Buffer, "N Essai Connex      :%d",FicPara.Nbr_of_redial);
	Vprintline_caisse(PRINT_NORM2,handle,Buffer,l++);

	//Délai interconnexion
  sprintf((PC)Buffer, "Delai interconnex   :%d",FicPara.redial_delay);
  Vprintline_caisse(PRINT_NORM2,handle,Buffer,l++);

	//Valeur Time out
	sprintf((PC)Buffer, "Time Out            :%d",FicPara.TimeOut);
	Vprintline_caisse(PRINT_NORM2,handle,Buffer,l++);

	//Tel auto Prod
	sprintf((PC)Buffer, "Tel.aut.prod:%.24s",FicPara.Tel_autor_pri);
	Vprintline_caisse(PRINT_NORM2,handle,Buffer,l++);

	//X25 auto Prod
	sprintf((PC)Buffer, "X25.aut.Prod:%.24s",FicPara.X25_autor_pri);
	Vprintline_caisse(PRINT_NORM2,handle,Buffer,l++);

	//Tel auto Sec
	sprintf((PC)Buffer, "Tel.aut.sec.:%.24s",FicPara.Tel_autor_sec);
	Vprintline_caisse(PRINT_NORM2,handle,Buffer,l++);

	//X25 auto Prod
	sprintf((PC)Buffer, "X25.aut.sec.:%.24s",FicPara.X25_autor_sec);
	Vprintline_caisse(PRINT_NORM2,handle,Buffer,l++);

	//Tel tlc Prod
	sprintf((PC)Buffer, "Tel.tlc.prod:%.24s",FicPara.Tel_telec_pri);
	Vprintline_caisse(PRINT_NORM2,handle,Buffer,l++);

	//X25 tlc Prod
	sprintf((PC)Buffer, "X25.tlc.Prod:%.24s",FicPara.X25_telec_pri);
	Vprintline_caisse(PRINT_NORM2,handle,Buffer,l++);

	//Tel tlc Sec
	sprintf((PC)Buffer, "Tel.tlc.sec.:%.24s",FicPara.Tel_telec_sec);
	Vprintline_caisse(PRINT_NORM2,handle,Buffer,l++);

	//X25 tlc Sec
	sprintf((PC)Buffer, "X25.tlc.sec.:%.24s",FicPara.X25_telec_sec);
	Vprintline_caisse(PRINT_NORM2,handle,Buffer,l++);

//****************************//

	//Tel serv App prim
	l++;
	sprintf((PC)Buffer, "Tel.Tlch.pri:%.24s",FicPara.Tel_telech_pri);
	Vprintline_caisse(PRINT_NORM2,handle,Buffer,l++);

	//X25 serv App prim
	sprintf((PC)Buffer, "X25.Tlch.pri:%.24s",FicPara.X25_telech_pri);
	Vprintline_caisse(PRINT_NORM2,handle,Buffer,l++);

	//Tel serv App sec
	sprintf((PC)Buffer, "Tel.Tlch.sec:%.24s",FicPara.Tel_telech_sec);
	Vprintline_caisse(PRINT_NORM2,handle,Buffer,l++);

	//X25 serv App sec
	sprintf((PC)Buffer, "X25.Tlch.sec:%.24s",FicPara.X25_telech_sec);
	Vprintline_caisse(PRINT_NORM2,handle,Buffer,l++);

	//Index clé maitresse
	l++;
	sprintf((PC)Buffer, "Index MasterKey    :%.3s",FicPara.Index_MK);
	Vprintline_caisse(PRINT_NORM2,handle,Buffer,l++);

	//Index clé mac
	sprintf((PC)Buffer, "Index MacKey       :%.3s",FicPara.Index_MacKey);
	Vprintline_caisse(PRINT_NORM2,handle,Buffer,l++);

	//Version para tpe
	l++;
	sprintf((PC)Buffer, "Version Pos param. :%.3s",FicPara.Version_Pos_Param);
	Vprintline_caisse(PRINT_NORM2,handle,Buffer,l++);

	//Version table bin
	sprintf((PC)Buffer, "Version Table Bins :%.3s",FicPara.Version_Bin_Prefixe);
	Vprintline_caisse(PRINT_NORM2,handle,Buffer,l++);

	//Version table emv
	sprintf((PC)Buffer, "Version Tables EMV :%.3s",FicPara.Version_Tab_Emv);
	Vprintline_caisse(PRINT_NORM2,handle,Buffer,l++);

	//Version compte
	sprintf((PC)Buffer, "Version Compte     :%.3s",FicPara.Version_Compte_Dom);
	Vprintline_caisse(PRINT_NORM2,handle,Buffer,l++);

	//Version logiciel
	sprintf((PC)Buffer, "Version Logiciel   :%.04s",FicPara.Version_appli);
	Vprintline_caisse(PRINT_NORM2,handle,Buffer,l++);

	//Staus para
	l++;
	sprintf((PC)Buffer, "Status parametres  :%.2s",((FicPara.status==OK) ? "OK" : "KO"));
	status=Vprintline_caisse(PRINT_NORM2,handle,Buffer,l++);

//	sprintf((PC)Buffer, "\n");

	return status;
}

short    maintenance_ppad(char state[4])
{
/*
	Init_Param();
	Init_Lop();
	Init_Tra();
	Init_Pre();
*/
	Init_Tra();
	strcpy(state,"000");
	return 0;
}

int controle_paiement(char * WaitedTM)
{

	int resultat=1;

	return resultat;// Pour test

	if(!memcmp(WaitedTM,"002",3))
	{
		if(memcmp(StructInfoCaisse[0].TM,"002",3))
		{
			APEMV_UI_MessageDisplayDelay ("MESSAGE RECU ERRONE\n-200", 2);
			resultat=0;
		}
		else if(memcmp(StructInfoCaisse[0].NS,SaveLastInfoCaisse.NS,3))
		{
			APEMV_UI_MessageDisplayDelay ("MESSAGE RECU ERRONE\n-201", 2);
			resultat=0;
		}
		else if(memcmp(StructInfoCaisse[0].STAN,SaveLastInfoCaisse.STAN,6))
		{
			APEMV_UI_MessageDisplayDelay ("MESSAGE RECU ERRONE\n-202", 2);
			resultat=0;
		}

	}
	else if(!memcmp(WaitedTM,"004",3))
	{
		if(memcmp(StructInfoCaisse[0].TM,"004",3))
		{
			APEMV_UI_MessageDisplayDelay ("MESSAGE RECU ERRONE\n-400", 2);
			resultat=0;
		}
		else if(memcmp(StructInfoCaisse[0].NS,SaveLastInfoCaisse.NS,3))
		{
			APEMV_UI_MessageDisplayDelay ("MESSAGE RECU ERRONE\n-401", 2);
			resultat=0;
		}
		else if(memcmp(StructInfoCaisse[0].STAN,SaveLastInfoCaisse.STAN,6))
		{
			APEMV_UI_MessageDisplayDelay ("MESSAGE RECU ERRONE\n-402", 2);
			resultat=0;
		}

	}
	return resultat;
}

/*****************/
