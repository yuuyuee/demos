# Author: Yue Yu (yuyue2200@hotmail.com)

all: main.cc share_lib_a.cc share_lib_b.cc static_lib_c.cc
	g++ -std=c++11 -g -rdynamic -fPIC -c -o static_lib_c.o static_lib_c.cc
	ar rcs libx.a static_lib_c.o
	g++ -std=c++11 -g -fvisibility=hidden -shared -fPIC -o libb.so share_lib_b.cc libx.a
	g++ -std=c++11 -g -fvisibility=hidden -shared -fPIC -o liba.so share_lib_a.cc libx.a
	g++ -std=c++11 -g -Wall -Wextra -Wl,-rpath,./ -o main main.cc -L. -la -lb

clean:
	-rm -rf *.o *.so *.a main
