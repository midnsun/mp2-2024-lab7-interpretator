#include "operand.h"

operand::operand(const std::string str, size_t ind, size_t pos, char _type) : commonLexem(str, ind, pos), type(_type), value(nullptr) { }

bool operand::isValidCharForOperand(char c)
{
	if (c <= 'z' && c >= 'A' || c <= '9' && c >= '0' || c == '.') return true;
	return false;
}

char operand::getTypeId() const noexcept
{
	return type; 
}

void operand::setTypeId(const char& _type) noexcept
{
	type = _type;
	return;
}

void* operand::getValue() const {
	return value;
}

#include <iostream>

void operand::setValue(void* v) {
	if (v != nullptr)
	{
		value = malloc(sizeof(v));
		memcpy(value, v, sizeof(v));
	}
	else
	{
		v = nullptr;
	}
}

operand::~operand() {
	delete value;
}

bool operand::isTrue() const {
	bool result = false;

	if (type == 1) result = *(int*)value;
	else if (type == 2) result = *(double*)value;
	else if (type == 3) result = (*(std::string*)value).length();

	return result;
}
