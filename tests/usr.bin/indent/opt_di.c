/* $NetBSD: opt_di.c,v 1.2 2021/10/24 17:51:19 rillig Exp $ */
/* $FreeBSD$ */

/*
 * Test the option '-di', which specifies the indentation of the variable
 * declarator.
 */

#indent input
int space;
int	tab;
int		tab16;

struct long_name long_name;
#indent end

#indent run -di8
int	space;
int	tab;
int	tab16;

struct long_name long_name;
#indent end


/*
 * The declarator can be a simple variable name. It can also be prefixed by
 * asterisks, for pointer variables. These asterisks are placed to the left of
 * the indentation line, so that the variable names are aligned.
 *
 * There can be multiple declarators in a single declaration, separated by
 * commas. Only the first of them is aligned to the indentation given by
 * '-di', the others are separated with a single space.
 */
#indent input
int var;
int *ptr, *****ptr;
#indent end

#indent run -di12
int	    var;
int	   *ptr, *****ptr;
#indent end


/*
 * Test the various values for indenting.
 */
#indent input
int decl ;
#indent end

/*
 * An indentation of 0 columns uses a single space between the declaration
 * specifiers (in this case 'int') and the declarator.
 */
#indent run -di0
int decl;
#indent end

/*
 * An indentation of 7 columns uses spaces for indentation since in the
 * default configuration, the next tab stop would be at indentation 8.
 */
#indent run -di7
int    decl;
#indent end

/* The indentation consists of a single tab. */
#indent run -di8
int	decl;
#indent end

/* The indentation consists of a tab and a space. */
#indent run -di9
int	 decl;
#indent end

#indent run -di16
int		decl;
#indent end


/*
 * Ensure that all whitespace is normalized to be indented by 8 columns,
 * which in the default configuration amounts to a single tab.
 */

#indent input
int space;
int	tab;
int		tab16;
struct long_name long_name;
#indent end

#indent run -di8
int	space;
int	tab;
int	tab16;
struct long_name long_name;
#indent end
