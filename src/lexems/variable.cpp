#include "variable.h"

bool variable::isValidCharForVariable(char c)
{
	if (c <= 'z' && c >= 'A' || c <= '9' && c >= '0' || c == '.') return true;
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

void variable::newType(char _newtype)
{
	type = _newtype;
	return;
}

char variable::getTypeId() const
{
	return type;
}

void variable::showInfo()
{
	std::cout << "variable: " << this->getName() << " " << this->getInd() << " " << this->getPos() << " " << int(type) << " " << arr << " ";
	return;
}

bool variableCMP::operator()(const variable& v1, const variable& v2) const
{
	std::string name1 = v1.getName();
	std::string name2 = v2.getName();
	for (size_t i = 0; i < std::min(name1.size(), name2.size()); ++i) {
		if (name1[i] < name2[i]) return true;
		if (name1[i] > name2[i]) return false;
	}
	return name1.size() < name2.size();
}
