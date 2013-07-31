#	@(#)Makefile	8.1 (Berkeley) 7/19/93
# $FreeBSD$

PROG=	mountd
NO_MAN=

PRECIOUSPROG=
INSTALLFLAGS=-b -B.bak
WARNS?=	6
DPADD+=  ${LIBEVOKE}
LDADD+=  -levoke

.include <bsd.prog.mk>
