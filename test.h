#ifndef TEST_H
#define TEST_H

#include <stdio.h>
#include <stdbool.h>
#include "bignum.h"

typedef struct _TestContext {
  bool quiet;
} TestContext;

int test_bignum(TestContext *cnt);

#endif // TEST_H
