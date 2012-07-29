#ifndef TEST_H
#define TEST_H

static int SUCCESS_COUNT = 0;
static int FAIL_COUNT = 0;

#ifdef TEST

#include "dbg.h"

#define test_success(M, ...) { fprintf(stderr, "["GREEN("SUCCESS")"] " M "\n", ##__VA_ARGS__); SUCCESS_COUNT += 1; }
#define test_fail(M, ...) { fprintf(stderr, "["RED("FAIL")"] " M "\n", ##__VA_ARGS__); FAIL_COUNT += 1; }
#define test(A, M_OK, M_ERR, ...) if(A) { test_success(M_OK, ##__VA_ARGS__); } else { test_fail(M_ERR, ##__VA_ARGS__); }

#else

#define test_success(M, ...)
#define test_fail(M, ...)
#define test(A, M_OK, M_ERR, ...)

#endif //TEST

int successes() { return SUCCESS_COUNT; }
int failures() { return FAIL_COUNT; }

int report_results()
{ 
    debug("Test results: [" GREEN("%d successes") ", "  RED("%d failures") "]", SUCCESS_COUNT, FAIL_COUNT);

    if(FAIL_COUNT > 0)
        return FAIL_COUNT;
    else
        return 0;
}

#endif //TEST_H
