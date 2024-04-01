#include "div.hpp"
#include <iostream>

using namespace std;


//	lookup_type divLUT[] = { 0b1000000000000000, 0b0111100001011100,
//			0b0111000101100010, 0b0110101011111111, 0b0110010100100011,
//			0b0101111110111110, 0b0101101011000101, 0b0101011000101101,
//			0b0101000111101100, 0b0100110111111001, 0b0100101001001110,
//			0b0100011011100011, 0b0100001110110100, 0b0100000010111010,
//			0b0011110111110001, 0b0011101101010110, 0b0011100011100100,
//			0b0011011010010111, 0b0011010001101110, 0b0011001001100101,
//			0b0011000001111001, 0b0010111010101001, 0b0010110011110011,
//			0b0010101101010100, 0b0010100111001100, 0b0010100001011000,
//			0b0010011011110111, 0b0010010110100111, 0b0010010001101001,
//			0b0010001100111010, 0b0010001000011001, 0b0010000100000110 };

//	lookup_type divLUT[] = { 0, 7387213, 6472871, 5635775, 4867464, 4160603, 3508808, 2906507, 2348810, 1831421, 1350547, 902830, 485291, 95278, 7849454, 7165823, 6524473, 5921987, 5355287, 4821595, 4318396, 3843410, 3394567, 2969983, 2567941, 2186873, 1825345, 1482044, 1155764, 845399, 549930, 268419 };
//	shamt_type shamtLUT[] = {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2};

//lookup_type divLUT[tableSize];
//shamt_type shamtLUT[tableSize];
//
//for (int i=0;i<tableSize;i++) {
////		double_cast current = {.d = 1.0/(1+i/32.0)/(1+i/32.0)};
//	float_cast current = {.f = 1.0/(1+i/32.0)/(1+i/32.0)};
//	divLUT[i] = current.parts.used;
//	shamtLUT[i] = 127-current.parts.exponent;
//}

out_type divider(in_type input) {
#ifndef LZC
#pragma HLS top name=DESIGN_TOP
#endif


//		lookup_type divLUT[] = { 0, 7387213, 6472871, 5635775, 4867464, 4160603,
//			3508808, 2906507, 2348810, 1831421, 1350547, 902830, 485291, 95278,
//			7849454, 7165823, 6524473, 5921987, 5355287, 4821595, 4318396,
//			3843410, 3394567, 2969983, 2567941, 2186873, 1825345, 1482044,
//			1155764, 845399, 549930, 268419 };
//	shamt_type shamtLUT[] = { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2,
//			2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 };
	lookup_type divLUT[tableSize];
	shamt_type shamtLUT[tableSize];
	init_divLut(divLUT, shamtLUT);
	operand_type op1 = input.op1;
	operand_type op2 = input.op2;
	bool_type division_by_zero = 0;
	operand_type quotient_first;
	operand_type quotient;
	operand_type remainder;
	if (op2 == 0) {
		division_by_zero = 1;
		quotient = 0;
		remainder = 0;
	} else {
		lzc_ret lzc = leading_zero_count(op2);
		operand_type niced_y = (op2 << lzc).range(31,0);
		operand_type yh = niced_y & maskH;
		operand_type yl = niced_y & maskL;
		ap_uint<tablenofbit> index = yh.range(30, 31-tablenofbit);
		quotient_first = ((op1 * operand_type(yh - yl)) * (1, divLUT[index]))>>(2*XLEN-2+shamtLUT[index]+usedLength-lzc);
		ap_int<32> tmp2 = (ap_int<64>) op1 - op2 * quotient_first;
		if (tmp2 < 0) {
			quotient = quotient_first-1;
			remainder = tmp2 + op2;
		} else {
			if (op2<= tmp2) {
				quotient = quotient_first +1;
				remainder = tmp2 - op2;
			} else {
				quotient = quotient_first;
				remainder = tmp2;
			}
		}
	}
	return {quotient, remainder, division_by_zero};
}

void init_divLut(lookup_type divLUT[tableSize], shamt_type shamtLUT[tableSize]) {
	int main_clz = 2*tablenofbit;
	for (int i=0;i<tableSize;i++) {
		ap_uint<64> current = ((ap_uint<64>)1<<63)/((ap_uint<64>)(tableSize+i)*(tableSize+i));
		int shamt_entry = clz(current) - main_clz;
		current[63-2*tablenofbit-shamt_entry] = 0;
		current = current >> (63-2*tablenofbit-shamt_entry-usedLength);
		divLUT[i] = current.range(usedLength-1,0);
		shamtLUT[i] = shamt_entry;
//		if (i!=0) {
//			if (64%i !=0) {
//				divLUT[i] += 1;
//			}
//		}
//
////		cout << divLUT[i]<<" ," ;
	}

}
int clz(ap_uint<64> bruh) {
	int ret = 0;
	for (int i = 0; i<64;i++) {
		if (bruh[63-i]) {
			break;
		} else {
			ret++;
		}
	}
	return ret;
}

