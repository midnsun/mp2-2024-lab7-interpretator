#include "interpretator.h"
#include "calculator.h"
#include <stack>
#include <algorithm>

// Идея создать вектор векторов лексем, разбиение первого вектора идет по командам, второго по ключевым словам, т.е. по лексемам
	// Пример:
	// int a = 1;
	// int b = 2;
	// b = a / b;
	// [ ["int", "a", "=", "1"], ["int", "b", "=", "2"], ["b", "=", "a", "/", "b"] ] - 
	// лексемы внутри векторов лексем (команды) внутри программы (вектора команд)
void interpretator::process(std::vector<std::string> source)
{
	std::vector<std::string> strParsing;
	std::string word;
	int wordPos, wordLen, tmpwordPos;
	std::vector <std::vector <std::pair <std::string, int> > > strProgram;
	std::vector< std::pair<std::string, int> > strCommand;
	size_t lineInd, pos, wordInd;
	//0. Замена строк константами типа 0s ... ns
	for (lineInd = 0; lineInd < source.size(); ++lineInd) {
		size_t startPos;
		for (pos = 0; pos < source[lineInd].length(); ++pos) {
			if (source[lineInd][pos] == '"') {
				startPos = pos;
				// Found a string - add to vector of strings
				while (source[lineInd][++pos] != '"')
					if (pos + 1 >= source[lineInd].length()) throw std::runtime_error("Line " + std::to_string(lineInd) + ", symbol " + std::to_string(startPos) + ": " + source[lineInd][startPos] + " - No closing quotes");
				strParsing.push_back(source[lineInd].substr(startPos + 1, pos - startPos - 1));
				// change it to 0s ... 10s
				source[lineInd].replace(startPos, pos - startPos + 1, std::to_string(strParsing.size() - 1) + "s");
				pos = startPos;
				continue;
			}
		}
	}


	// 1. Разбиение на слова - строки лексемы
		// 1.1 Разбиение по словам между пробелов
		// 

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
		//abs может возвращить int и double, но имя у него одно
		if (s == "cos" || s == "sin" || s == "sqrt"/* || s == "abs"*/) functions.insert(new function{s, 0, 0, 2, 0, 0});
		else functions.insert(new function{ s, 0, 0, 0, 0, 0 });
		
	}

	// тут сделать парсинг for. Принцип: 
	// for (expr1; expr2; expr3) { doSomething(); } ->
	// expr1;
	// while(expr2) {
	//  doSomething();
	// expr3;
	// }
	size_t begin = 0, end = 0, exprEnd = 0;
	size_t counter = 0, semicolon = 0;

	strCommand.clear();
	for (lineInd = 0; lineInd < strProgram.size(); ++lineInd) {
		for (wordInd = 0; wordInd < strProgram[lineInd].size(); ++wordInd) {
			if (strProgram[lineInd][wordInd].first == "for") {
				if (wordInd + 1 >= strProgram[lineInd].size() || strProgram[lineInd][wordInd + 1].first != "(") throw std::runtime_error("Line " + std::to_string(lineInd) + ", symbol " + std::to_string(strProgram[lineInd][wordInd].second) + ": " + strProgram[lineInd][wordInd].first + " - No expressions");
				++counter;
				for (exprEnd = wordInd + 2; exprEnd < strProgram[lineInd].size(); ++exprEnd) {
					if (strProgram[lineInd][exprEnd].first == "(") ++counter;
					if (strProgram[lineInd][exprEnd].first == ")") --counter;
					if (strProgram[lineInd][exprEnd].first == ";") ++semicolon;
					if (counter == 0) {
						++exprEnd;
						break;
					}
				}
				if (counter != 0) throw std::runtime_error("Line " + std::to_string(lineInd) + ", symbol " + std::to_string(strProgram[lineInd][wordInd].second) + ": " + strProgram[lineInd][wordInd].first + " - No closing bracket");
				if (semicolon != 2) throw std::runtime_error("Line " + std::to_string(lineInd) + ", symbol " + std::to_string(strProgram[lineInd][wordInd].second) + ": " + strProgram[lineInd][wordInd].first + " - No expressions");

				strProgram[lineInd][wordInd].first = "while";
				//expr1
				begin = wordInd + 2; end = wordInd + 2;
				while (strProgram[lineInd][end].first != ";") {
					strCommand.push_back(strProgram[lineInd][end]);
					++end;
				}
				strCommand.push_back(strProgram[lineInd][end++]);
				strProgram[lineInd].erase(strProgram[lineInd].begin() + begin, strProgram[lineInd].begin() + end);
				strProgram[lineInd].insert(strProgram[lineInd].begin() + wordInd, strCommand.begin(), strCommand.end());
				strCommand.clear();

				//expr2
				begin = end;
				while (strProgram[lineInd][end].first != ";") {
					++end;
				}
				strProgram[lineInd].erase(strProgram[lineInd].begin() + end);

				//expr3
				begin = end;
				while (strProgram[lineInd][end].first != ")") {
					strCommand.push_back(strProgram[lineInd][end]);
					++end;
				}
				strProgram[lineInd].erase(strProgram[lineInd].begin() + begin, strProgram[lineInd].begin() + end);
				strCommand.push_back(std::make_pair(";", -1));

				int bracketCounter = 0;
				bool exitFlag = false;
				size_t i;
				for (i = lineInd; i < strProgram.size(); ++i) {
					size_t j;
					if (i == lineInd) j = wordInd;
					else j = 0;
					for (; j < strProgram[i].size(); ++j) {
						if (strProgram[i][j].first == "{") {
							++bracketCounter;
						}
						if (strProgram[i][j].first == "}") {
							--bracketCounter;
							if (bracketCounter == 0) {
								end = j;
								exitFlag = true;
								break;
							}
						}
						if (bracketCounter < 0) throw std::runtime_error("Line " + std::to_string(i) + ", symbol " + std::to_string(strProgram[i][j].second) + ": " + strProgram[i][j].first + " - Invalid bracket");
					}
					if (exitFlag) break;
				}
				strProgram[i].insert(strProgram[i].begin() + end, strCommand.begin(), strCommand.end());

				//end
				strCommand.clear();
				begin = 0; end = 0; exprEnd = 0; counter = 0; semicolon = 0;
			}
		}
	}

	int arrCounter = 0;
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
//					begin = program.size();
					counter = 0;
					for (size_t i = lineInd; i < strProgram.size(); ++i) {
						size_t j;
						if (i == lineInd) j = wordInd;
						else j = 0;
						for (; j < strProgram[i].size(); ++j, ++counter) {
							if (strProgram[i][j].first == "{") {
								if (bracketCounter == 0) {
									begin = counter + program.size();
								}
								++bracketCounter;
							}
							if (strProgram[i][j].first == "}") {
								--bracketCounter;
								if (bracketCounter == 0) {
									end = counter + program.size();
									exitFlag = true;
									break;
								}
							}
							if (bracketCounter < 0) throw std::runtime_error("Line " + std::to_string(i) + ", symbol " + std::to_string(strProgram[i][j].second) + ": " + strProgram[i][j].first + " - Invalid bracket");
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
//					program[program.size() - 1]->showInfo(); ///
//					std::cout << std::endl;
					dataTypeAppeared = -1;
				}
			}
			else if (word[word.length() - 1] == 's' && constant::isInteger(word.substr(0, word.length() - 1))) {
				constant* tmpstr = new constant{ word, lineInd, size_t(wordPos), 3 };
				size_t tmpind = std::stoi(word.substr(0, word.length() - 1));
				tmpstr->setValue(strParsing[tmpind]);
				program.push_back(tmpstr);
			}
			else if (word != "\t") {
				throw std::runtime_error("Line " + std::to_string(lineInd) + ", symbol " + std::to_string(wordPos) + ": " + word + " - Unknown word");
			}
		}
	}
	// получим ситуацию, что новосозданные переменные будут иметь не -1 тип данных, а иначе будут иметь -1. Вот и отличие!

	// Кстати, после заполнения таблица functions больше не нужна, т.к. все позиции перехода записаны в программу. Нужны только стандартные функции.
	// Позиции перехода пишутся в индексах таблицы программы
	// Непонятно, как работать с массивами

