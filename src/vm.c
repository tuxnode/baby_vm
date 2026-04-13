#include "vm.h"
#include <stdio.h>
#include <stdlib.h>

extern char *vm_mem;

void handle_add(VM_Context* ctx) {
  if (ctx->sp < 2) { ctx->state = VM_CRASH; return; }
  int right_op = ctx->stack[--ctx->sp];
  int left_op = ctx->stack[--ctx->sp];
  ctx->stack[ctx->sp++] = left_op + right_op;
}

void handle_sub(VM_Context* ctx) {
  if (ctx->sp < 2) { ctx->state = VM_CRASH; return; }
  int right_op = ctx->stack[--ctx->sp];
  int left_op = ctx->stack[--ctx->sp];
  // 必须是 left - right
  ctx->stack[ctx->sp++] = left_op - right_op; 
}

void handle_cmp(VM_Context* ctx) {
  // 先出栈是右操作数
  // 后出栈是左操作数
  if (ctx->sp < 2) {
    printf("handle_cmp: Stack fault\n");
    ctx->state = VM_CRASH;
    return;
  }
  int right_op = ctx->stack[--ctx->sp];
  int left_op = ctx->stack[--ctx->sp];

  if (right_op == left_op) ctx->zf = 1;
  else ctx->zf = 0;
}

void handle_jz(VM_Context* ctx) {
  uint32_t target_addr = fetch_dword(ctx);
  if (ctx->zf == 1) {
    ctx->pc = target_addr;
  }
}

void handle_push(VM_Context* ctx) {
  // 检查栈溢出
  if (ctx->sp >= STACK_SIZE || ctx->sp < 0) {
    printf("handle_push: STACK OVERFLOW\n");
    ctx->state = VM_CRASH;
    return;
  }
  
  // 加载立即数
  uint32_t imm_val = fetch_dword(ctx); 
  ctx->stack[ctx->sp++] = imm_val;
}

void handle_pop(VM_Context* ctx) {
  // 检查栈下溢
  if (ctx->sp <= 0 || ctx->sp >= STACK_SIZE) {
    printf("handle_pop: STACK UNDERFLOW\n");
    ctx->state = VM_CRASH;
    return;
  }

  unsigned int reg_idx = (unsigned int) fetch_byte(ctx);
  if (reg_idx >= 4) {
    printf("VM Crash: handle_pop Invalid Register\n");
    ctx->state = VM_CRASH;
    return;
  }
  int popped_value = ctx->stack[--ctx->sp];
  ctx->reg[reg_idx] = popped_value;
}

void handle_ldi(VM_Context* ctx) {
  // 获取寄存器信息
  unsigned int reg_idx = (unsigned int) fetch_byte(ctx);
  if (reg_idx >= 4) {
    printf("VM Crash: handle_ldi Invalid Register\n");
    ctx->state = VM_CRASH;
    return;
  }
  // 获取数值
  uint32_t inum = fetch_dword(ctx);
  ctx->reg[reg_idx] = inum;
}

/*
 * reg[0]作为系统调用号的寄存器，
* reg[1]作为string地址的参数寄存器
*/

void handle_ecall(VM_Context* ctx) {
  int ecall_num = ctx->reg[0];

  switch (ecall_num) {
    case SYS_PRINT_STR: {
      sys_print_str(ctx);
      break;
    }
    case SYS_PRINT_INT: {
      sys_print_int(ctx);
      break;
    }
    case SYS_READ_STR: {
      sys_read_str(ctx);
      break;
    }
    default: {
      printf("Unknow ecall number\n");
      ctx->state = VM_CRASH;
    }
  }
}

void handle_ldb(VM_Context* ctx) {
  // 读取目的寄存器和源寄存器的索引 (各占 1 字节)
  unsigned int dst_reg = (unsigned int)fetch_byte(ctx);
  unsigned int src_reg = (unsigned int)fetch_byte(ctx);

  if (dst_reg >= 4 || src_reg >= 4) {
    ctx->state = VM_CRASH;
    return;
  }

  // 获取内存地址
  int addr = ctx->reg[src_reg];
  if (addr < 0 || addr >= MEM_SIZE) {
    ctx->state = VM_CRASH;
    return;
  }

  // printf("[DEBUG] LDB: 从地址 %d 读取了字符 '%c' (ASCII: %d)\n", addr, ctx->mem[addr], ctx->mem[addr]);
  // 读取 1 字节数据，并扩展为 32 位存入寄存器
  ctx->reg[dst_reg] = (uint32_t)ctx->mem[addr];
}

void handle_pushr(VM_Context* ctx) {
  if (ctx->sp >= STACK_SIZE) {
    printf("VM Crash: STACK OVERFLOW in PUSHR\n");
    ctx->state = VM_CRASH;
    return;
  }
  
  // 寄存器编号只有 1 个字节
  unsigned int reg_idx = (unsigned int) fetch_byte(ctx);
  if (reg_idx >= 4) {
    ctx->state = VM_CRASH;
    return;
  }
  
  // 将指定寄存器的值压入栈中
  ctx->stack[ctx->sp++] = ctx->reg[reg_idx];
}
