`timescale 1ns / 1ps
module MainCtr (OpCode, J, B, RegDst, RegWr, ALUSrc, MemWr, 
Mem2Reg, ExtendType, LoadType, ALUOp, Imm_ExtendType, BranchType);
    input [5:0] OpCode;
    output J;
    output B;
    output RegDst;
    output RegWr;
    output ALUSrc;
    output MemWr;
    output Mem2Reg;
    output [1:0] ExtendType; // to DigitExtend
    output LoadType;         // to Mux2to1
    output [1:0] ALUOp;
    output Imm_ExtendType; // to Mux2to1 to select signed/unsigned extended Imm
    output BranchType; // to Mux2to1 to select beq/bne

    reg [13:0] ControlSignals;
    assign J = ControlSignals[13];
    assign B = ControlSignals[12];
    assign BranchType = ControlSignals[11];
    assign RegDst = ControlSignals[10];
    assign RegWr = ControlSignals[9];
    assign ALUSrc = ControlSignals[8];
    assign MemWr = ControlSignals[7];
    assign Mem2Reg = ControlSignals[6];
    assign ALUOp = ControlSignals[5:4];
    assign ExtendType = ControlSignals[3:2];
    assign LoadType = ControlSignals[1];
    assign Imm_ExtendType = ControlSignals[0];

    always @(*) begin
        casex (OpCode)
            6'b000000: ControlSignals = 14'b0_0_0_1_1_0_0_0_10_xx_1_0;
            6'b100011: ControlSignals = 14'b0_0_0_0_1_1_0_1_00_xx_1_0;
            6'b101011: ControlSignals = 14'b0_0_0_x_0_1_1_x_00_xx_1_0;
            6'b000100: ControlSignals = 14'b0_1_0_x_0_0_0_x_01_xx_1_0; // beq
            6'b000101: ControlSignals = 14'b0_1_1_x_0_0_0_x_01_xx_1_0; // bne
            6'b000010: ControlSignals = 14'b1_0_0_x_0_x_0_x_xx_xx_1_0;
            6'b100000: ControlSignals = 14'b0_0_0_0_1_1_0_1_00_00_0_0; // lb
            6'b100100: ControlSignals = 14'b0_0_0_0_1_1_0_1_00_01_0_0; // lbu
            6'b100001: ControlSignals = 14'b0_0_0_0_1_1_0_1_00_10_0_0; // lh
            6'b100101: ControlSignals = 14'b0_0_0_0_1_1_0_1_00_11_0_0; // lhu
            6'b001000: ControlSignals = 14'b0_0_0_0_1_1_0_x_00_xx_1_1; // addi
            6'b001101: ControlSignals = 14'b0_0_0_0_1_1_0_x_11_xx_1_1; // ori
            default: ControlSignals = 14'b0_0_0_0_0_0_0_0_00_00_1_0;
        endcase
    end
endmodule