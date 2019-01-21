#include <stdbool.h>
#include <assert.h>
#include "common.h"

bool issrq(enum test_type_t test_type) {
	return test_type == TEST_TYPE_SRQTEST;
}
bool chk_test_type_t(enum test_type_t tsttype) {
	assert(tsttype >= TEST_TYPE_MIN && tsttype <= TEST_TYPE_MAX);
}
