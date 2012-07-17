#ifndef TEST_H
#define TEST_H

#include "dbg.h"

static int SUCCESS_COUNT = 0;
static int FAIL_COUNT = 0;

#define test_success(M, ...) { fprintf(stderr, "["GREEN("SUCCESS")"] " M "\n", ##__VA_ARGS__); SUCCESS_COUNT += 1; }
#define test_fail(M, ...) { fprintf(stderr, "["RED("FAIL")"] " M "\n", ##__VA_ARGS__); FAIL_COUNT += 1; }
#define test(A, M_OK, M_ERR, ...) if(A) { test_success(M_OK, ##__VA_ARGS__); } else { test_fail(M_ERR, ##__VA_ARGS__); }

int successes() { return SUCCESS_COUNT; }
int failures() { return FAIL_COUNT; }

#endif
