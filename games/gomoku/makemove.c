/*	$NetBSD: makemove.c,v 1.26 2022/05/28 04:52:23 rillig Exp $	*/

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
/*	@(#)makemove.c	8.2 (Berkeley) 5/3/95	*/
__RCSID("$NetBSD: makemove.c,v 1.26 2022/05/28 04:52:23 rillig Exp $");

#include "gomoku.h"

		/* direction deltas */
const int     dd[4] = {
	1,			/* right */
	-(BSZ + 1) + 1,		/* down + right */
	-(BSZ + 1),		/* down */
	-(BSZ + 1) - 1		/* down + left */
};

static const int weight[5] = { 0, 1, 7, 22, 100 };

static void update_overlap(struct spotstr *);

/*
 * Return values:
 *	MOVEOK	everything is OK.
 *	RESIGN	Player resigned.
 *	ILLEGAL	Illegal move.
 *	WIN	The winning move was just played.
 *	TIE	The game is a tie.
 */
int
makemove(int us, int mv)
{

	/* check for end of game */
	if (mv == RESIGN)
		return RESIGN;

	/* check for illegal move */
	struct spotstr *sp = &board[mv];
	if (sp->s_occ != EMPTY)
		return ILLEGAL;

	/* make move */
	sp->s_occ = us;
	movelog[nmoves++] = mv;

	/* compute new frame values */
	sp->s_wval = 0;
	struct spotstr *osp = sp;
	for (int r = 4; --r >= 0; ) {		/* for each direction */
	    int d = dd[r];
	    struct spotstr *fsp = osp;
	    int bmask = BFLAG << r;

	    for (int f = 5; --f >= 0; fsp -= d) {	/* for each frame */
		if (fsp->s_occ == BORDER)
		    goto nextr;
		if ((fsp->s_flags & bmask) != 0)
		    continue;

		/* remove this frame from the sorted list of frames */
		struct combostr *cbp = fsp->s_frame[r];
		if (cbp->c_next != NULL) {
			if (sortframes[BLACK] == cbp)
			    sortframes[BLACK] = cbp->c_next;
			if (sortframes[WHITE] == cbp)
			    sortframes[WHITE] = cbp->c_next;
			cbp->c_next->c_prev = cbp->c_prev;
			cbp->c_prev->c_next = cbp->c_next;
		}

		/* compute old weight value for this frame */
		union comboval cb;
		int val = 0;
		if ((cb = fsp->s_fval[BLACK][r]).s <= 0x500)
		    val += weight[5 - cb.cv_force - cb.cv_win];
		if ((cb = fsp->s_fval[WHITE][r]).s <= 0x500)
		    val += weight[5 - cb.cv_force - cb.cv_win];

		/* compute new combo value for this frame */
		bool space = fsp->s_occ == EMPTY;
		int n = 0;
		sp = fsp;
		for (int i = 5; --i >= 0; sp += d) {	/* for each spot */
		    if (sp->s_occ == us)
			n++;
		    else if (sp->s_occ == EMPTY)
			sp->s_wval -= val;
		    else {
			/* this frame is now blocked, adjust values */
			fsp->s_flags |= bmask;
			fsp->s_fval[BLACK][r].s = 0x600;
			fsp->s_fval[WHITE][r].s = 0x600;
			while (--i >= 0) {
			    sp += d;
			    if (sp->s_occ == EMPTY)
				sp->s_wval -= val;
			}
			goto nextf;
		    }
		}

		/* check for game over */
		if (n == 5)
		    return WIN;

		/* compute new value & combo number for this frame & color */
		fsp->s_fval[us != BLACK ? BLACK : WHITE][r].s = 0x600;
		union comboval *cp = &fsp->s_fval[us][r];
		/* both ends open? */
		if (space && sp->s_occ == EMPTY) {
		    cp->cv_force = 4 - n;
		    cp->cv_win = 1;
		} else {
		    cp->cv_force = 5 - n;
		    cp->cv_win = 0;
		}
		val = weight[n];
		sp = fsp;
		for (int i = 5; --i >= 0; sp += d)	/* for each spot */
		    if (sp->s_occ == EMPTY)
			sp->s_wval += val;

		/* add this frame to the sorted list of frames by combo value */
		struct combostr *cbp1 = sortframes[us];
		if (cbp1 == NULL)
		    sortframes[us] = cbp->c_next = cbp->c_prev = cbp;
		else {
		    union comboval *cp1 =
			&board[cbp1->c_vertex].s_fval[us][cbp1->c_dir];
		    if (cp->s <= cp1->s) {
			/* insert at the head of the list */
			sortframes[us] = cbp;
		    } else {
			do {
			    cbp1 = cbp1->c_next;
			    cp1 = &board[cbp1->c_vertex].s_fval[us][cbp1->c_dir];
			    if (cp->s <= cp1->s)
				break;
			} while (cbp1 != sortframes[us]);
		    }
		    cbp->c_next = cbp1;
		    cbp->c_prev = cbp1->c_prev;
		    cbp1->c_prev->c_next = cbp;
		    cbp1->c_prev = cbp;
		}

	    nextf:
		;
	    }

	    /* both ends open? */
	    if (fsp->s_occ == EMPTY) {
		union comboval *cp = &fsp->s_fval[BLACK][r];
		if (cp->cv_win != 0) {
		    cp->cv_force++;
		    cp->cv_win = 0;
		}
		cp = &fsp->s_fval[WHITE][r];
		if (cp->cv_win != 0) {
		    cp->cv_force++;
		    cp->cv_win = 0;
		}
	    }

	nextr:
	    ;
	}

	update_overlap(osp);

	/*
	 * TODO: Declare a tie as soon as all frames are blocked. This is
	 *  usually much earlier than when the whole board is filled.
	 */
	if (nmoves == BSZ * BSZ)
		return TIE;

	return MOVEOK;
}

