/* $NetBSD: psym_for_exprs.c,v 1.2 2021/11/20 16:54:17 rillig Exp $ */
/* $FreeBSD$ */

/*
 * Tests for the parser state psym_for_exprs, which represents the state after
 * reading the keyword 'for' and the 3 expressions, now waiting for the body
 * of the loop.
 */

#indent input
// TODO: add input
#indent end

#indent run-equals-input


/*
 * Since C99, the first expression of a 'for' loop may be a declaration, not
 * only an expression.
 */
#indent input
void
function(void)
{
	for (int i = 0; i < 3; i++)
		stmt();
}
#indent end

#indent run-equals-input
