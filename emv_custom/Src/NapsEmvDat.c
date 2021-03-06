#include "SDK_Naps.h"
#include "_emvdctag_.h"

#include "MyTlvTree.h"
#include "convert.h"
#include "gestion_param.h"

#define TAG_LIST_BUFFER_SIZE	1536

#define MSBYTE_PASTE        0x1F00
#define MSBYTE_PARSE        0x1F
#define LSBYTE              0xFF
#define TWO_BYTES_LENGTH    0x82
#define ONE_BYTE_LENGTH     0x81


uchar   	l_retour;


L APEMV_UI_MessageDisplayDelayHexa(char* Messagehexa,char lc, int duration);

//****************************************************************************/
//****************************************************************************/
//
//			NOM			:
//			DESCRIPTION	:
//****************************************************************************/
//
//			ENTREE		:
//			SORTIE		:
//			FCT APP		:
//			REMARQUE	:
//****************************************************************************/
//****************************************************************************/
/*L TagListInit(void)
{
	g_puc_tag_list = K_Malloc(TAG_LIST_BUFFER_SIZE, 200);
	if ((L)g_puc_tag_list <= 0)
	{
		g_puc_tag_list = NULL;
		return CT_ERR_NOT_ENOUGH_MEMORY;
	}

	memset(g_puc_tag_list, 0, TAG_LIST_BUFFER_SIZE);
	g_ul_tag_list_size = 0;
	g_ul_tag_list_buffer_size = TAG_LIST_BUFFER_SIZE;

	return CT_ERR_SUCCESS;
}*/

//****************************************************************************/
//****************************************************************************/
//
//			NOM			:
//			DESCRIPTION	:
//****************************************************************************/
//
//			ENTREE		:
//			SORTIE		:
//			FCT APP		:
//			REMARQUE	:
//****************************************************************************/
//****************************************************************************/
/*void TagListFree(void)
{
	if (g_puc_tag_list != NULL)
		K_Free(g_puc_tag_list);

	g_ul_tag_list_size = 0;
	g_ul_tag_list_buffer_size = 0;
}*/

//****************************************************************************/
//****************************************************************************/
//
//			NOM			:
//			DESCRIPTION	:
//****************************************************************************/
//
//			ENTREE		:
//			SORTIE		:
//			FCT APP		:
//			REMARQUE	:
//****************************************************************************/
//****************************************************************************/
/*L TagListGrow(UL i_ul_grow)
{
	PUC l_puc_buffer;
	UL l_ul_new_size;

	l_ul_new_size = (((g_ul_tag_list_size+i_ul_grow) / TAG_LIST_BUFFER_SIZE) + 1) * TAG_LIST_BUFFER_SIZE;

	l_puc_buffer = K_Malloc(l_ul_new_size, 200);
	if ((L)l_puc_buffer <= 0)
		return CT_ERR_NOT_ENOUGH_MEMORY;

	memcpy(l_puc_buffer, g_puc_tag_list, g_ul_tag_list_size);
	memset(l_puc_buffer+g_ul_tag_list_size, 0, l_ul_new_size-g_ul_tag_list_size);

	K_Free(g_puc_tag_list);
	g_puc_tag_list = l_puc_buffer;
	g_ul_tag_list_buffer_size = l_ul_new_size;

	return CT_ERR_SUCCESS;
}
*/
//****************************************************************************/
//****************************************************************************/
//
//			NOM			:
//			DESCRIPTION	:
//****************************************************************************/
//
//			ENTREE		:
//			SORTIE		:
//			FCT APP		:
//			REMARQUE	:
//****************************************************************************/
//****************************************************************************/
/*L TagListIsPresent(US i_us_tag)
{
	UL l_ul_i;
	PUC l_puc_tag;
	US l_us_buffer_tag;
	US l_us_buffer_length;

	l_ul_i = 0;
	l_puc_tag = g_puc_tag_list;
	while(l_ul_i < g_ul_tag_list_size)
	{
		memcpy(&l_us_buffer_tag, l_puc_tag, sizeof(l_us_buffer_tag));
		memcpy(&l_us_buffer_length, l_puc_tag+sizeof(US), sizeof(l_us_buffer_length));

		if (l_us_buffer_tag == i_us_tag)
			return CT_ERR_SUCCESS;

		l_ul_i += 2*sizeof(US)+l_us_buffer_length;
		l_puc_tag += 2*sizeof(US)+l_us_buffer_length;
	}

	return CT_ERR_NOT_FOUND;
}*/

//****************************************************************************/
//****************************************************************************/
//
//			NOM			:
//			DESCRIPTION	:
//****************************************************************************/
//
//			ENTREE		:
//			SORTIE		:
//			FCT APP		:
//			REMARQUE	:
//****************************************************************************/
//****************************************************************************/
/*L TagListAdd(US i_us_tag, US i_us_length, const UC* i_puc_value)
{
	L l_l_result;

	if (TagListIsPresent(i_us_tag) == CT_ERR_SUCCESS)
		return CT_ERR_INVALID_PARAMETER;

	if (g_ul_tag_list_size+2*sizeof(US)+i_us_length > g_ul_tag_list_buffer_size)
	{
		l_l_result = TagListGrow(2*sizeof(US)+i_us_length);
		if (l_l_result != CT_ERR_SUCCESS)
			return l_l_result;
	}

	memcpy(g_puc_tag_list+g_ul_tag_list_size, &i_us_tag, sizeof(US));
	g_ul_tag_list_size += sizeof(US);

	memcpy(g_puc_tag_list+g_ul_tag_list_size, &i_us_length, sizeof(US));
	g_ul_tag_list_size += sizeof(US);

	memcpy(g_puc_tag_list+g_ul_tag_list_size, i_puc_value, i_us_length);
	g_ul_tag_list_size += i_us_length;

	return CT_ERR_SUCCESS;
}*/

//****************************************************************************/
//****************************************************************************/
//
//			NOM			:
//			DESCRIPTION	:
//****************************************************************************/
//
//			ENTREE		:
//			SORTIE		:
//			FCT APP		:
//			REMARQUE	:
//****************************************************************************/
//****************************************************************************/
/*L TagListErase(US i_us_tag)
{
	UL l_ul_i;
	PUC l_puc_tag;
	US l_us_buffer_tag;
	US l_us_buffer_length;

	l_ul_i = 0;
	l_puc_tag = g_puc_tag_list;
	while(l_ul_i < g_ul_tag_list_size)
	{
		memcpy(&l_us_buffer_tag, l_puc_tag, sizeof(l_us_buffer_tag));
		memcpy(&l_us_buffer_length, l_puc_tag+sizeof(US), sizeof(l_us_buffer_length));

		if (l_us_buffer_tag == i_us_tag)
		{
			memmove(l_puc_tag, l_puc_tag+2*sizeof(US)+l_us_buffer_length,
						g_ul_tag_list_size-2*sizeof(US)+l_us_buffer_length);

			g_ul_tag_list_size -= 2*sizeof(US)+l_us_buffer_length;

			return CT_ERR_SUCCESS;
		}

		l_ul_i += 2*sizeof(US)+l_us_buffer_length;
		l_puc_tag += 2*sizeof(US)+l_us_buffer_length;
	}

	return CT_ERR_NOT_FOUND;
}*/

//****************************************************************************/
//****************************************************************************/
//
//			NOM			:
//			DESCRIPTION	:
//****************************************************************************/
//
//			ENTREE		:
//			SORTIE		:
//			FCT APP		:
//			REMARQUE	:
//****************************************************************************/
//****************************************************************************/
/*L TagListRead(US i_us_tag, PUS o_pus_length, PUC o_puc_value)
{
	UL l_ul_i;
	PUC l_puc_tag;
	US l_us_buffer_tag;
	US l_us_buffer_length;

	l_ul_i = 0;
	l_puc_tag = g_puc_tag_list;
	while(l_ul_i < g_ul_tag_list_size)
	{
		memcpy(&l_us_buffer_tag, l_puc_tag, sizeof(l_us_buffer_tag));
		memcpy(&l_us_buffer_length, l_puc_tag+sizeof(US), sizeof(l_us_buffer_length));

		if (l_us_buffer_tag == i_us_tag)
		{
			*o_pus_length = l_us_buffer_length;
			memcpy(o_puc_value, l_puc_tag+2*sizeof(US), l_us_buffer_length);

			return CT_ERR_SUCCESS;
		}

		l_ul_i += 2*sizeof(US)+l_us_buffer_length;
		l_puc_tag += 2*sizeof(US)+l_us_buffer_length;
	}

	return CT_ERR_NOT_FOUND;
}*/
/*************************************************************/
/*************************************************************/
US ReadTag(PUC Buffer, PL Tag)
{
    US Pos;

    if ((Buffer[0] & MSBYTE_PARSE) == MSBYTE_PARSE)
    {
        *Tag = Buffer[0] * 256 + Buffer[1];
        Pos = 2;
    }
    else
    {
        *Tag = Buffer[0];
        Pos = 1;
    }

    return	Pos;
}

