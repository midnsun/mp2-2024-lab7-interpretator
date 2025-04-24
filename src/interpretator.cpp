#include "interpretator.h"
#include "calculator.h"
#include <stack>

// Идея создать вектор векторов лексем, разбиение первого вектора идет по командам, второго по ключевым словам, т.е. по лексемам
	// Пример:
	// int a = 1;
	// int b = 2;
	// b = a / b;
	// [ ["int", "a", "=", "1"], ["int", "b", "=", "2"], ["b", "=", "a", "/", "b"] ] - 
	// лексемы внутри векторов лексем (команды) внутри программы (вектора команд)
void interpretator::process(const std::vector<std::string>& source)
{
	// 1. Разбиение на слова - строки лексемы
		// 1.1 Разбиение по словам между пробелов
		// 
	std::string word;
	int wordPos, wordLen, tmpwordPos;
	std::vector <std::vector <std::pair <std::string, int> > > strProgram;
	std::vector< std::pair<std::string, int> > strCommand;
	size_t lineInd, pos, wordInd;
	for (lineInd = 0; lineInd < source.size(); ++lineInd) {
		strProgram.push_back(std::vector<std::pair <std::string, int>>());
		pos = 0;
		while (pos < source[lineInd].length()) {
			word = "";
			for (; pos < source[lineInd].length(); ++pos) if (source[lineInd][pos] != ' ') break; // no spaces
			for (; pos < source[lineInd].length(); ++pos) { // words
				if (source[lineInd][pos] == ' ') break;
				word += source[lineInd][pos];
			}
			strProgram[lineInd].push_back(make_pair(word, pos - word.length()));
		}
	}

	// 1.2 Разбиение спец. символами
	for (lineInd = 0; lineInd < strProgram.size(); ++lineInd) {
		strCommand.clear();
		for (wordInd = 0; wordInd < strProgram[lineInd].size(); ++wordInd) {
			word = strProgram[lineInd][wordInd].first;
			wordPos = strProgram[lineInd][wordInd].second;
			tmpwordPos = wordPos;
			for (pos = 0; pos < word.length(); ++pos) {
				if (specialLexem::isSpecialLexem(word[pos])) {
					if (pos - wordPos + tmpwordPos > 0) strCommand.push_back(make_pair(word.substr(wordPos - tmpwordPos, pos - wordPos + tmpwordPos), wordPos));
					strCommand.push_back(make_pair(word.substr(pos, 1), tmpwordPos + pos));
					wordPos = tmpwordPos + pos + 1;
				}
			}
			if (pos - wordPos + tmpwordPos > 0) strCommand.push_back(make_pair(word.substr(wordPos - tmpwordPos, pos - wordPos + tmpwordPos), wordPos));
		}
		strProgram[lineInd] = strCommand;
	}

	// 1.3 Разбиение операторами +, - etc..
	for (lineInd = 0; lineInd < strProgram.size(); ++lineInd) {
		strCommand.clear();
		for (wordInd = 0; wordInd < strProgram[lineInd].size(); ++wordInd) {
			word = strProgram[lineInd][wordInd].first;
			wordPos = strProgram[lineInd][wordInd].second;
			tmpwordPos = wordPos;
			for (pos = 0; pos < word.length(); ++pos) {
				if (!operand::isValidCharForOperand(word[pos])) {
					if (pos - wordPos + tmpwordPos > 0) strCommand.push_back(make_pair(word.substr(wordPos - tmpwordPos, pos - wordPos + tmpwordPos), wordPos));
					for (wordLen = 1; pos + wordLen < word.length(); ++wordLen) if (operand::isValidCharForOperand(word[pos + wordLen])) break;
					strCommand.push_back(make_pair(word.substr(pos, wordLen), tmpwordPos + pos));
					wordPos = tmpwordPos + pos + wordLen;
					pos += wordLen - 1;
				}
			}
			if (pos - wordPos + tmpwordPos > 0) strCommand.push_back(make_pair(word.substr(wordPos - tmpwordPos, pos - wordPos + tmpwordPos), wordPos));
		}
		strProgram[lineInd] = strCommand;
	}

	// variable : type, context, arr; functions: type, begin, end; datatype; myoperators: begin, end
	// special lexem, constant, operation
	// 2. Распределить лексемы по соответствующим классам. Это не оператор, тогда начало с цифры - число, начало с буквы - переменная (функция)

	for (auto& s : standartFuinction) {
		//			functions.insert(*(new function{ s, size_t(-1), size_t(-1), -1, size_t(-1), size_t(-1), size_t(-1), size_t(-1) }));
		functions.insert(new function{ s, 0, 0, -1, 0, 0 });
	}

	size_t begin = 0, end = 0;
	size_t counter = 0;
	int arrCounter = -1;
	char dataTypeAppeared = -1;
	//		string context = "GLOBAL";
	for (lineInd = 0; lineInd < strProgram.size(); ++lineInd) {
		//			program.push_back(vector<commonLexem*>());
		for (wordInd = 0; wordInd < strProgram[lineInd].size(); ++wordInd) {
			word = strProgram[lineInd][wordInd].first;
			wordPos = strProgram[lineInd][wordInd].second;

			// SPECIAL LEXEM
			if (word.length() == 1 && specialLexem::isSpecialLexem(word[0])) program.push_back(new specialLexem{ word, lineInd, size_t(wordPos) });

			// OPERATIONS (+, -, ...)
			else if (operation::isOperation(word)) program.push_back(new operation{ word, lineInd, size_t(wordPos) });

			// DATA TYPES
			else if (dataType::isDataType(word)) {
				// remember its appearance
				dataType* tmpptr = new dataType{ word, lineInd, size_t(wordPos) };
				program.push_back(tmpptr);
				dataTypeAppeared = tmpptr->getTypeId();
			}

			// KEY WORD OPERATORS (if, else ...)
			else if (myoperators::isKeyWordOperator(word)) {
				// ищем открывающую figure скобку, записываем в begin, прибавляем счетчик. Когда нашли закрывающую и счетчик стал нулем - это end
				if (word != "return") {
					int bracketCounter = 0;
					bool exitFlag = false;
					begin = program.size();
					counter = 0;
					for (size_t i = lineInd; i < strProgram.size(); ++i) {
						size_t j;
						if (i == lineInd) j = wordInd;
						else j = 0;
						for (; j < strProgram[i].size(); ++j, ++counter) {
							if (strProgram[i][j].first == "{") {
								if (bracketCounter == 0) {
									//										lineBegin = i;
									//										wordBegin = j;
								}
								++bracketCounter;
							}
							if (strProgram[i][j].first == "}") {
								--bracketCounter;
								if (bracketCounter == 0) {
									end = counter + begin;
									exitFlag = true;
									break;
								}
							}
							if (bracketCounter < 0)  throw std::runtime_error("Line " + std::to_string(i) + ", symbol " + std::to_string(strProgram[i][j].second) + ": " + strProgram[i][j].first + " - Invalid bracket");
						}
						if (exitFlag) break;
					}
				}
				else {
					begin = 0;
					end = 0;
					//						context = "GLOBAL";
				}

				program.push_back(new myoperators{ word, lineInd, size_t(wordPos), begin, end });
			}

			// CONSTANT
			else if (constant::isValidConstant(word)) {
				program.push_back(new constant{ word, lineInd, size_t(wordPos), -1 });
			}

			// VARIABLE OR FUNCTION
			else if (variable::isValidVariable(word)) {
				// function
				if (wordInd < strProgram[lineInd].size() - 1 && strProgram[lineInd][wordInd + 1].first == "(") {
					// ищем открывающую figure скобку, записываем в begin, прибавляем счетчик. Когда нашли закрывающую и счетчик стал нулем - это end

					function* tmpptr = new function{ word, lineInd, size_t(wordPos), dataTypeAppeared, begin, end };
					program.push_back(tmpptr);

					// creating function
					if (dataTypeAppeared != -1) {
						if (functions.find(tmpptr) != functions.end()) throw std::runtime_error("Line " + std::to_string(lineInd) + ", symbol " + std::to_string(wordPos) + ": " + word + " - This function has already exists");

						int bracketCounter = 0;
						counter = 0;
						bool exitFlag = false;
						begin = program.size() - 1;
						for (size_t i = lineInd; i < strProgram.size(); ++i) {
							size_t j;
							if (i == lineInd) j = wordInd;
							else j = 0;
							for (; j < strProgram[i].size(); ++j, ++counter) {
								if (strProgram[i][j].first == "{") {
									if (bracketCounter == 0) {
										//											lineBegin = i;
										//											wordBegin = j;
									}
									++bracketCounter;
								}
								if (strProgram[i][j].first == "}") {
									--bracketCounter;
									if (bracketCounter == 0) {
										end = counter + begin;
										exitFlag = true;
										break;
									}
								}
								if (bracketCounter < 0)  throw std::runtime_error("Line " + std::to_string(i) + ", symbol " + std::to_string(strProgram[i][j].second) + ": " + strProgram[i][j].first + " - Invalid bracket");
							}
							if (exitFlag) break;
						}
						tmpptr->begin = begin;
						tmpptr->end = end;

						functions.insert(tmpptr);
						dataTypeAppeared = -1;
						//							context = word;
					}
					// function is just being called
					else {
						if (functions.find(tmpptr) == functions.end()) throw std::runtime_error("Line " + std::to_string(lineInd) + ", symbol " + std::to_string(wordPos) + ": " + word + " - There is no function with this name");
						tmpptr->type = (*functions.find(tmpptr))->type;
						tmpptr->begin = (*functions.find(tmpptr))->begin;
						tmpptr->end = (*functions.find(tmpptr))->end;
					}
				}
				// variable
				else {
					arrCounter = 0;
					// посчитать размерность массива
//						program[lineInd].push_back(new variable{ word, lineInd, size_t(wordPos), dataTypeAppeared, context, arrCounter });
					program.push_back(new variable{ word, lineInd, size_t(wordPos), dataTypeAppeared, arrCounter });
					dataTypeAppeared = -1;
				}
			}
			else {
				throw std::runtime_error("Line " + std::to_string(lineInd) + ", symbol " + std::to_string(wordPos) + ": " + word + " - Unknown word");
			}
		}
	}
	// получим ситуацию, что новосозданные переменные будут иметь не -1 тип данных, а иначе будут иметь -1. Вот и отличие!

	// Кстати, после заполнения таблица functions больше не нужна, т.к. все позиции перехода записаны в программу. Нужны только стандартные функции.
	// Позиции перехода пишутся в индексах таблицы программы
	// Непонятно, как работать с массивами

	// PRINTING
	for (wordInd = 0; wordInd < program.size(); ++wordInd) {
		std::cout << wordInd << ":	";
		program[wordInd]->showInfo();
		std::cout << std::endl;
	}

	// 3. Записать функции и переменные в соответствующие таблицы, рассмотреть случаи массивов
}

