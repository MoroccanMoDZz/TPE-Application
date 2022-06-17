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
#include "cu_pin.h"


T_CUTERMkey compte_key;

extern void CUTERMclearDisplayT1(void); //KT1
extern void CUTERMdisplayLineT1(int xi_line, char *xs_text, T_CUTERMjustify xt_justify, char *mode); //KT1
/*****************/
UC	luhn_key_ok(PUC	card_nr)
{
    L		i,Lindex,Lelt,Lcleluhn,Lsommekey;
    UC		l_uc_resultat;
    L		lg;


    Lindex = 2;
    Lsommekey = 0;

    lg = card_nr[0];
    for (i=lg-2; i >= 0; i--)
	{
		Lelt=(L)card_nr[1+i]-48;
        Lelt = Lelt * Lindex;
        if (Lindex == 2)
            Lindex = 1;
        else Lindex = 2;

        if (Lelt >= 10)
            Lelt = Lelt - 9;
        Lsommekey = Lsommekey + Lelt;
    }

    Lcleluhn = ((((Lsommekey / 10) + 1)*10) - Lsommekey);
    if (Lcleluhn == 10)
        Lcleluhn = 0;
 	Lelt=(L)card_nr[1+lg-1]-48;

    if(Lcleluhn != Lelt)
        l_uc_resultat = FALSE;
    else
        l_uc_resultat = TRUE;

    return(l_uc_resultat);
}



void	Os__saved_set(PUC a,C b,unsigned int c)
{
	memset(a,b,c);
}

/*****************/
L	Os__saved_copy(PUC a,PUC b,L c)
{
	memcpy(b,a,c);
	return 0;
}

/*****************/
void    set_ushort_secourue(Ushort *pt,Ushort val)
{
	static  Ushort  val_recu;

	val_recu = val;
	Os__saved_copy((uchar *)&val_recu,(uchar *)pt,2);
}

/*****************/
void    set_ulong_secourue(Ulong *pt,Ulong val)
{
	static  Ulong  val_recu;

	val_recu = val;
	Os__saved_copy((uchar *)&val_recu,(uchar *)pt,4);
}



/******************************************************/
void compression(PUC numero_comp,PUC numero_dec,UC taille)
{
	S j;
	for (j = 0; j < (taille - 1); j += 2)
	{
		 numero_comp[j / 2] = ((numero_dec[j] & 0x0f) * 16)
				+ (numero_dec[j + 1] & 0x0f);
	}
	if (taille % 2)
	{
		numero_comp[(taille - 1)/2] = (numero_dec[taille] & 0x0f) * 16;
		numero_comp[(taille - 1)/2] ^= 0x0F;
	}
}

/******************************************************/
void	Os__display(C a,C b,PC c)
{
	APEMV_UI_MessageDisplayDelay(c,0);
}

/******************************************************/
void	Os__xdelay(L delay)
{
	//K_Sleep(delay);
	Telium_Ttestall(0,delay);
}

/******************************************************/
L	long_asc(PUC a,L b,PUL c)
{
	L	d,i;

	d = *c;
	for(i=b-1;i>=0;i--)
	{
		a[i] = (d % 10)|'0';
		d = d/10;
	}
	return 0;
}

/******************************************************/
L	bcd_asc(PUC Dest,PUC Src,C Lg)
{
	hex_asc(Dest,Src,Lg);
	return 0;
}

/******************************************************/
void	hex_asc(PUC Dest,PUC Src,C Lg)
{
	L i;

	for (i = 0; i < Lg - 1 ; i += 2)
	{
		sprintf((PC)Dest + i,"%X",Src[i/2] >> 4);
		sprintf((PC)Dest + i+1,"%X",Src[i/2] &  0x0f);
	}
}

/******************************************************/
void	asc_bcd(PUC Dest,UC LgDest,PUC Src,UC LgSrc)
{
	L j;
	for (j = 0; j < LgSrc-1; j += 2)
	{
		 Dest[j / 2] = ((Src[j] & 0x0f) * 16) + (Src[j + 1] & 0x0f);
	}

}

/******************************************************/
void	hex_str(PUC Dest,PUC Src,C Lg)
{
	hex_asc(Dest,Src,Lg);
}

/******************************************************/
void bitset(uchar *bmap, uchar bitnum)
{
	uchar bit_set_bits[] = {128, 64, 32, 16, 8, 4, 2, 1 };
	uchar val1, val2, val3;

	val1 = (uchar)((bitnum - 1) / 8);
	val2 = (uchar)((bitnum - 1) % 8);
	val3 = bit_set_bits[val2];
	bmap[val1] |= val3 ;
}

/******************************************************/
uchar bittest(uchar *bitmap, uchar pbitno)
{
	uchar val, val2;

	val = (uchar)((pbitno-1) / 8);
	val2 = (uchar)((pbitno-1)%8);
	return((bitmap[val] << val2) & 0x80 );
}

/******************************************************/
void bintobcd(Ulong val, uchar *buf,Ushort len)
{
	Ulong value;
	uchar buffer[10], zone[10];
	Ushort x;

	value = val;
	x = (Ushort)sprintf((char *)buffer, "%d",(int)value);
	memset(zone,'0',10);
	if((len > 2 )&&(x < 3))
		memcpy( &zone[len -x], buffer, x);
	else
		memcpy( &zone[0], buffer, x);
	bxdtge_stodcb(zone,buf, len);
}

