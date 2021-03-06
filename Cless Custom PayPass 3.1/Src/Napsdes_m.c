#include "SDK_Naps.h"

/*----------------------------------------------------------------

		TABLES DE CHIFFREMENT

-----------------------------------------------------------------*/
/* table 1 : permutation initiale */
  unsigned char T1[] = {
	57,49,41,33,25,17, 9, 1,
	59,51,43,35,27,19,11, 3,
	61,53,45,37,29,21,13, 5,
	63,55,47,39,31,23,15, 7,
	56,48,40,32,24,16, 8, 0,
	58,50,42,34,26,18,10, 2,
	60,52,44,36,28,20,12, 4,
	62,54,46,38,30,22,14, 6
};

/* table 2 : permutation finale */
  unsigned char T2[] = {
	39, 7,47,15,55,23,63,31,
	38, 6,46,14,54,22,62,30,
	37, 5,45,13,53,21,61,29,
	36, 4,44,12,52,20,60,28,
	35, 3,43,11,51,19,59,27,
	34, 2,42,10,50,18,58,26,
	33, 1,41, 9,49,17,57,25,
	32, 0,40, 8,48,16,56,24
};

/* table 3 : fonction d'expansion E  ( valeur - 1 ) */
   unsigned char T3[] = {
	31, 0, 1, 2, 3, 4,
	 3, 4, 5, 6, 7, 8,
	 7, 8, 9,10,11,12,
	11,12,13,14,15,16,
	15,16,17,18,19,20,
	19,20,21,22,23,24,
	23,24,25,26,27,28,
	27,28,29,30,31, 0
};

/* table 5 : fonction de permutation P */
  unsigned char T0[] = {
	15, 6,19,20,
	28,11,27,16,
	 0,14,22,25,
	 4,17,30, 9,
	 1, 7,23,13,
	31,26, 2, 8,
	18,12,29, 5,
	21,10, 3,24
};

/* table 7 : table de choix 1 */
  unsigned char T7_1_2[56] =
{
	56,48,40,32,24,16, 8,
	 0,57,49,41,33,25,17,
	 9, 1,58,50,42,34,26,
	18,10, 2,59,51,43,35,

	62,54,46,38,30,22,14,
	 6,61,53,45,37,29,21,
	13, 5,60,52,44,36,28,
	20,12, 4,27,19,11, 3
};

/* table 8 : table de d?calage */
  unsigned char T8[] =
{
	 0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0
};

/* table 9 : table de choix 2 */
  unsigned char T9[] =
{
	13,16,10,23, 0, 4,
	 2,27,14, 5,20, 9,
	22,18,11, 3,25, 7,
	15, 6,26,19,12, 1,
	40,51,30,36,46,54,
	29,39,50,44,32,47,
	43,48,38,55,33,52,
	45,41,49,35,28,31
};

