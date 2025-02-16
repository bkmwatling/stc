##
# Data structures and algorithms in C
#
# @author Brendan Watling
# @version 0.2

# compiler flags
DEBUG          := -ggdb -gdwarf-4
OPTIMISE       := -O2
WARNING        := -Wall -Wextra -Werror -Wpedantic \
                  -Wno-gnu-statement-expression -Wno-gnu-auto-type
EXTRA          := -std=c11
INCLUDE         = $(addprefix -I, $(INCLUDEDIR))
STCOPT         := -DSTC_DISABLE_STD_MATH_H #-DSTC_ENABLE_DEBUG
CFLAGS          = $(DEBUG) $(OPTIMISE) $(WARNING) $(EXTRA) $(INCLUDE) $(STCOPT)
DFLAGS         := #-DDEBUG

# commands
CC             := clang
RM             := rm -f
COMPILE         = $(CC) $(CFLAGS) $(DFLAGS)

# directories
SRCDIR         := src
TESTDIR        := test
BINDIR         := bin
INCLUDEDIR     := ./include

# files
MATH_EXE       := testmath
VEC_EXE        := testvec
ARGPARSER_EXE  := testargparser

COMMON_OBJ     := $(SRCDIR)/common.o
MATH_OBJ       := $(SRCDIR)/math.o
HASHMAP_OBJ    := #$(SRCDIR)/collection/hashmap.o
HASHSET_OBJ    := #$(SRCDIR)/collection/hashset.o
LINKEDLIST_OBJ := $(SRCDIR)/collection/linkedlist.o
STRING_OBJ     := $(SRCDIR)/fatp/string.o
SV_OBJ         := $(SRCDIR)/fatp/str_view.o
VEC_OBJ        := $(SRCDIR)/fatp/vec.o
MEMBASE_OBJ    := $(SRCDIR)/memory/base.o
MEMLIBC_OBJ    := $(SRCDIR)/memory/libc.o
ARGPARSER_OBJ  := $(SRCDIR)/util/argparser.o
UTF_OBJ        := $(SRCDIR)/util/utf.o

EXE            := $(MATH_EXE) $(VEC_EXE) $(ARGPARSER_EXE)
OBJ            := $(COMMON_OBJ) $(MATH_OBJ) $(HASHMAP_OBJ) $(HASHSET_OBJ) \
                  $(LINKEDLIST_OBJ) $(STRING_OBJ) $(SV_OBJ) $(VEC_OBJ) \
                  $(MEMBASE_OBJ) $(MEMLIBC_OBJ) $(ARGPARSER_OBJ) $(UTF_OBJ)

### RULES ######################################################################

# executables

$(ARGPARSER_EXE): $(TESTDIR)/$(ARGPARSER_EXE).c $(ARGPARSER_OBJ) | $(BINDIR)
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
