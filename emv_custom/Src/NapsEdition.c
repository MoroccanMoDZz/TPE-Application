/* ======================================================================== */
/* Include Files                                                            */
/* ======================================================================== */

#include "SDK_Naps.h"

#include "cu_term.h"
#include "Goal.h"
#include <stdlib.h>
#include <string.h>

#define CHECK(CND,LBL) {if(!(CND)){goto LBL;}}
UC		Buffer[1500];
int		ii;

//extern MSG_CAISSE  StructInfoCaisse[2];
extern	int	GlLine;
extern	int	DebutLine;
#define	C_JUSTIFY1	1

L	Os__xprintaa(PC message);

static const ST_BORDER xBorder = {0, 0, 0, 0, GL_COLOR_BLACK};

static const ST_PRN_LINE xPrinter =
{
	GL_ALIGN_CENTER, GL_ALIGN_CENTER, FALSE, 0, {0, 0, 0, 0}, {0, 0, 0, 0, GL_COLOR_BLACK}, {1, 0, 1, 0}, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_XXSMALL}
};

void printLogo(uchar source){

	T_GL_HWIDGET xDocument=NULL;
	byte p;

	xDocument = GoalCreateDocument(APEMV_UI_GoalHandle(), GL_ENCODING_UTF8);
	p=0;

	//GoalPrnPicture(xDocument, p++, "file://flash/HOST/LOGO.png", &xBorder);
	// Copie Client ...
	if(source == CLIENT || source == COMMERCANT ){
		//GoalPrnPicture(xDocument, p++, "file://flash/HOST/BOSSFORPRINT.png", &xBorder);
		GoalPrnPicture(xDocument, p++, "file://flash/HOST/logomono.bmp", &xBorder);
	}
	else if(source == REFUSED)
		GoalPrnPicture(xDocument, p++, "file://flash/HOST/refuseMono.bmp", &xBorder);
	else if(source == ABORTED)
			GoalPrnPicture(xDocument, p++, "file://flash/HOST/DevenezLogoMono.bmp", &xBorder);
	else if(source == OR_FACTURE52)
				GoalPrnPicture(xDocument, p++, "file://flash/HOST/logoORmono.bmp", &xBorder);
	else
		GoalPrnPicture(xDocument, p++, "file://flash/HOST/valideMono.bmp", &xBorder);


	GoalPrnDocument(xDocument);

	if (xDocument)
		GoalDestroyDocument(&xDocument);





//	CUPRINT_Open_Printer();
//
//
//	CULOGO_Print_Ingenico();
//	CUPRINT_Boundary_Line(0, 24);
//	CUPRINT_Blanks_Lines(1);
//
//	CUPRINT_Close_Printer();


}

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


//uchar imp_buffer(char* buffer,int n_car)
//{
//	Telium_File_t *printer1;
//	STATUS	st;
//	int c;
//	T_GL_HWIDGET hDocument = NULL;
//	ST_PRN_LINE xLine;
//	byte p; // Printer line index
//	T_GL_HGRAPHIC_LIB hGoal = NULL;
//
//
//	OpenPeripherals();   // Open standard peripherals
//	GetPpdDisplay();     // Get pinpad type
//
//
//	p = 0;
//	hDocument = GoalCreateDocument(APEMV_UI_GoalHandle(),GL_ENCODING_ISO_8859_1);
//	xLine = xPrinter;
//
//
//	printer1 = Telium_Fopen("PRINTER","w-");
//	//if ( printer1 != NULL )
//	//{
//		for(c =0;c<n_car;c++)
//		{
//			//Telium_Fprintf(printer1,"%c",buffer[c]);
//			 GoalPrnLine(hDocument, p++, buffer[c], &xLine);
//			//Telium_Status(printer1 ,&st);
////			if(
////				( st & PAPER_OUT)
////	//			||( st & PRINT_END)
////				||( st & PRINT_ERROR)
////			)
////			{
////				Telium_Fclose(printer1);
////				//avertissement
////				printf("\x1B"" PAPIER ABSENT\n");
////				printf (" NO MORE PAPER");
////				switch (Telium_Ttestall( KEYBOARD , 500 ))
////				{
////				case KEYBOARD :
////					break;
////				case TIMEOUT :
////					break;
////				}
////
////				return KO;
////			}
//		}
//		GoalPrnDocument(hDocument);
//
//
//
//		if(n_car)
//		{
//			Telium_Fprintf(printer1,"\n");
//			Telium_Ttestall ( PRINTER , 300 );
//		}
//		Telium_Fclose(printer1);
//
//		GoalDestroyDocument(&hDocument);
//	}
//	return OK;
//}


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
uchar	Calcul_total(uchar ind_rec){
	// le 1er dim pour type carte,2?me di pour la devise,3?me dim pour type transac
//	PC tab;
//    sprintf(tab,"%c",ind_rec);
//	Os__xprint(tab);
//	Os__xprinta(tab);

	struct Totaux {

		long Total[NB_MAX_INS_REG][NB_MAX_DEV_ACC][9];
		ulong Nbre_tra[NB_MAX_INS_REG][NB_MAX_DEV_ACC][9];
	} StrReport;

	char	pPtr[600];
	ulong Nbre_tra;
	lignetra transac;
	uchar type_carte, trouve_type;
	uchar devise = 0, k = 0;
	ushort nombre_tra;
	int saisi_man = 0;
	uchar montant[13];
	uchar affiche_mont[14];
	uchar monnai_alpha[4];
	uchar c_nbr_tra[5];
	long l_total = 0;
	uchar Exp;
	int i = 0;
	uchar			status;

	long TotRECH = 0;
	ushort nbr_tra_RECH = 0;
	long TotDECH = 0;
	ushort nbr_tra_DECH = 0;
	long TotCASHIN = 0;
	ushort nbr_tra_CASHIN = 0;
	long TotCASHOUT = 0;
	ushort nbr_tra_CASHOUT = 0;
	long TotCARCART = 0;
	ushort nbr_tra_CARCART = 0;
	char contenu[5] = { '\0' };
	int res = 0;

	int iHeader, iFooter;
	int iRet;
	T_GL_HWIDGET hDocument = NULL;
	ST_PRN_LINE xLine;
	byte p; // Printer line index
	T_GL_HGRAPHIC_LIB hGoal = NULL;
	//int iRet;
	char var[100];
	ushort var2[10];
	uchar var3[10];

	p = 0;                                            // Create document
	hDocument = GoalCreateDocument(APEMV_UI_GoalHandle(),GL_ENCODING_ISO_8859_1);
	CHECK(hDocument != NULL, lblKO);



	memset(&StrReport, 0, sizeof(StrReport));
	Nbre_tra = 0;
	memset(montant, 0, 13);
	memset(affiche_mont, 0, 14);
	memset(monnai_alpha, 0, 4);
	memset(pPtr, 0x00, sizeof(pPtr));
	nombre_tra = FicTra.nombre_tra;

	if (nombre_tra == 0)
		return KO ;

	i = 0;
	while (i < nombre_tra) {
		read_trans_index(i, &transac);

		type_carte = transac.Type_carte;

		for (k = 0; k < NB_MAX_DEV_ACC; k++)           //l'indice de la 2?me dim
				{
			if (!memcmp(Fic_Devise_Acc.TabDevise[k].code_num,
					transac.NumCurrency, 3)) {
				devise = k;
				break;
			}
		}
		//ind_rec = 2;
		if (transac.CodeFunction != VOIDCODE) {

			if (transac.CodeFunction == REFUNDCODE) {
				StrReport.Total[type_carte][devise][CREDIT] +=
						transac.Transaction_Amount;
				StrReport.Nbre_tra[type_carte][devise][CREDIT] += 1;
			} else {
				if (ind_rec == 2)            //TOTAUX
						{
					if (transac.CodeFunction == PAIEMENT_POURBOIRE)    //POURBOIRE
						StrReport.Total[type_carte][devise][_ONLINE] += transac.Transaction_Amount+ transac.MontantPourboireInterchange;
					else
						StrReport.Total[type_carte][devise][_ONLINE] += transac.Transaction_Amount;

					StrReport.Nbre_tra[type_carte][devise][_ONLINE] += 1;
				} else {
					if (!transac.Flag_Dem_Aut)        //transac.Flag_Dem_Aut = 0
					{
						if (transac.CodeFunction == PAIEMENT_POURBOIRE)    //POURBOIRE
							StrReport.Total[type_carte][devise][_OFFLINE] +=
									transac.Transaction_Amount+ transac.MontantPourboireInterchange;

						else

							StrReport.Total[type_carte][devise][_OFFLINE] +=
								transac.Transaction_Amount;

						StrReport.Nbre_tra[type_carte][devise][_OFFLINE] += 1;
					} else {
						if (transac.CodeFunction == RECH_PREPAYE) { //par Hind SAHEL
							TotRECH += transac.Transaction_Amount;
							nbr_tra_RECH += 1;
						} else if (transac.CodeFunction == DECH_PREPAYE) {
							TotDECH += transac.Transaction_Amount;
							nbr_tra_DECH += 1;
						} else if (transac.CodeFunction == CAC_PREPAYE) {
							TotCARCART += transac.Transaction_Amount;
							nbr_tra_CARCART += 1;
						} else if (transac.CodeFunction == CASHIN_PREPAYE) {
							TotCASHIN += transac.Transaction_Amount;
							nbr_tra_CASHIN += 1;
						} else if (transac.CodeFunction == CASHOUT_PREPAYE) {
							TotCASHOUT += transac.Transaction_Amount;
							nbr_tra_CASHOUT += 1;
						} else {

							if (transac.CodeFunction == PAIEMENT_POURBOIRE)    //POURBOIRE
								StrReport.Total[type_carte][devise][_ONLINE] +=
										transac.Transaction_Amount+ transac.MontantPourboireInterchange;
							else
								StrReport.Total[type_carte][devise][_ONLINE] +=
									transac.Transaction_Amount;

							StrReport.Nbre_tra[type_carte][devise][_ONLINE] += 1;
						}
					}
				}
			}
		} else {
			if (transac.CodeFunction == PAIEMENT_POURBOIRE
				|| (transac.CodeFunction == VOIDCODE && transac.MontantPourboireInterchange != 0 )
				|| (transac.StatusAnnulAdjust && transac.MontantPourboireInterchange != 0 )//POURBOIRE
				)
				StrReport.Total[type_carte][devise][ANNUL_] +=
					transac.Transaction_Amount+ transac.MontantPourboireInterchange;
			else
				StrReport.Total[type_carte][devise][ANNUL_] +=
					transac.Transaction_Amount;

			StrReport.Nbre_tra[type_carte][devise][ANNUL_] += 1;
		}
		if (transac.Entry_Mode == TRS_MAN)                //Nbre saisie manuelle
			saisi_man += 1;

		Nbre_tra += 1;

		i++;
	}

//	res = get_env("#EMETTEUR", contenu, sizeof(contenu));
//	contenu[res] = '\0';

	//if (memcmp(contenu, "NAPS", 4) == 0) {


	    xLine = xPrinter;
		xLine.eBackAlign = GL_ALIGN_CENTER;
		xLine.eTextAlign = GL_ALIGN_CENTER;
		xLine.xFont.eScale = GL_SCALE_XLARGE;
		//xLine.xFont.eStyle = GL_FONT_STYLE_BOLD;
		sprintf(pPtr,"\nTransactions");
		iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
		CHECK(iRet >= 0, lblKO);
		sprintf(pPtr,"Prepay?es");
		iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
		CHECK(iRet >= 0, lblKO);



		//RECHARGE
		memset(montant, 0, 13);
		memset(affiche_mont, 0, 14);
		long_asc(montant, 12, (ulong *) &TotRECH);
		RemoveLeftZeros(montant);
		//Exp=Fic_Devise_Acc.TabDevise[k].Exposant-48;
		Exp = 2;
		FormatAmount(montant, affiche_mont, Exp);

		xLine.eBackAlign = GL_ALIGN_LEFT;
	    xLine.eTextAlign = GL_ALIGN_LEFT;
	    xLine.xFont.eScale = GL_SCALE_LARGE;

	    sprintf(Buffer, "  ");
	    iRet = GoalPrnLine(hDocument, p++, Buffer, &xLine);

		if (nbr_tra_RECH != 0) {
			sprintf(var, "%12s", affiche_mont);
			sprintf(pPtr, "%s",var);
			xLine.eBackAlign = GL_ALIGN_RIGHT;
			xLine.eTextAlign = GL_ALIGN_RIGHT;
			iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
			sprintf(var, ": %.3d", nbr_tra_RECH);
			sprintf(pPtr, "%*s", 18 + strlen(var), var);
		} else {

			sprintf(pPtr, "%s", "0.00");
			xLine.eBackAlign = GL_ALIGN_RIGHT;
			xLine.eTextAlign = GL_ALIGN_RIGHT;
			iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
			sprintf(var, ": 000");
			sprintf(pPtr, "%*s", 18 + strlen(var), var);
		}
		xLine.eBackAlign = GL_ALIGN_LEFT;
		xLine.eTextAlign = GL_ALIGN_LEFT;
		iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
		iRet = GoalPrnLine(hDocument, p++, "Rech", &xLine);
		CHECK(iRet >= 0, lblKO);

		//DECHARGE
		memset(montant, 0, 13);
		memset(affiche_mont, 0, 14);
		long_asc(montant, 12, (ulong *) &TotDECH);
		RemoveLeftZeros(montant);
		Exp = 2;
		FormatAmount(montant, affiche_mont, Exp);

		if (nbr_tra_DECH !=0){
			sprintf(var, "%12s", affiche_mont);
			sprintf(pPtr, "%s", var);
			xLine.eBackAlign = GL_ALIGN_RIGHT;
			xLine.eTextAlign = GL_ALIGN_RIGHT;
			iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
			sprintf(var, ": %.3d", nbr_tra_DECH);
			sprintf(pPtr, "%*s", 18 + strlen(var), var);
		} else {
			sprintf(pPtr, "%s", "0.00");
			xLine.eBackAlign = GL_ALIGN_RIGHT;
			xLine.eTextAlign = GL_ALIGN_RIGHT;
			iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
			sprintf(var, ": 000");
			sprintf(pPtr, "%*s", 18 + strlen(var), var);
		}
		xLine.eBackAlign = GL_ALIGN_LEFT;
		xLine.eTextAlign = GL_ALIGN_LEFT;
		iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
		iRet = GoalPrnLine(hDocument, p++, "Dech", &xLine);
		CHECK(iRet >= 0, lblKO);


		//CASHIN
		memset(montant, 0, 13);
		memset(affiche_mont, 0, 14);
		long_asc(montant, 12, (ulong *) &TotCASHIN);
		RemoveLeftZeros(montant);
		Exp = 2;
		FormatAmount(montant, affiche_mont, Exp);

		if (nbr_tra_CASHIN != 0){
			sprintf(var, "%12s", affiche_mont);
			sprintf(pPtr, "%s", var);
			xLine.eBackAlign = GL_ALIGN_RIGHT;
			xLine.eTextAlign = GL_ALIGN_RIGHT;
			iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
			sprintf(var, ": %.3d", nbr_tra_CASHIN);
			sprintf(pPtr, "%*s", 18 + strlen(var), var);
		} else {
			sprintf(pPtr, "%s", "0.00");
			xLine.eBackAlign = GL_ALIGN_RIGHT;
			xLine.eTextAlign = GL_ALIGN_RIGHT;
			iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
			sprintf(var, ": 000");
			sprintf(pPtr, "%*s", 18 + strlen(var), var);
		}
		xLine.eBackAlign = GL_ALIGN_LEFT;
		xLine.eTextAlign = GL_ALIGN_LEFT;
		iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
		iRet = GoalPrnLine(hDocument, p++, "CashIN", &xLine);
		CHECK(iRet >= 0, lblKO);


		//CASHOUT
		memset(montant, 0, 13);
		memset(affiche_mont, 0, 14);
		long_asc(montant, 12, (ulong *) &TotCASHOUT);
		RemoveLeftZeros(montant);
		Exp = 2;
		FormatAmount(montant, affiche_mont, Exp);

		if (nbr_tra_CASHOUT != 0) {
			sprintf(var, "%12s", affiche_mont);
			sprintf(pPtr, "%s",var);
			xLine.eBackAlign = GL_ALIGN_RIGHT;
			xLine.eTextAlign = GL_ALIGN_RIGHT;
			iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
			sprintf(var, ": %.3d", nbr_tra_CASHOUT);
			sprintf(pPtr, "%*s", 18 + strlen(var), var);
		} else {
			sprintf(pPtr, "%s", "0.00");
			xLine.eBackAlign = GL_ALIGN_RIGHT;
			xLine.eTextAlign = GL_ALIGN_RIGHT;
			iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
			sprintf(var, ": 000");
			sprintf(pPtr, "%*s", 18 + strlen(var), var);
		}
		xLine.eBackAlign = GL_ALIGN_LEFT;
		xLine.eTextAlign = GL_ALIGN_LEFT;
		iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
		iRet = GoalPrnLine(hDocument, p++, "CashOUT", &xLine);
		CHECK(iRet >= 0, lblKO);

		//CARTE A CARTE
		memset(montant, 0, 13);
		memset(affiche_mont, 0, 14);
		long_asc(montant, 12, (ulong *) &TotCARCART);
		RemoveLeftZeros(montant);
		Exp = 2;
		FormatAmount(montant, affiche_mont, Exp);

		if (nbr_tra_CASHOUT != 0) {
			sprintf(var, "%12s", affiche_mont);
			sprintf(pPtr, "%*s",  var);
			 xLine.eBackAlign = GL_ALIGN_RIGHT;
		     xLine.eTextAlign = GL_ALIGN_RIGHT;
			iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
			sprintf(var, ": %.3d", nbr_tra_CARCART);
			sprintf(pPtr, "%*s", 18 + strlen(var), var);
		} else {
			sprintf(pPtr, "%s", "0.00");
			xLine.eBackAlign = GL_ALIGN_RIGHT;
			xLine.eTextAlign = GL_ALIGN_RIGHT;
			iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
			sprintf(var, ": 000");
			sprintf(pPtr, "%*s", 18 + strlen(var), var);
		}
		 xLine.eBackAlign = GL_ALIGN_LEFT;
	     xLine.eTextAlign = GL_ALIGN_LEFT;
		iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
		iRet = GoalPrnLine(hDocument, p++, "TR C.C", &xLine);
		CHECK(iRet >= 0, lblKO);



	    xLine.eBackAlign = GL_ALIGN_CENTER;
	    xLine.eTextAlign = GL_ALIGN_CENTER;
	    xLine.xFont.eScale = GL_SCALE_XLARGE;
	    sprintf(pPtr, "\nTransactions");
		iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
		CHECK(iRet >= 0, lblKO);

		sprintf(pPtr, "Financi?res");
		iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
		CHECK(iRet >= 0, lblKO);

		xLine.eBackAlign = GL_ALIGN_LEFT;
	    xLine.eTextAlign = GL_ALIGN_LEFT;
	    xLine.xFont.eScale = GL_SCALE_LARGE;
		for (i = 0; i < NB_MAX_INS_REG; i++) {
			trouve_type = 0;
			for (k = 0; k < NB_MAX_DEV_ACC; k++) {
				if (StrReport.Total[i][k][_OFFLINE]
						+ StrReport.Total[i][k][_ONLINE]
						+ StrReport.Total[i][k][CREDIT]
						+ StrReport.Total[i][k][ANNUL_] != 0) {

					//if (memcmp(FicInsReg.Tab_Ins_reg[i].Libelle,"                ",16) != 0){
					if (trouve_type == 0) {
						sprintf(pPtr, "\n%s",FicInsReg.Tab_Ins_reg[i].Libelle);
						xLine.xFont.eStyle = GL_FONT_STYLE_BOLD;
						iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
						CHECK(iRet >= 0, lblKO);
						trouve_type = 1;
					}
					xLine.xFont.eStyle = GL_FONT_STYLE_NORMAL;
					if ((ind_rec == 0) || (ind_rec == 1)) {
						memcpy(monnai_alpha,
								Fic_Devise_Acc.TabDevise[k].code_alph, 3);
						sprintf(pPtr, "%.3s-", monnai_alpha);
						iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
						CHECK(iRet >= 0, lblKO);
						if (memcmp(monnai_alpha, "000", 3) != 0)


						//ONLINE
						memset(montant, 0, 13);
						memset(affiche_mont, 0, 14);
						memset(c_nbr_tra, 0, 4);
						long_asc(montant, 12, (ulong *) &(StrReport.Total[i][k][_ONLINE]));
						long_asc(c_nbr_tra, 3, (ulong *) &(StrReport.Nbre_tra[i][k][_ONLINE]));
						//Os__xprinta(c_nbr_tra);
						RemoveLeftZeros(montant);
						Exp = Fic_Devise_Acc.TabDevise[k].Exposant - 48;
						FormatAmount(montant, affiche_mont, Exp);

						sprintf(var, "%12s", affiche_mont);
						sprintf(pPtr, "%s", var);
						xLine.eBackAlign = GL_ALIGN_RIGHT;
						xLine.eTextAlign = GL_ALIGN_RIGHT;
						iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
						sprintf(var, ": %.3s", c_nbr_tra);
						sprintf(pPtr, "%*s", 18 + strlen(var), var);
						xLine.eBackAlign = GL_ALIGN_LEFT;
						xLine.eTextAlign = GL_ALIGN_LEFT;
						if (memcmp(c_nbr_tra, "000", 3) != 0) {
							//Os__xprinta("trace pr?autho1");
							iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
							iRet = GoalPrnLine(hDocument, p++, "Online", &xLine);
							CHECK(iRet >= 0, lblKO);
						} else {
							//Os__xprinta("trace pr?autho2");
							sprintf(pPtr, "%s", "0.00");
							xLine.eBackAlign = GL_ALIGN_RIGHT;
							xLine.eTextAlign = GL_ALIGN_RIGHT;
							iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
							sprintf(var, ": 000");
							sprintf(pPtr, "%*s", 18 + strlen(var), var);
							xLine.eBackAlign = GL_ALIGN_LEFT;
							xLine.eTextAlign = GL_ALIGN_LEFT;
							iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
							iRet = GoalPrnLine(hDocument, p++, "Online", &xLine);
							CHECK(iRet >= 0, lblKO);

						}

					    //OFFLINE
						memset(montant, 0, 13);
						memset(affiche_mont, 0, 14);
						memset(c_nbr_tra, 0, 4);
						long_asc(montant, 12,
								(ulong *) &(StrReport.Total[i][k][_OFFLINE]));
						long_asc(c_nbr_tra, 3,
								(ulong *) &(StrReport.Nbre_tra[i][k][_OFFLINE]));
						//Os__xprinta(c_nbr_tra);
						RemoveLeftZeros(montant);
						Exp = Fic_Devise_Acc.TabDevise[k].Exposant - 48;
						FormatAmount(montant, affiche_mont, Exp);

						sprintf(var, "%12s", affiche_mont);
						sprintf(pPtr, "%s", var);
						xLine.eBackAlign = GL_ALIGN_RIGHT;
						xLine.eTextAlign = GL_ALIGN_RIGHT;
						iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
						sprintf(var, ": %.3s", c_nbr_tra);
						sprintf(pPtr, "%*s", 18 + strlen(var), var);
						xLine.eBackAlign = GL_ALIGN_LEFT;
						xLine.eTextAlign = GL_ALIGN_LEFT;

						if (memcmp(c_nbr_tra, "000", 3) != 0) {
							//Os__xprinta("trace pr?autho3");
							iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
							iRet = GoalPrnLine(hDocument, p++, "Offline", &xLine);
							CHECK(iRet >= 0, lblKO);

						} else {
							//Os__xprinta("trace pr?autho4");
							sprintf(pPtr, "%s", "0.00");
							xLine.eBackAlign = GL_ALIGN_RIGHT;
							xLine.eTextAlign = GL_ALIGN_RIGHT;
							iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
							sprintf(var, ": 000");
							sprintf(pPtr, "%*s", 18 + strlen(var), var);
							xLine.eBackAlign = GL_ALIGN_LEFT;
							xLine.eTextAlign = GL_ALIGN_LEFT;
							iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
							iRet = GoalPrnLine(hDocument, p++, "Offline", &xLine);
							CHECK(iRet >= 0, lblKO);
						}


						//ANNULEE
						memset(montant, 0, 13);
						memset(affiche_mont, 0, 14);
						memset(c_nbr_tra, 0, 4);
						long_asc(montant, 12,
								(ulong *) &(StrReport.Total[i][k][ANNUL_]));
						long_asc(c_nbr_tra, 3,
								(ulong *) &(StrReport.Nbre_tra[i][k][ANNUL_]));
						RemoveLeftZeros(montant);
						Exp = Fic_Devise_Acc.TabDevise[k].Exposant - 48;
						FormatAmount(montant, affiche_mont, Exp);

						if (memcmp(c_nbr_tra, "000", 3) != 0) {
							sprintf(var, "%12s", affiche_mont);
							sprintf(pPtr, "%s", var);
							xLine.eBackAlign = GL_ALIGN_RIGHT;
							xLine.eTextAlign = GL_ALIGN_RIGHT;
							iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
							sprintf(var, ": %.3s", c_nbr_tra);
							sprintf(pPtr, "%*s", 18 + strlen(var), var);
						} else {
							sprintf(pPtr, "%s", "0.00");
							xLine.eBackAlign = GL_ALIGN_RIGHT;
							xLine.eTextAlign = GL_ALIGN_RIGHT;
							iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
							sprintf(var, ": 000");
							sprintf(pPtr, "%*s", 18 + strlen(var), var);

						}
						xLine.eBackAlign = GL_ALIGN_LEFT;
						xLine.eTextAlign = GL_ALIGN_LEFT;
						iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
						iRet = GoalPrnLine(hDocument, p++, "Annul?e", &xLine);
						CHECK(iRet >= 0, lblKO);



						//CREDIT
						memset(montant, 0, 13);
						memset(affiche_mont, 0, 14);
						memset(c_nbr_tra, 0, 4);
						long_asc(montant, 12,
								(ulong *) &(StrReport.Total[i][k][CREDIT]));
						long_asc(c_nbr_tra, 3,
								(ulong *) &(StrReport.Nbre_tra[i][k][CREDIT]));
						RemoveLeftZeros(montant);
						Exp = Fic_Devise_Acc.TabDevise[k].Exposant - 48;
						FormatAmount(montant, affiche_mont, Exp);


						if (memcmp(c_nbr_tra, "000", 3) != 0) {
							sprintf(var, "%12s", affiche_mont);
							sprintf(pPtr, "%s",  var);
							xLine.eBackAlign = GL_ALIGN_RIGHT;
							xLine.eTextAlign = GL_ALIGN_RIGHT;
							iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
							sprintf(var, ": %.3s", c_nbr_tra);
							sprintf(pPtr, "%*s", 18 + strlen(var), var);

						} else {
							sprintf(pPtr, "%s", "0.00");
							xLine.eBackAlign = GL_ALIGN_RIGHT;
							xLine.eTextAlign = GL_ALIGN_RIGHT;
							iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
							sprintf(var, ": 000");
							sprintf(pPtr, "%*s", 18 + strlen(var), var);

						}
						xLine.eBackAlign = GL_ALIGN_LEFT;
						xLine.eTextAlign = GL_ALIGN_LEFT;
						iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
						iRet = GoalPrnLine(hDocument, p++, "Cr?dit", &xLine);
						CHECK(iRet >= 0, lblKO);


						//TOTAL
						if ((StrReport.Total[i][k][_OFFLINE]
								+ StrReport.Total[i][k][_ONLINE])
								> (StrReport.Total[i][k][ANNUL_]
										+ StrReport.Total[i][k][CREDIT]))
							l_total = (StrReport.Total[i][k][_OFFLINE]
									+ StrReport.Total[i][k][_ONLINE])
									- (StrReport.Total[i][k][ANNUL_]
											+ StrReport.Total[i][k][CREDIT]);
						else
							l_total = (StrReport.Total[i][k][ANNUL_]
									+ StrReport.Total[i][k][CREDIT])
									- (StrReport.Total[i][k][_OFFLINE]
											+ StrReport.Total[i][k][_ONLINE]);

						memset(montant, 0, 13);
						memset(affiche_mont, 0, 14);
						long_asc(montant, 12, (ulong *) &l_total);
						RemoveLeftZeros(montant);
						Exp = Fic_Devise_Acc.TabDevise[k].Exposant - 48;
						FormatAmount(montant, affiche_mont, Exp);


						if ((StrReport.Total[i][k][_OFFLINE]
								+ StrReport.Total[i][k][_ONLINE])
								< (StrReport.Total[i][k][ANNUL_]
										+ StrReport.Total[i][k][CREDIT])) {


						if (memcmp(affiche_mont, "0.0000000000", 12) != 0){
						sprintf(var, "%14s", affiche_mont);
						sprintf(pPtr, "\n\n%s", var);
						xLine.eBackAlign = GL_ALIGN_RIGHT;
						xLine.eTextAlign = GL_ALIGN_RIGHT;
						xLine.xFont.eScale = GL_SCALE_XLARGE;
						xLine.xFont.eStyle = GL_FONT_STYLE_BOLD;
						iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
					} else {
						sprintf(pPtr, "\n\n%s", "0.00");
						xLine.eBackAlign = GL_ALIGN_RIGHT;
						xLine.eTextAlign = GL_ALIGN_RIGHT;
						xLine.xFont.eScale = GL_SCALE_XLARGE;
						xLine.xFont.eStyle = GL_FONT_STYLE_BOLD;
						iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
					}
						xLine.eBackAlign = GL_ALIGN_LEFT;
						xLine.eTextAlign = GL_ALIGN_LEFT;
					iRet = GoalPrnLine(hDocument, p++, "\nTotal :\n   -", &xLine);
					CHECK(iRet >= 0, lblKO);

							} else {
								if (memcmp(affiche_mont, "0.0000000000", 12) != 0) {
									sprintf(var, "%14s", affiche_mont);
									sprintf(pPtr, "\n\n%s", var);
									xLine.eBackAlign = GL_ALIGN_RIGHT;
									xLine.eTextAlign = GL_ALIGN_RIGHT;
									xLine.xFont.eScale = GL_SCALE_XLARGE;
									xLine.xFont.eStyle = GL_FONT_STYLE_BOLD;
									iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
								} else {
									sprintf(pPtr, "\n\n%s", "0.00");
									xLine.xFont.eScale = GL_SCALE_XLARGE;
									xLine.xFont.eStyle = GL_FONT_STYLE_BOLD;
									xLine.eBackAlign = GL_ALIGN_RIGHT;
									xLine.eTextAlign = GL_ALIGN_RIGHT;
									iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
								}
								xLine.eBackAlign = GL_ALIGN_LEFT;
								xLine.eTextAlign = GL_ALIGN_LEFT;
								iRet = GoalPrnLine(hDocument, p++, "\nTotal :\n   +",&xLine);
								CHECK(iRet >= 0, lblKO);

							xLine.xFont.eScale = GL_SCALE_LARGE;

						}

					} else {
						xLine.xFont.eScale = GL_SCALE_LARGE;
						memcpy(monnai_alpha,
								Fic_Devise_Acc.TabDevise[k].code_alph, 3);
						sprintf(pPtr, "%.3s-", monnai_alpha); // monnaie alpha
						iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
						CHECK(iRet >= 0, lblKO);

						//NOMBRE
						Nbre_tra = StrReport.Nbre_tra[i][k][_ONLINE]
								+ StrReport.Nbre_tra[i][k][CREDIT]
								+ StrReport.Nbre_tra[i][k][ANNUL_];
						memset(c_nbr_tra, 0, 4);
						long_asc(c_nbr_tra, 4, (ulong *) &Nbre_tra);

						iRet = GoalPrnLine(hDocument, p++, "\n", &xLine);
						CHECK(iRet >= 0, lblKO);

						sprintf(pPtr, ": %.4s", c_nbr_tra);
						xLine.eBackAlign = GL_ALIGN_RIGHT;
						xLine.eTextAlign = GL_ALIGN_RIGHT;
						iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
						xLine.eBackAlign = GL_ALIGN_LEFT;
						xLine.eTextAlign = GL_ALIGN_LEFT;
						iRet = GoalPrnLine(hDocument, p++, "Nbr.Tran", &xLine);
						CHECK(iRet >= 0, lblKO);

						//DEBIT
						memset(montant, 0, 13);
						memset(affiche_mont, 0, 14);
						memset(c_nbr_tra, 0, 4);
						long_asc(montant, 12,
								(ulong *) &(StrReport.Total[i][k][_ONLINE]));
						long_asc(c_nbr_tra, 4,
								(ulong *) &(StrReport.Nbre_tra[i][k][_ONLINE]));
						RemoveLeftZeros(montant);
						Exp = Fic_Devise_Acc.TabDevise[k].Exposant - 48;
						FormatAmount(montant, affiche_mont, Exp);

						sprintf(pPtr, ": %12s", affiche_mont);
						xLine.eBackAlign = GL_ALIGN_RIGHT;
						xLine.eTextAlign = GL_ALIGN_RIGHT;
						iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
						xLine.eBackAlign = GL_ALIGN_LEFT;
						xLine.eTextAlign = GL_ALIGN_LEFT;
						iRet = GoalPrnLine(hDocument, p++, "Debit", &xLine);
						CHECK(iRet >= 0, lblKO);

						//CREDIT
						memset(montant, 0, 13);
						memset(affiche_mont, 0, 14);
						memset(c_nbr_tra, 0, 4);
						long_asc(montant, 12,
								(ulong *) &(StrReport.Total[i][k][CREDIT]));
						long_asc(c_nbr_tra, 4,
								(ulong *) &(StrReport.Nbre_tra[i][k][CREDIT]));
						RemoveLeftZeros(montant);
						Exp = Fic_Devise_Acc.TabDevise[k].Exposant - 48;
						FormatAmount(montant, affiche_mont, Exp);

						sprintf(pPtr, ": %12s", affiche_mont);
						xLine.eBackAlign = GL_ALIGN_RIGHT;
						xLine.eTextAlign = GL_ALIGN_RIGHT;
						iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
						xLine.eBackAlign = GL_ALIGN_LEFT;
						xLine.eTextAlign = GL_ALIGN_LEFT;
						iRet = GoalPrnLine(hDocument, p++, "Credit", &xLine);
						CHECK(iRet >= 0, lblKO);

						//ANNUL
						memset(montant, 0, 13);
						memset(affiche_mont, 0, 14);
						memset(c_nbr_tra, 0, 4);
						long_asc(montant, 12,
								(ulong *) &(StrReport.Total[i][k][ANNUL_]));
						long_asc(c_nbr_tra, 4,
								(ulong *) &(StrReport.Nbre_tra[i][k][ANNUL_]));
						RemoveLeftZeros(montant);
						Exp = Fic_Devise_Acc.TabDevise[k].Exposant - 48;
						FormatAmount(montant, affiche_mont, Exp);


						sprintf(pPtr, ": %12s", affiche_mont);
						xLine.eBackAlign = GL_ALIGN_RIGHT;
						xLine.eTextAlign = GL_ALIGN_RIGHT;
						iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
						xLine.eBackAlign = GL_ALIGN_LEFT;
						xLine.eTextAlign = GL_ALIGN_LEFT;
						iRet = GoalPrnLine(hDocument, p++, "Annulation", &xLine);
						CHECK(iRet >= 0, lblKO);

						//TOTAL
						if ((StrReport.Total[i][k][_ONLINE])
								> (StrReport.Total[i][k][ANNUL_]
										+ StrReport.Total[i][k][CREDIT]))
							l_total = StrReport.Total[i][k][_ONLINE]
									- (StrReport.Total[i][k][ANNUL_]
											+ StrReport.Total[i][k][CREDIT]);
						else
							l_total = (StrReport.Total[i][k][ANNUL_]
									+ StrReport.Total[i][k][CREDIT])
									- StrReport.Total[i][k][_ONLINE];
						memset(montant, 0, 13);
						memset(affiche_mont, 0, 14);
						long_asc(montant, 12, (ulong *) &l_total);
						RemoveLeftZeros(montant);
						Exp = Fic_Devise_Acc.TabDevise[k].Exposant - 48;
						FormatAmount(montant, affiche_mont, Exp);


						if (StrReport.Total[i][k][_ONLINE] < (StrReport.Total[i][k][ANNUL_] + StrReport.Total[i][k][CREDIT])) {
							sprintf(var, "%20s", affiche_mont);
							sprintf(pPtr, "\n\n%s", var);
							xLine.eBackAlign = GL_ALIGN_RIGHT;
							xLine.eTextAlign = GL_ALIGN_RIGHT;
							xLine.xFont.eScale = GL_SCALE_XLARGE;
							xLine.xFont.eStyle = GL_FONT_STYLE_BOLD;
							iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
							xLine.eBackAlign = GL_ALIGN_LEFT;
							xLine.eTextAlign = GL_ALIGN_LEFT;
							iRet = GoalPrnLine(hDocument, p++, "\nTotal :\n   -",&xLine);
							CHECK(iRet >= 0, lblKO);

						} else {

							sprintf(pPtr, "\n\n%20s", affiche_mont);
							xLine.eBackAlign = GL_ALIGN_RIGHT;
							xLine.eTextAlign = GL_ALIGN_RIGHT;
							xLine.xFont.eScale = GL_SCALE_XLARGE;
							xLine.xFont.eStyle = GL_FONT_STYLE_BOLD;
							iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
							xLine.eBackAlign = GL_ALIGN_LEFT;
							xLine.eTextAlign = GL_ALIGN_LEFT;
							iRet = GoalPrnLine(hDocument, p++, "\nTotal :\n   +",&xLine);
							CHECK(iRet >= 0, lblKO);
						}

					}
					//}
				}
			}

		}



	if (ind_rec == 0) {
		sprintf(pPtr, "\nInd. reconciliation =1\n");
	} else {
	    sprintf(pPtr, "\nInd. reconciliation =0\n");
	}

	xLine.eBackAlign = GL_ALIGN_CENTER;
	xLine.eTextAlign = GL_ALIGN_CENTER;
	xLine.xFont.eStyle = GL_FONT_STYLE_NORMAL;
	iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
	CHECK(iRet >= 0, lblKO);

	if ((ind_rec == 0) || (ind_rec == 1)) {

        char var[4];
        sprintf(var,":%02d",FicTra.nbr_card_capture);
        sprintf(Buffer, "%s", var);
        xLine.eBackAlign = GL_ALIGN_RIGHT;
        xLine.eTextAlign = GL_ALIGN_RIGHT;
		iRet = GoalPrnLine(hDocument, p, Buffer, &xLine);
		xLine.eBackAlign = GL_ALIGN_LEFT;
	    xLine.eTextAlign = GL_ALIGN_LEFT;
		iRet = GoalPrnLine(hDocument, p++, "Cartes Captur?s", &xLine);
		CHECK(iRet >= 0, lblKO);

		sprintf(var,":%02d",saisi_man);
		sprintf(Buffer, "%s",var);
		xLine.eBackAlign = GL_ALIGN_RIGHT;
		xLine.eTextAlign = GL_ALIGN_RIGHT;
		iRet = GoalPrnLine(hDocument, p, Buffer, &xLine);
		xLine.eBackAlign = GL_ALIGN_LEFT;
		xLine.eTextAlign = GL_ALIGN_LEFT;
		iRet = GoalPrnLine(hDocument, p++, "Op?rations Man", &xLine);
		CHECK(iRet >= 0, lblKO);
	}



	iRet = GoalPrnDocument(hDocument);              // Print document
	CHECK(iRet >= 0, lblKO);

	iRet = 1;
	goto lblEnd;
	// Errors treatment
	// ****************

	lblKO: GL_Dialog_Message(hGoal, NULL, "Processing Error", GL_ICON_ERROR,
			GL_BUTTON_VALID, 5 * 1000);
	goto lblEnd;

	lblEnd: if (hDocument)
	GoalDestroyDocument(&hDocument);

//	CUTERMpaperFeed();
	return OK;
}


