#pragma once
#include <vector>
#include <string>
#include <set>
#include "operation.h"
#include "constant.h"
#include "variable.h"
#include "function.h"
#include "specialLexem.h"
#include "dataType.h"
#include "myoperators.h"
#include <iostream>
#include <sstream>
#include <stdexcept>

class interpretator {
	std::vector <  commonLexem* > program;
	std::set< variable*, variableCMP > GlobalVariables;
	std::set< function*, functionCMP > functions;
	void executePrint(const std::vector<constant>& agruments);
	constant executeCos(const std::vector<constant>& agruments);
	constant executeSin(const std::vector<constant>& agruments);
	constant executeAbs(const std::vector<constant>& agruments);
	constant executeSqrt(const std::vector<constant>& agruments);

public:
	interpretator(std::vector<std::string>& source);
	~interpretator();
	void process(std::vector<std::string> source);
	constant execute(const function const* func, const std::vector<constant>& agruments);
	constant executeWithoutErrorsHandling(const function const* func, const std::vector<constant>& agruments);
	constant startExecute();

	// ÂÛÏÎËÍÅÍÈÅ ÍÀ×ÈÍÀÅÒÑß Ñ ÔÓÍÊÖÈÈ (ïî÷òè) main, çàâåğøàåòñÿ return.
};