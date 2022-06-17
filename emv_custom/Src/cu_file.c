/**
* \file cu_file.c
* \brief This module contains the functions allowing the saving of global variables, in
* case of power failure. The maximum allowed size for saving is 8 Ko.
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
#include "_emvdctag_.h"
#include "cu_term.h"
#include "MyTlvTree.h"
#include "cu_batch.h"
#define FILE_H
#include "cu_file.h"

/* ======================================================================== */
/* Macros & preprocessor definitions                                        */
/* ======================================================================== */

/* =========================================================================*/
/* Types declarations                                                       */
/* =========================================================================*/

/* ======================================================================== */
/* Constant declarations                                                    */
/* ======================================================================== */

/* ========================================================================	*/
/* Global variables                                                         */
/* ========================================================================	*/
//T_Global_Data_File S_GlobalRamData1;
/* ======================================================================== */
/* Functions                                                                */
/* ======================================================================== */
void StoreGlobalData(void);
void ReadGlobalData(void);
//===========================================================================

void FILE_Save_Restore_global_RAM_data(void)
{

	// Request save / recovery of global data 
	//result = RegisterPowerFailure ((char *) LABEL_GLOBAL_DATA, (unsigned char *)&S_GlobalRamData,
	//								sizeof(STRUCT_GLOBAL_RAM_DATA));

	ReadGlobalData();
	memset (&S_GlobalRamData1,0,sizeof(T_Global_Data_File));
}

//===========================================================================

#define LABEL_DISK_GD "E_GLOBAL_VAR"
#define TAILLE_DISK_GD 0x50
#define LABEL_DATA_FILE "DATA"

int cu_CreateOrMountDisk (void)
{
  int               Retour;
  unsigned long     TailleDisk;
  S_FS_PARAM_CREATE ParamCreat;
  char              FilePath_L[33];

	ParamCreat.Mode         = FS_WRITEONCE;
	ParamCreat.IdentZone    = FS_WO_ZONE_DATA;

	strcpy(ParamCreat.Label, LABEL_DISK_GD);
	ParamCreat.AccessMode   = FS_WRTMOD;
	ParamCreat.NbFichierMax = 20;
	TailleDisk              = TAILLE_DISK_GD;

	sprintf(FilePath_L,"/%s",LABEL_DISK_GD);
	Retour = FS_mount (FilePath_L,&ParamCreat.Mode);

	if (Retour != FS_OK)
	{
		Retour=FS_dskcreate(&ParamCreat,&TailleDisk);
	}
   return (Retour);
}

void  ReadGlobalData(void)
{
S_FS_FILE 		*file1;
char             FilePath_L[33];
int ret;


	ret = cu_CreateOrMountDisk ();
	if (ret == FS_OK)
	{
		memclr(FilePath_L,sizeof(FilePath_L));
		sprintf(FilePath_L, "/%s/%s",LABEL_DISK_GD,LABEL_DATA_FILE);
		file1 = FS_open (FilePath_L, "r");

		if (file1 != NULL)
		{
			FS_read (&S_GlobalRamData1, sizeof(T_Global_Data_File), 1, file1);
		}
		FS_close (file1);
	}
}

void StoreGlobalData(void)
{
S_FS_FILE 		*file1;
char              FilePath_L[33];
int ret;

	ret = cu_CreateOrMountDisk ();
	if (ret == FS_OK)
	{
		memclr(FilePath_L,sizeof(FilePath_L));
		sprintf(FilePath_L, "/%s/%s",LABEL_DISK_GD,LABEL_DATA_FILE);
		ret = FS_exist (FilePath_L);

		if (ret == FS_OK)
		{
			ret = FS_unlink (FilePath_L);
		}
		file1 = FS_open (FilePath_L, "a");

		if (file1 != NULL)
		{
			FS_write(&S_GlobalRamData1, sizeof(T_Global_Data_File), 1, file1);
			FS_close(file1);
		}
	}
}
