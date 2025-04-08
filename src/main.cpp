#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>

using namespace std;

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
	commonLexem(std::string& s, size_t ind, size_t pos) : lex(s), lineIndex(ind), linePos(pos) { // name of lexem, number of line, position in the line
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
		// 4. special symbols: (, ), ;, {, }, , , (functions only), " (for print())
		//
		// Rules for program:
		// 1. If, While, Else blocks must be signed with {}
		// 2. C syntax
		//
		// New key words: 
		// JIF (expr) line - jump if expression is true
		// JMP line - jump always
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
};

class lexem : public commonLexem { // template for Stas's class of numeric operators, constants and variables

};

class operation : public lexem {

};

class operand : public lexem {

};

class constant : public operand {

};

class variable : public operand { 
	// по факту дл€ каждой функции и глобальной области будет сво€ таблица переменных из-за области видимости.
	// »наче можно создать отдельную переменную принадлежности переменной функции, затем пробегатьс€ по таблице и удал€ть после return
	char type; // int, double, etc...
};

class keyWords : public commonLexem { // for me

};

class dataType : public keyWords {

};

class operators : public keyWords {

};

class interpretator {
	std::vector < std::vector <commonLexem*> > program;
	// »де€ создать вектор векторов лексем, разбиение первого вектора идет по командам, второго по ключевым словам, т.е. по лексемам
	// ѕример:
	// int a = 1;
	// int b = 2;
	// b = a / b;
	// [ ["int", "a", "=", "1"], ["int", "b", "=", "2"], ["b", "=", "a", "/", "b"] ] - 
	// лексемы внутри векторов лексем (команды) внутри программы (вектора команд)
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