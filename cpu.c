#include "include/vm.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// 全局变量模拟内存
extern unsigned char vm_mem[MEM_SIZE];

uint8_t fetch_byte(VM_Context* ctx) {
  // 检查pc
  if (ctx->pc >= MEM_SIZE) {
    printf("fetch: Invalid pc value, out of memory\n");
    ctx->state = VM_CRASH;
    return 0;
  }

  // 取出字节并pc自增
  uint8_t byte = vm_mem[ctx->pc++];

  return byte;
}

void vm_run(VM_Context* ctx) {
  ctx->state = VM_RUNNING;

  while (ctx->state == VM_RUNNING) {
    uint8_t opcode = fetch_byte(ctx);

    // 译码
    switch (opcode) {
      case OP_EXIT: exit(1);
      case OP_ADD: handle_add(ctx);
      case OP_SUB: handle_sub(ctx);
      case OP_CMP: handle_cmp(ctx);
      case OP_JZ: handle_jz(ctx);
    }
  }
}


