#ifndef PARSER_H
#define PARSER_H

#define PARSER_COMPATIBILITY "APIv4"

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

typedef struct _LineStr {
  uint64_t globalLine;
  uint64_t globalPos;
  unsigned char *str;
} LineStr;

#define TOKENS_BLOCK (100)
#define STR_BLOCK (1000)

void numFree(Num *n);
void tokenFree(Token *tk);
Reterr programInit(Program **prg);
void programFree(Program *prg);
Reterr parser(FILE *f, Program *prg, LineStr *l);
char *getErrorMsg(int _err);

#endif // PARSER_H
