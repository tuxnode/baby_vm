#include <stdio.h>
#include "include/vm.h"

unsigned char vm_mem[MEM_SIZE];

int main(int argc, char *argv[]) {
  // 加载指令文件到内存
  load_binary(argc, argv);

#ifdef HEX_XOR
  // 对二进制的前几个字节进行混淆
  init_and_decrypt_vm();
#endif

}
