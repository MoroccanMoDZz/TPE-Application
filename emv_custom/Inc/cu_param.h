/**
* \file cu_param.h
* \brief This module contains the functions needed for parameters management.
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

#ifndef CU_PARAM_H
#define CU_PARAM_H

//! \addtogroup Group_param
//! @{

/* ======================================================================== */
/* Macros & preprocessor definitions                                        */
/* ======================================================================== */

//! \defgroup ParamType Definitions for parameters management.
//! @{

#define C_PARAM_INIT					0		//!< Default & parameter files values.
#define C_PARAM_DEFAULT					1		//!< Default values.
#define C_PARAM_DEFAULT_ICS				2		//!< Default values only for ICS parameters.
#define C_PARAM_ONLY_DEFAULT_AID		3		//!< Load default AID values in pTreeParam TLVStruct and save parameters in param.par file.
#define C_PARAM_ONLY_DEFAULT_AID_EVAL	4		//!< Load default EVAL AID values in pTreeParam TLVStruct and save parameters in param.par file.
#define C_PARAM_ONLY_DEFAULT_AID_VISA	5		//!< Load default VISA AID values in pTreeParam TLVStruct and save parameters in param.par file.
#define C_PARAM_ONLY_DEFAULT_ICS		6		//!< Load default ICS values in pTreeParam TLVStruct and save parameters in param.par file.
#define C_PARAM_FILE_TLVTSERIAL			7		//!< Parameter file(s) TLVTree serial format.
#define C_PARAM_FILE_XML				8		//!< Parameter file(s) XML format.
#define C_PARAM_INIT_APLI				9		//!< Reset application structure used to store specific application tags.
#define C_PARAM_INIT_DEFAULT			10		//!< Reset application structure used to store specific default application tags.

//! @}

/* Init Type for files access : */

#define C_SHA_BUFF_LENGTH				512

//! \addtogroup ParamType
//! @{

#define C_AID1_PARAM					"AID1.PAR"		//!< Name of the AID1 parameter file. This file can be loaded individually after global parameter file EMVCUST.PAR (xml format) to keep the compatibility with previous management.
#define C_AID2_PARAM					"AID2.PAR"		//!< Name of the AID2 parameter file. This file can be loaded individually after global parameter file EMVCUST.PAR (xml format) to keep the compatibility with previous management.
#define C_AID3_PARAM					"AID3.PAR"		//!< Name of the AID3 parameter file. This file can be loaded individually after global parameter file EMVCUST.PAR (xml format) to keep the compatibility with previous management.
#define C_AID4_PARAM					"AID4.PAR"		//!< Name of the AID4 parameter file. This file can be loaded individually after global parameter file EMVCUST.PAR (xml format) to keep the compatibility with previous management.
#define C_AID5_PARAM					"AID5.PAR"		//!< Name of the AID5 parameter file. This file can be loaded individually after global parameter file EMVCUST.PAR (xml format) to keep the compatibility with previous management.
#define C_AID6_PARAM					"AID6.PAR"		//!< Name of the AID6 parameter file. This file can be loaded individually after global parameter file EMVCUST.PAR (xml format) to keep the compatibility with previous management.
#define C_AID7_PARAM					"AID7.PAR"		//
#define C_AID8_PARAM					"AID8.PAR"		//
#define C_AID9_PARAM					"AID9.PAR"		//
#define C_AIDA_PARAM					"AIDA.PAR"		//
#define C_AIDB_PARAM					"AIDB.PAR"		//
#define C_AIDC_PARAM					"AIDC.PAR"		// naps
#define C_AIDD_PARAM					"AIDD.PAR"		// naps
#define C_AIDE_PARAM					"AIDE.PAR"		// naps
#define C_ICS0_PARAM					"ICS.PAR"		//!< Name of the ICS parameter file. This file can be loaded individually after global parameter file EMVCUST.PAR (xml format) to keep the compatibility with previous management.
#define C_ICS1_PARAM					"ICS1.PAR"		//!< Name of the ICS1 parameter file. This file can be loaded individually after global parameter file EMVCUST.PAR (xml format) to keep the compatibility with previous management.
#define C_ICS2_PARAM					"ICS2.PAR"		//!< Name of the ICS2 parameter file. This file can be loaded individually after global parameter file EMVCUST.PAR (xml format) to keep the compatibility with previous management.
#define C_ICS3_PARAM					"ICS3.PAR"		//!< Name of the ICS3 parameter file. This file can be loaded individually after global parameter file EMVCUST.PAR (xml format) to keep the compatibility with previous management.
#define C_ICS4_PARAM					"ICS4.PAR"		//!< Name of the ICS4 parameter file. This file can be loaded individually after global parameter file EMVCUST.PAR (xml format) to keep the compatibility with previous management.
#define C_ICS5_PARAM					"ICS5.PAR"		//!< Name of the ICS5 parameter file. This file can be loaded individually after global parameter file EMVCUST.PAR (xml format) to keep the compatibility with previous management.
#define C_CAKEYS_PARAM					"CAKEYS.PAR"	//!< Name of the CAKEYS parameter file. This file can be loaded individually after global parameter file EMVCUST.PAR (xml format) to keep the compatibility with previous management.
#define C_KREVOK_PARAM					"KREVOK.PAR"	//!< Name of the KREVOK parameter file. This file can be loaded individually after global parameter file EMVCUST.PAR (xml format) to keep the compatibility with previous management.
#define C_BLACKL_PARAM					"BLACKL.PAR"	//!< Name of the BLACKL parameter file. This file is only managed individually.
#define C_FILE_PARAM					"PARAM.PAR"		//!< Name of the PARAM parameter file. This file is used to save the current parameter configuration in case of power failure.
#define C_FILE_PARAM_XML				"EMVCUST.PAR"	//!< Name of the EMVCUST parameter file. This file contains a parameter list to configure the terminal (AID1, AID2, ICS1, ICS2, ...). When this file is loaded, the PARAM.PAR file is initialized with its content.

#define C_AID_NAME						"AID"			//!< Root of AIDx file name.
#define C_ICS_NAME						"ICS"			//!< Root of ICSx file name.
#define C_CAKEYS_NAME					"CAKEYS"		//!< Root of CAKEYS.PAR file name.
#define C_KREVOK_NAME					"KREVOK"		//!< Root of KREVOK.PAR file name.
#define C_BLACKL_NAME					"BLACKL"		//!< Root of BLACKL.PAR file name.
#define C_FILE_NAME_XML					"EMVCUST"		//!< Root of EMVCUST.PAR file name.

//! @}


//! \defgroup AidMgt Definitions for application identifier management.
//! @{

#define NB_AID_MAX						14		//!< Maximum number of AID.
#define NB_KEY_PER_AID					16		//!< Maximum number of keys per RID (EMV 2000).
#define LG_RID							5		//!< Root length of AID.
#define MAX_NB_TAG_FOR_REVOKED_CA		5		//!< Maximum number of tags for a revoked CA.

//! @} // end of AidMgt


//! \defgroup WorkType Parameter text file format
//! @{

#define C_WORK_FILE_TO_BIN				0		//!< Input file will be only one file.
#define C_WORK_BIN_TO_FILE				1		//!< Not used.
#define C_WORK_FILES_TO_BIN				2		//!< Input files may be AIDX.PAR, CAKEYS.PAR, KREVOK.PAR, ICSX.PAR.
#define C_WORK_BIN_TO_FILES				3		//!< Not used.

//! @} // end of WorkType

//! \defgroup TagType Definitions of tag types.
//! @{

#define C_TAG_TYPE_HEXA_BYTE			0		//!< The data of the tag is one hexadecimal byte.
#define C_TAG_TYPE_BUF_BYTE				1		//!< The data of the tag is a byte buffer.
#define C_TAG_TYPE_BUF_BYTE_WITH_MASQ	2		//!< The data of the tag is a byte buffer to be masked.
#define C_TAG_TYPE_BOOLEAN				3		//!< The data of the tag is a boolean value.
#define C_TAG_TYPE_TXT					4		//!< The data of the tag is a text value.

//! @} // end of TagType


//! \defgroup KeyMgt Definitions for keys management.
//! @{

#define MAX_FILE_REVOK_LG				60				//!< Buffer maximum length for a revoked CA in CAREVOK file.
#define NB_MAX_REVOKED_PER_TAG			30				//!< Maximum number of revoked CA per TAG.
#define NB_CAKEY_REVOKED_PER_RID		(NB_MAX_REVOKED_PER_TAG * MAX_NB_TAG_FOR_REVOKED_CA)	//!< Maximum number of revoked CA per RID.
#define NB_MAX_ALL_REVOKED_CA			(NB_CAKEY_REVOKED_PER_RID * NB_AID_MAX)					//!< Maximum number of revoked CA.

//! @} // end of KeyMgt

//! \defgroup FileOrder Definitions for parameter file management.
//! @{

#define C_MGT_PARAM_PAR_FILE			1				//!< Manage only PARAM.PAR parameter file.
#define C_MGT_PARAM_PAR_FILE_FIRST		2				//!< Manage PARAM.PAR parameter file first and if file not present, manage AIDx, CAREVOK...PAR files.
#define C_MGT_TXT_PAR_FILES				3				//!< Manage AIDx, CAREVOK, ICSx...PAR files.

//! @} // end of FileOrder

#define C_NB_MAX_TXT_FILE				8				//!< Maximum number of txt files used for parameters.


//! \defgroup TagMgt Definitions for tag management.
//! @{

#define C_TAG_AID						0x1000				//!< AID node number used when updating parameter TLV tree.
#define C_TAG_AID1						C_TAG_AID + 1		//!< AID1 node number of parameter TLV tree. The TLV tree is updated at loading of AID1.PAR file.
#define C_TAG_AID2						C_TAG_AID + 2		//!< AID2 node number of parameter TLV tree. The TLV tree is updated at loading of AID2.PAR file.
#define C_TAG_AID3						C_TAG_AID + 3		//!< AID3 node number of parameter TLV tree. The TLV tree is updated at loading of AID3.PAR file.
#define C_TAG_AID4						C_TAG_AID + 4		//!< AID4 node number of parameter TLV tree. The TLV tree is updated at loading of AID4.PAR file.
#define C_TAG_AID5						C_TAG_AID + 5		//!< AID5 node number of parameter TLV tree. The TLV tree is updated at loading of AID5.PAR file.
#define C_TAG_AID6						C_TAG_AID + 6		//!< AID6 node number of parameter TLV tree. The TLV tree is updated at loading of AID6.PAR file.
#define C_TAG_AID7						C_TAG_AID + 7		//
#define C_TAG_AID8						C_TAG_AID + 8		//
#define C_TAG_AID9						C_TAG_AID + 9		//
#define C_TAG_AIDA						C_TAG_AID + 10		//
#define C_TAG_AIDB						C_TAG_AID + 11		//
#define C_TAG_AIDC						C_TAG_AID + 12		// naps
#define C_TAG_AIDD						C_TAG_AID + 13		// naps
#define C_TAG_AIDE						C_TAG_AID + 14		// naps

#define C_TAG_CAKEYS					0x1100				//!< CAKEYS node number of parameter TLV tree. The TLV tree is updated at loading of CAKEYS.PAR file.

#define C_TAG_CAREVOK					0x1200				//!< CAREVOK node number of parameter TLV tree. The TLV tree is updated at loading of CAREVOK.PAR file.

#define C_TAG_ICS						0x1300				//!< ICS node number used when updating parameter TLV tree.
#define C_TAG_ICS0						C_TAG_ICS + 1		//!< ICS0 node number of parameter TLV tree. The TLV tree is updated at loading of ICS.PAR file.
#define C_TAG_ICS1						C_TAG_ICS + 2		//!< ICS1 node number of parameter TLV tree. The TLV tree is updated at loading of ICS1.PAR file.
#define C_TAG_ICS2						C_TAG_ICS + 3		//!< ICS2 node number of parameter TLV tree. The TLV tree is updated at loading of ICS2.PAR file.
#define C_TAG_ICS3						C_TAG_ICS + 4		//!< ICS3 node number of parameter TLV tree. The TLV tree is updated at loading of ICS3.PAR file.
#define C_TAG_ICS4						C_TAG_ICS + 5		//!< ICS4 node number of parameter TLV tree. The TLV tree is updated at loading of ICS4.PAR file.
#define C_TAG_ICS5						C_TAG_ICS + 6		//!< ICS5 node number of parameter TLV tree. The TLV tree is updated at loading of ICS5.PAR file.

#define C_TAG_XXXX						0x9F8BA0			// Dummy tag

#define	C_MARK_XXXX						"MARQ_XXXX"
#define	C_MARK_AID						"MARQ_AID"			//!< Label for AID node in parameter TLV tree.
#define	C_MARK_AID1						"MARQ_AID1"			//!< Label for AID1 node in parameter TLV tree.
#define	C_MARK_AID2						"MARQ_AID2"			//!< Label for AID2 node in parameter TLV tree.
#define	C_MARK_AID3						"MARQ_AID3"			//!< Label for AID3 node in parameter TLV tree.
#define	C_MARK_AID4						"MARQ_AID4"			//!< Label for AID4 node in parameter TLV tree.
#define	C_MARK_AID5						"MARQ_AID5"			//!< Label for AID5 node in parameter TLV tree.
#define	C_MARK_AID6						"MARQ_AID6"			//!< Label for AID6 node in parameter TLV tree.
#define	C_MARK_AID7						"MARQ_AID7"			//
#define	C_MARK_AID8						"MARQ_AID8"			//
#define	C_MARK_AID9						"MARQ_AID9"			//
#define	C_MARK_AIDA						"MARQ_AIDA"			//
#define	C_MARK_AIDB						"MARQ_AIDB"			//
#define	C_MARK_AIDC						"MARQ_AIDC"			// naps
#define	C_MARK_AIDD						"MARQ_AIDD"			// naps
#define	C_MARK_AIDE						"MARQ_AIDE"			// naps
#define	C_MARK_CAKEYS					"MARQ_CAKEYS"		//!< Label for CAKEYS node in parameter TLV tree.
#define	C_MARK_CAREVOK					"MARQ_CAREVOK"		//!< Label for CAREVOK node in parameter TLV tree.
#define	C_MARK_ICS						"MARQ_ICS"			//!< Label for ICS node in parameter TLV tree.
#define	C_MARK_ICS0						"MARQ_ICS0"			//!< Label for ICS0 node in parameter TLV tree.
#define	C_MARK_ICS1						"MARQ_ICS1"			//!< Label for ICS1 node in parameter TLV tree.
#define	C_MARK_ICS2						"MARQ_ICS2"			//!< Label for ICS2 node in parameter TLV tree.
#define	C_MARK_ICS3						"MARQ_ICS3"			//!< Label for ICS3 node in parameter TLV tree.
#define	C_MARK_ICS4						"MARQ_ICS4"			//!< Label for ICS4 node in parameter TLV tree.
#define	C_MARK_ICS5						"MARQ_ICS5"			//!< Label for ICS5 node in parameter TLV tree.

//! @} // end of TagMgt


/* =========================================================================*/
/* Types declarations                                                       */
/* =========================================================================*/

typedef unsigned char lst_Tag_RevokCA[255];

typedef struct
{
	lst_Tag_RevokCA revoked_CA[MAX_NB_TAG_FOR_REVOKED_CA];
	unsigned short  Taglength[MAX_NB_TAG_FOR_REVOKED_CA];

} T_CUSERV_lst_tagCArevok;

typedef struct
{
	unsigned char  rid[LG_RID];
	unsigned char  index[1];
	unsigned int   length;
	unsigned char  modulus[255];
	unsigned char  exponent[3];
	unsigned short exponentlength;
	unsigned char  expir_date[3];
} CA_PUB_KEY;

typedef struct
{
	unsigned char rid[LG_RID];
	unsigned char index[1];
	unsigned char serial_number[3];
}CA_REVOK;


#ifdef FILE_H

const unsigned char ay_Mask_Tc[]  = {0x00, 0xF8, 0xC8};
const unsigned char ay_Mask_Atc[] = {0xF0, 0x00, 0x00, 0x00, 0x00};

//const struct Struct_TagList FirstTagList[] =
const Struct_TagList FirstTagList[] = {
	{TAG_TERMINAL_TYPE,                      C_TAG_TYPE_HEXA_BYTE,   NULL},    // Terminal Type
	{TAG_TERMINAL_CAPABILITIES,              C_TAG_TYPE_BUF_BYTE,    NULL},    // Terminal Capabilities
	{TAG_TERMINAL_COUNTRY_CODE,              C_TAG_TYPE_BUF_BYTE,    NULL},    // Terminal country code
	{TAG_TRANSACTION_CURRENCY_CODE,          C_TAG_TYPE_BUF_BYTE,    NULL},    // Terminal currency
	{TAG_ADD_TERMINAL_CAPABILITIES,          C_TAG_TYPE_BUF_BYTE,    NULL},    // Additionnal Terminal Capabilities

	// Other
	{TAG_USE_PSE,                             C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_CARDHOLDER_CONFIRMATION,     C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_PREFERRED_DISPLAY_ORDER,     C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_MULTILANG_SUPPORTED,         C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_REVOK_SUPPORTED,             C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_PIN_BYPASS,                  C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_GETDATA_PIN_TRY_COUNTER,     C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_TRANSACTION_LOG,             C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_EXCEPTION_FILE,              C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_FORCED_ONLINE,               C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_FORCED_ACCEPTANCE,           C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_ONLINE_ADVICE,               C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_ISSUER_REFERRAL,             C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_CARD_REFERRAL,               C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_BATCH_CAPTURE,               C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_ONLINE_CAPTURE,              C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_POS_ENTRY_MODE,              C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_POS_ENTRY_MODE_CODE,                 C_TAG_TYPE_HEXA_BYTE,  NULL},
	{TAG_CUST_IS_PINPAD,                      C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_AMOUNT_PIN_SAME,             C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_DEFAULT_DDOL,                C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_FLOOR_LIMIT_CHECKING,        C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_RANDOM_TRANSACTION_SELECT,   C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_VELOCITY_CHECKING,           C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_DEFAULT_TDOL,                C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_TRM_OVERPASS_AIP,               C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_SKIP_DEFAULT,                C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_SKIP_ONLINE,                 C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_ODA_PROCESSING,              C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_ACCOUNT_TYPE,                C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_PSE_ALGO,                            C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_DETECT_FAIL_PRIOR_TAA,          C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_CDA_NEVER_REQUEST_ARQC_1GENAC,  C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_CDA_NEVER_REQUEST_ONLINE_2GENAC,C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_SUBSEQUENT_BYPASS_PIN,          C_TAG_TYPE_BOOLEAN,    NULL}
};


