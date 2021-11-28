/* $NetBSD: lsym_return.c,v 1.2 2021/11/28 16:31:39 rillig Exp $ */
/* $FreeBSD$ */

/*
 * Tests for the token lsym_return, which represents the keyword 'return' that
 * starts a 'return' statement for leaving the execution of a function.
 */

/*
 * Return statements having a single-line expression are simple to format.
 * Since 'return' is not a function name, there is a space between the
 * 'return' and the '('.
 */
#indent input
void
function(bool cond)
{
	if (cond)
		return;
}

int
calculate(int a, int b)
{
	return a;
	return (b);
	return (((a))) + b;
	return calculate(b, a);
}
#indent end

#indent run-equals-input


/*
 * Returning complex expressions may spread the expression over several lines.
 * The exact formatting depends on the option '-lp'.
 */
#indent input
int
multi_line(int a)
{
	return calculate(3,
			 4);
	return calculate(
			 3,
			 4);
	return calculate(
			 3,
			 4
		);
}
#indent end

#indent run-equals-input

#indent run -nlp
int
multi_line(int a)
{
	return calculate(3,
		4);
	return calculate(
		3,
		4);
	return calculate(
		3,
		4
		);
}
#indent end
