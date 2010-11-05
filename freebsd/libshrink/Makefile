# $shrink$

PREFIX?=/usr/local
BINDIR=${DESTDIR}${PREFIX}/bin
LIBDIR=${DESTDIR}${PREFIX}/lib

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
CFLAGS+=-I${LIBSHRINKDIR} -I/usr/local/include
LDADD+=-L${PREFIX}/lib

MAN=  shrink.3
HDRS= shrink.h

afterinstall:
	@cd ${.CURDIR}; for i in ${HDRS}; do \
	cmp -s ../../libshrink/$$i ${DESTDIR}${PREFIX}/include/$$i || \
	${INSTALL} ${INSTALL_COPY} -m 444 -o $(BINOWN) -g $(BINGRP) ../../libshrink/$$i ${DESTDIR}${PREFIX}/include; \
	echo ${INSTALL} ${INSTALL_COPY} -m 444 -o $(BINOWN) -g $(BINGRP) ../../libshrink/$$i ${DESTDIR}${PREFIX}/include;\
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
