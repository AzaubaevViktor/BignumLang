#include <stdio.h>
#include "test.h"
#include "statemachine.h"

int main(void)
{
  TestContext cnt;
  State state;
  Program prg;
  uint64_t globalPos = 0, pos = 0;
  FILE *f;

  cnt.quiet = 0;
  printf("Tests:\n");
  //test_bignum(&cnt);

  printf("Programs:\n");
  f = fopen("./test.bin","rt");
  if (!f) {
    printf("file open error\n");
    return 0;
  }
  printf("ERROR `%d`\n", parser(f, &prg, &globalPos));
  machine(&state, &prg);
  fclose(f);
  printf("Programm End!");
  return 0;
}