//	for (wordInd = 0; wordInd < program.size(); ++wordInd) {
//		std::cout << wordInd << ":	";
//		program[wordInd]->showInfo();
//		/*if (program[wordInd]->getClass() == "constant" || program[wordInd]->getClass() == "variable")
//			std::cout << " | " << (int)dynamic_cast<constant*>(program[wordInd])->getTypeId() << " | ";*/
//		std::cout << std::endl;
//	}

	// Реализуем систему JMP:
	myoperators* tmpKeyWordOperator;
	size_t tmpKeyWordOperatorPos;
	bool elseFlag = false;
	bool elifFlag = false;

	for (pos = 0; pos < program.size(); ++pos) {
		if (program[pos]->getClass() == "myoperators") {
			if (program[pos]->getName() == "while") {
				tmpKeyWordOperatorPos = pos;
				tmpKeyWordOperator = dynamic_cast<myoperators*>(program[pos]);
				// Прыжок если не выполнилось условие
				pos = tmpKeyWordOperator->getBegin();
				delete program[pos];
				program[pos] = new myoperators{"JMP", tmpKeyWordOperator->getEnd() + 1, tmpKeyWordOperator->getEnd() + 1, tmpKeyWordOperator->getEnd() + 1, tmpKeyWordOperator->getEnd() + 1 };
				// Прыжок в начало цикла
				pos = tmpKeyWordOperator->getEnd();
				delete program[pos];
				program[pos] = new myoperators{ "JMP", tmpKeyWordOperatorPos, tmpKeyWordOperatorPos, tmpKeyWordOperatorPos, tmpKeyWordOperatorPos };
				pos = tmpKeyWordOperatorPos;
			}

			if (program[pos]->getName() == "if") {
				elseFlag = false;
				elifFlag = true;
				tmpKeyWordOperatorPos = pos;
				// Вот тут посчитать конец последнего блока. Потом выполнять все нижеперечисленное в цикле.
				size_t endOfLastBlock;
				while (elifFlag || elseFlag) {
					elifFlag = false;
					tmpKeyWordOperator = dynamic_cast<myoperators*>(program[pos]);
					pos = tmpKeyWordOperator->getBegin();
					endOfLastBlock = tmpKeyWordOperator->getEnd() + 1;
					pos = endOfLastBlock; // Block iteration
					if (elseFlag) break;
					if (tmpKeyWordOperator->getEnd() + 1 < program.size() && program[tmpKeyWordOperator->getEnd() + 1]->getName() == "else") elseFlag = true;
					if (tmpKeyWordOperator->getEnd() + 1 < program.size() && program[tmpKeyWordOperator->getEnd() + 1]->getName() == "elif") elifFlag = true;
				}

				// обработка if
				elifFlag = false;
				pos = tmpKeyWordOperatorPos;
				tmpKeyWordOperator = dynamic_cast<myoperators*>(program[pos]);
				pos = tmpKeyWordOperator->getBegin();
				// Прыжок если не выполнилось условие (на следующий else, но после слова else или на слово elif)
				delete program[pos];
				program[pos] = new myoperators{ "JMP", tmpKeyWordOperator->getEnd() + 1, tmpKeyWordOperator->getEnd() + 1, tmpKeyWordOperator->getEnd() + 1, tmpKeyWordOperator->getEnd() + 1 };
				// Прыжок в конце if в конец блока
				pos = tmpKeyWordOperator->getEnd();
				delete program[pos];
				program[pos] = new myoperators{ "JMP", endOfLastBlock, endOfLastBlock, endOfLastBlock, endOfLastBlock };
				pos += 1;

				// обработка elif
				if (pos < program.size() && program[pos]->getName() == "elif") elifFlag = true;
				while (elifFlag) {
					elifFlag = false;
					tmpKeyWordOperator = dynamic_cast<myoperators*>(program[pos]);
					pos = tmpKeyWordOperator->getBegin();
					if (tmpKeyWordOperator->getEnd() + 1 < program.size() && program[tmpKeyWordOperator->getEnd() + 1]->getName() == "elif") elifFlag = true;
					// Прыжок если не выполнилось условие (на следующий else, но после слова else или на слово elif)
					delete program[pos];
					program[pos] = new myoperators{ "JMP", tmpKeyWordOperator->getEnd() + 1, tmpKeyWordOperator->getEnd() + 1, tmpKeyWordOperator->getEnd() + 1, tmpKeyWordOperator->getEnd() + 1 };
					// Прыжок в конце elif в конец блока
					pos = tmpKeyWordOperator->getEnd();
					delete program[pos];
					program[pos] = new myoperators{ "JMP", endOfLastBlock, endOfLastBlock, endOfLastBlock, endOfLastBlock };
					pos += 1; // block iteration
				}

				// обработка else (Больше не требуется!)
//				pos = tmpKeyWordOperator->getEnd();
//				if (elseFlag) {
//					tmpKeyWordOperator = dynamic_cast<myoperators*>(program[pos + 1]);
//					delete program[pos];
//					program[pos] = new myoperators{ "JMP", endOfLastBlock, endOfLastBlock, endOfLastBlock, endOfLastBlock };
//				}

				pos = tmpKeyWordOperatorPos;
			}
		}
	}

	// Обработка массивов.
	variable* var;
	for (wordInd = 0; wordInd < program.size(); ++wordInd) {
		if (program[wordInd]->getClass() == "variable") {
			var = dynamic_cast<variable*>(program[wordInd]);
			if (var->getTypeId() == -1) continue;
			arrCounter = 0;
			while (wordInd + 1 < program.size() && program[wordInd + 1]->getName() == "[") {
				counter = 1;
				wordInd += 2;
				while (counter != 0 && wordInd < program.size()) {
					if (program[wordInd]->getName() == "[") ++counter;
					if (program[wordInd]->getName() == "]") --counter;
					++wordInd;
				}
				if (counter != 0) throw std::runtime_error("Line " + std::to_string(program[wordInd]->getInd()) + ", symbol " + std::to_string(program[wordInd]->getPos()) + ": " + program[wordInd]->getName() + " - No closing ]");
				++arrCounter;
				--wordInd;
			}
			var->setArr(arrCounter);
		}
	}

	// PRINTING
	for (wordInd = 0; wordInd < program.size(); ++wordInd) {
		std::cout << wordInd << ":	";
		program[wordInd]->showInfo();
		/*if (program[wordInd]->getClass() == "constant" || program[wordInd]->getClass() == "variable")
			std::cout << " | " << (int)dynamic_cast<constant*>(program[wordInd])->getTypeId() << " | ";*/
		std::cout << std::endl;
	}
	std::cout << std::endl;
