TEST_NAME="Logger Test"

EXEC_FILE_NAME="temp_exec"

SRC_TEST_CODE='
#include <iostream>
#include "Logger.hpp"

int main() {
	logger::Logger l;

	std::cerr << std::endl;

	l.with_field("name", "Sandro").debug("Hello again!");
	l.debug("Hello World!");

	std::cerr << std::endl;
}
'

clean () {
	rm -f $EXEC_FILE_NAME
}

compile () {
	echo "$SRC_TEST_CODE" |
	clang++ -std="c++98" -Wall -Wextra -Werror -o $EXEC_FILE_NAME -x c++ - *.cpp
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

./$EXEC_FILE_NAME

clean
echo
echo Test Pass ✅
