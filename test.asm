_start:
    LDI reg[0], 1       ; Syscall ID: Print String
    LDI reg[1], my_str  ; 字符串的地址
    ECALL
    EXIT

my_str:
    .string "Hello from external file!\n"
