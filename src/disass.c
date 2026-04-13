#include <string.h>
#include <stdio.h>
#include "vm.h"

int disassemble_instruction(VM_Context *ctx, int addr, char *buffer) {
    uint8_t opcode = ctx->mem[addr];
    int len = 1; // 默认指令长度为 1

    switch (opcode) {
        case OP_ADD:   sprintf(buffer, "ADD"); break;
        case OP_SUB:   sprintf(buffer, "SUB"); break;
        case OP_CMP:   sprintf(buffer, "CMP"); break;
        case OP_ECALL: sprintf(buffer, "ECALL"); break;
        case OP_EXIT:  sprintf(buffer, "EXIT"); break;
        
        case OP_PUSH: {
            // PUSH <dword> (1 + 4 = 5 bytes)
            uint32_t imm = *(uint32_t*)&ctx->mem[addr + 1];
            sprintf(buffer, "PUSH 0x%X", imm);
            len = 5;
            break;
        }
        case OP_PUSHR: {
            // PUSHR <reg> (1 + 1 = 2 bytes)
            uint8_t reg = ctx->mem[addr + 1];
            sprintf(buffer, "PUSHR R%d", reg);
            len = 2;
            break;
        }
        case OP_POP: {
            // POP <reg> (1 + 1 = 2 bytes)
            uint8_t reg = ctx->mem[addr + 1];
            sprintf(buffer, "POP R%d", reg);
            len = 2;
            break;
        }
        case OP_LDI: {
            // LDI <reg> <dword> (1 + 1 + 4 = 6 bytes)
            uint8_t reg = ctx->mem[addr + 1];
            uint32_t imm = *(uint32_t*)&ctx->mem[addr + 2];
            sprintf(buffer, "LDI R%d, 0x%X", reg, imm);
            len = 6;
            break;
        }
        case OP_JZ: {
            // JZ <target> (1 + 4 = 5 bytes)
            uint32_t target = *(uint32_t*)&ctx->mem[addr + 1];
            sprintf(buffer, "JZ 0x%04X", target);
            len = 5;
            break;
        }
        case OP_LDB: {
            // LDB <dst_reg> <src_reg> (1 + 1 + 1 = 3 bytes)
            uint8_t dst = ctx->mem[addr + 1];
            uint8_t src = ctx->mem[addr + 2];
            sprintf(buffer, "LDB R%d, [R%d]", dst, src);
            len = 3;
            break;
        }
        default:
            sprintf(buffer, "UNKNOWN (0x%02X)", opcode);
            len = 1;
            break;
    }
    return len;
}
