/* $NetBSD: token_semicolon.c,v 1.4 2022/04/24 09:04:12 rillig Exp $ */

/*
 * Tests for the token ';'.
 *
 * The ';' ends a declaration.
 *
 * The ';' ends a statement.
 *
 * The ';' separates the 3 expressions in the head of the 'for' loop.
 */

//indent input
struct {
	int member;
} global_var;
//indent end

//indent run-equals-input -di0


//indent input
void
function(void)
{
	for ( ; ; )
		stmt();
	for (;;)
		stmt();
}
//indent end

//indent run
void
function(void)
{
	for (;;)
		stmt();
	for (;;)
		stmt();
}
//indent end
