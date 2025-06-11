`timescale 1ns / 1ps
module MIPSCPU (Clk, Reset);
    input Clk;
    input Reset;

    wire [31:0] Instr;
    wire [31:0] CurrentPC;
    wire J;
    wire B;
    wire RegDst;
    wire RegWr;
    wire ALUSrc;
    wire MemWr;
    wire Mem2Reg;
    wire [3:0] ALUCtr;
    wire Zero;
    wire Zero_Final;
    wire [1:0] Sel;
    wire [31:0] RsData;
    wire [31:0] RtData;
    wire [31:0] Res;
    wire [31:0] MemDataOut;
    wire [31:0] ExtendedDataOut;
    wire [31:0] FinalDataOut;
    wire [31:0] Imm32_Signed;
    wire [31:0] Imm32_Final;
    wire [31:0] ALUSrc2;
    wire [31:0] RegDataIn;
    wire [31:0] SeqPC;
    wire [31:0] BranPC;
    wire [31:0] JumpPC;
    wire [31:0] NextPC;
    wire [31:0] Imm32_Signed_offset;
    wire [27:0] Jmp_low_order;
    wire [4:0] WrAddr;
    wire [1:0] ExtendType;
    wire LoadType;
    wire Imm_ExtendType;
    wire BranchType;

    DataRAM RAM_U3 (
        .Addr(Res[6:2]),
        .DataIn(RtData),
        .MemWr(MemWr),
        .Clk(~Clk),
        .DataOut(MemDataOut)
    );

    InstrROM ROM_U0 (
        .Addr(CurrentPC[6:2]),
        .Clk(Clk),
        .Instr(Instr)
    );

    RegFile RegFile_U1 (
        .RsAddr(Instr[25:21]),
        .RtAddr(Instr[20:16]),
        .WrAddr(WrAddr),
        .DataIn(RegDataIn),
        .RegWr(RegWr),
        .Clk(~Clk),
        .RsData(RsData),
        .RtData(RtData)
    );

    ALU A (
        .In1(RsData),
        .In2(ALUSrc2),
        .ALUCtr(ALUCtr),
        .Res(Res),
        .Zero(Zero)
    );

    Controller CTR (
        .OpCode(Instr[31:26]),
        .Funct(Instr[5:0]),
        .J(J),
        .B(B),
        .RegDst(RegDst),
        .RegWr(RegWr),
        .ALUSrc(ALUSrc),
        .MemWr(MemWr),
        .Mem2Reg(Mem2Reg),
        .ExtendType(ExtendType),
        .LoadType(LoadType),
        .ALUCtr(ALUCtr),
        .Imm_ExtendType(Imm_ExtendType),
        .BranchType(BranchType)
    );

    DigitExtend DE (
        .ByteSelect(Res[1:0]),
        .ExtendType(ExtendType),
        .DataIn(MemDataOut),
        .DataOut(ExtendedDataOut)
    );

    SignedExtend SE (
        .In(Instr[15:0]),
        .Out(Imm32_Signed)
    );

    Mux2_1 M1 (
        .In1(RtData),
        .In2(Imm32_Final),
        .sel(ALUSrc),
        .Out(ALUSrc2)
    );

    Mux2_1 M2 (
        .In1(Res),
        .In2(FinalDataOut),
        .sel(Mem2Reg),
        .Out(RegDataIn)
    );

    Mux2_1 M3 (
        .In1(ExtendedDataOut),
        .In2(MemDataOut),
        .sel(LoadType),
        .Out(FinalDataOut)
    );

    Mux2_1 M6 (
        .In1(Imm32_Signed),
        .In2({16'b0, Instr[15:0]}),
        .sel(Imm_ExtendType),
        .Out(Imm32_Final)
    );

    Mux2_1 M7 (
        .In1(Zero),
        .In2(~Zero),
        .sel(BranchType),
        .Out(Zero_Final)
    );

    Mux2_1 # (
        .n(5)
    ) M4 (
        .In1(Instr[20:16]),
        .In2(Instr[15:11]),
        .sel(RegDst),
        .Out(WrAddr)
    );

    Mux3_1 M5 (
        .In1(SeqPC),
        .In2(BranPC),
        .In3(JumpPC),
        .sel(Sel),
        .Out(NextPC)
    );

    Adder A1 (
        .In1(SeqPC),
        .In2(Imm32_Signed_offset),
        .Out(BranPC)
    );

    Adder A2 (
        .In1(32'b0100),
        .In2(CurrentPC),
        .Out(SeqPC)
    );

    PC P (
        .D(NextPC),
        .Clk(~Clk),
        .Reset(Reset),
        .Q(CurrentPC)
    );

    Concat # (
        .n(1),
        .m(1)
    ) CC1 (
        .In1(J),
        .In2(B & Zero_Final),
        .Out(Sel)
    );

    Concat # (
        .n(4),
        .m(28)
    ) CC2 (
        .In1(SeqPC[31:28]),
        .In2(Jmp_low_order),
        .Out(JumpPC)
    );

    LeftShift LS1 (
        .In(Imm32_Signed),
        .Out(Imm32_Signed_offset)
    );

    LeftShift # (
        .n(26),
        .m(28),
        .x(2)
    ) LS2 (
        .In(Instr[25:0]),
        .Out(Jmp_low_order)
    );

endmodule