//char* concat(const char *s1, const char *s2)
//{
//    char *result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
//    // in real code you would check for errors in malloc here
//    strcpy(result, s1);
//    strcat(result, s2);
//    return result;
//}

void removeRightSpaces(char *string) {
	int i, idx = 0;
	for (i = 0; i < strlen(string); i++) {
		if (string[i] != ' ') {
			idx = i;
		}
	}

	string[idx + 1] = '\0';
}

uchar ImprimeTra(uchar langue, uchar source, uchar dup) {

	char pPtr[1500];
	char buff1[30], buff2[30];
	uchar data_time[13];
	char affich_Cs[11];
	uchar montant[13];
	uchar affiche_mont[14];
	uchar Card_Nb[20];
	uchar status;
	Ulong MontComp;
	char NumSeqReliquat[6];

	bool bContinue = TRUE, bContinue2 = TRUE;
	int iHeader, iFooter;
	int iRet;

	uchar		Date_YYMMDDHHmmss2[7];
	UC	        heure[4+1];
	DATE date;
	Telium_Read_date(&date);
	sprintf(heure,"%.2s%.2s",date.hour,date.minute);
	sprintf(Date_YYMMDDHHmmss2,"%.2s%.2s%.2s",date.day, date.month, date.year);

	T_GL_HWIDGET hDocument = NULL;
	T_GL_HWIDGET xDocument = NULL;
	ST_PRN_LINE xLine;
	byte p; // Printer line index
	T_GL_HGRAPHIC_LIB hGoal = NULL;
	//int iRet;
	char var[60];

	CUMORE_CMI_AFFICHE_MESSAGE(94,32,1000);

	//MessageWithoutConfirmation("Impression... Patientez SVP","file://flash/HOST/impression_encours.png");

	printLogo(source);

	p = 0;                                            // Create document
	hDocument = GoalCreateDocument(APEMV_UI_GoalHandle(),
			GL_ENCODING_ISO_8859_1);
	CHECK(hDocument != NULL, lblKO);
	xDocument = GoalCreateDocument(APEMV_UI_GoalHandle(), GL_ENCODING_ISO_8859_1);
	CHECK(xDocument != NULL, lblKO);
	// *** Font scale capability ***
	xLine = xPrinter;
	xLine.xFont.eStyle = GL_FONT_STYLE_BOLD;
	xLine.xFont.eScale = GL_SCALE_XLARGE;
	xLine.eTextAlign = GL_ALIGN_CENTER;
	xLine.eBackAlign = GL_ALIGN_CENTER;
	if (StructInfoPayment.CodeFunction == PREAUTHCODE) {
		sprintf(pPtr, "\nPRE-AUTORISATION");
		iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
		CHECK(iRet >= 0, lblKO);

		StructInfoPayment.SignatureHolder = REQUIRED;
	}

	else if (StructInfoPayment.CodeFunction == PREAUTHCOMPCODE) {
		sprintf(pPtr, "\nVAL.TRX.PRE-AUTO");
		iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
		CHECK(iRet >= 0, lblKO);
	}

//	else
//		sprintf(pPtr, "  ");

	xLine.eBackAlign = GL_ALIGN_CENTER;
	xLine.eTextAlign = GL_ALIGN_CENTER;
	xLine.xFont.eScale = GL_SCALE_XLARGE;
	sprintf(pPtr, "-------------------------------------");
	iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
	CHECK(iRet >= 0, lblKO);

	xLine.xFont.eStyle = GL_FONT_STYLE_NORMAL;
	xLine.xFont.eScale = GL_SCALE_LARGE;
	xLine.eTextAlign = GL_ALIGN_LEFT;
	xLine.eBackAlign = GL_ALIGN_LEFT;
	//CHECK(iRet >= 0, lblKO);
	//uchar data_time[13];
	memcpy(data_time, StructInfoPayment.Transaction_DateTime, 12);
	sprintf(pPtr, "%.2s/%.2s/%.2s                            %.2s:%.2s:%.2s",
			data_time + 4, data_time + 2, data_time, data_time + 6,
			data_time + 8, data_time + 10);
	iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
	CHECK(iRet >= 0, lblKO);

	xLine.xFont.eStyle = GL_FONT_STYLE_BOLD;
	xLine.eTextAlign = GL_ALIGN_CENTER;
	xLine.eBackAlign = GL_ALIGN_CENTER;


	int ib;
	for (ib = 21; (ib > 6) && (FicPara.Merchant_Name[ib] == ' '); ib--);
	FicPara.Merchant_Name[ib + 1] = 0;

	sprintf(buff1, "%10s", FicPara.Merchant_Name);
	sprintf(buff2, "%10s", FicPara.Merchant_City);

	removeRightSpaces(buff1);
	removeRightSpaces(buff2);

	if (strlen(buff1) + strlen(buff2) < 19)
		sprintf((PC) pPtr, "%s - %s\n", buff1, buff2);
	else
		sprintf((PC) pPtr, "%s\n%s\n", buff1, buff2);

	//sprintf((PC)Buffer, "%s - %s\n",buff1,buff2);
	iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
	CHECK(iRet >= 0, lblKO);

	xLine.xFont.eStyle = GL_FONT_STYLE_NORMAL;
	xLine.xFont.eScale = GL_SCALE_XLARGE;
	sprintf(pPtr, "-------------------------------------");
	iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
	CHECK(iRet >= 0, lblKO);

	xLine.eBackAlign = GL_ALIGN_LEFT;
	xLine.xFont.eScale = GL_SCALE_LARGE;
	xLine.eTextAlign = GL_ALIGN_LEFT;

	//Nom du porteur
	if (StructInfoPayment.Appli_label[0] != 0) {
		sprintf(pPtr, "%s", StructInfoPayment.Appli_aid + 1);
		iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
		CHECK(iRet >= 0, lblKO);
		sprintf(pPtr, "%.16s", StructInfoPayment.Appli_label);
		iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
		CHECK(iRet >= 0, lblKO);
		sprintf(pPtr, "%.24s", StructInfoPayment.Nom_Prenom);
		iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
		CHECK(iRet >= 0, lblKO);
	} else {
		sprintf(pPtr, "%.24s", StructInfoPayment.Nom_Prenom);
		iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
		CHECK(iRet >= 0, lblKO);
	}
	memset(Card_Nb, 0, 20);
	memcpy(Card_Nb, &StructInfoPayment.Holder_Card[1],StructInfoPayment.Holder_Card[0]);

	Ushort ik;
	if (source == COMMERCANT) {
		//		Card_Nb[strlen(Card_Nb)-1] = '*';
		//		Card_Nb[strlen(Card_Nb)-2] = '*';
		//		Card_Nb[strlen(Card_Nb)-3] = '*';
		//		Card_Nb[strlen(Card_Nb)-4] = '*';

		for (ik = 0; ik < strlen((char*) Card_Nb) - 4; ik++)
			Card_Nb[ik] = '*';
	} else {
		for (ik = 6; ik < strlen((char*) Card_Nb) - 4; ik++)
			Card_Nb[ik] = '*';
	}
	sprintf(pPtr, "%s", Card_Nb);
	iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
	CHECK(iRet >= 0, lblKO);

	if (StructInfoPayment.Origine_carte == CARTE_NATIONALE)
		sprintf(pPtr, "%.2s/%.2s CARTE NATIONALE",StructInfoPayment.Expired_Date + 2,StructInfoPayment.Expired_Date);
	else if (StructInfoPayment.Origine_carte == CARTE_DOMESTIQUE)
		sprintf(pPtr, "%.2s/%.2s CARTE DOMESTIQUE",StructInfoPayment.Expired_Date + 2,StructInfoPayment.Expired_Date);
	else if (StructInfoPayment.Origine_carte = CARTE_ETRANGATRS)
		sprintf(pPtr, "%.2s/%.2s CARTE ETRANGERE",StructInfoPayment.Expired_Date + 2,StructInfoPayment.Expired_Date);
	else
		sprintf(pPtr, "%.2s/%.2s CARTE ETRANGERE",StructInfoPayment.Expired_Date + 2,StructInfoPayment.Expired_Date);

	iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
	CHECK(iRet >= 0, lblKO);

//	if (StructInfoPayment.StatusAnnulAdjust) {
	memcpy(affich_Cs, StructInfoPayment.SeviceCode, 3);
	//if(StructInfoPayment.forcage)
	if ((StructInfoPayment.forcage)
			|| (StructInfoPayment.Top_Forcage == TOP_FOR_REFFERAL))
		affich_Cs[3] = '1';
	else
		affich_Cs[3] = '0';
	memcpy(affich_Cs + 4, &lop_gest.NumListe[0], 4);
	if (StructInfoPayment.Entry_Mode == TRS_EMV)
		affich_Cs[8] = '1';
	else if (StructInfoPayment.Entry_Mode == TRS_MAN)
		affich_Cs[8] = '2';
	else
		affich_Cs[8] = '0';

	sprintf(affich_Cs + 9, "%.02d",
			FicInsReg.Tab_Ins_reg[StructInfoPayment.Type_carte].Code_Prefixe);
	sprintf(pPtr, "%.3s-%.1s-%.4s-%.1s-%.2s", affich_Cs, affich_Cs + 3,
			affich_Cs + 4, affich_Cs + 8, affich_Cs + 9);
	iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
	CHECK(iRet >= 0, lblKO);
//	}

//	iRet = GoalPrnLine(hDocument, p++, "    ", &xLine);
//	CHECK(iRet >= 0, lblKO);
	iRet = GoalPrnLine(hDocument, p++, "    ", &xLine);
	CHECK(iRet >= 0, lblKO);

//	sprintf(var, ":  %.15s", FicPara.Num_commerce);// jrizki 25032019
//	sprintf(pPtr, "%*s", 29 + strlen(var), var);
//	iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
//	iRet = GoalPrnLine(hDocument, p++, "Num Commer?ant", &xLine);
//	CHECK(iRet >= 0, lblKO);


	sprintf(pPtr, "%.15s", FicPara.Num_commerce);// jrizki 25032019
	xLine.eTextAlign = GL_ALIGN_RIGHT;
	xLine.eBackAlign = GL_ALIGN_RIGHT;
	iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
	xLine.eTextAlign = GL_ALIGN_LEFT;
	xLine.eBackAlign = GL_ALIGN_LEFT;
	strcpy(var, ":");
	sprintf(pPtr, "%*s", 29 + strlen(var), var);
	iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
	iRet = GoalPrnLine(hDocument, p++, "Num Commer?ant", &xLine);
	CHECK(iRet >= 0, lblKO);


	sprintf(pPtr, "%.8s", FicPara.Num_Terminal);		// jrizki 25032019
	xLine.eTextAlign = GL_ALIGN_RIGHT;
	xLine.eBackAlign = GL_ALIGN_RIGHT;
	iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
	xLine.eTextAlign = GL_ALIGN_LEFT;
	xLine.eBackAlign = GL_ALIGN_LEFT;
	strcpy(var, ":");
	sprintf(pPtr, "%*s", 29 + strlen(var), var);
	iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
	iRet = GoalPrnLine(hDocument, p++, "Num TPE", &xLine);
	CHECK(iRet >= 0, lblKO);


//	sprintf(var, ":  %.15s", StructInfoPayment.Num_point_vente);
//	sprintf(pPtr, "%*s", 20 + strlen(var), var);
//	iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
//	iRet = GoalPrnLine(hDocument, p++, "N PDV", &xLine);
//	CHECK(iRet >= 0, lblKO);

	if ((StructInfoPayment.CodeFunction == PREAUTHCOMPCODE) && (!dup)
			&& (StructInfoPayment.Preauto_Amount_Final
					> StructInfoPayment.Preauto_Amount_Initial
							+ (StructInfoPayment.Preauto_Amount_Initial * 15)
									/ 100)) {
		//Pre-autorisation
		sprintf(pPtr, "\nPre-autorisation");
		xLine.eBackAlign = GL_ALIGN_CENTER;
		xLine.eTextAlign = GL_ALIGN_CENTER;
		xLine.xFont.eStyle = GL_FONT_STYLE_BOLD;
		iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
		CHECK(iRet >= 0, lblKO);
		xLine.xFont.eStyle = GL_FONT_STYLE_NORMAL;
		//JJ/MM/AA	HH:MM:SS
		memcpy(data_time, StrPreAut.Transaction_DateTime, 12);
		sprintf(pPtr,
				"%.2s/%.2s/%.2s                            %.2s:%.2s:%.2s",
				data_time + 4, data_time + 2, data_time, data_time + 6,
				data_time + 8, data_time + 10);
		xLine.eBackAlign = GL_ALIGN_LEFT;
		xLine.eTextAlign = GL_ALIGN_LEFT;
		iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
		CHECK(iRet >= 0, lblKO);
		//Montant
//		sprintf(var, ":  %d,%d%d%s", StrPreAut.Transaction_Amount / 100,
//				(StrPreAut.Transaction_Amount / 10) % 10,
//				StrPreAut.Transaction_Amount % 10,
//				StructInfoPayment.Devise_acc.code_alph);
//		sprintf(pPtr, "%*s", 29 + strlen(var), var);
//		iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
//		iRet = GoalPrnLine(hDocument, p++, "Mnt", &xLine);
//		CHECK(iRet >= 0, lblKO);

		iRet = GoalPrnLine(hDocument, p++, "    ", &xLine);
		CHECK(iRet >= 0, lblKO);

		xLine.xFont.eStyle = GL_FONT_STYLE_BOLD;
		sprintf(pPtr, "%d,%d%d %s", StrPreAut.Transaction_Amount / 100,
				(StrPreAut.Transaction_Amount / 10) % 10,
				StrPreAut.Transaction_Amount % 10,
				StructInfoPayment.Devise_acc.code_alph);
		xLine.eTextAlign = GL_ALIGN_RIGHT;
		xLine.eBackAlign = GL_ALIGN_RIGHT;
		iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
		xLine.eTextAlign = GL_ALIGN_LEFT;
		xLine.eBackAlign = GL_ALIGN_LEFT;
		strcpy(var, ":");
		sprintf(pPtr, "%*s", 29 + strlen(var), var);
		iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
		iRet = GoalPrnLine(hDocument, p++, "Montant", &xLine);
		CHECK(iRet >= 0, lblKO);

		xLine.xFont.eStyle = GL_FONT_STYLE_NORMAL;
		//Num transaction
//		sprintf(var, ":  %.6s", StructInfoPayment.Num_seq_save_compl_online);
//		sprintf(pPtr, "%*s", 29 + strlen(var), var);
//		iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
//		iRet = GoalPrnLine(hDocument, p++, "Num Transaction", &xLine);
//		CHECK(iRet >= 0, lblKO);

		sprintf(pPtr, "%.6s", StructInfoPayment.Num_seq_save_compl_online);
		xLine.eTextAlign = GL_ALIGN_RIGHT;
		xLine.eBackAlign = GL_ALIGN_RIGHT;
		iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
		xLine.eTextAlign = GL_ALIGN_LEFT;
		xLine.eBackAlign = GL_ALIGN_LEFT;
		strcpy(var, ":");
		sprintf(pPtr, "%*s", 29 + strlen(var), var);
		iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
		iRet = GoalPrnLine(hDocument, p++, "Num Transaction", &xLine);
		CHECK(iRet >= 0, lblKO);

//		sprintf(var, ":  %.6s", StrPreAut.Approval_Code);
//		sprintf(pPtr, "%*s", 29 + strlen(var), var);
//		iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
//		iRet = GoalPrnLine(hDocument, p++, "Num Autorisation", &xLine);
//		CHECK(iRet >= 0, lblKO);

		sprintf(pPtr, "%.6s", StrPreAut.Approval_Code);
		xLine.eTextAlign = GL_ALIGN_RIGHT;
		xLine.eBackAlign = GL_ALIGN_RIGHT;
		iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
		xLine.eTextAlign = GL_ALIGN_LEFT;
		xLine.eBackAlign = GL_ALIGN_LEFT;
		strcpy(var, ":");
		sprintf(pPtr, "%*s", 29 + strlen(var), var);
		iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
		iRet = GoalPrnLine(hDocument, p++, "Num Autorisation", &xLine);
		CHECK(iRet >= 0, lblKO);

//		sprintf(var, ":  %.6s", StrPreAut.STAN);
//		sprintf(pPtr, "%*s", 29 + strlen(var), var);
//		iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
//		iRet = GoalPrnLine(hDocument, p++, "Num STAN", &xLine);
//		CHECK(iRet >= 0, lblKO);

		sprintf(pPtr, "%.6s", StrPreAut.STAN);
		xLine.eTextAlign = GL_ALIGN_RIGHT;
		xLine.eBackAlign = GL_ALIGN_RIGHT;
		iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
		xLine.eTextAlign = GL_ALIGN_LEFT;
		xLine.eBackAlign = GL_ALIGN_LEFT;
		strcpy(var, ":");
		sprintf(pPtr, "%*s", 29 + strlen(var), var);
		iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
		iRet = GoalPrnLine(hDocument, p++, "Num STAN", &xLine);
		CHECK(iRet >= 0, lblKO);

		//Complement
		sprintf(pPtr, "\nComplement");
		xLine.xFont.eStyle = GL_FONT_STYLE_BOLD;
		xLine.eBackAlign = GL_ALIGN_CENTER;
		xLine.eTextAlign = GL_ALIGN_CENTER;
		iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
		CHECK(iRet >= 0, lblKO);
		//JJ/MM/AA	HH:MM:SS
		memcpy(data_time, StructInfoPayment.Transaction_DateTime, 12);
		sprintf(pPtr,
				"%.2s/%.2s/%.2s                            %.2s:%.2s:%.2s",
				data_time + 4, data_time + 2, data_time, data_time + 6,
				data_time + 8, data_time + 10);
		xLine.eBackAlign = GL_ALIGN_LEFT;
		xLine.eTextAlign = GL_ALIGN_LEFT;
		xLine.xFont.eStyle = GL_FONT_STYLE_NORMAL;
		iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
		CHECK(iRet >= 0, lblKO);
		iRet = GoalPrnLine(hDocument, p++, "    ", &xLine);
		CHECK(iRet >= 0, lblKO);
		//Montant
//		MontComp = StructInfoPayment.Transaction_Amount
//				- StrPreAut.Transaction_Amount;
//		sprintf(var, ":  %d,%d%d%s", MontComp / 100, (MontComp / 10) % 10,
//				MontComp % 10, StructInfoPayment.Devise_acc.code_alph);
//		sprintf(pPtr, "%*s", 29 + strlen(var), var);
//		iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
//		iRet = GoalPrnLine(hDocument, p++, "Mnt", &xLine);
//		CHECK(iRet >= 0, lblKO);
		xLine.xFont.eStyle = GL_FONT_STYLE_BOLD;
		MontComp = StructInfoPayment.Transaction_Amount
				- StrPreAut.Transaction_Amount;
		sprintf(pPtr, "%d,%d%d %s", MontComp / 100, (MontComp / 10) % 10,
				MontComp % 10, StructInfoPayment.Devise_acc.code_alph);
		xLine.eTextAlign = GL_ALIGN_RIGHT;
		xLine.eBackAlign = GL_ALIGN_RIGHT;
		iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
		xLine.eTextAlign = GL_ALIGN_LEFT;
		xLine.eBackAlign = GL_ALIGN_LEFT;
		strcpy(var, ":");
		sprintf(pPtr, "%*s", 29 + strlen(var), var);
		iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
		iRet = GoalPrnLine(hDocument, p++, "Montant", &xLine);
		CHECK(iRet >= 0, lblKO);


		xLine.xFont.eStyle = GL_FONT_STYLE_NORMAL;
		memcpy(NumSeqReliquat, StructInfoPayment.Num_seq_save_compl_online, 6);
		addition(NumSeqReliquat, "000001", 6);

		sprintf(pPtr, "%.6s", NumSeqReliquat);
		xLine.eTextAlign = GL_ALIGN_RIGHT;
		xLine.eBackAlign = GL_ALIGN_RIGHT;
		iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
		xLine.eTextAlign = GL_ALIGN_LEFT;
		xLine.eBackAlign = GL_ALIGN_LEFT;
		strcpy(var, ":");
		sprintf(pPtr, "%*s", 29 + strlen(var), var);
		iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
		iRet = GoalPrnLine(hDocument, p++, "Num Transaction", &xLine);
		CHECK(iRet >= 0, lblKO);


		sprintf(pPtr, "%.6s", StructInfoPayment.Approval_Code);
		xLine.eTextAlign = GL_ALIGN_RIGHT;
		xLine.eBackAlign = GL_ALIGN_RIGHT;
		iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
		xLine.eTextAlign = GL_ALIGN_LEFT;
		xLine.eBackAlign = GL_ALIGN_LEFT;
		strcpy(var, ":");
		sprintf(pPtr, "%*s", 29 + strlen(var), var);
		iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
		iRet = GoalPrnLine(hDocument, p++, "Num Autorisation", &xLine);
		CHECK(iRet >= 0, lblKO);


		sprintf(pPtr, "%.6s", StructInfoPayment.STAN);
		xLine.eTextAlign = GL_ALIGN_RIGHT;
		xLine.eBackAlign = GL_ALIGN_RIGHT;
		iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
		xLine.eTextAlign = GL_ALIGN_LEFT;
		xLine.eBackAlign = GL_ALIGN_LEFT;
		strcpy(var, ":");
		sprintf(pPtr, "%*s", 29 + strlen(var), var);
		iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
		iRet = GoalPrnLine(hDocument, p++, "Num STAN", &xLine);
		CHECK(iRet >= 0, lblKO);
	}

	else {

//		iRet = GoalPrnLine(hDocument, p++, "    ", &xLine);
//		CHECK(iRet >= 0, lblKO);

		if (StructInfoPayment.CodeFunction == PREAUTHCODE) {

			sprintf(pPtr, "%.6s", StructInfoPayment.Num_sequence);
			xLine.eTextAlign = GL_ALIGN_RIGHT;
			xLine.eBackAlign = GL_ALIGN_RIGHT;
			iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
			xLine.eTextAlign = GL_ALIGN_LEFT;
			xLine.eBackAlign = GL_ALIGN_LEFT;
			strcpy(var, ":");
			sprintf(pPtr, "%*s", 29 + strlen(var), var);
			iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
			iRet = GoalPrnLine(hDocument, p++, "Num Pre-auto", &xLine);
			CHECK(iRet >= 0, lblKO);

		} else {
			sprintf(pPtr, "%.6s", StructInfoPayment.Num_sequence);
			xLine.eTextAlign = GL_ALIGN_RIGHT;
			xLine.eBackAlign = GL_ALIGN_RIGHT;
			iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
			xLine.eTextAlign = GL_ALIGN_LEFT;
			xLine.eBackAlign = GL_ALIGN_LEFT;
			strcpy(var, ":");
			sprintf(pPtr, "%*s", 29 + strlen(var), var);
			iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
			iRet = GoalPrnLine(hDocument, p++, "Num Transaction", &xLine);
			CHECK(iRet >= 0, lblKO);

			StructInfoPayment.SignatureHolder = NOT_REQUIRED;

		}

		sprintf(pPtr, "%.6s", StructInfoPayment.Approval_Code);
		xLine.eTextAlign = GL_ALIGN_RIGHT;
		xLine.eBackAlign = GL_ALIGN_RIGHT;
		iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
		xLine.eTextAlign = GL_ALIGN_LEFT;
		xLine.eBackAlign = GL_ALIGN_LEFT;
		strcpy(var, ":");
		sprintf(pPtr, "%*s", 29 + strlen(var), var);
		iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
		iRet = GoalPrnLine(hDocument, p++, "Num Autorisation", &xLine);
		CHECK(iRet >= 0, lblKO);

		sprintf(pPtr, "%.6s", StructInfoPayment.STAN);
		xLine.eTextAlign = GL_ALIGN_RIGHT;
		xLine.eBackAlign = GL_ALIGN_RIGHT;
		iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
		xLine.eTextAlign = GL_ALIGN_LEFT;
		xLine.eBackAlign = GL_ALIGN_LEFT;
		strcpy(var, ":");
		sprintf(pPtr, "%*s", 29 + strlen(var), var);
		iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
		iRet = GoalPrnLine(hDocument, p++, "Num STAN", &xLine);
		CHECK(iRet >= 0, lblKO);

	}

	xLine.eBackAlign = GL_ALIGN_CENTER;
	xLine.eTextAlign = GL_ALIGN_CENTER;
	xLine.xFont.eScale = GL_SCALE_XLARGE;
	sprintf(pPtr, "-------------------------------------");
	iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
	CHECK(iRet >= 0, lblKO);
	xLine.eBackAlign = GL_ALIGN_LEFT;
	xLine.eTextAlign = GL_ALIGN_LEFT;
	xLine.xFont.eScale = GL_SCALE_LARGE;
	xLine.xFont.eStyle = GL_FONT_STYLE_BOLD;

//	iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
//	CHECK(iRet >= 0, lblKO);
	if (StructInfoPayment.CodeFunction == PAIEMENT_POURBOIRE
	|| (StructInfoPayment.CodeFunction == VOIDCODE && StructInfoPayment.MontantPourboireInterchange != 0 )
	|| (StructInfoPayment.StatusAnnulAdjust && StructInfoPayment.MontantPourboireInterchange != 0 )
	) {

		sprintf(var, "%d,%d%d %s", StructInfoPayment.Transaction_Amount / 100,
				(StructInfoPayment.Transaction_Amount / 10) % 10,
				StructInfoPayment.Transaction_Amount % 10,
				StructInfoPayment.Devise_acc.code_alph);
		sprintf(pPtr, "%s", var);
		xLine.eBackAlign = GL_ALIGN_RIGHT;
		xLine.eTextAlign = GL_ALIGN_RIGHT;
		iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
		xLine.eBackAlign = GL_ALIGN_LEFT;
		xLine.eTextAlign = GL_ALIGN_LEFT;
		strcpy(var, ":");
		sprintf(pPtr, "%*s", 29 + strlen(var), var);
		iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
		iRet = GoalPrnLine(hDocument, p++, "MONTANT", &xLine);
		CHECK(iRet >= 0, lblKO);

		sprintf(var, "%d,%d%d %s",
				StructInfoPayment.MontantPourboireInterchange / 100,
				(StructInfoPayment.MontantPourboireInterchange / 10) % 10,
				StructInfoPayment.MontantPourboireInterchange % 10,
				StructInfoPayment.Devise_acc.code_alph);
		sprintf(pPtr, "%s", var);
		xLine.eBackAlign = GL_ALIGN_RIGHT;
		xLine.eTextAlign = GL_ALIGN_RIGHT;
		iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
		xLine.eBackAlign = GL_ALIGN_LEFT;
		xLine.eTextAlign = GL_ALIGN_LEFT;
		strcpy(var, ":");
		sprintf(pPtr, "%*s", 29 + strlen(var), var);
		iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
		iRet = GoalPrnLine(hDocument, p++, "POURBOIRE", &xLine);
		CHECK(iRet >= 0, lblKO);

		sprintf(var, "%d,%d%d %s",
				(StructInfoPayment.MontantPourboireInterchange
						+ StructInfoPayment.Transaction_Amount) / 100,
				((StructInfoPayment.MontantPourboireInterchange
						+ StructInfoPayment.Transaction_Amount) / 10) % 10,
				(StructInfoPayment.MontantPourboireInterchange
						+ StructInfoPayment.Transaction_Amount) % 10,
				StructInfoPayment.Devise_acc.code_alph);
		sprintf(pPtr, "%s", var);
		xLine.eBackAlign = GL_ALIGN_RIGHT;
		xLine.eTextAlign = GL_ALIGN_RIGHT;
		iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
		xLine.eBackAlign = GL_ALIGN_LEFT;
		xLine.eTextAlign = GL_ALIGN_LEFT;
		strcpy(var, ":");
		sprintf(pPtr, "%*s", 29 + strlen(var), var);
		iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
		iRet = GoalPrnLine(hDocument, p++, "TOTAL", &xLine);
		CHECK(iRet >= 0, lblKO);

	} else {
		if (StructInfoPayment.Transaction_Amount / 100 >= 100000)
			xLine.xFont.eScale = GL_SCALE_LARGE;
		else
			xLine.xFont.eScale = GL_SCALE_XLARGE;

		sprintf(pPtr, "MONTANT : %d,%d%d %s",
				StructInfoPayment.Transaction_Amount / 100,
				(StructInfoPayment.Transaction_Amount / 10) % 10,
				StructInfoPayment.Transaction_Amount % 10,
				StructInfoPayment.Devise_acc.code_alph);
		xLine.eBackAlign = GL_ALIGN_CENTER;
		xLine.eTextAlign = GL_ALIGN_CENTER;
		iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
		CHECK(iRet >= 0, lblKO);
	}
	xLine.eBackAlign = GL_ALIGN_CENTER;
	xLine.eTextAlign = GL_ALIGN_CENTER;
	xLine.xFont.eScale = GL_SCALE_XLARGE;
	sprintf(pPtr, "-------------------------------------");
	xLine.xFont.eStyle = GL_FONT_STYLE_NORMAL;
	iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
	CHECK(iRet >= 0, lblKO);
	//xLine.xFont.eScale = GL_SCALE_LARGE;




	xLine.xFont.eScale = GL_SCALE_LARGE;
	switch(StructInfoPayment.CodeFunction)
		{
			case RECH_PREPAYE:
				sprintf(pPtr, "-RECHARGE PREPAYEE-");
				GoalPrnLine(hDocument, p++, pPtr, &xLine);
				break;
			case DECH_PREPAYE:
				sprintf(pPtr, "-DECHARGE PREPAYEE-");
				GoalPrnLine(hDocument, p++, pPtr, &xLine);
				break;
			case CAC_PREPAYE:
				sprintf(pPtr, "TRANSFERT CARTE\nA CARTE");
				GoalPrnLine(hDocument, p++, pPtr, &xLine);
				break;
			case CASHIN_PREPAYE:
				sprintf(pPtr, "TRANSFERT CARTE\nA CASH\n-CASH IN-");
				GoalPrnLine(hDocument, p++, pPtr, &xLine);
				break;
			case CASHOUT_PREPAYE:
				sprintf(pPtr, "TRANSFERT CARTE\nA CASH\n-CASH OUT-");
				GoalPrnLine(hDocument, p++, pPtr, &xLine);
				break;
			case VOIDCODE:
				sprintf(pPtr, "ANNULATION");
				GoalPrnLine(hDocument, p++, pPtr, &xLine);
				break;
			case REFUNDCODE:
				sprintf(pPtr,  "CREDIT");
				GoalPrnLine(hDocument, p++, pPtr, &xLine);
				break;
			case CASHADVANCESCODE:
				sprintf(pPtr, "CASHADVANCE");
				GoalPrnLine(hDocument, p++, pPtr, &xLine);
				break;
			case PREAUTHCODE:
			if(StructInfoPayment.StatusAnnulAdjust != 1)
				break;
				//printTTF_Align( canvas, "\n", strlen("\n"), kAlignCenter);
			case VOID_PREAUTO_TOTAL_CODE:
			case VOID_PREAUTO_PARTIEL_CODE:
				sprintf(pPtr, "ANNULATION PRE-AUTO\n");
				GoalPrnLine(hDocument, p++, pPtr, &xLine);
				break;

			default:
				if(StructInfoPayment.StatusAnnulAdjust){
					sprintf(pPtr, "ANNULATION");
					GoalPrnLine(hDocument, p++, pPtr, &xLine);
				}
				else if (StructInfoPayment.CodeFunction == CASHADVANCESCODE){
					sprintf(pPtr,  "CASH ADVANCE");
					GoalPrnLine(hDocument, p++, pPtr, &xLine);
				}
				else {
					sprintf(pPtr, "DEBIT");
					GoalPrnLine(hDocument, p++, pPtr, &xLine);
				}
				break;
		}





	switch (StructInfoPayment.CodeFunction) {
	case VOID_PREAUTO_TOTAL_CODE:
	case VOID_PREAUTO_PARTIEL_CODE:
		break;

	default:
		if (StructInfoPayment.SignatureHolder == REQUIRED) {
			if (source != CLIENT){

				sprintf(pPtr, "SIGNATURE CLIENT\n\n\n\n");
				xLine.xFont.eScale = GL_SCALE_LARGE;
				xLine.xFont.eStyle = GL_FONT_STYLE_BOLD;
				iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
			}

		}
		break;



	}


/*

	switch (StructInfoPayment.CodeFunction) {
	case VOIDCODE:
		sprintf(pPtr, "ANNULATION");
//		xLine.xFont.eStyle = GL_FONT_STYLE_BOLD;
//		iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
//		CHECK(iRet >= 0, lblKO);
//		if (StructInfoPayment.SignatureHolder == REQUIRED) {
//			if (source == CLIENT)	//kamal251105
//				sprintf(pPtr, "\n");
//			else
//				sprintf(pPtr, "Signature client\n\n\n");
//			//xLine.xFont.eScale = GL_SCALE_LARGE;
//			xLine.xFont.eStyle = GL_FONT_STYLE_BOLD;
//			//iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
//		}
		//xLine.xFont.eStyle = GL_FONT_STYLE_BOLD;
		break;
	case REFUNDCODE:
		sprintf(pPtr, "CREDIT");
		break;
	case PREAUTHCODE:
		sprintf(pPtr, "\n"); //KI Le 190106
		break;
	case VOID_PREAUTO_TOTAL_CODE:
	case VOID_PREAUTO_PARTIEL_CODE:
		xLine.xFont.eStyle = GL_FONT_STYLE_BOLD;
		sprintf(pPtr, "ANNULATION PRE-AUTO.");	//Mouna310305
		break;
	default:
		if (StructInfoPayment.StatusAnnulAdjust) {
			xLine.xFont.eStyle = GL_FONT_STYLE_BOLD;
			sprintf(pPtr, "ANNULATION");
		} else
			sprintf(pPtr, "DEBIT");
		break;
	}
	xLine.xFont.eScale = GL_SCALE_LARGE;
	iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
	CHECK(iRet >= 0, lblKO);


	if (StructInfoPayment.SignatureHolder == REQUIRED) {
		if (source == CLIENT)	//kamal251105
			sprintf(pPtr, "\n");
		else
			sprintf(pPtr, "Signature client\n\n\n\n");
		xLine.xFont.eScale = GL_SCALE_LARGE;
		xLine.xFont.eStyle = GL_FONT_STYLE_BOLD;
		iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
	}
*/

	if (dup) {
		sprintf(pPtr, "\nDUPLICATA\n\n");
		iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
		CHECK(iRet >= 0, lblKO);
	}

	xLine.xFont.eStyle = GL_FONT_STYLE_NORMAL;
	xLine.xFont.eScale = GL_SCALE_LARGE;

	if (langue == ENG) {
		//sprintf(pPtr, "TICKET TO SAVE");
		if (source == CLIENT)
			sprintf(pPtr, "Client Copy");
		else
			sprintf(pPtr, "Merchant Copy");
	} else {
		//sprintf(pPtr, "TICKET A CONSERVER");
		if (source == CLIENT)
			sprintf(pPtr, "Copie client");
		else
			sprintf(pPtr, "Copie commer?ant", 135);
	}
	iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
	CHECK(iRet >= 0, lblKO);
	iRet = GoalPrnDocument(hDocument);              // Print document
	CHECK(iRet >= 0, lblKO);
	GoalDestroyDocument(&hDocument);
//
//	iRet = GoalPrnDocument(hDocument);              // Print document
//	CHECK(iRet >= 0, lblKO);
//
//	iRet = 1;
//	goto lblEnd;
//	// Errors treatment
//	// ****************
//
//	lblKO: GL_Dialog_Message(hGoal, NULL, "Processing Error printer", GL_ICON_ERROR, GL_BUTTON_VALID, 5 * 1000);
//	goto lblEnd;
//
//	lblEnd: if (hDocument)
//		GoalDestroyDocument(&hDocument);
//
//	CUTERMpaperFeed();
////
	if (source != CLIENT)
	{
	sprintf(pPtr, "-------------------------------------");
	xLine.xFont.eScale = GL_SCALE_XLARGE;
	iRet = GoalPrnLine(xDocument, p++, pPtr, &xLine);
	CHECK(iRet >= 0, lblKO);
	xLine.xFont.eScale = GL_SCALE_LARGE;
	sprintf(pPtr, "Conservez-moi, je peux ?tre utile !");
	iRet = GoalPrnLine(xDocument, p++, pPtr, &xLine);
	CHECK(iRet >= 0, lblKO);

	xLine.xFont.eStyle = GL_FONT_STYLE_BOLD;
	sprintf(pPtr, "WWW.NAPS.MA");
	iRet = GoalPrnLine(xDocument, p++, pPtr, &xLine);
	CHECK(iRet >= 0, lblKO);
	}else {
		//printLogo(source);
		printLogo(ABORTED);
	}
	iRet = GoalPrnDocument(xDocument);              // Print document
	CHECK(iRet >= 0, lblKO);

	iRet = 1;
	goto lblEnd;
	// Errors treatment
	// ****************

	lblKO: GL_Dialog_Message(hGoal, NULL, "Processing Error printer", GL_ICON_ERROR, GL_BUTTON_VALID, 5 * 1000);
	goto lblEnd;

	lblEnd: if (xDocument)
		GoalDestroyDocument(&xDocument);

	CUTERMpaperFeed();


//	if (source == CLIENT) {
//
//		if ((memcmp(FicPara.Date_appel, Date_YYMMDDHHmmss2, 6) != 0) && (memcmp(heure, FicPara.Heure_appel, 4) > 0) && (FicPara.Top_TelAuto == 0)) {
//			Telium_Ttestall(0,300);
//			Imprime_ticket_teleAutoechou();
//		}
//	}


	return OK ;
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

	printLogo(source);


	/*************************************************************************/
	memcpy(data_time,StructInfoPayment.Transaction_DateTime,12) ;			// date/heure
	j += sprintf(pPtr+j, "\n%.2s/%.2s/%.2s						%.2s:%.2s",data_time+4,data_time+2,data_time,data_time+6,data_time+8);
	/************************************************************************/
	j += sprintf(pPtr+j, "\n%.10s",FicPara.Num_point_vente);				//Num point vente
	j += sprintf(pPtr+j, "\n\n%.8s",FicPara.Num_Terminal);					//Num Terminal
	j += sprintf(pPtr+j, "\n%.24s",FicPara.Merchant_Name);					//Enseigne comer?ant
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

	//Num?ro de la carte
	memset(Card_Nb,0,20);
	memcpy(Card_Nb,&StructInfoPayment.Holder_Card[1],StructInfoPayment.Holder_Card[0]);
	if(source==CLIENT)//231205
	{
		Ushort ik;

		for(ik=0;ik<strlen((char*)Card_Nb)-4;ik++)
			Card_Nb[ik] = '*';
	}
	j += sprintf(pPtr+j, "\n%s",Card_Nb);    								//Num carte
	j += sprintf(pPtr+j, "\n%.2s/%.2s",StructInfoPayment.Expired_Date+2,StructInfoPayment.Expired_Date);    //Date validit?

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
		j += sprintf(pPtr+j, "\n		Copie commer?ant");

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

//	Telium_File_t *printer1;
//	Ushort c;
//
//	printer1 = Telium_Fopen("PRINTER","w-");
//	if ( printer1 != NULL )
//	{
//		Telium_Fprintf(printer1,"\x0f");
//		for(c =0;c<n_car;c++)
//		{
//			Telium_Fprintf(printer1,"%02X",buffer[c]);
//			if(c!=0)
//			{
//				if(!(c%20))
//					Telium_Fprintf(printer1,"\n");
//			}
//		}
//		Telium_Fprintf(printer1,"\x1B""E""\n");
//		Telium_Ttestall ( PRINTER , 300 );
//		Telium_Fclose(printer1);
//	}

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
	char	pPtr1[600];
	char	pPtr[600];
	Ushort		j=0;

	uchar	montant[13]  ;
	uchar	affiche_mont[14];
	uchar	status;
	DATE date;
	bool bContinue = TRUE, bContinue2 = TRUE;
	int iHeader, iFooter;
	int iRet;

	uchar langue=FRAN;
	uchar source=COMMERCANT;
	T_GL_HWIDGET hDocument = NULL;
	T_GL_HWIDGET xDocument = NULL;
	ST_PRN_LINE xLine;
	byte p; // Printer line index
	T_GL_HGRAPHIC_LIB hGoal = NULL;
	//int iRet;


	StructInfoPayment.waitpuce = 0;
	/*Aff_indicNO_OUI(FicPara.Langue);*/
	CUMORE_CMI_AFFICHE_MESSAGE(94,32,1000);
	//Aff_mess(FicPara.Langue,94,0,0,0,0);
	//Aff_mess(FicPara.Langue,32,1,0,0,0);

	memset(pPtr, 0x00, sizeof(pPtr));
	memset(pPtr1, 0x00, sizeof(pPtr1));
	memset(montant, 0x00, 13);
	memset(affiche_mont, 0x00, 14);
	j = 0;

	printLogo(CLIENT);


	p = 0;                                            // Create document
	hDocument = GoalCreateDocument(APEMV_UI_GoalHandle(),GL_ENCODING_ISO_8859_1);
	CHECK(hDocument != NULL, lblKO);
	// *** Font scale capability ***
	xLine = xPrinter;
	xLine.xFont.eStyle = GL_FONT_STYLE_BOLD;
	xLine.xFont.eScale = GL_SCALE_XLARGE;
	xLine.eTextAlign = GL_ALIGN_CENTER;
	xLine.eBackAlign = GL_ALIGN_CENTER;


	if ((ind_rec == 0))
		sprintf(pPtr, "\nFIN DE JOURNEE");
	else {
		sprintf(pPtr, "\nTOTAL");
		//ind_rec = 1;
	}
	iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
	CHECK(iRet >= 0, lblKO);




//	xLine.xFont.eStyle = GL_FONT_STYLE_NORMAL;
//	xLine.xFont.eScale = GL_SCALE_LARGE;
//	xLine.eTextAlign = GL_ALIGN_LEFT;
//	xLine.eBackAlign = GL_ALIGN_LEFT;
//	xLine.xFont.eStyle = GL_FONT_STYLE_BOLD;
	xLine.eBackAlign = GL_ALIGN_LEFT;
	xLine.eTextAlign = GL_ALIGN_LEFT;
	xLine.xFont.eScale = GL_SCALE_LARGE;
	xLine.xFont.eStyle = GL_FONT_STYLE_NORMAL;

	Telium_Read_date(&date);
	sprintf(pPtr, "\n%.2s/%.2s/%.2s                            %.2s:%.2s:%.2s",
			date.day, date.month, date.year, date.hour, date.minute,
			date.second);
	iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
	CHECK(iRet >= 0, lblKO);
	sprintf(pPtr+j, "\n");
	iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
	CHECK(iRet >= 0, lblKO);
	sprintf(pPtr+j, "%.10s",FicPara.Num_point_vente);   //Num point vente
	iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
	CHECK(iRet >= 0, lblKO);
	sprintf(pPtr+j, "%.8s",FicPara.Num_Terminal);       //Num Terminal
	iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
	CHECK(iRet >= 0, lblKO);
	sprintf(pPtr + j, "%.24s", FicPara.Merchant_Name);   //Enseigne commercial
	iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
	CHECK(iRet >= 0, lblKO);
	sprintf(pPtr + j, "%.13s", FicPara.Merchant_City);     //Ville
	iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
	CHECK(iRet >= 0, lblKO);
	if(FicTra.nombre_tra == 0){
		xLine.xFont.eStyle = GL_FONT_STYLE_BOLD;
		xLine.xFont.eScale = GL_SCALE_XLARGE;
		xLine.eTextAlign = GL_ALIGN_CENTER;
		xLine.eBackAlign = GL_ALIGN_CENTER;
		sprintf(pPtr+j, "\n  AUCUNE TRANSACTION");
		iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
		CHECK(iRet >= 0, lblKO);
		sprintf(pPtr+j, "\n");
		iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
		CHECK(iRet >= 0, lblKO);
		}
	iRet = GoalPrnDocument(hDocument);              // Print document
	CHECK(iRet >= 0, lblKO);
	Calcul_total(ind_rec);
	xLine.eBackAlign = GL_ALIGN_CENTER;
	xLine.eTextAlign = GL_ALIGN_CENTER;
	xLine.xFont.eScale = GL_SCALE_LARGE;

	p = 0;                                            // Create document
	xDocument = GoalCreateDocument(APEMV_UI_GoalHandle(),GL_ENCODING_ISO_8859_1);
	CHECK(xDocument != NULL, lblKO);


	sprintf(pPtr1, "-------------------------------------");
	xLine.xFont.eScale = GL_SCALE_XLARGE;
	iRet = GoalPrnLine(xDocument, p++, pPtr1, &xLine);
	CHECK(iRet >= 0, lblKO);
	xLine.xFont.eScale = GL_SCALE_LARGE;
	xLine.xFont.eStyle = GL_FONT_STYLE_BOLD;

	sprintf(pPtr1, "Conservez-moi, je peux ?tre utile");
	iRet = GoalPrnLine(xDocument, p++, pPtr1, &xLine);
	CHECK(iRet >= 0, lblKO);

	if (langue == ENG) {
		sprintf(pPtr1, "TICKET TO SAVE");
		if (source == CLIENT)
			sprintf(pPtr1, "Client Copy");
		else
			sprintf(pPtr1, "Merchant Copy");
	} else {
		sprintf(pPtr1, "TICKET A CONSERVER");
		if (source == CLIENT)
			sprintf(pPtr1, "Copie client");
		else {

			sprintf(pPtr1, "Copie commer?ant", 135);
		}
	}
	iRet = GoalPrnLine(xDocument, p++, pPtr1, &xLine);
	CHECK(iRet >= 0, lblKO);

	xLine.xFont.eStyle = GL_FONT_STYLE_BOLD;
	sprintf(pPtr1, "\nWWW.NAPS.MA");
	iRet = GoalPrnLine(xDocument, p++, pPtr1, &xLine);
	CHECK(iRet >= 0, lblKO);


	iRet = GoalPrnDocument(xDocument);              // Print document
	CHECK(iRet >= 0, lblKO);


	iRet = 1;
	goto lblEnd;
	// Errors treatment
	// ****************

	lblKO: GL_Dialog_Message(hGoal, NULL, "Processing Error", GL_ICON_ERROR,
			GL_BUTTON_VALID, 5 * 1000);
	goto lblEnd;

	lblEnd: if (xDocument)
		GoalDestroyDocument(&hDocument);

	CUTERMpaperFeed();
	return OK;
}

