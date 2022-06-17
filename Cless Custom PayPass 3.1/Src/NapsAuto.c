/* ======================================================================== */
/* Include Files                                                            */
/* ======================================================================== */

#include "ClessSample_Implementation.h"
//#include "SDK_Naps.h"
//#include "_emvdctag_.h"
#include "TlvTree.h"
#include "del_lib.h"
#include "convert.h"
//#include "cu_base.h"
//#include "def_tag.h"
//#include "cu_mess.h"
//#include "cu_entry.h"
//#include "gestion_param.h"
#include "cu_term.h"
//#include "cu_param.h"

#include "Goal.h"

UC	connect_host_IP(PC number_phone,PC site);
UC	connect_host_GPRS(PC number_phone,PC site);

uchar	bitmap_avis[]       =	{	114,	52	,	133	,	65,	12,	224,	136,	3	};
#define CHECK(CND,LBL) {if(!(CND)){goto LBL;}}
static const ST_IMAGE na_xImage240 =
{
	{GL_COLOR_BLACK, TRUE, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_MEDIUM}, GL_DIRECTION_NONE, {0, 0, 0, 0, GL_COLOR_RED}, {20, 20, 20, 20} },
	{TRUE, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_MEDIUM}, GL_DIRECTION_NONE, {1, 1, 1, 1, GL_COLOR_BLACK}, {2, 1, 2, 1} }
};

T_GL_HWIDGET hPicture = NULL;
T_GL_HWIDGET xPicture = NULL;
long lRet0;
t_topstack *hTsk = NULL;
byte dum1;
tStatus usSta;
tEventList uiEve;
/****************************************************************************/
/****************************************************************************/
//static word usMainTaskNbr;
//static word SecondTask(void)    //larbi
//{
//	tStatus usSta;
//	usSta = Telium_SignalEvent(usMainTaskNbr, 15);
//	CHECK(usSta == OK, lblKO);
//
//	hPicture = GoalCreatePicture(APEMV_UI_GoalHandle()); // Create and clear screen
//	GL_Widget_SetBackColor(hPicture, GL_COLOR_WHITE);
//	GoalDspPicture(hPicture, "", "file://flash/HOST/tresor_carte.gif",
//			&na_xImage240, (GL_TIME_SECOND) * 5, true); //GL_TIME_INFINITE
//	GoalDestroyPicture(&hPicture);
//
//	//memset(&hPicture ,NULL);
//
//	xPicture = GoalCreatePicture(APEMV_UI_GoalHandle()); // Create and clear screen
//	GL_Widget_SetBackColor(xPicture, GL_COLOR_WHITE);
//	GoalDspPicture(xPicture, "Autorisation en cours","file://flash/HOST/Autorisation_encours.png",   //Pub1.PNG
//			&na_xImage240,GL_TIME_INFINITE, true);
//	GoalDestroyPicture(&xPicture);
//
//	lblKO: return 0;
//}


static word usMainTaskNbr;
static bool OnTimeout(T_GL_HMESSAGE message) {
	// Here we force the value to 0 to release the application of the loop on @ref GL_Window_Dispatch,
	// this assignment is an example and may be replaced by a more complete treatment
	GL_Message_SetResult(message, 0);
	// Stop the timer
	GL_Timer_Stop(GL_Message_GetWidget(message));
	// Here we force the value to 0 to release the application of the loop on GL_Window_Dispatch,
	// this assignment is an example and may be replaced by a more complete treatment
	return true;
}

