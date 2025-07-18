`timescale 1ns / 1ps
module Concat (In1, In2, Out);
    parameter n=4,m=28;
    input [n-1:0] In1;
    input [m-1:0] In2;
    output [n+m-1:0] Out;

    assign Out = {In1, In2};
endmodule