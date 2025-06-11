connect -url tcp:127.0.0.1:3121
targets -set -filter {jtag_cable_name =~ "Digilent Nexys4DDR 210292A4BBDDA" && level==0} -index 0
fpga -file D:/MYHUST/sophomore_year/second_semester/CCIT/Mini_IO/Mini_IO.sdk/design_mb_wrapper_hw_platform_0/download.bit
configparams mdm-detect-bscan-mask 2
targets -set -nocase -filter {name =~ "microblaze*#0" && bscan=="USER2"  && jtag_cable_name =~ "Digilent Nexys4DDR 210292A4BBDDA"} -index 0
rst -system
after 3000
targets -set -nocase -filter {name =~ "microblaze*#0" && bscan=="USER2"  && jtag_cable_name =~ "Digilent Nexys4DDR 210292A4BBDDA"} -index 0
dow D:/MYHUST/sophomore_year/second_semester/CCIT/Mini_IO/Mini_IO.sdk/hello_word/Debug/hello_word.elf
bpadd -addr &main
