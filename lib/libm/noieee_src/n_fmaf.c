/*	$NetBSD$	*/

#include <sys/cdefs.h>
#if defined(LIBM_SCCS) && !defined(lint)
__RCSID("$NetBSD$");
#endif

#include <math.h>

float
fmaf(float x, float y, float z)
{

	return x * y + z;
}
