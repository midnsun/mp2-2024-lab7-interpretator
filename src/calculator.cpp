#include "calculator.h"

calculator::calculator(const std::vector<commonLexem*>& _data, size_t _begin, size_t _end, const std::set<variable*, variableCMP>& _vars) : data(_data), begin(_begin), end(_end), vars(_vars) {

}

calculator::~calculator() {
	// NO COMMONLEXEM* DELETING!!!!!!!
}

constant calculator::calculate(interpretator* inter) {
	//ш1 вычисляем значения всех функций
	//ш2 переводим в польскую запись
	//ш3 вычисляем арифметическое выражение
	std::vector<commonLexem*> expression;
	for (size_t pos = begin; pos < end; pos++)
	{
		if (data[pos]->getClass() == "function")
		{
			function* func = dynamic_cast<function*>(data[pos]);
			std::vector<constant> args;
			bool flag = false;
			size_t argsCounter = 0;
			++pos;
			if (pos >= data.size() || data[pos]->getName() != "(")
			{
				throw std::runtime_error("Line " + std::to_string(func->getInd()) + ", symbol " + std::to_string(func->getPos()) + ": " + func->getName() + " - No opening bracket");
			}
			++pos;
			if (pos >= data.size())
			{
				throw std::runtime_error("Line " + std::to_string(func->getInd()) + ", symbol " + std::to_string(func->getPos()) + ": " + func->getName() + " - Invalid agruments processing: no closing bracket found");
			}
			if (data[pos]->getName() == ")")
			{
				flag = true;
			}
			while (pos < data.size() && !flag)
			{
				if (data[pos]->getClass() == "variable")
				{
					constant tmp("##UNNAMED##", -1, -1, dynamic_cast<variable*>(data[pos])->getTypeId());
					tmp.setValue(dynamic_cast<variable*>(data[pos])->getValue());
					args.push_back(tmp);
				}
				else if (data[pos]->getClass() == "constant")
				{
					args.push_back(*(dynamic_cast<constant*>(data[pos])));
				}
				else
				{
					throw std::runtime_error("Line " + std::to_string(data[pos]->getInd()) + ", symbol " + std::to_string(data[pos]->getPos()) + ": " + data[pos]->getName() + " - Variable is missing");
				}
				pos++;
				if (data[pos]->getName() == ")")
				{
					flag = true;
				}
				else if (data[pos]->getClass() != "specialLexems" && data[pos]->getName() != ",")
				{
					throw std::runtime_error("Line " + std::to_string(data[pos]->getInd()) + ", symbol " + std::to_string(data[pos]->getPos()) + ": " + data[pos]->getName() + " - comma is missing");
				}
				pos++;
			}
			constant tmp("##UNNAMED##", -1, -1, func->type);
			tmp = inter->execute(func, args);
			expression.push_back(dynamic_cast<commonLexem*>(&tmp));
		}
		else if (data[pos]->getClass() == "specialLexems" && (data[pos]->getName() != "(" || data[pos]->getName() != ")"))
		{
			throw std::runtime_error("Line " + std::to_string(data[pos]->getInd()) + ", symbol " + std::to_string(data[pos]->getPos()) + ": " + data[pos]->getName() + "an unexpected symbol");
		}
		else
		{
			expression.push_back(data[pos]);
		}
	}
	//expression - вектор из операнд, операций и ( )
	//можно переводить в постфикс

	constant result("##UNNAMED##", -1, -1, -1);
	return result;
}