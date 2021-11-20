/* $NetBSD: token_newline.c,v 1.2 2021/11/20 16:54:17 rillig Exp $ */
/* $FreeBSD$ */

/*-
 * Tests for the token '\n', which ends a line.
 *
 * A newline ends an end-of-line comment that has been started with '//'.
 *
 * When a line ends with a backslash immediately followed by '\n', these two
 * characters are merged and continue the logical line (C11 5.1.1.2p1i2).
 *
 * In other contexts, a newline is an ordinary space character from a
 * compiler's point of view. Indent preserves most line breaks though.
 */

#indent input
int var=
1
	+2
		+3
			+4;
#indent end

#indent run
int		var =
1
+ 2
+ 3
+ 4;
#indent end
