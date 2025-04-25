#include "commonLexem.h"

commonLexem::commonLexem(std::string s, size_t ind, size_t pos) : lex(s), lineIndex(ind), linePos(pos) { }

std::string commonLexem::getName() const noexcept
{
    return lex;
}

size_t commonLexem::getInd() const noexcept
{
    return lineIndex;
}

size_t commonLexem::getPos() const noexcept
{
    return linePos;
}