/*
 * fix up the overlap array due to updating spot osp.
 */
static void
update_overlap(struct spotstr *osp)
{

	for (int r = 4; --r >= 0; ) {		/* for each direction */
	    int d = dd[r];
	    struct spotstr *sp1 = osp;
	    int bmask = BFLAG << r;

	    for (int f = 0; f < 6; f++, sp1 -= d) {	/* for each frame */
		if (sp1->s_occ == BORDER)
		    break;
		if ((sp1->s_flags & bmask) != 0)
		    continue;
		/*
		 * Update all other frames that intersect the current one
		 * to indicate whether they still overlap or not.
		 * Since F1 overlap F2 == F2 overlap F1, we only need to
		 * do the rows 0 <= r1 <= r. The r1 == r case is special
		 * since the two frames can overlap at more than one point.
		 */
		int a = (int)(sp1->s_frame[r] - frames);
		u_char *str = &overlap[a * FAREA];
		struct spotstr *sp2 = sp1 - d;

		for (int i = f + 1; i < 6; i++, sp2 -= d) {
		    if (sp2->s_occ == BORDER)
			break;
		    if ((sp2->s_flags & bmask) != 0)
			continue;

		    /*
		     * count the number of empty spots to see if there is
		     * still an overlap.
		     */
		    int n = 0;
		    struct spotstr *sp = sp1;
		    struct spotstr *esp = NULL;
		    for (int b = i - f; b < 5; b++, sp += d) {
			if (sp->s_occ == EMPTY) {
			    esp = sp;	/* save the intersection point */
			    n++;
			}
		    }

		    int b = (int)(sp2->s_frame[r] - frames);
		    if (n == 0) {
			if (sp->s_occ == EMPTY) {
			    str[b] &= 0xA;
			    overlap[b * FAREA + a] &= 0xC;
			    intersect[a * FAREA + b] = (short)(sp - board);
			    intersect[b * FAREA + a] = (short)(sp - board);
			} else {
			    str[b] = 0;
			    overlap[b * FAREA + a] = 0;
			}
		    } else if (n == 1) {
			if (sp->s_occ == EMPTY) {
			    str[b] &= 0xAF;
			    overlap[b * FAREA + a] &= 0xCF;
			} else {
			    str[b] &= 0xF;
			    overlap[b * FAREA + a] &= 0xF;
			}
			intersect[a * FAREA + b] = (short)(esp - board);
			intersect[b * FAREA + a] = (short)(esp - board);
		    }
		    /* else no change, still multiple overlap */
		}

		/* the other directions can only intersect at spot osp */
		for (int r1 = r; --r1 >= 0; ) {
		    int d1 = dd[r1];
		    int bmask1 = BFLAG << r1;
		    struct spotstr *sp = osp;
		    for (int i = 6; --i >= 0; sp -= d1) { /* for each spot */
			if (sp->s_occ == BORDER)
			    break;
			if ((sp->s_flags & bmask1) != 0)
			    continue;
			int b = (int)(sp->s_frame[r1] - frames);
			str[b] = 0;
			overlap[b * FAREA + a] = 0;
		    }
		}
	    }
	}
}
