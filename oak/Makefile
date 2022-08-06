
all: master stringpiece_test any_test;

master:
	g++ -std=c++11 -Wall -Wextra -g -O0 -rdynamic -I. -Ithird_party/jsoncpp-1.9.5 -o $@ \
			oak/master.cc oak/common/trivial.cc oak/common/fs.cc oak/common/format.cc \
			oak/common/throw_delegate.cc oak/logging/logging.cc oak/config.cc \
			third_party/jsoncpp-1.9.5/jsoncpp.cpp

worker:
	g++ -std=c++11 -Wall -Wextra -g -O0 -rdynamic -I. -Ithird_party/jsoncpp-1.9.5 -o $@ \
			oak/worker.cc oak/common/trivial.cc oak/common/fs.cc oak/logging/logging.cc oak/config.cc \
			third_party/jsoncpp-1.9.5/jsoncpp.cpp

stringpiece_test:
	g++ -std=c++11 -Wall -Wextra -g -rdynamic -I. -Ithird_party/gtest -O2 -o $@ \
			oak/common/stringpiece_test.cc \
			third_party/gtest/gtest_main.cc third_party/gtest/gmock-gtest-all.cc \
			-lpthread

any_test:
	g++ -std=c++11 -Wall -Wextra -g -rdynamic -I. -Ithird_party/gtest -O2 -o $@ \
			oak/common/any_test.cc \
			third_party/gtest/gtest_main.cc third_party/gtest/gmock-gtest-all.cc \
			-lpthread

circle_test:
	g++ -std=c++11 -Wall -Wextra -g -rdynamic -I. -Ithird_party/gtest -O2 -o$@ \
			oak/common/circle_test.cc \
			third_party/gtest/gtest_main.cc third_party/gtest/gmock-gtest-all.cc \
			-lpthread

buffer_test:
	g++ -std=c++11 -Wall -Wextra -g -rdynamic -I. -Ithird_party/gtest -O2 -o$@ \
			oak/addons/buffer_test.cc \
			third_party/gtest/gtest_main.cc third_party/gtest/gmock-gtest-all.cc \
			-lpthread

clean:
	-rm -f master worker stringpiece_test any_test circle_test buffer_test

.PHYONE: master worker stringpiece_test any_test circle_test buffer_test
