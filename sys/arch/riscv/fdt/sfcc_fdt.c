/*	$NetBSD$	*/

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
 * ``AS IS'' AND ANY EXPRESS OR IMPLinIED WARRANTIES, INCLUDING, BUT NOT LIMITED
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

#include <sys/bus.h>
#if 0
#include <sys/cpu.h>
#include <sys/device.h>
#include <sys/evcnt.h>
#include <sys/intr.h>
#endif

#include <dev/fdt/fdtvar.h>

//#include <riscv/fdt/riscv_fdtvar.h>

#include <machine/cpufunc.h>

#define SFCC_L2_CONFIG			0x0000
#define  SFCC_L2_CONFIG_BANKS_MASK	__BITS( 7,  0)
#define  SFCC_L2_CONFIG_WAYS_MASK	__BITS(15,  8)
#define  SFCC_L2_CONFIG_LGSETS_MASK	__BITS(23, 16)
#define  SFCC_L2_CONFIG_LGBB_MASK	__BITS(31, 24)
#define SFCC_L2_WAYENABLE		0x0008

#define SFCC_L2_INJECTERROR		0x0040

#define SFCC_L2_DIRECCFIX_LOW		0x0100
#define SFCC_L2_DIRECCFIX_HIGH		0x0104
#define SFCC_L2_DIRECCFIX_COUNT 	0x0108

#define SFCC_L2_DIRECCFAIL_LOW		0x0120
#define SFCC_L2_DIRECCFAIL_HIGH 	0x0124
#define SFCC_L2_DIRECCFAIL_COUNT 	0x0128

#define SFCC_L2_DATECCFIX_LOW		0x0140
#define SFCC_L2_DATECCFIX_HIGH		0x0144
#define SFCC_L2_DATECCFIX_COUNT		0x0148

#define SFCC_L2_DATECCFAIL_LOW		0x0160
#define SFCC_L2_DATECCFAIL_HIGH		0x0164
#define SFCC_L2_DATECCFAIL_COUNT	0x0168

#define SFCC_L2_FLUSH64			0x0200

#define SFCC_L2_MAX_ECCINTR		4

#define SFCC_L2_FLUSH64_LINE_LEN	64

static const struct device_compatible_entry compat_data[] = {
	{ .compat = "sifive,fu540-c000-ccache" },
	{ .compat = "sifive,fu740-c000-ccache" },
	{ .compat = "starfive,jh7100-ccache" },
	{ .compat = "starfive,jh7110-ccache" },
	{ .compat = "starfive,ccache0" },
	DEVICE_COMPAT_EOL
};

struct sfcc_fdt_softc {
	device_t		 sc_dev;
	bus_space_tag_t		 sc_bst;
	bus_space_handle_t	 sc_bsh;

	uint32_t		 sc_line_size;
	uint32_t		 sc_size;
	uint32_t		 sc_sets;
	uint32_t		 sc_level;
};


static struct sfcc_fdt_softc *sfcc_sc;

#define SFCC_READ(sc, reg) \
	bus_space_read_4((sc)->sc_bst, (sc)->sc_bsh, (reg))
#define	SFCC_WRITE(sc, reg, val) \
	bus_space_write_4((sc)->sc_bst, (sc)->sc_bsh, (reg), (val))

#if 0
static int
sfcc_intr(void *arg)
{
//	struct clockframe * const cf = arg;
//	struct sfcc_fdt_softc * const sc = sfcc_sc;
	return 1;
}
#endif

#define	WR8(sc, reg, val)						\
	bus_space_write_8((sc)->sc_bst, (sc)->sc_bsh, (reg), (val))

static void
sfcc_cache_wbinv_range(vaddr_t va, paddr_t pa, psize_t len)
{
	struct sfcc_fdt_softc * const sc = sfcc_sc;

	KASSERT(powerof2(sc->sc_line_size));

	const paddr_t spa = rounddown2(pa, sc->sc_line_size);
	const paddr_t epa = roundup2(pa + len, sc->sc_line_size);

	asm volatile ("fence iorw,iorw" ::: "memory");

	for (paddr_t fpa = spa; fpa < epa; fpa += sc->sc_line_size) {
		WR8(sc, SFCC_L2_FLUSH64, fpa);
		asm volatile ("fence iorw,iorw" ::: "memory");
	}
}


static int
sfcc_match(device_t parent, cfdata_t cf, void *aux)
{
	struct fdt_attach_args * const faa = aux;

	return of_compatible_match(faa->faa_phandle, compat_data);
}

static void
sfcc_attach(device_t parent, device_t self, void *aux)
{
	struct sfcc_fdt_softc * const sc = device_private(self);
	const struct fdt_attach_args * const faa = aux;
	const int phandle = faa->faa_phandle;
	bus_addr_t addr;
	bus_size_t size;

	if (fdtbus_get_reg(phandle, 0, &addr, &size) != 0) {
		aprint_error(": couldn't get registers\n");
		return;
	}

	sc->sc_dev = self;
	sc->sc_bst = faa->faa_bst;

	if (bus_space_map(sc->sc_bst, addr, size, 0, &sc->sc_bsh) != 0) {
		aprint_error(": couldn't map registers\n");
		return;
	}

	int ret;
	ret = of_getprop_uint32(phandle, "cache-block-size",
	    &sc->sc_line_size);
	if (ret < 0) {
		aprint_error(": can't get cache-block-size\n");
		return;
	}

	ret = of_getprop_uint32(phandle, "cache-level",
	    &sc->sc_level);
	if (ret < 0) {
		aprint_error(": can't get cache-level\n");
		return;
	}

	ret = of_getprop_uint32(phandle, "cache-sets",
	    &sc->sc_sets);
	if (ret < 0) {
		aprint_error(": can't get cache-sets\n");
		return;
	}
	ret = of_getprop_uint32(phandle, "cache-size",
	    &sc->sc_size);
	if (ret < 0) {
		aprint_error(": can't get cache-size\n");
		return;
	}

	if (!of_hasprop(phandle, "cache-unified")) {
		aprint_error(": can't get cache-unified\n");
		return;
	}

	uint32_t ways = sc->sc_size / (sc->sc_sets * sc->sc_line_size);

	aprint_naive("\n");
	aprint_normal(": L%u cache controller. %u KiB/%uB %u-way (%u set).\n",
	    sc->sc_level, sc->sc_size / 1024, sc->sc_line_size, ways,
	    sc->sc_sets);

	sfcc_sc = sc;

	cpu_sdcache_wbinv_range = sfcc_cache_wbinv_range;
	cpu_sdcache_inv_range = sfcc_cache_wbinv_range;
	cpu_sdcache_wb_range = sfcc_cache_wbinv_range;
}

CFATTACH_DECL_NEW(sfcc_fdt, sizeof(struct sfcc_fdt_softc),
	sfcc_match, sfcc_attach, NULL, NULL);
