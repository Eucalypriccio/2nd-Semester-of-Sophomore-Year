`timescale 1ns / 1ps
module DigitExtend (ByteSelect, ExtendType, DataIn, DataOut);
    input [1:0] ByteSelect;
    input [1:0] ExtendType;
    input [31:0] DataIn;
    output reg [31:0] DataOut;

    reg [7:0] byte_data;
    reg [15:0] half_data;
    
    always @(*) begin
        case (ByteSelect)
            2'b00:
                begin
                    byte_data = DataIn[31:24];
                    half_data = DataIn[31:16];
                end
            2'b01:
                begin
                    byte_data = DataIn[23:16];
                    half_data = DataIn[31:16];
                end
            2'b10:
                begin
                    byte_data = DataIn[15:8];
                    half_data = DataIn[15:0];
                end
            2'b11:
                begin
                    byte_data = DataIn[7:0];
                    half_data = DataIn[15:0];
                end
        endcase    
    end

    always @(*) begin
        case (ExtendType)
            2'b00: DataOut = {{(24){byte_data[7]}}, byte_data};   // lb
            2'b01: DataOut = {24'b0, byte_data};                  // lbu
            2'b10: DataOut = {{(16){half_data[15]}}, half_data};  // lh
            2'b11: DataOut = {16'b0, half_data};                  // lhu
            default: DataOut = DataIn;                            // lw
        endcase
    end
    
endmodule