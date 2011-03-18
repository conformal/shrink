# $shrink$

LOCALBASE?=/usr/local
BINDIR=${LOCALBASE}/bin
LIBDIR=${LOCALBASE}/lib

LIBSHRINKDIR=${.CURDIR}/../../libshrink
SHRINKDIR=${.CURDIR}/../../shrink

.PATH: ${SHRINKDIR}

PROG= shrink
NO_MAN=

.if exists(${.OBJDIR}/../libshrink)
LDADD+=		-L${.OBJDIR}/../libshrink
DPADD+=${.OBJDIR}/../libshrink/libshrink.a
.else
LDADD+=		-L${.CURDIR}/../libshrink
DPADD+=${.CURDIR}/../libshrink/libshrink.a
.endif

SRCS= shrink.c fmt_scaled.c arc4random_buf.c
COPT+= -O2
DEBUG+= -ggdb3
CFLAGS+= -Wall -Werror -I${LOCALBASE}/include
LDADD+= -lutil -L${LOCALBASE}/lib -lshrink -lssl

CFLAGS+= -I${LIBSHRINKDIR} -I${.CURDIR}/../include

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
