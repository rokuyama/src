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

#ifndef _SIFIVE_JH7100CLKC_H
#define _SIFIVE_JH7100CLKC_H

#include <dev/clk/clk_backend.h>
#include <dev/fdt/syscon.h>

/*
 * Each clock has a 32-bit register indexed from the register base with
 * the following bit field definitions depending on type.
 */

/* register fields */
#define JH7100_CLK_ENABLE	__BIT(31)
#define JH7100_CLK_INVERT	__BIT(30)
#define JH7100_CLK_MUX_MASK	__BITS(27, 24)
#define JH7100_CLK_DIV_MASK	__BITS(23, 0)
#define JH7100_CLK_FRAC_MASK	__BITS(15, 8)
#define JH7100_CLK_INT_MASK	__BITS(7, 0)

/* fractional divider min/max */
#define JH7100_CLK_FRAC_MIN	100
#define JH7100_CLK_FRAC_MAX	(26600 - 1)

struct jh7100_clkc_softc;
struct jh7100_clkc_clk;

//XXXXNH good above
















#if 0
struct jh7100_clkc_reset;
/*
 * Resets
 */

struct jh7100_clkc_reset {
	bus_size_t	reg;
	uint32_t	mask;
};

#define	JH7100CLKC_RESET(_id, _reg, _bit)	\
	[_id] = {				\
		.reg = (_reg),			\
		.mask = __BIT(_bit),		\
	}
#endif

/*
 * Clocks
 */

enum jh7100_clkc_clktype {
	JH7100CLK_UNKNOWN,
	JH7100CLK_FIXED_FACTOR,
	JH7100CLK_GATE,
	JH7100CLK_DIV,
	JH7100CLK_FRACDIV,
	JH7100CLK_MUX,
	JH7100CLK_INV,

#if 0
	JH7100CLK_FIXED,
	JH7100CLK_PLL,
	JH7100CLK_MPLL,
#endif
#if 0
	JH7100CLK_GMD			// What's this?
	JH7100CLK_GMUX			// What's this?
	JH7100CLK_FDIV			// What's this?
	JH7100CLK_GDIV			// What's this?
	JH7100CLK_MDIV			// What's this?
#endif

};

#if 0
/*
 * Fixed clocks
 */

struct jh7100_clkc_fixed {
	u_int		jcf_rate;
};

u_int	jh7100_clkc_fixed_get_rate(struct jh7100_clkc_softc *,
	    struct jh7100_clkc_clk *);

#define	JH7100CLKC_FIXED(_id, _name, _rate)				      \
	[_id] = {							      \
		.jcc_type = JH7100CLK_FIXED,				      \
		.jcc_base.name = (_name),				      \
		.jcc_base.flags = 0,					      \
		.jcc_fixed.jcf_rate = (_rate),				      \
		.jcc_ops = &jh7100_clkc_fixed_ops,			      \
	}
#endif

#if 0

#define JH7100CLKC_EXT(_id, _name)					      \
	[_id] = {							      \
		.type = JH7100CLK_EXTERNAL,				      \
		.base.name = (_name),					      \
		.base.flags = 0,					      \
		.u.extclk = (_name),                    	              \
		.jcc_ops = &jh7100_clkc_ext_ops,			      \
	}

#endif

/*
 * Fixed-factor clocks
 */

struct jh7100_clkc_fixed_factor {
	const char *	jcff_parent;
	u_int		jcff_div;
	u_int		jcff_mult;
};

u_int	jh7100_clkc_fixed_factor_get_rate(struct jh7100_clkc_softc *,
	    struct jh7100_clkc_clk *);
int	jh7100_clkc_fixed_factor_set_rate(struct jh7100_clkc_softc *,
	    struct jh7100_clkc_clk *, u_int);
const char *
	jh7100_clkc_fixed_factor_get_parent(struct jh7100_clkc_softc *,
	    struct jh7100_clkc_clk *);

#define	JH7100CLKC_FIXED_FACTOR(_id, _name, _parent, _div, _mult)	      \
	[_id] = {							      \
		.jcc_type = JH7100CLK_FIXED_FACTOR,			      \
		.jcc_base.name = (_name),				      \
		.jcc_ffactor.jcff_parent = (_parent),			      \
		.jcc_ffactor.jcff_div = (_div),				      \
		.jcc_ffactor.jcff_mult = (_mult),			      \
		.jcc_ops = &jh7100_clkc_ffactor_ops,			      \
	}

