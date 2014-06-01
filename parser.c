#include <parser.h>

int _isDigit(int ch) {
  return
      (('0' <= ch) && (ch <= '9')) ||
      (('A' <= ch) && (ch <= 'F'));
}


unsigned char _sym2num(int ch) {
  if (('0' <= ch) && (ch <= '9')) {
    return ch - '0';
  }
  if (('A' <= ch) && (ch <= 'F')) {
    return ch - 'A';
  }
  return 0;
}


Reterr initNum(Num **n) {
  int _err;
  *n = calloc(1, sizeof(Num));
  ERR_CH(!check_alloc(*n));
  return 0;
}


Reterr readNum(unsigned char *str, uint64_t *pos, Num *num) {
  int ch = 0;
  int _err = 0;
  Bignum *ten = NULL;
  Bignum *res = NULL;
  Bignum *resDig = NULL;

  switch (*(str + *pos)) {
    case '$':
      num->isReg = 1;
      break;
    case '#':
      num->isReg = 0;
      break;
    default:
      return PARSE_ERR_READ_NUM;
      break;
  }

  (*pos)++;

  ERR_CH(bnInit(&(num->n),0));
  ERR_CH(bnInit(&ten,0));
  ERR_CH(bnInit(&res,0));
  ERR_CH(bnInit(&resDig,0));

  ERR_CH(bnSetInt(ten,0x10));

  while (1) {
    ch = *(str + *(pos));
    if (_isDigit(ch)) {
      ERR_CH(bnMul(num->n, ten, res));
      ERR_CH(bnCopy(res, num->n));

      ERR_CH(bnSetInt(resDig, _sym2num(ch)));

      ERR_CH(bnSum(num->n, resDig, res));
      ERR_CH(bnCopy(res, num->n));
    } else {
      break;
    }
    (*pos)++;
  }

  free(ten);
  free(res);
  free(resDig);

  return 0;
}


Reterr readArrow(unsigned char *str, uint64_t *pos) {
  if ('-' != *(str + *pos)) {
    return PARSE_ERR_ARROW_FIRST;
  }

  (*pos)++;

  if ('>' != *(str + *pos)) {
    return PARSE_ERR_ARROW_SECOND;
  }

  (*pos)++;
  return 0;
}


Reterr readToken(unsigned char *str, uint64_t *pos, Token *tk) {
  int _err = 0;

  switch (*(str + *pos)) {
    case '+':
      tk->tp = sum;
      break;
    case '-':
      tk->tp = diff;
      break;
    case '*':
      tk->tp = mul;
      break;
    case '/':
      tk->tp = ddiv;
      break;
    case '%':
      tk->tp = mmod;
      break;

    case '=':
      (*(pos))++;
      if ('=' == *(str + *(pos))) {
        tk->tp = eq;
      } else {
        return PARSE_ERR_EQ;
      }
      break;
    case '>':
      (*(pos))++;
      if ('=' == *(str + *(pos))) {
        tk->tp = greatEq;
      } else {
        if ('>' == *(str + *(pos))) {
          tk->tp = great;
        } else {
          return PARSE_ERR_GREAT;
        }
      }
      break;
    case '<':
      (*(pos))++;
      if ('=' == *(str + *(pos))) {
        tk->tp = lessEq;
      } else {
        if ('<' == *(str + *(pos))) {
          tk->tp = less;
        } else {
          return PARSE_ERR_EQ;
        }
      }
      break;

    case '&':
      tk->tp = copy;
      break;

    case '~':
      tk->tp = go;
      break;

    case 'i':
      tk->tp = interrupt;
      break;

    default:
      return PARSE_ERR_COMMAND;
      break;
  }
  (*pos)++;


  ERR_CH(initNum(&tk->one));
  ERR_CH(bnInit(&(tk->one->n), 0));
  ERR_CH(readNum(str, pos, tk->one));

  if (tk->tp < 10) {
    ERR_CH(initNum(&tk->two));
    ERR_CH(bnInit(&tk->two->n, 0));
    ERR_CH(readNum(str, pos, tk->two));
  }

  ERR_CH(readArrow(str,pos));

  ERR_CH(initNum(&tk->res));
  ERR_CH(bnInit(&tk->res->n, 0));
  ERR_CH(readNum(str, pos, tk->res));

  // Check result num
  if ((go != tk->tp) && (0 == tk->res->isReg))
    return PARSE_ERR_COPY_LAST_OP_NUM;

  return 0;
}


Reterr parser(FILE *f, Program *prg, uint64_t *globalPos) {
  int ch = 0;
  int _err = 0;
  unsigned char *str = NULL;
  uint64_t strLen = 0;
  uint64_t strPos = 0;
  uint64_t pos = 0;


  str = calloc(STR_BLOCK, sizeof(unsigned char));
  ERR_CH(!check_alloc(str));
  strLen = STR_BLOCK;
  strPos = 0;

  prg->len = 0;

  while (1) {
    ch = fgetc(f);

    if (feof(f)) {
      if (0 != strPos) {
        return PARSE_ERR_NOT_SEMICOLON;
      } else {
        break;
      }
    }

    (*globalPos)++;
    if (!isspace(ch))
      *(str + strPos++) = ch;

    if (strLen == strPos) {
      strLen += STR_BLOCK;
      str = realloc(str, strLen);
      ERR_CH(!check_alloc(str));
    }

    if (';' == ch) {
      if (!(prg->len % TOKENS_BLOCK)) {
        prg->tokens = realloc(prg->tokens, prg->len + TOKENS_BLOCK);
        ERR_CH(!check_alloc(prg->tokens));
      }
      pos = 0;
      ERR_CH(readToken(str, &pos, &prg->tokens[prg->len++]));

      memset(str, 0, strPos);
      strPos = 0;
    }

  }

  //free(str); Programm drop with sigsegv
  return 0;
}
