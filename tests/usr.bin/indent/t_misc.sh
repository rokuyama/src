#! /bin/sh
# $NetBSD: t_misc.sh,v 1.6 2021/10/24 16:46:12 rillig Exp $
#
# Copyright (c) 2021 The NetBSD Foundation, Inc.
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE NETBSD FOUNDATION, INC. AND CONTRIBUTORS
# ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
# TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
# PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR CONTRIBUTORS
# BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#
# $FreeBSD$

# Tests for indent that do not follow the input-profile-output scheme that is
# used in t_indent.

indent=$(atf_config_get usr.bin.indent.test_indent /usr/bin/indent)
nl='
'

atf_test_case 'in_place'
in_place_body()
{
	cat <<-\EOF > code.c
		int decl;
	EOF
	cat <<-\EOF > code.c.exp
		int		decl;
	EOF
	cp code.c code.c.orig

	atf_check \
	    env SIMPLE_BACKUP_SUFFIX=".bak" "$indent" code.c
	atf_check -o 'file:code.c.exp' \
	    cat code.c
	atf_check -o 'file:code.c.orig' \
	    cat code.c.bak
}

atf_test_case 'verbose_profile'
verbose_profile_body()
{
	cat <<-\EOF > .indent.pro
		-/* comment */bacc
		-v
		-fc1
	EOF
	cat <<-\EOF > before.c
		int decl;
	EOF
	cat <<-\EOF > after.c.exp
		int		decl;
	EOF
	cat <<-\EOF > stdout.exp
		profile: -fc1
		profile: -bacc
		profile: -v
		profile: -fc1
		There were 1 output lines and 0 comments
		(Lines with comments)/(Lines with code):  0.000
	EOF

	# The code in args.c function set_profile suggests that options from
	# profile files are echoed to stdout during startup. But since the
	# command line options are handled after the profile files, a '-v' in
	# the command line has no effect. That's why '-bacc' is not listed
	# in stdout, but '-fc1' is. The second round of '-bacc', '-v', '-fc1'
	# is listed because when running ATF, $HOME equals $PWD.

	atf_check \
	    -o 'file:stdout.exp' \
	    "$indent" -v before.c after.c
	atf_check \
	     -o 'file:after.c.exp' \
	     cat after.c
}

atf_test_case 'nested_struct_declarations'
nested_struct_declarations_body()
{
	# Trigger the warning about nested struct declarations.

	cat <<-\EOF > code.c
		struct s01 { struct s02 { struct s03 { struct s04 {
		struct s05 { struct s06 { struct s07 { struct s08 {
		struct s09 { struct s10 { struct s11 { struct s12 {
		struct s13 { struct s14 { struct s15 { struct s16 {
		struct s17 { struct s18 { struct s19 { struct s20 {
		struct s21 { struct s22 { struct s23 { struct s24 {
		};};};};
		};};};};
		};};};};
		};};};};
		};};};};
		};};};};
	EOF
	cat <<-\EOF > expected.out
		struct s01 {
		 struct s02 {
		  struct s03 {
		   struct s04 {
		    struct s05 {
		     struct s06 {
		      struct s07 {
		       struct s08 {
		        struct s09 {
		         struct s10 {
		          struct s11 {
		           struct s12 {
		            struct s13 {
		             struct s14 {
		              struct s15 {
		               struct s16 {
		                struct s17 {
		                 struct s18 {
		                  struct s19 {
		                   struct s20 {
		                    struct s21 {
		                     struct s22 {
		                      struct s23 {
		                       struct s24 {
		                       };
		                      };
		                     };
		                    };
		                   };
		                  };
		                 };
		                };
		               };
		              };
		             };
		            };
		           };
		          };
		         };
		        };
		       };
		      };
		     };
		    };
		   };
		  };
		 };
		};
	EOF
	cat <<-\EOF > expected.err
		/**INDENT** Warning@5: Reached internal limit of 20 struct levels */
		/**INDENT** Warning@6: Reached internal limit of 20 struct levels */
		/**INDENT** Warning@6: Reached internal limit of 20 struct levels */
		/**INDENT** Warning@6: Reached internal limit of 20 struct levels */
		/**INDENT** Warning@6: Reached internal limit of 20 struct levels */
	EOF

	atf_check -o 'file:expected.out' -e 'file:expected.err' \
	    "$indent" -i1 -nut < 'code.c'
}

