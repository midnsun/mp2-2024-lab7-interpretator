#pragma once
#include <vector>
#include "constant.h"

class calculator {
	std::vector<commonLexem*> data;
public:
	calculator(const std::vector<commonLexem*>& _data) : data(_data) {

	}
	// ���������� n ���� ������ � ���� ���������, ���� �� �����	��������� ������ ���������, ��� ���������������� ���������
	// � ������, constant.getValue() ������ ����� ���������� �� ��������, ������� ��� ���������� (double, int ...)
	constant calculate() {

	}
	~calculator() {
		// NO COMMONLEXEM* DELETING!!!!!!!
	}
};