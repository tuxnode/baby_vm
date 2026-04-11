#include "include/vm.h"


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
  uint8_t arg = fetch_byte(ctx);
  ctx->stack[ctx->sp++] = arg;
}
