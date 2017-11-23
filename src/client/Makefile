include ../../config.mk

.PHONY: all build install clean

all: build install

build:
	@$(CC) $(CFLAGS) client.c -o client `pkg-config --cflags gtk+-3.0` `pkg-config --libs gtk+-3.0` \
	-ltrace -lfunc -L $(DIR_OUT)/lib -I $(DIR_OUT)/include

install:
	@cp client $(DIR_OUT)/bin

clean:
	@rm client
