#pragma once
#include <vector>
#include "constant.h"

class calculator {
	const std::vector<commonLexem*>& data;
	const std::set<variable*, variableCMP>& vars;
	size_t begin, end;
public:
	calculator(const std::vector<commonLexem*>& _data, size_t _begin, size_t _end, const std::set<variable*, variableCMP>& _vars) : data(_data), begin(_begin), end(_end), vars(_vars) {

	}
	// ���������� n ���� ������ � ���� ���������, ���� �� �����	��������� ������ ���������, ��� ���������������� ���������
	// � ������, constant.getValue() ������ ����� ���������� �� ��������, ������� ��� ���������� (double, int ...)
	constant calculate() {

	}
	~calculator() {
		// NO COMMONLEXEM* DELETING!!!!!!!
	}
};