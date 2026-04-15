#include <stdio.h>
#include <unistd.h>
#include "vm.h"
#include "vmdb.h"

// unsigned char vm_mem[MEM_SIZE];
char *vm_mem;

VM_Context* ctx;

int main(int argc, char *argv[]) {
  int opt;
  int g_flag = 0;

  while ((opt = getopt(argc, argv, "g")) != -1) {
    switch (opt) {
      case 'g': {
        g_flag = 1;
        break;
      }
      default: {
        fprintf(stderr, "Usage: %s [-g] BIN\n", argv[0]);
        return 1;
      }
    }
  }
  if (optind > argc) {
    fprintf(stderr, "Expected argument after options\n");
    return 1;
  }

  init_mem();
  // 加载指令文件到内存
  load_binary(argv[optind]);

#ifdef HEX_XOR
  init_and_decrypt_vm();
#endif

  if (g_flag) {
    vmdb_cli(ctx);
  } else {
    vm_run(ctx, MAX_STEP);
  }
  mem_destory(ctx);
}
