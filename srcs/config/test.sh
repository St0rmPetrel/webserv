TEST_NAME="Logger Test"

EXEC_FILE_NAME="temp_exec"
WANT_FILE_NAME="temp_want"
GOT_FILE_NAME="temp_got"
EXEC_FILE_NAME_2="temp_parser"

BASIC_CONF_REF="../../configs/basic.txt"
BASIC_WCM_CONF_REF="../../configs/basic_with_comments.txt"
EMPTY_CONF_REF="../../configs/empty.txt"
BASIC_CONF_RESULT="../../configs/basic_result.txt"
BASIC_WCM_CONF_RESULT="../../configs/basic_with_comments_result.txt"
EMPTY_CONF_RESULT="../../configs/empty_result.txt"

SRC_TEST_CODE='
#include <iostream>

#include "Config.hpp"
#include "../logger/Logger.hpp"

int main() {
	logger::Logger log;
	config::Config conf(log);

	std::cerr << std::endl;
}
'

WANT=''

clean () {
	rm -f $EXEC_FILE_NAME
	rm -f $WANT_FILE_NAME
	rm -f $GOT_FILE_NAME
	rm -f $EXEC_FILE_NAME_2
	# deleting previously created output txt files
	rm -f $BASIC_CONF_RESULT
	rm -f $BASIC_WCM_CONF_RESULT
	rm -f $EMPTY_CONF_RESULT
	chmod +r $PWD/../../configs/no_rights.conf
}

compile () {
	echo "$SRC_TEST_CODE" |
	clang++ -std="c++98" \
		-Wall -Wextra -Werror \
		-o $EXEC_FILE_NAME \
		-x c++ - Config.cpp ../utils/*.cpp ../server/*.cpp ../server/http/*.cpp ../logger/*.cpp ; \
		clang++ -std="c++98"  \
		  -Wall -Wextra -Werror \
      Config.cpp ../server/*.cpp ../server/http/*.cpp ../logger/*.cpp tests/parser_test.cpp \
	  ../utils/*.cpp \
      -o $EXEC_FILE_NAME_2 ; \
      chmod -r $PWD/../../configs/no_rights.conf
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
echo "$WANT" > $WANT_FILE_NAME

if ! diff -c $GOT_FILE_NAME $WANT_FILE_NAME
then
	test_fail
fi

./$EXEC_FILE_NAME_2

if [ "$?" -eq "1" ]
then
  test_fail
fi

if ! diff $BASIC_CONF_REF $BASIC_CONF_RESULT || ! diff $BASIC_WCM_CONF_REF $BASIC_WCM_CONF_RESULT || ! diff $EMPTY_CONF_REF $EMPTY_CONF_RESULT
then
  test_fail
fi

clean
echo
echo Test Pass ✅