lzc_ret leading_zero_count(reg input) {
#ifdef LZC
#pragma HLS top name=DESIGN_TOP
#endif
	lzc_ret ret_val;
#if kernel_size < 4
	ap_uint<2> LUTS2[16];
	ap_uint<2> intermediate2;
#endif
#if kernel_size < 8
	ap_uint<4> LUTS4[8];
	ap_uint<4> intermediate3;
#endif
	ap_uint<4> LUTS8[4];
	ap_uint<5> LUTS16[2];
	ap_uint<4> intermediate4;
	ap_uint<5> intermediate5;
#if kernel_size < 8
#if kernel_size == 2
	for (int i = 0; i < 16; i++) {
#pragma HLS UNROLL
		LUTS2[i] = LUT(
				input.range(XLEN - 1 - kernel_size * i,
						XLEN - kernel_size * (i + 1)));
	}
#endif
	for (int i = 0; i < 8; i++) {
#pragma HLS UNROLL
#if kernel_size == 4
		LUTS4[i] = LUT(
				input.range(XLEN - 1 - kernel_size * i,
						XLEN - kernel_size * (i + 1)));
#elif kernel_size < 4
		intermediate2 = (LUTS2[2 * i][1], LUTS2[2 * i][1]) & LUTS2[2 * i + 1];
		LUTS4[i] = LUTS2[2 * i] + intermediate2;
#endif

	}
#endif
	for (int i = 0; i < 4; i++) {
#pragma HLS UNROLL
#if kernel_size == 8
#pragma HLS inline recursive
		LUTS8[i] = LUT(input.range(XLEN - 1 - kernel_size * i,
		XLEN - kernel_size * (i + 1)));
#elif kernel_size < 8
		intermediate3 = (LUTS4[2 * i][2], LUTS4[2 * i][2], LUTS4[2 * i][2])
				& LUTS4[2 * i + 1];
		LUTS8[i] = LUTS4[2 * i] + intermediate3;
#endif
	}
	for (int i = 0; i < 2; i++) {
#pragma HLS UNROLL
		intermediate4 =
				(LUTS8[2 * i][3], LUTS8[2 * i][3], LUTS8[2 * i][3], LUTS8[2 * i][3])
						& LUTS8[2 * i + 1];
		LUTS16[i] = LUTS8[2 * i] + intermediate4;
	}
	intermediate5 =
			(LUTS16[0][4], LUTS16[0][4], LUTS16[0][4], LUTS16[0][4], LUTS16[0][4])
					& LUTS16[1];
	ret_val = LUTS16[0] + intermediate5;
	return ret_val;
}

#if kernel_size == 2
#ifndef BITWISE
lut_output LUT(lut_input input) {
	lut_output ret_val;
	switch (input) {
	case 0:
		ret_val = 2;
		break;
	case 1:
		ret_val = 1;
	default:
		ret_val = 0;
		break;
	}
	return ret_val;
}
#else
lut_output LUT(lut_input input) {
	lut_output ret_val;
	switch(input) {
	case 0:
		ret_val[1] = 1;
		break;
	default:
		ret_val[1] = 0;
		break;
	}
	switch(input) {
	case 1:
		ret_val[0] = 1;
		break;
	default:
		ret_val[0] = 0;
		break;
	}
	return ret_val;
}
#endif
#endif

#if kernel_size == 4
#ifndef BITWISE
lut_output LUT(lut_input input) {
	lut_output ret;
	switch (input) {
	case 0:
		ret =4;
		break;
	case 1:
		ret =3;
		break;
	case 2:
	case 3:
		ret =2;
		break;
	case 4:
	case 5:
	case 6:
	case 7:
		ret =1;
		break;
	default:
		ret = 0;
		break;
	}
	return ret;
}
#else
lut_output LUT(lut_input input) {
	lut_output ret;
	switch (input) {
	case 0:
		ret[2] = 1;
		break;
	default:
		ret[2] = 0;
	}
	switch (input) {
	case 1:
	case 2:
	case 3:
		ret[1] = 1;
		break;
	default:
		ret[1] = 0;
	}
	switch (input) {
	case 1:
	case 4:
	case 5:
	case 6:
	case 7:
		ret[0] = 1;
		break;
	default:
		ret[0] = 0;
	}
	return ret;
}
#endif
#endif

