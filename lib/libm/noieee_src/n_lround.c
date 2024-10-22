/*	$NetBSD$	*/

#include <sys/cdefs.h>
#if defined(LIBM_SCCS) && !defined(lint)
__RCSID("$NetBSD: n_lround.c,v 1.1 2010/12/09 22:52:59 abs Exp $");
#endif

#include <math.h>

#ifndef __HAVE_LONG_DOUBLE
__weak_alias(lroundl, lround)
#endif

#define	stype	double
#define	dtype	long
#define	ceilit	ceil
#define	fn	lround

#include "n_roundf.c"
