#pragma once
#include <iostream>
#include <set>
#include "keyWords.h"

extern std::set<std::string> keyWordOperators;

class myoperators : public keyWords {
	size_t begin, end;
public:
	static bool isKeyWordOperator(const std::string& str);
	myoperators(const std::string str, size_t ind, size_t pos, size_t _begin, size_t _end);
	virtual void showInfo() const;
	virtual std::string getClass() const;
	size_t getBegin() const noexcept;
	size_t getEnd() const noexcept;
};