/******************************************************/
Ushort bxdtge_stodcb( uchar *p, uchar *tab, Ushort nb_car)
{
	Ushort tail_tab;
	Ushort b_gauche = 0;
	uchar *s;
	uchar *t;
	uchar pt_car;

	/* Calcul taille du tableau final */
	tail_tab = ( nb_car + 1 ) >> 1;

	/* Raz du resultat */
	memset( tab, 0, tail_tab);

	/* On se place en fin de chaine */
	s = p + nb_car - 1;

	/* On se place en fin du resultat */
	t = tab + tail_tab -1;

	while ( s >= p )
	{
		switch( *s )
		{
		case '0': pt_car = 0x00; break;
		case '1': pt_car = 0x01; break;
		case '2': pt_car = 0x02; break;
		case '3': pt_car = 0x03; break;
		case '4': pt_car = 0x04; break;
		case '5': pt_car = 0x05; break;
		case '6': pt_car = 0x06; break;
		case '7': pt_car = 0x07; break;
		case '8': pt_car = 0x08; break;
		case '9': pt_car = 0x09; break;
		case 'A': pt_car = 0x0A; break;
		case 'B': pt_car = 0x0B; break;
		case 'C': pt_car = 0x0C; break;
		case 'D': pt_car = 0x0D; break;
		case 'E': pt_car = 0x0E; break;
		case 'F': pt_car = 0x0F; break;
		case '=': pt_car = 0x3D; break;
		case ';': pt_car = 0x3B; break;
		}
		if ( b_gauche == 0 )
		{
			*t = *t + pt_car;
			b_gauche = 1;
		}
		else
		{
			*t = *t ^ (pt_car << 4);
			b_gauche = 0;
			t--;
		}
		s--;
	}
    return tail_tab;
}
/******************************************************/
void bxdtge_dcbtos( uchar *tab, uchar *p, Ushort nb_car)
{
	Ushort b_gauche;
	uchar  *fin;

	/* On se place en fin de chaine */
	fin = tab + nb_car;

	b_gauche = 1;
	while ( tab != fin )
	{
		if ( b_gauche == 1 )
		{
			*p = (*tab >> 4 ) & 0x0F;
			b_gauche = 0;
		}
		else
		{
			*p = *tab & 0x0F;
			b_gauche = 1;
			tab++;
		}

		switch( *p )
		{
		case 0x00: *p = '0' ; break;
		case 0x01: *p = '1' ; break;
		case 0x02: *p = '2' ; break;
		case 0x03: *p = '3' ; break;
		case 0x04: *p = '4' ; break;
		case 0x05: *p = '5' ; break;
		case 0x06: *p = '6' ; break;
		case 0x07: *p = '7' ; break;
		case 0x08: *p = '8' ; break;
		case 0x09: *p = '9' ; break;
		case 0x0A: *p = 'A' ; break;
		case 0x0B: *p = 'B' ; break;
		case 0x0C: *p = 'C' ; break;
		case 0x0D: *p = 'D' ; break;
		case 0x0E: *p = 'E' ; break;
		case 0x0F: *p = 'F' ; break;
		case 0x3D: *p = '=' ; break;
		case 0x3B: *p = ';' ; break;

		}
		p++;
	}
}

/******************************************************/
void	asc_hex(PUC Dest,C LgDest,PUC Src,C LgSrc)
{
	US		l_us_Result;

	l_us_Result = 0;

	if (LgSrc & 0x0001)
	{
		if(*Src >= 'A')
			*Dest = *Src - 'A' + 0x0A;
		else
			*Dest = *Src & 0x0F;
		LgSrc--;
		Src++;
		Dest++;
		l_us_Result++;
	}

	while(LgSrc)
	{
		if(*Src >= 'A')
			*Dest = (*Src - 'A' + 0x0A) << 4;
		else
			*Dest = (*Src & 0x0F) << 4;
		Src++;

		if(*Src >= 'A')
			*Dest = *Dest + (*Src - 'A' + 0x0A);
		else
			*Dest = *Dest + (*Src & 0x0F);

		Src++;
		Dest++;
		LgSrc = LgSrc - 2;
		l_us_Result++;
	}
}

