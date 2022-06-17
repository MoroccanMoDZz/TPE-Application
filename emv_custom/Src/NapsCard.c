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

void Crypto__3DesEnc(unsigned char *Input_Data,unsigned char *Output_Data,unsigned char *bkey); //KT
void Crypto__3DesDec(unsigned char *Input_Data,unsigned char *Output_Data,unsigned char *bkey); //KT

static Telium_File_t *hKbdReadCard;
/**************************************************************************
*                                                                         *
*       SOUS-PROGRAMME :  PresentCard                                     *
*                                                                         *
***************************************************************************
*                                                                         *
*       Fonction :  attente Lecture d'une carte porteur ou                *
*                   de la touche d'abandon                                *
*                                                                         *
*       Entree :    adr buffer de recuperation de la piste                *
*                                                                         *
*                                                                         *
*       Sortie :    status  OK     	carte lue correspond au commercant	  *
*                           ABORTED  abandon sur annulation               *
*							SAIS_MAN saisie de carte ogligatoire		  *
***************************************************************************/
uint8 PresentCard(uchar *Buffer,char saisie)
{
	uchar	l_l_return;
	int		l_l_ret_cmt;

	l_l_ret_cmt = choix_mode_tra(saisie);

	if(l_l_ret_cmt == 1)
		return SAIS_EMV;
	else if(l_l_ret_cmt == 2)
		return SAIS_MAN;
	else
		l_l_return	=	Read_Card(Buffer,CARTE_CLIENT);

	if(l_l_return == OK)
		return SAIS_MAG;
	else
		return l_l_return;
}
/**************************************************************************
*                                                                         *
*       SOUS-PROGRAMME :  Read_Card                                       *
*                                                                         *
***************************************************************************
*                                                                         *
*       Fonction :  attente Lecture d'une carte porteur ou                *
*                   de la touche d'abandon                                *
*                                                                         *
*       Entree :    adr buffer de recuperation de la piste                *
*                                                                         *
*                                                                         *
*       Sortie :    status    OK     	carte lue correspond au commercant *
*                             ABORTED  abandon sur annulation             *
*										SAIS_MAN saisie de carte ogligatoire        *
***************************************************************************/
uchar  Read_Card(uchar    *buffer,uchar type_carte)
{

	//APEMV_UI_MessageDisplayDelay((char *)"Read_Card", 2);
	if(type_carte==CARTE_COM)
	{
		CUMORE_CMI_AFFICHE_MESSAGE(99,100,1000);
		CarteCommercant();
	}
	else
	{
		CUMORE_CMI_AFFICHE_MESSAGE(101,102,1000);
		CarteClient();
	}
	switch(StructInfoPayment.Event)
	{
	case EVENTCARD:
		StructInfoPayment.Event=OK;
		memcpy( &buffer[0],StructInfoPayment.EventCard,sizeof(StructInfoPayment.EventCard));
		break;
	case EVENTILLISIBLE:
		StructInfoPayment.Event=ILLISIBLE;
		break;
	case EVENTVAL:
		StructInfoPayment.Event=SAIS_MAN;
		break;
	case EVENTESC:
	case EVENTABORTED:
	case EVENTTIMER:
	default:
		StructInfoPayment.Event=ABORTED;
		break;
	}
	return StructInfoPayment.Event;
}
/****************************************************************************
*                                                                           *
*      SOUS-PROGRAMME : Recup_Info_Card	                                    *
*                                                                           *
*****************************************************************************
*                                                                           *
*      FONCTION :  incorporation des xxdata iso dans la structure           *
*                  de traitement de la transaction                          *
*                                                                           *
*      ENTREE   :  structure EveIn                                          *
*                                                                           *
*      SORTIE   :  status                                                   *
*                                                                           *
****************************************************************************/

