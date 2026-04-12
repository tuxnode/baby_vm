_start:
    ; === 1. 测试基本输出 (SYS_PRINT_STR) ===
    LDI reg[0], 1           ; Syscall ID: 1 (Print String)
    LDI reg[1], msg_hello   ; 传入字符串地址
    ECALL

    ; === 2. 测试栈与算术运算 (计算 50 - 20 = 30) ===
    PUSH 50                 ; 压入被减数 50
    PUSH 20                 ; 压入减数 20
    SUB                     ; 栈顶两元素相减 (50 - 20)，结果 30 压入栈顶
    POP reg[1]              ; 将结果 30 弹出到 reg[1]
    
    LDI reg[0], 2           ; Syscall ID: 2 (Print Integer)
    ECALL                   ; 预期输出: 30

    ; === 3. 测试比较与跳转 (CMP & JZ) ===
    PUSH 100
    PUSH 100
    CMP                     ; 比较 100 和 100，因为相等，虚拟机会将 zf 置为 1
    JZ test_pass            ; 如果 zf == 1，绝对跳转到 test_pass 标签处
    
    ; 如果 JZ 失败（没有跳转），程序会执行到这里
    LDI reg[0], 1
    LDI reg[1], msg_fail
    ECALL
    EXIT

test_pass:
    ; 跳转成功，执行此处的代码
    LDI reg[0], 1
    LDI reg[1], msg_pass
    ECALL
    EXIT

; === 数据段 ===
msg_hello:
    .string "=== Baby VM Comprehensive Test ===\n"
msg_pass:
    .string "SUCCESS: Jump logic works! All tests passed.\n"
msg_fail:
    .string "ERROR: Jump failed! Check your CMP and JZ handlers.\n"
