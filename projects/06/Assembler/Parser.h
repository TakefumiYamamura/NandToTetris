#ifndef ___PARSER	
#define ___PARSER
#include <iostream>
#include <vector>
#include <string>
#include <fstream> 
#include <sstream>
#include <unordered_map>
#include <queue>
#include <set>
#include <map>
#include <random>
#include <regex>

enum COMMANDTYPE
{
    A_CMD    = 0,
    C_CMD    = 1,
    L_CMD    = 2,
    INVALID_CMD    = 2,
};



class Parser
{
public:
	int cmd_idx;
	std::unordered_map<std::string, std::string> dest_dict;
	std::unordered_map<std::string, std::string> comp_dict;
	std::unordered_map<std::string, std::string> jump_dict;

	std::string current_cmd;
	std::vector<std::string> cmds;

	Parser(std::string file_name);
	Parser();
	bool advance();
	bool reset();

	COMMANDTYPE get_command_type();
	std::string get_symbol();
	bool contain_symbol();
	std::string get_dest();
	std::string get_comp();
	std::string get_jump();
};

#endif