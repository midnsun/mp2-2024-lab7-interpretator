#include "dataType.h"

std::set<std::string> dataTypes{ "int", "double", "void", "string" };

dataType::dataType(const std::string str, size_t ind, size_t pos) : keyWords(str, ind, pos) { }

bool dataType::isDataType(const std::string& str)
{
	if (dataTypes.find(str) != dataTypes.end()) return true;
	return false;
}

char dataType::getTypeId()
{
	if (this->getName() == "void") return 0;
	else if (this->getName() == "int") return 1;
	else if (this->getName() == "double") return 2;
	else if (this->getName() == "string") return 3;
	else return -1;
}

void dataType::showInfo() const
{
	std::cout << "class: dataType name: " << this->getName() << " LineIndex: " << this->getInd() << " PosIndex: " << this->getPos() << " ";
	return;
}

std::string dataType::getClass() const { return "dataType"; }
