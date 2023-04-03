/* $NetBSD$ */

/*-
 * Copyright (c) 2023 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by Nick Hudson
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
 * THIS SOFTWARE IS PROVIDED BY THE NETBSD FOUNDATION, INC. AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <sys/cdefs.h>
__KERNEL_RCSID(0, "$NetBSD$");

#include <sys/param.h>



#if 0
[   1.2000464] jh7100_clkc_update: 01cc = 800004e2(80000005)
[   1.6712174] jh7100_clkc_update: 01d8 = 80000741(80000006)
[   2.1646251] jh7100_clkc_update: 01cc = 80000014(80000002)
[   2.6515227] jh7100_clkc_update: 01d8 = 8000000f(80000001)
#endif

// XXXNH rename clkc to clkgen?
#if 0
#include <sys/bus.h>
#include <sys/device.h>
#include <sys/intr.h>
#include <sys/systm.h>
#include <sys/time.h>
#include <sys/kmem.h>
#endif

#include <dev/clk/clk_backend.h>

#include <dev/fdt/fdtvar.h>

#include <riscv/sifive/jh7100_clkc.h>



/* Used as parents of clocks in dtsi */
#define JH7100_CLK_CPUNDBUS_ROOT	0
#define JH7100_CLK_DSP_ROOT		2
#define JH7100_CLK_GMACUSB_ROOT		3
#define JH7100_CLK_PERH0_ROOT		4
#define JH7100_CLK_PERH1_ROOT		5
#define JH7100_CLK_CPUNBUS_ROOT_DIV	12
#define JH7100_CLK_DSP_ROOT_DIV		13
#define JH7100_CLK_PERH0_SRC		14
#define JH7100_CLK_PERH1_SRC		15
#define JH7100_CLK_PLL2_REF		19
#define JH7100_CLK_CPU_CORE		20
#define JH7100_CLK_CPU_AXI		21
#define JH7100_CLK_AHB_BUS		22
#define JH7100_CLK_APB1_BUS		23
#define JH7100_CLK_APB2_BUS		24


/* Used in jh7100.dtsi */
#define JH7100_CLK_SGDMA2P_AXI          31
#define JH7100_CLK_SGDMA2P_AHB          33
#define JH7100_CLK_VP6_CORE             38
#define JH7100_CLK_JPEG_APB             50
#define JH7100_CLK_SGDMA1P_BUS          84
#define JH7100_CLK_SGDMA1P_AXI          85
#define JH7100_CLK_SDIO0_AHB            114
#define JH7100_CLK_SDIO0_CCLKINT        115
#define JH7100_CLK_SDIO0_CCLKINT_INV	116
#define JH7100_CLK_SDIO1_AHB            117
#define JH7100_CLK_SDIO1_CCLKINT        118
#define JH7100_CLK_SDIO1_CCLKINT_INV	119
#define JH7100_CLK_GMAC_AHB             120
#define JH7100_CLK_GMAC_ROOT_DIV	121
#define JH7100_CLK_GMAC_PTP_REF         122
#define JH7100_CLK_QSPI_AHB             137
#define JH7100_CLK_SEC_AHB              140
#define JH7100_CLK_TRNG_APB             144
#define JH7100_CLK_OTP_APB              145
#define JH7100_CLK_UART0_APB            146
#define JH7100_CLK_UART0_CORE           147
#define JH7100_CLK_UART1_APB            148
#define JH7100_CLK_UART1_CORE           149
#define JH7100_CLK_SPI0_APB             150
#define JH7100_CLK_SPI0_CORE            151
#define JH7100_CLK_SPI1_APB             152
#define JH7100_CLK_SPI1_CORE            153
#define JH7100_CLK_I2C0_APB             154
#define JH7100_CLK_I2C0_CORE            155
#define JH7100_CLK_I2C1_APB             156
#define JH7100_CLK_I2C1_CORE            157
#define JH7100_CLK_GPIO_APB             158
#define JH7100_CLK_UART2_APB            159
#define JH7100_CLK_UART2_CORE           160
#define JH7100_CLK_UART3_APB            161
#define JH7100_CLK_UART3_CORE           162
#define JH7100_CLK_SPI2_APB             163
#define JH7100_CLK_SPI2_CORE            164
#define JH7100_CLK_SPI3_APB             165
#define JH7100_CLK_SPI3_CORE            166
#define JH7100_CLK_I2C2_APB             167
#define JH7100_CLK_I2C2_CORE            168
#define JH7100_CLK_I2C3_APB             169
#define JH7100_CLK_I2C3_CORE            170
#define JH7100_CLK_PWM_APB              181

#define JH7100_CLK_PLL0_OUT		186
#define JH7100_CLK_PLL1_OUT		187
#define JH7100_CLK_PLL2_OUT		188

#define JH7100_NCLKS			189


#define JH7100_CLK_VOUT_ROOT		7
#define JH7100_CLK_AUDIO_ROOT		8
#define JH7100_CLK_VOUTBUS_ROOT		11
#define JH7100_CLK_DOM7AHB_BUS		26
#define JH7100_CLK_AUDIO_DIV		95
#define JH7100_CLK_AUDIO_SRC		96
#define JH7100_CLK_AUDIO_12288		97
#define JH7100_CLK_VOUT_SRC		109
#define JH7100_CLK_DISPBUS_SRC		110
#define JH7100_CLK_DISP_BUS		111
#define JH7100_CLK_DISP_AXI		112
#define JH7100_CLK_WDTIMER_APB		171
#define JH7100_CLK_WDT_CORE		172


struct jh7100_clkc_softc {
	device_t		sc_dev;
	bus_space_tag_t		sc_bst;
	bus_space_handle_t	sc_bsh;
	int			sc_phandle;
	struct clk_domain	sc_clkdom;
	struct jh7100_clkc_clk *sc_clk;
	size_t			sc_nclks;

	u_int			sc_osclk;
	u_int			sc_oaclk;
};

