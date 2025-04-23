#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include "operand.h"

class constant : public operand {
public:
	static bool isValidConstant(const std::string& str);
	static bool isInteger(const std::string& str);
	constant(const std::string& str, size_t ind, size_t pos) : operand(str, ind, pos) { }
	virtual void showInfo();
};