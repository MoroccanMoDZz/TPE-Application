/**
 * \file	DllTesting_Implementation.h
 *
 * \author	Ingenico
 * \author	Copyright (c) 2008 Ingenico, rue claude Chappe,\n
 *			07503 Guilherand-Granges, France, All Rights Reserved.
 *
 * \author	Ingenico has intellectual property rights relating to the technology embodied \n
 *			in this software. In particular, and without limitation, these intellectual property rights may\n
 *			include one or more patents.\n
 *			This software is distributed under licenses restricting its use, copying, distribution, and\n
 *			and decompilation. No part of this software may be reproduced in any form by any means\n
 *			without prior written authorization of Ingenico.
 **/

#ifndef __CLESS_SAMPLE_IMPLEMENTATION_H__INCLUDED__
#define __CLESS_SAMPLE_IMPLEMENTATION_H__INCLUDED__

#include "perf_log.h"

// TDS includes
#include "SDK_Naps.h"	//#include "sdk.h"
#include "TlvTree.h"
#include "del_lib.h"
#include "bitmap.h"
#include "MSGlib.h"
#include "hterm.h"
#include "XMLs.h"
#include "GL_GraphicLib.h"

// Cless includes
#include "oem_cless.h"
#include "oem_clmf1.h"

// Generic Tool Library includes
#include "GTL_Assert.h"
#include "GTL_StdTimer.h"
#include "GTL_Traces.h"
#include "GTL_BerTlv.h"
#include "GTL_BerTlvDecode.h"
#include "GTL_Convert.h"
#include "GTL_TagsInfo.h"
#include "GTL_SharedExchange.h"
#include "GTL_SHA.h"

#include "TPass.h"

// Security includes
#include "SEC_interface.h"

// EMV Lib includes
#include "EmvLib_Tags.h"

// Entry Point includes
#include "EntryPoint_Tags.h"

// Common cless kernel includes
#include "Common_Kernels_Tags.h"
#include "Common_Kernels_API.h"

// PayPass kernel includes
#include "PayPass_Tags.h"
#include "PayPass3_API.h"

// payWave kernel includes
#ifndef DISABLE_OTHERS_KERNELS
#include "payWave_API.h"
#include "payWave_Tags.h"

// VisaWave kernel includes
#include "VisaWave_API.h"
#include "VisaWave_Tags.h"

// ExpressPay kernel includes
#include "ExpressPay_Tags.h"
#include "ExpressPay_API.h"

// Discover kernel includes
#include "Discover_Tags.h"
#include "Discover_API.h"

// Interac kernel includes
#include "Interac_Tags.h"
#include "Interac_API.h"
#endif

// Cless Sample includes
#include "ClessSample_Tags.h"
#include "ClessSample_GuiState.h"
#include "ClessSample_DumpData.h"
#include "ClessSample_Receipt.h"
#include "ClessSample_Common.h"
#include "ClessSample_Scan.h"
#include "ClessSample_payWave.h"
#include "ClessSample_VisaWave.h"
#include "ClessSample_PayPass.h"
#include "ClessSample_ExpressPay.h"
#include "ClessSample_Discover.h"
#include "ClessSample_Interac.h"
#include "ClessSample_Parameters.h"
#include "ClessSample_DataExchange.h"
#include "ClessSample_Fill.h"
#include "ClessSample_FinancialCommunication.h"
#include "ClessSample_Customisation.h"
#include "ClessSample_ExplicitSelection.h"
#include "ClessSample_Menu.h"
#include "ClessSample_Disk.h"
#include "ClessSample_BlackList.h"
#include "ClessSample_PinEntryGoal.h"
#include "ClessSample_Goal.h"
#include "ClessSample_PinManagement.h"
#include "ClessSample_PinEnManagement.h"
#include "ClessSample_Term.h"
#include "ClessSample_Batch.h"
#include "ClessSample_Torn.h"
#include "ClessSample_TransactionFile.h"
#include "ClessSample_Comevent.h"
#include "ClessSample_Unattended.h"
#include "UserInterfaceHelpers.h"
#include "MessagesDefinitions.h"
#include "ClessSample_XML.h"
#include "ClessSample_NameList.h"
#include "ClessSample_Settings.h"
#include "ClessSample_TestPlan.h"
#include "ClessSample_GlobalTransactionData.h"
#include "servcomm.h"

#ifndef __TELIUM3__
#include "tlvVar_def.h"
#include "schVar_def.h"
#endif

#ifndef DISABLE_UNATTENDED
#include "UcmTelium.h"
#include "UcmHostDLL.h"
#include "UcmcLIB.h"
#endif

/////////////////////////////////////////////////////////////////
//// Macros & preprocessor definitions //////////////////////////

#define NUMBER_OF_ITEMS(Array)					(sizeof(Array)/sizeof((Array)[0]))

#define C_INDEX_TEST_BASE						0x0999 // See global range
#define SERVICE_CUSTOM_KERNEL					C_INDEX_TEST_BASE + 0x0000
#define SERVICE_DE_KERNEL						C_INDEX_TEST_BASE + 0x0001

#ifndef _EMVDCTAG_H

//#define  PROPRIETARY_METHOD							0x50			//!< Perform Proprietary method
//#define  NO_REMAINING_PIN							52				//!< PIN Try limit Exceeded.
//#define  INPUT_PIN_OFF								53				//!< Perform PIN Entry for offline verification.
#define  INPUT_PIN_ON								54				//!< Perform PIN Entry for online verification.
#define  INPUT_PIN_OK								55				//!< PIN entry performed.
#define  NO_INPUT									56				//!< CVM End.
#define  PP_HS										60				//!< Pinpad Out of Service.
#define  CANCEL_INPUT								61				//!< Pin entry cancelled.
#define  TO_INPUT  									62				//!< Time Out during PIN Entry.
//#define  REMOVED_CARD				 				63				//!< Card Removed.
//#define  ERROR_CARD									64				//!< Card Error.
//#define  MORE								 		65				//!< Internal use only.
//#define  ERROR_ENC_DATA								66				//!< Error during PIN Encipherment.
//#define  INPUT_INCORRECT_PIN                        67              //!< Incorrect Pin Verification

#endif


#ifndef DES_KEY_SIZE
#define DES_KEY_SIZE	(8)
#endif
#ifndef ERR_TIMEOUT
#define ERR_TIMEOUT                   (-1032)		/*!< Scheme Error code */
#endif
#ifndef TLV_TYPE_KDES
#define TLV_TYPE_KDES           1
#endif
#ifndef CIPHERING_PIN
#define CIPHERING_PIN		           2
#endif
#ifndef BK_SAGEM
#define BK_SAGEM                        0x80000000
#endif

/////////////////////////////////////////////////////////////////
//// Types //////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////
//// Global variables ///////////////////////////////////////////


/////////////////////////////////////////////////////////////////
//// Functions //////////////////////////////////////////////////

extern long atol(const char *nptr);

#undef PSQ_Give_Language
#define PSQ_Give_Language	My_PSQ_Give_Language
int My_PSQ_Give_Language(void);

#endif // __CLESS_SAMPLE_IMPLEMENTATION_H__INCLUDED__