uchar JournalEdit(void) {

	char pPtr[3000];
	char Titre[20];
	char pPtr1[100];
	Ushort i, k;
	Ushort nombre_tra;
	int j, jk;
	uchar trouve = KO;
	uchar etat = KO;
	uchar data_time[13];
	uchar montant[13];
	uchar Holder_Card[20];
	uchar iso2[38];
	uchar l;
	lignetra transac;
	DATE date;
	int ik;
	uchar langue = FRAN;
	uchar source = COMMERCANT;
	int iRet;

	StructInfoPayment.waitpuce = 0;
	CUMORE_CMI_AFFICHE_MESSAGE(94, 32, 1000);

	memset(pPtr, 0x00, sizeof(pPtr));
	memset(data_time, 0, 13);
	memset(iso2, 0, 38);

	Telium_Read_date(&date);

//	Header  //
	j = 0;

	printLogo(CLIENT);

	byte p; // Printer line index
	p = 0;
	ST_PRN_LINE xLine;
	xLine = xPrinter;

	xLine.eBackAlign = GL_ALIGN_CENTER;
	xLine.eTextAlign = GL_ALIGN_CENTER;
	xLine.xFont.eScale = GL_SCALE_XLARGE;
	xLine.xFont.eStyle = GL_FONT_STYLE_BOLD;

	T_GL_HWIDGET hDocument = NULL;
	hDocument = GoalCreateDocument(APEMV_UI_GoalHandle(),
			GL_ENCODING_ISO_8859_1);

	T_GL_HWIDGET xDocument = NULL;
	xDocument = GoalCreateDocument(APEMV_UI_GoalHandle(),
			GL_ENCODING_ISO_8859_1);

	sprintf(Titre, "\nJOURNAL");
	GoalPrnLine(hDocument, p, Titre, &xLine);
	GoalPrnDocument(hDocument);              // Print document
	GoalDestroyDocument(&hDocument);

	Telium_Read_date(&date);
	j += sprintf(pPtr + j,
			"\n %.2s/%.2s/%.2s                          %.2s:%.2s:%.2s\n",
			date.day, date.month, date.year, date.hour, date.minute,
			date.second);
	j += sprintf(pPtr + j, "\n %.15s", FicPara.Num_point_vente);
	j += sprintf(pPtr + j, "\n %.8s", FicPara.Num_Terminal);
	j += sprintf(pPtr + j, "\n %.24s", FicPara.Merchant_Name);
	j += sprintf(pPtr + j, "\n %.13s", FicPara.Merchant_City);

	nombre_tra = FicTra.nombre_tra;
	Os__xprintaa("FicTra.nombre_tra");
			Os__xprintd(FicTra.nombre_tra);
	if (nombre_tra == 0) {
		etat = OK;
		if (new_printer(pPtr, etat) != OK) {
			//MessageWithDelay("return KO", NULL, 1000);
			return KO ;
		}
		xLine.eBackAlign = GL_ALIGN_CENTER;
			xLine.eTextAlign = GL_ALIGN_CENTER;
			xLine.xFont.eScale = GL_SCALE_XLARGE;
			xLine.xFont.eStyle = GL_FONT_STYLE_BOLD;

			T_GL_HWIDGET hDocument = NULL;
			hDocument = GoalCreateDocument(APEMV_UI_GoalHandle(),
					GL_ENCODING_ISO_8859_1);

			T_GL_HWIDGET xDocument = NULL;
			xDocument = GoalCreateDocument(APEMV_UI_GoalHandle(),
					GL_ENCODING_ISO_8859_1);

			sprintf(Titre, " \n AUCUNE TRANSACTION");
			GoalPrnLine(hDocument, p, Titre, &xLine);
			GoalPrnDocument(hDocument);              // Print document
			GoalDestroyDocument(&hDocument);

		//return OK ;
	}

	i = 0;
	etat = OK;

	while (i < nombre_tra) {
		//MessageWithDelay("0000000", NULL, 1000);

		trouve = OK;
		read_trans_index(i, &transac);
		memset(Holder_Card, 0, 20);

		switch (transac.CodeFunction) {
		case SALECODE :
		case CASHADVANCESCODE :
		case PAIEMENT_POURBOIRE :
			if (transac.CodeFunction == PAIEMENT_POURBOIRE)
				j += sprintf(pPtr + j, "\n\n DEBIT POURBOIRE");
			else if (transac.Flag_Dem_Aut)
				j += sprintf(pPtr + j, "\n\n DEBIT");
			else
				j += sprintf(pPtr + j, "\n\n DEBIT");

			//MessageWithDelay("11111111111", NULL, 1000);
			break;

		case REFUNDCODE:
			j += sprintf(pPtr + j, "\n\n CREDIT ");
			break;

		case VOIDCODE:
			if (transac.Flag_Dem_Aut)
				j += sprintf(pPtr + j, "\n\n ANNULATION ");
			else
				j += sprintf(pPtr + j, "\n\n ANNULATION");
			break;

		case PREAUTHCOMPCODE:
			if (transac.Preauto_indic_completion == PRE_VAL_1_TRS)
				j += sprintf(pPtr + j, "\n\n COMPLEMENT PRE-AUTO ");
			else if (transac.Top_Forcage == TOP_FOR_ONLINE)
				j += sprintf(pPtr + j, "\n\n COMPLEMENT PRE-AUTO ");
			else
				j += sprintf(pPtr + j, "\n\n COMPLEMENT PRE-AUTO ");
			//MessageWithDelay("2222222", NULL, 1000);
			break;
		default:
			j += sprintf(pPtr + j, "\n\n");
			//MessageWithDelay("333333", NULL, 1000);
			break;
		}

		if (transac.Entry_Mode == TRS_MAN) {
			//MessageWithDelay("44444444", NULL, 1000);
			memcpy(Holder_Card, &transac.Compressed_Iso2[1], 19);

		} else if (transac.Entry_Mode == TRS_SCT) {
			//MessageWithDelay("************", NULL, 1000);
			//bcd_asc(iso2, &transac.Compressed_Iso2[1], 20);
			memcpy(iso2, &transac.Compressed_Iso2[1], 19);
//			//j += sprintf(iso2, "%s", &transac.Compressed_Iso2[1]);

			for (jk = 0;
					((jk < 20) && (iso2[jk] != 'd') && (iso2[jk] != 'D')
							&& (iso2[jk] != '=')); jk++)
				;

			memcpy(Holder_Card, iso2, jk);
			//memcpy(Holder_Card, &transac.Compressed_Iso2[1], 19);

		} else {
			//MessageWithDelay("5555555555", NULL, 1000);
			bcd_asc(iso2, &transac.Compressed_Iso2[1], 20);
			//eft30printhexKamal(iso2,20);

			for (l = 0;
					((l < 20) && (iso2[l] != 'd') && (iso2[l] != 'D')
							&& (iso2[l] != '=')); l++)
				;
			memcpy(Holder_Card, iso2, l);

		}

		for (ik = 6; ik < strlen((char*) Holder_Card) - 4; ik++)
			Holder_Card[ik] = '*';
		j += sprintf(pPtr + j, "\n %.19s", Holder_Card);
		//MessageWithDelay("6666666", NULL, 1000);
		memcpy(data_time, transac.Transaction_DateTime, 12);
		j += sprintf(pPtr + j,
				"\n %.2s/%.2s/%.2s                         %.2s:%.2s:%.2s",
				data_time + 4, data_time + 2, data_time, data_time + 6,
				data_time + 8, data_time + 10);
		//MessageWithDelay("77777777", NULL, 1000);
		j += sprintf(pPtr + j, "\n %.6s", transac.STAN);
		j += sprintf(pPtr + j, "\n %.6s  ", transac.Num_sequence);
		for (k = 0; k < NB_MAX_DEV_ACC; k++)           //l'indice de la 2?me dim
				{
			if (!memcmp(Fic_Devise_Acc.TabDevise[k].code_num,
					transac.NumCurrency, 3))
				break;
		}

		//if (transac.CodeFunction == PAIEMENT_POURBOIRE)
		if (transac.CodeFunction == PAIEMENT_POURBOIRE
					|| (transac.CodeFunction == VOIDCODE && transac.MontantPourboireInterchange != 0 )
					|| (transac.StatusAnnulAdjust && transac.MontantPourboireInterchange != 0 )
					) //Pourboire
			{
			j += sprintf(pPtr + j, "\n %d,%d%d %s", (transac.Transaction_Amount + transac.MontantPourboireInterchange)/100,
					(transac.Transaction_Amount + transac.MontantPourboireInterchange / 10) % 10,
					(transac.Transaction_Amount + transac.MontantPourboireInterchange) % 10,
					Fic_Devise_Acc.TabDevise[k].code_alph);
		}

		else {
			j += sprintf(pPtr + j, "\n %d,%d%d %s", transac.Transaction_Amount / 100,
									(transac.Transaction_Amount / 10) % 10,
									transac.Transaction_Amount % 10,
									Fic_Devise_Acc.TabDevise[k].code_alph);

		}

		if (j >= 2800) {
//			Os__xprintd(FreeSpace());
			if (new_printer(pPtr, etat) != OK) {
				return KO ;
			}
			trouve = KO;
			j = 0;
			memset(pPtr, 0, sizeof(pPtr));
		}

		i++;
	}

	p = 0;                                            // Create document
	if (nombre_tra == 0)
		sprintf(pPtr1, "\n-------------------------------------");
	else
		sprintf(pPtr1, "\n\n-------------------------------------");

	xLine.xFont.eScale = GL_SCALE_XLARGE;

	iRet = GoalPrnLine(xDocument, p++, pPtr1, &xLine);

	xLine.xFont.eScale = GL_SCALE_LARGE;
	xLine.xFont.eStyle = GL_FONT_STYLE_BOLD;
	xLine.eBackAlign = GL_ALIGN_CENTER;
	xLine.eTextAlign = GL_ALIGN_CENTER;

	sprintf(pPtr1, "Conservez-moi, je peux ?tre utile");
	iRet = GoalPrnLine(xDocument, p++, pPtr1, &xLine);

	if (langue == ENG) {
		sprintf(pPtr1, "TICKET TO SAVE");
		if (source == CLIENT)
			sprintf(pPtr1, "Client Copy");
		else
			sprintf(pPtr1, "Merchant Copy");
	} else {
		sprintf(pPtr1, "TICKET A CONSERVER");
		if (source == CLIENT)
			sprintf(pPtr1, "Copie client");
		else {

			sprintf(pPtr1, "Copie commer?ant");
		}
	}
	GoalPrnLine(xDocument, p++, pPtr1, &xLine);

	xLine.xFont.eStyle = GL_FONT_STYLE_BOLD;
	sprintf(pPtr1, "\nWWW.NAPS.MA");

	if (trouve == OK) {
		if (new_printer(pPtr, etat) != OK) {
			return KO ;
		}
	}

	GoalPrnLine(xDocument, p++, pPtr1, &xLine);
	GoalPrnDocument(xDocument);
	GoalDestroyDocument(&xDocument);
	CUTERMpaperFeed();
	return OK ;
}

