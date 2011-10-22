SUBDIRS = libshrink shrink
TARGETS = clean obj install uninstall depend

all: $(SUBDIRS)

$(TARGETS):
	@for d in $(SUBDIRS); do			\
		echo "===> $$d";			\
		$(MAKE) -C $$d/ $@ || exit $$?; 	\
	done

$(SUBDIRS):
	@echo "===> $@"
	$(MAKE) -C $@

.PHONY: all $(SUBDIRS) $(TARGETS)