uchar   Recup_Info_Card(uchar *p_puc_buffer)
{
	short	l_c_i;
	short	l_c_j;
	short	l_c_k;

	UC	l_s_Lg_iso1,l_s_lg_nom,i,j;
	UC	l_uc_Iso1[80];
	UC	l_uc_nom_porteur[27];

//	 Lecture de la piste ISO1 pour recuperee le nom du porteur
	memset(l_uc_nom_porteur,'\0',27);
//	if((*(p_puc_buffer+1) == 0)&&(*p_puc_buffer != 0))
	{
		l_s_Lg_iso1 = 50;//A VOIR
		memset(l_uc_Iso1,'\0',80);
		memcpy(l_uc_Iso1, p_puc_buffer, l_s_Lg_iso1);
		for(i=0;((i<l_s_Lg_iso1)&&(l_uc_Iso1[i]!=0x5e));i++);
		if(l_uc_Iso1[i]==0x5e)
		{
			i++;
			j=i;
			for(l_s_lg_nom=0;((j<l_s_Lg_iso1)&&(l_uc_Iso1[j]!=0x5e));j++,l_s_lg_nom++);
			if((l_uc_Iso1[j]==0x5e)&&(l_s_lg_nom<27))
			if(l_uc_Iso1[j]==0x5e)
				memcpy(l_uc_nom_porteur,&l_uc_Iso1[i],l_s_lg_nom);
		}
	}
	memcpy(StructInfoPayment.Nom_Prenom,l_uc_nom_porteur,27);

	//Os__xprintaa("Recup_Info_Card emv");

    for (l_c_k=0; (l_c_k <l_s_Lg_iso1) && (*(&p_puc_buffer[l_s_Lg_iso1+1+l_c_k])!='F');l_c_k += 1);
	//récuperation du numéro de la carte,sa longueur et la longueur de la piste ISO2

	StructInfoPayment.Compressed_Iso2[0]=l_c_k;

    /* controle lg piste ISO (max 40) */
    if (StructInfoPayment.Compressed_Iso2[0]> 37)//	à valider on a marqué 37 au lieu de 40
	{
    	//APEMV_UI_MessageDisplayDelay ("Recup_Info_Card\n\nCompressed_Iso2>37", 1);
		return(NOT_TREATED);
	}
	//compactage si nécessaire

	//StructInfoPayment.Normal_Iso2[0]=*(p_puc_buffer+p_puc_buffer[0]+6);
	StructInfoPayment.Normal_Iso2[0]=StructInfoPayment.Compressed_Iso2[0];
    for (l_c_i=1;
						(l_c_i <l_c_k)
						&&(*( &p_puc_buffer[l_s_Lg_iso1+l_c_i])!='=')
						&&(*( &p_puc_buffer[l_s_Lg_iso1+l_c_i])!='d')
						&&(*( &p_puc_buffer[l_s_Lg_iso1+l_c_i])!='D');
		l_c_i += 1)
	{
		StructInfoPayment.Holder_Card[l_c_i] = '1';

    	memcpy((char*)&StructInfoPayment.Holder_Card[l_c_i],(char*)&p_puc_buffer[l_s_Lg_iso1+l_c_i],1);
		memcpy((char*)&StructInfoPayment.Normal_Iso2[l_c_i],(char*)&p_puc_buffer[l_s_Lg_iso1+l_c_i],1);
	};
	p_puc_buffer[l_s_Lg_iso1+l_c_i]='=';
	compression((PUC)&StructInfoPayment.Compressed_Iso2[1],&p_puc_buffer[l_s_Lg_iso1+1],38);
	/****/
/*	ShowTrace_Mouna("ad0");
	imp_buffer(StructInfoPayment.Compressed_Iso2,StructInfoPayment.Compressed_Iso2[0]);
	bcd_asc(iso2,&StructInfoPayment.Compressed_Iso2[1],20);
	imp_buffer(iso2,20);*/
	/****/

	StructInfoPayment.Holder_Card[0]=l_c_i-1;

	for(l_c_j=l_c_i;l_c_j<l_c_k+1;l_c_j+=1)
		memcpy((char*)&StructInfoPayment.Normal_Iso2[l_c_j],(char*)&p_puc_buffer[l_s_Lg_iso1+l_c_j],1);

/****/
/*	imp_buffer(StructInfoPayment.Normal_Iso2,StructInfoPayment.Normal_Iso2[0]);*/
/****/

	// Traitement clé de luhn
	if(luhn_key_ok(StructInfoPayment.Holder_Card)!=TRUE)
	{
		ShowTrace("Card 6");
		CUMORE_CMI_AFFICHE_MESSAGE(132,133,1000);
		return(NOT_TREATED);
	}

	/* controle lg numero porteur de 13 a 19 */
	if ( (l_c_i < 14) || (l_c_i > 20) )
	{
		ShowTrace("Card 7");
		return(NOT_TREATED);
	}

	l_c_i++;
    /* lecture date fin de validite */
	strncpy((PC)StructInfoPayment.Expired_Date,(PC)(&p_puc_buffer[l_s_Lg_iso1+l_c_i]),4);
	l_c_i += 4;

    /* lecture code service  */
	if(
		(StructInfoPayment.Compressed_Iso2[0]==29)&&
		((memcmp(&StructInfoPayment.Holder_Card[1],"34",2)==0)||(memcmp(&StructInfoPayment.Holder_Card[1],"37",2)==0))
	   )
		memset((PC)StructInfoPayment.SeviceCode,0,3);
	else
		strncpy((PC)StructInfoPayment.SeviceCode,(PC)(&p_puc_buffer[l_s_Lg_iso1+l_c_i]),3);

	return(OK);
}
/****************************************************************************
*                                                                           *
*      SOUS-PROGRAMME : Recup_Info_badge	                                *
*                                                                           *
*****************************************************************************
*                                                                           *
*      FONCTION :  incorporation des xxdata iso dans la structure           *
*                  de traitement de la transaction                          *
*                                                                           *
*      ENTREE   :  structure EveIn                                          *
*                                                                           *
*      SORTIE   :  status                                                   *
*                                                                           *
****************************************************************************/
UC   Recup_Info_badge(PUC p_puc_buffer,UC init)
{
	PUC PtIso2;
	UC	Num_point_vente[15+3];
	UC	Num_commerce[15+3];
	UC	Num_Terminal[12+3];
	UC	uc_key[4];
	UC 	*pSep1,*pSep2,*pSep3;

	pSep1=pSep2=pSep3=NULL ;
	memset(Num_point_vente,0,18) ;
	memset(Num_commerce,0,18) ;
	memset(Num_Terminal,0,15) ;
	memset(uc_key,0,4) ;

	PtIso2=p_puc_buffer+1;

	ShowTrace((char *)PtIso2);
	pSep1 =(PUC)strchr((PC)PtIso2,'D') ;
	if((pSep1)&&((pSep1-PtIso2)<16))
	{
		memcpy(&Num_commerce[1],PtIso2,pSep1-PtIso2);
		Num_commerce[0]=pSep1-PtIso2+1 ;
	}
	else
	{
		ShowTrace("111");
		return KO ;
	}
	pSep2=(PUC)strchr((PC)pSep1+1,'D');
	if((pSep2)&&((pSep2-pSep1-1)<16))
	{
		memcpy(&Num_point_vente[1],pSep1+1,pSep2-pSep1-1);
		Num_point_vente[0]=pSep2-pSep1 ;
	}
	else
	{
		ShowTrace("222");
		return KO ;
	}

	pSep3=(PUC)strchr((PC)pSep2+1,'D') ;
	if((pSep3)&&((pSep3-pSep2-1)<9))
	{
		memcpy(&Num_Terminal[1],pSep2+1,pSep3-pSep2-1);
		Num_Terminal[0]= pSep3-pSep2 ;
	}
	else
	{
		ShowTrace("333");
		return KO ;
	}

	memcpy(uc_key,pSep3+1,3);
	Num_commerce[strlen((PC)Num_commerce)]=uc_key[0];
	Num_point_vente[strlen((PC)Num_point_vente)]=uc_key[1];
	Num_Terminal[strlen((PC)Num_Terminal)]=uc_key[2];

//	// Traitement clé de luhn
//	if(  (luhn_key_ok(Num_commerce)!=TRUE)
//		||(luhn_key_ok(Num_point_vente)!=TRUE)
//		||(luhn_key_ok(Num_Terminal)!=TRUE)
//		)
//	{
//		//if(g_trace)Os__xprint("LUHN KEY INCORRECT");
//		return KO;
//	}

	if((FicPara.Num_commerce[0]!=0)&&
		(FicPara.Num_point_vente[0]!=0)&&
		(FicPara.Num_Terminal[0]!=0)
		)
	{
//				Os__xprinta("33333.01");
//				Os__xprinta(FicPara.Num_commerce);
//				Os__xprinta(FicPara.Num_point_vente);
//				Os__xprinta(FicPara.Num_Terminal);
//
//				Os__xprinta("33333.02");
//				Os__xprinta(&Num_commerce[1]);
//				Os__xprintd(Num_commerce[0]-1);
//				Os__xprinta(&Num_point_vente[1]);
//				Os__xprinta(&Num_Terminal[1]);

		//		if((memcmp(FicPara.Num_commerce,&Num_commerce[1],Num_commerce[0]-1))
		//			||(memcmp(FicPara.Num_point_vente,&Num_point_vente[1],Num_point_vente[0]-1))
		//			||(memcmp(FicPara.Num_Terminal,&Num_Terminal[1],Num_Terminal[0]-1))
		//			)
				if(memcmp(FicPara.Num_commerce,&Num_commerce[1],Num_commerce[0]-1))
			{
//				Os__xprinta("444");
				return KO ;
			}else {
//				Os__xprinta("else 444");
			}

	}
/*
	if(init)
	{
		if(tst_fic_vide()==OK)
		{
			//Os__saved_set ((uchar *)&FicPara, 0, 45);
			if((Num_commerce[0]-1)<16)
			{
				Os__saved_set ((uchar *)&FicPara.Num_commerce, 0, sizeof(FicPara.Num_commerce));
				Os__saved_copy((uchar *)&Num_commerce[1],(uchar *)&FicPara.Num_commerce,Num_commerce[0]-1) ;
			}
			else
			{
				Os__xprinta("LG. NUM. COMM. INCORRECT");
				return KO;
			}

			if((Num_point_vente[0]-1)<16)
			{
				Os__saved_set ((uchar *)&FicPara.Num_point_vente, 0, sizeof(FicPara.Num_point_vente));
				Os__saved_copy((uchar *)&Num_point_vente[1],(uchar *)&FicPara.Num_point_vente,Num_point_vente[0]-1) ;
			}
			else
			{
				Os__xprinta("LG. PT. VENTE INCORRECT");
				return KO;
			}

			if((Num_Terminal[0]-1)==8)
			{
				Os__saved_set ((uchar *)&FicPara.Num_Terminal, 0, sizeof(FicPara.Num_Terminal));
				Os__saved_copy((uchar *)&Num_Terminal[1],(uchar *)&FicPara.Num_Terminal,Num_Terminal[0]-1) ;
			}
			else
			{
				Os__xprinta("LG. NUM. TERM. INCORRECT");
				return KO;
			}
		}
	}
	else
	{
				Os__xprinta("33333.03");
				Os__xprinta(FicPara.Num_commerce);
				Os__xprinta(FicPara.Num_point_vente);
				Os__xprinta(FicPara.Num_Terminal);

				Os__xprinta("33333.04");
				Os__xprinta(&Num_commerce[1]);
				Os__xprinta(&Num_point_vente[1]);
				Os__xprinta(&Num_Terminal[1]);

		//		if((memcmp(FicPara.Num_commerce,&Num_commerce[1],Num_commerce[0]-1))
		//			||(memcmp(FicPara.Num_point_vente,&Num_point_vente[1],Num_point_vente[0]-1))
		//			||(memcmp(FicPara.Num_Terminal,&Num_Terminal[1],Num_Terminal[0]-1))
		//			)
			if(memcmp(FicPara.Num_point_vente,&Num_point_vente[1],Num_point_vente[0]-1))
			{
				ShowTrace("555");
				ShowTrace((PC)FicPara.Num_commerce);
				ShowTrace((PC)FicPara.Num_point_vente);
				ShowTrace((PC)FicPara.Num_Terminal);
				ShowTrace((PC)&Num_commerce[1]);
				ShowTrace((PC)&Num_point_vente[1]);
				ShowTrace((PC)&Num_Terminal[1]);
				return KO ;
			}else {
				Os__xprinta("else 555");
			}

	}
	*/
	return(OK);
}

