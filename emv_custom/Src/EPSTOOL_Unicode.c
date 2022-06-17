/**
* \file
* \brief This module implements functions to convert ISO8859 strings into UTF-8 strings.
*
* \author Ingenico
* \author Copyright (c) 2012 Ingenico, 28-32, boulevard de Grenelle,\n
* 75015 Paris, France, All Rights Reserved.
*
* \author Ingenico has intellectual property rights relating to the technology embodied\n
* in this software. In particular, and without limitation, these intellectual property rights may\n
* include one or more patents.\n
* This software is distributed under licenses restricting its use, copying, distribution, and\n
* and decompilation. No part of this software may be reproduced in any form by any means\n
* without prior written authorisation of Ingenico.
**/

/////////////////////////////////////////////////////////////////
//// Includes ///////////////////////////////////////////////////

#include "sdk.h" // IngeDev automatic refactoring - 2014/04/29 13:46:13 - Replace '#include "SDK30.h"' to '#include "sdk.h"'
#include "GTL_ASSERT.h"

#include "EPSTOOL_Unicode.h"

//// Macros & preprocessor definitions //////////////////////////

#define EPSTOOL_UNICODE_SWAP_CHAR		(0xFFFE)		//!< UNICODE character ZERO WIDTH NO-BREAK SPACE.
#define EPSTOOL_UNICODE_MAX_TABLE		(96)			//!< Size of an ISO8859 to UNICODE conversion table.

//// Types //////////////////////////////////////////////////////

//// Static function definitions ////////////////////////////////

//// Global variables ///////////////////////////////////////////

#define SWPCHR	EPSTOOL_UNICODE_SWAP_CHAR				//!< UNICODE character ZERO WIDTH NO-BREAK SPACE.

