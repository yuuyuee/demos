
all: rs;

rs:
	g++ -std=c++11 -Wall -Wextra -g -O0 -rdynamic -o $@ \
			-I. \
			-Ithird_party/jsoncpp-1.9.5 \
			-Ithird_party/asio-1.22.2/asio/include \
			oak/rs.cc	\
			oak/worker.cc \
			oak/common/system.cc \
			oak/common/fs.cc \
			oak/common/format.cc \
			oak/common/throw_delegate.cc \
			oak/common/debug.cc \
			oak/logging/logging.cc \
			oak/config.cc \
			oak/common/error_code.cc \
			third_party/jsoncpp-1.9.5/jsoncpp.cpp \
			-lpthread
	if test -e "dist/bin" -a -d "dist/bin"; then \
		cp rs dist/bin/; \
	fi;

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

codec_test:
	g++ -std=c++11 -Wall -Wextra -g -rdynamic -I. -Ithird_party/gtest -O2 -o$@ \
			oak/common/codec_test.cc \
			third_party/gtest/gtest_main.cc third_party/gtest/gmock-gtest-all.cc \
			-lpthread

clean:
	-rm -f rs stringpiece_test any_test circle_test buffer_test codec_test

.PHONY: rs stringpiece_test any_test circle_test buffer_test codec_test
