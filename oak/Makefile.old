
all: rs;

rs:
	g++ -std=c++11 -Wall -Wextra -faligned-new -g -O0 -rdynamic -o $@ \
			-I. \
			-Ithird_party/jsoncpp-1.9.5 \
			-Ithird_party/asio-1.22.2/asio/include \
			oak/rs.cc	\
			oak/worker.cc \
			oak/addons/module.cc \
			oak/addons/event_handle.cc \
			oak/addons/source_handle.cc \
			oak/addons/parser_handle.cc \
			oak/addons/sink_handle.cc \
			oak/addons/buffer_internal.cc \
			oak/event/kafka_event_module.cc \
			oak/common/system.cc \
			oak/common/fs.cc \
			oak/common/format.cc \
			oak/common/throw_delegate.cc \
			oak/common/debug.cc \
			oak/logging/logging.cc \
			oak/config.cc \
			oak/common/error_code.cc \
			third_party/jsoncpp-1.9.5/jsoncpp.cpp \
			-lpthread -ldl
	if test -e "dist/bin" -a -d "dist/bin"; then \
		cp rs dist/bin/; \
	fi;

stringpiece_test:
	g++ -std=c++11 -Wall -Wextra -g -rdynamic -I. -Ithird_party/gtest -O2 -o $@ \
			oak/common/stringpiece_test.cc \
			third_party/gtest/gtest_main.cc third_party/gtest/gmock-gtest-all.cc \
			-lpthread

channel_test:
	g++ -std=c++11 -Wall -Wextra -g -rdynamic -I. -Ithird_party/gtest -O2 -o $@ \
			oak/common/channel_test.cc \
			third_party/gtest/gtest_main.cc third_party/gtest/gmock-gtest-all.cc \
			-lpthread

any_test:
	g++ -std=c++11 -Wall -Wextra -g -rdynamic -I. -Ithird_party/gtest -O0 -o $@ \
			oak/common/any_test.cc \
			third_party/gtest/gtest_main.cc third_party/gtest/gmock-gtest-all.cc \
			-lpthread

ring_test:
	g++ -std=c++11 -Wall -Wextra -g -rdynamic -I. -Ithird_party/gtest -O2 -o$@ \
			oak/common/ring_test.cc \
			third_party/gtest/gtest_main.cc third_party/gtest/gmock-gtest-all.cc \
			-lpthread

buffer_test:
	g++ -std=c++11 -Wall -Wextra -g -rdynamic -I. -Ithird_party/gtest -O2 -o$@ \
			oak/addons/buffer_internal.cc \
			oak/addons/buffer_test.cc \
			third_party/gtest/gtest_main.cc third_party/gtest/gmock-gtest-all.cc \
			-lpthread

codec_test:
	g++ -std=c++11 -Wall -Wextra -g -rdynamic -I. -Ithird_party/gtest -O2 -o$@ \
			oak/common/codec_test.cc \
			third_party/gtest/gtest_main.cc third_party/gtest/gmock-gtest-all.cc \
			-lpthread

kafka_example:
	g++ -std=c++11 -Wall -Wextra -g -rdynamic -I. -Ithird_party/gtest -O2 -o$@ \
			oak/common/kafka.cc \
			oak/common/kafka_example.cc \
			oak/common/format.cc \
			oak/common/throw_delegate.cc \
			oak/addons/buffer_internal.cc \
			oak/logging/logging.cc \
			-lpthread -lrdkafka -lrdkafka++

clean:
	-rm -f rs stringpiece_test any_test circle_test buffer_test codec_test channel_test \
				kafka_example

.PHONY: rs stringpiece_test any_test circle_test buffer_test codec_test channel_test \
				kafka_example
