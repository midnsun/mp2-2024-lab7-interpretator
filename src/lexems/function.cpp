#include "function.h"

std::set<std::string> standartFuinction{ "sin", "cos", "abs", "sqrt", "print", "scan" };

function::function(const std::string str, size_t ind, size_t pos, char _type, size_t _begin, size_t _end) :
	commonLexem(str, ind, pos), type(_type), begin(_begin), end(_end) {

}

void function::showInfo() const
{
	std::cout << "function: " << this->getName() << " " << this->getInd() << " " << this->getPos() << " " << int(type) << " " << begin << " " << end << " ";
	return;
}

bool functionCMP::operator()(const function* f1, const function* f2) const
{
	std::string name1 = f1->getName();
	std::string name2 = f2->getName();
	for (size_t i = 0; i < std::min(name1.size(), name2.size()); ++i) {
		if (name1[i] < name2[i]) return true;
		if (name1[i] > name2[i]) return false;
	}
	return name1.size() < name2.size();
}

std::string function::getClass() const { return "function"; }