//	lineInd = 0;
//	for (wordInd = 0; wordInd < program.size(); ++wordInd) {
//		if (lineInd != program[wordInd]->getInd()) {
//			lineInd = program[wordInd]->getInd();
//			std::cout << std::endl;
//		}
//		std::cout << program[wordInd]->getName() << " ";
//	}

	// 3. Записать функции и переменные в соответствующие таблицы, рассмотреть случаи массивов
}

constant interpretator::executeWithoutErrorsHandling(const function const* func, const std::vector<constant>& arguments) {
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
	return result;
}

constant interpretator::startExecute()
{
	function* tmpptr = new function("main", -1, -1, -1, -1, -1);
	if (functions.find(tmpptr) == functions.end()) throw std::runtime_error("main not found"); //main не принимает аргументы
	return execute(*functions.find(tmpptr), std::vector<constant>());
}

void interpretator::executePrint(const std::vector<constant>& arguments)
{
	for (size_t i = 0; i < arguments.size(); i++)
	{
		if (arguments[i].getValue() == nullptr)
		{
			throw std::runtime_error("Line " + std::to_string(arguments[i].getInd()) + ", symbol " + std::to_string(arguments[i].getPos())
				+ ": " + arguments[i].getName() + " - an uninitialized variable");
		}
		if (arguments[i].getTypeId() == 1)
		{
			std::cout << *(int*)arguments[i].getValue() << " ";
		}
		else if (arguments[i].getTypeId() == 2)
		{
			std::cout << *(double*)arguments[i].getValue() << " ";
		}
		else if (arguments[i].getTypeId() == 3)
		{
			std::cout << *reinterpret_cast<std::string*>(arguments[i].getValue()) << " ";
		}
	}
	std::cout << std::endl;
	return;
}

