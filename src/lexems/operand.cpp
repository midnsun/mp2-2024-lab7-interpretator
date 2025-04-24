#include "operand.h"

bool operand::isValidCharForOperand(char c)
{
	if (c <= 'z' && c >= 'A' || c <= '9' && c >= '0' || c == '.') return true;
	return false;
}

char operand::getTypeId() const { return type; }

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
