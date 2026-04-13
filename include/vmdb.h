#ifndef VMDB_H
#define VMDB_H

#include <stdio.h>
#include "vm.h"

// 颜色宏
#define COLOR_GREEN  "\033[1;32m"
#define COLOR_BLUE   "\033[1;34m"
#define COLOR_RESET  "\033[0m"
#define CLEAR_SCREEN "\033[2J\033[H"

void print_prompt();
int vmdb_cli(VM_Context*);
void execute_command(char *line, VM_Context*);

void cmd_help(char *args, VM_Context* ctx);
void cmd_si(char *args, VM_Context* ctx);

#endif
