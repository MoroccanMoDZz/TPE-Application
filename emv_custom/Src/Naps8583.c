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


short FLAG_Ref_SSL = -1;	//Kamalssl


Telium_File_t	*gmodem;
long	l_msg=0;
char	typehangup=1;
#define MAX_TRS_PAR_BLOC	3              // nbre max transaction telecollecte


struct BITD  bittab[] = {
	{ 1  ,	0         	,	BITS      , 8  },
	{ 2  ,	LLVAR     	,	NUMERIC_  , 20 },
	{ 3  ,	0         	,	NUMERIC_  , 6  },
	{ 4  ,	0         	,	NUMERIC_  , 12 },
	{ 5  ,	0         	,	NUMERIC_  , 12 },
	{ 6  ,	0         	,	NUMERIC_  , 12 },
	{ 7  ,	YYMMDDhhmm	,	NUMERIC_  , 10 },
	{ 11 ,	0         	,	NUMERIC_  , 6  },
	{ 12 ,	YYMMDDhhmmss	, 	NUMERIC_  , 12 },
	{ 14 ,	YYMM      	,	NUMERIC_  , 4  },
	{ 15 ,	YYMMDD    	,	NUMERIC_  , 6  },
	{ 17 ,	MMDD    	,	NUMERIC_  , 4  },
	{ 22 ,	0         	,	ALPHANUM  , 12 },
	{ 23 ,	0         	,	NUMERIC_  , 3  },
	{ 24 ,	0         	,	NUMERIC_  , 3  },
	{ 25 ,	0         	,	NUMERIC_  , 4  },
	{ 26 ,	0         	,	NUMERIC_  , 4  },
	{ 30 ,	0         	,	NUMERIC_  , 12 },
	{ 32  ,	LLVAR     	,	NUMERIC_  , 11 },
	{ 35 ,	LLVAR     	, 	TRACKX    , 37 },
	{ 37 ,	0         	,	NUMERIC_  , 12 },
	{ 38 ,	0         	,	ALPHANUM  , 6  },
	{ 39 ,	0         	,	NUMERIC_  , 3  },
	{ 41 ,	0         	,	ALPHANUMS , 8  },
	{ 42 ,	0         	,	ALPHANUMS , 15 },
	{ 43 ,	LLVAR      	,	ALPHANUMS , 0  },
	{ 44 ,	LLLVAR     	,	ALPHANUMS , 0  },
	{ 48 ,	LLLVAR    	,	ALPHANUMS , 0  },
	{ 49 ,	0         	,	NUMERIC_  , 3  },
	{ 50 ,	0         	,	NUMERIC_  , 3  },
	{ 52 ,	0         	,	BITS      , 8  },
	{ 53 ,	LLVAR    	,	NUMERIC_  , 0  },
	{ 54 ,	LLLVAR    	,	ALPHANUM  , 0  },
	{ 55 ,	LLLVAR    	,	ALPHANUM  , 0  },
	{ 56 ,	LLVAR     	,	NUMERIC_  , 35 },
	{ 62 ,	LLLVAR    	,	ALPHANUMS , 0  },
	{ 63 ,	LLLVAR    	,	ALPHANUMS , 0  },
	{ 64 ,	0         	,	BITS      , 8  },
	{ 71 ,	0 		,	NUMERIC_  , 8  },
	{ 72 ,	LLLVAR    	,	ALPHANUMS , 0  },
	{ 73 ,	0         	,	ALPHANUMS , 6  },
	{ 74 ,	0 		    ,	NUMERIC_  , 10 },
	{ 75 ,	0 		    ,	NUMERIC_  , 10 },
	{ 76 ,	0 		    ,	NUMERIC_  , 10 },
	{ 77 ,	0 		    ,	NUMERIC_  , 10 },
	{ 83 ,	0 		    ,	NUMERIC_  , 10 },
	{ 84 ,	0 		    ,	NUMERIC_  , 10 },
	{ 86 ,	0 		    ,	NUMERIC_  , 16 },
	{ 87 ,	0 		    ,	NUMERIC_  , 16 },
	{ 88 ,	0 		    ,	NUMERIC_  , 16 },
	{ 89 ,	0 		    ,	NUMERIC_  , 16 },
	{ 97 ,	0         	,	ALPHANUMS , 17 },
	{101 ,	LLVAR    	,	ALPHANUMS , 17 },
	{110 ,	LLLVAR    	,	ALPHANUMS , 0  },
	{128 ,	0         	,	BITS      , 8  },
	{255 ,	0         	,	0         , 0, }
};

uchar	bitmap1304[]		=	{	130,	48,	129,	0,	8,	192,	8,	2,	2,	0,	0,	0,	8,	0,	0,	1};
uchar	bmp1804[]	        =	{	  2,	48,	129,	0,	8,	192,	8,	3};
uchar	bmp1520[]	        =	{	130,	48,	129,	1,	8,	192,	76,	6,	0,	120,	55,	128,	128,	0,	0,	1};
uchar	bmp1200[]	        =	{	114,	52	,	133	,	65,  8,	224,	136,	7	};//3 kamal
uchar	bmp1420[]	        =	{	122,	52	,	129	,  129,	12,	192,	201,	3	};

uchar	bitmapPINChange[]	=	{	250,	52,	    133,	65,  8,	224,   136,	2,	1,	0,	0,	0,	0,	0,	0,	1};
uchar	bitmapErecharge[]	=	{	242,	52,	    133,	65,  8,	224,   136,	2,	1,	0,	0,	0,	0,	0,	0,	1};
uchar	bitmapMiniReleve[]	=	{	242,	52,	    133,	65,  8,	224,   136,	2,	1,	0,	0,	0,	0,	0,	0,	1};
uchar	bitmapMicroCredit[]	=	{	242,	52,	    133,	65,  8,	224,   136,	2,	1,	0,	0,	0,	0,	0,	0,	1};
uchar	bitmapCredit[]		=	{	242,	52,	    133,	65,  8,	224,   136,	2,	1,	0,	0,	0,	0,	0,	0,	1};


uchar	*g_bitmap;
uchar	*bptr;
uchar	*bptr1;
Ushort	*l_bptr1;
Ushort 	count,count_r;

uchar	*NPhone;
long	receivepac(unsigned char *buffer,char trace);
void	CallHostSample(void);
void	eft30printhexGPRS(char* buffer,Ushort n_car);

int		CallPerformTCP_GPRS( char *szHostname, int nPort);
int		Emettre_Frame_GPRS( char *txbuf,unsigned short count);
int		Receive_Frame_GPRS( unsigned char *buffer);
void	Deconnection_GPRS(void);

int		CallPerformConnectionIP( const char *szHostName, unsigned int nPort);
int		SendFrameIP( char *sbuffer,unsigned short count);
int		RecieveFrameIP(char *rbuffer);
void	DeconnectionIP(void);
UC		connect_host_IP_TLS(void);



//Mo 13/04/06 GSM
static char g_szAPN[40];
static char g_szLogin[40];
static char g_szPassword[40];

static char g_szHostName[40];
static int  g_nPort;

extern t_topstack *alternateNbr;
extern int AlternanceIsOn;


/*
------------------------------------------------------------
*            hang_up ()
------------------------------------------------------------
*
*       PURPOSE: déconnecte le terminal
*
*       PARAMETER:
*
*       RETURN:
*
------------------------------------------------------------
*/
long hang_up(char type,char trace)
{
	//Wifi_PowerOff();
	if ( FicPara.g_type_connexion =='M')
	{
		//MessageWithDelay("g_type_connexion ==M",NULL,1000);
		gsm_disconnect();
	}
	if ( FicPara.g_type_connexion =='G')
	{
		//MessageWithDelay("g_type_connexion ==G",NULL,1000);
		Deconnection_GPRS();
		return 0;
	}
	else if(FicPara.g_type_connexion == 'I')
	{
		//MessageWithDelay("g_type_connexion == I",NULL,1000);
		DeconnectionIP();
	}
	else if(gmodem!=NULL)
	{
		if(type)
		{
			//MessageWithDelay("type",NULL,1000);
			// abort the dialling process and hang-up
			//Fermer17esc_command();
			//Fermer17reset_buf (gmodem,_receive_id);
			Telium_Fprintf ( gmodem ,"ATH0\r");
			Telium_Ttestall ( MODEM , 0 )  ;
			// let time to release the line for the modem
			Telium_Ttestall(0,300);
			Telium_Fclose(gmodem);
			gmodem=NULL;
			return 0;
		}
		else
		{
			//MessageWithDelay("else type",NULL,1000);
			if(trace)ShowTrace("dcnx");
			Telium_Ttestall(0,300);
			Telium_Fclose(gmodem);
			gmodem=NULL;
			return 0;
		}
	}
	return 0;
}
char	conn_etabli(void)
{
	if(gmodem != NULL)
		return 1;
	return 0;
}
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			NOM			:connect1
//			DESCRIPTION	:connexion au modem
//
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
/*T_Bool CU_SchGetKey( int xi_timeout, T_CUTERMkey *xp_key, FILE*iapp)
{
T_Bool				res_key;
T_KSUkey			key;
int					timeout;
unsigned int	uiEvent;
int	iEnded;
FILE				*lp_keybd;

   lp_keybd = Telium_Fopen("KEYBOARD", "r");

	upload_diagnostic_txt ("sgk0\n");
	res_key	= B_TRUE;
	// Telium_Ttestall	in 10th	of ms
	timeout	= (xi_timeout /	10);	//	timeout	68k	is more	occurate than timeout KSU
	iEnded = FALSE;
	while (!iEnded)
	{
		uiEvent = Telium_Ttestall (IAPP | KEYBOARD,timeout);
		{char buf [40];
		sprintf (buf, "uiEvent %x\n",uiEvent);
	        upload_diagnostic_txt (buf);
		}

		if (uiEvent == 0)
		{
			res_key = B_FALSE;
			iEnded = TRUE;
		}
		if ((uiEvent & KEYBOARD) != 0)
        {
			key = getchar();
	        upload_diagnostic_txt ("get main keyboard key\n");
			if (key == T_ANN)
			{
				*xp_key = CANCEL;
				IAPP_SchPutSync (SYNC_CANCEL, iapp);
				iEnded = TRUE;
			}
		}

		else if ((uiEvent & (IAPP )) != 0)
		{
			key=(T_KSUkey)getc(iapp);
				iEnded = TRUE;

		   switch (key)
		   {
			   case	T_VAL	:
				  *xp_key =	CU_VALID;
			   break;
			   case	T_ANN :
				  *xp_key =	CANCEL;
							IAPP_SchPutSync (SYNC_CANCEL, iapp);
			   break;
			   case	0 :
				  res_key =	B_FALSE;
			   break;
			   case	T_CORR :
				  *xp_key =	CORRECT;
			   break;
			   default :
				   *xp_key = ZERO; // This is a	Numeric	key
			   break;
		   }
	   }
	}
	Telium_Fclose (lp_keybd);
	return (res_key);
}*/
long Dial(char *phone,char trace)
{
	unsigned int	uiEvent;
	Telium_File_t	*lp_keybd;
	T_CUTERMkey		key;//ToSee T_KSUkey		key;
	size_t lng;
	unsigned char buffer[20];
	unsigned char phonenumber[20];
	L l_result = 1;
	C	pPhoneNumberLogic[20];
	C	pPhoneNumberScreen[20];


	lp_keybd =   Telium_Fopen("KEYBOARD", "r");

	memset(pPhoneNumberLogic,0,sizeof(pPhoneNumberLogic));
	memset(pPhoneNumberScreen,0,sizeof(pPhoneNumberScreen));

	strncpy(pPhoneNumberLogic,(PC)FicPara.standard,strlen((PC)FicPara.standard));
	strncat(pPhoneNumberLogic,phone,strlen((PC)phone));

	memclr(phonenumber,sizeof(phonenumber));

	if(strlen((PC)FicPara.standard) != 0)
	{
		strncpy(pPhoneNumberScreen,(PC)FicPara.standard,strlen((PC)FicPara.standard));
		pPhoneNumberScreen[strlen((PC)FicPara.standard)]=',';
		sprintf((char*)phonenumber,"ATX3S37=5D%s\r",pPhoneNumberLogic);
	}
	else
		sprintf((char*)phonenumber,"ATS37=5D%s\r",pPhoneNumberLogic);

	strncat(pPhoneNumberScreen,phone,strlen(phone));


	CUMORE_CMI_PHONE_NUMBER(pPhoneNumberScreen);


	gmodem = Telium_Fopen("MODEM","rw");

	//Fermer17mask_event (gmodem,COM_REC_NOT_EMPTY );
	//Fermer17reset_buf ( gmodem , _receive_id);

	Telium_Fprintf ( gmodem, "ATZE0\r");
	uiEvent = Telium_Ttestall ( KEYBOARD | MODEM , 6000 );
	if ((uiEvent & KEYBOARD) != 0)
    {
		key = getchar();
	    //upload_diagnostic_txt ("get main keyboard key\n");
		if (key == T_ANN)
		{
			if(trace)ShowTrace("001");
			CUMORE_CMI_AFFICHE_MESSAGE(108,45,1000);
			Telium_Ttestall(0,300);
			Telium_Fclose (lp_keybd);
			return (PAS_CON);
		}
	}
	else if ((uiEvent & (MODEM )) != 0)
	{
		if(trace)ShowTrace("000");
		memclr(buffer,sizeof(buffer));
		lng = Telium_Fread(buffer,1,20,gmodem);
		CUMORE_CMI_AFFICHE_MESSAGE(108,42,1000);
	}


	Telium_Fprintf ( gmodem, "ATB0\r");
	uiEvent = Telium_Ttestall ( KEYBOARD | MODEM , 6000 );
	if ((uiEvent & KEYBOARD) != 0)
    {
		key = getchar();
	    //upload_diagnostic_txt ("get main keyboard key\n");
		if (key == T_ANN)
		{
			if(trace)ShowTrace("004");
			CUMORE_CMI_AFFICHE_MESSAGE(108,45,1000);
			Telium_Ttestall(0,300);
			Telium_Fclose (lp_keybd);
			return (PAS_CON);
		}
	}
	else if ((uiEvent & (MODEM )) != 0)
	{
		if(trace)ShowTrace("003");
		memclr(buffer,sizeof(buffer));
		lng = Telium_Fread(buffer,1,20,gmodem);
		CUMORE_CMI_AFFICHE_MESSAGE(108,42,1000);
	}

	Telium_Fprintf ( gmodem, (char*)phonenumber);
	uiEvent = Telium_Ttestall ( KEYBOARD | MODEM , 6000 );
	if ((uiEvent & KEYBOARD) != 0)
    {
		key = getchar();
		//upload_diagnostic_txt ("get main keyboard key\n");
		if (key == T_ANN)
		{
			if(trace)ShowTrace("008");
			CUMORE_CMI_AFFICHE_MESSAGE(108,45,1000);
			Telium_Fclose (lp_keybd);
			return (PAS_CON);
		}
	}
	else if ((uiEvent & (MODEM )) != 0)
	{
		if(trace)ShowTrace("007");
		memclr(buffer,sizeof(buffer));
		lng = Telium_Fread(buffer,1,20,gmodem);
		ShowTrace((char *)buffer);
		//kamal06
		if(rechmem1(buffer,20,(PUC)"TONE",4) == 1)
		{
			CUMORE_CMI_AFFICHE_MESSAGE(185,138,1000);
			if(trace)ShowTrace("PAS DE TONALITE");
			Telium_Ttestall(0,300);
			Telium_Fclose (lp_keybd);
			return (PAS_CON);
		}
		if(rechmem1(buffer,20,(PUC)"CARRIER",7) == 1)
		{
			CUMORE_CMI_AFFICHE_MESSAGE(189,138,1000);
			if(trace)ShowTrace("NO CARRIER");
			Telium_Ttestall(0,300);
			Telium_Fclose (lp_keybd);
			return (PAS_CON);
		}
		if(rechmem1(buffer,20,(PUC)"BUSY",4) == 1)
		{
			CUMORE_CMI_AFFICHE_MESSAGE(186,138,1000);
			if(trace)ShowTrace("OCCUPE");
			Telium_Ttestall(0,300);
			Telium_Fclose (lp_keybd);
			return (PAS_CON);
		}
		//kamal06
		if(rechmem1(buffer,20,(PUC)"CONNECT",7) != 1)
		{
			if(trace)ShowTrace("009");
			Telium_Ttestall(0,300);
			Telium_Fclose (lp_keybd);
			return (PAS_CON);
		}
	}
	else//231205
	{
		if(trace)ShowTrace("009.5");
		Telium_Ttestall(0,300);
		Telium_Fclose (lp_keybd);
		return (PAS_CON);
	}

	if(trace)ShowTrace("010kam");
	Telium_Fclose (lp_keybd);
	l_result =ReceptParam(delayK, (PC) "PAC",3);
/*	if(l_result!=OK)
	{
		Os__xprint("Recept Param");
		CUMORE_CMI_AFFICHE_MESSAGE(57,138,1000);
		return KO;
	}
*/	CUMORE_CMI_AFFICHE_MESSAGE_SD(110,138,0);
	l_result = FixationParam();
	if(l_result!=OK)
	{
		Os__xprint("Fixation Param");
		CUMORE_CMI_AFFICHE_MESSAGE(57,138,1000);
		return KO;
	}
	return (BON_CON);
}
/*-----------------------------------------------------------------
* Fonction   :rechmem1
* objet      :
*
* Parametre  :source,lgs,ref,lgr
* Retour     :résultat de la recherche
*-----------------------------------------------------------------
*/
long   rechmem1(unsigned char *source,long lgs,unsigned char *ref,unsigned short lgr)
{
	short    i;
	short   status;

	for (i=0,status=0 ; (i < lgs)&&(status==0) ; i++,source++){
		if (*source==*ref){
			if (memcmp(source,ref,lgr)==0){
				status=1;
				break;
			}
		}
	}
	return(status);
}

/*-----------------------------------------------------------------
* Fonction   : Se connecter à l'application serveur
* objet      : Passage des étapes de connection à l'application
*              serveur.
* Parametre  : Aucun
* Retour     : 0 (bon) ou 1 (mauvais)
*-----------------------------------------------------------------
*/
UC	connect_host(PC number_phone,PC site)
{
	L l_result = 1;

	l_result = Dial((PC)number_phone,g_trace);

	if(l_result!=BON_CON)
	{
		CUMORE_CMI_AFFICHE_MESSAGE(117,138,1000);
		return PAS_CON;
	}

	sortir_boucle=KO;
	memset((PC)&g_tompon1[0],'\x00',sizeof(g_tompon1));

	l_result = ValidAbonne(site);
	if(l_result!=OK)
	{
		hang_up(1,g_trace);
		Os__xprint("Valid Abonne");
		CUMORE_CMI_AFFICHE_MESSAGE(187,188,1000);//kamal06
		return KO;
	}
	l_result=OK;
	return OK;
}
/*-----------------------------------------------------------------
* Fonction   : Se connecter à l'application serveur  (NBE)
* objet      : Passage des étapes de connection à l'application
*              serveur.
* Parametre  : Aucun
* Retour     : 0 (bon) ou 1 (mauvais)
*-----------------------------------------------------------------
*/
UC	connect_host_GPRS(PC number_phone,PC site)
{
	int i_res =2 ;
	int res = 0;

	memcpy(g_szAPN,FicPara.apn_gprs,40);
	memcpy(g_szLogin,FicPara.login_gprs,40);
	memcpy(g_szPassword,FicPara.pwd_gprs,40);
	memcpy(g_szAPN,FicPara.apn_gprs,40);
	memcpy(g_szHostName,FicPara.ip_gprs,15);
	g_nPort=atol((PC)FicPara.port_gprs);

//	i_res=CallPerformTCP_GPRS(g_szHostName, g_nPort);
//
//	if (i_res==KO)
//		return KO;
//	else
	{
		res=CallPerformConnectionGPRS_TLS((char*)FicPara.ip_gprs,atoi((char*)FicPara.port_gprs));
		if (res == OK)
			return OK ;
		else
			return KO ;
	}
	
	return OK;
}

UC	connect_host_GSM(PC number_phone,PC site)
{
	return OK;
}
UC	connect_host_IP(PC number_phone,PC site)
{
	int				res =0;
	
	res = CallPerformConnectionIP((char*)FicPara.ip_gprs,atoi((char*)FicPara.port_gprs));

	if (res ==OK)
		return OK;
	else
		return KO;

	return OK;
}
/*-----------------------------------------------------------------
* Fonction   :ReceptParam
* objet      :Réception des paramétres de CMI
*
* Parametre  :
* Retour     :
*-----------------------------------------------------------------
*/
L ReceptParam(UL delay, PC mot,UL Lg)
{
	L	i=0;
	L l_l_status	=	0;

	g_size=0;
	memset((PC)&g_tompon1[0],'\x00',MAX_MESS);
	l_l_status=receivepac(g_tompon1,g_trace);
	if(l_l_status!=OK)
	{
		return RECEPTION_KO;
	}
	for(i=0;i<50;i++) //A CORRIGER
		g_tompon1[i] = g_tompon1[i] & 0x7f;


	if(rechmem1(g_tompon1,100,(PUC)mot,Lg) != 1)
	{
		ShowTrace("rechmem1 KO");
		return KO;
	}
	return l_l_status;

}

/*-----------------------------------------------------------------
* Fonction   :FixationParam
* objet      :émission de la fixation des paramètres pour X25
*
* Parametre  :NEANT
* Retour     :résultat de la fonction
*-----------------------------------------------------------------
*/
L FixationParam()
{
	C				l_snd_data[30] = "";
	L				l_result;

	l_snd_data[0] = DLE;

	memset((PC)&g_tompon1[0],'\x00',sizeof(g_tompon1));

	strncpy(&l_snd_data[1], (PC)"SET 1:0,2:0,3:0,4:9,5:0", 23);
	l_snd_data[24] = CR;
	Emettre((PC)l_snd_data,25);
	l_result = KO;
	l_result = ReceptParam(delayI,(PC)"\x0d\x0A",2);
	if(l_result!=OK)
		return KO;
	return OK;
}