uchar new_printer(char* buffer, uchar etat) {


	byte p; // Printer line index
	p = 0;
	ST_PRN_LINE xLine;
	xLine = xPrinter;

	if(etat==OK){
	xLine.eBackAlign = GL_ALIGN_LEFT;
	xLine.eTextAlign = GL_ALIGN_LEFT;
	xLine.xFont.eScale = GL_SCALE_LARGE;
	xLine.xFont.eStyle = GL_FONT_STYLE_NORMAL;
	}
	else{
	xLine.eBackAlign = GL_ALIGN_CENTER;
	xLine.eTextAlign = GL_ALIGN_CENTER;
	xLine.xFont.eScale = GL_SCALE_LARGE;
	xLine.xFont.eStyle = GL_FONT_STYLE_BOLD;
	}

	T_GL_HWIDGET hDocument = NULL;
	hDocument = GoalCreateDocument(APEMV_UI_GoalHandle(),GL_ENCODING_ISO_8859_1);
	GoalPrnLine(hDocument, p, buffer, &xLine);
	GoalPrnDocument(hDocument);              // Print document
	GoalDestroyDocument(&hDocument);
	free(hDocument);

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
	char		pPtr[3000];
	char		pPtr1[100];
	char        Titre[20];
	Ushort 		i,k;
	Ushort		nbr_preauto;
	int			j;
	int         ii=0;
	uchar       type = KO;
	uchar		trouve = KO;
	uchar		etat = OK;
	uchar		data_time[13];
	uchar	  	montant[13];
	uchar		Holder_Card[20];
	uchar    	iso2[38];
	uchar		l;
	lignePreAut	preauto ;
	DATE date;
	int ik;
	uchar langue=FRAN;
	uchar source=COMMERCANT;

	bool bContinue = TRUE, bContinue2 = TRUE;
	int iHeader, iFooter;
	int iRet;

	T_GL_HWIDGET xDocument = NULL;
	T_GL_HWIDGET hDocument = NULL;
	ST_PRN_LINE xLine;
	byte p; // Printer line index
	T_GL_HGRAPHIC_LIB hGoal = NULL;
	//int iRet;

	CUMORE_CMI_AFFICHE_MESSAGE(94, 32, 1000);
	printLogo(CLIENT);

	p = 0;                                            // Create document
	hDocument = GoalCreateDocument(APEMV_UI_GoalHandle(),GL_ENCODING_ISO_8859_1);
	xDocument = GoalCreateDocument(APEMV_UI_GoalHandle(),GL_ENCODING_ISO_8859_1);

	// *** Font scale capability ***
	xLine = xPrinter;
	xLine.xFont.eStyle = GL_FONT_STYLE_BOLD;
	xLine.xFont.eScale = GL_SCALE_XLARGE;
	xLine.eTextAlign = GL_ALIGN_CENTER;
	xLine.eBackAlign = GL_ALIGN_CENTER;

	sprintf(Titre, "\nJOURNAL PRE AUTO");
	GoalPrnLine(hDocument, p, Titre, &xLine);
	GoalPrnDocument(hDocument);              // Print document
	GoalDestroyDocument(&hDocument);

	StructInfoPayment.waitpuce = 0;


	memset(pPtr,0x00,sizeof(pPtr)) ;
	memset(data_time,0,13);
	memset(iso2,0,38);


	j=0;
	xLine.xFont.eStyle = GL_FONT_STYLE_NORMAL;
	xLine.xFont.eScale = GL_SCALE_LARGE;
	xLine.eTextAlign = GL_ALIGN_LEFT;
	xLine.eBackAlign = GL_ALIGN_LEFT;
	Telium_Read_date(&date);
	j += sprintf(pPtr + j, "\n%.2s/%.2s/%.2s                            %.2s:%.2s:%.2s",
			date.day,date.month, date.year, date.hour, date.minute, date.second);

	j += sprintf(pPtr + j, "\n%.15s",FicPara.Num_point_vente);
	j += sprintf(pPtr + j, "\n%.8s",FicPara.Num_Terminal);
	j += sprintf(pPtr + j, "\n%.24s",FicPara.Merchant_Name);
	j += sprintf(pPtr + j, "\n%.13s",FicPara.Merchant_City);

	nbr_preauto = FicPreauth.nombre_tra ;
	if(nbr_preauto==0 && etat == OK)
	{
		type = OK;
		j += sprintf(pPtr + j, "\n\n   PAS DE PRE-AUTORISATION");
		if (new_printer(pPtr, type) != OK) {
			return KO ;
		}
		//CUTERMpaperFeed();
		etat = KO;
		return OK;
	}

	i=0 ;
	type = OK;
	while(i<nbr_preauto)
	{
		trouve=OK ;
		read_preauto_index( i ,&preauto) ;
		memset(Holder_Card,0,20);

		//Num Carte			Num Sequ
		if(preauto.Entry_Mode == TRS_MAN)
 		{
 			memcpy(Holder_Card,&preauto.Compressed_Iso2[1],19);
 		}
		else if(preauto.Entry_Mode == TRS_SCT)
		{
			sprintf(iso2,"%s",&preauto.Compressed_Iso2[1]);

			for(ii=0;((ii<20)&&(iso2[ii] !='d')&&(iso2[ii] !='D')&&(iso2[ii] !='='));ii++);
			memcpy(Holder_Card,iso2,ii);
			StructInfoPayment.Holder_Card[0] = ii;
		}
		else
 		{

			bcd_asc(iso2,&preauto.Compressed_Iso2[1],20);

  			for(l=0;((l<20)&&(iso2[l] !='d')&&(iso2[l] !='D')&&(iso2[l] !='='));l++);
   			memcpy(Holder_Card,iso2,l);
		}

		for (ik = 6; ik < strlen((char*) Holder_Card) - 4; ik++)
			Holder_Card[ik] = '*';

		j += sprintf(pPtr + j, "\n\n%.19s", Holder_Card);
		memcpy(data_time, preauto.Transaction_DateTime, 12);
		j += sprintf(pPtr + j,
				"\n%.2s/%.2s/%.2s                            %.2s:%.2s:%.2s",
				data_time + 4, data_time + 2, data_time, data_time + 6,
				data_time + 8, data_time + 10);

        j += sprintf(pPtr + j, "\n%.6s                                %.6s",preauto.STAN,preauto.Num_sequence);
		memset(montant,0,13);

		for(k=0;k<NB_MAX_DEV_ACC;k++)            //l'indice de la 2?me dim
		{
			if(!memcmp(Fic_Devise_Acc.TabDevise[k].code_num,preauto.NumCurrency,3))
				break;
		}

		xLine.eTextAlign = GL_ALIGN_CENTER;
		xLine.eBackAlign = GL_ALIGN_CENTER;
		j += sprintf(pPtr + j, "\n                 %d,%d%d %s",
				preauto.Transaction_Amount/100,
				(preauto.Transaction_Amount/10)%10,
				preauto.Transaction_Amount%10,
				Fic_Devise_Acc.TabDevise[k].code_alph);

		j += sprintf(pPtr + j, "\n                   %d,%d%d %s",
				preauto.Montant_Accomp/100,
				(preauto.Montant_Accomp/10)%10,
				preauto.Montant_Accomp%10,
				Fic_Devise_Acc.TabDevise[k].code_alph);


		xLine.eTextAlign = GL_ALIGN_LEFT;
		xLine.eBackAlign = GL_ALIGN_LEFT;
		if(preauto.CodeFunction == VOID_PREAUTO_TOTAL_CODE)
			j += sprintf(pPtr + j, "\n_%s_","ANNULATION TOTALE");
		else if(preauto.CodeFunction == VOID_PREAUTO_PARTIEL_CODE)
			j += sprintf(pPtr + j, "\n_%s_","ANNULATION PARTIELLE");
		else if(preauto.StatusAnnulAdjust == 1)
			j += sprintf(pPtr + j, "\n_%s_","PRE AUTO ANNULEE");
		else if(preauto.Indic_Accomp == 1)
			j += sprintf(pPtr + j, "\n_%s_","PRE AUTO ACCOMPLIE");
		else	//1.11.62 R.TARIK 151217
			j += sprintf(pPtr + j, "\n_%s_","PRE AUTORISATION");


//		iRet = GoalPrnLine(hDocument, p++, "   ", &xLine);
//		CHECK(iRet >= 0, lblKO);
		if (j >= 2800) {


			if (new_printer(pPtr, type) != OK) {
				return KO ;
			}
			trouve = KO;
			j = 0;
			memset(pPtr, 0, sizeof(pPtr));
		}

  		i++ ;
	}


	xLine.eBackAlign = GL_ALIGN_CENTER;
	xLine.eTextAlign = GL_ALIGN_CENTER;
	//xLine.xFont.eStyle = GL_FONT_STYLE_NORMAL;
	xLine.xFont.eScale = GL_SCALE_LARGE;
	p = 0;
	sprintf(pPtr1, "\n\n-------------------------------------");
	xLine.xFont.eScale = GL_SCALE_XLARGE;
	iRet = GoalPrnLine(xDocument, p++, pPtr1, &xLine);

	xLine.xFont.eScale = GL_SCALE_LARGE;
	xLine.xFont.eStyle = GL_FONT_STYLE_BOLD;

	sprintf(pPtr1, "Conservez-moi, je peux ?tre utile");
	iRet = GoalPrnLine(xDocument, p++, pPtr1, &xLine);

	if (langue == ENG) {
		sprintf(pPtr1, "TICKET TO SAVE");
		if (source == CLIENT)
			sprintf(pPtr1, "Client Copy");
		else
			sprintf(pPtr1, "Merchant Copy");
	} else {
		sprintf(pPtr1, "TICKET A CONSERVER");
		if (source == CLIENT)
			sprintf(pPtr1, "Copie client");
		else {

			sprintf(pPtr1, "Copie commer?ant");
		}
	}
	iRet = GoalPrnLine(xDocument, p++, pPtr1, &xLine);

	sprintf(pPtr1, "\nWWW.NAPS.MA");
	iRet = GoalPrnLine(xDocument, p++, pPtr1, &xLine);


	if (trouve == OK) {
		if (new_printer(pPtr, type) != OK) {
			return KO ;
		}
	}

	GoalPrnDocument(xDocument);
	GoalDestroyDocument(&xDocument);
	CUTERMpaperFeed();
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

	printLogo(CLIENT);

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

	printLogo(CLIENT);


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

	printLogo(CLIENT);

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

	printLogo(CLIENT);

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

	printLogo(CLIENT);

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
uchar	ImprimeTraComp(uchar langue,uchar source,uchar dup,lignetra		*transac)//badr 300620
{
	char		pPtr[1500];
	char buff1[30], buff2[30];

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
	char var[60];
	StructInfoPayment.waitpuce = 0;
	/*Aff_indicNO_OUI(FicPara.Langue);*/
	int iRet;
	T_GL_HWIDGET hDocument = NULL;
	T_GL_HWIDGET xDocument = NULL;
	ST_PRN_LINE xLine;
	byte p; // Printer line index
	T_GL_HGRAPHIC_LIB hGoal = NULL;
	//int iRet;



	CUMORE_CMI_AFFICHE_MESSAGE(94,32,1000);
	//Aff_mess(FicPara.Langue,94,0,0,0,0);
	//Aff_mess(FicPara.Langue,32,1,0,0,0);

	memset(pPtr,0x00,sizeof(pPtr)) ;
	memset(affich_Cs,0x00,11) ;
	memset(montant,0x00,13) ;
	memset(affiche_mont,0x00,14) ;


	//CMI Gras
	//j=0 ;
	//j += sprintf(pPtr+j, "<#f=%08lx#>",(UL)SYSTEM_FONT_8x14_ID);//Font Gra
	//j += sprintf(pPtr+j, "\x1B""@");//Font Normal
//	j += sprintf(pPtr+j, "\x1B""E");//Font Gra
	p = 0;                                            // Create document
	hDocument = GoalCreateDocument(APEMV_UI_GoalHandle(),
			GL_ENCODING_ISO_8859_1);
	CHECK(hDocument != NULL, lblKO);
	xDocument = GoalCreateDocument(APEMV_UI_GoalHandle(), GL_ENCODING_ISO_8859_1);
	CHECK(xDocument != NULL, lblKO);
	// *** Font scale capability ***
	xLine = xPrinter;
	xLine.xFont.eStyle = GL_FONT_STYLE_BOLD;
	xLine.xFont.eScale = GL_SCALE_XLARGE;
	xLine.eTextAlign = GL_ALIGN_CENTER;
	xLine.eBackAlign = GL_ALIGN_CENTER;

	printLogo(CLIENT);

	//DUPLICATA Gras	||	PRE-AUTORISATION	||	VAL.TRX.PRE-AUTO	||
	if(dup){
		sprintf(pPtr, "\nDUPLICATA");
		iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
		CHECK(iRet >= 0, lblKO);

	}
	if(StructInfoPayment.CodeFunction == PREAUTHCODE){
		sprintf(pPtr, "\nPRE-AUTORISATION");
		iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
		CHECK(iRet >= 0, lblKO);
	}else if(StructInfoPayment.CodeFunction == PREAUTHCOMPCODE){
		sprintf(pPtr, "\nVAL.TRX.PRE-AUTO");
		iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
		CHECK(iRet >= 0, lblKO);
	}
	xLine.eBackAlign = GL_ALIGN_CENTER;
	xLine.eTextAlign = GL_ALIGN_CENTER;
	xLine.xFont.eScale = GL_SCALE_XLARGE;
	sprintf(pPtr, "-------------------------------------");
	iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
	CHECK(iRet >= 0, lblKO);

	xLine.xFont.eStyle = GL_FONT_STYLE_NORMAL;
	xLine.xFont.eScale = GL_SCALE_LARGE;
	xLine.eTextAlign = GL_ALIGN_LEFT;
	xLine.eBackAlign = GL_ALIGN_LEFT;

	memcpy(data_time, StructInfoPayment.Transaction_DateTime, 12);
	sprintf(pPtr, "%.2s/%.2s/%.2s                            %.2s:%.2s:%.2s",
			data_time + 4, data_time + 2, data_time, data_time + 6,
			data_time + 8, data_time + 10);
	iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
	CHECK(iRet >= 0, lblKO);

	xLine.xFont.eStyle = GL_FONT_STYLE_BOLD;
	xLine.eTextAlign = GL_ALIGN_CENTER;
	xLine.eBackAlign = GL_ALIGN_CENTER;

int ib;
	for (ib = 21; (ib > 6) && (FicPara.Merchant_Name[ib] == ' '); ib--);
	FicPara.Merchant_Name[ib + 1] = 0;

	sprintf(buff1, "%10s", FicPara.Merchant_Name);
	sprintf(buff2, "%10s", FicPara.Merchant_City);

	removeRightSpaces(buff1);
	removeRightSpaces(buff2);

	if (strlen(buff1) + strlen(buff2) < 19)
		sprintf((PC) pPtr, "%s - %s\n", buff1, buff2);
	else
		sprintf((PC) pPtr, "%s\n%s\n", buff1, buff2);

	//sprintf((PC)Buffer, "%s - %s\n",buff1,buff2);
	iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
	CHECK(iRet >= 0, lblKO);

	xLine.xFont.eStyle = GL_FONT_STYLE_NORMAL;
	xLine.xFont.eScale = GL_SCALE_XLARGE;
	sprintf(pPtr, "-------------------------------------");
	iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
	CHECK(iRet >= 0, lblKO);

	xLine.eBackAlign = GL_ALIGN_LEFT;
	xLine.xFont.eScale = GL_SCALE_LARGE;
	xLine.eTextAlign = GL_ALIGN_LEFT;

	//Nom du porteur
	if (StructInfoPayment.Appli_label[0] != 0) {
		sprintf(pPtr, "%s", StructInfoPayment.Appli_aid + 1);
		iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
		CHECK(iRet >= 0, lblKO);
		sprintf(pPtr, "%.16s", StructInfoPayment.Appli_label);
		iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
		CHECK(iRet >= 0, lblKO);
		sprintf(pPtr, "%.24s", StructInfoPayment.Nom_Prenom);
		iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
		CHECK(iRet >= 0, lblKO);
	} else {
		sprintf(pPtr, "%.24s", StructInfoPayment.Nom_Prenom);
		iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
		CHECK(iRet >= 0, lblKO);
	}


	//Num?ro de la carte
	memset(Card_Nb,0,20);
	memcpy(Card_Nb,&StructInfoPayment.Holder_Card[1],StructInfoPayment.Holder_Card[0]);

	Ushort ik;
	if (source == COMMERCANT) {
		//		Card_Nb[strlen(Card_Nb)-1] = '*';
		//		Card_Nb[strlen(Card_Nb)-2] = '*';
		//		Card_Nb[strlen(Card_Nb)-3] = '*';
		//		Card_Nb[strlen(Card_Nb)-4] = '*';

		for (ik = 0; ik < strlen((char*) Card_Nb) - 4; ik++)
			Card_Nb[ik] = '*';
	} else {
		for (ik = 6; ik < strlen((char*) Card_Nb) - 4; ik++)
			Card_Nb[ik] = '*';
	}
	sprintf(pPtr, "%s", Card_Nb);
	iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
	CHECK(iRet >= 0, lblKO);

	//MM/AA	+	CARTE ETRANGERE||CARTE NATIONALE
	if (StructInfoPayment.Origine_carte == CARTE_NATIONALE)
		sprintf(pPtr, "%.2s/%.2s CARTE NATIONALE",StructInfoPayment.Expired_Date + 2,StructInfoPayment.Expired_Date);
	else if (StructInfoPayment.Origine_carte == CARTE_DOMESTIQUE)
		sprintf(pPtr, "%.2s/%.2s CARTE DOMESTIQUE",StructInfoPayment.Expired_Date + 2,StructInfoPayment.Expired_Date);
	else if (StructInfoPayment.Origine_carte = CARTE_ETRANGATRS)
		sprintf(pPtr, "%.2s/%.2s CARTE ETRANGERE",StructInfoPayment.Expired_Date + 2,StructInfoPayment.Expired_Date);
	else
		sprintf(pPtr, "%.2s/%.2s CARTE ETRANGERE",StructInfoPayment.Expired_Date + 2,StructInfoPayment.Expired_Date);

	iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
	CHECK(iRet >= 0, lblKO);

	memcpy(affich_Cs, StructInfoPayment.SeviceCode, 3);
	//if(StructInfoPayment.forcage)
	if ((StructInfoPayment.forcage)
			|| (StructInfoPayment.Top_Forcage == TOP_FOR_REFFERAL))
		affich_Cs[3] = '1';
	else
		affich_Cs[3] = '0';
	memcpy(affich_Cs + 4, &lop_gest.NumListe[0], 4);
	if (StructInfoPayment.Entry_Mode == TRS_EMV)
		affich_Cs[8] = '1';
	else if (StructInfoPayment.Entry_Mode == TRS_MAN)
		affich_Cs[8] = '2';
	else
		affich_Cs[8] = '0';

	sprintf(affich_Cs + 9, "%.02d",
			FicInsReg.Tab_Ins_reg[StructInfoPayment.Type_carte].Code_Prefixe);
	sprintf(pPtr, "%.3s-%.1s-%.4s-%.1s-%.2s", affich_Cs, affich_Cs + 3,
			affich_Cs + 4, affich_Cs + 8, affich_Cs + 9);
	iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
	CHECK(iRet >= 0, lblKO);
//	}

//	iRet = GoalPrnLine(hDocument, p++, "    ", &xLine);
//	CHECK(iRet >= 0, lblKO);
	iRet = GoalPrnLine(hDocument, p++, "    ", &xLine);
	CHECK(iRet >= 0, lblKO);



	sprintf(pPtr, "%.15s", FicPara.Num_commerce);// badr
	xLine.eTextAlign = GL_ALIGN_RIGHT;
	xLine.eBackAlign = GL_ALIGN_RIGHT;
	iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
	xLine.eTextAlign = GL_ALIGN_LEFT;
	xLine.eBackAlign = GL_ALIGN_LEFT;
	strcpy(var, ":");
	sprintf(pPtr, "%*s", 29 + strlen(var), var);
	iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
	iRet = GoalPrnLine(hDocument, p++, "Num Commer?ant", &xLine);
	CHECK(iRet >= 0, lblKO);


	sprintf(pPtr, "%.8s", FicPara.Num_Terminal);		// badr
	xLine.eTextAlign = GL_ALIGN_RIGHT;
	xLine.eBackAlign = GL_ALIGN_RIGHT;
	iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
	xLine.eTextAlign = GL_ALIGN_LEFT;
	xLine.eBackAlign = GL_ALIGN_LEFT;
	strcpy(var, ":");
	sprintf(pPtr, "%*s", 29 + strlen(var), var);
	iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
	iRet = GoalPrnLine(hDocument, p++, "Num TPE", &xLine);
	CHECK(iRet >= 0, lblKO);

	//pre autorisation
	if(
			(StructInfoPayment.CodeFunction==PREAUTHCOMPCODE)
		&&	(StructInfoPayment.Preauto_Amount_Final > StructInfoPayment.Preauto_Amount_Initial +
														(StructInfoPayment.Preauto_Amount_Initial*15)/100)
	)
	{
		//Pre-autorisation
		sprintf(pPtr, "\nPre-autorisation");
		xLine.eBackAlign = GL_ALIGN_CENTER;
		xLine.eTextAlign = GL_ALIGN_CENTER;
		xLine.xFont.eStyle = GL_FONT_STYLE_BOLD;
		iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
		CHECK(iRet >= 0, lblKO);

		xLine.xFont.eStyle = GL_FONT_STYLE_NORMAL;
		//JJ/MM/AA	HH:MM:SS
		memcpy(data_time,StrPreAut.Transaction_DateTime,12);
		sprintf(pPtr,
				"%.2s/%.2s/%.2s                            %.2s:%.2s:%.2s",
				data_time + 4, data_time + 2, data_time, data_time + 6,
				data_time + 8, data_time + 10);
		xLine.eBackAlign = GL_ALIGN_LEFT;
		xLine.eTextAlign = GL_ALIGN_LEFT;
		iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
		CHECK(iRet >= 0, lblKO);

		//Montant

		iRet = GoalPrnLine(hDocument, p++, "    ", &xLine);
		CHECK(iRet >= 0, lblKO);

		xLine.xFont.eStyle = GL_FONT_STYLE_BOLD;
		memset(affiche_mont,0x00,14) ;
		long_asc (montant, 12,&StructInfoPayment.Transaction_Amount);
		RemoveLeftZeros(montant) ;
		Exp=StructInfoPayment.Devise_acc.Exposant-48;
		FormatAmount(montant,affiche_mont,Exp);


		sprintf(pPtr, "%.13s%.3s", affiche_mont,StructInfoPayment.Devise_acc.code_alph);
		xLine.eTextAlign = GL_ALIGN_RIGHT;
		xLine.eBackAlign = GL_ALIGN_RIGHT;
		iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
		xLine.eTextAlign = GL_ALIGN_LEFT;
		xLine.eBackAlign = GL_ALIGN_LEFT;
		strcpy(var, ":");
		sprintf(pPtr, "%*s", 29 + strlen(var), var);
		iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
		iRet = GoalPrnLine(hDocument, p++, "Montant", &xLine);
		CHECK(iRet >= 0, lblKO);

		xLine.xFont.eStyle = GL_FONT_STYLE_NORMAL;



		//Num transaction

		sprintf(pPtr, "%.6s", StructInfoPayment.Num_sequence);
		xLine.eTextAlign = GL_ALIGN_RIGHT;
		xLine.eBackAlign = GL_ALIGN_RIGHT;
		iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
		xLine.eTextAlign = GL_ALIGN_LEFT;
		xLine.eBackAlign = GL_ALIGN_LEFT;
		strcpy(var, ":");
		sprintf(pPtr, "%*s", 29 + strlen(var), var);
		iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
		iRet = GoalPrnLine(hDocument, p++, "Num Transaction", &xLine);
		CHECK(iRet >= 0, lblKO);


		//Num autorisation
		sprintf(pPtr, "%.6s", StructInfoPayment.Approval_Code);
		xLine.eTextAlign = GL_ALIGN_RIGHT;
		xLine.eBackAlign = GL_ALIGN_RIGHT;
		iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
		xLine.eTextAlign = GL_ALIGN_LEFT;
		xLine.eBackAlign = GL_ALIGN_LEFT;
		strcpy(var, ":");
		sprintf(pPtr, "%*s", 29 + strlen(var), var);
		iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
		iRet = GoalPrnLine(hDocument, p++, "Num Autorisation", &xLine);
		CHECK(iRet >= 0, lblKO);

		//Stan
		sprintf(pPtr, "%.6s", StructInfoPayment.STAN);
		xLine.eTextAlign = GL_ALIGN_RIGHT;
		xLine.eBackAlign = GL_ALIGN_RIGHT;
		iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
		xLine.eTextAlign = GL_ALIGN_LEFT;
		xLine.eBackAlign = GL_ALIGN_LEFT;
		strcpy(var, ":");
		sprintf(pPtr, "%*s", 29 + strlen(var), var);
		iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
		iRet = GoalPrnLine(hDocument, p++, "Num STAN", &xLine);
		CHECK(iRet >= 0, lblKO);


		//Complement

		sprintf(pPtr, "\nComplement");
		xLine.xFont.eStyle = GL_FONT_STYLE_BOLD;
		xLine.eBackAlign = GL_ALIGN_CENTER;
		xLine.eTextAlign = GL_ALIGN_CENTER;
		iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
		CHECK(iRet >= 0, lblKO);

		//JJ/MM/AA	HH:MM:SS

		memcpy(data_time, StructInfoPayment.Transaction_DateTime, 12);
		sprintf(pPtr,
				"%.2s/%.2s/%.2s                            %.2s:%.2s:%.2s",
				data_time + 4, data_time + 2, data_time, data_time + 6,
				data_time + 8, data_time + 10);
		xLine.eBackAlign = GL_ALIGN_LEFT;
		xLine.eTextAlign = GL_ALIGN_LEFT;
		xLine.xFont.eStyle = GL_FONT_STYLE_NORMAL;
		iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
		CHECK(iRet >= 0, lblKO);
		iRet = GoalPrnLine(hDocument, p++, "    ", &xLine);
		CHECK(iRet >= 0, lblKO);


		//Montant
		xLine.xFont.eStyle = GL_FONT_STYLE_BOLD;

		memset(affiche_mont,0x00,14) ;
		MontComp = (*transac).Transaction_Amount;
		long_asc (montant, 12, &MontComp);
		RemoveLeftZeros(montant) ;
		Exp=StructInfoPayment.Devise_acc.Exposant-48;
		FormatAmount(montant,affiche_mont,Exp);

		sprintf(pPtr, "%.13s%.3s",affiche_mont,StructInfoPayment.Devise_acc.code_alph);

		xLine.eTextAlign = GL_ALIGN_RIGHT;
		xLine.eBackAlign = GL_ALIGN_RIGHT;
		iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
		xLine.eTextAlign = GL_ALIGN_LEFT;
		xLine.eBackAlign = GL_ALIGN_LEFT;
		strcpy(var, ":");
		sprintf(pPtr, "%*s", 29 + strlen(var), var);
		iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
		iRet = GoalPrnLine(hDocument, p++, "Montant", &xLine);
		CHECK(iRet >= 0, lblKO);



		//Num transaction
		xLine.xFont.eStyle = GL_FONT_STYLE_NORMAL;
		memcpy(NumSeqReliquat, (*transac).Num_sequence, 6);
		addition(NumSeqReliquat, "000001", 6);

		sprintf(pPtr, "%.6s", NumSeqReliquat);
		xLine.eTextAlign = GL_ALIGN_RIGHT;
		xLine.eBackAlign = GL_ALIGN_RIGHT;
		iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
		xLine.eTextAlign = GL_ALIGN_LEFT;
		xLine.eBackAlign = GL_ALIGN_LEFT;
		strcpy(var, ":");
		sprintf(pPtr, "%*s", 29 + strlen(var), var);
		iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
		iRet = GoalPrnLine(hDocument, p++, "Num Transaction", &xLine);
		CHECK(iRet >= 0, lblKO);

		//Num autorisation

		sprintf(pPtr, "%.6s", (*transac).Approval_Code);
		xLine.eTextAlign = GL_ALIGN_RIGHT;
		xLine.eBackAlign = GL_ALIGN_RIGHT;
		iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
		xLine.eTextAlign = GL_ALIGN_LEFT;
		xLine.eBackAlign = GL_ALIGN_LEFT;
		strcpy(var, ":");
		sprintf(pPtr, "%*s", 29 + strlen(var), var);
		iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
		iRet = GoalPrnLine(hDocument, p++, "Num Autorisation", &xLine);
		CHECK(iRet >= 0, lblKO);
		//Stan
		sprintf(pPtr, "%.6s", (*transac).STAN);
		xLine.eTextAlign = GL_ALIGN_RIGHT;
		xLine.eBackAlign = GL_ALIGN_RIGHT;
		iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
		xLine.eTextAlign = GL_ALIGN_LEFT;
		xLine.eBackAlign = GL_ALIGN_LEFT;
		strcpy(var, ":");
		sprintf(pPtr, "%*s", 29 + strlen(var), var);
		iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
		iRet = GoalPrnLine(hDocument, p++, "Num STAN", &xLine);
		CHECK(iRet >= 0, lblKO);

		//Montant total
		xLine.eBackAlign = GL_ALIGN_CENTER;
		xLine.eTextAlign = GL_ALIGN_CENTER;
		xLine.xFont.eScale = GL_SCALE_XLARGE;
		sprintf(pPtr, "-------------------------------------");
		iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
		CHECK(iRet >= 0, lblKO);
		xLine.eBackAlign = GL_ALIGN_LEFT;
		xLine.eTextAlign = GL_ALIGN_LEFT;
		xLine.xFont.eScale = GL_SCALE_LARGE;
		xLine.xFont.eStyle = GL_FONT_STYLE_BOLD;

		memset(affiche_mont,0x00,14);
		if (StructInfoPayment.Transaction_Amount / 100 >= 100000)
			xLine.xFont.eScale = GL_SCALE_LARGE;
		else
			xLine.xFont.eScale = GL_SCALE_XLARGE;
		MontComp = StructInfoPayment.Transaction_Amount+(*transac).Transaction_Amount;
		long_asc (montant, 12, &MontComp);
		RemoveLeftZeros(montant);
		Exp=StructInfoPayment.Devise_acc.Exposant-48;
		FormatAmount(montant,affiche_mont,Exp);

		sprintf(pPtr, "MONTANT : %.13s%.3s",affiche_mont,StructInfoPayment.Devise_acc.code_alph);
		xLine.eBackAlign = GL_ALIGN_CENTER;
		xLine.eTextAlign = GL_ALIGN_CENTER;
		iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
		CHECK(iRet >= 0, lblKO);
		xLine.eBackAlign = GL_ALIGN_CENTER;
		xLine.eTextAlign = GL_ALIGN_CENTER;
		xLine.xFont.eScale = GL_SCALE_XLARGE;
		sprintf(pPtr, "-------------------------------------");
		xLine.xFont.eStyle = GL_FONT_STYLE_NORMAL;
		iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
		CHECK(iRet >= 0, lblKO);


	}
	else
	{
		//Montant
		xLine.xFont.eStyle = GL_FONT_STYLE_BOLD;

		long_asc (montant, 12, &StructInfoPayment.Transaction_Amount);
		RemoveLeftZeros(montant) ;
		Exp=StructInfoPayment.Devise_acc.Exposant-48;
		FormatAmount(montant,affiche_mont,Exp);
		sprintf(pPtr, "%.13s%.3s",affiche_mont,StructInfoPayment.Devise_acc.code_alph);
		xLine.eTextAlign = GL_ALIGN_RIGHT;
		xLine.eBackAlign = GL_ALIGN_RIGHT;
		iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
		xLine.eTextAlign = GL_ALIGN_LEFT;
		xLine.eBackAlign = GL_ALIGN_LEFT;
		strcpy(var, ":");
		sprintf(pPtr, "%*s", 29 + strlen(var), var);
		iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
		iRet = GoalPrnLine(hDocument, p++, "Montant", &xLine);
		CHECK(iRet >= 0, lblKO);
		//Num transaction//Num Pre autorisation
		if(StructInfoPayment.CodeFunction==PREAUTHCODE){
			xLine.xFont.eStyle = GL_FONT_STYLE_NORMAL;
			memcpy(NumSeqReliquat, StructInfoPayment.Num_sequence, 6);
			//addition(NumSeqReliquat, "000001", 6);

			sprintf(pPtr, "%.6s", NumSeqReliquat);
			xLine.eTextAlign = GL_ALIGN_RIGHT;
			xLine.eBackAlign = GL_ALIGN_RIGHT;
			iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
			xLine.eTextAlign = GL_ALIGN_LEFT;
			xLine.eBackAlign = GL_ALIGN_LEFT;
			strcpy(var, ":");
			sprintf(pPtr, "%*s", 29 + strlen(var), var);
			iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
			iRet = GoalPrnLine(hDocument, p++, "Num Pre-autori", &xLine);
			CHECK(iRet >= 0, lblKO);


	}else{
			xLine.xFont.eStyle = GL_FONT_STYLE_NORMAL;
			memcpy(NumSeqReliquat, StructInfoPayment.Num_sequence, 6);
			//addition(NumSeqReliquat, "000001", 6);

			sprintf(pPtr, "%.6s", NumSeqReliquat);
			xLine.eTextAlign = GL_ALIGN_RIGHT;
			xLine.eBackAlign = GL_ALIGN_RIGHT;
			iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
			xLine.eTextAlign = GL_ALIGN_LEFT;
			xLine.eBackAlign = GL_ALIGN_LEFT;
			strcpy(var, ":");
			sprintf(pPtr, "%*s", 29 + strlen(var), var);
			iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
			iRet = GoalPrnLine(hDocument, p++, "Num Transaction", &xLine);
			CHECK(iRet >= 0, lblKO);

	}
		//Num autorisation
		sprintf(pPtr, "%.6s", StructInfoPayment.Approval_Code);
		xLine.eTextAlign = GL_ALIGN_RIGHT;
		xLine.eBackAlign = GL_ALIGN_RIGHT;
		iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
		xLine.eTextAlign = GL_ALIGN_LEFT;
		xLine.eBackAlign = GL_ALIGN_LEFT;
		strcpy(var, ":");
		sprintf(pPtr, "%*s", 29 + strlen(var), var);
		iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
		iRet = GoalPrnLine(hDocument, p++, "Num Autorisation", &xLine);
		CHECK(iRet >= 0, lblKO)
		//Stan
		sprintf(pPtr, "%.6s", StructInfoPayment.STAN);
		xLine.eTextAlign = GL_ALIGN_RIGHT;
		xLine.eBackAlign = GL_ALIGN_RIGHT;
		iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
		xLine.eTextAlign = GL_ALIGN_LEFT;
		xLine.eBackAlign = GL_ALIGN_LEFT;
		strcpy(var, ":");
		sprintf(pPtr, "%*s", 29 + strlen(var), var);
		iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
		iRet = GoalPrnLine(hDocument, p++, "STAN", &xLine);
		CHECK(iRet >= 0, lblKO)


	}
	//DEBIT	||	ANNULATION	||	CREDIT
	//j += sprintf(pPtr+j, "<#f=%08lx#>",(UL)SYSTEM_FONT_8x14_ID);//Font Gra

	xLine.xFont.eScale = GL_SCALE_LARGE;

	switch(StructInfoPayment.CodeFunction)
	{
   	case VOIDCODE:
		sprintf(pPtr, "ANNULATION");
		GoalPrnLine(hDocument, p++, pPtr, &xLine);
		break;
	case REFUNDCODE:
		sprintf(pPtr, "CREDIT");
		GoalPrnLine(hDocument, p++, pPtr, &xLine);
		break;
	case PREAUTHCODE:
		//j += sprintf(pPtr+j, "\n\n");    //KI Le 190106
		break;
	case VOID_PREAUTO_TOTAL_CODE:
	case VOID_PREAUTO_PARTIEL_CODE:
		sprintf(pPtr, "ANNULATION PRE-AUTO.");
		GoalPrnLine(hDocument, p++, pPtr, &xLine);
		break;
	default:
		sprintf(pPtr, "DEBIT");
		GoalPrnLine(hDocument, p++, pPtr, &xLine);
		break;
	}
	//j += sprintf(pPtr+j, "<#f=%08lx#>",(UL)SYSTEM_FONT_5x7_ID);//Font normal
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
   			if (source != CLIENT){

				sprintf(pPtr, "SIGNATURE CLIENT\n\n\n\n");
				xLine.xFont.eScale = GL_SCALE_LARGE;
				xLine.xFont.eStyle = GL_FONT_STYLE_BOLD;
				iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
			}
		}
		break;
	}
	xLine.xFont.eStyle = GL_FONT_STYLE_NORMAL;
	xLine.xFont.eScale = GL_SCALE_LARGE;
	//if(!dup)
