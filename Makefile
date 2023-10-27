##
# Data structures and algorithms in C
#
# @author Brendan Watling
# @version 0.1

# compiler flags
DEBUG          := -ggdb -gdwarf-4
OPTIMISE       := -O2
WARNINGS       := -Wall -Wextra -Wno-variadic-macros -Wno-overlength-strings \
                  -pedantic
STCOPTS        := -DSTC_DISABLE_STD_MATH_H #-DSTC_ENABLE_DEBUG
CFLAGS         := $(DEBUG) $(OPTIMISE) $(WARNINGS) $(STCOPTS)
DFLAGS         := #-DDEBUG

# commands
CC             := clang
RM             := rm -f
COMPILE        := $(CC) $(CFLAGS) $(DFLAGS)

# directories
SRCDIR         := src
TESTDIR        := test
BINDIR         := bin

# files
MATH_EXE       := testmath
VEC_EXE        := testvec
ARGS_EXE       := testargs

COMMON_OBJ     := $(SRCDIR)/common.o
MATH_OBJ       := $(SRCDIR)/math.o
HASHMAP_OBJ    := $(SRCDIR)/collection/hashmap.o
LINKEDLIST_OBJ := $(SRCDIR)/collection/linkedlist.o
SLICE_OBJ      := $(SRCDIR)/fatp/slice.o
STRING_OBJ     := $(SRCDIR)/fatp/string.o
SV_OBJ         := $(SRCDIR)/fatp/string_view.o
VEC_OBJ        := $(SRCDIR)/fatp/vec.o
MEMBASE_OBJ    := $(SRCDIR)/memory/base.o
MEMLIBC_OBJ    := $(SRCDIR)/memory/libc.o
ARGS_OBJ       := $(SRCDIR)/util/args.o
UTF_OBJ        := $(SRCDIR)/util/utf.o

EXE            := $(MATH_EXE) $(VEC_EXE) $(ARGS_EXE)
OBJ            := $(COMMON_OBJ) $(MATH_OBJ) $(HASHMAP_OBJ) $(LINKEDLIST_OBJ) \
                  $(SLICE_OBJ) $(STRING_OBJ) $(SV_OBJ) $(VEC_OBJ) \
                  $(MEMBASE_OBJ) $(MEMLIBC_OBJ) $(UTF_OBJ) $(ARGS_OBJ)

### RULES ######################################################################

# executables

$(ARGS_EXE): $(TESTDIR)/$(ARGS_EXE).c $(ARGS_OBJ) | $(BINDIR)
	$(COMPILE) -o $(BINDIR)/$@ $^

$(MATH_EXE): $(TESTDIR)/$(MATH_EXE).c $(MATH_OBJ) | $(BINDIR)
	$(COMPILE) -o $(BINDIR)/$@ $^

$(VEC_EXE): $(TESTDIR)/$(VEC_EXE).c $(VEC_OBJ) | $(BINDIR)
	$(COMPILE) -o $(BINDIR)/$@ $^

# units

%.o: %.c
	$(COMPILE) -c -o $@ $<

# BINDIR

$(BINDIR):
	mkdir -p $(BINDIR)

### PHONY TARGETS ##############################################################

all: $(EXE)

allobj: $(OBJ)

clean: cleanobj cleanbin

cleanobj:
	$(RM) $(OBJ)

cleanbin:
	$(RM) $(addprefix $(BINDIR)/, $(EXE))

.PHONY: all allobj clean cleanobj cleanbin

# end