#define	RD4(sc, reg)							\
	bus_space_read_4((sc)->sc_bst, (sc)->sc_bsh, (reg))
#define	WR4(sc, reg, val)						\
	bus_space_write_4((sc)->sc_bst, (sc)->sc_bsh, (reg), (val))



static void
jh7100_clkc_update(struct jh7100_clkc_softc * const sc,
    struct jh7100_clkc_clk *jcc, uint32_t set, uint32_t clr)
{
	// lock
	uint32_t val = RD4(sc, jcc->jcc_reg);
	uint32_t before = val;
	val &= ~clr;
	val |=  set;
	WR4(sc, jcc->jcc_reg, val);
	printf("%s: %04" PRIxBUSADDR 	" = %08x(%08x)\n", __func__,
	    jcc->jcc_reg, val, before);
}








#if 0
/* external clocks */
#define JH7100_CLK_OSC_SYS		(JH7100_NCLKS + 0)
#define JH7100_CLK_OSC_AUD		(JH7100_NCLKS + 1)
#endif
#if 0
#define JH7100_CLK_GMAC_RMII_REF	(JH7100_CLK_END + 2)
#define JH7100_CLK_GMAC_GR_MII_RX	(JH7100_CLK_END + 3)
#endif

#if 0
/* Don't do this and just return the struct clk * from get parent */
/* external clocks */
	JH7100CLKC_EXT(JH7100_CLK_OSC_SYS, "osc_sys"),
	JH7100CLKC_EXT(JH7100_CLK_OSC_AUD, "osc_aud"),
#endif


#if 0

struct jh7100_clkc_clkops jh7100_clkc_fixed_ops = {
	.jcco_getrate = jh7100_clkc_fixed_get_rate,
};


struct jh7100_clkc_clkops jh7100_clkc_ext_ops = {
	.jcco_enable = jh7100_clkc_extclk_enable,
	.jcco_getrate = jh7100_clkc_ext_get_rate,
	.jcco_setrate = jh7100_clkc_ext_set_rate,
	.jcco_getparent = jh7100_clkc_ext_get_parent,
};

#endif









/*
 * FIXED_FACTOR operations
 */

static u_int
jh7100_clkc_fixed_factor_get_parent_rate(struct clk *clk)
{
	struct clk *clk_parent = clk_get_parent(clk);
	if (clk_parent == NULL)
		return 0;
printf("%s: %u (rate)\n", __func__, clk_get_rate(clk_parent));
	return clk_get_rate(clk_parent);
}

u_int
jh7100_clkc_fixed_factor_get_rate(struct jh7100_clkc_softc *sc,
    struct jh7100_clkc_clk *jcc)
{
	struct jh7100_clkc_fixed_factor * const jcff = &jcc->jcc_ffactor;
	struct clk *clk = &jcc->jcc_base;

	KASSERT(jcc->jcc_type == JH7100CLK_FIXED_FACTOR);

	uint64_t rate = jh7100_clkc_fixed_factor_get_parent_rate(clk);
	uint64_t tmp = rate;
	if (rate == 0)
		return 0;

	rate *= jcff->jcff_mult;
	rate /= jcff->jcff_div;

printf("%s: %u (rate) (%"PRIu64") (%"PRIu64" * %u / %u)\n", __func__,
    (u_int)rate, rate, tmp, jcff->jcff_mult, jcff->jcff_div);

	return rate;
}

static int
jh7100_clkc_fixed_factor_set_parent_rate(struct clk *clk, u_int rate)
{
	struct clk *clk_parent = clk_get_parent(clk);
	if (clk_parent == NULL)
		return ENXIO;

printf("%s: %u (set parent)\n", __func__, rate);

	return clk_set_rate(clk_parent, rate);
}




int
jh7100_clkc_fixed_factor_set_rate(struct jh7100_clkc_softc *sc,
    struct jh7100_clkc_clk *jcc, u_int rate)
{
	struct jh7100_clkc_fixed_factor * const jcff = &jcc->jcc_ffactor;
	struct clk *clk = &jcc->jcc_base;

	KASSERT(jcc->jcc_type == JH7100CLK_FIXED_FACTOR);

	uint64_t tmp = rate;
	tmp *= jcff->jcff_div;
	tmp /= jcff->jcff_mult;

printf("%s: %u (set parent)\n", __func__, (u_int)tmp);

	return jh7100_clkc_fixed_factor_set_parent_rate(clk, tmp);
}

const char *
jh7100_clkc_fixed_factor_get_parent(struct jh7100_clkc_softc *sc,
    struct jh7100_clkc_clk *jcc)
{
	struct jh7100_clkc_fixed_factor * const jcff = &jcc->jcc_ffactor;

	KASSERT(jcc->jcc_type == JH7100CLK_FIXED_FACTOR);

printf("%s: '%s' has parent '%s'\n", __func__, jcc->jcc_base.name, jcff->jcff_parent);
	return jcff->jcff_parent;
}


/*
 * MUX operations
 */

int
jh7100_clkc_mux_set_parent(struct jh7100_clkc_softc *sc,
    struct jh7100_clkc_clk *jcc, const char *name)
{
	KASSERT(jcc->jcc_type == JH7100CLK_MUX);

	struct jh7100_clkc_mux * const jcm = &jcc->jcc_mux;
printf("%s: '%s' setting parent to'%s'\n", __func__, jcc->jcc_base.name, name);

	size_t i;
	for (i = 0; i < jcm->jcm_nparents; i++) {
                if (jcm->jcm_parents[i] != NULL &&
                    strcmp(jcm->jcm_parents[i], name) == 0)
                        break;
        }
        if (i >= jcm->jcm_nparents)
                return EINVAL;

        uint32_t val = RD4(sc, jcc->jcc_reg);
	uint32_t before = val;
        val &= JH7100_CLK_MUX_MASK;
        val |= __SHIFTIN(i, JH7100_CLK_MUX_MASK);
	printf("%s: %04" PRIxBUSADDR 	" = %08x(%08x)\n", __func__,
	    jcc->jcc_reg, val, before);
        WR4(sc, jcc->jcc_reg, val);

