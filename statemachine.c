#include "statemachine.h"

int machine(State *state, Program *prg) {
  uint64_t i = 0;
  for (i=0; i<prg->len; i++) {
    printf("{%d}",prg->tokens[i].tp);
    printf((prg->tokens[i].one->isReg) ? " $" : " #");
    bnPrintHex(prg->tokens[i].one->n);
    if (prg->tokens[i].two) {
      printf((prg->tokens[i].two->isReg) ? " $" : " #");
      bnPrintHex(prg->tokens[i].two->n);
    }
    printf(" -> ");
    printf((prg->tokens[i].res->isReg) ? " $" : " #");
    bnPrintHex(prg->tokens[i].res->n);
    printf("\n");
  }
  return 0;
}
