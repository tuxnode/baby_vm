#include "vm.h"
#include <stdio.h>
#include <stdlib.h>

void handle_add(VM_Context* ctx) {
  int a = ctx->stack[--ctx->sp];
  int b = ctx->stack[--ctx->sp];
  ctx->stack[ctx->sp++] = a + b;
}

void handle_sub(VM_Context* ctx) {
  int a = ctx->stack[--ctx->sp];
  int b = ctx->stack[--ctx->sp];
  ctx->stack[ctx->sp++] = a - b;
}

void handle_cmp(VM_Context* ctx) {
  // 先出栈是右操作数
  // 后出栈是左操作数
  int right_op = ctx->stack[--ctx->sp];
  int left_op = ctx->stack[--ctx->sp];

  if (right_op == left_op) ctx->zf = 1;
  else ctx->zf = 0;
}

void handle_jz(VM_Context* ctx) {
  int target = ctx->stack[--ctx->sp];
  if (ctx->zf == 1) {
    ctx->pc = target;
  }
}

void handle_push(VM_Context* ctx) {
  unsigned int arg = (unsigned int) fetch_byte(ctx);
  if (arg >= 4) {
    ctx->state = VM_CRASH;
    return;
  }
  ctx->stack[ctx->sp++] = ctx->reg[(int) arg];
}

void handle_pop(VM_Context* ctx) {
  // 安全溢出检查
  if(ctx->sp == 0) {
    printf("handle_pop: sp is already ZERO\n");
    ctx->state = VM_CRASH;
    return;
  }

  // 获取需要存入哪个寄存器
  unsigned int reg_idx = (unsigned int) fetch_byte(ctx);
  if (reg_idx >= 4) {
    printf("handle_pop: Invalid Arg\n");
    ctx->state = VM_CRASH;
    return;
  }

  int popped_value = ctx->stack[--ctx->sp];
  ctx->reg[reg_idx] = popped_value;
}

void handle_ldi(VM_Context* ctx) {
  // 获取寄存器信息
  unsigned int reg_idx = (unsigned int) fetch_byte(ctx);
  if (reg_idx > 4) {
    printf("handle_push: Invalid Arg\n");
    ctx->state = VM_CRASH;
    return;
  }
  // 获取数值
  uint32_t inum = fetch_dword(ctx);
  ctx->reg[reg_idx] = inum;
}
