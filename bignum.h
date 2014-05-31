#ifndef BIGNUM_H
#define BIGNUM_H

#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>
#include <errors.h>
#include <string.h>

#define BNCHUNK (16)

#define ceil(a,b) (((a) / (b)) + !!((a) % (b)))
#define min(a,b) (((a) < (b)) ? (a) : (b))
#define max(a,b) (((a) > (b)) ? (a) : (b))

typedef struct _Bignum {
  bool sign; // 1 -- negative
  unsigned char *num;
  uint64_t len;
} Bignum;

#endif // BIGNUM_H
