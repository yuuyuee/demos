TOPDIR	= $(CURDIR)
target	= $(subst .cc,,$(notdir $(wildcard *.cc)))
subdir  := $(notdir $(shell find . -maxdepth 1 -type d))
unexpect= . .git docs
subdir  := $(filter-out $(unexpect),$(subdir))

CC      = g++
CFLAGS  += -std=c++11 -Wall -Werror -g -c -DBOOST_ALL_DYN_LINK
LDFLAGS += -lboost_log \
		   -lboost_log_setup \
		   -lboost_date_time \
		   -lboost_filesystem \
		   -lboost_system \
		   -lboost_thread \
		   -lpthread

define make-target
$1: $1.o
	$(CC) -o $$@ $$< $(LDFLAGS)
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

