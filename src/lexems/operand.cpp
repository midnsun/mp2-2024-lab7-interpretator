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
void operand::setValue(void* v) {
	value = v;
}

operand::~operand() {
	delete value;
}

bool operand::isTrue() const {
	bool result = false;
	return false;
}