//	{
//		sprintf(pPtr, "%.24s",StructInfoPayment.Message2);
//		GoalPrnLine(hDocument, p++, pPtr, &xLine);
//	}


	if (langue == ENG) {
		//sprintf(pPtr, "TICKET TO SAVE");
		if (source == CLIENT)
			sprintf(pPtr, "Client Copy");
		else
			sprintf(pPtr, "Merchant Copy");
	} else {
		//sprintf(pPtr, "TICKET A CONSERVER");
		if (source == CLIENT)
			sprintf(pPtr, "Copie client");
		else
			sprintf(pPtr, "Copie commer?ant", 135);
	}
	iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
	CHECK(iRet >= 0, lblKO);
	iRet = GoalPrnDocument(hDocument);              // Print document
	CHECK(iRet >= 0, lblKO);
	GoalDestroyDocument(&hDocument);

	// Errors treatment
	// ****************
	if (source != CLIENT)
		{

		sprintf(pPtr, "-------------------------------------");
		xLine.xFont.eScale = GL_SCALE_XLARGE;
		iRet = GoalPrnLine(xDocument, p++, pPtr, &xLine);
		CHECK(iRet >= 0, lblKO);
		xLine.xFont.eScale = GL_SCALE_LARGE;
		sprintf(pPtr, "Conservez-moi, je peux ?tre utile !");
		iRet = GoalPrnLine(xDocument, p++, pPtr, &xLine);
		CHECK(iRet >= 0, lblKO);

		xLine.xFont.eStyle = GL_FONT_STYLE_BOLD;
		sprintf(pPtr, "WWW.NAPS.MA");
		iRet = GoalPrnLine(xDocument, p++, pPtr, &xLine);
		CHECK(iRet >= 0, lblKO);
		}else {

			printLogo(ABORTED);
		}
		iRet = GoalPrnDocument(xDocument);              // Print document
		CHECK(iRet >= 0, lblKO);

		iRet = 1;
		goto lblEnd;



	lblKO: GL_Dialog_Message(hGoal, NULL, "Processing Error printer", GL_ICON_ERROR, GL_BUTTON_VALID, 5 * 1000);
	goto lblEnd;

	lblEnd: if (xDocument)
	GoalDestroyDocument(&xDocument);



	CUTERMpaperFeed();