//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			NOM			:Emettre
//			DESCRIPTION	:envoi de message
//
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
long Emettre(char *txbuf,unsigned short count)
{
	size_t	lg;
	int		ret = -1;

	if (FicPara.g_type_connexion=='R')
	{
		//Fermer17reset_buf ( gmodem , _send_id);
		//Fermer17mask_event (gmodem,COM_SEND_EMPTY | DCD );
	}

/*
	if(count < 256)
		eft30printhexKamal(txbuf,count);
	else
	{
		if(count < 512)
		{
			eft30printhexKamal(txbuf,256);
			eft30printhexKamal(txbuf+256,count-256);
		}
		else
		{
			eft30printhexKamal(txbuf,256);
			eft30printhexKamal(txbuf+256,256);
			eft30printhexKamal(txbuf+512,count-256);
		}
	}
*/
	if (FicPara.g_type_connexion=='M')
		;//Fermer17Send_Frame_GSM((PC)txbuf,count);
	if (FicPara.g_type_connexion=='G')
		Emettre_Frame_GPRS((PC)txbuf,count);
	if (FicPara.g_type_connexion=='R')
		lg=Telium_Fwrite((char*)txbuf,1,count,gmodem);
	if (FicPara.g_type_connexion=='I')
	{
		if (FicPara.Etat_TLS==0)	//Kamalssl
		{
			SendFrameIP(txbuf,count);
		}
		else
		{
			ret=SendFrameSSL(txbuf,count);
			if(ret!=OK)
			{
				return KO;
			}
		}
	}
	return(OK);
}
/*-----------------------------------------------------------------
* Fonction   :ValidAbonne
* objet      :émission du numéro d'abonnée
*
* Parametre  :site
* Retour     :résultat de la fonction
*-----------------------------------------------------------------
*/
L ValidAbonne(PC site)
{
	C				bufemis[26];
	L				l_result;
	L				l_lg;

	l_lg=strlen(site);

	memcpy(bufemis,site,l_lg);
	memcpy(&bufemis[l_lg],"DCBLEUTA",8);
	memcpy(&bufemis[l_lg+8],"\x0D",1);
	Emettre((PC)bufemis,l_lg+9);
	l_result=KO;
	l_result=ReceptParam(delayI, (PC)"COM",3) ;
	if(l_result!=OK)
		return KO;
	return OK;
}
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			NOM			:receive
//			DESCRIPTION	:traitement de la réception physique
//
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
long receive(unsigned char *buffer,char trace)
{
	unsigned char	cr;
	size_t			lg,lgmess;
	unsigned char	st;
	char			trancherecu[1024];

	int				nError;
	if (FicPara.g_type_connexion == 'M')
	;//Fermer17nError=Receive_Frame_GSM(buffer);
	if (FicPara.g_type_connexion == 'G')
	{
		nError=Receive_Frame_GPRS(buffer);
		if(FLAG_Ref_SSL == -1)
		{
			//Deconnection_GPRS_2();//probleme de retour a la premiere application
			Deconnection_GPRS();//Belhadeg correction du probleme du retour a la premiere application
			//Telium_Ttestall(0, 4 * 100);
		}

	}
	if (FicPara.g_type_connexion == 'I')
	{
		if(FicPara.Etat_TLS==0)		//Kamalssl
			nError=RecieveFrameIP((char*)buffer);
		else
			nError=RecieveFrameSSL((char*)buffer);
	}
	if (FicPara.g_type_connexion != 'R')
	{
		if (!nError) // rien n'est reçu
		{
			ShowTrace("rien n'est reçu");

			return KO;
		}
		ShowTrace("bien reçu");
		return OK;
	}

	//Fermer17reset_buf ( gmodem , _receive_id);
	//Fermer17mask_event (gmodem,COM_REC_NOT_EMPTY| DCD );
	cr=Telium_Ttestall ( MODEM , 2000 );
	if ( cr == MODEM )
	{
//		ShowTrace("1111");
		Telium_Status(gmodem,&st);
		if( st & COM_REC_NOT_EMPTY )
		{
			//			ShowTrace("2222");
			Telium_Ttestall(0,100);
			memset(buffer,0,sizeof(buffer));
			lgmess=0;

			do{
				memset(trancherecu,0,256);
				lg=Telium_Fread(trancherecu,1,20,gmodem);
				//ShowTrace(trancherecu);
				memcpy(buffer+lgmess,trancherecu,lg);
				lgmess += lg;

				Telium_Ttestall(0,10);
			}while(lg != 0);

//			ShowTrace("receive");
			//eft30printhexKamal(buffer,lgmess);
			//ShowTrace(buffer);
			//for(i=0;i< lgmess - 2;i++)
			//{
			//	buffer[i] = buffer[i+2];
			//}
			//buffer[ lgmess - 2] =0;
			//buffer[ lgmess - 1] =0;
			//l_msg=lgmess-2;
			l_msg=lgmess;
		}
		if ( st & DCD )
		{
			CUMORE_CMI_AFFICHE_MESSAGE(118,138,1000);
			//printf("\x1B""  Ligne Perdue\n");
			ShowTrace("Comm. Ligne perdue");
			//eft30print("Comm. Ligne perdue",1);
			typehangup=0;
			//Telium_Fclose(stdin());
			return(KO);
		}
	}
	else if ( cr == KEYBOARD ) //A developper
	{
		CUMORE_CMI_AFFICHE_MESSAGE(33,138,1000);
		ShowTrace("Comm. Abandon");
		typehangup=1;
		return(KO);
	}
	else if ( cr == TIMEOUT  )
	{
		CUMORE_CMI_AFFICHE_MESSAGE(36,138,1000);
		ShowTrace("Comm. TimeOut");
		if(typehangup==0)typehangup=0;
		else typehangup=1;
		return(KO);
	}
	else
	{
		ShowTrace("Comm. Autre");
		return(KO);
	}
	typehangup=1;
	return OK;
}


long receivepac(unsigned char *buffer,char trace)
{
	unsigned char cr;
	size_t lg,lgmess;
	unsigned char st;
	char	trancherecu[256];


	int nError;


	if (FicPara.g_type_connexion=='G'){
		/*__TCP_TEST_RECEIVE_100K 	__TCP_TEST_ECHO  __TCP_TEST_LOOP_HTTP 	*/
		nError=Receive_Frame_GPRS(buffer);
		if (nError==0)
				return(OK);
		else
				return(KO);//KO
	}

	//Fermer17reset_buf ( gmodem , _receive_id);
	//Fermer17mask_event (gmodem,COM_REC_NOT_EMPTY| DCD );
	cr=Telium_Ttestall ( MODEM , 500 );
	if ( cr == MODEM )
	{
		Telium_Status(gmodem,&st);
		if( st & COM_REC_NOT_EMPTY )
		{
			Telium_Ttestall(0,50);
			memset(buffer,0,sizeof(buffer));
			lgmess=0;

			do{
				memset(trancherecu,0,256);
				lg=Telium_Fread(trancherecu,1,20,gmodem);
				memcpy(buffer+lgmess,trancherecu,lg);
				lgmess += lg;

				Telium_Ttestall(0,10);
			}while(lg != 0);

//			ShowTrace("receive");
			//eft30printhex(buffer,lgmess);
			l_msg=lgmess;
		}
		if ( st & DCD )
		{
			//CUMORE_CMI_AFFICHE_MESSAGE(118,138,1000);
			ShowTrace("Comm. Ligne perdue");
			typehangup=0;
			return(KO);
		}
	}
	else if ( cr == KEYBOARD )
	{
		//CUMORE_CMI_AFFICHE_MESSAGE(33,138,1000);
		ShowTrace("Comm. Abandon");
		typehangup=1;
		return(KO);
	}
	else if ( cr == TIMEOUT  )
	{
		//CUMORE_CMI_AFFICHE_MESSAGE(36,138,1000);
		ShowTrace("Comm. TimeOut");
		if(typehangup==0)typehangup=0;
		else typehangup=1;
		return(KO);
	}
	else
	{
		ShowTrace("Comm. Autre");
		return(KO);
	}
	typehangup=1;
	return OK;
}
/*-----------------------------------------------------------------
* Fonction   :rechmem1
* objet      :
*
* Parametre  :source,lgs,ref,lgr
* Retour     :résultat de la recherche
*-----------------------------------------------------------------
*/
L   imadrech(PC source,UL lgs,PC ref,UL lgr)
{
	UL    i;
	UL   status;

	for (i=0,status=0;(i<lgs)&&(status==0);i++,source++)
	{
		if (*source==*ref)
		{
			if (memcmp(source,ref,lgr)==0)
			{
				status=1;
				break;
			}
		}
	}
	return status;
}

/****************************************************************************
*                                                                           *
*      SOUS-PROGRAMME :  Teleparametrage                                    *
*                                                                           *
*****************************************************************************
*                                                                           *
*      FONCTION :                                                           *
*                                                                           *
*      ENTREE   :  Numeroter												*
*					InitModem												*
*                                                                           *
*      SORTIE   :  status                                                   *
*                                                                           *
****************************************************************************/
UC	Teleparametrage(UC Numeroter,UC InitModem)
{
	UC	phone[17],site[16];
	UC	status;
	US	Essai_cnx = 0;
	US	rejet;
	US InterfaceWifi =0;

	FLAG_Ref_SSL=0;			//Kamalssl


//Mouna050405_début
	if(Numeroter)
	{
		do{
			UC Message[128];

			sprintf(Message,"REFERENCEMENT\nCONNEXION:%d\nEN COURS",Essai_cnx);

			//APEMV_UI_MessageDisplayDelay (Message, 0);

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
				status = connect_host_GSM((PC)phone,(PC)site);
			if(FicPara.g_type_connexion == 'R')
				status = connect_host((PC)phone,(PC)site);
			if(FicPara.g_type_connexion == 'G')
				status = connect_host_GPRS((PC)phone,(PC)site);
			if(FicPara.g_type_connexion == 'I')
			{
				if(FicPara.Etat_TLS==0)	//Kamalssl
					status = connect_host_IP((PC)phone,(PC)site);
				else
					status = connect_host_IP_TLS();
			}
			if(status != OK)
			{
				hang_up(0,g_trace);
//				InterfaceWifi++;
//				if(InterfaceWifi==2 && (FicPara.g_type_connexion == 'G')){
//					Wifi_PowerOff();
//					Wifi_PowerOn();
//					//Telium_Ttestall(0, 1*100);
//					InterfaceWifi = 0;
//						}
			}
			Essai_cnx++;


		}while((Essai_cnx < 3)&&(status != OK));
		if(status != OK){
			//MessageWithoutConfirmation("echec de connexion", "file://flash/HOST/naps-tpe-icons-4.png");
			MessageWithDelay("ECHEC DE CONNEXION", "file://flash/HOST/naps-tpe-icons-4.png", 2000);
			return CONNEXION_KO;
		}
	}
//Mouna050405_fin

	rejet	=	0;
	status = OK;

	CUMORE_CMI_AFFICHE_MESSAGE(34,138,1000);
	//Uti__display1_clr2("TELEPARAMETRAGE");

	Os__display(1,0,"POS PARAM");
	//Os__xprintaa("0001");
	status = Transmission_file(FILE_PARAM);
	//Os__xprintaa("status");
	//Os__xprintd(status);
	//status = KO ;
	if(status != OK)
	{
		Os__xprint((PC)"Pb. POS PARAMETER");
		//Os__display(1,0,"POS PARAM");

		//Os__xprintaa("0003");

		if(InitModem)
			hang_up(typehangup,g_trace);
		return KO;
	}

	FLAG_Ref_SSL=1;		//Kamalssl

	Os__display(1,0,"KEY");
	status = Transmission_file(FILE_KEY);
	if(status != OK)
	{
		Os__xprint((PC)"Pb. KEY");
		if(InitModem)
			hang_up(typehangup,g_trace);
		return KO;
	}

	Os__display(1,0,"MAC KEY");
	status = Transmission_file(FILE_MACKEY);
	if(status != OK)
	{
		Os__xprint((PC)"Pb. MACKEY");
		if(InitModem)
			hang_up(typehangup,g_trace);
		return KO;
	}

	Os__display(1,0,"BIN");
	status = Transmission_file(FILE_BIN);
	if(status != OK)
	{
		Os__xprint((PC)"Pb. BIN");
		if(InitModem)
			hang_up(typehangup,g_trace);
		return KO;
	}

	Os__display(1,0,"PREFIXE");
	status = Transmission_file(FILE_PREFIXES);
	if(status != OK)
	{
	  Os__xprint((PC)"Pb. Prefixes");
	  if(InitModem)
		  hang_up(typehangup,g_trace);
	  return KO;
	}

	Os__display(1,0,"COMPTE DOM.");
	status = Transmission_file(FILE_COMPTE);
	if(status != OK)
	{
	  Os__xprint((PC)"Pb. COMPTE DOM.");
	  if(InitModem)
		  hang_up(typehangup,g_trace);
	  return KO;
	}


	Os__display(1,0,"EMV AID");
	status = Transmission_file(FILE_EMV_AID);
	if(status != OK)
	{
	  Os__xprint((PC)"Pb. EMV AID");
	  if(InitModem)
		  hang_up(typehangup,g_trace);
	  return KO;
	}

	Os__display(1,0,"EMV KEY");
	status = Transmission_file(FILE_EMV_KEYS);
	if(status != OK)
	{
	  Os__xprint((PC)"Pb. EMV KEY");
	  if(InitModem)
		  hang_up(typehangup,g_trace);
	  return KO;
	}

	Os__display(1,0,"EMV TAC");
	status = Transmission_file(FILE_EMV_TAC);
	if(status != OK)
	{
	  Os__xprint((PC)"Pb. EMV TAC");
	  if(InitModem)
		  hang_up(typehangup,g_trace);
	  return KO;
	}

	Os__display(1,0,"EMV TDOL");
	status = Transmission_file(FILE_EMV_TDOL);
	if(status != OK)
	{
	  Os__xprint((PC)"Pb. EMV TDOL");
	  if(InitModem)
		  hang_up(typehangup,g_trace);
	  return KO;
	}

	Os__display(1,0,"EMV DDOL");
	status = Transmission_file(FILE_EMV_DDOL);
	if(status != OK)
	{
	  Os__xprint((PC)"Pb. EMV DDOL");
	  if(InitModem)
		  hang_up(typehangup,g_trace);
	  return KO;
	}

	Os__display(1,0,"EMV RAND CALL");
	status = Transmission_file(FILE_EMV_RAND);
	if(status != OK)
	{
	  Os__xprint((PC)"Pb. EMV RAND");
	  if(InitModem)
		  hang_up(typehangup,g_trace);
	  return KO;
	}

	Os__display(1,0,"EMV TCC");
	status = Transmission_file(FILE_EMV_TCC);
	if(status != OK)
	{
	  Os__xprint((PC)"Pb. EMV TCC");
	  if(InitModem)
		  hang_up(typehangup,g_trace);
	  return KO;
	}

	/*//Init_Param_EMV();
	#ifndef WIN32
	//EmvInit();//AVOIR
	#endif*/


	Os__display(1,0,"PARA INTERCHANGE");
	status = Transmission_file(FILE_INTERCHANGE);
	if(status != OK)
	{
	  Os__xprint((PC)"Pb. PARA INTERCHANGE");
	  if(InitModem)
		  hang_up(typehangup,g_trace);
	  return KO;
	}

	FLAG_Ref_SSL=2;	//Kamalssl

	Os__display(1,0,"TPE MENU.");
	status = Transmission_file(FILE_MENUTPE);
	if(status != OK)
	{
	  Os__xprint((PC)"Pb. Menu TPE");
	  if(InitModem)
		  hang_up(typehangup,g_trace);
	  return KO;
	}

	/*//Init_Param_EMV();
	#ifndef WIN32
	//EmvInit();//AVOIR
	#endif*/



	Os__saved_set(&FicPara.status,OK,1);

	if(InitModem)
	  hang_up(typehangup,g_trace);

	if(R_Transac8583.l_champs44>=12)
	{
		Os__saved_set((uchar*)&FicPara.Update_param_Emv,'0',1);

		if(FicPara.Update_Appli != '1')
			Os__saved_set((uchar*)&FicPara.Update_Appli,R_Transac8583.champs44[1],1);

		Os__saved_set((uchar*)&FicPara.Update_bin,'0',1);

		if(FicPara.Update_LN != '1')
			Os__saved_set((uchar*)&FicPara.Update_LN,R_Transac8583.champs44[4],1);

		Os__saved_set((uchar*)&FicPara.Update_mk,'0',1);
		Os__saved_set((uchar*)&FicPara.Update_mac,'0',1);
		Os__saved_set((uchar*)&FicPara.Update_compt_dom,'0',1);
		Os__saved_set((uchar*)&FicPara.Update_pos_param,'0',1);
	}

	/* realease the SSL socket */

	FLAG_Ref_SSL=-1;	//Kamalssl

	return	OK;
}

//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			NOM			: Transmission_file
//			DESCRIPTION	: gestion de la transmission
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			ENTREE		: Nom de fichier
//
//			SORTIE		:resultat de la transmisson
//
//			REMARQUE	:
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
uchar	Transmission_file(uchar file_name)
{
	uchar 	status;
	Ushort	rejet=0;
	uchar	Premier_Mess;

	g_Num_Seq_Bloc = 0;
	Premier_Mess = 1;
	//	ShowTrace("TF0");
	do{
	//	Os__xprintaa("0111");

		memcpy(&E_Transac8583.e_type_mess,"1304",4);
		memcpy(&E_Transac8583.r_type_mess,"1314",4);
		//		ShowTrace("TF1");
		status = Transmission(E_Transac8583.e_type_mess,bitmap1304,file_name);
//		Os__xprintaa("0112");

		//ShowTrace("TF2");
		switch(status)
		{
		//Os__xprintaa("0113");
		case OK:
			//ShowTrace("TF3");
			if(
				memcmp(R_Transac8583.champs39,"000",3)&&
				memcmp(R_Transac8583.champs39,"300",3)
				)
			{
				/*Printtampon(R_Transac8583.champs39,3);*/
				ShowTrace((char*)R_Transac8583.champs39);
				ShowTrace("TF4");
				status = REJET_RECEPT;
				rejet += 1;
				break;
			}
			//ShowTrace("TF5");
			switch(file_name)
			{
			case FILE_TRS:
				//ShowTrace("TF6");
				break;
			case FILE_KEY:
				status = Trait_transfer_Key(Premier_Mess);
				break;
			case FILE_MACKEY:
				status = Trait_transfer_MacKey(Premier_Mess);
				break;
			case FILE_BIN:
				status = Trait_transfer_BIN(Premier_Mess);
				break;
			case FILE_PREFIXES:
				status = Trait_transfer_Prefixes(Premier_Mess);
				break;
			case FILE_COMPTE:
				status = Trait_transfer_Compte_Dom(Premier_Mess);
				break;
			case FILE_DOM:
				status = Trait_transfer_modif_compte_Dom();
				break;
				case FILE_LOP:
				status = Trait_transfer_Lop(Premier_Mess);
				break;
			case FILE_PARAM:
				//ShowTrace("TF7");
				status = Trait_transfer_param(Premier_Mess);
				break;
			case FILE_EMV_AID:
				status = Trait_transfer_EMV_AID(Premier_Mess);
				break;
			case FILE_EMV_KEYS:
				status = Trait_transfer_EMV_KEYS(Premier_Mess);
				break;
			case FILE_EMV_TAC:
				status = Trait_transfer_EMV_TAC(Premier_Mess);
				break;
			case FILE_EMV_TDOL:
				status = Trait_transfer_EMV_TDOL(Premier_Mess);
				break;
			case FILE_EMV_DDOL:
				status = Trait_transfer_EMV_DDOL(Premier_Mess);
				break;
			case FILE_EMV_RAND:
				status = Trait_transfer_EMV_Rand_Call(Premier_Mess);
				break;
			case FILE_EMV_TCC:
				status = Trait_transfer_EMV_TCC(Premier_Mess);
				break;
			case FILE_INTERCHANGE:
				status = Trait_transfer_INTERCHANGE(Premier_Mess);
				break;
			case FILE_MENUTPE:
				status = Trait_transfer_MenuTPE(Premier_Mess);
				break;
			default:
				break;
			}
			if((status!=OK)&&(status!=FIN_TRANSFER))
				rejet += 1;
            else
				Premier_Mess = 0;
			break;

			case RECEPT_KO:
			case FORMAT_MSG_REC_KO:
				status = REJET_RECEPT;
				rejet += 1;
				break;

			case FORMAT_KO:
			case EMISSION_KO:
				status = KO;
				break;

			default:
				status = KO;
				break;
		}
	}while( (status!=FIN_TRANSFER) && (status!=KO) && (rejet <1) ) ;
	//Os__xprintaa("0114");
	if((status!=FIN_TRANSFER))
		return status;
	//Os__xprintaa("0115");
	PutAllParam();
	return OK;
}

//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			NOM			:Transmission
//			DESCRIPTION	:gestion de la transmission
//
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			ENTREE		:type de message
//						 code de traitement
//
//			SORTIE		:resultat de la transmisson
//
//			FCT APP		:Format
//						 Emission
//						 Reception
//
//			REMARQUE	:
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
uchar	Transmission(uchar *typemessage,uchar *bitmap,uchar file_name)
{
	uchar status;
	//Os__xprintaa("1111");
	//	ShowTrace("T0");
	status = Format(typemessage,bitmap,file_name);
	//Os__xprintaa("1112");
	if(status != OK)
	{
		ShowTrace("T1");
		return FORMAT_KO;
	}

	status = Emission(typemessage,bitmap);
	//Os__xprintaa("       0003");
	if(status != OK)
	{
		ShowTrace("T2");
		return EMISSION_KO;
	}
	memset(g_tompon1,0,sizeof(g_tompon1));
	status = Reception();
	//Os__xprintaa("       0002");
	return status;
	//return FIN_TRANSFER;
}

