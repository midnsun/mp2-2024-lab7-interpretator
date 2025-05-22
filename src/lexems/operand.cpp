#include <iostream>
#include "operand.h"

operand::operand(const std::string str, size_t ind, size_t pos, char _type) : commonLexem(str, ind, pos), type(_type), value(nullptr) { }

operand::operand(const operand& op) : commonLexem(op.getName(), op.getInd(), op.getPos())
{
	type = op.getTypeId();
	setValue(op.getValue());
}

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

void operand::copyValue(void* v)
{
	value = v;
	return;
}


//обязательно сначала задавать тип операнда и только потом устанавливать значение!
void operand::setValue(const void* v) {
	
	if (v == nullptr) return;
	//delete value;
	if (getTypeId() == 1)
	{
		value = new int(*(int*)v);
	}
	else if (getTypeId() == 2)
	{
		value = new double(*(double*)v);
	}
	else if (getTypeId() == 3)
	{
		//std::cout << "string " << *((std::string*)v) << std::endl;
		value = new std::string(*(std::string*)v);
	}
	else
	{
		value = nullptr;
	}
}

void operand::setValue(const std::string& s) {
	if (getTypeId() == 3)
	{
		value = new std::string(s);
	}
	else {
		value = nullptr;
	}
}

operand::~operand() {
	if (getTypeId() != 3)
		delete value;
}

bool operand::isTrue() const {
	bool result = false;

	if (type == 1) result = *(int*)value != 0;
	else if (type == 2) result = std::abs(*(double*)value) > 1e-14;
	else if (type == 3) result = (*(std::string*)value).length() != 0;

	return result;
}
