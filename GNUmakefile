
SUBDIRS = libshrink shrink
TARGETS = clean obj install uninstall depend

all: $(SUBDIRS)

$(TARGETS):
	@for i in $(SUBDIRS); do $(MAKE) -C $$i/ $@; done

$(SUBDIRS): 
	$(MAKE) -C $@

.PHONY: all $(SUBDIRS) $(TARGETS)

