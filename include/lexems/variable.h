#pragma once
#include <string>
#include <iostream>
#include "operand.h"

class variable : public operand {
	// �� ����� ��� ������ ������� � ���������� ������� ����� ���� ������� ���������� ��-�� ������� ���������.
	// ����� ����� ������� ��������� ���������� �������������� ���������� �������, ����� ����������� �� ������� � ������� ����� return
	// � ����� ������ ������ ������� �������� ������ ����������, ��������� ������ ���, � ���������� ����������
	char type; // int, double, etc... 0 - int, 1 - double
	//	string context; // what function it belongs. Functions are marked as 0 (as global), 1, 2, 3...
	int arr;
	// � ����� ������� �������� ������ ����� ��������, �� ������� �������� ��������� ��� ���������� � filed = ������ �������
public:
	//	variable(std::string str, size_t ind, size_t pos, char _type, string _context, int _arr) : operand{ str, ind, pos }, type(_type), context(_context), arr(_arr) {}
	variable(std::string str, size_t ind, size_t pos, char _type, int _arr) : operand{ str, ind, pos }, type(_type), arr(_arr) {}
	static bool isValidCharForVariable(char c);
	static bool isValidVariable(const std::string& str);
	void newType(char _newtype);
	char getTypeId() const;
	virtual void showInfo();
};

struct variableCMP {
	bool operator ()(const variable& v1, const variable& v2) const;
};