constant interpretator::executeCos(const std::vector<constant>& agruments)
{
	if (agruments.size() == 0) throw std::runtime_error("not enough arguments");
	if (agruments.size() > 1) throw std::runtime_error("There are too many arguments");
	if (agruments[0].getTypeId() == 3 || agruments[0].getTypeId() == -1) throw std::runtime_error("invalid type of argument");
	constant result("##UNNAMED##", -1, -1, 2);
	if (agruments[0].getTypeId() == 1)
	{
		double t = std::cos(*(int*)(agruments[0].getValue()));
		result.setValue(&t);
	}
	if (agruments[0].getTypeId() == 2)
	{
		double t = std::cos(*(double*)(agruments[0].getValue()));
		result.setValue(&t);
	}
	return result;
}

constant interpretator::executeSin(const std::vector<constant>& agruments)
{
	if (agruments.size() == 0) throw std::runtime_error("not enough arguments");
	if (agruments.size() > 1) throw std::runtime_error("There are too many arguments");
	if (agruments[0].getTypeId() == 3 || agruments[0].getTypeId() == -1) throw std::runtime_error("invalid type of argument");
	constant result("##UNNAMED##", -1, -1, 2);
	if (agruments[0].getTypeId() == 1)
	{
		double t = std::sin(*(int*)(agruments[0].getValue()));
		result.setValue(&t);
	}
	if (agruments[0].getTypeId() == 2)
	{
		double t = std::sin(*(double*)(agruments[0].getValue()));
		result.setValue(&t);
	}
	return result;
}