//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			NOM			:Format
//			DESCRIPTION	:formatage du message à émettre
//
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			ENTREE		:	type de message
//						 		Bitmap
//								Nom de fichier dans la cas du message 1304
//
//			SORTIE		:	résultat de formatage
//
//			REMARQUE	:
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
uchar	Format(uchar *typemessage,uchar *bitmap,uchar file_name)
{
	Ushort	l_l_typemessage ;
	uchar	tmp[5];
	uchar	Date_YYMMDDHHmmss[13];
	uchar	montant[13] ;
	DATE date;

	uchar   iso2[38];
	uchar	j;
	Ushort	Index_Cmpte,OffSet;
	uchar	Tmp_Num_Seq[4];
    Ulong	Montant_remplacement;

	int 	i;
	char	chmps72[100] = {'\0'};




    //	ShowTrace("F0");
	memset(tmp,0,5);
	memcpy(tmp,typemessage,4);
	l_l_typemessage = (Ushort)atol((PC)tmp);

	Telium_Read_date(&date);
	sprintf ((char*)Date_YYMMDDHHmmss,
		"%.2s%.2s%.2s%.2s%.2s%.2s",
		date.year,
		date.month,
		date.day,
		date.hour,
		date.minute,
		date.second);
	memcpy(E_Transac8583.champs7,Date_YYMMDDHHmmss,10);

	/*	Os__read_date(Date_sys);
	Os__read_time_sec(Time_sys);
	memset(Date_YYMMDDHHmmss,'\0',13);
	memcpy(Date_YYMMDDHHmmss,&Date_sys[4],2);
	memcpy(&Date_YYMMDDHHmmss[2],&Date_sys[2],2);
	memcpy(&Date_YYMMDDHHmmss[4],Date_sys,2);
	memcpy(&Date_YYMMDDHHmmss[6],Time_sys,6);
	memcpy(E_Transac8583.champs7,Date_YYMMDDHHmmss,10);*/

	memcpy(&E_Transac8583.champs71[0],"00000001",8);
	g_Num_Seq_Bloc +=1;
	memset(Tmp_Num_Seq,0,4);
	sprintf((PC)Tmp_Num_Seq,"%03d",g_Num_Seq_Bloc);
	memcpy(&E_Transac8583.champs71[5],Tmp_Num_Seq,3);
	memcpy(E_Transac8583.champs12,Date_YYMMDDHHmmss,12);
	//	ShowTrace("F1");
	switch(l_l_typemessage)
	{
	case	1304: // Gestion de fichier
//		ShowTrace("F0");
		memcpy(E_Transac8583.e_type_mess,"1304",4);
		memcpy(E_Transac8583.r_type_mess,"1314",4);
		memcpy(E_Transac8583.bitmap,bitmap,8);
		memcpy(E_Transac8583.bitmap2,bitmap+8,8);
		memcpy(E_Transac8583.champs11,FicPara.Message_Number,6);
		IncStan();

		memcpy(E_Transac8583.champs17,&Date_YYMMDDHHmmss[2],4);
		memcpy(E_Transac8583.champs24,"300",3);
		memcpy(E_Transac8583.champs37,FicPara.Num_sequence,6);
		memcpy(&E_Transac8583.champs37[6],FicPara.Num_seqAnnul,6) ;

		memcpy(E_Transac8583.champs41,FicPara.Num_Terminal,8);
		memset(E_Transac8583.champs42,' ',15);
		memcpy(E_Transac8583.champs42,FicPara.Num_point_vente,strlen((PC)FicPara.Num_point_vente));
		E_Transac8583.l_champs53 = 10;
		memcpy(E_Transac8583.champs53,"0199000000",10);
		switch(file_name)
		{
		case FILE_TRS:
			E_Transac8583.l_champs101 = 11;
			memcpy(E_Transac8583.champs101,"TRANSACTION",11);
			break;
		case FILE_KEY:
			E_Transac8583.l_champs101 = 3;
			memcpy(E_Transac8583.champs101,"KEY",3);
			break;
		case FILE_MACKEY:
			E_Transac8583.l_champs101 = 7;
			memcpy(E_Transac8583.champs101,"MAC KEY",7);
			break;
		case FILE_BIN:
			E_Transac8583.l_champs101 = 10;
			memcpy(E_Transac8583.champs101,"BIN_LOCAUX",10);
			break;
		case FILE_PREFIXES:
			E_Transac8583.l_champs101 = 8;
			memcpy(E_Transac8583.champs101,"PREFIXES",8);
			break;
		case FILE_COMPTE:
			E_Transac8583.l_champs101 = 6;
			memcpy(E_Transac8583.champs101,"COMPTE",6);
			break;
		case FILE_DOM:
			bitset(E_Transac8583.bitmap2,72-64);
			E_Transac8583.l_champs72 = 2;
			Index_Cmpte = FicPara.Index_compte;
			memcpy(E_Transac8583.champs72,Fic_compte_dom.tab_compte[Index_Cmpte].Indice_compte,2);
			E_Transac8583.l_champs101 = 13;
			memcpy(E_Transac8583.champs101,"DOMICILIATION",13);
			break;
		case FILE_LOP:
			E_Transac8583.l_champs101 = 9;
			memcpy(E_Transac8583.champs101,"STOP LIST",9);
			memcpy(&E_Transac8583.champs71[5],lop_gest.NumBloc,3);
			break;
		case FILE_PARAM:
			E_Transac8583.l_champs101 = 13;
			memcpy(E_Transac8583.champs101,"POS PARAMETER",13);
			break;
		case FILE_EMV_AID:
			E_Transac8583.l_champs101 = 7;
			memcpy(E_Transac8583.champs101,"EMV_AID",7);
			break;
		case FILE_EMV_KEYS:
			E_Transac8583.l_champs101 = 8;
			memcpy(E_Transac8583.champs101,"EMV_KEYS",8);
			break;
		case FILE_EMV_TAC:
			E_Transac8583.l_champs101 = 7;
			memcpy(E_Transac8583.champs101,"EMV_TAC",7);
			break;
		case FILE_EMV_TDOL:
			E_Transac8583.l_champs101 = 8;
			memcpy(E_Transac8583.champs101,"EMV_TDOL",8);
			break;
		case FILE_EMV_DDOL:
			E_Transac8583.l_champs101 = 8;
			memcpy(E_Transac8583.champs101,"EMV_DDOL",8);
			break;
		case FILE_EMV_RAND:
			E_Transac8583.l_champs101 = 13;
			memcpy(E_Transac8583.champs101,"EMV_RAND_CALL",13);
			break;
		case FILE_EMV_TCC:
			E_Transac8583.l_champs101 = 7;
			memcpy(E_Transac8583.champs101,"EMV_TCC",7);
			break;
		case FILE_INTERCHANGE:
			E_Transac8583.l_champs101 = 11;
			memcpy(E_Transac8583.champs101,"INTERCHANGE",11);
			break;
		case FILE_MENUTPE:
			E_Transac8583.l_champs101 = 8;
			memcpy(E_Transac8583.champs101,"MENU TPE",8);
			break;
		default:
			E_Transac8583.l_champs101 = 13;
			memcpy(E_Transac8583.champs101,"POS PARAMETER",13);
			break;
		}
		break;

		case	1100: // Demande de preautorisation
		case	1200: // Demande d'autorisation
		case 	0200: // Demande de recharge/Carte à Carte Prépayée/Transfert Carte à CASH par Hind SAHEL
		case	0300: // Demande de consultation écheances crédit par Hind SAHEL
			if ((StructInfoPayment.CodeFunction == CREDIT_PARTIEL) || (StructInfoPayment.CodeFunction == CREDIT_TOTAL)){
				memcpy(E_Transac8583.bitmap,bitmap,16);
			}else
				memcpy(E_Transac8583.bitmap,bitmap,8);

			if ((StructInfoPayment.CodeFunction != CAC_PREPAYE) && (StructInfoPayment.CodeFunction != CASHOUT_PREPAYE)){
				E_Transac8583.l_champs2 = StructInfoPayment.Holder_Card[0];
				memcpy(E_Transac8583.champs2,&StructInfoPayment.Holder_Card[1],StructInfoPayment.Holder_Card[0]);
			}
			if (StructInfoPayment.CodeFunction == CASHOUT_PREPAYE){
				E_Transac8583.l_champs2 = 16;
				memcpy(E_Transac8583.champs2,"0000000000000000",E_Transac8583.l_champs2);
			}
			memcpy(E_Transac8583.champs3,StructInfoPayment.Proc_Code,6);

			long_asc (montant, 12, &StructInfoPayment.Transaction_Amount);
			memcpy(E_Transac8583.champs4,montant,12);

			if (StructInfoPayment.CodeFunction == PAIEMENT_POURBOIRE){    //POURBOIRE
				Os__xprinta("pourboire 22");
				UL MNT_Tot = StructInfoPayment.Transaction_Amount + StructInfoPayment.MontantPourboireInterchange;

				long_asc (montant, 12, &MNT_Tot);
				memcpy(E_Transac8583.champs4,montant,12);
			}

			memcpy(E_Transac8583.champs11,StructInfoPayment.STAN,6);
			memcpy(StructInfoPayment.Transaction_DateTime,Date_YYMMDDHHmmss,12); //1.11.43
			memcpy(E_Transac8583.champs12,StructInfoPayment.Transaction_DateTime,12);
			memcpy(E_Transac8583.champs14,StructInfoPayment.Expired_Date,4);
			//champ17,		date business
			memcpy(E_Transac8583.champs17,&Date_YYMMDDHHmmss[2],4);

			if (StructInfoPayment.CodeFunction != CASHOUT_PREPAYE){
				Code_PointService(E_Transac8583.champs22,StructInfoPayment.Entry_Mode,
					StructInfoPayment.Entry_Pin,StructInfoPayment.Lg_pin);

					if (StructInfoPayment.Entry_Mode == TRS_MAG && (StructInfoPayment.SeviceCode[0]=='2' || StructInfoPayment.SeviceCode[0]=='6')){
						memcpy(E_Transac8583.champs35,&StructInfoPayment.Normal_Iso2[1],StructInfoPayment.Normal_Iso2[0]);
						for (i=0; (E_Transac8583.champs35[i] != '='); i++);
						E_Transac8583.champs35[i] = 'D';
						memcpy(E_Transac8583.champs22,"801",3);

					}else if(StructInfoPayment.Entry_Mode == TRS_MAG){
						memcpy(E_Transac8583.champs35,&StructInfoPayment.Normal_Iso2[1],StructInfoPayment.Normal_Iso2[0]);
						for (i=0; (E_Transac8583.champs35[i] != '='); i++);
						E_Transac8583.champs35[i] = 'D';
						memcpy(E_Transac8583.champs22,"901",3);

					}else if(StructInfoPayment.Entry_Mode == TRS_EMV){
						memcpy(E_Transac8583.champs35,&StructInfoPayment.Normal_Iso2[1],StructInfoPayment.Normal_Iso2[0]);
							for (i=0; i < StructInfoPayment.Normal_Iso2[0]; i++){
								if (E_Transac8583.champs35[i] == '='){
									E_Transac8583.champs35[i] = 'D';
									break;
								}
							}
						memcpy(E_Transac8583.champs22,"051",3);
					}else if(StructInfoPayment.Entry_Mode == TRS_MAN){
						memcpy(E_Transac8583.champs22,"601",3);
					}
				}
				if(l_l_typemessage!=1100)
					memcpy(E_Transac8583.champs24,"200",3);
				else
					memcpy(E_Transac8583.champs24,"101",3);
				memcpy(E_Transac8583.champs26,FicPara.MCC,4);

				E_Transac8583.l_champs32 = 6;
				memcpy(E_Transac8583.champs32,"011010",6);//memcpy(E_Transac8583.champs32,"101010",6); Enlevé par Hind SAHEL pour envoyer le code banque du commerçant
				//memcpy(E_Transac8583.champs32,"000011",6);//memcpy(E_Transac8583.champs32,"101010",6); Enlevé par Hind SAHEL pour envoyer le code banque du commerçant

				if (StructInfoPayment.CodeFunction != CASHOUT_PREPAYE){
					E_Transac8583.l_champs35 = StructInfoPayment.Normal_Iso2[0];
					memcpy(E_Transac8583.champs35,&StructInfoPayment.Normal_Iso2[1],StructInfoPayment.Normal_Iso2[0]);
				}
				memcpy(E_Transac8583.champs37,FicPara.Num_sequence,6);
				memcpy(&E_Transac8583.champs37[6],FicPara.Num_seqAnnul,6) ;
				//Os__xprint(E_Transac8583.champs37);
				memset(E_Transac8583.champs41,' ',8);
				memcpy(E_Transac8583.champs41,FicPara.Num_Terminal,strlen((PC)FicPara.Num_Terminal));
				//Os__xprintaa(E_Transac8583.champs41);
				memset(E_Transac8583.champs42,' ',15);
				memcpy(E_Transac8583.champs42,FicPara.Num_point_vente,strlen((PC)FicPara.Num_point_vente));

				E_Transac8583.l_champs43 = 24;
				memcpy(E_Transac8583.champs43,FicPara.Merchant_Adrees,24);
				if (StructInfoPayment.CodeFunction != CASHOUT_PREPAYE){
					//champ48
					E_Transac8583.l_champs48 = 0;
					if(StructInfoPayment.Entry_Pin==PIN_ONLINE)
					{
						memcpy(&E_Transac8583.champs48[E_Transac8583.l_champs48],"P10048",6); //T_MAN
						E_Transac8583.l_champs48 += 6;
						//ShowTrace_Mouna(StructInfoPayment.cle_travail_cryp);
						memcpy(&E_Transac8583.champs48[E_Transac8583.l_champs48],StructInfoPayment.cle_travail_cryp,48);
						E_Transac8583.l_champs48 += 48;
					}
					if(StructInfoPayment.Entry_Mode==TRS_MAN) //T_MAN
					{
//						StructInfoPayment.Entry_Pin = NO_PIN;
//						E_Transac8583.l_champs48 = 0;

						//Os__xprinta("Transaction Man");
						memcpy(&E_Transac8583.champs48[E_Transac8583.l_champs48],"P92003",6);
						E_Transac8583.l_champs48 += 6;
						memcpy(&E_Transac8583.champs48[E_Transac8583.l_champs48],StructInfoPayment.CVV2,3);
						E_Transac8583.l_champs48 += 3;
					}
					if(FicPara.Flag_service == 1) //avis badr
					{
						memset(&E_Transac8583.champs48[E_Transac8583.l_champs48],' ',44); /* 44= 6+9+6+23 */

						memcpy(&E_Transac8583.champs48[E_Transac8583.l_champs48],"P60009",6);
						E_Transac8583.l_champs48 += 6;
						memcpy(&E_Transac8583.champs48[E_Transac8583.l_champs48],StructInfoPayment.AV_NumContrat,strlen(StructInfoPayment.AV_NumContrat));
						E_Transac8583.l_champs48 += 9;
						memcpy(&E_Transac8583.champs48[E_Transac8583.l_champs48],"P61023",6);
						E_Transac8583.l_champs48 += 6;
						memcpy(&E_Transac8583.champs48[E_Transac8583.l_champs48],StructInfoPayment.AV_CodeMotif,strlen(StructInfoPayment.AV_CodeMotif));
						E_Transac8583.l_champs48 += 23;
						FicPara.Flag_service = 0;
						PutAllParam();
					} if(FicPara.Flag_service == 2) //Lydec badr
					{
						memset(&E_Transac8583.champs48[E_Transac8583.l_champs48],' ',68); /* 68= 6+10+6+10+6+30 */

						memcpy(&E_Transac8583.champs48[E_Transac8583.l_champs48],"I01010",6);
						E_Transac8583.l_champs48 += 6;
						memcpy(&E_Transac8583.champs48[E_Transac8583.l_champs48],StructInfoPayment.LY_RefContra,strlen(StructInfoPayment.LY_RefContra));
						E_Transac8583.l_champs48 += 10;
						memcpy(&E_Transac8583.champs48[E_Transac8583.l_champs48],"I02010",6);
						E_Transac8583.l_champs48 += 6;
						memcpy(&E_Transac8583.champs48[E_Transac8583.l_champs48],StructInfoPayment.LY_NumGSM,strlen(StructInfoPayment.LY_NumGSM));
						E_Transac8583.l_champs48 += 10;
						memcpy(&E_Transac8583.champs48[E_Transac8583.l_champs48],"I03030",6);
						E_Transac8583.l_champs48 += 6;
						memcpy(&E_Transac8583.champs48[E_Transac8583.l_champs48],StructInfoPayment.LY_AdrEmail,strlen(StructInfoPayment.LY_AdrEmail));
						E_Transac8583.l_champs48 += 30;
						FicPara.Flag_service = 0;
						PutAllParam();
					}

					//memcpy(&E_Transac8583.champs52,StructInfoPayment.pin_code_cryp,8);
					if(StructInfoPayment.Entry_Pin == PIN_ONLINE){
						if (StructInfoPayment.CodeFunction == PINCHANGE){
							memcpy(E_Transac8583.bitmap,bitmap,16);

							E_Transac8583.l_champs72 = 0;

							memcpy(E_Transac8583.champs72+E_Transac8583.l_champs72,"001",3);//
							E_Transac8583.l_champs72 += 3;

							E_Transac8583.l_champs72 += sprintf(E_Transac8583.champs72+E_Transac8583.l_champs72,"%.03d",8);
							E_Transac8583.l_champs72 += sprintf(E_Transac8583.champs72+E_Transac8583.l_champs72,StructInfoPayment.Pin_Change_crypte);
							}

							//memcpy(E_Transac8583.champs52,&StructInfoPayment.Pin_Block_crypte,8);
							memcpy(&E_Transac8583.champs52,StructInfoPayment.pin_code_cryp,8);

					}
					else if (StructInfoPayment.CodeFunction == PINCHANGE){ //1.11.31
							memcpy(E_Transac8583.bitmap,bitmap,16);

							E_Transac8583.l_champs72 = 0;

							memcpy(E_Transac8583.champs72+E_Transac8583.l_champs72,"001",3);//
							E_Transac8583.l_champs72 += 3;

							E_Transac8583.l_champs72 += sprintf(E_Transac8583.champs72+E_Transac8583.l_champs72,"%.03d",8);
							E_Transac8583.l_champs72 += sprintf(E_Transac8583.champs72+E_Transac8583.l_champs72,StructInfoPayment.Pin_Change_crypte);

					}

					E_Transac8583.l_champs53 = 10;
					memcpy(E_Transac8583.champs53,"0199000000",10);
					if(StructInfoPayment.Entry_Pin==PIN_ONLINE)
						memcpy(E_Transac8583.champs53+2,"01",2);
					else
						memcpy(E_Transac8583.champs53+2,"99",2);

					if(StructInfoPayment.Entry_Mode == TRS_EMV)
					{
						E_Transac8583.l_champs55 = StructInfoPayment.l_champ55;
						memcpy(E_Transac8583.champs55,StructInfoPayment.Champ55,StructInfoPayment.l_champ55);
						if (StructInfoPayment.PanSeqNum5F34[0] != 0)//Ajouté par Hind SAHEL pour certif
							memcpy(E_Transac8583.champs23,StructInfoPayment.PanSeqNumber,3);
						else
							memcpy(E_Transac8583.champs23,"000",3);
					}

					if (StructInfoPayment.CodeFunction == CREDIT_PARTIEL){
						memcpy(E_Transac8583.bitmap,bitmap,16);
						long_asc (montant, 12, &StructInfoPayment.Transaction_Amount);
						memcpy(E_Transac8583.champs4,montant,12);
						E_Transac8583.l_champs72 = 0;

						memcpy(E_Transac8583.champs72+E_Transac8583.l_champs72,"00?",3);//
						E_Transac8583.l_champs72 += 3;
						//memcpy(E_Transac8583.champs72+E_Transac8583.l_champs72,
						E_Transac8583.l_champs72 += sprintf(E_Transac8583.champs72+E_Transac8583.l_champs72,"001%.03d%s002012%s;",strlen(StructinfoEcheance[0].Echeance),
						StructinfoEcheance[0].Echeance,E_Transac8583.champs4);
						//Os__xprintaa(E_Transac8583.champs72);

					}
					if (StructInfoPayment.CodeFunction == CREDIT_TOTAL){
						memcpy(E_Transac8583.bitmap,bitmap,16);
						E_Transac8583.l_champs72 = 0;
						FicPara.NbrEchChoisi =1;
						for (j = 0; j < FicPara.NbrEchChoisi; j++)
						{
							sprintf(chmps72,"001%.03d%s002012%s*",strlen(StructinfoEcheance[j].Echeance),StructinfoEcheance[j].Echeance,StructinfoEcheance[j].Montant);
						}
						chmps72[strlen(chmps72) - 1] = ';';
						//Os__xprintaa(chmps72);
						E_Transac8583.l_champs72 += sprintf(E_Transac8583.champs72+E_Transac8583.l_champs72,"%.02d?",FicPara.NbrEchChoisi);
						E_Transac8583.l_champs72 += sprintf(E_Transac8583.champs72+E_Transac8583.l_champs72,"%s",chmps72);
						//Os__xprintaa(E_Transac8583.champs72);
					}
			}
			if(StructInfoPayment.CodeFunction == DE_RECHARGE || StructInfoPayment.CodeFunction == E_RECHARGE)
			{
			//Os__xprintaa("test champs63");
			//memcpy(E_Transac8583.champs12,StructInfoPayment.Transaction_DateTime,12);

			E_Transac8583.l_champs62 = 0;
			memset(R_Transac8583.champs62,'\0',sizeof(R_Transac8583.champs62));
			memset(E_Transac8583.champs62,'\0',sizeof(E_Transac8583.champs62));



			memcpy(E_Transac8583.champs62 + E_Transac8583.l_champs62,StructInfoPayment.Operateur,strlen(StructInfoPayment.Operateur));
			E_Transac8583.l_champs62 += strlen(StructInfoPayment.Operateur);

			memcpy(E_Transac8583.champs62 + E_Transac8583.l_champs62,"?",1);
			E_Transac8583.l_champs62 += 1;

			memcpy(E_Transac8583.champs62 + E_Transac8583.l_champs62,StructInfoPayment.NumeroGsm,strlen(StructInfoPayment.NumeroGsm));
			E_Transac8583.l_champs62 += strlen(StructInfoPayment.NumeroGsm);

						if(StructInfoPayment.CodeFunction == E_RECHARGE)
						{
						memcpy(E_Transac8583.champs62 + E_Transac8583.l_champs62,"?",1);
						E_Transac8583.l_champs62 += 1;
						memcpy(E_Transac8583.champs62 + E_Transac8583.l_champs62,StructInfoPayment.Promotion,strlen(StructInfoPayment.Promotion));
						E_Transac8583.l_champs62 += strlen(StructInfoPayment.Promotion);
						}
			}
			else if(StructInfoPayment.CodeFunction == OR_FACTURE50)
			{
				E_Transac8583.l_champs2 = 16;
				memcpy(E_Transac8583.champs2,"0000000000000000",E_Transac8583.l_champs2);

				E_Transac8583.l_champs62 = 0;
				memset(R_Transac8583.champs62,'\0',sizeof(R_Transac8583.champs62));
				memset(E_Transac8583.champs62,'\0',sizeof(E_Transac8583.champs62));
				//Os__xprintaa("001");
				memcpy(E_Transac8583.champs62 + E_Transac8583.l_champs62,StructInfoPayment.OR_ConsultantId,strlen(StructInfoPayment.OR_ConsultantId));
				E_Transac8583.l_champs62 += strlen(StructInfoPayment.OR_ConsultantId);
//				//Os__xprintaa("002");
//				memcpy(E_Transac8583.champs62 + E_Transac8583.l_champs62,"*",1);
//				E_Transac8583.l_champs62 += 1;
//				//Os__xprintaa("003");
//				memcpy(E_Transac8583.champs62 + E_Transac8583.l_champs62,StructInfoPayment.OR_ConsultantId,strlen(StructInfoPayment.OR_ConsultantId));
//				E_Transac8583.l_champs62 += strlen(StructInfoPayment.OR_ConsultantId);
//				//Os__xprintaa("004");


			}
			else if(StructInfoPayment.CodeFunction == OR_FACTURE51)
			{
				E_Transac8583.l_champs2 = 16;
				memcpy(E_Transac8583.champs2,"0000000000000000",E_Transac8583.l_champs2);
				//Os__xprintaa("001");

				E_Transac8583.l_champs62 = 0;
				memset(R_Transac8583.champs62,'\0',sizeof(R_Transac8583.champs62));
				memset(E_Transac8583.champs62,'\0',sizeof(E_Transac8583.champs62));
				memset(E_Transac8583.champs62,' ',37);
				//Os__xprintaa("001");
				memcpy(E_Transac8583.champs62 + E_Transac8583.l_champs62,StructInfoPayment.OR_ConsultantId,strlen(StructInfoPayment.OR_ConsultantId));
				E_Transac8583.l_champs62 += 7;

				memcpy(E_Transac8583.champs62 + E_Transac8583.l_champs62,StructInfoPayment.OR_invoiceNumber62,strlen(StructInfoPayment.OR_invoiceNumber62));
				E_Transac8583.l_champs62 += 10;

				//long_asc (montant, 12, &StructInfoPayment.Transaction_Amount);
				memcpy(E_Transac8583.champs4,montant,12);

				memcpy(E_Transac8583.champs62 + E_Transac8583.l_champs62,montant,strlen(montant));
				E_Transac8583.l_champs62 += 12;

				memcpy(E_Transac8583.champs62 + E_Transac8583.l_champs62,StructInfoPayment.STAN,strlen(StructInfoPayment.STAN));
			//	E_Transac8583.champs62[E_Transac8583.l_champs62]='B';
				E_Transac8583.l_champs62 += 6;

//				//Os__xprintaa("002");
//				memcpy(E_Transac8583.champs62 + E_Transac8583.l_champs62,"*",1);
//				E_Transac8583.l_champs62 += 1;
//				//Os__xprintaa("003");
//				memcpy(E_Transac8583.champs62 + E_Transac8583.l_champs62,StructInfoPayment.OR_ConsultantId,strlen(StructInfoPayment.OR_ConsultantId));
//				E_Transac8583.l_champs62 += strlen(StructInfoPayment.OR_ConsultantId);
//				//Os__xprintaa("004");


			}
			else if(StructInfoPayment.CodeFunction == RECH_PREPAYE)
			{

			E_Transac8583.l_champs62 = 0;
			memset(E_Transac8583.champs62,'\0',sizeof(E_Transac8583.champs62));

			if(memcmp(StructInfoPayment.Police,"\0",strlen(StructInfoPayment.Police)))
			{

			memcpy(E_Transac8583.champs62 ,"S?",2);
			E_Transac8583.l_champs62 = 2;
			memcpy(E_Transac8583.champs62 + E_Transac8583.l_champs62,StructInfoPayment.Police,strlen(StructInfoPayment.Police));
			E_Transac8583.l_champs62  += strlen(StructInfoPayment.Police);
			}
			}
			else if(StructInfoPayment.CodeFunction == REMISE)
			{

			E_Transac8583.l_champs62 = 0;


			memset(E_Transac8583.champs62,'\0',sizeof(E_Transac8583.champs62));
			memcpy(E_Transac8583.champs62,R_Transac8583.champs62,strlen(R_Transac8583.champs62));
			E_Transac8583.l_champs62 = strlen(R_Transac8583.champs62);

			}
			else if (StructInfoPayment.CodeFunction == PAIEMENT_NFOIS){

			memcpy(E_Transac8583.champs62 ,"N?",2);
			E_Transac8583.l_champs62 = 2;
			memcpy(E_Transac8583.champs62 + E_Transac8583.l_champs62,StructInfoPayment.Nbr_Fois,strlen(StructInfoPayment.Nbr_Fois));
			E_Transac8583.l_champs62  += strlen(StructInfoPayment.Nbr_Fois);
			memcpy(E_Transac8583.champs62+ E_Transac8583.l_champs62 ,"?",1);
			E_Transac8583.l_champs62  += 1;
			memcpy(E_Transac8583.champs62 + E_Transac8583.l_champs62,StructInfoPayment.montant_total,strlen(StructInfoPayment.montant_total));
			E_Transac8583.l_champs62  += strlen(StructInfoPayment.montant_total);
			memcpy(E_Transac8583.champs62+ E_Transac8583.l_champs62 ,"?",1);
			E_Transac8583.l_champs62  += 1;
			memcpy(E_Transac8583.champs62 + E_Transac8583.l_champs62,StructInfoPayment.ModeReglement,strlen(StructInfoPayment.ModeReglement));
			E_Transac8583.l_champs62  += strlen(StructInfoPayment.ModeReglement);

			}
			else if (StructInfoPayment.CodeFunction == PAIEMENT_DIFFERE){
				long_asc (montant, 12, &StructInfoPayment.MontantDiffere);
				//RemoveLeftZeros(montant) ;
				memcpy(E_Transac8583.champs62 ,"D?",2);
				E_Transac8583.l_champs62 = 2;
				memcpy(E_Transac8583.champs62 + E_Transac8583.l_champs62,montant,12);
				E_Transac8583.l_champs62  += strlen(montant);
			}

			else if (StructInfoPayment.CodeFunction == PAIEMENT_POURBOIRE){
				//Os__xprinta("montant.pourb");
				long_asc (montant, 12, &StructInfoPayment.Transaction_Amount);
				//RemoveLeftZeros(montant) ;
				memcpy(E_Transac8583.champs62 ,"P?",2);
				E_Transac8583.l_champs62 = 2;
				memcpy(E_Transac8583.champs62 + E_Transac8583.l_champs62,montant,12);
				E_Transac8583.l_champs62  += 12;
				memcpy(E_Transac8583.champs62 + E_Transac8583.l_champs62,"?",1);
				E_Transac8583.l_champs62  += 1;
				long_asc (montant, 12, &StructInfoPayment.MontantPourboireInterchange);
				memcpy(E_Transac8583.champs62 + E_Transac8583.l_champs62,montant,12);
				E_Transac8583.l_champs62  += 12;
			}
			else if (StructInfoPayment.CodeFunction == PAIEMENT_INTERCHANGE){
				long_asc (montant, 12, &StructInfoPayment.Transaction_Amount);
				memcpy(E_Transac8583.champs62 ,"I?",2);
				E_Transac8583.l_champs62 = 2;
				memcpy(E_Transac8583.champs62 + E_Transac8583.l_champs62,montant,12);
				E_Transac8583.l_champs62  += 12;
				memcpy(E_Transac8583.champs62 + E_Transac8583.l_champs62,"?",1);
				E_Transac8583.l_champs62  += 1;
				long_asc (montant, 12, &StructInfoPayment.MontantPourboireInterchange);
				memcpy(E_Transac8583.champs62 + E_Transac8583.l_champs62,montant,12);
				E_Transac8583.l_champs62  += 12;

				StructInfoPayment.Transaction_Amount += StructInfoPayment.MontantPourboireInterchange;
				long_asc (montant, 12, &StructInfoPayment.Transaction_Amount);
				memcpy(E_Transac8583.champs4,montant,12);
			}
			else if (StructInfoPayment.CodeFunction == NOTATION){
				memcpy(E_Transac8583.champs62 ,StructInfoPayment.NotationService,strlen(StructInfoPayment.NotationService));
				E_Transac8583.l_champs62 = strlen(StructInfoPayment.NotationService);
				//Os__xprintaa(E_Transac8583.champs62);
				if(StructInfoPayment.NotationService[8]== RECH_PREPAYE || StructInfoPayment.NotationService[8]== DECH_PREPAYE
				|| StructInfoPayment.NotationService[8]== CAC_PREPAYE )
					{
				memcpy(E_Transac8583.champs4 ,StructInfoNotation.DataPrepayee,12);
				memcpy(E_Transac8583.champs42 ,StructInfoNotation.DataPrepayee +12,12);

				if(StructInfoPayment.NotationService[8]== CAC_PREPAYE)
				//memset(E_Transac8583.champs22,'\0',sizeof(E_Transac8583.champs22));
				memcpy(E_Transac8583.champs22 ,StructInfoNotation.DataPrepayee + 24, 6);
					//Os__xprintaa(E_Transac8583.champs22);
			}
			else if (StructInfoPayment.NotationService[8]== PINCHANGE)
				{
				if(StructInfoPayment.Etat_PIN == 0)
				memcpy(E_Transac8583.champs5 ,"A" ,1);
				else
				memcpy(E_Transac8583.champs5 ,"C" ,1);
				}

			}
			memcpy(E_Transac8583.champs49,StructInfoPayment.Devise_acc.code_num,3);
			break;

		case 1201:
//			ShowTrace("F1");
			memcpy(E_Transac8583.bitmap,bitmap,8);
			E_Transac8583.l_champs2 = StructInfoPayment.Holder_Card[0];
			memcpy(E_Transac8583.champs2,&StructInfoPayment.Holder_Card[1],StructInfoPayment.Holder_Card[0]);
			memcpy(E_Transac8583.champs3,StructInfoPayment.Proc_Code,6);
			long_asc (montant, 12, &StructInfoPayment.Transaction_Amount);
			memcpy(E_Transac8583.champs4,montant,12);
			memcpy(E_Transac8583.champs11,StructInfoPayment.STAN,6);
			memcpy(E_Transac8583.champs12,StructInfoPayment.Transaction_DateTime,12);
			memcpy(E_Transac8583.champs14,StructInfoPayment.Expired_Date,4);
			//champ17,		date business
			memcpy(E_Transac8583.champs17,&Date_YYMMDDHHmmss[2],4);
			Code_PointService(E_Transac8583.champs22,StructInfoPayment.Entry_Mode,
				StructInfoPayment.Entry_Pin,StructInfoPayment.Lg_pin);
			if(l_l_typemessage!=1100)
				memcpy(E_Transac8583.champs24,"200",3);
			else
				memcpy(E_Transac8583.champs24,"101",3);
			memcpy(E_Transac8583.champs26,FicPara.MCC,4);

			E_Transac8583.l_champs32 = 6;
			memcpy(E_Transac8583.champs32,"011010",6);

			E_Transac8583.l_champs35 = StructInfoPayment.Normal_Iso2[0];
			memcpy(E_Transac8583.champs35,&StructInfoPayment.Normal_Iso2[1],StructInfoPayment.Normal_Iso2[0]);

			memcpy(E_Transac8583.champs37,FicPara.Num_sequence,6);
			memcpy(&E_Transac8583.champs37[6],FicPara.Num_seqAnnul,6) ;
			//Os__xprint((char*)E_Transac8583.champs37);
			memcpy(E_Transac8583.champs41,FicPara.Num_Terminal,8);

			memset(E_Transac8583.champs42,' ',15);
			memcpy(E_Transac8583.champs42,FicPara.Num_point_vente,strlen((PC)FicPara.Num_point_vente));

			E_Transac8583.l_champs43 = 24;
			memcpy(E_Transac8583.champs43,FicPara.Merchant_Adrees,24);

			//champ48
			E_Transac8583.l_champs48 = 0;
			if(StructInfoPayment.Entry_Pin==PIN_ONLINE)
			{
				memcpy(&E_Transac8583.champs48[E_Transac8583.l_champs48],"P10048",6); //T_MAN
				E_Transac8583.l_champs48 += 6;
				//ShowTrace_Mouna(StructInfoPayment.cle_travail_cryp);
				memcpy(&E_Transac8583.champs48[E_Transac8583.l_champs48],StructInfoPayment.cle_travail_cryp,48);
				E_Transac8583.l_champs48 += 48;
			}
			if(StructInfoPayment.Entry_Mode==TRS_MAN) //T_MAN
			{
//				StructInfoPayment.Entry_Pin = NO_PIN;
//				E_Transac8583.l_champs48 = 0;

				memcpy(&E_Transac8583.champs48[E_Transac8583.l_champs48],"P92003",6);
				E_Transac8583.l_champs48 += 6;
				memcpy(&E_Transac8583.champs48[E_Transac8583.l_champs48],StructInfoPayment.CVV2,3);
				E_Transac8583.l_champs48 += 3;
			}

			memcpy(E_Transac8583.champs49,StructInfoPayment.Devise_acc.code_num,3);

			memcpy(&E_Transac8583.champs52,StructInfoPayment.pin_code_cryp,8);

			E_Transac8583.l_champs53 = 10;
			memcpy(E_Transac8583.champs53,"0199000000",10);
			if(StructInfoPayment.Entry_Pin==PIN_ONLINE)
				memcpy(E_Transac8583.champs53+2,"01",2);
			else
				memcpy(E_Transac8583.champs53+2,"99",2);

			if(StructInfoPayment.Entry_Mode == TRS_EMV)
			{
				E_Transac8583.l_champs55 = StructInfoPayment.l_champ55;
				memcpy(E_Transac8583.champs55,StructInfoPayment.Champ55,StructInfoPayment.l_champ55);
				memcpy(E_Transac8583.champs23,StructInfoPayment.PanSeqNumber,3);
			}
			break ;

		case	1220: // Avis de transactions financière
//			ShowTrace("F2");
			memcpy(E_Transac8583.bitmap,bitmap,8);
			if(StrLineTra.Entry_Mode == TRS_MAN)
			{
/*				ShowTrace_Mouna("ad4");
				imp_buffer(StrLineTra.Compressed_Iso2,StrLineTra.Compressed_Iso2[0]);*/
				E_Transac8583.l_champs2 =StrLineTra.Compressed_Iso2[0];
				memcpy(E_Transac8583.champs2,&StrLineTra.Compressed_Iso2[1],E_Transac8583.l_champs2);
			}
			else
			{
/*				ShowTrace_Mouna("ad5");
				imp_buffer(StrLineTra.Compressed_Iso2,StrLineTra.Compressed_Iso2[0]);*/
				bcd_asc(iso2,&StrLineTra.Compressed_Iso2[1],20);
/*				imp_buffer(iso2,20);*/
				for(j=0;((j<20)&&(iso2[j] !='d')&&(iso2[j] !='D')&&(iso2[j] !='='));j++);
				memcpy(E_Transac8583.champs2,iso2,j);
				E_Transac8583.l_champs2 =j;
			}
			//memcpy(E_Transac8583.champs3,"000000",6);
			memcpy(E_Transac8583.champs3,StrLineTra.Proc_Code,6);
			long_asc (montant, 12, &StrLineTra.Transaction_Amount);
			memcpy(E_Transac8583.champs4,montant,12);
			memcpy(E_Transac8583.champs11,StrLineTra.STAN,6);//modif	04/04
			memcpy(E_Transac8583.champs12,StrLineTra.Transaction_DateTime,12);
			memcpy(E_Transac8583.champs14,StrLineTra.Expired_Date,4);
			//champ17,		date business
			memcpy(E_Transac8583.champs17,&Date_YYMMDDHHmmss[2],4);
			Code_PointService(E_Transac8583.champs22,StrLineTra.Entry_Mode,
				StrLineTra.Entry_Pin,StrLineTra.Lg_pin);
			memcpy(E_Transac8583.champs24,"200",3);
			memcpy(E_Transac8583.champs26,FicPara.MCC,4);

			E_Transac8583.l_champs32 = 6;
			memcpy(E_Transac8583.champs32,"011010",6);

			memcpy(E_Transac8583.champs37,StrLineTra.RRN,6);
			memcpy(&E_Transac8583.champs37[6],FicPara.Num_seqAnnul,6) ;
			memcpy(E_Transac8583.champs38,StrLineTra.Approval_Code,6);
			memcpy(E_Transac8583.champs41,FicPara.Num_Terminal,8);
			memset(E_Transac8583.champs42,' ',15);
			memcpy(E_Transac8583.champs42,FicPara.Num_point_vente,strlen((PC)FicPara.Num_point_vente));
			//memcpy(E_Transac8583.champs42,StrLineTra.Num_point_vente,strlen(StrLineTra.Num_point_vente));

			E_Transac8583.l_champs43 = 24;
			memcpy(E_Transac8583.champs43,FicPara.Merchant_Adrees,24);

			memcpy(E_Transac8583.champs49,StrLineTra.NumCurrency,3);
			memcpy(E_Transac8583.champs50,StrLineTra.NumCurrency_cons,3);
			E_Transac8583.l_champs53 = 10;
			memcpy(E_Transac8583.champs53,"0199000000",10);
			if(StrLineTra.Entry_Pin==PIN_ONLINE)
				memcpy(E_Transac8583.champs53+2,"01",2);
			else
				memcpy(E_Transac8583.champs53+2,"99",2);
			//champ55 pour les trans EMV
			if(StrLineTra.Entry_Mode == TRS_EMV)
			{
				memcpy(E_Transac8583.champs23,StrLineTra.PanSeqNumber,3);
				E_Transac8583.l_champs55 = StrLineTra.Lg_EmvData;
				memcpy(E_Transac8583.champs55,StrLineTra.EmvData,StrLineTra.Lg_EmvData);
			}
			break ;

		case	1420: // Avis de redressement
			memcpy(E_Transac8583.bitmap,bitmap,8);
			E_Transac8583.l_champs2 = StructInfoPayment.Holder_Card[0];
			memcpy(E_Transac8583.champs2,&StructInfoPayment.Holder_Card[1],StructInfoPayment.Holder_Card[0]);
			memcpy(E_Transac8583.champs3,StructInfoPayment.Proc_Code,6);

			long_asc (montant, 12, &StructInfoPayment.Transaction_Amount);
			memcpy(E_Transac8583.champs4,montant,12);
			memset(montant,0,12) ;
			long_asc (montant, 12, &StructInfoPayment.Consolidation_Amount);
			memcpy(E_Transac8583.champs5,montant,12);


			if (StructInfoPayment.MontantPourboireInterchange != 0)    //POURBOIRE
			{
				Ulong          		AmountP;

				AmountP = StructInfoPayment.Transaction_Amount + StructInfoPayment.MontantPourboireInterchange;
				long_asc (montant, 12, &AmountP);
				memcpy(E_Transac8583.champs4,montant,12);

				memset(montant,0,12) ;
				AmountP = StructInfoPayment.Consolidation_Amount + StructInfoPayment.MontantPourboireInterchange;
				long_asc (montant, 12, &AmountP);
				memcpy(E_Transac8583.champs5,montant,12);
			}



			memcpy(E_Transac8583.champs11,StructInfoPayment.STAN,6);//modif	04/04
			memcpy(E_Transac8583.champs12,StructInfoPayment.Transaction_DateTime,12);
			memcpy(E_Transac8583.champs14,StructInfoPayment.Expired_Date,4);
			//champ17,		date business
			memcpy(E_Transac8583.champs17,&Date_YYMMDDHHmmss[2],4);
			memset(E_Transac8583.champs22,'\0',12);
				if (StructInfoPayment.Entry_Mode == TRS_MAG && (StructInfoPayment.SeviceCode[0]=='2' || StructInfoPayment.SeviceCode[0]=='6')){
					memcpy(E_Transac8583.champs22,"801",3);

				}else if(StructInfoPayment.Entry_Mode == TRS_MAG){
					memcpy(E_Transac8583.champs22,"901",3);

				}else if(StructInfoPayment.Entry_Mode == TRS_EMV){
					memcpy(E_Transac8583.champs22,"051",3);
//					Os__xprintaa("Annulation EMV");
					bitset(bitmap,55);
					bitset(E_Transac8583.bitmap,55);
					E_Transac8583.l_champs55 = StructInfoPayment.l_champ55;
					memcpy(E_Transac8583.champs55,StructInfoPayment.Champ55,StructInfoPayment.l_champ55);
					if (StructInfoPayment.PanSeqNum5F34[0] != 0)
						memcpy(E_Transac8583.champs23,StructInfoPayment.PanSeqNumber,3);
					else
						memcpy(E_Transac8583.champs23,"000",3);

				}else if(StructInfoPayment.Entry_Mode == TRS_MAN){
						memcpy(E_Transac8583.champs22,"601",3);
					}


			if(StructInfoPayment.CodeFunction==VOIDCODE)
				memcpy(E_Transac8583.champs23,StrLineTra.PanSeqNumber,3);
			else
				memcpy(E_Transac8583.champs23,StrPreAut.PanSeqNumber,3);

			if(StructInfoPayment.CodeFunction == VOID_PREAUTO_TOTAL_CODE)
				memcpy(E_Transac8583.champs24,"400",3);
			else
				if(StructInfoPayment.CodeFunction == VOID_PREAUTO_PARTIEL_CODE)
					memcpy(E_Transac8583.champs24,"401",3);
				else
					memcpy(E_Transac8583.champs24,"200",3);
				// champ25
				memcpy(E_Transac8583.champs25,"4000",4);


/*			if(StructInfoPayment.CodeFunction == VOID_PREAUTO_PARTIEL_CODE) //ailal add 130716 changement de valeur pour dï¿½bloquer test switch national
			{
				memcpy(E_Transac8583.champs24,"402",3);
				memcpy(E_Transac8583.champs25,"4004",4);
			}*/

				if(StructInfoPayment.CodeFunction == VOID_PREAUTO_PARTIEL_CODE)
				{
					memset(montant,0,12) ;
					Montant_remplacement = StrPreAut.Montant_Accomp;
					long_asc (montant, 12, &Montant_remplacement);
					memcpy(E_Transac8583.champs30,montant,12);
				}
				E_Transac8583.l_champs32 = 6;
				memcpy(E_Transac8583.champs32,"011010",6);

				//champ37
				memcpy(E_Transac8583.champs37,FicPara.Num_sequence,6);
				memcpy(&E_Transac8583.champs37[6],FicPara.Num_seqAnnul,6) ;
				memcpy(E_Transac8583.champs38,StructInfoPayment.Approval_Code,6);
				memcpy(E_Transac8583.champs41,FicPara.Num_Terminal,8);
				memset(E_Transac8583.champs42,' ',15);
				memcpy(E_Transac8583.champs42,FicPara.Num_point_vente,strlen((PC)FicPara.Num_point_vente));
				memcpy(E_Transac8583.champs49,StructInfoPayment.Devise_acc.code_num,3);
				memcpy(E_Transac8583.champs50,StructInfoPayment.NumCurrency_cons,3);
				//champ53 Information de controle de sécurité
				E_Transac8583.l_champs53 = 10;
				memcpy(E_Transac8583.champs53,"0199000000",10);
				if(StructInfoPayment.Entry_Pin==PIN_ONLINE)
					memcpy(E_Transac8583.champs53+2,"01",2);
				else
					memcpy(E_Transac8583.champs53+2,"99",2);
				//champ56
				if(StructInfoPayment.CodeFunction==VOIDCODE)
				{
					memcpy(E_Transac8583.champs56,StrLineTra.Message_Type,4);
					memcpy(&E_Transac8583.champs56[4],StrLineTra.STAN,6);
					memcpy(&E_Transac8583.champs56[10],StrLineTra.Transaction_DateTime,12);
					memcpy(&E_Transac8583.champs56[22],"11111111111",11);
					E_Transac8583.l_champs56 = 33;
				}
				else
				{
					memcpy(E_Transac8583.champs56,StrPreAut.Message_Type,4);
					memcpy(&E_Transac8583.champs56[4],StrPreAut.STAN,6);
					memcpy(&E_Transac8583.champs56[10],StrPreAut.Transaction_DateTime,12);
					memcpy(&E_Transac8583.champs56[22],"11111111111",11);
					E_Transac8583.l_champs56 = 33;
				}
				break ;

		case	1520: // Avis de consolidation
//			ShowTrace("F4");
			break;

		case	1804: // Demande de consolidation
//			ShowTrace("F5");
			break;

		default:
			break;
	}
	E_Transac8583.l_champs63 = 26;//CCD
	OffSet = 0;
	memcpy(&E_Transac8583.champs63[OffSet],FicPara.Index_MacKey,3);
	OffSet += 3;
	memcpy(&E_Transac8583.champs63[OffSet],FicPara.Index_MK,3);
	OffSet += 3;
	memcpy(&E_Transac8583.champs63[OffSet],FicPara.Version_Tab_Emv,3);
	OffSet += 3;
	memcpy(&E_Transac8583.champs63[OffSet],FicPara.Version_Bin_Prefixe,3);
	OffSet += 3;
	memcpy(&E_Transac8583.champs63[OffSet],lop_gest.NumListe,4);
	OffSet += 4;
	memcpy(&E_Transac8583.champs63[OffSet],FicPara.Version_Pos_Param,3);
	OffSet += 3;
	memcpy(&E_Transac8583.champs63[OffSet],FicPara.Version_appli,4);
	OffSet += 4;
	memcpy(&E_Transac8583.champs63[OffSet],FicPara.Version_Compte_Dom,3);
	OffSet += (Ushort)3;
	return OK;
}

