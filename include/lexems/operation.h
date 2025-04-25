#pragma once
#include <string>
#include <iostream>
#include <set>
#include "commonLexem.h"

extern std::set<std::string> operations;

class operation : public commonLexem {
	char priority;
public:
	static bool isOperation(const std::string& str);
	operation(const std::string& str, size_t ind, size_t pos);
	virtual void showInfo() const;
	virtual std::string getClass() const;
	char getPriority() const noexcept;
	void setPriority(char newPriority) noexcept;
};