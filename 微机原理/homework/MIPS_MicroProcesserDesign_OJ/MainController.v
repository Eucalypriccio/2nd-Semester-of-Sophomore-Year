`timescale 1ns / 1ps
module MainCtr (OpCode, J, B, RegDst, RegWr, ALUSrc, MemWr, Mem2Reg, ALUOp);
    input [5:0] OpCode;
    output reg J;
    output reg B;
    output reg RegDst;
    output reg RegWr;
    output reg ALUSrc;
    output reg MemWr;
    output reg Mem2Reg;
    output reg [1:0] ALUOp;

    always @(*) begin
        casex (OpCode)
            6'b000000: 
                begin
                    J = 0;
                    B = 0;
                    RegDst = 1;
                    RegWr = 1;
                    ALUSrc = 0;
                    MemWr = 0;
                    Mem2Reg = 0;
                    ALUOp = 2'b10;
                end
            6'b100011: 
                begin
                    J = 0;
                    B = 0;
                    RegDst = 0;
                    RegWr = 1;
                    ALUSrc = 1;
                    MemWr = 0;
                    Mem2Reg = 1;
                    ALUOp = 2'b00;
                end
            6'b101011:
                begin
                    J = 0;
                    B = 0;
                    RegDst = 1'bx;
                    RegWr = 0;
                    ALUSrc = 1;
                    MemWr = 1;
                    Mem2Reg = 1'bx;
                    ALUOp = 2'b00;
                end
            6'b000100:
                begin
                    J = 0;
                    B = 1;
                    RegDst = 1'bx;
                    RegWr = 0;
                    ALUSrc = 0;
                    MemWr = 0;
                    Mem2Reg = 1'bx;
                    ALUOp = 2'b01;
                end
            6'b000010:
                begin
                    J = 1;
                    B = 0;
                    RegDst = 1'bx;
                    RegWr = 0;
                    ALUSrc = 1'bx;
                    MemWr = 0;
                    Mem2Reg = 1'bx;
                    ALUOp = 2'bxx;
                end
            default: 
                begin
                    J = 0;
                    B = 0;
                    RegDst = 0;
                    RegWr = 0;
                    ALUSrc = 0;
                    MemWr = 0;
                    Mem2Reg = 0;
                    ALUOp = 2'b00;
                end
        endcase
    end
endmodule