        return 0;
}


const char *
jh7100_clkc_mux_get_parent(struct jh7100_clkc_softc *sc,
    struct jh7100_clkc_clk *jcc)
{
	KASSERT(jcc->jcc_type == JH7100CLK_MUX);
	uint32_t val = RD4(sc, jcc->jcc_reg);
	size_t pindex = __SHIFTOUT(val, JH7100_CLK_MUX_MASK);

	if (pindex >= jcc->jcc_mux.jcm_nparents)
		return NULL;

printf("%s: '%s' has parent '%s'\n", __func__, jcc->jcc_base.name, jcc->jcc_mux.jcm_parents[pindex]);

	return jcc->jcc_mux.jcm_parents[pindex];
}


/*
 * GATE operations
 */

int
jh7100_clkc_gate_enable(struct jh7100_clkc_softc *sc,
    struct jh7100_clkc_clk *jcc, int enable)
{
	KASSERT(jcc->jcc_type == JH7100CLK_GATE);

	jh7100_clkc_update(sc, jcc,
	    (enable ? JH7100_CLK_ENABLE : 0), JH7100_CLK_ENABLE);

printf("%s: '%s' has been %s\n", __func__, jcc->jcc_base.name, enable ? "enabled" : "disabled");

	return 0;
}

const char *
jh7100_clkc_gate_get_parent(struct jh7100_clkc_softc *sc,
    struct jh7100_clkc_clk *jcc)
{
	KASSERT(jcc->jcc_type == JH7100CLK_GATE);

	struct jh7100_clkc_gate *jcc_gate = &jcc->jcc_gate;
printf("%s: '%s' has parent '%s'\n", __func__, jcc->jcc_base.name, jcc_gate->jcg_parent);

	return jcc_gate->jcg_parent;
}


/*
 * DIVIDER operations
 */


u_int
jh7100_clkc_div_get_rate(struct jh7100_clkc_softc *sc,
    struct jh7100_clkc_clk *jcc)
{
	KASSERT(jcc->jcc_type == JH7100CLK_DIV);

//	struct jh7100_clkc_div * const jcc_div = &jcc->jcc_div;
	struct clk * const clk = &jcc->jcc_base;
	struct clk * const clk_parent = clk_get_parent(clk);

	if (clk_parent == NULL) {
panic("%s: %s has no parent", __func__, clk->name);
		return 0;
	}

printf("\n%s: getting parent '%s' rate of '%s' = ...\n", __func__, clk_parent->name, clk->name);
	u_int rate = clk_get_rate(clk_parent);
printf("%s: got parent '%s' rate of '%s' = %u\n", __func__, clk_parent->name, clk->name, rate);
	if (rate == 0)
		return 0;

//	u_int rate, ratio;
//	CLK_LOCK(sc);
	uint32_t val = RD4(sc, jcc->jcc_reg);
	uint32_t div = __SHIFTOUT(val, JH7100_CLK_DIV_MASK);
//	CLK_UNLOCK(sc);
printf("%s: '%s' %u (rate) val %#08x div %d\n", __func__, clk->name,
    div != 0 ? rate / div : 0, val, div);

	return div != 0 ? rate / div : 0;
}

int
jh7100_clkc_div_set_rate(struct jh7100_clkc_softc *sc,
    struct jh7100_clkc_clk *jcc, u_int new_rate)
{
	KASSERT(jcc->jcc_type == JH7100CLK_DIV);

	struct jh7100_clkc_div * const jcc_div = &jcc->jcc_div;
	struct clk * const clk = &jcc->jcc_base;
	struct clk * const clk_parent = clk_get_parent(clk);

printf("%s: clk %p parent %p new_rate %u\n", __func__, clk, clk_parent, new_rate);

	if (clk_parent == NULL)
		return ENXIO;

	if (jcc_div->jcd_maxdiv == 0)
		return ENXIO;

printf("%s: getting parent (%p) rate of %p\n", __func__, clk_parent, clk);

	u_int parent_rate = clk_get_rate(clk_parent);

	if (parent_rate == 0) {
		return (new_rate == 0) ? 0 : ERANGE;
	}
	u_int ratio = howmany(parent_rate, new_rate);
	u_int div = uimin(ratio, jcc_div->jcd_maxdiv);

printf("%s: %u/%u (ratio of %u:%u)\n", __func__, div, ratio, parent_rate,
    new_rate);
#if 0
	jh7100_clkc_update(sc, jcc,
	    __SHIFTIN(div, JH7100_CLK_DIV_MASK), JH7100_CLK_DIV_MASK);

	    __SHIFTIN(ratio, JH7100_CLK_DIV_MASK), JH7100_CLK_DIV_MASK);
#else
	// somethings wrong above ... do this to see the programmed value.
	jh7100_clkc_update(sc, jcc, 0, 0);
#endif
//	CLK_UNLOCK(sc);

	return 0;
}

const char *
jh7100_clkc_div_get_parent(struct jh7100_clkc_softc *sc,
    struct jh7100_clkc_clk *jcc)
{
	KASSERT(jcc->jcc_type == JH7100CLK_DIV);

	struct jh7100_clkc_div *jcc_div = &jcc->jcc_div;

//printf("%s: '%s' has parent '%s'\n", __func__, jcc->jcc_base.name, jcc_div->jcd_parent);

	return jcc_div->jcd_parent;
}




/*
 * FRACTIONAL DIVIDER operations
 */

u_int
jh7100_clkc_fracdiv_get_rate(struct jh7100_clkc_softc *sc,
    struct jh7100_clkc_clk *jcc)
{
	KASSERT(jcc->jcc_type == JH7100CLK_FRACDIV);

	struct clk * const clk = &jcc->jcc_base;
	struct clk * const clk_parent = clk_get_parent(clk);

	if (clk_parent == NULL)
		return 0;

