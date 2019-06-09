TOPDIR	= $(CURDIR)
target	= $(subst .cc,,$(notdir $(wildcard *.cc)))
subdir  := $(notdir $(shell find . -maxdepth 1 -type d))
<<<<<<< HEAD
unexpect= . .git docs .settings gcc
subdir  := $(filter-out $(unexpect),$(subdir))

CC      = g++
CFLAGS  += -std=c++11 -Wall -Werror -g -c -D_GNU_SOURCE
LDFLAGS += -lpthread
#libcurl.a -lcrypto -lcrypt -ldl -lz -lssl

define make-target
$1: $1.o
	$(CC) -rdynamic -o $$@ $$< $(LDFLAGS)
=======
unexpect= . .git docs
subdir  := $(filter-out $(unexpect),$(subdir))

CC      = g++
CFLAGS  += -std=c++11 -Wall -Werror -g -c
LDFLAGS +=

define make-target
$1: $1.o
	$(CC) -o $$@ $$< $(LDFLAGS)
>>>>>>> 41b0cdc026df773ac91fd7a50f6a4205cb0c7772
$1-clean:
	rm -f $1 $1.o
.PHYONE: $1
endef

all: $(foreach t, $(target), $(t))
	$(foreach d, $(subdir), make -C $(d);)
clean: $(foreach t, $(target), $(t)-clean)
	$(foreach d, $(subdir), make -C $(d) clean;)
$(foreach t, $(target), $(eval $(call make-target,$(t))))
%.o: %.cc
	$(CC) $(CFLAGS) -o $@ $^
.PHYONE: all clean $(foreach t, $(target), $(t)-clean $(t))
<<<<<<< HEAD
=======

>>>>>>> 41b0cdc026df773ac91fd7a50f6a4205cb0c7772
