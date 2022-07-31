
all: master stringpiece_test;

master:
	g++ -std=c++11 -Wall -Wextra -g -rdynamic -I. -Ithird_party/jsoncpp-1.9.5 -o master \
			oak/master.cc oak/common/trivial.cc oak/logging/logging.cc oak/config.cc \
			third_party/jsoncpp-1.9.5/jsoncpp.cpp

stringpiece_test:
	g++ -std=c++11 -Wall -Wextra -g -rdynamic -I. -Ithird_party/gtest -O2 -o stringpiece_test \
			oak/common/trivial.cc oak/common/stringpiece_test.cc \
			third_party/gtest/gtest_main.cc third_party/gtest/gmock-gtest-all.cc \
			-lpthread

clean:
	-rm -f master stringpiece_test

.PHYONE: master stringpiece_test
