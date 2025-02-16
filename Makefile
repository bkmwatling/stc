##
# Data structures and algorithms in C
#
# @author Brendan Watling
# @version 0.3

# commands
CC             := clang
RM             := rm -f
COMPILE         = $(CC) $(CFLAGS) $(DFLAGS)

# compiler flags
BUILD_TYPE     ?= Debug
ifeq ($(BUILD_TYPE), Debug)
    DEBUG      := -ggdb -gdwarf-4
    OPTIMISE   := -O0
else ifeq ($(BUILD_TYPE), RelWithDebInfo)
    DEBUG      := -ggdb -gdwarf-4
    OPTIMISE   := -O2
else ifeq ($(BUILD_TYPE), Release)
    DEBUG      :=
    OPTIMISE   := -O2
endif

WARN           := -Wall -Wextra -Werror -Wpedantic
ifeq ($(CC), gcc)
    FATP_WARN  += -Wno-strict-aliasing
else ifeq ($(CC), clang)
    WARN       += -Wno-gnu-statement-expression -Wno-gnu-auto-type
endif
EXTRA          += -std=c11
INCLUDE         = $(addprefix -I, $(INCLUDEDIR))
STCOPT         := -DSTC_DISABLE_STD_MATH_H #-DSTC_ENABLE_DEBUG
CFLAGS          = $(DEBUG) $(OPTIMISE) $(WARN) $(EXTRA) $(INCLUDE) $(STCOPT)
DFLAGS         := #-DDEBUG

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

COLLECTION_OBJ := $(HASHMAP_OBJ) $(HASHSET_OBJ) $(LINKEDLIST_OBJ)
FATP_OBJ       := $(STRING_OBJ) $(SV_OBJ) $(VEC_OBJ)
MEMORY_OBJ     := $(MEMBASE_OBJ) $(MEMLIBC_OBJ)
UTIL_OBJ       := $(ARGPARSER_OBJ) $(UTF_OBJ)

EXE            := $(MATH_EXE) $(VEC_EXE) $(ARGPARSER_EXE)
OBJ            := $(COMMON_OBJ) $(MATH_OBJ) $(COLLECTION_OBJ) $(FATP_OBJ) \
                  $(FATP_OBJ) $(MEMORY_OBJ) $(UTIL_OBJ)

### RULES ######################################################################

# executables

$(ARGPARSER_EXE): $(TESTDIR)/$(ARGPARSER_EXE).c $(ARGPARSER_OBJ) | $(BINDIR)
	$(COMPILE) -o $(BINDIR)/$@ $^

$(MATH_EXE): $(TESTDIR)/$(MATH_EXE).c $(MATH_OBJ) | $(BINDIR)
	$(COMPILE) -o $(BINDIR)/$@ $^

$(VEC_EXE): $(TESTDIR)/$(VEC_EXE).c $(VEC_OBJ) | $(BINDIR)
	$(COMPILE) -o $(BINDIR)/$@ $^

# units

$(FATP_OBJ): %.o: %.c
	$(COMPILE) $(FATP_WARN) -c -o $@ $<

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
