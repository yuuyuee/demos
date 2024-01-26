# Author: Yue Yu (yuyue2200@hotmail.com)

all: main.cc a.cc b.cc
	g++ -std=c++11 -g -rdynamic -shared -fPIC -o libb.so b.cc
	g++ -std=c++11 -g -rdynamic -shared -fPIC -Wl,-rpath,'lib' -o liba.so a.cc -L. -lb
	g++ -std=c++11 -g -Wall -Wextra -o main main.cc
	mkdir -p lib && mv liba.so libb.so lib

clean:
	-rm -rf ./lib
	-rm -rf main
