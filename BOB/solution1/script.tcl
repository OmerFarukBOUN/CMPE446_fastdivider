############################################################
## This file is generated automatically by Vitis HLS.
## Please DO NOT edit it.
## Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
## Copyright 2022-2024 Advanced Micro Devices, Inc. All Rights Reserved.
############################################################
open_project BOB
set_top divider
add_files div.cpp
add_files div.hpp
add_files -tb test_div.cpp -cflags "-Wno-unknown-pragmas"
open_solution "solution1" -flow_target vivado
set_part {xczu7ev-ffvf1517-3-e}
create_clock -period 10 -name default
config_cosim -tool xsim -trace_level port -wave_debug
source "./BOB/solution1/directives.tcl"
csim_design -clean
csynth_design
cosim_design -wave_debug -trace_level port
export_design -format ip_catalog
