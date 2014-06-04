#include "regs.h"

Reterr rgInit(Regs **regs) {
  int _err = 0;
  *regs = calloc(1, sizeof(Regs));
  MEM_ERR_CH(*regs);
  (*regs)->len = 0;
  (*regs)->regs = NULL;
  return 0;
}


Reterr _rgAdd(Regs *regs, Bignum *key, Bignum *val) {
  int _err = 0;
  regs->regs = realloc(regs->regs, (++(regs->len)) * sizeof(Reg));
  MEM_ERR_CH(regs->regs);
  ERR_CH(bnInit(&(regs->regs[regs->len].key), 0));
  ERR_CH(bnInit(&(regs->regs[regs->len].val), 0));
  ERR_CH(bnCopy(key, regs->regs[regs->len].key));
  ERR_CH(bnCopy(val, regs->regs[regs->len].val));
  return 0;
}

Reterr rgGet(Regs *regs, Bignum *key, Bignum **val) {
  int _err = 0;
  uint64_t i = 0;

  for (i=0; i < regs->len; i++) {
    if (!bnCmp(key, regs->regs[regs->len].key)) {
      *val = regs->regs[regs->len].val;
      return 0;
    }
  }
  return RUNTIME_REGISTER_NOT_SET;
}

Reterr rgSet(Regs *regs, Bignum *key, Bignum *val) {
  int _err;
  Bignum *_val = NULL;
  if (RUNTIME_REGISTER_NOT_SET == rgGet(regs, key, &val)) {
    ERR_CH(_rgAdd(regs, key, val));
  } else {
    ERR_CH(bnCopy(val, _val));
  }
  return 0;
}
