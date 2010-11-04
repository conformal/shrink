# $shrink$

PROG= shrink
#MAN= shrink.8
NOMAN=1
BINDIR=${PREFIX}/sbin

.if exists(${.CURDIR}/../libshrink/obj)
LDFLAGS+=		-L${.CURDIR}/../libshrink/obj
DPADD+=${.CURDIR}/../libshrink/obj/libshrink.a
.else
LDFLAGS+=		-L${.CURDIR}/../libshrink
DPADD+=${.CURDIR}/../libshrink/libshrink.a
.endif

SRCS= shrink.c
COPT+= -O2
DEBUG+= -ggdb3
CFLAGS+= -Wall -Werror
LDFLAGS+= -L${LIBDIR} -L/usr/local/lib -l shrink -lutil -llzo2 -lz

CFLAGS+= -I${.CURDIR}/../libshrink -I.

.include <bsd.prog.mk>
