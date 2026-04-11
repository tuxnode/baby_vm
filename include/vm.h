#ifndef VM_H
#define VM_H
#include <stdint.h>
/*
#define OP_PUSH 0x3A   // 入栈：PUSH <val>
#define OP_POP  0x7C   // 出栈到寄存器：POP <reg>
#define OP_ADD  0x12   // 栈顶两元素相加，结果压栈
#define OP_SUB  0x9F   // 栈顶两元素相减，结果压栈
#define OP_CMP  0x55   // 比较栈顶和次栈顶，设置zf
#define OP_JZ   0x4B   // 若zf==1则跳转：JZ <offset>
#define OP_EXIT 0xFF   // 结束执行
*/

enum OPCODE {
  OP_PUSH = 0x3A,
  OP_POP = 0x7C,
  OP_ADD = 0x12,
  OP_SUB = 0x9F,
  OP_CMP = 0x55,
  OP_JZ = 0x4B,
  OP_LDI = 0x8A,
  OP_EXIT = 0xFF,
};

// Config
// #define HEX_XOR

#define VM_RUNNING 1
#define VM_STOP 2
#define VM_CRASH 3

#define MEM_SIZE 1024 * 64
#define STACK_SIZE 256

typedef struct {
    unsigned int reg[4];              // 四个通用虚拟寄存器
    int pc;                           // 虚拟程序计数器 (Program Counter)，指向当前执行的字节码
    int sp;                           // 虚拟栈指针 (Stack Pointer)
    int zf;                           // 零标志位 (Zero Flag)，用于条件跳转
    unsigned int stack[STACK_SIZE];   // 虚拟机的私有栈
    int state;                        // CPU运行状态
} VM_Context;

void handle_add(VM_Context* ctx);
void handle_sub(VM_Context* ctx);
void handle_cmp(VM_Context* ctx);
void handle_jz(VM_Context* ctx);
void handle_push(VM_Context* ctx);
void handle_pop(VM_Context* ctx);
void handle_ldi(VM_Context* ctx);

// init.c
void load_binary(int argc, char *argv[]);
void init_and_decrypt_vm();

// cpu.c
void vm_run(VM_Context* ctx);
uint8_t fetch_byte(VM_Context* ctx);
uint32_t fetch_dword(VM_Context* ctx);

#endif