/****************************************************************************
*                                                                           *
*      SOUS-PROGRAMME : BinControl                                          *
*                                                                           *
*****************************************************************************
*                                                                           *
*      FONCTION :  Controle des bin                                         *
*                                                                           *
*      ENTREE   :  Card Number(Card[0] = Lg card)                           *
*                                                                           *
*      SORTIE   :  status                                                   *
*                                                                           *
****************************************************************************/
uchar	BinControl(uchar*	Card)
{
	Ushort	i;
	Ushort	indexfin ;
	uchar	OffSet,k;

	if((indexfin=FicBin.Indice_fin)==0)
	{
		//if(g_trace)Os__xprint("(NO BIN IN POS)");
//		Os__xprintaa("4444");
		return KO ;
	}
	i=0 ;
	while(i<indexfin)
	{
		if((memcmp(Card+1,FicBin.Tab_Bin[i].Bin_Debut,11)>=0)&&(memcmp(Card+1,FicBin.Tab_Bin[i].Bin_Fin,11)<=0))
			break ;
		i++ ;
	}
	if(i==indexfin)
	{
		if(g_trace)Os__xprint("(BIN NOT IN POS)");
//		Os__xprintaa("5555");
		return KO;
	}

	if(FicBin.Tab_Bin[i].LgCard_Type=='F')
	{
		if((FicBin.Tab_Bin[i].LgCard_Min!=Card[0])&&(FicBin.Tab_Bin[i].LgCard_Max!=Card[0]))
		{
			if(g_trace)Os__xprint("(LG CARD NOT ACCEPTED)");
//			Os__xprintaa("6666");
			return KO;
		}
	}
	else
	{
		if((Card[0] < FicBin.Tab_Bin[i].LgCard_Min)||(Card[0] > FicBin.Tab_Bin[i].LgCard_Max))
		{
			if(g_trace)Os__xprint("(LG CARD NOT ACCEPTED)");
//			Os__xprintaa("7777");
			return KO;
		}
	}

	if(FicBin.Tab_Bin[i].OffSet_Octet36!= 0)
	{
		//OffSet = FicBin.Tab_Bin[i].OffSet_Octet36;
		OffSet = FicBin.Tab_Bin[i].OffSet_Octet36 -1;
		if(FicPara.Indic_accept_carte[OffSet]!='1')
		{
			if(g_trace)Os__xprint("RESEAU DE LA CARTE NON ACCEPTE");
//			Os__xprintaa("7777");
			return KO;
		}
	}

	/* Analyse code service */
	if(
		(FicBin.Tab_Bin[i].Code_Service[0]!='!')&&
		(FicBin.Tab_Bin[i].Code_Service[0]!=StructInfoPayment.SeviceCode[0])
	  )
		StructInfoPayment.Flag_Dem_Aut=1 ;
	if(
		(FicBin.Tab_Bin[i].Code_Service[1]!='!')&&
		(FicBin.Tab_Bin[i].Code_Service[1]!=StructInfoPayment.SeviceCode[1])
	  )
		StructInfoPayment.Flag_Dem_Aut=1 ;
	if(
		(FicBin.Tab_Bin[i].Code_Service[2]!='!')&&
		(FicBin.Tab_Bin[i].Code_Service[2]!=StructInfoPayment.SeviceCode[2])
	  )
		StructInfoPayment.Flag_Dem_Aut=1 ;


	if((FicBin.Tab_Bin[i].Gest_Pin=='1')||(FicBin.Tab_Bin[i].Gest_Pin=='3'))
	{
		//ShowTrace_Mouna("11");
		StructInfoPayment.Pin_Required = 1 ;
	}
	else if(FicBin.Tab_Bin[i].Gest_Pin=='0')
	{
		if(
			(StructInfoPayment.SeviceCode[2]=='0')||
			(StructInfoPayment.SeviceCode[2]=='5')||
			(StructInfoPayment.SeviceCode[2]=='6')||
			(StructInfoPayment.SeviceCode[2]=='7')
		  )
		{
			//ShowTrace_Mouna("22");
			StructInfoPayment.Pin_Required = 1 ;
		}
	}

	StructInfoPayment.Plafond = FicBin.Tab_Bin[i].Plafond ;
	StructInfoPayment.Langue = FicBin.Tab_Bin[i].Langue ;

	k=0;
	while(k<FicPrefixe.Indice_fin)
	{
		if(FicInsReg.Tab_Ins_reg[k].Code_Prefixe == FicBin.Tab_Bin[i].Code_Prefixe)
			break;
		k++;
	}
	if(k==FicPrefixe.Indice_fin)
	{
//		if(g_trace)Os__xprint("PREFIXE NOT FOUND");
//		Os__xprintaa("8888");
		return KO;
	}
	StructInfoPayment.Type_carte = k;
	StructInfoPayment.Origine_carte = CARTE_NATIONALE;//modif 04/04
	memcpy(StructInfoPayment.Num_point_vente,FicInsReg.Tab_Ins_reg[k].Num_point_vente,15);
	memcpy(StructInfoPayment.Message1,FicInsReg.Tab_Ins_reg[k].Message1,64);
	memcpy(StructInfoPayment.Message2,FicInsReg.Tab_Ins_reg[k].Message2,64);
	StructInfoPayment.Code_Prefixe = FicInsReg.Tab_Ins_reg[k].Code_Prefixe;
	return OK ;
}


