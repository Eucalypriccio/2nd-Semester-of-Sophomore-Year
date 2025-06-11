`timescale 1ns/1ps

module Controller_tb;
    reg [5:0] OpCode;
    reg [5:0] Funct;
    wire J;
    wire B;
    wire RegDst;
    wire RegWr;
    wire ALUSrc;
    wire MemWr;
    wire Mem2Reg;
    wire [1:0] ExtendType;
    wire LoadType;
    wire [3:0] ALUCtr;

    reg [5:0] test_opcodes [8:0];
    reg [5:0] test_functs [4:0];
    integer i, j;

    Controller ctrut (
        .OpCode(OpCode),
        .Funct(Funct),
        .J(J),
        .B(B),
        .RegDst(RegDst),
        .RegWr(RegWr),
        .ALUSrc(ALUSrc),
        .MemWr(MemWr),
        .Mem2Reg(Mem2Reg),
        .ExtendType(ExtendType),
        .LoadType(LoadType),
        .ALUCtr(ALUCtr)
    );

    initial begin
        test_opcodes[0] = 6'b000000;
        test_opcodes[1] = 6'b100011;
        test_opcodes[2] = 6'b101011;
        test_opcodes[3] = 6'b000100;
        test_opcodes[4] = 6'b000010;
        test_opcodes[5] = 6'b100000;
        test_opcodes[6] = 6'b100100;
        test_opcodes[7] = 6'b100001;
        test_opcodes[8] = 6'b100101;

        test_functs[0] = 6'b100000;
        test_functs[1] = 6'b100010;
        test_functs[2] = 6'b100100;
        test_functs[3] = 6'b100101;
        test_functs[4] = 6'b101010;

        for (i = 0; i < 9; i = i + 1) begin
            OpCode = test_opcodes[i];
            for (j = 0; j < 5; j = j + 1) begin
                Funct = test_functs[j];
                #1;
            end
        end

        $finish;
    end
endmodule