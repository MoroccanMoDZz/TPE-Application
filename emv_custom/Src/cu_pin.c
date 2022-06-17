/**
* \file cu_pin.c
* \brief This module contains the functions that perform the entry of the PIN code.
* The PIN code can be entered directly on the terminal, or on a pinpad.
* It is kept in the booster.
* In case of enciphered PIN for online presentation, this module also
* encipher the PIN code, and returns the result.
*
* \author Ingenico France
* \author Copyright (c) 2009 Ingenico France, 1 rue claude Chappe,\n
* 07503 Guilherand-Granges, France, All Rights Reserved.
*
* \author Ingenico France has intellectual property rights relating to the technology embodied\n
* in this software. In particular, and without limitation, these intellectual property rights may\n
* include one or more patents.\n
* This software is distributed under licenses restricting its use, copying, distribution, and\n
* and decompilation. No part of this software may be reproduced in any form by any means\n
* without prior written authorization of Ingenico France.
**/

/* ======================================================================== */
/* Include Files                                                            */
/* ======================================================================== */

#include "sdk.h" // IngeDev automatic refactoring - 2014/04/29 14:29:58 - Replace '#include "SDK30.H"' to '#include "sdk.h"'
#include "cu_pinen.h"
#include "cu_term.h"
#include "TlvTree.h"
#include "cu_base.h"
#include "cu_mess.h"
#include "MessagesDefinitions.h"
#include "cu_pin.h"
#include "cu_pinen.h"
#include "del_lib.h"
#include "cu_serv.h"
#include "def_tag.h"
#include "cu_entry.h"
#include "convert.h"
#include "cu_param.h"
#include "gestion_param.h"
#include "cu_logos.h"

#ifdef _USE_UCM_
#include "ucmtelium.h"
#include "ucmhostdll.h"
#include "ucmclib.h"
#endif

#include "sec_interface.h"
#include "schvar_def.h"

#include "GL_GraphicLib.h"
#include "GTL_Assert.h"
#include "GTL_Convert.h"

#include "EPSTOOL_Convert.h"
#include "EPSTOOL_TlvTree.h"
#include "EPSTOOL_PinEntry.h"
#include "EPSTOOL_Unicode.h"
#include "cu_UserInterfaceDisplay.h"


//! \addtogroup Group_cu_pin
//! @{

/* ======================================================================== */
/* Macros & preprocessor definitions                                        */
/* ======================================================================== */

#define CU_INPUT_PIN_OK               0


/* ========================================================================	*/
/* Global variables                                                         */
/* ========================================================================	*/
static unsigned short * ptr_Bypass_Required;

int PinpadValue;

/* ========================================================================	*/
/* Static function definitions                                              */
/* ========================================================================	*/

static void CU_SetConfPinpad (int * PinpadValue);


/* ======================================================================== */
/* Functions                                                                */
/* ======================================================================== */

//===========================================================================
unsigned char CUPIN_EnterPinOnLine(unsigned char nb_try,
								   unsigned char AllowTermIfPPadHS,
								   ENTRY_BUFFER *buffer,
								   unsigned char * ptrPrimaryAccountNumber,
								   unsigned short * ptrBypass)
{
	unsigned char  cr_enter_pin;
	int            iRes, iLenEnc,iResInject;
    int  pinLength ;

	// Header management / display on X07
	//StateHeader (0);

	// Global variable affectation for later use
	ptr_Bypass_Required = ptrBypass;

	iResInject = KO;

	cr_enter_pin = CU_INPUT_PIN_OK;
	// Inject a dummy secret key in the booster
	{
#ifdef _USE_DEMO_
		PIN_InjectKeyDemo();
/*&BEGIN FT/09/5296/MA    SEC_ISO9563 has to be called only if Injection successfull */
		iResInject = OK;
#else
		CU_SetConfPinpad (&PinpadValue); 

		iResInject = OK;
#endif	    
	}

	if (iResInject == OK )
	{
/*&END FT/09/5296/MA    SEC_ISO9563 has to be called only if Injection successfull */

		cr_enter_pin = CUPIN_GL_EnterPin(ONLINE_PIN, &pinLength);

		if ( cr_enter_pin == INPUT_PIN_OK )
		{
			{
#ifdef _USE_DEMO_
			iRes = PIN_OnlineEncipherDemo (ptrPrimaryAccountNumber, buffer->d_entry,&iLenEnc);
#else
				// Encipher the entered PIN
				iRes = PIN_OnlineEncipher (ptrPrimaryAccountNumber, buffer->d_entry,&iLenEnc);
#endif
				if (iRes)
				{
					cr_enter_pin = INPUT_PIN_ON;
					buffer->d_len = (char) iLenEnc;
				}
				else
				{
					cr_enter_pin = PP_HS;
				}
			}
		}
	}
	
	// Header management / display on X07
	//StateHeader (1);

	return(cr_enter_pin);
}


//===========================================================================
//! \brief This function sets the value of the the parameter PinpadValue, according 
//! to the pinpad type.
//! \param[out] PinpadValue : type of the pinpad
//===========================================================================
static void CU_SetConfPinpad (int * PinpadValue)
{


#ifdef _USE_UCM_
	if (!UCM_Present)
	{
#endif
			*PinpadValue=0xff;
			*PinpadValue = PSQ_Pinpad_Value();
#ifdef _USE_UCM_
	}
#endif

}


//! @}

