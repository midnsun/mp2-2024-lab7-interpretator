#include "constant.h"

constant::constant(const std::string& str, size_t ind, size_t pos, char type) : operand(str, ind, pos, type) { }

bool constant::isValidConstant(const std::string& str) // проверить, это число или нет
{
	char junk;
	double d;
	std::stringstream ss(str);
	return (ss >> d && !(ss >> junk));
}

bool constant::isInteger(const std::string& str)
{
	for (char c : str) if (!isdigit(c)) return false;
	return true;
}

void constant::showInfo() const
{
	std::cout << "constant: " << this->getName() << " " << this->getInd() << " " << this->getPos() << " " << this->getTypeId();
	return;
}

std::string constant::getClass() const { return "constant"; }

constant& constant::operator=(const constant& c) {
	return *this;
}
