// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/4/Fill.asm

// Runs an infinite loop that listens to the keyboard input. 
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel. When no key is pressed, 
// the screen should be cleared.

//// Replace this comment with your code.

// The keyboard memory map is as follows:
(DETECTKEY)
@KBD
D=M
@DESCRIBE_WHITE
D;JEQ
@DESCRIBE_BLACK
0;JMP

(DESCRIBE_WHITE)
@SCREEN
D=A
@R1
A=D+M
M=0
@R1
M=M+1
@8192
D=A
@R1
D=D-M
@RESET_COUNTER
D;JEQ
@DETECTKEY
0;JMP

(DESCRIBE_BLACK)
@SCREEN
D=A
@R1
A=D+M
M=-1
@R1
M=M+1
@8192
D=A
@R1
D=D-M
@RESET_COUNTER
D;JEQ
// 每画一帧就检查一次键盘输入，如果有键被按下，就继续画下一帧，否则就清屏
@DETECTKEY
0;JMP

// 重置计数器R1
(RESET_COUNTER)
@R1
M=0
@DETECTKEY
0;JMP