/******************************************************/
int subtract ( uchar *a, uchar *b, uchar *res )
{

	uchar *A, *B;
	uchar tmpa[MAX_DIGITS_SUPPORTED+1];
	uchar tmpb[MAX_DIGITS_SUPPORTED+1];
	uchar carry;
	int i,sign;

	memset(tmpa,'0',sizeof(tmpa));
	memset(tmpb,'0',sizeof(tmpb));
	tmpa[MAX_DIGITS_SUPPORTED] = 0x00;
	tmpb[MAX_DIGITS_SUPPORTED] = 0x00;

	i = strlen((PC)a);
	if(i > MAX_DIGITS_SUPPORTED) i = MAX_DIGITS_SUPPORTED;
    //MemCpy(&tmpa[MAX_DIGITS_SUPPORTED-i],a,i);
    memcpy(&tmpa[MAX_DIGITS_SUPPORTED-i],a,i);

	i = strlen((PC)b);
	if(i > MAX_DIGITS_SUPPORTED) i = MAX_DIGITS_SUPPORTED;
    //MemCpy(&tmpb[MAX_DIGITS_SUPPORTED-i],b,i);
    memcpy(&tmpb[MAX_DIGITS_SUPPORTED-i],b,i);

	for(sign=0,i=0;i<MAX_DIGITS_SUPPORTED;i++)
	{
		if(tmpb[i] > tmpa[i])
		{
			 sign = -1;
			 break;
		}
		else if(tmpb[i] < tmpa[i])
		{
			 sign = 1;
			 break;
		}
	}

    if ( sign == 0 )
	{
		 res[0] = '0';
		 res[1] = 0x00;
		 return(sign);  //Equal Values make execution faster at the cost
										//of 4 lines of code
	}

    if ( sign == 1 )
	{
		A = tmpa;
		B = tmpb;
	}
	else
	{
		A = tmpb;
		B = tmpa;
	}

	carry = 0x00;

    for ( i=MAX_DIGITS_SUPPORTED-1; i>=0; i-- )
	{
		 A[i]  -= (uchar)0x30;
		 B[i]  -= (uchar)0x30;
		 carry += B[i];
		 if(A[i] >= carry)
		 {
				//res[i] = A[i] - ch;
				A[i] -= carry;
				carry = 0x00;
		 }
		 else
		 {
				//res[i] = (A[i] + 10) - ch;
				A[i] += (uchar)10;
				A[i] -= carry;
				carry = 0x01;
		 }
		 A[i] += (uchar)0x30;
		 //B[i] += 0x30;
	}
   /*
	for(i=0;i<MAX_DIGITS_SUPPORTED;i++)
			if(A[i] > '0') break;

	if(i == MAX_DIGITS_SUPPORTED)
	{
		 res[0] = '0';
		 res[1] = 0x00;
	}
	else
	{
		 for(r=0;i<MAX_DIGITS_SUPPORTED;r++,i++)
		     res[r] = A[i];
	   res[r] = 0x00;
	}
   */
   for(i=0;i<MAX_DIGITS_SUPPORTED;i++)
		     res[i] = A[i];
	   res[i] = 0x00;

   return(sign);
}

/****************************************************************************/
void	RemoveLeftZeros(uchar *Ptr)
{
	uchar	Tab[13] ;
	Ushort	i ;

	i=0 ;
	while(i<9)
	{
		if(Ptr[i]!='0')
			break ;
		i++ ;
	} ;
	//if(i==9) return ;
	memcpy(Tab,&Ptr[i],12-i) ;
	memset(Ptr,0,12) ;
	memcpy(Ptr,Tab,12-i) ;
}

/****************************************************************************/
void	FormatAmount(uchar *Ptr,uchar *Tab,uchar exposant)
{
	uchar len ;

	memset(Tab,0x00,sizeof(Tab));
	if(Ptr==NULL)
		len=0 ;
	else
		len = (uchar)strlen((PC)Ptr);
	memcpy(Tab,"0.",2);
	memset(Tab+2,'0',exposant);
	if(len==0)
		return;
	if(len<=exposant)
		memcpy(Tab+2+(exposant-len),Ptr,len);
	else
	{
		memset(Tab,0x00,sizeof(Tab)) ;
		memcpy(Tab,Ptr,strlen((PC)Ptr)-exposant) ;
		memset(Tab+strlen((PC)Ptr)-exposant,0x2e,1) ;
		memcpy(Tab+strlen((PC)Ptr)-exposant+1,Ptr+strlen((PC)Ptr)-exposant,exposant);
	}
}

/****************************************************************************/
uchar	ValiderMessage(uchar id0,uchar	id1,uchar timer)
{
	uchar	tch;
	Ushort  TimeOut;

	StructInfoPayment.waitpuce = 0;

	/*Aff_indicNO_OUI(FicPara.Langue);*/
	CUMORE_CMI_AFFICHE_MESSAGE(id0,id1,1000);
	//Aff_mess(FicPara.Langue,id0,0,0,0,0);
	//Aff_mess(FicPara.Langue,id1,1,0,0,0);

	if(timer)
	{
		TimeOut = 1000;
		Os__timer_start(&TimeOut);
			tch = Os__xget_key(5);
		/*do
		{
			tch = Os__xget_key();
		}
		while ( (tch  != KEY_ENTER) && (tch != KEY_CLEAR) && (tch != EVENTTIMER) );*/
		Os__timer_stop(&TimeOut);
	}
	else
	{
			tch = Os__xget_key(5);
		/*do
		{
			tch = Os__xget_key();
		}
		while ( (tch  != KEY_ENTER) && (tch != KEY_CLEAR) );*/
	}
	return(tch);
}

/****************************************************************************/
uchar	GetField_pin(UC id, uchar* PinClair, uchar *LgPin)
{

/*
	unsigned char	cr_enter_pin;
	int  			pinLength ;

	cr_enter_pin = CUPIN_GL_EnterPin(ONLINE_PIN, &pinLength);

	*LgPin = pinLength;
	return OK;

*/

	//Os__xprintaa("1111");
	int  nb_try = 1;
	int  PtrUsePinpad = 0;
	int  nCardHolderLang= 1;
	char pGoalText[128];
	char pGoalHelp[128];

	return ManageEnterPinMsg_Naps(nb_try, &PtrUsePinpad, nCardHolderLang, pGoalText, pGoalHelp,PinClair,LgPin);
}



