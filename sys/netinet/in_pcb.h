/*	$NetBSD: in_pcb.h,v 1.72 2022/10/28 05:23:09 ozaki-r Exp $	*/

/*
 * Copyright (C) 1995, 1996, 1997, and 1998 WIDE Project.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the project nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE PROJECT AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE PROJECT OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/*
 * Copyright (c) 1982, 1986, 1990, 1993
 *	The Regents of the University of California.  All rights reserved.
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
 *
 *	@(#)in_pcb.h	8.1 (Berkeley) 6/10/93
 */

#ifndef _NETINET_IN_PCB_H_
#define _NETINET_IN_PCB_H_

#include <sys/types.h>

#include <net/route.h>

#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip6.h>

typedef int (*pcb_overudp_cb_t)(struct mbuf **, int, struct socket *,
    struct sockaddr *, void *);

struct ip_moptions;
struct mbuf;
struct icmp6_filter;

/*
 * Common structure pcb for internet protocol implementation.
 * Here are stored pointers to local and foreign host table
 * entries, local and foreign socket numbers, and pointers
 * up (to a socket structure) and down (to a protocol-specific)
 * control block.
 */

struct inpcb {
	LIST_ENTRY(inpcb) inp_hash;
	LIST_ENTRY(inpcb) inp_lhash;
	TAILQ_ENTRY(inpcb) inp_queue;
	int	  inp_af;		/* address family - AF_INET or AF_INET6 */
	void *	  inp_ppcb;		/* pointer to per-protocol pcb */
	int	  inp_state;		/* bind/connect state */
#define	INP_ATTACHED		0
#define	INP_BOUND		1
#define	INP_CONNECTED		2
	int       inp_portalgo;
	struct	  socket *inp_socket;	/* back pointer to socket */
	struct	  inpcbtable *inp_table;
	struct	  inpcbpolicy *inp_sp;	/* security policy */
	struct route	inp_route;	/* placeholder for routing entry */
	u_int16_t	inp_fport;	/* foreign port */
	u_int16_t	inp_lport;	/* local port */
	int	 	inp_flags;	/* generic IP/datagram flags */
	union {				/* header prototype. */
		struct ip inp_ip;
		struct ip6_hdr inp_ip6;
	};
#define	inp_flowinfo	inp_ip6.ip6_flow
	struct mbuf	*inp_options;	/* IP options */
	bool		inp_bindportonsend;

	/* We still need both for IPv6 due to v4-mapped addresses */
	struct ip_moptions *inp_moptions;	/* IPv4 multicast options */
	struct ip6_moptions *inp_moptions6;	/* IPv6 multicast options */

	union {
		/* IPv4 only stuffs */
		struct {
			int	inp_errormtu;	/* MTU of last xmit status = EMSGSIZE */
			uint8_t	inp_ip_minttl;
			struct in_addr	inp_prefsrcip; /* preferred src IP when wild  */
		};
		/* IPv6 only stuffs */
		struct {
			int	inp_hops6;	/* default IPv6 hop limit */
			int	inp_cksum6;	/* IPV6_CHECKSUM setsockopt */
			struct icmp6_filter	*inp_icmp6filt;
			struct ip6_pktopts	*inp_outputopts6; /* IP6 options for outgoing packets */
		};
	};

	pcb_overudp_cb_t	inp_overudp_cb;
	void		*inp_overudp_arg;
};

#define	inp_faddr	inp_ip.ip_dst
#define	inp_laddr	inp_ip.ip_src
#define inp_faddr6	inp_ip6.ip6_dst
#define inp_laddr6	inp_ip6.ip6_src

LIST_HEAD(inpcbhead, inpcb);

/* flags in inp_flags: */
#define	INP_RECVOPTS		0x0001	/* receive incoming IP options */
#define	INP_RECVRETOPTS		0x0002	/* receive IP options for reply */
#define	INP_RECVDSTADDR		0x0004	/* receive IP dst address */
#define	INP_HDRINCL		0x0008	/* user supplies entire IP header */
#define	INP_HIGHPORT		0x0010	/* (unused; FreeBSD compat) */
#define	INP_LOWPORT		0x0020	/* user wants "low" port binding */
#define	INP_ANONPORT		0x0040	/* port chosen for user */
#define	INP_RECVIF		0x0080	/* receive incoming interface */
/* XXX should move to an UDP control block */
#define INP_ESPINUDP		0x0100	/* ESP over UDP for NAT-T */
#define INP_ESPINUDP_NON_IKE	0x0200	/* ESP over UDP for NAT-T */
#define INP_NOHEADER		0x0400	/* Kernel removes IP header
					 * before feeding a packet
					 * to the raw socket user.
					 * The socket user will
					 * not supply an IP header.
					 * Cancels INP_HDRINCL.
					 */
