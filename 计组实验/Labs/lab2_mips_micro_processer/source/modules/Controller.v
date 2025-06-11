`timescale 1ns / 1ps
module Controller (OpCode, Funct, J, B, RegDst, RegWr, ALUSrc, 
MemWr, Mem2Reg, ExtendType, LoadType, ALUCtr, Imm_ExtendType, BranchType);
    input [5:0] OpCode;
    input [5:0] Funct;
    output J;
    output B;
    output RegDst;
    output RegWr;
    output ALUSrc;
    output MemWr;
    output Mem2Reg;
    output [1:0] ExtendType;
    output LoadType;
    output Imm_ExtendType;
    output BranchType;
    output [3:0] ALUCtr;

    wire [1:0] ALUOp;

    MainCtr MC (
        .OpCode(OpCode),
        .J(J),
        .B(B),
        .RegDst(RegDst),
        .RegWr(RegWr),
        .ALUSrc(ALUSrc),
        .MemWr(MemWr),
        .Mem2Reg(Mem2Reg),
        .ExtendType(ExtendType),
        .LoadType(LoadType),
        .ALUOp(ALUOp),
        .Imm_ExtendType(Imm_ExtendType),
        .BranchType(BranchType)
    );

    ALUControl AC (
        .ALUOp(ALUOp),
        .Funct(Funct),
        .ALUCtr(ALUCtr)
    );
endmodule