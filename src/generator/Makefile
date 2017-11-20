include ../../config.mk

.PHONY: all build install clean

all: build install

build:
	@$(CC) $(CFLAGS) generator.c -o generator
install:
	@cp generator $(DIR_OUT)/bin

clean:
	@rm generator
