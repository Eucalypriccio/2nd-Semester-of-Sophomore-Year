`timescale 1ns/1ps

module DigitExtend_tb;
    reg [1:0] ByteSelect;
    reg [1:0] ExtendType;
    reg [31:0] DataIn;
    wire [31:0] DataOut;
    
    integer i;
    DigitExtend dut (
        .ByteSelect(ByteSelect),
        .ExtendType(ExtendType),
        .DataIn(DataIn),
        .DataOut(DataOut)
    );
    
    initial begin
        DataIn = 32'h123489ab;  // 保持不变的数据输入

        // Test lb
        ExtendType = 2'b00;
        for (i=0; i<4; i=i+1) begin
            ByteSelect = i[1:0];
            #10;
        end

        // Test lbu
        ExtendType = 2'b01;
        for (i=0; i<4; i=i+1) begin
            ByteSelect = i[1:0];
            #10;
        end

        // Test lh
        ExtendType = 2'b10;
        for (i=0; i<2; i=i+1) begin
            ByteSelect = {i[0], 1'b0};
            #10;
        end

        // Test lhu (unsigned halfword extend)
        ExtendType = 2'b11;
        for (i=0; i<2; i=i+1) begin
            ByteSelect = {i[0], 1'b0};
            #10;
        end

        $finish;
    end
endmodule