constant interpretator::executeAbs(const std::vector<constant>& agruments)
{
	if (agruments.size() == 0) throw std::runtime_error("not enough arguments");
	if (agruments.size() > 1) throw std::runtime_error("There are too many arguments");
	if (agruments[0].getTypeId() == 3 || agruments[0].getTypeId() == -1) throw std::runtime_error("invalid type of argument");
	constant result("##UNNAMED##", -1, -1, agruments[0].getTypeId());
	if (agruments[0].getTypeId() == 1)
	{
		int t = std::abs(*(int*)(agruments[0].getValue()));
		result.setValue(&t);
	}
	if (agruments[0].getTypeId() == 2)
	{
		double t = std::abs(*(double*)(agruments[0].getValue()));
		result.setValue(&t);
	}
	return result;
}

constant interpretator::executeSqrt(const std::vector<constant>& agruments)
{
	if (agruments.size() == 0) throw std::runtime_error("not enough arguments");
	if (agruments.size() > 1) throw std::runtime_error("There are too many arguments");
	if (agruments[0].getTypeId() == 3 || agruments[0].getTypeId() == -1) throw std::runtime_error("invalid type of argument");
	constant result("##UNNAMED##", -1, -1, 2);
	if (agruments[0].getTypeId() == 1)
	{
		double t = std::sqrt(*(int*)(agruments[0].getValue()));
		result.setValue(&t);
	}
	if (agruments[0].getTypeId() == 2)
	{
		double t = std::sqrt(*(double*)(agruments[0].getValue()));
		result.setValue(&t);
	}
	return result;
}

interpretator::interpretator(std::vector<std::string>& source) {
	process(source); // предобработка кода для исполнения
}

interpretator::~interpretator() {
	for (size_t i = 0; i < program.size(); ++i) {
		delete program[i];
	}
}