//const struct Struct_TagList MajorTagList[] =
const Struct_TagList MajorTagList[] =
{
	{TAG_TERMINAL_TYPE,                       C_TAG_TYPE_HEXA_BYTE,  NULL},                        // Terminal Type
	{TAG_TERMINAL_CAPABILITIES,               C_TAG_TYPE_BUF_BYTE,   (unsigned char*)ay_Mask_Tc},  // Terminal Capabilities
	{TAG_ADD_TERMINAL_CAPABILITIES,           C_TAG_TYPE_BUF_BYTE,   (unsigned char*)ay_Mask_Atc}, // Additionnal Terminal Capabilities
	// Other
	{TAG_USE_PSE,                             C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_CARDHOLDER_CONFIRMATION,     C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_PREFERRED_DISPLAY_ORDER,     C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_REVOK_SUPPORTED,             C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_PIN_BYPASS,                  C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_GETDATA_PIN_TRY_COUNTER,     C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_TRANSACTION_LOG,             C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_EXCEPTION_FILE,              C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_FORCED_ONLINE,               C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_FORCED_ACCEPTANCE,           C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_ONLINE_ADVICE,               C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_ISSUER_REFERRAL,             C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_CARD_REFERRAL,               C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_BATCH_CAPTURE,               C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_ONLINE_CAPTURE,              C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_PINPAD,                      C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_AMOUNT_PIN_SAME,             C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_DEFAULT_DDOL,                C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_FLOOR_LIMIT_CHECKING,        C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_RANDOM_TRANSACTION_SELECT,   C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_VELOCITY_CHECKING,           C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_DEFAULT_TDOL,                C_TAG_TYPE_BOOLEAN,    NULL}
};
const Struct_TagList FirstTagListEntire[] = {
	{TAG_TERMINAL_TYPE,                      C_TAG_TYPE_HEXA_BYTE,   NULL},    // Terminal Type
	{TAG_TERMINAL_CAPABILITIES,              C_TAG_TYPE_BUF_BYTE,    NULL},    // Terminal Capabilities
	{TAG_TERMINAL_COUNTRY_CODE,              C_TAG_TYPE_BUF_BYTE,    NULL},    // Terminal country code
	{TAG_TRANSACTION_CURRENCY_CODE,          C_TAG_TYPE_BUF_BYTE,    NULL},    // Terminal currency
	{TAG_ADD_TERMINAL_CAPABILITIES,          C_TAG_TYPE_BUF_BYTE,    NULL},    // Additionnal Terminal Capabilities

	// Other
	{TAG_USE_PSE,                             C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_CARDHOLDER_CONFIRMATION,     C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_PREFERRED_DISPLAY_ORDER,     C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_MULTILANG_SUPPORTED,         C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_REVOK_SUPPORTED,             C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_PIN_BYPASS,                  C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_GETDATA_PIN_TRY_COUNTER,     C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_TRANSACTION_LOG,             C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_EXCEPTION_FILE,              C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_FORCED_ONLINE,               C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_FORCED_ACCEPTANCE,           C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_ONLINE_ADVICE,               C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_ISSUER_REFERRAL,             C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_CARD_REFERRAL,               C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_BATCH_CAPTURE,               C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_ONLINE_CAPTURE,              C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_POS_ENTRY_MODE,              C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_POS_ENTRY_MODE_CODE,                 C_TAG_TYPE_HEXA_BYTE,  NULL},
	{TAG_CUST_IS_PINPAD,                      C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_AMOUNT_PIN_SAME,             C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_DEFAULT_DDOL,                C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_FLOOR_LIMIT_CHECKING,        C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_RANDOM_TRANSACTION_SELECT,   C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_VELOCITY_CHECKING,           C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_DEFAULT_TDOL,                C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_TRM_OVERPASS_AIP,               C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_SKIP_DEFAULT,                C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_SKIP_ONLINE,                 C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_ODA_PROCESSING,              C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_ACCOUNT_TYPE,                C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_PSE_ALGO,                            C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_DETECT_FAIL_PRIOR_TAA,          C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_CDA_NEVER_REQUEST_ARQC_1GENAC,  C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_CDA_NEVER_REQUEST_ONLINE_2GENAC,C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_SUBSEQUENT_BYPASS_PIN,          C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_AMOUNT_UNKNOWN_BEFORE_CVM,   C_TAG_TYPE_BOOLEAN,    NULL}
};
const Struct_TagList FirstTagListEntireFull[] = {
	{TAG_TERMINAL_TYPE,                      C_TAG_TYPE_HEXA_BYTE,   NULL},    // Terminal Type
	{TAG_TERMINAL_CAPABILITIES,              C_TAG_TYPE_BUF_BYTE,    NULL},    // Terminal Capabilities
	{TAG_TERMINAL_COUNTRY_CODE,              C_TAG_TYPE_BUF_BYTE,    NULL},    // Terminal country code
	{TAG_TRANSACTION_CURRENCY_CODE,          C_TAG_TYPE_BUF_BYTE,    NULL},    // Terminal currency
	{TAG_ADD_TERMINAL_CAPABILITIES,          C_TAG_TYPE_BUF_BYTE,    NULL},    // Additionnal Terminal Capabilities

	// Other
	{TAG_USE_PSE,                             C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_CARDHOLDER_CONFIRMATION,     C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_PREFERRED_DISPLAY_ORDER,     C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_MULTILANG_SUPPORTED,         C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_REVOK_SUPPORTED,             C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_PIN_BYPASS,                  C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_GETDATA_PIN_TRY_COUNTER,     C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_TRANSACTION_LOG,             C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_EXCEPTION_FILE,              C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_FORCED_ONLINE,               C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_FORCED_ACCEPTANCE,           C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_ONLINE_ADVICE,               C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_ISSUER_REFERRAL,             C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_CARD_REFERRAL,               C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_BATCH_CAPTURE,               C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_ONLINE_CAPTURE,              C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_POS_ENTRY_MODE,              C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_POS_ENTRY_MODE_CODE,                 C_TAG_TYPE_HEXA_BYTE,  NULL},
	{TAG_CUST_IS_PINPAD,                      C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_AMOUNT_PIN_SAME,             C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_DEFAULT_DDOL,                C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_FLOOR_LIMIT_CHECKING,        C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_RANDOM_TRANSACTION_SELECT,   C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_VELOCITY_CHECKING,           C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_DEFAULT_TDOL,                C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_TRM_OVERPASS_AIP,               C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_SKIP_DEFAULT,                C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_SKIP_ONLINE,                 C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_ODA_PROCESSING,              C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_ACCOUNT_TYPE,                C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_PSE_ALGO,                            C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_DETECT_FAIL_PRIOR_TAA,          C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_CDA_NEVER_REQUEST_ARQC_1GENAC,  C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_CDA_NEVER_REQUEST_ONLINE_2GENAC,C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_SUBSEQUENT_BYPASS_PIN,          C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_AMOUNT_UNKNOWN_BEFORE_CVM,   C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_SELECTABLE_KERNEL,			  C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_DELETABLE_TAC,				  C_TAG_TYPE_BOOLEAN,    NULL}
};