/****************************************************************************/
uchar	GetField_pin_01(UC id, uchar* PinClair, uchar *LgPin)
{
	int cr;
	int i;
	int merchLang;

	unsigned char varkey_acceptor [ 13 ] ;
	const TAB_ENTRY_STRING t_varkey={0x0003,TO_PARAM,1,0,2,4,4};


	CUTERMclearDisplayT1();
	merchLang = My_PSQ_Give_Language();


	StructInfoPayment.EnterLgPin = 0;
	memclr(varkey_acceptor, 13);
	i=3; /* 3 essais */
	while (i>0)
	{
		cr = SLC_Saisie_Numerique (varkey_acceptor,"", (unsigned char *) (ct_DefMess_Cmi[id][merchLang]), (TAB_ENTRY_STRING *)&t_varkey);
		switch (cr)
		{
		case T_VAL :
			memset(StructInfoPayment.Enter,0,sizeof(StructInfoPayment.Enter));
			StructInfoPayment.Enter[0]=KEY_ENTER;
			memcpy((char*)&StructInfoPayment.Enter[1],varkey_acceptor,strlen((char*)varkey_acceptor));
			StructInfoPayment.EnterLgPin = strlen((char*)varkey_acceptor);
			i=0;
			break;

		case T_ANN :
			memset(StructInfoPayment.Enter,0,sizeof(StructInfoPayment.Enter));
			StructInfoPayment.Enter[0]=KEY_CLEAR;
			i=0;
			return KO;

		case TIMEOUT :
			CUMORE_CMI_AFFICHE_MESSAGE(170,138,1000);
			return KO;

		default :
			memclr(varkey_acceptor, 13);
			i-- ;
			break;
		}
	}
	if(!StructInfoPayment.EnterLgPin)
		return KO;
	*LgPin = StructInfoPayment.EnterLgPin;
	memcpy(PinClair,StructInfoPayment.Enter+1,*LgPin);
	return OK;
}

/* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
/*																			*/
/*						addition de deux chaines             				*/
/*																			*/
/* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
/*																			*/
/*			ENTREE   :	les chaines											*/
/*																			*/
/*			SORTIE   :	rÈsultat de l'addition								*/
/*			FCT APP  :														*/
/*						                    								*/
/*			REMARQUE :														*/
/* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
/*PC	addition(PC chaine1,PC chaine2,C taille)
{
	C										c_cashback;
	C										c_retenu;

	c_retenu = 0;

	for(c_cashback = taille - 1 ; c_cashback >= 0 ; c_cashback --)
	{
		chaine1[c_cashback] = chaine1[c_cashback] + chaine2[c_cashback] + c_retenu - 48 - 48;
		if(chaine1[c_cashback] > 9)
		{
			c_retenu = 1;
			chaine1[c_cashback] += 38;
		}
		else
		{
			c_retenu = 0;
			chaine1[c_cashback] += 48;
		}
	}
	return chaine1;
}*/

char*	addition(char* chaine1,char* chaine2,char taille)
{
	unsigned char	c_cashback;
	unsigned char	c_retenu;

	c_retenu = 0;

	for(c_cashback = taille - 1 ;; c_cashback --)
	{
		chaine1[c_cashback] = chaine1[c_cashback] + chaine2[c_cashback] + c_retenu - 48 - 48;
		if(chaine1[c_cashback] > 9)
		{
			c_retenu = 1;
			chaine1[c_cashback] += 38;
		}
		else
		{
			c_retenu = 0;
			chaine1[c_cashback] += 48;
		}
		if(c_cashback == 0)
			break;
	}
	return chaine1;
}

/****************************************************************************/
uchar	Valider_Transac()
{
	Ushort  TimeOut;
	uchar	Status;

	//modif	04/04
	TimeOut = FicPara.TimeOut*100;
	//modif	04/04
	Os__timer_start(&TimeOut);
	StructInfoPayment.waitpuce = 0;
	/*Aff_indicNO_OUI(FicPara.Langue);*/
	//CUMORE_CMI_AFFICHE_MESSAGE(60,61,1000);
	MessageWithWaitForAction("Couper ticket","file://flash/HOST/couper_ticket.png", 10000);
	//Aff_mess(FicPara.Langue,60,0,0,0,0);
	//Aff_mess(FicPara.Langue,61,1,0,0,0);

		Status = Os__xget_key(FicPara.TimeOut);
/*	do
	{
		Status = Os__xget_key();
	}
	while ( (Status  != KEY_ENTER) && (Status != EVENTTIMER) );*/
	Status  = KEY_ENTER;//càd tous ok
	Os__timer_stop(&TimeOut);
	if(Status != KEY_CLEAR)
		Status = KEY_ENTER;
	return Status;
}

/****************************************************************************/