/****************************************************************************
*                                                                           *
*      SOUS-PROGRAMME : PrefixControl                                       *
*                                                                           *
*****************************************************************************
*                                                                           *
*      FONCTION :  Controle des prefixe                                     *
*                                                                           *
*      ENTREE   :  Card Number(Card[0] = Lg card)                           *
*                                                                           *
*      SORTIE   :  status                                                   *
*                                                                           *
****************************************************************************/
uchar	PrefixControl(uchar*	Card)
{
	Ushort	i,k;
	Ushort	indexfin ;
	uchar	OffSet;

	if((indexfin=FicPrefixe.Indice_fin)==0)
	{
		if(g_trace)Os__xprint("NO PREFIXE IN POS");
//		Os__xprintaa("8888");
		return KO;
	}
	i=0 ;
	while(i<indexfin)
	{
		if(
      	(memcmp(Card+1,FicPrefixe.Tab_Prefixe[i].Pre_Debut,11)>=0)&&
         (memcmp(Card+1,FicPrefixe.Tab_Prefixe[i].Pre_Fin,11)<=0)
        )
			break ;
		i++ ;
	}
	if(i==indexfin)
	{
		if(g_trace)Os__xprint("PREFIXE NOT FOUND");
//		Os__xprintaa("9999");
		return KO ;
	}
	if(FicPrefixe.Tab_Prefixe[i].LgCard_Type=='F')
	{
		if((FicPrefixe.Tab_Prefixe[i].LgCard_Min!=Card[0])&&(FicPrefixe.Tab_Prefixe[i].LgCard_Max!=Card[0]))
		{
			if(g_trace)Os__xprint("(LG CARD NOT ACCEPTED)");
//			Os__xprintaa("98989");
			return KO;
		}
	}
	else
	{
		if((Card[0] < FicPrefixe.Tab_Prefixe[i].LgCard_Min)||(Card[0] > FicPrefixe.Tab_Prefixe[i].LgCard_Max))
		{
			if(g_trace)Os__xprint("(LG CARD NOT ACCEPTED)");
//			Os__xprinta("121212");
			return KO;
		}
	}

	if(FicPrefixe.Tab_Prefixe[i].OffSet_Octet36!= 0)
	{
		//OffSet = FicPrefixe.Tab_Prefixe[i].OffSet_Octet36;
		OffSet = FicPrefixe.Tab_Prefixe[i].OffSet_Octet36 -1;
		if(FicPara.Indic_accept_carte[OffSet]!='1')
		{
			if(g_trace)Os__xprint("RESEAU DE LA CARTE NON ACCEPTE");
//			Os__xprintaa("131313");
			return KO;
		}
	}

	if((FicPrefixe.Tab_Prefixe[i].Gest_Pin=='1')||(FicPrefixe.Tab_Prefixe[i].Gest_Pin=='3'))
	{
		//ShowTrace_Mouna("33");
		StructInfoPayment.Pin_Required = 1 ;
	}
	else if(FicPrefixe.Tab_Prefixe[i].Gest_Pin=='0')
	{
		if(
			(StructInfoPayment.SeviceCode[2]=='0')||
			(StructInfoPayment.SeviceCode[2]=='5')||
			(StructInfoPayment.SeviceCode[2]=='6')||
			(StructInfoPayment.SeviceCode[2]=='7')
		  )
		{
			//ShowTrace_Mouna("44");
			StructInfoPayment.Pin_Required = 1 ;
		}
	}

	k = FicPrefixe.Tab_Prefixe[i].Indice_Ins_Reg;
	StructInfoPayment.Plafond = FicInsReg.Tab_Ins_reg[k].Plafond ;
	StructInfoPayment.Type_carte = (C)k;
	memcpy(StructInfoPayment.Num_point_vente,FicInsReg.Tab_Ins_reg[k].Num_point_vente,15);
	memcpy(StructInfoPayment.Message1,FicInsReg.Tab_Ins_reg[k].Message1,64);
	memcpy(StructInfoPayment.Message2,FicInsReg.Tab_Ins_reg[k].Message2,64);
	StructInfoPayment.Code_Prefixe = FicInsReg.Tab_Ins_reg[k].Code_Prefixe;
	return OK ;
}
/****************************************************************************
*                                                                           *
*      SOUS-PROGRAMME : BinPrefixControl                                    *
*                                                                           *
*****************************************************************************
*                                                                           *
*      FONCTION :  Controle des bin                                         *
*                                                                           *
*      ENTREE   :  bin                                                      *
*                                                                           *
*      SORTIE   :  status                                                   *
*                                                                           *
****************************************************************************/
uchar	BinPrefixControl(uchar *Card,C message)
{
	//modif	04/04
	if(		(StructInfoPayment.Fallback == 0)
		&&	(StructInfoPayment.Entry_Mode == TRS_MAG)
		&&	((StructInfoPayment.SeviceCode[0]=='2')||(StructInfoPayment.SeviceCode[0]=='6'))
		&&	(StructInfoPayment.CodeFunction != REFUNDCODE)
		&&	(StructInfoPayment.CodeFunction != VOIDCODE)
		&&	(StructInfoPayment.CodeFunction != PREAUTHCOMPCODE)
		&&	(message)
	)
	{
		CUMORE_CMI_AFFICHE_MESSAGE(161,162,1000);
		return KO;
	}
	//modif	04/04
	//Os__xprintaa("StructInfoPayment.SeviceCode");
	//Os__xprintaa(StructInfoPayment.SeviceCode);
	if(BinControl(Card)!=OK)
	{
		if(
			(StructInfoPayment.SeviceCode[0]!=0)&&
			(StructInfoPayment.SeviceCode[0]!='1')&&
			(StructInfoPayment.SeviceCode[0]!='2')&&
			(StructInfoPayment.Entry_Mode != TRS_MAN)&&
			(StructInfoPayment.Entry_Mode != TRS_EMV)&&
			(StructInfoPayment.Entry_Mode != TRS_SCT)//cless PAN 19
		  )
		{
			if(g_trace)Os__xprint("(BIN KO) et (SC #1**,2**)");
//			uchar Card_Nb[20];
//			memset(Card_Nb, 0, 20);
//			memcpy(Card_Nb, &StructInfoPayment.Holder_Card[1],
//					StructInfoPayment.Holder_Card[0]);
			//Os__xprintaa("2222");
			CUMORE_CMI_AFFICHE_MESSAGE(134,135,1000);
			return KO;
		}
	}
	else
		return OK;

	if(PrefixControl(Card)!=OK)
	{
		if(g_trace)Os__xprint("(PREFIXE  KO)");
		if(StructInfoPayment.Entry_Mode != TRS_EMV){
//			uchar Card_Nb[20];
//			memset(Card_Nb, 0, 20);
//			memcpy(Card_Nb, &StructInfoPayment.Holder_Card[1],
//					StructInfoPayment.Holder_Card[0]);
//			Os__xprinta(Card_Nb);
//			Os__xprintaa("3333");
			CUMORE_CMI_AFFICHE_MESSAGE(134,135,1000);
		}
		return KO;
	}
	return OK;

}
/**************************************************************************
*                                                                         *
*       SOUS-PROGRAMME :  Saisie_Info_Carte	                              *
*                                                                         *
***************************************************************************
*                                                                         *
*       Fonction :  saisie d'infos de la carte manuellement				  *
*                                                                         *
*       Entree :														  *
*                                                                         *
*                                                                         *
*       Sortie :    status    OK  tous saisi							  *
*                             KO  abandon saisi							  *
***************************************************************************/
uchar  Saisie_Info_Carte(char codefunction)
{
	uchar	status;
	uchar	PtIso[MAXPTISO];

	uchar	*ptKeyboard;
	uchar 	tmp[2];

//	if(codefunction != VOIDCODE)
//	{
//		//badge commercant
//		do
//		{
//			status = Read_Card(PtIso,CARTE_COM);
//		}while(status==SAIS_MAN);
//		if(status==OK)
//		{
//			if(Recup_Info_badge(PtIso,0)!=OK)
//			{
//				CUMORE_CMI_AFFICHE_MESSAGE(14,16,3000);
//				return KO;
//			}
//		}
//		else if(status==ILLISIBLE)
//		{
//			status=KEY_CLEAR ;
//			CUMORE_CMI_AFFICHE_MESSAGE(14,15,3000);
//			return KO;
//		}
//		else
//			return KO;
//	}
	//numero de la carte
	do
	{
//		if(ucGetField(103,"TRANSACTION MANUELLE",&StructInfoPayment.Holder_Card[1],20,13,19,"0",30000,0)!=OK)
//			return KO;
	//	if( GetItemAlphaNumerique(NumCarte, 31,"NUMERO DE CARTE",0x0000) == 0 )
		if(customInputReader(138, "NUMERO DE CARTE", 19, TRUE, &StructInfoPayment.Holder_Card[1])!=OK)
			return KO;
	}while((strlen((PC)&StructInfoPayment.Holder_Card[1])<13));


	StructInfoPayment.Holder_Card[0] = strlen((PC)&StructInfoPayment.Holder_Card[1]);
	memcpy(StructInfoPayment.Compressed_Iso2,StructInfoPayment.Holder_Card,20);
	StructInfoPayment.Entry_Mode = TRS_MAN;

	// Traitement clé de luhn
	if(luhn_key_ok(StructInfoPayment.Holder_Card)!=TRUE)
	{
		CUMORE_CMI_AFFICHE_MESSAGE(132,133,1000);
		return KO;
	}


	//date d'expiration
	switch(codefunction)
	{
	case REFUNDCODE:
	case MANUALTRACODE:
	case VOIDCODE:
	do
	{
//		if(ucGetField(104,"TRANSACTION MANUELLE",StructInfoPayment.Expired_Date,5,4,4,"0",30000,0)!=OK)
//			return KO;
		if(customInputReader(138,"DATE EXP. MMAA",4, TRUE, StructInfoPayment.Expired_Date)!=OK)
			return KO;
	}while(strlen((PC)StructInfoPayment.Expired_Date)<4);

	memcpy(tmp,StructInfoPayment.Expired_Date,2);
	memcpy(StructInfoPayment.Expired_Date,StructInfoPayment.Expired_Date+2,2);
	memcpy(StructInfoPayment.Expired_Date+2,tmp,2);

	if(
		(memcmp(&StructInfoPayment.Expired_Date[2],"01",2)<0)
		||(memcmp(&StructInfoPayment.Expired_Date[2],"12",2)>0)
		)
	{
		CUMORE_CMI_AFFICHE_MESSAGE(105,138,1000);
		return KO;
	}
	break;

	default:
	break;
	}

	//cvv
	switch(codefunction)
	{
	case MANUALTRACODE:
		StructInfoPayment.Entry_Mode = TRS_MAN;
		do
		{
//			if(ucGetField(141,"TRANSACTION MANUELLE",StructInfoPayment.CVV2,4,3,3,"0",30000,0)!=OK)
//				return KO;
			if(customInputReader(138,"CVV",3, TRUE, StructInfoPayment.CVV2)!=OK)
				return KO;
		}while(strlen((PC)StructInfoPayment.CVV2)!=3);
		break;

	default:
	break;
	}

	return OK;
}
uchar  Saisie_Info_Or(char codefunction)
{
	uchar	status;
	uchar	PtIso[MAXPTISO];

	uchar	*ptKeyboard;
	//char pCam0=5;

	uchar 	tmp[2];

	switch(codefunction)
		{
		case OR_FACTURE50:
	do
		{
			memset(StructInfoPayment.OR_ConsultantId,0,sizeof(StructInfoPayment.OR_ConsultantId));
			if(customInputReader(138, "NUMERO DE CONSULTANT", 7, TRUE, &StructInfoPayment.OR_ConsultantId[0])!=OK)
					return KO;
		}while((strlen((PC)StructInfoPayment.OR_ConsultantId)<1) || (strlen((PC)StructInfoPayment.OR_ConsultantId)>7));

		break;
		case OR_FACTURE51:
			//WaitBothMagneticAndCam_OR(&FicPara.vCam0,&StructInfoPayment.OR_debt);

			break;
	default:
		break;
		}

	return OK;
}

