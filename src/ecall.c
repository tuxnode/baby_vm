#include "vm.h"
#include <stdio.h>

extern char *vm_mem;

void sys_print_str(VM_Context* ctx) {
  int str_addr = ctx->reg[1];

  if (str_addr < 0 || str_addr >= MEM_SIZE) {
    printf("ECALL Error: Invalid string address %d 0x%08X\n",str_addr,  str_addr);
    ctx->state = VM_CRASH;
    return;
  }
  printf("%s\n", &vm_mem[str_addr]);
  fflush(stdout);
}

void sys_print_int(VM_Context* ctx) {
  int int_addr = ctx->reg[1];

  printf("%d\n", int_addr);
  fflush(stdout);
}

void sys_read_str(VM_Context* ctx) {
  int str_addr = ctx->reg[1];
  int max_len = ctx->reg[2];

  // 1. 严格的安全边界检查
  if (str_addr < 0 || str_addr + max_len >= MEM_SIZE) {
    printf("VM Crash: ECALL buffer overflow protection triggered!\n");
    ctx->state = VM_CRASH;
    return;
  }

  // 2. 从标准输入读取字符串到虚拟机的内存中
  if (fgets((char*)&ctx->mem[str_addr], max_len, stdin) != NULL) {
    // 消除回车换行符（fgets 会把回车也读进去，影响 Flag 对比）
    for (int i = 0; i < max_len; i++) {
      if (ctx->mem[str_addr + i] == '\n' || ctx->mem[str_addr + i] == '\r') {
        ctx->mem[str_addr + i] = '\0';
        break;
      }
    }
  }
}
