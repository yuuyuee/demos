TOPDIR	= $(CURDIR)
target	= $(subst .cpp,,$(notdir $(wildcard *.cpp)))

CPPFLAGS = -std=c++11 -Wall -Werror -g
LDFLAGS  = -std=c++11 -Wall -Werror

define make-target
$1: $1.o
	g++ $(LDFLAGS) -o $$@ $$<

$1-clean:
	rm $1 $1.o

.PHYONE: $1
endef

%.o: %.cpp
	g++ $(CPPFLAGS) -c -o $@ $^

$(foreach t, $(target), $(eval $(call make-target,$(t))))

clean: $(foreach t, $(target), $(t)-clean);
all: $(foreach t, $(target), $(t));
.PHYONE: all clean

