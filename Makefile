client := client
server := server
targets := $(client) $(server)

SUBDIRS = . client/bin server/bin 
SUBDIRSCLEAN=$(addsuffix clean,$(SUBDIRS))

all: $(targets)

$(targets):
	$(MAKE) --no-print-directory --directory=$@

clean: $(SUBDIRSCLEAN)

clean_curdir:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c

%clean: %
	$(MAKE) --no-print-directory -C $< -f $(PWD)/Makefile clean_curdir

disclean: $(SUBDIRSCLEAN)
	rm -rf client.out server.out

.PHONY: all $(targets)