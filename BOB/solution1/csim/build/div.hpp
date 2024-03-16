#pragma once

#include <ap_int.h>
#include <cmath>
#include "ap_fixed.h"
using namespace std;

#define kernel_size 4
#define leading_return_size 6
#define XLEN 32
#define HALF 32


typedef ap_uint<kernel_size> lzc;
typedef ap_uint<XLEN> reg;
typedef ap_uint<XLEN> operand_type;
typedef ap_uint<leading_return_size> lzc_ret;
lzc_ret leading_zero_count(reg input);
ap_uint<4> LUT(ap_uint<8> input);
