/*	$NetBSD: intel_huc_fw.h,v 1.1.1.1 2021/12/18 20:15:33 riastradh Exp $	*/

/* SPDX-License-Identifier: MIT */
/*
 * Copyright © 2014-2019 Intel Corporation
 */

#ifndef _INTEL_HUC_FW_H_
#define _INTEL_HUC_FW_H_

struct intel_huc;

void intel_huc_fw_init_early(struct intel_huc *huc);
int intel_huc_fw_upload(struct intel_huc *huc);

#endif