//! \brief Conversion table from ISO8859 2-16 into UNICODE.
static const unsigned short __EPSTOOL_Unicode_Table[EPSTOOL_UNICODE_ISO_8859_16-EPSTOOL_UNICODE_ISO_8859_1][EPSTOOL_UNICODE_MAX_TABLE] =
{
	/** ISO  2 */{0xA0,  0x104, 0x2D8, 0x141, 0xA4,  0x13D, 0x15A, 0xA7,  0xA8,  0x160, 0x15E, 0x164, 0x179, 0xAD,  0x17D, 0x17B, 0xB0,  0x105, 0x2DB, 0x142, 0xB4,  0x13E, 0x15B, 0x2C7, 0xB8,  0x161, 0x15F, 0x165, 0x17A, 0x2DD, 0x17E, 0x17C, 0x154, 0xC1,  0xC2,  0x102, 0xC4,  0x139, 0x106, 0xC7,  0x10C, 0xC9,  0x118, 0xCB,  0x11A, 0xCD,  0xCE,  0x10E, 0x110, 0x143, 0x147, 0xD3,  0xD4,  0x150, 0xD6,  0xD7,  0x158, 0x16E, 0xDA,  0x170, 0xDC,  0xDD,  0x162, 0xDF,  0x155, 0xE1,  0xE2,  0x103, 0xE4,  0x13A, 0x107, 0xE7,  0x10D, 0xE9,  0x119, 0xEB,  0x11B, 0xED,  0xEE,  0x10F, 0x111, 0x144, 0x148, 0xF3,  0xF4,  0x151, 0xF6,  0xF7,  0x159, 0x16F, 0xFA,  0x171, 0xFC,  0xFD,  0x163, 0x2D9},
	/** ISO  3 */{0xA0,  0x126, 0x2D8, 0xA3,  0xA4,  SWPCHR,0x124, 0xA7,  0xA8,  0x130, 0x15E, 0x11E, 0x134, 0xAD,  SWPCHR,0x17B, 0xB0,  0x127, 0xB2,  0xB3,  0xB4,  0xB5,  0x125, 0xB7,  0xB8,  0x131, 0x15F, 0x11F, 0x135, 0xBD,  SWPCHR,0x17C, 0xC0,  0xC1,  0xC2,  SWPCHR,0xC4,  0x10A, 0x108, 0xC7,  0xC8,  0xC9,  0xCA,  0xCB,  0xCC,  0xCD,  0xCE,  0xCF,  SWPCHR,0xD1,  0xD2,  0xD3,  0xD4,  0x120, 0xD6,  0xD7,  0x11C, 0xD9,  0xDA,  0xDB,  0xDC,  0x16C, 0x15C, 0xDF,  0xE0,  0xE1,  0xE2,  SWPCHR,0xE4,  0x10B, 0x109, 0xE7,  0xE8,  0xE9,  0xEA,  0xEB,  0xEC,  0xED,  0xEE,  0xEF,  SWPCHR,0xF1,  0xF2,  0xF3,  0xF4,  0x121, 0xF6,  0xF7,  0x11D, 0xF9,  0xFA,  0xFB,  0xFC,  0x16D, 0x15D, 0x2D9},
	/** ISO  4 */{0xA0,  0x104, 0x138, 0x156, 0xA4,  0x128, 0x13B, 0xA7,  0xA8,  0x160, 0x112, 0x122, 0x166, 0xAD,  0x17D, 0xAF,  0xB0,  0x105, 0x2DB, 0x157, 0xB4,  0x129, 0x13C, 0x2C7, 0xB8,  0x161, 0x113, 0x123, 0x167, 0x14A, 0x17E, 0x14B, 0x100, 0xC1,  0xC2,  0xC3,  0xC4,  0xC5,  0xC6,  0x12E, 0x10C, 0xC9,  0x118, 0xCB,  0x116, 0xCD,  0xCE,  0x12A, 0x110, 0x145, 0x14C, 0x136, 0xD4,  0xD5,  0xD6,  0xD7,  0xD8,  0x172, 0xDA,  0xDB,  0xDC,  0x168, 0x16A, 0xDF,  0x101, 0xE1,  0xE2,  0xE3,  0xE4,  0xE5,  0xE6,  0x12F, 0x10D, 0xE9,  0x119, 0xEB,  0x117, 0xED,  0xEE,  0x12B, 0x111, 0x146, 0x14D, 0x137, 0xF4,  0xF5,  0xF6,  0xF7,  0xF8,  0x173, 0xFA,  0xFB,  0xFC,  0x169, 0x16B, 0x2D9},
	/** ISO  5 */{0xA0,  0x401, 0x402, 0x403, 0x404, 0x405, 0x406, 0x407, 0x408, 0x409, 0x40A, 0x40B, 0x40C, 0xAD,  0x40E, 0x40F, 0x410, 0x411, 0x412, 0x413, 0x414, 0x415, 0x416, 0x417, 0x418, 0x419, 0x41A, 0x41B, 0x41C, 0x41D, 0x41E, 0x41F, 0x420, 0x421, 0x422, 0x423, 0x424, 0x425, 0x426, 0x427, 0x428, 0x429, 0x42A, 0x42B, 0x42C, 0x42D, 0x42E, 0x42F, 0x430, 0x431, 0x432, 0x433, 0x434, 0x435, 0x436, 0x437, 0x438, 0x439, 0x43A, 0x43B, 0x43C, 0x43D, 0x43E, 0x43F, 0x440, 0x441, 0x442, 0x443, 0x444, 0x445, 0x446, 0x447, 0x448, 0x449, 0x44A, 0x44B, 0x44C, 0x44D, 0x44E, 0x44F, 0x2116,0x451, 0x452, 0x453, 0x454, 0x455, 0x456, 0x457, 0x458, 0x459, 0x45A, 0x45B, 0x45C, 0xA7,  0x45E, 0x45F},
	/** ISO  6 */{0xA0,  SWPCHR,SWPCHR,SWPCHR,0xA4,  SWPCHR,SWPCHR,SWPCHR,SWPCHR,SWPCHR,SWPCHR,SWPCHR,0x60C, 0xAD,  SWPCHR,SWPCHR,SWPCHR,SWPCHR,SWPCHR,SWPCHR,SWPCHR,SWPCHR,SWPCHR,SWPCHR,SWPCHR,SWPCHR,SWPCHR,0x61B, SWPCHR,SWPCHR,SWPCHR,0x61F, SWPCHR,0x621, 0x622, 0x623, 0x624, 0x625, 0x626, 0x627, 0x628, 0x629, 0x62A, 0x62B, 0x62C, 0x62D, 0x62E, 0x62F, 0x630, 0x631, 0x632, 0x633, 0x634, 0x635, 0x636, 0x637, 0x638, 0x639, 0x63A, SWPCHR,SWPCHR,SWPCHR,SWPCHR,SWPCHR,0x640, 0x641, 0x642, 0x643, 0x644, 0x645, 0x646, 0x647, 0x648, 0x649, 0x64A, 0x64B, 0x64C, 0x64D, 0x64E, 0x64F, 0x650, 0x651, 0x652, SWPCHR,SWPCHR,SWPCHR,SWPCHR,SWPCHR,SWPCHR,SWPCHR,SWPCHR,SWPCHR,SWPCHR,SWPCHR,SWPCHR,SWPCHR},
	/** ISO  7 */{0xA0,  0x2018,0x2019,0xA3,  0x20AC,0x20AF,0xA6,  0xA7,  0xA8,  0xA9,  0x37A, 0xAB,  0xAC,  0xAD,  SWPCHR,0x2015,0xB0,  0xB1,  0xB2,  0xB3,  0x384, 0x385, 0x386, 0xB7,  0x388, 0x389, 0x38A, 0xBB,  0x38C, 0xBD,  0x38E, 0x38F, 0x390, 0x391, 0x392, 0x393, 0x394, 0x395, 0x396, 0x397, 0x398, 0x399, 0x39A, 0x39B, 0x39C, 0x39D, 0x39E, 0x39F, 0x3A0, 0x3A1, SWPCHR,0x3A3, 0x3A4, 0x3A5, 0x3A6, 0x3A7, 0x3A8, 0x3A9, 0x3AA, 0x3AB, 0x3AC, 0x3AD, 0x3AE, 0x3AF, 0x3B0, 0x3B1, 0x3B2, 0x3B3, 0x3B4, 0x3B5, 0x3B6, 0x3B7, 0x3B8, 0x3B9, 0x3BA, 0x3BB, 0x3BC, 0x3BD, 0x3BE, 0x3BF, 0x3C0, 0x3C1, 0x3C2, 0x3C3, 0x3C4, 0x3C5, 0x3C6, 0x3C7, 0x3C8, 0x3C9, 0x3CA, 0x3CB, 0x3CC, 0x3CD, 0x3CE, SWPCHR},
	/** ISO  8 */{0xA0,  SWPCHR,0xA2,  0xA3,  0xA4,  0xA5,  0xA6,  0xA7,  0xA8,  0xA9,  0xD7,  0xAB,  0xAC,  0xAD,  0xAE,  0xAF,  0xB0,  0xB1,  0xB2,  0xB3,  0xB4,  0xB5,  0xB6,  0xB7,  0xB8,  0xB9,  0xF7,  0xBB,  0xBC,  0xBD,  0xBE,  SWPCHR,SWPCHR,SWPCHR,SWPCHR,SWPCHR,SWPCHR,SWPCHR,SWPCHR,SWPCHR,SWPCHR,SWPCHR,SWPCHR,SWPCHR,SWPCHR,SWPCHR,SWPCHR,SWPCHR,SWPCHR,SWPCHR,SWPCHR,SWPCHR,SWPCHR,SWPCHR,SWPCHR,SWPCHR,SWPCHR,SWPCHR,SWPCHR,SWPCHR,SWPCHR,SWPCHR,SWPCHR,0x2017,0x5D0, 0x5D1, 0x5D2, 0x5D3, 0x5D4, 0x5D5, 0x5D6, 0x5D7, 0x5D8, 0x5D9, 0x5DA, 0x5DB, 0x5DC, 0x5DD, 0x5DE, 0x5DF, 0x5E0, 0x5E1, 0x5E2, 0x5E3, 0x5E4, 0x5E5, 0x5E6, 0x5E7, 0x5E8, 0x5E9, 0x5EA, SWPCHR,SWPCHR,0x200E,0x200F,SWPCHR},
	/** ISO  9 */{0xA0,  0xA1,  0xA2,  0xA3,  0xA4,  0xA5,  0xA6,  0xA7,  0xA8,  0xA9,  0xAA,  0xAB,  0xAC,  0xAD,  0xAE,  0xAF,  0xB0,  0xB1,  0xB2,  0xB3,  0xB4,  0xB5,  0xB6,  0xB7,  0xB8,  0xB9,  0xBA,  0xBB,  0xBC,  0xBD,  0xBE,  0xBF,  0xC0,  0xC1,  0xC2,  0xC3,  0xC4,  0xC5,  0xC6,  0xC7,  0xC8,  0xC9,  0xCA,  0xCB,  0xCC,  0xCD,  0xCE,  0xCF,  0x11E, 0xD1,  0xD2,  0xD3,  0xD4,  0xD5,  0xD6,  0xD7,  0xD8,  0xD9,  0xDA,  0xDB,  0xDC,  0x130, 0x15E, 0xDF,  0xE0,  0xE1,  0xE2,  0xE3,  0xE4,  0xE5,  0xE6,  0xE7,  0xE8,  0xE9,  0xEA,  0xEB,  0xEC,  0xED,  0xEE,  0xEF,  0x11F, 0xF1,  0xF2,  0xF3,  0xF4,  0xF5,  0xF6,  0xF7,  0xF8,  0xF9,  0xFA,  0xFB,  0xFC,  0x131, 0x15F, 0xFF},
	/** ISO 10 */{0xA0,  0x104, 0x112, 0x122, 0x12A, 0x128, 0x136, 0xA7,  0x13B, 0x110, 0x160, 0x166, 0x17D, 0xAD,  0x16A, 0x14A, 0xB0,  0x105, 0x113, 0x123, 0x12B, 0x129, 0x137, 0xB7,  0x13C, 0x111, 0x161, 0x167, 0x17E, 0x2015,0x16B, 0x14B, 0x100, 0xC1,  0xC2,  0xC3,  0xC4,  0xC5,  0xC6,  0x12E, 0x10C, 0xC9,  0x118, 0xCB,  0x116, 0xCD,  0xCE,  0xCF,  0xD0,  0x145, 0x14C, 0xD3,  0xD4,  0xD5,  0xD6,  0x168, 0xD8,  0x172, 0xDA,  0xDB,  0xDC,  0xDD,  0xDE,  0xDF,  0x101, 0xE1,  0xE2,  0xE3,  0xE4,  0xE5,  0xE6,  0x12F, 0x10D, 0xE9,  0x119, 0xEB,  0x117, 0xED,  0xEE,  0xEF,  0xF0,  0x146, 0x14D, 0xF3,  0xF4,  0xF5,  0xF6,  0x169, 0xF8,  0x173, 0xFA,  0xFB,  0xFC,  0xFD,  0xFE,  0x138},
	/** ISO 11 */{0xA0,  0xE01, 0xE02, 0xE03, 0xE04, 0xE05, 0xE06, 0xE07, 0xE08, 0xE09, 0xE0A, 0xE0B, 0xE0C, 0xE0D, 0xE0E, 0xE0F, 0xE10, 0xE11, 0xE12, 0xE13, 0xE14, 0xE15, 0xE16, 0xE17, 0xE18, 0xE19, 0xE1A, 0xE1B, 0xE1C, 0xE1D, 0xE1E, 0xE1F, 0xE20, 0xE21, 0xE22, 0xE23, 0xE24, 0xE25, 0xE26, 0xE27, 0xE28, 0xE29, 0xE2A, 0xE2B, 0xE2C, 0xE2D, 0xE2E, 0xE2F, 0xE30, 0xE31, 0xE32, 0xE33, 0xE34, 0xE35, 0xE36, 0xE37, 0xE38, 0xE39, 0xE3A, SWPCHR,SWPCHR,SWPCHR,SWPCHR,0xE3F, 0xE40, 0xE41, 0xE42, 0xE43, 0xE44, 0xE45, 0xE46, 0xE47, 0xE48, 0xE49, 0xE4A, 0xE4B, 0xE4C, 0xE4D, 0xE4E, 0xE4F, 0xE50, 0xE51, 0xE52, 0xE53, 0xE54, 0xE55, 0xE56, 0xE57, 0xE58, 0xE59, 0xE5A, 0xE5B, SWPCHR,SWPCHR,SWPCHR,SWPCHR},
	/** ISO 13 */{0xA0,  0x201D,0xA2,  0xA3,  0xA4,  0x201E,0xA6,  0xA7,  0xD8,  0xA9,  0x156, 0xAB,  0xAC,  0xAD,  0xAE,  0xC6,  0xB0,  0xB1,  0xB2,  0xB3,  0x201C,0xB5,  0xB6,  0xB7,  0xF8,  0xB9,  0x157, 0xBB,  0xBC,  0xBD,  0xBE,  0xE6,  0x104, 0x12E, 0x100, 0x106, 0xC4,  0xC5,  0x118, 0x112, 0x10C, 0xC9,  0x179, 0x116, 0x122, 0x136, 0x12A, 0x13B, 0x160, 0x143, 0x145, 0xD3,  0x14C, 0xD5,  0xD6,  0xD7,  0x172, 0x141, 0x15A, 0x16A, 0xDC,  0x17B, 0x17D, 0xDF,  0x105, 0x12F, 0x101, 0x107, 0xE4,  0xE5,  0x119, 0x113, 0x10D, 0xE9,  0x17A, 0x117, 0x123, 0x137, 0x12B, 0x13C, 0x161, 0x144, 0x146, 0xF3,  0x14D, 0xF5,  0xF6,  0xF7,  0x173, 0x142, 0x15B, 0x16B, 0xFC,  0x17C, 0x17E, 0x2019},
	/** ISO 14 */{0xA0,  0x1E02,0x1E03,0xA3,  0x10A, 0x10B, 0x1E0A,0xA7,  0x1E80,0xA9,  0x1E82,0x1E0B,0x1EF2,0xAD,  0xAE,  0x178, 0x1E1E,0x1E1F,0x120, 0x121, 0x1E40,0x1E41,0xB6,  0x1E56,0x1E81,0x1E57,0x1E83,0x1E60,0x1EF3,0x1E84,0x1E85,0x1E61,0xC0,  0xC1,  0xC2,  0xC3,  0xC4,  0xC5,  0xC6,  0xC7,  0xC8,  0xC9,  0xCA,  0xCB,  0xCC,  0xCD,  0xCE,  0xCF,  0x174, 0xD1,  0xD2,  0xD3,  0xD4,  0xD5,  0xD6,  0x1E6A,0xD8,  0xD9,  0xDA,  0xDB,  0xDC,  0xDD,  0x176, 0xDF,  0xE0,  0xE1,  0xE2,  0xE3,  0xE4,  0xE5,  0xE6,  0xE7,  0xE8,  0xE9,  0xEA,  0xEB,  0xEC,  0xED,  0xEE,  0xEF,  0x175, 0xF1,  0xF2,  0xF3,  0xF4,  0xF5,  0xF6,  0x1E6B,0xF8,  0xF9,  0xFA,  0xFB,  0xFC,  0xFD,  0x177, 0xFF},
	/** ISO 15 */{0xA0,  0xA1,  0xA2,  0xA3,  0x20AC,0xA5,  0x160, 0xA7,  0x161, 0xA9,  0xAA,  0xAB,  0xAC,  0xAD,  0xAE,  0xAF,  0xB0,  0xB1,  0xB2,  0xB3,  0x17D, 0xB5,  0xB6,  0xB7,  0x17E, 0xB9,  0xBA,  0xBB,  0x152, 0x153, 0x178, 0xBF,  0xC0,  0xC1,  0xC2,  0xC3,  0xC4,  0xC5,  0xC6,  0xC7,  0xC8,  0xC9,  0xCA,  0xCB,  0xCC,  0xCD,  0xCE,  0xCF,  0xD0,  0xD1,  0xD2,  0xD3,  0xD4,  0xD5,  0xD6,  0xD7,  0xD8,  0xD9,  0xDA,  0xDB,  0xDC,  0xDD,  0xDE,  0xDF,  0xE0,  0xE1,  0xE2,  0xE3,  0xE4,  0xE5,  0xE6,  0xE7,  0xE8,  0xE9,  0xEA,  0xEB,  0xEC,  0xED,  0xEE,  0xEF,  0xF0,  0xF1,  0xF2,  0xF3,  0xF4,  0xF5,  0xF6,  0xF7,  0xF8,  0xF9,  0xFA,  0xFB,  0xFC,  0xFD,  0xFE,  0xFF},
	/** ISO 16 */{0xA0,  0x104, 0x105, 0x141, 0x20AC,0x201E,0x160, 0xA7,  0x161, 0xA9,  0x218, 0xAB,  0x179, 0xAD,  0x17A, 0x17B, 0xB0,  0xB1,  0x10C, 0x142, 0x17D, 0x201D,0xB6,  0xB7,  0x17E, 0x10D, 0x219, 0xBB,  0x152, 0x153, 0x178, 0x17C, 0xC0,  0xC1,  0xC2,  0x102, 0xC4,  0x106, 0xC6,  0xC7,  0xC8,  0xC9,  0xCA,  0xCB,  0xCC,  0xCD,  0xCE,  0xCF,  0x110, 0x143, 0xD2,  0xD3,  0xD4,  0x150, 0xD6,  0x15A, 0x170, 0xD9,  0xDA,  0xDB,  0xDC,  0x118, 0x21A, 0xDF,  0xE0,  0xE1,  0xE2,  0x103, 0xE4,  0x107, 0xE6,  0xE7,  0xE8,  0xE9,  0xEA,  0xEB,  0xEC,  0xED,  0xEE,  0xEF,  0x111, 0x144, 0xF2,  0xF3,  0xF4,  0x151, 0xF6,  0x15B, 0x171, 0xF9,  0xFA,  0xFB,  0xFC,  0x119, 0x21B, 0xFF},
};