/* table 6 : s?lection de fonctions S1 ? S8 */
//   unsigned char T6[8][64] =
  unsigned char T6[][64] =
{
/* S1 */
	{
    14, 4,13, 1, 2,15,11, 8, 3,10, 6,12, 5, 9, 0, 7,
	 0,15, 7, 4,14, 2,13, 1,10, 6,12,11, 9, 5, 3, 8,
	 4, 1,14, 8,13, 6, 2,11,15,12, 9, 7, 3,10, 5, 0,
	15,12, 8, 2, 4, 9, 1, 7, 5,11, 3,14,10, 0, 6,13
    },

/* S2 */
	{
    15, 1, 8,14, 6,11, 3, 4, 9, 7, 2,13,12, 0, 5,10,
	 3,13, 4, 7,15, 2, 8,14,12, 0, 1,10, 6, 9,11, 5,
	 0,14, 7,11,10, 4,13, 1, 5, 8,12, 6, 9, 3, 2,15,
	13, 8,10, 1, 3,15, 4, 2,11, 6, 7,12, 0, 5,14, 9
    },

/* S3 */
	{
    10, 0, 9,14, 6, 3,15, 5, 1,13,12, 7,11, 4, 2, 8,
	13, 7, 0, 9, 3, 4, 6,10, 2, 8, 5,14,12,11,15, 1,
	13, 6, 4, 9, 8,15, 3, 0,11, 1, 2,12, 5,10,14, 7,
	 1,10,13, 0, 6, 9, 8, 7, 4,15,14, 3,11, 5, 2,12
    },

/* S4 */
	{
	 7,13,14, 3, 0, 6, 9,10, 1, 2, 8, 5,11,12, 4,15,
	13, 8,11, 5, 6,15, 0, 3, 4, 7, 2,12, 1,10,14, 9,
	10, 6, 9, 0,12,11, 7,13,15, 1, 3,14, 5, 2, 8, 4,
	 3,15, 0, 6,10, 1,13, 8, 9, 4, 5,11,12, 7, 2,14
    },

/* S5 */
	{
     2,12, 4, 1, 7,10,11, 6, 8, 5, 3,15,13, 0,14, 9,
	14,11, 2,12, 4, 7,13, 1, 5, 0,15,10, 3, 9, 8, 6,
	 4, 2, 1,11,10,13, 7, 8,15, 9,12, 5, 6, 3, 0,14,
	11, 8,12, 7, 1,14, 2,13, 6,15, 0, 9,10, 4, 5, 3
    },

/* S6 */
	{
    12, 1,10,15, 9, 2, 6, 8, 0,13, 3, 4,14, 7, 5,11,
	10,15, 4, 2, 7,12, 9, 5, 6, 1,13,14, 0,11, 3, 8,
	 9,14,15, 5, 2, 8,12, 3, 7, 0, 4,10, 1,13,11, 6,
	 4, 3, 2,12, 9, 5,15,10,11,14, 1, 7, 6, 0, 8,13
    },

/* S7 */
	{
     4,11, 2,14,15, 0, 8,13, 3,12, 9, 7, 5,10, 6, 1,
	13, 0,11, 7, 4, 9, 1,10,14, 3, 5,12, 2,15, 8, 6,
	 1, 4,11,13,12, 3, 7,14,10,15, 6, 8, 0, 5, 9, 2,
	 6,11,13, 8, 1, 4,10, 7, 9, 5, 0,15,14, 2, 3,12
    },

/* S8 */
	{
    13, 2, 8, 4, 6,15,11, 1,10, 9, 3,14, 5, 0,12, 7,
	 1,15,13, 8,10, 3, 7, 4,12, 5, 6,11, 0,14, 9, 2,
	 7,11, 4, 1, 9,12,14, 2, 0, 6,10,13,15, 3, 5, 8,
	 2, 1,14, 7, 4,10, 8,13,15,12, 9, 0, 3, 5, 6,11
    }
};

/* Table pour eclatement des valeurs precedentes */
// unsigned char TE[16][4] =
 unsigned char TE[][4] =
{
   {0,0,0,0},
   {0,0,0,1},
   {0,0,1,0},
   {0,0,1,1},
   {0,1,0,0},
   {0,1,0,1},
   {0,1,1,0},
   {0,1,1,1},
   {1,0,0,0},
   {1,0,0,1},
   {1,0,1,0},
   {1,0,1,1},
   {1,1,0,0},
   {1,1,0,1},
   {1,1,1,0},
   {1,1,1,1}
};

/*---------------------------------------------------------------
		      CLES
---------------------------------------------------------------*/
/*----------------------------------------------------------------

   Figure 3. Key Schedule Calculation.

-----------------------------------------------------------------*/
void Ks(unsigned char *Key, unsigned char Kn[16][48])
{
     unsigned char cd[56];

     unsigned char zt[60] ;

     int n;
     unsigned char tmp11, tmp12, tmp21, tmp22;
     int i;
     unsigned char *Knn;

//	ShowTrace("KS0");
	// choix 1 
	for (i = 0; i < 56; i++)
	{
		cd[i] = Key[T7_1_2[i]];
	}

	for (n = 0; n < 16; n++)
	{
		// rotation ? gauche du vecteur en fonction de l'indice 
		if (T8[n] == 0)
		{
			tmp11 = cd[0];
			tmp21 = cd[28];
                        memcpy( zt , &cd[1] , 55 );
                        memcpy( cd , zt     , 55 );

			cd[27] = tmp11;
			cd[55] = tmp21;
		}
		else
		{
			tmp11 = cd[0];
			tmp12 = cd[1];
			tmp21= cd[28];
			tmp22 = cd[29];

                        memcpy( zt , &cd[2] , 54 );
						memcpy( cd , zt     , 54 );

			cd[26] = tmp11;
			cd[27] = tmp12;
			cd[54] = tmp21;
			cd[55] = tmp22;
		}
                // choix 2 
		Knn = Kn[n];
		for (i = 0; i < 48; i++)
		{
			Knn[i] = cd[T9[i]];
		}
	}
//	ShowTrace("KS1");
}