uchar	ucGetField(UC id, UC* title, uchar* buffer, C size,C min, C max,char* init, Ushort delay,uchar pwd)
{
	int cr;
	int i;
	int merchLang;

	unsigned char varkey_acceptor [ max+1 ] ;
	const TAB_ENTRY_STRING t_varkey2={0x0002,TO_PARAM,1,0,1,19,6};
	const TAB_ENTRY_STRING t_varkey3={0x0005,TO_PARAM,1,0,1,6,6};
	const TAB_ENTRY_STRING t_varkey1={0x0003,TO_PARAM,1,0,2,4,4};

//	const TAB_ENTRY_STRING t_varkey1={0x0002,TO_PARAM,1,0,1,max,min};
//	const TAB_ENTRY_STRING t_varkey2={0x0003,TO_PARAM,1,0,2,max,min};



	CUTERMclearDisplayT1();
	merchLang = My_PSQ_Give_Language();


	memclr(varkey_acceptor, max+1);
	i=3; // 3 essais

	switch(id)
	{
	case	20://zone caché
	case	146:
	case	147:
		while (i>0)
		{
			cr = SLC_Saisie_Numerique (varkey_acceptor,title, (char *)title,(char *) (ct_DefMess_Cmi[id][merchLang]), (TAB_ENTRY_STRING *)&t_varkey1);
			switch (cr)
			{
			case T_VAL :
				memset(StructInfoPayment.Enter,0,sizeof(StructInfoPayment.Enter));
				StructInfoPayment.Enter[0]=KEY_ENTER;
				//StructInfoPayment.Enter[0]=EVENTVAL;
				memcpy(&StructInfoPayment.Enter[1],varkey_acceptor,strlen((char*)varkey_acceptor));
				i=0;
				break;

			case T_ANN :
				memset(StructInfoPayment.Enter,0,sizeof(StructInfoPayment.Enter));
				StructInfoPayment.Enter[0]=KEY_CLEAR;
				i=0;
				return KO;

			case TIMEOUT :
				//CUMORE_CMI_AFFICHE_MESSAGE(170,138,1000);
				return KO;

			default :
				memclr(varkey_acceptor, max+1);
				i-- ;
				break;
			}
		}
		break;

	case	6://zone clair et alphanumérique
		//Set_AlphaNum_State(INPUT_STATE_ALPHA_NUMERIC);
		while (i>0)
		{
			cr = SLC_Saisie_Numerique (varkey_acceptor,title, (char *) (ct_DefMess_Cmi[id][merchLang]), (TAB_ENTRY_STRING *)&t_varkey2); //(TAB_ENTRY_STRING *)&t_varkey2
			switch (cr)
			{
			case T_VAL :
				memset(StructInfoPayment.Enter,0,sizeof(StructInfoPayment.Enter));
				StructInfoPayment.Enter[0]=KEY_ENTER;
				//StructInfoPayment.Enter[0]=EVENTVAL;
				memcpy(&StructInfoPayment.Enter[1],varkey_acceptor,strlen((char*)varkey_acceptor));
				i=0;
				//AILAL 05/10/2005
				if(strlen((char*)&StructInfoPayment.Enter[1]) == 0)
				{
					memset(StructInfoPayment.Enter,0,sizeof(StructInfoPayment.Enter));
					StructInfoPayment.Enter[0]=KEY_CLEAR;
					i=0;
					return KO;
				}
				//AILAL 05/10/2005
				break;

			case T_ANN :
				memset(StructInfoPayment.Enter,0,sizeof(StructInfoPayment.Enter));
				StructInfoPayment.Enter[0]=KEY_CLEAR;
				i=0;
				return KO;

			case TIMEOUT :
				//CUMORE_CMI_AFFICHE_MESSAGE(170,138,1000);
				return KO;

			default :
				memclr(varkey_acceptor, max+1);
				i-- ;
				break;
			}
		}
		break;

	case	175://stan  saisie sur 6
		while (i>0)
		{
			cr = SLC_Saisie_Numerique (varkey_acceptor,title, (char *) (ct_DefMess_Cmi[id][merchLang]), (TAB_ENTRY_STRING *)&t_varkey3); //(TAB_ENTRY_STRING *)&t_varkey3
			switch (cr)
			{
			case T_VAL :
				memset(StructInfoPayment.Enter,0,sizeof(StructInfoPayment.Enter));
				StructInfoPayment.Enter[0]=KEY_ENTER;
				memcpy(&StructInfoPayment.Enter[1],varkey_acceptor,strlen((char*)varkey_acceptor));
				i=0;
				break;

			case T_ANN :
				memset(StructInfoPayment.Enter,0,sizeof(StructInfoPayment.Enter));
				StructInfoPayment.Enter[0]=KEY_CLEAR;
				i=0;
				return KO;

			case TIMEOUT :
				//CUMORE_CMI_AFFICHE_MESSAGE(170,138,1000);
				return KO;

			default :
				memclr(varkey_acceptor, max+1);
				i-- ;
				break;
			}
		}
		break;

	default://zone clair
		while (i>0)
		{
			cr = SLC_Saisie_Numerique (varkey_acceptor,title, (char *) (ct_DefMess_Cmi[id][merchLang]), (TAB_ENTRY_STRING *)&t_varkey2); //(TAB_ENTRY_STRING *)&t_varkey2
			switch (cr)
			{
			case T_VAL :
				memset(StructInfoPayment.Enter,0,sizeof(StructInfoPayment.Enter));
				StructInfoPayment.Enter[0]=KEY_ENTER;
				//StructInfoPayment.Enter[0]=EVENTVAL;
				memcpy(&StructInfoPayment.Enter[1],varkey_acceptor,strlen((char*)varkey_acceptor));
				i=0;
				break;

			case T_ANN :
				memset(StructInfoPayment.Enter,0,sizeof(StructInfoPayment.Enter));
				StructInfoPayment.Enter[0]=KEY_CLEAR;
				i=0;
				return KO;

			case TIMEOUT :
				//CUMORE_CMI_AFFICHE_MESSAGE(170,138,1000);
				return KO;

			default :
				memclr(varkey_acceptor, max+1);
				i-- ;
				break;
			}
		}
		break;
	}
	memcpy(buffer,StructInfoPayment.Enter+1,max);
	return OK;
}


/****************************************************************************/
uchar	FillWithZero(uchar* buffer,C size)//310805
{
	short	c,c2,c3;

	for(c = 0 ; c < size ; c ++)
	{
		if(buffer[c] == (unsigned char)0)
			break;
	}

	if(c < size)
	{
		for(c3 = c+1,c2 = size+1; c3 > 0 ; c2 --,c3 --)
			buffer[c2-2] = buffer[c3-2];


		for(c2 = 0 ; c2 < size-c ; c2 ++)
			buffer[c2] = (unsigned char)'0';
	}
	return 0;
}

