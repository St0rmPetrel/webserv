TEST_NAME="Logger Test"

EXEC_FILE_NAME="temp_exec"
WANT_FILE_NAME="temp_want"
GOT_FILE_NAME="temp_got"

SRC_TEST_CODE='
#include <iostream>

#include "../config/Config.hpp"
#include "../logger/Logger.hpp"
#include "Server.hpp"

int main() {
	logger::Logger log;
	config::Config conf(log);
	server::Server serv(log, conf.get_server());

	std::cerr << std::endl;
}
'

WANT=''

clean () {
	rm -f $EXEC_FILE_NAME
	rm -f $WANT_FILE_NAME
	rm -f $GOT_FILE_NAME
}

compile () {
	echo "$SRC_TEST_CODE" |
	clang++ -std="c++98" -Wall -Wextra -Werror -o $EXEC_FILE_NAME -x c++ - *.cpp ../logger/*.cpp ../config/*.cpp ./http/*.cpp ../utils/*.cpp
}

test_fail () {
	clean
	echo $TEST_NAME Fail 🔥
	exit 1
}

if ! compile
then
	echo Does not compile error
	test_fail
fi

./$EXEC_FILE_NAME 2> $GOT_FILE_NAME
# doesn't work properly on mac with flag -e
echo "$WANT" > $WANT_FILE_NAME

if ! diff -c $GOT_FILE_NAME $WANT_FILE_NAME
then
	test_fail
fi

clean
echo
echo Test Pass ✅
