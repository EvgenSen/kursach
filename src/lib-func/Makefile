include ../../config.mk

.PHONY: all build install clean

all: build install

build:
	@$(CC) $(CFLAGS) -fPIC -c lib-func.c
	@$(CC) $(CFLAGS) -shared -o libfunc.so lib-func.o

install:
	@cp libfunc.so $(DIR_OUT)/lib
	@cp lib-func.h $(DIR_OUT)/include

clean:
	@rm lib-func.o
	@rm libfunc.so
