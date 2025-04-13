#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <set>
#include <stack>
#include <sstream>

using namespace std;

std::set<std::string> keyWordOperators{"if", "else", "while", "return"};
std::set<std::string> operations{"=", "+", "-", "*", "/", "+=", "-=", "*=", "/=", "&&", "||", "==", "<=", "<", ">=", ">", "!="};
std::set<std::string> dataTypes{"int", "double", "void"};
std::set<char> specialLexems{'(', ')', '{', '}', '[', ']', ',', ';'};
std::set<string> standartFuinction{"sin", "cos", "abs", "sqrt", "print", "scan"};

// ЧТО ТРЕБУЕТСЯ СДЕЛАТЬ:
// 1. СОЗДАТЬ ВИРТУАЛЬНЫЕ ФУНКЦИИ whatTypeOfLexem() КОТОРЫЕ БУДУТ ВОЗВРАЩАТЬ ПЕРЕМЕННАЯ ЭТО, ИЛИ КЛЮЧЕВОЕ СЛОВО, ИЛИ КОНСТАНТА И Т.Д. ДЛЯ УДОБСТВА
// 2. ПУСТЬ ЭТА ФУНКЦИЯ ВОЗВРАЩАЕТ stting НАЗВАНИЕ СВОЕГО КЛАССА
// 3. В ПРОЦЕССИНГЕ - ОТКРЫЫВАЮЩИЕ ЗАКРЫВАЮЩИЕ СКОБКИ
// 4. В ПРОЦЕССИНГЕ - РАЗМЕРНОСТЬ МАССИВА

class commonLexem {
private:
	std::string lex;
	size_t lineIndex;
	size_t linePos;
public:
	// E.g.
	//
	// 0 int A;
	// 1 int B;
	//
	// lexem B has ind = 1, pos = 4
	commonLexem(std::string s, size_t ind, size_t pos) : lex(s), lineIndex(ind), linePos(pos) { // name of lexem, number of line, position in the line
		// Allowed Lexem: 
		// 1. key word:
		// 1.1 data types: int, double, void (functions only), var[size]
		// 1.2 operators: if(){}, else{}, while(){}, print(), scan()
		// 1.3 return
		// 2. operators:
		// 2.1 numeric: +, -, *, /, +=, -=, *=, /=
		// 2.2 logic: &&, ||, ==, !=, <, <=, >, >=
		// 2.3 special: =
		// 3. numbers: without . or e - integer, with . or e - real
		// 4. special symbols: (, ), ;, {, }, [, ], ,, (functions only), " (for print())
		//
		// Rules for program:
		// 1. A new line after each ; or {
		// 2. If, While, Else blocks must be signed with {}
		// 3. C syntax
		// 4. Function with name GLOBAL is prohibited
		//
		// New key words: 
		// JIF (expr) cmd - jump if expression is true
		// JMP cmd - jump always
		//
	}
	std::string getName() const noexcept {
		return lex;
	}
	size_t getInd() const noexcept {
		return lineIndex;
	}
	size_t getPos() const noexcept {
		return linePos;
	}
	virtual ~commonLexem() {}
	virtual void showInfo() = 0;
};

class operation : public commonLexem {
public:
	static bool isOperation(const std::string& str) {
		if (operations.find(str) != operations.end())
			return true;
		return false;
	}
	operation(const string& str, size_t ind, size_t pos) : commonLexem(str, ind, pos) {

	}
	virtual void showInfo() {
		cout << "operation: " << this->getName() << " " << this->getInd() << " " << this->getPos() << " ";
	}
};

class operand : public commonLexem {
public:
	static bool isValidCharForOperand(char c) {
		if (c <= 'z' && c >= 'A' || c <= '9' && c >= '0' || c == '.') return true;
		return false;
	}
	operand(const std::string str, size_t ind, size_t pos) : commonLexem(str, ind, pos) {

	}
	virtual void showInfo() = 0;
};

class constant : public operand {
public:
	static bool isValidConstant(const std::string& str) { // проверить, это число или нет
		char junk;
		double d;
		stringstream ss(str);
		return (ss >> d && !(ss >> junk));
	}
	static bool isInteger(const std::string& str) {
		for (char c : str) if (!isdigit(c)) return false;
		return true;
	}
	constant(const string& str, size_t ind, size_t pos) : operand(str, ind, pos) {

	}
	virtual void showInfo() {
		cout << "constant: " << this->getName() << " " << this->getInd() << " " << this->getPos() << " ";
	}
};