	u_int rate = clk_get_rate(clk_parent);
	if (rate == 0)
		return 0;
	uint32_t val = RD4(sc, jcc->jcc_reg);
#if 0


	u32 reg = jh7100_clk_reg_get(clk);
	unsigned long div100 = 100 * (reg & JH7100_CLK_INT_MASK) +
			       ((reg & JH7100_CLK_FRAC_MASK) >> JH7100_CLK_FRAC_SHIFT);

	return (div100 >= JH7100_CLK_FRAC_MIN) ? 100 * parent_rate / div100 : 0;
#endif


	uint32_t div = __SHIFTOUT(val, JH7100_CLK_DIV_MASK);

	return rate / div;
}

int
jh7100_clkc_fracdiv_set_rate(struct jh7100_clkc_softc *sc,
    struct jh7100_clkc_clk *jcc, u_int new_rate)
{
	KASSERT(jcc->jcc_type == JH7100CLK_FRACDIV);

//	struct jh7100_clkc_fracdiv * const jcc_fracdiv = &jcc->jcc_fracdiv;
	struct clk * const clk = &jcc->jcc_base;
	struct clk * const clk_parent = clk_get_parent(clk);

	if (clk_parent == NULL)
		return ENXIO;

#if 0
	if (jcc_div->jcd_maxdiv == 0)
		return ENXIO;

	u_int parent_rate = clk_get_rate(clk_parent);

	if (parent_rate == 0) {
		return (new_rate == 0) ? 0 : ERANGE;
	}
	u_int ratio = howmany(parent_rate, new_rate);
	u_int div = uimin(ratio, jcc_div->jcd_maxdiv);

//	struct jh7100_clkc_div *jcc_gate = &jcc->jcc_gate;

//	CLK_LOCK(sc);

	jh7100_clkc_update(sc, jcc,
	    __SHIFTIN(div, JH7100_CLK_DIV_MASK), JH7100_CLK_DIV_MASK);
#endif

//	CLK_UNLOCK(sc);

	return 0;
}

const char *
jh7100_clkc_fracdiv_get_parent(struct jh7100_clkc_softc *sc,
    struct jh7100_clkc_clk *jcc)
{
	KASSERT(jcc->jcc_type == JH7100CLK_FRACDIV);

	struct jh7100_clkc_fracdiv *jcc_fracdiv = &jcc->jcc_fracdiv;

	return jcc_fracdiv->jcd_parent;
}



/*
 * INV operations
 */

static int
jh7100_clkc_inv_set_rate(struct jh7100_clkc_softc *sc,
    struct jh7100_clkc_clk *jcc, u_int new_rate)

{
	KASSERT(jcc->jcc_type == JH7100CLK_INV);

//	struct jh7100_clkc_inv * const jcc_inv = &jcc->jcc_inv;
	struct clk * const clk = &jcc->jcc_base;
	struct clk * const clk_parent = clk_get_parent(clk);

	// XXXNH Make sure INV bit is it???
	if (clk_parent == NULL)
		return ENXIO;

printf("%s: %u (set parent)\n", __func__, new_rate);

	return clk_set_rate(clk_parent, new_rate);
}


const char *
jh7100_clkc_inv_get_parent(struct jh7100_clkc_softc *sc,
    struct jh7100_clkc_clk *jcc)
{
	struct jh7100_clkc_inv * const jci = &jcc->jcc_inv;

	KASSERT(jcc->jcc_type == JH7100CLK_INV);

//printf("%s: '%s' has parent '%s'\n", __func__, jcc->jcc_base.name, jci->jci_parent);
	return jci->jci_parent;
}



static struct jh7100_clkc_clkops jh7100_clkc_gate_ops = {
	.jcco_enable = jh7100_clkc_gate_enable,
	.jcco_getparent = jh7100_clkc_gate_get_parent,
};

static struct jh7100_clkc_clkops jh7100_clkc_div_ops = {
	.jcco_setrate = jh7100_clkc_div_set_rate,
	.jcco_getrate = jh7100_clkc_div_get_rate,
	.jcco_getparent = jh7100_clkc_div_get_parent,
};

static struct jh7100_clkc_clkops jh7100_clkc_fracdiv_ops = {
	.jcco_setrate = jh7100_clkc_fracdiv_set_rate,
	.jcco_getrate = jh7100_clkc_fracdiv_get_rate,
	.jcco_getparent = jh7100_clkc_fracdiv_get_parent,
};

#if 0
struct jh7100_clkc_clkops jh7100_clkc_gdiv_ops = {
	.jcco_enable = jh7100_clkc_gate_enable,
	.jcco_getrate = jh7100_clkc_div_get_rate,
	.jcco_setrate = jh7100_clkc_div_set_rate,
	.jcco_getparent = jh7100_clkc_gate_get_parent,	// ???
};
#endif

struct jh7100_clkc_clkops jh7100_clkc_ffactor_ops = {
	.jcco_setrate = jh7100_clkc_fixed_factor_set_rate,
	.jcco_getrate = jh7100_clkc_fixed_factor_get_rate,
	.jcco_getparent = jh7100_clkc_fixed_factor_get_parent,	// ???
};


struct jh7100_clkc_clkops jh7100_clkc_mux_ops = {
	.jcco_setparent = jh7100_clkc_mux_set_parent,
#if 0
	.jcco_getrate = jh7100_clkc_mux_get_rate,
	.jcco_setrate = jh7100_clkc_mux_set_rate,
#endif
	.jcco_getparent = jh7100_clkc_mux_get_parent,
};


struct jh7100_clkc_clkops jh7100_clkc_inv_ops = {
	.jcco_setrate = jh7100_clkc_inv_set_rate,
//	.jcco_getrate = jh7100_clkc_inv_get_rate,
	.jcco_getparent = jh7100_clkc_inv_get_parent,
};



static const char *cpundbus_root_parents[] = {
	"osc_sys", "pll0_out", "pll1_out", "pll2_out",
};

