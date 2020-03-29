#ifndef ___SYMBOLTABLE	
#define ___SYMBOLTABLE
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>


class  SymbolTable
{
public:
	std::unordered_map< std::string, int > um;
	int next_address;

	SymbolTable();
	void add(std::string symbol);
	void add_with_address(std::string symbol, int address);
	bool contain(std::string symbol);
	int get_address(std::string symbol);
};

#endif