//const struct Struct_TagList MajorTagList[] =
const Struct_TagList MajorTagListEntire[] =
{
	{TAG_TERMINAL_TYPE,                       C_TAG_TYPE_HEXA_BYTE,  NULL},                        // Terminal Type
	{TAG_TERMINAL_CAPABILITIES,               C_TAG_TYPE_BUF_BYTE,   (unsigned char*)ay_Mask_Tc},  // Terminal Capabilities
	{TAG_ADD_TERMINAL_CAPABILITIES,           C_TAG_TYPE_BUF_BYTE,   (unsigned char*)ay_Mask_Atc}, // Additionnal Terminal Capabilities
	// Other
	{TAG_USE_PSE,                             C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_CARDHOLDER_CONFIRMATION,     C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_PREFERRED_DISPLAY_ORDER,     C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_REVOK_SUPPORTED,             C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_PIN_BYPASS,                  C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_GETDATA_PIN_TRY_COUNTER,     C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_TRANSACTION_LOG,             C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_EXCEPTION_FILE,              C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_FORCED_ONLINE,               C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_FORCED_ACCEPTANCE,           C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_ONLINE_ADVICE,               C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_ISSUER_REFERRAL,             C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_CARD_REFERRAL,               C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_BATCH_CAPTURE,               C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_ONLINE_CAPTURE,              C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_PINPAD,                      C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_AMOUNT_PIN_SAME,             C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_DEFAULT_DDOL,                C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_FLOOR_LIMIT_CHECKING,        C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_RANDOM_TRANSACTION_SELECT,   C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_VELOCITY_CHECKING,           C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_DEFAULT_TDOL,                C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_AMOUNT_UNKNOWN_BEFORE_CVM,   C_TAG_TYPE_BOOLEAN,    NULL}

};
//const struct Struct_TagList MajorTagList[] =
const Struct_TagList MajorTagListEntireFull[] =
{
	{TAG_TERMINAL_TYPE,                       C_TAG_TYPE_HEXA_BYTE,  NULL},                        // Terminal Type
	{TAG_TERMINAL_CAPABILITIES,               C_TAG_TYPE_BUF_BYTE,   (unsigned char*)ay_Mask_Tc},  // Terminal Capabilities
	{TAG_ADD_TERMINAL_CAPABILITIES,           C_TAG_TYPE_BUF_BYTE,   (unsigned char*)ay_Mask_Atc}, // Additionnal Terminal Capabilities
	// Other
	{TAG_USE_PSE,                             C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_CARDHOLDER_CONFIRMATION,     C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_PREFERRED_DISPLAY_ORDER,     C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_REVOK_SUPPORTED,             C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_PIN_BYPASS,                  C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_GETDATA_PIN_TRY_COUNTER,     C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_TRANSACTION_LOG,             C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_EXCEPTION_FILE,              C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_FORCED_ONLINE,               C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_FORCED_ACCEPTANCE,           C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_ONLINE_ADVICE,               C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_ISSUER_REFERRAL,             C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_CARD_REFERRAL,               C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_BATCH_CAPTURE,               C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_ONLINE_CAPTURE,              C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_PINPAD,                      C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_AMOUNT_PIN_SAME,             C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_DEFAULT_DDOL,                C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_FLOOR_LIMIT_CHECKING,        C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_RANDOM_TRANSACTION_SELECT,   C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_VELOCITY_CHECKING,           C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_DEFAULT_TDOL,                C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_AMOUNT_UNKNOWN_BEFORE_CVM,   C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_SELECTABLE_KERNEL,			  C_TAG_TYPE_BOOLEAN,    NULL},
	{TAG_CUST_IS_DELETABLE_TAC,				  C_TAG_TYPE_BOOLEAN,    NULL}

};
/* Default parameters */
const char DefaultParam[] =
{
	"	<?xml version=\"1.0\" standalone=\"yes\" ?>"
	"	<!-- Fichier de parametres base -->"
	""
	"	<tlvtree ver=\"1.0\">"
	""
	"	<node tag=\"0x0\">"
	"		<node tag=\"0x1000\">                                <!-- C_TAG_AID -->"
	""
	"			<node tag=\"0x1001\">"
	"				<!-- AID1.0 file content -->"
	"				<node tag=\"0x9FFF00\"> 41 49 44 31 </node>            <!-- AID1 Parameters -->"
	"				<node tag=\"0x9F06\"> 07 B0 12 34 56 78 12 34 </node>"
	"				<node tag=\"0x9F1A\"> 05 04 </node>                    <!-- Country code -->"
	"				<node tag=\"0x5F2A\"> 05 04 </node>                    <!-- Currency Codef -->"
	"				<node tag=\"0x5F36\"> 02 </node>                       <!-- Currency Exponent -->"
	"				<node tag=\"0x9F812B\"> 00 00 00 05 </node>            <!-- Threshold Value -->"
	"				<node tag=\"0x9F8124\"> 9F 37 04 9F 47 01 8F 01 9F 32 01 </node>               <!-- Default DDOL -->"
	"				<node tag=\"0x9F8125\"> 9F 02 06 5F 2A 02 9A 03 9C 01 95 05 9F 37 04 </node>   <!-- Default TDOL -->"
	"				<node tag=\"0x9F8126\"> 00 </node>                     <!-- Max percentage for biased random selection -->"
	"				<node tag=\"0x9F8127\"> 00 </node>                     <!-- Target percentage for biased random selection -->"
	"				<node tag=\"0x9F8128\"> 00 00 00 00 00 </node>         <!-- TAC Default -->"
	"				<node tag=\"0x9F8129\"> 00 00 00 00 00 </node>         <!-- TAC Denial -->"
	"				<node tag=\"0x9F812A\"> 00 C0 00 00 00 </node>         <!-- TAC Online -->"
	"				<node tag=\"0x9F09\"> 00 02 </node>                    <!-- Version Number -->"
	"				<node tag=\"0x9F1B\"> 00 00 27 10 </node>              <!-- Floor Limit -->"
	"				<node tag=\"0x9F841D\"> 00 </node>                     <!-- ASI -->"
	"			</node>"
	""
	"			<node tag=\"0x1002\">"
	"				<!-- AID2.0 file content -->"
	"				<node tag=\"0x9FFF00\"> 4D 61 73 74 65 72 43 61 72 64 </node> <!-- Master Card -->"
	"				<node tag=\"0x9F06\"> 07 A0 00 00 00 04 10 10 </node>"
	"				<node tag=\"0x9F1A\"> 05 04 </node>"
	"				<node tag=\"0x5F2A\"> 05 04 </node>"
	"				<node tag=\"0x5F36\"> 02 </node>"
	"				<node tag=\"0x9F812B\"> 00 00 00 05 </node>"
	"				<node tag=\"0x9F8124\"> 9F 37 04 9F 47 01 8F 01 9F 32 01 </node>"
	"				<node tag=\"0x9F8125\"> 9F 08 02 </node>"
	"				<node tag=\"0x9F8126\"> 50 </node>"
	"				<node tag=\"0x9F8127\"> 20 </node>"
	"				<node tag=\"0x9F8128\"> 58 40 00 A8 00</node>"
	"				<node tag=\"0x9F8129\"> 00 00 00 00 00 </node>"
	"				<node tag=\"0x9F812A\"> 58 C0 04 F8 00</node>"
	"				<node tag=\"0x9F09\"> 00 02 </node>"
	"				<node tag=\"0x9F1B\"> 00 00 00 00 </node>"
	"				<node tag=\"0x9F841D\"> 00 </node>"
	"			</node>"
	""
	"			<node tag=\"0x1003\">"
	"				<!-- AID3.0 VISA file content -->"
	"				<node tag=\"0x9FFF00\"> 56 53 44 43 </node>               <!-- VSDC -->"
	"				<node tag=\"0x9F06\"> 07 A0 00 00 00 03 10 10 </node>"
	"				<node tag=\"0x9F1A\"> 05 04 </node>"
	"				<node tag=\"0x5F2A\"> 05 04 </node>"
	"				<node tag=\"0x5F36\"> 02 </node>"
	"				<node tag=\"0x9F812B\"> 00 00 00 40 </node>"
	"				<node tag=\"0x9F8125\"> 9F 02 06 5F 2A 02 9A 03 9C 01 95 05 9F 37 04 </node>"
	"				<node tag=\"0x9F8126\"> 50 </node>"
	"				<node tag=\"0x9F8127\"> 20 </node>"
	"				<node tag=\"0x9F8128\"> 58 40 00 A8 00</node>"
	"				<node tag=\"0x9F8129\"> 00 00 00 00 00 </node>"
	"				<node tag=\"0x9F812A\"> 58 C0 04 F8 00</node>"
	"				<node tag=\"0x9F01\"> 45 00 00 00 00 00 </node>"
	"				<node tag=\"0x9F09\"> 00 96 </node>"
	"				<node tag=\"0x9F1B\"> 00 00 00 00 </node>"
	"				<node tag=\"0x9F16\"> 01 20 00 00 00 00 00 00 </node>"
	"				<node tag=\"0x9F15\"> 61 11 </node>"
	"				<node tag=\"0x9F7A\"> 01 </node>"
	"				<node tag=\"0x9F841D\"> 01 </node>"
	"			</node>"
	""
	"			<node tag=\"0x1004\">"
	"				<!-- AID4.0 file content -->"
	"				<node tag=\"0x9FFF00\"> 4D 61 65 73 74 72 6F </node>    <!-- MaestroParameters -->"
	"				<node tag=\"0x9F06\"> 07 A0 00 00 00 99 90 90 </node>"
	"				<node tag=\"0x9F1A\"> 05 04 </node>"
	"				<node tag=\"0x5F2A\"> 05 04 </node>"
	"				<node tag=\"0x5F36\"> 02 </node>"
	"				<node tag=\"0x9F812B\"> 00 00 00 05 </node>"
	"				<node tag=\"0x9F8124\"> 9F 37 04 9F 47 01 8F 01 9F 32 01 </node>"
	"				<node tag=\"0x9F8125\"> 9F 08 02 </node>"
	"				<node tag=\"0x9F8126\"> 00 </node>"
	"				<node tag=\"0x9F8127\"> 00 </node>"
	"				<node tag=\"0x9F8128\"> 00 00 00 00 00 </node>"
	"				<node tag=\"0x9F8129\"> 00 00 00 00 00 </node>"
	"				<node tag=\"0x9F812A\"> 00 00 00 00 00 </node>"
	"				<node tag=\"0x9F09\"> 00 02 </node>"
	"				<node tag=\"0x9F1B\"> 00 00 27 10 </node>"
	"				<node tag=\"0x9F841D\"> 01 </node>"
	"			</node>"
	""
	"			<node tag=\"0x1005\">"
	"				<!-- AID5.0 file content -->"
	"				<node tag=\"0x9FFF00\"> 41 49 44 33 </node>    <!-- VisaElectronParameters -->"
	"				<node tag=\"0x9F06\"> 07 A0 00 00 00 03 20 10 </node>"
	"				<node tag=\"0x9F1A\"> 05 04 </node>"
	"				<node tag=\"0x5F2A\"> 05 04 </node>"
	"				<node tag=\"0x5F36\"> 02 </node>"
	"				<node tag=\"0x9F812B\"> 00 00 00 05 </node>"
	"				<node tag=\"0x9F8124\"> 9F 37 04 9F 47 01 8F 01 9F 32 01 </node>"
	"				<node tag=\"0x9F8125\"> 9F 02 06 5F 2A 02 9A 03 9C 01 95 05 9F 37 04 </node>"
	"				<node tag=\"0x9F8126\"> 00 </node>"
	"				<node tag=\"0x9F8127\"> 00 </node>"
	"				<node tag=\"0x9F8128\"> 00 00 00 00 00 </node>"
	"				<node tag=\"0x9F8129\"> 00 00 00 00 00 </node>"
	"				<node tag=\"0x9F812A\"> 58 C0 04 F8 00 </node>"
	"				<node tag=\"0x9F09\"> 01 40 </node>"
	"				<node tag=\"0x9F1B\"> 00 00 00 00 </node>"
	"				<node tag=\"0x9F841D\"> 01 </node>"
	"			</node> "
	""
	"			<node tag=\"0x1006\">"
	"				<!-- AID6.0 file content -->"
	"				<node tag=\"0x9FFF00\"> 41 49 44 36 </node>    <!-- Discover -->"
	"				<node tag=\"0x9F06\"> 09 A0 00 00 00 04 99 99 D5 04 </node>   <!-- Discover remplace par AID Carte NAPS -->"
	"				<node tag=\"0x9F1A\"> 05 04 </node>"
	"				<node tag=\"0x5F2A\"> 05 04 </node>"
	"				<node tag=\"0x5F36\"> 02 </node>"
	"				<node tag=\"0x9F812B\"> 00 00 00 05 </node>"
	"				<node tag=\"0x9F8124\"> 9F 37 04 9F 47 01 8F 01 9F 32 01 </node>"
	"				<node tag=\"0x9F8125\"> 9F 02 06 5F 2A 02 9A 03 9C 01 95 05 9F 37 04 </node>"
	"				<node tag=\"0x9F8126\"> 00 </node>"
	"				<node tag=\"0x9F8127\"> 00 </node>"
	"				<node tag=\"0x9F8128\"> 00 00 00 00 00 </node>"
	"				<node tag=\"0x9F8129\"> 00 00 00 00 00 </node>"
	"				<node tag=\"0x9F812A\"> 00 00 00 00 00 </node>"
	"				<node tag=\"0x9F09\"> 01 40 </node>"
	"				<node tag=\"0x9F1B\"> 00 00 27 10 </node>"
	"				<node tag=\"0x9F841D\"> 01 </node>"
	"			</node> <!-- End node 1006 -->"
	""
	"			<node tag=\"0x1007\">"
	"				<!-- AID7.0 file content -->"
	"				<node tag=\"0x9FFF00\"> 4D 41 45 53 54 52 4F </node> <!-- Maestro -->"
	"				<node tag=\"0x9F06\"> 07 A0 00 00 00 04 30 60 </node>"
	"				<node tag=\"0x9F1A\"> 05 04 </node>"
	"				<node tag=\"0x5F2A\"> 05 04 </node>"
	"				<node tag=\"0x5F36\"> 02 </node>"
	"				<node tag=\"0x9F812B\"> 00 00 00 05 </node>"
	"				<node tag=\"0x9F8124\"> 9F 37 04 9F 47 01 8F 01 9F 32 01 </node>"
	"				<node tag=\"0x9F8125\"> 9F 08 02 </node>"
	"				<node tag=\"0x9F8126\"> 00 </node>"
	"				<node tag=\"0x9F8127\"> 00 </node>"
	"				<node tag=\"0x9F8128\"> 58 40 00 A8 00 </node>"
	"				<node tag=\"0x9F8129\"> 00 10 00 00 00 </node>"
	"				<node tag=\"0x9F812A\"> 00 10 00 00 00 </node>"
	"				<node tag=\"0x9F09\"> 00 02 </node>"
	"				<node tag=\"0x9F1B\"> 00 00 00 00 </node>"
	"				<node tag=\"0x9F841D\"> 00 </node>"
	"			</node> <!-- End node 1007 -->"
	""
	"			<node tag=\"0x1008\">"
	"				<!-- AID8.0 file content -->"
	"				<node tag=\"0x9FFF00\"> 4D 61 73 74 65 72 43 61 72 64 </node> <!-- Master Card -->"
	"				<node tag=\"0x9F06\"> 08 A0 00 00 00 04 10 10 02 </node>"
	"				<node tag=\"0x9F1A\"> 05 04 </node>"
	"				<node tag=\"0x5F2A\"> 05 04 </node>"
	"				<node tag=\"0x5F36\"> 02 </node>"
	"				<node tag=\"0x9F812B\"> 00 00 00 05 </node>"
	"				<node tag=\"0x9F8124\"> 9F 37 04 9F 47 01 8F 01 9F 32 01 </node>"
	"				<node tag=\"0x9F8125\"> 9F 08 02 </node>"
	"				<node tag=\"0x9F8126\"> 50 </node>"
	"				<node tag=\"0x9F8127\"> 20 </node>"
	"				<node tag=\"0x9F8128\"> 58 40 00 A8 00</node>"
	"				<node tag=\"0x9F8129\"> 00 10 00 00 00 </node>"
	"				<node tag=\"0x9F812A\"> 58 C0 04 F8 00</node>"
	"				<node tag=\"0x9F09\"> 00 02 </node>"
	"				<node tag=\"0x9F1B\"> 00 00 00 00 </node>"
	"				<node tag=\"0x9F841D\"> 00 </node>"
	"			</node>"
	""
	"			<node tag=\"0x1009\">"
	"				<!-- AID9.0 file content -->"
	"				<node tag=\"0x9FFF00\"> 4D 61 73 74 65 72 43 61 72 64 </node> <!-- Master Card -->"
	"				<node tag=\"0x9F06\"> 0B A0 00 00 00 04 10 10 D0 56 22 22 </node>"
	"				<node tag=\"0x9F1A\"> 05 04 </node>"
	"				<node tag=\"0x5F2A\"> 05 04 </node>"
	"				<node tag=\"0x5F36\"> 02 </node>"
	"				<node tag=\"0x9F812B\"> 00 00 00 05 </node>"
	"				<node tag=\"0x9F8124\"> 9F 37 04 9F 47 01 8F 01 9F 32 01 </node>"
	"				<node tag=\"0x9F8125\"> 9F 08 02 </node>"
	"				<node tag=\"0x9F8126\"> 50 </node>"
	"				<node tag=\"0x9F8127\"> 20 </node>"
	"				<node tag=\"0x9F8128\"> 58 40 00 A8 00</node>"
	"				<node tag=\"0x9F8129\"> 00 10 00 00 00 </node>"
	"				<node tag=\"0x9F812A\"> 58 C0 04 F8 00</node>"
	"				<node tag=\"0x9F09\"> 00 02 </node>"
	"				<node tag=\"0x9F1B\"> 00 00 00 00 </node>"
	"				<node tag=\"0x9F841D\"> 00 </node>"
	"			</node>"
	""
		"			<node tag=\"0x100A\">"
		"				<!-- AID10.0 file content -->"
		"				<node tag=\"0x9FFF00\"> 4D 61 73 74 65 72 43 61 72 64 </node> <!-- Master Card -->"
		"				<node tag=\"0x9F06\"> 08 A0 00 00 00 04 10 10 01 </node>"
		"				<node tag=\"0x9F1A\"> 05 04 </node>"
		"				<node tag=\"0x5F2A\"> 05 04 </node>"
		"				<node tag=\"0x5F36\"> 02 </node>"
		"				<node tag=\"0x9F812B\"> 00 00 00 05 </node>"
		"				<node tag=\"0x9F8124\"> 9F 37 04 9F 47 01 8F 01 9F 32 01 </node>"
		"				<node tag=\"0x9F8125\"> 9F 08 02 </node>"
		"				<node tag=\"0x9F8126\"> 50 </node>"
		"				<node tag=\"0x9F8127\"> 20 </node>"
		"				<node tag=\"0x9F8128\"> 58 40 00 A8 00</node>"
		"				<node tag=\"0x9F8129\"> 00 10 00 00 00 </node>"
		"				<node tag=\"0x9F812A\"> 58 C0 04 F8 00</node>"
		"				<node tag=\"0x9F09\"> 00 02 </node>"
		"				<node tag=\"0x9F1B\"> 00 00 00 00 </node>"
		"				<node tag=\"0x9F841D\"> 00 </node>"
		"			</node>"
		""
		"			<node tag=\"0x100B\">"
		"				<!-- AID11.0 file content -->"
		"				<node tag=\"0x9FFF00\"> 4D 61 73 74 65 72 43 61 72 64 </node> <!-- Master Card -->"
		"				<node tag=\"0x9F06\"> 0B A0 00 00 00 04 10 10 D0 56 11 11 </node>"
		"				<node tag=\"0x9F1A\"> 05 04 </node>"
		"				<node tag=\"0x5F2A\"> 05 04 </node>"
		"				<node tag=\"0x5F36\"> 02 </node>"
		"				<node tag=\"0x9F812B\"> 00 00 00 05 </node>"
		"				<node tag=\"0x9F8124\"> 9F 37 04 9F 47 01 8F 01 9F 32 01 </node>"
		"				<node tag=\"0x9F8125\"> 9F 08 02 </node>"
		"				<node tag=\"0x9F8126\"> 50 </node>"
		"				<node tag=\"0x9F8127\"> 20 </node>"
		"				<node tag=\"0x9F8128\"> 58 40 00 A8 00</node>"
		"				<node tag=\"0x9F8129\"> 00 10 00 00 00 </node>"
		"				<node tag=\"0x9F812A\"> 58 C0 04 F8 00</node>"
		"				<node tag=\"0x9F09\"> 00 02 </node>"
		"				<node tag=\"0x9F1B\"> 00 00 00 00 </node>"
		"				<node tag=\"0x9F841D\"> 00 </node>"
		"			</node>"
		""
		"			<node tag=\"0x100C\">"
		"				<!-- AID11.0 file content -->"
		"				<node tag=\"0x9FFF00\"> 4D 61 73 74 65 72 43 61 72 64 </node> <!-- Master Card -->"
		"				<node tag=\"0x9F06\"> 08 A0 00 00 05 02 10 10 01 </node>"
		"				<node tag=\"0x9F1A\"> 05 04 </node>"
		"				<node tag=\"0x5F2A\"> 05 04 </node>"
		"				<node tag=\"0x5F36\"> 02 </node>"
		"				<node tag=\"0x9F812B\"> 00 00 00 05 </node>"
		"				<node tag=\"0x9F8124\"> 9F 37 04 9F 47 01 8F 01 9F 32 01 </node>"
		"				<node tag=\"0x9F8125\"> 9F 08 02 </node>"
		"				<node tag=\"0x9F8126\"> 50 </node>"
		"				<node tag=\"0x9F8127\"> 20 </node>"
		"				<node tag=\"0x9F8128\"> 58 40 00 A8 00</node>"
		"				<node tag=\"0x9F8129\"> 00 10 00 00 00 </node>"
		"				<node tag=\"0x9F812A\"> 58 C0 04 F8 00</node>"
		"				<node tag=\"0x9F09\"> 00 02 </node>"
		"				<node tag=\"0x9F1B\"> 00 00 00 00 </node>"
		"				<node tag=\"0x9F841D\"> 00 </node>"
		"			</node>"
		""
		"			<node tag=\"0x100D\">"
		"				<!-- AID11.0 file content -->"
		"				<node tag=\"0x9FFF00\"> 4D 61 73 74 65 72 43 61 72 64 </node> <!-- Master Card -->"
		"				<node tag=\"0x9F06\"> 08 A0 00 00 05 02 10 10 02 </node>"
		"				<node tag=\"0x9F1A\"> 05 04 </node>"
		"				<node tag=\"0x5F2A\"> 05 04 </node>"
		"				<node tag=\"0x5F36\"> 02 </node>"
		"				<node tag=\"0x9F812B\"> 00 00 00 05 </node>"
		"				<node tag=\"0x9F8124\"> 9F 37 04 9F 47 01 8F 01 9F 32 01 </node>"
		"				<node tag=\"0x9F8125\"> 9F 08 02 </node>"
		"				<node tag=\"0x9F8126\"> 50 </node>"
		"				<node tag=\"0x9F8127\"> 20 </node>"
		"				<node tag=\"0x9F8128\"> 58 40 00 A8 00</node>"
		"				<node tag=\"0x9F8129\"> 00 10 00 00 00 </node>"
		"				<node tag=\"0x9F812A\"> 58 C0 04 F8 00</node>"
		"				<node tag=\"0x9F09\"> 00 02 </node>"
		"				<node tag=\"0x9F1B\"> 00 00 00 00 </node>"
		"				<node tag=\"0x9F841D\"> 00 </node>"
		"			</node>"
		""
		"			<node tag=\"0x100E\">"
		"				<!-- AID11.0 file content -->"
		"				<node tag=\"0x9FFF00\"> 4D 61 73 74 65 72 43 61 72 64 </node> <!-- Master Card -->"
		"				<node tag=\"0x9F06\"> 08 A0 00 00 05 02 10 10 03 </node>"
		"				<node tag=\"0x9F1A\"> 05 04 </node>"
		"				<node tag=\"0x5F2A\"> 05 04 </node>"
		"				<node tag=\"0x5F36\"> 02 </node>"
		"				<node tag=\"0x9F812B\"> 00 00 00 05 </node>"
		"				<node tag=\"0x9F8124\"> 9F 37 04 9F 47 01 8F 01 9F 32 01 </node>"
		"				<node tag=\"0x9F8125\"> 9F 08 02 </node>"
		"				<node tag=\"0x9F8126\"> 50 </node>"
		"				<node tag=\"0x9F8127\"> 20 </node>"
		"				<node tag=\"0x9F8128\"> 58 40 00 A8 00</node>"
		"				<node tag=\"0x9F8129\"> 00 10 00 00 00 </node>"
		"				<node tag=\"0x9F812A\"> 58 C0 04 F8 00</node>"
		"				<node tag=\"0x9F09\"> 00 02 </node>"
		"				<node tag=\"0x9F1B\"> 00 00 00 00 </node>"
		"				<node tag=\"0x9F841D\"> 00 </node>"
		"			</node>"
		""
	"		</node> <!-- End node 1000 -->"
	""
	"		<node tag=\"0x1100\">                  <!-- C_TAG_CAKEYS -->"
	"			<node tag=\"0x1101\">"
	"				<node tag=\"0x9F06\"> A000000003 </node>"
	"				<node tag=\"0x9F22\"> 01 </node>"
	"				<node tag=\"0x9F8123\"> C696034213D7D8546984579D1D0F0EA519CFF8DEFFC429354CF3A871A6F7183F1228DA5C7470C055387100CB935A712C4E2864DF5D64BA93FE7E63E71F25B1E5F5298575EBE1C63AA617706917911DC2A75AC28B251C7EF40F2365912490B939BCA2124A30A28F54402C34AECA331AB67E1E79B285DD5771B5D9FF79EA630B75 </node>"
	"				<node tag=\"0x9F8122\"> 03 </node>"
	"				<node tag=\"0x9F8121\"> D34A6A776011C7E7CE3AEC5F03AD2F8CFC5503CC </node>"
	"			</node>"

	"			<node tag=\"0x1102\">"
	"				<node tag=\"0x9F06\"> A000000003 </node>"
	"				<node tag=\"0x9F22\"> 03 </node>"
	"				<node tag=\"0x9F8123\"> B3E5E667506C47CAAFB12A2633819350846697DD65A796E5CE77C57C626A66F70BB630911612AD2832909B8062291BECA46CD33B66A6F9C9D48CED8B4FC8561C8A1D8FB15862C9EB60178DEA2BE1F82236FFCFF4F3843C272179DCDD384D541053DA6A6A0D3CE48FDC2DC4E3E0EEE15F </node>"
	"				<node tag=\"0x9F8122\"> 03 </node>"
	"				<node tag=\"0x9F8121\"> FE70AB3B4D5A1B9924228ADF8027C758483A8B7E </node>"
	"			</node>"

	"			<node tag=\"0x1103\">"
	"				<node tag=\"0x9F06\"> A000000003 </node>"
	"				<node tag=\"0x9F22\"> 05 </node>"
	"				<node tag=\"0x9F8123\"> D0135CE8A4436C7F9D5CC66547E30EA402F98105B71722E24BC08DCC80AB7E71EC23B8CE6A1DC6AC2A8CF55543D74A8AE7B388F9B174B7F0D756C22CBB5974F9016A56B601CCA64C71F04B78E86C501B193A5556D5389ECE4DEA258AB97F52A3 </node>"
	"				<node tag=\"0x9F8122\"> 03 </node>"
	"				<node tag=\"0x9F8121\"> 86DF041E7995023552A79E2623E49180C0CD957A </node>"
	"			</node>"

	"			<node tag=\"0x1104\">"
	"				<node tag=\"0x9F06\"> A000000003 </node>"
	"				<node tag=\"0x9F22\"> 06 </node>"
	"				<node tag=\"0x9F8123\"> F934FC032BE59B609A9A649E04446F1B365D1D23A1E6574E490170527EDF32F398326159B39B63D07E95E6276D7FCBB786925182BC0667FBD8F6566B361CA41A38DDF227091B87FA4F47BAC780AC47E15A6A0FB65393EB3473E8D193A07EB579 </node>"
	"				<node tag=\"0x9F8122\"> 03 </node>"
	"				<node tag=\"0x9F8121\"> A0DF5DAA385AE3E0E21BFD34D9D8A30506B19B12 </node>"
	"			</node>"

	"			<node tag=\"0x1105\">"
	"				<node tag=\"0x9F06\"> A000000003 </node>"
	"				<node tag=\"0x9F22\"> 07 </node>"
	"				<node tag=\"0x9F8123\"> A89F25A56FA6DA258C8CA8B40427D927B4A1EB4D7EA326BBB12F97DED70AE5E4480FC9C5E8A972177110A1CC318D06D2F8F5C4844AC5FA79A4DC470BB11ED635699C17081B90F1B984F12E92C1C529276D8AF8EC7F28492097D8CD5BECEA16FE4088F6CFAB4A1B42328A1B996F9278B0B7E3311CA5EF856C2F888474B83612A82E4E00D0CD4069A6783140433D50725F </node>"
	"				<node tag=\"0x9F8122\"> 03 </node>"
	"				<node tag=\"0x9F8121\"> B4BC56CC4E88324932CBC643D6898F6FE593B172 </node>"
	"			</node>"

	"			<node tag=\"0x1106\">"
	"				<node tag=\"0x9F06\"> A000000003 </node>"
	"				<node tag=\"0x9F22\"> 08 </node>"
	"				<node tag=\"0x9F8123\"> D9FD6ED75D51D0E30664BD157023EAA1FFA871E4DA65672B863D255E81E137A51DE4F72BCC9E44ACE12127F87E263D3AF9DD9CF35CA4A7B01E907000BA85D24954C2FCA3074825DDD4C0C8F186CB020F683E02F2DEAD3969133F06F7845166ACEB57CA0FC2603445469811D293BFEFBAFAB57631B3DD91E796BF850A25012F1AE38F05AA5C4D6D03B1DC2E568612785938BBC9B3CD3A910C1DA55A5A9218ACE0F7A21287752682F15832A678D6E1ED0B </node>"
	"				<node tag=\"0x9F8122\"> 03 </node>"
	"				<node tag=\"0x9F8121\"> 20D213126955DE205ADC2FD2822BD22DE21CF9A8 </node>"
	"			</node>"

	"			<node tag=\"0x1107\">"
	"				<node tag=\"0x9F06\"> A000000003 </node>"
	"				<node tag=\"0x9F22\"> 09 </node>"
	"				<node tag=\"0x9F8123\"> 9D912248DE0A4E39C1A7DDE3F6D2588992C1A4095AFBD1824D1BA74847F2BC4926D2EFD904B4B54954CD189A54C5D1179654F8F9B0D2AB5F0357EB642FEDA95D3912C6576945FAB897E7062CAA44A4AA06B8FE6E3DBA18AF6AE3738E30429EE9BE03427C9D64F695FA8CAB4BFE376853EA34AD1D76BFCAD15908C077FFE6DC5521ECEF5D278A96E26F57359FFAEDA19434B937F1AD999DC5C41EB11935B44C18100E857F431A4A5A6BB65114F174C2D7B59FDF237D6BB1DD0916E644D709DED56481477C75D95CDD68254615F7740EC07F330AC5D67BCD75BF23D28A140826C026DBDE971A37CD3EF9B8DF644AC385010501EFC6509D7A41 </node>"
	"				<node tag=\"0x9F8122\"> 03 </node>"
	"				<node tag=\"0x9F8121\"> 1FF80A40173F52D7D27E0F26A146A1C8CCB29046 </node>"
	"			</node>"

	"			<node tag=\"0x1108\">"
	"				<node tag=\"0x9F06\"> A000000003 </node>"
	"				<node tag=\"0x9F22\"> 10 </node>"
	"				<node tag=\"0x9F8123\"> 9F2701C0909CCBD8C3ED3E071C69F776160022FF3299807ED7A035ED5752770E232D56CC3BE159BD8F0CA8B59435688922F406F55C75639457BBABEFE9A86B2269EF223E34B91AA6DF2CCAD03B4AD4B443D61575CA960845E6C69040101E231D9EF811AD99B0715065A0E661449C41B4B023B7716D1E4AFF1C90704E55AE1225 </node>"
	"				<node tag=\"0x9F8122\"> 03 </node>"
	"				<node tag=\"0x9F8121\"> 833B1947778036B6D759FCE3F618DDEB2749372C </node>"
	"			</node>"

	"			<node tag=\"0x1109\">"
	"				<node tag=\"0x9F06\"> A000000003 </node>"
	"				<node tag=\"0x9F22\"> 20 </node>"
	"				<node tag=\"0x9F8123\"> 998D2AD946A60FC597D93807DB54B2B0A550871E43F1779F073AF08D9B04ABD17C8A7DAA3E66EE443F30F92648FC53DA57A78364B062FEDB50F7235B937E16E5F6D9E6BA8F106FB325ECA25125111CE04B43098CDEA8A41426FC6D94F8A47619EDB12789581808692CFBA1F38E8008CC5E02066A1889D52F77B9A121E6597F39 </node>"
	"				<node tag=\"0x9F8122\"> 03 </node>"
	"				<node tag=\"0x9F8121\"> 7AC3D80EF01E9A998F0A77181E64B36747DC51EB </node>"
	"			</node>"

	"			<node tag=\"0x110A\">"
	"				<node tag=\"0x9F06\"> A000000003 </node>"
	"				<node tag=\"0x9F22\"> 50 </node>"
	"				<node tag=\"0x9F8123\"> D11197590057B84196C2F4D11A8F3C05408F422A35D702F90106EA5B019BB28AE607AA9CDEBCD0D81A38D48C7EBB0062D287369EC0C42124246AC30D80CD602AB7238D51084DED4698162C59D25EAC1E66255B4DB2352526EF0982C3B8AD3D1CCE85B01DB5788E75E09F44BE7361366DEF9D1E1317B05E5D0FF5290F88A0DB47 </node>"
	"				<node tag=\"0x9F8122\"> 010001 </node>"
	"				<node tag=\"0x9F8121\"> B769775668CACB5D22A647D1D993141EDAB7237B </node>"
	"			</node>"

#if 0
		//RID A000000003 I 51, SHA:0748D2B9 B5233F5A 5745FE22 4D37043E 715D99C4
	"			<node tag=\"0x110B\">"
	"				<node tag=\"0x9F06\"> A000000003 </node>"
	"				<node tag=\"0x9F22\"> 51 </node>"
	"				<node tag=\"0x9F8123\"> DB5FA29D1FDA8C1634B04DCCFF148ABEE63C772035C79851D3512107586E02A917F7C7E885E7C4A7D529710A145334CE67DC412CB1597B77AA2543B98D19CF2CB80C522BDBEA0F1B113FA2C86216C8C610A2D58F29CF3355CEB1BD3EF410D1EDD1F7AE0F16897979DE28C6EF293E0A19282BD1D793F1331523FC71A228800468C01A3653D14C6B4851A5C029478E757F </node>"
	"				<node tag=\"0x9F8122\"> 03 </node>"
	"				<node tag=\"0x9F8121\"> 969299D792D3CC08AD28F2D544CEE3309DADF1B9 </node>"
	"			</node>"

		//RID A000000003 I 52, SHA:6E6ED942 B5E51712 9C07C29C 9F3D0CE5 1DFCB655
	"			<node tag=\"0x110C\">"
	"				<node tag=\"0x9F06\"> A000000003 </node>"
	"				<node tag=\"0x9F22\"> 52 </node>"
	"				<node tag=\"0x9F8123\"> AFF740F8DBE763F333A1013A43722055C8E22F41779E219B0E1C409D60AFD45C8789C57EECD71EA4A269A675916CC1C5E1A05A35BD745A79F94555CE29612AC9338769665B87C3CA8E1AC4957F9F61FA7BFFE4E17631E937837CABF43DD6183D6360A228A3EBC73A1D1CDC72BF09953C81203AB7E492148E4CB774CDDFAAC3544D0DD4F8C8A0E9C70B877EA79F2C22E4CE52C69F3EF376F61B0F43A540FE96C63F586310C3B6E39C78C4D647CADB5933 </node>"
	"				<node tag=\"0x9F8122\"> 03 </node>"
	"				<node tag=\"0x9F8121\"> D6F78FB14CB58B0E0B67BFA7870FB8DFBEE2AD01 </node>"
	"			</node>"

		//RID A000000003 I 53, SHA:2E3A21AC A30C2C0D 1320D05A 586D5323 574E7E09
	"			<node tag=\"0x110D\">"
	"				<node tag=\"0x9F06\"> A000000003 </node>"
	"				<node tag=\"0x9F22\"> 53 </node>"
	"				<node tag=\"0x9F8123\"> BCD83721BE52CCCC4B6457321F22A7DC769F54EB8025913BE804D9EABBFA19B3D7C5D3CA658D768CAF57067EEC83C7E6E9F81D0586703ED9DDDADD20675D63424980B10EB364E81EB37DB40ED100344C928886FF4CCC37203EE6106D5B59D1AC102E2CD2D7AC17F4D96C398E5FD993ECB4FFDF79B17547FF9FA2AA8EEFD6CBDA124CBB17A0F8528146387135E226B005A474B9062FF264D2FF8EFA36814AA2950065B1B04C0A1AE9B2F69D4A4AA979D6CE95FEE9485ED0A03AEE9BD953E81CFD1EF6E814DFD3C2CE37AEFA38C1F9877371E91D6A5EB59FDEDF75D3325FA3CA66CDFBA0E57146CC789818FF06BE5FCC50ABD362AE4B80996D </node>"
	"				<node tag=\"0x9F8122\"> 03 </node>"
	"				<node tag=\"0x9F8121\"> A84A53964513A5D9363B4BA13AF5D43B83A83CE7 </node>"
	"			</node>"

		//RID A000000003 I 58, SHA:AAD03E75 5ACDE423 E7EA69BD 4A684B90 227CA534
	"			<node tag=\"0x110E\">"
	"				<node tag=\"0x9F06\"> A000000003 </node>"
	"				<node tag=\"0x9F22\"> 58 </node>"
	"				<node tag=\"0x9F8123\"> 99552C4A1ECD68A0260157FC4151B5992837445D3FC57365CA5692C87BE358CDCDF2C92FB6837522842A48EB11CDFFE2FD91770C7221E4AF6207C2DE4004C7DEE1B6276DC62D52A87D2CD01FBF2DC4065DB52824D2A2167A06D19E6A0F781071CDB2DD314CB94441D8DC0E936317B77BF06F5177F6C5ABA3A3BC6AA30209C97260B7A1AD3A192C9B8CD1D153570AFCC87C3CD681D13E997FE33B3963A0A1C79772ACF991033E1B8397AD0341500E48A24770BC4CBE19D2CCF419504FDBF0389BC2F2FDCD4D44E61F </node>"
	"				<node tag=\"0x9F8122\"> 010001 </node>"
	"				<node tag=\"0x9F8121\"> E6D302EBE7DC6F267E4D00F7D488F0AB6235F105 </node>"
	"			</node>"
#endif

	"			<node tag=\"0x110F\">"
	"				<node tag=\"0x9F06\"> A000000003 </node>"
	"				<node tag=\"0x9F22\"> 90 </node>"
	"				<node tag=\"0x9F8123\"> C26B3CB3833E42D8270DC10C8999B2DA18106838650DA0DBF154EFD51100AD144741B2A87D6881F8630E3348DEA3F78038E9B21A697EB2A6716D32CBF26086F1 </node>"
	"				<node tag=\"0x9F8122\"> 03 </node>"
	"				<node tag=\"0x9F8121\"> B3AE2BC3CAFC05EEEFAA46A2A47ED51DE679F823 </node>"
	"			</node>"

	"			<node tag=\"0x1110\">"
	"				<node tag=\"0x9F06\"> A000000003 </node>"
	"				<node tag=\"0x9F22\"> 92 </node>"
	"				<node tag=\"0x9F8123\"> 996AF56F569187D09293C14810450ED8EE3357397B18A2458EFAA92DA3B6DF6514EC060195318FD43BE9B8F0CC669E3F844057CBDDF8BDA191BB64473BC8DC9A730DB8F6B4EDE3924186FFD9B8C7735789C23A36BA0B8AF65372EB57EA5D89E7D14E9C7B6B557460F10885DA16AC923F15AF3758F0F03EBD3C5C2C949CBA306DB44E6A2C076C5F67E281D7EF56785DC4D75945E491F01918800A9E2DC66F60080566CE0DAF8D17EAD46AD8E30A247C9F </node>"
	"				<node tag=\"0x9F8122\"> 03 </node>"
	"				<node tag=\"0x9F8121\"> 429C954A3859CEF91295F663C963E582ED6EB253 </node>"
	"			</node>"

	"			<node tag=\"0x1111\">"
	"				<node tag=\"0x9F06\"> A000000003 </node>"
	"				<node tag=\"0x9F22\"> 94 </node>"
	"				<node tag=\"0x9F8123\"> ACD2B12302EE644F3F835ABD1FC7A6F62CCE48FFEC622AA8EF062BEF6FB8BA8BC68BBF6AB5870EED579BC3973E121303D34841A796D6DCBC41DBF9E52C4609795C0CCF7EE86FA1D5CB041071ED2C51D2202F63F1156C58A92D38BC60BDF424E1776E2BC9648078A03B36FB554375FC53D57C73F5160EA59F3AFC5398EC7B67758D65C9BFF7828B6B82D4BE124A416AB7301914311EA462C19F771F31B3B57336000DFF732D3B83DE07052D730354D297BEC72871DCCF0E193F171ABA27EE464C6A97690943D59BDABB2A27EB71CEEBDAFA1176046478FD62FEC452D5CA393296530AA3F41927ADFE434A2DF2AE3054F8840657A26E0FC617 </node>"
	"				<node tag=\"0x9F8122\"> 03 </node>"
	"				<node tag=\"0x9F8121\"> C4A3C43CCF87327D136B804160E47D43B60E6E0F </node>"
	"			</node>"

	"			<node tag=\"0x1112\">"
	"				<node tag=\"0x9F06\"> A000000003 </node>"
	"				<node tag=\"0x9F22\"> 95 </node>"
	"				<node tag=\"0x9F8123\"> BE9E1FA5E9A803852999C4AB432DB28600DCD9DAB76DFAAA47355A0FE37B1508AC6BF38860D3C6C2E5B12A3CAAF2A7005A7241EBAA7771112C74CF9A0634652FBCA0E5980C54A64761EA101A114E0F0B5572ADD57D010B7C9C887E104CA4EE1272DA66D997B9A90B5A6D624AB6C57E73C8F919000EB5F684898EF8C3DBEFB330C62660BED88EA78E909AFF05F6DA627B </node>"
	"				<node tag=\"0x9F8122\"> 03 </node>"
	"				<node tag=\"0x9F8121\"> EE1511CEC71020A9B90443B37B1D5F6E703030F6 </node>"
	"			</node>"

	"			<node tag=\"0x1113\">"
	"				<node tag=\"0x9F06\"> A000000003 </node>"
	"				<node tag=\"0x9F22\"> 96 </node>"
	"				<node tag=\"0x9F8123\"> B74586D19A207BE6627C5B0AAFBC44A2ECF5A2942D3A26CE19C4FFAEEE920521868922E893E7838225A3947A2614796FB2C0628CE8C11E3825A56D3B1BBAEF783A5C6A81F36F8625395126FA983C5216D3166D48ACDE8A431212FF763A7F79D9EDB7FED76B485DE45BEB829A3D4730848A366D3324C3027032FF8D16A1E44D8D </node>"
	"				<node tag=\"0x9F8122\"> 03 </node>"
	"				<node tag=\"0x9F8121\"> 7616E9AC8BE014AF88CA11A8FB17967B7394030E </node>"
	"			</node>"

	"			<node tag=\"0x1114\">"
	"				<node tag=\"0x9F06\"> A000000003 </node>"
	"				<node tag=\"0x9F22\"> 97 </node>"
	"				<node tag=\"0x9F8123\"> AF0754EAED977043AB6F41D6312AB1E22A6809175BEB28E70D5F99B2DF18CAE73519341BBBD327D0B8BE9D4D0E15F07D36EA3E3A05C892F5B19A3E9D3413B0D97E7AD10A5F5DE8E38860C0AD004B1E06F4040C295ACB457A788551B6127C0B29 </node>"
	"				<node tag=\"0x9F8122\"> 03 </node>"
	"				<node tag=\"0x9F8121\"> 8001CA76C1203955E2C62841CD6F201087E564BF </node>"
	"			</node>"

	"			<node tag=\"0x1115\">"
	"				<node tag=\"0x9F06\"> A000000003 </node>"
	"				<node tag=\"0x9F22\"> 98 </node>"
	"				<node tag=\"0x9F8123\"> CA026E52A695E72BD30AF928196EEDC9FAF4A619F2492E3FB31169789C276FFBB7D43116647BA9E0D106A3542E3965292CF77823DD34CA8EEC7DE367E08070895077C7EFAD939924CB187067DBF92CB1E785917BD38BACE0C194CA12DF0CE5B7A50275AC61BE7C3B436887CA98C9FD39 </node>"
	"				<node tag=\"0x9F8122\"> 03 </node>"
	"				<node tag=\"0x9F8121\"> E7AC9AA8EED1B5FF1BD532CF1489A3E5557572C1 </node>"
	"			</node>"

	"			<node tag=\"0x1116\">"
	"				<node tag=\"0x9F06\"> A000000003 </node>"
	"				<node tag=\"0x9F22\"> 99 </node>"
	"				<node tag=\"0x9F8123\"> AB79FCC9520896967E776E64444E5DCDD6E13611874F3985722520425295EEA4BD0C2781DE7F31CD3D041F565F747306EED62954B17EDABA3A6C5B85A1DE1BEB9A34141AF38FCF8279C9DEA0D5A6710D08DB4124F041945587E20359BAB47B7575AD94262D4B25F264AF33DEDCF28E09615E937DE32EDC03C54445FE7E382777 </node>"
	"				<node tag=\"0x9F8122\"> 03 </node>"
	"				<node tag=\"0x9F8121\"> 4ABFFD6B1C51212D05552E431C5B17007D2F5E6D </node>"
	"			</node>"

	"			<node tag=\"0x1118\">"
	"				<node tag=\"0x9F06\"> A000000004 </node>"
	"				<node tag=\"0x9F22\"> 00 </node>"
	"				<node tag=\"0x9F8123\"> 9E15214212F6308ACA78B80BD986AC287516846C8D548A9ED0A42E7D997C902C3E122D1B9DC30995F4E25C75DD7EE0A0CE293B8CC02B977278EF256D761194924764942FE714FA02E4D57F282BA3B2B62C9E38EF6517823F2CA831BDDF6D363D </node>"
	"				<node tag=\"0x9F8122\"> 03 </node>"
	"				<node tag=\"0x9F8121\"> 8BB99ADDF7B560110955014505FB6B5F8308CE27 </node>"
	"			</node>"

	"			<node tag=\"0x1119\">"
	"				<node tag=\"0x9F06\"> A000000004 </node>"
	"				<node tag=\"0x9F22\"> 01 </node>"
	"				<node tag=\"0x9F8123\"> D2010716C9FB5264D8C91A14F4F32F8981EE954F20087ED77CDC5868431728D3637C632CCF2718A4F5D92EA8AB166AB992D2DE24E9FBDC7CAB9729401E91C502D72B39F6866F5C098B1243B132AFEE65F5036E168323116338F8040834B98725 </node>"
	"				<node tag=\"0x9F8122\"> 03 </node>"
	"				<node tag=\"0x9F8121\"> EA950DD4234FEB7C900C0BE817F64DE66EEEF7C4 </node>"
	"			</node>"

	"			<node tag=\"0x111A\">"
	"				<node tag=\"0x9F06\"> A000000004 </node>"
	"				<node tag=\"0x9F22\"> 02 </node>"
	"				<node tag=\"0x9F8123\"> CF4264E1702D34CA897D1F9B66C5D63691EACC612C8F147116BB22D0C463495BD5BA70FB153848895220B8ADEEC3E7BAB31EA22C1DC9972FA027D54265BEBF0AE3A23A8A09187F21C856607B98BDA6FC908116816C502B3E58A145254EEFEE2A3335110224028B67809DCB8058E24895 </node>"
	"				<node tag=\"0x9F8122\"> 03 </node>"
	"				<node tag=\"0x9F8121\"> AF1CC1FD1C1BC9BCA07E78DA6CBA2163F169CBB7 </node>"
	"			</node>"

	"			<node tag=\"0x111B\">"
	"				<node tag=\"0x9F06\"> A000000004 </node>"
	"				<node tag=\"0x9F22\"> 03 </node>"
	"				<node tag=\"0x9F8123\"> C2490747FE17EB0584C88D47B1602704150ADC88C5B998BD59CE043EDEBF0FFEE3093AC7956AD3B6AD4554C6DE19A178D6DA295BE15D5220645E3C8131666FA4BE5B84FE131EA44B039307638B9E74A8C42564F892A64DF1CB15712B736E3374F1BBB6819371602D8970E97B900793C7C2A89A4A1649A59BE680574DD0B60145 </node>"
	"				<node tag=\"0x9F8122\"> 03 </node>"
	"				<node tag=\"0x9F8121\"> 5ADDF21D09278661141179CBEFF272EA384B13BB </node>"
	"			</node>"

	"			<node tag=\"0x111C\">"
	"				<node tag=\"0x9F06\"> A000000004 </node>"
	"				<node tag=\"0x9F22\"> 04 </node>"
	"				<node tag=\"0x9F8123\"> A6DA428387A502D7DDFB7A74D3F412BE762627197B25435B7A81716A700157DDD06F7CC99D6CA28C2470527E2C03616B9C59217357C2674F583B3BA5C7DCF2838692D023E3562420B4615C439CA97C44DC9A249CFCE7B3BFB22F68228C3AF13329AA4A613CF8DD853502373D62E49AB256D2BC17120E54AEDCED6D96A4287ACC5C04677D4A5A320DB8BEE2F775E5FEC5 </node>"
	"				<node tag=\"0x9F8122\"> 03 </node>"
	"				<node tag=\"0x9F8121\"> 381A035DA58B482EE2AF75F4C3F2CA469BA4AA6C </node>"
	"			</node>"

	"			<node tag=\"0x111D\">"
	"				<node tag=\"0x9F06\"> A000000004 </node>"
	"				<node tag=\"0x9F22\"> 05 </node>"
	"				<node tag=\"0x9F8123\"> B8048ABC30C90D976336543E3FD7091C8FE4800DF820ED55E7E94813ED00555B573FECA3D84AF6131A651D66CFF4284FB13B635EDD0EE40176D8BF04B7FD1C7BACF9AC7327DFAA8AA72D10DB3B8E70B2DDD811CB4196525EA386ACC33C0D9D4575916469C4E4F53E8E1C912CC618CB22DDE7C3568E90022E6BBA770202E4522A2DD623D180E215BD1D1507FE3DC90CA310D27B3EFCCD8F83DE3052CAD1E48938C68D095AAC91B5F37E28BB49EC7ED597 </node>"
	"				<node tag=\"0x9F8122\"> 03 </node>"
	"				<node tag=\"0x9F8121\"> EBFA0D5D06D8CE702DA3EAE890701D45E274C845 </node>"
	"			</node>"

	"			<node tag=\"0x111E\">"
	"				<node tag=\"0x9F06\"> A000000004 </node>"
	"				<node tag=\"0x9F22\"> 06 </node>"
	"				<node tag=\"0x9F8123\"> CB26FC830B43785B2BCE37C81ED334622F9622F4C89AAE641046B2353433883F307FB7C974162DA72F7A4EC75D9D657336865B8D3023D3D645667625C9A07A6B7A137CF0C64198AE38FC238006FB2603F41F4F3BB9DA1347270F2F5D8C606E420958C5F7D50A71DE30142F70DE468889B5E3A08695B938A50FC980393A9CBCE44AD2D64F630BB33AD3F5F5FD495D31F37818C1D94071342E07F1BEC2194F6035BA5DED3936500EB82DFDA6E8AFB655B1EF3D0D7EBF86B66DD9F29F6B1D324FE8B26CE38AB2013DD13F611E7A594D675C4432350EA244CC34F3873CBA06592987A1D7E852ADC22EF5A2EE28132031E48F74037E3B34AB747F </node>"
	"				<node tag=\"0x9F8122\"> 03 </node>"
	"				<node tag=\"0x9F8121\"> F910A1504D5FFB793D94F3B500765E1ABCAD72D9 </node>"
	"			</node>"

	"			<node tag=\"0x111F\">"
	"				<node tag=\"0x9F06\"> A000000004 </node>"
	"				<node tag=\"0x9F22\"> 09 </node>"
	"				<node tag=\"0x9F8123\"> 967B6264436C96AA9305776A5919C70DA796340F9997A6C6EF7BEF1D4DBF9CB4289FB7990ABFF1F3AE692F12844B2452A50AE075FB327976A40E8028F279B1E3CCB623957D696FC1225CA2EC950E2D415E9AA931FF18B13168D661FBD06F0ABB </node>"
	"				<node tag=\"0x9F8122\"> 03 </node>"
	"				<node tag=\"0x9F8121\"> 1D90595C2EF9FC6E71B0C721118333DF8A71FE21 </node>"
	"			</node>"

	"			<node tag=\"0x1120\">"
	"				<node tag=\"0x9F06\"> A000000004 </node>"
	"				<node tag=\"0x9F22\"> 22 </node>"
	"				<node tag=\"0x9F8123\"> BBE43877CC28C0CE1E14BC14E8477317E218364531D155BB8AC5B63C0D6E284DD24259193899F9C04C30BAF167D57929451F67AEBD3BBD0D41444501847D8F02F2C2A2D14817D97AE2625DC163BF8B484C40FFB51749CEDDE9434FB2A0A41099 </node>"
	"				<node tag=\"0x9F8122\"> 03 </node>"
	"				<node tag=\"0x9F8121\"> 008C39B1D119498268B07843349427AC6E98F807 </node>"
	"			</node>"

	"			<node tag=\"0x1121\">"
	"				<node tag=\"0x9F06\"> A000000004 </node>"
	"				<node tag=\"0x9F22\"> 52 </node>"
	"				<node tag=\"0x9F8123\"> B831414E0B4613922BD35B4B36802BC1E1E81C95A27C958F5382003DF646154CA92FC1CE02C3BE047A45E9B02A9089B4B90278237C965192A0FCC86BB49BC82AE6FDC2DE709006B86C7676EFDF597626FAD633A4F7DC48C445D37EB55FCB3B1ABB95BAAA826D5390E15FD14ED403FA2D0CB841C650609524EC555E3BC56CA957 </node>"
	"				<node tag=\"0x9F8122\"> 010001 </node>"
	"				<node tag=\"0x9F8121\"> DEB81EDB2626A4BB6AE23B77D19A77539D0E6716 </node>"
	"			</node>"

	"			<node tag=\"0x1122\">"
	"				<node tag=\"0x9F06\"> A000000004 </node>"
	"				<node tag=\"0x9F22\"> EF </node>"
	"				<node tag=\"0x9F8123\"> A191CB87473F29349B5D60A88B3EAEE0973AA6F1A082F358D849FDDFF9C091F899EDA9792CAF09EF28F5D22404B88A2293EEBBC1949C43BEA4D60CFD879A1539544E09E0F09F60F065B2BF2A13ECC705F3D468B9D33AE77AD9D3F19CA40F23DCF5EB7C04DC8F69EBA565B1EBCB4686CD274785530FF6F6E9EE43AA43FDB02CE00DAEC15C7B8FD6A9B394BABA419D3F6DC85E16569BE8E76989688EFEA2DF22FF7D35C043338DEAA982A02B866DE5328519EBBCD6F03CDD686673847F84DB651AB86C28CF1462562C577B853564A290C8556D818531268D25CC98A4CC6A0BDFFFDA2DCCA3A94C998559E307FDDF915006D9A987B07DDAEB3B </node>"
	"				<node tag=\"0x9F8122\"> 03 </node>"
	"				<node tag=\"0x9F8121\"> 21766EBB0EE122AFB65D7845B73DB46BAB65427A </node>"
	"			</node>"

	"			<node tag=\"0x1123\">"
	"				<node tag=\"0x9F06\"> A000000004 </node>"
	"				<node tag=\"0x9F22\"> F0 </node>"
	"				<node tag=\"0x9F8123\"> 7563C51B5276AA6370AB8405522414645832B6BEF2A989C771475B2E8DC654DC8A5BFF9E28E31FF1A370A40DC3FFEB06BC85487D5F1CB61C2441FD71CBCD05D883F8DE413B243AFC9DCA768B061E35B884B5D21B6B016AA36BA12DABCFE49F8E528C893C34C7D4793977E4CC99AB09640D9C7AAB7EC5FF3F40E3D4D18DF7E3A7 </node>"
	"				<node tag=\"0x9F8122\"> 03 </node>"
	"				<node tag=\"0x9F8121\"> AE667445F8DE6F82C38800E5EBABA322F03F58F2 </node>"
	"			</node>"

	"			<node tag=\"0x1124\">"
	"				<node tag=\"0x9F06\"> A000000004 </node>"
	"				<node tag=\"0x9F22\"> F1 </node>"
	"				<node tag=\"0x9F8123\"> A0DCF4BDE19C3546B4B6F0414D174DDE294AABBB828C5A834D73AAE27C99B0B053A90278007239B6459FF0BBCD7B4B9C6C50AC02CE91368DA1BD21AAEADBC65347337D89B68F5C99A09D05BE02DD1F8C5BA20E2F13FB2A27C41D3F85CAD5CF6668E75851EC66EDBF98851FD4E42C44C1D59F5984703B27D5B9F21B8FA0D93279FBBF69E090642909C9EA27F898959541AA6757F5F624104F6E1D3A9532F2A6E51515AEAD1B43B3D7835088A2FAFA7BE7 </node>"
	"				<node tag=\"0x9F8122\"> 03 </node>"
	"				<node tag=\"0x9F8121\"> D8E68DA167AB5A85D8C3D55ECB9B0517A1A5B4BB </node>"
	"			</node>"

	"			<node tag=\"0x1125\">"
	"				<node tag=\"0x9F06\"> A000000004 </node>"
	"				<node tag=\"0x9F22\"> F3 </node>"
	"				<node tag=\"0x9F8123\"> 98F0C770F23864C2E766DF02D1E833DFF4FFE92D696E1642F0A88C5694C6479D16DB1537BFE29E4FDC6E6E8AFD1B0EB7EA0124723C333179BF19E93F10658B2F776E829E87DAEDA9C94A8B3382199A350C077977C97AFF08FD11310AC950A72C3CA5002EF513FCCC286E646E3C5387535D509514B3B326E1234F9CB48C36DDD44B416D23654034A66F403BA511C5EFA3 </node>"
	"				<node tag=\"0x9F8122\"> 03 </node>"
	"				<node tag=\"0x9F8121\"> A69AC7603DAF566E972DEDC2CB433E07E8B01A9A </node>"
	"			</node>"

	"			<node tag=\"0x1126\">"
	"				<node tag=\"0x9F06\"> A000000004 </node>"
	"				<node tag=\"0x9F22\"> F5 </node>"
	"				<node tag=\"0x9F8123\"> A6E6FB72179506F860CCCA8C27F99CECD94C7D4F3191D303BBEE37481C7AA15F233BA755E9E4376345A9A67E7994BDC1C680BB3522D8C93EB0CCC91AD31AD450DA30D337662D19AC03E2B4EF5F6EC18282D491E19767D7B24542DFDEFF6F62185503532069BBB369E3BB9FB19AC6F1C30B97D249EEE764E0BAC97F25C873D973953E5153A42064BBFABFD06A4BB486860BF6637406C9FC36813A4A75F75C31CCA9F69F8DE59ADECEF6BDE7E07800FCBE035D3176AF8473E23E9AA3DFEE221196D1148302677C720CFE2544A03DB553E7F1B8427BA1CC72B0F29B12DFEF4C081D076D353E71880AADFF386352AF0AB7B28ED49E1E672D11F9 </node>"
	"				<node tag=\"0x9F8122\"> 010001 </node>"
	"				<node tag=\"0x9F8121\"> C2239804C8098170BE52D6D5D4159E81CE8466BF </node>"
	"			</node>"

	"			<node tag=\"0x1127\">"
	"				<node tag=\"0x9F06\"> A000000004 </node>"
	"				<node tag=\"0x9F22\"> F6 </node>"
	"				<node tag=\"0x9F8123\"> A25A6BD783A5EF6B8FB6F83055C260F5F99EA16678F3B9053E0F6498E82C3F5D1E8C38F13588017E2B12B3D8FF6F50167F46442910729E9E4D1B3739E5067C0AC7A1F4487E35F675BC16E233315165CB142BFDB25E301A632A54A3371EBAB6572DEEBAF370F337F057EE73B4AE46D1A8BC4DA853EC3CC12C8CBC2DA18322D68530C70B22BDAC351DD36068AE321E11ABF264F4D3569BB71214545005558DE26083C735DB776368172FE8C2F5C85E8B5B890CC682911D2DE71FA626B8817FCCC08922B703869F3BAEAC1459D77CD85376BC36182F4238314D6C4212FBDD7F23D3 </node>"
	"				<node tag=\"0x9F8122\"> 03 </node>"
	"				<node tag=\"0x9F8121\"> 502909ED545E3C8DBD00EA582D0617FEE9F6F684 </node>"
	"			</node>"

	"			<node tag=\"0x1128\">"
	"				<node tag=\"0x9F06\"> A000000004 </node>"
	"				<node tag=\"0x9F22\"> F7 </node>"
	"				<node tag=\"0x9F8123\"> 94EA62F6D58320E354C022ADDCF0559D8CF206CD92E869564905CE21D720F971B7AEA374830EBE1757115A85E088D41C6B77CF5EC821F30B1D890417BF2FA31E5908DED5FA677F8C7B184AD09028FDDE96B6A6109850AA800175EABCDBBB684A96C2EB6379DFEA08D32FE2331FE103233AD58DCDB1E6E077CB9F24EAEC5C25AF </node>"
	"				<node tag=\"0x9F8122\"> 010001 </node>"
	"				<node tag=\"0x9F8121\"> EEB0DD9B2477BEE3209A914CDBA94C1C4A9BDED9 </node>"
	"			</node>"

	"			<node tag=\"0x1129\">"
	"				<node tag=\"0x9F06\"> A000000004 </node>"
	"				<node tag=\"0x9F22\"> F8 </node>"
	"				<node tag=\"0x9F8123\"> A1F5E1C9BD8650BD43AB6EE56B891EF7459C0A24FA84F9127D1A6C79D4930F6DB1852E2510F18B61CD354DB83A356BD190B88AB8DF04284D02A4204A7B6CB7C5551977A9B36379CA3DE1A08E69F301C95CC1C20506959275F41723DD5D2925290579E5A95B0DF6323FC8E9273D6F849198C4996209166D9BFC973C361CC826E1 </node>"
	"				<node tag=\"0x9F8122\"> 03 </node>"
	"				<node tag=\"0x9F8121\"> F06ECC6D2AAEBF259B7E755A38D9A9B24E2FF3DD </node>"
	"			</node>"

	"			<node tag=\"0x112A\">"
	"				<node tag=\"0x9F06\"> A000000004 </node>"
	"				<node tag=\"0x9F22\"> F9 </node>"
	"				<node tag=\"0x9F8123\"> A99A6D3E071889ED9E3A0C391C69B0B804FC160B2B4BDD570C92DD5A0F45F53E8621F7C96C40224266735E1EE1B3C06238AE35046320FD8E81F8CEB3F8B4C97B940930A3AC5E790086DAD41A6A4F5117BA1CE2438A51AC053EB002AED866D2C458FD73359021A12029A0C043045C11664FE0219EC63C10BF2155BB2784609A106421D45163799738C1C30909BB6C6FE52BBB76397B9740CE064A613FF8411185F08842A423EAD20EDFFBFF1CD6C3FE0C9821479199C26D8572CC8AFFF087A9C3 </node>"
	"				<node tag=\"0x9F8122\"> 03 </node>"
	"				<node tag=\"0x9F8121\"> 336712DCC28554809C6AA9B02358DE6F755164DB </node>"
	"			</node>"

	"			<node tag=\"0x112B\">"
	"				<node tag=\"0x9F06\"> A000000004 </node>"
	"				<node tag=\"0x9F22\"> FA </node>"
	"				<node tag=\"0x9F8123\"> 9C6BE5ADB10B4BE3DCE2099B4B210672B89656EBA091204F613ECC623BEDC9C6D77B660E8BAEEA7F7CE30F1B153879A4E36459343D1FE47ACDBD41FCD710030C2BA1D9461597982C6E1BDD08554B726F5EFF7913CE59E79E357295C321E26D0B8BE270A9442345C753E2AA2ACFC9D30850602FE6CAC00C6DDF6B8D9D9B4879B2826B042A07F0E5AE526A3D3C4D22C72B9EAA52EED8893866F866387AC05A1399 </node>"
	"				<node tag=\"0x9F8122\"> 03 </node>"
	"				<node tag=\"0x9F8121\"> 0ABCADAD2C7558CA9C7081AE55DDDC714F8D45F8 </node>"
	"			</node>"

#if 0
		//RID A000000004 I FB, SHA:AEB44CA5 4F9DBDC9 A2569757 DB9CE0B9 E877E449
	"			<node tag=\"0x112C\">"
	"				<node tag=\"0x9F06\"> A000000004 </node>"
	"				<node tag=\"0x9F22\"> FB </node>"
	"				<node tag=\"0x9F8123\"> A9548DFB398B48123FAF41E6CFA4AE1E2352B518AB4BCEFECDB0B3EDEC090287D88B12259F361C1CC088E5F066494417E8EE8BBF8991E2B32FF16F994697842B3D6CB37A2BB5742A440B6356C62AA33DB3C455E59EDDF7864701D03A5B83EE9E9BD83AB93302AC2DFE63E66120B051CF081F56326A71303D952BB336FF12610D </node>"
	"				<node tag=\"0x9F8122\"> 02 </node>"
	"				<node tag=\"0x9F8121\"> 6C7289632919ABEE6E1163D7E6BF693FD88EBD35 </node>"
	"			</node>"

		//RID A000000004 I FC, SHA:21557574 F9AC0CF6 99750ED2 E141D257 57C9E02B
	"			<node tag=\"0x112D\">"
	"				<node tag=\"0x9F06\"> A000000004 </node>"
	"				<node tag=\"0x9F22\"> FC </node>"
	"				<node tag=\"0x9F8123\"> B37BFD2A9674AD6221C1A001081C62653DC280B0A9BD052C677C913CE7A0D902E77B12F4D4D79037B1E9B923A8BB3FAC3C612045BB3914F8DF41E9A1B61BFA5B41705A691D09CE6F530FE48B30240D98F4E692FFD6AADB87243BA8597AB237586ECF258F4148751BE5DA5A3BE6CC34BD </node>"
	"				<node tag=\"0x9F8122\"> 02 </node>"
	"				<node tag=\"0x9F8121\"> 7FB377EEBBCF7E3A6D04015D10E1BDCB15E21B80 </node>"
	"			</node>"

		//RID A000000004 I FD, SHA:46893162 DD48315E 198EC22A 03589134 E4609AAD
	"			<node tag=\"0x112E\">"
	"				<node tag=\"0x9F06\"> A000000004 </node>"
	"				<node tag=\"0x9F22\"> FD </node>"
	"				<node tag=\"0x9F8123\"> B3572BA49AE4C7B7A0019E5189E142CFCDED9498DDB5F0470567AB0BA713B8DA226424622955B54B937ABFEFAAD97919E377621E22196ABC1419D5ADC123484209EA7CB7029E66A0D54C5B45C8AD615AEDB6AE9E0A2F75310EA8961287241245 </node>"
	"				<node tag=\"0x9F8122\"> 02 </node>"
	"				<node tag=\"0x9F8121\"> 23CF0D702E0AEFE518E4FA6B836D3CD45B8AAA71 </node>"
	"			</node>"
#endif

	"			<node tag=\"0x112F\">"
	"				<node tag=\"0x9F06\"> A000000004 </node>"
	"				<node tag=\"0x9F22\"> FE </node>"
	"				<node tag=\"0x9F8123\"> A653EAC1C0F786C8724F737F172997D63D1C3251C44402049B865BAE877D0F398CBFBE8A6035E24AFA086BEFDE9351E54B95708EE672F0968BCD50DCE40F783322B2ABA04EF137EF18ABF03C7DBC5813AEAEF3AA7797BA15DF7D5BA1CBAF7FD520B5A482D8D3FEE105077871113E23A49AF3926554A70FE10ED728CF793B62A1 </node>"
	"				<node tag=\"0x9F8122\"> 03 </node>"
	"				<node tag=\"0x9F8121\"> 9A295B05FB390EF7923F57618A9FDA2941FC34E0 </node>"
	"			</node>"

	"			<node tag=\"0x1130\">"
	"				<node tag=\"0x9F06\"> A000000004 </node>"
	"				<node tag=\"0x9F22\"> FF </node>"
	"				<node tag=\"0x9F8123\"> B855CC64313AF99C453D181642EE7DD21A67D0FF50C61FE213BCDC18AFBCD07722EFDD2594EFDC227DA3DA23ADCC90E3FA907453ACC954C47323BEDCF8D4862C457D25F47B16D7C3502BE081913E5B0482D838484065DA5F6659E00A9E5D570ADA1EC6AF8C57960075119581FC81468D </node>"
	"				<node tag=\"0x9F8122\"> 03 </node>"
	"				<node tag=\"0x9F8121\"> B4E769CECF7AAC4783F305E0B110602A07A6355B </node>"
	"			</node>"

	"			<node tag=\"0x1131\">"
	"				<node tag=\"0x9F06\"> A000000025 </node>"
	"				<node tag=\"0x9F22\"> 01 </node>"
	"				<node tag=\"0x9F8123\"> AFAD7010F884E2824650F764D47D7951A16EED6DBB881F384DEDB6702E0FB55C0FBEF945A2017705E5286FA249A591E194BDCD74B21720B44CE986F144237A25F95789F38B47EA957F9ADB2372F6D5D41340A147EAC2AF324E8358AE1120EF3F </node>"
	"				<node tag=\"0x9F8122\"> 03 </node>"
	"				<node tag=\"0x9F8121\"> ACFE734CF09A84C7BF025F0FFC6FA8CA25A22869 </node>"
	"			</node>"

	"			<node tag=\"0x1132\">"
	"				<node tag=\"0x9F06\"> A000000025 </node>"
	"				<node tag=\"0x9F22\"> 02 </node>"
	"				<node tag=\"0x9F8123\"> AF4B8D230FDFCB1538E975795A1DB40C396A5359FAA31AE095CB522A5C82E7FFFB252860EC2833EC3D4A665F133DD934EE1148D81E2B7E03F92995DDF7EB7C90A75AB98E69C92EC91A533B21E1C4918B43AFED5780DE13A32BBD37EBC384FA3DD1A453E327C56024DACAEA74AA052C4D </node>"
	"				<node tag=\"0x9F8122\"> 03 </node>"
	"				<node tag=\"0x9F8121\"> 33F5B0344943048237EC89B275A95569718AEE20 </node>"
	"			</node>"

	"			<node tag=\"0x1133\">"
	"				<node tag=\"0x9F06\"> A000000025 </node>"
	"				<node tag=\"0x9F22\"> 03 </node>"
	"				<node tag=\"0x9F8123\"> B0C2C6E2A6386933CD17C239496BF48C57E389164F2A96BFF133439AE8A77B20498BD4DC6959AB0C2D05D0723AF3668901937B674E5A2FA92DDD5E78EA9D75D79620173CC269B35F463B3D4AAFF2794F92E6C7A3FB95325D8AB95960C3066BE548087BCB6CE12688144A8B4A66228AE4659C634C99E36011584C095082A3A3E3 </node>"
	"				<node tag=\"0x9F8122\"> 03 </node>"
	"				<node tag=\"0x9F8121\"> 8708A3E3BBC1BB0BE73EBD8D19D4E5D20166BF6C </node>"
	"			</node>"

	"			<node tag=\"0x1134\">"
	"				<node tag=\"0x9F06\"> A000000025 </node>"
	"				<node tag=\"0x9F22\"> 04 </node>"
	"				<node tag=\"0x9F8123\"> D0F543F03F2517133EF2BA4A1104486758630DCFE3A883C77B4E4844E39A9BD6360D23E6644E1E071F196DDF2E4A68B4A3D93D14268D7240F6A14F0D714C17827D279D192E88931AF7300727AE9DA80A3F0E366AEBA61778171737989E1EE309 </node>"
	"				<node tag=\"0x9F8122\"> 03 </node>"
	"				<node tag=\"0x9F8121\"> FDD7139EC7E0C33167FD61AD3CADBD68D66E91C5 </node>"
	"			</node>"

	"			<node tag=\"0x1135\">"
	"				<node tag=\"0x9F06\"> A000000025 </node>"
	"				<node tag=\"0x9F22\"> 10 </node>"
	"				<node tag=\"0x9F8123\"> CF98DFEDB3D3727965EE7797723355E0751C81D2D3DF4D18EBAB9FB9D49F38C8C4A826B99DC9DEA3F01043D4BF22AC3550E2962A59639B1332156422F788B9C16D40135EFD1BA94147750575E636B6EBC618734C91C1D1BF3EDC2A46A43901668E0FFC136774080E888044F6A1E65DC9AAA8928DACBEB0DB55EA3514686C6A732CEF55EE27CF877F110652694A0E3484C855D882AE191674E25C296205BBB599455176FDD7BBC549F27BA5FE35336F7E29E68D783973199436633C67EE5A680F05160ED12D1665EC83D1997F10FD05BBDBF9433E8F797AEE3E9F02A34228ACE927ABE62B8B9281AD08D3DF5C7379685045D7BA5FCDE58637 </node>"
	"				<node tag=\"0x9F8122\"> 03 </node>"
	"				<node tag=\"0x9F8121\"> C729CF2FD262394ABC4CC173506502446AA9B9FD </node>"
	"			</node>"

	"			<node tag=\"0x1136\">"
	"				<node tag=\"0x9F06\"> A000000025 </node>"
	"				<node tag=\"0x9F22\"> 52 </node>"
	"				<node tag=\"0x9F8123\"> B831414E0B4613922BD35B4B36802BC1E1E81C95A27C958F5382003DF646154CA92FC1CE02C3BE047A45E9B02A9089B4B90278237C965192A0FCC86BB49BC82AE6FDC2DE709006B86C7676EFDF597626FAD633A4F7DC48C445D37EB55FCB3B1ABB95BAAA826D5390E15FD14ED403FA2D0CB841C650609524EC555E3BC56CA957 </node>"
	"				<node tag=\"0x9F8122\"> 010001 </node>"
	"				<node tag=\"0x9F8121\"> 9D93DA5E86FDF16318D268CA6AC57031EDFCA3CB </node>"
	"			</node>"

	"			<node tag=\"0x1137\">"
	"				<node tag=\"0x9F06\"> A000000025 </node>"
	"				<node tag=\"0x9F22\"> 60 </node>"
	"				<node tag=\"0x9F8123\"> D0F543F03F2517133EF2BA4A1104486758630DCFE3A883C77B4E4844E39A9BD6360D23E6644E1E071F196DDF2E4A68B4A3D93D14268D7240F6A14F0D714C17827D279D192E88931AF7300727AE9DA80A3F0E366AEBA61778171737989E1EE309 </node>"
	"				<node tag=\"0x9F8122\"> 03 </node>"
	"				<node tag=\"0x9F8121\"> C08E256F276ED814021B11CAF6EC3701EC7553A1 </node>"
	"			</node>"

	"			<node tag=\"0x1138\">"
	"				<node tag=\"0x9F06\"> A000000025 </node>"
	"				<node tag=\"0x9F22\"> 62 </node>"
	"				<node tag=\"0x9F8123\"> BA29DE83090D8D5F4DFFCEB98918995A768F41D0183E1ACA3EF8D5ED9062853E4080E0D289A5CEDD4DD96B1FEA2C53428436CE15A2A1BFE69D46197D3F5A79BCF8F4858BFFA04EDB07FC5BE8560D9CE38F5C3CA3C742EDFDBAE3B5E6DDA45557 </node>"
	"				<node tag=\"0x9F8122\"> 03 </node>"
	"				<node tag=\"0x9F8121\"> CCC7303FF295A9F35BA61BD31E27EABD59658265 </node>"
	"			</node>"

	"			<node tag=\"0x1139\">"
	"				<node tag=\"0x9F06\"> A000000025 </node>"
	"				<node tag=\"0x9F22\"> 64 </node>"
	"				<node tag=\"0x9F8123\"> B0DD551047DAFCD10D9A5E33CF47A9333E3B24EC57E8F066A72DED60E881A8AD42777C67ADDF0708042AB943601EE60248540B67E0637018EEB3911AE9C873DAD66CB40BC8F4DC77EB2595252B61C21518F79B706AAC29E7D3FD4D259DB72B6E6D446DD60386DB40F5FDB076D80374C993B4BB2D1DB977C3870897F9DFA454F5 </node>"
	"				<node tag=\"0x9F8122\"> 03 </node>"
	"				<node tag=\"0x9F8121\"> 792B121D86D0F3A99582DB06974481F3B2E18454 </node>"
	"			</node>"

	"			<node tag=\"0x113A\">"
	"				<node tag=\"0x9F06\"> A000000025 </node>"
	"				<node tag=\"0x9F22\"> 65 </node>"
	"				<node tag=\"0x9F8123\"> E53EB41F839DDFB474F272CD0CBE373D5468EB3F50F39C95BDF4D39FA82B98DABC9476B6EA350C0DCE1CD92075D8C44D1E57283190F96B3537D9E632C461815EBD2BAF36891DF6BFB1D30FA0B752C43DCA0257D35DFF4CCFC98F84198D5152EC61D7B5F74BD09383BD0E2AA42298FFB02F0D79ADB70D72243EE537F75536A8A8DF962582E9E6812F3A0BE02A4365400D </node>"
	"				<node tag=\"0x9F8122\"> 03 </node>"
	"				<node tag=\"0x9F8121\"> 894C5D08D4EA28BB79DC46CEAD998B877322F416 </node>"
	"			</node>"

	"			<node tag=\"0x113B\">"
	"				<node tag=\"0x9F06\"> A000000025 </node>"
	"				<node tag=\"0x9F22\"> 66 </node>"
	"				<node tag=\"0x9F8123\"> BD1478877B9333612D257D9E3C9C23503E28336B723C71F47C25836670395360F53C106FD74DEEEA291259C001AFBE7B4A83654F6E2D9E8148E2CB1D9223AC5903DA18B433F8E3529227505DE84748F241F7BFCD2146E5E9A8C5D2A06D19097087A069F9AE3D610C7C8E1214481A4F27025A1A2EDB8A9CDAFA445690511DB805 </node>"
	"				<node tag=\"0x9F8122\"> 03 </node>"
	"				<node tag=\"0x9F8121\"> F367CB70F9C9B67B580F533819E302BAC0330090 </node>"
	"			</node>"

	"			<node tag=\"0x113C\">"
	"				<node tag=\"0x9F06\"> A000000025 </node>"
	"				<node tag=\"0x9F22\"> 67 </node>"
	"				<node tag=\"0x9F8123\"> C687ADCCF3D57D3360B174E471EDA693AA555DFDC6C8CD394C74BA25CCDF8EABFD1F1CEADFBE2280C9E81F7A058998DC22B7F22576FE84713D0BDD3D34CFCD12FCD0D26901BA74103D075C664DABCCAF57BF789494051C5EC303A2E1D784306D3DB3EB665CD360A558F40B7C05C919B2F0282FE1ED9BF6261AA814648FBC263B14214491DE426D242D65CD1FFF0FBE4D4DAFF5CFACB2ADC7131C9B147EE791956551076270696B75FD97373F1FD7804F </node>"
	"				<node tag=\"0x9F8122\"> 03 </node>"
	"				<node tag=\"0x9F8121\"> 52A2907300C8445BF54B970C894691FEADF2D28E </node>"
	"			</node>"

	"			<node tag=\"0x113D\">"
	"				<node tag=\"0x9F06\"> A000000025 </node>"
	"				<node tag=\"0x9F22\"> 68 </node>"
	"				<node tag=\"0x9F8123\"> F4D198F2F0CF140E4D2D81B765EB4E24CED4C0834822769854D0E97E8066CBE465029B3F410E350F6296381A253BE71A4BBABBD516625DAE67D073D00113AAB9EA4DCECA29F3BB7A5D46C0D8B983E2482C2AD759735A5AB9AAAEFB31D3E718B8CA66C019ECA0A8BE312E243EB47A62300620BD51CF169A9194C17A42E51B34D83775A98E80B2D66F4F98084A448FE0507EA27C905AEE72B62A8A29438B6A4480FFF72F93280432A55FDD648AD93D82B9ECF01275C0914BAD8EB3AAF46B129F8749FEA425A2DCDD7E813A08FC0CA7841EDD49985CD8BC6D5D56F17AB9C67CEC50BA422440563ECCE21699E435C8682B6266393672C693D8B7 </node>"
	"				<node tag=\"0x9F8122\"> 03 </node>"
	"				<node tag=\"0x9F8121\"> 415E5FE9EC966C835FBB3E6F766A9B1A4B8674C3 </node>"
	"			</node>"

	"			<node tag=\"0x113E\">"
	"				<node tag=\"0x9F06\"> A000000025 </node>"
	"				<node tag=\"0x9F22\"> 96 </node>"
	"				<node tag=\"0x9F8123\"> BC9AA294B1FDD263176E3243D8F448BBFFCB6ABD02C31811289F5085A9262B8B1B7C6477EB58055D9EF32A83D1B72D4A1471ECA30CE76585C3FD05372B686F92B795B1640959201523230149118D52D2425BD11C863D9B2A7C4AD0A2BFDBCA67B2713B290F493CD5521E5DDF05EF1040FC238D0A851C8E3E3B2B1F0D5D9D4AED </node>"
	"				<node tag=\"0x9F8122\"> 03 </node>"
	"				<node tag=\"0x9F8121\"> E7433E5CFC6001151D8ECD252EBC6E61F7AB2217 </node>"
	"			</node>"

	"			<node tag=\"0x113F\">"
	"				<node tag=\"0x9F06\"> A000000025 </node>"
	"				<node tag=\"0x9F22\"> 97 </node>"
	"				<node tag=\"0x9F8123\"> E178FFE834B4B767AF3C9A511F973D8E8505C5FCB2D3768075AB7CC946A955789955879AAF737407151521996DFA43C58E6B130EB1D863B85DC9FFB4050947A2676AA6A061A4A7AE1EDB0E36A697E87E037517EB8923136875BA2CA1087CBA7EC7653E5E28A0C261A033AF27E3A67B64BBA26956307EC47E674E3F8B722B3AE0498DB16C7985310D9F3D117300D32B09 </node>"
	"				<node tag=\"0x9F8122\"> 03 </node>"
	"				<node tag=\"0x9F8121\"> EBDA522B631B3EB4F4CBFC0679C450139D2B69CD </node>"
	"			</node>"

	"			<node tag=\"0x1140\">"
	"				<node tag=\"0x9F06\"> A000000025 </node>"
	"				<node tag=\"0x9F22\"> 98 </node>"
	"				<node tag=\"0x9F8123\"> D31A7094FB221CBA6660FB975AAFEA80DB7BB7EAFD7351E748827AB62D4AEECCFC1787FD47A04699A02DB00D7C382E80E804B35C59434C602389D691B9CCD51ED06BE67A276119C4C10E2E40FC4EDDF9DF39B9B0BDEE8D076E2A012E8A292AF8EFE18553470639C1A032252E0E5748B25A3F9BA4CFCEE073038B061837F2AC1B04C279640F5BD110A9DC665ED2FA6828BD5D0FE810A892DEE6B0E74CE8863BDE08FD5FD61A0F11FA0D14978D8CED7DD3 </node>"
	"				<node tag=\"0x9F8122\"> 03 </node>"
	"				<node tag=\"0x9F8121\"> D4DBA428CF11D45BAEB0A35CAEA8007AD8BA8D71 </node>"
	"			</node>"

#if 0
		//RID A000000025 I 99, SHA:020D7994 2D694F0F 369F2859 07721845 3BB60580
	"			<node tag=\"0x1141\">"
	"				<node tag=\"0x9F06\"> A000000025 </node>"
	"				<node tag=\"0x9F22\"> 99 </node>"
	"				<node tag=\"0x9F8123\"> E1740074229FA0D228A9623581D7A322903FB89BA7686712E601FA8AB24A9789186F15B70CCBBE7421B1CB110D45361688135FFD0DB15A3F516BB291D4A123EBF5A06FBF7E1EE6311B737DABB289570A7959D532B25F1DA6758C84DDCCADC049BC764C05391ABD2CADEFFA7E242D5DD06E56001F0E68151E3388074BD9330D6AFA57CBF33946F531E51E0D4902EE235C756A905FB733940E6EC897B4944A5EDC765705E2ACF76C78EAD78DD9B066DF0B2C88750B8AEE00C9B4D4091FA7338449DA92DBFC908FA0781C0128C492DB993C88BA8BB7CADFE238D477F2517E0E7E3D2B11796A0318CE2AD4DA1DB8E54AB0D94F109DB9CAEEFBEF </node>"
	"				<node tag=\"0x9F8122\"> 03 </node>"
	"				<node tag=\"0x9F8121\"> F0885777642C96BB24441FA057AD9A3490763BD2 </node>"
	"			</node>"
#endif

	"			<node tag=\"0x1142\">"
	"				<node tag=\"0x9F06\"> A000000025 </node>"
	"				<node tag=\"0x9F22\"> 0E </node>"
	"				<node tag=\"0x9F8123\"> AA94A8C6DAD24F9BA56A27C09B01020819568B81A026BE9FD0A3416CA9A71166ED5084ED91CED47DD457DB7E6CBCD53E560BC5DF48ABC380993B6D549F5196CFA77DFB20A0296188E969A2772E8C4141665F8BB2516BA2C7B5FC91F8DA04E8D512EB0F6411516FB86FC021CE7E969DA94D33937909A53A57F907C40C22009DA7532CB3BE509AE173B39AD6A01BA5BB85 </node>"
	"				<node tag=\"0x9F8122\"> 03 </node>"
	"				<node tag=\"0x9F8121\"> A7266ABAE64B42A3668851191D49856E17F8FBCD </node>"
	"			</node>"

	"			<node tag=\"0x1143\">"
	"				<node tag=\"0x9F06\"> A000000025 </node>"
	"				<node tag=\"0x9F22\"> 0F </node>"
	"				<node tag=\"0x9F8123\"> C8D5AC27A5E1FB89978C7C6479AF993AB3800EB243996FBB2AE26B67B23AC482C4B746005A51AFA7D2D83E894F591A2357B30F85B85627FF15DA12290F70F05766552BA11AD34B7109FA49DE29DCB0109670875A17EA95549E92347B948AA1F045756DE56B707E3863E59A6CBE99C1272EF65FB66CBB4CFF070F36029DD76218B21242645B51CA752AF37E70BE1A84FF31079DC0048E928883EC4FADD497A719385C2BBBEBC5A66AA5E5655D18034EC5 </node>"
	"				<node tag=\"0x9F8122\"> 03 </node>"
	"				<node tag=\"0x9F8121\"> A73472B3AB557493A9BC2179CC8014053B12BAB4 </node>"
	"			</node>"

#if 0
		//RID A000000025 I A1, SHA:5E1C8C6E 2FC96CF6 1972A140 7338A337 5BBE63AF
	"			<node tag=\"0x1144\">"
	"				<node tag=\"0x9F06\"> A000000025 </node>"
	"				<node tag=\"0x9F22\"> A1 </node>"
	"				<node tag=\"0x9F8123\"> 99D17396421EE3F919BA549D9554BE0D4F92CB8B53B4878ED60CC5B2DEEDC79B85C8BD6FD2F23C22E68B381AEEB74153AFB3C96E6C96AD018E73C2025D1EE77622A72BEE973C1AF7B908468D74FDB53DCE8380523E38C30D0A8A226529726824E209E668F49F43B0E8CD2FE527CE7CC41F33F434F95D6E2FE2F589372032F2D6504340F8C542D298B499A53D95AF4083 </node>"
	"				<node tag=\"0x9F8122\"> 010001 </node>"
	"				<node tag=\"0x9F8121\"> BBB9ABE889611198C387B5B0AB374934BC2B2EA9 </node>"
	"			</node>"
#endif

	"			<node tag=\"0x1145\">"
	"				<node tag=\"0x9F06\"> A000000025 </node>"
	"				<node tag=\"0x9F22\"> C1 </node>"
	"				<node tag=\"0x9F8123\"> E69E319C34D1B4FB43AED4BD8BBA6F7A8B763F2F6EE5DDF7C92579A984F89C4A9C15B27037764C58AC7E45EFBC34E138E56BA38F76E803129A8DDEB5E1CC8C6B30CF634A9C9C1224BF1F0A9A18D79ED41EBCF1BE78087AE8B7D2F896B1DE8B7E784161A138A0F2169AD33E146D1B16AB595F9D7D98BE671062D217F44EB68C68640C7D57465A063F6BAC776D3E2DAC61 </node>"
	"				<node tag=\"0x9F8122\"> 03 </node>"
	"				<node tag=\"0x9F8121\"> DC79D6B5FC879362299BC5A637DAD2E0D99656B8 </node>"
	"			</node>"

	"			<node tag=\"0x1146\">"
	"				<node tag=\"0x9F06\"> A000000025 </node>"
	"				<node tag=\"0x9F22\"> C2 </node>"
	"				<node tag=\"0x9F8123\"> B875002F38BA26D61167C5D440367604AD38DF2E93D8EE8DA0E8D9C0CF4CC5788D11DEA689E5F41D23A3DA3E0B1FA5875AE25620F5A6BCCEE098C1B35C691889D7D0EF670EB8312E7123FCC5DC7D2F0719CC80E1A93017F944D097330EDF945762FEE62B7B0BA0348228DBF38D4216E5A67A7EF74F5D3111C44AA31320F623CB3C53E60966D6920067C9E082B746117E48E4F00E110950CA54DA3E38E5453BD5544E3A6760E3A6A42766AD2284E0C9AF </node>"
	"				<node tag=\"0x9F8122\"> 03 </node>"
	"				<node tag=\"0x9F8121\"> 8E748296359A7428F536ADDA8E2C037E2B697EF6 </node>"
	"			</node>"

	"			<node tag=\"0x1147\">"
	"				<node tag=\"0x9F06\"> A000000025 </node>"
	"				<node tag=\"0x9F22\"> C3 </node>"
	"				<node tag=\"0x9F8123\"> B93182ABE343DFBF388C71C4D6747DCDEC60367FE63CFAA942D7D323E688D0832836548BF0EDFF1EDEEB882C75099FF81A93FA525C32425B36023EA02A8899B9BF7D7934E86F997891823006CEAA93091A73C1FDE18ABD4F87A22308640C064C8C027685F1B2DB7B741B67AB0DE05E870481C5F972508C17F57E4F833D63220F6EA2CFBB878728AA5887DE407D10C6B8F58D46779ECEC1E2155487D52C78A5C03897F2BB580E0A2BBDE8EA2E1C18F6AAF3EB3D04C3477DEAB88F150C8810FD1EF8EB0596866336FE2C1FBC6BEC22B4FE5D885647726DB59709A505F75C49E0D8D71BF51E4181212BE2142AB2A1E8C0D3B7136CD7B7708E4D </node>"
	"				<node tag=\"0x9F8122\"> 03 </node>"
	"				<node tag=\"0x9F8121\"> 12F1790CB0273DC73C6E70784BC24C12E8DB71F6 </node>"
	"			</node>"

	"			<node tag=\"0x1148\">"
	"				<node tag=\"0x9F06\"> A000000025 </node>"
	"				<node tag=\"0x9F22\"> C8 </node>"
	"				<node tag=\"0x9F8123\"> BF0CFCED708FB6B048E3014336EA24AA007D7967B8AA4E613D26D015C4FE7805D9DB131CED0D2A8ED504C3B5CCD48C33199E5A5BF644DA043B54DBF60276F05B1750FAB39098C7511D04BABC649482DDCF7CC42C8C435BAB8DD0EB1A620C31111D1AAAF9AF6571EEBD4CF5A08496D57E7ABDBB5180E0A42DA869AB95FB620EFF2641C3702AF3BE0B0C138EAEF202E21D </node>"
	"				<node tag=\"0x9F8122\"> 03 </node>"
	"				<node tag=\"0x9F8121\"> 33BD7A059FAB094939B90A8F35845C9DC779BD50 </node>"
	"			</node>"

	"			<node tag=\"0x1149\">"
	"				<node tag=\"0x9F06\"> A000000025 </node>"
	"				<node tag=\"0x9F22\"> C9 </node>"
	"				<node tag=\"0x9F8123\"> B362DB5733C15B8797B8ECEE55CB1A371F760E0BEDD3715BB270424FD4EA26062C38C3F4AAA3732A83D36EA8E9602F6683EECC6BAFF63DD2D49014BDE4D6D603CD744206B05B4BAD0C64C63AB3976B5C8CAAF8539549F5921C0B700D5B0F83C4E7E946068BAAAB5463544DB18C63801118F2182EFCC8A1E85E53C2A7AE839A5C6A3CABE73762B70D170AB64AFC6CA482944902611FB0061E09A67ACB77E493D998A0CCF93D81A4F6C0DC6B7DF22E62DB </node>"
	"				<node tag=\"0x9F8122\"> 03 </node>"
	"				<node tag=\"0x9F8121\"> 8E8DFF443D78CD91DE88821D70C98F0638E51E49 </node>"
	"			</node>"

	"			<node tag=\"0x114A\">"
	"				<node tag=\"0x9F06\"> A000000025 </node>"
	"				<node tag=\"0x9F22\"> CA </node>"
	"				<node tag=\"0x9F8123\"> C23ECBD7119F479C2EE546C123A585D697A7D10B55C2D28BEF0D299C01DC65420A03FE5227ECDECB8025FBC86EEBC1935298C1753AB849936749719591758C315FA150400789BB14FADD6EAE2AD617DA38163199D1BAD5D3F8F6A7A20AEF420ADFE2404D30B219359C6A4952565CCCA6F11EC5BE564B49B0EA5BF5B3DC8C5C6401208D0029C3957A8C5922CBDE39D3A564C6DEBB6BD2AEF91FC27BB3D3892BEB9646DCE2E1EF8581EFFA712158AAEC541C0BBB4B3E279D7DA54E45A0ACC3570E712C9F7CDF985CFAFD382AE13A3B214A9E8E1E71AB1EA707895112ABC3A97D0FCB0AE2EE5C85492B6CFD54885CDD6337E895CC70FB3255E3 </node>"
	"				<node tag=\"0x9F8122\"> 03 </node>"
	"				<node tag=\"0x9F8121\"> 6BDA32B1AA171444C7E8F88075A74FBFE845765F </node>"
	"			</node>"

	"			<node tag=\"0x114B\">"
	"				<node tag=\"0x9F06\"> A000000065 </node>"
	"				<node tag=\"0x9F22\"> 07 </node>"
	"				<node tag=\"0x9F8123\"> B8DAB883EF1B5763E4F861F9EA3044B276635D402F3AE5E6B6C0547E368E79A36366DAC5609B6EC486DA1A8D2002CA4F4EFC2CB0EC1573A0B0917969EB60645BAEDF11C050C5D07FED817D11E84A174859A0DAE7F7935F109229C0AC4EE5BFB3D65533A679F0486C5AEFCC937379833BEC45D79DCF97B5228B1910FA03765331 </node>"
	"				<node tag=\"0x9F8122\"> 010001 </node>"
	"				<node tag=\"0x9F8121\"> 4896DD5A9EB5B11B9DCCA6DFE336C009F69F509A </node>"
	"			</node>"

	"			<node tag=\"0x114C\">"
	"				<node tag=\"0x9F06\"> A000000065 </node>"
	"				<node tag=\"0x9F22\"> 08 </node>"
	"				<node tag=\"0x9F8123\"> B74670DAD1DC8983652000E5A7F2F8B35DFD083EE593E5BA895C95729F2BADE9C8ABF3DD9CE240C451C6CEFFC768D83CBAC76ABB8FEA58F013C647007CFF7617BAC2AE3981816F25CC7E5238EF34C4F02D0B01C24F80C2C65E7E7743A4FA8E23206A23ECE290C26EA56DB085C5C5EAE26292451FC8292F9957BE8FF20FAD53E5 </node>"
	"				<node tag=\"0x9F8122\"> 03 </node>"
	"				<node tag=\"0x9F8121\"> DD36D5896228C8C4900742F107E2F91FE50BC7EE </node>"
	"			</node>"

	"			<node tag=\"0x114D\">"
	"				<node tag=\"0x9F06\"> A000000065 </node>"
	"				<node tag=\"0x9F22\"> 09 </node>"
	"				<node tag=\"0x9F8123\"> B72A8FEF5B27F2B550398FDCC256F714BAD497FF56094B7408328CB626AA6F0E6A9DF8388EB9887BC930170BCC1213E90FC070D52C8DCD0FF9E10FAD36801FE93FC998A721705091F18BC7C98241CADC15A2B9DA7FB963142C0AB640D5D0135E77EBAE95AF1B4FEFADCF9C012366BDDA0455C1564A68810D7127676D493890BD </node>"
	"				<node tag=\"0x9F8122\"> 03 </node>"
	"				<node tag=\"0x9F8121\"> 4410C6D51C2F83ADFD92528FA6E38A32DF048D0A </node>"
	"			</node>"

	"			<node tag=\"0x114E\">"
	"				<node tag=\"0x9F06\"> A000000065 </node>"
	"				<node tag=\"0x9F22\"> 10 </node>"
	"				<node tag=\"0x9F8123\"> 99B63464EE0B4957E4FD23BF923D12B61469B8FFF8814346B2ED6A780F8988EA9CF0433BC1E655F05EFA66D0C98098F25B659D7A25B8478A36E489760D071F54CDF7416948ED733D816349DA2AADDA227EE45936203CBF628CD033AABA5E5A6E4AE37FBACB4611B4113ED427529C636F6C3304F8ABDD6D9AD660516AE87F7F2DDF1D2FA44C164727E56BBC9BA23C0285 </node>"
	"				<node tag=\"0x9F8122\"> 03 </node>"
	"				<node tag=\"0x9F8121\"> C75E5210CBE6E8F0594A0F1911B07418CADB5BAB </node>"
	"			</node>"

	"			<node tag=\"0x114F\">"
	"				<node tag=\"0x9F06\"> A000000065 </node>"
	"				<node tag=\"0x9F22\"> 11 </node>"
	"				<node tag=\"0x9F8123\"> A2583AA40746E3A63C22478F576D1EFC5FB046135A6FC739E82B55035F71B09BEB566EDB9968DD649B94B6DEDC033899884E908C27BE1CD291E5436F762553297763DAA3B890D778C0F01E3344CECDFB3BA70D7E055B8C760D0179A403D6B55F2B3B083912B183ADB7927441BED3395A199EEFE0DEBD1F5FC3264033DA856F4A8B93916885BD42F9C1F456AAB8CFA83AC574833EB5E87BB9D4C006A4B5346BD9E17E139AB6552D9C58BC041195336485 </node>"
	"				<node tag=\"0x9F8122\"> 03 </node>"
	"				<node tag=\"0x9F8121\"> D9FD62C9DD4E6DE7741E9A17FB1FF2C5DB948BCB </node>"
	"			</node>"

	"			<node tag=\"0x1150\">"
	"				<node tag=\"0x9F06\"> A000000065 </node>"
	"				<node tag=\"0x9F22\"> 12 </node>"
	"				<node tag=\"0x9F8123\"> ADF05CD4C5B490B087C3467B0F3043750438848461288BFEFD6198DD576DC3AD7A7CFA07DBA128C247A8EAB30DC3A30B02FCD7F1C8167965463626FEFF8AB1AA61A4B9AEF09EE12B009842A1ABA01ADB4A2B170668781EC92B60F605FD12B2B2A6F1FE734BE510F60DC5D189E401451B62B4E06851EC20EBFF4522AACC2E9CDC89BC5D8CDE5D633CFD77220FF6BBD4A9B441473CC3C6FEFC8D13E57C3DE97E1269FA19F655215B23563ED1D1860D8681 </node>"
	"				<node tag=\"0x9F8122\"> 03 </node>"
	"				<node tag=\"0x9F8121\"> 874B379B7F607DC1CAF87A19E400B6A9E25163E8 </node>"
	"			</node>"

	"			<node tag=\"0x1151\">"
	"				<node tag=\"0x9F06\"> A000000065 </node>"
	"				<node tag=\"0x9F22\"> 14 </node>"
	"				<node tag=\"0x9F8123\"> AEED55B9EE00E1ECEB045F61D2DA9A66AB637B43FB5CDBDB22A2FBB25BE061E937E38244EE5132F530144A3F268907D8FD648863F5A96FED7E42089E93457ADC0E1BC89C58A0DB72675FBC47FEE9FF33C16ADE6D341936B06B6A6F5EF6F66A4EDD981DF75DA8399C3053F430ECA342437C23AF423A211AC9F58EAF09B0F837DE9D86C7109DB1646561AA5AF0289AF5514AC64BC2D9D36A179BB8A7971E2BFA03A9E4B847FD3D63524D43A0E8003547B94A8A75E519DF3177D0A60BC0B4BAB1EA59A2CBB4D2D62354E926E9C7D3BE4181E81BA60F8285A896D17DA8C3242481B6C405769A39D547C74ED9FF95A70A796046B5EFF36682DC29 </node>"
	"				<node tag=\"0x9F8122\"> 03 </node>"
	"				<node tag=\"0x9F8121\"> C0D15F6CD957E491DB56DCDD1CA87A03EBE06B7B </node>"
	"			</node>"

	"			<node tag=\"0x1152\">"
	"				<node tag=\"0x9F06\"> A000000065 </node>"
	"				<node tag=\"0x9F22\"> 0A </node>"
	"				<node tag=\"0x9F8123\"> 99B63464EE0B4957E4FD23BF923D12B61469B8FFF8814346B2ED6A780F8988EA9CF0433BC1E655F05EFA66D0C98098F25B659D7A25B8478A36E489760D071F54CDF7416948ED733D816349DA2AADDA227EE45936203CBF628CD033AABA5E5A6E4AE37FBACB4611B4113ED427529C636F6C3304F8ABDD6D9AD660516AE87F7F2DDF1D2FA44C164727E56BBC9BA23C0285 </node>"
	"				<node tag=\"0x9F8122\"> 03 </node>"
	"				<node tag=\"0x9F8121\"> B8A17CEDC3DFD3958D87AA7D87B6048CA557B787 </node>"
	"			</node>"

	"			<node tag=\"0x1153\">"
	"				<node tag=\"0x9F06\"> A000000065 </node>"
	"				<node tag=\"0x9F22\"> 0C </node>"
	"				<node tag=\"0x9F8123\"> ADF05CD4C5B490B087C3467B0F3043750438848461288BFEFD6198DD576DC3AD7A7CFA07DBA128C247A8EAB30DC3A30B02FCD7F1C8167965463626FEFF8AB1AA61A4B9AEF09EE12B009842A1ABA01ADB4A2B170668781EC92B60F605FD12B2B2A6F1FE734BE510F60DC5D189E401451B62B4E06851EC20EBFF4522AACC2E9CDC89BC5D8CDE5D633CFD77220FF6BBD4A9B441473CC3C6FEFC8D13E57C3DE97E1269FA19F655215B23563ED1D1860D8681 </node>"
	"				<node tag=\"0x9F8122\"> 03 </node>"
	"				<node tag=\"0x9F8121\"> CD82812F9E844048F48ED56ED84EFB3267E870F8 </node>"
	"			</node>"

	"			<node tag=\"0x1154\">"
	"				<node tag=\"0x9F06\"> A000000065 </node>"
	"				<node tag=\"0x9F22\"> 0E </node>"
	"				<node tag=\"0x9F8123\"> AEED55B9EE00E1ECEB045F61D2DA9A66AB637B43FB5CDBDB22A2FBB25BE061E937E38244EE5132F530144A3F268907D8FD648863F5A96FED7E42089E93457ADC0E1BC89C58A0DB72675FBC47FEE9FF33C16ADE6D341936B06B6A6F5EF6F66A4EDD981DF75DA8399C3053F430ECA342437C23AF423A211AC9F58EAF09B0F837DE9D86C7109DB1646561AA5AF0289AF5514AC64BC2D9D36A179BB8A7971E2BFA03A9E4B847FD3D63524D43A0E8003547B94A8A75E519DF3177D0A60BC0B4BAB1EA59A2CBB4D2D62354E926E9C7D3BE4181E81BA60F8285A896D17DA8C3242481B6C405769A39D547C74ED9FF95A70A796046B5EFF36682DC29 </node>"
	"				<node tag=\"0x9F8122\"> 03 </node>"
	"				<node tag=\"0x9F8121\"> A163724B2CB923376367BBE06B09C03F136F4393 </node>"
	"			</node>"

	"			<node tag=\"0x1155\">"
	"				<node tag=\"0x9F06\"> A000000065 </node>"
	"				<node tag=\"0x9F22\"> 0F </node>"
	"				<node tag=\"0x9F8123\"> 9EFBADDE4071D4EF98C969EB32AF854864602E515D6501FDE576B310964A4F7C2CE842ABEFAFC5DC9E26A619BCF2614FE07375B9249BEFA09CFEE70232E75FFD647571280C76FFCA87511AD255B98A6B577591AF01D003BD6BF7E1FCE4DFD20D0D0297ED5ECA25DE261F37EFE9E175FB5F12D2503D8CFB060A63138511FE0E125CF3A643AFD7D66DCF9682BD246DDEA1 </node>"
	"				<node tag=\"0x9F8122\"> 03 </node>"
	"				<node tag=\"0x9F8121\"> 2A1B82DE00F5F0C401760ADF528228D3EDE0F403 </node>"
	"			</node>"

	"			<node tag=\"0x1156\">"
	"				<node tag=\"0x9F06\"> A000000065 </node>"
	"				<node tag=\"0x9F22\"> DA </node>"
	"				<node tag=\"0x9F8123\"> DC7449EC24944EA4C09EF37656F5390594DE4F1686AFC4B1C21C893F5F3AEFC5A8109E06A52389C0414E7DFBC44293B04D5F4E85528FA85F1A9706BAAAA034E8B44111C043B1CC95309C6946225971D4B158909F4438726812777FAC4D06879A7AA41089F0DD2C27B3EAA23A8D02E2A9A9B8EDAD0CA32AE91B383740CF50E5E5 </node>"
	"				<node tag=\"0x9F8122\"> 03 </node>"
	"				<node tag=\"0x9F8121\"> 5C037E1815E38B79D4D5C15FF90CE4E3F2070863 </node>"
	"			</node>"

	"			<node tag=\"0x1157\">"
	"				<node tag=\"0x9F06\"> A000000065 </node>"
	"				<node tag=\"0x9F22\"> EA </node>"
	"				<node tag=\"0x9F8123\"> A9796C29E39C2D44FDDAE7EE1341DA5461DCE4DCB31438D583B2BC0845B64AA37D055B190D7F5152E5057A5FB9CD27634EAC4003A2803C804E22D492738A164369A17F265F8016C622DA0631494F03B2DA4D5E7D13F7082F9BD8A7393B119AC70A39E861B645B1FBF29BA9CC1B8A5A97B5A8444DB0FCA5BC511E68E7B01D7ADCB8E46D9648A995E256F7715251B431B3 </node>"
	"				<node tag=\"0x9F8122\"> 03 </node>"
	"				<node tag=\"0x9F8121\"> 88F6D04BED35D6B43E697D765AE3293F649A961F </node>"
	"			</node>"

	"			<node tag=\"0x1158\">"
	"				<node tag=\"0x9F06\"> A000000065 </node>"
	"				<node tag=\"0x9F22\"> EB </node>"
	"				<node tag=\"0x9F8123\"> A9A696A7E3C6AC1421E4DE8936EFB6D66CE960EAFA6EA5E03D066F296284A61DA2890A6D0D869BA8C9A9E01E4EFF3BBE331ECE2CE1429C066DD885781E2ADABC86CFFD76854FB9F1BCDAEDCE6B54727D9C2C01C9642E9CD1BFC4CF24A6A2E49C1541B02EBC0534744481CC8922073A21F8E0D72BC8998BD529C698567A87F16450DFB969961A4186AB4CC648A4D41B149E3DD21393A993833D5EEEEBFCBCE0C777B52447ED3B816E3A2984930C07C021 </node>"
	"				<node tag=\"0x9F8122\"> 03 </node>"
	"				<node tag=\"0x9F8121\"> A19CAF700603E37BC7C2E5C420A11A56D1A8BAC3 </node>"
	"			</node>"

	"			<node tag=\"0x1159\">"
	"				<node tag=\"0x9F06\"> A000000065 </node>"
	"				<node tag=\"0x9F22\"> EC </node>"
	"				<node tag=\"0x9F8123\"> A9EDFDC58029A7EC003D13F22F6AED5622786D45F7C36516A3DBFE4D75BFCE00F4CF656670CD07A66A99A7CD35D2F5228CB2D794B95C4930FDDAD17F8C9293164AFEC876D5644DD31ABFE86B7AA512C58D5C71310FB36E8D7CCFF4C958669C0042DFF048F52E412B530C3BB77555B6F9B35E2C0F1B17A6180D03D94914B4970A42309F259DB37EC77FF6BA04BACF6B17FF7B10C1A04272D08C043A1C8E8951681DE41BE30F4E42D3ED3FE3328BD4C6327B19D110A2E85D9DC4C34225A2F0CA7684FF5C05C1F01135FC51D7331E3A413AED0942C8BBDB975104E171B08EE7C2B388EC4EA493BE5FCB0C416DF2A9DBBCDFA5D12344EC30576B </node>"
	"				<node tag=\"0x9F8122\"> 03 </node>"
	"				<node tag=\"0x9F8121\"> A15945946935956845ADB8ABE73E5B0BEEF76ECB </node>"
	"			</node>"

	"			<node tag=\"0x115A\">"
	"				<node tag=\"0x9F06\"> B012345678 </node>"
	"				<node tag=\"0x9F22\"> 00 </node>"
	"				<node tag=\"0x9F8123\"> 9C6BE5ADB10B4BE3DCE2099B4B210672B89656EBA091204F613ECC623BEDC9C6D77B660E8BAEEA7F7CE30F1B153879A4E36459343D1FE47ACDBD41FCD710030C2BA1D9461597982C6E1BDD08554B726F5EFF7913CE59E79E357295C321E26D0B8BE270A9442345C753E2AA2ACFC9D30850602FE6CAC00C6DDF6B8D9D9B4879B2826B042A07F0E5AE526A3D3C4D22C72B9EAA52EED8893866F866387AC05A1399 </node>"
	"				<node tag=\"0x9F8122\"> 03 </node>"
	"				<node tag=\"0x9F8121\"> 5D2970E64675727E60460765A8DB75342AE14783 </node>"
	"			</node>"

	"			<node tag=\"0x115B\">"
	"				<node tag=\"0x9F06\"> B012345678 </node>"
	"				<node tag=\"0x9F22\"> 02 </node>"
	"				<node tag=\"0x9F8123\"> A99A6D3E071889ED9E3A0C391C69B0B804FC160B2B4BDD570C92DD5A0F45F53E8621F7C96C40224266735E1EE1B3C06238AE35046320FD8E81F8CEB3F8B4C97B940930A3AC5E790086DAD41A6A4F5117BA1CE2438A51AC053EB002AED866D2C458FD73359021A12029A0C043045C11664FE0219EC63C10BF2155BB2784609A106421D45163799738C1C30909BB6C6FE52BBB76397B9740CE064A613FF8411185F08842A423EAD20EDFFBFF1CD6C3FE0C9821479199C26D8572CC8AFFF087A9C3 </node>"
	"				<node tag=\"0x9F8122\"> 03 </node>"
	"				<node tag=\"0x9F8121\"> 294BE20239AB15245A63BEA46CC6C175A25562D1 </node>"
	"			</node>"

	"			<node tag=\"0x115C\">"
	"				<node tag=\"0x9F06\"> B012345678 </node>"
	"				<node tag=\"0x9F22\"> 05 </node>"
	"				<node tag=\"0x9F8123\"> A1F5E1C9BD8650BD43AB6EE56B891EF7459C0A24FA84F9127D1A6C79D4930F6DB1852E2510F18B61CD354DB83A356BD190B88AB8DF04284D02A4204A7B6CB7C5551977A9B36379CA3DE1A08E69F301C95CC1C20506959275F41723DD5D2925290579E5A95B0DF6323FC8E9273D6F849198C4996209166D9BFC973C361CC826E1 </node>"
	"				<node tag=\"0x9F8122\"> 03 </node>"
	"				<node tag=\"0x9F8121\"> B9A1D65CAFE06B054EDD7EA82597AB85F130E663 </node>"
	"			</node>"

	"			<node tag=\"0x115D\">"
	"				<node tag=\"0x9F06\"> B012345678 </node>"
	"				<node tag=\"0x9F22\"> F5 </node>"
	"				<node tag=\"0x9F8123\"> A25A6BD783A5EF6B8FB6F83055C260F5F99EA16678F3B9053E0F6498E82C3F5D1E8C38F13588017E2B12B3D8FF6F50167F46442910729E9E4D1B3739E5067C0AC7A1F4487E35F675BC16E233315165CB142BFDB25E301A632A54A3371EBAB6572DEEBAF370F337F057EE73B4AE46D1A8BC4DA853EC3CC12C8CBC2DA18322D68530C70B22BDAC351DD36068AE321E11ABF264F4D3569BB71214545005558DE26083C735DB776368172FE8C2F5C85E8B5B890CC682911D2DE71FA626B8817FCCC08922B703869F3BAEAC1459D77CD85376BC36182F4238314D6C4212FBDD7F23D3 </node>"
	"				<node tag=\"0x9F8122\"> 03 </node>"
	"				<node tag=\"0x9F8121\"> F75E8802855C9B14027E517345717E5C3635B91B </node>"
	"			</node>"

	"			<node tag=\"0x115E\">"
	"				<node tag=\"0x9F06\"> B012345678 </node>"
	"				<node tag=\"0x9F22\"> F6 </node>"
	"				<node tag=\"0x9F8123\"> A1F5E1C9BD8650BD43AB6EE56B891EF7459C0A24FA84F9127D1A6C79D4930F6DB1852E2510F18B61CD354DB83A356BD190B88AB8DF04284D02A4204A7B6CB7C5551977A9B36379CA3DE1A08E69F301C95CC1C20506959275F41723DD5D2925290579E5A95B0DF6323FC8E9273D6F849198C4996209166D9BFC973C361CC826E1 </node>"
	"				<node tag=\"0x9F8122\"> 03 </node>"
	"				<node tag=\"0x9F8121\"> E9406B6510C143AB1E9B9D79A3C1DFF8909A347C </node>"
	"			</node>"

	"			<node tag=\"0x115F\">"
	"				<node tag=\"0x9F06\"> B012345678 </node>"
	"				<node tag=\"0x9F22\"> F7 </node>"
	"				<node tag=\"0x9F8123\"> 98F0C770F23864C2E766DF02D1E833DFF4FFE92D696E1642F0A88C5694C6479D16DB1537BFE29E4FDC6E6E8AFD1B0EB7EA0124723C333179BF19E93F10658B2F776E829E87DAEDA9C94A8B3382199A350C077977C97AFF08FD11310AC950A72C3CA5002EF513FCCC286E646E3C5387535D509514B3B326E1234F9CB48C36DDD44B416D23654034A66F403BA511C5EFA3 </node>"
	"				<node tag=\"0x9F8122\"> 03 </node>"
	"				<node tag=\"0x9F8121\"> F78113E860F030A872923FCE93E3381C77A42A30 </node>"
	"			</node>"

	"			<node tag=\"0x1160\">"
	"				<node tag=\"0x9F06\"> B012345678 </node>"
	"				<node tag=\"0x9F22\"> F8 </node>"
	"				<node tag=\"0x9F8123\"> A99A6D3E071889ED9E3A0C391C69B0B804FC160B2B4BDD570C92DD5A0F45F53E8621F7C96C40224266735E1EE1B3C06238AE35046320FD8E81F8CEB3F8B4C97B940930A3AC5E790086DAD41A6A4F5117BA1CE2438A51AC053EB002AED866D2C458FD73359021A12029A0C043045C11664FE0219EC63C10BF2155BB2784609A106421D45163799738C1C30909BB6C6FE52BBB76397B9740CE064A613FF8411185F08842A423EAD20EDFFBFF1CD6C3FE0C9821479199C26D8572CC8AFFF087A9C3 </node>"
	"				<node tag=\"0x9F8122\"> 03 </node>"
	"				<node tag=\"0x9F8121\"> 66469C88E7DC111529C7D379D7938C8DF3E4C25E </node>"
	"			</node>"

	"			<node tag=\"0x1161\">"
	"				<node tag=\"0x9F06\"> B012345678 </node>"
	"				<node tag=\"0x9F22\"> F9 </node>"
	"				<node tag=\"0x9F8123\"> A6E6FB72179506F860CCCA8C27F99CECD94C7D4F3191D303BBEE37481C7AA15F233BA755E9E4376345A9A67E7994BDC1C680BB3522D8C93EB0CCC91AD31AD450DA30D337662D19AC03E2B4EF5F6EC18282D491E19767D7B24542DFDEFF6F62185503532069BBB369E3BB9FB19AC6F1C30B97D249EEE764E0BAC97F25C873D973953E5153A42064BBFABFD06A4BB486860BF6637406C9FC36813A4A75F75C31CCA9F69F8DE59ADECEF6BDE7E07800FCBE035D3176AF8473E23E9AA3DFEE221196D1148302677C720CFE2544A03DB553E7F1B8427BA1CC72B0F29B12DFEF4C081D076D353E71880AADFF386352AF0AB7B28ED49E1E672D11F9 </node>"
	"				<node tag=\"0x9F8122\"> 010001 </node>"
	"				<node tag=\"0x9F8121\"> AEACA45480C8834CB0BEBDCC570B7B2B74BB4B79 </node>"
	"			</node>"
	"		</node> <!-- End node 1100 -->"
	""
	""
	"		<node tag=\"0x1200\">                   <!-- C_TAG_CAREVOK -->"
	"			<!-- KREVOK default values -->"
	""
	"			<node tag=\"0x1201\">"
	"				<node tag=\"0x9F06\"> A0 00 00 00 04 </node>"
	"				<node tag=\"0x9F22\"> F8 </node>"
	"				<node tag=\"0x9F8424\"> 00 10 00 </node>"
	"			</node> <!-- End -->"
	""
	"			<node tag=\"0x1202\">"
	"				<node tag=\"0x9F06\"> A0 00 00 00 04 </node>"
	"				<node tag=\"0x9F22\"> F7 </node>"
	"				<node tag=\"0x9F8424\"> 00 10 01 </node>"
	"			</node> <!-- End node -->"
	""
	"			<node tag=\"0x1203\">"
	"				<node tag=\"0x9F06\"> A0 00 00 00 05 </node>"
	"				<node tag=\"0x9F22\"> F6 </node>"
	"				<node tag=\"0x9F8424\"> 00 10 02 </node>"
	"			</node> <!-- End node -->"
	""
	"			<node tag=\"0x1204\">"
	"				<node tag=\"0x9F06\"> A0 00 00 00 04 </node>"
	"				<node tag=\"0x9F22\"> F5 </node>"
	"				<node tag=\"0x9F8424\"> 00 10 03 </node>"
	"			</node> <!-- End node -->"
	"		</node> <!-- End node -->"
	""
	""
	"		<node tag=\"0x1300\">                                <!-- C_TAG_ICS -->"
	""
	"			<node tag=\"0x1301\">"
	"				<!-- ICS0 values -->"
	"				<node tag=\"0x9f8450\"> 44 45 46 41 55 4c 54 20 43 4f 4e 46 49 47 </node> <!-- DEFAULT CONFIG -->"
	"				<node tag=\"0x9F35\"> 21 </node>"
	"				<node tag=\"0x9F33\"> 60 F8 C8 </node>"
	"				<node tag=\"0x9F1A\"> 05 04 </node>    <!-- Default country : France, currency : Euro -->"
	"				<node tag=\"0x5F2A\"> 05 04 </node>"
	"				<node tag=\"0x9F40\"> E0 00 F0 F0 01 </node>"
	"				<node tag=\"0x9F8142\"> 00 </node>"
	"				<node tag=\"0x9F8195\"> 00 </node>"
	"				<node tag=\"0x9F844B\"> 01 </node>"
	"				<node tag=\"0x9F8440\"> 00 </node>"
	"				<node tag=\"0x9F8441\"> 01 </node>"
	"				<node tag=\"0x9F8442\"> 01 </node>"
	"				<node tag=\"0x9F840A\"> 01 </node>"
	"				<node tag=\"0x9F844D\"> 01 </node>"
	"				<node tag=\"0x9F8443\"> 01 </node>"
	"				<node tag=\"0x9F8444\"> 01 </node>"
	"				<node tag=\"0x9F844C\"> 01 </node>"
	"				<node tag=\"0x9F840E\"> 01 </node>"
	"				<node tag=\"0x9F840F\"> 01 </node>"
	"				<node tag=\"0x9F840B\"> 00 </node>"
	"				<node tag=\"0x9F8445\"> 01 </node>"
	"				<node tag=\"0x9F8446\"> 00 </node>"
	"				<node tag=\"0x9F8447\"> 00 </node>"
	"				<node tag=\"0x9F8448\"> 00 </node>"
	"				<node tag=\"0x9F844E\"> 01 </node>"
	"				<node tag=\"0x9F39\"> 81 </node>"
	"				<node tag=\"0x9F8449\"> 00 </node>"
	"				<node tag=\"0x9F844A\"> 01 </node>"
	"				<node tag=\"0x9F8452\"> 01 </node>"
	"				<node tag=\"0x9F8453\"> 01 </node>"
	"				<node tag=\"0x9F8454\"> 01 </node>"
	"				<node tag=\"0x9F8455\"> 01 </node>"
	"				<node tag=\"0x9f8456\"> 01 </node>"
	"				<node tag=\"0x9F841E\"> 00 </node>"
	"				<node tag=\"0x9F845A\"> 01 </node>"
	"				<node tag=\"0x9F845B\"> 01 </node>"
	"				<node tag=\"0x9F8458\"> 01 </node>"
	"				<node tag=\"0x9F8459\"> 00 </node>"
	"				<node tag=\"0x9F845C\"> 01 </node>"
	"				<node tag=\"0x9F845D\"> 00 </node>"
	"				<node tag=\"0x9F845E\"> 00 </node>"
	"				<node tag=\"0x9F8431\"> 65 6e 66 72 </node> <!-- enfr -->"
	"				<node tag=\"0x9F8460\"> 01 </node>"
	"				<node tag=\"0x9F8464\"> 00 </node>"
	"				<node tag=\"0x9F8465\"> 00 </node>"
	"				<node tag=\"0x9F8466\"> 00 </node>"
	"				<node tag=\"0x9F8468\"> 00 </node>"
	"				<node tag=\"0x9F8469\"> 01 20 </node>"
	"				<node tag=\"0x9F846A\"> 01 </node>"
	"				<node tag=\"0x9F846B\"> 00 </node>"
	"			</node> <!-- End node 9F8B91 -->"
	""
#if 0
	"			<node tag=\"0x1302\">"
	"				<!-- ICS1 values -->"
	"				<node tag=\"0x9f8450\"> 44 45 46 41 55 4c 54 20 34 4f 4e 46 49 47 </node> <!-- DEFAULT CONFIG -->"
	"				<node tag=\"0x9F35\"> 21 </node>"
	"				<node tag=\"0x9F33\"> 60 F8 C8 </node>"
	"				<node tag=\"0x9F1A\"> 05 04 </node>    <!-- Default country : France, currency : Euro -->"
	"				<node tag=\"0x5F2A\"> 05 04 </node>"
	"				<node tag=\"0x9F40\"> E0 00 F0 F0 01 </node>"
	"				<node tag=\"0x9F8142\"> 01 </node>"
	"				<node tag=\"0x9F8195\"> 00 </node>"
	"			</node> <!-- End node -->"
	""
	"			<node tag=\"0x1303\">"
	"				<!-- ICS2 values -->"
	"				<node tag=\"0x9f8450\"> 44 45 46 41 55 4c 54 20 34 4f 4e 46 49 47 </node> <!-- DEFAULT CONFIG -->"
	"				<node tag=\"0x9F35\"> 21 </node>"
	"				<node tag=\"0x9F33\"> 60 F8 C8 </node>"
	"				<node tag=\"0x9F1A\"> 05 04 </node>    <!-- Default country : France, currency : Euro -->"
	"				<node tag=\"0x5F2A\"> 05 04 </node>"
	"				<node tag=\"0x9F40\"> E0 00 F0 F0 01 </node>"
	"				<node tag=\"0x9F8142\"> 01 </node>"
	"				<node tag=\"0x9F8195\"> 00 </node>"
	"				<node tag=\"0x9F844B\"> 01 </node>"
	"				<node tag=\"0x9F8440\"> 00 </node>"
	"			</node> <!-- End node -->"
	""
	"			<node tag=\"0x1304\">"
	"				<!-- ICS3 values -->"
	"				<node tag=\"0x9f8450\"> 44 45 46 41 55 4c 54 20 34 4f 4e 46 49 47 </node> <!-- DEFAULT CONFIG -->"
	"				<node tag=\"0x9F35\"> 21 </node>"
	"				<node tag=\"0x9F33\"> 60 F8 C8 </node>"
	"				<node tag=\"0x9F1A\"> 05 04 </node>    <!-- Default country : France, currency : Euro -->"
	"				<node tag=\"0x5F2A\"> 05 04 </node>"
	"				<node tag=\"0x9F40\"> E0 00 F0 F0 01 </node>"
	"				<node tag=\"0x9F8142\"> 00 </node>"
	"				<node tag=\"0x9F8195\"> 00 </node>"
	"				<node tag=\"0x9F844B\"> 01 </node>"
	"				<node tag=\"0x9F8440\"> 00 </node>"
	"				<node tag=\"0x9F8459\"> 00 </node>"
	"			</node> <!-- End node -->"
	""
	"			<node tag=\"0x1305\">"
	"				<!-- ICS4 values -->"
	"				<node tag=\"0x9f8450\"> 44 45 46 41 55 4c 54 20 34 4f 4e 46 49 47 </node> <!-- DEFAULT CONFIG -->"
	"				<node tag=\"0x9F35\"> 21 </node>"
	"				<node tag=\"0x9F33\"> 60 F8 C8 </node>"
	"				<node tag=\"0x9F1A\"> 05 04 </node>    <!-- Default country : France, currency : Euro -->"
	"				<node tag=\"0x5F2A\"> 05 04 </node>"
	"				<node tag=\"0x9F40\"> E0 00 F0 F0 01 </node>"
	"				<node tag=\"0x9F8142\"> 01 </node>"
	"				<node tag=\"0x9F8195\"> 00 </node>"
	"				<node tag=\"0x9F844B\"> 01 </node>"
	"				<node tag=\"0x9F8440\"> 00 </node>"
	"				<node tag=\"0x9F8441\"> 01 </node>"
	"			</node> <!-- End node -->"
#endif /* 0 */
	"		</node> <!-- End node -->"
	"	</node>																													    "
	"</tlvtree>"
};

