#include "variable.h"

variable::variable(std::string str, size_t ind, size_t pos, char _type, int _arr) : operand(str, ind, pos, _type), arr(_arr) {}

variable::variable(const variable& var) : operand(var.getName(), var.getInd(), var.getPos(), var.getTypeId()), arr(var.arr)
{
	this->setValue(var.getValue());
}

bool variable::isValidCharForVariable(char c)
{
	if (c <= 'z' && c >= 'a' || c <= 'Z' && c >= 'A' || c <= '9' && c >= '0') return true;
	return false;
}

bool variable::isValidVariable(const std::string& str)
{
	if (str.length() == 0) return false;
	if (std::isdigit(str[0])) return false;
	for (auto c : str)
		if (!isValidCharForVariable(c)) return false;
	return true;
}

void variable::showInfo() const
{
	std::cout << "class variable name: " << this->getName() << " LineIndex: " << this->getInd() << " PosIndex: " << this->getPos() << " type: " << this->getTypeId() << /*" " << arr << */" ";
	return;
}

std::string variable::getClass() const { return "variable"; }

bool variableCMP::operator()(const variable* v1, const variable* v2) const
{
	std::string name1 = v1->getName();
	std::string name2 = v2->getName();
	for (size_t i = 0; i < std::min(name1.size(), name2.size()); ++i) {
		if (name1[i] < name2[i]) return true;
		if (name1[i] > name2[i]) return false;
	}
	return name1.size() < name2.size();
}
