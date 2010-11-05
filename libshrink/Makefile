# $shrink$

PREFIX?=/usr/local
BINDIR=${DESTDIR}${PREFIX}/bin
LIBDIR=${DESTDIR}${PREFIX}/lib

#WANTLINT=
LIB= shrink
SRCS= shrink.c
.if defined(${COMPILER_VERSION})  &&  ${COMPILER_VERSION:L} == "gcc4"
CFLAGS+= -fdiagnostics-show-option
.endif
CFLAGS+= -Wall -Werror
CFLAGS+= -ggdb3
CPPFLAGS+=-I${.CURDIR} -I/usr/local/include
LDADD+=-L${PREFIX}/lib

MAN=  shrink.3
HDRS= shrink.h

afterinstall:
	@cd ${.CURDIR}; for i in ${HDRS}; do \
	cmp -s $$i ${DESTDIR}${PREFIX}/include/$$i || \
	${INSTALL} ${INSTALL_COPY} -m 444 -o $(BINOWN) -g $(BINGRP) $$i ${DESTDIR}${PREFIX}/include; \
	echo ${INSTALL} ${INSTALL_COPY} -m 444 -o $(BINOWN) -g $(BINGRP) $$i ${DESTDIR}${PREFIX}/include;\
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
