set_property -dict {PACKAGE_PIN C17 IOSTANDARD LVCMOS33} [get_ports {SS1[0]}]
set_property -dict {PACKAGE_PIN D18 IOSTANDARD LVCMOS33} [get_ports MISO]
set_property -dict {PACKAGE_PIN G17 IOSTANDARD LVCMOS33} [get_ports SCLK1]

set_property -dict {PACKAGE_PIN D14 IOSTANDARD LVCMOS33} [get_ports {SS0[0]}]
set_property -dict {PACKAGE_PIN F16 IOSTANDARD LVCMOS33} [get_ports MOSI]
set_property -dict {PACKAGE_PIN H14 IOSTANDARD LVCMOS33} [get_ports SCLK0]

set_property PACKAGE_PIN K1 [get_ports screen_io_rxd]
set_property PACKAGE_PIN G6 [get_ports screen_io_txd]
set_property IOSTANDARD LVCMOS33 [get_ports screen_io_rxd]
set_property IOSTANDARD LVCMOS33 [get_ports screen_io_txd]

set_property PACKAGE_PIN K1 [get_ports rx_uart3]
set_property PACKAGE_PIN G6 [get_ports tx_uart3]
set_property IOSTANDARD LVCMOS33 [get_ports rx_uart3]
set_property IOSTANDARD LVCMOS33 [get_ports tx_uart3]
