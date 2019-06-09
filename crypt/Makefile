
utils_test: utils.cc
	g++ -std=c++0x -DUTILS_TEST -Wall -Werror -I. -g -o $@ $^ -lcrypt -lz

maclib_test: utils.cc maclib.cc
	g++ -std=c++0x -DMACLIB_TEST -Wall -Werror -I. -g -o $@ $^ -lcrypt -lz

clean:
	[ -e "utils_test" ] && rm "utils_test"
	[ -e "maclib_test" ] && rm "maclib_test"
