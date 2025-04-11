#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <set>
#include <stack>

using namespace std;

static std::set<std::string> keyWordOperators;
static std::set<std::string> operations;
static std::set<std::string> dataTypes{"int", "double", "void"};
static std::set<char> specialLexems;

// ЧТО ТРЕБУЕТСЯ СДЕЛАТЬ:
// 1. СОЗДАТЬ ТАБЛИЦУ ДЛЯ ПОДДЕРЖИВАЕМЫХ ОПЕРАТОРОВ, СПЕЦИАЛЬНЫХ СИМВОЛОВ И КЛЮЧЕВЫХ СЛОВ В ГЛОБАЛЬНОМ ПОЛЕ ЧТОБЫ КАЖДЫЙ РАЗ НЕ ПЕРЕДЕЛЫВАТЬ КЛАССЫ
// 2. ЗАМЕНИТЬ ФУНКЦИИ isSomething()

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
};

class lexem : public commonLexem { // template for Stas's class of numeric operators, constants and variables
public:
	lexem(const std::string str, size_t ind, size_t pos) : commonLexem(str, ind, pos) {

	}
};

class operation : public lexem {
public:
	static bool isOperation(const std::string& str) {
		if (str == "=" || str == "+" || str == "-" || str == "*" || str == "/" || str == "+=" || str == "-=" || str == "*=" || str == "/=" || str == "=" ||
			str == "&&" || str == "||" || str == "==" || str == "<=" || str == "<" || str == ">=" || str == ">" || str == "!=")
			return true;
		return false;
	}
};

class operand : public lexem {
public:
	static bool isValidCharForOperand(char c) {
		if (c > 'z' && c < 'A' && c > '9' && c < '0' && c != '.') return false;
		return true;
	}
	operand(const std::string str, size_t ind, size_t pos) : lexem(str, ind, pos) {

	}
};

class constant : public operand {
public:

	static bool isConstant(const std::string& str) { // проверить, это число или нет

	}
	static bool isInteger(const std::string& str) {
		for (char c : str) if (!isdigit(c)) return false;
		return true;
	}
};

class variable : public operand {
	// по факту для каждой функции и глобальной области будет своя таблица переменных из-за области видимости.
	// Иначе можно создать отдельную переменную принадлежности переменной функции, затем пробегаться по таблице и удалять после return
	// В таком случае внутри функции доступны только переменные, созданные внутри нее, и глобальные переменные
	char type; // int, double, etc... 0 - int, 1 - double
	char context; // what function it belongs. Functions are marked as 0 (as global), 1, 2, 3...
	int arr;
	// В стеке вызовов хранится только точка возврата, по команде возврата удаляются все переменные с filed = номеру функции
public:
	variable(std::string str, size_t ind, size_t pos, char _type, char _context, int _arr) : operand{ str, ind, pos }, type(_type), context(_context), arr(_arr) {

	}
	static bool isValidCharForVariable(char c) {
		if (c > 'z' && c < 'A' && c > '9' && c < '0') return false;
		return true;
	}
	static bool isValidVariable(const std::string& str) {
		if (str.length() == 0) return false;
		if (std::isdigit(str[0])) return false;
		for (auto c : str) 
			if (!isValidCharForVariable(c)) return false;
		return true;
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
	function(const std::string str, size_t ind, size_t pos) : commonLexem(str, ind, pos) {

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
		if (c == '(' || c == ')' || c == ',' || c == '{' || c == '}' || c == '[' || c == ']' || c == ';') return true;
		return false;
	}
	specialLexem(const std::string str, size_t ind, size_t pos) : commonLexem(str, ind, pos) {

	}
};

class keyWords : public commonLexem { // template for me
public:
	keyWords(const std::string str, size_t ind, size_t pos) : commonLexem(str, ind, pos) {

	}
};

class dataType : public keyWords {
public:
	static bool isDataType(const std::string& str) {
		if (str == "double" || str == "int" || str == "void") return true;
		return false;
	}
	dataType(const std::string str, size_t ind, size_t pos) : keyWords(str, ind, pos) {

	}
	char getTypeId() {
		if (this->getName() == "int") return 0;
		else if (this->getName() == "double") return 1;
		else return -1;
	}
};

class operators : public keyWords {
public:
	static bool isKeyWordOperator(const std::string& str) {
		if (str == "if" || str == "else" || str == "while" || str == "return") return true;
		return false;
	}
	operators(const std::string str, size_t ind, size_t pos) : keyWords(str, ind, pos) {

	}
};

class interpretator {
	std::vector < std::vector < commonLexem* > > program;
	std::set< variable, variableCMP > variables;
	std::set< function, functionCMP > functions;
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
		int wordPos;
		std::vector <std::vector <std::pair <std::string, int> > > strProgram;
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
		std::vector< std::pair<std::string, int> > strCommand;
		for (lineInd = 0; lineInd < program.size(); ++lineInd) {
			strCommand.clear();
			for (wordInd = 0; wordInd < program[lineInd].size(); ++wordInd) {
				word = strProgram[lineInd][wordInd].first;
				wordPos = strProgram[lineInd][wordInd].second;
				for (pos = 0; pos < word.length(); ++pos) {
					if (specialLexem::isSpecialLexem(word[pos])) {
						strCommand.push_back(make_pair(word.substr(wordPos, pos), wordPos));
						strCommand.push_back(make_pair(word.substr(wordPos + pos, 1), wordPos + pos));
						wordPos = pos + 1;
					}
				}
			}
			strProgram[lineInd] = strCommand;
		}

		// 1.3 Разбиение операторами +, - etc..

	}
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
	} // при получении вектора lexem* затем можно сделать так при уточнении конкретной лексемы:
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
	std::fstream file("D:/Users/maksi/Desktop/dz_38/UNN/mp2-2024-lab5-interpretator/code/code.cpp");
	std::string str;
	std::vector<std::string> file_content;
	while (std::getline(file, str)) {
		file_content.push_back(str);
	}
	printFileContent(file_content);

	return 0;
}