constant interpretator::executeWithoutErrorsHandling(function* func, std::vector<constant> arguments) {
	constant result("##UNNAMED##", -1, -1, func->type);
	size_t pos, argsCounter;
	bool flag = false;
	std::set<variable*, variableCMP> vars;
	variable* var;

	pos = func->begin;
	argsCounter = 0;
	++pos;
	++pos;
	if (program[pos]->getName() == ")") {
		flag = true;
	}

	while (pos < program.size() && !flag) {
		// data type
		++pos;
		// variable
		var = dynamic_cast<variable*>(program[pos]);
		var->setValue(arguments[argsCounter].getValue());
		vars.insert(var);
		++argsCounter;
		++pos;
		// ) or ,
		if (program[pos]->getName() == ")") {
			flag = true;
			break;
		}
		++pos;
	}
	++pos;
	++pos;

	flag = false;
	std::stack<size_t> cycles;

	for (; pos <= func->end; ++pos) {
		// executing
		// находим слово, обозначающее dataType - добавляем в переменные, проверяя, есть ли там такая же. Если есть - ошибка. 
		// находим слово, обозначающее keyWord - выполняем. Jump как обычно выполняем (по стеку). Все просто
	}
}

constant interpretator::execute(function* func, std::vector<constant> arguments) {

	// действия требуются только при ключевых словах, т.е:
	// int, double...
	// while, if, else

	constant result("##UNNAMED##", -1, -1, func->type);
	size_t pos, argsCounter;
	size_t begin, end;
	bool flag = false;
//	std::vector<variable*> argvars; // SET!!!!!!
	std::set<variable*, variableCMP> vars;
	variable* var;

	pos = func->begin;
	argsCounter = 0;
	++pos;
	if (pos >= program.size() || program[pos]->getName() != "(") throw std::runtime_error("Line " + std::to_string(func->getInd()) + ", symbol " + std::to_string(func->getPos()) + ": " + func->getName() + " - No opening bracket");
	++pos;
	if (pos >= program.size()) throw std::runtime_error("Line " + std::to_string(func->getInd()) + ", symbol " + std::to_string(func->getPos()) + ": " + func->getName() + " - Invalid agruments processing: no closing bracket found");
	if (program[pos]->getName() == ")") {
		flag = true;
	}

	while (pos < program.size() && !flag) {
		// data type
		if (program[pos]->getClass() != "dataType") throw std::runtime_error("Line " + std::to_string(program[pos]->getInd()) + ", symbol " + std::to_string(program[pos]->getPos()) + ": " + program[pos]->getName() + " - Data type is missing");
		++pos;
		// variable
		if (pos >= program.size() || program[pos]->getClass() != "variable") throw std::runtime_error("Line " + std::to_string(program[pos]->getInd()) + ", symbol " + std::to_string(program[pos]->getPos()) + ": " + program[pos]->getName() + " - Variable is missing");
		if (argsCounter >= arguments.size()) throw std::runtime_error("Line " + std::to_string(func->getInd()) + ", symbol " + std::to_string(func->getPos()) + ": " + func->getName() + " - Incorrect count of arguments");
		var = dynamic_cast<variable*>(program[pos]);
		if (arguments[argsCounter].getTypeId() != var->getTypeId()) throw std::runtime_error("Line " + std::to_string(var->getInd()) + ", symbol " + std::to_string(var->getPos()) + ": " + var->getName() + " - Incompatible types of arguments");
		if (vars.find(var) != vars.end()) throw std::runtime_error("Line " + std::to_string(var->getInd()) + ", symbol " + std::to_string(var->getPos()) + ": " + var->getName() + " - This variable has already exists");
		var->setValue(arguments[argsCounter].getValue());
		vars.insert(var);
		++argsCounter;
		++pos;
		// ) or ,
		if (pos >= program.size()) throw std::runtime_error("Line " + std::to_string(func->getInd()) + ", symbol " + std::to_string(func->getPos()) + ": " + func->getName() + " - Invalid agruments processing: no closing bracket found");
		else if (program[pos]->getName() == ")") {
			flag = true;
			break;
		}
		else if (program[pos]->getClass() != "specialLexem" || program[pos]->getName() != ",")  throw std::runtime_error("Line " + std::to_string(program[pos]->getInd()) + ", symbol " + std::to_string(program[pos]->getPos()) + ": " + program[pos]->getName() + " - Comma or closing bracket is missing");
		++pos;
	}
	if (!flag) throw std::runtime_error("Line " + std::to_string(func->getInd()) + ", symbol " + std::to_string(func->getPos()) + ": " + func->getName() + " - Invalid agruments processing: no closing bracket found");
	++pos;
	if (pos >= program.size() || program[pos]->getName() != "{") throw std::runtime_error("Line " + std::to_string(func->getInd()) + ", symbol " + std::to_string(func->getPos()) + ": " + func->getName() + " - Invalid agruments processing: no opening figure bracket found");
	++pos;
	if (argsCounter != arguments.size()) throw std::runtime_error("Line " + std::to_string(func->getInd()) + ", symbol " + std::to_string(func->getPos()) + ": " + func->getName() + " - Incorrect count of arguments");
	// setting variables as given arguments
	/*
	if (argvars.size() != arguments.size()) throw std::runtime_error("Line " + std::to_string(func->getInd()) + ", symbol " + std::to_string(func->getPos()) + ": " + func->getName() + " - Incorrect count of arguments");
	for (size_t i = 0; i < arguments.size(); ++i) {
		if (argvars[i]->getTypeId() != arguments[i].getTypeId()) throw std::runtime_error("Line " + std::to_string(argvars[i]->getInd()) + ", symbol " + std::to_string(argvars[i]->getPos()) + ": " + argvars[i]->getName() + " - Incompatible types of arguments");
		argvars[i]->setValue(arguments[i].getValue());
	}
	*/

	flag = false;
	bool returnFlag = false;
	std::stack<size_t> cycles;

	for (; pos <= func->end; ++pos) {
		// executing
		// находим слово, обозначающее dataType - добавляем в переменные, проверяя, есть ли там такая же. Если есть - ошибка. 
		if (program[pos]->getClass() == "dataType") {
			++pos;
			if (pos >= program.size() || program[pos]->getClass() != "variable") throw std::runtime_error("Line " + std::to_string(program[pos]->getInd()) + ", symbol " + std::to_string(program[pos]->getPos()) + ": " + program[pos]->getName() + " - Variable is missing");
			var = dynamic_cast<variable*>(program[pos]);
			if (vars.find(var) != vars.end()) throw std::runtime_error("Line " + std::to_string(var->getInd()) + ", symbol " + std::to_string(var->getPos()) + ": " + var->getName() + " - This variable has already exists");
			vars.insert(var);
			begin = pos++;
		}
		// находим слово, обозначающее keyWord - выполняем. Jump как обычно выполняем (по стеку). Все просто
		else if (program[pos]->getClass() == "myoperators") {
			if (program[pos]->getName() == "return") {
				begin = ++pos;
				returnFlag = true;
			}
			else if (program[pos]->getName() == "while") {

			}
			else if (program[pos]->getName() == "if") {
				
			}
			else {
				throw std::runtime_error("Line " + std::to_string(program[pos]->getInd()) + ", symbol " + std::to_string(program[pos]->getPos()) + ": " + program[pos]->getName() + " - Invalid key word operator");
			}
		}
		// вычисляем арифметические выражения
		while (pos <= func->end && program[pos]->getName() != ";") ++pos;
		end = pos;
		calculator calc(program, begin, end, vars);
		constant tmpResult = calc.calculate();
		if (flag) {
			result = tmpResult;
			break;
		}
	}

	if (result.getTypeId() != func->type) throw std::runtime_error("Line " + std::to_string(var->getInd()) + ", symbol " + std::to_string(var->getPos()) + ": " + var->getName() + " - Incompatible types of arguments");
	return result;
}