/****************************************************************************/
void Code_PointService(uchar *Code_pt_service,uchar Entry_Mode,uchar Entry_Pin,uchar Lg_pin) //larbi
{
	uchar	lg_pin;
	lg_pin=Lg_pin;

	Code_pt_service[5]='1' ;    // carte presente
	Code_pt_service[11]='0' ;   // longueur de pin
	Code_pt_service[1]='2' ;
	Code_pt_service[7]='5' ;
	Code_pt_service[8]='4';
	Code_pt_service[0]='5' ;	//Kamal-Mouadden : '4' veut dire terminal supporte en plus le sans contact
	switch(Entry_Mode)
	{
   	case TRS_MAG :
		//Code_pt_service[0]='2' ;
		Code_pt_service[6]='2' ;

		if(StructInfoPayment.Fallback == 1)
			Code_pt_service[6]='8' ;

		break ;
	case TRS_MAN :
		//Code_pt_service[0]='6' ;
		Code_pt_service[6]='9' ;   // 6====> 9 larbi 24042019 //T_MAN
		break ;
	case TRS_EMV :
		//Code_pt_service[0]='9' ;
		Code_pt_service[6]='5' ;
		break ;
	}
	if(Entry_Pin==PIN_ONLINE)
	{
		switch(lg_pin)
		{
       	case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8: case 9:
			Code_pt_service[11]=lg_pin+48 ; // longueur de pin
            break;
		case 10:
			Code_pt_service[11]='A' ; // longueur de pin
            break;
		case 11:
			Code_pt_service[11]='B' ; // longueur de pin
            break;
		case 12:
			Code_pt_service[11]='C' ; // longueur de pin
            break;
		}
		Code_pt_service[1]='1';
		Code_pt_service[7]='1';
		Code_pt_service[8]='3';
	}
	/*else
  		Code_pt_service[11] = '4';*/

	if(Entry_Pin==PIN_OFFLINE)
	{
		Code_pt_service[1]='1';
		Code_pt_service[7]='1';
		Code_pt_service[8]='1';
	}
	Code_pt_service[1]='1';
	Code_pt_service[2]='0';
	Code_pt_service[3]='3';
	Code_pt_service[4]='0';
	Code_pt_service[5]='1';
	Code_pt_service[9]='1';
	Code_pt_service[10]='4';

	Os__xprint((char*)Code_pt_service);
}

//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			NOM			:Emission
//			DESCRIPTION	:gestion de l'émission du message
//
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			ENTREE		:rien
//
//			SORTIE		:résultat de l'émission
//
//			FCT APP		:
//
//			REMARQUE	:
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
uchar	Emission(uchar *typemessage,uchar *bitmap)
{
	uchar		status;
	uchar		Len_Mess[5];
	uchar		Mac_Key[9];
	uchar		MAC[8];
	Ulong		ul_len_mess;

	//	ShowTrace("E0");
	status = ProcRequest(bitmap);
	//	ShowTrace("E1");

	memset(Len_Mess,'\0',5);
	ul_len_mess = (Ulong)(count+8);
	long_asc(Len_Mess,4,&ul_len_mess);
	memcpy(&g_tompon1[3],Len_Mess,4);

	memset(Mac_Key,'\0',9);
	Os__saved_set ((uchar *)&FicPara.Mac_Key, '\x01', 8);//A enlever
	memcpy(Mac_Key,FicPara.Mac_Key,8);
	//MACC
	//	ShowTrace("E2");
	if(
		(memcmp(typemessage,"1304",4)==0)||
		(memcmp(typemessage,"1520",4)==0)
		)
		Calcul_Mac(g_tompon1+36, count-36, MAC, Mac_Key);
	else
		Calcul_Mac(g_tompon1+28, count-28, MAC, Mac_Key);

	memcpy(&g_tompon1[count],MAC,8);
	count += (Ushort)8;

	//eft30printhex(g_tompon1,count);
	status = Emettre((char*)g_tompon1,count);
	//	ShowTrace("E4");

	return status;
}

//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			NOM			: ProcRequest
//			DESCRIPTION	: construction du message à envoyer
//
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			ENTREE		: *bit : bitmap du message à construire
//
//			SORTIE		:
//
//			FCT APP		:
//
//			REMARQUE	:
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
uchar	ProcRequest(uchar *bitmp )
{
	uchar tmp[900], i, j;
	Ushort  log;


	memset(g_tompon1,'\0',MAX_MESS);

	init_tpdu();

	init_pack(E_Transac8583.e_type_mess, g_tompon1+16);

//	ShowTrace("66");
//	eft30printhex(g_tompon1,28);

	for (i=1; i<64; i++)
	{
		if(bittest(bitmp, i))
		{
			log = format_Emis(i,tmp);
			//eft30printhex(tmp,log);
			pack(i, tmp, log);

			if((i == 49)||(i == 53)||(i == 55));
				//eft30printhexKamal(g_tompon1,bptr-g_tompon1);

			eft30printhexGPRS((char*)g_tompon1,(Ushort)(bptr-g_tompon1));
		}
	}

	if(bittest(bitmp, 1))
	{
		for (i=65; i<128; i++)
		{
			j=i-64;
			if (bittest(E_Transac8583.bitmap2, j))//wd
			{
				log = format_Emis(i,tmp);
				pack(i, tmp, log);
			}
		}
	}
	count=bptr-g_tompon1;

	//eft30printhex(g_tompon1,300);
	//ShowTrace("G_TOMPON1");
	if(count<256)
		eft30printhex((char*)g_tompon1,count);
	else if(count< 512)
	{
		eft30printhex((char*)g_tompon1,256);
		eft30printhex((char*)g_tompon1+256,count-256);
	}
	else
	{
		eft30printhex((char*)g_tompon1,256);
		eft30printhex((char*)g_tompon1+256,256);
		eft30printhex((char*)g_tompon1+512,count-512);
	}
	return OK;
}

