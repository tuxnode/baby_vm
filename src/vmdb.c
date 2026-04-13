#include "vm.h"
#include "vmdb.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct CMD {
  const char *name;
  const char *description;
  void (*handler) (char *, VM_Context* ctx);
} cmd_table[] = {
  {"help", "Print VMDB usage\n", cmd_help},
  {"si", "Step pc\n", cmd_si},
  {NULL, NULL, NULL}
};

void print_prompt() {
  printf(COLOR_GREEN "gemini-vm" COLOR_RESET ":" COLOR_BLUE "~" COLOR_RESET "$ ");
  fflush(stdout);
}

void execute_command(char *line, VM_Context* ctx) {
  char *cmd_name = strtok(line, " ");
  if (cmd_name == NULL) return;

  char *args = strtok(NULL, "");

  // 查找命令表
  for (int i = 0; cmd_table[i].name != NULL; i++) {
    if (strcmp(cmd_name, cmd_table[i].name) == 0) {
      cmd_table[i].handler(args, ctx);
      return;
    }
  }
}

void vmdb_cli(VM_Context* ctx) {
  char input[256];

  printf(CLEAR_SCREEN);
  printf("==========================================\n");
  printf("   Virtual Machine Debugger Shell v1.0    \n");
  printf("   Type 'help' to see available commands. \n");
  printf("==========================================\n\n");

  while (true) {
    print_prompt();

    if (!fgets(input, sizeof(input), stdin)) break;

    // 去掉换行符
    input[strcspn(input, "\n")] = 0;

    if (strlen(input) == 0) continue;

    if (strcmp(input, "exit") == 0) {
        break;
    }

    execute_command(input, ctx);
  }
  return;
}

static bool check_state(VM_Context* ctx) {
  if (ctx->state == VM_CRASH || ctx->state == VM_STOP) {
    return false;
  }
  return true;
}

void cmd_help(char *args, VM_Context* ctx) {
  if (args != NULL && args[0] != '\0') {
    printf("Usage: help\n");
    return;
  }

  check_state(ctx);

  printf("Available commands:\n");

  for (int i = 0; cmd_table[i].name != NULL; i++) {
    printf("  %-8s : %s\n", cmd_table[i].name, cmd_table[i].description);
  }
}

void cmd_si(char *args, VM_Context* ctx) {
  if (args != NULL) {
    printf("Usage: si don't need args\n");
    return;
  }
  check_state(ctx);

  char disasm_buf[64];
  int cur_pc = ctx->pc;
  int step = 1;
  ctx->is_vmdb = true;

  // 反汇编
  disassemble_instruction(ctx, cur_pc, disasm_buf);
  if (ctx->is_vmdb) {
    printf(COLOR_RED " [ASM] 0x%04X: %-15s " COLOR_RESET, cur_pc, disasm_buf);
  }

  vm_run(ctx, step);
  show_info(ctx);

}