/*************************************************************/
/*************************************************************/
US ReadDataLength(PUC Buffer, PUS Length)
{
    US Pos;

    switch (Buffer[0])
    {
    case TWO_BYTES_LENGTH:
        *Length = Buffer[1] * 256 + Buffer[2];
        Pos = 3;

        break;

    case ONE_BYTE_LENGTH:
        *Length = Buffer[1];
        Pos = 2;

        break;

    default:
        *Length = Buffer[0];
        Pos = 1;

        break;
    }

    return	Pos;
}
/*********************************************************************************************/
/*********************************************************************************************/
C FindTag(L srcTag, PUS pOffset, PUS pDataLength, PUC pData, US Length)
{
    C	Status = KO;
    US	Position;
    L	Tag;
    US	Len;

    /* loop through the data looking for the required tag */
    Position = 0;
    while ((Position < (Length - 2)) && (Status == KO))
    {
        /* skip nulls */
        while (((pData[Position] == 0x00) || (pData[Position] == 0xFF)) &&
               (Position < (Length - 2)))
        {
            if (Position < (Length - 2))
                Position++;
            else
                /* didn't find it */
                Position = Length;
        };

        if (Position < Length)
        {
            Position += ReadTag(&pData[Position], &Tag);

            /* read length of data */
            Position += ReadDataLength(&pData[Position], &Len);

            /* check length not past end */
            if ((Position + Len) <= Length)
            {
                if (Tag == srcTag)
                {
                    *pOffset = Position;
                    *pDataLength = Len;

                    Status = OK;
                }
                else
                    Position += Len;
            }
        }
    };

    return (Status);
}
//****************************************************************************/
//****************************************************************************/
C ProcessScriptData(uint8 *MessageData, uint16 DataLength,uint8 *Offset71,uint8 *Script71,uint8 *Offset72,uint8 *Script72)
{
	/**/
    uint8  Status = OK;
    US	   Position;
    L 	   Tag;
    uint16 Len;

	//eft30printhexKamal((char*)MessageData,DataLength);

    // loop through the data looking for type 71 and type 72 scripts
    Position = 0;
    while ((Position < DataLength) && (Status == OK))
    {
        Position += ReadTag(&MessageData[Position], &Tag);
		//eft30printhexKamal((char*)&MessageData[Position],1);
        // read length of data
        Position += ReadDataLength(&MessageData[Position], &Len);
		//eft30printhexKamal((char*)&MessageData[Position],Len);
        // check length not past end
        if ((Position + Len) <= DataLength)
        {
            switch (Tag)
            {
            case 0x71:
                //check the data length
                if ((Len + *Offset71 + 2) <= SCRIPT_DATA_BUFFER_SIZE)
                {
                	  if(MessageData[Position +1]== 0x0d)
                    {
                     	Os__xprint("Pb Lg script");
                        Position += Len;
                    }
                    else
                    {
                    		//concatenate the data into the script 71 buffer for the specified length
                    		memcpy (&Script71[*Offset71], &MessageData[Position - 2], (Len + 2));

                    		*Offset71 += Len + 2;
                    		Position += Len;
                    }
                }
                else
                    Status = KO;

                break;

            case 0x72:
                //check the data length
                if ((Len + *Offset72 + 2) <= SCRIPT_DATA_BUFFER_SIZE)
                {
                    //concatenate the data into the script 71 buffer for the specified length
                    memcpy (&Script72[*Offset72], &MessageData[Position - 2], (Len + 2));

                    *Offset72 += Len + 2;
                    Position += Len;
                }
                else
                    Status = KO;

                break;

            default:
                //skip the tags data
                Position += Len;
                break;
            }
        }
    };

    if (Status == OK)
    {
        if (*Offset71 > 0)
        {
            //ok to store it
		}
        if (*Offset72 > 0)
        {
            //ok to store it
		}
    }

    return (Status);
}
//****************************************************************************/
//****************************************************************************/
L	Remplir_Infopayment(void)
{
	L	l_l_result;
	US	l_us_length;
	UC	l_uc_data[100];
	UC	l_c_chaine[40];
	UC	Lg,i;
	UC	Card_Holder_Nbre_ISO2[21],Expired_Date_ISO2[5];
	
//Signature
	EmvReadIccData(TAG_CVR_RESULT, &l_us_length, l_uc_data);	//EmvReadIccData(SEMV_TAG_9F34_CVM_RESULT, &l_us_length, l_uc_data);
	
	if( ((l_uc_data[0]&0x0f)== CVM_PLAINTEXT_PIN_VERIF_SIGN)
		||((l_uc_data[0]&0x0f) == CVM_ENCIPH_PIN_VERIF_SIGN)
		||((l_uc_data[0]&0x0f) == CVM_SIGN_ONLY)
		||((l_uc_data[0]&0x0f) == CVM_NO_CVM_REQ)
	)
		StructInfoPayment.SignatureHolder= REQUIRED;
	else
		StructInfoPayment.SignatureHolder= NOT_REQUIRED;
//Signature

	switch(StructInfoPayment.CodeFunction)
	{
	case CASHADVANCESCODE:
		memcpy(StructInfoPayment.Proc_Code,"170000",6);
		break;
	case SALECASHBACKCODE:
		memcpy(StructInfoPayment.Proc_Code,"090000",6);
		break;
	default: //Sale
		memcpy(StructInfoPayment.Proc_Code,"000000",6);
		break;
	}
	//TAG_TRANSACTION_DATE

	l_l_result = EmvReadIccData(TAG_TRANSACTION_DATE, &l_us_length, l_uc_data);	//SEMV_TAG_009A_TRANSAC_DATE
	if (l_l_result == TRUE)
	{
		CONVERT_htoa ( l_uc_data,3,StructInfoPayment.Transaction_DateTime);
	}
	//TAG_TRANSACTION_TIME
	l_l_result = EmvReadIccData(TAG_TRANSACTION_TIME, &l_us_length, l_uc_data);//SEMV_TAG_9F21_TRANSAC_TIME
	if (l_l_result == TRUE)
	{
		CONVERT_htoa ( l_uc_data,3,StructInfoPayment.Transaction_DateTime+6);
	}
	//TAG_ISSUER_CODE_TABLE_INDEX
	l_l_result = EmvReadIccData(TAG_ISSUER_CODE_TABLE_INDEX, &l_us_length, l_uc_data);//SEMV_TAG_9F11_ISSUER_COD_TAB_IDX
	if (l_l_result == TRUE)
	{
		//ShowTrace("SEMV_TAG_9F11_ISSUER_COD_TAB_IDX");
		//eft30printhex(l_uc_data,l_us_length);
		if((l_uc_data[0]&0x01)==0x01)
		{

			//Prefered Name
			//TAG_APPLICATION_PREFFERRED_NAME
			l_l_result = EmvReadIccData(TAG_APPLICATION_PREFFERRED_NAME, &l_us_length, l_uc_data);//SEMV_TAG_9F12_APPLI_PREFERED_NAME
			if (l_l_result == TRUE)
			{
				//ShowTrace("SEMV_TAG_9F12_APPLI_PREFERED_NAME");
				//eft30printhex(l_uc_data,l_us_length);
				memcpy(StructInfoPayment.Appli_label, l_uc_data, l_us_length);
			}
			else
			{
				//Application Label
				//TAG_APPLICATION_LABEL
				l_l_result = EmvReadIccData(TAG_APPLICATION_LABEL, &l_us_length, l_uc_data);//SEMV_TAG_0050_APPLI_LABEL
				if (l_l_result == TRUE)
				{
					//ShowTrace("SEMV_TAG_0050_APPLI_LABEL");
					//eft30printhex(l_uc_data,l_us_length);
					memcpy(StructInfoPayment.Appli_label, l_uc_data, l_us_length);
				}
			}
		}
		else
		{
			//Application Label
			//TAG_APPLICATION_LABEL
			l_l_result = EmvReadIccData(TAG_APPLICATION_LABEL, &l_us_length, l_uc_data);//SEMV_TAG_0050_APPLI_LABEL
			if (l_l_result == TRUE)
			{
				//ShowTrace("SEMV_TAG_0050_APPLI_LABEL");
				//eft30printhex(l_uc_data,l_us_length);
				memcpy(StructInfoPayment.Appli_label, l_uc_data, l_us_length);
			}
		}
	}
	else
	{
		//Application Label
		//TAG_APPLICATION_LABEL
		l_l_result = EmvReadIccData(TAG_APPLICATION_LABEL, &l_us_length, l_uc_data);//SEMV_TAG_0050_APPLI_LABEL
		if (l_l_result == TRUE)
		{
			//ShowTrace("SEMV_TAG_0050_APPLI_LABEL");
			//eft30printhex(l_uc_data,l_us_length);
			memcpy(StructInfoPayment.Appli_label, l_uc_data, l_us_length);
		}
	}

	//CardHolder name
	//TAG_CARDHOLDER_NAME
	l_l_result = EmvReadIccData(TAG_CARDHOLDER_NAME, &l_us_length, l_uc_data);//SEMV_TAG_5F20_CARD_HOLDER_NAME
	if (l_l_result == TRUE)
	{
		//ShowTrace("SEMV_TAG_5F20_CARD_HOLDER_NAME");
		//eft30printhex(l_uc_data,l_us_length);
		memcpy(StructInfoPayment.Nom_Prenom, l_uc_data, l_us_length);
	}

	//Application Aid
	//TAG_AID_TERMINAL ou TAG_DF_NAME
	l_l_result = EmvReadIccData(TAG_AID_TERMINAL, &l_us_length, l_uc_data);//SEMV_TAG_9F06_AID
	StructInfoPayment.Appli_aid[0] = l_us_length;
	if (l_l_result == TRUE)
	{
		//ShowTrace("SEMV_TAG_9F06_AID");
		//eft30printhex(l_uc_data,l_us_length);
		memset(l_c_chaine,'\0',40);
		CONVERT_htoa ( l_uc_data, StructInfoPayment.Appli_aid[0],StructInfoPayment.Appli_aid+1);
	}
	//Card Number
	//TAG_APPLI_PRIM_ACCOUNT_NB
	l_l_result = EmvReadIccData(TAG_APPLI_PRIM_ACCOUNT_NB, &l_us_length, l_uc_data);//SEMV_TAG_005A_APPLICATION_PAN
	if (l_l_result == TRUE)
	{
		//ShowTrace("SEMV_TAG_005A_APPLICATION_PAN");
		//eft30printhex(l_uc_data,l_us_length);
		memset(l_c_chaine,'\0',40);
		CONVERT_htoa(l_uc_data, l_us_length, l_c_chaine);
		for(i=0;((i<2*l_us_length)&&(l_c_chaine[i]!='F')&&(l_c_chaine[i]!='f')&&(l_c_chaine[i]!=' ')&&(l_c_chaine[i]!='\0'));i++);
		if(i==20)
		{
			if(g_trace_emv)Os__xprint("LG 005A PAN > 19");
			return FALSE;
		}
		StructInfoPayment.Holder_Card[0] = i;
		memcpy((PC)&StructInfoPayment.Holder_Card[1],l_c_chaine,StructInfoPayment.Holder_Card[0]);
	}
	//Iso2
	//TAG_TRACK2_EQU_DATA
	l_l_result = EmvReadIccData(TAG_TRACK2_EQU_DATA, &l_us_length, l_uc_data);//SEMV_TAG_0057_TRACK_2_EQU_DATA
	if (l_l_result == TRUE)
	{
		short cara;
		//ShowTrace("SEMV_TAG_0057_TRACK_2_EQU_DATA");
		memset(l_c_chaine,'\0',40);
		//CONVERT_dcbtoa(l_uc_data, l_us_length, (PC)l_c_chaine, FALSE);
		CONVERT_htoa(l_uc_data,l_us_length,(PC)l_c_chaine);//belhadeg probleme des transactions rejet?es
		Lg = (UC)l_us_length*2;
		for(cara=0;
		((cara<Lg)&&(cara<37)&&(l_c_chaine[cara] != 'F')&&(l_c_chaine[cara] != 'f')&&(l_c_chaine[cara] != 0)&&(l_c_chaine[cara] != ' '));
		cara ++);
		Lg=cara;
		StructInfoPayment.Normal_Iso2[0] = Lg;
		memcpy(&StructInfoPayment.Normal_Iso2[1],l_c_chaine,Lg);
		StructInfoPayment.Compressed_Iso2[0]= Lg;
		memcpy(&StructInfoPayment.Compressed_Iso2[1],l_uc_data,19);

		//eft30printhexKamal(StructInfoPayment.Compressed_Iso2,20);
		//eft30printhexKamal(StructInfoPayment.Normal_Iso2,40);
		/****/
/*		ShowTrace_Mouna("ad3");
		imp_buffer(StructInfoPayment.Compressed_Iso2,StructInfoPayment.Compressed_Iso2[0]);
		bcd_asc(iso2,&StructInfoPayment.Compressed_Iso2[1],20);
		imp_buffer(iso2,20);*/
		/****/
		memcpy(StructInfoPayment.SeviceCode,&StructInfoPayment.Normal_Iso2[StructInfoPayment.Holder_Card[0]+6],3);
	}
	//Expired Date
	//TAG_APPLI_EXPIRATION_DATE
	l_l_result = EmvReadIccData(TAG_APPLI_EXPIRATION_DATE, &l_us_length, l_uc_data);//SEMV_TAG_5F24_APPLI_EXPIR_DATE
	if (l_l_result == TRUE)
	{
		//ShowTrace("SEMV_TAG_5F24_APPLI_EXPIR_DATE");
		//eft30printhex(l_uc_data,l_us_length);
		memset(l_c_chaine,'\0',40);
		CONVERT_dcbtoa(l_uc_data, l_us_length, (PC)l_c_chaine, FALSE);
		//CONVERT_htoa(l_uc_data,l_us_length,(PC)l_c_chaine);//

		memcpy((PC)&StructInfoPayment.Expired_Date,(PC)l_c_chaine,2*l_us_length);
	}
	for(i=0;((i<20)&&(StructInfoPayment.Normal_Iso2[i+1]!='D')&&(StructInfoPayment.Normal_Iso2[i+1]!='d')&&(StructInfoPayment.Normal_Iso2[i+1]!='='));i++);
	if(i!=20)
	{
		memset(Card_Holder_Nbre_ISO2,0,sizeof(Card_Holder_Nbre_ISO2));
		Card_Holder_Nbre_ISO2[0] = i;
		memcpy(&Card_Holder_Nbre_ISO2[1],&StructInfoPayment.Normal_Iso2[1],i);
		memset(Expired_Date_ISO2,0,sizeof(Expired_Date_ISO2));
		memcpy(Expired_Date_ISO2,&StructInfoPayment.Normal_Iso2[i+2],4);
	}
	if(
		(Card_Holder_Nbre_ISO2[0]!=StructInfoPayment.Holder_Card[0])||
		memcmp(&Card_Holder_Nbre_ISO2[1],&StructInfoPayment.Holder_Card[1],StructInfoPayment.Holder_Card[0])||
		memcmp(Expired_Date_ISO2,StructInfoPayment.Expired_Date,4)
		)
	{
		Os__xprint("ATTENTION INCONSIS");
	}
	//Pan Seq Number
	//TAG_APPLI_PRIM_ACCOUNT_NB_SEQ_NB

	l_l_result = EmvReadIccData(TAG_APPLI_PRIM_ACCOUNT_NB_SEQ_NB, &l_us_length, l_uc_data);//SEMV_TAG_5F34_APPLI_PAN_SEQ_NUMBER
	if (l_l_result != TRUE)
		StructInfoPayment.PanSeqNum5F34[0] = 0;
	else
		memcpy(StructInfoPayment.PanSeqNum5F34,l_uc_data,l_us_length);
	char_asc(StructInfoPayment.PanSeqNumber,3,StructInfoPayment.PanSeqNum5F34);    // v?rification verifone 18062019


	//	{TAG_AMOUNT_AUTH_NUM,				0x9F,0x02},//SEMV_TAG_9F02_AMOUNT_AUTH_NUMERIC
//	l_l_result = EmvReadIccData(TAG_AMOUNT_AUTH_NUM, &l_us_length, l_uc_data);//SEMV_TAG_9F06_AID	//POURBOIRE  15042019
//	if (l_l_result == TRUE)
//	{
//		memset(l_c_chaine,'\0',40);
//		CONVERT_htoa ( l_uc_data, l_us_length,l_c_chaine);
//		StructInfoPayment.Transaction_Amount = atol(l_c_chaine);
//	}

	//	{TAG_TRANSACTION_CURRENCY_CODE,		0x5F,0x2A},//SEMV_TAG_5F2A_TRANSAC_CUR_CODE
//	l_l_result = EmvReadIccData(TAG_TRANSACTION_CURRENCY_CODE, &l_us_length, l_uc_data);//SEMV_TAG_9F06_AID
//	if (l_l_result == TRUE)
//	{
//		memset(l_c_chaine,'\0',40);
//		CONVERT_htoa ( l_uc_data, l_us_length,l_c_chaine);
//		memcpy(StructInfoPayment.Devise_acc.code_num,l_c_chaine+1,3);
//		//StructInfoPayment.Devise_acc.code_alph
//	}


	return TRUE;
}
//****************************************************************************/
//****************************************************************************/
uint8 construire_champ55(void)
{
	typedef	struct{
		uint16	tag;
		uint8	semitag1;
		uint8	semitag2;
	}Stag;

	US		l_us_length=0;
	UC		l_uc_data[128];

	uint16  i;

	uint8	l_champs55;
	uint8	champs55[255];
	uint8   ConvertBuffer[20];
	uint8   Aid[5];

	unsigned char uc_l_tmp_lg_9 [ 9 ] ;

	Stag Tag_Chmp55[21] = {
		{TAG_AIP,							0x00,0x82},//SEMV_TAG_0082_AIP
		//      0x0082  TAG_AIP
		{TAG_AID_TERMINAL,					0x00,0x84},//SEMV_TAG_9F06_AID
		//      0x0084  TAG_AID_TERMINAL
		{TAG_TVR,							0x00,0x95},//SEMV_TAG_0095_TVR
		//      0x0095  TAG_TVR
		{TAG_TRANSACTION_DATE,				0x00,0x9A},//SEMV_TAG_009A_TRANSAC_DATE
		//      0x009A  TAG_TRANSACTION_DATE
		{TAG_TRANSACTION_TYPE,				0x00,0x9C},//SEMV_TAG_009C_TRANSAC_TYPE
		//      0x009C  TAG_TRANSACTION_TYPE
		{TAG_TRANSACTION_CURRENCY_CODE,		0x5F,0x2A},//SEMV_TAG_5F2A_TRANSAC_CUR_CODE
		//      0x5F2A  TAG_TRANSACTION_CURRENCY_CODE
		{TAG_AMOUNT_AUTH_NUM,				0x9F,0x02},//SEMV_TAG_9F02_AMOUNT_AUTH_NUMERIC
		//      0x9F02  TAG_AMOUNT_AUTH_NUM
		{TAG_AMOUNT_OTHER_NUM,				0x9F,0x03},//SEMV_TAG_9F03_AMOUNT_OTHER_NUMERIC
		//      0x9F03  TAG_AMOUNT_OTHER_NUM
		{TAG_VERSION_NUMBER_TERMINAL,		0x9F,0x09},//SEMV_TAG_9F09_TERM_APPLI_VERS_NUMB
		//      0x9F09  TAG_VERSION_NUMBER_TERMINAL
		{TAG_ISSUER_APPLICATION_DATA,		0x9F,0x10},//SEMV_TAG_9F10_ISSUER_APPLI_DATA
		//      0x9F10  TAG_ISSUER_APPLICATION_DATA
		{TAG_TERMINAL_COUNTRY_CODE,			0x9F,0x1A},//SEMV_TAG_9F1A_TERM_COUNTRY_CODE
		//      0x9F1A  TAG_TERMINAL_COUNTRY_CODE
		{TAG_IFD_SERIAL_NUMBER,				0x9F,0x1E},//SEMV_TAG_9F1E_IFD
		//      0x9F1E  TAG_IFD_SERIAL_NUMBER
		{TAG_TRACK2_EQU_DATA,				0x00,0x57},//SEMV_TAG_0057_TRACK_2_EQU_DATA
		//      0x0057  TAG_TRACK2_EQU_DATA
		{TAG_APPLI_CRYPTOGRAMME_AC,			0x9F,0x26},//SEMV_TAG_9F26_APPLI_CRYPTO
		//      0x9F26  TAG_APPLI_CRYPTOGRAMME_AC
		{TAG_CRYPTOGRAM_INFORMATION_DATA,	0x9F,0x27},//SEMV_TAG_9F27_CRYPTO_INFO_DATA
		//      0x9F27  TAG_CRYPTOGRAM_INFORMATION_DATA
		{TAG_TERMINAL_CAPABILITIES,			0x9F,0x33},//SEMV_TAG_9F33_TERM_CAPABILITIES
		//      0x9F33  TAG_TERMINAL_CAPABILITIES
		{TAG_CVR_RESULT,					0x9F,0x34},//SEMV_TAG_9F34_CVM_RESULT
		//      0x9F34  TAG_CVR_RESULT
		{TAG_TERMINAL_TYPE,					0x9F,0x35},//SEMV_TAG_9F35_TERM_TYPE
		//      0x9F35  TAG_TERMINAL_TYPE
		{TAG_ATC,							0x9F,0x36},//SEMV_TAG_9F36_ATC
		//      0x9F36  TAG_ATC
		{TAG_UNPREDICTABLE_NUMBER,			0x9F,0x37},//SEMV_TAG_9F37_UNPREDICT_NUMBER
		//      0x9F37  TAG_UNPREDICTABLE_NUMBER
		{TAG_TRANSACTION_SEQ_COUNTER,		0x9F,0x41},//SEMV_TAG_9F41_TRANSAC_SEQ_COUNTER
		//      0x9F41  TAG_TRANSACTION_SEQ_COUNTER

	};
//	ShowTrace("CHAMPS 55");
	l_champs55 = 0;
	for(i=0;(i<21);i+=1)
	{
		switch(i)
		{
			case 0:
			case 1:
			case 2:
			case 3:
			case 5:
			case 6:
			case 7:   //laarbi
			case 8:
			case 9:
			case 10:
			case 12:
			case 13:
			case 14:
			case 15:
			case 16:
			case 18:
			case 19:
				if(EmvReadIccData(Tag_Chmp55[i].tag, &l_us_length, l_uc_data) == TRUE)
				{
					champs55[l_champs55++] =Tag_Chmp55[i].semitag1;
					if(Tag_Chmp55[i].semitag1 == 0x00)
						champs55[l_champs55-1] =Tag_Chmp55[i].semitag2;
					else
						champs55[l_champs55++] =Tag_Chmp55[i].semitag2;

					champs55[l_champs55++] =(UC)l_us_length;
					memcpy(&champs55[l_champs55],l_uc_data,l_us_length);
					l_champs55 += l_us_length;

					if(i == 13)
					{
						memset (ConvertBuffer, 0, 8);
						l_us_length =l_us_length*2;
						hex_str (ConvertBuffer, l_uc_data, (C)l_us_length);
						memcpy(StructInfoPayment.Certif,ConvertBuffer, l_us_length);
					}
					if(i == 1)
						memcpy(Aid,l_uc_data,5);
					if(i == 16)
					{
						if( ((l_uc_data[0]&0x0f)== CVM_PLAINTEXT_PIN_VERIF_SIGN)
							||((l_uc_data[0]&0x0f) == CVM_ENCIPH_PIN_VERIF_SIGN)
							||((l_uc_data[0]&0x0f) == CVM_SIGN_ONLY)
							||((l_uc_data[0]&0x0f) == CVM_NO_CVM_REQ)
							)
							StructInfoPayment.SignatureHolder= REQUIRED;
						else
							StructInfoPayment.SignatureHolder= NOT_REQUIRED;
					}
				}
			break;


			case 11:
			CUTERMgetSerialNumber((char*)uc_l_tmp_lg_9);
			champs55[l_champs55++] =Tag_Chmp55[i].semitag1;
			if(Tag_Chmp55[i].semitag1 == 0x00)
				champs55[l_champs55-1] =Tag_Chmp55[i].semitag2;
			else
				champs55[l_champs55++] =Tag_Chmp55[i].semitag2;

			champs55[l_champs55++] =(UC)8;
			memcpy(&champs55[l_champs55],uc_l_tmp_lg_9,8);
			l_champs55 += 8;
			break;




			case 4:
			case 17:
			case 20:
				if(EmvReadIccData(Tag_Chmp55[i].tag, &l_us_length, l_uc_data) == TRUE)
				{
					champs55[l_champs55++] =Tag_Chmp55[i].semitag1;
					if(Tag_Chmp55[i].semitag1 == 0x00)
						champs55[l_champs55-1] =Tag_Chmp55[i].semitag2;
					else
						champs55[l_champs55++] =Tag_Chmp55[i].semitag2;

					champs55[l_champs55++] =(UC)l_us_length;
					memcpy(&champs55[l_champs55],l_uc_data,l_us_length);
					l_champs55 += l_us_length;

					if(		(i == 4)
						&&	(memcmp(FicPara.MCC,"6010",4) == 0)
					)
					{
						memcpy(&champs55[l_champs55-1],"\x01",1);
					}
				}
			break;
/*			case 20:
				if(EmvReadIccData(Tag_Chmp55[i].tag, &l_us_length, l_uc_data) == TRUE)
				{
					champs55[l_champs55++] =Tag_Chmp55[i].semitag1;
					if(Tag_Chmp55[i].semitag1 == 0x00)
						champs55[l_champs55-1] =Tag_Chmp55[i].semitag2;
					else
						champs55[l_champs55++] =Tag_Chmp55[i].semitag2;

					champs55[l_champs55++] =(UC)l_us_length;
					memcpy(&champs55[l_champs55],"\x30\x30\x30\x38",l_us_length);
					l_champs55 += l_us_length;
				}
			break;
*/		}
//		eft30printhex(l_uc_data,l_us_length);
	}
	//TCC pour Mci
	if(	(memcmp("\xA0\x00\x00\x00\x04",Aid,5)==0)
		||(memcmp("\xA0\x00\x00\x00\x10",Aid,5)==0)
	)
	{
		if(EmvReadIccData(TAG_TCC, &l_us_length, l_uc_data) == TRUE)
		{
			champs55[l_champs55++] =0x9F;
			champs55[l_champs55++] =0x53;

			champs55[l_champs55++] =(UC)l_us_length;
			memcpy(&champs55[l_champs55],l_uc_data,l_us_length);
			l_champs55 += l_us_length;
		}
		else
			Os__xprint("Missing TAG TCC");
	}


	if(EmvReadIccData(TAG_APPLI_PRIM_ACCOUNT_NB_SEQ_NB, &l_us_length, l_uc_data) ==  TRUE)
	{
		champs55[l_champs55++] =0x5F;
		champs55[l_champs55++] =0x34;

		champs55[l_champs55++] =(UC)l_us_length;
		memcpy(&champs55[l_champs55],l_uc_data,l_us_length);
		l_champs55 += l_us_length;
	}

	StructInfoPayment.l_champ55 = l_champs55;
	memcpy(StructInfoPayment.Champ55,champs55,l_champs55);


	Data_Last_TRS_Emv_Rej_tmp.l_champ55 = l_champs55;
	memcpy(Data_Last_TRS_Emv_Rej_tmp.Champ55,champs55,l_champs55);


//	SEMV_TAG_9F0D_IAC_DEFAULT			TAG_IssActCodDef	9F0D
	if(EmvReadIccData((US)TAG_TERMINAL_ACTION_CODE_DEFAULT, &l_us_length, l_uc_data) == TRUE)
	{
		Data_Last_TRS_Emv_Rej_tmp.Champ55[Data_Last_TRS_Emv_Rej_tmp.l_champ55++] =0x9F;
		Data_Last_TRS_Emv_Rej_tmp.Champ55[Data_Last_TRS_Emv_Rej_tmp.l_champ55++] =0x0D;

		Data_Last_TRS_Emv_Rej_tmp.Champ55[Data_Last_TRS_Emv_Rej_tmp.l_champ55++] =(UC)l_us_length;
		memcpy(&Data_Last_TRS_Emv_Rej_tmp.Champ55[Data_Last_TRS_Emv_Rej_tmp.l_champ55],l_uc_data,l_us_length);
		Data_Last_TRS_Emv_Rej_tmp.l_champ55 += l_us_length;
	}

//	SEMV_TAG_9F0E_IAC_DENIAL			TAG_IssActCodDen	9F0E
	if(EmvReadIccData((US)TAG_TERMINAL_ACTION_CODE_DENIAL, &l_us_length, l_uc_data) == TRUE)
	{
		Data_Last_TRS_Emv_Rej_tmp.Champ55[Data_Last_TRS_Emv_Rej_tmp.l_champ55++] =0x9F;
		Data_Last_TRS_Emv_Rej_tmp.Champ55[Data_Last_TRS_Emv_Rej_tmp.l_champ55++] =0x0E;

		Data_Last_TRS_Emv_Rej_tmp.Champ55[Data_Last_TRS_Emv_Rej_tmp.l_champ55++] =(UC)l_us_length;
		memcpy(&Data_Last_TRS_Emv_Rej_tmp.Champ55[Data_Last_TRS_Emv_Rej_tmp.l_champ55],l_uc_data,l_us_length);
		Data_Last_TRS_Emv_Rej_tmp.l_champ55 += l_us_length;
	}

//	SEMV_TAG_9F0F_IAC_ONLINE			TAG_IssActCodOnl	9F0F
	if(EmvReadIccData((US)TAG_TERMINAL_ACTION_CODE_ONLINE, &l_us_length, l_uc_data) == TRUE)
	{
		Data_Last_TRS_Emv_Rej_tmp.Champ55[Data_Last_TRS_Emv_Rej_tmp.l_champ55++] =0x9F;
		Data_Last_TRS_Emv_Rej_tmp.Champ55[Data_Last_TRS_Emv_Rej_tmp.l_champ55++] =0x0F;

		Data_Last_TRS_Emv_Rej_tmp.Champ55[Data_Last_TRS_Emv_Rej_tmp.l_champ55++] =(UC)l_us_length;
		memcpy(&Data_Last_TRS_Emv_Rej_tmp.Champ55[Data_Last_TRS_Emv_Rej_tmp.l_champ55],l_uc_data,l_us_length);
		Data_Last_TRS_Emv_Rej_tmp.l_champ55 += l_us_length;
	}

//	SEMV_TAG_9F21_TRANSAC_TIME			TAG_TraTim			9F21
	if(EmvReadIccData(Tag_Chmp55[i].tag, &l_us_length, l_uc_data) == TRUE)
	{
		Data_Last_TRS_Emv_Rej_tmp.Champ55[Data_Last_TRS_Emv_Rej_tmp.l_champ55++] =0x9F;
		Data_Last_TRS_Emv_Rej_tmp.Champ55[Data_Last_TRS_Emv_Rej_tmp.l_champ55++] =0x21;

		Data_Last_TRS_Emv_Rej_tmp.Champ55[Data_Last_TRS_Emv_Rej_tmp.l_champ55++] =(UC)l_us_length;
		memcpy(&Data_Last_TRS_Emv_Rej_tmp.Champ55[Data_Last_TRS_Emv_Rej_tmp.l_champ55],l_uc_data,l_us_length);
		Data_Last_TRS_Emv_Rej_tmp.l_champ55 += l_us_length;
	}

//	SEMV_TAG_005A_APPLICATION_PAN		TAG_AppPan			005A
	if(EmvReadIccData(TAG_APPLI_PRIM_ACCOUNT_NB, &l_us_length, l_uc_data) == TRUE)
	{
		Data_Last_TRS_Emv_Rej_tmp.Champ55[Data_Last_TRS_Emv_Rej_tmp.l_champ55++] =0x5A;

		Data_Last_TRS_Emv_Rej_tmp.Champ55[Data_Last_TRS_Emv_Rej_tmp.l_champ55++] =(UC)l_us_length;
		memcpy(&Data_Last_TRS_Emv_Rej_tmp.Champ55[Data_Last_TRS_Emv_Rej_tmp.l_champ55],l_uc_data,l_us_length);
		Data_Last_TRS_Emv_Rej_tmp.l_champ55 += l_us_length;
	}

//	SEMV_TAG_5F34_APPLI_PAN_SEQ_NUMBER	TAG_AppPanSeqNbr	5F34
	if(EmvReadIccData(TAG_APPLI_PRIM_ACCOUNT_NB_SEQ_NB, &l_us_length, l_uc_data) == TRUE)
	{
		Data_Last_TRS_Emv_Rej_tmp.Champ55[Data_Last_TRS_Emv_Rej_tmp.l_champ55++] =0x5F;
		Data_Last_TRS_Emv_Rej_tmp.Champ55[Data_Last_TRS_Emv_Rej_tmp.l_champ55++] =0x34;

		Data_Last_TRS_Emv_Rej_tmp.Champ55[Data_Last_TRS_Emv_Rej_tmp.l_champ55++] =(UC)l_us_length;
		memcpy(&Data_Last_TRS_Emv_Rej_tmp.Champ55[Data_Last_TRS_Emv_Rej_tmp.l_champ55],l_uc_data,l_us_length);
		Data_Last_TRS_Emv_Rej_tmp.l_champ55 += l_us_length;
	}

//	SEMV_TAG_5F24_APPLI_EXPIR_DATE		TAG_AppExpDat		5F24
	if(EmvReadIccData(TAG_APPLI_EXPIRATION_DATE, &l_us_length, l_uc_data) == TRUE)
	{
		Data_Last_TRS_Emv_Rej_tmp.Champ55[Data_Last_TRS_Emv_Rej_tmp.l_champ55++] =0x5F;
		Data_Last_TRS_Emv_Rej_tmp.Champ55[Data_Last_TRS_Emv_Rej_tmp.l_champ55++] =0x24;

		Data_Last_TRS_Emv_Rej_tmp.Champ55[Data_Last_TRS_Emv_Rej_tmp.l_champ55++] =(UC)l_us_length;
		memcpy(&Data_Last_TRS_Emv_Rej_tmp.Champ55[Data_Last_TRS_Emv_Rej_tmp.l_champ55],l_uc_data,l_us_length);
		Data_Last_TRS_Emv_Rej_tmp.l_champ55 += l_us_length;
	}

//	SEMV_TAG_5F30_SERVICE_CODE			TAG_SerCod			5F30
	if(EmvReadIccData(TAG_SERVICE_CODE, &l_us_length, l_uc_data) == TRUE)
	{
		Data_Last_TRS_Emv_Rej_tmp.Champ55[Data_Last_TRS_Emv_Rej_tmp.l_champ55++] =0x5F;
		Data_Last_TRS_Emv_Rej_tmp.Champ55[Data_Last_TRS_Emv_Rej_tmp.l_champ55++] =0x30;

		Data_Last_TRS_Emv_Rej_tmp.Champ55[Data_Last_TRS_Emv_Rej_tmp.l_champ55++] =(UC)l_us_length;
		memcpy(&Data_Last_TRS_Emv_Rej_tmp.Champ55[Data_Last_TRS_Emv_Rej_tmp.l_champ55],l_uc_data,l_us_length);
		Data_Last_TRS_Emv_Rej_tmp.l_champ55 += l_us_length;
	}

	return OK;
}
/*********************************************************************************************/
/*********************************************************************************************/
L	EmvAuthorisationProcessing(void)
{

	uint8   	Status = OK;
	uchar		update_param[6];
	uchar		update_date[11];




	StructInfoPayment.Flag_Dem_Aut = 1;
//	memcpy(StructInfoPayment.Message_TypeisoEmi,"1200",4);
//	memcpy(StructInfoPayment.Message_TypeisoRec,"1210",4);
//	memcpy(StructInfoPayment.Bit_Map,bmp1200,8);
//	Prepare_Transaction();

	/********************010705*/
	if(
		(StructInfoPayment.CodeFunction==CASHADVANCESCODE)||
		(StructInfoPayment.CodeFunction==SALECODE)||
		(StructInfoPayment.CodeFunction==PREAUTHCOMPCODE)||
		(StructInfoPayment.CodeFunction==PREAUTHCODE)
	  )
	{
		if(StructInfoPayment.Entry_Pin==PIN_ONLINE)
		{
			bitset(StructInfoPayment.Bit_Map, 48);
			bitset(StructInfoPayment.Bit_Map, 52);
		}
		else if((FicPara.Flag_service == 1)||(FicPara.Flag_service == 2))//services badr
			bitset(StructInfoPayment.Bit_Map, 48);



	}
	/********************010705*/

	// Construction du champ 55
	construire_champ55();
	StructInfoPayment.Author_Status = 0xff;

	l_retour = DemAutor8583(update_param,update_date);


	FicPara.sondage = 0;
	//hang_up(typehangup,g_trace);
	//l_retour = OK;
	StructInfoPayment.Author_Status = l_retour;

	ShowTrace("champs 55r");
	//eft30printhexKamal(R_Transac8583.champs55,R_Transac8583.l_champs55);
	//APEMV_UI_MessageDisplayDelayHexa(R_Transac8583.champs55,R_Transac8583.l_champs55, 5);


	switch(l_retour)
	{
	case OK :
		//APEMV_UI_MessageDisplayDelay("OKOK",2);
		g_arqc = 1;
		if(g_trace)Os__xprint("Authorised");
		StructInfoPayment.Top_Forcage = TOP_FOR_ONLINE;
		//Fermer17StructInfoPayment.TacFF34 = SEMV_ACTION_TC;
		memcpy(StructInfoPayment.Approval_Code,R_Transac8583.champs38,6);
		{
			US  Position = 0;
			US  Len;

			//Authorisation Response Code //SEMV_TAG_008A_AUTHOR_RESPONSE_CODE
			if (FindTag (TAG_AUTHORISATION_RESPONSE_CODE, &Position, &Len,R_Transac8583.champs55, R_Transac8583.l_champs55) == OK)
			{
				memcpy(StructInfoPayment.AuthRespCode8A,&R_Transac8583.champs55[Position],2);
				//APEMV_UI_MessageDisplayDelayHexa(StructInfoPayment.AuthRespCode8A,2, 5);
			}
			else
			{
				////APEMV_UI_MessageDisplayDelay("FindTag 8A KO",6);
				memcpy(StructInfoPayment.AuthRespCode8A,&R_Transac8583.champs39[1],2);
				//APEMV_UI_MessageDisplayDelayHexa(StructInfoPayment.AuthRespCode8A,2, 3);
			}

		}
		//Printtampon(StructInfoPayment.AuthRespCode8A,2);
		Status = OK;
		break;

	case REFERRAL :
		//APEMV_UI_MessageDisplayDelay("REFERRAL",2);
		if(g_trace)
			Os__xprint("Referral");
		StructInfoPayment.Flag_Dem_Aut = 0;
		//Fermer17StructInfoPayment.TacFF34 = SEMV_ACTION_AAR;
		StructInfoPayment.Top_Forcage = TOP_FOR_REFFERAL;
		Status = OK;
		memcpy(StructInfoPayment.AuthRespCode8A,"51",2);
		if(
			(StructInfoPayment.CodeFunction == PREAUTHCODE)
			||(StructInfoPayment.CodeFunction == PREAUTHCOMPCODE)
			)
		{
			memcpy(StructInfoPayment.AuthRespCode8A,"51",2);
		}
		break;

	case CAPTURE_CARTE :
	case NON_AUTOR :
	case PIN_FAUX :
		//APEMV_UI_MessageDisplayDelay("NONAUTOR",2);
		g_arqc = 1;
		if(g_trace)
			Os__xprint("Refused");
		StructInfoPayment.Flag_Dem_Aut = 0;
		//Fermer17StructInfoPayment.TacFF34 = SEMV_ACTION_AAC;
		Status = OK;
		memcpy(StructInfoPayment.AuthRespCode8A,"51",2);
		break ;

	case CONNEXION_KO:
	case FORMAT_KO :
	case EMISSION_KO :
	case RECEPT_KO :
	case FORMAT_MSG_REC_KO:
		//APEMV_UI_MessageDisplayDelay("CON KO",2);
		if(g_trace)Os__xprint("KO");
		StructInfoPayment.Flag_Dem_Aut = 0;
		memcpy(StructInfoPayment.AuthRespCode8A,"51",2);
		Status = KO;
		break;

	default :
		//APEMV_UI_MessageDisplayDelay("DEFAUT",2);
		if(g_trace)Os__xprint("Default");
		StructInfoPayment.Flag_Dem_Aut = 0;
		memcpy(StructInfoPayment.AuthRespCode8A,"51",2);
		Status = KO;
		break;
	}

	if(
		(StructInfoPayment.CodeFunction == PREAUTHCODE)
		||(StructInfoPayment.CodeFunction == PREAUTHCOMPCODE)
		)
	{
		if(StructInfoPayment.Author_Status != OK)
		{
			CUMORE_CMI_AFFICHE_MESSAGE(138,128,1000);
			//return TRUE;
		}
	}
	return Status;
	/*
	if (Status == OK)
		return EmvSecondAction();
	else
	{
		l_l_result = TagListErase(SEMV_TAG_FF30_ONLINE_CAPAB_INDIC);
		l_uc_data[0] = FALSE;
		if (TagListAdd(SEMV_TAG_FF30_ONLINE_CAPAB_INDIC, SEMV_TAG_FF30_LNG, l_uc_data) != CT_ERR_SUCCESS)
			return FALSE;
		do
		{
			l_l_result = EmvService(SEMV_SERVICE_TERMINAL_ACTION);
		}while(l_l_result == CT_ERR_MORE_DATA);

		if (l_l_result == CT_ERR_SUCCESS)
		{
			EmvReadAllIccData(l_uc_data,0);
			TraceDebugBuffer(0, l_us_length, l_uc_data);
			l_l_result = EmvReadIccData(SEMV_TAG_FF33_ACTION_CODE_EMV, &l_us_length, l_uc_data);
			StructInfoPayment.TacFF34 = l_uc_data[0];
		}
		return	EmvUnableToGoOnline();
	}
	return 0;
	*/
}
/*-----------------------------------------------------------------
 * Fonction   :
 * objet      :
 * Parametre  : Aucun
 * Retour     : Aucun
 *-----------------------------------------------------------------
*/
uint8 Rech_Aid(Struct_EMV_AID *St_Aid,char *ptValue,int length) //0001 A000000003 04 1010     0083 021
{
	uint16	Nb_Max_Aids;
	uint8	aid[17];
	uint16	Lg_Aid,i;
	uint8	PartialMatchAllowed;

	US		l_us_length;
	UC		l_uc_data[100];

	//char 	trace[125];

	memcpy(l_uc_data,ptValue,length);

	Nb_Max_Aids = Fic_EMV_AID.Indice_fin;
	i=0;

	//sprintf(trace,"%d",Fic_EMV_AID.Indice_fin);
	////APEMV_UI_MessageDisplayDelay(trace, 5);

	while(i<Nb_Max_Aids)
	{
		Lg_Aid = Fic_EMV_AID.tab_aid[i].Lg_pix+5;
		memcpy(aid,Fic_EMV_AID.tab_aid[i].RID,5);
		memcpy(&aid[5],Fic_EMV_AID.tab_aid[i].PIX,Fic_EMV_AID.tab_aid[i].Lg_pix);

		PartialMatchAllowed = Fic_EMV_AID.tab_aid[i].priorite;


		//APEMV_UI_MessageDisplayDelayHexa(aid,5, 2);
		//APEMV_UI_MessageDisplayDelayHexa(l_uc_data,5, 2);


		if(memcmp(aid,l_uc_data,5+Fic_EMV_AID.tab_aid[i].Lg_pix)==0)
		{
			memcpy((uchar *)St_Aid,(uchar *)&Fic_EMV_AID.tab_aid[i],sizeof(Struct_EMV_AID));

			Data_Last_TRS_Emv_Rej_tmp.Aid[0]=5+Fic_EMV_AID.tab_aid[i].Lg_pix;
			memcpy(Data_Last_TRS_Emv_Rej_tmp.Aid+1,aid,Data_Last_TRS_Emv_Rej_tmp.Aid[0]);
			////APEMV_UI_MessageDisplayDelay("Rech_Aid.OK.00", 2);
			return OK;
		}
		i+=1;
	}
	////APEMV_UI_MessageDisplayDelay("Rech_Aid.KO.00", 2);
	return KO;
}
/*-----------------------------------------------------------------
 * Fonction   :
 * objet      :
 * Parametre  : Aucun
 * Retour     : Aucun
 *-----------------------------------------------------------------
*/
uint8 Rech_TAC_Aid(Struct_EMV_TAC *St_TAC,US lgAid,PUC vAid)
{
	uint16	Nb_Max_TACs;
	uint8		aid[17];
	uint16	Lg_Aid,i;


	Nb_Max_TACs = Fic_EMV_TAC.Indice_fin;
	i=0;
	while(i<Nb_Max_TACs)
	{
		Lg_Aid = Fic_EMV_TAC.tab_emv_tac[i].Lg_PIX+5;
		memcpy(aid,Fic_EMV_TAC.tab_emv_tac[i].RID,5);
		memcpy(&aid[5],Fic_EMV_TAC.tab_emv_tac[i].PIX,Fic_EMV_TAC.tab_emv_tac[i].Lg_PIX);

		if(memcmp(aid,vAid,5+Fic_EMV_TAC.tab_emv_tac[i].Lg_PIX)==0)
		{
			memcpy((uchar *)St_TAC,(uchar *)&Fic_EMV_TAC.tab_emv_tac[i],sizeof(Struct_EMV_TAC));
			Data_Last_TRS_Emv_Rej_tmp.Idx_Emv_Tac=i+1;
			return OK;
		}
		i+=1;
	}
	//if(g_trace)Os__xprint("Erreur Param.");
	//if(g_trace)Os__xprint("TACs Absent");
	return KO;
}
/*-----------------------------------------------------------------
 * Fonction   :
 * objet      :
 * Parametre  : Aucun
 * Retour     : Aucun
 *-----------------------------------------------------------------
*/
uint8 Rech_TDOL_Aid(Struct_EMV_TDOL *St_TDOL,US lgAid,PUC vAid)
{
	uint16		Nb_Max_TDOLs;
	uint8		rid[5];
	uint16		i;

	//char 	trace[125];

	Nb_Max_TDOLs = Fic_EMV_TDOL.Indice_fin;
	i=0;

	//sprintf(trace,"%d",Fic_EMV_TDOL.Indice_fin);
	////APEMV_UI_MessageDisplayDelay(trace, 5);

	while(i<Nb_Max_TDOLs)
	{
		memcpy(rid,Fic_EMV_TDOL.tab_emv_tdol[i].RID,5);


		//APEMV_UI_MessageDisplayDelayHexa(vAid,5, 2);
		//APEMV_UI_MessageDisplayDelayHexa(rid,5, 2);

		if(memcmp(rid,vAid,5)==0)
		{
			memcpy((uchar *)St_TDOL,(uchar *)&Fic_EMV_TDOL.tab_emv_tdol[i],sizeof(Struct_EMV_TDOL));
			Data_Last_TRS_Emv_Rej_tmp.Idx_Emv_TDOL=i+1;
			////APEMV_UI_MessageDisplayDelay("Rech_TDOL_Aid.OK", 3);
			return OK;
		}
		i+=1;
	}
	////APEMV_UI_MessageDisplayDelay("Rech_TDOL_Aid.KO", 3);
	return KO;
}
/*-----------------------------------------------------------------
 * Fonction   :
 * objet      :
 * Parametre  : Aucun
 * Retour     : Aucun
 *-----------------------------------------------------------------
*/
uint8 Rech_DDOL_Aid(Struct_EMV_DDOL *St_DDOL,US lgAid,PUC vAid)
{
	uint16	Nb_Max_DDOLs;
	uint8		aid[17];
	uint16	Lg_Aid,i;

	Nb_Max_DDOLs = Fic_EMV_DDOL.Indice_fin;
	i=0;
	while(i<Nb_Max_DDOLs)
	{
		Lg_Aid = Fic_EMV_DDOL.tab_emv_ddol[i].Lg_PIX+5;
		memcpy(aid,Fic_EMV_DDOL.tab_emv_ddol[i].RID,5);
		memcpy(&aid[5],Fic_EMV_DDOL.tab_emv_ddol[i].PIX,Fic_EMV_DDOL.tab_emv_ddol[i].Lg_PIX);

		if(memcmp(aid,vAid,5+Fic_EMV_DDOL.tab_emv_ddol[i].Lg_PIX)==0)
		{
			memcpy((uchar *)St_DDOL,(uchar *)&Fic_EMV_DDOL.tab_emv_ddol[i],sizeof(Struct_EMV_DDOL));
			Data_Last_TRS_Emv_Rej_tmp.Idx_Emv_DDOL=i+1;
			return OK;
		}
		i+=1;
	}
	if(g_trace)Os__xprint("Erreur Param.");
	if(g_trace)Os__xprint("DDOL Absent");
	return KO;
}
/*-----------------------------------------------------------------
 * Fonction   :
 * objet      :
 * Parametre  : Aucun
 * Retour     : Aucun
 *-----------------------------------------------------------------
*/
uint8 Rech_Rand_Call_Aid(uint8 *pTraCurrCod,Struct_EMV_rand *St_EMV_rand,US lgAid,PUC vAid)
{
	uint16	Nb_Max;
	uint8	aid[17];
	uint16	Lg_Aid,i;
	uint8	Devise[4];


	//eft30printhexKamal(vAid,lgAid);
	//eft30printhexKamal(pTraCurrCod,2);

	Nb_Max = Fic_EMV_rand.Indice_fin;
	i=0;
	while(i<Nb_Max)
	{
		//eft30printhexKamal(&Fic_EMV_rand.tab_emv_rand[i],sizeof(Struct_EMV_rand));

		Lg_Aid = Fic_EMV_rand.tab_emv_rand[i].Lg_PIX+5;
		memcpy(aid,Fic_EMV_rand.tab_emv_rand[i].RID,5);
		memcpy(&aid[5],Fic_EMV_rand.tab_emv_rand[i].PIX,Fic_EMV_rand.tab_emv_rand[i].Lg_PIX);
		memset(Devise,0,4);
		Devise[0] = ((0x00 & 0x0f) * 16) + (Fic_EMV_rand.tab_emv_rand[i].devise[0] & 0x0f);
		Devise[1] = ((Fic_EMV_rand.tab_emv_rand[i].devise[1] & 0x0f) * 16) + (Fic_EMV_rand.tab_emv_rand[i].devise[2] & 0x0f);

		//eft30printhexKamal(Devise,2);
		//eft30printhexKamal(aid,7);
		if(
			(memcmp(aid,vAid,5+Fic_EMV_rand.tab_emv_rand[i].Lg_PIX)==0)||
			(memcmp(Devise,pTraCurrCod,2)==0)
			)
		{
			memcpy((uchar *)St_EMV_rand,(uchar *)&Fic_EMV_rand.tab_emv_rand[i],sizeof(Struct_EMV_rand));
			Data_Last_TRS_Emv_Rej_tmp.Idx_Emv_RAND=i+1;
			return OK;
		}
		i+=1;
	}
	if(g_trace)Os__xprint("Erreur Param.");
	if(g_trace)Os__xprint("RAND CALL Absent");
	return KO;
}
/*-----------------------------------------------------------------
 * Fonction   :
 * objet      :
 * Parametre  : Aucun
 * Retour     : Aucun
 *-----------------------------------------------------------------
*/
uint8 Rech_TCC_Aid(Struct_EMV_TCC *St_EMV_TCC,US lgAid,PUC vAid)
{
	uint16	Nb_Max;
	uint8	aid[17];
	uint16	Lg_Aid,i;

	Nb_Max = Fic_EMV_TCC.Indice_fin;
	i=0;

	while(i<Nb_Max)
	{
		Lg_Aid = Fic_EMV_TCC.tab_emv_tcc[i].Lg_PIX+5;
		memcpy(aid,Fic_EMV_TCC.tab_emv_tcc[i].RID,5);
		memcpy(&aid[5],Fic_EMV_TCC.tab_emv_tcc[i].PIX,Fic_EMV_TCC.tab_emv_tcc[i].Lg_PIX);

		if(memcmp(aid,vAid,Lg_Aid)==0)
		{
			memcpy((uchar *)St_EMV_TCC,(uchar *)&Fic_EMV_TCC.tab_emv_tcc[i],sizeof(Struct_EMV_TCC));
			return OK;
		}
		i+=1;
	}
	return KO;
}
/*-----------------------------------------------------------------
 * Fonction   :
 * objet      :
 * Parametre  : Aucun
 * Retour     : Aucun
 *-----------------------------------------------------------------
*/
uint8 RechModulusKey(PUC vAid,UC vIndexIcc,PUC vModulus,unsigned short * lgModulus,PUC vExponent,unsigned short * lgExponent)
{
	uint16	i = 0;

	while(i<Fic_EMV_Keys.Indice_fin)
	{
		if(memcmp(Fic_EMV_Keys.tab_emv_key[i].RID,vAid,5)==0)
		{
			if(Fic_EMV_Keys.tab_emv_key[i].Key_Index == vIndexIcc)
			{
				memcpy(vModulus,Fic_EMV_Keys.tab_emv_key[i].Key_mod,Fic_EMV_Keys.tab_emv_key[i].Lg_modulo);
				*lgModulus= (unsigned short)Fic_EMV_Keys.tab_emv_key[i].Lg_modulo;
				memcpy(vExponent,Fic_EMV_Keys.tab_emv_key[i].Key_Exp,Fic_EMV_Keys.tab_emv_key[i].Lg_Exp);
				*lgExponent= (unsigned short)Fic_EMV_Keys.tab_emv_key[i].Lg_Exp;
				return OK;
			}
		}
		i+=1;
	}
	return KO;
}


