#include "include/vm.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// 全局变量模拟内存
extern unsigned char vm_mem[MEM_SIZE];

// 连续读取 4 个字节，并拼装成 32 位无符号整数 (小端序)
uint32_t fetch_dword(VM_Context* ctx) {
    uint8_t b0 = fetch_byte(ctx); // 最低字节
    uint8_t b1 = fetch_byte(ctx);
    uint8_t b2 = fetch_byte(ctx);
    uint8_t b3 = fetch_byte(ctx); // 最高字节
    
    return (uint32_t)(b0 | (b1 << 8) | (b2 << 16) | (b3 << 24));
}

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
    // 记录当前pc
    int cur_pc = ctx->pc;

    uint8_t opcode = fetch_byte(ctx);
    if (ctx->state != VM_RUNNING) break;

    // 译码
    switch (opcode) {
      case OP_EXIT: {
        ctx->state = VM_STOP;
        break;
      }
      case OP_ADD: {
        handle_add(ctx);
        break;
      }
      case OP_SUB: {
        handle_sub(ctx);
        break;
      }
      case OP_CMP: {
        handle_cmp(ctx);
        break;
      }
      case OP_JZ: {
        handle_jz(ctx);
        break;
      }
      case OP_PUSH: {
        handle_push(ctx); 
        break;
      }
      case OP_POP: {
        handle_pop(ctx);
        break;
      }
      case OP_LDI: {
        handle_ldi(ctx);
        break;
      }
      default: {
        printf("vm_run: Unknown OPCODE: 0x%02X, pc: %d\n", opcode, cur_pc);
        ctx->state = VM_CRASH;
        break;
      }
    }
  }
  if (ctx->state == VM_STOP) {
    printf("虚拟机正常停机\n");
  } else if (ctx->state == VM_CRASH) {
    printf("虚拟机崩溃\n");
  }
}


