`timescale 1ns / 1ps
module ALUControl (ALUOp, Funct, ALUCtr);
    input [1:0] ALUOp;
    input [5:0] Funct;
    output reg [3:0] ALUCtr;

    always @(*) begin
        casex (ALUOp)
            2'b00:
                ALUCtr = 4'b0010;
            2'b01:
                ALUCtr = 4'b0110;
            2'b10:
                if (Funct == 6'b100000) begin
                    ALUCtr = 4'b0010;
                end
                else if (Funct == 6'b100010) begin
                    ALUCtr = 4'b0110;
                end
                else if (Funct == 6'b100100) begin
                    ALUCtr = 4'b0000;
                end
                else if (Funct == 6'b100101) begin
                    ALUCtr = 4'b0001;
                end
                else begin
                    ALUCtr = 4'b0111;
                end
        endcase
    end
endmodule