/*	Os__xlinefeed(4);*/
	return OK;

}

uchar	ImprimeTraCompOri(uchar langue,uchar source,uchar dup,lignetra		*transac)
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
	printLogo(CLIENT);

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

	//Num?ro de la carte
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
	//	j += sprintf(pPtr+j, "\n\n		%.24s",StructInfoPayment.Message1);

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
			j += sprintf(pPtr+j, "\n	  	Copie commer?ant");
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
	int  h, j;
	US k;
	C Buffer[1500];
	uchar status;
	Ushort Index_Cmpte, Indice_fin;
	uchar Reseau_accept[25];
	DATE date;

	bool bContinue = TRUE, bContinue2 = TRUE;
	int iHeader, iFooter;
	int iRet;

	T_GL_HWIDGET hDocument = NULL;
	ST_PRN_LINE xLine;
	byte p; // Printer line index
	T_GL_HGRAPHIC_LIB hGoal = NULL;
	//int iRet;

	StructInfoPayment.waitpuce = 0;
	/*Aff_indicNO_OUI(FicPara.Langue);*/
	CUMORE_CMI_AFFICHE_MESSAGE(94, 32, 1000);
	//Aff_mess(FicPara.Langue,94,0,0,0,0);
	//Aff_mess(FicPara.Langue,32,1,0,0,0);

	memset(Buffer, 0, sizeof(Buffer));

	printLogo(CLIENT);

	// Printing in progress
	// ********************
	GL_Dialog_Message(hGoal, NULL, "Please Wait" "\n" "Printing In Progress",
			GL_ICON_INFORMATION, GL_BUTTON_NONE, 0);

	p = 0;                                            // Create document
	hDocument = GoalCreateDocument(APEMV_UI_GoalHandle(),
			GL_ENCODING_ISO_8859_1);
	CHECK(hDocument != NULL, lblKO);
	// *** Font scale capability ***
	xLine = xPrinter;
	xLine.xFont.eStyle = GL_FONT_STYLE_BOLD;
	xLine.xFont.eScale = GL_SCALE_XLARGE;
	xLine.eTextAlign = GL_ALIGN_CENTER;
	xLine.eBackAlign = GL_ALIGN_CENTER;
	sprintf(Buffer,"\nPARAMETRES");
	iRet = GoalPrnLine(hDocument, p++, Buffer, &xLine);
	CHECK(iRet >= 0, lblKO);


	xLine.xFont.eStyle = GL_FONT_STYLE_NORMAL;
	xLine.xFont.eScale = GL_SCALE_LARGE;
	xLine.eTextAlign = GL_ALIGN_LEFT;
	xLine.eBackAlign = GL_ALIGN_LEFT;
	Telium_Read_date(&date);
	sprintf(Buffer, "%.2s/%.2s/%.2s                            %.2s:%.2s:%.2s",
			date.day, date.month, date.year, date.hour, date.minute,
			date.second);
	iRet = GoalPrnLine(hDocument, p++, Buffer, &xLine);
	CHECK(iRet >= 0, lblKO);


//	sprintf(Buffer, "%.15s ",FicPara.Num_commerce);
//	iRet = GoalPrnLine(hDocument, p++, Buffer, &xLine);
//	CHECK(iRet >= 0, lblKO);
	//N point de vente
	sprintf(Buffer, "%.15s",FicPara.Num_point_vente);
	iRet = GoalPrnLine(hDocument, p++, Buffer, &xLine);
	CHECK(iRet >= 0, lblKO);
	//N Terminal
	sprintf(Buffer, "%.8s",FicPara.Num_Terminal);
	iRet = GoalPrnLine(hDocument, p++, Buffer, &xLine);
	CHECK(iRet >= 0, lblKO);
	//Enseigne
	sprintf(Buffer, "%.24s",FicPara.Merchant_Name);
	iRet = GoalPrnLine(hDocument, p++, Buffer, &xLine);
	CHECK(iRet >= 0, lblKO);
	//MCC
	sprintf(Buffer, "MCC :%.4s",FicPara.MCC);
	iRet = GoalPrnLine(hDocument, p++, Buffer, &xLine);
	CHECK(iRet >= 0, lblKO);
	//Compte Domiciliation
//	if(Fic_compte_dom.Indice_fin)
//	{
//		Index_Cmpte = FicPara.Index_compte;
//		i += sprintf(Buffer+i, "\nCompte dom.:%.12s",Fic_compte_dom.tab_compte[Index_Cmpte].Compte);
//	}
	//Adresse
	sprintf(Buffer, "%.24s",FicPara.Merchant_Adrees);
	iRet = GoalPrnLine(hDocument, p++, Buffer, &xLine);
	CHECK(iRet >= 0, lblKO);
	//Ville
	sprintf(Buffer, "%.13s",FicPara.Merchant_City);
	iRet = GoalPrnLine(hDocument, p++, Buffer, &xLine);
	CHECK(iRet >= 0, lblKO);
	//Reseaux
	sprintf(Buffer, "\nReseaux Accept?s :");
	iRet = GoalPrnLine(hDocument, p++, Buffer, &xLine);
	CHECK(iRet >= 0, lblKO);

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
   sprintf(Buffer, "\n%s",Reseau_accept);
   iRet = GoalPrnLine(hDocument, p++, Buffer, &xLine);
   CHECK(iRet >= 0, lblKO);



	Indice_fin = FicInsReg.Indice_fin;
	//i += sprintf(Buffer+i, "\x0f");
	if(Indice_fin!=0)
	{
		C	TmpBuffer[24];

		for(k=0;k<Indice_fin;k++)
		{
			memset(TmpBuffer,0,24);
			memcpy(TmpBuffer,FicInsReg.Tab_Ins_reg[k].Libelle,strlen((char*)FicInsReg.Tab_Ins_reg[k].Libelle));
			(Ushort)sprintf(Buffer, "%s\n                                     %s",TmpBuffer,FicInsReg.Tab_Ins_reg[k].Num_point_vente);
			iRet = GoalPrnLine(hDocument, p++, Buffer, &xLine);
			CHECK(iRet >= 0, lblKO);
		}
	}

	//Acceptance
	sprintf(Buffer, "\nAcceptances :");
	iRet = GoalPrnLine(hDocument, p++, Buffer, &xLine);
	CHECK(iRet >= 0, lblKO);
	//i += sprintf(Buffer+i, "\x0f");
//	i += sprintf(Buffer+i, "\ntrs puce           :%.3s",((FicPara.Accept_puce=='1') ? "OUI" : "NON"));
//	i += sprintf(Buffer+i, "\ntrs man            :%.3s",((FicPara.Accept_Man=='1') ? "OUI" : "NON"));
//	i += sprintf(Buffer+i, "\ntrs annul.         :%.3s",((FicPara.Indic_annul=='1') ? "OUI" : "NON"));
//	i += sprintf(Buffer+i, "\ntrs credit.        :%.3s",((FicPara.Indic_Refund=='1') ? "OUI" : "NON"));
//	i += sprintf(Buffer+i, "\npre-autor.         :%.3s",((FicPara.Indic_PreAutor=='1') ? "OUI" : "NON"));
//	i += sprintf(Buffer+i, "\ntrs offline        :%.3s",((FicPara.Indic_Offline=='1') ? "OUI" : "NON"));
//	i += sprintf(Buffer+i, "\nForcage            :%.3s",((FicPara.Indic_forcage=='1') ? "OUI" : "NON"));
//	i += sprintf(Buffer+i, "\nControle fraude    :%.3s",((FicPara.Ctrl_fraude=='1') ? "OUI" : "NON"));
//	i += sprintf(Buffer+i, "\nChgmnt compte      :%.3s",((FicPara.Indic_Change_Cpte_Dom=='1') ? "OUI" : "NON"));
//	i += sprintf(Buffer+i, "\nCmmrc offshore     :%.3s",((FicPara.Indic_OffShort=='1') ? "OUI" : "NON"));


	//Acceptance

	sprintf(Buffer, "%*s", 50, ((FicPara.MenuTPE[1] == '1') ? ":OUI" : ":NON"));
	iRet = GoalPrnLine(hDocument, p, Buffer, &xLine);
	iRet = GoalPrnLine(hDocument, p++, "Recharge Prepaye", &xLine);

	sprintf(Buffer, "%*s", 50, ((FicPara.MenuTPE[2] == '1') ? ":OUI" : ":NON"));
	iRet = GoalPrnLine(hDocument, p, Buffer, &xLine);
	iRet = GoalPrnLine(hDocument, p++, "Decharge Prepaye", &xLine);

	sprintf(Buffer, "%*s", 50, ((FicPara.MenuTPE[3] == '1') ? ":OUI" : ":NON"));
	iRet = GoalPrnLine(hDocument, p, Buffer, &xLine);
	iRet = GoalPrnLine(hDocument, p++, "Transfert Prepaye", &xLine);

	sprintf(Buffer, "%*s", 50, ((FicPara.MenuTPE[6] == '1') ? ":OUI" : ":NON"));
	iRet = GoalPrnLine(hDocument, p, Buffer, &xLine);
	iRet = GoalPrnLine(hDocument, p++, "Trx manuelle", &xLine);

	sprintf(Buffer,"%*s", 50,((FicPara.MenuTPE[7] == '1') ? ":OUI" : ":NON"));
	iRet = GoalPrnLine(hDocument, p, Buffer, &xLine);
	iRet = GoalPrnLine(hDocument, p++, "Trx annulation", &xLine);

	sprintf(Buffer,"%*s", 50,((FicPara.MenuTPE[8] == '1') ? ":OUI" : ":NON"));
	iRet = GoalPrnLine(hDocument, p, Buffer, &xLine);
	iRet = GoalPrnLine(hDocument, p++, "Trx cr?dit", &xLine);

	sprintf(Buffer, "%*s", 50,((FicPara.MenuTPE[9] == '1') ? ":OUI" : ":NON"));
	iRet = GoalPrnLine(hDocument, p, Buffer, &xLine);
	iRet = GoalPrnLine(hDocument, p++, "T?l?collecte", &xLine);

	//Os__xprinta("11111");
	sprintf(Buffer, "%*s", 50,((FicPara.MenuTPE[11] == '1') ? ":OUI" : ":NON"));
	iRet = GoalPrnLine(hDocument, p, Buffer, &xLine);
	iRet = GoalPrnLine(hDocument, p++, "Duplicata", &xLine);


	sprintf(Buffer, "%*s", 50,((FicPara.MenuTPE[12] == '1') ? ":OUI" : ":NON"));
	iRet = GoalPrnLine(hDocument, p, Buffer, &xLine);
	iRet = GoalPrnLine(hDocument, p++, "Cash Advance", &xLine);

	sprintf(Buffer, "%*s", 50,((FicPara.MenuTPE[13] == '1') ? ":OUI" : ":NON"));
	iRet = GoalPrnLine(hDocument, p, Buffer, &xLine);
	iRet = GoalPrnLine(hDocument, p++, "Pre autorisation", &xLine);

	sprintf(Buffer, "%*s", 50,((FicPara.MenuTPE[14] == '1') ? ":OUI" : ":NON"));
	iRet = GoalPrnLine(hDocument, p, Buffer, &xLine);
	iRet = GoalPrnLine(hDocument, p++, "Activation Carte", &xLine);

	sprintf(Buffer, "%*s", 50,((FicPara.MenuTPE[15] == '1') ? ":OUI" : ":NON"));
	iRet = GoalPrnLine(hDocument, p, Buffer, &xLine);
	iRet = GoalPrnLine(hDocument, p++, "Demande Solde", &xLine);

	sprintf(Buffer, "%*s", 50,((FicPara.MenuTPE[16] == '1') ? ":OUI" : ":NON"));
	iRet = GoalPrnLine(hDocument, p, Buffer, &xLine);
	iRet = GoalPrnLine(hDocument, p++, "Mini Releve", &xLine);

	sprintf(Buffer, "%*s", 50,((FicPara.MenuTPE[17] == '1') ? ":OUI" : ":NON"));
	iRet = GoalPrnLine(hDocument, p, Buffer, &xLine);
	iRet = GoalPrnLine(hDocument, p++, "Pin Change", &xLine);

	sprintf(Buffer, "%*s", 50,((FicPara.MenuTPE[18] == '1') ? ":OUI" : ":NON"));
	iRet = GoalPrnLine(hDocument, p, Buffer, &xLine);
	iRet = GoalPrnLine(hDocument, p++, "E recharge", &xLine);

	sprintf(Buffer, "%*s", 50,((FicPara.MenuTPE[19] == '1') ? ":OUI" : ":NON"));
	iRet = GoalPrnLine(hDocument, p, Buffer, &xLine);
	iRet = GoalPrnLine(hDocument, p++, "E Facture", &xLine);

	sprintf(Buffer, "%*s", 50,((FicPara.MenuTPE[20] == '1') ? ":OUI" : ":NON"));
	iRet = GoalPrnLine(hDocument, p, Buffer, &xLine);
	iRet = GoalPrnLine(hDocument, p++, "Micro Cr?dit", &xLine);

	sprintf(Buffer, "%*s", 50,((FicPara.MenuTPE[21] == '1') ? ":OUI" : "NON"));
	iRet = GoalPrnLine(hDocument, p, Buffer, &xLine);
	iRet = GoalPrnLine(hDocument, p++, "Ticket Journal", &xLine);

	sprintf(Buffer, "%*s", 50,((FicPara.MenuTPE[22] == '1') ? ":OUI" : ":NON"));
	iRet = GoalPrnLine(hDocument, p, Buffer, &xLine);
	iRet = GoalPrnLine(hDocument, p++, "Fidelit?", &xLine);

	sprintf(Buffer, "%*s", 50,((FicPara.MenuTPE[23] == '1') ? ":OUI" : ":NON"));
	iRet = GoalPrnLine(hDocument, p, Buffer, &xLine);
	iRet = GoalPrnLine(hDocument, p++, "Rembours. Siniste", &xLine);

	sprintf(Buffer, "%*s", 50,((FicPara.MenuTPE[24] == '1') ? ":OUI" : ":NON"));
	iRet = GoalPrnLine(hDocument, p, Buffer, &xLine);
	iRet = GoalPrnLine(hDocument, p++, "Paiement N Fois", &xLine);

	sprintf(Buffer, "%*s", 50,((FicPara.MenuTPE[25] == '1') ? ":OUI" : ":NON"));
	iRet = GoalPrnLine(hDocument, p, Buffer, &xLine);
	iRet = GoalPrnLine(hDocument, p++, "Paiement Diff?r?", &xLine);

	sprintf(Buffer, "%*s", 50,((FicPara.MenuTPE[26] == '1') ? ":OUI" : ":NON"));
	iRet = GoalPrnLine(hDocument, p, Buffer, &xLine);
	iRet = GoalPrnLine(hDocument, p++, "Paiement Pourboire", &xLine);

	sprintf(Buffer, "%*s", 50,((FicPara.MenuTPE[27] == '1') ? ":OUI" : ":NON"));
	iRet = GoalPrnLine(hDocument, p, Buffer, &xLine);
	iRet = GoalPrnLine(hDocument, p++, "Paiement Surco?t", &xLine);


//	switch(FicPara.Langue)
//	{
//   	case FRAN:
//		i += sprintf(Buffer+i, "\n\nLangue       :FRANCAIS");
//		break;
//	case ENG:
//		i += sprintf(Buffer+i, "\n\nLangue       :ANGLAIS");
//		break;
//	case ARABE:
//		i += sprintf(Buffer+i, "\n\nLangue       :ARABE");
//		break;
//	default:
//		i += sprintf(Buffer+i, "\n\nLangue       :FRANCAIS");
//		break;
//	}

	char var1[4];
    sprintf(var1,":%02d",FicPara.Max_trs_off);
    sprintf(Buffer,"%*s",49,var1);
	iRet = GoalPrnLine(hDocument, p, Buffer, &xLine);
	iRet = GoalPrnLine(hDocument, p++, "Nbr Max Trs Off-line", &xLine);

	sprintf(var1,":%02d",FicPara.Max_avis);
	sprintf(Buffer,"%*s",49,var1);
	iRet = GoalPrnLine(hDocument, p, Buffer, &xLine);
	iRet = GoalPrnLine(hDocument, p++, "Nbr Max Avis Trs", &xLine);


	//type appli
	switch(FicPara.Appli_type)
	{
   	case '0':
		sprintf(Buffer, "Type appli : Achat");
		break;
	case '1':
		sprintf(Buffer, "Type appli : Restaurant");
		break;
	case '2':
		sprintf(Buffer, "Type appli : Supermarche");
		break;
	case '3':
		sprintf(Buffer, "Type appli : Hotel");
		break;
	case '4':
		sprintf(Buffer, "Type appli : Loc. voiture");
		break;
	case '5':
		sprintf(Buffer, "Type appli : Cash advance");
		break;
	case '6':
		sprintf(Buffer, "Type appli : Stat service");
		break;
	default:
		sprintf(Buffer, "Type appli : Autre");
		break;
	}
	iRet = GoalPrnLine(hDocument, p++, Buffer, &xLine);
	CHECK(iRet >= 0, lblKO);

	iRet = GoalPrnLine(hDocument, p++, "    ", &xLine);
	CHECK(iRet >= 0, lblKO);




	char var[10];
	sprintf(var, ":%.2sH", FicPara.Heure_appel);
	sprintf(Buffer, "%*s%.2s", 46,var,&FicPara.Heure_appel[2]);
	iRet = GoalPrnLine(hDocument, p, Buffer, &xLine);
	CHECK(iRet >= 0, lblKO);
	iRet = GoalPrnLine(hDocument, p++, "Heure appel", &xLine);
	CHECK(iRet >= 0, lblKO);


	//Nombre d'essai connexion
	sprintf(var, ":%d", FicPara.Nbr_of_redial);
	sprintf(Buffer, "%*s", 49,var);
	iRet = GoalPrnLine(hDocument, p, Buffer, &xLine);
	CHECK(iRet >= 0, lblKO);
	iRet = GoalPrnLine(hDocument, p++, "N Essai Connex", &xLine);
	CHECK(iRet >= 0, lblKO);


	//D?lai interconnexion
	sprintf(var, ":%d", FicPara.redial_delay);
	sprintf(Buffer, "%*s", 49, var);
	iRet = GoalPrnLine(hDocument, p, Buffer, &xLine);
	CHECK(iRet >= 0, lblKO);
	iRet = GoalPrnLine(hDocument, p++, "Delai interconnex", &xLine);
	CHECK(iRet >= 0, lblKO);


	//Valeur Time out
	sprintf(var, ":%d", FicPara.TimeOut);
	sprintf(Buffer, "%*s", 49,var);
	iRet = GoalPrnLine(hDocument, p, Buffer, &xLine);
	CHECK(iRet >= 0, lblKO);
	iRet = GoalPrnLine(hDocument, p++, "Time Out", &xLine);
	CHECK(iRet >= 0, lblKO);

	//Tel auto Prod
//	i += sprintf(Buffer+i, "\nTel.aut.prod:%.24s",FicPara.Tel_autor_pri);
//	//X25 auto Prod
//	i += sprintf(Buffer+i, "\nX25.aut.Prod:%.24s",FicPara.X25_autor_pri);
//	//Tel auto Sec
//	i += sprintf(Buffer+i, "\nTel.aut.sec.:%.24s",FicPara.Tel_autor_sec);
//	//X25 auto Prod
//	i += sprintf(Buffer+i, "\nX25.aut.sec.:%.24s",FicPara.X25_autor_sec);
//	//Tel tlc Prod
//	i += sprintf(Buffer+i, "\nTel.tlc.prod:%.24s",FicPara.Tel_telec_pri);
//	//X25 tlc Prod
//	i += sprintf(Buffer+i, "\nX25.tlc.Prod:%.24s",FicPara.X25_telec_pri);
//	//Tel tlc Sec
//	i += sprintf(Buffer+i, "\nTel.tlc.sec.:%.24s",FicPara.Tel_telec_sec);
//	//X25 tlc Sec
//	i += sprintf(Buffer+i, "\nX25.tlc.sec.:%.24s",FicPara.X25_telec_sec);
	//Tel serv App prim
