#	@(#)Makefile	8.1 (Berkeley) 7/19/93
# $FreeBSD$

PROG=	nexusd
NO_MAN=

PRECIOUSPROG=
INSTALLFLAGS=-b -B.bak
CFLAGS+=-DDEBUGSHELL -DSECURE -DLOGIN_CAP -DCOMPAT_SYSV_INIT
DPADD=	${LIBUTIL} ${LIBCRYPT} ${LIBEVOKE}
LDADD=	-lutil -lcrypt -levoke
DPADD+=  ${LIBMD}                                                                                                            
LDADD+=  -lmd

NO_SHARED?=	YES

.include <bsd.prog.mk>
