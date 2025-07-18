`timescale 1ns / 1ps
module Adder (In1,In2,Out);
    parameter n=32;
    input [n-1:0] In1;
    input [n-1:0] In2;
    output [n-1:0] Out;

    assign Out = In1 + In2;
endmodule
