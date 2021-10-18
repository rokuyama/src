/* $NetBSD: token_comment.c,v 1.1 2021/10/18 19:36:30 rillig Exp $ */
/* $FreeBSD$ */

/*
 * Tests for formatting comments.  C99 defines block comments and end-of-line
 * comments.  Indent further distinguishes box comments that are a special
 * kind of block comments.
 *
 * See opt-fc1, opt-nfc1.
 */

/*-
 * TODO: systematically test comments
 *
 * - starting in column 1, with opt.format_col1_comments
 * - starting in column 1, without opt.format_col1_comments
 * - starting in column 9, independent of opt.format_col1_comments
 * - starting in column 33, the default
 * - starting in column 65, which is already close to the default right margin
 * - starting in column 81, spilling into the right margin
 *
 * - block comment starting with '/' '*' '-'
 * - block comment starting with '/' '*' '*'
 * - block comment starting with '/' '*' '\n'
 * - end-of-line comment starting with '//'
 * - end-of-line comment starting with '//x', so without leading space
 * - block comment starting with '/' '*' 'x', so without leading space
 *
 * - block/end-of-line comment to the right of a label
 * - block/end-of-line comment to the right of code
 * - block/end-of-line comment to the right of label with code
 *
 * - with/without opt.comment_delimiter_on_blankline
 * - with/without opt.star_comment_cont
 * - with/without opt.format_block_comments
 * - with varying opt.max_line_length (32, 64, 80, 140)
 * - with varying opt.unindent_displace (0, 2, -5)
 * - with varying opt.indent_size (3, 4, 8)
 * - with varying opt.tabsize (3, 4, 8, 16)
 * - with varying opt.block_comment_max_line_length (60, 78, 90)
 * - with varying opt.decl_comment_column (0, 1, 33, 80)
 * - with/without ps.decl_on_line
 * - with/without ps.last_nl
 */

/* For variations on this theme, try some of these options: */
/* -c20 */
/* -cd20 */
/* -cdb */
/* -d */
/* -fc1 */
/* -fcb */
/* -lc60 */
/* -sc */

#indent input
typedef enum x {
	aaaaaaaaaaaaaaaaaaaaaa = 1 << 0,	/* test a */
	bbbbbbbbbbbbbbbbb = 1 << 1,	/* test b */
	cccccccccccccc = 1 << 1,	/* test c */
	dddddddddddddddddddddddddddddd = 1 << 2	/* test d */
} x;
#indent end

#indent run-equals-input -bbb

#indent input
/* See FreeBSD r303597, r303598, r309219, and r309343 */
void
t(void) {
	/*
	 * Old indent wrapped the URL near where this sentence ends.
	 *
	 * https://www.freebsd.org/cgi/man.cgi?query=indent&apropos=0&sektion=0&manpath=FreeBSD+12-current&arch=default&format=html
	 */

	/*
	 * The default maximum line length for comments is 78, and the 'kk' at
	 * the end makes the line exactly 78 bytes long.
	 *
	 * aaaaaa bbbbbb cccccc dddddd eeeeee ffffff ggggg hhhhh iiiii jjjj kk
	 */

	/*
	 * Old indent unnecessarily removed the star comment continuation on the next line.
	 *
	 * *test*
	 */

	/* r309219 Go through linked list, freeing from the malloced (t[-1]) address. */

	/* r309343	*/
}
#indent end

#indent run -bbb
/* See FreeBSD r303597, r303598, r309219, and r309343 */
void
t(void)
{
	/*
	 * Old indent wrapped the URL near where this sentence ends.
	 *
	 * https://www.freebsd.org/cgi/man.cgi?query=indent&apropos=0&sektion=0&manpath=FreeBSD+12-current&arch=default&format=html
	 */

	/*
	 * The default maximum line length for comments is 78, and the 'kk' at
	 * the end makes the line exactly 78 bytes long.
	 *
	 * aaaaaa bbbbbb cccccc dddddd eeeeee ffffff ggggg hhhhh iiiii jjjj kk
	 */

	/*
	 * Old indent unnecessarily removed the star comment continuation on
	 * the next line.
	 *
	 * *test*
	 */

	/*
	 * r309219 Go through linked list, freeing from the malloced (t[-1])
	 * address.
	 */

	/* r309343	*/
}
#indent end

#indent input
int c(void)
{
	if (1) { /*- a christmas tree  *
				      ***
				     ***** */
		    /*- another one *
				   ***
				  ***** */
	    7;
	}

	if (1) /*- a christmas tree  *
				    ***
				   ***** */
		    /*- another one *
				   ***
				  ***** */
	    1;
}
#indent end

