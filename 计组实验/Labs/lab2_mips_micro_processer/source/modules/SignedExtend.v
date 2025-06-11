`timescale 1ns / 1ps
module SignedExtend (In, Out);
    parameter n=16, m=32;
    input [n-1:0] In;
    output [m-1:0] Out;

    assign Out = {{(m-n){In[n-1]}}, In};
endmodule