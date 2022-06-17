/**
* \file cu_receipt.h
* \brief This module contains the functions needed to print a receipt.
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

#ifndef CU_RECEIPT_H
#define CU_RECEIPT_H

/* ======================================================================== */
/* Include Files                                                            */
/* ======================================================================== */

#include "cu_mess.h"	// to use S_TAB_CONFIG
#include "cu_batch.h"	// to use T_CUBATCH

//! \defgroup Group_cu_receipt Receipt printing management.
//! @{

/* ======================================================================== */
/* Macros & preprocessor definitions                                        */
/* ======================================================================== */

/* =========================================================================*/
/* Types declarations                                                       */
/* =========================================================================*/

//! \brief This enumerator defines the states when reading APDU commands stored in EMVDC data base.
typedef enum {
	IS_DONE,												//!< All the APDU command have been read.
	IS_IN_PROGRESS,											//!< The reading of an APDU command is in progress.
	IS_NOT_AVAILABLE										//!< No APDU command is stored.
} t_process;

//! \brief This enumerator defines the alignement for the text to print.
typedef enum {
	ALIGN_LEFT =    0,										//!< Text alignement on the left.
	ALIGN_CENTER =  1,										//!< Text alignement on the center.
	ALIGN_RIGHT =   2										//!< Text alignement on the right.
} t_align;

//! \brief This enumerator defines the printing levels.
typedef enum {
	L0 =    0,												//!< Level 0 of printing. (condensed mode).
	L1 =    1,												//!< Level 1 of printing. (normal mode).
	L2 =    2,												//!< Level 2 of printing. (double height mode).
	L3 =    3,												//!< Level 3 of printing. (double width mode).
	L4 =    4												//!< Level 4 of printing. (bold mode).
} t_level;

//! \brief This enumerator defines the application selection method.
typedef enum {
	DIRECTORY_SELECTION_METHOD               = 0x01,		//!< The method is directory selection.
	EXPLICIT_SELECTION_METHOD                = 0x02,		//!< The method is explicit selection.
	CARD_IS_BLOCKED_OR_COMMAND_NOT_SUPPORTED = 0x04,		//!< The card is blocked or the command is not supported.
	APPLICATION_IS_BLOCKED                   = 0x08,		//!< The application is blocked.
	ERROR_RESPONSE_UNKNOWN                   = 0xFF			//!< Unknown response of the card.
} Selection_Method;

//! \brief This enumerator defines the offline data authentication modes.
typedef enum {
	IS_NOT_SUPPORTED =    0,		//!< The offline data authentication is not supported.
	IS_SDA,							//!< The mode is static data authentication.
	IS_DDA,							//!< The mode is dynamic data authentication. This mode offers fraud protection. The card generates a RSA signature using transaction-specific data for validation by the terminal.
	IS_CDA							//!< The mode is combined dynamic data authentication / application cryptogram generation.
} t_oda;

//! \brief This enumerator defines the different cryptograms.
typedef enum {
	CRYPTO_AAC  = 0x00,				//!< Application authentication cryptogram. An application cryptogram generated when declining a transaction.
	CRYPTO_TC   = 0x40,				//!< Transaction certificate. An application cryptogram generated when accepting a transaction.
	CRYPTO_ARQC = 0x80				//!< Application request cryptogram. An application cryptogram generated when requesting online authorization.
} t_cryptogram;

//! \brief This enumerator defines the different presentation modes of pin code.
typedef enum {
	P_PLAINTEXT_PIN,				//!< The pin code is presented in plain text.
	P_ENCIPHERED_PIN,				//!< The pin code is presented enciphered.
	P_ONLINE_PIN,					//!< The pin code is presented online.
	P_NO_PIN						//!< No method is used.
} t_pin;

//! \brief Definition of transaction flag status.
typedef unsigned int    TFS;

/* ========================================================================	*/
/* Global variables                                                         */
/* ========================================================================	*/

/* ======================================================================== */
/* Global function declarations                                             */
/* ======================================================================== */

//===========================================================================
//! \brief This function prints the transaction ticket.
//! \param[in] hInputTLVTree : input TLV tree containing the transaction informations : 
//! - currency,
//! - date,
//! - time,
//! - amount.
//! \param[in] status_tr : transaction status.
//===========================================================================
void CURECEIPT_Print_Ticket_POS_Demo(TLV_TREE_NODE hInputTLVTree, unsigned char status_tr);

#ifdef _TOOLWARE_
//===========================================================================
//! \brief This function prints the advance demo ticket which contains the all
//! timings (terminal, card, MMI, Host) of the last transaction. This function is 
//! active only if flag _TOOLWARE_ is defined and if the Kernel (EMV DC)
//! supports the TOOLWARE option.
//===========================================================================
void CURECEIPT_Print_Ticket_Advance_Demo(void);
#endif

//===========================================================================
//! \brief This function prints the current EMV Configuration loaded in terminal.
//===========================================================================
void CURECEIPT_Print_EMV_Config(void);

void CUPRINT_String_XT(const unsigned char *string, const t_align align, const t_level level);

void CUPRINT_Paper_Feed(void);

//! @}

#endif
