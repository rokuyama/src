/* $NetBSD: opt_bacc.c,v 1.2 2021/10/16 06:02:18 rillig Exp $ */
/* $FreeBSD$ */

/*
 * Test the options '-bacc' and '-nbacc'.
 *
 * The option '-bacc' forces a blank line around every conditional compilation
 * block.  For example, in front of every #ifdef and after every #endif.
 * Other blank lines surrounding such blocks are swallowed.
 *
 * The option '-nbacc' TODO.
 */


/* Example code without surrounding blank lines. */
#indent input
int		a;
#if 0
int		b;
#endif
int		c;
#indent end

/*
 * XXX: As of 2021-10-05, the option -bacc has no effect on declarations since
 * process_decl resets prefix_blankline_requested unconditionally.
 */
#indent run -bacc
int		a;
/* $ FIXME: expecting a blank line here */
#if 0
int		b;
#endif
/* $ FIXME: expecting a blank line here */
int		c;
#indent end

/*
 * With '-nbacc' the code is unchanged since there are no blank lines to
 * remove.
 */
#indent run -nbacc
int		a;
#if 0
int		b;
#endif
int		c;
#indent end


/* Example code containing blank lines. */
#indent input
int		space_a;

#if 0

int		space_b;

#endif

int		space_c;
#indent end

#indent run -bacc
int		space_a;
/* $ FIXME: expecting a blank line here */
#if 0

/* $ FIXME: expecting NO blank line here */
int		space_b;
#endif

int		space_c;
#indent end

/* The option '-nbacc' does not remove anything. */
#indent run -nbacc
int		space_a;

#if 0

int		space_b;

#endif

int		space_c;
#indent end

/*
 * Preprocessing directives can also occur in function bodies.
 */
#indent input
const char *
os_name(void)
{
#if defined(__NetBSD__) || defined(__FreeBSD__)
	return "BSD";
#else
	return "unknown";
#endif
}
#indent end

#indent run -bacc
/* $ XXX: The '*' should not be set apart from the rest of the return type. */
const char     *
os_name(void)
{
/* $ FIXME: expecting a blank line here. */
#if defined(__NetBSD__) || defined(__FreeBSD__)
/* $ FIXME: expecting NO blank line here. */

	return "BSD";
#else
/* $ FIXME: expecting NO blank line here. */

	return "unknown";
#endif
/* $ FIXME: expecting a blank line here. */
}
#indent end

#indent run -nbacc
const char     *
os_name(void)
{
#if defined(__NetBSD__) || defined(__FreeBSD__)
	return "BSD";
#else
	return "unknown";
#endif
}
#indent end