static const char *dsp_root_parents[] = {
	"osc_sys", "pll0_out", "pll1_out", "pll2_out",
};

static const char *gmacusb_root_parents[] = {
	"osc_sys", "pll0_out", "pll2_out",
};

static const char *perh0_root_parents[] = {
	"osc_sys", "pll0_out",
};

static const char *perh1_root_parents[] = {
	"osc_sys", "pll2_out",
};

static const char *pll2_refclk_parents[] = {
	"osc_sys", "osc_aud",
};

static const char *vout_root_parents[] = {
	"osc_aud", "pll0_out", "pll2_out",
};


static struct jh7100_clkc_clk jh7100_clocks[] = {
	JH7100CLKC_FIXED_FACTOR(JH7100_CLK_PLL0_OUT,	"pll0_out",	"osc_sys",	1, 40),
	JH7100CLKC_FIXED_FACTOR(JH7100_CLK_PLL1_OUT,	"pll1_out",	"osc_sys",	1, 64),
	JH7100CLKC_FIXED_FACTOR(JH7100_CLK_PLL2_OUT,	"pll2_out",	"pll2_refclk",	1, 55),

/* parents */
	JH7100CLKC_MUX(JH7100_CLK_CPUNDBUS_ROOT,	"cpundbus_root", cpundbus_root_parents),
	JH7100CLKC_MUX(JH7100_CLK_DSP_ROOT, 		"dsp_root",	 dsp_root_parents),
	JH7100CLKC_MUX(JH7100_CLK_GMACUSB_ROOT,		"gmacusb_root",  gmacusb_root_parents),
	JH7100CLKC_MUX(JH7100_CLK_PERH0_ROOT,   	"perh0_root",    perh0_root_parents),
	JH7100CLKC_MUX(JH7100_CLK_PERH1_ROOT,   	"perh1_root",    perh1_root_parents),
	JH7100CLKC_MUX(JH7100_CLK_PLL2_REF,		"pll2_refclk",   pll2_refclk_parents),

	JH7100CLKC_MUX(JH7100_CLK_VOUT_ROOT,		"vout_root",	 vout_root_parents),
	JH7100CLKC_MUX(JH7100_CLK_VOUTBUS_ROOT,		"voutbus_root",	 vout_root_parents),

	JH7100CLKC_DIV(JH7100_CLK_CPUNBUS_ROOT_DIV,	"cpunbus_root_div",
									 2, "cpundbus_root"),
	JH7100CLKC_DIV(JH7100_CLK_PERH0_SRC,		"perh0_src",	 4, "perh0_root"),
	JH7100CLKC_DIV(JH7100_CLK_PERH1_SRC,		"perh1_src",	 4, "perh1_root"),

	JH7100CLKC_DIV(JH7100_CLK_AHB_BUS,		"ahb_bus",	 8, "cpunbus_root_div"),
	JH7100CLKC_DIV(JH7100_CLK_APB1_BUS,		"apb1_bus",	 8, "ahb_bus"),
	JH7100CLKC_DIV(JH7100_CLK_APB2_BUS,		"apb2_bus",	 8, "ahb_bus"),
	JH7100CLKC_DIV(JH7100_CLK_CPU_CORE,		"cpu_core",	 8, "cpunbus_root_div"),
	JH7100CLKC_DIV(JH7100_CLK_CPU_AXI,		"cpu_axi",	 8, "cpu_core"),
	JH7100CLKC_DIV(JH7100_CLK_GMAC_ROOT_DIV,	"gmac_root_div", 8, "gmacusb_root"),
	JH7100CLKC_DIV(JH7100_CLK_DSP_ROOT_DIV,		"dsp_root_div",	 4, "dsp_root"),
	JH7100CLKC_DIV(JH7100_CLK_SGDMA1P_BUS,		"sgdma1p_bus",	 8, "cpunbus_root_div"),

	JH7100CLKC_DIV(JH7100_CLK_DISPBUS_SRC,		"dispbus_src",	 4, "voutbus_root"),

	JH7100CLKC_DIV(JH7100_CLK_DISP_BUS,		"disp_bus",	 4, "dispbus_src"),

/* used in jh7100.dtsi */