/*
 * Gate clocks
 */

struct jh7100_clkc_gate {
//	bus_size_t	jcg_reg;
	const char	*jcg_parent;
#if 0
//	uint32_t	jcg_mask;
	uint32_t	flags;
#define	JH7100CLKC_GATE_SET_TO_DISABLE		__BIT(0)
#endif

};

int	jh7100_clkc_gate_enable(struct jh7100_clkc_softc *,
	    struct jh7100_clkc_clk *, int);
const char *
	jh7100_clkc_gate_get_parent(struct jh7100_clkc_softc *,
	    struct jh7100_clkc_clk *);

#define	JH7100CLKC_GATE(_id, _name, _pname)				      \
	[_id] = {							      \
		.jcc_type = JH7100CLK_GATE,				      \
		.jcc_base = {						      \
			.name = (_name),				      \
			.flags = CLK_SET_RATE_PARENT,			      \
		},							      \
		.jcc_reg = (_id) * sizeof(uint32_t),			      \
		.jcc_gate.jcg_parent = (_pname),			      \
		.jcc_ops = &jh7100_clkc_gate_ops,			      \
	}

/*
 * Divider clocks
 */

struct jh7100_clkc_div {
	bus_size_t	jcd_reg;
	const char *	jcd_parent;
	uint32_t	jcd_maxdiv;
	uint32_t	jcd_flags;
#define	JH7100CLKC_DIV_GATE	__BIT(0)
};

u_int	jh7100_clkc_div_get_rate(struct jh7100_clkc_softc *,
	    struct jh7100_clkc_clk *);
int	jh7100_clkc_div_set_rate(struct jh7100_clkc_softc *,
	    struct jh7100_clkc_clk *, u_int);
const char *
	jh7100_clkc_div_get_parent(struct jh7100_clkc_softc *,
	    struct jh7100_clkc_clk *);

#define	JH7100CLKC_DIV_FLAGS(_id, _name, _maxdiv, _parent, _flags)	      \
	[_id] = {							      \
		.jcc_type = JH7100CLK_DIV,				      \
		.jcc_base = {						      \
			.name = (_name),				      \
		},							      \
		.jcc_reg = (_id) * sizeof(uint32_t),			      \
		.jcc_div = {						      \
			.jcd_parent = (_parent),			      \
			.jcd_maxdiv = (_maxdiv),			      \
			.jcd_flags = (_flags),				      \
		},							      \
		.jcc_ops = &jh7100_clkc_div_ops,			      \
	}

#define	JH7100CLKC_DIV(_id, _n, _m, _p)		  			      \
    JH7100CLKC_DIV_FLAGS((_id), (_n), (_m), (_p), 0)

#define	JH7100CLKC_GATEDIV(_id, _n, _m, _p)				      \
    JH7100CLKC_DIV_FLAGS((_id), (_n), (_m), (_p), JH7100CLKC_DIV_GATE)

/*
 * Fractional Divider clocks
 */

struct jh7100_clkc_fracdiv {
	bus_size_t	jcd_reg;
	const char *	jcd_parent;
	uint32_t	jcd_flags;
#define	JH7100CLKC_DIV_GATE	__BIT(0)
};

u_int	jh7100_clkc_fracdiv_get_rate(struct jh7100_clkc_softc *,
	    struct jh7100_clkc_clk *);
int	jh7100_clkc_fracdiv_set_rate(struct jh7100_clkc_softc *,
	    struct jh7100_clkc_clk *, u_int);
const char *
	jh7100_clkc_fracdiv_get_parent(struct jh7100_clkc_softc *,
	    struct jh7100_clkc_clk *);

#define	JH7100CLKC_FRACDIV(_id, _name, _parent)				      \
	[_id] = {							      \
		.jcc_type = JH7100CLK_FRACDIV,				      \
		.jcc_base = {						      \
			.name = (_name),				      \
		},							      \
		.jcc_reg = (_id) * sizeof(uint32_t),			      \
		.jcc_fracdiv = {					      \
			.jcd_parent = (_parent),			      \
		},							      \
		.jcc_ops = &jh7100_clkc_fracdiv_ops,			      \
	}



/*
 * Mux clocks
 */

