//Copyright 1986-2018 Xilinx, Inc. All Rights Reserved.
//--------------------------------------------------------------------------------
//Tool Version: Vivado v.2018.3 (win64) Build 2405991 Thu Dec  6 23:38:27 MST 2018
//Date        : Tue May 27 13:29:59 2025
//Host        : EucalypriccioTB14p running 64-bit major release  (build 9200)
//Command     : generate_target design_mb_wrapper.bd
//Design      : design_mb_wrapper
//Purpose     : IP block netlist
//--------------------------------------------------------------------------------
`timescale 1 ps / 1 ps

module design_mb_wrapper
   (MISO,
    MOSI,
    SCLK0,
    SCLK1,
    SS0,
    SS1,
    dip_switches_16bits_tri_i,
    dual_seven_seg_led_disp_tri_o,
    led_16bits_tri_o,
    push_buttons_5bits_tri_i,
    reset,
    rgb_led_tri_o,
    rx_uart3,
    seven_seg_led_an_tri_o,
    sys_clock,
    tx_uart3,
    usb_uart_rxd,
    usb_uart_txd);
  input MISO;
  output MOSI;
  output SCLK0;
  output SCLK1;
  output [0:0]SS0;
  output [0:0]SS1;
  input [15:0]dip_switches_16bits_tri_i;
  output [7:0]dual_seven_seg_led_disp_tri_o;
  output [15:0]led_16bits_tri_o;
  input [4:0]push_buttons_5bits_tri_i;
  input reset;
  output [5:0]rgb_led_tri_o;
  input rx_uart3;
  output [7:0]seven_seg_led_an_tri_o;
  input sys_clock;
  output tx_uart3;
  input usb_uart_rxd;
  output usb_uart_txd;

  wire MISO;
  wire MOSI;
  wire SCLK0;
  wire SCLK1;
  wire [0:0]SS0;
  wire [0:0]SS1;
  wire [15:0]dip_switches_16bits_tri_i;
  wire [7:0]dual_seven_seg_led_disp_tri_o;
  wire [15:0]led_16bits_tri_o;
  wire [4:0]push_buttons_5bits_tri_i;
  wire reset;
  wire [5:0]rgb_led_tri_o;
  wire rx_uart3;
  wire [7:0]seven_seg_led_an_tri_o;
  wire sys_clock;
  wire tx_uart3;
  wire usb_uart_rxd;
  wire usb_uart_txd;

  design_mb design_mb_i
       (.MISO(MISO),
        .MOSI(MOSI),
        .SCLK0(SCLK0),
        .SCLK1(SCLK1),
        .SS0(SS0),
        .SS1(SS1),
        .dip_switches_16bits_tri_i(dip_switches_16bits_tri_i),
        .dual_seven_seg_led_disp_tri_o(dual_seven_seg_led_disp_tri_o),
        .led_16bits_tri_o(led_16bits_tri_o),
        .push_buttons_5bits_tri_i(push_buttons_5bits_tri_i),
        .reset(reset),
        .rgb_led_tri_o(rgb_led_tri_o),
        .rx_uart3(rx_uart3),
        .seven_seg_led_an_tri_o(seven_seg_led_an_tri_o),
        .sys_clock(sys_clock),
        .tx_uart3(tx_uart3),
        .usb_uart_rxd(usb_uart_rxd),
        .usb_uart_txd(usb_uart_txd));
endmodule
