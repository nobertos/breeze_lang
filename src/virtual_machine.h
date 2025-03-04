#ifndef breeze_virtual_machine_h
#define breeze_virtual_machine_h

#include <stdint.h>
#include <stdio.h>

#include "common.h"
#include "object.h"
#include "table.h"
#include "value.h"

#define FRAMES_MAX 64
#define STACK_MAX (FRAMES_MAX * UINT8_COUNT)

typedef struct {
  ObjClosure *closure;
  uint8_t *inst_ptr;
  Value *frame_ptr;
} CallFrame;

typedef struct {
  CallFrame frames[FRAMES_MAX];
  uint32_t frames_len;
  Value stack[STACK_MAX];
  Value *stack_ptr;
  Table globals;
  Table strings;
  ObjUpvalue *open_upvalues;

  size_t bytes_allocated;
  size_t next_gc;
  Obj *objects;

  uint32_t gray_stack_len;
  uint32_t gray_stack_capacity;
  Obj **gray_stack;
} VirtualMachine;

typedef enum {
  InterpretOk,
  InterpretCompileErr,
  InterpretRuntimeErr,
} InterpretResult;

extern VirtualMachine vm;

void init_vm();
void free_vm();
InterpretResult interpret(const char *source);
void push_stack(Value value);
Value pop_stack();

#endif // !breeze_virtual_machine_h
