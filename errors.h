#ifndef ERRORS_H
#define ERRORS_H

#include <wchar.h>
#include <inttypes.h>

typedef struct _Error {
  int code;
  union {
    wchar_t wstr[256];
    char str[256];
    int64_t num;
  };
} Error;

typedef int Reterr;

#define MEMORY_ALLOCATE_ERROR (1)

#define DIVISION_BY_ZERO (10)

#define PARSE_ERR_READ_NUM (20)
#define PARSE_ERR_EQ (21)
#define PARSE_ERR_COMMAND (22)
#define PARSE_ERR_GREAT (23)
#define PARSE_ERR_ARROW_FIRST (24)
#define PARSE_ERR_ARROW_SECOND (25)
#define PARSE_ERR_COPY_LAST_OP_NUM (26)
#define PARSE_ERR_NOT_SEMICOLON (27)

Reterr check_alloc(void *var);


#endif // ERRORS_H