class variable : public operand {
	// по факту для каждой функции и глобальной области будет своя таблица переменных из-за области видимости.
	// Иначе можно создать отдельную переменную принадлежности переменной функции, затем пробегаться по таблице и удалять после return
	// В таком случае внутри функции доступны только переменные, созданные внутри нее, и глобальные переменные
	char type; // int, double, etc... 0 - int, 1 - double
//	string context; // what function it belongs. Functions are marked as 0 (as global), 1, 2, 3...
	int arr;
	// В стеке вызовов хранится только точка возврата, по команде возврата удаляются все переменные с filed = номеру функции
public:
//	variable(std::string str, size_t ind, size_t pos, char _type, string _context, int _arr) : operand{ str, ind, pos }, type(_type), context(_context), arr(_arr) {}
	variable(std::string str, size_t ind, size_t pos, char _type, int _arr) : operand{ str, ind, pos }, type(_type), arr(_arr) {}
	static bool isValidCharForVariable(char c) {
		if (c <= 'z' && c >= 'A' || c <= '9' && c >= '0' || c == '.') return true;
		return false;
	}
	static bool isValidVariable(const std::string& str) {
		if (str.length() == 0) return false;
		if (std::isdigit(str[0])) return false;
		for (auto c : str) 
			if (!isValidCharForVariable(c)) return false;
		return true;
	}
	void newType(char _newtype) {
		type = _newtype;
	}
	char getTypeId() const {
		return type;
	}
	virtual void showInfo() {
//		cout << "variable: " << this->getName() << " " << this->getInd() << " " << this->getPos() << " " << int(type) << " " << context << " " << arr << " ";
		cout << "variable: " << this->getName() << " " << this->getInd() << " " << this->getPos() << " " << int(type) << " " << arr << " ";
	}
};
struct variableCMP {
	bool operator ()(const variable& v1, const variable& v2) const {
		std::string name1 = v1.getName();
		std::string name2 = v2.getName();
		for (size_t i = 0; i < std::min(name1.size(), name2.size()); ++i) {
			if (name1[i] < name2[i]) return true;
			if (name1[i] > name2[i]) return false;
		}
		return name1.size() < name2.size();
	}
};

class function : public commonLexem {
public:
	char type;
	size_t lineBegin, lineEnd, wordBegin, wordEnd;
public:
	function(const std::string str, size_t ind, size_t pos, char _type, size_t _linebegin, size_t _lineend, size_t _wordbegin, size_t _wordend) : 
		commonLexem(str, ind, pos), type(_type), lineBegin(_linebegin), lineEnd(_lineend), wordBegin(_wordbegin), wordEnd(_wordend) {

	}
	virtual void showInfo() {
		cout << "function: " << this->getName() << " " << this->getInd() << " " << this->getPos() << " " << int(type) << " " << lineBegin << " " << lineEnd << " " << wordBegin << " " << wordEnd << " ";
	}
};

struct functionCMP {
	bool operator()(const function& f1, const function& f2) const {
		std::string name1 = f1.getName();
		std::string name2 = f2.getName();
		for (size_t i = 0; i < std::min(name1.size(), name2.size()); ++i) {
			if (name1[i] < name2[i]) return true;
			if (name1[i] > name2[i]) return false;
		}
		return name1.size() < name2.size();
	}
};

class specialLexem : public commonLexem {
public:
	static bool isSpecialLexem(char c) {
		if (specialLexems.find(c) != specialLexems.end()) return true;
		return false;
	}
	specialLexem(const std::string str, size_t ind, size_t pos) : commonLexem(str, ind, pos) {

	}
	virtual void showInfo() {
		cout << "special lexem: " << this->getName() << " " << this->getInd() << " " << this->getPos() << " ";
	}
};

class keyWords : public commonLexem { // template for me
public:
	keyWords(const std::string str, size_t ind, size_t pos) : commonLexem(str, ind, pos) {

	}
	virtual void showInfo() = 0;
};

class dataType : public keyWords {
public:
	static bool isDataType(const std::string& str) {
		if (dataTypes.find(str) != dataTypes.end()) return true;
		return false;
	}
	dataType(const std::string str, size_t ind, size_t pos) : keyWords(str, ind, pos) {

	}
	char getTypeId() {
		if (this->getName() == "void") return 0;
		else if (this->getName() == "int") return 1;
		else if (this->getName() == "double") return 2;
		else return -1;
	}
	virtual void showInfo() {
		cout << "dataType: " << this->getName() << " " << this->getInd() << " " << this->getPos() << " ";
	}
};

