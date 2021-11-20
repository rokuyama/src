/*	$NetBSD: label.c,v 1.2 2021/11/20 11:13:18 rillig Exp $	*/
/* $FreeBSD: head/usr.bin/indent/tests/label.0 313544 2017-02-10 09:31:39Z pstef $ */

/* See FreeBSD r303489 */

#indent input
void t(void) {
	switch (1)
	{
		case 1:	/* test */
		case 2:		/* test */
	}
CLEANUP:
	;
V: ;
U: ;
}
#indent end

#indent run
void
t(void)
{
	switch (1) {
	case 1:			/* test */
	case 2:			/* test */
	}
CLEANUP:
	;
V:	;
U:	;
}
#indent end


#indent input
void
label_width(void)
{
L:;
L2:;
L_3:;
L__4:;
L___5:;
L____6:;
L_____7:;
L______8:;
}
#indent end

#indent run
void
label_width(void)
{
L:	;
L2:	;
L_3:	;
L__4:	;
L___5:	;
L____6:	;
L_____7:;
L______8:;
}
#indent end


/*
 * The indentation of statement labels is fixed to -2, it is not configurable.
 */
#indent input
void
label_indentation(void)
{
	if (level1) {
	if (level2) {
	if (level3) {
	if (level4) {
	if (level5) {
	label5:
	statement();
	}
	label4:
	statement();
	}
	label3:
	statement();
	}
	label2:
	statement();
	}
	label1:
	statement();
	}
	label0:
	statement();
}
#indent end

#indent run
void
label_indentation(void)
{
	if (level1) {
		if (level2) {
			if (level3) {
				if (level4) {
					if (level5) {
				label5:
						statement();
					}
			label4:
					statement();
				}
		label3:
				statement();
			}
	label2:
			statement();
		}
label1:
		statement();
	}
label0:
	statement();
}
#indent end