static word SecondTask(void)    //larbi
{
	tStatus usSta;
	usSta = Telium_SignalEvent(usMainTaskNbr, 15);
	CHECK(usSta == OK, lblKO);

	//hPicture = GoalCreatePicture(APEMV_UI_GoalHandle()); // Create and clear screen
	//GL_Widget_SetBackColor(hPicture, GL_COLOR_WHITE);
//	GoalDspPicture(hPicture,"","file://flash/HOST/tresor_carte.gif", &na_xImage240,(GL_TIME_SECOND) * 5, true);//GL_TIME_INFINITE
//	GoalDestroyPicture(&hPicture);

/*	T_GL_HWIDGET window;
	T_GL_HWIDGET picture;
	T_GL_HWIDGET timer;

	window = GL_Window_Create(APEMV_UI_GoalHandle());
	timer = GL_Timer_Create(window);
	// Creating a blank window

	// Reduces the size of window
	GL_Widget_SetSize(window, 92, 92, GL_UNIT_PERCENT);
	// Creating a picture
	picture = GL_Picture_Create(window);
	// Set the image source
	GL_Widget_SetSource(picture, "file://flash/HOST/Pub1.PNG");
	//GL_Window_MainLoop(window);

	GL_Timer_SetInterval(timer, 5000);
	// Registering a callback. This callback will be called every second
	GL_Widget_RegisterCallback(timer, GL_EVENT_TIMER_OUT, OnTimeout);
	// Here we loop indefinitely. To break this loop you must set the result of window to 0 (GL_Message_SetResult or GL_Window_SetResult),
	// which was made earlier in the call back OnClick
	GL_Window_MainLoop(window);
	// Destruction of the window. Destruction frees all allocated memory,
	// all the widgets attached to the window and suppress the window on the screen.
	// This call is crucial, if it forgotten you get a memory leak.
	GL_Widget_Destroy(picture);
	GL_Widget_Destroy(window);
*/

//	GoalDspPicture(hPicture,"","file://flash/HOST/Pub1.PNG", &na_xImage240_1,(GL_TIME_SECOND) * 5, true);//GL_TIME_INFINITE
//	GoalDestroyPicture(&hPicture);
	//memset(&hPicture ,NULL);

//	xPicture = GoalCreatePicture(APEMV_UI_GoalHandle()); // Create and clear screen
//	GL_Widget_SetBackColor(xPicture, GL_COLOR_WHITE);
//	GoalDspPicture(xPicture, "Autorisation en cours","file://flash/HOST/Autorisation_encours.png", &na_xImage240,
//			GL_TIME_INFINITE, true);
//	GoalDestroyPicture(&xPicture);

    MessageWithoutConfirmation("Autorisation en cours","file://flash/HOST/Autorisation_encours.png");

	lblKO: return 0;
}




