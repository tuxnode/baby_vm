_start:
    ; === 阶段 1：边界逻辑测试（合法的边缘操作） ===
    LDI reg[0], 1
    LDI reg[1], msg_start
    ECALL

    ; 连续压栈与解构，测试栈指针的完美平衡
    PUSH 100
    PUSH 200
    SUB          ; 产生负数 -100
    POP reg[1]
    PUSH reg[1]  ; 测试 PUSHR 指令
    POP reg[2]

    ; === 阶段 2：未知系统调用攻击 ===
    ; 期待结果：虚拟机进入 default 分支，触发 VM_CRASH 安全退出。
    LDI reg[0], 1
    LDI reg[1], msg_ecall
    ECALL

    LDI reg[0], 99  ; 传入不存在的 Syscall ID
    ECALL

    ; [!] 如果你的 ECALL 没有安全退出，代码将继续往下执行，发动更致命的攻击！
    LDI reg[0], 1
    LDI reg[1], msg_survive_1
    ECALL

    ; === 阶段 3：寄存器越界写 (OOB Write) ===
    ; 期待结果：汇编器将 99 编码进指令，C 语言由于没有检查索引，直接覆写宿主机关键内存！
    ; 如果你的防御生效，应当拦截并触发 VM_CRASH。
    LDI reg[0], 1
    LDI reg[1], msg_oob
    ECALL

    ; LDI reg[99], 0xDEADBEEF ; 恶意向 reg[99] 写入脏数据！

    ; [!] 如果依然没有崩溃，进入最终阶段
    LDI reg[0], 1
    LDI reg[1], msg_survive_2
    ECALL

    ; === 阶段 4：栈溢出爆破 (Stack Overflow DoS) ===
    ; 期待结果：触发 SP >= STACK_SIZE 检查并安全退出。如果漏查，将吃光宿主机的堆内存并 Segfault。
    LDI reg[0], 1
    LDI reg[1], msg_dos
    ECALL

loop_boom:
    PUSH 0xBADF00D  ; 恶意死循环疯狂压栈
    PUSH 0
    PUSH 0
    CMP             ; 构造 0==0
    JZ loop_boom    ; 绝对跳转，形成死循环

safe_exit:
    EXIT

; ====== 数据段 ======
msg_start:
    .string "[+] Phase 1: Logic & Math boundaries passed.\n"
msg_ecall:
    .string "[+] Phase 2: Testing Invalid Syscall (Should trigger safe VM_CRASH)...\n"
msg_survive_1:
    .string "[!] WARNING: VM failed to handle unknown ECALL. Proceeding to OOB Attack...\n"
msg_oob:
    .string "[+] Phase 3: Testing Register OOB Write (If Segfault -> You LOSE!)...\n"
msg_survive_2:
    .string "[!] WARNING: OOB failed to crash VM. Proceeding to Stack Overflow...\n"
msg_dos:
    .string "[+] Phase 4: Launching Infinite Stack Overflow DoS Attack...\n"
