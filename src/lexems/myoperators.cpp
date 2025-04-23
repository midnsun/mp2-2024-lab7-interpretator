#include "myoperators.h"

std::set<std::string> keyWordOperators{ "if", "else", "while", "return" };

bool myoperators::isKeyWordOperator(const std::string& str)
{
	if (keyWordOperators.find(str) != keyWordOperators.end()) return true;
	return false;
}

void myoperators::showInfo()
{
	std::cout << "keyWordOperator: " << this->getName() << " " << this->getInd() << " " << this->getPos() << " " << begin << " " << end << " ";
	return;
}