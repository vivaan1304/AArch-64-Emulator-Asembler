CC      ?= gcc
CFLAGS  ?= -std=c17 -g\
	-D_POSIX_SOURCE -D_DEFAULT_SOURCE\
	-Wall -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: all clean

all: assemble emulate

ASS_OBJS = include_assembler/utils/symbol_table.c include_assembler/utils/tokenization_io.c  \
include_assembler/instructions/data_processing.c include_assembler/instructions/stdi.c \
include_assembler/instructions/dp_helpers.c include_assembler/instructions/general_helpers.c\
include_assembler/instructions/branching.c include_assembler/instructions/assemble_line.c \
assemble.c

ASS_OUT = assemble

assemble: 
	$(CC) $(CFLAGS) $(ASS_OBJS) -o $(ASS_OUT)

EMU_OBJS = include_emulator/instructions/branch.c include_emulator/instructions/immediate.c \
include_emulator/instructions/load.c include_emulator/instructions/shifts.c \
include_emulator/utils/ioutils.c include_emulator/utils/utilities.c \
emulate.c

EMU_OUT = emulate
emulate:
	$(CC) $(CFLAGS) $(EMU_OBJS) -o $(EMU_OUT)

clean:
	rm -f $(wildcard *.o)
	rm -rf $(wildcard *.dSYM)
	rm -f assemble
	rm -f emulate
	