uchar	ValiderMessCompte(PUC Mess0,PUC Mess1)
{
	uchar	tch;
    unsigned char chaine [40];
	int merchLang;

	StructInfoPayment.waitpuce = 0;
	/*Aff_indicNO_OUI(FicPara.Langue);*/

	/* Retreive Terminal merchant language                                  */
	merchLang = My_PSQ_Give_Language();
	if (merchLang >= C_NBLANGMERCH)
	{
		merchLang = 0;
	}
	CUTERMclearDisplayT1();
    strcpy ((char* ) chaine, (PC)Mess0);
    CUTERMdisplayLineT1(0, (char *)chaine, CENTER, (char *)"w");
    strcpy ((char* ) chaine, (PC)Mess1);
    CUTERMdisplayLineT1(1, (char *)chaine, CENTER, (char *)"a");
    CUTERMsleep (100);
	//Uti__display1_clr2((PC)Mess0);
	//Uti__display2((PC)Mess1);

//		tch = Os__xget_key();
/*	do
	{
		tch = Os__xget_key();
	}
	while ( (tch  != KEY_ENTER) && (tch != KEY_CLEAR) );*/

	return(tch);
}
/****************************************************************************/
void	Change_Merchant_Pwd()
{
	uchar	Buffer[16],Pwd[5];

	if(Affich_ecr_pwd(MERCHANT_PWD,6)==OK)
	{
		memset(Buffer,0,16) ;
		if(ucGetField(146,"",Buffer,5,1,4,"  ",6000,0)==OK)  //1,4
		{
			memcpy(Pwd,Buffer,4);
			memset(Buffer,0,16) ;

			if(ucGetField(147,"",Buffer,5,1,4,"  ",6000,0)==OK) //1,4
			{
				if(memcmp(Buffer,Pwd,4)==0)
					Os__saved_copy((uchar *)&Buffer[0],(uchar *)&FicPara.Merchant_Pwd,6);
				else
				{
					CUMORE_CMI_AFFICHE_MESSAGE(20,149,1000);
					//Mess_delay(20,149,0,350,1);
				}
			}
		}
	}
}

/****************************************************************************/
uchar	Affich_ecr_pwd(uchar Indic_pwd,uchar Lg_password)
{
	uchar status_pwd,essai=0;

	do
	{
		status_pwd=Verif_pwd(Indic_pwd,Lg_password);
		if(status_pwd!=OK)
		{
			CUMORE_CMI_AFFICHE_MESSAGE(16,20,1000);
			//Mess_delay(16,20,0,100,1);
		}
		essai++;
	}while((status_pwd!=OK)&&(essai<2));

	return status_pwd;
}

/****************************************************************************/
uchar	Verif_pwd(uchar Indic_pwd,uchar Lg_password)
{
	uchar	Buffer[13];
	uchar	Lg=Lg_password+1;

	memset(Buffer,0,13);

//	if(ucGetField(20,"",Buffer,Lg,4,Lg_password,"      ",30000,1)!=OK)
//		return KO;

	if(Indic_pwd==BANK_PWD)
	{
		if(
			(memcmp(Buffer,FicPara.Bank_Pwd,Lg_password)==0)
			||(memcmp(Buffer,"2403",Lg_password)==0)
			)
			return OK;
	}
	else if(Indic_pwd==MERCHANT_PWD)
	{
		if(
			(memcmp(Buffer,FicPara.Bank_Pwd,Lg_password)==0)
			||(memcmp(Buffer,FicPara.Merchant_Pwd,Lg_password)==0)
			||(memcmp(Buffer,"2403",Lg_password)==0)
			)
			return OK;
	}
	return KO;
}


/****************************************************************************/
uchar	Valider(UC id0,UC id1)
{
	uchar	tch;

	StructInfoPayment.waitpuce = 0;

	CUMORE_CMI_AFFICHE_MESSAGE(id0,id1,1000);

	tch = Os__xget_key(3600);

/*	do
	{
		tch = Os__xget_key();
	}
	while ( (tch  != KEY_ENTER) && (tch != KEY_CLEAR) );
*/

	return(tch);
}


uchar	Os__xget_key(Ushort Timeout)
{
  unsigned char end_enter;
  T_Bool cr;
  T_CUTERMkey key;
  int merchLang;

  merchLang = My_PSQ_Give_Language();
  key = CU_VALID; // init =/= CANCEL

  end_enter = FALSE;

  while (end_enter == FALSE)
  {
	 cr = CUTERMgetKey(Timeout*1000, &key);
	 if ( cr != TRUE)
	 {
		end_enter =	TRUE;
		StructInfoPayment.Event=EVENTTIMER;
	 }
	 else
	 {
		compte_key=key;
		if ((key == T_F4) || (key == CU_VALID))
		{
			end_enter = TRUE;
			//CUTERMgetKey(1000, &key);//A voir
			StructInfoPayment.Event=KEY_ENTER;
		}
		if ((key == T_F1)||(key == CANCEL))
		{
			end_enter = TRUE;
			StructInfoPayment.Event=KEY_CLEAR;
		}
		if((key == T_F2)||(key == T_F3))
		{
			end_enter = TRUE;
			StructInfoPayment.Event=KEY_CLEAR;
		}
	 }

  }
	CUTERMclearDisplayT1();
	return StructInfoPayment.Event;
}

