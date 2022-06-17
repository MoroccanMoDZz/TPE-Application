/**
* \file cu_pinen.c
* \brief This module contains the functions that perform the encipherment of the PIN code,
* for online presentation.
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
#include "sec_interface.h"
#include "cu_term.h"
#include "tlvVar_def.h"
#include "schVar_def.h"

/* ======================================================================== */
/* Macros & preprocessor definitions                                        */
/* ======================================================================== */

/* ========================================================================	*/
/* Global variables                                                         */
/* ========================================================================	*/

T_SEC_DATAKEY_ID stKey;

/* ======================================================================== */
/* Functions                                                                */
/* ======================================================================== */
//===========================================================================




//===========================================================================
int PIN_OnlineEncipher (unsigned char pucPAN [],	unsigned char pucEncipheredPin [],int *iLenEnc)
{
	int iRes;
	int iAllIsOk;
	char debug[50];
	unsigned char       iso9564Result       [50];
	unsigned int uiLengthOut;


	memset ( iso9564Result , 0 , 50);	

	// cyphering key to use initialisation 
	stKey.iSecretArea = 0x00012030;
	stKey.cAlgoType = TLV_TYPE_KTDES;
	stKey.usNumber = 16*4;
	stKey.uiBankId = 0x80000000;
		
	iRes = SEC_Iso9564 (C_SEC_PINCODE, &stKey, ISO9564_F0_TDES, &(pucPAN[0]), &iso9564Result[0], &uiLengthOut);

	sprintf(debug, "SEC_Iso9564 %d\n length=%u\n",iRes, uiLengthOut);
	upload_diagnostic_txt((unsigned char*)debug);
	sprintf(debug, "SEC_Iso9564 %d", iRes);
	trace(SAP_KERNEL_TRAP_RESET, strlen(debug), debug);
	sprintf(debug, " length=%u", uiLengthOut);
	trace(SAP_KERNEL_TRAP_RESET, strlen(debug), debug);

	sprintf(debug, " %02X%02X%02X%02X%02X%02X%02X%02X\n",iso9564Result[0],iso9564Result[1],iso9564Result[2],
		iso9564Result[3],iso9564Result[4],iso9564Result[5],iso9564Result[6],iso9564Result[7]);
	upload_diagnostic_txt((unsigned char*)debug);
	sprintf(debug, " %02X%02X%02X%02X%02X%02X%02X%02X",iso9564Result[0],iso9564Result[1],iso9564Result[2],
		iso9564Result[3],iso9564Result[4],iso9564Result[5],iso9564Result[6],iso9564Result[7]);
	trace(SAP_KERNEL_TRAP_RESET, strlen(debug), debug);

	iAllIsOk = (iRes == OK);
	if (iAllIsOk)
	{
		memcpy (pucEncipheredPin, iso9564Result, uiLengthOut);
		*iLenEnc = uiLengthOut;
	}

	return (iAllIsOk);
}

#ifdef _USE_DEMO_
//===========================================================================
int PIN_OnlineEncipherDemo (unsigned char pucPAN [],	unsigned char pucEncipheredPin [],int *iLenEnc)
{
	*iLenEnc = 8;
	return 1;
}

//===========================================================================
void PIN_InjectKeyDemo (void)
{

}
#endif


