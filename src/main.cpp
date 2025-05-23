#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "interpretator.h"

using namespace std;

// ��� ��������� �������:
// 1. ������� ����������� ������� whatTypeOfLexem() ������� ����� ���������� ���������� ���, ��� �������� �����, ��� ��������� � �.�. ��� ��������
// 2. ����� ��� ������� ���������� stting �������� ������ ������
// 3. � ����������� - ����������� ����������� ������ (DONE)
// 4. � ����������� - ����������� �������

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
	std::fstream file("../../code/code.cpp");
	std::string str;
	std::vector<std::string> file_content;
	while (std::getline(file, str)) {
		file_content.push_back(str);
	}
//	printFileContent(file_content);
//	try {
		interpretator program(file_content);
		constant res = program.startExecute();
//	}
//	catch (std::exception& e) {
//		std::cout << e.what() << std::endl;
//	}
	return 0;
}