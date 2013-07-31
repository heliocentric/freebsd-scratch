#	@(#)Makefile	8.1 (Berkeley) 7/19/93
# $FreeBSD$

PROG=	authd
NO_MAN=

PRECIOUSPROG=
INSTALLFLAGS=-b -B.bak
DPADD=	${LIBUTIL} ${LIBV}
LDADD=	-lutil -lcrypt
DPADD+=  ${LIBMD}                                                                                                            
LDADD+=  -lmd -lV

NO_SHARED?=	YES

.include <bsd.prog.mk>
