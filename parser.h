#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <wchar.h>
#include <stdbool.h>
#include <ctype.h>
#include "bignum.h"

typedef struct _Num {
  Bignum *n;
  bool isReg;
} Num;

enum TypeOfToken {
  sum = 0,
  diff = 1,
  mul = 2,
  ddiv = 3,
  mmod = 4,
  eq = 5,
  great = 6,
  less = 7,
  greatEq = 8,
  lessEq = 9,
  copy = 10,
  go = 11,
  interrupt = 12
};

typedef struct _Token {
  enum TypeOfToken tp;
  Num *one;
  Num *two;
  Num *res;
} Token;

typedef struct _Program {
  uint64_t len;
  Token *tokens;
} Program;

#define TOKENS_BLOCK (100)
#define STR_BLOCK (1000)

Reterr parser(FILE *f, Program *prg, uint64_t *globalPos);

#endif // PARSER_H
