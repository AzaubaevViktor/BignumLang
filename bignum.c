#include "bignum.h"

#define MEM_ERR_CH(var) \
  if (!check_alloc(var)) return MEMORY_ALLOCATE_ERROR;

#define ERR_VAR_CH if (_err) return _err
#define ERR_CH(func) _err = (func); ERR_VAR_CH


Reterr bnInit(Bignum **n, uint64_t len) {
  *n = calloc(1,sizeof(Bignum));
  MEM_ERR_CH(*n);

  (*n)->len = ceil(len, BNCHUNK);

  (*n)->num = calloc((*n)->len, sizeof(unsigned char));
  MEM_ERR_CH((*n)->num);

  (*n)->sign = 0;
  return 0;
}


int bnChLen(Bignum *n, const int64_t len) {
  uint64_t oldLen = n->len;
  uint64_t newLen = ceil(len,BNCHUNK)*BNCHUNK;
  newLen = newLen ? newLen : BNCHUNK;
  n->num = realloc(n->num, newLen*sizeof(unsigned char));
  MEM_ERR_CH(n->num);
  if (newLen > oldLen)
    memset(n->num + oldLen, 0, newLen-oldLen);
  n->len = newLen;
  return 0;
}


Reterr bnNul(Bignum *n) {
  int _err = 0;
  ERR_CH(bnChLen(n, 0));
  n->sign = 0;
  return 0;
}


int bnSetInt(Bignum *n, const int64_t _var) {
  int _err = 0;
  uint64_t i = 0;
  int64_t var = _var;
  ERR_CH(bnChLen(n,ceil(8, BNCHUNK)));

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
  ERR_CH(bnChLen(n, ceil(strlen(str), BNCHUNK)));

  n->sign = 1;

  for (i=0; i<n->len; i++) {
    (n->num)[i] = str[i];
  }

  return 0;
}


#define _f(x,_max,val) ((x < _max) ? (val) : 0)


