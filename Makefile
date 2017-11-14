.PHONY: all build configure unconfig clean

NORMAL    = $$(tput sgr0)
RED       = $$(tput setaf 1)
GREEN     = $$(tput setaf 2)
WHITE     = $$(tput setaf 7)
BRIGHT    = $$(tput bold)

all: build

build: configure
	@printf "$(BRIGHT)$(GREEN)\n   Start $@ project \n\n$(NORMAL)"
	@mkdir -p $(PWD)/obj
	@cd obj && make all
	@printf "$(BRIGHT)$(GREEN)\n   Project build successfully \n\n$(NORMAL)"

configure: config.mk
	@mkdir -p $(PWD)/obj
	@cp -rdflu $(PWD)/src/* $(PWD)/obj
	@mkdir -p $(PWD)/obj/out
	@mkdir -p $(PWD)/obj/out/bin
	@mkdir -p $(PWD)/obj/out/lib
	@mkdir -p $(PWD)/obj/out/include
	@rm -f out
	@ln -s obj/out out

config.mk:
	@echo "creating config.mk..."
	@echo "# Generated automatically - do not edit!" > config.mk
	@echo "" >> config.mk
	@echo "export CC=gcc" >> config.mk
	@echo "" >> config.mk
	@echo "export DIR_PROJECT=$(PWD)" >> config.mk
	@echo "export DIR_OUT=$(PWD)/out" >> config.mk
	@echo "config.mk was created"

clean:
	@printf "$(BRIGHT)$(GREEN)\n   $@ project \n\n$(NORMAL)"
	@rm -rf obj
	@rm -rf out
	@rm -f config.mk
