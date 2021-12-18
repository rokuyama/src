/*	$NetBSD: intel_guc_ads.h,v 1.1.1.1 2021/12/18 20:15:33 riastradh Exp $	*/

/* SPDX-License-Identifier: MIT */
/*
 * Copyright © 2014-2019 Intel Corporation
 */

#ifndef _INTEL_GUC_ADS_H_
#define _INTEL_GUC_ADS_H_

struct intel_guc;

int intel_guc_ads_create(struct intel_guc *guc);
void intel_guc_ads_destroy(struct intel_guc *guc);
void intel_guc_ads_reset(struct intel_guc *guc);

#endif
