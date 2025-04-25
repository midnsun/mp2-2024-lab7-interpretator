#pragma once
#include <vector>
#include "interpretator.h"

class calculator {
	const std::vector<commonLexem*>& data;
	const std::set<variable*, variableCMP>& vars;
	size_t begin, end;
public:
	calculator(const std::vector<commonLexem*>& _data, size_t _begin, size_t _end, const std::set<variable*, variableCMP>& _vars);
	~calculator();
	// возвращает n байт пам€ти в виде константы, одно из полей	константы должно объ€снить, как интерпретировать результат
	// в идеале, constant.getValue() должен сразу возвращать то значение, которое там получилось (double, int ...)
	constant calculate(interpretator* inter);
	
};