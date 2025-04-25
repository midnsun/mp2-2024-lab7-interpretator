#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include "operand.h"

class constant : public operand {
private:

public:
	static bool isValidConstant(const std::string& str);
	static bool isInteger(const std::string& str);
	constant(const std::string& str, size_t ind, size_t pos, char type);
	virtual void showInfo() const;
	virtual std::string getClass() const;
	constant& operator=(const constant& c);
};