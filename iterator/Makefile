TOPDIR	= $(CURDIR)
target	= $(subst .cc,,$(notdir $(wildcard *.cc)))

CC      = g++
CFLAGS  += -std=c++11 -Wall -Werror -g -c

define make-target
$1: $1.o
	$(CC) $(LDFLAGS) -o $$@ $$<

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

