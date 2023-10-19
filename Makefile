##
# Data structures and algorithms in C
#
# @author Brendan Watling
# @version 0.1

# compiler flags
DEBUG           := -ggdb -gdwarf-4
OPTIMISE        := -O2
WARNINGS        := -Wall -Wextra -Wno-variadic-macros -Wno-overlength-strings \
				   -pedantic
STCOPTS         := -DSTC_DISABLE_STD_MATH_H #-DSTC_ENABLE_DEBUG
CFLAGS          := $(DEBUG) $(OPTIMISE) $(WARNINGS) $(STCOPTS)
DFLAGS          := #-DDEBUG

# commands
CC              := clang
RM              := rm -f
COMPILE         := $(CC) $(CFLAGS) $(DFLAGS)

# directories
SRCDIR          := src
TESTDIR         := test
BINDIR          := bin

# files
MATH_EXE        := testmath
VEC_EXE         := testvec

COMMON_OBJS     := $(SRCDIR)/common.o
MATH_OBJS       := $(SRCDIR)/math.o
HASHMAP_OBJS    := $(SRCDIR)/collection/hashmap.o
LINKEDLIST_OBJS := $(SRCDIR)/collection/linkedlist.o
SLICE_OBJS      := $(SRCDIR)/fatp/slice.o
STRING_OBJS     := $(SRCDIR)/fatp/string.o
SV_OBJS         := $(SRCDIR)/fatp/string_view.o
VEC_OBJS        := $(SRCDIR)/fatp/vec.o
MEMBASE_OBJS    := $(SRCDIR)/memory/base.o
MEMLIBC_OBJS    := $(SRCDIR)/memory/libc.o
ARGS_OBJS       := $(SRCDIR)/util/args.o
UTF_OBJS        := $(SRCDIR)/util/utf.o

EXES            := $(MATH_EXE) $(VEC_EXE)
OBJS            := $(COMMON_OBJS) $(MATH_OBJS) $(HASHMAP_OBJS) \
				   $(LINKEDLIST_OBJS) $(SLICE_OBJS) $(STRING_OBJS) $(SV_OBJS) \
				   $(VEC_OBJS) $(MEMBASE_OBJS) $(MEMLIBC_OBJS) $(UTF_OBJS) \
				   $(ARGS_OBJS)

### RULES ######################################################################

# executables

$(MATH_EXE): $(TESTDIR)/$(MATH_EXE).c $(MATH_OBJS) | $(BINDIR)
	$(COMPILE) -o $(BINDIR)/$@ $^

$(VEC_EXE): $(TESTDIR)/$(VEC_EXE).c $(VEC_OBJS) | $(BINDIR)
	$(COMPILE) -o $(BINDIR)/$@ $^

# units

%.o: %.c
	$(COMPILE) -c -o $@ $<

# BINDIR

$(BINDIR):
	mkdir -p $(BINDIR)

### PHONY TARGETS ##############################################################

all: $(EXES)

allobj: $(OBJS)

clean: cleanobj cleanbin

cleanobj:
	$(RM) $(OBJS)

cleanbin:
	$(RM) $(addprefix $(BINDIR)/, $(EXES))

.PHONY: all allobj clean cleanobj cleanbin

# end
