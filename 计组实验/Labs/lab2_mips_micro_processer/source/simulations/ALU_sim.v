`timescale 1ns/1ps

module ALU_tb;
    reg [31:0] In1;
    reg [31:0] In2;
    reg [3:0] ALUCtr;
    wire [31:0] Res;
    wire Zero;

    ALU aluut (
        .In1(In1),
        .In2(In2),
        .ALUCtr(ALUCtr),
        .Res(Res),
        .Zero(Zero)
    );

    initial begin
        In1 = 32'h00000004;
        In2 = 32'h00000008;
        ALUCtr = 4'b0000;
        #2;

        ALUCtr = 4'b0001;
        #2;

        ALUCtr = 4'b0010;
        #2;

        ALUCtr = 4'b0110;
        #2;

        ALUCtr = 4'b0111;
        #2;
        
        In1 = 32'h44444444;
        In2 = 32'h88888888;
        ALUCtr = 4'b0000;
        #2;

        ALUCtr = 4'b0001;
        #2;
        
        ALUCtr = 4'b0010;
        #2;

        ALUCtr = 4'b0110;
        #2;

        ALUCtr = 4'b0111;
        #2;

        In1 = 32'h99999999;
        ALUCtr = 4'b0000;
        #2;

        ALUCtr = 4'b0001;
        #2;
        
        ALUCtr = 4'b0010;
        #2;

        ALUCtr = 4'b0110;
        #2;

        ALUCtr = 4'b0111;
        #2;

        $finish;
    end
    
endmodule