//****************************//
//	status = imp_buffer(Buffer,i);
//	if(status!=OK)
//	{
//		Os__display(0,0,"Pb. Impression");
//		Os__xget_key(5);
//		CUTERMpaperFeed();
//		return status;
//	}
//	i=0;
//	memset(Buffer,0,sizeof(Buffer));
//****************************//
//	i += sprintf(Buffer+i, "\nTel.Tlch.pri:%.24s",FicPara.Tel_telech_pri);
//	//X25 serv App prim
//	i += sprintf(Buffer+i, "\nX25.Tlch.pri:%.24s",FicPara.X25_telech_pri);
//	//Tel serv App sec
//	i += sprintf(Buffer+i, "\nTel.Tlch.sec:%.24s",FicPara.Tel_telech_sec);
//	//X25 serv App sec
//	i += sprintf(Buffer+i, "\nX25.Tlch.sec:%.24s",FicPara.X25_telech_sec);
	//Tel serv App prim

	iRet = GoalPrnLine(hDocument, p++, "    ", &xLine);
	CHECK(iRet >= 0, lblKO);

	//Os__xprinta("1555555");
	sprintf(Buffer, "Srv.Tlch : %.24s", FicPara.Tel_telech_pri);//1.11.22     //lkeddouch
	iRet = GoalPrnLine(hDocument, p++, Buffer, &xLine);
	CHECK(iRet >= 0, lblKO);
	//Tel serv App sec
	sprintf(Buffer, "Port.Srv.Tlch : %.24s",FicPara.X25_telech_pri);	//1.11.22     //lkeddouch
	iRet = GoalPrnLine(hDocument, p++, Buffer, &xLine);
	CHECK(iRet >= 0, lblKO);



//	sprintf(Buffer, "%.4s", 50,FicPara.Tel_telech_pri);
//	iRet = GoalPrnLine(hDocument, p, Buffer, &xLine);
//	iRet = GoalPrnLine(hDocument, p++, "Srv.Tlch :", &xLine);

	//Os__xprinta("66666");

//	sprintf(Buffer, "\nSrv.Tlch:%.24s", FicPara.Tel_telech_pri);//1.11.22     //lkeddouch
//	iRet = GoalPrnLine(hDocument, p++, Buffer, &xLine);
//	CHECK(iRet >= 0, lblKO);
	//Tel serv App sec

//	sprintf(Buffer, "%.4s", 50,FicPara.X25_telech_pri);
//	iRet = GoalPrnLine(hDocument, p, Buffer, &xLine);
//	iRet = GoalPrnLine(hDocument, p++, "Port.Srv.Tlch :", &xLine);

//	sprintf(Buffer, "Port.Srv.Tlch               :%.24s", FicPara.X25_telech_pri);//1.11.22     //lkeddouch
//	iRet = GoalPrnLine(hDocument, p++, Buffer, &xLine);
//	CHECK(iRet >= 0, lblKO);
	//Index cl? maitresse

	iRet = GoalPrnLine(hDocument, p++, "    ", &xLine);
	CHECK(iRet >= 0, lblKO);


	sprintf(var, ":%.3s", FicPara.Index_MK);
	sprintf(Buffer, "%*s", 50, var);
	iRet = GoalPrnLine(hDocument, p, Buffer, &xLine);
	iRet = GoalPrnLine(hDocument, p++, "Index MasterKey", &xLine);

//	sprintf(Buffer, "\nIndex MasterKey                     :%.3s", FicPara.Index_MK);
//	iRet = GoalPrnLine(hDocument, p++, Buffer, &xLine);
//    CHECK(iRet >= 0, lblKO);
	//Index cl? mac

	sprintf(var, ":%.3s", FicPara.Index_MacKey);
	sprintf(Buffer, "%*s", 50, var);
	iRet = GoalPrnLine(hDocument, p, Buffer, &xLine);
	iRet = GoalPrnLine(hDocument, p++, "Index MacKey", &xLine);


//	sprintf(Buffer, "Index MacKey                          :%.3s",FicPara.Index_MacKey);
//	iRet = GoalPrnLine(hDocument, p++, Buffer, &xLine);
//    CHECK(iRet >= 0, lblKO);
	//Index liste d'opposition
//	i += sprintf(Buffer+i, "\nIndex liste Opp.   :%.4s",lop_gest.NumListe);
	//Version para tpe
	iRet = GoalPrnLine(hDocument, p++, "    ", &xLine);
	CHECK(iRet >= 0, lblKO);

	sprintf(var, ":%.3s", FicPara.Version_Pos_Param);
	sprintf(Buffer, "%*s", 50, var);
	iRet = GoalPrnLine(hDocument, p, Buffer, &xLine);
	iRet = GoalPrnLine(hDocument, p++, "Version Pos param.", &xLine);

//	sprintf(Buffer, "\nVersion Pos param.                  :%.3s",FicPara.Version_Pos_Param);
//	iRet = GoalPrnLine(hDocument, p++, Buffer, &xLine);
//    CHECK(iRet >= 0, lblKO);
	//Version table bin

	sprintf(var, ":%.3s", FicPara.Version_Bin_Prefixe);
	sprintf(Buffer, "%*s", 50, var);
	iRet = GoalPrnLine(hDocument, p, Buffer, &xLine);
	iRet = GoalPrnLine(hDocument, p++, "Version Table Bins", &xLine);
	//Os__xprinta("77777");

//	sprintf(Buffer, "Version Table Bins                   :%.3s",FicPara.Version_Bin_Prefixe);
//	iRet = GoalPrnLine(hDocument, p++, Buffer, &xLine);
//	CHECK(iRet >= 0, lblKO);
	//Version table emv

	sprintf(var, ":%.3s", FicPara.Version_Tab_Emv);
	sprintf(Buffer, "%*s", 50, var);
	iRet = GoalPrnLine(hDocument, p, Buffer, &xLine);
	iRet = GoalPrnLine(hDocument, p++, "Version Tables EMV", &xLine);

//	sprintf(Buffer, "Version Tables EMV               :%.3s",FicPara.Version_Tab_Emv);
//	iRet = GoalPrnLine(hDocument, p++, Buffer, &xLine);
//    CHECK(iRet >= 0, lblKO);
	//Version compte
	sprintf(var, ":%.3s", FicPara.Version_Compte_Dom);
	sprintf(Buffer, "%*s", 50, var);
	iRet = GoalPrnLine(hDocument, p, Buffer, &xLine);
	iRet = GoalPrnLine(hDocument, p++, "Version Compte", &xLine);
	//Os__xprinta("888888");
	//Version logiciel
//	i += sprintf(Buffer+i, "\nVersion Logiciel   :%.04s",FicPara.Version_appli);
	//Version Liste Oppositions

	sprintf(var, ":%.04s", FicPara.Version_Opp);
	sprintf(Buffer, "%*s", 50, var);
	iRet = GoalPrnLine(hDocument, p, Buffer, &xLine);
	iRet = GoalPrnLine(hDocument, p++, "Version Opposition", &xLine);


//	sprintf(Buffer, "Version Opposition                  :%.04s", FicPara.Version_Opp);  //lkeddouch
//	iRet = GoalPrnLine(hDocument, p++, Buffer, &xLine);
//	CHECK(iRet >= 0, lblKO);
	//Staus para
	iRet = GoalPrnLine(hDocument, p++, "    ", &xLine);
	CHECK(iRet >= 0, lblKO);
	sprintf(Buffer, "%*.3s",49,((FicPara.status==OK) ? ":OK" : ":KO"));
	iRet = GoalPrnLine(hDocument, p, Buffer, &xLine);
	iRet = GoalPrnLine(hDocument, p++, "Status parametres", &xLine);
	//Os__xprinta("99999");

//	sprintf(Buffer, "\nStatus parametres                   :%.2s",((FicPara.status==OK) ? "OK" : "KO"));
//	iRet = GoalPrnLine(hDocument, p++, Buffer, &xLine);
//    CHECK(iRet >= 0, lblKO);
	//Status liste d'opposition
//	i += sprintf(Buffer+i, "\nStatus LOP         :%.2s",((lop_gest.stat==OK) ? "OK" : "KO"));
	//i += sprintf(Buffer+i, "\x12");


	//Mo 13/04/06
//	if (FicPara.g_type_connexion=='R')
//		i += sprintf(Buffer+i, "\nTYPE CONNEXION :%.3s","RTC");
//	if (FicPara.g_type_connexion=='M')
//		i += sprintf(Buffer+i, "\nTYPE CONNEXION :%.3s","GSM");
//	if (FicPara.g_type_connexion=='G')
//		i += sprintf(Buffer+i, "\nTYPE CONNEXION :%.4s","GPRS");
//	if (FicPara.g_type_connexion=='I')
//		i += sprintf(Buffer+i, "\nTYPE CONNEXION :%.4s","TCPIP");
//
//	i += sprintf(Buffer+i, "\nIP :%.15s",FicPara.ip_gprs);
//	i += sprintf(Buffer+i, "\nPort :%.6s",FicPara.port_gprs);
//	i += sprintf(Buffer+i, "\nAPN :%.40s",FicPara.apn_gprs);
//
//	i += sprintf(Buffer+i, "\nLOGIN GPRS :%.40s",FicPara.login_gprs);
//	i += sprintf(Buffer+i, "\nPASSWORD GPRS:%.40s",FicPara.pwd_gprs);
	//Mo 13/04/06

	//if (!memcmp(FicPara.EMETTEUR, "NAPS", 4))
	sprintf(Buffer, "\nVersion Logiciel   :%.04s", FicPara.Version_appli);
	iRet = GoalPrnLine(hDocument, p++, Buffer, &xLine);
	CHECK(iRet >= 0, lblKO);


	xLine.eBackAlign = GL_ALIGN_CENTER;
	xLine.eTextAlign = GL_ALIGN_CENTER;
	//xLine.xFont.eStyle = GL_FONT_STYLE_NORMAL;
	xLine.xFont.eScale = GL_SCALE_LARGE;

	xLine.xFont.eStyle = GL_FONT_STYLE_BOLD;
	sprintf(Buffer, "\nWWW.NAPS.MA");
	iRet = GoalPrnLine(hDocument, p++, Buffer, &xLine);
	CHECK(iRet >= 0, lblKO);
	//Os__xprinta("1100000");
	iRet = GoalPrnDocument(hDocument);              // Print document
	CHECK(iRet >= 0, lblKO);

	iRet = 1;
	goto lblEnd;
	// Errors treatment
	// ****************

	lblKO: GL_Dialog_Message(hGoal, NULL, "Processing Error", GL_ICON_ERROR,
			GL_BUTTON_VALID, 5 * 1000);
	goto lblEnd;

	lblEnd: if (hDocument)
		GoalDestroyDocument(&hDocument);

	CUTERMpaperFeed();
	return OK;

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
L	Os__xprintaa(PC message)
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
L	Os__xprintd(L num)
{
	return 0;
	Telium_File_t *printer ;

	printer = Telium_Fopen("PRINTER","w-");

	/* ##1 print the contents in condensed mode  */
	Telium_Fprintf(printer,"\x0f""%.06d \n",num);
	/* ##1 print the reading status in bold */
	Telium_Fprintf(printer,"\x12""\n");
	/* ##1 wait for print end */
	Telium_Ttestall ( PRINTER , 300 );
	Telium_Fclose(printer);
	return 0;
}
L	Os__xprintVersion(PC message)
{
	//return 0;
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
	//Num?ro de la carte
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
			Vprintline_caisse_gc(C_JUSTIFY2,handle,"Copie Commer?ant",l++);
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

		}else if(etareso==3){//Veuillez saisir?? votre Code PIN sur le PINPAD 002

			sprintf(buffer_echo,"001003105003%.3d%s004%.3d%s011003002014008%.2d%.2d%.4d015006%.2d%.2d%.2d",
			strlen(StructInfoCaisse[1].NCAI),StructInfoCaisse[1].NCAI,strlen(StructInfoCaisse[1].NS),StructInfoCaisse[1].NS,day,mth,yr,hr,min,sec);
			strcat(buffer_echo,"!");
			send_caisse_echo(buffer_echo,strlen(buffer_echo),1);
			return 0;

		}else if(etareso==4){//PIN incorrect, Veuillez r?essayer la saisie de votre Code PIN sur le PINPAD 003

			sprintf(buffer_echo,"001003105003%.3d%s004%.3d%s011003003014008%.2d%.2d%.4d015006%.2d%.2d%.2d",
			strlen(StructInfoCaisse[1].NCAI),StructInfoCaisse[1].NCAI,strlen(StructInfoCaisse[1].NS),StructInfoCaisse[1].NS,day,mth,yr,hr,min,sec);
			strcat(buffer_echo,"!");
			send_caisse_echo(buffer_echo,strlen(buffer_echo),1);
			return 0;
		}else if(etareso==6){//PIN incorrect, Attention, troisi?me tentative de saisie de votre Code PIN 004

			sprintf(buffer_echo,"001003105003%.3d%s004%.3d%s011003004014008%.2d%.2d%.4d015006%.2d%.2d%.2d",
			strlen(StructInfoCaisse[1].NCAI),StructInfoCaisse[1].NCAI,strlen(StructInfoCaisse[1].NS),StructInfoCaisse[1].NS,day,mth,yr,hr,min,sec);
			strcat(buffer_echo,"!");
			send_caisse_echo(buffer_echo,strlen(buffer_echo),1);
			return 0;
		}else if(etareso==7){//Probl?me lecture carte, Veuillez retirer et ins?rer ?? nouveau votre carte bancaire 006

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
	//CUMORE_CMI_AFFICHE_MESSAGE(94,32,1000);
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

	//D?lai interconnexion
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

	//Index cl? maitresse
	l++;
	sprintf((PC)Buffer, "Index MasterKey    :%.3s",FicPara.Index_MK);
	Vprintline_caisse(PRINT_NORM2,handle,Buffer,l++);

	//Index cl? mac
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
	//sprintf((PC)Buffer, "Version Logiciel   :%.04s",FicPara.Version_appli);
	sprintf((PC)Buffer, "Version Logiciel   :0506");
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

	if(mdp_merchant_cmi() != CR_SAISIE_OK)
	{
		CUMORE_CMI_AFFICHE_MESSAGE(16,20,5000);
		strcpy(state,"306");
		return 0;
	}
	Init_Param();
	Init_Lop();
	Init_Tra();
	Init_Pre();
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
			//APEMV_UI_MessageDisplayDelay ("MESSAGE RECU ERRONE\n-200", 2);
			resultat=0;
		}
		else if(memcmp(StructInfoCaisse[0].NS,SaveLastInfoCaisse.NS,3))
		{
			//APEMV_UI_MessageDisplayDelay ("MESSAGE RECU ERRONE\n-201", 2);
			resultat=0;
		}
		else if(memcmp(StructInfoCaisse[0].STAN,SaveLastInfoCaisse.STAN,6))
		{
			//APEMV_UI_MessageDisplayDelay ("MESSAGE RECU ERRONE\n-202", 2);
			resultat=0;
		}

	}
	else if(!memcmp(WaitedTM,"004",3))
	{
		if(memcmp(StructInfoCaisse[0].TM,"004",3))
		{
			//APEMV_UI_MessageDisplayDelay ("MESSAGE RECU ERRONE\n-400", 2);
			resultat=0;
		}
		else if(memcmp(StructInfoCaisse[0].NS,SaveLastInfoCaisse.NS,3))
		{
			//APEMV_UI_MessageDisplayDelay ("MESSAGE RECU ERRONE\n-401", 2);
			resultat=0;
		}
		else if(memcmp(StructInfoCaisse[0].STAN,SaveLastInfoCaisse.STAN,6))
		{
			//APEMV_UI_MessageDisplayDelay ("MESSAGE RECU ERRONE\n-402", 2);
			resultat=0;
		}

	}

	return resultat;
}


int OpenPeripherals(void)
{
	// Local variables
    // ***************
	int iRet;
	Telium_File_t *hDsp=NULL;          // Handle of the peripheral Display
	Telium_File_t *hTsc=NULL;          // Handle of the peripheral Touch Screen
	T_GL_HGRAPHIC_LIB hGoal=NULL;      // Handle of the graphics object library

	// Open peripherals
	// ****************
	hDsp = Telium_Stdperif("DISPLAY", NULL);  // //CHECK "display" peripheral already opened? (done by Manager)
	if (hDsp == NULL)                         // No, then open it
	{
		hDsp = Telium_Fopen("DISPLAY", "w*"); // Open "display" peripheral
		//CHECK(hDsp!=NULL, lblKO);
	}

#ifndef __TELIUM3__
	hPrn = Telium_Fopen("PRINTER", "w-*");    // Open "printer" peripheral
	//CHECK(hPrn!=NULL, lblKO);

	hKbd = Telium_Fopen("KEYBOARD", "r*");    // Open "keyboard" peripheral
	//CHECK(hKbd!=NULL, lblKO);
#endif

	if(GL_GraphicLib_IsTouchPresent(hGoal))   // //CHECK if it is a touch screen
	{
		DisplayFooter(_OFF_);                 // Remove footer
		hTsc = Telium_Fopen("TSCREEN", "r*"); // Open "touch" peripheral
		//CHECK(hTsc!=NULL, lblKO);
	}

	iRet=0;
	goto lblEnd;

	// Errors treatment
    // ****************
lblKO:                                        // Open peripherals failed
	iRet=-1;
	goto lblEnd;
lblEnd:
	return iRet;
}

int GetPpdDisplay(void)
{
	// Local variables
    // ***************
    int iDisplay;
    int iRet;

    // //CHECK if pinpad declared from Manager and connected to the terminal
    // *******************************************************************
    iRet = PSQ_Is_pinpad(); //CHECK(iRet!=0, lblKO);
	iRet = IsPPSConnected(); //CHECK(iRet==RET_PP_OK, lblKO);

	// Retrieve the type of display
	// ****************************
	iRet = PSQ_Pinpad_Value();
	switch (iRet)
	{
	case PP30_PPAD:          // Pinpad CHARACTERS no-reader Booster1
	case PP30S_PPAD:         // Pinpad CHARACTERS no-reader Booster2
	case IPP220_PPAD:        // Pinpad CHARACTERS no-reader Booster3
	case IPP250_PPAD:        // Pinpad CHARACTERS no-reader color Booster3
		iDisplay=1;          // Display = CHARACTERS
        break;
	case PPC30_PPAD:         // Pinpad GRAPHIC no-reader Booster1
	case PPR30_PPAD:         // Pinpad GRAPHIC reader Booster1
	case P30_PPAD:           // Pinpad GRAPHIC reader Booster2
	case IPP280_PPAD:        // Pinpad GRAPHIC no-reader color Booster3
	case IPP320_PPAD:        // Pinpad GRAPHIC reader Booster3
	case IPP350_PPAD:        // Pinpad GRAPHIC reader color Booster3
		iDisplay=2;          // Display = GRAPHIC
		break;
	default:
		iDisplay=-1;         // Pinpad unknown
		break;
	}

	goto lblEnd;

	// Errors treatment
    // ****************
lblKO:
    iDisplay=0;              // No pinpad connected
    goto lblEnd;
lblEnd:
    return iDisplay;
}

/*-----------------------------------------------------------------
 * Fonction   :Imprime_ticket_teleechou
 * objet      :
 *
 * Parametre  :
 * Retour     :
 *-----------------------------------------------------------------
*/
void Imprime_ticket_teleAutoechou()
{

	char pPtr[500];
	uchar data_time[13];
    DATE date;

	int iRet;

	T_GL_HWIDGET hDocument = NULL;
	T_GL_HWIDGET xDocument = NULL;

	ST_PRN_LINE xLine;

	byte p; // Printer line index
	T_GL_HGRAPHIC_LIB hGoal = NULL;


	CUMORE_CMI_AFFICHE_MESSAGE(94,32,1000);
	printLogo(COMMERCANT);

	p = 0;                                            // Create document
	hDocument = GoalCreateDocument(APEMV_UI_GoalHandle(), GL_ENCODING_ISO_8859_1);
	xDocument = GoalCreateDocument(APEMV_UI_GoalHandle(), GL_ENCODING_ISO_8859_1);
	CHECK(hDocument != NULL, lblKO);
	// *** Font scale capability ***
	xLine = xPrinter;
	xLine.xFont.eScale = GL_SCALE_LARGE;
	xLine.eTextAlign = GL_ALIGN_CENTER;
	xLine.eBackAlign = GL_ALIGN_CENTER;


	xLine.xFont.eStyle = GL_FONT_STYLE_NORMAL;
	sprintf(pPtr, "-----------------------------------------------");
	iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
	CHECK(iRet >= 0, lblKO);

	xLine.xFont.eStyle = GL_FONT_STYLE_BOLD;
	Telium_Read_date(&date);
	sprintf(pPtr, "%.2s/%.2s/%.2s                            %.2s:%.2s:%.2s",
			date.day, date.month, date.year, date.hour, date.minute,
			date.second);
	iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
	CHECK(iRet >= 0, lblKO);

	xLine.xFont.eStyle = GL_FONT_STYLE_NORMAL;
	sprintf(pPtr, "-----------------------------------------------");
	iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
	CHECK(iRet >= 0, lblKO);


	xLine.xFont.eStyle = GL_FONT_STYLE_BOLD;
	sprintf(pPtr, "TELECOLLECTE\nAUTOMATIQUE ECHOUEE");
	iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
	CHECK(iRet >= 0, lblKO);

	iRet = GoalPrnDocument(hDocument);              // Print document
	CHECK(iRet >= 0, lblKO);
	GoalDestroyDocument(&hDocument);

	printLogo(REFUSED);

	p=0;
	xLine.xFont.eScale = GL_SCALE_LARGE;
	xLine.xFont.eStyle = GL_FONT_STYLE_NORMAL;
	sprintf(pPtr, "-----------------------------------------------");
	iRet = GoalPrnLine(xDocument, p++, pPtr, &xLine);
	CHECK(iRet >= 0, lblKO);
	xLine.xFont.eStyle = GL_FONT_STYLE_BOLD;
	sprintf(pPtr, "MERCI DE RELANCER LA\nTELECOLLECTE");
	iRet = GoalPrnLine(xDocument, p++, pPtr, &xLine);
	CHECK(iRet >= 0, lblKO);

	xLine.xFont.eStyle = GL_FONT_STYLE_NORMAL;
	sprintf(pPtr, "-----------------------------------------------");
	iRet = GoalPrnLine(xDocument, p++, pPtr, &xLine);
	CHECK(iRet >= 0, lblKO);



	xLine.xFont.eStyle = GL_FONT_STYLE_BOLD;
	sprintf(pPtr, "WWW.NAPS.MA");
	iRet = GoalPrnLine(xDocument, p++, pPtr, &xLine);
	CHECK(iRet >= 0, lblKO);

	iRet = GoalPrnDocument(xDocument);              // Print document
	lblKO: GL_Dialog_Message(hGoal, NULL, "Processing Error printer", GL_ICON_ERROR, GL_BUTTON_VALID, 5 * 1000);
	if (xDocument)
		GoalDestroyDocument(&xDocument);

	CUTERMpaperFeed();
	return OK ;

}