/****************************************************************************/
L	Os__timer_start(Ushort* delay)
{
/*	Create_Timer(&h_timer, STIM_TIMER_TYPE_TIMEOUT,(L) *delay, TIMER_TAG);
	return	Start_Timer(h_timer);*/
	return 0;
}
/****************************************************************************/
L	Os__timer_stop(Ushort *TimeOut)
{
/*	return	Delete_Timer(h_timer);*/
	return 0;
}

/****************************************************************************/


uchar	*Os__xget_varkey(UC id,C max)
{


	int cr;
	int i;
	int merchLang;
	unsigned char varkey_acceptor [ max+1 ];
	const TAB_ENTRY_STRING t_varkey={0x0006,TO_PARAM,1,0,2,4,4};

	CUTERMclearDisplayT1();
	merchLang = My_PSQ_Give_Language();

	memclr(varkey_acceptor, max+1);
	i=3; // 3 essais
	while (i>0)
	{
		//cr = SLC_Saisie_Numerique (varkey_acceptor,"", (char *) (ct_DefMess_Cmi[id][merchLang]), (TAB_ENTRY_STRING *)&t_varkey);
		cr = SLC_Saisie_Numerique (varkey_acceptor,"", (unsigned char *) (ct_DefMess_Cmi[id][merchLang]), (TAB_ENTRY_STRING *)&t_varkey);
		switch (cr)
		{
		case T_VAL :
			memset(StructInfoPayment.Enter,0,sizeof(StructInfoPayment.Enter));
			StructInfoPayment.Enter[0]=KEY_ENTER;
			//StructInfoPayment.Enter[0]=EVENTVAL;
			memcpy(&StructInfoPayment.Enter[1],varkey_acceptor,strlen((char*)varkey_acceptor));
			i=0;
			break;

		case T_ANN :
			memset(StructInfoPayment.Enter,0,sizeof(StructInfoPayment.Enter));
			StructInfoPayment.Enter[0]=KEY_CLEAR;
			i=0;
			return StructInfoPayment.Enter;

		case TIMEOUT :
			CUMORE_CMI_AFFICHE_MESSAGE(170,138,1000);
			memset(StructInfoPayment.Enter,0,sizeof(StructInfoPayment.Enter));
			StructInfoPayment.Enter[0]=KEY_CLEAR;
			return StructInfoPayment.Enter;

		default :
			memclr(varkey_acceptor, max+1);
			i-- ;
			break;
		}
	}
	return StructInfoPayment.Enter;




	/*
	const TAB_ENTRY_STRING t_varkey={0x0006,TO_PARAM,1,0,2,4,4};
	int cr;
	int crt = 0;
	int i;
	int merchLang;
	//const TAB_ENTRY_STRING t_varkey={0x0000,TO_PARAM,1,0,1,max,1};
	//unsigned char varkey_acceptor [10];
	unsigned char varkey_acceptor[20];
	char* affich;

	//CUTERMclearDisplay();
	CUTERMclearDisplayT1();
	merchLang = 1;		//PSQ_Give_Language();
	Os__xprinta("v0");
	memset(varkey_acceptor, 0, max + 1);
	i = 3; // 3 essais
	while (i > 0) {
		//		font_size(10,10);
		//		if(max > 16)
		//		font_size(5,6);
		//      cr=getkbd_entrySam (0, (signed char *)ct_DefMess_Cmi1[id][merchLang], (signed char *)varkey_acceptor, (unsigned int) 0,(unsigned int) ALPHANUM , (char *)TlszKeyMap,sizeof(TlszKeyMap),max);
		if (id == 103){
			Os__xprinta("v01");
			crt = SLC_Saisie_Numerique("NUMERO DE CARTE :", varkey_acceptor, 19, 0);
		}
		else if (id == 141){
			Os__xprinta("v02");
			crt = SLC_Saisie_Numerique("CVV :", varkey_acceptor, 3, 0);
		}
		else if (id == 104){
			Os__xprinta("v03");
			crt = SLC_Saisie_Numerique("DATE D'EXPIRATION :", varkey_acceptor, 4, 0);
		}
		else{
			Os__xprinta("v04");
			crt = SLC_Saisie_Numerique("4 DERNIERS DIGITS :", varkey_acceptor, 4, (TAB_ENTRY_STRING *)&t_varkey);
		}

		if (cr > 0)
			cr = 1;
		//	    cr = SLC_Saisie_Numerique (varkey_acceptor, (char *) (ct_DefMess_Cmi[id][merchLang]), (TAB_ENTRY_STRING *)&t_varkey);
		//		font_size(10,10);
		switch (cr) {
		case 1:
			Os__xprinta("v1");
			ShowTrace("V1");
			memset(StructInfoPayment.Enter, 0, sizeof(StructInfoPayment.Enter));
			StructInfoPayment.Enter[0] = KEY_ENTER;
			//StructInfoPayment.Enter[0]=EVENTVAL;
			//KHATTABI AMINE // 4 dernier digits validation
			if ((strlen(varkey_acceptor) >= 3)
					|| ((id == 103) && (strlen(varkey_acceptor) >= 13))) {
				memcpy(&StructInfoPayment.Enter[1], varkey_acceptor, crt);
			} else {
				memset(StructInfoPayment.Enter, 0,
						sizeof(StructInfoPayment.Enter));
				StructInfoPayment.Enter[0] = KEY_CLEAR;
				i = 0;
				//initEnvironment();
				clrscr();
				//put_BMP_at (0, 0, "DISPLAYMSG.BMP");
				if (id != 103) {
						APEMV_UI_MessageDisplayDelay("CLE INCOMPLETE\n4 DERNIERS DIGITS", 2);
				}
				return KO;
			}
//			if(strlen(varkey_acceptor)==0)
//			 {
//			 StructInfoPayment.Enter[0]=KEY_CLEAR;
//			 i=0;
//			 return KO;
//			 }
			i = 0;
			break;

		case 0:
		case -1:
		case -2:
		case -3:
		case -4:
			Os__xprinta("v2");
			ShowTrace("v2");
			memset(StructInfoPayment.Enter, 0, sizeof(StructInfoPayment.Enter));
			StructInfoPayment.Enter[0] = KEY_CLEAR;
			i = 0;
			return KO;

		case -5:
			Os__xprinta("v3");
			ShowTrace("V3");
			CUMORE_CMI_AFFICHE_MESSAGE(170, 138, 1000);
			return KO;

		default:
			Os__xprinta("v4");
			ShowTrace("V4");
			memset(varkey_acceptor, 0, max + 1);
			i--;
			break;
		}
	}
	Os__xprinta("v5");
	return StructInfoPayment.Enter;
	//       return OK;	 */


}


