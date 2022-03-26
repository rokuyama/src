/* $NetBSD: igpioreg.h,v 1.2 2022/03/26 19:35:35 riastradh Exp $ */

/*
 * Copyright (c) 2021 Emmanuel Dreyfus
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _IGPIOREG_H
#define _IGPIOREG_H

#define IGPIO_REVID	0x0000
#define IGPIO_CAPLIST	0x0004
#define IGPIO_PADBAR	0x000c

#define IGPIO_PADCFG0	0x0000

#define IGPIO_PADCFG0_RXEVCFG_SHIFT           25
#define IGPIO_PADCFG0_RXEVCFG_MASK            __BITS(26, 25)
#define IGPIO_PADCFG0_RXEVCFG_LEVEL           0
#define IGPIO_PADCFG0_RXEVCFG_EDGE            1
#define IGPIO_PADCFG0_RXEVCFG_DISABLED        2
#define IGPIO_PADCFG0_RXEVCFG_EDGE_BOTH       3
#define IGPIO_PADCFG0_PREGFRXSEL              __BIT(24)
#define IGPIO_PADCFG0_RXINV                   __BIT(23)
#define IGPIO_PADCFG0_GPIROUTIOXAPIC          __BIT(20)
#define IGPIO_PADCFG0_GPIROUTSCI              __BIT(19)
#define IGPIO_PADCFG0_GPIROUTSMI              __BIT(18)
#define IGPIO_PADCFG0_GPIROUTNMI              __BIT(17)
#define IGPIO_PADCFG0_PMODE_SHIFT             10
#define IGPIO_PADCFG0_PMODE_MASK              __BITS(13, 10)
#define IGPIO_PADCFG0_PMODE_GPIO              0
#define IGPIO_PADCFG0_GPIORXDIS               __BIT(9)
#define IGPIO_PADCFG0_GPIOTXDIS               __BIT(8)
#define IGPIO_PADCFG0_GPIORXSTATE             __BIT(1)
#define IGPIO_PADCFG0_GPIOTXSTATE             __BIT(0)

#define IGPIO_PADCFG1	0x0004
#define IGPIO_PADCFG1_TERM_UP                 __BIT(13)
#define IGPIO_PADCFG1_TERM_SHIFT              10
#define IGPIO_PADCFG1_TERM_MASK               __BITS(12, 10)
#define IGPIO_PADCFG1_TERM_20K                __BIT(2)
#define IGPIO_PADCFG1_TERM_5K                 __BIT(1)
#define IGPIO_PADCFG1_TERM_1K                 __BIT(0)
#define IGPIO_PADCFG1_TERM_833                (__BIT(1) | BIT(0))

#define IGPIO_CAPLIST_ID_GPIO_HW_INFO	1
#define IGPIO_CAPLIST_ID_PWM		2
#define IGPIO_CAPLIST_ID_BLINK		3
#define IGPIO_CAPLIST_ID_EXP		4


#define IGPIO_PINCTRL_FEATURE_DEBOUNCE		0x001
#define IGPIO_PINCTRL_FEATURE_1K_PD		0x002
#define IGPIO_PINCTRL_FEATURE_GPIO_HW_INFO	0x004
#define IGPIO_PINCTRL_FEATURE_PWM		0x010
#define IGPIO_PINCTRL_FEATURE_BLINK		0x020
#define IGPIO_PINCTRL_FEATURE_EXP		0x040

struct igpio_bank_setup {
	const char *ibs_acpi_hid;
	int ibs_barno;
	int ibs_first_pin;
	int ibs_last_pin;
	int ibs_gpi_is;
	int ibs_gpi_ie;
};

struct igpio_pin_group {
	const char *ipg_acpi_hid;
	int ipg_groupno;
	int ipg_first_pin;
	const char *ipg_name;
};

struct igpio_bank_setup igpio_bank_setup[] = {
	/* Sunrisepoint-LP */
	{ "INT344B",    0,   0,  47, 0x100, 0x120 },
	{ "INT344B",    1,  48, 119, 0x100, 0x120 },
	{ "INT344B",    2, 120, 151, 0x100, 0x120 },

	/* Sunrisepoint-H */
	{ "INT3451",    0,   0,  47, 0x100, 0x120 },
	{ "INT3451",    1,  48, 180, 0x100, 0x120 },
	{ "INT3451",    2, 181, 191, 0x100, 0x120 },

	/* Sunrisepoint-H */
	{ "INT345D",    0,   0,  47, 0x100, 0x120 },
	{ "INT345D",    1,  48, 180, 0x100, 0x120 },
	{ "INT345D",    2, 181, 191, 0x100, 0x120 },

	/* Baytrail XXX GPI_IS and GPI_IE */
	{ "INT33B2",    0,   0, 101, 0x000, 0x000 },
	{ "INT33FC",    0,   0, 101, 0x000, 0x000 },

	/* Lynxpoint XXX GPI_IS and GPI_IE */
	{ "INT33C7",    0,   0,  94, 0x000, 0x000 },
	{ "INT3437",    0,   0,  94, 0x000, 0x000 },

	/* Cannonlake-H */
	{ "INT3450",    0,   0,  50, 0x100, 0x120 },
	{ "INT3450",    1,  51, 154, 0x100, 0x120 },
	{ "INT3450",    2, 155, 248, 0x100, 0x120 },
	{ "INT3450",    3, 249, 298, 0x100, 0x120 },

	/* Cannonlake-LP */
	{ "INT34BB",    0,   0,  67, 0x100, 0x120 },
	{ "INT34BB",    1,  68, 180, 0x100, 0x120 },
	{ "INT34BB",    2, 181, 243, 0x100, 0x120 },

	/* Alderlake */
	{ "INTC1056",   0,   0,  94, 0x200, 0x220 },
	{ "INTC1056",   1,  95, 150, 0x200, 0x220 },
	{ "INTC1056",   2, 151, 199, 0x200, 0x220 },
	{ "INTC1056",   3, 200, 269, 0x200, 0x220 },
	{ "INTC1056",   4, 270, 303, 0x200, 0x220 },

	/* Icelake */
	{ "INT3455",    0,   0,  58, 0x100, 0x110 },
	{ "INT3455",    1,  59, 152, 0x100, 0x110 },
	{ "INT3455",    2, 153, 215, 0x100, 0x110 },
	{ "INT3455",    3, 216, 240, 0x100, 0x110 },

	/* Lakefield */
	{ "INT34C4",    0,   0,  59, 0x100, 0x110 },
	{ "INT34C4",    1,  60, 148, 0x100, 0x110 },
	{ "INT34C4",    2, 149, 237, 0x100, 0x110 },
	{ "INT34C4",    3, 238, 266, 0x100, 0x110 },

	/* Tigerlake-LP */
	{ "INT34C5",    0,   0,  66, 0x100, 0x120 },
	{ "INT34C5",    1,  67, 170, 0x100, 0x120 },
	{ "INT34C5",    2, 171, 259, 0x100, 0x120 },
	{ "INT34C5",    3, 260, 276, 0x100, 0x120 },

	/* Tigerlake-LP */
	{ "INTC1055",    0,   0,  66, 0x100, 0x120 },
	{ "INTC1055",    1,  67, 170, 0x100, 0x120 },
	{ "INTC1055",    2, 171, 259, 0x100, 0x120 },
	{ "INTC1055",    3, 260, 276, 0x100, 0x120 },

	/* Tigerlake-LP */
	{ "INTC1057",    0,   0,  66, 0x100, 0x120 },
	{ "INTC1057",    1,  67, 170, 0x100, 0x120 },
	{ "INTC1057",    2, 171, 259, 0x100, 0x120 },
	{ "INTC1057",    3, 260, 276, 0x100, 0x120 },

	/* Tigerlake-H */
	{ "INT34C6",    0,   0,  78, 0x100, 0x120 },
	{ "INT34C6",    1,  79, 180, 0x100, 0x120 },
	{ "INT34C6",    2, 181, 217, 0x100, 0x120 },
	{ "INT34C6",    3, 218, 266, 0x100, 0x120 },
	{ "INT34C6",    4, 267, 290, 0x100, 0x120 },

	/* Jasperlake */
	{ "INT34C8",    0,   0,  91, 0x100, 0x120 },
	{ "INT34C8",    1,  92, 194, 0x100, 0x120 },
	{ "INT34C8",    2, 195, 224, 0x100, 0x120 },
	{ "INT34C8",    3, 225, 232, 0x100, 0x120 },

	/* Lewisburg */
	{ "INT3536",    0,   0,   7, 0x100, 0x110 },
	{ "INT3536",    1,  72,  13, 0x100, 0x110 },
	{ "INT3536",    3, 133,  14, 0x100, 0x110 },
	{ "INT3536",    4, 144,  17, 0x100, 0x110 },
	{ "INT3536",    5, 179, 246, 0x100, 0x110 },

	/* Emmitsburg */
	{ "INTC1071",    0,   0,  65, 0x200, 0x210 },
	{ "INTC1071",    1,  66, 111, 0x200, 0x210 },
	{ "INTC1071",    2, 112, 145, 0x200, 0x210 },
	{ "INTC1071",    3, 146, 183, 0x200, 0x210 },
	{ "INTC1071",    4, 184, 261, 0x200, 0x210 },

	/* Denverton */
        { "INTC3000",    0,   0,  40, 0x100, 0x120 },
        { "INTC3000",    1,  41, 153, 0x100, 0x120 },

	/* Cedarfork */
        { "INTC3001",    0,   0, 167, 0x200, 0x230 },
        { "INTC3001",    1, 168, 236, 0x200, 0x230 },

	/* Geminilake */
	{ "INT3453",     0,   0,  34, 0x100, 0x110 },

