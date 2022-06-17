/**
 * \file
 * This module implements the explicit transaction flow.
 *
 * \author  Ingenico
 * \author  Copyright (c) 2012 Ingenico
 *
 * \author  Ingenico has intellectual property rights relating to the technology embodied \n
 *       in this software. In particular, and without limitation, these intellectual property rights may\n
 *       include one or more patents.\n
 *       This software is distributed under licenses restricting its use, copying, distribution, and\n
 *       and decompilation. No part of this software may be reproduced in any form by any means\n
 *       without prior written authorization of Ingenico.
 */

#include "SDK_Naps.h"	//#include "sdk.h"
#include "APCLESS.h"

#include "Goal.h"


/////////////////////////////////////////////////////////////////
//// Macros & preprocessor definitions //////////////////////////

#define __APCLESS_EXPLICIT_STEP_DETECTION                 1        ///< Explicit execution step to detect the card
#define __APCLESS_EXPLICIT_STEP_SELECTION                 2        ///< Explicit execution step to select a card application
#define __APCLESS_EXPLICIT_STEP_KERNEL_EXECUTION          3        ///< Explicit execution step to run a kernel
#define __APCLESS_EXPLICIT_STEP_KERNEL_EXECUTION_BIS      4        ///< Explicit execution step to run again the kernel (used by Visa card for payWave kernel)
#define __APCLESS_EXPLICIT_STEP_CHECK_RESULT              5        ///< Explicit execution step to check kernel result ( to manage restart)
#define __APCLESS_EXPLICIT_STEP_END                       99       ///< Explicit execution step to stop processing


// Properties of image (Goal)
// =======================================
static const ST_IMAGE m_xImage480 =
{
	{GL_COLOR_WHITE, TRUE, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_MEDIUM}, GL_DIRECTION_NONE, {0, 0, 0, 0, GL_COLOR_RED}, {70, 70, 70, 70} },
	{TRUE, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_MEDIUM}, GL_DIRECTION_NONE, {1, 1, 1, 1, GL_COLOR_BLACK}, {2, 1, 2, 1} }
};

static const ST_IMAGE m_xImage240 =
{
	{GL_COLOR_BLACK, TRUE, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_MEDIUM}, GL_DIRECTION_NONE, {0, 0, 0, 0, GL_COLOR_RED}, {20, 20, 20, 20} },
	{TRUE, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_MEDIUM}, GL_DIRECTION_NONE, {1, 1, 1, 1, GL_COLOR_BLACK}, {2, 1, 2, 1} }
};

static const ST_IMAGE m_xImage2_480 =
{
	{GL_COLOR_BLACK, NULL, {"LUCIDA", GL_FONT_STYLE_NORMAL, GL_SCALE_MEDIUM}, GL_DIRECTION_NONE, {0, 0, 0, 0, GL_COLOR_RED}, {100, 0, 100, 100} },
	{TRUE, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_MEDIUM}, GL_DIRECTION_NONE, {1, 1, 1, 1, GL_COLOR_BLACK}, {2, 1, 2, 1} }
};

static const ST_IMAGE m_xImage2_240 =
{
	{GL_COLOR_BLACK, NULL, {"LUCIDA", GL_FONT_STYLE_NORMAL, GL_SCALE_MEDIUM}, GL_DIRECTION_NONE, {0, 0, 0, 0, GL_COLOR_RED}, {50, 0, 50, 50} },
	{TRUE, {NULL, GL_FONT_STYLE_NORMAL, GL_SCALE_MEDIUM}, GL_DIRECTION_NONE, {1, 1, 1, 1, GL_COLOR_BLACK}, {2, 1, 2, 1} }
};


/////////////////////////////////////////////////////////////////
//// Global data definition /////////////////////////////////////


/////////////////////////////////////////////////////////////////
//// Static functions definition ////////////////////////////////


/////////////////////////////////////////////////////////////////
//// Functions //////////////////////////////////////////////////


