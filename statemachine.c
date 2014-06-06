#include "statemachine.h"


Reterr machineInitState(State **state) {
  int _err = 0;
  *state = (State *) calloc(1, sizeof(State));
  ERR_CH(rgInit(&((*state)->regs)));
  (*state)->prg = NULL;
  (*state)->cs = 0;
  (*state)->op = 0;
  (*state)->quiet = 0;
  ERR_CH(bnInit(&((*state)->nul),0));
  ERR_CH(bnInit(&((*state)->bnI),0));
  ERR_CH(bnInit(&((*state)->bn1),0));
  ERR_CH(bnInit(&((*state)->res),0));

  ERR_CH(bnSetInt((*state)->bn1, 1));
  return 0;
}

void machineFreeState(State *state) {
  rgFree(state->regs);
  programFree(state->prg);
  bnFree(state->nul);
  bnFree(state->bnI);
  bnFree(state->bn1);
  bnFree(state->res);
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
  Bignum *one = NULL, *two = NULL, *result = NULL, *regResult = NULL;
  Num _res;

  isJump = 0;

  tk = &(state->prg->tokens[state->cs]);

  state->op = 0;

  ERR_CH(getNumByReg(state->regs, tk->one, &one));
  (state->op)++;

  if (tk->two) {
    ERR_CH(getNumByReg(state->regs, tk->two, &two));
    (state->op)++;
  }

  _err = getNumByReg(state->regs, tk->res, &result);
  if (RUNTIME_REGISTER_NOT_SET == _err) {
    rgSet(state->regs, tk->res->n, state->nul);
    ERR_CH(getNumByReg(state->regs, tk->res, &result));
  }

  if (go != tk->tp) {
    _res.isReg = 1;
    _res.n = result;
    _err = getNumByReg(state->regs, &_res, &regResult);
    if (RUNTIME_REGISTER_NOT_SET == _err) {
      rgSet(state->regs, result, state->nul);
      ERR_CH(getNumByReg(state->regs, &_res, &regResult));
    }
  }


  (state->op)++;

  if (tk->tp <= 9) {
    if (tk->tp <= 4) { // sum..mmod
      switch (tk->tp) {
        case sum:
          ERR_CH(bnSum(one, two, regResult));
          break;
        case diff:
          ERR_CH(bnDiff(one, two, regResult));
          break;
        case mul:
          ERR_CH(bnMul(one, two, regResult));
          break;
        case ddiv:
          ERR_CH(bnDiv(one, two, regResult));
          break;
        case mmod:
          ERR_CH(bnMod(one, two, regResult));
          break;
        default:
          break;
      }
    } else if (tk->tp <= 9) { // eq..lessEq
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
      bnSetInt(regResult, (uint64_t) _cmp);
    }
  } else if (tk->tp == copy) { // copy
    ERR_CH(bnCopy(one, regResult));
  } else if (tk->tp == go) { // go
    if (!bnCmp(one, state->nul)) {
      state->cs = bnBignumToUInt64(result);
      isJump = 1;
    }
  } else if (tk->tp == interrupt) { // interrupt
    ERR_CH(bnSetInt(state->bnI, 0));

    if (!bnCmp(one, state->bnI)) { // 0
      (state->quiet) || printf("inp>>`");
      ERR_CH(bnSetInt(regResult, getchar()));
      (state->quiet) || printf("`");
    }

    ERR_CH(bnSum(state->bnI,state->bn1,state->res));
    ERR_CH(bnCopy(state->res, state->bnI));

    if (!bnCmp(one, state->bnI)) { // 1
      (state->quiet) || printf("out:`");
      putchar((int) (bnBignumToUInt64(regResult) % 256));
      (state->quiet) || printf("`\n");
    }

    ERR_CH(bnSum(state->bnI,state->bn1,state->res));
    ERR_CH(bnCopy(state->res, state->bnI));

    if (!bnCmp(one, state->bnI)) { // 2

    }

    ERR_CH(bnSum(state->bnI,state->bn1,state->res));
    ERR_CH(bnCopy(state->res, state->bnI));

    if (!bnCmp(one, state->bnI)) { // 3

    }

    ERR_CH(bnSum(state->bnI,state->bn1,state->res));
    ERR_CH(bnCopy(state->res, state->bnI));

    if (!bnCmp(one, state->bnI)) { // 4

    }
  }

  if (!isJump) (state->cs)++;

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
