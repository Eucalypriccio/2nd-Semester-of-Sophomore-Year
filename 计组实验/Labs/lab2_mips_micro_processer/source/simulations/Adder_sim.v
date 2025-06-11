`timescale 1ns/1ps

module Adder_tb;
    reg [31:0] In1;
    reg [31:0] In2;
    wire [31:0] Out;

    Adder aut (
        .In1(In1),
        .In2(In2),
        .Out(Out)
    );

    initial begin
        In1 = 32'h00000004;
        In2 = 32'h00000008;
        #10;

        In1 = 32'h44444444;
        In2 = 32'h88888888;
        #10;

        $finish;
    end

endmodule