/*-----------------------------------------------------------------
 * Fonction   :Imprime_ticket_teleechou
 * objet      :probleme de connectivit?
 *
 * Parametre  :
 * Retour     :
 *-----------------------------------------------------------------
*/
void Imprime_ticket_teleechou()
{

	char pPtr[500];
	uchar data_time[13];

	DATE date;
	int iRet;

	T_GL_HWIDGET hDocument = NULL;
	T_GL_HWIDGET xDocument = NULL;
	ST_PRN_LINE xLine;
	byte p; // Printer line index
	T_GL_HGRAPHIC_LIB hGoal = NULL;


	CUMORE_CMI_AFFICHE_MESSAGE(94,32,1000);
	printLogo(COMMERCANT);

	p = 0;                                            // Create document
	hDocument = GoalCreateDocument(APEMV_UI_GoalHandle(),
			GL_ENCODING_ISO_8859_1);
	xDocument = GoalCreateDocument(APEMV_UI_GoalHandle(),
				GL_ENCODING_ISO_8859_1);

	CHECK(hDocument != NULL, lblKO);
	// *** Font scale capability ***
	xLine = xPrinter;
	xLine.xFont.eScale = GL_SCALE_LARGE;
	xLine.eTextAlign = GL_ALIGN_CENTER;
	xLine.eBackAlign = GL_ALIGN_CENTER;
	xLine.xFont.eStyle = GL_FONT_STYLE_NORMAL;

	sprintf(pPtr, "-----------------------------------------------");
	iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
	CHECK(iRet >= 0, lblKO);

	xLine.xFont.eStyle = GL_FONT_STYLE_BOLD;
	Telium_Read_date(&date);
	sprintf(pPtr, "%.2s/%.2s/%.2s                            %.2s:%.2s:%.2s",
			date.day, date.month, date.year, date.hour, date.minute,
			date.second);
	iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
	CHECK(iRet >= 0, lblKO);

	xLine.xFont.eStyle = GL_FONT_STYLE_NORMAL;
	sprintf(pPtr, "-----------------------------------------------");
	iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
	CHECK(iRet >= 0, lblKO);


	xLine.xFont.eStyle = GL_FONT_STYLE_BOLD;
	sprintf(pPtr, "ECHEC DE LA TELECOLLECTE");
	iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
	CHECK(iRet >= 0, lblKO);


	iRet = GoalPrnDocument(hDocument);              // Print document
	CHECK(iRet >= 0, lblKO);
	GoalDestroyDocument(&hDocument);


	printLogo(REFUSED);

	p=0;
	xLine.xFont.eScale = GL_SCALE_LARGE;
	sprintf(pPtr, "VERIFIER LA\nCONNECTIVITE DU TPE");
	iRet = GoalPrnLine(xDocument, p++, pPtr, &xLine);
	CHECK(iRet >= 0, lblKO);

	xLine.xFont.eStyle = GL_FONT_STYLE_NORMAL;
	sprintf(pPtr, "-----------------------------------------------");
	iRet = GoalPrnLine(xDocument, p++, pPtr, &xLine);
	CHECK(iRet >= 0, lblKO);

	xLine.xFont.eStyle = GL_FONT_STYLE_BOLD;
	sprintf(pPtr, "VEUILLEZ RELANCER\nLA TELECOLLECTE");
	iRet = GoalPrnLine(xDocument, p++, pPtr, &xLine);
	CHECK(iRet >= 0, lblKO);

	xLine.xFont.eStyle = GL_FONT_STYLE_NORMAL;
	sprintf(pPtr, "-----------------------------------------------");
	iRet = GoalPrnLine(xDocument, p++, pPtr, &xLine);
	CHECK(iRet >= 0, lblKO);


	xLine.xFont.eStyle = GL_FONT_STYLE_BOLD;
	sprintf(pPtr, "WWW.NAPS.MA");
	iRet = GoalPrnLine(xDocument, p++, pPtr, &xLine);
	CHECK(iRet >= 0, lblKO);

	iRet = GoalPrnDocument(xDocument);              // Print document
	CHECK(iRet >= 0, lblKO);




	lblKO: GL_Dialog_Message(hGoal, NULL, "Processing Error printer", GL_ICON_ERROR, GL_BUTTON_VALID, 5 * 1000);

	if (xDocument)
		GoalDestroyDocument(&xDocument);

	CUTERMpaperFeed();
	return OK ;

}
/*-----------------------------------------------------------------
 * Fonction   :Imprime_ticket_telezero
 * objet      :
 *
 * Parametre  :
 * Retour     :
 *-----------------------------------------------------------------
*/
void Imprime_ticket_telezero()
{
	char pPtr[500];
	uchar data_time[13];

	DATE date;
	int iRet;

	T_GL_HWIDGET hDocument = NULL;
	T_GL_HWIDGET xDocument = NULL;
	ST_PRN_LINE xLine;
	byte p; // Printer line index
	T_GL_HGRAPHIC_LIB hGoal = NULL;

	CUMORE_CMI_AFFICHE_MESSAGE(94, 32, 1000);
	printLogo(COMMERCANT);

	p = 0;                                            // Create document
	hDocument = GoalCreateDocument(APEMV_UI_GoalHandle(), GL_ENCODING_ISO_8859_1);
	xDocument = GoalCreateDocument(APEMV_UI_GoalHandle(), GL_ENCODING_ISO_8859_1);
	CHECK(hDocument != NULL, lblKO);
	// *** Font scale capability ***
	xLine = xPrinter;
	xLine.xFont.eScale = GL_SCALE_LARGE;
	xLine.eTextAlign = GL_ALIGN_CENTER;
	xLine.eBackAlign = GL_ALIGN_CENTER;



	xLine.xFont.eStyle = GL_FONT_STYLE_NORMAL;
	xLine.xFont.eScale = GL_SCALE_LARGE;

	sprintf(pPtr, "-----------------------------------------------");
	iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
	CHECK(iRet >= 0, lblKO);

	xLine.xFont.eStyle = GL_FONT_STYLE_BOLD;
	Telium_Read_date(&date);
	sprintf(pPtr, "%.2s/%.2s/%.2s                            %.2s:%.2s:%.2s",
			date.day, date.month, date.year, date.hour, date.minute,
			date.second);
	iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
	CHECK(iRet >= 0, lblKO);

	xLine.xFont.eStyle = GL_FONT_STYLE_NORMAL;
	sprintf(pPtr, "-----------------------------------------------");
	iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
	CHECK(iRet >= 0, lblKO);
//	memcpy(data_time, StructInfoPayment.Transaction_DateTime, 12);
//	sprintf(pPtr, "%.2s/%.2s/%.2s                            %.2s:%.2s:%.2s",
//			data_time + 4, data_time + 2, data_time, data_time + 6,
//			data_time + 8, data_time + 10);
//	iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
//	CHECK(iRet >= 0, lblKO);

	xLine.xFont.eStyle = GL_FONT_STYLE_BOLD;
	sprintf(pPtr, "TELECOLLECTE LANCEE");
	iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
	CHECK(iRet >= 0, lblKO);


	iRet = GoalPrnDocument(hDocument);              // Print document
	CHECK(iRet >= 0, lblKO);
	GoalDestroyDocument(&hDocument);

	printLogo(OK);

//	sprintf(pPtr, " ");
//	iRet = GoalPrnLine(xDocument, p++, pPtr, &xLine);
//	CHECK(iRet >= 0, lblKO);

	xLine.xFont.eStyle = GL_FONT_STYLE_NORMAL;

	sprintf(pPtr, "-----------------------------------------------");
	iRet = GoalPrnLine(xDocument, p++, pPtr, &xLine);
	CHECK(iRet >= 0, lblKO);

	xLine.xFont.eStyle = GL_FONT_STYLE_BOLD;
	sprintf(pPtr, "AUCUNE TRANSACTION");
	iRet = GoalPrnLine(xDocument, p++, pPtr, &xLine);
	CHECK(iRet >= 0, lblKO);

	xLine.xFont.eStyle = GL_FONT_STYLE_NORMAL;
	sprintf(pPtr, "-----------------------------------------------");
	iRet = GoalPrnLine(xDocument, p++, pPtr, &xLine);
	CHECK(iRet >= 0, lblKO);


	xLine.xFont.eStyle = GL_FONT_STYLE_BOLD;
	sprintf(pPtr, "WWW.NAPS.MA");
	iRet = GoalPrnLine(xDocument, p++, pPtr, &xLine);
	CHECK(iRet >= 0, lblKO);

	iRet = GoalPrnDocument(xDocument);              // Print document
	CHECK(iRet >= 0, lblKO);


	lblKO: GL_Dialog_Message(hGoal, NULL, "Processing Error printer",
			GL_ICON_ERROR, GL_BUTTON_VALID, 5 * 1000);
	if (xDocument)
		GoalDestroyDocument(&xDocument);

	CUTERMpaperFeed();
	return OK ;
}

/*-----------------------------------------------------------------
 * Fonction   :Imprime_ticket_trarefus
 * objet      :
 *
 * Parametre  :
 * Retour     :
 *-----------------------------------------------------------------
*/
void Imprime_ticket_trarefus()
{

	ulong MontComp;
	char affiche_mont[14];
	char Exp;

	char montant[13];
	char pPtr[1500];
	uchar data_time[13];
	char affich_Cs[11];
	char buff1[30], buff2[30];

	uchar Card_Nb[20];
	DATE date;
	int iRet;

	T_GL_HWIDGET hDocument = NULL;
	T_GL_HWIDGET xDocument = NULL;
	ST_PRN_LINE xLine;
	byte p; // Printer line index
	T_GL_HGRAPHIC_LIB hGoal = NULL;
	//int iRet;
	char var[60];

	CUMORE_CMI_AFFICHE_MESSAGE(94,32,1000);

	//MessageWithoutConfirmation("Impression... Patientez SVP","file://flash/HOST/impression_encours.png");

	printLogo(COMMERCANT);

	p = 0;                                            // Create document
	hDocument = GoalCreateDocument(APEMV_UI_GoalHandle(), GL_ENCODING_ISO_8859_1);
	xDocument = GoalCreateDocument(APEMV_UI_GoalHandle(), GL_ENCODING_ISO_8859_1);

	// *** Font scale capability ***
	xLine = xPrinter;

	xLine.eBackAlign = GL_ALIGN_CENTER;
	xLine.eTextAlign = GL_ALIGN_CENTER;
	xLine.xFont.eScale = GL_SCALE_LARGE;
	sprintf(pPtr, "-----------------------------------------------");
	iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
	CHECK(iRet >= 0, lblKO);

	xLine.xFont.eStyle = GL_FONT_STYLE_NORMAL;
	xLine.eTextAlign = GL_ALIGN_LEFT;
	xLine.eBackAlign = GL_ALIGN_LEFT;
	//CHECK(iRet >= 0, lblKO);



	Telium_Read_date(&date);
	sprintf(pPtr, "%.2s/%.2s/%.2s                            %.2s:%.2s:%.2s",
			date.day, date.month, date.year, date.hour, date.minute,
			date.second);
	iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
	CHECK(iRet >= 0, lblKO);

	xLine.xFont.eStyle = GL_FONT_STYLE_NORMAL;
	xLine.xFont.eScale = GL_SCALE_LARGE;
	sprintf(pPtr, "-----------------------------------------------");
	iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
	CHECK(iRet >= 0, lblKO);

	xLine.xFont.eStyle = GL_FONT_STYLE_BOLD;
	xLine.eBackAlign = GL_ALIGN_LEFT;
	xLine.xFont.eScale = GL_SCALE_LARGE;
	xLine.eTextAlign = GL_ALIGN_LEFT;
	sprintf(pPtr, "%.15s", FicPara.Num_commerce);// jrizki 25032019
	xLine.eTextAlign = GL_ALIGN_RIGHT;
	xLine.eBackAlign = GL_ALIGN_RIGHT;
	iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
	xLine.eTextAlign = GL_ALIGN_LEFT;
	xLine.eBackAlign = GL_ALIGN_LEFT;
	strcpy(var, ":");
	sprintf(pPtr, "%*s", 29 + strlen(var), var);
	iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
	iRet = GoalPrnLine(hDocument, p++, "Num Commer?ant", &xLine);
	CHECK(iRet >= 0, lblKO);

	xLine.xFont.eStyle = GL_FONT_STYLE_NORMAL;
	xLine.xFont.eScale = GL_SCALE_LARGE;
	sprintf(pPtr, "-----------------------------------------------");
	iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
	CHECK(iRet >= 0, lblKO);


	xLine.eBackAlign = GL_ALIGN_LEFT;
	xLine.xFont.eScale = GL_SCALE_LARGE;
	xLine.eTextAlign = GL_ALIGN_LEFT;

	//Nom du porteur

	sprintf(pPtr, "%.24s", StructInfoPayment.Nom_Prenom);
	iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
	CHECK(iRet >= 0, lblKO);

	memset(Card_Nb, 0, 20);
	memcpy(Card_Nb, &StructInfoPayment.Holder_Card[1],StructInfoPayment.Holder_Card[0]);

	Ushort ik;

	for (ik = 0; ik < strlen((char*) Card_Nb) - 4; ik++)
		Card_Nb[ik] = '*';

	sprintf(pPtr, "%s", Card_Nb);
	iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
	CHECK(iRet >= 0, lblKO);


	xLine.eBackAlign = GL_ALIGN_CENTER;
	xLine.eTextAlign = GL_ALIGN_CENTER;
	xLine.xFont.eScale = GL_SCALE_LARGE;
	xLine.xFont.eStyle = GL_FONT_STYLE_BOLD;

	//	iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
	//	CHECK(iRet >= 0, lblKO);

	if (StructInfoPayment.Transaction_Amount / 100 >= 100000)
		xLine.xFont.eScale = GL_SCALE_LARGE;
	else
		xLine.xFont.eScale = GL_SCALE_LARGE;

	switch (StructInfoPayment.CodeFunction) {
	case VOID_PREAUTO_PARTIEL_CODE:
		memset(affiche_mont, 0x00, 14);
		MontComp = StructInfoPayment.Transaction_Amount
				- StrPreAut.Montant_Accomp;
		long_asc(montant, 12, &MontComp);
		RemoveLeftZeros(montant);
		Exp = 2;
		FormatAmount(montant, affiche_mont, Exp);

		sprintf(pPtr, "MONTANT : %.13s%.3s", affiche_mont,
				StructInfoPayment.Devise_acc.code_alph);
		break;

	default:
		sprintf(pPtr, "MONTANT : %d,%d%d %s",
				StructInfoPayment.Transaction_Amount / 100,
				(StructInfoPayment.Transaction_Amount / 10) % 10,
				StructInfoPayment.Transaction_Amount % 10,
				StructInfoPayment.Devise_acc.code_alph);
		break;
	}

	//		sprintf(pPtr, "\nMONTANT : %d,%d%d %s",
	//				StructInfoPayment.Transaction_Amount / 100,
	//				(StructInfoPayment.Transaction_Amount / 10) % 10,
	//				StructInfoPayment.Transaction_Amount % 10,
	//				StructInfoPayment.Devise_acc.code_alph);
	xLine.eBackAlign = GL_ALIGN_CENTER;
	xLine.eTextAlign = GL_ALIGN_CENTER;
	iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
	CHECK(iRet >= 0, lblKO);

	xLine.xFont.eStyle = GL_FONT_STYLE_NORMAL;
	xLine.xFont.eScale = GL_SCALE_LARGE;

	sprintf(pPtr, "-----------------------------------------------");
	iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
	CHECK(iRet >= 0, lblKO);


	xLine.xFont.eScale = GL_SCALE_LARGE;
	xLine.xFont.eStyle = GL_FONT_STYLE_BOLD;

	sprintf(pPtr, "TRANSACTION REFUSEE");
	iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
	CHECK(iRet >= 0, lblKO);

	iRet = GoalPrnDocument(hDocument);              // Print document
	CHECK(iRet >= 0, lblKO);
	GoalDestroyDocument(&hDocument);

	printLogo(REFUSED);
	p = 0;

	xLine.eBackAlign = GL_ALIGN_CENTER;
	xLine.eTextAlign = GL_ALIGN_CENTER;
	xLine.xFont.eScale = GL_SCALE_LARGE;
	xLine.xFont.eStyle = GL_FONT_STYLE_NORMAL;
	sprintf(pPtr, "-----------------------------------------------");
	iRet = GoalPrnLine(xDocument, p++, pPtr, &xLine);
	CHECK(iRet >= 0, lblKO);
	xLine.xFont.eScale = GL_SCALE_LARGE;

	sprintf(pPtr, "Conservez-moi, j? peux ?tre utile !");
	iRet = GoalPrnLine(xDocument, p++, pPtr, &xLine);
	CHECK(iRet >= 0, lblKO);

	xLine.xFont.eStyle = GL_FONT_STYLE_BOLD;
	sprintf(pPtr, "WWW.NAPS.MA");
	iRet = GoalPrnLine(xDocument, p++, pPtr, &xLine);
	CHECK(iRet >= 0, lblKO);

	iRet = GoalPrnDocument(xDocument);              // Print document
	CHECK(iRet >= 0, lblKO);

	iRet = 1;
	goto lblEnd;
	// Errors treatment
	// ****************

	lblKO: GL_Dialog_Message(hGoal, NULL, "Processing Error printer", GL_ICON_ERROR, GL_BUTTON_VALID, 5 * 1000);
	goto lblEnd;

	lblEnd: if (xDocument)
		GoalDestroyDocument(&xDocument);

	CUTERMpaperFeed();
	return OK ;


}



/*ORIFLAME*/
/*-----------------------------------------------------------------
 * Fonction   :ImprimePay
 * objet      :
 *
 * Parametre  :
 * Retour     :
 *-----------------------------------------------------------------
*/
uchar ImprimePay(uchar langue, uchar source, uchar dup, int i) {

	char pPtr[1500];
	char buff1[30], buff2[30];
	uchar data_time[13];
	char affich_Cs[11];
	uchar montant[13];
	uchar affiche_mont[14];
	uchar Card_Nb[20];
	uchar status;
	Ulong MontComp;
	char NumSeqReliquat[6];

	bool bContinue = TRUE, bContinue2 = TRUE;
	int iHeader, iFooter;
	int iRet,j;

	uchar		Date_YYMMDDHHmmss2[7];
	UC	        heure[4+1];
	DATE date;
	Telium_Read_date(&date);
	sprintf(heure,"%.2s%.2s",date.hour,date.minute);
	sprintf(Date_YYMMDDHHmmss2,"%.2s%.2s%.2s",date.day, date.month, date.year);

	T_GL_HWIDGET hDocument = NULL;
	T_GL_HWIDGET xDocument = NULL;
	ST_PRN_LINE xLine;
	byte p;
	T_GL_HGRAPHIC_LIB hGoal = NULL;
	char var[60];

	CUMORE_CMI_AFFICHE_MESSAGE(94,32,1000);

	printLogo(OR_FACTURE52);

	p = 0;                                            // Create document
	hDocument = GoalCreateDocument(APEMV_UI_GoalHandle(),
			GL_ENCODING_ISO_8859_1);
	CHECK(hDocument != NULL, lblKO);
	xDocument = GoalCreateDocument(APEMV_UI_GoalHandle(), GL_ENCODING_ISO_8859_1);
	CHECK(xDocument != NULL, lblKO);
	// *** Font scale capability ***
	xLine = xPrinter;
	xLine.eBackAlign = GL_ALIGN_CENTER;
	xLine.eTextAlign = GL_ALIGN_CENTER;
	xLine.xFont.eScale = GL_SCALE_XLARGE;
	sprintf(pPtr, "-------------------------------------");
	iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
	CHECK(iRet >= 0, lblKO);

	xLine.xFont.eStyle = GL_FONT_STYLE_NORMAL;
	xLine.xFont.eScale = GL_SCALE_LARGE;
	xLine.eTextAlign = GL_ALIGN_LEFT;
	xLine.eBackAlign = GL_ALIGN_LEFT;

	memcpy(data_time, StructInfoPayment.Transaction_DateTime, 12);
	sprintf(pPtr, "%.2s/%.2s/%.2s                            %.2s:%.2s:%.2s",
			data_time + 4, data_time + 2, data_time, data_time + 6,
			data_time + 8, data_time + 10);
	iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
	CHECK(iRet >= 0, lblKO);

	xLine.xFont.eStyle = GL_FONT_STYLE_BOLD;
	xLine.eTextAlign = GL_ALIGN_CENTER;
	xLine.eBackAlign = GL_ALIGN_CENTER;

	sprintf(pPtr, "DETAIL FACTURE");
	iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);

	xLine.xFont.eStyle = GL_FONT_STYLE_NORMAL;
	xLine.xFont.eScale = GL_SCALE_XLARGE;
	sprintf(pPtr, "-------------------------------------");
	iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
	CHECK(iRet >= 0, lblKO);

for(j=0;j<(i+1);j++)
{


	xLine.xFont.eScale = GL_SCALE_LARGE;
	xLine.eTextAlign = GL_ALIGN_LEFT;
	xLine.eBackAlign = GL_ALIGN_LEFT;
	sprintf(pPtr, "%.20s.", StructInfoPayment.OR_greeting[j]);
	iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
	CHECK(iRet >= 0, lblKO);
	sprintf(pPtr, "\n");
	iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
	CHECK(iRet >= 0, lblKO);


	sprintf(pPtr, "%.20s", StructInfoPayment.OR_firstName[j]);
	iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
	CHECK(iRet >= 0, lblKO);
	sprintf(pPtr, "\n");
	iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
	CHECK(iRet >= 0, lblKO);


	sprintf(pPtr, "%.20s", StructInfoPayment.OR_lastName[j]);
	iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
	CHECK(iRet >= 0, lblKO);
	sprintf(pPtr, "\n");
	iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
	CHECK(iRet >= 0, lblKO);


	sprintf(pPtr, "%.10s", StructInfoPayment.OR_financeID[j]);
	xLine.eTextAlign = GL_ALIGN_RIGHT;
	xLine.eBackAlign = GL_ALIGN_RIGHT;
	iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
	xLine.eTextAlign = GL_ALIGN_LEFT;
	xLine.eBackAlign = GL_ALIGN_LEFT;
	strcpy(var, ":");
	sprintf(pPtr, "%*s", 29 + strlen(var), var);
	iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
	iRet = GoalPrnLine(hDocument, p++, "ID Finance", &xLine);
	CHECK(iRet >= 0, lblKO);



	sprintf(pPtr, "%.12s", StructInfoPayment.OR_remainingDebt[j]);
	xLine.eTextAlign = GL_ALIGN_RIGHT;
	xLine.eBackAlign = GL_ALIGN_RIGHT;
	iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
	xLine.eTextAlign = GL_ALIGN_LEFT;
	xLine.eBackAlign = GL_ALIGN_LEFT;
	strcpy(var, ":");
	sprintf(pPtr, "%*s", 29 + strlen(var), var);
	iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
	iRet = GoalPrnLine(hDocument, p++, "Montant restant", &xLine);
	CHECK(iRet >= 0, lblKO);


	sprintf(pPtr, "%.7s", StructInfoPayment.OR_consultantNumber[j]);
	xLine.eTextAlign = GL_ALIGN_RIGHT;
	xLine.eBackAlign = GL_ALIGN_RIGHT;
	iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
	xLine.eTextAlign = GL_ALIGN_LEFT;
	xLine.eBackAlign = GL_ALIGN_LEFT;
	strcpy(var, ":");
	sprintf(pPtr, "%*s", 29 + strlen(var), var);
	iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
	iRet = GoalPrnLine(hDocument, p++, "Num Consultant", &xLine);
	CHECK(iRet >= 0, lblKO);


	sprintf(pPtr, "%.10s", StructInfoPayment.OR_invoiceNumber[j]);
	xLine.eTextAlign = GL_ALIGN_RIGHT;
	xLine.eBackAlign = GL_ALIGN_RIGHT;
	iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
	xLine.eTextAlign = GL_ALIGN_LEFT;
	xLine.eBackAlign = GL_ALIGN_LEFT;
	strcpy(var, ":");
	sprintf(pPtr, "%*s", 29 + strlen(var), var);
	iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
	iRet = GoalPrnLine(hDocument, p++, "Num facture", &xLine);
	CHECK(iRet >= 0, lblKO);


	StructInfoPayment.Transaction_Amount = 0;
	StructInfoPayment.Transaction_Amount += (StructInfoPayment.OR_paidamount[j][11]-48);
	StructInfoPayment.Transaction_Amount += 10*(StructInfoPayment.OR_paidamount[j][10]-48);
	StructInfoPayment.Transaction_Amount += 10*10*(StructInfoPayment.OR_paidamount[j][9]-48);
	StructInfoPayment.Transaction_Amount += 10*10*10*(StructInfoPayment.OR_paidamount[j][8]-48);
	StructInfoPayment.Transaction_Amount += 10*10*10*10*(StructInfoPayment.OR_paidamount[j][7]-48);
	StructInfoPayment.Transaction_Amount += 10*10*10*10*10*(StructInfoPayment.OR_paidamount[j][6]-48);
	StructInfoPayment.Transaction_Amount += 10*10*10*10*10*10*(StructInfoPayment.OR_paidamount[j][5]-48);


//	if (StructInfoPayment.Transaction_Amount / 100 >= 100000)
//		xLine.xFont.eScale = GL_SCALE_LARGE;
//	else
//		xLine.xFont.eScale = GL_SCALE_XLARGE;

	sprintf(pPtr, "%d,%d%d %s", StructInfoPayment.Transaction_Amount / 100,
			(StructInfoPayment.Transaction_Amount / 10) % 10,
			StructInfoPayment.Transaction_Amount % 10,
			StructInfoPayment.Devise_acc.code_alph);
	xLine.eTextAlign = GL_ALIGN_RIGHT;
	xLine.eBackAlign = GL_ALIGN_RIGHT;
	iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);
	xLine.eTextAlign = GL_ALIGN_LEFT;
	xLine.eBackAlign = GL_ALIGN_LEFT;
	strcpy(var, ":");
	sprintf(pPtr, "%*s", 29 + strlen(var), var);
	iRet = GoalPrnLine(hDocument, p, pPtr, &xLine);

	if(!strcmp(StructInfoPayment.OR_invoiceNumber[j]," "))
		iRet = GoalPrnLine(hDocument, p++, "Avance", &xLine);
	else
		iRet = GoalPrnLine(hDocument, p++, "Montant Pay?", &xLine);



	CHECK(iRet >= 0, lblKO);




	sprintf(pPtr, "\n");
		iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
		CHECK(iRet >= 0, lblKO);

}
	xLine.eBackAlign = GL_ALIGN_CENTER;
	xLine.eTextAlign = GL_ALIGN_CENTER;
	xLine.xFont.eScale = GL_SCALE_XLARGE;
	sprintf(pPtr, "-------------------------------------");
	xLine.xFont.eStyle = GL_FONT_STYLE_NORMAL;
	iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
	CHECK(iRet >= 0, lblKO);





//	if (dup) {
//		sprintf(pPtr, "\nDUPLICATA\n\n");
//		iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
//		CHECK(iRet >= 0, lblKO);
//	}

	xLine.xFont.eStyle = GL_FONT_STYLE_NORMAL;
	xLine.xFont.eScale = GL_SCALE_LARGE;

	if (langue == ENG) {
		if (source == CLIENT)
			sprintf(pPtr, "Client Copy");
		else
			sprintf(pPtr, "Merchant Copy");
	} else {
		if (source == CLIENT)
			sprintf(pPtr, "Copie client");
		else
			sprintf(pPtr, "Copie commer?ant", 135);
	}
	iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
	CHECK(iRet >= 0, lblKO);



	sprintf(pPtr, "-------------------------------------");
	xLine.xFont.eScale = GL_SCALE_XLARGE;
	iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
	CHECK(iRet >= 0, lblKO);
	xLine.xFont.eScale = GL_SCALE_LARGE;
	sprintf(pPtr, "Conservez-moi, je peux ?tre utile !");
	iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
	CHECK(iRet >= 0, lblKO);

	xLine.xFont.eStyle = GL_FONT_STYLE_BOLD;
	sprintf(pPtr, "WWW.ORIFLAME.MA");
	iRet = GoalPrnLine(hDocument, p++, pPtr, &xLine);
	CHECK(iRet >= 0, lblKO);

	iRet = GoalPrnDocument(hDocument);              // Print document
	CHECK(iRet >= 0, lblKO);

//	Os__xprintaa("StructInfoPayment.OR_invoiceNumber[j-1]");
//	Os__xprintd(StructInfoPayment.OR_invoiceNumber[j]);
//	Os__xprintd(strlen(StructInfoPayment.OR_invoiceNumber[j-1]));

	iRet = 1;
	goto lblEnd;
	// Errors treatment
	// ****************

	lblKO: GL_Dialog_Message(hGoal, NULL, "Processing Error printer", GL_ICON_ERROR, GL_BUTTON_VALID, 5 * 1000);
	goto lblEnd;

	lblEnd: if (hDocument)
		GoalDestroyDocument(&hDocument);

	CUTERMpaperFeed();


	return OK ;
}


