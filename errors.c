#include <errors.h>

Reterr check_alloc(void *var) {
  return (!!(var))*MEMORY_ALLOCATE_ERROR;
}


char *getErrorMsg(int _err) {
  switch (_err) {
    case MEMORY_ALLOCATE_ERROR:
      return
          "Memory allocate error.\n"
          "Causes:\n"
          " * Too big number\n"
          " * Not enought memory in system";
      break;
    case DIVISION_BY_ZERO:
      return
          "Runtime Error: division by zero";
      break;
    case PARSE_ERR_READ_NUM:
      return
          "Expected `$` or `#`";
      break;
    case PARSE_ERR_EQ:
      return
          "Expected `=` after `=`";
      break;
    case PARSE_ERR_COMMAND:
      return
          "Expected `+`, `-`, `*`, `/`, `%`, `==`, `>>`, `<<`, `>=`, `<=`, `&`, `~` or`i` after `;` or in first line";
      break;
    case PARSE_ERR_GREAT:
      return
          "Expected `>` or `=` after `>`";
      break;
    case PARSE_ERR_LESS:
      return
          "Expected `<` or `=` after `<`";
      break;
    case PARSE_ERR_ARROW_FIRST:
      return
          "Expected '->' after register/number";
      break;
    case PARSE_ERR_ARROW_SECOND:
      return
          "Expected '>' after `-`";
      break;
    case PARSE_ERR_NOT_SEMICOLON:
      return
          "Expected `;` after command";
      break;
    case RUNTIME_REGISTER_NOT_SET:
      return
          "Runtime error: register not set";
      break;
    default:
      return "Unknown error";
      break;
  }
  return "";
}
