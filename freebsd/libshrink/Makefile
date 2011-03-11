# $shrink$

LOCALBASE?=/usr/local
BINDIR=${LOCALBASE}/bin
LIBDIR=${LOCALBASE}/lib

LIBSHRINKDIR=${.CURDIR}/../../libshrink

.include <${LIBSHRINKDIR}/shlib_version>
.PATH: ${LIBSHRINKDIR}
SHLIB_MAJOR=${major}

#WANTLINT=
LIB= shrink
SRCS= shrink.c
CFLAGS+= -fdiagnostics-show-option
CFLAGS+= -Wall -Werror
CFLAGS+= -ggdb3
CFLAGS+=-I${LIBSHRINKDIR} -I${LOCALBASE}include
LDADD+=-L${LOCALBASE}/lib

MAN=  shrink.3
MANDIR=${LOCALBASE}/man/cat

HDRS= shrink.h

afterinstall:
	@cd ${.CURDIR}; for i in ${HDRS}; do \
	cmp -s ../../libshrink/$$i ${LOCALBASE}/include/$$i || \
	${INSTALL} ${INSTALL_COPY} -m 444 -o $(BINOWN) -g $(BINGRP) ../../libshrink/$$i ${DESTDIR}${LOCALBASE}/include; \
	echo ${INSTALL} ${INSTALL_COPY} -m 444 -o $(BINOWN) -g $(BINGRP) ../../libshrink/$$i ${DESTDIR}${LOCALBASE}/include;\
	done

.include <bsd.own.mk>
.include <bsd.lib.mk>

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
