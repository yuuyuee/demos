TOPDIR	= $(CURDIR)
target	= $(subst .cc,,$(notdir $(wildcard *.cc)))
SUBDIR  = $(shell find $(TOPDIR) -maxdepth 1 -and -type d -not -name ".git")

CC      = g++
CFLAGS  += -std=c++11 -Wall -Werror -g -c
LDFLAGS += -lboost_system -lboost_filesystem

define make-target
$1: $1.o
	$(CC) -o $$@ $$< $(LDFLAGS)

$1-clean:
	rm -f $1 $1.o

.PHYONE: $1
endef

%.o: %.cc
	$(CC) $(CFLAGS) -o $@ $^

$(foreach t, $(target), $(eval $(call make-target,$(t))))

clean: $(foreach t, $(target), $(t)-clean);
all: $(foreach t, $(target), $(t));
.PHYONE: all clean $(foreach t, $(target), $(t)-clean $(t))