/*----------------------------------------------------------------

	 Figure 2. Calculation of f(R, K)

-----------------------------------------------------------------*/
void fonction(unsigned char *Knn, unsigned char *r, unsigned char *s)
{
	// n est l'indice de 1 a 16 pour choisir la cle
	//   r est R.
	//   s est le r?sultat. 

     unsigned char x[32];
     unsigned long *px;
     int i, l;
     unsigned char c;
     unsigned char t;
     unsigned char Tmp_Chaine[5];

	// fonction E     //  + OU exclusif 
	// S?lection Sn 
	//ShowTrace("FC0");
    for (i = 0, l = 0, px = (unsigned long *) x; i < 8;)
	{
		//ShowTrace("FC1");
		c = 32 * (r[T3[l]] ^ Knn[l]);
		//ShowTrace("FC2");
		l++;
		c += 8 * (r[T3[l]] ^ Knn[l]);
		//ShowTrace("FC3");
		l++;
		c += 4 * (r[T3[l]] ^ Knn[l]);
		//ShowTrace("FC4");
        l++;
		c += 2 * (r[T3[l]] ^ Knn[l]);
		//ShowTrace("FC5");
        l++;
		c += 1 * (r[T3[l]] ^ Knn[l]);
		//ShowTrace("FC6");
		l++;
		c += 16 * (r[T3[l]] ^ Knn[l]);
		//ShowTrace("FC7");
		l++;
        // extraction de la valeur 
        t = T6[i][c];

        //DisplaySmall(0, 0, "funct.1:");

		memcpy(&x[i*4],TE[t],4);

		//ShowTrace("FC8");
		i++;
		// Eclatement de la valeur; 4 bits -> 4 bytes 
		//*px = *(long *)TE[t];
		//scanf(px,"%lu",(long *)TE[t]);

		//DisplaySmall(0, 0, "funct.2:");

	//	memset(Tmp_Chaine,0,sizeof(Tmp_Chaine));
	//	memcpy(Tmp_Chaine,TE[t],4);
	//	*px = *((unsigned long *)Tmp_Chaine);
		
		//ShowTrace("FC9");
        px++;
		//ShowTrace("FC10");

    
	
	}
	// fonction P 
	//ShowTrace("FC11");
	for (i = 0; i < 32; i++)
	{
		s[i] = x[T0[i]];
	}
	//ShowTrace("FC12");
}

/*----------------------------------------------------------------
 Permutations initiale et finale
-----------------------------------------------------------------*/
void permutation(unsigned char *org, unsigned char *tab)
{
     unsigned char tmp[64];
     int i;


//	ShowTrace("P0");
	memcpy(tmp, org, 64);
	for (i = 0; i < 64; i++)
	{
		org[i] = tmp[tab[i]];
	}
//	ShowTrace("P1");
}

/*----------------------------------------------------------------

       Figure 1. Enciphering Computation.

-----------------------------------------------------------------*/
void chiffrement(unsigned char *xi, unsigned char *xo, unsigned char Kn[16][48])
{
     unsigned char r[32], l[32];
     unsigned char rp[32], lp[32];

     int i;
	 int n;

//	ShowTrace("C0");
	memcpy(l, &xi[0], 32);
	memcpy(r, &xi[32], 32);

	for (n = 0; n < 16; n++)
	{
		//ShowTrace("C1");
		memcpy(lp, r, 32);

		fonction(Kn[n], r, rp);
		//ShowTrace("C2");
		for (i = 0; i < 32; i++)
		{
			r[i] =( ( l[i]) ^ (rp[i] )  ) ;
		}
		//ShowTrace("C3");
		memcpy(l, lp, 32);
	}
	memcpy(&xo[0], r, 32);
	memcpy(&xo[32], l, 32);
//	ShowTrace("C3");

}


/*----------------------------------------------------------------

       Deciphering Computation.

-----------------------------------------------------------------*/

void dechiffrement(unsigned char *xi, unsigned char *xo, unsigned char Kn[16][48])
{
     unsigned char r[32], l[32], rp[32], lp[32];

     int i;
     int n;

	memcpy(l, &xi[0], 32);
	memcpy(r, &xi[32], 32);

	for (n = 0; n < 16; n++)
	{
		memcpy(lp, r, 32);
		fonction(Kn[15 - n], r, rp);
		for (i = 0; i < 32; i++)
		{
			r[i] =( ( l[i] ) ^ ( rp[i] )) ;
		}
		memcpy(l, lp, 32);
	}

	memcpy(&xo[0], r, 32);
	memcpy(&xo[32], l, 32);
}

