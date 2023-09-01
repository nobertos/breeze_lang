#include <stdio.h>

#include "chunk.h"
#include "debug.h"
#include "value.h"

void disassemble_chunk(Chunk* chunk, const char* name) {
  printf("== %s ==\n", name);

  for (uint32_t offset = 0; offset < chunk->count;) {
    offset = disassemble_inst(chunk, offset);
  }
}
uint32_t simple_inst(const char* name, uint32_t offset) {
  printf("%s\n", name);
  return offset+1;
}

uint32_t constant_inst(const char* name, Chunk* chunk, uint32_t offset) {
  uint8_t constant = chunk->code[offset+1];
  printf("%-16s %4d '", name, constant);
  print_value(chunk->constants.values[constant]);
  printf("'\n");
  return offset + 2;
}

uint32_t disassemble_inst(Chunk *chunk, uint32_t offset) {
  printf("%04d ", offset);
  uint32_t curr_line = get_line(chunk->lines, offset);
  uint32_t prev_line = offset > 0 ? get_line(chunk->lines, offset-1) : 0;
  if (offset > 0 && curr_line == prev_line) {
    printf("    | ");
  } else {
    printf("%4d ", curr_line); 
  }

  uint8_t inst = chunk->code[offset];
  switch (inst) {
    case OpReturn:
      return simple_inst("OpReturn", offset) ;
    case OpConstant: {
      return constant_inst("OpConstant", chunk, offset);
    }
    default:
      printf("Unknown opcode %d\n", inst);
      return offset+1;
  }
}

