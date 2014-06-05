#include "statemachine.h"


Reterr machineInitState(State **state) {
  int _err = 0;
  *state = (State *) calloc(1, sizeof(State));
  ERR_CH(rgInit(&((*state)->regs)));
  (*state)->prg = NULL;
  (*state)->cs = 0;
  (*state)->op = 0;
  return 0;
}

void machineFreeState(State *state) {
  rgFree(state->regs);
  programFree(state->prg);
  free(state);
}


Reterr getNumByReg(Regs *regs, Num *num, Bignum **val) {
  int _err;
  if (num->isReg) {
    ERR_CH(rgGet(regs, num->n, val));
  } else {
    *val = num->n;
  }
  return 0;
}


Reterr machine(State *state) {
  int _err = 0;
  int _cmp = 0;
  int isJump = 0;
  Token *tk;
  Bignum *one = NULL, *two = NULL, *res = NULL, *reg = NULL;
  Num _res;
  Bignum *_nul = NULL;
  ERR_CH(bnInit(&_nul, 0));

  isJump = 0;

  tk = &(state->prg->tokens[state->cs]);

  state->op = 0;
  ERR_CH(getNumByReg(state->regs, tk->one, &one));
  (state->op)++;

  if (tk->two) {
    ERR_CH(getNumByReg(state->regs, tk->two, &two));
    (state->op)++;
  }

  _err = getNumByReg(state->regs, tk->res, &res);
  if (RUNTIME_REGISTER_NOT_SET == _err) {
    rgSet(state->regs, tk->res->n, _nul);
    ERR_CH(getNumByReg(state->regs, tk->res, &res));
  }

  if (go != tk->tp) {
    _res.isReg = 1;
    _res.n = res;
    _err = getNumByReg(state->regs, &_res, &reg);
    if (RUNTIME_REGISTER_NOT_SET == _err) {
      rgSet(state->regs, res, _nul);
      ERR_CH(getNumByReg(state->regs, &_res, &reg));
    }
  }


  (state->op)++;

  if (tk->tp <= 9) {
    if (tk->tp <= 4) {
      switch (tk->tp) {
        case sum:
          ERR_CH(bnSum(one, two, reg));
          break;
        case diff:
          ERR_CH(bnDiff(one, two, reg));
          break;
        case mul:
          ERR_CH(bnMul(one, two, reg));
          break;
        case ddiv:
          ERR_CH(bnDiv(one, two, reg));
          break;
        case mmod:
          ERR_CH(bnMod(one, two, reg));
          break;
        default:
          break;
      }
    } else if (tk->tp <= 9) {
      _cmp = bnCmp(one, two);
      switch (tk->tp) {
        case eq:
          _cmp = (0 == _cmp);
          break;
        case great:
          _cmp = (1 == _cmp);
          break;
        case less:
          _cmp = (-1 == _cmp);
          break;
        case greatEq:
          _cmp = (-1 != _cmp);
          break;
        case lessEq:
          _cmp = (1 != _cmp);
          break;
        default:
          break;
      }
      bnSetInt(reg, (uint64_t) _cmp);
    }
    //ERR_CH(rgSet(state->regs, tk->res->n, res));
    //ERR_CH(bnCopy(res, reg));
  } else if (tk->tp == copy) {
    //ERR_CH(rgSet(state->regs, tk->res->n, one));
    ERR_CH(bnCopy(one, reg));
  } else if (tk->tp == go) {
    if (0 != bnCmp(one, _nul)) {
      state->cs = bnBignumToUInt64(res);
      isJump = 1;
    }
  } else if (tk->tp == interrupt) {
    printf("Interrupt!");
  }

  if (!isJump) (state->cs)++;

  bnFree(_nul);

  return 0;
}


const char* tokenNames[] = {"+","-","*","/","*","==",">>","<<",">=","<=","&","~","i"};


void printToken(Token *tk) {
  printf("{%s}",tokenNames[tk->tp]);
  printf((tk->one->isReg) ? " $" : " #");
  bnPrintHex(tk->one->n);
  if (tk->two) {
    printf((tk->two->isReg) ? " $" : " #");
    bnPrintHex(tk->two->n);
  }
  printf(" -> ");
  printf((tk->res->isReg) ? " $" : " #");
  bnPrintHex(tk->res->n);
  printf("\n");
}


void printState(State *state) {
  uint64_t i = 0;
  printf("CS: `%"PRIu64"`\n", state->cs);
  printf("Regs:\n");

  for (i=0; i < state->regs->len; i++) {
    printf("`");
    bnPrintHex(state->regs->reg[i].key);
    printf("`: ");

    printf("`");
    bnPrintHex(state->regs->reg[i].val);
    printf("`\n");
  }
  printf("Token: ");
  if (state->cs < state->prg->len) {
  printToken(&state->prg->tokens[state->cs]);
  } else {
    printf("EOPRG");
  }
  printf("\n");
}
