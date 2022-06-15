OBJS = $(subst .cc,, $(notdir $(wildcard *.cc)))
OBJS += $(subst .cpp,, $(notdir $(wildcard *.cpp)))
IGNORES = . .git docs .settings .vscode $(notdir $(CURDIR))
SUBDIR = $(filter-out $(IGNORES), $(notdir $(shell find $(CURDIR) -maxdepth 1 -type d)))

CC ?= gcc
CXX ?= g++

CPPFLAGS += -D_GNU_SOURCE
CFLAGS += $(CPPFLAGS) -std=c99 -Wall -Wextra -g -rdynamic
CXXFLAGS += $(CPPFLAGS) -std=c++11 -Wall -Wextra -g -rdynamic
LDFLAGS += -lpthread -lz -lcurl -lcrypt

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $^

define MAKE_TARGET
$1: $1.o
	$(CXX) -o $$@ $$^ $(LDFLAGS)

$1_clean:
	-rm -f $1 $1.o
endef

all: $(foreach o, $(OBJS), $o)
	$(foreach d, $(SUBDIR), \
		test ! -e $d/Makefile && cp Makefile $d/; \
		make -s -C $d;)

clean: $(foreach o, $(OBJS), $o_clean)
	$(foreach d, $(SUBDIR), \
		test ! -e $d/Makefile && cp Makefile $d/; \
		make -s -C $d clean;)

$(foreach o, $(OBJS), $(eval $(call MAKE_TARGET,$o)))

.PHYONE: all clean $(foreach o, $(OBJS), $o_clean $o)
