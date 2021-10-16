/* $NetBSD: opt_lpl.c,v 1.3 2021/10/16 21:32:10 rillig Exp $ */
/* $FreeBSD$ */

/*
 * Tests for the options '-lpl' and '-nlpl'.
 *
 * The option '-lpl' lines up code surrounded by parentheses in continuation
 * lines, even if it would extend past the right margin.
 *
 * The option '-nlpl' moves continuation lines that would stick over the right
 * margin to the left, to keep it within the margin, if that does not require
 * placing it to the left of the prevailing indentation level.
 *
 * These switches have no effect if '-nlp' is selected.
 */

/* $ TODO: Add code that differs between -lpl and -nlpl. */

#indent input
void
example(void)
{
	int sum1 = 1+2+3+4+5+6+7+8+9+10+11+12+13+14+15+16+17+18+19+20+21;
	int sum2 = (1+2+3+4+5+6+7+8+9+10+11+12+13+14+15+16+17+18+19+20+21);

	int sum3 = 1+2+3+4+5+
		6+7+8+9+10+
		11+12+13+14+15+
		16+17+18+19+20+
		21;
	int sum4 = (1+2+3+4+5+
		6+7+8+9+10+
		11+12+13+14+15+
		16+17+18+19+20+
		21);

	call_function(call_function(call_function(call_function(call_function(call_function())))));

	call_function((call_function(call_function(call_function(call_function(call_function()))))));
}
#indent end

#indent run -lpl
void
example(void)
{
	int		sum1 = 1 + 2 + 3 + 4 + 5 + 6 + 7 + 8 + 9 + 10 + 11 + 12 + 13 + 14 + 15 + 16 + 17 + 18 + 19 + 20 + 21;
	int		sum2 = (1 + 2 + 3 + 4 + 5 + 6 + 7 + 8 + 9 + 10 + 11 + 12 + 13 + 14 + 15 + 16 + 17 + 18 + 19 + 20 + 21);

/* $ XXX: There should be at least _some_ indentation for the */
/* $ continuation lines. */
	int		sum3 = 1 + 2 + 3 + 4 + 5 +
	6 + 7 + 8 + 9 + 10 +
	11 + 12 + 13 + 14 + 15 +
	16 + 17 + 18 + 19 + 20 +
	21;
	int		sum4 = (1 + 2 + 3 + 4 + 5 +
				6 + 7 + 8 + 9 + 10 +
				11 + 12 + 13 + 14 + 15 +
				16 + 17 + 18 + 19 + 20 +
				21);

	call_function(call_function(call_function(call_function(call_function(call_function())))));

	call_function((call_function(call_function(call_function(call_function(call_function()))))));
}
#indent end

#indent run -nlpl
void
example(void)
{
	int		sum1 = 1 + 2 + 3 + 4 + 5 + 6 + 7 + 8 + 9 + 10 + 11 + 12 + 13 + 14 + 15 + 16 + 17 + 18 + 19 + 20 + 21;
	int		sum2 = (1 + 2 + 3 + 4 + 5 + 6 + 7 + 8 + 9 + 10 + 11 + 12 + 13 + 14 + 15 + 16 + 17 + 18 + 19 + 20 + 21);

/* $ XXX: There should be at least _some_ indentation for the */
/* $ continuation lines. */
	int		sum3 = 1 + 2 + 3 + 4 + 5 +
	6 + 7 + 8 + 9 + 10 +
	11 + 12 + 13 + 14 + 15 +
	16 + 17 + 18 + 19 + 20 +
	21;
	int		sum4 = (1 + 2 + 3 + 4 + 5 +
				6 + 7 + 8 + 9 + 10 +
				11 + 12 + 13 + 14 + 15 +
				16 + 17 + 18 + 19 + 20 +
				21);

	call_function(call_function(call_function(call_function(call_function(call_function())))));

	call_function((call_function(call_function(call_function(call_function(call_function()))))));
}
#indent end
