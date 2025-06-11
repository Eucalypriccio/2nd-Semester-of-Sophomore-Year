`timescale 1ns / 1ps
module Mux2_1 (In1, In2, sel, Out);
    parameter n=32;
    input [n-1:0] In1;
    input [n-1:0] In2;
    input sel;
    output reg [n-1:0] Out;

    always @(*) begin
        case (sel)
            1'b0: Out = In1;
            1'b1: Out = In2; 
        endcase
    end
endmodule
