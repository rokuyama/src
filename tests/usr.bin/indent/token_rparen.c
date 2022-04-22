/* $NetBSD: token_rparen.c,v 1.2 2022/04/22 21:21:20 rillig Exp $ */

/*
 * Tests for the token ')', which ends the corresponding ')', as well as ']',
 * which ends the corresponding ']'.
 */

#indent input
int var = (3);
int cast = (int)3;
int cast = (int)(3);
int call = function(3);
int array[3] = {1, 2, 3};
int array[3] = {[2] = 3};
#indent end

#indent run-equals-input -di0
