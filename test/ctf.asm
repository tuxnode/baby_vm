_start:
    ; 1. 打印提示信息 "Enter Flag: "
    LDI reg[0], 1
    LDI reg[1], msg_prompt
    ECALL

    ; 2. 读取用户输入到 input_buf (最多读 40 字节)
    LDI reg[0], 3         ; Syscall 3: Read String
    LDI reg[1], input_buf ; 存放地址
    LDI reg[2], 40        ; 最大长度设为 40，适配长 Flag
    ECALL

    ; 3. 初始化对比循环的指针
    LDI reg[1], input_buf ; reg[1] = 用户输入的指针
    LDI reg[2], enc_flag  ; reg[2] = 内存中密文的指针

check_loop:
    ; 读取用户输入的当前字符到 reg[0]
    LDB reg[0], reg[1]
    
    ; 读取密文的当前字符到 reg[3]
    LDB reg[3], reg[2]

    ; 检查是否到达字符串末尾 (读取到 \0 且两者相等则进入最终校验)
    PUSH reg[0]
    PUSH 0
    CMP
    JZ check_win_condition 

    ; === 核心加密对比逻辑 ===
    ; 加密算法：每个字符 ASCII + 1
    PUSH reg[0]
    PUSH 1
    ADD           ; 输入字符 + 1
    POP reg[0]    ; 放回 reg[0]

    ; 开始比对当前字符
    PUSH reg[0]
    PUSH reg[3]
    CMP
    JZ char_match ; 如果这个字符相等，跳转到匹配逻辑继续下一个字符

    ; === 绝对跳转：失败分支 ===
    ; 如果执行到了这里，说明上一步 CMP 不相等
    PUSH 0
    PUSH 0
    CMP           ; 构造 0 == 0，强行让 ZF = 1
    JZ fail_jump  ; 稳稳地跳向失败深渊

char_match:
    ; 指针自增 1，准备检查下一个字符
    PUSH reg[1]
    PUSH 1
    ADD
    POP reg[1]    ; input_buf++

    PUSH reg[2]
    PUSH 1
    ADD
    POP reg[2]    ; enc_flag++

    ; === 绝对跳转：循环 ===
    ; 无条件跳回 check_loop 继续读取下一个字符
    PUSH 0
    PUSH 0
    CMP           
    JZ check_loop

check_win_condition:
    ; 用户输入到头了，看看密文是不是也刚好到头了 (防止用户只输入一半的 payload 绕过)
    PUSH reg[3]
    PUSH 0
    CMP
    JZ win_jump
    
    ; 否则依然失败
    PUSH 0
    PUSH 0
    CMP
    JZ fail_jump

fail_jump:
    ; 打印失败信息
    LDI reg[0], 1
    LDI reg[1], msg_wrong
    ECALL
    EXIT

win_jump:
    ; 打印成功信息
    LDI reg[0], 1
    LDI reg[1], msg_correct
    ECALL
    EXIT

; ====== 数据段 ======
msg_prompt:
    .string "Enter Flag: "
msg_correct:
    .string "Correct! You got the flag.\n"
msg_wrong:
    .string "Wrong flag, try again.\n"

enc_flag:
    ; 存放 "flag{you_are_a_true_vm_master}" 的密文 (ASCII + 1)
    .string "gmbh|zpv`bsf`b`usvf`wn`nbtufs~" 
    
input_buf:
    ; 预留 40 个空白空间存放输入 (空格占位)
    .string "                                        "
