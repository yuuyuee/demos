
all:
	g++ -std=c++11 -Wall -Wextra -g -rdynamic -Iinclude -Isrc -o master \
			src/master.cc src/common/trivial.cc src/logging/logging.cc

stringpiece_test:
	g++ -std=c++11 -Wall -Wextra -g -rdynamic -Iinclude -Isrc -Ithird_party/gtest -o stringpiece_test \
			src/common/trivial.cc src/common/stringpiece_test.cc \
			third_party/gtest/gtest_main.cc third_party/gtest/gmock-gtest-all.cc \
			-lpthread

clean:
	-rm -f master stringpiece_test
