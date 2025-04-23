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


	// ���������� ���������� � ������� main, ����������� return.
	// � ����� ������, ���������� � ����������� �������
	/*
	void execute(function* func, std::vector<constant> arguments) {

		// �������� ��������� ������ ��� �������� ������, �.�:
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
	void processOld(std::vector<std::string>& source) { // ������������� ���� ��� ����������
		// ���������� ������ �� �������� ����� � ������ �������!! ������ ������ ������� �������������� ��� � ���� ���� ��������
		// ����������� int, double, void, if, else, while, return. scan, print - ������� ���������
		// ������� ����������� ��� �������� { ��� �������� ; ��� ������ ������. ������� �� ����
		std::vector <commonLexem* > lexCommand;
		std::string command;
		std::string variableName;
		bool success;
		bool isFunction;
		char arrDim;
		size_t context = 0; // function or condition block we are inside of
		for (size_t lineInd = 0; lineInd < source.size(); ++lineInd) {
			lexCommand.clear();
			// ���� ������ ������ �� ��������� �����
			for (size_t i = 0; i < source[lineInd].length(); ++i) {
				// command processing - ���������� ������� �� ������������ �������. ���� ������ ��� ( - ��� ����� �������.
				// ��������� ����� �������
				if (source[lineInd][i] == ';' || source[lineInd][i] == '{') {
					if (source[lineInd][i] == '{')
						lexCommand.push_back(new specialLexem{ std::string(1, source[lineInd][i]), lineInd, i });
					for (; i < source[lineInd].length(); ++i)
						if (source[lineInd][i] != ' ')
							throw std::runtime_error("Line " + std::to_string(lineInd) + ", symbol " + std::to_string(i) + ": " + source[lineInd][i] + " - bad line ending");
				}
				// ��������� ����� �����
				if ((source[lineInd][i] == ' ' || specialLexem::isSpecialLexem(source[lineInd][i])) && command != "") {
					// ���� ������
					if (specialLexem::isSpecialLexem(source[lineInd][i]))
						lexCommand.push_back(new specialLexem{ std::string(1, source[lineInd][i]), lineInd, i });
					// �������� ��������
					if (myoperators::isKeyWordOperator(command)) {
						lexCommand.push_back( new myoperators{ command, lineInd, i - command.length() });
						if (command == "return") context = 0;
						else context = lineInd;
					}
					// ��� ������
					if (dataType::isDataType(command)) {
						lexCommand.push_back(new dataType{ command, lineInd, i - command.length() });
						char type = dynamic_cast<dataType*>(lexCommand[lexCommand.size() - 1])->getTypeId();
						for (; i < source[lineInd].length(); ++i) if (source[lineInd][i] != ' ') break; // no spaces
						variableName = "";
						success = false;
						isFunction = false;
						arrDim = 0;
						// �������� �������� ���
						for (; i < source[lineInd].length(); ++i) {
							if (source[lineInd][i] == ';' && variableName.length() > 0) {
								success = true;
								break;
							}
							else if (source[lineInd][i] == ' ') {
								success = false;
								break;
							}
							// ��� �� ���������� � �������!
							else if (source[lineInd][i] == '(') {
								if (context != 0) throw std::runtime_error("Line " + std::to_string(lineInd) + ", symbol " + std::to_string(i) + ": " + source[lineInd][i] + " - nested functions declarations are not allowed");
								lexCommand.push_back(new function{ variableName, lineInd, i - variableName.length()});
								functions.insert(*dynamic_cast<function*>(lexCommand[lexCommand.size() - 1]));
								success = true;
								isFunction = true;
								context = lineInd;
								break;
							}
							// ��� n-������ ������
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
						// � ������ ������ ��������� � ����������
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
	// ��� ��������� ������� lexem* ����� ����� ������� ��� ��� ��������� ���������� �������:
	// lexem* lex = vec[i];
	// vec[i] = new operand {lex->name, lex->pos, lex->ind};
	// delete[] lex;
	interpretator(std::vector<std::string>& source) {
		process(source); // ������������� ���� ��� ����������
	}

	~interpretator() {
		for (size_t i = 0; i < program.size(); ++i) {
			delete program[i];
		}
	}
};