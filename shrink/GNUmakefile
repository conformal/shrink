# Attempt to include platform specific makefile.
# OSNAME may be passed in.
OSNAME ?= $(shell uname -s)
OSNAME := $(shell echo $(OSNAME) | tr A-Z a-z)
-include ../config/Makefile.$(OSNAME)

# Default paths.
LOCALBASE ?= /usr/local
BINDIR ?= ${LOCALBASE}/bin
LIBDIR ?= ${LOCALBASE}/lib
INCDIR ?= ${LOCALBASE}/include

# Use obj directory if it exists.
OBJPREFIX ?= obj/
ifeq "$(wildcard $(OBJPREFIX))" ""
	OBJPREFIX =
endif

# Compression algorithm support.
-include ../Makefile.inc
# must be after Makefile.inc is included
ifdef SUPPORT_LZW
CPPFLAGS += -DSUPPORT_LZW
LDADD+=-lz
endif

ifdef SUPPORT_LZO2
CPPFLAGS += -DSUPPORT_LZO2
LDADD+=-llzo2
endif

ifdef SUPPORT_LZMA
CPPFLAGS += -DSUPPORT_LZMA
LDADD+=-llzma
endif

# System utils.
CC ?= gcc
INSTALL ?= install
MKDIR ?= mkdir
RM ?= rm -f

# Compiler and linker flags.
CPPFLAGS += -DNEED_LIBCLENS
INCFLAGS += -I ../libshrink -I $(INCDIR)/clens -I $(LOCALBASE)/ssl/include
WARNFLAGS ?= -Wall -Werror
DEBUG += -g
CFLAGS += $(INCFLAGS) $(WARNFLAGS) $(DEBUG)
LDLIBS += -L../libshrink/obj -L../libshrink -lshrink -lclens
LDLIBS += ${LIB.LINKSTATIC} -lssl -lcrypto ${LIB.LINKDYNAMIC} -ldl

BIN.NAME = shrink
BIN.SRCS = shrink.c
BIN.OBJS = $(addprefix $(OBJPREFIX), $(BIN.SRCS:.c=.o))
BIN.DEPS = $(addsuffix .depend, $(BIN.OBJS))
BIN.LDFLAGS = $(LDFLAGS.EXTRA) $(LDFLAGS)
BIN.LDLIBS = $(LDLIBS) $(LDADD)

all: $(OBJPREFIX)$(BIN.NAME)

obj:
	-$(MKDIR) obj

$(OBJPREFIX)$(BIN.NAME): $(BIN.OBJS)
	$(CC) $(BIN.LDFLAGS) -o $@ $^ $(BIN.LDLIBS)

$(OBJPREFIX)%.o: %.c
	@echo "Generating $@.depend"
	@$(CC) $(INCFLAGS) -MM $(CPPFLAGS) $< | \
	sed 's,$*\.o[ :]*,$@ $@.depend : ,g' >> $@.depend
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ -c $<

depend:
	@echo "Dependencies are automatically generated.  This target is not necessary."

install:

uninstall:

clean:
	$(RM) $(BIN.OBJS)
	$(RM) $(OBJPREFIX)$(BIN.NAME)
	$(RM) $(BIN.DEPS)

-include $(BIN.DEPS)

.PHONY: clean depend install uninstall

