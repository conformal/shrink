# $OpenBSD: Makefile,v 1.3 2011/06/20 15:31:26 dhill Exp $

COMMENT=	compression wrapper library

DISTNAME=	shrink-0.5.2
EPOCH=		0
PKGNAME=	lib${DISTNAME}
CATEGORIES=    	archivers devel
SHARED_LIBS=	shrink	3.0

HOMEPAGE=	http://opensource.conformal.com/wiki/Shrink	
MASTER_SITES=	http://opensource.conformal.com/snapshots/shrink/

MAINTAINER=	Conformal Systems LLC <info@conformal.com>

WANTLIB=	c crypto lzma lzo2 ssl util z
LIB_DEPENDS=	archivers/lzo2 \
		archivers/xz

NO_TEST=	Yes

# ISC
PERMIT_PACKAGE_CDROM=	Yes

FAKE_FLAGS=	MANDIR=${PREFIX}/man/man

do-configure:
	printf "major=${LIBshrink_VERSION:R}\nminor=${LIBshrink_VERSION:E}\n" \
            > ${WRKSRC}/libshrink/shlib_version

.include <bsd.port.mk>
