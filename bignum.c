#include "bignum.h"

#define MEM_ERR_CH(var) \
  if (check_alloc(var)) return MEMORY_ALLOCATE_ERROR;

#define ERR_VAR_CH if (_err) return _err

#define ERR_CH(func) _err = (func); ERR_VAR_CH

Reterr bnInit(Bignum *n, uint64_t len) {
  n = calloc(1,sizeof(Bignum));
  MEM_ERR_CH(n);

  n->len = ceil(len, BNCHUNK);

  n->num = calloc(n->len, sizeof(unsigned char));
  MEM_ERR_CH(n->num);

  n->sign = 0;
  return 0;
}

int bnChLen(Bignum *n, const int64_t len) {
  uint64_t oldLen = n->len;
  n->num = realloc(n->num, ceil(len,BNCHUNK)*sizeof(Bignum));
  MEM_ERR_CH(n->num);
  memset(n->num + oldLen + 1, 0, len-oldLen);
  return 0;
}

int bnSetInt(Bignum *n, const int64_t _var) {
  int _err = 0;
  uint64_t i = 0;
  int64_t var = _var;
  if (n->len < ceil(8, BNCHUNK)) {
    ERR_CH(bnChLen(n,ceil(8, BNCHUNK)));
  }

  n->sign = (var < 0);
  var = (var < 0) ? -var : var;

  for (i=0; i < n->len; i++) {
    (n->num)[i] = var % 256;
    var /= 256;
  }

  return 0;
}

Reterr bnSetStr(Bignum *n, const char *str) {
  int _err = 0;
  uint64_t i = 0;
  if (n->len < ceil(strlen(str), BNCHUNK)) {
    ERR_CH(bnChLen(n, ceil(strlen(str), BNCHUNK)));
  }

  n->sign = 1;

  for (i=0; i<n->len; i++) {
    (n->num)[i] = str[i];
  }

  return 0;
}

#define signSec (_signSec != b->sign)

Reterr bnSum(const Bignum *a, const Bignum *b, Bignum *n, bool _signSec) {
  /* _signSec = 1 -- -,
   * 0 -- sum
   */
  int _err = 0;
  uint64_t i = 0;
  int16_t res = 0;
  int16_t carry = 0;

  ERR_CH(bnSetInt(n, 0));
  ERR_CH(bnChLen(n, max(a->len, b->len)));

  for (i=0; i<n->len; i++) {
    res = (int16_t) a->num[i]
          + signSec * ((int16_t) b->num[i])
          + carry;
    n->num[i] = res % 256;
    carry = res / 256;
  }

  if (carry) {
    ERR_CH(bnChLen(n, n->len + 1));
        n->num[i] = carry;
  }

  return 0;
}


int bnCmp(const Bignum *a, Bignum *b) {
  /* 1: a>b
   * 0: a==b
   * -1: a<b
   */
  uint64_t i = 0;

  if ((a->len) != (b->len)) return (a->len) > (b->len);

  for(i=0;i<a->len;i++) {
    if (a->num[i] != b->num[i]) return (a->num[i] > b->num[i]);
  }

  return 0;
}


Reterr bnMul(const Bignum *a, const Bignum *b, Bignum *n) {
  int _err = 0;
  uint16_t res = 0;
  uint64_t i=0, j=0;
  unsigned char carry = 0;
  ERR_CH(bnSetInt(n, 0));
  ERR_CH(bnChLen(n, (a->len) + (b->len) + 1)); // delete +1

      for (i=0; i < a->len; i++) {
    carry = 0;
    for (j=0; j < b->len; j++) {
      res = (a->num[i]) * (b->num[j]) + carry + n->num[i+j];
      n->num[i+j] = res % 255;
      carry = res / 255;
    }
    n->num[i+j] = carry;
  }

  return 0;
}


Reterr bnSetFDeg(Bignum *n, int64_t deg) {
  int _err = 0;
  ERR_CH(bnSetInt(n, 0));
  ERR_CH(bnChLen(n, deg / 2));
  n->num[deg / 2] = (deg % 2) ? 0xF0 : 0x0F;
  return 0;
}


Reterr bnCopy(const Bignum *a, Bignum *n) {
  int _err = 0;
  uint64_t i = 0;
  ERR_CH(bnSetInt(n, 0));
  ERR_CH(bnChLen(n, a->len));

  n->len = a->len;
  n->sign = a->sign;
  for (i=0; i<a->len; i++) {
    n->num[i] = a->num[i];
  }
  return 0;
}


Reterr bnDiv(const Bignum *a, const Bignum *b, Bignum *n) {
  int _err = 0;
  uint64_t i = 0;
  Bignum *deg = NULL;
  Bignum *res = NULL;
  Bignum *divide = NULL;
  Bignum *_a = NULL;

  // Memory leaks when error
  ERR_CH(bnInit(deg, a->len - b->len + 1)); // delete +1
  ERR_CH(bnInit(res, 0));
  ERR_CH(bnInit(divide, 0));
  ERR_CH(bnCopy(a, _a));

  ERR_CH(bnSetInt(n, 0));
  ERR_CH(bnChLen(n, a->len - b->len +1));

  for (i=(a->len - b->len);i!=0;i--) {
    // Will be work slow, because every time he change size of `deg`
    ERR_CH(bnSetFDeg(deg, i));
    ERR_CH(bnMul(b, deg, divide));
    while (bnCmp(_a, divide)) {
      ERR_CH(bnSum(_a, divide, _a, 1));
      ERR_CH(bnSum(n, deg, n, 0));
    }
  }

  return 0;
}
