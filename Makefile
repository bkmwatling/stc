##
# Data structures and algorithms in C
#
# @author Brendan Watling
# @version 0.1

# compiler flags
DEBUG    := -ggdb -gdwarf-4
OPTIMISE := -O2
WARNINGS := -Wall -Wextra -Wno-variadic-macros -Wno-overlength-strings -pedantic
CFLAGS   := $(DEBUG) $(OPTIMISE) $(WARNINGS)
DFLAGS   := # -DDEBUG

# commands
CC       := clang
RM       := rm -f
COMPILE  := $(CC) $(CFLAGS) $(DFLAGS)

# directories
SRCDIR   := src
BINDIR   := bin
FATPDIR   := fatp

# files
VEC_EXE  := testvec
VEC_OBJS := $(FATPDIR)/vec.o

EXES     := $(VEC_EXE)
OBJS     := $(VEC_OBJS)

### RULES ######################################################################

# executables

$(VEC_EXE): $(SRCDIR)/$(VEC_EXE).c $(VEC_OBJS) | $(BINDIR)
	$(COMPILE) -o $(BINDIR)/$@ $^

# units

%.o: %.c
	$(COMPILE) -c -o $@ $<

# BINDIR

$(BINDIR):
	mkdir -p $(BINDIR)

### PHONY TARGETS ##############################################################

all: $(EXES)

clean: cleanobj cleanbin

cleanobj:
	$(RM) $(OBJS)

cleanbin:
	$(RM) $(addprefix $(BINDIR)/, $(EXES))

.PHONY: all clean cleanobj cleanbin

# end
