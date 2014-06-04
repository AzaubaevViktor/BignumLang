#ifndef REGS_H
#define REGS_H

#include <inttypes.h>
#include "bignum.h"
#include "errors.h"


typedef struct _Reg {
  Bignum *key;
  Bignum *val;
} Reg;

typedef struct _Regs {
  uint64_t len;
  Reg *reg;
} Regs;

#include "regs.h"

Reterr rgInit(Regs **regs);
Reterr _rgAdd(Regs *regs, Bignum *key, Bignum *val);
Reterr rgGet(Regs *regs, Bignum *key, Bignum **val);
Reterr rgSet(Regs *regs, Bignum *key, Bignum *val);

#endif // REGS_H
