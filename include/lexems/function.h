#pragma once
#include <string>
#include <iostream>
#include <set>
#include "commonLexem.h"

extern std::set<std::string> standartFuinction;

class function : public commonLexem {
public:
	char type;
	size_t begin, end;
public:
	function(const std::string str, size_t ind, size_t pos, char _type, size_t _begin, size_t _end) :
		commonLexem(str, ind, pos), type(_type), begin(_begin), end(_end) {

	}
	virtual void showInfo() const;
	virtual std::string getClass() const;
};

struct functionCMP {
	bool operator()(const function* f1, const function* f2) const;
};