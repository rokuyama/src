/* $NetBSD: token_decl.c,v 1.3 2022/04/24 09:04:12 rillig Exp $ */

/*
 * Tests for declarations.
 *
 * Indent distinguishes global and local declarations.
 *
 * Declarations can be for functions or for variables.
 */

//indent input
int global_var;
int global_array = [1,2,3,4];
int global_array = [
1
,2,
3,
4,
];
//indent end

//indent run -di0
int global_var;
int global_array = [1, 2, 3, 4];
int global_array = [
		    1
		    ,2,
		    3,
		    4,
];
//indent end
