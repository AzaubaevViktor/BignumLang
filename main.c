#include <stdio.h>
#include "error.h"
#include "test.h"
#include "statemachine.h"

int main(void)
{
  int _err = 0;
  TestContext cnt;
  State *state;
  FILE *f;
  LineStr l;

  printf("Statemachine version compatibility : %s\n", STATEMACHINE_COMPATIBILITY);
  printf("Parser version compatibility       : %s\n", PARSER_COMPATIBILITY);

  cnt.quiet = 0;
  printf("TESTS:\n");
  //test_bignum(&cnt);

  printf("PROGRAMM:\n");
  f = fopen("./test.bin","rt");
  if (!f) {
    printf("File open error\n");
    return 0;
  }

  machineInitState(&state);
  programInit(&(state->prg));
  _err = parser(f, state->prg, &l);

  if (_err) {
    printf("On line `%"PRIu64"`, pos: `%"PRIu64"`:\n", l.globalLine, l.globalPos);
    printf("\"%s\"\n", l.str);
    printf(" _% *s\n", (int) l.globalPos, "^");
    printf("%s\n\n", getErrorMsg(_err));
    free(l.str);
    fclose(f);
    return 1;
  }
  free(l.str);


  while (state->cs < state->prg->len) {
    printf("===== STEP =====\n");
    printState(state);
    _err = machine(state);
  }
  if (_err) printf("'%s' on operand `%"PRIu64"`\n", getErrorMsg(_err), state->op);
  printState(state);

  machineFreeState(state);

  fclose(f);
  printf("Program End!\n");
  return 0;
}