#indent run -bbb
int
c(void)
{
	if (1) {		/*- a christmas tree  *
					             ***
					            ***** */
		/*- another one *
			       ***
			      ***** */
		7;
	}

	if (1)			/*- a christmas tree  *
						     ***
						    ***** */
		/*- another one *
			       ***
			      ***** */
		1;
}
#indent end

/*
 * The following comments test line breaking when the comment ends with a
 * space.
 */
#indent input
/* 456789 123456789 123456789 123456789 123456789 123456789 123456789 12345 */
/* 456789 123456789 123456789 123456789 123456789 123456789 123456789 123456 */
/* 456789 123456789 123456789 123456789 123456789 123456789 123456789 1234567 */
/* 456789 123456789 123456789 123456789 123456789 123456789 123456789 12345678 */
/* 456789 123456789 123456789 123456789 123456789 123456789 123456789 123456789 */
#indent end

#indent run
/* 456789 123456789 123456789 123456789 123456789 123456789 123456789 12345 */
/*
 * 456789 123456789 123456789 123456789 123456789 123456789 123456789 123456
 */
/*
 * 456789 123456789 123456789 123456789 123456789 123456789 123456789 1234567
 */
/*
 * 456789 123456789 123456789 123456789 123456789 123456789 123456789 12345678
 */
/*
 * 456789 123456789 123456789 123456789 123456789 123456789 123456789
 * 123456789
 */
#indent end

/*
 * The following comments test line breaking when the comment does not end
 * with a space. Since indent adds a trailing space to a single-line comment,
 * this space has to be taken into account when computing the line length.
 */
#indent input
/* x							. line length 75*/
/* x							.. line length 76*/
/* x							... line length 77*/
/* x							.... line length 78*/
/* x							..... line length 79*/
/* x							...... line length 80*/
/* x							....... line length 81*/
/* x							........ line length 82*/
#indent end

#indent run
/* x							. line length 75 */
/* x							.. line length 76 */
/* x							... line length 77 */
/* x							.... line length 78 */
/*
 * x							..... line length 79
 */
/*
 * x							...... line length 80
 */
/*
 * x							....... line length 81
 */
/*
 * x							........ line length 82
 */
#indent end

/*
 * The different types of comments that indent distinguishes, starting in
 * column 1 (see options '-fc1' and '-nfc1').
 */
#indent input
/* This is a traditional C block comment. */

// This is a C99 line comment.

/*
 * This is a box comment since its first line (above this line) is empty.
 *
 *
 *
 * Its text gets wrapped.
 * Empty lines serve as paragraphs.
 */

/**
 * This is a box comment
 * that is not re-wrapped.
 */

/*-
 * This is a box comment
 * that is not re-wrapped.
 * It is often used for copyright declarations.
 */
#indent end

#indent run
/* This is a traditional C block comment. */

// This is a C99 line comment.

/*
 * This is a box comment since its first line (above this line) is empty.
 *
 *
 *
 * Its text gets wrapped. Empty lines serve as paragraphs.
 */

/**
 * This is a box comment
 * that is not re-wrapped.
 */

/*-
 * This is a box comment
 * that is not re-wrapped.
 * It is often used for copyright declarations.
 */
#indent end

/*
 * The different types of comments that indent distinguishes, starting in
 * column 9, so they are independent of the option '-fc1'.
 */
#indent input
void
function(void)
{
	/* This is a traditional C block comment. */

	/*
	 * This is a box comment.
	 *
	 * It starts in column 9, not 1,
	 * therefore it gets re-wrapped.
	 */

	/**
	 * This is a box comment
	 * that is not re-wrapped, even though it starts in column 9, not 1.
	 */

	/*-
	 * This is a box comment
	 * that is not re-wrapped.
	 * It is often used for copyright declarations.
	 */
}
#indent end

#indent run
void
function(void)
{
	/* This is a traditional C block comment. */

	/*
	 * This is a box comment.
	 *
	 * It starts in column 9, not 1, therefore it gets re-wrapped.
	 */

	/**
	 * This is a box comment
	 * that is not re-wrapped, even though it starts in column 9, not 1.
	 */

	/*-
	 * This is a box comment
	 * that is not re-wrapped.
	 * It is often used for copyright declarations.
	 */
}
#indent end

/*
 * Comments to the right of declarations.
 */
#indent input
void
function(void)
{
	int decl;	/* declaration comment */

	int decl;	/* short
			 * multi-line
			 * declaration
			 * comment */

	int decl;	/* long single-line declaration comment that is longer than the allowed line width */

	int decl;	/* long multi-line declaration comment
 * that is longer than
 * the allowed line width */

	int decl;	// C99 declaration comment

	{
		int decl;	/* indented declaration */
		{
			int decl;	/* indented declaration */
			{
				int decl;	/* indented declaration */
				{
					int decl;	/* indented declaration */
				}
			}
		}
	}
}
#indent end

