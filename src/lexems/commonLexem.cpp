#include "commonLexem.h"

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
