`timescale 1ns / 1ps
module ALU (In1, In2, ALUCtr, Res, Zero);
    input [31:0] In1;
    input [31:0] In2;
    input [3:0] ALUCtr;
    output reg [31:0] Res;
    output Zero;

    wire signed [31:0] signed_In1 = In1;
    wire signed [31:0] signed_In2 = In2;
    
    always @(*) begin
        case (ALUCtr)
            4'b0010: Res = In1 + In2;
            4'b0110: Res = In1 - In2;
            4'b0111: Res = (signed_In1 < signed_In2) ? 32'b1 : 32'b0;
            4'b0000: Res = In1 & In2;
            4'b0001: Res = In1 | In2;
            default: Res = 0;
        endcase
    end

    assign Zero = (Res == 32'b0);
endmodule
