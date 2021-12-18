/*	$NetBSD: intel_dvo.h,v 1.1.1.1 2021/12/18 20:15:29 riastradh Exp $	*/

/* SPDX-License-Identifier: MIT */
/*
 * Copyright © 2019 Intel Corporation
 */

#ifndef __INTEL_DVO_H__
#define __INTEL_DVO_H__

struct drm_i915_private;

void intel_dvo_init(struct drm_i915_private *dev_priv);

#endif /* __INTEL_DVO_H__ */
