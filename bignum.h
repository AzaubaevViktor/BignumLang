#ifndef BIGNUM_H
#define BIGNUM_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>
#include <errors.h>
#include <string.h>

#define BNCHUNK (1)

#define ceil(a,b) (((a) / (b)) + !!((a) % (b)))
#define min(a,b) (((a) < (b)) ? (a) : (b))
#define max(a,b) (((a) > (b)) ? (a) : (b))

typedef struct _Bignum {
  bool sign; // 1 -- negative
  unsigned char *num;
  uint64_t len;
} Bignum;

Reterr bnInit(Bignum **n, uint64_t len);
int bnChLen(Bignum *n, const int64_t len);
Reterr bnNul(Bignum *n);
int bnSetInt(Bignum *n, const int64_t _var);
Reterr bnSetStr(Bignum *n, const char *str);
uint64_t bnGetLen(const Bignum *a);
int _bnCmp(const Bignum *a, const Bignum *b, bool isSignCheck);
#define bnCmp(a,b) _bnCmp(a,b,1)
#define bnCmpAbs(a,b) _bnCmp(a,b,0)
Reterr _bnSum(const Bignum *a, const Bignum *b, Bignum *n, bool _signSec);
Reterr bnCopy(const Bignum *a, Bignum *n);
#define bnSum(a,b,n) (_bnSum(a,b,n,0))
#define bnDiff(a,b,n) (_bnSum(a,b,n,1))
Reterr bnMul(const Bignum *a, const Bignum *b, Bignum *n);
Reterr bnSet1Deg(Bignum *n, int64_t deg);
Reterr bnDiv(const Bignum *a, const Bignum *b, Bignum *n);
Reterr bnMod(const Bignum *a, const Bignum *b, Bignum *n);
void bnPrintHex(const Bignum *a);

#define ERR_VAR_CH if (_err) return _err
#define ERR_CH(func) _err = (func); ERR_VAR_CH

#endif // BIGNUM_H
