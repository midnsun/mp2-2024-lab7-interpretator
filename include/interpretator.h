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
//	std::set< variable, variableCMP > variables;
	std::set< function*, functionCMP > functions;
	void executePrint(const std::vector<constant>& agruments);

public:
	interpretator(std::vector<std::string>& source);
	~interpretator();
	void process(const std::vector<std::string>& source);
	constant execute(const function const* func, const std::vector<constant>& agruments);
	constant executeWithoutErrorsHandling(const function const* func, const std::vector<constant>& agruments);
	constant startExecute();

	// ВЫПОЛНЕНИЕ НАЧИНАЕТСЯ С ФУНКЦИИ main, завершается return.
	// В общем случае, начинается с прозвольной функции
	/*
	void execute(function* func, std::vector<constant> arguments) {

		// действия требуются только при ключевых словах, т.е:
		// int, double...
		// while, if, else


		size_t lineInd, pos, wordInd;
		bool returnFlag = false;
		bool flag = false;
		std::vector<variable*> vars;

		lineInd = func->lineBegin;
		wordInd = func->wordBegin;
		++wordInd;

		while (lineInd < program.size()) {
			if (program[lineInd][wordInd]->getName() == ")") {
				flag = true;
				break;
			}


			++lineInd;
		}

		if (!flag) throw std::runtime_error("Line " + std::to_string(func->lineBegin) + ", symbol " + std::to_string(func->wordEnd) + ": " + func->getName() + " - Invalid agruments processing");

		flag = false;
		returnFlag = false;

		for (; lineInd <= func->lineEnd; ++lineInd) {
			for (wordInd = 0; wordInd < program[lineInd].size(); ++wordInd) {
				if (lineInd == func->lineEnd && wordInd > func->wordEnd) {
					throw std::runtime_error("Line " + std::to_string(func->lineBegin) + ", symbol " + std::to_string(func->wordEnd) + ": " + func->getName() + " - No return in this function");
				}
				if (!flag) {
					flag = true;
					wordInd = pos;
				}

				// executing

			}
			if (returnFlag) break;
		}
	}*/

	// при получении вектора lexem* затем можно сделать так при уточнении конкретной лексемы:
	// lexem* lex = vec[i];
	// vec[i] = new operand {lex->name, lex->pos, lex->ind};
	// delete[] lex;
};