/*----------------------------------------------------------------
  Eclater 64 bits en 64 octets
-----------------------------------------------------------------*/
void eclater(unsigned char *buf_bit, unsigned char *byte)
{
	int i;
	unsigned char m;
	
	//	ShowTrace("EC0");
	for (i = 0; i < 8; i++)
	{
		for (m = 0x80; m != 0;  )
		{
			if ((buf_bit[i] & m) != 0)
				*byte = 1;
			else
				*byte = 0;
			byte++;
			m=m/2 ;
		}
	}
	//	ShowTrace("EC1");
	
}
/*----------------------------------------------------------------
  Compacter 64 octets en 64 bits
-----------------------------------------------------------------*/
void compacter(unsigned char *byte, unsigned char *buf_bit)
{
	int i;
	unsigned char m, n;
	
	//	ShowTrace("CP0");
	for (i = 0; i < 8; i++)
	{
		n = 0;
		for (m = 0x80; m != 0; )
		{
			if (*byte++)
				n = n | m;
			m=m/2 ;
			
		}
		buf_bit[i] = n;
	}
	//	ShowTrace("CP1");
}


/*----------------------------------------------------------------
  D E S
-----------------------------------------------------------------*/
void des(unsigned char *binput, unsigned char *boutput, unsigned char *bkey)
{
     unsigned char Key[64];
     unsigned char input[64], // data input 
              output[64]; // data output 
     unsigned char Kn[16][48];

	eclater(binput, input);
	eclater(bkey, Key);
	Ks(Key, Kn);

	permutation(input, T1);
	chiffrement(input, output, Kn);

	permutation(output, T2);
	compacter(output, boutput);
}

void desm1(unsigned char *binput, unsigned char *boutput, unsigned char *bkey)
{
     unsigned char Key[64];
     unsigned char input[64], // data input 
              output[64]; // data output 
     unsigned char Kn[16][48];


	eclater(binput, input);
	eclater(bkey, Key);
	Ks(Key, Kn);

	permutation(input, T1);
	dechiffrement(input, output, Kn);
	permutation(output, T2);
	compacter(output, boutput);

}

void Calcul_Mac(unsigned char *binput, UL Len_Mess, unsigned char *boutput, unsigned char *bkey)
{
	unsigned char Input_Data[8],Output_Data[8];
	unsigned long i,j,Lg_mess;
	
//	ShowTrace("CM0");
	if(Len_Mess%8)
		Lg_mess = (Len_Mess/8 +1)*8;
	else
		Lg_mess = Len_Mess;
	
//	ShowTrace("CM1");
	memset(Output_Data,'\0',8);
	memcpy(Input_Data,binput,8);
//	ShowTrace("CM2");
	des(Input_Data,Output_Data,bkey);
//	ShowTrace("CM3");
	for(i=8;(i<Lg_mess);i+=8)
	{
//		ShowTrace("CM4");
		memcpy(Input_Data,binput+i,8);
		for(j=0; j<8; j++)
		{
//			ShowTrace("CM5");
			Input_Data[j] = Input_Data[j] ^ Output_Data[j];
		}
		des(Input_Data,Output_Data,bkey);
	}
//	ShowTrace("CM6");
	hex_asc(boutput,Output_Data,8);
}

void Crypto__3DesEnc(unsigned char *Input_Data,unsigned char *Output_Data,unsigned char *bkey)
{
   unsigned char Output1[9],Output2[9];

   memset(Output1,0,sizeof(Output1));
   memset(Output2,0,sizeof(Output2));

   des(Input_Data,Output1,bkey);
   desm1(Output1,Output2,bkey+8);
   des(Output2,Output_Data,bkey+16);
}

void Crypto__3DesDec(unsigned char *Input_Data,unsigned char *Output_Data,unsigned char *bkey)
{
   unsigned char Output1[9],Output2[9];

   memset(Output1,0,sizeof(Output1));
   memset(Output2,0,sizeof(Output2));

   desm1(Input_Data,Output1,bkey+16);
   des(Output1,Output2,bkey+8);
   desm1(Output2,Output_Data,bkey);

}

/*----------------------------------------------------------------
 Fin
-----------------------------------------------------------------*/
