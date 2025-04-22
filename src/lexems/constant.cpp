#include "constant.h"

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

void constant::showInfo()
{
	std::cout << "constant: " << this->getName() << " " << this->getInd() << " " << this->getPos() << " ";
	return;
}
