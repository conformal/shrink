# $shrink$

LOCALBASE?=/usr/local
BINDIR?=${LOCALBASE}/bin
LIBDIR?=${LOCALBASE}/lib

PROG= shrink
NOMAN=

.if exists(${.CURDIR}/../libshrink/obj)
LDADD+=		-L${.CURDIR}/../libshrink/obj
DPADD+=${.CURDIR}/../libshrink/obj/libshrink.a
.else
LDADD+=		-L${.CURDIR}/../libshrink
DPADD+=${.CURDIR}/../libshrink/libshrink.a
.endif

SRCS= shrink.c
COPT+= -O2
CFLAGS+= -Wall -Werror -ggdb3
LDADD+= -lutil -lssl -lcrypto -L${LOCALBASE}/lib -lshrink

CFLAGS+= -I${.CURDIR}/../libshrink -I.

.include <bsd.prog.mk>

# must be after Makefile.inc is included
.if defined(SUPPORT_LZW)
CFLAGS += -DSUPPORT_LZW
EXTRALIB+=-lz
.endif

.if defined(SUPPORT_LZO2)
CFLAGS += -DSUPPORT_LZO2
EXTRALIB+=-llzo2
.endif

.if defined(SUPPORT_LZMA)
CFLAGS += -DSUPPORT_LZMA
EXTRALIB+=-llzma
.endif

.if defined(LDSTATIC)
LDADD+= ${EXTRALIB}
.endif
