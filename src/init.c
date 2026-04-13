#include "vm.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// extern unsigned char vm_mem[MEM_SIZE];
extern char* vm_mem;
extern VM_Context* ctx;


const unsigned char encrypted_code[] = {0x60, 0x50, 0x60, 0x4E, 0x48, 0x26, 0x5A, 0xA5};
const unsigned char xor_key = 0x7B;

static int code_len = 0;

static int check_null(void *ptr) {
  if (ptr == NULL) {
    printf("Malloc Faild\n");
    return 1;
  }
  return 0;
}

void init_mem() {
  VM_Context *m_ctx = calloc(1, sizeof(VM_Context));
  if (check_null(m_ctx) != 0) return;
  ctx = m_ctx;

  // char* mem = malloc(MEM_SIZE);
  // if (check_null(mem) != 0) {
  //   free(ctx);
  //   return;
  // }
  vm_mem = (char *) &ctx->mem[0];

  memset(ctx->mem, 0, sizeof(ctx->mem));
  memset(ctx->reg, 0, sizeof(ctx->reg));
}

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
  for (int i = 0; i < code_len; i++) {
    vm_mem[i] = vm_mem[i] ^ xor_key;
  }
printf("[*] 成功在内存中原地解密了 %d 字节的指令。\n", code_len);
}

void dump_stack(VM_Context* ctx) {
  printf("STACK INFO\n");
  printf("SP: %d / %d\n", ctx->sp, STACK_SIZE);

  if (ctx->sp <= 0) {
    printf("[ Empty Stack ]\n");
  } else {
    for (int i = ctx->sp - 1; i >= 0; i--) {
      printf("  [%03d]: 0x%08X (%u)", i, ctx->stack[i], ctx->stack[i]);
      if (i == ctx->sp - 1) {
        printf(" <--- TOP");
      }
      printf("\n");
    }
  }
  printf("STACK END\n");
}