	JH7100CLKC_GATE(JH7100_CLK_UART0_APB,		"uart0_apb",	"apb1_bus"),
	JH7100CLKC_GATE(JH7100_CLK_UART1_APB,		"uart1_apb",	"apb1_bus"),
	JH7100CLKC_GATE(JH7100_CLK_UART2_APB,		"uart2_apb",	"apb2_bus"),
	JH7100CLKC_GATE(JH7100_CLK_UART3_APB,		"uart3_apb",	"apb2_bus"),
	JH7100CLKC_GATE(JH7100_CLK_SGDMA2P_AXI,		"sgdma2p_axi",	"cpu_axi"),
	JH7100CLKC_GATE(JH7100_CLK_SGDMA2P_AHB,		"sgdma2p_ahb",	"ahb_bus"),
	JH7100CLKC_GATE(JH7100_CLK_SGDMA1P_AXI,		"sgdma1p_axi",	"sgdma1p_bus"),
	JH7100CLKC_GATE(JH7100_CLK_GPIO_APB,		"gpio_apb",	"apb1_bus"),
	JH7100CLKC_GATE(JH7100_CLK_I2C0_APB,		"i2c0_apb",	"apb1_bus"),
	JH7100CLKC_GATE(JH7100_CLK_I2C1_APB,		"i2c1_apb",	"apb1_bus"),
	JH7100CLKC_GATE(JH7100_CLK_I2C2_APB,		"i2c2_apb",	"apb2_bus"),
	JH7100CLKC_GATE(JH7100_CLK_I2C3_APB,		"i2c3_apb",	"apb2_bus"),
	JH7100CLKC_GATE(JH7100_CLK_TRNG_APB,		"trng_apb",	"apb1_bus"),
	JH7100CLKC_GATE(JH7100_CLK_SEC_AHB,		"sec_ahb",	"ahb_bus"),
	JH7100CLKC_GATE(JH7100_CLK_GMAC_AHB,		"gmac_ahb",	"ahb_bus"),
	JH7100CLKC_GATE(JH7100_CLK_GMAC_AHB,		"gmac_ahb",	"ahb_bus"),
	JH7100CLKC_GATE(JH7100_CLK_JPEG_APB,		"jpeg_apb",	"apb1_bus"),
	JH7100CLKC_GATE(JH7100_CLK_PWM_APB,		"pwm_apb",	"apb2_bus"),
	JH7100CLKC_GATE(JH7100_CLK_QSPI_AHB,		"qspi_ahb",	"ahb_bus"),
	JH7100CLKC_GATE(JH7100_CLK_SPI0_APB,		"spi0_apb",	"apb1_bus"),
	JH7100CLKC_GATE(JH7100_CLK_SPI1_APB,		"spi1_apb",	"apb1_bus"),
	JH7100CLKC_GATE(JH7100_CLK_SPI2_APB,		"spi2_apb",	"apb2_bus"),
	JH7100CLKC_GATE(JH7100_CLK_SPI3_APB,		"spi3_apb",	"apb2_bus"),
	JH7100CLKC_GATE(JH7100_CLK_SDIO0_AHB,		"sdio0_ahb",	"ahb_bus"),
        JH7100CLKC_GATE(JH7100_CLK_SDIO1_AHB,		"sdio1_ahb",	"ahb_bus"),
	JH7100CLKC_GATE(JH7100_CLK_OTP_APB,		"otp_apb",	"apb1_bus"),
	JH7100CLKC_GATE(JH7100_CLK_DOM7AHB_BUS,		"dom7ahb_bus",  "ahb_bus"),
	JH7100CLKC_GATE(JH7100_CLK_AUDIO_SRC,		"audio_src",	"audio_div"),
	JH7100CLKC_GATE(JH7100_CLK_AUDIO_12288,		"audio_12288",	"osc_aud"),

	JH7100CLKC_GATE(JH7100_CLK_DISP_AXI,		"disp_axi",	"disp_bus"),

	JH7100CLKC_GATE(JH7100_CLK_WDTIMER_APB,		"wdtimer_apb",	"apb2_bus"),


	JH7100CLKC_GATEDIV(JH7100_CLK_UART0_CORE,	"uart0_core",		63, "perh1_src"),
	JH7100CLKC_GATEDIV(JH7100_CLK_UART1_CORE,	"uart1_core",		63, "perh1_src"),
	JH7100CLKC_GATEDIV(JH7100_CLK_UART2_CORE,	"uart2_core",		63, "perh0_src"),
	JH7100CLKC_GATEDIV(JH7100_CLK_UART3_CORE,	"uart3_core",		63, "perh0_src"),
	JH7100CLKC_GATEDIV(JH7100_CLK_I2C0_CORE,	"i2c0_core",		63, "perh1_src"),
	JH7100CLKC_GATEDIV(JH7100_CLK_I2C1_CORE,	"i2c1_core",		63, "perh1_src"),
	JH7100CLKC_GATEDIV(JH7100_CLK_I2C2_CORE,	"i2c2_core",		63, "perh0_src"),
	JH7100CLKC_GATEDIV(JH7100_CLK_I2C3_CORE,	"i2c3_core",		63, "perh0_src"),
	JH7100CLKC_GATEDIV(JH7100_CLK_GMAC_PTP_REF,	"gmac_ptp_refclk",	31, "gmac_root_div"),
	JH7100CLKC_GATEDIV(JH7100_CLK_VP6_CORE,		"vp6_core",		 4, "dsp_root_div"),
	JH7100CLKC_GATEDIV(JH7100_CLK_VP6_CORE,		"vp6_core",		 4, "dsp_root_div"),
	JH7100CLKC_GATEDIV(JH7100_CLK_SPI0_CORE,	"spi0_core",		63, "perh1_src"),
	JH7100CLKC_GATEDIV(JH7100_CLK_SPI1_CORE,	"spi1_core",		63, "perh1_src"),
	JH7100CLKC_GATEDIV(JH7100_CLK_SPI2_CORE,	"spi2_core",		63, "perh0_src"),
	JH7100CLKC_GATEDIV(JH7100_CLK_SPI3_CORE,	"spi3_core",		63, "perh0_src"),
	JH7100CLKC_GATEDIV(JH7100_CLK_VP6_CORE,		"vp6_core",		 4, "dsp_root_div"),
	JH7100CLKC_GATEDIV(JH7100_CLK_SDIO0_CCLKINT,	"sdio0_cclkint",	24, "perh0_src"),
	JH7100CLKC_GATEDIV(JH7100_CLK_SDIO1_CCLKINT, 	"sdio1_cclkint",	24, "perh1_src"),
	JH7100CLKC_GATEDIV(JH7100_CLK_AUDIO_ROOT,	"audio_root",		 8, "pll0_out"),
	JH7100CLKC_GATEDIV(JH7100_CLK_VOUT_SRC,		"vout_src",		 4, "vout_root"),
	JH7100CLKC_GATEDIV(JH7100_CLK_WDT_CORE,		"wdt_coreclk",		63, "perh0_src"),

	JH7100CLKC_FRACDIV(JH7100_CLK_AUDIO_DIV,	"audio_div",		"audio_root"),

	JH7100CLKC_INV(JH7100_CLK_SDIO0_CCLKINT_INV,	"sdio0_cclkint_inv",	"sdio0_cclkint"),
	JH7100CLKC_INV(JH7100_CLK_SDIO1_CCLKINT_INV,	"sdio1_cclkint_inv",	"sdio1_cclkint"),

};

static int jh7100_clkc_match(device_t, cfdata_t, void *);
static void jh7100_clkc_attach(device_t, device_t, void *);

//static u_int jh7100_clkc_get_rate(void *, struct clk *);

static const struct device_compatible_entry compat_data[] = {
	{ .compat = "starfive,jh7100-clkgen" },
	DEVICE_COMPAT_EOL
};