const char DefaultParamEval[] =
{
	"	<?xml version=\"1.0\" standalone=\"yes\" ?>"
	"	<!-- Fichier de parametres base -->"
	""
	"	<tlvtree ver=\"1.0\">"
	""
	"	<node tag=\"0x0\">"
	"		<node tag=\"0x1000\">                                <!-- C_TAG_AID -->"
	"			<node tag=\"0x1001\">"
	"				<!-- AID1_EVAL file content -->"
	"				<node tag=\"0x9FFF00\"> 41 49 44 31 </node>    <!--     AID1_EVAL Parameters -->"
	"				<node tag=\"0x9F06\"> 07 B0 12 34 56 78 12 34 </node>"
	"				<node tag=\"0x9F1A\"> 05 04 </node>"
	"				<node tag=\"0x5F2A\"> 05 04 </node>"
	"				<node tag=\"0x5F36\"> 02 </node>"
	"				<node tag=\"0x9F812B\"> 00 00 01 F4 </node>"
	"				<node tag=\"0x9F8124\"> 9F 37 04 9F 47 01 8F 01 9F 32 01 </node>"
	"				<node tag=\"0x9F8125\"> 9F 08 02 </node>"
	"				<node tag=\"0x9F8126\"> 00 </node>"
	"				<node tag=\"0x9F8127\"> 00 </node>"
	"				<node tag=\"0x9F8128\"> 00 00 00 00 00 </node>"
	"				<node tag=\"0x9F8129\"> 00 00 00 00 00 </node>"
	"				<node tag=\"0x9F812A\"> 00 00 00 00 00 </node>"
	"				<node tag=\"0x9F09\"> 00 02 </node>"
	"				<node tag=\"0x9F1B\"> 00 00 27 10 </node>"
	"				<node tag=\"0x9F841D\"> 00 </node>"
	"			</node>"
	""
	"			<node tag=\"0x1002\">"
	"				<!-- AID2_EVAL.0 file content -->"
	"				<node tag=\"0x9FFF00\"> 4D 61 73 74 65 72 43 61 72 64 </node>"
	"				<node tag=\"0x9F06\"> 07 A0 00 00 00 04 10 10 </node>"
	"				<node tag=\"0x9F1A\"> 05 04 </node>"
	"				<node tag=\"0x5F2A\"> 05 04 </node>"
	"				<node tag=\"0x5F36\"> 02 </node>"
	"				<node tag=\"0x9F812B\"> 00 00 01 F4 </node>"
	"				<node tag=\"0x9F8124\"> 9F 37 04 9F 47 01 8F 01 9F 32 01 </node>"
	"				<node tag=\"0x9F8125\"> 9F 08 02 </node>"
	"				<node tag=\"0x9F8126\"> 00 </node>"
	"				<node tag=\"0x9F8127\"> 00 </node>"
	"				<node tag=\"0x9F8128\"> 00 00 00 00 00 </node>"
	"				<node tag=\"0x9F8129\"> 00 00 00 00 00 </node>"
	"				<node tag=\"0x9F812A\"> 00 00 00 00 00 </node>"
	"				<node tag=\"0x9F09\"> 00 02 </node>"
	"				<node tag=\"0x9F1B\"> 00 00 27 10 </node>"
	"				<node tag=\"0x9F841D\"> 00 </node>"
	"			</node>"
	""
	"			<node tag=\"0x1003\">"
	"				<!-- AID3.0 EVAL file content. Settings done according to Visa 1.4.0 -->"
	"				<node tag=\"0x9FFF00\"> 41 49 44 33 </node>"
	"				<node tag=\"0x9F06\"> 05 A0 00 00 00 10 </node>"
	"				<node tag=\"0x9F1A\"> 05 04 </node>"
	"				<node tag=\"0x5F2A\"> 05 04 </node>"
	"				<node tag=\"0x5F36\"> 02 </node>"
	"				<node tag=\"0x9F812B\"> 00 00 01 F4 </node>"
	"				<node tag=\"0x9F8124\"> 9F 37 04 9F 47 01 8F 01 9F 32 01 </node>"
	"				<node tag=\"0x9F8125\"> 9F 08 02 </node>"
	"				<node tag=\"0x9F8126\"> 00 </node>"
	"				<node tag=\"0x9F8127\"> 00 </node>"
	"				<node tag=\"0x9F8128\"> 00 00 00 00 00 </node>"
	"				<node tag=\"0x9F8129\"> 00 00 00 00 00 </node>"
	"				<node tag=\"0x9F812A\"> 00 00 00 00 00 </node>"
	"				<node tag=\"0x9F09\"> 00 02 </node>"
	"				<node tag=\"0x9F1B\"> 00 00 27 10 </node>"
	"				<node tag=\"0x9F841D\"> 01 </node>"
	"			</node> <!-- End node 9F8B03 -->"
	"		</node>"
	"	</node>"
	"</tlvtree>		<!-- End -->"
};

