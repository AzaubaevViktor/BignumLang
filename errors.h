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

#define MEMORY_ALLOCATE_ERROR (1);

#define DIVISION_BY_ZERO (10);

int check_alloc(void *var);


#endif // ERRORS_H
