/* $NetBSD: psym_decl.c,v 1.1 2021/11/18 21:19:19 rillig Exp $ */
/* $FreeBSD$ */

/*
 * Tests for the parser symbol psym_decl, which represents a declaration.
 *
 * Since C99, declarations and statements can be mixed in blocks.
 *
 * A label can be followed by a statement but not by a declaration.
 */

// TODO: prove that psym_decl can only ever occur at the top of the stack.
// TODO: delete decl_level if the above is proven.

#indent input
// TODO: add input
#indent end

#indent run-equals-input