//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			NOM			:init_pack
//			DESCRIPTION	:initialisation du header du message à emettre
//
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			ENTREE		:
//
//			SORTIE		:
//
//			FCT APP		:
//
//			REMARQUE	:
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
void init_tpdu(void)
{
	memcpy(g_tompon1,"ISO",3);
	memcpy(&g_tompon1[3],"0000",4);
	g_tompon1[7] = '2';
//	memset(&g_tompon1[8],'0',5);
//	memcpy(&g_tompon1[8],FicPara.Version_appli,4);
	memcpy(&g_tompon1[8],"92005",5);//CCD
	memcpy(&g_tompon1[13],"000",3);

}

//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			NOM			:init_pack
//			DESCRIPTION	:initialisation du header du message à emettre
//
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			ENTREE		:
//
//			SORTIE		:
//
//			FCT APP		:
//
//			REMARQUE	:
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
void init_pack( uchar *msg_type, uchar *buf)
{
	memcpy(buf,msg_type,4);

	g_bitmap = buf+4;
	memset(g_bitmap, 0x00, 8);
	memcpy(g_bitmap, E_Transac8583.bitmap, 8);

	bptr = buf+12;
}

//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			NOM			: format_Emis
//			DESCRIPTION	: construction du message à envoyer
//
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			ENTREE		: a       : numéro de champs
//                        *result : recupère le contenu des champs
//
//			SORTIE		:
//
//			FCT APP		:
//
//			REMARQUE	:
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
Ushort format_Emis(Ushort a, uchar *result)
{
	switch(a)
	{
   	case 1:
		ShowTrac_GPRS("champs 1");
		memcpy(result, E_Transac8583.bitmap2, 8);
ShowTrac_GPRS((char *)E_Transac8583.bitmap2);
eft30printhexGPRS((char*)E_Transac8583.bitmap2,sizeof(E_Transac8583.bitmap2));
		return(8);

   	case 2:
		ShowTrac_GPRS("champs 2");
		memcpy(result, E_Transac8583.champs2, E_Transac8583.l_champs2);
ShowTrac_GPRS((char *)E_Transac8583.champs2);
eft30printhexGPRS((char*)E_Transac8583.champs2,sizeof(E_Transac8583.champs2));
		return(E_Transac8583.l_champs2);

	case 3:
		ShowTrac_GPRS("champs 3");
		memcpy(result, E_Transac8583.champs3, 6);
ShowTrac_GPRS((char *)E_Transac8583.champs3);
eft30printhexGPRS((char*)E_Transac8583.champs3,sizeof( E_Transac8583.champs3 ));
		return(6);

	case 4:
		ShowTrac_GPRS("champs 4");
		memcpy(result, E_Transac8583.champs4, 12);
ShowTrac_GPRS((char *)E_Transac8583.champs4);
eft30printhexGPRS((char*)E_Transac8583.champs4,sizeof( E_Transac8583.champs4 ));
		return(12);

	case 5:
		ShowTrac_GPRS("champs 5");
		memcpy(result, E_Transac8583.champs5, 12);
ShowTrac_GPRS((char *)E_Transac8583.champs5);
eft30printhexGPRS((char*)E_Transac8583.champs5,sizeof( E_Transac8583.champs5 ));
		return(12);

	case 7:
		ShowTrac_GPRS("champs 7");
		memcpy(result, E_Transac8583.champs7, 10);
ShowTrac_GPRS((char *) E_Transac8583.champs7);
eft30printhexGPRS((char*) E_Transac8583.champs7,sizeof(  E_Transac8583.champs7 ));
		return(10);

	case 11:
		ShowTrac_GPRS("champs 11");
		memcpy(result, E_Transac8583.champs11, 6);
ShowTrac_GPRS((char *)E_Transac8583.champs11);
eft30printhexGPRS((char*)E_Transac8583.champs11,sizeof( E_Transac8583.champs11 ));
		return(6);

	case 12:
		ShowTrac_GPRS("champs 12");
		memcpy(result, E_Transac8583.champs12, 12);
ShowTrac_GPRS((char *)E_Transac8583.champs12);
eft30printhexGPRS((char*)E_Transac8583.champs12,sizeof( E_Transac8583.champs12 ));
		return(12);

	case 14:
		ShowTrac_GPRS("champs 14");
		memcpy(result, E_Transac8583.champs14, 4);
ShowTrac_GPRS((char *)E_Transac8583.champs14);
eft30printhexGPRS((char*)E_Transac8583.champs14,sizeof( E_Transac8583.champs14 ));
		return(4);
		/*
		case 15:
		memcpy(result, E_Transac8583.champs15, 6);
		return(6);
		*/
	case 17:
		ShowTrac_GPRS("champs 17");
		memcpy(result, E_Transac8583.champs17, 4);
ShowTrac_GPRS((char *)E_Transac8583.champs17);
eft30printhexGPRS((char*)E_Transac8583.champs17,sizeof( E_Transac8583.champs17 ));
		return(4);

	case 22:
		ShowTrac_GPRS("champs 22");
		memcpy(result, E_Transac8583.champs22, 12);
ShowTrac_GPRS((char *)E_Transac8583.champs22);
eft30printhexGPRS((char*)E_Transac8583.champs22,sizeof( E_Transac8583.champs22 ));
		return(12);

	case 23:
		ShowTrac_GPRS("champs 23");
		memcpy(result, E_Transac8583.champs23, 3);
ShowTrac_GPRS((char *)E_Transac8583.champs23);
eft30printhexGPRS((char*)E_Transac8583.champs23,sizeof( E_Transac8583.champs23 ));
		return(3);

	case 24:
		ShowTrac_GPRS("champs 24");
		memcpy(result, E_Transac8583.champs24, 3);
ShowTrac_GPRS((char *)E_Transac8583.champs24);
eft30printhexGPRS((char*)E_Transac8583.champs24,sizeof( E_Transac8583.champs24 ));
		return(3);

	case 25:
		ShowTrac_GPRS("champs 25");
		memcpy(result, E_Transac8583.champs25, 4);
ShowTrac_GPRS((char *)E_Transac8583.champs25);
eft30printhexGPRS((char*)E_Transac8583.champs25,sizeof( E_Transac8583.champs25 ));
		return(4);

	case 26:
		ShowTrac_GPRS("champs 26");
		memcpy(result, E_Transac8583.champs26, 4);
ShowTrac_GPRS((char *)E_Transac8583.champs26);
eft30printhexGPRS((char*)E_Transac8583.champs26,sizeof( E_Transac8583.champs26 ));
		return(4);

	case 30:
		ShowTrac_GPRS("champs 30");
		memcpy(result, E_Transac8583.champs30, 12);
ShowTrac_GPRS((char *)E_Transac8583.champs30);
eft30printhexGPRS((char*)E_Transac8583.champs30,sizeof( E_Transac8583.champs30 ));
		return(12);

	case 32:
		ShowTrac_GPRS("champs 32");
		memcpy(result, E_Transac8583.champs32,E_Transac8583.l_champs32);
ShowTrac_GPRS((char *)E_Transac8583.champs32);
eft30printhexGPRS((char*)E_Transac8583.champs32,sizeof( E_Transac8583.champs32 ));
		return(E_Transac8583.l_champs32);


	case 35:
		ShowTrac_GPRS("champs 35");
		memcpy(result, E_Transac8583.champs35,  E_Transac8583.l_champs35);
ShowTrac_GPRS((char *)E_Transac8583.champs35);
eft30printhexGPRS((char*)E_Transac8583.champs35,sizeof( E_Transac8583.champs35 ));
		return(E_Transac8583.l_champs35);

	case 37:
		ShowTrac_GPRS("champs 37");
		memcpy(result, E_Transac8583.champs37,  12);
		//ShowTrace_Mouna("champs 37");
		//ShowTrace_Mouna(E_Transac8583.champs37);
ShowTrac_GPRS((char *)E_Transac8583.champs37);
eft30printhexGPRS((char*)E_Transac8583.champs37,sizeof( E_Transac8583.champs37 ));
		return(12);

	case 38:
		ShowTrac_GPRS("champs 38");
		memcpy(result, E_Transac8583.champs38,  6);
ShowTrac_GPRS((char *)E_Transac8583.champs38);
eft30printhexGPRS((char*)E_Transac8583.champs38,sizeof( E_Transac8583.champs38 ));
		return(6);

	case 39:
		ShowTrac_GPRS("champs 39");
		memcpy(result, E_Transac8583.champs39,  3);
ShowTrac_GPRS((char *)E_Transac8583.champs39);
eft30printhexGPRS((char*)E_Transac8583.champs39,sizeof( E_Transac8583.champs39 ));
		return(3);

	case 41:
		ShowTrac_GPRS("champs 41");
		memcpy(result, E_Transac8583.champs41,8);
ShowTrac_GPRS((char *)E_Transac8583.champs41);
eft30printhexGPRS((char*)E_Transac8583.champs41,sizeof( E_Transac8583.champs41 ));
		return(8);

	case 42:
		ShowTrac_GPRS("champs 42");
		memcpy(result, E_Transac8583.champs42,15);
ShowTrac_GPRS((char *)E_Transac8583.champs42);
eft30printhexGPRS((char*)E_Transac8583.champs42,sizeof( E_Transac8583.champs42 ));
		return(15);

	case 43:
		ShowTrac_GPRS("champs 43");
		memcpy(result, E_Transac8583.champs43,  E_Transac8583.l_champs43);
ShowTrac_GPRS((char *)E_Transac8583.champs43);
eft30printhexGPRS((char*)E_Transac8583.champs43,sizeof( E_Transac8583.champs43 ));
		return(E_Transac8583.l_champs43);

		/*
		case 44:
		memcpy(result, E_Transac8583.champs44,  E_Transac8583.l_champs44);
		return(E_Transac8583.l_champs44);
		*/

	case 48:
		ShowTrac_GPRS("champs 48");
		memcpy(result, E_Transac8583.champs48,  E_Transac8583.l_champs48);
		//eft30printhexKamal(E_Transac8583.champs48,E_Transac8583.l_champs48);
ShowTrac_GPRS((char *)E_Transac8583.champs48);
eft30printhexGPRS((char*)E_Transac8583.champs48,sizeof( E_Transac8583.champs48 ));
		return(E_Transac8583.l_champs48);

	case 49:
		ShowTrac_GPRS("champs 49");
		memcpy(result, E_Transac8583.champs49,3);
ShowTrac_GPRS((char *)E_Transac8583.champs49);
eft30printhexGPRS((char*)E_Transac8583.champs49,sizeof( E_Transac8583.champs49 ));
		return(3);

	case 50:
		ShowTrac_GPRS("champs 50");
		memcpy(result, E_Transac8583.champs50,3);
ShowTrac_GPRS((char *)E_Transac8583.champs50);
eft30printhexGPRS((char*)E_Transac8583.champs50,sizeof( E_Transac8583.champs50 ));
		return(3);

	case 52:
		ShowTrac_GPRS("champs 52");
		memcpy(result, E_Transac8583.champs52,8);
ShowTrac_GPRS((char *)E_Transac8583.champs52);
eft30printhexGPRS((char*)E_Transac8583.champs52,sizeof( E_Transac8583.champs52 ));
		return(8);

	case 53:
		ShowTrac_GPRS("champs 53");
		memcpy(result, E_Transac8583.champs53,E_Transac8583.l_champs53);
ShowTrac_GPRS((char *)E_Transac8583.champs53);
eft30printhexGPRS((char*)E_Transac8583.champs53,sizeof( E_Transac8583.champs53 ));
		return(E_Transac8583.l_champs53);

	case 54:
		ShowTrac_GPRS("champs 54");
		memcpy(result, E_Transac8583.champs54,E_Transac8583.l_champs54);
ShowTrac_GPRS((char *)E_Transac8583.champs54);
eft30printhexGPRS((char*)E_Transac8583.champs54,sizeof( E_Transac8583.champs54 ));
		return(E_Transac8583.l_champs54);

	case 55:
		ShowTrac_GPRS("champs 55e");
		//eft30printhexKamal(E_Transac8583.champs55,E_Transac8583.l_champs55);
		memcpy(result, E_Transac8583.champs55,E_Transac8583.l_champs55);
ShowTrac_GPRS((char *)E_Transac8583.champs55);
eft30printhexGPRS((char*)E_Transac8583.champs55,sizeof( E_Transac8583.champs55 ));
		return(E_Transac8583.l_champs55);

	case 56:
		ShowTrac_GPRS("champs 56");
		memcpy(result, E_Transac8583.champs56,E_Transac8583.l_champs56);
ShowTrac_GPRS((char *)E_Transac8583.champs56);
eft30printhexGPRS((char*)E_Transac8583.champs56,sizeof( E_Transac8583.champs56 ));
		return(E_Transac8583.l_champs56);

	case 62:
		ShowTrac_GPRS("champs 62");
		memcpy(result, E_Transac8583.champs62,E_Transac8583.l_champs62);
ShowTrac_GPRS((char *)E_Transac8583.champs62);
eft30printhexGPRS((char*)E_Transac8583.champs62,sizeof( E_Transac8583.champs62 ));
		return(E_Transac8583.l_champs62);

	case 63:
		ShowTrac_GPRS("champs 63");
		memcpy(result, E_Transac8583.champs63,E_Transac8583.l_champs63);
ShowTrac_GPRS((char *)E_Transac8583.champs63);
eft30printhexGPRS((char*)E_Transac8583.champs63,sizeof( E_Transac8583.champs63 ));
		return(E_Transac8583.l_champs63);

	case 64:
		ShowTrac_GPRS("champs 64");
		memcpy(result, E_Transac8583.champs64,8);
ShowTrac_GPRS((char *)E_Transac8583.champs64);
eft30printhexGPRS((char*)E_Transac8583.champs64,sizeof( E_Transac8583.champs64 ));
		return(8);

	case 71:
		ShowTrac_GPRS("champs 71");
		memcpy(result, E_Transac8583.champs71,8);
ShowTrac_GPRS((char *)E_Transac8583.champs71);
eft30printhexGPRS((char*)E_Transac8583.champs71,sizeof( E_Transac8583.champs71 ));
		return(8);

	case 72:
		ShowTrac_GPRS("champs 72");
		memcpy(result, E_Transac8583.champs72,E_Transac8583.l_champs72);
ShowTrac_GPRS((char *)E_Transac8583.champs72);
eft30printhexGPRS((char*)E_Transac8583.champs72,sizeof( E_Transac8583.champs72 ));
		return(E_Transac8583.l_champs72);

		/**********************************/
	case 73:
		ShowTrac_GPRS("champs 73");
		memcpy( result,E_Transac8583.champs73,6);
ShowTrac_GPRS((char *)E_Transac8583.champs73);
eft30printhexGPRS((char*)E_Transac8583.champs73,sizeof( E_Transac8583.champs73 ));
		return(6);

	case 74:
		ShowTrac_GPRS("champs 74");
		memcpy(result, E_Transac8583.champs74,10);
ShowTrac_GPRS((char *)E_Transac8583.champs74);
eft30printhexGPRS((char*)E_Transac8583.champs74,sizeof( E_Transac8583.champs74 ));
		return(10);

	case 75:
		ShowTrac_GPRS("champs 75");
		memcpy(result, E_Transac8583.champs75,10);
ShowTrac_GPRS((char *)E_Transac8583.champs75);
eft30printhexGPRS((char*)E_Transac8583.champs75,sizeof( E_Transac8583.champs75 ));
		return(10);

	case 76:
		ShowTrac_GPRS("champs 76");
		memcpy(result, E_Transac8583.champs76,10);
ShowTrac_GPRS((char *)E_Transac8583.champs76);
eft30printhexGPRS((char*)E_Transac8583.champs76,sizeof( E_Transac8583.champs76 ));
		return(10);

	case 77:
		ShowTrac_GPRS("champs 77");
		memcpy(result, E_Transac8583.champs77,10);
ShowTrac_GPRS((char *)E_Transac8583.champs77);
eft30printhexGPRS((char*)E_Transac8583.champs77,sizeof( E_Transac8583.champs77 ));
		return(10);

	case 83:
		ShowTrac_GPRS("champs 83");
		memcpy(result, E_Transac8583.champs83,10);
ShowTrac_GPRS((char *)E_Transac8583.champs83);
eft30printhexGPRS((char*)E_Transac8583.champs83,sizeof( E_Transac8583.champs83 ));
		return(10);

	case 84:
		ShowTrac_GPRS("champs 84");
		memcpy(result, E_Transac8583.champs84,10);
ShowTrac_GPRS((char *)E_Transac8583.champs84);
eft30printhexGPRS((char*)E_Transac8583.champs84,sizeof( E_Transac8583.champs84 ));
		return(10);

	case 86:
		ShowTrac_GPRS("champs 86");
		memcpy(result, E_Transac8583.champs86,16);
ShowTrac_GPRS((char *)E_Transac8583.champs86);
eft30printhexGPRS((char*)E_Transac8583.champs86,sizeof( E_Transac8583.champs86 ));
		return(16);

	case 87:
		ShowTrac_GPRS("champs 87");
		memcpy(result, E_Transac8583.champs87,16);
ShowTrac_GPRS((char *)E_Transac8583.champs87);
eft30printhexGPRS((char*)E_Transac8583.champs87,sizeof( E_Transac8583.champs87 ));
		return(16);

	case 88:
		ShowTrac_GPRS("champs 88");
		memcpy(result, E_Transac8583.champs88,16);
ShowTrac_GPRS((char *)E_Transac8583.champs88);
eft30printhexGPRS((char*)E_Transac8583.champs88,sizeof( E_Transac8583.champs88 ));
		return(16);

	case 89:
		ShowTrac_GPRS("champs 89");
		memcpy(result, E_Transac8583.champs89,16);
ShowTrac_GPRS((char *)E_Transac8583.champs89);
eft30printhexGPRS((char*)E_Transac8583.champs89,sizeof( E_Transac8583.champs89 ));
		return(16);

	case 97:
		ShowTrac_GPRS("champs 97");
		memcpy(result, E_Transac8583.champs97,17);
ShowTrac_GPRS((char *)E_Transac8583.champs97);
eft30printhexGPRS((char*)E_Transac8583.champs97,sizeof( E_Transac8583.champs97 ));
		return(17);

	case 101:
		ShowTrac_GPRS("champs 101");
		memcpy(result, E_Transac8583.champs101, E_Transac8583.l_champs101);
ShowTrac_GPRS((char *)E_Transac8583.champs101);
eft30printhexGPRS((char*)E_Transac8583.champs101,sizeof( E_Transac8583.champs101 ));
		return(E_Transac8583.l_champs101);

	case 110:
		ShowTrac_GPRS("champs 110");
		memcpy(result, E_Transac8583.champs110,E_Transac8583.l_champs110);
ShowTrac_GPRS((char *)E_Transac8583.champs110);
eft30printhexGPRS((char*)E_Transac8583.champs110,sizeof( E_Transac8583.champs110 ));
		return(E_Transac8583.l_champs110);

	case 128:
		ShowTrac_GPRS("champs 128");
		memcpy(result, E_Transac8583.champs128,8);
ShowTrac_GPRS((char *)E_Transac8583.champs128);
eft30printhexGPRS((char*)E_Transac8583.champs128,sizeof( E_Transac8583.champs128 ));
		return(8);

	default:
		return(0);
	}
}

//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			NOM			:*pack
//			DESCRIPTION	:formatage des champs à emettre
//
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			ENTREE		:
//
//			SORTIE		:
//
//			FCT APP		:
//
//			REMARQUE	:
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
uchar *pack(uchar bitn, uchar *buf, Ushort len)
{
	uchar  i;
	uchar  buffer[10];
	Ushort x;

	for(i=0; bittab[i].bitn != bitn && bittab[i].bitn != 255; i++);
	if( bittab[i].bitn == 255 )
		return(NULL);
	//bitset(bitmap, bitn);
	bitset(g_bitmap, bitn);
	switch(bittab[i].attrib)
	{
  		case NUMERIC_ :
			if( bittab[i].format == LLLVAR )
			{
				x = (Ushort)sprintf((PC)(char *)buffer, "%d",(int)len);
				if(x > 3)
					bintobcd(len, bptr, x);
				else
					bintobcd(len, bptr, 3);
				bptr += 2;
			}
			else
				if( bittab[i].format == LLVAR )
				{
					x = (Ushort)sprintf((PC)buffer, "%d",(int) len);
					bintobcd(len, bptr, x);
					++bptr;
				}
				memcpy( bptr, buf, len);
				bptr += len;
				break;

		case TRACKX :
			bintobcd(len, bptr, 2);
			++bptr;
			memcpy( bptr, buf, len);
			bptr += len;
			break;

		case ALPHANUM :
		case ALPHANUMS :
			if (bittab[i].format == LLLVAR)
			{
				x = sprintf((PC)buffer, "%d",(unsigned int) len);
				if(x > 3)
					bintobcd(len, bptr, x);
				else
					bintobcd(len, bptr, 3);
				bptr += 2;
			}
			else
				if (bittab[i].format == LLVAR)
				{
					x = sprintf((PC)buffer, "%d",(unsigned int) len);
					bintobcd(len, bptr, x);
					++bptr;
				}
				memcpy(bptr, buf, len);
				bptr += len;
				break;

		case BITS :
			memcpy(bptr, buf, 8);
			bptr += 8;
			break;
	}
	return(bptr);
}

//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			NOM			:Reception
//			DESCRIPTION	:gestion de la réception
//
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			ENTREE		:rien
//
//			SORTIE		:résultata de la réception
//
//			FCT APP		:
//
//			REMARQUE	:
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
uchar	Reception(void)
{
	long status	=	0;
	Ushort   TimeOut;

	TimeOut = 3000;
	/*status = Receive(delayC);*/
	//ShowTrace("R0");
	status = receive(g_tompon1,g_trace);
	//Os__xprintaa("       0012");
	if(status != OK)
	{
		ShowTrace("R1");
		return RECEPT_KO;
	}
	status = WaitRxMsg(E_Transac8583.r_type_mess);

	if (status != OK)
	{
		ShowTrace("R3");
		return FORMAT_MSG_REC_KO;
	}
	return OK;
}
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			NOM			:WaitRxMsg
//			DESCRIPTION	:
//
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			ENTREE		:
//
//			SORTIE		:
//
//			FCT APP		:
//
//			REMARQUE	:
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
long	WaitRxMsg(uchar *Buff)
{
	uchar	Mac_calc[9],Mac_recu[9];
	uchar	Mac_Key[9];
	L		l_length=0;
	C		c_length[5];

	memset((uchar *)&R_Transac8583,0,sizeof(R_Transac8583));

	//ShowTrace("W0");
	if ( check_header(Buff) != HEADER_OK)
		return HEADER_KO;

	//ShowTrace("W1");
	ProcRecept();
	//ShowTrace("W2");

	if(Check_Message()!= OK)
		return CHAMP_ERROR;

	//ShowTrace("W3");
	memcpy(c_length,g_tompon1+3,4);c_length[4]=0;l_length=atol(c_length);
	// Controle MAC à ajouter
	memset(Mac_calc,'\0',9);
	memset(Mac_recu,'\0',9);
	memcpy(Mac_recu,&g_tompon1[l_length-8],8);
	memset(&g_tompon1[l_length-8],0,8);

	memset(Mac_Key,'\0',9);
	memcpy(Mac_Key,FicPara.Mac_Key,8);

	if(memcmp(E_Transac8583.r_type_mess,"1314",4)==0)
		Calcul_Mac(g_tompon1+36, l_length-36-8, Mac_calc, Mac_Key);
	else
		Calcul_Mac(g_tompon1+28, l_length-28-8, Mac_calc, Mac_Key);


	//ShowTrace("W4");
	if( memcmp(Mac_recu,Mac_calc,8) && memcmp(FicPara.Index_MacKey,"000",3) )
	{
		////APEMV_UI_MessageDisplayDelay("MAC Control",1);
		//return HEADER_KO;
	}

	//ShowTrace("W5");
	return OK;
}


