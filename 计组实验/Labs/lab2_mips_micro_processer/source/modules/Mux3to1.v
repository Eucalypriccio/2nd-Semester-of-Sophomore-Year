`timescale 1ns / 1ps
module Mux3_1 (In1, In2, In3, sel, Out);
    parameter n=32;
    input [n-1:0] In1;
    input [n-1:0] In2;
    input [n-1:0] In3;
    input [1:0] sel;
    output [n-1:0] Out;

    wire [n-1:0] Out_1;

    Mux2_1 M1 (
        .In1(In1),
        .In2(In2),
        .sel(sel[0]),
        .Out(Out_1)
    );

    Mux2_1 M2 (
        .In1(Out_1),
        .In2(In3),
        .sel(sel[1]),
        .Out(Out)
    );
endmodule