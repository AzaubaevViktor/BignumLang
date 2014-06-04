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

  tk = &(state->prg->tokens[state->cs]);

  if (tk->tp < 2) {
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

    switch (tk->tp) {
      case sum:
        ERR_CH(bnSum(one, two, res));
        break;
      case diff:
        ERR_CH(bnDiff(one,two,res));
        break;
      default:
        break;
    }

    ERR_CH(rgSet(state->regs, tk->res->n, res));
  }


  (state->cs)++;

  bnFree(nul);

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