//// Functions //////////////////////////////////////////////////

//! \brief Convert a character from a charset into an UNICODE character.
//! \param[in] character Character to decode.
//! \param[in] charset Charset of the \a character to convert.
//! \return Unicode character.
EPSTOOL_WideChar_t EPSTOOL_UnicodeChar_Decode(char character, EPSTOOL_Unicode_charset_e charset)
{
	ASSERT((charset >= EPSTOOL_UNICODE_ISO_8859_1) && (charset < EPSTOOL_UNICODE_LAST));

	if ((((unsigned char)character) < 0xA0) || (charset == EPSTOOL_UNICODE_ISO_8859_1))
	{
		return (EPSTOOL_WideChar_t)((unsigned char)character);
	}
	else
	{
		if ((charset >= EPSTOOL_UNICODE_ISO_8859_2) && (charset <= EPSTOOL_UNICODE_LAST-1))
		{
			return (EPSTOOL_WideChar_t)(__EPSTOOL_Unicode_Table[charset - EPSTOOL_UNICODE_ISO_8859_2][((unsigned char)character) - 0xA0]);
		}
	}
	return EPSTOOL_UNICODE_SWAP_CHAR;
}

//! \brief Get the length in bytes of a UNICODE character when it is encoded UTF-8.
//! \param[in] unicode The UNICODE character.
//! \return length Length in bytes of \a unicode when encoded in UTF-8. */
unsigned long EPSTOOL_UnicodeChar_Utf8Length(EPSTOOL_WideChar_t unicode)
{
	unsigned long result;

	ASSERT(unicode <= 0x7FFFFFFF);

	result = 0;

	// 0XXX XXXX one byte
	if (unicode <= 0x7F)
	{
		result = 1;
	}
	// 110X XXXX  two bytes 
	else if (unicode <= 0x7FF)
	{
		result = 2;
	}
	// 1110 XXXX  three bytes 
	else if (unicode <= 0xFFFF)
	{
		result = 3;
	}
	// 1111 0XXX  four bytes 
	else if (unicode <= 0x1FFFFF)
	{
		result = 4;
	}
	// 1111 10XX  five bytes 
	else if (unicode <= 0x3FFFFFF)
	{
		result = 5;
	}
	// 1111 110X  six bytes 
	else if (unicode <= 0x7FFFFFFF)
	{
		result = 6;
	}
	return result;
}

