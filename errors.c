#include <errors.h>

int check_alloc(void *var) {
  return (!!(var))*MEMORY_ALLOCATE_ERROR;
}