uchar	Mess39Screen(PC ch39)
{
	uchar	l_c_ch39[4];
	long	l_l_ch39;


	memcpy(l_c_ch39,ch39,3);l_c_ch39[3] =0;

	l_l_ch39 = atol((PC)l_c_ch39);

	switch(l_l_ch39)
	{
	case 911:
	case 912:
	case 992:
		CUMORE_CMI_AFFICHE_MESSAGE(170,138,1000);
		//message_communication(170,138,200);//time out
		break;
	case 116:
	case 206:
	case 106:
		CUMORE_CMI_AFFICHE_MESSAGE(171,138,1000);
		//message_communication(171,138,200);//TRANSACTION REFUSEE
		break;
	case 117:
		CUMORE_CMI_AFFICHE_MESSAGE(172,149,1000);
		//message_communication(172,149,200);//PIN INCORRECT
		break;
	case 183:
		CUMORE_CMI_AFFICHE_MESSAGE(141,16,1000);
		//message_communication(141,16,200);//CVV INVALID
		break;
	case 184:
		CUMORE_CMI_AFFICHE_MESSAGE(173,16,1000);
		//message_communication(173,16,200);//DATE INVALID
		break;
	case 482:
		CUMORE_CMI_AFFICHE_MESSAGE(7,9,1000);
		//message_communication(7,9,200);//Transaction dejà annule
		break;
	default:
		break;
	}
	return 0;
}
/****************************************************************************
*                                                                           *
*      SOUS-PROGRAMME :  DemAutor8583                                       *
*                                                                           *
*****************************************************************************
*                                                                           *
*      FONCTION :    traitement de la demande d'autorisation                *
*                                                                           *
*      ENTREE   :  1 si exite une transa pour l'avis de reconsiliation      *
*                                                                           *
*      SORTIE   :  status                                                   *
*                                                                           *
****************************************************************************/
uchar   DemAutor8583(uchar* update_param,uchar* update_date)
{
	//	DRV_OUT		*ptKeyboard;
	uchar		phone[17],site[16];
	uchar		longMess[3];
	uchar		num_aut[7];
	uchar		status;
	uchar		emission;
	uchar		reception;
	Ushort		Essai_cnx = 0;


	/*****************************
			memcpy(R_Transac8583.champs39,"000", 3);
			R_Transac8583.l_champs55 = 4;
			memcpy(R_Transac8583.champs55,"\x8A\x02\x30\x30",R_Transac8583.l_champs55);
			memcpy(R_Transac8583.champs38,"123ABC",6);
			status = OK;
			return status ;
	*****************************/

	//if(FicPara.Etat_TLS==0)		//Kamalssl
	{
		memset(longMess,0,3);
		memset(num_aut,0,7);
		do{
			memset(phone,0x00,17);
			memset(site,0x00,16);
			if(Essai_cnx%2)
			{
				memcpy(phone,FicPara.Tel_autor_sec,16);
				memcpy(site,FicPara.X25_autor_sec,15);
			}
			else
			{
				memcpy(phone,FicPara.Tel_autor_pri,16);
				memcpy(site,FicPara.X25_autor_pri,15);
			}
			if(FicPara.g_type_connexion == 'M')
				;//Fermer17status = connect_host_GSM((PC)phone,(PC)site);
			if(FicPara.g_type_connexion == 'R')
				;//Fermer17status = connect_host((PC)phone,(PC)site);
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
			}
			Essai_cnx++;
		}while((Essai_cnx < 3)&&(status != OK));

		if(status != OK)
		{
			strncpy(StructInfoCaisse[1].CRPIN,"880",3);//Cless_ 880
			return CONNEXION_KO;
		}

		//FLAG_Ref_SSL=1;		//Kamalssl
	}


	if	(
		(StructInfoPayment.CodeFunction == VOIDCODE)||
		(StructInfoPayment.CodeFunction == VOID_PREAUTO_TOTAL_CODE)||
		(StructInfoPayment.CodeFunction == VOID_PREAUTO_PARTIEL_CODE)
		)
			CUMORE_CMI_AFFICHE_MESSAGE(184,138,1000);
	else{

		//CUMORE_CMI_AFFICHE_MESSAGE(120,138,1000);
		MessageWithoutConfirmation("Demande autorisation", "file://flash/HOST/Autorisation_encours.png");
	}

	memcpy(E_Transac8583.e_type_mess,StructInfoPayment.Message_TypeisoEmi ,4);
	memcpy(E_Transac8583.r_type_mess,StructInfoPayment.Message_TypeisoRec ,4);
	memcpy(E_Transac8583.champs3,StructInfoPayment.Proc_Code,6);
	Format(E_Transac8583.e_type_mess,StructInfoPayment.Bit_Map,0);
	// l'emission doit prendre en considération le header (***) non pour void

	emission=Emission(E_Transac8583.e_type_mess,StructInfoPayment.Bit_Map);

	if (emission != OK)
	{
		hang_up(typehangup,g_trace);
		return EMISSION_KO ;
	}

	FicPara.AutoReversalOK =  0;
	PutAllParam();

