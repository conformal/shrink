SYSTEM != uname -s
.if exists(${.CURDIR}/../config/Makefile.$(SYSTEM:L))
.  include "${.CURDIR}/../config/Makefile.$(SYSTEM:L)"
.endif

LOCALBASE?=/usr/local
BINDIR?=${LOCALBASE}/bin
INCDIR?=${LOCALBASE}/include

PROG= shrink
NOMAN=
NO_MAN=

.if ${.CURDIR} == ${.OBJDIR}
LDADD+=	-L${.CURDIR}/../libshrink
.elif ${.CURDIR}/obj == ${.OBJDIR}
LDADD+=	-L${.CURDIR}/../libshrink/obj
.else
LDADD+=	-L${.OBJDIR}/../libshrink
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
LDADD+=-lz
.endif

.if defined(SUPPORT_LZO2)
CFLAGS += -DSUPPORT_LZO2
LDADD+=-llzo2
.endif

.if defined(SUPPORT_LZMA)
CFLAGS += -DSUPPORT_LZMA
LDADD+=-llzma
.endif

