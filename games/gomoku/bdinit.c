/*	$NetBSD: bdinit.c,v 1.24 2022/05/28 08:32:55 rillig Exp $	*/

/*
 * Copyright (c) 1994
 *	The Regents of the University of California.  All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Ralph Campbell.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <sys/cdefs.h>
/*	from: @(#)bdinit.c	8.2 (Berkeley) 5/3/95	*/
__RCSID("$NetBSD: bdinit.c,v 1.24 2022/05/28 08:32:55 rillig Exp $");

#include <string.h>
#include "gomoku.h"

static void init_overlap(void);

void
bdinit(struct spotstr *bp)
{
	struct spotstr *sp;
	struct combostr *cbp;

	game.nmoves = 0;
	game.winning_spot = 0;

	/* mark the borders as such */
	sp = bp;
	for (int i = 0; i < 1 + BSZ + 1; i++, sp++) {
		sp->s_occ = BORDER;			/* top border */
		sp->s_flags = BFLAGALL;
	}

	/* fill entire board with EMPTY spots */
	memset(frames, 0, sizeof(frames));
	cbp = frames;
	for (int j = 0; ++j < BSZ + 1; sp++) {		/* for each row */
		for (int i = 0; ++i < BSZ + 1; sp++) {	/* for each column */
			sp->s_occ = EMPTY;
			sp->s_flags = 0;
			sp->s_wval = 0;
			if (j < 5) {
				/* directions 1, 2, 3 are blocked */
				sp->s_flags |= (BFLAG << 1) | (BFLAG << 2) |
				    (BFLAG << 3);
				sp->s_fval[BLACK][1].s = 0x600;
				sp->s_fval[BLACK][2].s = 0x600;
				sp->s_fval[BLACK][3].s = 0x600;
				sp->s_fval[WHITE][1].s = 0x600;
				sp->s_fval[WHITE][2].s = 0x600;
				sp->s_fval[WHITE][3].s = 0x600;
			} else if (j == 5) {
				/* five spaces, blocked on one side */
				sp->s_fval[BLACK][1].s = 0x500;
				sp->s_fval[BLACK][2].s = 0x500;
				sp->s_fval[BLACK][3].s = 0x500;
				sp->s_fval[WHITE][1].s = 0x500;
				sp->s_fval[WHITE][2].s = 0x500;
				sp->s_fval[WHITE][3].s = 0x500;
			} else {
				/* six spaces, not blocked */
				sp->s_fval[BLACK][1].s = 0x401;
				sp->s_fval[BLACK][2].s = 0x401;
				sp->s_fval[BLACK][3].s = 0x401;
				sp->s_fval[WHITE][1].s = 0x401;
				sp->s_fval[WHITE][2].s = 0x401;
				sp->s_fval[WHITE][3].s = 0x401;
			}
			if (i > (BSZ - 4)) {
				/* directions 0, 1 are blocked */
				sp->s_flags |= BFLAG | (BFLAG << 1);
				sp->s_fval[BLACK][0].s = 0x600;
				sp->s_fval[BLACK][1].s = 0x600;
				sp->s_fval[WHITE][0].s = 0x600;
				sp->s_fval[WHITE][1].s = 0x600;
			} else if (i == (BSZ - 4)) {
				sp->s_fval[BLACK][0].s = 0x500;
				sp->s_fval[WHITE][0].s = 0x500;
				/* if direction 1 is not blocked */
				if ((sp->s_flags & (BFLAG << 1)) == 0) {
					sp->s_fval[BLACK][1].s = 0x500;
					sp->s_fval[WHITE][1].s = 0x500;
				}
			} else {
				sp->s_fval[BLACK][0].s = 0x401;
				sp->s_fval[WHITE][0].s = 0x401;
				if (i < 5) {
					/* direction 3 is blocked */
					sp->s_flags |= (BFLAG << 3);
					sp->s_fval[BLACK][3].s = 0x600;
					sp->s_fval[WHITE][3].s = 0x600;
				} else if (i == 5 &&
				    (sp->s_flags & (BFLAG << 3)) == 0) {
					sp->s_fval[BLACK][3].s = 0x500;
					sp->s_fval[WHITE][3].s = 0x500;
				}
			}
			/*
			 * Allocate a frame structure for non-blocked frames.
			 */
			for (int r = 4; --r >= 0; ) {
				if ((sp->s_flags & (BFLAG << r)) != 0)
					continue;
				cbp->c_combo.s = sp->s_fval[BLACK][r].s;
				cbp->c_vertex = (u_short)(sp - board);
				cbp->c_nframes = 1;
				cbp->c_dir = r;
				sp->s_frame[r] = cbp;
				cbp++;
			}
		}
		sp->s_occ = BORDER;		/* left & right border */
		sp->s_flags = BFLAGALL;
	}

	/* mark the borders as such */
	for (int i = 0; i < BSZ + 1; i++, sp++) {
		sp->s_occ = BORDER;			/* bottom border */
		sp->s_flags = BFLAGALL;
	}

	sortframes[BLACK] = NULL;
	sortframes[WHITE] = NULL;
	init_overlap();
}