void AfficheMontantCless(void)
{
	char AFFMONT[128];
	char DEVALP[4];

	if(!memcmp(StructInfoPayment.Devise_acc.code_num,"504",3))
		sprintf(DEVALP,"%s","DH");//MAD
	else if(!memcmp(StructInfoPayment.Devise_acc.code_num,"978",3))
		sprintf(DEVALP,"%s","EUR");
	else if(!memcmp(StructInfoPayment.Devise_acc.code_num,"840",3))
		sprintf(DEVALP,"%s","USD");
	else
		sprintf(DEVALP,"%s","ERR");

//	sprintf(AFFMONT,"ATTENTE CARTE\nSANS CONTACT\n%s\n%d,%.02d%s","MONTANT :                        ",

	if (StructInfoPayment.CodeFunction == PAIEMENT_POURBOIRE) {
		StructInfoPayment.CodeFunction = PAIEMENT_POURBOIRE;
		sprintf(AFFMONT, "ATTENTE CARTE\n%s%d,%.02d%s", "MONTANT :",
				(StructInfoPayment.Transaction_Amount
						+ StructInfoPayment.MontantPourboireInterchange) / 100,
				(StructInfoPayment.Transaction_Amount
						+ StructInfoPayment.MontantPourboireInterchange) % 100, DEVALP);
	} else {
//			sprintf(pc_l_DisplayTxt1,"%d,%.02d%s\nPIN:",StructInfoPayment.Transaction_Amount/100,StructInfoPayment.Transaction_Amount%100,"MAD");
		//StructInfoPayment.CodeFunction = PAIEMENT_POURBOIRE;
		sprintf(AFFMONT,"ATTENTE CARTE\n%s%d,%.02d%s","MONTANT :",
			StructInfoPayment.Transaction_Amount/100,
			StructInfoPayment.Transaction_Amount%100,
			DEVALP);
	}



//	sprintf(AFFMONT,"ATTENTE CARTE\n%s%d,%.02d%s","MONTANT :",
//	StructInfoPayment.Transaction_Amount/100,
//	StructInfoPayment.Transaction_Amount%100,
//	DEVALP);

	//APEMV_UI_MessageDisplayDelay (AFFMONT, 0);


	T_GL_HWIDGET hPicture=NULL;

	MessageWithoutConfirmation(AFFMONT, "file://flash/HOST/sans_contact.png");
//	MessageWithoutConfirmation(AFFMONT, NULL);
//	hPicture = GoalCreatePicture(APEMV_UI_GoalHandle());                             // Create and clear screen
//	GoalDspPicture(hPicture, AFFMONT, "file://flash/HOST/logosanscontact_2.png", &m_xImage240, (GL_TIME_SECOND) * 0, true);
//	GoalDestroyPicture(&hPicture);
}

/**
 * This function performs all a contactless transaction using the explicit selection method.
 * The main steps are:
 *  - parameters initialization
 *  - card detection and application selection
 *  - contactless kernel execution
 */