static struct clk *
jh7100_clkc_get(void *priv, const char *name)
{
	struct jh7100_clkc_softc * const sc = priv;

	// XXXNH Special case OSC_SYS and OSC_AUD???
	for (u_int id = 0; id < sc->sc_nclks; id++) {
		struct jh7100_clkc_clk * const jcc = &sc->sc_clk[id];

		if (strcmp(name, jcc->jcc_base.name) == 0) {
			return &jcc->jcc_base;
		}
	}

	return NULL;
}

static void
jh7100_clkc_put(void *priv, struct clk *clk)
{
}


static int
jh7100_clkc_set_rate(void *priv, struct clk *clk, u_int rate)
{
	struct jh7100_clkc_softc * const sc = priv;
	struct jh7100_clkc_clk * const jcc =
	    container_of(clk, struct jh7100_clkc_clk, jcc_base);

        if (clk->flags & CLK_SET_RATE_PARENT) {
printf("%s: XXX happened\n", __func__);
		struct clk *clk_parent = clk_get_parent(clk);
                if (clk_parent == NULL) {
                        aprint_debug("%s: no parent for %s\n", __func__,
			    jcc->jcc_base.name);
                        return ENXIO;
                }
                return clk_set_rate(clk_parent, rate);
	}

	if (jcc->jcc_ops->jcco_setrate) {
		printf("%s: set rate for '%s' rate %u\n", __func__, clk->name, rate);

		return jcc->jcc_ops->jcco_setrate(sc, jcc, rate);
	}

printf("%s: set rate for '%s' rate %u failed\n", __func__, clk->name, rate);

        return ENXIO;
}






static u_int
jh7100_clkc_get_rate(void *priv, struct clk *clk)
{
	struct jh7100_clkc_softc * const sc = priv;
	struct jh7100_clkc_clk * const jcc =
	    container_of(clk, struct jh7100_clkc_clk, jcc_base);

	// XXXNH might not need this
	if (jcc->jcc_ops->jcco_getrate)
		return jcc->jcc_ops->jcco_getrate(sc, jcc);

	struct clk * const clk_parent = clk_get_parent(clk);
	if (clk_parent == NULL) {
		aprint_debug("%s: no parent for %s\n", __func__,
		    jcc->jcc_base.name);
		return 0;
	}

printf("%s: get rate for '%s'\n", __func__, clk->name);
	return clk_get_rate(clk_parent);
}

static int
jh7100_clkc_enable(void *priv, struct clk *clk)
{
	struct jh7100_clkc_softc * const sc = priv;
	struct jh7100_clkc_clk * const jcc =
	    container_of(clk, struct jh7100_clkc_clk, jcc_base);
printf("%s: enabling '%s' (%p/%p)\n", __func__, clk->name, clk, jcc);

	struct clk * const clk_parent = clk_get_parent(clk);
	if (clk_parent == NULL) {
		int error = clk_enable(clk_parent);
		if (error != 0)
			return error;
	}

	switch (jcc->jcc_type) {
	case JH7100CLK_GATE:
		jh7100_clkc_update(sc, jcc, JH7100_CLK_ENABLE, 0);
		break;

	case JH7100CLK_DIV: {
		struct jh7100_clkc_div * const jcc_div = &jcc->jcc_div;
		if (jcc_div->jcd_flags & JH7100CLKC_DIV_GATE) {
			jh7100_clkc_update(sc, jcc, JH7100_CLK_ENABLE, 0);
			break;
		}
		/* FALLTHROUGH */
	    }

	case JH7100CLK_FIXED_FACTOR:
	case JH7100CLK_MUX:
	case JH7100CLK_INV:
		printf("%s: type %d NOP\n", __func__, jcc->jcc_type);
		break;

	default:
		printf("%s: type %d\n", __func__, jcc->jcc_type);
		return ENXIO;
	}
	return 0;
}

static int
jh7100_clkc_disable(void *priv, struct clk *clk)
{
	struct jh7100_clkc_softc * const sc = priv;
	struct jh7100_clkc_clk * const jcc =
	    container_of(clk, struct jh7100_clkc_clk, jcc_base);

	switch (jcc->jcc_type) {
	case JH7100CLK_GATE:
		jh7100_clkc_update(sc, jcc, JH7100_CLK_ENABLE, 0);
		return 0;

	default:
		return ENXIO;
	}
}



static struct jh7100_clkc_clk *
jh7100_clkc_clock_find(struct jh7100_clkc_softc *sc, const char *name)
{
	for (size_t id = 0; id < sc->sc_nclks; id++) {
		struct jh7100_clkc_clk * const jcc = &sc->sc_clk[id];

		if (jcc->jcc_base.name == NULL)
			continue;
		if (strcmp(jcc->jcc_base.name, name) == 0)
			return jcc;
	}

	return NULL;
}



static int
jh7100_clkc_set_parent(void *priv, struct clk *clk,
    struct clk *clk_parent)
{
	struct jh7100_clkc_softc * const sc = priv;
	struct jh7100_clkc_clk * const jcc =
	    container_of(clk, struct jh7100_clkc_clk, jcc_base);

printf("%s: '%s' (%p/%p)\n", __func__, clk->name, clk, jcc);

	if (jcc->jcc_ops->jcco_setparent == NULL) {
		printf("%s: ... EINVAL\n", __func__);

		return EINVAL;
	}

	return jcc->jcc_ops->jcco_setparent(sc, jcc, clk_parent->name);
}


