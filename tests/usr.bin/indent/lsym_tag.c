/* $NetBSD: lsym_tag.c,v 1.6 2023/05/15 13:53:32 rillig Exp $ */

/*
 * Tests for the token lsym_tag, which represents one of the keywords
 * 'struct', 'union' or 'enum' that declare, define or use a tagged type.
 */

/* TODO: Add systematic tests for 'struct'. */
/* TODO: Add systematic tests for 'union'. */
/* TODO: Add systematic tests for 'enum'. */


//indent input
int
indent_enum_constants(void)
{
	enum color {
		red,
		green
	};
	enum color	colors[] = {
		red,
		red,
		red,
	};
	/*
	 * Ensure that the token sequence 'enum type {' only matches if there
	 * are no other tokens in between, to prevent statement continuations
	 * from being indented like enum constant declarations.
	 *
	 * See https://gnats.netbsd.org/55453.
	 */
	if (colors[0] == (enum color)1) {
		return 1
		  + 2
		  + 3;
	}
	return 0;
}
//indent end

//indent run-equals-input -ci2


//indent input
struct stat {
	mode_t		st_mode;
};

union variant {
	enum {
	}		tag;
	int		v_int;
	long		v_long;
	bool		v_bool;
	void	       *v_pointer;
};
//indent end

//indent run-equals-input


/* See FreeBSD r303485. */
//indent input
int f(struct x *a);

void
t(void)
{
	static const struct {
		int	a;
		int	b;
	} c[] = {
		{ D, E },
		{ F, G }
	};
}

void u(struct x a) {
	int b;
	struct y c = (struct y *)&a;
}
//indent end

//indent run
int		f(struct x *a);

void
t(void)
{
	static const struct {
		int		a;
		int		b;
	}		c[] = {
		{D, E},
		{F, G}
	};
}

void
u(struct x a)
{
	int		b;
	struct y	c = (struct y *)&a;
}
//indent end


/* Comment between 'struct' and the tag name; doesn't occur in practice. */
//indent input
struct   /* comment */   tag var;
//indent end

//indent run -di0
struct /* comment */ tag var;
//indent end


/*
 * Ensure that the names of struct members are all indented the same.
 * Before XXXX-XX-XX, the indentation depended on their type name.
 */
//indent input
struct {
	enum {
		untagged_constant,
	} untagged_member;
	enum tag_name {
		tagged_constant,
	} tagged_member;
};
//indent end

//indent run -di0
struct {
	enum {
		untagged_constant,
	}    untagged_member;
	/* $ FIXME: 4 spaces above, should be 1. */
	enum tag_name {
		tagged_constant,
	}        tagged_member;
	/* $ FIXME: 8 spaces above, should be 1. */
};
//indent end