void APCLESS_Explicit_DoTransaction(void)
{
   int result;
   TLV_TREE_NODE selectionTlvTree = NULL;
   T_SHARED_DATA_STRUCT* selectionSharedData = NULL;
   T_SHARED_DATA_STRUCT* kernelSharedBuffer = NULL;
   Telium_File_t* keyboard;
   unsigned char end, cancel = FALSE;
   int event;
   int kernelToUse;
   int step;
   int previousUiState;


   unsigned char ucTempBuffer[6];
   unsigned long ulAmountAuth = 0;

   // ********** Initialize application parameters **************
   // Setup the user interface environment
	previousUiState = APCLESS_Gui_TransactionBegin();
   // ********** Initialize transaction parameters **************
   // Set the transaction parameters
   APCLESS_ParamTrn_SetTransactionType(APCLESS_TRANSACTION_TYPE_DEBIT);

   ulAmountAuth = StructInfoPayment.Transaction_Amount;//Cless_ Montant recu de la caisse
   memset (ucTempBuffer, 0, sizeof(ucTempBuffer));
   GTL_Convert_UlToDcbNumber(ulAmountAuth, ucTempBuffer, 6);
   APCLESS_ParamTrn_SetAmountBcd((unsigned char*) ucTempBuffer); 		//APCLESS_ParamTrn_SetAmountBcd((unsigned char*) "\x00\x00\x00\x00\x01\x23");  //set the transaction amount 1.23 EUR

   memset (ucTempBuffer, 0, sizeof(ucTempBuffer));
   ucTempBuffer[0] = (unsigned char)(StructInfoPayment.Devise_acc.code_num[0] - '0');	//Cless_ Devise recu de la caisse
   ucTempBuffer[1] = (unsigned char)(((StructInfoPayment.Devise_acc.code_num[1] - '0') << 4) | (StructInfoPayment.Devise_acc.code_num[2] - '0'));
   APCLESS_ParamTrn_SetCurrencyCode((unsigned char*) ucTempBuffer); 	//APCLESS_ParamTrn_SetCurrencyCode((unsigned char*) "\x09\x78"); // Euro currency code

   APCLESS_ParamTrn_SetCurrencyExponent(2);
   APCLESS_ParamTrn_SetDateTime();  // use terminal current date
   // Indicate the selection is explicit
   APCLESS_Selection_SetMethod(APCLESS_SELECTION_EXPLICIT);
   // Set the transaction kernel to "unknown"
   APCLESS_ParamTrn_SetCurrentPaymentScheme(APCLESS_SCHEME_UNKNOWN);
   // Indicate no tap is in progress
   APCLESS_ParamTrn_SetDoubleTapInProgress(FALSE);

   // ********** Initialize card detection & selection parameters **************
   // create selection TLV tree (for selection input parameters)
   selectionTlvTree = TlvTree_New(0);
   if (selectionTlvTree == NULL)
   {
      goto End;
   }
   // Create the selection shared buffer (for selection result)
   selectionSharedData = GTL_SharedExchange_InitShared(10240);
   if (selectionSharedData == NULL)
   {
      goto End;
   }
   // Create the kernel shared buffer
   kernelSharedBuffer = GTL_SharedExchange_InitShared(C_SHARED_KERNEL_BUFFER_SIZE);
   if (kernelSharedBuffer == NULL)
   {
      goto End;
   }

   // Set and load data for application explicit selection
   result = APCLESS_Selection_GiveInfo(&selectionTlvTree, TRUE);
   if (result != TRUE)
   {
      goto End;
   }
   result = Cless_ExplicitSelection_LoadData(selectionTlvTree);
   if (result != CLESS_STATUS_OK)
   {
      goto End;
   }
   // Set the customization function to be called for GUI customization
   AfficheMontantCless();	//Cless_ExplicitSelection_Custom_RegistrationForGui(&APCLESS_Selection_GuiCustomisation);
   // Perform the selection pre-processing
   result = Cless_ExplicitSelection_EntryPoint_TransactionPreProcessing();
   if (result != CLESS_STATUS_OK)
   {
      goto End;
   }

   // ********** Execute transaction steps (detection, selection, kernel execution) **************

   step = __APCLESS_EXPLICIT_STEP_DETECTION;
   while(step < __APCLESS_EXPLICIT_STEP_END)
   {
      switch(step)
      {
         // ********** Execute card detection **************
         case __APCLESS_EXPLICIT_STEP_DETECTION:

            // Clear the selection Shared buffer (for selection result)
            GTL_SharedExchange_ClearEx(selectionSharedData, FALSE);
            // Launch the card detection
            result = Cless_ExplicitSelection_GlobalCardDetection();
            if (result != CLESS_STATUS_OK)
            {
               step = __APCLESS_EXPLICIT_STEP_END;
               break;
            }
            AfficheMontantCless();
            // Wait contact'less event (or cancel if red key is pressed)
            end = FALSE;
            cancel = FALSE;

            keyboard = Telium_Fopen("KEYBOARD", "r");
/*
           // int counter = TMT_Retrieve_Clock();
           // 24062019
            do                                                        //larbi_timeout
            {
//				#define T_VAL        0x16
				event = Telium_Ttestall(CLESS, 5*100);
    //           event = Telium_Ttestall(KEYBOARD | CLESS, 10*100);
//               event = Telium_Ttestall(KEYBOARD | CLESS | CAM0 | SWIPE31 | SWIPE2 | SWIPE3, 10*100);
//               if(TMT_Retrieve_Clock() - counter > 9000){
////            	   Os__xprinta("TMT_Retrieve_Clock");
//            	   break;
//               }else

			   if (event & CLESS)
               {
                  end = TRUE;
            	  // Os__xprinta("CLESS");
            	   break;
               }
               else if (event & KEYBOARD)   // event  KEYBOARD
               {
                  if (Telium_Getc(keyboard) == T_ANN)
                  {
//                	  Os__xprinta("ANN");
                     Cless_ExplicitSelection_GlobalCardDetectionCancel();
                     cancel = TRUE;
                     end = TRUE;
                     break;
                  }
               }
               else		//event TIME OUT
               {
//            	   Os__xprinta("ELSE");
                   cancel = TRUE;
                   end = TRUE;
                   break;
               }
            } while(!end);
*/

            do {
				event = Telium_Ttestall(KEYBOARD | CLESS, 6*100);
				if (event & KEYBOARD) {
					//Os__xprintaa("001");
					if (Telium_Getc(keyboard) == T_ANN) {
						//Os__xprintaa("002");
//						if (Cless_ExplicitSelection_GlobalCardDetectionCancel()
//								== CLESS_STATUS_OK) {
//							Os__xprintaa("003");
							cancel = TRUE;
//						}
					}
				} else {
					end = TRUE;
				}
			} while (!end);
			//Os__xprintaa("004");


//            Telium_File_t * keyboard = NULL;
//            unsigned char Status;
//            do
//            {
//                Telium_Ttestall(KEYBOARD | CLESS , 6*100); // time = 1s
//               Telium_Status( keyboard, Status );// bState = 0x20 if a key is pressed (ex: if key '1', '3', '5' or '6' is pressed)
//            }
//            while(Status!=0);


            if (keyboard != NULL){
    			//Os__xprintaa("005");
            	Telium_Fclose (keyboard);
            }
            // Check if card detection has been canceled
            if(cancel == TRUE)
            {
            	//Os__xprintaa("006");
               step = __APCLESS_EXPLICIT_STEP_END;
               //Os__xprinta("Here 1");
               Retour_Caisse("118");			//Cless_ Carte non enregistrée
               break;
            }
            //Os__xprintaa("007");
            // Get the card detection result
            result = Cless_Generic_CardDetectionGetResults(selectionSharedData, TRUE);
            if (result != CLESS_STATUS_OK)
            {
               step = __APCLESS_EXPLICIT_STEP_END;
               //Os__xprinta("Here 2");
               Retour_Caisse("118");			//Cless_ Carte non enregistrée
               break;
            }

            step = __APCLESS_EXPLICIT_STEP_SELECTION;
            break;

         // ********** Execute card selection **************
         case __APCLESS_EXPLICIT_STEP_SELECTION:
        	 //APEMV_UI_MessageDisplayDelay("DD.02",1);
            // Launch application selection
            result = Cless_ExplicitSelection_Selection_ApplicationSelectionProcess(selectionSharedData);
            if (result != CLESS_STATUS_OK)
            {
               if(result == CLESS_STATUS_COMMUNICATION_ERROR)
               {  // communication error -> retry card detection & selection
                  result = CLESS_CR_MANAGER_RESTART;
                  step = __APCLESS_EXPLICIT_STEP_CHECK_RESULT;
               }
               else
               {
                  step = __APCLESS_EXPLICIT_STEP_END;
               }
               break;
            }

            step = __APCLESS_EXPLICIT_STEP_KERNEL_EXECUTION;
            break;

         // ********** Execute a kernel **************
         case __APCLESS_EXPLICIT_STEP_KERNEL_EXECUTION:
        	 //APEMV_UI_MessageDisplayDelay("DD.03",1);
            // First clear the buffer to be used with the contactless kernel
            GTL_SharedExchange_ClearEx (kernelSharedBuffer, FALSE);

            // Add the generic transaction data (previously saved) in the shared buffer (date, time, amount, etc).
            if (!APCLESS_Kernel_AddTransactionGenericData(kernelSharedBuffer))
            {
               step = __APCLESS_EXPLICIT_STEP_END;
               break;
            }

            // Add AID related data to the shared buffer used with kernels
            if (!APCLESS_Kernel_AddAidRelatedData (selectionSharedData, kernelSharedBuffer, &kernelToUse))
            {
               step = __APCLESS_EXPLICIT_STEP_END;
               break;
            }
            // Call kernel in relationship with AID
            switch (kernelToUse)
            {
               case DEFAULT_EP_KERNEL_PAYPASS :
                  //result = APCLESS_PayPass_PerformTransaction(kernelSharedBuffer);
            	  //APEMV_UI_MessageDisplayDelay("PAYPASS KERNEL SELECTED",1);
                  result = ClessSample_PayPass_PerformTransaction(kernelSharedBuffer);
                  break;

               case DEFAULT_EP_KERNEL_VISA :
                  //result = APCLESS_payWave_PerformTransaction(kernelSharedBuffer);
            	  //APEMV_UI_MessageDisplayDelay("VISA KERNEL SELECTED",1);
                  result = ClessSample_PayWave_PerformTransaction(kernelSharedBuffer);
                  break;

               case DEFAULT_EP_KERNEL_AMEX :
                  //result = APCLESS_ExpressPay_PerformTransaction(kernelSharedBuffer);
                  break;

               case DEFAULT_EP_KERNEL_DISCOVER :
                  //result = APCLESS_Discover_PerformTransaction(kernelSharedBuffer);
                  break;

               case DEFAULT_EP_KERNEL_INTERAC :
                  // result = APCLESS_Interac_PerformTransaction(kernelSharedBuffer);
                  break;

               // ******* Default case **********
               default:
                  // Unknown kernel... problem in parameters ?
                  // Display or log an error ?
                  break;
            }

            step = __APCLESS_EXPLICIT_STEP_CHECK_RESULT;
            break;

         // ********** Check (kernel) result (restart needed ?) **************
         case __APCLESS_EXPLICIT_STEP_CHECK_RESULT:
            switch(result)
            {
               case CLESS_CR_MANAGER_REMOVE_AID:
                  // Restart with application selection
                  APCLESS_ParamTrn_SetCurrentPaymentScheme(APCLESS_SCHEME_UNKNOWN);
                  step = __APCLESS_EXPLICIT_STEP_SELECTION;
                  break;

               case CLESS_CR_MANAGER_RESTART_DOUBLE_TAP:
                  // Restart with card detection
                  Cless_ExplicitSelection_DetectionPrepareForRestart(TRUE);
                  APCLESS_ParamTrn_SetCurrentPaymentScheme(APCLESS_SCHEME_UNKNOWN);
                  step = __APCLESS_EXPLICIT_STEP_DETECTION;
                  break;

               case CLESS_CR_MANAGER_RESTART:
               case CLESS_CR_MANAGER_RESTART_NO_MESSAGE_BEFORE_RETRY:
                  // Restart with card detection
                  Cless_ExplicitSelection_DetectionPrepareForRestart(FALSE);
                  APCLESS_ParamTrn_SetCurrentPaymentScheme(APCLESS_SCHEME_UNKNOWN);
                  step = __APCLESS_EXPLICIT_STEP_DETECTION;
                  break;

               default:
                  // Other result value -> end processing
                  step = __APCLESS_EXPLICIT_STEP_END;
                  break;
            }

            break;

         // ******* Default case **********
         default:    // iStep value unknown -> end processing
            step = __APCLESS_EXPLICIT_STEP_END;
            break;
      }
   }


End :   // End processing (cleaning)

   // Wait end of message display
   APCLESS_Gui_DisplayScreen(APCLESS_SCREEN_WAIT_END_DISPLAY);

   // Release the TLV Tree if allocated
   if (selectionTlvTree != NULL)
   {
      TlvTree_Release(selectionTlvTree);
      selectionTlvTree = NULL;
   }

   // Release shared buffer if allocated
   if (selectionSharedData != NULL)
      GTL_SharedExchange_DestroyShare(selectionSharedData);

   if (kernelSharedBuffer != NULL)
      GTL_SharedExchange_DestroyShare(kernelSharedBuffer);

   // Clear the detection/selection resources
   Cless_ExplicitSelection_ClearGlobalData();

   // Reset APCLESS transaction parameters
   APCLESS_Selection_SetMethod(APCLESS_SELECTION_UNKNOWN);
   APCLESS_ParamTrn_SetCurrentPaymentScheme(APCLESS_SCHEME_UNKNOWN);
   APCLESS_ParamTrn_SetDoubleTapInProgress(FALSE);

   // Set the LEDs into the idle state
   APCLESS_Gui_IndicatorIdle();

	// Restore the user interface environment
   APCLESS_Gui_TransactionEnd(previousUiState);
}