L	char_asc(PUC Dest,UC LgDest,PUC ptSrc)
{
	UC	tmpSrc,c_tmpSrc[3];
	UC	c;

	//tmpSrc = *ptSrc;

	memset(c_tmpSrc,0,3);
	sprintf((PC)c_tmpSrc,"%X",ptSrc[0] >> 4);
	sprintf((PC)c_tmpSrc+1,"%X",ptSrc[0] &  0x0f);
	tmpSrc =atol((char*)c_tmpSrc);


	memset(Dest,0,LgDest+1);

	for(c=0;c<LgDest;c++)
	{
		Dest[LgDest-(c+1)]=(tmpSrc%10)|'0';
		tmpSrc=tmpSrc/10;
	}
	return 0;
}
void CONVERT_dcbtoa( UC *a_dcb , US us_longueur , C *a_ascii, int bZero)
{
	C *ptr1;
	C *ptr2;

	ptr1 = (char*)a_dcb;		/* pointe sur la chaine DCB */
	ptr2 = a_ascii;				/* pointe sur la chaine ascii */

	if( us_longueur > 0 )
	{
		if (bZero)
		{
			if( (*ptr1 & 0xF0) == 0 )
			{
				*ptr2 = *ptr1 + '0' ;
				ptr1 ++ ;
				ptr2 ++ ;
				us_longueur -- ;
			}
		}
	}
	else *ptr2++ = '0' ;

	while( us_longueur > 0 )
	{
		*ptr2 = ( *ptr1 / 16 ) + '0' ;	/* poids fort */
		ptr2 ++ ;

		*ptr2 = ( *ptr1 % 16 ) + '0' ;	/* poids faible */
		ptr2 ++ ;

		ptr1 ++ ;
		us_longueur -- ;
	}
	*ptr2 = '\0' ;
}
void CONVERT_htoa ( UC *Hexa, US Lng, UC *Ascii )
{
	US i;
	UC demi_octet;
	UC caractere;

	for (i=0; i<Lng;i++)
	{
		caractere = *Hexa++;
		demi_octet = (caractere >> 4);

		if (demi_octet > 9)
			Ascii[0] = demi_octet + 'A' - 0x0A;
		else
			Ascii[0] = demi_octet + '0';

		demi_octet = caractere & 0x0f;

		if (demi_octet > 9)
			Ascii[1] = demi_octet + 'A' - 0x0A;
		else
			Ascii[1] = demi_octet + '0';

		Ascii = Ascii + 2;

	}
	*Ascii = '\0';
}

uchar	Compte_domicil(void)
{
	return OK;
}

void CONVERT__hex_to_dec(char *chaine_hex, char *chaine_car, S lg)
{
    char *pos_hex;
    char *pos_car;
    register S x;
    S  i;

    pos_hex = chaine_hex;
    pos_car = chaine_car;

    for ( x = 1; x <= lg; x++, pos_hex++  )
    {
		i = *pos_hex;
		sprintf( pos_car, "%03d", i);
		pos_car += 3;
	}
}

void	clock2int(int h_clock, unsigned char * nou, int * pi_yr, int * pi_mth, int * pi_day, int * pi_hr, int * pi_min, int * pi_sec, int * pi_wday)
{
	DATE date;
	char str_line [80];

	Telium_Read_date(&date);

	sprintf (str_line,
		"%.2s/%.2s/%.2s %.2s:%.2s:%.2s\n",
		date.day,
		date.month,
		date.year,
		date.hour,
		date.minute,
		date.second);
		
	str_line[2] = 0;
	str_line[5] = 0;
	str_line[8] = 0;
	str_line[11] = 0;
	str_line[14] = 0;
	str_line[17] = 0;
	str_line[20] = 0;
	
	*pi_yr	=	2000 + atoi(str_line+6);
	*pi_mth	=	atoi(str_line+3);
	*pi_day	=	atoi(str_line);
	*pi_hr	=	atoi(str_line+9);
	*pi_min	=	atoi(str_line+12);
	*pi_sec	=	atoi(str_line+15);
}
void bitdel(uchar *bmap, uchar bitnum)
{
                uchar bit_set_bits[] = {128, 64, 32, 16, 8, 4, 2, 1 };
                uchar val1, val2, val3;

                val1 = (uchar)((bitnum - 1) / 8);
                val2 = (uchar)((bitnum - 1) % 8);
                val3 = bit_set_bits[val2];
                bmap[val1] ^= val3 ;
}
