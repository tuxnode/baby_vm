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