//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			NOM			:check_header
//			DESCRIPTION	:contrôle du header du message reçu
//
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			ENTREE		:*typemess : type de message attendu
//
//			SORTIE		:
//
//			FCT APP		:
//
//			REMARQUE	:
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
uchar check_header(uchar *typemess)
{
	uchar 	Id_protocole[4];
	uchar	Code_produit;
	uchar	Num_version[5];
	uchar	Num_chp_error[4];
	uchar	Type_mess[5];

	memset(Id_protocole,'\0',4);
	memset(Num_version,'\0',5);
	memset(Num_chp_error,'\0',4);
	memset(Type_mess,'\0',5);

	//eft30printhexKamal(g_tompon1,256);
	ShowTrace((char*)g_tompon1);
	memcpy(Id_protocole,g_tompon1,3);
	if(memcmp(Id_protocole,"ISO",3))
		return HEADER_KO;

	Code_produit = g_tompon1[7];
	if(Code_produit!='2')
	{
		Os__xprint((PC)"Header message KO");
		return HEADER_KO;
	}

	memcpy(Num_version,&g_tompon1[8],5);
	memcpy(Num_chp_error,&g_tompon1[13],3);
	memcpy(Type_mess,&g_tompon1[16],4);
	if(memcmp(Type_mess,E_Transac8583.r_type_mess,4))
	{
		Os__xprint((PC)"Pb. Type message");
		return HEADER_KO;
	}

	//ShowTrace("CH0");
	return HEADER_OK;
}

//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			NOM			:ProcRecept
//			DESCRIPTION	: decompose le message reçu et remplie la structure des champs ISO
//
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			ENTREE		:
//
//			SORTIE		:
//
//			FCT APP		:
//
//			REMARQUE	:
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
uchar	ProcRecept()
{
	uchar i,j;

	g_bitmap = g_tompon1+20;
	/* Id Protocole sur 3 position = ISO */
	/* Entete Message sur 13 position    */
	/* Type message sur 4 position       */
	bptr   = g_tompon1+28;

	//ShowTrace("PR0");
	for (i=1; i<65; i++)
	{
		if (bittest(g_bitmap, i))
		{
			format_Recept(i);
			unpack(i, bptr1);
		}
	}
//	ShowTrace("PR1");
	if(bittest(g_bitmap, 1))
	{
		//ShowTrace("PR2");
		for (i=65; i<129; i++)
		{
			j=i-64;
			if (bittest(R_Transac8583.bitmap2, j))//wd
			{
				format_Recept(i);
				unpack(i, bptr1);
			}
		}
	}
	count_r = bptr - g_tompon1;

//	ShowTrace("PR5");
	return OK;
}

//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			NOM			:format_Recept
//			DESCRIPTION	:remplie la structure des champs ISO
//
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			ENTREE		:
//
//			SORTIE		:
//
//			FCT APP		:
//
//			REMARQUE	:
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
void format_Recept(Ushort a)
{
	switch(a)
	{
   	case 1:
		//ShowTrace("champs 1");
		bptr1= R_Transac8583.bitmap2;
		//imp_buffer(R_Transac8583.bitmap2,8);
		break;

   	case 2:
		//ShowTrace("champs 2");
		l_bptr1 =(Ushort *)&R_Transac8583.l_champs2;
		bptr1= R_Transac8583.champs2;
		//imp_buffer(R_Transac8583.champs2,R_Transac8583.l_champs2);
		break;

	case 3:
		//ShowTrace("champs 3");
		bptr1= R_Transac8583.champs3;
		//imp_buffer(R_Transac8583.champs3,6);
		break;

	case 4:
		//ShowTrace("champs 4");
		bptr1= R_Transac8583.champs4;
		//imp_buffer(R_Transac8583.champs4,12);
		break;

	case 5:
		//ShowTrace("champs 5");
		bptr1= R_Transac8583.champs5;
		//imp_buffer(R_Transac8583.champs5,12);
		break;

	case 7:
		//ShowTrace("champs 7");
		bptr1= R_Transac8583.champs7;
		//imp_buffer(R_Transac8583.champs7,10);
		break;

	case 11:
		//ShowTrace("champs 11");
		bptr1= R_Transac8583.champs11;
		//imp_buffer(R_Transac8583.champs11,6);
		break;

	case 12:
		//ShowTrace("champs 12");
		bptr1= R_Transac8583.champs12;
		//imp_buffer(R_Transac8583.champs12,12);
		break;

	case 15:
		//ShowTrace("champs 15");
		bptr1= R_Transac8583.champs15;
		//imp_buffer(R_Transac8583.champs15,6);
		break;

	case 17:
		//ShowTrace("champs 17");
		bptr1= R_Transac8583.champs17;
		//imp_buffer(R_Transac8583.champs17,4);
		break;

	case 24:
		//ShowTrace("champs 24");
		bptr1 = R_Transac8583.champs24;
		//imp_buffer(R_Transac8583.champs24,3);
		break;

	case 30:
		//ShowTrace("champs 30");
		bptr1= R_Transac8583.champs30;
		//imp_buffer(R_Transac8583.champs30,12);
		break;

	case 32:
		//ShowTrace("champs 32");
		l_bptr1 = &R_Transac8583.l_champs32;
		bptr1= R_Transac8583.champs32;
		//imp_buffer(R_Transac8583.champs32,R_Transac8583.l_champs32);
		break;

	case 35:
		//ShowTrace("champs 35");
		l_bptr1 = &R_Transac8583.l_champs35;
		bptr1= R_Transac8583.champs35;
		//imp_buffer(R_Transac8583.champs35,R_Transac8583.l_champs35);
		break;

	case 37:
		//ShowTrace("champs 37");
		bptr1= R_Transac8583.champs37;
		//imp_buffer(R_Transac8583.champs37,12);
		break;

	case 38:
		//ShowTrace("champs 38");
		bptr1= R_Transac8583.champs38;
		//imp_buffer(R_Transac8583.champs38,6);
		break;

	case 39:
		//ShowTrace("champs 39");
		bptr1= R_Transac8583.champs39;
		//ShowTrace("Dchamp39");
		//ShowTrace(R_Transac8583.champs39);
		//ShowTrace("Fchamp39");
		break;

	case 41:
		//ShowTrace("champs 41");
		bptr1= R_Transac8583.champs41;
		//imp_buffer(R_Transac8583.champs41,15);
		break;

	case 42:
		//ShowTrace("champs 42");
		bptr1= R_Transac8583.champs42;
		//imp_buffer((char*)R_Transac8583.champs42,15);
		break;

	case 44:
		//ShowTrace("champs 44");
		l_bptr1 = (Ushort *)&R_Transac8583.l_champs44;
		bptr1= R_Transac8583.champs44;
		//imp_buffer(R_Transac8583.champs44,R_Transac8583.l_champs44);
		break;

	case 48:
		//ShowTrace("champs 48");
		l_bptr1 = &R_Transac8583.l_champs48;
		bptr1= R_Transac8583.champs48;
		//imp_buffer(R_Transac8583.champs48,R_Transac8583.l_champs48);
		break;

	case 49:
		//ShowTrace("champs 49");
		bptr1= R_Transac8583.champs49;
		//imp_buffer(R_Transac8583.champs49,3);
		break;

	case 50:
		//ShowTrace("champs 50");
		bptr1= R_Transac8583.champs50;
		//imp_buffer(R_Transac8583.champs50,3);
		break;

	case 53:
		//ShowTrace("champs 53");
		l_bptr1 = &R_Transac8583.l_champs53;
		bptr1= R_Transac8583.champs53;
		//imp_buffer(R_Transac8583.champs53,R_Transac8583.l_champs53);
		break;

	case 54:
		//ShowTrace("champs 54");
		l_bptr1 =  &R_Transac8583.l_champs54;
		bptr1= R_Transac8583.champs54;
		//imp_buffer(R_Transac8583.champs54,20);
		break;

	case 55:
		//ShowTrace("champs 55r");
		l_bptr1 = (Ushort *)&R_Transac8583.l_champs55;
		bptr1= R_Transac8583.champs55;
		//imp_buffer(R_Transac8583.champs55,R_Transac8583.l_champs55);
		//eft30printhexKamal(R_Transac8583.champs55,R_Transac8583.l_champs55);
		break;

	case 56:
		//ShowTrace("champs 56");
		l_bptr1 = &R_Transac8583.l_champs56;
		bptr1= R_Transac8583.champs56;
		//imp_buffer(R_Transac8583.champs56,R_Transac8583.l_champs56);
		break;

	case 63:
		//ShowTrace("champs 63");
		l_bptr1 = &R_Transac8583.l_champs63;
		bptr1= R_Transac8583.champs63;
		//imp_buffer(R_Transac8583.champs63,R_Transac8583.l_champs63);
		break;

	case 64:
		//ShowTrace("champs 64");
		bptr1= R_Transac8583.champs64;
		//imp_buffer(R_Transac8583.champs64,8);
		break;

	case 71:
		//ShowTrace("champs 71");
		bptr1= R_Transac8583.champs71;
		//imp_buffer(R_Transac8583.champs71,8);
		break;

	case 72:
		//ShowTrace("champs 72");
		l_bptr1 = (Ushort *)&R_Transac8583.l_champs72;
		bptr1= R_Transac8583.champs72;
		//imp_buffer(R_Transac8583.champs72,R_Transac8583.l_champs72);
		break;

	case 73:
		//ShowTrace("champs 73");
		bptr1= R_Transac8583.champs73;
		//imp_buffer(R_Transac8583.champs73,6);
		break;

	case 101:
		//ShowTrace("champs 101");
		l_bptr1 = (Ushort *)&R_Transac8583.l_champs101;
		//ShowTrace("champs ---");
		bptr1= R_Transac8583.champs101;
		//ShowTrace("champs +++");
		//imp_buffer(R_Transac8583.champs101,R_Transac8583.l_champs101);
		//ShowTrace("champs ***");
		break;

	case 110:
		//ShowTrace("champs 110");
		l_bptr1 = (Ushort *)&R_Transac8583.l_champs110;
		bptr1= R_Transac8583.champs110;
		//imp_buffer(R_Transac8583.champs110,R_Transac8583.l_champs110);
		break;

	case 128:
		//ShowTrace("champs 128");
		bptr1= R_Transac8583.champs128;
		//imp_buffer(R_Transac8583.champs128,8);
		break;

	default:
		break;
	}
}

//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			NOM			:*unpack
//			DESCRIPTION	:formatage des champs reçu
//
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			ENTREE		:
//
//			SORTIE		:
//
//			FCT APP		:
//
//			REMARQUE	:
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
uchar *unpack(Ushort a, uchar *fbuf)
{
	uchar  j, longueur[5];
	Ushort len;

	//ShowTrace("UN0");
	memset(longueur,0x00,5);
	for (j=0; bittab[j].bitn != 255; j++)
		if (bittab[j].bitn == a)
			break;

		if (bittab[j].bitn == 255)
			return(NULL);

		switch(bittab[j].attrib)
		{
		case NUMERIC_ :
			if (bittab[j].format == LLLVAR)
			{
				bxdtge_dcbtos(bptr,longueur, 2);
				longueur[4]	=	0x00;
				len = (Ushort)atoi((PC)longueur);
				bptr += 2;
				*l_bptr1=len;
			}
			else
				if (bittab[j].format == LLVAR)
				{
					bxdtge_dcbtos(bptr,longueur, 1);
					longueur[2]	=	0x00;
					len = (Ushort)atoi((PC)longueur);
					++bptr;
					*l_bptr1=len;
				}
				else
					len = bittab[j].max;
				memcpy( fbuf, bptr, len);
				bptr += len;
				break;

		case TRACKX :
			bxdtge_dcbtos( bptr, longueur, 1);
			longueur[2]	=	0x00;
			len = (Ushort)atoi((PC)longueur);
			*l_bptr1=len;
			++bptr;
			memcpy( fbuf, bptr, len);
			bptr += len;
			break;

		case ALPHANUM :
		case ALPHANUMS :
			if (bittab[j].format == LLLVAR)
			{
				bxdtge_dcbtos( bptr, longueur, 2);
				longueur[4]	=	0x00;
				len = (Ushort)atoi((PC)longueur);
				bptr += 2;
				*l_bptr1=len;
			}
			else
				if (bittab[j].format == LLVAR)
				{
					bxdtge_dcbtos( bptr, longueur, 1);
					longueur[4]	=	0x00;
					len = (Ushort)atoi((PC)longueur);
					++bptr;
					*l_bptr1=len;
				}
				else
					len = bittab[j].max;
				memcpy(fbuf, bptr, len);
				bptr += len;
				break;

		case BITS :
			memcpy(fbuf, bptr, 8);
			bptr += 8;
			break;
		}
	//ShowTrace("UN1");
		return(bptr);
}

//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			NOM			:Check_Message
//			DESCRIPTION	:contrôle les champs obligatoire et copied
//
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//
//			ENTREE		:
//
//			SORTIE		:
//
//			FCT APP		:
//
//			REMARQUE	:
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
//* _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _*/
uchar Check_Message(void)
{
	uchar		Bitmap_chp_obli[16];
	uchar		Bitmap_chp_copied[16];
	Ushort		Type_Message;
	uchar		tmp[5];
	uchar		i,j,Chaine[24];
	Ulong		ul_champ;

	memset(Bitmap_chp_obli,0,16);
	memset(Bitmap_chp_copied,0,16);

	memset(tmp,0,5);
	memcpy(tmp,E_Transac8583.r_type_mess,4);
	Type_Message = (Ushort)atol((PC)tmp);

	switch(Type_Message)
	{
	case 1314:
		bitset(Bitmap_chp_obli,1);
		bitset(Bitmap_chp_obli,24);
		bitset(Bitmap_chp_obli,39);
		bitset(Bitmap_chp_obli,44);
		bitset(Bitmap_chp_obli,53);
		bitset(Bitmap_chp_obli,63);
		bitset(Bitmap_chp_obli+8,73-64);
		//bitset(Bitmap_chp_obli+8,128-64);
		/********************/
		bitset(Bitmap_chp_obli,7);
		bitset(Bitmap_chp_obli,11);
		bitset(Bitmap_chp_obli,12);
		bitset(Bitmap_chp_obli,17);
		bitset(Bitmap_chp_obli,37);
		bitset(Bitmap_chp_obli,41);
		bitset(Bitmap_chp_obli,42);
		bitset(Bitmap_chp_obli+8,71-64);
		bitset(Bitmap_chp_obli+8,101-64);
		/********************/
		bitset(Bitmap_chp_copied,7);
		bitset(Bitmap_chp_copied,11);
		bitset(Bitmap_chp_copied,12);
		bitset(Bitmap_chp_copied,17);
		bitset(Bitmap_chp_copied,37);
		bitset(Bitmap_chp_copied,41);
		bitset(Bitmap_chp_copied,42);
		//bitset(Bitmap_chp_copied+8,71-64);
		bitset(Bitmap_chp_copied+8,101-64);
		break;

	case 1110:
		bitset(Bitmap_chp_obli,5);
		bitset(Bitmap_chp_obli,15);
		bitset(Bitmap_chp_obli,39);
		bitset(Bitmap_chp_obli,44);
		bitset(Bitmap_chp_obli,50);
		bitset(Bitmap_chp_obli,53);
		//bitset(Bitmap_chp_obli,64);
		/*****************************/
		bitset(Bitmap_chp_obli,2);
		bitset(Bitmap_chp_obli,3);
		bitset(Bitmap_chp_obli,4);
		bitset(Bitmap_chp_obli,7);
		bitset(Bitmap_chp_obli,11);
		bitset(Bitmap_chp_obli,12);
		bitset(Bitmap_chp_obli,17);
		bitset(Bitmap_chp_obli,32);
		bitset(Bitmap_chp_obli,37);
		bitset(Bitmap_chp_obli,41);
		bitset(Bitmap_chp_obli,42);
		bitset(Bitmap_chp_obli,49);
		/*****************************/
		bitset(Bitmap_chp_copied,2);
		bitset(Bitmap_chp_copied,3);
		bitset(Bitmap_chp_copied,4);
		bitset(Bitmap_chp_copied,7);
		bitset(Bitmap_chp_copied,11);
		bitset(Bitmap_chp_copied,12);
		bitset(Bitmap_chp_copied,17);
		bitset(Bitmap_chp_copied,32);
		bitset(Bitmap_chp_copied,37);
		bitset(Bitmap_chp_copied,41);
		bitset(Bitmap_chp_copied,42);
		bitset(Bitmap_chp_copied,49);
		break;

	case 1210:
		bitset(Bitmap_chp_obli,5);
		bitset(Bitmap_chp_obli,15);
		bitset(Bitmap_chp_obli,39);
		bitset(Bitmap_chp_obli,44);
		bitset(Bitmap_chp_obli,50);
		bitset(Bitmap_chp_obli,53);
		//bitset(Bitmap_chp_obli,64);
		/*****************************/
		bitset(Bitmap_chp_obli,2);
		bitset(Bitmap_chp_obli,3);
		bitset(Bitmap_chp_obli,4);
		bitset(Bitmap_chp_obli,7);
		bitset(Bitmap_chp_obli,11);
		bitset(Bitmap_chp_obli,12);
		bitset(Bitmap_chp_obli,17);
		bitset(Bitmap_chp_obli,32);
		bitset(Bitmap_chp_obli,37);
		bitset(Bitmap_chp_obli,41);
		bitset(Bitmap_chp_obli,42);
		bitset(Bitmap_chp_obli,49);
		/*****************************/
		bitset(Bitmap_chp_copied,2);
		bitset(Bitmap_chp_copied,3);
		bitset(Bitmap_chp_copied,4);
		bitset(Bitmap_chp_copied,7);
		bitset(Bitmap_chp_copied,11);
		bitset(Bitmap_chp_copied,12);
		bitset(Bitmap_chp_copied,17);
		bitset(Bitmap_chp_copied,32);
		bitset(Bitmap_chp_copied,37);
		bitset(Bitmap_chp_copied,41);
		bitset(Bitmap_chp_copied,42);
		bitset(Bitmap_chp_copied,49);
		break;

	case 1230:
		bitset(Bitmap_chp_obli,5);
		bitset(Bitmap_chp_obli,15);
		bitset(Bitmap_chp_obli,39);
		bitset(Bitmap_chp_obli,44);
		bitset(Bitmap_chp_obli,50);
		bitset(Bitmap_chp_obli,53);
		//bitset(Bitmap_chp_obli,64);
		/*****************************/
		bitset(Bitmap_chp_obli,2);
		bitset(Bitmap_chp_obli,3);
		bitset(Bitmap_chp_obli,4);
		bitset(Bitmap_chp_obli,7);
		bitset(Bitmap_chp_obli,11);
		bitset(Bitmap_chp_obli,12);
		bitset(Bitmap_chp_obli,17);
		bitset(Bitmap_chp_obli,32);
		bitset(Bitmap_chp_obli,37);
		bitset(Bitmap_chp_obli,41);
		bitset(Bitmap_chp_obli,42);
		bitset(Bitmap_chp_obli,49);
		/*****************************/
		bitset(Bitmap_chp_copied,2);
		bitset(Bitmap_chp_copied,3);
		bitset(Bitmap_chp_copied,4);
		bitset(Bitmap_chp_copied,7);
		bitset(Bitmap_chp_copied,11);
		bitset(Bitmap_chp_copied,12);
		bitset(Bitmap_chp_copied,17);
		bitset(Bitmap_chp_copied,32);
		bitset(Bitmap_chp_copied,37);
		bitset(Bitmap_chp_copied,41);
		bitset(Bitmap_chp_copied,42);
		bitset(Bitmap_chp_copied,49);
		break;

	case 1430:
		bitset(Bitmap_chp_obli,15);
		bitset(Bitmap_chp_obli,39);
		bitset(Bitmap_chp_obli,44);
		bitset(Bitmap_chp_obli,50);
		bitset(Bitmap_chp_obli,53);
		//bitset(Bitmap_chp_obli,64);
		/*****************************/
		bitset(Bitmap_chp_obli,2);
		bitset(Bitmap_chp_obli,5);
		bitset(Bitmap_chp_obli,3);
		bitset(Bitmap_chp_obli,4);
		bitset(Bitmap_chp_obli,7);
		bitset(Bitmap_chp_obli,11);
		bitset(Bitmap_chp_obli,12);
		bitset(Bitmap_chp_obli,32);
		bitset(Bitmap_chp_obli,37);
		//bitset(Bitmap_chp_obli,38);
		bitset(Bitmap_chp_obli,41);
		bitset(Bitmap_chp_obli,42);
		bitset(Bitmap_chp_obli,49);
		/*****************************/
		bitset(Bitmap_chp_copied,2);
		bitset(Bitmap_chp_copied,5);
		bitset(Bitmap_chp_copied,3);
		bitset(Bitmap_chp_copied,4);
		bitset(Bitmap_chp_copied,7);
		bitset(Bitmap_chp_copied,11);
		bitset(Bitmap_chp_copied,12);
		bitset(Bitmap_chp_copied,32);
		bitset(Bitmap_chp_copied,37);
		//bitset(Bitmap_chp_obli,38);
		bitset(Bitmap_chp_copied,41);
		bitset(Bitmap_chp_copied,42);
		bitset(Bitmap_chp_copied,49);
		break;

	case 1530:
		bitset(Bitmap_chp_obli,15);
		bitset(Bitmap_chp_obli,39);
		bitset(Bitmap_chp_obli,44);
		//bitset(Bitmap_chp_obli,64);
		/*****************************/
		bitset(Bitmap_chp_obli,7);
		bitset(Bitmap_chp_obli,11);
		bitset(Bitmap_chp_obli,12);
		bitset(Bitmap_chp_obli,17);
		bitset(Bitmap_chp_obli,37);
		bitset(Bitmap_chp_obli,41);
		bitset(Bitmap_chp_obli,42);
		bitset(Bitmap_chp_obli,53);
		/*****************************/
		bitset(Bitmap_chp_copied,7);
		bitset(Bitmap_chp_copied,11);
		bitset(Bitmap_chp_copied,12);
		bitset(Bitmap_chp_copied,17);
		bitset(Bitmap_chp_copied,37);
		bitset(Bitmap_chp_copied,41);
		bitset(Bitmap_chp_copied,42);
		bitset(Bitmap_chp_copied,53);
		break;

	case 1814:
		bitset(Bitmap_chp_obli,39);
		bitset(Bitmap_chp_obli,44);
		bitset(Bitmap_chp_obli,53);
		bitset(Bitmap_chp_obli,63);
		//bitset(Bitmap_chp_obli+8,72-64);
		//bitset(Bitmap_chp_obli+8,128-64);
		/*****************************/
		bitset(Bitmap_chp_obli,7);
		bitset(Bitmap_chp_obli,11);
		bitset(Bitmap_chp_obli,41);
		bitset(Bitmap_chp_obli,42);
		/*****************************/
		bitset(Bitmap_chp_copied,7);
		bitset(Bitmap_chp_copied,11);
		bitset(Bitmap_chp_copied,41);
		bitset(Bitmap_chp_copied,42);
		break;

	default :
		return KO;
	}

	for (i=1; i<65; i++)
	{
		if (bittest(Bitmap_chp_obli, i))
			if (!bittest(g_bitmap, i))
			{
				memset(Chaine,0,24);
				memcpy(Chaine,"Missing DE:",11);
				ul_champ = (Ulong)i;
				long_asc(&Chaine[11],3,&ul_champ);
				Os__xprint((PC)Chaine);
				return KO;
			}
	}
	for (i=65; i<129; i++)
	{
		j=i-64;
		if (bittest(Bitmap_chp_obli+8, j))//wd
			if (!bittest(R_Transac8583.bitmap2, j))//wd
			{
				memset(Chaine,0,24);
				memcpy(Chaine,"Missing DE:",11);
				ul_champ = (Ulong)i;
				long_asc(&Chaine[11],3,&ul_champ);
				Os__xprint((PC)Chaine);
				return KO;
			}
	}


	for (i=1; i<65; i++)
	{
		if (bittest(Bitmap_chp_copied, i))
			if (!bittest(g_bitmap, i))
			{
				memset(Chaine,0,24);
				memcpy(Chaine,"Problem DE:",11);
				ul_champ = (Ulong)i;
				long_asc(&Chaine[11],3,&ul_champ);
				Os__xprint((PC)Chaine);
				return KO;
			}
	}
	for (i=65; i<129; i++)
	{
		j=i-64;
		if (bittest(Bitmap_chp_copied+8, j))//wd
			if (!bittest(R_Transac8583.bitmap2, j))//wd
			{
				memset(Chaine,0,24);
				memcpy(Chaine,"Problem DE:",11);
				ul_champ = (Ulong)i;
				long_asc(&Chaine[11],3,&ul_champ);
				Os__xprint((PC)Chaine);
				return KO;
			}
	}
	//ShowTrace("CMES0");
	return OK;
}

