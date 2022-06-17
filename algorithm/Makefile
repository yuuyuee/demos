# Author: Yue Yu (yuyue2200@hotmail.com)

CXXOBJS += $(patsubst %.cc,%, $(notdir $(wildcard *.cc)))
CXXOBJS += $(patsubst %.cpp,%, $(notdir $(wildcard *.cpp)))
COBJS   += $(patsubst %.c,%, $(notdir $(wildcard *.c)))

CPPFLAGS += -D_GNU_SOURCE
CFLAGS   += $(CPPFLAGS) -std=c99 -Wall -Wextra -g -rdynamic
CXXFLAGS += $(CPPFLAGS) -std=c++11 -Wall -Wextra -g -rdynamic
LDFLAGS  += -lpthread

# quiet mode
Q = @

###########################################################

CC ?= gcc
CXX ?= g++
CC_SUFFIX = _c
CXX_SUFFIX = _cxx

%$(CC_SUFFIX).o: %.c
	$Q$(CC) $(CFLAGS) -c -o $@ $^

%$(CXX_SUFFIX).o: %.cc
	$Q$(CXX) $(CXXFLAGS) -c -o $@ $^

%$(CXX_SUFFIX).o: %.cpp
	$Q$(CXX) $(CXXFLAGS) -c -o $@ $^

define make_c_target
$1: $1$(CC_SUFFIX).o
	$Q$(CC) -o $$@ $$^ $(LDFLAGS)
	$@rm -f $$^

$1_clean:
	-$Qrm -f $1
endef

define make_cxx_target
$1: $1$(CXX_SUFFIX).o
	$Q$(CXX) -o $$@ $$^ $(LDFLAGS)
	$@rm -f $$^

$1_clean:
	-$Qrm -f $1
endef

all: $(foreach o, $(COBJS) $(CXXOBJS), $o);
clean: $(foreach o, $(COBJS) $(CXXOBJS), $o_clean);
$(foreach o, $(COBJS), $(eval $(call make_c_target,$o)))
$(foreach o, $(CXXOBJS), $(eval $(call make_cxx_target,$o)))
.PHYONE: all clean $(foreach o, $(COBJS) $(CXXOBJS), $o_clean $o)
