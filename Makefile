##===- Makefile --------------------------------------------*- Makefile -*-===##
#
#                     The LLVM Compiler Infrastructure
#
# This file is distributed under the University of Illinois Open Source
# License. See LICENSE.TXT for details.
#
##===----------------------------------------------------------------------===##

# If FLANG_LEVEL is not set, then we are the top-level Makefile. Otherwise, we
# are being included from a subdirectory makefile.

ifndef FLANG_LEVEL

IS_TOP_LEVEL := 1
FLANG_LEVEL := .
DIRS := include lib tools

PARALLEL_DIRS :=

endif

ifeq ($(MAKECMDGOALS),libs-only)
  DIRS := $(filter-out tools docs, $(DIRS))
  OPTIONAL_DIRS :=
endif

###
# Common Makefile code, shared by all Flang Makefiles.

# Set LLVM source root level.
LEVEL := $(FLANG_LEVEL)/../..

# Include LLVM common makefile.
include $(LEVEL)/Makefile.common

ifneq ($(ENABLE_DOCS),1)
  DIRS := $(filter-out docs, $(DIRS))
endif

# Set common Flang build flags.
CPP.Flags += -I$(PROJ_SRC_DIR)/$(FLANG_LEVEL)/include -I$(PROJ_OBJ_DIR)/$(FLANG_LEVEL)/include
ifdef FLANG_VENDOR
CPP.Flags += -DFLANG_VENDOR='"$(FLANG_VENDOR) "'
endif

# Disable -fstrict-aliasing. Darwin disables it by default (and LLVM doesn't
# work with it enabled with GCC), Flang/llvm-gc don't support it yet, and newer
# GCC's have false positive warnings with it on Linux (which prove a pain to
# fix). For example:
#
#   http://gcc.gnu.org/PR41874
#   http://gcc.gnu.org/PR41838
#
# We can revisit this when LLVM/Flang support it.
CXX.Flags += -fno-strict-aliasing

# Set up Clang's tblgen.
ifndef CLANG_TBLGEN
  ifeq ($(LLVM_CROSS_COMPILING),1)
    CLANG_TBLGEN := $(BuildLLVMToolDir)/clang-tblgen$(BUILD_EXEEXT)
  else
    CLANG_TBLGEN := $(LLVMToolDir)/clang-tblgen$(EXEEXT)
  endif
endif
ClangTableGen = $(CLANG_TBLGEN) $(TableGen.Flags)

###
# Flang Top Level specific stuff.

ifeq ($(IS_TOP_LEVEL),1)

ifneq ($(PROJ_SRC_ROOT),$(PROJ_OBJ_ROOT))
$(RecursiveTargets)::
	$(Verb) if [ ! -f test/Makefile ]; then \
	  $(MKDIR) test; \
	  $(CP) $(PROJ_SRC_DIR)/test/Makefile test/Makefile; \
	fi
endif

test::
	@ $(MAKE) -C test

report::
	@ $(MAKE) -C test report

clean::
	@ $(MAKE) -C test clean

libs-only: all

tags::
	$(Verb) etags `find . -type f -name '*.h' -or -name '*.cpp' | \
	  grep -v /lib/Headers | grep -v /test/`

cscope.files:
	find lib include -name '*.cpp' \
	             -or -name '*.def' \
	             -or -name '*.td' \
	             -or -name '*.h' > cscope.files

.PHONY: test report clean cscope.files

endif
