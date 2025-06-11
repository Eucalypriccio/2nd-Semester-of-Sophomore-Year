`timescale 1ns/1ps

module MIPSCPU_tb;
    reg Clk, Reset;
    MIPSCPU uut (
        .Clk(Clk),
        .Reset(Reset)
    );
    
    parameter period = 10;

    always begin
        Clk = 1'b0;
        #(period / 2) Clk = 1'b1;
        #(period / 2);
    end

    integer i, j;
    
    initial begin
        for (i = 0; i < 32; i = i + 1) begin
            uut.RegFile_U1.regs[i] = i * 4;
            uut.RAM_U3.regs[i] = i * 4;
        end

        uut.RAM_U3.regs[2] = 32'h123489ab;      // 00010010_00110100_10001001_10101011

        $readmemh("machine_instrs.txt", uut.ROM_U0.regs);
        for (j = 0; j < 28; j = j + 1) begin
            $display("%b", uut.ROM_U0.regs[j]);
        end

        Reset = 1'b0;
        #10;
        Reset = 1'b1;
        #10;
        Reset = 1'b0;
        #300;

        $stop;
    end

endmodule