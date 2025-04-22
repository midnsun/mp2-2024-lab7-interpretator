#pragma once
#include <vector>
#include "constant.h"

class calculator {
	std::vector<commonLexem*> data;
public:
	calculator(const std::vector<commonLexem*>& _data) : data(_data) {

	}
	// возвращает n байт пам€ти в виде константы, одно из полей	константы должно объ€снить, как интерпретировать результат
	// в идеале, constant.getValue() должен сразу возвращать то значение, которое там получилось (double, int ...)
	constant calculate() {

	}
	~calculator() {
		// NO COMMONLEXEM* DELETING!!!!!!!
	}
};