#indent run -ldi0
void
function(void)
{
	int decl;		/* declaration comment */

	int decl;		/* short multi-line declaration comment */

	int decl;		/* long single-line declaration comment that
				 * is longer than the allowed line width */

	int decl;		/* long multi-line declaration comment that is
				 * longer than the allowed line width */

	int decl;		// C99 declaration comment

	{
		int decl;	/* indented declaration */
		{
			int decl;	/* indented declaration */
			{
				int decl;	/* indented declaration */
				{
					int decl;	/* indented declaration */
				}
			}
		}
	}
}
#indent end

/*
 * Comments to the right of code.
 */
#indent input
void
function(void)
{
	code();			/* code comment */
	code();			/* code comment _________ to line length 78 */
	code();			/* code comment __________ to line length 79 */
	code();			/* code comment ___________ to line length 80 */
	code();			/* code comment ____________ to line length 81 */
	code();			/* code comment _____________ to line length 82 */

/* $ In the following comments, the line length is measured after formatting. */
	code();			/* code comment _________ to line length 78*/
	code();			/* code comment __________ to line length 79*/
	code();			/* code comment ___________ to line length 80*/
	code();			/* code comment ____________ to line length 81*/
	code();			/* code comment _____________ to line length 82*/

	code();			/* short
				 * multi-line
				 * code
				 * comment */

	code();			/* long single-line code comment that is longer than the allowed line width */

	code();			/* long multi-line code comment
 * that is longer than
 * the allowed line width */

	code();			// C99 code comment
	code();			// C99 code comment ________ to line length 78
	code();			// C99 code comment _________ to line length 79
	code();			// C99 code comment __________ to line length 80
	code();			// C99 code comment ___________ to line length 81
	code();			// C99 code comment ____________ to line length 82

	if (cond) /* comment */
		if (cond) /* comment */
			if (cond) /* comment */
				if (cond) /* comment */
					if (cond) /* comment */
						code(); /* comment */
}
#indent end

#indent run
void
function(void)
{
	code();			/* code comment */
	code();			/* code comment _________ to line length 78 */
	code();			/* code comment __________ to line length 79 */
	code();			/* code comment ___________ to line length 80 */
	code();			/* code comment ____________ to line length 81 */
	code();			/* code comment _____________ to line length
				 * 82 */

/* $ In the following comments, the line length is measured after formatting. */
	code();			/* code comment _________ to line length 78 */
	code();			/* code comment __________ to line length 79 */
	code();			/* code comment ___________ to line length 80 */
	code();			/* code comment ____________ to line length 81 */
	code();			/* code comment _____________ to line length
				 * 82 */

	code();			/* short multi-line code comment */

	code();			/* long single-line code comment that is
				 * longer than the allowed line width */

	code();			/* long multi-line code comment that is longer
				 * than the allowed line width */

/* $ Trailing C99 comments are not wrapped, as indent would not correctly */
/* $ recognize the continuation lines as continued comments. For block */
/* $ comments this works since the comment has not ended yet. */
	code();			// C99 code comment
	code();			// C99 code comment ________ to line length 78
	code();			// C99 code comment _________ to line length 79
	code();			// C99 code comment __________ to line length 80
	code();			// C99 code comment ___________ to line length 81
	code();			// C99 code comment ____________ to line length 82

	if (cond)		/* comment */
		if (cond)	/* comment */
			if (cond)	/* comment */
				if (cond)	/* comment */
					if (cond)	/* comment */
						code();	/* comment */
}
#indent end

#indent input
void
function(void)
{
	code();
}

/*INDENT OFF*/
#indent end

#indent run
void
function(void)
{
	code();
}
/* $ FIXME: Missing empty line. */
/*INDENT OFF*/
 
/* $ FIXME: The line above has a trailing space. */
#indent end

/*
 * The special comments 'INDENT OFF' and 'INDENT ON' toggle whether the code
 * is formatted or kept as is.
 */
#indent input
/*INDENT OFF*/
/* No formatting takes place here. */
int format( void ) {{{
/*INDENT ON*/
}}}

/* INDENT OFF */
void indent_off ( void ) ;
/*  INDENT */
void indent_on ( void ) ;
/* INDENT OFF */
void indent_off ( void ) ;
	/* INDENT ON */
void indent_on ( void ) ;	/* the comment may be indented */
/* INDENT		OFF					*/
void indent_off ( void ) ;
/* INDENTATION ON */
void indent_still_off ( void ) ;	/* due to the word 'INDENTATION' */
/* INDENT ON * */
void indent_still_off ( void ) ;	/* due to the extra '*' at the end */
/* INDENT ON */
void indent_on ( void ) ;
/* INDENT: OFF */
void indent_still_on ( void ) ;	/* due to the colon in the middle */
/* INDENT OFF */		/* extra comment */
void indent_still_on ( void ) ;	/* due to the extra comment to the right */
#indent end

