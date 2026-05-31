# Nand2Tetris 学习项目

> **从 Nand 到 Tetris** — 构建一台完整的计算机
> 
> 基于《计算机系统要素》（The Elements of Computing Systems）与 nand2tetris.org 课程

## 目录结构

```
nand2tetris/
├── projects/           # 13 个项目
│   ├── 00/             # 项目 0：入门（提交练习）
│   ├── 01/             # 项目 1：基本逻辑门
│   ├── 02/             # 项目 2：算术逻辑单元（ALU）
│   ├── 03/             # 项目 3：时序逻辑（寄存器、RAM、PC）
│   ├── 04/             # 项目 4：机器语言编程
│   ├── 05/             # 项目 5：计算机体系结构（CPU + Memory + Computer）
│   ├── 06/             # 项目 6：汇编器（Hack Assembler）
│   ├── 07/             # 项目 7：VM 译码器 I（堆栈运算）
│   ├── 08/             # 项目 8：VM 译码器 II（程序流与函数调用）
│   ├── 09/             # 项目 9：高级语言编程（Jack）
│   ├── 10/             # 项目 10：Jack 编译器 I（语法分析）
│   ├── 11/             # 项目 11：Jack 编译器 II（代码生成）
│   ├── 12/             # 项目 12：操作系统
│   └── 13/             # 项目 13：更多乐趣（终章）
│
└── tools/              # 官方工具集
    ├── Assembler.sh    # 汇编器（Java GUI）
    ├── CPUEmulator.sh  # CPU 模拟器
    ├── HardwareSimulator.sh  # 硬件模拟器
    ├── JackCompiler.sh # Jack 编译器
    ├── VMEmulator.sh   # VM 模拟器
    └── TextComparer.sh # 文本比较工具
```

## 已完成的实现

### 项目 1 — 基本逻辑门

| 芯片 | 描述 |
|------|------|
| `Not.hdl` | 非门 |
| `And.hdl` | 与门 |
| `Or.hdl` | 或门 |
| `Xor.hdl` | 异或门 |
| `Mux.hdl` | 数据选择器 |
| `DMux.hdl` | 数据分配器 |
| `Not16.hdl` | 16 位非门 |
| `And16.hdl` | 16 位与门 |
| `Or16.hdl` | 16 位或门 |
| `Mux16.hdl` | 16 位数据选择器 |
| `Or8Way.hdl` | 8 路或门 |
| `Mux4Way16.hdl` | 4 路 16 位选择器 |
| `Mux8Way16.hdl` | 8 路 16 位选择器 |
| `DMux4Way.hdl` | 4 路分配器 |
| `DMux8Way.hdl` | 8 路分配器 |

### 项目 2 — 算术逻辑单元

| 芯片 | 描述 |
|------|------|
| `HalfAdder.hdl` | 半加器 |
| `FullAdder.hdl` | 全加器 |
| `Add16.hdl` | 16 位加法器 |
| `Inc16.hdl` | 16 位递增器 |
| `ALU.hdl` | **算术逻辑单元**（核心部件） |

### 项目 5 — 计算机体系结构

| 芯片 | 描述 |
|------|------|
| `Memory.hdl` | 内存（RAM + Screen + Keyboard） |
| `CPU.hdl` | **中央处理单元** |
| `Computer.hdl` | 完整计算机（CPU + ROM + RAM + I/O） |

### 项目 6 — 汇编器

`projects/6/HackAssembler.cpp` / `.h`

用 C++ 实现的 **Hack 汇编器**，将 `.asm` 汇编代码翻译为 `.hack` 机器码。

**功能：**
- 支持 A 指令（`@value` / `@symbol`）
- 支持 C 指令（`dest=comp;jump`）
- 支持标签声明（`(LOOP)`）
- 支持预定义符号（`R0`-`R15`, `SCREEN`, `KBD` 等）
- 自动变量分配（从 RAM[16] 开始）
- 完整的注释和空白处理

**已验证的测试程序：**
| 程序 | 代码行数 | 状态 |
|------|---------|------|
| `add/Add.asm` | 6 | ✅ 匹配参考实现 |
| `max/Max.asm` | 16 | ✅ 匹配参考实现 |
| `max/MaxL.asm` | 16 | ✅ 匹配参考实现 |
| `rect/Rect.asm` | 25 | ✅ 匹配参考实现 |
| `rect/RectL.asm` | 25 | ✅ |
| `pong/Pong.asm` | 27,483 | ✅ |
| `pong/PongL.asm` | 27,483 | ✅ |

---

## 使用指南

### 汇编器（项目 6）

```bash
cd projects/6

# 编译汇编器
g++ -std=c++11 -o HackAssembler HackAssembler.cpp

# 汇编程序
./HackAssembler myProgram.asm          # 自动生成 myProgram.hack
./HackAssembler input.asm output.hack  # 指定输出文件名
```

### 硬件模拟器（HDL 测试）

```bash
# 打开硬件模拟器 GUI，加载 .tst 测试脚本
sh tools/HardwareSimulator.sh
```

### CPU 模拟器（运行 .hack 程序）

```bash
# 打开 CPU 模拟器 GUI
sh tools/CPUEmulator.sh
```

### 文本比较工具（验证输出）

```bash
sh tools/TextComparer.sh file1.txt file2.txt
```

所有工具需要 **Java 运行时** 环境。

---

## 学习路线

```
项目 0: 入门
  ↓
项目 1: 布尔逻辑（基本门电路）
  ↓
项目 2: 布尔算术（ALU）
  ↓
项目 3: 存储器（寄存器、RAM、PC）
  ↓
项目 4: 机器语言（汇编编程基础）
  ↓
项目 5: 计算机架构（CPU + 内存 + I/O）
  ↓
项目 6: 汇编器（asm → 机器码）
  ↓
项目 7-8: 虚拟机（Stack VM）
  ↓
项目 9: 高级语言（Jack）
  ↓
项目 10-11: 编译器（Jack → VM）
  ↓
项目 12: 操作系统
```

每完成一层，你就在从零搭建一台完整的计算机！
