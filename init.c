#include "include/vm.h"
#include <stdlib.h>
#include <stdio.h>

extern unsigned char vm_mem[MEM_SIZE];

const unsigned char encrypted_code[] = {0x60, 0x50, 0x60, 0x4E, 0x48, 0x26, 0x5A, 0xA5};
const unsigned char xor_key = 0x7B;

static int code_len = 0;

void load_binary(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Usage: vm BIN\n");
    exit(1);
  }

  const char *filename = argv[1];
  
  FILE *fp = fopen(filename, "rb");
  if (fp == NULL) {
    printf("load_binary: fopen faild\n");
    exit(1);
  }

  fseek(fp, 0, SEEK_END);      // 将文件指针移动到文件末尾
  long file_size = ftell(fp);  // 获取当前指针位置（即文件总字节数）
  fseek(fp, 0, SEEK_SET);      // 将文件指针重新移回文件开头，准备读取

  // 安全检查
  if (file_size > MEM_SIZE) {
    printf("load_binary: Out of Virtual Memory\n");
    fclose(fp);
    exit(1);
  }

  long read_size = (long) fread(vm_mem, 1, file_size, fp);
  if (read_size != file_size) {
    printf("load_binary: Read file size failt\n");
    fclose(fp);
    exit(1);
  }

  code_len = (int) file_size;
  fclose(fp);
  printf("Success load the BIN file\n");
}

void init_and_decrypt_vm() {
  int code_len = sizeof(encrypted_code);

  for (int i = 0; i < code_len; i++) {
    vm_mem[i] = encrypted_code[i] ^ xor_key;
  }
}
