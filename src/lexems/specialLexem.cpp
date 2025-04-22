#include "specialLexem.h"

std::set<char> specialLexems{ '(', ')', '{', '}', '[', ']', ',', ';' };

bool specialLexem::isSpecialLexem(char c)
{
	if (specialLexems.find(c) != specialLexems.end()) return true;
	return false;
}

void specialLexem::showInfo()
{
	std::cout << "special lexem: " << this->getName() << " " << this->getInd() << " " << this->getPos() << " ";
	return;
}
