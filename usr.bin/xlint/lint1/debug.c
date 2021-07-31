/* $NetBSD: debug.c,v 1.1 2021/07/31 18:16:42 rillig Exp $ */

/*-
 * Copyright (c) 2021 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by Roland Illig <rillig@NetBSD.org>.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE NETBSD FOUNDATION, INC. AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#if HAVE_NBTOOL_CONFIG_H
#include "nbtool_config.h"
#endif

#include <sys/cdefs.h>
#if defined(__RCSID) && !defined(lint)
__RCSID("$NetBSD: debug.c,v 1.1 2021/07/31 18:16:42 rillig Exp $");
#endif

#include "lint1.h"


#ifdef DEBUG

static int debug_indentation = 0;


void __printflike(1, 2)
debug_printf(const char *fmt, ...)
{
	va_list va;

	va_start(va, fmt);
	vfprintf(stdout, fmt, va);
	va_end(va);
}

void
debug_indent(void)
{

	debug_printf("%*s", 2 * debug_indentation, "");
}

void
debug_indent_inc(void)
{

	debug_indentation++;
}

void
debug_indent_dec(void)
{

	debug_indentation--;
}

void
(debug_enter)(const char *func)
{

	printf("%*s+ %s\n", 2 * debug_indentation++, "", func);
}

void __printflike(1, 2)
debug_step(const char *fmt, ...)
{
	va_list va;

	debug_indent();
	va_start(va, fmt);
	vfprintf(stdout, fmt, va);
	va_end(va);
	printf("\n");
}

void
(debug_leave)(const char *func)
{

	printf("%*s- %s\n", 2 * --debug_indentation, "", func);
}

void
debug_node(const tnode_t *tn, int indent)
{
	op_t op;

	if (tn == NULL) {
		printf("%*s" "null\n", indent, "");
		return;
	}

	op = tn->tn_op;
	printf("%*s%s with type '%s'%s%s",
	    2 * indent, "",
	    op == CVT && !tn->tn_cast ? "convert" : modtab[op].m_name,
	    type_name(tn->tn_type), tn->tn_lvalue ? ", lvalue" : "",
	    tn->tn_parenthesized ? ", parenthesized" : "");

	if (op == NAME)
		printf(" %s\n", tn->tn_sym->s_name);
	else if (op == CON && is_floating(tn->tn_type->t_tspec))
		printf(", value %Lg", tn->tn_val->v_ldbl);
	else if (op == CON && is_uinteger(tn->tn_type->t_tspec))
		printf(", value %llu\n", (unsigned long long)tn->tn_val->v_quad);
	else if (op == CON && is_integer(tn->tn_type->t_tspec))
		printf(", value %lld\n", (long long)tn->tn_val->v_quad);
	else if (op == CON)
		printf(", unknown value\n");
	else if (op == STRING)
		printf(", length %zu\n", tn->tn_string->st_len);
	else {
		printf("\n");

		debug_node(tn->tn_left, indent + 1);
		if (modtab[op].m_binary || tn->tn_right != NULL)
			debug_node(tn->tn_right, indent + 1);
	}
}

#endif
