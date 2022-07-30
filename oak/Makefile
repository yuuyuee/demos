
all: master stringpiece_test;

master:
	g++ -std=c++11 -Wall -Wextra -g -rdynamic -I. -o master \
			oak/master.cc oak/common/trivial.cc oak/logging/logging.cc

stringpiece_test:
	g++ -std=c++11 -Wall -Wextra -g -rdynamic -I. -Ithird_party/gtest -o stringpiece_test \
			oak/common/trivial.cc oak/common/stringpiece_test.cc \
			third_party/gtest/gtest_main.cc third_party/gtest/gmock-gtest-all.cc \
			-lpthread

clean:
	-rm -f master stringpiece_test