#define	INP_RECVTTL		0x0800	/* receive incoming IP TTL */
#define	INP_RECVPKTINFO		0x1000	/* receive IP dst if/addr */
#define	INP_BINDANY		0x2000	/* allow bind to any address */
#define	INP_CONTROLOPTS		(INP_RECVOPTS|INP_RECVRETOPTS|INP_RECVDSTADDR|\
				INP_RECVIF|INP_RECVTTL|INP_RECVPKTINFO)

/*
 * Flags for IPv6 in inp_flags
 * We define KAME's original flags in higher 16 bits as much as possible
 * for compatibility with *bsd*s.
 */
#define IN6P_RECVOPTS		0x00001000 /* receive incoming IP6 options */
#define IN6P_RECVRETOPTS	0x00002000 /* receive IP6 options for reply */
#define IN6P_RECVDSTADDR	0x00004000 /* receive IP6 dst address */
#define IN6P_IPV6_V6ONLY	0x00008000 /* restrict AF_INET6 socket for v6 */
#define IN6P_PKTINFO		0x00010000 /* receive IP6 dst and I/F */
#define IN6P_HOPLIMIT		0x00020000 /* receive hoplimit */
#define IN6P_HOPOPTS		0x00040000 /* receive hop-by-hop options */
#define IN6P_DSTOPTS		0x00080000 /* receive dst options after rthdr */
#define IN6P_RTHDR		0x00100000 /* receive routing header */
#define IN6P_RTHDRDSTOPTS	0x00200000 /* receive dstoptions before rthdr */
#define IN6P_TCLASS		0x00400000 /* traffic class */
#define IN6P_BINDANY		0x00800000 /* allow bind to any address */
#define IN6P_HIGHPORT		0x01000000 /* user wants "high" port binding */
#define IN6P_LOWPORT		0x02000000 /* user wants "low" port binding */
#define IN6P_ANONPORT		0x04000000 /* port chosen for user */
#define IN6P_FAITH		0x08000000 /* accept FAITH'ed connections */
/* XXX should move to an UDP control block */
#define IN6P_ESPINUDP		INP_ESPINUDP /* ESP over UDP for NAT-T */

#define IN6P_RFC2292		0x40000000 /* RFC2292 */
#define IN6P_MTU		0x80000000 /* use minimum MTU */

#define IN6P_CONTROLOPTS	(IN6P_PKTINFO|IN6P_HOPLIMIT|IN6P_HOPOPTS|\
				 IN6P_DSTOPTS|IN6P_RTHDR|IN6P_RTHDRDSTOPTS|\
				 IN6P_TCLASS|IN6P_RFC2292|\
				 IN6P_MTU)

#define	sotoinpcb(so)		((struct inpcb *)(so)->so_pcb)
#define soaf(so) 		(so->so_proto->pr_domain->dom_family)
#define	inp_lock(inp)		solock((inp)->inp_socket)
#define	inp_unlock(inp)		sounlock((inp)->inp_socket)
#define	inp_locked(inp)		solocked((inp)->inp_socket)

TAILQ_HEAD(inpcbqueue, inpcb);

struct vestigial_hooks;

/* It's still referenced by kvm users */
struct inpcbtable {
	struct	  inpcbqueue inpt_queue;
	struct	  inpcbhead *inpt_porthashtbl;
	struct	  inpcbhead *inpt_bindhashtbl;
	struct	  inpcbhead *inpt_connecthashtbl;
	u_long	  inpt_porthash;
	u_long	  inpt_bindhash;
	u_long	  inpt_connecthash;
	u_int16_t inpt_lastport;
	u_int16_t inpt_lastlow;

	struct vestigial_hooks *vestige;
};
#define inpt_lasthi inpt_lastport

#ifdef _KERNEL

#include <sys/kauth.h>
#include <sys/queue.h>

struct lwp;
struct rtentry;
struct sockaddr_in;
struct socket;
struct vestigial_inpcb;

void	in_losing(struct inpcb *);
int	in_pcballoc(struct socket *, void *);
int	in_pcbbindableaddr(const struct inpcb *, struct sockaddr_in *,
    kauth_cred_t);
int	in_pcbbind(void *, struct sockaddr_in *, struct lwp *);
int	in_pcbconnect(void *, struct sockaddr_in *, struct lwp *);
void	in_pcbdetach(void *);
void	in_pcbdisconnect(void *);
void	in_pcbinit(struct inpcbtable *, int, int);
struct inpcb *
	in_pcblookup_port(struct inpcbtable *,
			  struct in_addr, u_int, int, struct vestigial_inpcb *);
struct inpcb *
	in_pcblookup_bind(struct inpcbtable *,
	    struct in_addr, u_int);
