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
#define PARSE_ERR_LESS (24)
#define PARSE_ERR_ARROW_FIRST (25)
#define PARSE_ERR_ARROW_SECOND (26)
#define PARSE_ERR_NOT_SEMICOLON (28)

#define RUNTIME_REGISTER_NOT_SET (30)

Reterr check_alloc(void *var);
#define MEM_ERR_CH(var) \
  /*printf("Allocate '%s' on `%s`:`%d` '0x%X'\n", # var, __FILE__, __LINE__, (unsigned int) (var));*/\
  if (!check_alloc(var)) return MEMORY_ALLOCATE_ERROR;
#define ERR_VAR_CH if (_err) return _err
#define ERR_CH(func) _err = (func); ERR_VAR_CH

#endif // ERRORS_H