/***********************	Reconciliation et tèlècollecte	*******************/
/****************************************************************************
*                                                                           *
*      SOUS-PROGRAMME :  Fin_jour                                           *
*                                                                           *
*****************************************************************************
*                                                                           *
*      FONCTION :    traitement de la réconciliation et de la tèlècollecte  *
*                                                                           *
*      ENTREE   :  Rien                                                     *
*                                                                           *
*      SORTIE   :  status                                                   *
*                                                                           *
****************************************************************************/
uchar	Fin_jour(uchar Numeroter,uchar InitModem)
{
	char			phone[17],site[16];
	uchar			longMess[3];
	uchar			status;
	uchar			devise,last_devise;
	Ushort			Essai_cnx = 0;
	Ushort			InterfaceWifi = 0;
	Ushort			nbr_tra,nbr_ann=0,nbr_pre,i,j,k,k1=0,m,nbr_tra_a_envoyer;
	Ushort			last_avis_tr,Nbr_Trs_non_avis;
	Ushort			Tab_avis[400];
	long			Tab_mont[NB_MAX_DEV_ACC][7];
	lignetra		transac;

	lignetra		transac2;

	uchar			bitmap[16];
	char			taaba[20];//CCD
	char tmp[100];

	memset(longMess,0,3);
	memset(Tab_avis,0,sizeof(Tab_avis));
	memset(Tab_mont,0,sizeof(Tab_mont));
	memset((uchar*)&transac,0,sizeof(transac));
	memset((uchar*)&transac2,0,sizeof(transac2));
	nbr_tra=FicTra.nombre_tra;
	nbr_pre=FicPreauth.nombre_tra;

	FicPara.AutoCollecteExecute = 1;

	//HWCNF_SetStandbyDelay(600);//management du mode veille
	//standby=HWCNF_GetStandbyDelay();
	//HWCNF_SetStandbyDelay(standby);

	//APEMV_UI_MessageDisplayDelay ("TELECOLLECTE\nEN COURS", 1);
	i=0;j=0;
	nbr_ann = 0;
	while(i<nbr_tra)
	{

		read_trans_index( i ,&transac) ;

		// Remplir le tableau de positions des trans offl non avisée
		if(
				//(memcmp(transac.Message_Type,"1200",4) != 0) && (memcmp(transac.Message_Type,"0200",4) != 0) &&//CCD
				(!transac.avis)&&(!transac.Flag_Dem_Aut) &&
				((transac.CodeFunction!=VOIDCODE) && (!transac.StatusAnnulAdjust))
			  )
//		if (
//		//(memcmp(transac.Message_Type,"1200",4) != 0) && (memcmp(transac.Message_Type,"0200",4) != 0) &&//CCD    //10042019
//		(!transac.avis) && (!transac.Flag_Dem_Aut)
//				&& ((transac.CodeFunction != VOIDCODE)
//						&& (!transac.StatusAnnulAdjust)))
		{

			//Os__xprintaa("!=1200");
			Tab_avis[j]=i;
			memset(taaba,0,sizeof(taaba));//CCD
			sprintf(taaba,"Tab_avis[%.02d]=%.02d",j,Tab_avis[j]);//CCD
			//Os__xprinta(taaba);
			j++;
		}
		// devise
		for(k=0;k<NB_MAX_DEV_ACC;k++)            //l'indice de la 2éme dim
		{
			if(!memcmp(Fic_Devise_Acc.TabDevise[k].code_num,transac.NumCurrency,3))
			{
				devise=(UC)k;//wd
				break;
			}
		}
		// Nbre et mont de redressements de trans débits online ou offline avisées
		if(transac.CodeFunction==VOIDCODE)
		{
			nbr_ann ++;
      		//if((transac.Flag_Dem_Aut)||(transac.avis))
			{
				Tab_mont[devise][NBR_REDR]+=1 ;    							// Nbre



				if(transac.MontantPourboireInterchange != 0)
					Tab_mont[devise][MONT_REDR]+=transac.Transaction_Amount  + transac.MontantPourboireInterchange;
				else
					Tab_mont[devise][MONT_REDR]+=transac.Transaction_Amount ;   // Montant



				Tab_mont[devise][0]=1 ;										// Présence de devise
			}
		}
		else
		{
			if(transac.CodeFunction==REFUNDCODE)
			{
	         	Tab_mont[devise][NBR_CRE]+=1;
				Tab_mont[devise][MONT_CRE]+=transac.Transaction_Amount;
				Tab_mont[devise][0]=1 ;
			}
			else
			{
				if(
					((transac.StatusAnnulAdjust) && ((transac.avis)||(transac.Flag_Dem_Aut))) ||
					(!transac.StatusAnnulAdjust))
				{
         			Tab_mont[devise][NBR_DEB]+=1 ;


         			if (transac.CodeFunction == PAIEMENT_POURBOIRE)    //POURBOIRE
         				Tab_mont[devise][MONT_DEB]+=transac.Transaction_Amount + transac.MontantPourboireInterchange;
         			else
         				Tab_mont[devise][MONT_DEB]+=transac.Transaction_Amount ;

					Tab_mont[devise][0]=1 ;
				}
				if(transac.StatusAnnulAdjust)
				{
					nbr_ann ++;
				}
			}
		}
		i++ ;
	}
//nombre des annulations
//avoir nbr_tra et nbr_ann

//	Os__xprintaa("nbr_ann");
//	Os__xprintd(nbr_ann);

	memset(taaba,0,sizeof(taaba));//CCD
	sprintf(taaba,"Tab_avis[1]=%.02d",Tab_avis[1]);//CCD
	//Os__xprinta(taaba);
	last_avis_tr=j;
	Nbr_Trs_non_avis = j;

	FLAG_Ref_SSL=0;		//Kamalssl

	// Ouvrir connexion
	if(Numeroter)
	{
		do{
			UC Message[128];

			sprintf(Message,"TELECOLLECTE\nCONNEXION:%d\nEN COURS",Essai_cnx);

			//APEMV_UI_MessageDisplayDelay (Message, 0);

			memset(phone,0x00,17);
			memset(site,0x00,16);
			if(Essai_cnx%2)
			{
				memcpy(phone,FicPara.Tel_telec_sec,17);
				memcpy(site,FicPara.X25_telec_sec,15);
			}
			else
			{
				memcpy(phone,FicPara.Tel_telec_pri,17);
				memcpy(site,FicPara.X25_telec_pri,15);
			}
			if(FicPara.g_type_connexion == 'M')
				status = connect_host_GSM((PC)phone,(PC)site);
			if(FicPara.g_type_connexion == 'R')
				status = connect_host((PC)phone,(PC)site);
			if(FicPara.g_type_connexion == 'G')
				status = connect_host_GPRS((PC)phone,(PC)site);
			if(FicPara.g_type_connexion == 'I')
			{
				if(FicPara.Etat_TLS==0)	//Kamalssl
					status = connect_host_IP((PC)phone,(PC)site);
				else
					status = connect_host_IP_TLS();
			}
			if(status != OK)
			{
				hang_up(0,g_trace);
//				InterfaceWifi++;
//				if(InterfaceWifi==2 && (FicPara.g_type_connexion == 'G')){
//					Wifi_PowerOff();
//					Wifi_PowerOn();
//					//Telium_Ttestall(0, 1*100);
//					InterfaceWifi = 0;
//						}
			}
			Essai_cnx++;
		}while((Essai_cnx < 3)&&(status != OK));

		if(status != OK)
		{
			//MessageWithoutConfirmation("echec de connexion", "file://flash/HOST/naps-tpe-icons-4.png");
			MessageWithDelay("ECHEC DE CONNEXION", "file://flash/HOST/naps-tpe-icons-4.png", 2000);
			//HWCNF_SetStandbyDelay(180);//mode veille initiale
			return CONNEXION_KO;
	}
	}

	CUMORE_CMI_AFFICHE_MESSAGE(122,123,1000);
	//Aff_mess(FicPara.Langue,122,0,0,0,0);
	//Aff_mess(FicPara.Langue,123,1,0,0,0);

	Format_dem_fj();
	memset(taaba,0,sizeof(taaba));//CCD
	sprintf(taaba,"Tab_avis[1]=%.02d",Tab_avis[1]);//CCD
//	Os__xprinta(taaba);

	if(Emis_batch(INITA)!=OK)
	{
		hang_up(typehangup,g_trace);
		DisconnectSSL();
		//HWCNF_SetStandbyDelay(180);//mode veille initiale
		return RECO_KO;
	}

	FLAG_Ref_SSL=1;		//Kamalssl

	if(Nbr_Trs_non_avis)
	{
		// Avis de transactions offline
		CUMORE_CMI_AFFICHE_MESSAGE(138,124,1000);
		//Aff_mess(FicPara.Langue,124,1,0,0,0);
		k=1;
		g_Num_Seq_Bloc = 0;//kamal251105
		E_Transac8583.l_champs72 = 0;
		E_Transac8583.l_champs110 = 0;
		memset(E_Transac8583.champs72,0,sizeof(E_Transac8583.champs72));
		memset(E_Transac8583.champs110,0,sizeof(E_Transac8583.champs110));

		memcpy(&E_Transac8583.e_type_mess,"1304",4);
		memcpy(&E_Transac8583.r_type_mess,"1314",4);
		memcpy(bitmap,bitmap1304,16);
		bitset(bitmap+8,72-64);
		bitset(bitmap, 48);

		while(k<=Nbr_Trs_non_avis)
		{
			///////////////////////////////////////////////////////////
			memset(taaba,0,sizeof(taaba));//CCD
			sprintf(taaba,"Tab_avis[%.02d]=%.02d",k-1,Tab_avis[k-1]);//CCD
			//Os__xprinta(taaba);
			Format_Transfer_Trans(Tab_avis[k-1],E_Transac8583.champs72,&E_Transac8583.l_champs72,E_Transac8583.champs110,&E_Transac8583.l_champs110);
			//Printtampon(E_Transac8583.champs72,E_Transac8583.l_champs72);
			//Printtampon(E_Transac8583.champs110,E_Transac8583.l_champs110);
			if(((k%MAX_TRS_PAR_BLOC)==0)||(k==Nbr_Trs_non_avis))
			{
				/******
				if(E_Transac8583.l_champs72 < 256)
					eft30printhexKamal(E_Transac8583.champs72,E_Transac8583.l_champs72);
				else
				{
					if(E_Transac8583.l_champs72 < 512)
					{
						eft30printhexKamal(E_Transac8583.champs72,256);
						eft30printhexKamal(E_Transac8583.champs72+256,E_Transac8583.l_champs72-256);
					}
					else
					{
						eft30printhexKamal(E_Transac8583.champs72,256);
						eft30printhexKamal(E_Transac8583.champs72+256,256);
						eft30printhexKamal(E_Transac8583.champs72+512,E_Transac8583.l_champs72-256);
					}
				}
				******/

				if(E_Transac8583.l_champs110!=0)
					bitset(bitmap+8,110-64);
				status = Format(E_Transac8583.e_type_mess,bitmap,FILE_TRS);
				//if(k<(Nbr_Trs_non_avis-1))
				if(k<Nbr_Trs_non_avis)
					memcpy(E_Transac8583.champs24,"301",3);
				//if(Emis_batch(AVIS)!=OK)
				if(k == Nbr_Trs_non_avis) FLAG_Ref_SSL = 2; // jrizki
				if(Emis_batch(AVIS) == CONNEXION_KO)
				{
					hang_up(typehangup,g_trace);
					DisconnectSSL();
					//HWCNF_SetStandbyDelay(180);//mode veille initiale
					return RECO_KO;
				}
				else
				{
					E_Transac8583.l_champs72 = 0;
					memset(E_Transac8583.champs72,0,sizeof(E_Transac8583.champs72));
					E_Transac8583.l_champs110 = 0;
					memset(E_Transac8583.champs110,0,sizeof(E_Transac8583.champs110));
					memcpy(&E_Transac8583.e_type_mess,"1304",4);
					memcpy(&E_Transac8583.r_type_mess,"1314",4);
					memcpy(bitmap,bitmap1304,16);
					bitset(bitmap+8,72-64);
				}
			}
			k+=1 ;
			///////////////////////////////////////////////////////////
		}
	}
	//Telium_Ttestall(0, 61*100);
	CUMORE_CMI_AFFICHE_MESSAGE(138,125,1000);
	//Aff_mess(FicPara.Langue,125,1,0,0,0);
	//reconciliation par devise
	if(nbr_tra!=0)
	{
		for(k=0,last_devise=0;k<NB_MAX_DEV_ACC;k++)
		{
			if(Tab_mont[k][0])
				last_devise=(UC)k;//wd
		}
		//HWCNF_SetBacklightDuration(120);
		k=0;
		while(k<NB_MAX_DEV_ACC)
		{
			if(Tab_mont[k][0])
			{
				if(k==last_devise)

					Format_rec(1,(UC)k,Tab_mont[k]);//wd
				else
					Format_rec(0,(UC)k,Tab_mont[k]);//wd    //Format de la derniére rec

				status=Emis_batch(RECONC);
				if(status!=OK)
					break;
			}
			k++ ;
		}
	}
	else
	{
		sprintf(tmp, "%d Transaction(s)");
		MessageWithoutConfirmation("Aucune transaction", NULL);
		Os__xprinta("Aucune transaction");
		Tab_mont[0][NBR_REDR]  = 0 ;
		Tab_mont[0][MONT_REDR] = 0;
		Tab_mont[0][0]=1 ;
		Tab_mont[0][NBR_CRE] = 0 ;
		Tab_mont[0][MONT_CRE] =0 ;
		Tab_mont[0][NBR_DEB] = 0 ;
		Tab_mont[0][MONT_DEB] = 0 ;
		Format_rec(1,0,Tab_mont[0]);
		status=Emis_batch(RECONC);
		if(memcmp(R_Transac8583.champs39,"581",3)==0)
			status = OK;//ici

		if(status!=OK)
		{
			DisconnectSSL();
			//HWCNF_SetStandbyDelay(180);//mode veille initiale
			return RECO_KO;
		}
	}

	// Si la réconciliation a échouée alors demande de tèlècollecte
	if(status!=OK)
	{
		CUMORE_CMI_AFFICHE_MESSAGE(138,126,1000);
			sprintf(tmp, "%d Transaction(s)", nbr_tra);
			MessageWithoutConfirmation(tmp, NULL);
		//Aff_mess(FicPara.Langue,126,1,0,0,0);
		k=1;
		g_Num_Seq_Bloc = 0;//kamal251105
		nbr_tra_a_envoyer =0;
		E_Transac8583.l_champs72 = 0;
		E_Transac8583.l_champs110 = 0;
		memset(E_Transac8583.champs72,0,sizeof(E_Transac8583.champs72));
		memset(E_Transac8583.champs110,0,sizeof(E_Transac8583.champs110));
		memcpy(&E_Transac8583.e_type_mess,"1304",4);
		memcpy(&E_Transac8583.r_type_mess,"1314",4);
		memcpy(bitmap,bitmap1304,16);
		bitset(bitmap+8,72-64);
		k1=1;
		while(k<=nbr_tra)
		{
			sprintf(tmp, "%d Transaction(s)\nEnvoi ... (%d) ", nbr_tra, k);
			MessageWithoutConfirmation(tmp, NULL);
			m=k-1;
			read_trans_index( m ,&transac);

			if(
				//(transac.CodeFunction==VOIDCODE) ||
				((transac.CodeFunction!=VOIDCODE)&&(!transac.StatusAnnulAdjust))
			)
			{
//				sprintf(tmp, "%d Transaction(s)\nEnvoi ... (%d) ", nbr_tra, k);
//				MessageWithoutConfirmation(tmp, NULL);
				Format_Transfer_Trans(m,E_Transac8583.champs72,&E_Transac8583.l_champs72,E_Transac8583.champs110,&E_Transac8583.l_champs110);
				nbr_tra_a_envoyer +=1;
			}
			//if((nbr_tra_a_envoyer==MAX_TRS_PAR_BLOC)||(k==nbr_tra))
			if((nbr_tra_a_envoyer==MAX_TRS_PAR_BLOC)||(k1 == (nbr_tra-nbr_ann)))
			{
				/******
				if(E_Transac8583.l_champs110 < 256)
					eft30printhexKamal(E_Transac8583.champs110,E_Transac8583.l_champs110);
				else
				{
					if(E_Transac8583.l_champs110 < 512)
					{
						eft30printhexKamal(E_Transac8583.champs110,256);
						eft30printhexKamal(E_Transac8583.champs110+256,E_Transac8583.l_champs110-256);
					}
					else
					{
						eft30printhexKamal(E_Transac8583.champs110,256);
						eft30printhexKamal(E_Transac8583.champs110+256,256);
						eft30printhexKamal(E_Transac8583.champs110+512,E_Transac8583.l_champs110-256);
					}

				}
				******/

				if(E_Transac8583.l_champs110!=0)
					bitset(bitmap+8,110-64);
				status = Format(E_Transac8583.e_type_mess,bitmap,FILE_TRS);
				//if(k<(nbr_tra-1))
				//if(k<nbr_tra)
				if(k1< (nbr_tra-nbr_ann))
					memcpy(E_Transac8583.champs24,"301",3);

               if(nbr_tra_a_envoyer!=0)
               {

						if(Emis_batch(TELEC)!=OK)
						{

							hang_up(typehangup,g_trace);
							DisconnectSSL();
							//HWCNF_SetStandbyDelay(180);//mode veille initiale
							return RECO_KO;
						}
						else
						{
							nbr_tra_a_envoyer = 0;
							E_Transac8583.l_champs72 = 0;
							memset(E_Transac8583.champs72,0,sizeof(E_Transac8583.champs72));
							E_Transac8583.l_champs110 = 0;
							memset(E_Transac8583.champs110,0,sizeof(E_Transac8583.champs110));

							memcpy(&E_Transac8583.e_type_mess,"1304",4);
							memcpy(&E_Transac8583.r_type_mess,"1314",4);
							memcpy(bitmap,bitmap1304,16);
							bitset(bitmap+8,72-64);
						}
                }
			}
			//}

			k+=1;
			//trs non annulee
			if((transac.CodeFunction!=VOIDCODE)&&(!transac.StatusAnnulAdjust))
				k1+=1;
		}

		if(InitModem)
			hang_up(typehangup,g_trace);

		if(R_Transac8583.l_champs44>=12)
		{
			if(FicPara.Update_param_Emv != '1')
				Os__saved_set((uchar*)&FicPara.Update_param_Emv,R_Transac8583.champs44[0],1);
			if(FicPara.Update_Appli != '1')
				Os__saved_set((uchar*)&FicPara.Update_Appli,R_Transac8583.champs44[1],1);
			if(FicPara.Update_bin != '1')
				Os__saved_set((uchar*)&FicPara.Update_bin,R_Transac8583.champs44[3],1);
			if(FicPara.Update_LN != '1')
				Os__saved_set((uchar*)&FicPara.Update_LN,R_Transac8583.champs44[4],1);
			if(FicPara.Update_mk != '1')
				Os__saved_set((uchar*)&FicPara.Update_mk,R_Transac8583.champs44[5],1);
			if(FicPara.Update_mac != '1')
				Os__saved_set((uchar*)&FicPara.Update_mac,R_Transac8583.champs44[6],1);
			if(FicPara.Update_compt_dom != '1')
				Os__saved_set((uchar*)&FicPara.Update_compt_dom,R_Transac8583.champs44[7],1);
			if(FicPara.Update_pos_param != '1')
				Os__saved_set((uchar*)&FicPara.Update_pos_param,R_Transac8583.champs44[8],1);
		}
		DisconnectSSL();
		//HWCNF_SetStandbyDelay(180);//mode veille initiale
		return TEL_OK;
	}
	if(InitModem)
		hang_up(typehangup,g_trace);

	if(R_Transac8583.l_champs44>=12)
	{
		if(FicPara.Update_param_Emv != '1')
			Os__saved_set((uchar*)&FicPara.Update_param_Emv,R_Transac8583.champs44[0],1);
		if(FicPara.Update_Appli != '1')
			Os__saved_set((uchar*)&FicPara.Update_Appli,R_Transac8583.champs44[1],1);
		if(FicPara.Update_bin != '1')
			Os__saved_set((uchar*)&FicPara.Update_bin,R_Transac8583.champs44[3],1);
		if(FicPara.Update_LN != '1')
			Os__saved_set((uchar*)&FicPara.Update_LN,R_Transac8583.champs44[4],1);
		if(FicPara.Update_mk != '1')
			Os__saved_set((uchar*)&FicPara.Update_mk,R_Transac8583.champs44[5],1);
		if(FicPara.Update_mac != '1')
			Os__saved_set((uchar*)&FicPara.Update_mac,R_Transac8583.champs44[6],1);
		if(FicPara.Update_compt_dom != '1')
			Os__saved_set((uchar*)&FicPara.Update_compt_dom,R_Transac8583.champs44[7],1);
		if(FicPara.Update_pos_param != '1')
			Os__saved_set((uchar*)&FicPara.Update_pos_param,R_Transac8583.champs44[8],1);
	}
	DisconnectSSL();
	//HWCNF_SetStandbyDelay(180);//mode veille initiale
	return RECO_OK;
}

/****************************************************************************
*                                                                           *
*      SOUS-PROGRAMME :  Format_dem_fj                                      *
*                                                                           *
*****************************************************************************
*                                                                           *
*      FONCTION :                                                           *
*                                                                           *
*      ENTREE   :  Formatage du message pour une demande de fin de journée  *
*                                                                           *
*      SORTIE   :  status                                                   *
*                                                                           *
****************************************************************************/
uchar Format_dem_fj()
{
	uchar	Date_YYMMDDHHmmss[13];
	Ushort	OffSet;
	DATE date;

	memcpy(E_Transac8583.e_type_mess,"1804",4);
	memcpy(E_Transac8583.r_type_mess,"1814",4);
	memcpy(E_Transac8583.bitmap,bmp1804,8);

	Telium_Read_date(&date);
	sprintf ((char *)Date_YYMMDDHHmmss,
		"%.2s%.2s%.2s%.2s%.2s%.2s",
		date.year,
		date.month,
		date.day,
		date.hour,
		date.minute,
		date.second);
	memcpy(E_Transac8583.champs7,Date_YYMMDDHHmmss,10);

/*	Os__read_date(Date_sys);
	Os__read_time_sec(Time_sys);
	memset(Date_YYMMDDHHmmss,'\0',13);
	memcpy(Date_YYMMDDHHmmss,&Date_sys[4],2);
	memcpy(&Date_YYMMDDHHmmss[2],&Date_sys[2],2);
	memcpy(&Date_YYMMDDHHmmss[4],Date_sys,2);
	memcpy(&Date_YYMMDDHHmmss[6],Time_sys,6);
	memcpy(E_Transac8583.champs7,Date_YYMMDDHHmmss,10);*/
	memcpy(E_Transac8583.champs11,FicPara.Message_Number,6);
	IncStan();

	memcpy(E_Transac8583.champs12,Date_YYMMDDHHmmss,12);
	memcpy(E_Transac8583.champs17,&Date_YYMMDDHHmmss[2],4);
	memcpy(E_Transac8583.champs24,"801",3);
	memcpy(E_Transac8583.champs37,FicPara.Num_sequence,6);
	memcpy(&E_Transac8583.champs37[6],FicPara.Num_seqAnnul,6) ;
	memcpy(E_Transac8583.champs41,FicPara.Num_Terminal,8);
	memset(E_Transac8583.champs42,' ',15);
	memcpy(E_Transac8583.champs42,FicPara.Num_point_vente,strlen((PC)FicPara.Num_point_vente));
	E_Transac8583.l_champs53 = 10;
	memcpy(E_Transac8583.champs53,"0199000000",10);

	E_Transac8583.l_champs63 = 26;//CCD
	OffSet = 0;
	memcpy(&E_Transac8583.champs63[OffSet],FicPara.Index_MacKey,3);
	OffSet += 3;
	memcpy(&E_Transac8583.champs63[OffSet],FicPara.Index_MK,3);
	OffSet += 3;
	memcpy(&E_Transac8583.champs63[OffSet],FicPara.Version_Tab_Emv,3);
	OffSet += 3;
	memcpy(&E_Transac8583.champs63[OffSet],FicPara.Version_Bin_Prefixe,3);
	OffSet += 3;
	memcpy(&E_Transac8583.champs63[OffSet],lop_gest.NumListe,4);
	OffSet += 4;
	memcpy(&E_Transac8583.champs63[OffSet],FicPara.Version_Pos_Param,3);
	OffSet += 3;
	//modif	04/04
	memcpy(&E_Transac8583.champs63[OffSet],FicPara.Version_appli,4);
	OffSet += 4;
	//modif	04/04

	memcpy(&E_Transac8583.champs63[OffSet],FicPara.Version_Compte_Dom,3);
	OffSet += (Ushort)3;

   return OK ;
}
/****************************************************************************
*                                                                           *
*      SOUS-PROGRAMME :  Emis_batch                                         *
*                                                                           *
*****************************************************************************
*                                                                           *
*      FONCTION :                                                           *
*                                                                           *
*      ENTREE   :  Type de transmission(Avis,dem tel,reconciliation)        *
*                                                                           *
*      SORTIE   :  status                                                   *
*                                                                           *
****************************************************************************/
uchar	Emis_batch(uchar type)
{
	uchar  							status;

	status = Emission(E_Transac8583.e_type_mess,E_Transac8583.bitmap);

	if (status != OK)
	{
		return CONNEXION_KO ;
	}
	//***********************************************
	//ShowTrace_TRC_TELEC("Emission passe _______");
	//Telium_Ttestall( 0, 50 );
	//***********************************************
	status = Reception();
	if (status != OK)
	{
		return CONNEXION_KO;
	}
	switch(type)
	{
		case INITA:
			if(memcmp(R_Transac8583.champs39,"800",3))
				return KO ;
			//ShowTrace("INITA");
			break;
		case AVIS:
			if(memcmp(R_Transac8583.champs39,"300",3))
				return KO ;
			//ShowTrace("AVIS");
			break;
		case RECONC:
			if(memcmp(R_Transac8583.champs39,"500",3))
				return KO ;
			//ShowTrace("RECONC");
			break;
		case TELEC:
			if(memcmp(R_Transac8583.champs39,"300",3))
				return KO ;
			//ShowTrace("TELEC");
			break;
		case CONFIRM:
			if(memcmp(R_Transac8583.champs39,"300",3))
				return KO ;
			//ShowTrace("CONFIRM");
			break;
		case COMPTE:
			if(memcmp(R_Transac8583.champs39,"300",3))
				return KO ;
			//ShowTrace("COMPTE");
			break;
      default :
      	return KO;
	}
	if(R_Transac8583.l_champs44>=12)
	{
		if(FicPara.Update_param_Emv != '1')
			Os__saved_set((uchar*)&FicPara.Update_param_Emv,R_Transac8583.champs44[0],1);
		if(FicPara.Update_Appli != '1')
			Os__saved_set((uchar*)&FicPara.Update_Appli,R_Transac8583.champs44[1],1);
		if(FicPara.Update_bin != '1')
			Os__saved_set((uchar*)&FicPara.Update_bin,R_Transac8583.champs44[3],1);
		if(FicPara.Update_LN != '1')
			Os__saved_set((uchar*)&FicPara.Update_LN,R_Transac8583.champs44[4],1);
		if(FicPara.Update_mk != '1')
			Os__saved_set((uchar*)&FicPara.Update_mk,R_Transac8583.champs44[5],1);
		if(FicPara.Update_mac != '1')
			Os__saved_set((uchar*)&FicPara.Update_mac,R_Transac8583.champs44[6],1);
		if(FicPara.Update_compt_dom != '1')
			Os__saved_set((uchar*)&FicPara.Update_compt_dom,R_Transac8583.champs44[7],1);
		if(FicPara.Update_pos_param != '1')
			Os__saved_set((uchar*)&FicPara.Update_pos_param,R_Transac8583.champs44[8],1);
	}
	return OK ;
}

