`timescale 1ns / 1ps
module SignedExtend (In, Out);
    parameter n=16,m=32;
    input [n-1:0] In;
    output [m-1:0] Out;

    assign Out = {(In[15] == 1)?(16'b1111111111111111):(16'b0000000000000000), In};
endmodule