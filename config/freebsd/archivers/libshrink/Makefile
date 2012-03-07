# New ports collection makefile for:    clens
# Date created:         March 5, 2012
# Whom:                 Dale Rahn <drahn@conformal.com>
#
# $FreeBSD$
#

PORTNAME=       libshrink
PORTVERSION=    0.3.0
CATEGORIES=     archivers devel
MASTER_SITES=   https://opensource.conformal.com/snapshots/shrink/ \
		LOCAL/bf
DISTNAME=       shrink-${PORTVERSION}

MAINTAINER=     drahn@conformal.com
COMMENT=        Shrink library provides a API to several compression algorithms

BUILD_DEPENDS=  ${LOCALBASE}/lib/libclens.a:${PORTSDIR}/devel/clens
LIB_DEPENDS=	lzo2.2:${PORTSDIR}/archivers/lzo2

LICENSE=        ISCL
LICENSE_COMB=   single

.include <bsd.port.mk>
