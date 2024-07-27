CC      ?= gcc
CFLAGS  ?= -std=c17 -g\
	-D_POSIX_SOURCE -D_DEFAULT_SOURCE\
	-Wall -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: all clean

all: extension

EXT_OBJS = utilities/defs.c utilities/utils.c utilities/gamePlay.c \
 utilities/shop.c unfair_casino.c

EXT_OUT = unfair_casino
extension:
	$(CC) $(CFLAGS) $(EXT_OBJS) -o $(EXT_OUT)

clean:
	rm -f $(wildcard *.o)
	rm -rf $(wildcard *.dSYM)
	rm -f assemble
	rm -f emulate
	