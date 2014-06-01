#include <errors.h>

Reterr check_alloc(void *var) {
  return (!!(var))*MEMORY_ALLOCATE_ERROR;
}