struct jh7100_clkc_mux {
	size_t		jcm_nparents;
	const char **	jcm_parents;
};

int	jh7100_clkc_mux_set_parent(struct jh7100_clkc_softc *,
	    struct jh7100_clkc_clk *, const char *);
const char *
	jh7100_clkc_mux_get_parent(struct jh7100_clkc_softc *,
	    struct jh7100_clkc_clk *);

#if 0
#define	JH7100CLKC_MUX_RATE(_id, _name, _parents, _reg, _sel,		\
			   _getratefn, _setratefn, _flags)		\
	[_id] = {							\
		.jcc_type = JH7100CLK_MUX,					\
		.jcc_base.name = (_name),					\
		.jcc_base.flags = 0,					\
		.jcc_mux.parents = (_parents),				\
		.jcc_mux.nparents = __arraycount(_parents),		\
		.jcc_mux.reg = (_reg),					\
		.jcc_mux.sel = (_sel),					\
		.jcc_mux.flags = (_flags),				\
		.jcc_getrate = _getratefn,					\
		.jcc_setrate = _setratefn,					\
		.jcc_getparent = jh7100_clkc_mux_get_parent,			\
	}
#endif

#define	JH7100CLKC_MUX(_id, _name, _parents)				      \
	[_id] = {							      \
		.jcc_type = JH7100CLK_MUX,				      \
		.jcc_base = {						      \
			.name = (_name),				      \
		},							      \
		.jcc_reg = (_id) * sizeof(uint32_t),			      \
		.jcc_mux = {						      \
			.jcm_parents = (_parents),			      \
			.jcm_nparents = __arraycount(_parents),		      \
		},							      \
		.jcc_ops = &jh7100_clkc_mux_ops,			      \
	}

#if 0
/*
 * PLL clocks
 */

struct jh7100_clkc_pll_reg {
	bus_size_t	reg;
	uint32_t	mask;
};

#define	JH7100CLKC_PLL_REG(_reg, _mask)					\
	{ .reg = (_reg), .mask = (_mask) }
#define	JH7100CLKC_PLL_REG_INVALID	JH7100CLK_PLL_REG(0,0)

struct jh7100_clkc_pll {
	struct jh7100_clkc_pll_reg	jcp_enable;
	struct jh7100_clkc_pll_reg	jcp_m;
	struct jh7100_clkc_pll_reg	jcp_n;
	struct jh7100_clkc_pll_reg	jcp_frac;
	struct jh7100_clkc_pll_reg	jcp_l;
	struct jh7100_clkc_pll_reg	jcp_reset;
	const char			*jcp_parent;
	uint32_t			jcp_flags;
};

u_int	jh7100_clkc_pll_get_rate(struct jh7100_clkc_softc *,
	    struct jh7100_clkc_clk *);
int	jh7100_clkc_pll_set_rate(struct jh7100_clkc_softc *,
	    struct jh7100_clkc_clk *, u_int new_rate);
const char *jh7100_clkc_pll_get_parent(struct jh7100_clkc_softc *,
	    struct jh7100_clkc_clk *);
int	jh7100_clkc_pll_wait_lock(struct jh7100_clkc_softc *sc,
	    struct jh7100_clkc_pll *pll);


#define	JH7100CLKC_PLL_RATE(_id, _name, _parent, _enable, _m, _n, _frac, _l,	\
		      _reset, _setratefn, _flags)			\
	[_id] = {							\
		.jcc_type = JH7100CLK_PLL,					\
		.jcc_base.name = (_name),					\
		.pll.parent = (_parent),				\
		.pll.enable = _enable,				\
		.pll.m = _m,						\
		.pll.n = _n,						\
		.pll.frac = _frac,					\
		.pll.l = _l,						\
		.pll.reset = _reset,					\
		.pll.flags = (_flags),				\
		.jcc_setrate = (_setratefn),				\
		.jcc_getrate = jh7100_clkc_pll_get_rate,			\
		.jcc_getparent = jh7100_clkc_pll_get_parent,			\
	}

