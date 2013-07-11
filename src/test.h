// License: BSD 2-clause
// Author: Dane Larsen
// See LICENSE.txt for the complete license text

#ifndef TEST_H
#define TEST_H

#include <stdio.h>

static int SUCCESS_COUNT = 0;
static int FAIL_COUNT = 0;

#ifdef TEST

#define test_success(M, ...) { fprintf(stderr, "[SUCCESS] " M "\n", ##__VA_ARGS__); SUCCESS_COUNT += 1; }
#define test_fail(M, ...) { fprintf(stderr, "*** FAIL *** " M "\n\n", ##__VA_ARGS__); FAIL_COUNT += 1; }
#define test(A, M, ...) if(A) { test_success(M, ##__VA_ARGS__); } else { test_fail(M, ##__VA_ARGS__); }

#else

#define test_success(M, ...)
#define test_fail(M, ...)
#define test(A, M, ...)

#endif //TEST

int successes() { return SUCCESS_COUNT; }
int failures() { return FAIL_COUNT; }

int report_results()
{
    fprintf(stderr, "Test results: [%d successes, %d failures]\n", SUCCESS_COUNT, FAIL_COUNT);

    if(FAIL_COUNT > 0)
        return FAIL_COUNT;
    else
        return 0;
}

#endif //TEST_H
