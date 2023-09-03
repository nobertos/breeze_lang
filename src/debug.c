#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "chunk.h"
#include "debug.h"
#include "value.h"


static uint32_t simple_inst(const char* name, uint32_t offset) {
  printf("%s\n", name);
  return offset + 1;
}

static uint32_t constant_inst(const char* name, Chunk *chunk, uint32_t offset) {
  uint8_t constant_idx = chunk->code[offset + 1];
  printf("%-16s %4d '", name, constant_idx);
  print_value(chunk->constants.values[constant_idx]);
  printf("'\n");
  return offset + 2;
}

static uint32_t constant_long_inst(const char* name, Chunk *chunk, uint32_t offset) {
  uint32_t constant_long_idx = chunk->code[offset + 1] |
                               (chunk->code[offset + 2] << 8) |
                               (chunk->code[offset + 3] << 16);

  printf("%-16s %4d '", name, constant_long_idx);
  print_value(chunk->constants.values[constant_long_idx]);
  printf("\n");
  return offset + 4;
}

uint32_t disassemble_inst(Chunk* chunk, uint32_t offset) {
  printf("%04d ", offset);
  uint32_t curr_line = get_line(chunk->lines, offset);
  uint32_t prev_line = offset > 0 ? get_line(chunk->lines, offset - 1) : 0;
  if (offset > 0 && curr_line == prev_line) {
    printf("    | ");
  } else {
    printf("%4d ", curr_line);
  }

  uint8_t inst = chunk->code[offset];
  switch (inst) {
  case OpRet:
    return simple_inst("OpRet", offset);
  case OpConst: {
    return constant_inst("OpConst", chunk, offset);
  }
  case OpConstLong: {
    return constant_long_inst("OpConstLong", chunk, offset);
  }
  case OpNeg: {
    return simple_inst("OpNeg", offset);
  }

  default:
    printf("Unknown opcode %d\n", inst);
    return offset + 1;
  }
}

void disassemble_chunk(Chunk* chunk, const char *name) {
  printf("== %s ==\n\n", name);

  for (uint32_t offset = 0; offset < chunk->len;) {
    offset = disassemble_inst(chunk, offset);
  }
  printf("\n===");
  for (uint8_t i=0; i<strlen(name); i+=1) {
   printf("=");
  }
  printf("===\n");
}