#define	JH7100CLKC_PLL(_id, _name, _parent, _enable, _m, _n, _frac, _l,	\
		      _reset, _flags)					\
	[_id] = {							\
		.jcc_type = JH7100CLK_PLL,					\
		.jcc_base.name = (_name),					\
		.pll.parent = (_parent),				\
		.pll.enable = _enable,				\
		.pll.m = _m,						\
		.pll.n = _n,						\
		.pll.frac = _frac,					\
		.pll.l = _l,						\
		.pll.reset = _reset,					\
		.pll.flags = (_flags),				\
		.jcc_getrate = jh7100_clkc_pll_get_rate,			\
		.jcc_getparent = jh7100_clkc_pll_get_parent,			\
	}
#endif

#if 0
/*
 * MPLL clocks
 */

struct jh7100_clkc_mpll {
	struct jh7100_clkc_pll_reg	sdm;
	struct jh7100_clkc_pll_reg	sdm_enable;
	struct jh7100_clkc_pll_reg	n2;
	struct jh7100_clkc_pll_reg	ssen;
	const char			*parent;
	uint32_t			flags;
};

u_int	jh7100_clkc_mpll_get_rate(struct jh7100_clkc_softc *,
				struct jh7100_clkc_clk *);
const char *jh7100_clkc_mpll_get_parent(struct jh7100_clkc_softc *,
				      struct jh7100_clkc_clk *);

#define	JH7100CLKC_MPLL(_id, _name, _parent, _sdm, _sdm_enable, _n2,	\
		       _ssen, _flags)					\
	[_id] = {							\
		.jcc_type = JH7100CLK_MPLL,					\
		.jcc_base.name = (_name),					\
		.mpll.parent = (_parent),				\
		.mpll.sdm = _sdm,					\
		.mpll.sdm_enable = _sdm_enable,				\
		.mpll.n2 = _n2,						\
		.mpll.ssen = _ssen,					\
		.mpll.flags = (_flags),					\
		.jcc_getrate = jh7100_clkc_mpll_get_rate,			\
		.jcc_getparent = jh7100_clkc_mpll_get_parent,		\
	}
#endif







struct jh7100_clkc_inv {
	const char *	jci_parent;
};

const char *
jh7100_clkc_inv_get_parent(struct jh7100_clkc_softc *sc,
    struct jh7100_clkc_clk *jcc);



#define	JH7100CLKC_INV(_id, _name, _pname)				      \
	[_id] = {							      \
		.jcc_type = JH7100CLK_INV,				      \
		.jcc_base = {						      \
			.name = (_name),				      \
			.flags = CLK_SET_RATE_PARENT,			      \
		},							      \
		.jcc_reg = (_id) * sizeof(uint32_t),			      \
		.jcc_inv.jci_parent = (_pname),				      \
		.jcc_ops = &jh7100_clkc_inv_ops,			      \
	}

















struct jh7100_clkc_clkops {

	int		(*jcco_enable)(struct jh7100_clkc_softc *,
			    struct jh7100_clkc_clk *, int);
	u_int		(*jcco_getrate)(struct jh7100_clkc_softc *,
			    struct jh7100_clkc_clk *);
	int		(*jcco_setrate)(struct jh7100_clkc_softc *,
			    struct jh7100_clkc_clk *, u_int);
#if 0
// XXXNH used?
	u_int		(*jcc_roundrate)(struct jh7100_clkc_softc *,
			    struct jh7100_clkc_clk *, u_int);
#endif
	const char *    (*jcco_getparent)(struct jh7100_clkc_softc *,
			    struct jh7100_clkc_clk *);
	int		(*jcco_setparent)(struct jh7100_clkc_softc *,
			    struct jh7100_clkc_clk *, const char *);
};


struct jh7100_clkc_clk {
	struct clk		 		jcc_base;	// XXXNH rename jcc_clk
	enum jh7100_clkc_clktype 		jcc_type;
	bus_size_t				jcc_reg;
	union {
//		struct jh7100_clkc_fixed 	jcc_fixed;
		struct jh7100_clkc_gate		jcc_gate;
		struct jh7100_clkc_div		jcc_div;
		struct jh7100_clkc_fracdiv	jcc_fracdiv;
		struct jh7100_clkc_fixed_factor jcc_ffactor;
		struct jh7100_clkc_mux		jcc_mux;
		struct jh7100_clkc_inv		jcc_inv;
//		struct jh7100_clkc_pll		jcc_pll;
//		struct jh7100_clkc_mpll		jcc_mpll;
	};
	struct jh7100_clkc_clkops *		jcc_ops;
};

#endif