//	usMainTaskNbr = Telium_CurrentTask();
//	hTsk = Telium_Fork(SecondTask, &dum1, 0);
//	//CHECK(hTsk!=NULL, lblKO);
//
//	usSta = Telium_WaitEvents(0x00008000, 0, &uiEve);
//	//CHECK(usSta==OK, lblKO);
//	usSta = Telium_ClearEvents(0x00008000);
//	//CHECK(usSta==OK, lblKO);
//	lRet0 = TimerStart(0, 100 * 1000);
	reception=Reception();
	status=OK;
	Mess39Screen((PC)R_Transac8583.champs39);
	long duree;
	char xtime[20];
	if(reception	!=	OK)
	{
		//Telium_Kill(hTsk, "");//larbi
		hang_up(typehangup,g_trace);
		AnnulationAutomatique();

		strncpy(StructInfoCaisse[1].CRPIN,"911",3);//Cless_ 880
		//TimerStop(0);
		return reception ;
	}
	else if(reception == OK)
	{
		if(memcmp(R_Transac8583.champs39,"909", 3)==0)
		{
			//Telium_Kill(hTsk, "");
			hang_up(typehangup,g_trace);
			AnnulationAutomatique();

			//TimerStop(0);
			return KO ;
		}
		if(
			(memcmp(R_Transac8583.champs39,"000", 3)==0)||
			(memcmp(R_Transac8583.champs39,"001", 3)==0)||
			(memcmp(R_Transac8583.champs39,"003", 3)==0)||
			(memcmp(R_Transac8583.champs39,"007", 3)==0)
			)
		{
			//while (TimerGet(0) > 95000);
			status = OK;
		}
		else if (
			(memcmp(R_Transac8583.champs39,"107", 3)==0)||
			(memcmp(R_Transac8583.champs39,"108", 3)==0)
			)
		{
			//message exterieur
			hang_up(typehangup,g_trace);
			status = REFERRAL;
		}
		else if(
			(memcmp(R_Transac8583.champs39,"200", 3)==0)||
			(memcmp(R_Transac8583.champs39,"201", 3)==0)||
			(memcmp(R_Transac8583.champs39,"202", 3)==0)||
			(memcmp(R_Transac8583.champs39,"204", 3)==0)||
			(memcmp(R_Transac8583.champs39,"205", 3)==0)||
			(memcmp(R_Transac8583.champs39,"207", 3)==0)||
			(memcmp(R_Transac8583.champs39,"208", 3)==0)||
			(memcmp(R_Transac8583.champs39,"209", 3)==0)||
			(memcmp(R_Transac8583.champs39,"210", 3)==0)||
			(memcmp(R_Transac8583.champs39,"299", 3)==0)
			)
		{
			//message exterieur
			//Os__hang_up();
			status = CAPTURE_CARTE;
			memcpy(FicPara.Last_Card_Rejected,&StructInfoPayment.Holder_Card[1],StructInfoPayment.Holder_Card[0]);
		}
		else if (
			(memcmp(R_Transac8583.champs39,"106", 3)==0)||
			(memcmp(R_Transac8583.champs39,"206", 3)==0)
			)
		{
			CUMORE_CMI_AFFICHE_MESSAGE(34,138,1000);
			//Uti__display1_clr2("  NOMBRE ESSAIS");
            //Uti__display2("   PIN DEPASSE");
            Os__xdelay(500);
			hang_up(typehangup,g_trace);
            status = NON_AUTOR;
			memcpy(FicPara.Last_Card_Rejected,&StructInfoPayment.Holder_Card[1],StructInfoPayment.Holder_Card[0]);
		}
		else if(memcmp(R_Transac8583.champs39,"117", 3)==0)
		{
			//message exterieur
			status = PIN_FAUX;
			memcpy(FicPara.Last_Card_Rejected,&StructInfoPayment.Holder_Card[1],StructInfoPayment.Holder_Card[0]);
		}
		else if(memcmp(R_Transac8583.champs39,"480", 3)==0)
		{
			if	(
				(StructInfoPayment.CodeFunction == VOIDCODE)||
				(StructInfoPayment.CodeFunction == VOID_PREAUTO_TOTAL_CODE)||
				(StructInfoPayment.CodeFunction == VOID_PREAUTO_PARTIEL_CODE)
				)

				status = OK;
			else
			{
				status = NON_AUTOR;
				memcpy(FicPara.Last_Card_Rejected,&StructInfoPayment.Holder_Card[1],StructInfoPayment.Holder_Card[0]);
			}
		}
		else if(memcmp(R_Transac8583.champs39,"481", 3)==0)
		{
			if	(
				(StructInfoPayment.CodeFunction == VOIDCODE)||
				(StructInfoPayment.CodeFunction == VOID_PREAUTO_TOTAL_CODE)||
				(StructInfoPayment.CodeFunction == VOID_PREAUTO_PARTIEL_CODE)
				)
			{
				CUMORE_CMI_AFFICHE_MESSAGE(39,133,1000);
				//Uti__display1_clr2("  REDRESSEMENT");
 				//Uti__display2("   INCORRECT");
				Os__xdelay(500);
			}
			status = NON_AUTOR;
			memcpy(FicPara.Last_Card_Rejected,&StructInfoPayment.Holder_Card[1],StructInfoPayment.Holder_Card[0]);
		}
		else if(memcmp(R_Transac8583.champs39,"482", 3)==0)
		{
			if	(
				(StructInfoPayment.CodeFunction == VOIDCODE)||
				(StructInfoPayment.CodeFunction == VOID_PREAUTO_TOTAL_CODE)||
				(StructInfoPayment.CodeFunction == VOID_PREAUTO_PARTIEL_CODE)
				)
			{
				CUMORE_CMI_AFFICHE_MESSAGE(7,9,1000);
				//Uti__display1_clr2("   TRANSACTION");
				//Uti__display2("  DEJA ANNULEE");
				Os__xdelay(500);
			}
			hang_up(typehangup,g_trace);//Mouna310305
			status = NON_AUTOR;
			memcpy(FicPara.Last_Card_Rejected,&StructInfoPayment.Holder_Card[1],StructInfoPayment.Holder_Card[0]);
		}
		else
		{
			//Os__hang_up();
			status = NON_AUTOR;
			memcpy(FicPara.Last_Card_Rejected,&StructInfoPayment.Holder_Card[1],StructInfoPayment.Holder_Card[0]);
		}
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

	// récupérer la date serveur
	memcpy(update_date,R_Transac8583.champs7,10);
	hang_up(typehangup,g_trace);
//	Telium_Kill(hTsk, "");
//	TimerStop(0);
	return status ;
}

