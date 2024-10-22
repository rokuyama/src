/*	$NetBSD$	*/

#include <sys/cdefs.h>
__RCSID("$NetBSD: n_lroundf.c,v 1.1 2010/12/09 22:52:59 abs Exp $");

#define	stype	float
#define	dtype	long
#define	ceilit	ceilf
#define	fn	lroundf

#include "n_roundf.c"
