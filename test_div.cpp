#include "div.hpp"
#include <iostream>
#include "ap_fixed.h"
#include <stdint.h>

using namespace std;






#ifndef LZC



int main() {
//	int main_clz = 2*tablenofbit;
//	for (int i=0;i<tableSize;i++) {
//		ap_uint<64> current = ((ap_uint<64>)1<<63)/((ap_uint<64>)(tableSize+i)*(tableSize+i));
//		int shamt_entry = clz(current) - main_clz;
//		current[63-2*tablenofbit-shamt_entry] = 0;
//		current = current >> (63-2*tablenofbit-shamt_entry-usedLength);
//		cout << shamt_entry<<", ";
//	}

	operand_type op1[] = { 0xFFFFFFFF,29000, 250225, 16435, 100, 300, 10, 12, 14, 16, 18,
			0xFFFFABCD };
	operand_type op2[] = { 1431655765,2, 25000, 2389, 13, 26, 5, 3, 2, 0, 4, 0x20FC0000 };

	for (int i=0; i<12 ; i++) {
		out_type output = divider({op1[i], op2[i]});
		if (output.division_by_zero == 1)
				cout << "division by zero" << endl;
			else
				cout << output.quotient << " " << output.remainder << " " << op1[i]/op2[i] << endl;
	}
//	for (int i = 0; i < 0xFFFFFF; i++) {
//		out_type output = divider( { 0xFFFFFF, i+1 });
//		if (output.quotient =! 0xFFFFFF/(i+1)) {
//			cout << i+1 <<endl;
//			if (output.division_by_zero == 1)
//				cout << "division by zero" << endl;
//			else
//				cout << output.quotient << " " << output.remainder << " " << 0xFFFF/(i+1) << endl;
//		}
//
//	}

//	for (operand_type j = 125; j < 12341234; j = j*210){
//		for (operand_type k = 2; k < 47; k = k + 7) {
//			out_type output = divider({j, k});
//			cout << j << "/" << k << ":" << output.quotient << " " << output.remainder << endl;
//		}
//	}
}



#else
int main() {
	ap_uint<32> test_list[] = { 0x4, 0xFF, 0x1, -0x1, 0xFFFF, 0x1FF1, 0x1453,
			0x12345678 };
	for (int i = 0; i < 8; i++) {
		cout << (int) leading_zero_count(test_list[i]) << endl;
	}
	return 0;
}
#endif