void ExploiField7()
{
	uchar date_serveur[13];
	uchar	date_client[13];
	DATE date;

	memset(date_serveur,0,13);
	memset(date_client,0,13);

	memcpy(date_serveur	  ,R_Transac8583.champs7,10);
	memcpy(date_serveur+10,"00"					  ,2);

	Telium_Read_date(&date);
	sprintf ((char*)date_client,
		"%.2s%.2s%.2s%.2s%.2s%.2s",
		date.year,
		date.month,
		date.day,
		date.hour,
		date.minute,
		date.second);

	if(memcmp(date_serveur,date_client,10))
	{
		//memcpy(&date,date_serveur,10);
		memcpy(date.year	,date_serveur+0,2);
		memcpy(date.month	,date_serveur+2,2);
		memcpy(date.day		,date_serveur+4,2);
		memcpy(date.hour	,date_serveur+6,2);
		memcpy(date.minute	,date_serveur+8,2);
		memcpy(date.second	,date_serveur+10,2);
		//Fermer17Telium_write_date (&date);
	}
}

uchar	gest_param()
{
	uchar		status=OK;
	//affectation de la date serveur
	ExploiField7();

	PutAllParam();
	PutAllTra();
	PutAllPre();
//	PutAllLop();
	if((FicPara.Update_pos_param!='0')&&(status==OK))
	{
		CUMORE_CMI_AFFICHE_MESSAGE(34,138,1000);
		//Uti__display1_clr2("TELEPARAMETRAGE");
		Os__display(1,0,"POS PARAM.");
		status = Transmission_file(FILE_PARAM);
		if(status==OK)
			Os__saved_set ((uchar *)&FicPara.Update_pos_param, '0', 1);
	}
	if((FicPara.Update_mk!='0')&&(status==OK))
	{
		CUMORE_CMI_AFFICHE_MESSAGE(34,138,1000);
		//Uti__display1_clr2("TELEPARAMETRAGE");
		Os__display(1,0,"KEY");
		status = Transmission_file(FILE_KEY);
		if(status==OK)
			Os__saved_set ((uchar *)&FicPara.Update_mk, '0', 1);
	}
	if((FicPara.Update_mac!='0')&&(status==OK))
	{
		CUMORE_CMI_AFFICHE_MESSAGE(34,138,1000);
		//Uti__display1_clr2("TELEPARAMETRAGE");
		Os__display(1,0,"MAC KEY");
		status = Transmission_file(FILE_MACKEY);
		if(status==OK)
			Os__saved_set ((uchar *)&FicPara.Update_mac, '0', 1);
	}
	if((FicPara.Update_bin!='0')&&(status==OK))
	{
		CUMORE_CMI_AFFICHE_MESSAGE(34,138,1000);
		//Uti__display1_clr2("TELEPARAMETRAGE");
		Os__display(1,0,"BIN");
		status = Transmission_file(FILE_BIN);
		if(status==OK)
		{
			CUMORE_CMI_AFFICHE_MESSAGE(34,138,1000);
			//Uti__display1_clr2("TELEPARAMETRAGE");
			Os__display(1,0,"PREFIXE");
			status = Transmission_file(FILE_PREFIXES);
		}
		if(status==OK)
			Os__saved_set ((uchar *)&FicPara.Update_bin, '0', 1);
	}
	if((FicPara.Update_compt_dom!='0')&&(status==OK))
	{
		CUMORE_CMI_AFFICHE_MESSAGE(34,138,1000);
		//Uti__display1_clr2("TELEPARAMETRAGE");
		Os__display(1,0,"COMPTE DOM.");
		status = Transmission_file(FILE_COMPTE);
		if(status==OK)
			Os__saved_set ((uchar *)&FicPara.Update_compt_dom, '0', 1);
	}
   /////////////////////////////////////////////////
	if((FicPara.Update_param_Emv!='0')&&(status==OK))
	{
		CUMORE_CMI_AFFICHE_MESSAGE(34,138,1000);
		//Uti__display1_clr2("TELEPARAMETRAGE");
		Os__display(1,0,"EMV AID");
		status = Transmission_file(FILE_EMV_AID);
		if(status==OK)
		{
			CUMORE_CMI_AFFICHE_MESSAGE(34,138,1000);
			//Uti__display1_clr2("TELEPARAMETRAGE");
			Os__display(1,0,"EMV KEY");
			status = Transmission_file(FILE_EMV_KEYS);
		}
		if(status==OK)
		{
			CUMORE_CMI_AFFICHE_MESSAGE(34,138,1000);
			//Uti__display1_clr2("TELEPARAMETRAGE");
			Os__display(1,0,"EMV TAC");
			status = Transmission_file(FILE_EMV_TAC);
		}
		if(status==OK)
		{
			CUMORE_CMI_AFFICHE_MESSAGE(34,138,1000);
			//Uti__display1_clr2("TELEPARAMETRAGE");
			Os__display(1,0,"EMV TDOL");
			status = Transmission_file(FILE_EMV_TDOL);
		}
		if(status==OK)
		{
			CUMORE_CMI_AFFICHE_MESSAGE(34,138,1000);
			//Uti__display1_clr2("TELEPARAMETRAGE");
			Os__display(1,0,"EMV DDOL");
			status = Transmission_file(FILE_EMV_DDOL);
		}
		if(status==OK)
		{
			CUMORE_CMI_AFFICHE_MESSAGE(34,138,1000);
			//Uti__display1_clr2("TELEPARAMETRAGE");
			Os__display(1,0,"EMV RAND CALL");
			status = Transmission_file(FILE_EMV_RAND);
		}
		if(status==OK)
		{
			CUMORE_CMI_AFFICHE_MESSAGE(34,138,1000);
			//Uti__display1_clr2("TELEPARAMETRAGE");
			Os__display(1,0,"EMV TCC");
			status = Transmission_file(FILE_EMV_TCC);
		}
		if(status==OK)
		{
			Os__saved_set ((uchar *)&FicPara.Update_param_Emv, '0', 1);
			//Fermer17CUPAR_InitDefFiles_Cmi ();//060605
		}
	}

	if(status==OK)
		Os__saved_set(&FicPara.status,OK,1);

	if((FicPara.Update_LN!='0')&&(status==OK))
	{
//		status = ListeNoire(0,0);
//		if(status==OK)
//			Os__saved_set ((uchar *)&FicPara.Update_LN, '0', 1);
	}
	hang_up(typehangup,g_trace);
	return OK ;
}


