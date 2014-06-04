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
  *n = (Num *) calloc(1, sizeof(Num));
  MEM_ERR_CH(*n);
  ERR_CH(bnInit(&((*n)->n), 0));
  return 0;
}


void numFree(Num *n) {
  free(n->n);
  free(n);
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

  bnFree(ten);
  bnFree(res);
  bnFree(resDig);

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
          return PARSE_ERR_LESS;
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
  ERR_CH(readNum(str, pos, tk->one));

  if (tk->tp < 10) {
    ERR_CH(initNum(&tk->two));
    ERR_CH(readNum(str, pos, tk->two));
  }

  ERR_CH(readArrow(str,pos));

  ERR_CH(initNum(&tk->res));
  ERR_CH(readNum(str, pos, tk->res));

  // Check result num
  if ((go != tk->tp) && (0 == tk->res->isReg))
    return PARSE_ERR_COPY_LAST_OP_NUM;

  return 0;
}


Reterr parser(FILE *f, Program *prg, LineStr *l) {
  int ch = 0;
  int _err = 0;
  uint64_t strLen = 0;
  uint64_t strPos = 0;
  uint64_t pos = 0;

  l->str = calloc(STR_BLOCK, sizeof(unsigned char));
  MEM_ERR_CH(l->str);
  strLen = STR_BLOCK;
  strPos = 0;

  l->globalLine = 1;
  l->globalPos = 0;

  prg->len = 0;
  prg->tokens = 0;

  while (1) {
    ch = fgetc(f);

    if (feof(f)) {
      if (0 != strPos) {
        return PARSE_ERR_NOT_SEMICOLON;
      } else {
        break;
      }
    }

    (l->globalPos)++;

    if ('\n' == ch) {
      (l->globalLine)++;
      l->globalPos = 0;
    }

    if (!isspace(ch))
      *(l->str + strPos++) = ch;

    if (strLen == strPos) {
      strLen += STR_BLOCK;
      l->str = (unsigned char *)realloc(l->str, strLen * sizeof(unsigned char));
      MEM_ERR_CH(l->str);
    }

    if (';' == ch) {
      if (!(prg->len % TOKENS_BLOCK)) {
        prg->tokens = (Token *) realloc(prg->tokens, (prg->len + TOKENS_BLOCK) * sizeof(Token));
        MEM_ERR_CH(prg->tokens);
      }
      pos = 0;
      _err = readToken(l->str, &pos, &(prg->tokens[(prg->len)++]));
      if ((';' != l->str[pos]) && (!_err)) _err = PARSE_ERR_NOT_SEMICOLON;
      if (_err) l->globalPos -= (strPos - pos);
      ERR_VAR_CH;

      memset(l->str, 0, strPos);
      strPos = 0;
    }

  }

  return 0;
}