struct inpcb *
	in_pcblookup_connect(struct inpcbtable *,
			     struct in_addr, u_int, struct in_addr, u_int,
			     struct vestigial_inpcb *);
int	in_pcbnotify(struct inpcbtable *, struct in_addr, u_int,
	    struct in_addr, u_int, int, void (*)(struct inpcb *, int));
void	in_pcbnotifyall(struct inpcbtable *, struct in_addr, int,
	    void (*)(struct inpcb *, int));
void	in_pcbpurgeif0(struct inpcbtable *, struct ifnet *);
void	in_pcbpurgeif(struct inpcbtable *, struct ifnet *);
void	in_purgeifmcast(struct ip_moptions *, struct ifnet *);
void	in_pcbstate(struct inpcb *, int);
void	in_rtchange(struct inpcb *, int);
void	in_setpeeraddr(struct inpcb *, struct sockaddr_in *);
void	in_setsockaddr(struct inpcb *, struct sockaddr_in *);
struct rtentry *
	in_pcbrtentry(struct inpcb *);
void	in_pcbrtentry_unref(struct rtentry *, struct inpcb *);

void	in6_pcbinit(struct inpcbtable *, int, int);
int	in6_pcbbind(void *, struct sockaddr_in6 *, struct lwp *);
int	in6_pcbconnect(void *, struct sockaddr_in6 *, struct lwp *);
void	in6_pcbdetach(struct inpcb *);
void	in6_pcbdisconnect(struct inpcb *);
struct	inpcb *in6_pcblookup_port(struct inpcbtable *, struct in6_addr *,
				   u_int, int, struct vestigial_inpcb *);
int	in6_pcbnotify(struct inpcbtable *, const struct sockaddr *,
	u_int, const struct sockaddr *, u_int, int, void *,
	void (*)(struct inpcb *, int));
void	in6_pcbpurgeif0(struct inpcbtable *, struct ifnet *);
void	in6_pcbpurgeif(struct inpcbtable *, struct ifnet *);
void	in6_pcbstate(struct inpcb *, int);
void	in6_rtchange(struct inpcb *, int);
void	in6_setpeeraddr(struct inpcb *, struct sockaddr_in6 *);
void	in6_setsockaddr(struct inpcb *, struct sockaddr_in6 *);

/* in in6_src.c */
int	in6_selecthlim(struct inpcb *, struct ifnet *);
int	in6_selecthlim_rt(struct inpcb *);
int	in6_pcbsetport(struct sockaddr_in6 *, struct inpcb *, struct lwp *);

extern struct rtentry *
	in6_pcbrtentry(struct inpcb *);
extern void
	in6_pcbrtentry_unref(struct rtentry *, struct inpcb *);
extern struct inpcb *in6_pcblookup_connect(struct inpcbtable *,
					    const struct in6_addr *, u_int, const struct in6_addr *, u_int, int,
					    struct vestigial_inpcb *);
extern struct inpcb *in6_pcblookup_bind(struct inpcbtable *,
	const struct in6_addr *, u_int, int);

static inline void
in_pcb_register_overudp_cb(struct inpcb *inp, pcb_overudp_cb_t cb, void *arg)
{

	inp->inp_overudp_cb = cb;
	inp->inp_overudp_arg = arg;
}

/* compute hash value for foreign and local in6_addr and port */
#define IN6_HASH(faddr, fport, laddr, lport) 			\
	(((faddr)->s6_addr32[0] ^ (faddr)->s6_addr32[1] ^	\
	  (faddr)->s6_addr32[2] ^ (faddr)->s6_addr32[3] ^	\
	  (laddr)->s6_addr32[0] ^ (laddr)->s6_addr32[1] ^	\
	  (laddr)->s6_addr32[2] ^ (laddr)->s6_addr32[3])	\
	 + (fport) + (lport))

// from in_pcb_hdr.h
struct vestigial_inpcb;
struct in6_addr;

/* Hooks for vestigial pcb entries.
 * If vestigial entries exist for a table (TCP only)
 * the vestigial pointer is set.
 */
typedef struct vestigial_hooks {
	/* IPv4 hooks */
	void	*(*init_ports4)(struct in_addr, u_int, int);
	int	(*next_port4)(void *, struct vestigial_inpcb *);
	int	(*lookup4)(struct in_addr, uint16_t,
			   struct in_addr, uint16_t,
			   struct vestigial_inpcb *);
	/* IPv6 hooks */
	void	*(*init_ports6)(const struct in6_addr*, u_int, int);
	int	(*next_port6)(void *, struct vestigial_inpcb *);
	int	(*lookup6)(const struct in6_addr *, uint16_t,
			   const struct in6_addr *, uint16_t,
			   struct vestigial_inpcb *);
} vestigial_hooks_t;

#endif	/* _KERNEL */

#endif	/* !_NETINET_IN_PCB_H_ */
