#pragma once
#include <iostream>
#include <set>
#include "keyWords.h"

extern std::set<std::string> keyWordOperators;

class operators : public keyWords {
	size_t begin, end;
public:
	static bool isKeyWordOperator(const std::string& str);
	operators(const std::string str, size_t ind, size_t pos, size_t _begin, size_t _end) :
		keyWords(str, ind, pos), begin(_begin), end(_end) {

	}
	virtual void showInfo();
};

std::set<std::string> keyWordOperators{ "if", "else", "while", "return" };

bool operators::isKeyWordOperator(const std::string& str)
{
	if (keyWordOperators.find(str) != keyWordOperators.end()) return true;
	return false;
}

void operators::showInfo()
{
	std::cout << "keyWordOperator: " << this->getName() << " " << this->getInd() << " " << this->getPos() << " " << begin << " " << end << " ";
	return;
}