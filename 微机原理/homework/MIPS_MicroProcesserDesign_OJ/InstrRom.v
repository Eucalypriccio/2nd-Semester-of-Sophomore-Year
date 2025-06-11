`timescale 1ns / 1ps
module InstrROM (Addr, Clk, Instr);
    parameter n=5;
    input [n-1:0] Addr;
    input Clk;
    output reg [31:0] Instr;

    reg [n-1:0] regs[0:(1<<n)-1];

    always @(posedge Clk) begin
        Instr <= regs[Addr];
    end

endmodule