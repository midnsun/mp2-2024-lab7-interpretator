#pragma once
#include <string>
#include "commonLexem.h"


class operand : public commonLexem {
public:
	static bool isValidCharForOperand(char c);
	operand(const std::string str, size_t ind, size_t pos) : commonLexem(str, ind, pos) { }
	virtual void showInfo() = 0;
};