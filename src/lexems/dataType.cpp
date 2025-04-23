#include "dataType.h"

std::set<std::string> dataTypes{ "int", "double", "void" };

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
	else return -1;
}

void dataType::showInfo() const
{
	std::cout << "dataType: " << this->getName() << " " << this->getInd() << " " << this->getPos() << " ";
	return;
}

std::string dataType::getClass() const { return "dataType"; }
