#include <stdio.h>
#include "vm.h"

// unsigned char vm_mem[MEM_SIZE];
char *vm_mem;

VM_Context* ctx;

int main(int argc, char *argv[]) {
  init_mem();
  // 加载指令文件到内存
  load_binary(argc, argv);

  init_and_decrypt_vm();

  vm_run(ctx);
}
