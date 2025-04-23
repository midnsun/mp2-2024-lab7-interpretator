#pragma once
#include <iostream>
#include <set>
#include "keyWords.h"

extern std::set<std::string> dataTypes;

class dataType : public keyWords {
public:
	static bool isDataType(const std::string& str);
	dataType(const std::string str, size_t ind, size_t pos) : keyWords(str, ind, pos) { }
	char getTypeId();
	virtual void showInfo() const;
	virtual std::string getClass() const;
};