static struct clk *
jh7100_clkc_get_parent(void *priv, struct clk *clk)
{
	struct jh7100_clkc_softc * const sc = priv;
	struct jh7100_clkc_clk * const jcc =
	    container_of(clk, struct jh7100_clkc_clk, jcc_base);

        if (jcc->jcc_ops->jcco_getparent == NULL)
                return NULL;

        const char *parent = jcc->jcc_ops->jcco_getparent(sc, jcc);
//printf("%s: '%s' parent\n", __func__, parent);
        if (parent == NULL)
                return NULL;

        struct jh7100_clkc_clk *jcc_parent = jh7100_clkc_clock_find(sc, parent);
//printf("%s: '%s' parent (%p/%p)\n", __func__, parent, &jcc_parent->jcc_base, jcc_parent);
        if (jcc_parent != NULL)
                return &jcc_parent->jcc_base;

        /* No parent in this domain, try FDT */
        return fdtbus_clock_get(sc->sc_phandle, parent);
}


static const struct clk_funcs jh7100_clkc_funcs = {
	.get = jh7100_clkc_get,
	.put = jh7100_clkc_put,
	.get_rate = jh7100_clkc_get_rate,
	.set_rate = jh7100_clkc_set_rate,
	.enable = jh7100_clkc_enable,
	.disable = jh7100_clkc_disable,
        .set_parent = jh7100_clkc_set_parent,
        .get_parent = jh7100_clkc_get_parent,
};


























#if 0
int
jh7100_clkc_mux_set_parent(struct jh7100_clkc_softc *sc,
    struct jh7100_clkc_clk *jcc, const char *name)
{
	struct sunxi_ccu_mux *mux = &clk->u.mux;
        uint32_t val;
        u_int index;

        KASSERT(clk->type == SUNXI_CCU_MUX);

        if (mux->sel == 0)
                return ENODEV;

        for (index = 0; index < mux->nparents; index++) {
                if (mux->parents[index] != NULL &&
                    strcmp(mux->parents[index], name) == 0)
                        break;
        }
        if (index == mux->nparents)
                return EINVAL;

        val = CCU_READ(sc, mux->reg);
        val &= ~mux->sel;
        val |= __SHIFTIN(index, mux->sel);
        CCU_WRITE(sc, mux->reg, val);

        return 0;
}
#endif





static struct clk *
jh7100_clkc_fdt_decode(device_t dev, int phandle, const void *data,
    size_t len)
{
	struct jh7100_clkc_softc * const sc = device_private(dev);
printf("%s: decode (len %zu)\n", __func__, len);

	if (len != 4) {
		return NULL;
	}

	u_int id = be32dec(data);
	if (id >= sc->sc_nclks) {
		return NULL;
	}
	if (sc->sc_clk[id].jcc_type == JH7100CLK_UNKNOWN) {
		printf("Unknown clock %d\n", id);
		return NULL;
	}
printf("%s: returning clock for id %d\n", __func__, id);
	return &sc->sc_clk[id].jcc_base;
}

static const struct fdtbus_clock_controller_func jh7100_clkc_fdt_funcs = {
	.decode = jh7100_clkc_fdt_decode
};

static int
jh7100_clkc_match(device_t parent, cfdata_t cf, void *aux)
{
	struct fdt_attach_args * const faa = aux;

	return of_compatible_match(faa->faa_phandle, compat_data);
}

static void
jh7100_clkc_attach(device_t parent, device_t self, void *aux)
{
	struct jh7100_clkc_softc * const sc = device_private(self);
	struct fdt_attach_args * const faa = aux;
	const int phandle = faa->faa_phandle;
	bus_addr_t addr;
	bus_size_t size;

	if (fdtbus_get_reg(phandle, 0, &addr, &size) != 0) {
		aprint_error(": couldn't get registers\n");
		return;
	}

	sc->sc_dev = self;
	sc->sc_phandle = phandle;
	sc->sc_bst = faa->faa_bst;
	if (bus_space_map(sc->sc_bst, addr, size, 0, &sc->sc_bsh) != 0) {
		aprint_error(": couldn't map registers\n");
		return;
	}

	struct clk * const osclk = fdtbus_clock_get(phandle, "osc_sys");
	if (osclk == NULL) {
		aprint_error(": couldn't get osc_sys\n");
		return;
	}
	sc->sc_osclk = clk_get_rate(osclk);

	struct clk * const oaclk = fdtbus_clock_get(phandle, "osc_aud");
	if (oaclk == NULL) {
		aprint_error(": couldn't get osc_aud\n");
		return;
	}
	sc->sc_oaclk = clk_get_rate(oaclk);

	sc->sc_clkdom.name = device_xname(self);
	sc->sc_clkdom.funcs = &jh7100_clkc_funcs;
	sc->sc_clkdom.priv = sc;

	sc->sc_clk = jh7100_clocks;
	sc->sc_nclks = __arraycount(jh7100_clocks);
	for (size_t id = 0; id < sc->sc_nclks; id++) {
		if (sc->sc_clk[id].jcc_type == JH7100CLK_UNKNOWN)
			continue;

		sc->sc_clk[id].jcc_base.domain = &sc->sc_clkdom;
#if 0
		const char *clkname = fdtbus_get_string_index(phandle,
		    "clock-output-names", clkid);
		if (clkname != NULL) {
			sc->sc_clk[clkid].name = kmem_asprintf("%s", clkname);
		}
#endif
		// Names already populated.
		clk_attach(&sc->sc_clk[id].jcc_base);
	}

	aprint_naive("\n");
	aprint_normal(": JH7100 (OSC0 %u Hz, OSC1 %u Hz)\n",
	    sc->sc_osclk, sc->sc_oaclk);


	for (size_t id = 0; id < sc->sc_nclks; id++) {
		if (sc->sc_clk[id].jcc_type == JH7100CLK_UNKNOWN)
			continue;

		struct clk * const clk = &sc->sc_clk[id].jcc_base;

		aprint_debug_dev(self, "\n\nid %zu [%s]: %u Hz\n", id,
		    clk->name ? clk->name : "<none>", clk_get_rate(clk));
	}

	fdtbus_register_clock_controller(self, phandle, &jh7100_clkc_fdt_funcs);
}

CFATTACH_DECL_NEW(jh7100_clkc, sizeof(struct jh7100_clkc_softc),
	jh7100_clkc_match, jh7100_clkc_attach, NULL, NULL);