//! \brief Convert a UNICODE character into an UTF-8 character.
//! \param[in] unicode The UNICODE character.
//! \param[out] utf8 Destination buffer that will contain the converted character.
//! \param[in] size Destination buffer size.
//! \return The number of bytes used in the \a utf8 buffer to write the character.
unsigned long EPSTOOL_UnicodeChar_UnicodeToUtf8 (EPSTOOL_WideChar_t unicode, char *utf8, unsigned long size)
{
	unsigned long result;
	unsigned long length;

	result = 0;
	if (utf8 != NULL)
	{
		length = EPSTOOL_UnicodeChar_Utf8Length(unicode);

		if (size >= length)
		{
			memclr(utf8, size);

			switch(length)
			{
			case 1: *utf8++ = (byte) unicode; break;
			case 2: *utf8++ = (byte) (0xC0 | (unicode >>  6)); break;
			case 3: *utf8++ = (byte) (0xE0 | (unicode >> 12)); break;
			case 4: *utf8++ = (byte) (0xF0 | (unicode >> 18)); break;
			case 5: *utf8++ = (byte) (0xF8 | (unicode >> 24)); break;
			case 6: *utf8++ = (byte) (0xFC | (unicode >> 30)); break;
			default: result = FALSE; break;
			}
			
			switch(length)
			{
			case 6: *utf8++ = (byte) (0x80 | ((unicode >> 24) & 0x3F));
			case 5: *utf8++ = (byte) (0x80 | ((unicode >> 18) & 0x3F));
			case 4: *utf8++ = (byte) (0x80 | ((unicode >> 12) & 0x3F));
			case 3: *utf8++ = (byte) (0x80 | ((unicode >>  6) & 0x3F));
			case 2: *utf8++ = (byte) (0x80 |  (unicode        & 0x3F));
			}
			result = length;
		}
	}
	return result;
}

