##
# Vector library
#
# @author Brendan Watling
# @file main.c
# @version 0.1

# compiler flags
DEBUG    := -ggdb
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
VECDIR   := vec

# files
VEC_EXE  := testvec
VEC_OBJS := $(patsubst %.c, %.o, $(wildcard $(VECDIR)/*.c))

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
