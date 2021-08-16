/*	$NetBSD: msg_277.c,v 1.5 2021/08/16 20:11:03 rillig Exp $	*/
# 3 "msg_277.c"

// Test for message: initialization of '%s' with '%s' [277]

/* lint1-extra-flags: -e */

enum E {
	E1
};

void sink_enum(enum E);
void sink_int(int);

void
example(enum E e, int i)
{
	enum E e2 = e;
	enum E e3 = { i };	/* expect: 277 */
	int i2 = { e };		/* expect: 277 */
	int i3 = i;

	sink_enum(e2);
	sink_enum(e3);
	sink_int(i2);
	sink_int(i3);

	enum E init_0 = 0;
	/* expect+1: warning: initialization of 'enum E' with 'int' [277] */
	enum E init_1 = 1;
}
