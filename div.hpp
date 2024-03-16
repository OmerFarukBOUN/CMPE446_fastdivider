#pragma once

#include <ap_int.h>
#include <cmath>
#include "ap_fixed.h"

#define XLEN 32
#define kernel_size 8
#define LUT_output_size 32 - __builtin_clz(kernel_size)
#define HALF 16
#define leading_return_size 6
#define maskH 0xFFFFF000
#define maskL 0x00000FFF

//#define BITWISE
//#define ARRAY
//#define LZC

typedef ap_int<XLEN> operand_type;
typedef ap_int<HALF> half_type;
typedef ap_uint<1> bool_type;
typedef ap_uint<16> lookup_type;
typedef ap_uint<kernel_size> lut_input;
typedef ap_uint<LUT_output_size> lut_output;
typedef ap_uint<XLEN> reg;
typedef ap_uint<leading_return_size> lzc_ret;
typedef ap_uint<XLEN> uns;

typedef struct {
	operand_type op1;
	operand_type op2;
} in_type;

typedef struct {
	operand_type quotient;
	operand_type remainder;
	bool_type division_by_zero;
} out_type;

out_type divider(in_type input);
lzc_ret leading_zero_count(reg input);
lut_output LUT(lut_input input);
