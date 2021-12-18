/*	$NetBSD: intel_dp_aux_backlight.h,v 1.1.1.1 2021/12/18 20:15:29 riastradh Exp $	*/

/* SPDX-License-Identifier: MIT */
/*
 * Copyright © 2019 Intel Corporation
 */

#ifndef __INTEL_DP_AUX_BACKLIGHT_H__
#define __INTEL_DP_AUX_BACKLIGHT_H__

struct intel_connector;

int intel_dp_aux_init_backlight_funcs(struct intel_connector *intel_connector);

#endif /* __INTEL_DP_AUX_BACKLIGHT_H__ */
