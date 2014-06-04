#include "statemachine.h"


Reterr getNum(Num *num, Regs *regs, Bignum **val) {
  int _err;
  if (num->isReg) {
    ERR_CH(rgGet(regs, num->n, val));
  } else {
    *val = num->n;
  }
  return 0;
}


Reterr machine(State *state) {
  int _err;
  Token *tk;
  Bignum *one = NULL, *two = NULL, *res = NULL;
  Bignum *nul = NULL;
  ERR_CH(bnInit(&nul,0));
  ERR_CH(bnSetInt(nul, 0));

  tk = &(state->prg->tokens[state->cs]);
  switch (tk->tp) {
    case sum:
      state->op = 0;
      ERR_CH(getNum(tk->one, state->regs, &one));
      (state->op)++;
      ERR_CH(getNum(tk->two, state->regs, &two));
      (state->op)++;
      _err = getNum(tk->res, state->regs, &res);
      if (RUNTIME_REGISTER_NOT_SET == _err) {
        rgSet(state->regs, tk->res->n, nul);
        ERR_CH(getNum(tk->res, state->regs, &res));
      }
      (state->op)++;
      ERR_CH(bnSum(one, two, res));
      rgSet(state->regs, tk->res->n, res);
      break;

    default:
      ;
      break;
  }

  (state->cs)++;
  return 0;
}


void printToken(Token *tk) {
  printf("{%d}",tk->tp);
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
  printf("Token:");
  printToken(&state->prg->tokens[state->cs]);
  printf("\n");
}