const char DefaultParamVisa[] =
{
	"	<?xml version=\"1.0\" standalone=\"yes\" ?>"
	"	<!-- Fichier de parametres base -->"
	""
	"	<tlvtree ver=\"1.0\">"
	""
	"	<node tag=\"0x0\">"
	"		<node tag=\"0x1000\">                                <!-- C_TAG_AID -->"
	""
	"			<node tag=\"0x1001\">"
	"				<!-- AID1 VISA file content -->"
	"				<node tag=\"0x9FFF00\"> 56 53 44 43 </node>               <!-- VSDC -->"
	"				<node tag=\"0x9F06\"> 07 A0 00 00 00 03 10 10 </node>"
	"				<node tag=\"0x9F1A\"> 05 04 </node>"
	"				<node tag=\"0x5F2A\"> 05 04 </node>"
	"				<node tag=\"0x5F36\"> 02 </node>"
	"				<node tag=\"0x9F812B\"> 00 00 00 40 </node>"
	"				<node tag=\"0x9F8125\"> 9F 02 06 5F 2A 02 9A 03 9C 01 95 05 9F 37 04 </node>"
	"				<node tag=\"0x9F8126\"> 50 </node>"
	"				<node tag=\"0x9F8127\"> 20 </node>"
	"				<node tag=\"0x9F8128\"> 00 00 00 00 00 </node>"
	"				<node tag=\"0x9F8129\"> 00 00 00 00 00 </node>"
	"				<node tag=\"0x9F812A\"> 00 00 00 00 00 </node>"
	"				<node tag=\"0x9F01\"> 45 00 00 00 00 00 </node>"
	"				<node tag=\"0x9F09\"> 00 8C </node>"
	"				<node tag=\"0x9F1B\"> 00 00 27 10 </node>"
	"				<node tag=\"0x9F16\"> 01 20 00 00 00 00 00 00 </node>"
	"				<node tag=\"0x9F15\"> 61 11 </node>"
	"				<node tag=\"0x9F7A\"> 01 </node>"
	"				<node tag=\"0x9F841D\"> 01 </node>"
	"			</node>"
	""
	"			<node tag=\"0x1002\">"
	"				<!-- AID2 file content -->"
	"				<node tag=\"0x9FFF00\"> 4D 61 65 73 74 72 6F </node>    <!-- MaestroParameters -->"
	"				<node tag=\"0x9F06\"> 07 A0 00 00 00 99 90 90 </node>"
	"				<node tag=\"0x9F1A\"> 05 04 </node>"
	"				<node tag=\"0x5F2A\"> 05 04 </node>"
	"				<node tag=\"0x5F36\"> 02 </node>"
	"				<node tag=\"0x9F812B\"> 00 00 00 05 </node>"
	"				<node tag=\"0x9F8124\"> 9F 37 04 9F 47 01 8F 01 9F 32 01 </node>"
	"				<node tag=\"0x9F8125\"> 9F 08 02 </node>"
	"				<node tag=\"0x9F8126\"> 00 </node>"
	"				<node tag=\"0x9F8127\"> 00 </node>"
	"				<node tag=\"0x9F8128\"> 00 00 00 00 00 </node>"
	"				<node tag=\"0x9F8129\"> 00 00 00 00 00 </node>"
	"				<node tag=\"0x9F812A\"> 00 00 00 00 00 </node>"
	"				<node tag=\"0x9F09\"> 00 02 </node>"
	"				<node tag=\"0x9F1B\"> 00 00 27 10 </node>"
	"				<node tag=\"0x9F841D\"> 01 </node>"
	"			</node>"
	""
	"			<node tag=\"0x1003\">"
	"				<!-- AID3 file content -->"
	"				<node tag=\"0x9FFF00\"> 41 49 44 33 </node>    <!-- VisaElectronParameters -->"
	"				<node tag=\"0x9F06\"> 07 A0 00 00 00 03 20 10 </node>"
	"				<node tag=\"0x9F1A\"> 05 04 </node>"
	"				<node tag=\"0x5F2A\"> 05 04 </node>"
	"				<node tag=\"0x5F36\"> 02 </node>"
	"				<node tag=\"0x9F812B\"> 00 00 00 05 </node>"
	"				<node tag=\"0x9F8124\"> 9F 37 04 9F 47 01 8F 01 9F 32 01 </node>"
	"				<node tag=\"0x9F8125\"> 9F 02 06 5F 2A 02 9A 03 9C 01 95 05 9F 37 04 </node>"
	"				<node tag=\"0x9F8126\"> 00 </node>"
	"				<node tag=\"0x9F8127\"> 00 </node>"
	"				<node tag=\"0x9F8128\"> 00 00 00 00 00 </node>"
	"				<node tag=\"0x9F8129\"> 00 00 00 00 00 </node>"
	"				<node tag=\"0x9F812A\"> 00 00 00 00 00 </node>"
	"				<node tag=\"0x9F09\"> 01 40 </node>"
	"				<node tag=\"0x9F1B\"> 00 00 27 10 </node>"
	"				<node tag=\"0x9F841D\"> 01 </node>"
	"			</node>"
	""
	"			<node tag=\"0x1004\">"
	"				<!-- AID4 file content -->"
	"				<node tag=\"0x9FFF00\"> 41 49 44 33 </node>"
	"				<node tag=\"0x9F06\"> 07 A0 00 00 00 03 20 10 </node>"
	"				<node tag=\"0x9F1A\"> 05 04 </node>"
	"				<node tag=\"0x5F2A\"> 05 04 </node>"
	"				<node tag=\"0x5F36\"> 02 </node>"
	"				<node tag=\"0x9F812B\"> 00 00 00 05 </node>"
	"				<node tag=\"0x9F8124\"> 9F 37 04 9F 47 01 8F 01 9F 32 01 </node>"
	"				<node tag=\"0x9F8125\"> 9F 02 06 5F 2A 02 9A 03 9C 01 95 05 9F 37 04 </node>"
	"				<node tag=\"0x9F8126\"> 00 </node>"
	"				<node tag=\"0x9F8127\"> 00 </node>"
	"				<node tag=\"0x9F8128\"> 00 00 00 00 00 </node>"
	"				<node tag=\"0x9F8129\"> 00 00 00 00 00 </node>"
	"				<node tag=\"0x9F812A\"> 00 00 00 00 00 </node>"
	"				<node tag=\"0x9F09\"> 01 40 </node>"
	"				<node tag=\"0x9F1B\"> 00 00 27 10 </node>"
	"				<node tag=\"0x9F841D\"> 01 </node>"
	"			</node> <!-- End node -->"
	"		</node> <!-- End node -->"
	"	</node>"
	"</tlvtree>	<!-- End -->"
};

