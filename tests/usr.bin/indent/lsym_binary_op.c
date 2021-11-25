/* $NetBSD: lsym_binary_op.c,v 1.2 2021/11/25 17:46:51 rillig Exp $ */
/* $FreeBSD$ */

/*
 * Tests for the token lsym_binary_op, which represents a binary operator in
 * an expression.  Examples for binary operators are '>>', '=', '+', '&&'.
 *
 * Binary operators are surrounded by blanks.
 *
 * Some tokens like '+', '*' or '&' can be either binary or unary operators,
 * with an entirely different meaning.
 *
 * The token '*' is not only a binary or a unary operator, it is used in types
 * as well, to derive a pointer type.
 *
 * See also:
 *	lsym_postfix_op.c	for postfix unary operators
 *	lsym_unary_op.c		for prefix unary operators
 *	lsym_colon.c		for ':'
 *	lsym_question.c		for '?'
 *	lsym_comma.c		for ','
 *	C99 6.4.6		"Punctuators"
 */

#indent input
// TODO: add input
#indent end

#indent run-equals-input


/*
 * If a '*' is immediately followed by another '*', they still form separate
 * operators. The first is a binary operator, the second is unary.
 */
#indent input
int var = expr**ptr;
#indent end

#indent run -di0
int var = expr * *ptr;
#indent end
