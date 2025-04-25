#include "commonLexem.h"

commonLexem::commonLexem(std::string s, size_t ind, size_t pos) : lex(s), lineIndex(ind), linePos(pos) { }

std::string commonLexem::getName() const noexcept
{
    return lex;
}

void commonLexem::setName(std::string s) noexcept
{
    lex = s;
    return;
}

size_t commonLexem::getInd() const noexcept
{
    return lineIndex;
}

size_t commonLexem::getPos() const noexcept
{
    return linePos;
}

void commonLexem::setInd(size_t _ind) noexcept
{
    lineIndex = _ind;
    return;
}

void commonLexem::setPos(size_t _pos) noexcept
{
    linePos = _pos;
    return;
}