#endif /* FILE_H */
/* ======================================================================== */
/* Global variables                                                         */
/* ======================================================================== */

/* ======================================================================== */
/* Global function declarations                                             */
/* ======================================================================== */

//===========================================================================
//! \brief This function initializes aidX.par cakeys.par krevok.par or param.par
//! files with default values.
//! \param[in] y_x_PrintAID : TRUE if supported AIDs may be printed.
//! \return TRUE if successfull.
//! \note pTreeDefault must have been updated.
//===========================================================================
int CUPAR_InitDefFiles (unsigned char y_x_PrintAID);

//===========================================================================
//! \brief This function performs the initialisation of the parameters.
//! \param[in] uc_x_InitType : initialisation type. It can take the following values :
//! - C_PARAM_APPLI : reset TLV tree used to store specific application tags.
//! - C_PARAM_INIT : read default parameters values and param.par file
//! and build two TLV trees. Reset the marks array used to manage the activation.
//! - C_PARAM_DEFAULT : read default parameters values and update one TLV tree.
//! Don't touch the marks array used to manage the activation.
//! - C_PARAM_FILE_TLVTSERIAL : read the param.par file and update one TLV tree.
//! Don't touch the marks array used to manage the activation.
//! - C_PARAM_FILE_XML : read the param.par (xml format) file and update one TLV tree.
//! Don't touch the marks array used to manage the activation.
//! \param[in] pc_x_DirFile : file path and name used for param.par file.
//! \return
//! - 0 if successfull.
//! - Error code, otherwise.
//! \note This function allows to select a main branch of the tree.
//! It corresponds to choose one configuration.
//! The next mark selects will be performed in the part of the tree choosen.
//===========================================================================
int PAR_InitParam(unsigned char uc_x_InitType, char *pc_x_DirFile);