#if kernel_size == 8
#ifndef BITWISE
#ifndef ARRAY
lut_output LUT(lut_input input) {
	lut_output ret;
	switch (input) {
	case 0:
		ret =  8;
		break;
	case 1:
		ret =7;
		break;
	case 2:
	case 3:
		ret =6;
		break;
	case 4:
	case 5:
	case 6:
	case 7:
		ret =5;
		break;
	case 8:
	case 9:
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
		ret = 4;
		break;
	case 16:
	case 17:
	case 18:
	case 19:
	case 20:
	case 21:
	case 22:
	case 23:
	case 24:
	case 25:
	case 26:
	case 27:
	case 28:
	case 29:
	case 30:
	case 31:
		ret = 3;
		break;
	case 32:
	case 33:
	case 34:
	case 35:
	case 36:
	case 37:
	case 38:
	case 39:
	case 40:
	case 41:
	case 42:
	case 43:
	case 44:
	case 45:
	case 46:
	case 47:
	case 48:
	case 49:
	case 50:
	case 51:
	case 52:
	case 53:
	case 54:
	case 55:
	case 56:
	case 57:
	case 58:
	case 59:
	case 60:
	case 61:
	case 62:
	case 63:
		ret = 2;
		break;
	case 64:
	case 65:
	case 66:
	case 67:
	case 68:
	case 69:
	case 70:
	case 71:
	case 72:
	case 73:
	case 74:
	case 75:
	case 76:
	case 77:
	case 78:
	case 79:
	case 80:
	case 81:
	case 82:
	case 83:
	case 84:
	case 85:
	case 86:
	case 87:
	case 88:
	case 89:
	case 90:
	case 91:
	case 92:
	case 93:
	case 94:
	case 95:
	case 96:
	case 97:
	case 98:
	case 99:
	case 100:
	case 101:
	case 102:
	case 103:
	case 104:
	case 105:
	case 106:
	case 107:
	case 108:
	case 109:
	case 110:
	case 111:
	case 112:
	case 113:
	case 114:
	case 115:
	case 116:
	case 117:
	case 118:
	case 119:
	case 120:
	case 121:
	case 122:
	case 123:
	case 124:
	case 125:
	case 126:
	case 127:
		ret = 1;
		break;
	default:
		ret = 0;
		break;
	}
	return ret;
}
#else
lut_output LUT(lut_input input) {
	lut_output LUT_array[] = { 8, 7, 6, 6, 5, 5, 5, 5, 4, 4, 4, 4, 4, 4, 4, 4,
			3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2,
			2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
			2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
			1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
			1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	return LUT_array[input];
}
#endif
#else
lut_output LUT(lut_input input) {
	lut_output ret;
	switch(input) {
	case 0:
		ret[3] = 1;
		break;
	default:
		ret[3] = 0;
	}
	switch(input) {
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
		ret[2] = 1;
		break;
	default:
		ret[2] = 0;
	}
	switch (input) {
	case 1:
	case 2:
	case 3:
	case 16:
	case 17:
	case 18:
	case 19:
	case 20:
	case 21:
	case 22:
	case 23:
	case 24:
	case 25:
	case 26:
	case 27:
	case 28:
	case 29:
	case 30:
	case 31:
	case 32:
	case 33:
	case 34:
	case 35:
	case 36:
	case 37:
	case 38:
	case 39:
	case 40:
	case 41:
	case 42:
	case 43:
	case 44:
	case 45:
	case 46:
	case 47:
	case 48:
	case 49:
	case 50:
	case 51:
	case 52:
	case 53:
	case 54:
	case 55:
	case 56:
	case 57:
	case 58:
	case 59:
	case 60:
	case 61:
	case 62:
	case 63:
		ret[1] = 1;
		break;
	default:
		ret[1] = 0;
		break;
	}
	switch (input) {
	case 1:
	case 4:
	case 5:
	case 6:
	case 7:
	case 16:
	case 17:
	case 18:
	case 19:
	case 20:
	case 21:
	case 22:
	case 23:
	case 24:
	case 25:
	case 26:
	case 27:
	case 28:
	case 29:
	case 30:
	case 31:
	case 64:
	case 65:
	case 66:
	case 67:
	case 68:
	case 69:
	case 70:
	case 71:
	case 72:
	case 73:
	case 74:
	case 75:
	case 76:
	case 77:
	case 78:
	case 79:
	case 80:
	case 81:
	case 82:
	case 83:
	case 84:
	case 85:
	case 86:
	case 87:
	case 88:
	case 89:
	case 90:
	case 91:
	case 92:
	case 93:
	case 94:
	case 95:
	case 96:
	case 97:
	case 98:
	case 99:
	case 100:
	case 101:
	case 102:
	case 103:
	case 104:
	case 105:
	case 106:
	case 107:
	case 108:
	case 109:
	case 110:
	case 111:
	case 112:
	case 113:
	case 114:
	case 115:
	case 116:
	case 117:
	case 118:
	case 119:
	case 120:
	case 121:
	case 122:
	case 123:
	case 124:
	case 125:
	case 126:
	case 127:
		ret[0] = 1;
		break;
	default:
		ret[0] = 0;
	}
	return ret;
}
#endif
#endif