class operators : public keyWords {
	size_t lineBegin, lineEnd, wordBegin, wordEnd;
public:
	static bool isKeyWordOperator(const std::string& str) {
		if (keyWordOperators.find(str) != keyWordOperators.end()) return true;
		return false;
	}
	operators(const std::string str, size_t ind, size_t pos, size_t _linebegin, size_t _lineend, size_t _wordbegin, size_t _wordend) :
		keyWords(str, ind, pos), lineBegin(_linebegin), lineEnd(_lineend), wordBegin(_wordbegin), wordEnd(_wordend) {

	}
	virtual void showInfo() {
		cout << "keyWordOperator: " << this->getName() << " " << this->getInd() << " " << this->getPos() << " " << lineBegin << " " << lineEnd << " " << wordBegin << " " << wordEnd << " ";
	}
};

class calculator {
	std::vector<commonLexem*> data;
public:
	calculator(const vector<commonLexem*>& _data) : data(_data) {

	}
	~calculator() {
		// NO COMMONLEXEM* DELETING!!!!!!!
	}
};

class interpretator {
	std::vector < std::vector < commonLexem* > > program;
	std::set< variable, variableCMP > variables;
	std::set< function, functionCMP > functions;
public:
	// Идея создать вектор векторов лексем, разбиение первого вектора идет по командам, второго по ключевым словам, т.е. по лексемам
	// Пример:
	// int a = 1;
	// int b = 2;
	// b = a / b;
	// [ ["int", "a", "=", "1"], ["int", "b", "=", "2"], ["b", "=", "a", "/", "b"] ] - 
	// лексемы внутри векторов лексем (команды) внутри программы (вектора команд)
	void process(const std::vector<std::string>& source) {
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

		// variable : type, context, arr; functions: type, begin, end; datatype; operators: begin, end
		// special lexem, constant, operation
		// 2. Распределить лексемы по соответствующим классам. Это не оператор, тогда начало с цифры - число, начало с буквы - переменная (функция)

		for (auto& s : standartFuinction) {
//			functions.insert(*(new function{ s, size_t(-1), size_t(-1), -1, size_t(-1), size_t(-1), size_t(-1), size_t(-1) }));
			functions.insert(*(new function{ s, 0, 0, -1, 0, 0, 0, 0 }));
		}

		size_t lineBegin = -1, lineEnd = -1, wordBegin = -1, wordEnd = -1;
		int arrCounter = -1;
		char dataTypeAppeared = -1;
//		string context = "GLOBAL";
		for (lineInd = 0; lineInd < strProgram.size(); ++lineInd) {
			program.push_back(vector<commonLexem*>());
			for (wordInd = 0; wordInd < strProgram[lineInd].size(); ++wordInd) {
				word = strProgram[lineInd][wordInd].first;
				wordPos = strProgram[lineInd][wordInd].second;

				// SPECIAL LEXEM
				if (word.length() == 1 && specialLexem::isSpecialLexem(word[0])) program[lineInd].push_back(new specialLexem{ word, lineInd, size_t(wordPos) });

				// OPERATIONS (+, -, ...)
				else if (operation::isOperation(word)) program[lineInd].push_back(new operation{ word, lineInd, size_t(wordPos) });

				// DATA TYPES
				else if (dataType::isDataType(word)) {
					// remember its appearance
					dataType* tmpptr = new dataType{ word, lineInd, size_t(wordPos) };
					program[lineInd].push_back(tmpptr);
					dataTypeAppeared = tmpptr->getTypeId();
				}

				// KEY WORD OPERATORS (if, else ...)
				else if (operators::isKeyWordOperator(word)) {
					// ищем открывающую figure скобку, записываем в begin, прибавляем счетчик. Когда нашли закрывающую и счетчик стал нулем - это end
					if (word != "return") {
						int bracketCounter = 0;
						bool exitFlag = false;
						for (size_t i = lineInd; i < strProgram.size(); ++i) {
							size_t j;
							if (i == lineInd) j = wordInd;
							else j = 0;
							for (; j < strProgram[i].size(); ++j) {
								if (strProgram[i][j].first == "{") {
									if (bracketCounter == 0) {
										lineBegin = i;
										wordBegin = j;
									}
									++bracketCounter;
								}
								if (strProgram[i][j].first == "}") {
									--bracketCounter;
									if (bracketCounter == 0) {
										lineEnd = i;
										wordEnd = j;
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
						lineBegin = 0;
						lineEnd = 0;
						wordBegin = 0;
						wordEnd = 0;
//						context = "GLOBAL";
					}

					program[lineInd].push_back(new operators{ word, lineInd, size_t(wordPos), lineBegin, lineEnd, wordBegin, wordEnd });
				}

				// CONSTANT
				else if (constant::isValidConstant(word)) {
					program[lineInd].push_back(new constant{ word, lineInd, size_t(wordPos) });
				}

				// VARIABLE OR FUNCTION
				else if (variable::isValidVariable(word)) {
					// function
					if (wordInd < strProgram[lineInd].size() - 1 && strProgram[lineInd][wordInd + 1].first == "(") {
						// ищем открывающую figure скобку, записываем в begin, прибавляем счетчик. Когда нашли закрывающую и счетчик стал нулем - это end

						function* tmpptr = new function{ word, lineInd, size_t(wordPos), dataTypeAppeared, lineBegin, lineEnd, wordBegin, wordEnd };
						program[lineInd].push_back(tmpptr);
						
						// creating function
						if (dataTypeAppeared != -1) {
							if (functions.find(*tmpptr) != functions.end()) throw std::runtime_error("Line " + std::to_string(lineInd) + ", symbol " + std::to_string(wordPos) + ": " + word + " - This function has already exists");

							int bracketCounter = 0;
							bool exitFlag = false;
							for (size_t i = lineInd; i < strProgram.size(); ++i) {
								size_t j;
								if (i == lineInd) j = wordInd;
								else j = 0;
								for (; j < strProgram[i].size(); ++j) {
									if (strProgram[i][j].first == "{") {
										if (bracketCounter == 0) {
											lineBegin = i;
											wordBegin = j;
										}
										++bracketCounter;
									}
									if (strProgram[i][j].first == "}") {
										--bracketCounter;
										if (bracketCounter == 0) {
											lineEnd = i;
											wordEnd = j;
											exitFlag = true;
											break;
										}
									}
									if (bracketCounter < 0)  throw std::runtime_error("Line " + std::to_string(i) + ", symbol " + std::to_string(strProgram[i][j].second) + ": " + strProgram[i][j].first + " - Invalid bracket");
								}
								if (exitFlag) break;
							}
							tmpptr->lineBegin = lineBegin;
							tmpptr->lineEnd = lineEnd;
							tmpptr->wordBegin = wordBegin;
							tmpptr->wordEnd = wordEnd;

							functions.insert(*tmpptr);
							dataTypeAppeared = -1;
//							context = word;
						}
						// function is just being called
						else {
							if (functions.find(*tmpptr) == functions.end()) throw std::runtime_error("Line " + std::to_string(lineInd) + ", symbol " + std::to_string(wordPos) + ": " + word + " - There is no function with this name");
							tmpptr->type = (*functions.find(*tmpptr)).type;
							tmpptr->lineBegin = (*functions.find(*tmpptr)).lineBegin;
							tmpptr->lineEnd = (*functions.find(*tmpptr)).lineEnd;
							tmpptr->wordBegin = (*functions.find(*tmpptr)).wordBegin;
							tmpptr->wordEnd = (*functions.find(*tmpptr)).wordEnd;
						}
					}
					// variable
					else {
						arrCounter = 0;
						// посчитать размерность массива
//						program[lineInd].push_back(new variable{ word, lineInd, size_t(wordPos), dataTypeAppeared, context, arrCounter });
						program[lineInd].push_back(new variable{ word, lineInd, size_t(wordPos), dataTypeAppeared, arrCounter });
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

		for (lineInd = 0; lineInd < program.size(); ++lineInd) {
			for (wordInd = 0; wordInd < program[lineInd].size(); ++wordInd) {
				program[lineInd][wordInd]->showInfo();
			}
			cout << endl;
		}


		// 3. Записать функции и переменные в соответствующие таблицы, рассмотреть случаи массивов
	}

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
					if (operators::isKeyWordOperator(command)) {
						lexCommand.push_back( new operators{ command, lineInd, i - command.length() });
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
			for (size_t j = 0; j < program[i].size(); ++j) {
				delete program[i][j];
			}
		}
	}
};


void printFileContent(std::vector<std::string>& v) {
	for (int i = 0; i < v.size(); ++i) {
		cout << v[i] << endl;
	}
	cout << endl;
}

void checkFileContent(std::vector<std::string>& v) { //
	std::string str;
	for (size_t strInd = 0; strInd < v.size(); ++strInd) {
		str = v[strInd];
		for (size_t i = 0; i < str.length(); ++i) {
			if (str[i] == ';') for (; i < str.length(); ++i) if (str[i] != ' ') throw std::runtime_error("Line " + std::to_string(strInd) + ", symbol " + std::to_string(i) + ": " + str[i]);
		}
	}
}

int main() {
	std::fstream file("D:/Users/maksi/Desktop/dz_38/UNN/mp2-2024-lab7-interpretator/code/code.cpp");
	std::string str;
	std::vector<std::string> file_content;
	while (std::getline(file, str)) {
		file_content.push_back(str);
	}
//	printFileContent(file_content);
	try {
		interpretator program(file_content);
	}
	catch (std::exception& e) {
		std::cout << e.what() << endl;
	}

	return 0;
}