atf_test_case 'option_P_in_profile_file'
option_P_in_profile_file_body()
{
	# Mentioning another profile via -P has no effect since only a single
	# profile can be specified on the command line, and there is no
	# 'include' option.

	# It's syntactically possible to specify a profile file inside another
	# profile file.  Such a profile file is ignored since only a single
	# profile file is ever loaded.
	printf '%s\n' '-P/nonexistent' > .indent.pro

	echo 'syntax # error' > code.c

	atf_check -o 'inline:syntax'"$nl"'#error'"$nl" \
	    "$indent" < code.c
}

atf_test_case 'opt'
opt_body()
{
	# Test parsing of command line options from a profile file.

	cat <<-\EOF > code.c
		int global_var;

		int function(int expr) {
		switch (expr) { case 1: return 1; default: return 0; }
		}
	EOF

	cat << \EOF > .indent.pro
/* The latter of the two options wins. */
-di5
-di12

/*
 * It is possible to embed comments in the middle of an option, but nobody
 * does that.
 */
-/* comment */bacc
-T/* define
a type */custom_type

/* For int options, trailing garbage would lead to an error message. */
-i3

/*
 * For float options, trailing garbage is ignored.
 *
 * See atof.
 */
-cli3.5garbage

-b/*/acc	/* The comment is '/' '*' '/', making the option '-bacc'. */
EOF

	sed '/[$]/d' << \EOF > code.exp
/* $ The variable name is indented by 12 characters due to -di12. */
int	    global_var;

int
function(int expr)
{
   switch (expr) {
/* $ The indentation is 3 + (int)(3.5 * 3), so 3 + 10.5, so 13. */
/* $ See parse.c, function parse, 'case switch_expr'. */
	     case 1:
/* $ The indentation is 3 + (int)3.5 * 3 + 3, so 3 + 9 + 3, so 15. */
/* $ See parse.c, function parse, 'case switch_expr'. */
	       return 1;
	     default:
	       return 0;
   }
}
EOF

	atf_check -o 'file:code.exp' \
	    "$indent" code.c -st
}

atf_test_case 'opt_npro'
opt_npro_body()
{
	# Mentioning the option -npro in a .pro file has no effect since at
	# that point, indent has already decided to load the .pro file, and
	# it only decides once.

	echo ' -npro -di8' > .indent.pro
	echo 'int var;' > code.c
	printf 'int\tvar;\n' > code.exp

	atf_check -o 'file:code.exp' \
	    "$indent" code.c -st
}

atf_test_case 'opt_U'
opt_U_body()
{
	# From each line of this file, the first word is taken to be a type
	# name.
	#
	# Since neither '/*' nor '' are syntactically valid type names, this
	# means that all kinds of comments are effectively ignored.  When a
	# type name is indented by whitespace, it is ignored as well.
	#
	# Since only the first word of each line is relevant, any remaining
	# words can be used for comments.
	cat <<-\EOF > code.types
		/* Comments are effectively ignored since they never match. */
		# This comment is ignored as well.
		; So is this comment.
		# The following line is empty and adds a type whose name is empty.

		size_t			from stddef.h
		off_t			for file offsets
 		 ignored_t		is ignored since it is indented
	EOF

	cat <<-\EOF > code.c
		int known_1 = (size_t)   *   arg;
		int known_2 = (off_t)   *   arg;
		int ignored = (ignored_t)   *   arg;
	EOF
	cat <<-\EOF > code.exp
		int known_1 = (size_t)*arg;
		int known_2 = (off_t)*arg;
		int ignored = (ignored_t) * arg;
	EOF

	atf_check -o 'file:code.exp' \
	    "$indent" -Ucode.types code.c -di0 -st
}

atf_test_case 'line_no_counting'
line_no_counting_body()
{
	cat <<-\EOF > code.c
		void line_no_counting(void)
		{
			())
		}
	EOF

	# FIXME: the wrong ')' is in line 3, not 2.
	cat <<-\EOF > code.err
		/**INDENT** Warning@2: Extra ) */
	EOF

	atf_check -o 'ignore' -e 'file:code.err' \
	    "$indent" code.c -st
}

atf_init_test_cases()
{
	atf_add_test_case 'in_place'
	atf_add_test_case 'verbose_profile'
	atf_add_test_case 'nested_struct_declarations'
	atf_add_test_case 'option_P_in_profile_file'
	atf_add_test_case 'opt'
	atf_add_test_case 'opt_npro'
	atf_add_test_case 'opt_U'
	atf_add_test_case 'line_no_counting'
}
