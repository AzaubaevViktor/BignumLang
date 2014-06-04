#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#define STATEMACHINE_COMPATIBILITY "0.0.3a"

#include <stdio.h>
#include <inttypes.h>
#include "parser.h"
#include "regs.h"

typedef struct _State {
  Regs *regs;
  Program *prg;
  uint64_t cs;
  uint64_t op;
} State;

Reterr machineInitState(State **state);
void machineFreeState(State *state);
Reterr machine(State *state);
void printState(State *state);

#endif // STATEMACHINE_H