/***************************************************************************************/
uchar	Format_Transfer_Trans(Ushort Index_trs,uchar *BufferTrs,Ushort *LgBuff,uchar *DataEmv,Ushort *LgDataEmv)
{
	lignetra		transac ;
	Ushort		OffSet,i,Lg;
	uchar			iso2[40],montant[13],Date_tra[7],Tmp_Chaine[5];

	read_trans_index( Index_trs ,&transac) ;
	OffSet = *LgBuff;

	memcpy(&BufferTrs[OffSet],transac.Message_Type,4);
	OffSet +=(Ushort)4;

	memcpy(&BufferTrs[OffSet],transac.Proc_Code,6);
	OffSet +=(Ushort)6;

	memcpy(&BufferTrs[OffSet],transac.STAN,6);
	OffSet +=(Ushort)6;


	if(transac.Entry_Mode == TRS_MAN)
 	{
		memcpy(&BufferTrs[OffSet],&transac.Compressed_Iso2[1],transac.Compressed_Iso2[0]);
		OffSet += (Ushort)transac.Compressed_Iso2[0];
 	}
 	else if(transac.Entry_Mode == TRS_SCT)
 	{
 		sprintf(iso2,"%s",&transac.Compressed_Iso2[1]);

 		//for(i=0;((i<20)&&(iso2[i] !='d')&&(iso2[i] !='D')&&(iso2[i] !='='));i++);
 		for(i=0;((i<19)&&(iso2[i] !='d')&&(iso2[i] !='D')&&(iso2[i] !='='));i++);//badr problème d'annulation des transaction avec PAN CARD = 19
		memcpy(&BufferTrs[OffSet],iso2,i);
		OffSet+=i ;
//		Os__xprintaa("Compressed_Iso2");
//		Os__xprintaa(transac.Compressed_Iso2);
//		Os__xprintaa("iso2 aa");
//		Os__xprintaa(iso2);
//		Os__xprintaa("BufferTrs");
//		Os__xprintaa(BufferTrs);
 	}
 	else
 	{
		bcd_asc(iso2,&transac.Compressed_Iso2[1],20);
		for(i=0;((i<20)&&(iso2[i] !='d')&&(iso2[i] !='D')&&(iso2[i] !='='));i++);
		memcpy(&BufferTrs[OffSet],iso2,i);
		OffSet+=i ;
//		Os__xprintaa("Compressed_Iso2");
//		Os__xprintaa(transac.Compressed_Iso2);
//		Os__xprintaa("iso2 aa");
//		Os__xprintaa(iso2);
//		Os__xprintaa("BufferTrs");
//		Os__xprintaa(BufferTrs);
	}


/*
	if(transac.Entry_Mode == TRS_MAN)
	{
		memcpy(&BufferTrs[OffSet],&transac.Compressed_Iso2[1],transac.Compressed_Iso2[0]);
		OffSet += (Ushort)transac.Compressed_Iso2[0];
	}
	else
	{
		bcd_asc(iso2,&transac.Compressed_Iso2[1],20);
		for(i=0;((i<20)&&(iso2[i] !='D')&&(iso2[i] !='='));i++);
		memcpy(&BufferTrs[OffSet],iso2,i);
		OffSet+=i ;
	}
*/


	BufferTrs[OffSet] = '*';
	OffSet+=(Ushort)1 ;

	memset(montant,0,13) ;

	long_asc (montant, 12, &transac.Transaction_Amount);

	if (transac.CodeFunction == PAIEMENT_POURBOIRE){    //POURBOIRE
		Os__xprinta("pourboire 33");
		UL MNT_Tot = transac.Transaction_Amount + transac.MontantPourboireInterchange;

		long_asc (montant, 12, &MNT_Tot);
	}

	memcpy(&BufferTrs[OffSet],montant, 12);
	OffSet+=(Ushort)12 ;



	memcpy(&BufferTrs[OffSet],transac.NumCurrency, 3);
	OffSet+=(Ushort)3 ;

	memset(montant,0,13) ;
	long_asc (montant, 12, &transac.Consolidation_Amount);
	memcpy(&BufferTrs[OffSet],montant, 12);
	OffSet+=(Ushort)12 ;

	memcpy(transac.NumCurrency_cons,transac.NumCurrency,3);/* AILAL */ /* Déblocage RHOMMARI */ /* Fichier LIS HPSS */
	memcpy(&BufferTrs[OffSet],transac.NumCurrency_cons, 3);
	OffSet+=(Ushort)3 ;

	memset(Date_tra,0,7) ;
	sprintf((PC)Date_tra,"%.2s%.2s%.2s",transac.Transaction_DateTime+4,
					  transac.Transaction_DateTime+2,transac.Transaction_DateTime);
	memcpy(&BufferTrs[OffSet],Date_tra, 6);
	OffSet+=(Ushort)6 ;

	memcpy(&BufferTrs[OffSet],transac.Approval_Code, 6);
	OffSet+=(Ushort)6 ;

	memcpy(&BufferTrs[OffSet],"000000000",9); // Reservé à revoir avec le CMI
	OffSet+=(Ushort)9 ;

	switch(transac.Top_Forcage)
	{
		case TOP_FOR_ONLINE:
			BufferTrs[OffSet] = '1';
			break;
		case TOP_FOR_REFFERAL:
		case TOP_FOR_FORCED:
			BufferTrs[OffSet] = '2';
			break;
		default:
			BufferTrs[OffSet] = '0';
			break;
	}
	OffSet+=(Ushort)1 ;

	BufferTrs[OffSet] = '?' ;
	OffSet+=1 ;

	*LgBuff = OffSet;

	//////////////////// Data EMV ////////////////////
	if(transac.Entry_Mode == TRS_EMV)
	{
		OffSet = *LgDataEmv;
		memset(Tmp_Chaine,0,sizeof(Tmp_Chaine));
		Lg = transac.Lg_EmvData+3;
		sprintf((PC)Tmp_Chaine,"%04d",Lg);

		asc_hex(&DataEmv[OffSet],2,Tmp_Chaine,4);
		OffSet+=(Ushort)2 ;

		asc_bcd(&DataEmv[OffSet],3,transac.STAN,6);
		OffSet+=(Ushort)3 ;

		memcpy(&DataEmv[OffSet],transac.EmvData, transac.Lg_EmvData);
		OffSet+=transac.Lg_EmvData ;
		*LgDataEmv = OffSet;
	}
	return OK;
}

/****************************************************************************
*                                                                           *
*      SOUS-PROGRAMME :  Format_rec                                         *
*                                                                           *
*****************************************************************************
*                                                                           *
*      FONCTION : Formatage du message pour une demande de consolidation    *
*                                                                           *
*      ENTREE   :  dernier devise,sa position,tableau des totaux            *
*                                                                           *
*      SORTIE   :  status                                                   *
*                                                                           *
****************************************************************************/
uchar	Format_rec(uchar last_devise,uchar pos_devise,long *Tab_mont)
{
	uchar			Nbre_tra[11];
	uchar			montant[17];
	uchar			Date_YYMMDDHHmmss[13];
	Ulong			Montant_net;
	Ushort			OffSet;
	DATE date;

	memcpy(E_Transac8583.e_type_mess,"1520",4);
	memcpy(E_Transac8583.r_type_mess,"1530",4);
	memcpy(E_Transac8583.bitmap,bmp1520,16);

	Telium_Read_date(&date);
	sprintf ((char *)Date_YYMMDDHHmmss,
		"%.2s%.2s%.2s%.2s%.2s%.2s",
		date.year,
		date.month,
		date.day,
		date.hour,
		date.minute,
		date.second);
	memcpy(E_Transac8583.champs7,Date_YYMMDDHHmmss,10);

	/*Os__read_date(Date_sys);
	Os__read_time_sec(Time_sys);
	memset(Date_YYMMDDHHmmss,'\0',13);
	memcpy(Date_YYMMDDHHmmss,&Date_sys[4],2);
	memcpy(&Date_YYMMDDHHmmss[2],&Date_sys[2],2);
	memcpy(&Date_YYMMDDHHmmss[4],Date_sys,2);
	memcpy(&Date_YYMMDDHHmmss[6],Time_sys,6);
	memcpy(E_Transac8583.champs7,Date_YYMMDDHHmmss,10);*/
	memcpy(E_Transac8583.champs11,FicPara.Message_Number,6);
	IncStan();
	memcpy(E_Transac8583.champs12,Date_YYMMDDHHmmss,12);
	memcpy(E_Transac8583.champs17,&Date_YYMMDDHHmmss[2],4);

	if(last_devise)
		memcpy(E_Transac8583.champs24,"500",3);     //consolidation final
	else
		memcpy(E_Transac8583.champs24,"501",3);     //consol interm dans une monnaie

	E_Transac8583.l_champs32 = 6;
	memcpy(E_Transac8583.champs32,"011010",6);

	memcpy(E_Transac8583.champs37,FicPara.Num_sequence,6);
	memcpy(&E_Transac8583.champs37[6],FicPara.Num_seqAnnul,6) ;
	memcpy(E_Transac8583.champs41,FicPara.Num_Terminal,8);
	memset(E_Transac8583.champs42,' ',15);
	memcpy(E_Transac8583.champs42,FicPara.Num_point_vente,strlen((PC)FicPara.Num_point_vente));
	memcpy(E_Transac8583.champs50,Fic_Devise_Acc.TabDevise[pos_devise].code_num,3);
	E_Transac8583.l_champs53 = 10;
	memcpy(E_Transac8583.champs53,"0199000000",10);

	//champ54
	memset(E_Transac8583.champs54,'0',20);
	E_Transac8583.l_champs54 = 20;

	E_Transac8583.l_champs62 = 30;
	memset(E_Transac8583.champs62,'0',30);

	E_Transac8583.l_champs63 = 26;//CCD
	OffSet = 0;
	memcpy(&E_Transac8583.champs63[OffSet],FicPara.Index_MacKey,3);
	OffSet += (Ushort)3;
	memcpy(&E_Transac8583.champs63[OffSet],FicPara.Index_MK,3);
	OffSet += (Ushort)3;
	memcpy(&E_Transac8583.champs63[OffSet],FicPara.Version_Tab_Emv,3);
	OffSet += (Ushort)3;
	memcpy(&E_Transac8583.champs63[OffSet],FicPara.Version_Bin_Prefixe,3);
	OffSet += (Ushort)3;
	memcpy(&E_Transac8583.champs63[OffSet],lop_gest.NumListe,4);
	OffSet += (Ushort)4;
	memcpy(&E_Transac8583.champs63[OffSet],FicPara.Version_Pos_Param,3);
	OffSet += (Ushort)3;
	memcpy(&E_Transac8583.champs63[OffSet],FicPara.Version_appli,4);
	OffSet += 4;
	memcpy(&E_Transac8583.champs63[OffSet],FicPara.Version_Compte_Dom,3);
	OffSet += (Ushort)3;
	//NC	74	74	74	74	74	74	74	74	74	74	74	74	74	74	74
	memset(Nbre_tra,0,11);
	long_asc ((uchar *)Nbre_tra, 10, (Ulong *)&Tab_mont[NBR_CRE]);
	memcpy(E_Transac8583.champs74,Nbre_tra,10);
	//NCR	75	75	75	75	75	75	75	75	75	75	75	75	75	75	75
	memcpy(E_Transac8583.champs75,"0000000000",10);
	//ND	76	76	76	76	76	76	76	76	76	76	76	76	76	76	76
	memset(Nbre_tra,0,11);
	long_asc ((uchar *)Nbre_tra, 10, (Ulong *)&Tab_mont[NBR_DEB]);
	memcpy(E_Transac8583.champs76,Nbre_tra,10);
	//NDR	77	77	77	77	77	77	77	77	77	77	77	77	77	77	77
	memset(Nbre_tra,0,11);
	long_asc (Nbre_tra, 10, (Ulong *)&Tab_mont[NBR_REDR]);
	memcpy(E_Transac8583.champs77,Nbre_tra,10);
	//NP	83	83	83	83	83	83	83	83	83	83	83	83	83	83	83
	memcpy(E_Transac8583.champs83,"0000000000",10);
	//NPR	84	84	84	84	84	84	84	84	84	84	84	84	84	84	84
	memcpy(E_Transac8583.champs84,"0000000000",10);
	//MC	86	86	86	86	86	86	86	86	86	86	86	86	86	86	86
	memset(montant,0,17);
	long_asc (montant, 16, (Ulong *)&Tab_mont[MONT_CRE]);
	memcpy(E_Transac8583.champs86,montant,16);
	//MCR	87	87	87	87	87	87	87	87	87	87	87	87	87	87	87
	memcpy(E_Transac8583.champs87,"0000000000000000",16);
	//MD	88	88	88	88	88	88	88	88	88	88	88	88	88	88	88
	memset(montant,0,17);
	long_asc (montant, 16, (Ulong *)&Tab_mont[MONT_DEB]);
	memcpy(E_Transac8583.champs88,montant,16);
	//MDR	89	89	89	89	89	89	89	89	89	89	89	89	89	89	89
	memset(montant,0,17);
	long_asc (montant, 16, (Ulong *)&Tab_mont[MONT_REDR]);
	memcpy(E_Transac8583.champs89,montant,16);
	//TBRUT	97	97	97	97	97	97	97	97	97	97	97	97	97	97	97
	if(Tab_mont[MONT_DEB]>=(Tab_mont[MONT_REDR]+Tab_mont[MONT_CRE]))
	{
		E_Transac8583.champs97[0] = 'C';
		Montant_net = Tab_mont[MONT_DEB] - (Tab_mont[MONT_REDR]+Tab_mont[MONT_CRE]);
	}
	else
	{
		E_Transac8583.champs97[0] = 'D';
		Montant_net = (Tab_mont[MONT_REDR]+Tab_mont[MONT_CRE]) - Tab_mont[MONT_DEB];
	}
	memset(montant,0,17);
	long_asc (montant, 16, &Montant_net);
	memcpy(&E_Transac8583.champs97[1],montant,16);
	return OK;
}

uchar	CheckMacEft30(void)
{
	uchar		status,i;
	uchar		Mac_Key[9];
	uchar		MAC[8];

	memset(Mac_Key,'\0',9);
	Os__saved_set ((uchar *)&FicPara.Mac_Key, '\x01', 8);
	memcpy(Mac_Key,FicPara.Mac_Key,8);
	i=0;
	memset(g_tompon1,0,sizeof(g_tompon1));
	memcpy(g_tompon1+i,"\x49\x53\x4F\x30\x31\x37\x34\x32\x30",9);i+=9;
	memcpy(g_tompon1+i,"\x31\x30\x33\x30\x30\x30\x30\x31",8);i+=8;
	memcpy(g_tompon1+i,"\x33\x30\x34\x82\x30\x81\x00\x08",8);i+=8;
	memcpy(g_tompon1+i,"\xC0\x08\x02\x02\x00\x00\x00\x08",8);i+=8;
	memcpy(g_tompon1+i,"\x00\x00\x01\x30\x34\x30\x37\x31",8);i+=8;
	memcpy(g_tompon1+i,"\x34\x30\x38\x34\x33\x33\x37\x30",8);i+=8;
	memcpy(g_tompon1+i,"\x30\x30\x31\x30\x34\x30\x37\x31",8);i+=8;
	memcpy(g_tompon1+i,"\x34\x30\x38\x34\x33\x33\x37\x30",8);i+=8;
	memcpy(g_tompon1+i,"\x37\x31\x34\x33\x30\x30\x30\x30",8);i+=8;
	memcpy(g_tompon1+i,"\x30\x30\x30\x31\x30\x30\x30\x30",8);i+=8;
	memcpy(g_tompon1+i,"\x30\x30\x39\x30\x30\x30\x33\x30",8);i+=8;
	memcpy(g_tompon1+i,"\x30\x31\x36\x36\x30\x31\x30\x30",8);i+=8;
	memcpy(g_tompon1+i,"\x30\x33\x30\x37\x20\x20\x20\x20",8);i+=8;
	memcpy(g_tompon1+i,"\x20\x10\x30\x31\x39\x39\x30\x30",8);i+=8;
	memcpy(g_tompon1+i,"\x30\x30\x30\x30\x00\x25\x30\x30",8);i+=8;
	memcpy(g_tompon1+i,"\x30\x30\x30\x30\x30\x30\x30\x30",8);i+=8;
	memcpy(g_tompon1+i,"\x30\x30\x00\x00\x00\x00\x30\x30",8);i+=8;
	memcpy(g_tompon1+i,"\x30\x30\x31\x30\x33\x30\x30\x30",8);i+=8;
	memcpy(g_tompon1+i,"\x30\x30\x30\x30\x30\x30\x31\x13",8);i+=8;
	memcpy(g_tompon1+i,"\x50\x4F\x53\x20\x50\x41\x52\x41",8);i+=8;
	memcpy(g_tompon1+i,"\x4D\x45\x54\x45\x52",5);i+=5;

	Calcul_Mac(g_tompon1+36, i-36, MAC, Mac_Key);
	//eft30printhex(g_tompon1,i+8);
	//ShowTrace("MAC");
	//eft30printhex(MAC,8);
	//imp_buffer(g_tompon1,i+8);
	return status;
}
void Telecharegement_auto(void)
{
//Fermer17
}
uchar tel_auto(void) {
//
//	unsigned char	heure[5];
//	unsigned char	heur[10];
//	Ushort			cptapp;
//	DATE			date;
//	uchar 			update_param[6];
//	uchar 			update_date[11];
//	uchar			l_l_retour;
//
//	// test si fichiers valides
//
//	if(FicPara.status==KO)
//		//APEMV_UI_MessageDisplayDelay("\nAuto Job:Para status KO",2);
//
//	// si compteur superieur au nombre max de tentative sortie
///*	if ( (FicPara.Nbr_of_redial== '0')||(FicPara.compteur_appel >= FicPara.Nbr_of_redial) )
//		//APEMV_UI_MessageDisplayDelay("\nAuto Job:compt app>=Nbr re dial",2);
//	else
//*/	{
//		Telium_Read_date(&date);
//		sprintf((char *)heur,"%.2s:%.2s:%.2s",date.hour,date.minute,date.second);
//		sprintf((char *)heure,"%.2s%.2s",date.hour,date.minute);
//
////		//APEMV_UI_MessageDisplayDelay(heure,3);
////		//APEMV_UI_MessageDisplayDelay(FicPara.Heure_appel,3);
//
//		//ShowTrace_Mouna("AA");
//		if((memcmp(heure,FicPara.Heure_appel,4) == 0) && (FicPara.AutoCollecteExecute !=  1))
//		{
//			FicPara.AutoCollecteOK =  0;
//			PutAllParam();
//		}
//
//		if(FicPara.AutoCollecteOK == 0)
//		{
//			memset(update_param,0,6);
//			memset(update_date,0,11);
//			l_l_retour = Fin_jour(1,0);
//			if((l_l_retour == RECO_KO) && (FicTra.nombre_tra != 0))
//			{
//				//incrementation du compteur de tentatives
//				cptapp=FicPara.compteur_appel+1;
//				set_ushort_secourue(&FicPara.compteur_appel,cptapp);
//				if (FicPara.compteur_appel >= FicPara.Nbr_of_redial)
//				{
//					//Mise … zero compteur d'connection_param, reinit time d'connection_param
//					//prise en compte connection_param comme si effectu‚ correctement
//					Telium_Read_date(&date);
//					sprintf((char *)heur,"%.2s%.2s%.2s",date.hour,date.minute,date.second);
//					Os__saved_copy( &heur[0], &FicPara.Date_appel[0],6);
//					set_ushort_secourue(&FicPara.compteur_appel,0);
//				}
//				PutAllTra();
//				PurgePreauto();
//			}
//			else
//			{
//				ImprimeTelecollecte(1,l_l_retour);
//				Efface_Trs();
//
//				FicPara.AutoCollecteOK =  1;
//				PutAllParam();
//				PutAllTra();
//
//				/**/
//				Telium_Read_date(&date);
//				sprintf((char *)heur,"%.2s%.2s%.2s",date.hour,date.minute,date.second);
//				Os__saved_copy( &heur[0], &FicPara.Date_appel[0],6);
//				set_ushort_secourue(&FicPara.compteur_appel,0);
//
//				gest_param();
//				PutAllTra();
//				/*
//				PurgePreauto();
//				*/
//
//				initialisation_cmi();
//				//APEMV_UI_MessageDisplayDelay ("BONJOUR\nTELECOLLECTE", 0); 			//Test17
//			}
//
//			/*
//			if((FicPara.Update_Appli == '1')&&(!FicTra.nombre_tra))
//			{
//				Os__saved_set((uchar*)&FicPara.Update_Appli,0,1);
//				PutAllParam();
//
//				;//Fermer17CallHostSample();
//			}
//			*/
//
//			////APEMV_UI_MessageDisplayDelay ("BONJOUR\nTELECOLLECTE", 0); 			//Test17
//		}
//	}
	findejournee_cmi(0);
	return(OK);
}

UC connect_host_IP_TLS()
{
	int	res =0;

	res=CallPerformConnectionIP_TLS(FicPara.ip_gprs,atoi(FicPara.port_gprs));

	if (res ==OK)
		return OK;
	else
		return KO;

	return OK;
}