#indent run
/*INDENT OFF*/
/* No formatting takes place here. */
int format( void ) {{{
/* $ XXX: Why is the INDENT ON comment indented? */
/* $ XXX: Why does the INDENT ON comment get spaces, but not the OFF comment? */
			/* INDENT ON */
}
}
}
/* $ FIXME: The empty line disappeared but shouldn't. */
/* INDENT OFF */
void indent_off ( void ) ;
/* $ XXX: The double space from the below comment got merged to a single */
/* $ XXX: space even though the comment might be regarded to be still in */
/* $ XXX: the OFF section. */
/* INDENT */
void
indent_on(void);
/* INDENT OFF */
void indent_off ( void ) ;
/* $ XXX: The below comment got moved from column 9 to column 1. */
/* INDENT ON */
void
indent_on(void);		/* the comment may be indented */
/* INDENT		OFF					*/
void indent_off ( void ) ;
/* INDENTATION ON */
void indent_still_off ( void ) ;	/* due to the word 'INDENTATION' */
/* INDENT ON * */
void indent_still_off ( void ) ;	/* due to the extra '*' at the end */
/* INDENT ON */
void
indent_on(void);
/* INDENT: OFF */
void
indent_still_on(void);		/* due to the colon in the middle */
/* $ The extra comment got moved to the left since there is no code in */
/* $ that line. */
/* INDENT OFF *//* extra comment */
void
indent_still_on(void);		/* due to the extra comment to the right */
#indent end

#indent input
/*
	 * this
		 * is a boxed
			 * staircase.
*
* Its paragraphs get wrapped.

There may also be
		lines without asterisks.

 */
#indent end

#indent run
/*
 * this is a boxed staircase.
 *
 * Its paragraphs get wrapped.
 *
 * There may also be lines without asterisks.
 *
 */
#indent end

#indent input
void loop(void)
{
while(cond)/*comment*/;

	while(cond)
	/*comment*/;
}
#indent end

#indent run
void
loop(void)
{
	while (cond)		/* comment */
		;

	while (cond)
/* $ XXX: The spaces around the comment look unintentional. */
		 /* comment */ ;
}
#indent end

/*
 * The following comment starts really far to the right. To avoid that each
 * line only contains a single word, the maximum allowed line width is
 * extended such that each comment line may contain 22 characters.
 */
#indent input
int global_variable_with_really_long_name_that_reaches_up_to_column_xx;	/* 1234567890123456789 1 1234567890123456789 12 1234567890123456789 123 1234567890123456789 1234 1234567890123456789 12345 1234567890123456789 123456 */
#indent end

#indent run
int		global_variable_with_really_long_name_that_reaches_up_to_column_xx;	/* 1234567890123456789 1
											 * 1234567890123456789 12
											 * 1234567890123456789
											 * 123
											 * 1234567890123456789
											 * 1234
											 * 1234567890123456789
											 * 12345
											 * 1234567890123456789
											 * 123456 */
#indent end

/*
 * Demonstrates handling of line-end '//' comments.
 *
 * Even though this type of comments had been added in C99, indent didn't
 * support these comments until 2021 and instead messed up the code in
 * unpredictable ways. It treated any sequence of '/' as a binary operator,
 * no matter whether it was '/' or '//' or '/////'.
 */

#indent input
int dummy // comment
    = // eq
    1 // one
    + // plus
    2; // two

/////separator/////

void function(void){}

// Note: removing one of these line-end comments affected the formatting
// of the main function below, before indent supported '//' comments.

int
main(void)
{
}
#indent end

#indent run
int		dummy		// comment
=				// eq
1				// one
+				// plus
2;				// two

/////separator/////

void
function(void)
{
}

// Note: removing one of these line-end comments affected the formatting
// of the main function below, before indent supported '//' comments.

int
main(void)
{
}
#indent end

/*
 * Between March 2021 and October 2021, indent supported C99 comments only
 * very basically. It messed up the following code, repeating the identifier
 * 'bar' twice in a row.
 */
#indent input
void c99_comment(void)
{
foo(); // C++ comment
bar();
}
#indent end

#indent run
void
c99_comment(void)
{
	foo();			// C++ comment
	bar();
}
#indent end

#indent input
void
comment_at_end_of_function(void)
{
    if (cond)
	statement();
    // comment
}
#indent end

#indent run
void
comment_at_end_of_function(void)
{
	if (cond)
		statement();
	// comment
}
#indent end

#indent input
int		decl;
// end-of-line comment at the end of the file
#indent end
#indent run-equals-input