/**************************************************************************
*                                                                         *
*       SOUS-PROGRAMME :  Saisie_cvv		                              *
*                                                                         *
***************************************************************************
*                                                                         *
*       Fonction :  saisie de cvv										  *
*                                                                         *
*       Entree :														  *
*                                                                         *
*                                                                         *
*       Sortie :    status    OK  tous saisi							  *
*                             KO  abandon saisi							  *
***************************************************************************/
uchar  Saisie_cvv(void)
{
	uchar	*ptKeyboard;

	do
	{
		ptKeyboard = Os__xget_varkey(141,3);
		if(ptKeyboard[0]==KEY_CLEAR)
			return KO;
		else
		{
			memset(StructInfoPayment.CVV2,0,4) ;
			memcpy(StructInfoPayment.CVV2,StructInfoPayment.Enter+1,strlen((PC)StructInfoPayment.Enter)-1);
		}
	}while(strlen((PC)StructInfoPayment.CVV2)>3);

	return OK;
}
/**************************************************************************
*                                                                         *
*       SOUS-PROGRAMME :  												  *
*                                                                         *
***************************************************************************
*                                                                         *
*       Fonction :  													  *
*                                                                         *
*       Entree :														  *
*                                                                         *
*                                                                         *
*       Sortie :    													  *
*																		  *
***************************************************************************/
int CarteCommercant(void)
{
	Telium_File_t *swipe2;
	unsigned short int cr;
	unsigned char buffer[41],pan_length;
	unsigned char status;


	CUMORE_CMI_AFFICHE_MESSAGE(99,100,200);

	hKbdReadCard = Telium_Fopen("KEYBOARD","r");
	swipe2 =  Telium_Fopen("SWIPE2","r");
	memclr(buffer,sizeof(buffer));

    switch ( cr = Telium_Ttestall( KEYBOARD | SWIPE2 , 1000 ) )
	{
	case SWIPE2 :
		Telium_Is_iso2 (swipe2, &pan_length, buffer); //Remplacer17ret_iso=is_iso2 ( swipe2,&pan_length,buffer);
		//if ( ! ret_iso)
		{
			g_trace=0;//Mouna310305
			ShowTrace((char*)buffer);
			status = Recup_Info_badge(buffer,1);
			if(status == KO)
			{
				CUMORE_CMI_AFFICHE_MESSAGE (14,16,3000);
				StructInfoPayment.Event=EVENTILLISIBLE;
				break;
			}
			Telium_Ttestall(0,100);
			Telium_Fclose(swipe2);
			if (hKbdReadCard)
				Telium_Fclose(hKbdReadCard);
			StructInfoPayment.Event=EVENTCARD;
			memcpy(StructInfoPayment.EventCard,buffer,sizeof(buffer));//A CORRIGER
			return OK;
		}
//		else
//		{
//			CUMORE_CMI_AFFICHE_MESSAGE (14,15,3000);
//			StructInfoPayment.Event=EVENTILLISIBLE;
//		}
		break;
	case KEYBOARD :
		CUMORE_CMI_AFFICHE_MESSAGE (33,138,3000);
		StructInfoPayment.Event=EVENTABORTED;
		break;
	case TIMEOUT :
		CUMORE_CMI_AFFICHE_MESSAGE (170,138,3000);
		StructInfoPayment.Event=EVENTTIMER;
		break;
	}
	Telium_Ttestall(0,100);
	Telium_Fclose(swipe2);
	if (hKbdReadCard)
		Telium_Fclose(hKbdReadCard);
	return KO;
}
/**************************************************************************
*                                                                         *
*       SOUS-PROGRAMME :  												  *
*                                                                         *
***************************************************************************
*                                                                         *
*       Fonction :  													  *
*                                                                         *
*       Entree :														  *
*                                                                         *
*                                                                         *
*       Sortie :    													  *
*																		  *
***************************************************************************/
int CarteClient(void)
{
	Telium_File_t *swipe2;
	Telium_File_t *swipe1;
	unsigned short int cr;
	unsigned char buffer[41],pan_length;
	unsigned char buffer1[41],pan_length1;

	T_CUTERMkey key;//ToSee T_KSUkey key;


	CUMORE_CMI_AFFICHE_MESSAGE(101,102,200);

	hKbdReadCard = Telium_Fopen("KEYBOARD","r");
	swipe2 =  Telium_Fopen("SWIPE2","r");
	swipe1 =  Telium_Fopen("SWIPE31","r");
	memclr(buffer,sizeof(buffer));

    switch ( cr = Telium_Ttestall( KEYBOARD | SWIPE2 | SWIPE31 , 1000 ) )
	{
	case SWIPE2 :
	case SWIPE31 :
		ShowTrace("swipe ok");
		cr |= Telium_Ttestall(cr ^ (SWIPE2 | SWIPE31), 10);
		if(swipe1 != NULL)
		{
			ShowTrace("swipe1 ok");
			Telium_Is_iso1 (swipe1, &pan_length1, buffer1); //Remplacer17ret_iso=is_iso1 ( swipe1,&pan_length1,buffer1);
		}
		g_trace=0;
		ShowTrace("swipe1/2");
		//ShowTrace_Mouna(buffer1);
		memcpy(&StructInfoPayment.EventCard[0],buffer1,sizeof(buffer1));

		if(swipe2 != NULL)
		{
			ShowTrace("swipe2 ok");
			Telium_Is_iso2 (swipe2, &pan_length, buffer); //Remplacer17ret_iso=is_iso2 ( swipe2,&pan_length,buffer);
		}

		memcpy(&StructInfoPayment.EventCard[50],buffer,sizeof(buffer));
		StructInfoPayment.Event=EVENTCARD;

		Telium_Ttestall(0,100);
		Telium_Fclose(swipe1);
		Telium_Fclose(swipe2);
		if (hKbdReadCard)
			Telium_Fclose(hKbdReadCard);
		return OK;
		break;

	case KEYBOARD :
		key = getchar();
		if ((key == T_ANN)||(key == T_F1))
		{
			Os__xprint("T_ANN");
			StructInfoPayment.Event=EVENTESC;
		}
		else if ((key == T_VAL)||(key == T_F4))
		{
			Os__xprint("T_VAL");
			StructInfoPayment.Event=EVENTVAL;
		}
		break;

	case TIMEOUT :
		ShowTrace("swipe2 timeout");
		CUMORE_CMI_AFFICHE_MESSAGE (170,138,3000);
		StructInfoPayment.Event=EVENTTIMER;
		break;
	}
	Telium_Ttestall(0,100);
	Telium_Fclose(swipe1);
	Telium_Fclose(swipe2);
	if (hKbdReadCard)
		Telium_Fclose(hKbdReadCard);
	return KO;
}

