#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include "SymbolTable.h"

using namespace std;

SymbolTable::SymbolTable() {
	um["SP"] = 0;
	um["LCL"] = 1;
	um["ARG"] = 2;
	um["THIS"] = 3;
	um["THAT"] = 4;
	um["R0"] = 0;
	um["R1"] = 1;
	um["R2"] = 2;
	um["R3"] = 3;
	um["R4"] = 4;
	um["R5"] = 5;
	um["R6"] = 6;
	um["R7"] = 7;
	um["R8"] = 8;
	um["R9"] = 9;
	um["R10"] = 10;
	um["R11"] = 11;
	um["R12"] = 12;
	um["R13"] = 13;
	um["R14"] = 14;
	um["R15"] = 15;
	um["SCREEN"] = 16384;
	um["KBD"] = 24576;
	next_address = 16;
	// for (int i = 0; i < 16384; ++i)
	// {
	// 	um[to_string(i)] = i;
	// }
}

void SymbolTable::add(string symbol) {
	um[symbol] = next_address;
	next_address++;
	if(next_address >= 16384) {
		cerr << "RAM does not have an enough memory to store the symbol." << endl;
	}
}

void SymbolTable::add_with_address(string symbol, int address) {
	um[symbol] = address;
}

bool SymbolTable::contain(string symbol) {
	return um.find(symbol) != um.end();
}

int SymbolTable::get_address(string symbol) {
	if(contain(symbol)) {
		return um[symbol];
	} else {
		add(symbol);
		return um[symbol];
	}
}

