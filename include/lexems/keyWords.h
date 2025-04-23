#pragma once 
#include <string>
#include "commonLexem.h"

class keyWords : public commonLexem { // template for me
public:
	keyWords(const std::string str, size_t ind, size_t pos) : commonLexem(str, ind, pos) {

	}
	virtual void showInfo() = 0;
};

