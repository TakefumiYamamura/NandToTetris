#include <iostream>
#include <vector>
#include <string>
#include <fstream> 
#include <sstream>
#include <unordered_map>
#include <queue>
#include <set>
#include <algorithm>
#include <cassert>
#include "SymbolTable.h"


using namespace std;

int main() {
	SymbolTable st = SymbolTable();
	assert(st.get_address("SP") == 0);
	st.add("TAKE");
	assert(st.get_address("TAKE") == 16);
	st.add("YAMAMTAK");
	assert(st.get_address("YAMAMTAK") == 17);
}