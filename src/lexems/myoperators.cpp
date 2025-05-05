#include "myoperators.h"

std::set<std::string> keyWordOperators{ "if", "else", "elif", "while", "return", "for" }; // + JMP

myoperators::myoperators(const std::string str, size_t ind, size_t pos, size_t _begin, size_t _end) :
	keyWords(str, ind, pos), begin(_begin), end(_end) {

}

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