//===========================================================================
//! \brief This function reads the parameters from aidX.par, cakeys.par,
//! krevok.par, ics.par, or param.par file and stores data in a TLV tree.
//! \param[in] pc_x_DirFile : file path and file name.
//! \param[in] pc_x_fileName : name of the parameter file.
//! \return TLVTree node.
//===========================================================================
TLV_TREE_NODE CUPAR_FileRead(char *pc_x_DirFile, char *pc_x_fileName);

//===========================================================================
//! \brief This function updates the output file(s) (serial TLVTree binary format)
//! with pTreeParam TLVTree informations.
//! \param[out] ptr_OutBuf : binary file (serial TVLTree binary format).
//! \return
//! - 0 if successfull.
//! - 1 otherwise.
//===========================================================================
int CUPAR_Update_Param_File(char * ptr_OutBuf);

//===========================================================================
//! \brief This function inits the ICS structure.
//! \return
//! - TRUE if all ICS parameters are stored.
//! - FALSE otherwise.
//===========================================================================
int CUPAR_InitICS (void);

//===========================================================================
//! \brief This function gives the key module and index.
//! \param[out] AidValue : card AID.
//! \param[in] Index : certification Authority Public Key Index.
//! \param[out] Modulus : key module.
//! \param[out] ModulusLength : module length.
//! \param[out] Exponent : key exponent.
//! \param[out] ExponentLength : exponent length.
//! \return
//! - TRUE if successfull.
//! - FALSE otherwise.
//===========================================================================
int CUPAR_GiveKey (unsigned char  *AidValue,unsigned char  Index,unsigned char  *Modulus,unsigned short *ModulusLength,unsigned char  *Exponent,unsigned short *ExponentLength);

