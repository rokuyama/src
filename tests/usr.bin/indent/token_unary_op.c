/* $NetBSD: token_unary_op.c,v 1.2 2021/11/04 17:12:12 rillig Exp $ */
/* $FreeBSD$ */

/*
 * Tests for unary operators, such as '+', '-', '*', '&'.
 */

#indent input
int var=+3;
int mixed=+-+-+-+-+-+-+-+-+-+-+-+-+-3;
int count=~-~-~-~-~-~-~-~-~-~-~-~-~-3;
int same = + + + + + - - - - - 3;
#indent end

#indent run -di0
int var = +3;
int mixed = +-+-+-+-+-+-+-+-+-+-+-+-+-3;
int count = ~-~-~-~-~-~-~-~-~-~-~-~-~-3;
int same = + + + + +- - - - -3;
#indent end
