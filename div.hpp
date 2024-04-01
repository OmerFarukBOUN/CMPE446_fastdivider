#pragma once

#include <ap_int.h>
#include <cmath>
#include "ap_fixed.h"

#define XLEN 32
#define kernel_size 4
#define LUT_output_size 4
#define HALF 16
#define leading_return_size 6
#define tablenofbit 5
#define tableSize (1 << tablenofbit)
#define maskH ((1<<tablenofbit)-1) << (32-tablenofbit)
#define maskL (0xFFFFFFFF ^ maskH)
#define usedLength 32


//#define BITWISE
//#define ARRAY
//#define LZC

typedef ap_uint<XLEN> operand_type;
typedef ap_int<HALF> half_type;
typedef ap_uint<1> bool_type;
typedef ap_uint<usedLength> lookup_type;
typedef ap_uint<kernel_size> lut_input;
typedef ap_uint<LUT_output_size> lut_output;
typedef ap_uint<XLEN> reg;
typedef ap_uint<leading_return_size> lzc_ret;
typedef ap_uint<XLEN> uns;
typedef ap_uint<2> shamt_type;

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
void init_divLut(lookup_type divLUT[tableSize], shamt_type shamtLUT[tableSize]);
int clz(ap_uint<64> bruh);

//typedef union {
//	double d;
//	struct {
//		unsigned int unused :52 - usedLength;
//		unsigned int used :usedLength;
//		unsigned int exponent :11;
//		unsigned int sign :1;
//	} parts;
//} double_cast;
//
//typedef union {
//  float f;
//  struct {
////	unsigned int unused: 23 - usedLength;
//	unsigned int used: usedLength;
//    unsigned int exponent : 8;
//    unsigned int sign : 1;
//  } parts;
//} float_cast;
