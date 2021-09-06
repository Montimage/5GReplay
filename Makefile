CC     = gcc
CXX    = g++
RM     = rm -rf
MKDIR  = mkdir -p
CP     = cp
MV     = mv
LN     = ln -s

#name of executable file to generate
OUTPUT   = 5Greplay

#directory where probe will be installed on
ifndef MMT_BASE
  MMT_BASE             := /opt/mmt
  NEED_ROOT_PERMISSION := 1
else
  $(info INFO: Set default folder of MMT to $(MMT_BASE))
endif

INSTALL_DIR := $(MMT_BASE)/5greplay

# directory where MMT-DPI was installed
MMT_DPI_DIR := $(MMT_BASE)/dpi


#get git version abbrev
GIT_VERSION := $(shell git log --format="%h" -n 1)
VERSION     := 0.0.1

CACHE_LINESIZE := 64 #$(shell getconf LEVEL1_DCACHE_LINESIZE)

#always embeddes libconfuse
LIBS += -l:libconfuse.a

#set of library
LIBS     += -ldl -lpthread
#Jul. 19 2021: we currently use directly the function inside libmmt_tmobile.so

ifdef STATIC_LINK
  CFLAGS += -DSTATIC_LINK
  LIBS   += -l:libxml2.a -l:libicuuc.a -l:libz.a -l:liblzma.a -l:libicudata.a #xml2 and its dependencies
  LIBS   +=  -l:libmmt_core.a -l:libmmt_tcpip.so -l:libmmt_tmobile.a  -l:libsctp.a -l:libpcap.a
else
  LIBS   += -l:libmmt_core.so -l:libmmt_tmobile.so -l:libxml2.so -l:libsctp.so -l:libpcap.so
endif

CFLAGS   += -fPIC -Wall -DINSTALL_DIR=\"$(INSTALL_DIR)\" -DVERSION_NUMBER=\"$(VERSION)\" -DGIT_VERSION=\"$(GIT_VERSION)\" -DLEVEL1_DCACHE_LINESIZE=$(CACHE_LINESIZE) \
				-Wno-unused-variable -Wno-unused-function -Wuninitialized\
				-I/usr/include/libxml2/  -I$(MMT_DPI_DIR)/include

CLDFLAGS += -L$(MMT_DPI_DIR)/lib -L./plugins -L/usr/local/lib

#a specific flag for each .o file
CFLAGS += $(CFLAGS-$@)

#for debuging
ifdef DEBUG
  CFLAGS   += -g -DDEBUG_MODE -O0 -fstack-protector-all
else
  CFLAGS   += -O3
endif

ifdef VALGRIND
  CFLAGS += -DVALGRIND_MODE
endif

#Enable update_rules if this parameter is different 0 
ifndef UPDATE_RULES 
else
  ifneq "$(UPDATE_RULES)" "0"
  		CFLAGS += -DMODULE_ADD_OR_RM_RULES_RUNTIME
  endif
endif

#folders containing source files
SRCDIR := $(abspath ./src )

#list all .c files inside ./src and its sub-folders
LIB_SRCS  := $(shell find $(SRCDIR)/ -type f -name '*.c')
#exclude main.c
#LIB_SRCS  := $(filter-out $(SRCDIR)/main.c, $(LIB_SRCS))
LIB_OBJS  := $(patsubst %.c,%.o, $(LIB_SRCS))

ifndef VERBOSE
  QUIET := @
endif

#make starts with the first target (not targets whose names start with ‘.’). 
# This is called the default goal.
# https://www.gnu.org/software/make/manual/html_node/How-Make-Works.html
all: main


#this is useful when running the tools, such as, gen_dpi, compile_rule
# but libmmt_core, ... are not found by ldd
export LD_LIBRARY_PATH=$(MMT_DPI_DIR)/lib

# check if there exists the folder of MMT-DPI 
$(MMT_DPI_DIR):
	@echo "ERROR: Not found MMT-DPI at folder $(MMT_DPI_DIR).\n"
	@exit 1
	
%.o: %.c
	@echo "[COMPILE] $(notdir $@)"
	$(QUIET) $(CC) $(CFLAGS) $(CLDFLAGS) -c -o $@ $<
	
main:  $(MMT_DPI_DIR) $(LIB_OBJS) --refresh-plugin-engine
	@echo "[COMPILE] $@"
# When compiling using static link, we need to use g++ as DPI uses stdc++
ifdef STATIC_LINK
	$(QUIET) $(CXX) -std=c++11 -Wl,--export-dynamic -o $(OUTPUT) $(CLDFLAGS) $(LIB_OBJS)  $(RULE_OBJS) $(LIBS)
else
	$(QUIET) $(CC) -Wl,--export-dynamic -o $(OUTPUT) $(CLDFLAGS) $(LIB_OBJS)  $(RULE_OBJS) $(LIBS)
endif
	


RULE_XML := $(sort $(wildcard rules/*.xml))

# This target is to deal with the issue when user uses 
#   2 differrent values of INSTALL_DIR for "make" and "make install"
# Ex: make; sudo make install INSTALL_DIR=/tmp/mmt/security
#   - the first "make" will set in the codes MMT_SEC_PLUGINS_REPOSITORY_OPT to /opt/mmt/security/rules
#   - while the second "make install" will install to /tmp/mmt
# Thus we need to recompile the codes that use MMT_SEC_PLUGINS_REPOSITORY_OPT to update the new directory.
# The following target will remove the object files of the codes, thus it will trigger to recompile them.
# So, in the example above, the MMT_SEC_PLUGINS_REPOSITORY_OPT will be update to /tmp/mmt/security/rules.
	
--refresh-plugin-engine:
	$(QUIET) echo [RE-COMPILE] plugins_engine.o
	$(QUIET) $(CC) $(CFLAGS) $(CLDFLAGS) $(CFLAG-PLUGINS-ENGINE) -c -o $(SRCDIR)/engine/plugins_engine.o $(SRCDIR)/engine/plugins_engine.c


uninstall:
	$(QUIET) $(RM) $(INSTALL_DIR)
	
rules/%.so: main
	$(QUIET) ./$(OUTPUT) compile rules/$*.so rules/$*.xml
	
sample-rules: $(SAMPLE_RULES)

clean-rules:
	$(QUIET) $(RM) rules/*.so rules/*.o rules/*.c
clean: clean-rules
	$(QUIET) $(RM) $(MAIN_OBJS) $(LIB_OBJS) $(OUTPUT) test.* \
			$(RULE_OBJS) $(TMP_DIR)
	
clean-all: clean
	$(QUIET) $(RM) $(MMT_DPI_HEADER)
	