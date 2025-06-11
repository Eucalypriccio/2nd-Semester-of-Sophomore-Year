`timescale 1ns / 1ps
module RegFile (RsAddr, RtAddr, WrAddr, DataIn, RegWr, Clk, RsData, RtData);
    input [4:0] RsAddr;
    input [4:0] RtAddr;
    input [4:0] WrAddr;
    input [31:0] DataIn;
    input RegWr;
    input Clk;
    output [31:0] RsData;
    output [31:0] RtData;

    reg [31:0] regs [0:31];        // 总共 32 个通用寄存器

    always @(posedge Clk) begin
        if (RegWr)
            regs[WrAddr] <= DataIn;
    end

    assign RsData = regs[RsAddr];
    assign RtData = regs[RtAddr];
endmodule