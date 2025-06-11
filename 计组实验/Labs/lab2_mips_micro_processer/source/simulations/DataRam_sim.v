`timescale 1ns / 1ps

module DataRAM_tb;
    reg [4:0] Addr;
    reg [31:0] DataIn;
    reg MemWr;
    reg Clk;
    wire [31:0] DataOut;

    DataRAM drut (
        .Addr(Addr),
        .DataIn(DataIn),
        .MemWr(MemWr),
        .Clk(Clk),
        .DataOut(DataOut)
    );

    initial Clk = 0;
    always #1 Clk = ~Clk;

    initial begin
        Addr   = 0;
        DataIn = 0;
        MemWr  = 1;

        repeat (20) begin
            #2 Addr = (Addr == 9) ? 0 : Addr + 1;
        end

        #40 $finish;
    end

    initial begin
        MemWr = 1;
        #20 MemWr = 0;
    end

    initial begin
        DataIn = 32'h0000;

        while ($time < 20) begin
            #2 DataIn = DataIn + 32'h4;
        end

        DataIn = 32'h0024;
    end

endmodule