constant interpretator::execute(const function const* func, const std::vector<constant>& arguments) {

	// действия требуются только при ключевых словах, т.е:
	// int, double...
	// while, if, else
	
	constant result("##UNNAMED##", -1, -1, func->type);
	if (standartFuinction.find(func->getName()) != standartFuinction.end())
	{
		if (func->getName() == "print") executePrint(arguments);
		try
		{
			if (func->getName() == "cos") return executeCos(arguments);
			if (func->getName() == "sin") return executeSin(arguments);
			if (func->getName() == "sqrt") return executeSqrt(arguments);
			if (func->getName() == "abs") return executeAbs(arguments);
		}
		catch (std::exception& e)
		{
			throw std::runtime_error("Line " + std::to_string(func->getInd()) + ", symbol " + std::to_string(func->getPos()) + ": " + func->getName() + " - " + e.what());
		}
		return result;
	}
	constant tmpResult = result;
	size_t pos, argsCounter, counter;
	size_t begin, end, arrBegin, arrEnd;
	std::vector<int> arrCounter;
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
		//var = dynamic_cast<variable*>(program[pos]);
		var = new variable(*dynamic_cast<variable*>(program[pos]));
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
	myoperators* tmpKeyWord;
	for (; pos < func->end - 1; ++pos) {
		// executing
		// находим слово, обозначающее dataType - добавляем в переменные, проверяя, есть ли там такая же. Если есть - ошибка. 
		begin = pos;
		if (program[pos]->getClass() == "dataType") {
			++pos;
			if (pos >= program.size() || program[pos]->getClass() != "variable") throw std::runtime_error("Line " + std::to_string(program[pos]->getInd()) + ", symbol " + std::to_string(program[pos]->getPos()) + ": " + program[pos]->getName() + " - Variable is missing");
			//var = dynamic_cast<variable*>(program[pos]);
			var = new variable(*dynamic_cast<variable*>(program[pos]));
			if (vars.find(var) != vars.end()) throw std::runtime_error("Line " + std::to_string(var->getInd()) + ", symbol " + std::to_string(var->getPos()) + ": " + var->getName() + " - This variable has already exists");
			vars.insert(var);
			if (var->getSizes().size() > 0) {
				pos += 1; // first [
				while (arrCounter.size() < var->getSizes().size()) {
					if (program[pos]->getName() != "[") throw std::runtime_error("Line " + std::to_string(program[pos]->getInd()) + ", symbol " + std::to_string(program[pos]->getPos()) + ": " + program[pos]->getName() + " - Missing [");
					pos += 1; // pos probably [
					arrBegin = pos; // symbol after [
					counter = 1;
					while (counter != 0 && pos < func->end) {
						if (program[pos]->getName() == "[") ++counter;
						if (program[pos]->getName() == "]") --counter;
						++pos;
					}
					arrEnd = pos - 1; // pos - symbol after ], probably [
					calculator calc(program, arrBegin, arrEnd, vars);
					tmpResult = calc.calculate(this);
					arrCounter.push_back(*(int*)tmpResult.getValue());
				}
				var->setSizes(arrCounter);
			}
			begin = pos++;
		}
		// находим слово, обозначающее keyWord - выполняем. Jump как обычно выполняем (по стеку). Все просто
		else if (program[pos]->getClass() == "myoperators") {
			if (program[pos]->getName() == "else") continue;
			if (program[pos]->getName() == "return") {
				begin = ++pos;
				returnFlag = true;
			}
			else if (program[pos]->getName() == "while" || program[pos]->getName() == "if" || program[pos]->getName() == "elif") {
				tmpKeyWord = dynamic_cast<myoperators*>(program[pos]);
				calculator calc(program, pos + 1, tmpKeyWord->getBegin(), vars);
				tmpResult = calc.calculate(this);
				pos = tmpKeyWord->getBegin() + tmpResult.isTrue() - 1; // if true, avoid JMP operator by increasing value on 1
				continue;
			}
			else if (program[pos]->getName() == "JMP") {
				tmpKeyWord = dynamic_cast<myoperators*>(program[pos]);
				pos = tmpKeyWord->getInd();
				--pos;
				continue;
			}
			else {
				throw std::runtime_error("Line " + std::to_string(program[pos]->getInd()) + ", symbol " + std::to_string(program[pos]->getPos()) + ": " + program[pos]->getName() + " - Invalid key word operator");
			}
		}
		else if (program[pos]->getName() == "}" || program[pos]->getName() == "{") continue;
		// вычисляем арифметические выражения
		while (pos < func->end && program[pos]->getName() != ";") ++pos;
		end = pos;
		calculator calc(program, begin, end, vars);
		tmpResult = calc.calculate(this);
		if (returnFlag) {
			result = tmpResult;
			break;
		}
	}

	if (result.getTypeId() != func->type) throw std::runtime_error("Line " + std::to_string(func->getInd()) + ", symbol " + std::to_string(func->getPos()) + ": " + func->getName() + " - Incompatible types of arguments");
	return result;
}