#include "operation.h"

std::set<std::string> operations{ "=", "+", "-", "*", "/", "%", "+=", "-=", "*=", "/=", "%=", "&&", "||", "==", "<=", "<", ">=", ">", "!="};

operation::operation(const std::string& str, size_t ind, size_t pos) : commonLexem(str, ind, pos)
{
	if (str == "*" || str == "/" || str == "%") priority = 1;
	if (str == "-" || str == "+") priority = 2;
	if (str == "<=" || str == "<" || str == ">=" || str == ">") priority = 3;
	if (str == "==" || str == "!=") priority = 4;
	if (str == "&&") priority = 5;
	if (str == "||") priority = 6;
	if (str == "=" || str == "+=" || str == "-=" || str == "*=" || str == "/=") priority = 7;
}

bool operation::isOperation(const std::string& str)
{
	if (operations.find(str) != operations.end())
		return true;
	return false;
}

void operation::showInfo() const
{
	std::cout << "operation: " << this->getName() << " " << this->getInd() << " " << this->getPos() << " ";
	return;
}

std::string operation::getClass() const { return "operation"; }

char operation::getPriority() const noexcept
{
	return priority;
}

void operation::setPriority(char newPriority) noexcept
{
	priority = newPriority;
	return;
}