#ifdef notyet
	/*
	 * BAR mappings not obvious, further studying required
	 */
	/* Broxton */
	{ "apollolake-pinctrl", 0,   0,   0, 0x100, 0x110 },
	{ "broxton-pinctrl",    0,   0,   0, 0x100, 0x110 },
	{ "INT34D1",     0,   0,    0, 0x100, 0x110 },
	{ "INT3452",     0,   0,    0, 0x100, 0x110 },

	/* Cherryview */
	{ "INT33FF",     0,   0,    0, 0x000, 0x000 },
#endif

	{      NULL,     0,   0,   0,  0x000, 0x000 },
};

struct igpio_pin_group igpio_pin_group[] = {
	/* Sunrisepoint-LP */
	{ "INT344B",    0, 151,  "A" },

	/* Sunrisepoint-H */
	{ "INT3451",    0,   0,  "A" },
	{ "INT3451",    1,  24,  "B" },
	{ "INT3451",    0,  48,  "C" },
	{ "INT3451",    1,  72,  "D" },
	{ "INT3451",    2,  96,  "E" },
	{ "INT3451",    3, 109,  "F" },
	{ "INT3451",    4, 133,  "G" },
	{ "INT3451",    5, 157,  "H" },
	{ "INT3451",    0, 181,  "I" },

