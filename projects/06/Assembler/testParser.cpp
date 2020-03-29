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
#include <regex>
#include "Parser.h"


using namespace std;

int main() {
	Parser p = Parser("/Users/yamamuratakefumi/nand2tetris/projects/06/add/Add.asm");
	vector<string> trim_strs{"@2","D=A","@3","D=D+A","@0","M=D"};
	vector<COMMANDTYPE> cmd_types{A_CMD, C_CMD, A_CMD, C_CMD, A_CMD, C_CMD};

	int loop_cnt = 0;

	do
	{
		assert(p.current_cmd == trim_strs[loop_cnt]);
		assert(p.get_command_type() == cmd_types[loop_cnt]);
		loop_cnt++;
		// cout << p.current_cmd << endl;
	} while (p.advance());
	//output should be 
/**
@2
D=A
@3
D=D+A
@0
M=D
**/


}