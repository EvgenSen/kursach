include ../config.mk
include subprojects

NORMAL    = $$(tput sgr0)
GREEN     = $$(tput setaf 2)
BRIGHT    = $$(tput bold)

.PHONY: all build install clean $(targets) 

all: $(targets)

$(targets):
	@printf "$(BRIGHT)$(GREEN)\n   subproject: make $@\n\n$(NORMAL)"
	@make -C $@ -f Makefile build install

build install clean:
	@for i in $(targets); \
	do \
		printf "$(BRIGHT)$(GREEN)\n   $$i: make $@\n\n$(NORMAL)" ; \
		make -C $$i -f Makefile $@; \
	done