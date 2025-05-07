#pragma once
#include <string>
#include <iostream>
#include "operand.h"

class variable : public operand {
	// �� ����� ��� ������ ������� � ���������� ������� ����� ���� ������� ���������� ��-�� ������� ���������.
	// ����� ����� ������� ��������� ���������� �������������� ���������� �������, ����� ����������� �� ������� � ������� ����� return
	// � ����� ������ ������ ������� �������� ������ ����������, ��������� ������ ���, � ���������� ����������
	//	string context; // what function it belongs. Functions are marked as 0 (as global), 1, 2, 3...
	int arr;
	// � ����� ������� �������� ������ ����� ��������, �� ������� �������� ��������� ��� ���������� � filed = ������ �������
public:
	//	variable(std::string str, size_t ind, size_t pos, char _type, string _context, int _arr) : operand{ str, ind, pos }, type(_type), context(_context), arr(_arr) {}
	variable(std::string str, size_t ind, size_t pos, char _type, int _arr);
	variable(const variable& var);
	static bool isValidCharForVariable(char c);
	static bool isValidVariable(const std::string& str);
	virtual void showInfo() const;
	virtual std::string getClass() const;
};

struct variableCMP {
	bool operator ()(const variable* v1, const variable* v2) const;
};