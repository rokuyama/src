# SPDX-License-Identifier: GPL-2.0
dtb-$(CONFIG_SOC_SIFIVE) += hifive-unleashed-a00.dtb \
			    hifive-unmatched-a00.dtb
dtb-$(CONFIG_SOC_SIFIVE) += starfive_vic7100_beagle_v.dtb
dtb-${CONFIG_SOC_SIFIVE} += starfive_vic7100_evb.dtb

obj-$(CONFIG_BUILTIN_DTB) += $(addsuffix .o, $(dtb-y))
