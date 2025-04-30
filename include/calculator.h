#pragma once
#include <vector>
#include "interpretator.h"

class calculator {
	const std::vector<commonLexem*>& data;
	const std::set<variable*, variableCMP>& vars;
	size_t begin, end;
	std::vector<commonLexem*> toPostfix(const std::vector<commonLexem*>& expr);
	operand* calcPlus(operand* v1, operand* v2);
	operand* calcBinaryNegative(operand* v1, operand* v2);
	operand* calcUnaryNegative(operand* v1);
	operand* calcMultiplication(operand* v1, operand* v2);
	operand* calcDivision(operand* v1, operand* v2);
	operand* calcDivisionWithRemainder(operand* v1, operand* v2);
	operand* calcAnd(operand* v1, operand* v2);
	operand* calcOr(operand* v1, operand* v2);
	operand* calcLess(operand* v1, operand* v2);
	operand* calcMore(operand* v1, operand* v2);
	operand* calclessEqual(operand* v1, operand* v2);
	operand* calcMoreEqual(operand* v1, operand* v2);
	operand* calcEqually(operand* v1, operand* v2);
	operand* calcNotEqually(operand* v1, operand* v2);
	operand* assignment(operand* v1, operand* v2);
	operand* addAndAssign(operand* v1, operand* v2);
	operand* NegativeAndAssign(operand* v1, operand* v2);
	operand* MultiplicationAndAssign(operand* v1, operand* v2);
	operand* DivisionAndAssign(operand* v1, operand* v2);
	operand* DivisionWithRemainderAndAssign(operand* v1, operand* v2);
	void executeScan(const size_t& sBegin, const size_t& sEnd);
	std::vector<commonLexem*> calculatingFunctions(interpretator* inter);
	void checkUnaryNegative();
	void initialConstantAndVarisble();
	
	operand* calcArithmetic(const std::vector<commonLexem*>& expr);
public:
	calculator(const std::vector<commonLexem*>& _data, size_t _begin, size_t _end, const std::set<variable*, variableCMP>& _vars);
	~calculator();
	// возвращает n байт пам€ти в виде константы, одно из полей	константы должно объ€снить, как интерпретировать результат
	// в идеале, constant.getValue() должен сразу возвращать то значение, которое там получилось (double, int ...)
	constant calculate(interpretator* inter);
	
};