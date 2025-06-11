`timescale 1ns / 1ps
module DataRAM(Addr, DataIn, MemWr, Clk, DataOut);
    parameter n=5,m=32;
    input [n-1:0] Addr;
    input [m-1:0] DataIn;
    input MemWr;
    input Clk;
    output [m-1:0] DataOut;

    reg [m-1:0] regs [0:(1<<n)-1];

    always @(posedge Clk) begin
        if (MemWr)
            regs[Addr] <= DataIn;
    end

    assign DataOut = regs[Addr];

endmodule