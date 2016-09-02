# makefile for generic c++ project
# generated with `makeproject` on Thu Sep  1 20:58:12 CEST 2016
# Author: Dan Guest <dguest@cern.ch>

# _______________________________________________________________
# Basic Setup

# --- set dirs
BUILD        := build
SRC          := src
INC          := include
DICT         := dict
OUTPUT       := bin
LIB          := lib

#  set search path
vpath %.cxx  $(SRC)
vpath %.hh   $(INC)
vpath %.h    $(INC)
vpath %Dict.h $(DICT)
vpath %Dict.cxx $(DICT)

# --- set compiler and flags (roll c options and include paths together)
CXX          ?= g++
CXXFLAGS     := -O2 -Wall -fPIC -I$(INC) -I/Users/dantrim/boost_1_60_0 -g -std=c++11
LIBS         := # blank, more will be added below
LIBS = -L/usr/local/opt/boost/lib -lboost_filesystem -lboost_system
LDFLAGS      := # blank, more will be added below

# ---- define objects from files in the SRC directory
GEN_OBJ_SRC   := $(wildcard $(SRC)/*.cxx)
GEN_OBJ       := $(notdir $(GEN_OBJ_SRC:%.cxx=%.o))

# this list may be manipulated in other segments further down
GEN_OBJ_PATHS := $(GEN_OBJ:%=$(BUILD)/%)

# --- all top level (added further down)
ALL_TOP_LEVEL := # blank, more will be added below

# _______________________________________________________________
# Add Top Level Objects

# --- stuff used for the c++ executable
# everything with this prefix will be built as an executable
EXE_PREFIX   := map-

ALL_EXE_SRC   := $(wildcard $(SRC)/$(EXE_PREFIX)*.cxx)
ALL_EXE       := $(notdir $(ALL_EXE_SRC:%.cxx=%))
ALL_EXE_PATHS := $(ALL_EXE:%=$(OUTPUT)/%)

# filter out the general objects
GEN_OBJ_PATHS := $(filter-out $(BUILD)/$(EXE_PREFIX)%.o,$(GEN_OBJ_PATHS))

# add to all top level
ALL_TOP_LEVEL += $(ALL_EXE_PATHS)

# --- first call here
all: $(ALL_TOP_LEVEL)

# _______________________________________________________________
# Add Build Rules

# build exe
$(OUTPUT)/$(EXE_PREFIX)%: $(GEN_OBJ_PATHS) $(BUILD)/$(EXE_PREFIX)%.o
	@mkdir -p $(OUTPUT)
	@echo "linking $^ --> $@"
	@$(CXX) -o $@ $^ $(LIBS) $(LDFLAGS)

# compile rule
$(BUILD)/%.o: %.cxx
	@echo compiling $<
	@mkdir -p $(BUILD)
	@$(CXX) -c $(CXXFLAGS) $< -o $@

# use auto dependency generation
ALLOBJ       := $(GEN_OBJ)
DEP          := $(BUILD)

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(MAKECMDGOALS),rmdep)
include  $(ALLOBJ:%.o=$(DEP)/%.d)
endif
endif

DEPTARGSTR = -MT $(BUILD)/$*.o -MT $(DEP)/$*.d
$(DEP)/%.d: %.cxx
	@echo making dependencies for $<
	@mkdir -p $(DEP)
	@$(CXX) -MM -MP $(DEPTARGSTR) $(CXXFLAGS) $(PY_FLAGS) $< -o $@

# clean
.PHONY : clean rmdep all
CLEANLIST     = *~ *.o *.o~ *.d core
clean:
	rm -fr $(CLEANLIST) $(CLEANLIST:%=$(BUILD)/%) $(CLEANLIST:%=$(DEP)/%)
	rm -fr $(BUILD) $(DICT) $(OUTPUT) #$(ROOT_DICT_TMP).*

rmdep:
	rm -f $(DEP)/*.d
