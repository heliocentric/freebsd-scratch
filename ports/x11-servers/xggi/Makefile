# New ports collection makefile for:   xggi
# Date created:        3 December 2009
# Whom:                Dylan Cochran <a134qaed@gmail.com>
#
# $FreeBSD$
# $Id: Makefile 1316 2009-12-08 05:56:43Z heliocentric $
#

PORTNAME=      xggi
PORTVERSION=   200912051535
CATEGORIES=    x11-servers
MASTER_SITES=  http://evoke.googlecode.com/files/

MAINTAINER=    a134qaed@gmail.com


LIB_DEPENDS=	ggi.2:${PORTSDIR}/graphics/libggi
LIB_DEPENDS+=	png.5:${PORTSDIR}/graphics/png
LIB_DEPENDS+=	freetype.9:${PORTSDIR}/print/freetype2

BUILD_DEPENDS=  ${LOCALBASE}/libdata/pkgconfig/fontconfig.pc:${PORTSDIR}/x11-fonts/fontconfig

USE_AUTOTOOLS=  autoconf:262:env libtool:22:env aclocal:110:env automake:110:env

USE_GNOME= 	pkgconfig

WRKSRC=		${WRKDIR}/xggi
FAKEROOT=	${WRKDIR}/fakeroot


do-build:
	${MKDIR} ${FAKEROOT}
	cd ${WRKSRC} && CFLAGS='-Dft_isdigit=isdigit ${CFLAGS}' CPPFLAGS="-I${LOCALBASE}/xggi/include -I${LOCALBASE}/include" LDFLAGS="-L${LOCALBASE}/xggi/lib -L${LOCALBASE}/lib" LOCALBASE="${LOCALBASE}" ./build-release.sh --with-ggi=${LOCALBASE} ${LOCALBASE}/xggi build

.include <bsd.port.mk>
