#include <stdio.h>
#include "test.h"

int main(void)
{
  TestContext cnt;
  cnt.quiet = 0;
  printf("Hello World!\n");
  test_bignum(&cnt);
  return 0;
}