/****************************************************************************
*                                                                           *
*      SOUS-PROGRAMME :  Dem_avis                                           *
*                                                                           *
*****************************************************************************
*                                                                           *
*      FONCTION :    Demande d'avis financiére                              *
*                                                                           *
*      ENTREE   :  Rien                                                     *
*                                                                           *
*      SORTIE   :  status                                                   *
*                                                                           *
****************************************************************************/
uchar	Dem_avis()
{
	uchar	bitmap[8];
	uchar	emission;
	uchar	reception;

	memcpy(bitmap,bitmap_avis,8);
	if(StrLineTra.Entry_Mode==TRS_EMV)
	{
		bitset(bitmap,23);
		bitset(bitmap,55);
	}
	else if(StrLineTra.Entry_Mode==TRS_SCT)
	{
		bitset(bitmap,23);
		bitset(bitmap,55);
	}

	memcpy(E_Transac8583.e_type_mess,"1220" ,4);//précison du type de message
	memcpy(E_Transac8583.r_type_mess,"1230" ,4);
	memcpy(StructInfoPayment.Bit_Map,bitmap,8);
	Format(E_Transac8583.e_type_mess,bitmap, 0);
	// l'emission doit prendre en considération le header (***) non pour void
	emission=Emission(E_Transac8583.e_type_mess,bitmap);
	if (emission != OK)
	{
		if(g_trace)Os__xprint("Emission ko");
		return KO ;
	}
	reception=Reception();
	if(reception	!=	OK)
	{
		// Gestion de numéro de séquence
		if(g_trace)Os__xprint("Reception ko");
		return KO ;
	}
	else if(reception == OK)
	{
		if(memcmp(R_Transac8583.champs39,"000", 3))
		{
			if(g_trace)Os__xprint("ch39 ko");
			return KO ;
		}
	}
	return OK ;
}
int AnnulationAutomatique()
{
	uchar	bmp1420[]	        =	{	122,	52	,	133	,  129,	44,	192,	201,	3	};

	uchar		phone[17],site[16];
	uchar		longMess[3];
	uchar		num_aut[7];
	uchar		status;
	Ushort		Essai_cnx = 0;



	if(FicPara.AutoReversalOK == 4)
		return 0;

	if(FicPara.AutoReversalOK == 3)
		return 0;

	if(FicPara.AutoReversalOK == 2)
		return 0;

	FicPara.AutoReversalOK =  1;
	memcpy(FicPara.CodeReponnseAvantAutoReversal,R_Transac8583.champs39,3);
	PutAllParam();



	Telium_Ttestall(0,200);




	if(FicPara.Etat_TLS==0)		//Kamalssl
	{
		memset(longMess,0,3);
		memset(num_aut,0,7);
		do{
			memset(phone,0x00,17);
			memset(site,0x00,16);
			if(Essai_cnx%2)
			{
				memcpy(phone,FicPara.Tel_autor_sec,16);
				memcpy(site,FicPara.X25_autor_sec,15);
			}
			else
			{
				memcpy(phone,FicPara.Tel_autor_pri,16);
				memcpy(site,FicPara.X25_autor_pri,15);
			}
			if(FicPara.g_type_connexion == 'M')
				;//Fermer17status = connect_host_GSM((PC)phone,(PC)site);
			if(FicPara.g_type_connexion == 'R')
				;//Fermer17status = connect_host((PC)phone,(PC)site);
			if(FicPara.g_type_connexion == 'G')
				status = connect_host_GPRS((PC)phone,(PC)site);
			if(FicPara.g_type_connexion == 'I')
				status = connect_host_IP((PC)phone,(PC)site);
			if(status != OK)
			{
				hang_up(0,g_trace);
			}
			Essai_cnx++;
		}while((Essai_cnx < 3)&&(status != OK));

		if(status != OK)
		{
			return CONNEXION_KO;
		}
	}

	CUMORE_CMI_AFFICHE_MESSAGE(184,138,1000);//Demande Annul

	StructInfoPayment.Entry_Pin = 0;
	//StructInfoPaymentEntry_PinForcee = 0;
	StructInfoPayment.CodeFunction = VOIDCODE;
	memcpy(StructInfoPayment.Message_TypeisoEmi,"1420",4);
	memcpy(StructInfoPayment.Message_TypeisoRec,"1430",4);
	memcpy(StructInfoPayment.Bit_Map,bmp1420,8);

	memcpy(E_Transac8583.e_type_mess,StructInfoPayment.Message_TypeisoEmi ,4);
	memcpy(E_Transac8583.r_type_mess,StructInfoPayment.Message_TypeisoRec ,4);

	memcpy(E_Transac8583.champs3,StructInfoPayment.Proc_Code,6);
	Format(E_Transac8583.e_type_mess,StructInfoPayment.Bit_Map,0);

	memcpy(E_Transac8583.champs56,"1200",4);
	memcpy(&E_Transac8583.champs56[4],StructInfoPayment.STAN,6);
	memcpy(&E_Transac8583.champs56[10],StructInfoPayment.Transaction_DateTime,12);
	memcpy(&E_Transac8583.champs56[22],"11111111111",11);
	E_Transac8583.l_champs56 = 33;


	if(Emission(E_Transac8583.e_type_mess,StructInfoPayment.Bit_Map) == OK)
	{
		hang_up(0,g_trace);
		DisconnectSSL();
	}
	FicPara.AutoReversalOK =  2;
	PutAllParam();

	return 0;

}
