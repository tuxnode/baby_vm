import struct
import sys
import os

# ==========================================
# 虚拟机架构配置
# ==========================================

# 1. 指令集映射表
OPCODES = {
    "PUSH": 0x3A,   # PUSH <reg>
    "POP":  0x7C,   # POP <reg>
    "ADD":  0x12,   # ADD (无参数)
    "SUB":  0x9F,   # SUB (无参数)
    "CMP":  0x55,   # CMP (无参数)
    "JZ":   0x4B,   # JZ (无参数，目标地址从栈顶取)
    "LDI":  0x8A,   # LDI <reg>, <imm32>
    "EXIT": 0xFF    # EXIT
}

# 2. 寄存器映射表
REGISTERS = {
    "R0": 0, "R1": 1, "R2": 2, "R3": 3
}

# 3. 加密配置 (与 C 代码保持一致)
XOR_KEY = 0x7B

# ==========================================
# 核心编译逻辑
# ==========================================

def compile_asm(input_file, output_file):
    if not os.path.exists(input_file):
        print(f"[!] 错误: 找不到输入文件 '{input_file}'")
        return

    with open(input_file, 'r', encoding='utf-8') as f:
        lines = f.readlines()

    machine_code = bytearray()
    
    print(f"[*] 开始编译: {input_file}")

    for line_num, line in enumerate(lines, 1):
        # 1. 预处理：去除注释和首尾空格
        original_line = line.strip()
        line = line.split(';')[0].strip()
        if not line:
            continue
            
        # 将逗号替换为空格，方便统一按空格分割参数
        parts = line.replace(',', ' ').split()
        op = parts[0].upper()
        
        # 2. 检查非法指令
        if op not in OPCODES:
            print(f"[!] 编译失败 -> 第 {line_num} 行: 未知指令 '{op}'")
            print(f"    代码: {original_line}")
            sys.exit(1)
            
        # 写入 Opcode
        machine_code.append(OPCODES[op])
        
        # 3. 处理带有参数的指令
        try:
            if op in ["PUSH", "POP"]:
                # 格式: PUSH R0
                reg_name = parts[1].upper()
                if reg_name not in REGISTERS:
                    raise ValueError(f"无效的寄存器 '{reg_name}'")
                machine_code.append(REGISTERS[reg_name])
                
            elif op == "LDI":
                # 格式: LDI R0, 1000
                reg_name = parts[1].upper()
                if reg_name not in REGISTERS:
                    raise ValueError(f"无效的寄存器 '{reg_name}'")
                
                # 支持 10 进制和 16 进制 (0x) 立即数
                imm_value = int(parts[2], 0) 
                
                # 写入 1 字节的寄存器索引
                machine_code.append(REGISTERS[reg_name])
                
                # 将 32 位整数转换为 4 字节的小端序字节数组，并追加
                # '<I' 代表 Little-Endian Unsigned Int
                imm_bytes = struct.pack('<I', imm_value) 
                machine_code.extend(imm_bytes)

            # ADD, SUB, CMP, JZ, EXIT 是零操作数指令，不需要额外处理
                
        except Exception as e:
            print(f"[!] 编译失败 -> 第 {line_num} 行: 语法错误 ({e})")
            print(f"    代码: {original_line}")
            sys.exit(1)

    # 4. 对生成的明文机器码进行异或加密
    encrypted_code = bytearray([b ^ XOR_KEY for b in machine_code])
    
    # 5. 写入二进制文件
    with open(output_file, "wb") as f:
        f.write(encrypted_code)
        
    print(f"[*] 编译成功！")
    print(f"[*] 明文代码长度: {len(machine_code)} 字节")
    print(f"[*] 已应用 XOR 加密 (Key: 0x{XOR_KEY:02X})")
    print(f"[*] 输出文件: {output_file}")

# ==========================================
# 命令行入口
# ==========================================
if __name__ == "__main__":
    if len(sys.argv) == 3:
        input_filename = sys.argv[1]
        output_filename = sys.argv[2]
        compile_asm(input_filename, output_filename)
    else:
        print("用法: python3 assembler.py <输入.asm> <输出.bin>")
        print("示例: python3 assembler.py test.asm challenge.bin")
        
        # 如果没有带参数运行，生成一个默认的测试用例文件方便快速体验
        print("\n[*] 未提供参数，正在生成默认测试文件 'test.asm'...")
        sample_code = """
; 虚拟机综合测试程序
; 逻辑：15 + 20 = 35

LDI R0, 15      ; 将 15 加载到 R0
LDI R1, 20      ; 将 20 加载到 R1

PUSH R0         ; 将 R0 压栈
PUSH R1         ; 将 R1 压栈
ADD             ; 栈顶相加

POP R2          ; 弹出结果(35)到 R2
EXIT            ; 停机
"""
        with open("test.asm", "w", encoding="utf-8") as f:
            f.write(sample_code.strip())
        
        print("[*] 正在编译默认测试文件...")
        compile_asm("test.asm", "challenge.bin")
