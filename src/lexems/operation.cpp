#include "operation.h"

std::set<std::string> operations{ "=", "+", "-", "*", "/", "+=", "-=", "*=", "/=", "&&", "||", "==", "<=", "<", ">=", ">", "!=" };

bool operation::isOperation(const std::string& str)
{
	if (operations.find(str) != operations.end())
		return true;
	return false;
}

void operation::showInfo()
{
	std::cout << "operation: " << this->getName() << " " << this->getInd() << " " << this->getPos() << " ";
	return;
}
