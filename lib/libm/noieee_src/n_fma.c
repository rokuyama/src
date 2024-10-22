/*	$NetBSD$	*/

#include <sys/cdefs.h>
#if defined(LIBM_SCCS) && !defined(lint)
__RCSID("$NetBSD$");
#endif

#include <math.h>

#ifndef __HAVE_LONG_DOUBLE
__weak_alias(fmal, fma)
#endif

double
fma(double x, double y, double z)
{

	return x * y + z;
}