L EmvReadIccData(US i_us_tag, PUS o_pus_length, PUC o_puc_data)
{
    unsigned short found;
    DataElement elt;
	TLV_TREE_NODE  hTree;
	
	char	Message[512];	//Test17
	int		c;				//Test17

	hTree = Engine_GetDataElement(i_us_tag);
	found = CUSERV_Extract_Element_TLV_Tree(&hTree, i_us_tag, &elt);

	//found = PAR_ReadParameter(i_us_tag, (DataElementExt *)&elt);

	if (found)
	{
		*o_pus_length= elt.length;
		memcpy (o_puc_data, elt.ptValue,*o_pus_length);
		
		
		sprintf(Message,"[");
		for(c =0;c<elt.length;c++)
		{
			sprintf(Message+1+2*c,"%02X",o_puc_data[c]);
		}
		sprintf(Message+1+(2*c),"]");
		
		if(i_us_tag == TAG_TRANSACTION_TYPE)
		{
			Os__xprinta("Message");
			Os__xprinta(Message);
		}

		////APEMV_UI_MessageDisplayDelay ((char *)Message, 3); //Test17
		
		return TRUE;
	}
	return FALSE;
}


L APEMV_UI_MessageDisplayDelayHexa(char* Messagehexa,char lc, int duration)
{
	char	Message[256];
	char 	c;

	sprintf(Message,"[");

	for(c =0;c<lc;c++)
	{
		sprintf(Message+1+2*c,"%02X",Messagehexa[c]);
	}
	sprintf(Message+1+(2*c),"]");

	APEMV_UI_MessageDisplayDelay ((char *)Message, duration);

	return TRUE;
}