int CarteClient01(void)
{
	Telium_File_t *swipe2;
	Telium_File_t *swipe1;
	unsigned short int cr,cr1;
	unsigned char buffer[41],pan_length;
	unsigned char buffer1[41],pan_length1;

	int key; //T_KSUkey key;


	CUMORE_CMI_AFFICHE_MESSAGE(101,102,200);

	hKbdReadCard = Telium_Fopen("KEYBOARD","r");
	swipe2 =  Telium_Fopen("SWIPE2","r");
	swipe1 =  Telium_Fopen("SWIPE31","r");
	memclr(buffer,sizeof(buffer));

    switch ( cr = Telium_Ttestall( KEYBOARD | SWIPE2 , 1000 ) )
	{
	case SWIPE2 :
		ShowTrace("swipe2");
		switch ( cr1 = Telium_Ttestall( SWIPE31 , 1000 ))
		{
		case SWIPE31 :
			Telium_Is_iso1 (swipe1, &pan_length1, buffer1); //Remplacer17ret_iso=is_iso1 ( swipe1,&pan_length1,buffer1);
			//if ( ! ret_iso)
			{
				g_trace=0;//Mouna310305
				ShowTrace("swipe1/2");
				//ShowTrace_Mouna(buffer1);
				memcpy(&StructInfoPayment.EventCard[0],buffer1,sizeof(buffer1));
				Telium_Is_iso2 (swipe2, &pan_length, buffer); //Remplacer17ret_iso=is_iso2 ( swipe2,&pan_length,buffer);
				//if ( ! ret_iso)
				{
					memcpy(&StructInfoPayment.EventCard[50],buffer,sizeof(buffer));
					StructInfoPayment.Event=EVENTCARD;
					//ShowTrace_Mouna(buffer);
				}
//				else
//				{
//					CUMORE_CMI_AFFICHE_MESSAGE (53,15,3000);
//					StructInfoPayment.Event=EVENTILLISIBLE;
//				}

				Telium_Ttestall(0,100);
				Telium_Fclose(swipe1);
				Telium_Fclose(swipe2);
				if (hKbdReadCard)
					Telium_Fclose(hKbdReadCard);
				return OK;
			}
//			else
//			{
//				CUMORE_CMI_AFFICHE_MESSAGE (53,15,3000);
//				StructInfoPayment.Event=EVENTILLISIBLE;
//			}
			break;
		case TIMEOUT :
			ShowTrace("swipe1/2 timeout");
			CUMORE_CMI_AFFICHE_MESSAGE (170,138,3000);
			StructInfoPayment.Event=EVENTTIMER;
			break;
		}
		break;
	case KEYBOARD :
		key = getchar();
		if ((key == T_ANN)||(key == T_F1))
		{
			Os__xprint("T_ANN");
			StructInfoPayment.Event=EVENTESC;
		}
		else if ((key == T_VAL)||(key == T_F4))
		{
			Os__xprint("T_VAL");
			StructInfoPayment.Event=EVENTVAL;
		}
		break;

	case TIMEOUT :
		ShowTrace("swipe2 timeout");
		CUMORE_CMI_AFFICHE_MESSAGE (170,138,3000);
		StructInfoPayment.Event=EVENTTIMER;
		break;
	}
	Telium_Ttestall(0,100);
	Telium_Fclose(swipe1);
	Telium_Fclose(swipe2);
	if (hKbdReadCard)
		Telium_Fclose(hKbdReadCard);
	return KO;
}
/****************************************************************************
*                                                                           *
*      SOUS-PROGRAMME : forcage                                             *
*                                                                           *
*****************************************************************************
*                                                                           *
*      FONCTION : droits de forcage                                         *
*                                                                           *
*      ENTREE   : Rien                                                      *
*                                                                           *
*      SORTIE   : uchar                                                     *
*                                                                           *
****************************************************************************/
uchar forcage()
{
	uchar status=KO;
	if(FicPara.Indic_forcage=='1')
		status=OK;
	if(
		(StructInfoPayment.SeviceCode[1]=='2')||
		(StructInfoPayment.Entry_Mode == TRS_MAN)||
		(StructInfoPayment.CodeFunction == PREAUTHCODE)||
		(FicPara.Indic_Offline!='1')||
		(StructInfoPayment.Carte_EnLN)
	)
		status=KO;
	return status;
}