//! \brief Convert a string encoded in a given charset into a UNICODE string.
//! \param[in] string The string to convert, encoded in \a charset.
//! \param[in] stringLength The length of the \a string buffer in bytes.
//! \param[in] charset The charset used to encode \a string.
//! \param[out] utf8 The converted \a string.
//! \param[in] utf8Length The length of the \a utf8 buffer in bytes.
//! \return If \a utf8 is \a NULL or \a utf8Length is 0, the function returns the required size of the converted UTF-8, without the ending zero.
//! Otherwise, it returns the number of bytes filled in the \a utf8 buffer, without the ending zero.
unsigned long EPSTOOL_Unicode_CharsetToUtf8(const char *string, unsigned long stringLength,
		EPSTOOL_Unicode_charset_e charset, char *utf8, unsigned long utf8Length)
{
	unsigned long result;
	char charsetChar;
	EPSTOOL_WideChar_t wchar;
	unsigned long consumed;
	int convert;

	result = 0;
	if ((string != NULL) && ((stringLength > 0)))
	{
		// Shall the string must be converted or shall we only calculate the required buffer size?
		convert = ((utf8 != NULL) && (utf8Length > 0));

		do
		{
			charsetChar = *string;
			if (charsetChar != 0)
			{
				// Convert a charset character into a UNICODE one
				wchar = EPSTOOL_UnicodeChar_Decode(charsetChar, charset);
				string++;
				stringLength--;

				if (convert)
				{
					// Convert the UNICODE character into a UTF-8 one
					consumed = EPSTOOL_UnicodeChar_UnicodeToUtf8(wchar, utf8, utf8Length - 1);
					ASSERT(consumed <= utf8Length - 1);
					utf8 += consumed;
					utf8Length -= consumed;
				}
				else
				{
					// Calculate the required length
					consumed = EPSTOOL_UnicodeChar_Utf8Length(wchar);
				}
				result += consumed;
			}
			else
			{
				consumed = 0;
			}

			// Continue until:
			// - end of 'string' (charsetChar != 0)
			// - end of buffer 'string' (stringLength > 0)
			// - not enough place in 'utf8' (consumed > 0)
		} while((charsetChar != 0) && (stringLength > 0) && (consumed > 0));

		if (convert)
		{
			ASSERT(utf8 != NULL);
			ASSERT(utf8Length >= 1);

			// Add end of string
			*utf8 = 0;
		}
	}

	return result;
}

//! \brief Convert a string encoded in a given charset into a UNICODE string.
//! \param[in] string The string to convert, encoded in \a charset.
//! \param[in] stringLength The length of the \a string buffer in bytes.
//! \param[in] charset The charset used to encode \a string.
//! \return A pointer to the converted UNICODE string.
//! \warning The caller must free the returned pointer using \a ufree.
char *EPSTOOL_Unicode_CharsetToNewUtf8(const char *string, unsigned long stringLength, EPSTOOL_Unicode_charset_e charset)
{
	unsigned long length;
	char *utf8;

	length = EPSTOOL_Unicode_CharsetToUtf8(string, stringLength, charset, NULL, 0);
	utf8 = (char*)umalloc(length + 1);
	if (utf8 != NULL)
	{
		length = EPSTOOL_Unicode_CharsetToUtf8(string, stringLength, charset, utf8, length + 1);
	}

	return utf8;
}