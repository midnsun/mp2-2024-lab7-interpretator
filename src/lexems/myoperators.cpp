#include "myoperators.h"

std::set<std::string> keyWordOperators{ "if", "else", "while", "return" }; // + JMP

bool myoperators::isKeyWordOperator(const std::string& str)
{
	if (keyWordOperators.find(str) != keyWordOperators.end()) return true;
	return false;
}

void myoperators::showInfo() const
{
	std::cout << "keyWordOperator: " << this->getName() << " " << this->getInd() << " " << this->getPos() << " " << begin << " " << end << " ";
	return;
}

std::string myoperators::getClass() const { return "myoperators"; }

size_t myoperators::getBegin() const noexcept {
	return begin;
}
size_t myoperators::getEnd() const noexcept {
	return end;
}