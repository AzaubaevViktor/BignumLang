#include <stdio.h>
#include "test.h"
#include "statemachine.h"

int main(void)
{
  int _err = 0;
  TestContext cnt;
  State state;
  Program prg;
  FILE *f;
  LineStr l;

  cnt.quiet = 0;
  printf("Tests:\n");
  //test_bignum(&cnt);

  printf("Program:\n");
  f = fopen("./test.bin","rt");
  if (!f) {
    printf("file open error\n");
    return 0;
  }
  _err = parser(f, &prg, &l);
  if (_err) {
    printf("On line `%"PRIu64"`, pos: `%"PRIu64"`:\n", l.globalLine, l.globalPos);
    printf("\"%s\"\n", l.str);
    printf(" % *s\n", (int) l.globalPos, "^");
    printf("%s\n\n", getErrorMsg(_err));
  }

  machine(&state, &prg);
  fclose(f);
  printf("Program End!\n");
  return 0;
}

