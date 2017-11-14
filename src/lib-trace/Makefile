include ../../config.mk

.PHONY: all build install clean

all: build install

build:
	@$(CC) $(CFLAGS) -fPIC -c lib-trace.c
	@$(CC) $(CFLAGS) -shared -o libtrace.so lib-trace.o

install:
	@cp libtrace.so $(DIR_OUT)/lib
	@cp lib-trace.h $(DIR_OUT)/include

clean:
	@rm lib-trace.o
	@rm libtrace.so