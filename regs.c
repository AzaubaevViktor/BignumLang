#include "regs.h"

Reterr rgInit(Regs **regs) {
  int _err = 0;
  *regs = calloc(1, sizeof(Regs));
  MEM_ERR_CH(*regs);
  (*regs)->len = 0;
  (*regs)->reg = NULL;
  return 0;
}


void rgFree(Regs *regs) {
  uint64_t i = 0;
  if (regs) {
    for (i=0; i < regs->len; i++) {
      bnFree(regs->reg[i].key);
      bnFree(regs->reg[i].val);
    }
    free(regs->reg);
    free(regs);
  }
}


Reterr _rgAdd(Regs *regs, Bignum *key, Bignum *val) {
  int _err = 0;
  regs->reg = (Reg *) realloc(regs->reg, (regs->len + 1) * sizeof(Reg));
  MEM_ERR_CH(regs->reg);
  ERR_CH(bnInit(&((regs->reg[regs->len]).key), 0));
  ERR_CH(bnInit(&((regs->reg[regs->len]).val), 0));
  ERR_CH(bnCopy(key, (regs->reg[regs->len]).key));
  ERR_CH(bnCopy(val, (regs->reg[regs->len]).val));
  (regs->len)++;
  return 0;
}

Reterr rgGet(Regs *regs, Bignum *key, Bignum **val) {
  int _err = 0;
  uint64_t i = 0;

  for (i=0; i < regs->len; i++) {
    if (!bnCmp(key, regs->reg[i].key)) {
      *val = regs->reg[i].val;
      return 0;
    }
  }
  return RUNTIME_REGISTER_NOT_SET;
}

Reterr rgSet(Regs *regs, Bignum *key, Bignum *val) {
  int _err;
  Bignum *_val = NULL;
  if (RUNTIME_REGISTER_NOT_SET == rgGet(regs, key, &_val)) {
    ERR_CH(_rgAdd(regs, key, val));
  } else {
    ERR_CH(bnCopy(val, _val));
  }
  return 0;
}
