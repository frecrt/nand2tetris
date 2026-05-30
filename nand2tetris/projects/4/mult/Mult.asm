// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/4/Mult.asm

// Multiplies R0 and R1 and stores the result in R2.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)
// The algorithm is based on repetitive addition.

//// Replace this comment with your code.


@R2
M=0 // R2 = 0

(LOOP)
@R1
D=M
@END
D;JEQ  // while (R1 != 0)

@R0
D=M // D = R0

@R2
M = M + D // R2 = R2 + R0

@R1
D = M // D = R1
M = D - 1 // R1 = R1 - 1


@LOOP
0;JMP

(END)
@END
0;JMP
