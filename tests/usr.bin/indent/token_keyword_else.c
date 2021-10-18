/* $NetBSD: token_keyword_else.c,v 1.1 2021/10/18 22:30:34 rillig Exp $ */
/* $FreeBSD$ */

/*
 * Tests for the keyword 'else'.
 *
 * When parsing nested incomplete 'if' statements, the problem of the
 * 'dangling else' occurs.  It is resolved by binding the 'else' to the
 * innermost incomplete 'if' statement.
 */

/*
 * In 'parse', an if_expr_stmt is reduced to a simple statement, unless the
 * next token is 'else'. The comment does not influence this since it never
 * reaches 'parse'.
 */
#indent input
void
example(bool cond)
{
	if (cond)
	if (cond)
	if (cond)
	stmt();
	else
	stmt();
	/* comment */
	else
	stmt();
}
#indent end

#indent run
void
example(bool cond)
{
	if (cond)
		if (cond)
			if (cond)
				stmt();
			else
				stmt();
	/* comment */
		else
			stmt();
}
#indent end
