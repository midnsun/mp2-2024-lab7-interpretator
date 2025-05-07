#include "specialLexem.h"

std::set<char> specialLexems{ '(', ')', '{', '}', '[', ']', ',', ';' };

specialLexem::specialLexem(const std::string str, size_t ind, size_t pos) : commonLexem(str, ind, pos) { }

bool specialLexem::isSpecialLexem(char c)
{
	if (specialLexems.find(c) != specialLexems.end()) return true;
	return false;
}

void specialLexem::showInfo() const
{
	std::cout << "class: special lexem name: " << this->getName() << " LineIndex: " << this->getInd() << " PosIndex: " << this->getPos() << " ";
	return;
}

std::string specialLexem::getClass() const { return "specialLexem"; }