/****************************************************************************
*                                                                           *
*      SOUS-PROGRAMME : traitement_code_conf                                *
*                                                                           *
*****************************************************************************
*                                                                           *
*      FONCTION :  traitement code confidentiel                             *
*                                                                           *
*      ENTREE   :  Rien                                                     *
*                                                                           *
*      SORTIE   :  status                                                   *
*                                                                           *
****************************************************************************/
uchar   traitement_code_conf(void)
{
	uchar   pin_clair[12+1];
	uchar	Lg_pin;
//	uchar tracepin[20];


	//Os__xprintaa("0000000");
	memset(pin_clair,0,13) ;
	if(GetField_pin(153,pin_clair,&Lg_pin)!=OK){
		//Os__xprintaa("999999");
		return ABORTED;
	}
	StructInfoPayment.Lg_pin = Lg_pin;
	encryp_code_conf(pin_clair,Lg_pin);

//	sprintf(tracepin,"%d",(int)Lg_pin);
//	//APEMV_UI_MessageDisplayDelay (tracepin, 3);



	return OK;
}

/****************************************************************************
*                                                                           *
*      SOUS-PROGRAMME : encryp_code_conf                                    *
*                                                                           *
*****************************************************************************
*                                                                           *
*      FONCTION :  encryptage du code confidentiel                          *
*                                                                           *
*      ENTREE   :  code pin                                                 *
*                                                                           *
*      SORTIE   :  status                                                   *
*                                                                           *
****************************************************************************/
uchar   encryp_code_conf(uchar* pin,uchar Lg_pin)
{
	uchar i;
	uchar length;
	uchar pin_b[8];
	uchar pan_b[8];
	uchar pin_pan[8];
	uchar pan_b1[16];
	uchar entree[8];
	uchar sortie[8];
	//uchar cle_travail[8];
	uchar cle_travail[24];
	uchar result_c[8];
	uchar texte16[48];

	uchar code_c[12];
	uchar chaine[8];

	uchar MasterKey[24];
	Ushort	aleas;
	DATE date;
	uchar date_appel[9];

//	char tracepin[32];

	memset (&cle_travail[0],'0',sizeof(cle_travail));
	memset (&entree[0],'0',sizeof(entree));
	memset (&sortie[0],'0',sizeof(sortie));
	memset (&texte16[0],'0',sizeof(texte16));

	memset (code_c,0xff,sizeof(code_c));
	memcpy (code_c,pin,Lg_pin);

	// création aleatoire de la cl‚ de travail working_key
	for(i=0;i<24;i+=2)
	{
		aleas = rand();
		cle_travail[i] = (uchar)aleas;
		cle_travail[i+1] = (uchar)(aleas>>8);
	}

	Telium_Read_date(&date);
	sprintf ((char*)date_appel,
		"%.2s%.2s%.2s%.2s",
		date.year,
		date.hour,
		date.minute,
		date.second);
	memcpy(chaine,date_appel,8);

	/*
	Os__read_time_sec(Heure);
	Os__read_date(Date);
	memcpy(chaine,Date,2);
	memcpy(&chaine[2],Heure,6);
	*/
	for(i=0;i<8;i++)
	{
		cle_travail[i] = cle_travail[i] ^ chaine[i];
		cle_travail[i+8] = cle_travail[i+8] ^ chaine[i];
		cle_travail[i+16] = cle_travail[i+16] ^ chaine[i];
	}
	/* Ajustement parité de la Working Key */
	AjusterPariteImpaire(cle_travail,24);
	if(FicPara.Methode_Encry=='D')
		memcpy(&cle_travail[16],cle_travail,8);

	//ShowTrace_Mouna("cle_travail 1");
	//eft30printhexKamal(cle_travail,24);

	memset(pin_b,0xff,8);
	pin_b[0]=Lg_pin;
	asc_bcd(&pin_b[1],6,code_c,12);


//	APEMV_UI_MessageDisplayDelayHexa(pin_b,8, 3);

//	sprintf(tracepin,"%d",(int)StructInfoPayment.Holder_Card[0]);
//	//APEMV_UI_MessageDisplayDelay (tracepin, 3);

	/*pan_bloc*/
	memset(pan_b1,0,16);
	length = StructInfoPayment.Holder_Card[0]-1 ; //sans cle luhn
	if(length <= 12)
		memcpy(&pan_b1[16-length],&StructInfoPayment.Holder_Card[1],length);
	else
		memcpy(&pan_b1[4],&StructInfoPayment.Holder_Card[length-12+1],12);
	asc_bcd(pan_b,8,pan_b1,16);


//	APEMV_UI_MessageDisplayDelayHexa(pan_b,8, 3);
	/*pin_pan*/
	for(i=0; i<8; i++)
		pin_pan[i] = pin_b[i] ^ pan_b[i];


//	APEMV_UI_MessageDisplayDelayHexa(pin_pan,8, 3);


	memcpy(cle_travail,"\x1A\x83\x07\x01\x32\x26\xEC\x16\xB3\xFE\xDC\x7F\xE5\xF4\x3B\x10\x08\x91\x70\x34\xD6\xBA\xC4\xB0",24);

	//Encryption avec la clé du travail
	Crypto__3DesEnc(pin_pan,sortie,cle_travail);
	memcpy(StructInfoPayment.pin_code_cryp,sortie,8) ;

	//Encryption la clé du travail avec le master key
	memcpy(MasterKey,FicPara.Master_Key,24);

//	//APEMV_UI_MessageDisplayDelay ("MasterKey", 2);
//	APEMV_UI_MessageDisplayDelayHexa(MasterKey,24, 3);
//	//APEMV_UI_MessageDisplayDelay ("cle_travail 1", 2);
//	APEMV_UI_MessageDisplayDelayHexa(cle_travail,24, 3);

	Crypto__3DesEnc(cle_travail,result_c,MasterKey);


	hex_asc(texte16,result_c,16);
	memcpy(StructInfoPayment.cle_travail_cryp,texte16,16);

	Crypto__3DesEnc(cle_travail+8,result_c,MasterKey);
	hex_asc(texte16,result_c,16);
	memcpy(StructInfoPayment.cle_travail_cryp+16,texte16,16);

	Crypto__3DesEnc(cle_travail+16,result_c,MasterKey);
	hex_asc(texte16,result_c,16);
	memcpy(StructInfoPayment.cle_travail_cryp+32,texte16,16);

	//ShowTrace_Mouna(StructInfoPayment.cle_travail_cryp);

	//ShowTrace_Mouna("pin_pan");
	//eft30printhexKamal(pin_pan,8);

	//ShowTrace_Mouna("pin_code_cryp");
	//eft30printhexKamal(StructInfoPayment.pin_code_cryp,8);


	return OK;
}

/****************************************************************************/
void AjusterPariteImpaire(uchar *Str,Ushort lg)
{
	uchar C1;
	Ushort i,j;
	Ushort Parite;
	uchar key[7];

	key[0]=0x40;
	key[1]=0x20;
	key[2]=0x10;
	key[3]=0x08;
	key[4]=0x04;
	key[5]=0x02;
	key[6]=0x01;

	for(j=0;j<lg;j++)
   {
		C1 = Str[j];

		Parite = 0;
		for(i=0;i<7;i++)
      {
			if((key[i] & C1) == key[i])
				Parite++;
		}
		if((Parite - ((Parite / 2) * 2)) == 0)
			Str[j]^=0x01;
	}
}