/*
 * Initialize the overlap array.
 * Each entry in the array is a bit mask with eight bits corresponding
 * to whether frame B overlaps frame A (as indexed by overlap[A * FAREA + B]).
 * The eight bits correspond to whether A and B are open-ended (length 6) or
 * closed (length 5).
 *	0	A closed and B closed
 *	1	A closed and B open
 *	2	A open and B closed
 *	3	A open and B open
 *	4	A closed and B closed and overlaps in more than one spot
 *	5	A closed and B open and overlaps in more than one spot
 *	6	A open and B closed and overlaps in more than one spot
 *	7	A open and B open and overlaps in more than one spot
 * As pieces are played, it can make frames not overlap if there are no
 * common open spaces shared between the two frames.
 */
static void
init_overlap(void)
{

	memset(overlap, 0, sizeof(overlap));
	memset(intersect, 0, sizeof(intersect));
	u_char *op = &overlap[FAREA * FAREA];
	short *ip = &intersect[FAREA * FAREA];

	for (unsigned fi = FAREA; fi-- > 0; ) {	/* each frame */
	    struct combostr *cbp = &frames[fi];
	    op -= FAREA;
	    ip -= FAREA;
	    int vertex = cbp->c_vertex;
	    struct spotstr *sp1 = &board[vertex];
	    int d1 = dd[cbp->c_dir];
	    /*
	     * s = 5 if closed, 6 if open.
	     * At this point black & white are the same.
	     */
	    int s = 5 + sp1->s_fval[BLACK][cbp->c_dir].cv_win;
	    /* for each spot in frame A */
	    for (int i = 0; i < s; i++, sp1 += d1, vertex += d1) {
		/* the sixth spot in frame A only overlaps if it is open */
		int mask = (i == 5) ? 0xC : 0xF;
		/* for each direction */
		for (int r = 4; --r >= 0; ) {
		    struct spotstr *sp2 = sp1;
		    int d2 = dd[r];
		    /* for each frame that intersects at spot sp1 */
		    for (int f = 0; f < 6; f++, sp2 -= d2) {
			if (sp2->s_occ == BORDER)
			    break;
			if ((sp2->s_flags & BFLAG << r) != 0)
			    continue;
			int n = (int)(sp2->s_frame[r] - frames);
			ip[n] = (short)vertex;
			op[n] |= (f == 5) ? mask & 0xA : mask;
			if (r == cbp->c_dir) {
			    /* compute the multiple spot overlap values */
			    switch (i) {
			    case 0:	/* sp1 is the first spot in A */
				if (f == 4)
				    op[n] |= 0xA0;
				else if (f != 5)
				    op[n] |= 0xF0;
				break;
			    case 1:	/* sp1 is the second spot in A */
				if (f == 5)
				    op[n] |= 0xA0;
				else
				    op[n] |= 0xF0;
				break;
			    case 4:	/* sp1 is the penultimate spot in A */
				if (f == 0)
				    op[n] |= 0xC0;
				else
				    op[n] |= 0xF0;
				break;
			    case 5:	/* sp1 is the last spot in A */
				if (f == 1)
				    op[n] |= 0xC0;
				else if (f != 0)
				    op[n] |= 0xF0;
				break;
			    default:
				op[n] |= 0xF0;
			    }
			}
		    }
		}
	    }
	}
}
