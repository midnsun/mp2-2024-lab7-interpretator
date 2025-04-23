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
	std::set< variable, variableCMP > variables;
	std::set< function, functionCMP > functions;
public:
	
	void process(const std::vector<std::string>& source);


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

	/*
	void processOld(std::vector<std::string>& source) { // предобработка кода для исполнения
		// Разделение только на ключевые слова и прочие лексемы!! Дальше прочие лексемы обрабатываются как в коде лабы постфикс
		// разделяются int, double, void, if, else, while, return. scan, print - обычные операторы
		// команда завершается или символом { или символом ; или концом строки. Другого не дано
		std::vector <commonLexem* > lexCommand;
		std::string command;
		std::string variableName;
		bool success;
		bool isFunction;
		char arrDim;
		size_t context = 0; // function or condition block we are inside of
		for (size_t lineInd = 0; lineInd < source.size(); ++lineInd) {
			lexCommand.clear();
			// цикл внутри строки на командные слова
			for (size_t i = 0; i < source[lineInd].length(); ++i) {
				// command processing - разделение команды на составляющие лексемы. Если пробел или ( - это новая лексема.
				// обработка конца команды
				if (source[lineInd][i] == ';' || source[lineInd][i] == '{') {
					if (source[lineInd][i] == '{')
						lexCommand.push_back(new specialLexem{ std::string(1, source[lineInd][i]), lineInd, i });
					for (; i < source[lineInd].length(); ++i)
						if (source[lineInd][i] != ' ')
							throw std::runtime_error("Line " + std::to_string(lineInd) + ", symbol " + std::to_string(i) + ": " + source[lineInd][i] + " - bad line ending");
				}
				// обработка конца слова
				if ((source[lineInd][i] == ' ' || specialLexem::isSpecialLexem(source[lineInd][i])) && command != "") {
					// спец символ
					if (specialLexem::isSpecialLexem(source[lineInd][i]))
						lexCommand.push_back(new specialLexem{ std::string(1, source[lineInd][i]), lineInd, i });
					// условный оператор
					if (myoperators::isKeyWordOperator(command)) {
						lexCommand.push_back( new myoperators{ command, lineInd, i - command.length() });
						if (command == "return") context = 0;
						else context = lineInd;
					}
					// тип данных
					if (dataType::isDataType(command)) {
						lexCommand.push_back(new dataType{ command, lineInd, i - command.length() });
						char type = dynamic_cast<dataType*>(lexCommand[lexCommand.size() - 1])->getTypeId();
						for (; i < source[lineInd].length(); ++i) if (source[lineInd][i] != ' ') break; // no spaces
						variableName = "";
						success = false;
						isFunction = false;
						arrDim = 0;
						// пытаемся получить имя
						for (; i < source[lineInd].length(); ++i) {
							if (source[lineInd][i] == ';' && variableName.length() > 0) {
								success = true;
								break;
							}
							else if (source[lineInd][i] == ' ') {
								success = false;
								break;
							}
							// это не переменная а функция!
							else if (source[lineInd][i] == '(') {
								if (context != 0) throw std::runtime_error("Line " + std::to_string(lineInd) + ", symbol " + std::to_string(i) + ": " + source[lineInd][i] + " - nested functions declarations are not allowed");
								lexCommand.push_back(new function{ variableName, lineInd, i - variableName.length()});
								functions.insert(*dynamic_cast<function*>(lexCommand[lexCommand.size() - 1]));
								success = true;
								isFunction = true;
								context = lineInd;
								break;
							}
							// это n-мерный массив
							else if (source[lineInd][i] == '[') {
								arrDim = 1;
							}
							else if (variable::isValidCharForVariable(source[lineInd][i])) {
								if (variableName.size() == 0 && std::isdigit(source[lineInd][i]))
									throw std::runtime_error("Line " + std::to_string(lineInd) + ", symbol " + std::to_string(i) + ": " + source[lineInd][i] + " - variable or function name cannot start with a digit");
								variableName += source[lineInd][i];
							}
							else {
								success = false;
								break;
							}
						}
						// в случае успеха добавляем в переменные
						if (!success) throw std::runtime_error("Line " + std::to_string(lineInd) + ", symbol " + std::to_string(i) + ": " + source[lineInd][i] + " - no valid variable or function name");
						if (!isFunction) {
							lexCommand.push_back(new variable{ variableName, lineInd, i - variableName.length(), type, context, arrDim });
							variables.insert(*dynamic_cast<variable*>(lexCommand[lexCommand.size() - 1]));
						}
						--i;
					}

					command = "";
				}
				program.push_back(lexCommand);
			}
		}
	}
	*/
	// при получении вектора lexem* затем можно сделать так при уточнении конкретной лексемы:
	// lexem* lex = vec[i];
	// vec[i] = new operand {lex->name, lex->pos, lex->ind};
	// delete[] lex;
	interpretator(std::vector<std::string>& source) {
		process(source); // предобработка кода для исполнения
	}

	~interpretator() {
		for (size_t i = 0; i < program.size(); ++i) {
			delete program[i];
		}
	}
};