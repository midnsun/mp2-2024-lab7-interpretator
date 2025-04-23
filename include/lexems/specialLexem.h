#pragma once
#include <string>
#include <iostream>
#include <set>
#include "commonLexem.h"

extern std::set<char> specialLexems;

class specialLexem : public commonLexem {
public:
	static bool isSpecialLexem(char c);
	specialLexem(const std::string str, size_t ind, size_t pos) : commonLexem(str, ind, pos) { }
	virtual void showInfo() const;
	virtual std::string getClass() const;
};