int _bnCmp(const Bignum *a, const Bignum *b, bool isSignCheck) {
  /* 1: a>b
   * 0: a==b
   * -1: a<b
   */
  uint64_t i = 0, j = 0;

  if ((isSignCheck) && (a->sign != b->sign)) return ((a->sign) > (b->sign)) ? -1 : 1;

  for(j = max(a->len, b->len); j != 0; j--) {
    i = j - 1;
    if (_f(i, a->len, a->num[i]) != _f(i, b->len, b->num[i]))
      return (_f(i, a->len, a->num[i]) < _f(i, b->len, b->num[i])) ? -1 : 1;
  }

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


Reterr _bnSum(const Bignum *a, const Bignum *b, Bignum *n, bool _signSec) {
  /* _signSec = 1 -- -,
   * 0 -- sum
   */
  int _err = 0;
  int bMoreA = 0;
  uint64_t i = 0;
  int16_t isSub = 0;
  int16_t res = 0;
  int16_t carry = 0;
  int16_t signFir = a->sign ? -1 : 1;
  int16_t signSec = (_signSec != b->sign) ? -1 : 1;
  Bignum *_b = NULL, *_a = NULL;
  int16_t trans = 0;

  ERR_CH(bnNul(n));
  ERR_CH(bnChLen(n, max(a->len, b->len)));

  ERR_CH(bnInit(&_a,0));
  ERR_CH(bnInit(&_b,0));

  if (signFir == signSec) {
    isSub = 1;
    _a = (Bignum *) a;
    _b = (Bignum *) b;
    n->sign = a->sign; // == b->sign;
    signFir = 1;
    signSec = 1;
  } else {
    isSub = -1;
    bMoreA = (-1 == bnCmpAbs(a,b));
    bnCopy(bMoreA ? b : a, _a);
    bnCopy(bMoreA ? a : b, _b);
    if (bMoreA) {
      res = signFir;
      signFir = signSec;
      signSec = res;
    }

    // After that |_a| >= |_b|, _b<0;

    // Determine sign of `n`
    _b->sign = !_b->sign;
    n->sign = n->sign != (1 == bnCmp(_a,_b));
    _b->sign = !_b->sign;
  }

  for (i=0; i<n->len; i++) {
    res = 0;

    trans = ((signFir != signSec) && (_f(i, _a->len, _a->num[i]) < _f(i, _b->len, _b->num[i])));

    res += trans*256 + (int16_t) _f(i, a->len, _a->num[i])
          + isSub * ((int16_t) _f(i, _b->len, _b->num[i]))
          + carry;

    n->num[i] = res % 256;
    carry = res / 256 - trans;
  }

  if (carry) {
    ERR_CH(bnChLen(n, n->len + 1));
        n->num[i] = carry;
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

  n->sign = (a->sign != b->sign);

  for (i=0; i < a->len; i++) {
    carry = 0;
    for (j=0; j < b->len; j++) {
      res = (a->num[i]) * (b->num[j]) + carry + n->num[i+j];
      n->num[i+j] = res % 256;
      carry = res / 256;
    }
    n->num[i+j] = carry;
  }

  return 0;
}


Reterr bnSetFDeg(Bignum *n, int64_t deg) {
  int _err = 0;
  ERR_CH(bnSetInt(n, 0));
  ERR_CH(bnChLen(n, deg / 2 + 1));
  n->num[deg / 2] = (deg % 2) ? 0xF0 : 0x0F;
  return 0;
}


Reterr bnDiv(const Bignum *a, const Bignum *b, Bignum *n) {
  int _err = 0;
  uint64_t i = 0;
  Bignum *deg = NULL;
  Bignum *res = NULL;
  Bignum *divide = NULL;
  Bignum *_a = NULL;
  Bignum *nul = NULL;


  // Memory leaks when error
  ERR_CH(bnInit(&nul,0));
  if (!bnCmp(a,nul)) {
    free(nul);
    return DIVISION_BY_ZERO;
  }
  ERR_CH(bnInit(&deg, a->len - b->len + 1)); // delete +1
  ERR_CH(bnInit(&res, 0));
  ERR_CH(bnInit(&divide, 0));
  ERR_CH(bnInit(&_a, 0));
  ERR_CH(bnCopy(a, _a));

  ERR_CH(bnSetInt(n, 0));
  ERR_CH(bnChLen(n, a->len - b->len +1));

  for (i=(a->len - b->len + 1);i!=0;i--) {
    // Will be work slow, because every time he change size of `deg`
    ERR_CH(bnSetFDeg(deg, i-1));
    ERR_CH(bnMul(b, deg, divide));
    while (bnCmp(_a, divide)) {
      ERR_CH(bnDiff(_a, divide, _a));
      ERR_CH(bnSum(n, deg, n));
    }
  }

  free(nul);
  free(deg);
  free(res);
  free(divide);
  free(_a);
  return 0;
}

Reterr bnMod(const Bignum *a, const Bignum *b, Bignum *n) {
  int _err = 0;
  Bignum *div = NULL;
  Bignum *divMulB = NULL;

  ERR_CH(bnDiv(a,b,div));
  ERR_CH(bnMul(div, b, divMulB));
  ERR_CH(bnDiff(a, divMulB, n));

  free(div);
  free(divMulB);
  return 0;
}


void bnPrintHex(const Bignum *a) {
  uint64_t i = 0, firstNul = 0;
  int isWrite = 0;
  (a->sign) ? printf("-") : 0;
  for (i=a->len; i!=0; i--) {
    if (!a->num[i-1] && !isWrite) {
      firstNul++;
    } else {
      if (!isWrite) printf("[%"PRIu64"]", firstNul);
      isWrite = 1;
      printf("%.2X",a->num[i-1]);
    }
  }
  if (!isWrite) printf("[%"PRIu64"]", firstNul);
}


#undef MEM_ERR_CH
#undef ERR_VAR_CH
#undef ERR_CH
