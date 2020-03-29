#include <iostream>
#include <vector>
#include <string>
#include <fstream> 
#include <sstream>
#include <unordered_map>
#include <queue>
#include <set>
#include <algorithm>
#include <regex>
#include "Parser.h"
#include "SymbolTable.h"

using namespace std;

class Assembler
{
public:
	Parser p;
	SymbolTable st;
	Assembler(string file_name) {
		p = Parser(file_name + ".asm");
		ofstream myfile;
  		myfile.open (file_name + ".hack");
		st = SymbolTable();
		int inst_cnt = 0;
		do
		{
			// cout << p.current_cmd << " "<< p.get_command_type() <<  endl;
			if(p.get_command_type() == L_CMD) {
				if(!st.contain(p.get_symbol())) {
					st.add_with_address(p.get_symbol(), inst_cnt);
				}
			} else {
				inst_cnt++;
			}
		} while (p.advance());
		p.reset();
		do
		{
			if(p.get_command_type() == A_CMD && p.contain_symbol()) {
				if(!st.contain(p.get_symbol())) {
					st.add(p.get_symbol());
				}
			}
		} while (p.advance());
		p.reset();
		do
		{
			// if(p.current_cmd == "AM=M-1") {

			// 	cout << p.get_comp() << " " <<  p.get_dest() << " " <<  p.get_jump() << endl;

			// }
			if(p.get_command_type() == A_CMD ) {
				myfile << get_a_inst() << endl;
			}
			if(p.get_command_type() == C_CMD ) {
				// cout << p.current_cmd << " "<< p.get_command_type() <<  endl;
				myfile << get_c_inst() << endl;
			} 
		} while (p.advance());

		// myfile << "Writing this to a file.\n";
  		myfile.close();
	}

	string get_a_inst() {
		int num;
		if(p.contain_symbol()) {
			num = st.get_address(p.get_symbol());
		} else {
			num = stoi(p.get_symbol());
		}
		return get_binary_value(num);
	}

	string get_c_inst() {
		string str = "111" + p.get_comp() + p.get_dest() + p.get_jump();
		return str;
	}

	string get_binary_value(int num) {
		string ans = "";
		while(num > 0){
			if(num & 1){
				ans = "1" + ans;
			} else {
				ans = "0" + ans;
			}
			num >>= 1;
		}
		while(ans.size() < 16) {
			ans = "0" + ans;
		}
		return ans;
	}
};

int main() {
	Assembler a1 = Assembler("/Users/yamamuratakefumi/nand2tetris/projects/06/add/Add");
	Assembler a2 = Assembler("/Users/yamamuratakefumi/nand2tetris/projects/06/max/Max");
	Assembler a3 = Assembler("/Users/yamamuratakefumi/nand2tetris/projects/06/pong/Pong");
	Assembler a4 = Assembler("/Users/yamamuratakefumi/nand2tetris/projects/06/rect/Rect");
}