	/* Sunrisepoint-H */
	{ "INT345D",    0,   0,  "A" },
	{ "INT345D",    1,  24,  "B" },
	{ "INT345D",    0,  48,  "C" },
	{ "INT345D",    1,  72,  "D" },
	{ "INT345D",    2,  96,  "E" },
	{ "INT345D",    3, 109,  "F" },
	{ "INT345D",    4, 133,  "G" },
	{ "INT345D",    5, 157,  "H" },
	{ "INT345D",    0, 181,  "I" },


	/* Baytrail */
	{ "INT33B2",    0, 101,  "A" },

	/* Lynxpoint */
	{ "INT33C7",    0,  94,  "A" },
	{ "INT3437",    0,  94,  "A" },

	/* Cannonlake-H */
	{ "INT3450",  0,  0,  "GPP_A" },
	{ "INT3450",  1,  25,  "GPP_B" },
	{ "INT3450",  0,  51,  "GPP_C" },
	{ "INT3450",  1,  75,  "GPP_D" },
	{ "INT3450",  2,  99,  "GPP_G" },
	{ "INT3450",  3,  107,  "AZA" },
	{ "INT3450",  4,  115,  "vGPIO_0" },
	{ "INT3450",  5,  147,  "vGPIO_1" },
	{ "INT3450",  0,  155,  "GPP_K" },
	{ "INT3450",  1,  179,  "GPP_H" },
	{ "INT3450",  2,  203,  "GPP_E" },
	{ "INT3450",  3,  216,  "GPP_F" },
	{ "INT3450",  4,  240,  "SPI" },
	{ "INT3450",  0,  249,  "CPU" },
	{ "INT3450",  1,  260,  "JTAG" },
	{ "INT3450",  2,  269,  "GPP_I" },
	{ "INT3450",  3,  287,  "GPP_J" },

