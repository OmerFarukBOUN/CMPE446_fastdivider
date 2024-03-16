
/home/omerfaruk/tools/Xilinx/Vivado/2023.2/bin/xelab xil_defaultlib.apatb_divider_top glbl -Oenable_linking_all_libraries  -prj divider.prj -L smartconnect_v1_0 -L axi_protocol_checker_v1_1_12 -L axi_protocol_checker_v1_1_13 -L axis_protocol_checker_v1_1_11 -L axis_protocol_checker_v1_1_12 -L xil_defaultlib -L unisims_ver -L xpm  -L floating_point_v7_1_17 -L floating_point_v7_0_22 --lib "ieee_proposed=./ieee_proposed" -s divider -debug all
/home/omerfaruk/tools/Xilinx/Vivado/2023.2/bin/xsim --noieeewarnings divider -tclbatch divider.tcl -gui -view divider_dataflow_ana.wcfg -protoinst divider.protoinst