//===========================================================================
//! \brief This function restores the parameter file. If param.par file doesn't
//! exist, the function tries to build it with other files : AIDx.par, KREVOK.PAR, ...
//! \param[in] uc_x_FileMgtType : PARAM.PAR or other parameters files indicator
//! management.
//===========================================================================
void CUPAR_RestoreParam (unsigned char uc_x_FileMgtType);

//===========================================================================
//! \brief This function gives the AID list from the parameter structure.
//! \param[out] nb_aid : number of AID found.
//! \param[out] tab_AID : array updated with AID informations.
//! \return
//! - TRUE if successfull.
//! - FALSE otherwise.
//===========================================================================
int CUPAR_GiveAIDList(unsigned char *nb_aid, T_CUDB_AID_Elt *tab_AID);

//===========================================================================
//! \brief This function gives the AID list from the parameter structure.
//! \param[out] nb_aid : number of AID found.
//! \param[out] phOutputTLVTree : output TLV tree containing AID informations.
//! \return
//! - TRUE if successfull.
//! - FALSE otherwise.
//===========================================================================
int CUPAR_GiveAIDListTlvTree(unsigned char *nb_aid, TLV_TREE_NODE *phOutputTLVTree);

//===========================================================================
//! \brief This function copies a file in a specific area.
//! \param[in] DirFile : file directory.
//! \param[in] fileName : file name.
//===========================================================================
void CUPAR_FileCopy(char *DirFile, char *fileName);

//===========================================================================
//! \brief This function gives all revoked Certificates matching the AID given
//! in input parameter.
//! \param[in] AidValue : card AID.
//! \param[out] CA_revok : structure with all revoked CA.
//! \return
//! - TRUE if successfull.
//! - FALSE otherwise.
//===========================================================================
int CUPAR_GiveRevokedCA (unsigned char *AidValue, T_CUSERV_lst_tagCArevok *CA_revok);

//===========================================================================
//! \brief This function inits the blacklist file with the one given as parameter.
//! \param[in] fileName : new black list file name.
//! \return
//! - TRUE if init successfull.
//! - FALSE otherwise.
//===========================================================================
T_Bool CUPAR_BlackListInit (char *fileName);

//===========================================================================
//! \brief This function prints the current ICS.
//! \return
//! - TRUE if successfull.
//! - FALSE otherwise.
//===========================================================================
int CUPAR_Print_ICS_Config(void);

//===========================================================================
//! \brief This function gets the value of the tag, given as input parameter,
//! in the EMV configuration.
//! \param[in] tag : parameter tag to get.
//! \param[out] value : it can take the following values :
//! - value of the tag in text format.
//! - "YES" or "NO" if the tag is binary compatible.
//! - "NONE" if the tag isn't reachable
//! \return Always TRUE.
//===========================================================================
int CUPAR_Get_Param(unsigned long tag, unsigned char *value);

//===========================================================================
//! \brief This function retreives informations (Version and CRC) about kernel
//! and system.
//! \return Always TRUE.
//===========================================================================
int CUPAR_Update_Kernel_and_System_Infos (void);

//===========================================================================
//! \brief This function saves a list of parameters in the global database.
//===========================================================================
void CUPAR_UpdateGlobalParam(void);

//===========================================================================
//! \brief This function control the key parameters (sha...).
//! \return
//! - TRUE if all the keys are OK.
//! - FALSE otherwise.
//===========================================================================
int CUPAR_ControlCAKEYS (void);


#ifdef _TOOLWARE_
//===========================================================================
//! \brief This function checks if the system and the kernel support ToolWare
//! \return Always TRUE.
//===========================================================================
int CUPAR_Init_ToolWare(void);
#endif

#ifdef _USE_UCM_

//===========================================================================
//! \brief This function sets the com 0 for the EMV tool.
//===========================================================================
void SetOutputCom0(void);


//===========================================================================
//! \brief This function performs a service call to EMV tool to get the tags,
//! and to get the trace of the last transaction.
//! \return Always OK.
//===========================================================================
int DumpTags(void);

#endif /* _USE_UCM_ */
//===========================================================================
//! \brief This function resets ICS mark parameter in relation with selected AID.
//===========================================================================
void CUPAR_ResetAdditionnalICS(void);

//===========================================================================
//! \brief This function memorizes ICS mark parameter in relation with selected AID.
//! \param[in] ul_x_ICSMark : ICS mark parameter in relation with AID.
//===========================================================================
void CUPAR_MemorizeAdditionnalICS(unsigned long  ul_x_ICSMark);

//===========================================================================
//! \brief This function selects ICS mark parameter in relation with selected AID.
//! \return
//! - TRUE if mark has been found and selected.
//! - FALSE otherwise.
//===========================================================================
int  CUPAR_SelectAdditionnalICS(void);

//===========================================================================
//! \brief This function resets mark parameter in relation with selected AID.
//===========================================================================
void CUPAR_ResetAdditionnalAID(void);

//===========================================================================
//! \brief This function memorizes mark parameter in relation with selected AID.
//! \param[in] ul_x_ICSMark : mark parameter.
//===========================================================================
void CUPAR_MemorizeAdditionnalAID(unsigned long  ul_x_ICSMark);

//===========================================================================
//! \brief This function selects parameter in relation with selected AID.
//! \return
//! - TRUE if mark was found and selected.
//! - FALSE otherwise.
//===========================================================================
int CUPAR_SelectAdditionnalAID(void);

//===========================================================================
//! \brief This function resets mark parameter in relation with selected KRevok.
//===========================================================================
void CUPAR_ResetAdditionnalKRevok(void);

//===========================================================================
//! \brief This function memorizes mark parameter in relation with selected KRevok.
//! \param[in] ul_x_KRevokMark : mark parameter.
//===========================================================================
void CUPAR_MemorizeAdditionnalKRevok(unsigned long  ul_x_KRevokMark);

//===========================================================================
//! \brief This function selects parameter in relation with selected KRevok.
//! \return
//! - TRUE if mark has been found and selected.
//! - FALSE otherwise.
//===========================================================================
int CUPAR_SelectAdditionnalKRevok(void);

//! @}

#endif /* CU_PARAM_H */