	/* Cannonlake-LP */
	{ "INT34BB",  0,  0,  "GPP_A" },
	{ "INT34BB",  1,  25,  "GPP_B" },
	{ "INT34BB",  2,  51,  "GPP_G" },
	{ "INT34BB",  3,  59,  "SPI" },
	{ "INT34BB",  0,  68,  "GPP_D" },
	{ "INT34BB",  1,  93,  "GPP_F" },
	{ "INT34BB",  2,  117,  "GPP_H" },
	{ "INT34BB",  3,  141,  "vGPIO" },
	{ "INT34BB",  4,  173,  "vGPIO" },
	{ "INT34BB",  0,  181,  "GPP_C" },
	{ "INT34BB",  1,  205,  "GPP_E" },
	{ "INT34BB",  2,  229,  "JTAG" },
	{ "INT34BB",  3,  238,  "HVCMOS" },

	/* Alderlake */
	{ "INTC1056",  0,  0,  "GPP_I" },
	{ "INTC1056",  1,  25,  "GPP_R" },
	{ "INTC1056",  2,  48,  "GPP_J" },
	{ "INTC1056",  3,  60,  "vGPIO" },
	{ "INTC1056",  4,  87,  "vGPIO_0" },
	{ "INTC1056",  0,  95,  "GPP_B" },
	{ "INTC1056",  1,  119,  "GPP_G" },
	{ "INTC1056",  2,  127,  "GPP_H" },
	{ "INTC1056",  0,  151,  "SPI0" },
	{ "INTC1056",  1,  160,  "GPP_A" },
	{ "INTC1056",  2,  176,  "GPP_C" },
	{ "INTC1056",  0,  200,  "GPP_S" },
	{ "INTC1056",  1,  208,  "GPP_E" },
	{ "INTC1056",  2,  231,  "GPP_K" },
	{ "INTC1056",  3,  246,  "GPP_F" },
	{ "INTC1056",  0,  270,  "GPP_D" },
	{ "INTC1056",  1,  295,  "JTAG" },


	/* Icelake */
	{ "INT3455",  0,  0,  "GPP_G" },
	{ "INT3455",  1,  8,  "GPP_B" },
	{ "INT3455",  2,  34,  "GPP_A" },
	{ "INT3455",  0,  59,  "GPP_H" },
	{ "INT3455",  1,  83,  "GPP_D" },
	{ "INT3455",  2,  104,  "GPP_F" },
	{ "INT3455",  3,  124,  "vGPIO" },
	{ "INT3455",  0,  153,  "GPP_C" },
	{ "INT3455",  1,  177,  "HVCMOS" },
	{ "INT3455",  2,  183,  "GPP_E" },
	{ "INT3455",  3,  207,  "JTAG" },
	{ "INT3455",  2,  232,  "SPI" },


	/* Lakefield */
	{ "INT34C4",  0,  0,  "EAST_0" },
	{ "INT34C4",  1,  32,  "EAST_1" },
	{ "INT34C4",  0,  60,  "NORTHWEST_0" },
	{ "INT34C4",  1,  92,  "NORTHWEST_1" },
	{ "INT34C4",  2,  124,  "NORTHWEST_2" },
	{ "INT34C4",  0,  149,  "WEST_0" },
	{ "INT34C4",  1,  181,  "WEST_1" },
	{ "INT34C4",  2,  213,  "WEST_2" },
	{ "INT34C4",  0,  238,  "SOUTHEAST" },


