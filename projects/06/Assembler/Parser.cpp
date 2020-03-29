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

using namespace std;

Parser::Parser() {

}

Parser::Parser(string file_name){
	ifstream ifs(file_name);
    if (ifs.fail()) {
        cerr << "Failed to open file." << endl;
        return;
    }

    string str;
    while (getline(ifs, str)) {
    	if(str[0] =='/' && str[1] =='/') continue;
    	str.erase(remove_if(str.begin(), str.end(), ::isspace), str.end());
    	if(str == "") continue;
        for (int i = 0; i + 1 < str.size(); ++i)
        {
            if(str[i] == '/' && str[i+1] == '/') {
                str = str.substr(0, i);
            }
        }
        cmds.push_back(str);
    }
    cmd_idx = 0;
    if(cmds.size() > 1) {
    	current_cmd = cmds[0];
    }


    dest_dict[""] = "000";
    dest_dict["M"] = "001";
    dest_dict["D"] = "010";
    dest_dict["MD"] = "011";
    dest_dict["A"] = "100";
    dest_dict["AM"] = "101";
    dest_dict["AD"] = "110";
    dest_dict["AMD"] = "111";

    comp_dict["0"] = "0101010";
    comp_dict["1"] = "0111111";
    comp_dict["-1"] = "0111010";
    comp_dict["D"] = "0001100";
    comp_dict["A"] = "0110000";
    comp_dict["M"] = "1110000";
    comp_dict["!D"] = "0001100";
    comp_dict["!A"] = "0110001";
    comp_dict["!M"] = "1110001";
    comp_dict["-D"] = "0001111";
    comp_dict["-A"] = "0110011";
    comp_dict["-M"] = "1110011";
    comp_dict["D+1"] = "0011111";
    comp_dict["A+1"] = "0110111";
    comp_dict["M+1"] = "1110111";
    comp_dict["D-1"] = "0001110";
    comp_dict["A-1"] = "0110010";
    comp_dict["M-1"] = "1110010";
    comp_dict["D+A"] = "0000010";
    comp_dict["D+M"] = "1000010";
    comp_dict["D-A"] = "0010011";
    comp_dict["D-M"] = "1010011";
    comp_dict["A-D"] = "0000111";
    comp_dict["M-D"] = "1000111";
    comp_dict["D&A"] = "0000000";
    comp_dict["D&M"] = "1000000";
    comp_dict["D|A"] = "0010101";
    comp_dict["D|M"] = "1010101";

    jump_dict[""] = "000";
    jump_dict["JGT"] = "001";
    jump_dict["JEQ"] = "010";
    jump_dict["JGE"] = "011";
    jump_dict["JLT"] = "100";
    jump_dict["JNE"] = "101";
    jump_dict["JLE"] = "110";
    jump_dict["JMP"] = "111";
}

bool Parser::advance() {
	if(cmd_idx + 1 < cmds.size() ) {
		cmd_idx++;
		current_cmd = cmds[cmd_idx];
		return true;
	}
	return false;
}

bool Parser::reset() {
    if(cmds.size() != 0) {
        cmd_idx = 0;
        current_cmd = cmds[0];
        return true;
    }
    return false;
}


COMMANDTYPE Parser::get_command_type() {
    const regex re_a("@([0-9A-Za-z_\\.\\$\\:]+)"); // @symbol, @13
    const regex re_c("A?M?D?=?.*;?"); //D=D+A,  M,  
    const regex re_l("\\([0-9A-Za-z_\\.\\$\\:]+\\)"); //(ADFA)
    if(regex_match(current_cmd, re_a)) {
        return A_CMD;
    }
    if(regex_match(current_cmd, re_l)) {
        return L_CMD;
    }
    if(regex_match(current_cmd, re_c)) {
        return C_CMD;
    }

    return INVALID_CMD;
}

bool Parser::contain_symbol() {
    if(get_command_type() == A_CMD){
        const regex re_not_symbol_a("@([0-9]+)");
        if(regex_match(current_cmd, re_not_symbol_a)) return false;
        return true;
    }
    if(get_command_type() == L_CMD){
        const regex re_not_symbol_l("\\(([0-9A-Za-z_\\.\\$\\:]+)\\)");
        if(regex_match(current_cmd, re_not_symbol_l)) return false;
        return true;
    }
    assert(get_command_type() == A_CMD || get_command_type() == L_CMD);
    return false;

}

string Parser::get_symbol() {
    if(get_command_type() == A_CMD){
        const regex re_symbol_a("@([0-9A-Za-z_\\.\\$\\:]+)");
        smatch base_match;    
        if (std::regex_match(current_cmd, base_match, re_symbol_a)) {
            if (base_match.size() == 2) {
                ssub_match base_sub_match = base_match[1];
                string base = base_sub_match.str();
                return base;
            }
        }
    }
    if(get_command_type() == L_CMD){
        const regex re_symbol_l("\\(([0-9A-Za-z_\\.\\$\\:]+)\\)");
        smatch base_match;    
        if (std::regex_match(current_cmd, base_match, re_symbol_l)) {
            // cout << base_match.size() << endl;
            if (base_match.size() == 2) {
                ssub_match base_sub_match = base_match[1];
                string base = base_sub_match.str();
                // cout << base << endl;
                return base;
            }
        }
    }
    assert(get_command_type() == A_CMD || get_command_type() == L_CMD);
	return "";
}

string Parser::get_dest() {
    if(get_command_type() == C_CMD){
        const regex re_not_dest_c(".*=.*");
        if(!regex_match(current_cmd, re_not_dest_c)) return dest_dict[""];
        const regex re_dest_c("(A?M?D?)=.*;?");
        smatch base_match;    
        if (std::regex_match(current_cmd, base_match, re_dest_c)) {
            if (base_match.size() == 2) {
                ssub_match base_sub_match = base_match[1];
                string base = base_sub_match.str();
                return dest_dict[base];
            }
        }
    }
    assert(get_command_type() == C_CMD);
	return "";
}

string Parser::get_comp() {
    if(get_command_type() == C_CMD){
        const regex re_comp_c1("([01AMD\\-\\+\\!\\&\\|]*);.*");
        smatch base_match1;
        if (std::regex_match(current_cmd, base_match1, re_comp_c1)) {
            if (base_match1.size() == 2) {
                ssub_match base_sub_match = base_match1[1];
                string base = base_sub_match.str();
                if(base != "") return comp_dict[base];
            }
        }

        const regex re_comp_c2(".*=([01AMD\\-\\+\\!\\&\\|]*);?.*");
        smatch base_match2;
        if (std::regex_match(current_cmd, base_match2, re_comp_c2)) {
            if (base_match2.size() == 2) {
                ssub_match base_sub_match = base_match2[1];
                string base = base_sub_match.str();
                return comp_dict[base];
            }
        }
    }
    assert(get_command_type() == C_CMD);
    return "";
}

string Parser::get_jump() {
    if(get_command_type() == C_CMD){
        const regex re_not_jump_c(".*;.*");
        if(!regex_match(current_cmd, re_not_jump_c)) return jump_dict[""];
        const regex re_jump_c(".*;([A-Z]*)");
        smatch base_match;    
        if (std::regex_match(current_cmd, base_match, re_jump_c)) {
            if (base_match.size() == 2) {
                ssub_match base_sub_match = base_match[1];
                string base = base_sub_match.str();
                return jump_dict[base];
            }
        }
    }
    assert(get_command_type() == C_CMD);
    return "";
}
