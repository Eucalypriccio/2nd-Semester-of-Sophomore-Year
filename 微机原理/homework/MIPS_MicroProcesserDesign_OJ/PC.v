`timescale 1ns / 1ps
module PC(D, Clk, Reset, Q);
    parameter n=32;
    input [n-1:0] D;
    input Clk;
    input Reset;
    output reg [n-1:0] Q;

    always @(posedge Clk, posedge Reset) begin
        if (Reset)
            Q = 32'b0;
        else
            Q <= D;
    end
endmodule