L TVRTSI()
{
	US	i_us_tag;

	US o_pus_length;
	UC o_puc_data[256];

    unsigned short found;
    DataElement elt;
	TLV_TREE_NODE  hTree;

	char	Message[56];	//Test17
	int		c;				//Test17


	i_us_tag = TAG_TVR;

	hTree = Engine_GetDataElement(i_us_tag);
	found = CUSERV_Extract_Element_TLV_Tree(&hTree, i_us_tag, &elt);

	if (found)
	{
		o_pus_length= elt.length;
		memcpy (o_puc_data, elt.ptValue,o_pus_length);


		sprintf(Message,"[");
		for(c =0;c<elt.length;c++)
		{
			sprintf(Message+1+2*c,"%02X",o_puc_data[c]);
		}
		sprintf(Message+1+(2*c),"]");

		//APEMV_UI_MessageDisplayDelay ((char *)Message, 3);
		Os__xprinta(Message);
	}

	i_us_tag = TAG_TSI;

	hTree = Engine_GetDataElement(i_us_tag);
	found = CUSERV_Extract_Element_TLV_Tree(&hTree, i_us_tag, &elt);

	if (found)
	{
		o_pus_length= elt.length;
		memcpy (o_puc_data, elt.ptValue,o_pus_length);


		sprintf(Message,"[");
		for(c =0;c<elt.length;c++)
		{
			sprintf(Message+1+2*c,"%02X",o_puc_data[c]);
		}
		sprintf(Message+1+(2*c),"]");

		//APEMV_UI_MessageDisplayDelay ((char *)Message, 3);
		Os__xprinta(Message);
	}

	return 0;
}
