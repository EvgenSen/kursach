include ../../config.mk

.PHONY: all build install clean

all: build install

build:
	@$(CC) $(CFLAGS) server.c -o server `pkg-config --cflags --libs gtk+-2.0`

install:
	@cp server $(DIR_OUT)/bin

clean:
	@rm server