	/* Tigerlake-LP */
	{ "INT34C5",  0,  0,  "GPP_A" },
	{ "INT34C5",  1,  25,  "GPP_R" },
	{ "INT34C5",  2,  45,  "GPP_B" },
	{ "INT34C5",  3,  71,  "vGPIO_0" },
	{ "INT34C5",  0,  79,  "GPP_D" },
	{ "INT34C5",  1,  105,  "GPP_C" },
	{ "INT34C5",  2,  129,  "GPP_S" },
	{ "INT34C5",  3,  137,  "GPP_G" },
	{ "INT34C5",  4,  154,  "vGPIO" },
	{ "INT34C5",  0,  181,  "GPP_E" },
	{ "INT34C5",  1,  194,  "GPP_F" },
	{ "INT34C5",  0,  218,  "GPP_H" },
	{ "INT34C5",  1,  242,  "GPP_J" },
	{ "INT34C5",  2,  252,  "GPP_K" },
	{ "INT34C5",  0,  267,  "GPP_I" },
	{ "INT34C5",  1,  282,  "JTAG" },


	/* Tigerlake-LP */
	{ "INTC1055",  0,  0,  "GPP_A" },
	{ "INTC1055",  1,  25,  "GPP_R" },
	{ "INTC1055",  2,  45,  "GPP_B" },
	{ "INTC1055",  3,  71,  "vGPIO_0" },
	{ "INTC1055",  0,  79,  "GPP_D" },
	{ "INTC1055",  1,  105,  "GPP_C" },
	{ "INTC1055",  2,  129,  "GPP_S" },
	{ "INTC1055",  3,  137,  "GPP_G" },
	{ "INTC1055",  4,  154,  "vGPIO" },
	{ "INTC1055",  0,  181,  "GPP_E" },
	{ "INTC1055",  1,  194,  "GPP_F" },
	{ "INTC1055",  0,  218,  "GPP_H" },
	{ "INTC1055",  1,  242,  "GPP_J" },
	{ "INTC1055",  2,  252,  "GPP_K" },
	{ "INTC1055",  0,  267,  "GPP_I" },
	{ "INTC1055",  1,  282,  "JTAG" },


	/* Tigerlake-LP */
	{ "INTC1057",  0,  0,  "GPP_A" },
	{ "INTC1057",  1,  25,  "GPP_R" },
	{ "INTC1057",  2,  45,  "GPP_B" },
	{ "INTC1057",  3,  71,  "vGPIO_0" },
	{ "INTC1057",  0,  79,  "GPP_D" },
	{ "INTC1057",  1,  105,  "GPP_C" },
	{ "INTC1057",  2,  129,  "GPP_S" },
	{ "INTC1057",  3,  137,  "GPP_G" },
	{ "INTC1057",  4,  154,  "vGPIO" },
	{ "INTC1057",  0,  181,  "GPP_E" },
	{ "INTC1057",  1,  194,  "GPP_F" },
	{ "INTC1057",  0,  218,  "GPP_H" },
	{ "INTC1057",  1,  242,  "GPP_J" },
	{ "INTC1057",  2,  252,  "GPP_K" },
	{ "INTC1057",  0,  267,  "GPP_I" },
	{ "INTC1057",  1,  282,  "JTAG" },

	/* Tigerlake-H */
	{ "INT34C6",  0,  0,  "GPP_B" },
	{ "INT34C6",  1,  26,  "GPP_T" },
	{ "INT34C6",  2,  42,  "GPP_A" },
	{ "INT34C6",  0,  67,  "GPP_S" },
	{ "INT34C6",  1,  75,  "GPP_H" },
	{ "INT34C6",  2,  99,  "GPP_D" },
	{ "INT34C6",  3,  120,  "GPP_U" },
	{ "INT34C6",  4,  144,  "vGPIO" },
	{ "INT34C6",  0,  171,  "GPP_C" },
	{ "INT34C6",  1,  195,  "GPP_F" },
	{ "INT34C6",  2,  220,  "HVCMOS" },
	{ "INT34C6",  3,  226,  "GPP_E" },
	{ "INT34C6",  4,  251,  "JTAG" },
	{ "INT34C6",  0,  260,  "GPP_R" },
	{ "INT34C6",  1,  268,  "SPI" },

