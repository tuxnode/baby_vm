; 虚拟机综合测试程序
; 逻辑：15 + 20 = 35

LDI R0, 15      ; 将 15 加载到 R0
LDI R1, 20      ; 将 20 加载到 R1

PUSH R0         ; 将 R0 压栈
PUSH R1         ; 将 R1 压栈
ADD             ; 栈顶相加

POP R2          ; 弹出结果(35)到 R2
EXIT            ; 停机