	/* Jasperlake */
	{ "INT34C8",  0,  0,  "GPP_F" },
	{ "INT34C8",  1,  20,  "SPI" },
	{ "INT34C8",  2,  29,  "GPP_B" },
	{ "INT34C8",  3,  55,  "GPP_A" },
	{ "INT34C8",  4,  76,  "GPP_S" },
	{ "INT34C8",  5,  84,  "GPP_R" },
	{ "INT34C8",  0,  92,  "GPP_H" },
	{ "INT34C8",  1,  116,  "GPP_D" },
	{ "INT34C8",  2,  142,  "vGPIO" },
	{ "INT34C8",  3,  171,  "GPP_C" },
	{ "INT34C8",  0,  195,  "HVCMOS" },
	{ "INT34C8",  1,  201,  "GPP_E" },
	{ "INT34C8",  0,  225,  "GPP_G" },


	/* Lewisburg */
	{ "INT3536",   0,   7,  "" },

	/* Emmitsburg */
	{ "INTC1071",  0,  0,  "GPP_A" },
	{ "INTC1071",  1,  21,  "GPP_B" },
	{ "INTC1071",  2,  45,  "SPI" },
	{ "INTC1071",  0,  66,  "GPP_C" },
	{ "INTC1071",  1,  88,  "GPP_D" },
	{ "INTC1071",  0,  112,  "GPP_E" },
	{ "INTC1071",  1,  136,  "JTAG" },
	{ "INTC1071",  0,  146,  "GPP_H" },
	{ "INTC1071",  1,  166,  "GPP_J" },
	{ "INTC1071",  0,  184,  "GPP_I" },
	{ "INTC1071",  1,  208,  "GPP_L" },
	{ "INTC1071",  2,  226,  "GPP_M" },
	{ "INTC1071",  3,  244,  "GPP_N" },

	/* Denverton */
	{ "INTC3000",  0,  0,  "North" },
	{ "INTC3000",  1,  32,  "North" },
	{ "INTC3000",  0,  41,  "South" },
	{ "INTC3000",  1,  59,  "South" },
	{ "INTC3000",  2,  91,  "South" },
	{ "INTC3000",  3,  112,  "South" },
	{ "INTC3000",  4,  144,  "South" },


	/* Cedarfork */
	{ "INTC3001",  0,  0,  "WEST2" },
	{ "INTC3001",  1,  24,  "WEST3" },
	{ "INTC3001",  2,  48,  "WEST01" },
	{ "INTC3001",  3,  71,  "WEST5" },
	{ "INTC3001",  4,  91,  "WESTC" },
	{ "INTC3001",  5,  97,  "WESTC_DFX" },
	{ "INTC3001",  6,  102,  "WESTA" },
	{ "INTC3001",  7,  112,  "WESTB" },
	{ "INTC3001",  8,  124,  "WESTD" },
	{ "INTC3001",  9,  144,  "WESTD_PECI" },
	{ "INTC3001",  10,  145,  "WESTF" },
	{ "INTC3001",  0,  168,  "EAST2" },
	{ "INTC3001",  1,  192,  "EAST3" },
	{ "INTC3001",  2,  203,  "EAST0" },
	{ "INTC3001",  3,  226,  "EMMC" },

	/* Geminilake */
	{ "INT3453",     0,   34,  "" },

#ifdef notyet
	/*
	 * BAR mappings not obvious, further studying required
	 */
	/* Broxton */
	{ "apollolake-pinctrl", 0,   0,   "" },
	{ "broxton-pinctrl",    0,   0,   "" },
	{ "INT34D1",     0,   0,   "" },
	{ "INT3452",     0,   0,   "" },

	/* Cherryview */
	{ "INT33FF",     0,   0,   "" },
#endif

	{      NULL,    0,   0,   0 },
};

#endif /* _IGPIOREG_H */
