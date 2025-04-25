#include <stack>
#include "calculator.h"

std::vector<commonLexem*> calculator::toPostfix(const std::vector<commonLexem*>& expr)
{
	std::stack<commonLexem*> op;
	std::vector<commonLexem*> postfix;
	for (int i = 0; i < expr.size(); i++)
	{
		if (expr[i]->getClass() == "constant" || expr[i]->getClass() == "variable")
		{
			if (i - 1 >= 0 && expr[i - 1]->getName() == ")")
			{
				throw std::runtime_error("Line " + std::to_string(expr[i]->getInd())
					+ ", symbol " + std::to_string(expr[i]->getPos()) + ": the operation before this operand was skipped");
			}
			postfix.push_back(expr[i]);
		}
		else if (expr[i]->getName() == "(")
		{
			if (expr.size() > i + 1 && expr[i + 1]->getClass() == "operation" && dynamic_cast<operation*>(expr[i + 1])->getPriority() != 0)
			{
				throw std::runtime_error("Line " + std::to_string(expr[i + 1]->getInd())
					+ ", symbol " + std::to_string(expr[i + 1]->getPos()) + ": the operation before this operand was skipped");
			}
			if (i - 1 >= 0 && (expr[i - 1]->getClass() != "operation" || expr[i - 1]->getName() == ")"))
			{
				throw std::runtime_error("Line " + std::to_string(expr[i - 1]->getInd())
					+ ", symbol " + std::to_string(expr[i - 1]->getPos()) + ": the operation before this operand was skipped");
			}
			if (expr.size() > i + 1 && expr[i + 1]->getClass() == "operation" && expr[i - 1]->getName() == ")")
			{
				throw std::runtime_error("Line " + std::to_string(expr[i + 1]->getInd())
					+ ", symbol " + std::to_string(expr[i + 1]->getPos()) + ": the expression in parentheses is lost");
			}
			op.push(expr[i]);
		}
		else if (expr[i]->getName() == ")")
		{
			if (i - 1 >= 0 && expr[i - 1]->getClass() == "operation")
			{
				throw std::runtime_error("Line " + std::to_string(expr[i - 1]->getInd())
					+ ", symbol " + std::to_string(expr[i - 1]->getPos()) + ": the operation before this operand was skipped");
			}
			if (expr.size() < i + 1 && expr[i + 1]->getClass() != "operation")
			{
				throw std::runtime_error("Line " + std::to_string(expr[i]->getInd())
					+ ", symbol " + std::to_string(expr[i]->getPos()) + ": the operation before this operand was skipped");
			}
			if (op.empty())
			{
				throw std::runtime_error("Line " + std::to_string(expr[i]->getInd())
					+ ", symbol " + std::to_string(expr[i]->getPos()) + ": there is no opening bracket for this closing bracket");
			}
			while (op.top()->getName() != "(")
			{
				postfix.push_back(op.top());
				op.pop();
				if (op.empty())
				{
					throw std::runtime_error("Line " + std::to_string(expr[i]->getInd())
						+ ", symbol " + std::to_string(expr[i]->getPos()) + ": there is no opening bracket for this closing bracket");
				}
			}
			op.pop();
		}
		else if (expr[i]->getClass() == "operation")
		{
			operation* newOp = dynamic_cast<operation*>(expr[i]);
			if (newOp->getPriority() == 0 && i - 1 >= 0 && expr[i - 1]->getName() == "-")
			{
				throw std::runtime_error("Line " + std::to_string(expr[i]->getInd())
					+ ", symbol " + std::to_string(expr[i]->getPos()) + ": the opening parenthesis before the unary minus was skipped");
			}
			if (expr.size() > i + 1 && expr[i + 1]->getClass() == "operation")
			{
				throw std::runtime_error("Line " + std::to_string(expr[i]->getInd())
					+ ", symbol " + std::to_string(expr[i]->getPos()) + ": the operand for this operation is lost");
			}
			while (!op.empty() && op.top()->getName() != "(" && dynamic_cast<operation*>(op.top())->getPriority() <= newOp->getPriority())
			{
				postfix.push_back(op.top());
				op.pop();
			}
			op.push(newOp);
		}
	}
	while (!op.empty())
	{
		if (op.top()->getName() == "(")
		{
			throw std::runtime_error("there is no closing bracket for this opening bracket " + op.top()->getPos());
		}
		postfix.push_back(op.top());
		op.pop();
	}
	return postfix;
}

operand* calculator::calcPlus(operand* v1, operand* v2)
{
	if (v1->getTypeId() == 1 && v2->getTypeId() == 1)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 1);
		int* a = new int(*((int*)(v1->getValue())) + *((int*)(v2->getValue())));
		tmpres->setValue((void*)(a));
		return dynamic_cast<operand*>(tmpres);
	}
	else if (v1->getTypeId() == 2 && v2->getTypeId() == 2 || v1->getTypeId() == 2 && v2->getTypeId() == 1)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 2);
		double* a = new double(*((double*)(v1->getValue())) + *((double*)(v2->getValue())));
		tmpres->setValue((void*)(a));
		return dynamic_cast<operand*>(tmpres);
	}
	else if (v1->getTypeId() == 1 && v2->getTypeId() == 2)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 2);
		double* a = new double(*((int*)(v1->getValue())) + *((double*)(v2->getValue())));
		tmpres->setValue((void*)(a));
		return dynamic_cast<operand*>(tmpres);
	}
	else if (v1->getTypeId() == 2 && v2->getTypeId() == 1)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 2);
		double* a = new double(*((double*)(v1->getValue())) + *((int*)(v2->getValue())));
		tmpres->setValue((void*)(a));
		return dynamic_cast<operand*>(tmpres);
	}
	else if (v1->getTypeId() == 3 && v2->getTypeId() == 3)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 3);
		std::string* a = new std::string(*((std::string*)(v1->getValue())) + *((std::string*)(v2->getValue())));
		tmpres->setValue((void*)(a));
		return dynamic_cast<operand*>(tmpres);
	}
	throw std::runtime_error("");//недопустимая операция
}

operand* calculator::calcBinaryNegative(operand* v1, operand* v2)
{
	if (v1->getTypeId() == 1 && v2->getTypeId() == 1)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 1);
		int* a = new int((*((int*)(v1->getValue())) - *((int*)(v2->getValue()))));
		tmpres->setValue((void*)(a));
		return dynamic_cast<operand*>(tmpres);
	}
	else if (v1->getTypeId() == 2 && v2->getTypeId() == 2 || v1->getTypeId() == 2 && v2->getTypeId() == 1)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 2);
		double* a = new double((*((double*)(v1->getValue())) - *((double*)(v2->getValue()))));
		tmpres->setValue((void*)(a));
		return dynamic_cast<operand*>(tmpres);
	}
	else if (v1->getTypeId() == 1 && v2->getTypeId() == 2)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 2);
		double* a = new double((*((int*)(v1->getValue())) - *((double*)(v2->getValue()))));
		tmpres->setValue((void*)(a));
		return dynamic_cast<operand*>(tmpres);
	}
	else if (v1->getTypeId() == 2 && v2->getTypeId() == 1)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 2);
		double* a = new double((*((double*)(v1->getValue())) - *((int*)(v2->getValue()))));
		tmpres->setValue((void*)(a));
		return dynamic_cast<operand*>(tmpres);
	}
	throw std::runtime_error("");//недопустимая операция
}

operand* calculator::calcUnaryNegative(operand* v1)
{
	if (v1->getTypeId() == 1)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 1);
		int* a = new int(*((int*)(v1->getValue())));
		*a = -(*a);
		tmpres->setValue((void*)(a));
		return dynamic_cast<operand*>(tmpres);
	}
	else if (v1->getTypeId() == 2)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 2);
		double* a = new double(*((double*)(v1->getValue())));
		*a = -(*a);
		tmpres->setValue((void*)(a));
		return dynamic_cast<operand*>(tmpres);
	}
	throw std::runtime_error("");//недопустимая операция
}

operand* calculator::calcMultiplication(operand* v1, operand* v2)
{
	if (v1->getTypeId() == 1 && v2->getTypeId() == 1)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 1);
		int* a = new int((*((int*)(v1->getValue())) * (*((int*)(v2->getValue())))));
		tmpres->setValue((void*)(a));
		return dynamic_cast<operand*>(tmpres);
	}
	else if (v1->getTypeId() == 2 && v2->getTypeId() == 2 || v1->getTypeId() == 2 && v2->getTypeId() == 1)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 2);
		double* a = new double((*((double*)(v1->getValue())) * *((double*)(v2->getValue()))));
		tmpres->setValue((void*)(a));
		return dynamic_cast<operand*>(tmpres);
	}
	else if (v1->getTypeId() == 1 && v2->getTypeId() == 2)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 2);
		double* a = new double((*((int*)(v1->getValue())) * *((double*)(v2->getValue()))));
		tmpres->setValue((void*)(a));
		return dynamic_cast<operand*>(tmpres);
	}
	else if (v1->getTypeId() == 2 && v2->getTypeId() == 1)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 2);
		double* a = new double((*((double*)(v1->getValue())) * *((int*)(v2->getValue()))));
		tmpres->setValue((void*)(a));
		return dynamic_cast<operand*>(tmpres);
	}
	throw std::runtime_error("");//недопустимая операция
}

operand* calculator::calcDivision(operand* v1, operand* v2)
{
	if (v1->getTypeId() == 1 && v2->getTypeId() == 1)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 1);
		int* a = new int (*((int*)(v1->getValue())) / *((int*)(v2->getValue())));
		tmpres->setValue((void*)(a));
		return dynamic_cast<operand*>(tmpres);
	}
	else if (v1->getTypeId() == 2 && v2->getTypeId() == 2 || v1->getTypeId() == 2 && v2->getTypeId() == 1)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 2);
		double* a = new double (*((double*)(v1->getValue())) / *((double*)(v2->getValue())));
		tmpres->setValue((void*)(a));
		return dynamic_cast<operand*>(tmpres);
	}
	else if (v1->getTypeId() == 1 && v2->getTypeId() == 2)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 2);
		double* a = new double (*((int*)(v1->getValue())) / *((double*)(v2->getValue())));
		tmpres->setValue((void*)(a));
		return dynamic_cast<operand*>(tmpres);
	}
	else if (v1->getTypeId() == 2 && v2->getTypeId() == 1)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 2);
		double* a = new double (*((double*)(v1->getValue())) / *((int*)(v2->getValue())));
		tmpres->setValue((void*)(a));
		return dynamic_cast<operand*>(tmpres);
	}
	throw std::runtime_error("");//недопустимая операция
}

operand* calculator::calcDivisionWithRemainder(operand* v1, operand* v2)
{
	if (v1->getTypeId() == 1 && v2->getTypeId() == 1)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 1);
		int* a = new int(*((int*)(v1->getValue())) % *((int*)(v2->getValue())));
		tmpres->setValue((void*)(a));
		return dynamic_cast<operand*>(tmpres);
	}
	throw std::runtime_error("");//недопустимая операция
}

operand* calculator::calcAnd(operand* v1, operand* v2)
{
	if (v1->getTypeId() == 1 && v2->getTypeId() == 1)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 1);
		int* a = new int(*((int*)(v1->getValue())) && *((int*)(v2->getValue())));
		tmpres->setValue((void*)(a));
		return dynamic_cast<operand*>(tmpres);
	}
	else if (v1->getTypeId() == 2 && v2->getTypeId() == 1)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 1);
		int* a = new int(*((double*)(v1->getValue())) && *((int*)(v2->getValue())));
		tmpres->setValue((void*)(a));
		return dynamic_cast<operand*>(tmpres);
	}
	else if (v1->getTypeId() == 1 && v2->getTypeId() == 2)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 1);
		int* a = new int(*((int*)(v1->getValue())) && *((double*)(v2->getValue())));
		tmpres->setValue((void*)(a));
		return dynamic_cast<operand*>(tmpres);
	}
	else if (v1->getTypeId() == 2 && v2->getTypeId() == 2)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 1);
		int* a = new int(*((double*)(v1->getValue())) && *((double*)(v2->getValue())));
		tmpres->setValue((void*)(a));
		return dynamic_cast<operand*>(tmpres);
	}
	throw std::runtime_error("");//недопустимая операция
}

operand* calculator::calcOr(operand* v1, operand* v2)
{
	if (v1->getTypeId() == 1 && v2->getTypeId() == 1)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 1);
		int* a = new int(*((int*)(v1->getValue())) || *((int*)(v2->getValue())));
		tmpres->setValue((void*)(a));
		return dynamic_cast<operand*>(tmpres);
	}
	else if (v1->getTypeId() == 2 && v2->getTypeId() == 1)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 1);
		int* a = new int(*((double*)(v1->getValue())) || *((int*)(v2->getValue())));
		tmpres->setValue((void*)(a));
		return dynamic_cast<operand*>(tmpres);
	}
	else if (v1->getTypeId() == 1 && v2->getTypeId() == 2)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 1);
		int* a = new int(*((int*)(v1->getValue())) || *((double*)(v2->getValue())));
		tmpres->setValue((void*)(a));
		return dynamic_cast<operand*>(tmpres);
	}
	else if (v1->getTypeId() == 2 && v2->getTypeId() == 2)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 1);
		int* a = new int(*((double*)(v1->getValue())) || *((double*)(v2->getValue())));
		tmpres->setValue((void*)(a));
		return dynamic_cast<operand*>(tmpres);
	}
	throw std::runtime_error("");//недопустимая операция
}

operand* calculator::calcLess(operand* v1, operand* v2)
{
	if (v1->getTypeId() == 1 && v2->getTypeId() == 1)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 1);
		int* a = new int(*((int*)(v1->getValue())) < *((int*)(v2->getValue())));
		tmpres->setValue((void*)(a));
		return dynamic_cast<operand*>(tmpres);
	}
	else if (v1->getTypeId() == 2 && v2->getTypeId() == 1)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 1);
		int* a = new int(*((double*)(v1->getValue())) < *((int*)(v2->getValue())));
		tmpres->setValue((void*)(a));
		return dynamic_cast<operand*>(tmpres);
	}
	else if (v1->getTypeId() == 1 && v2->getTypeId() == 2)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 1);
		int* a = new int(*((int*)(v1->getValue())) < *((double*)(v2->getValue())));
		tmpres->setValue((void*)(a));
		return dynamic_cast<operand*>(tmpres);
	}
	else if (v1->getTypeId() == 2 && v2->getTypeId() == 2)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 1);
		int* a = new int(*((double*)(v1->getValue())) < *((double*)(v2->getValue())));
		tmpres->setValue((void*)(a));
		return dynamic_cast<operand*>(tmpres);
	}
	else if (v1->getTypeId() == 3 && v2->getTypeId() == 3)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 1);
		int* a = new int(*((std::string*)(v1->getValue())) < *((std::string*)(v2->getValue())));
		tmpres->setValue((void*)(a));
		return dynamic_cast<operand*>(tmpres);
	}
	throw std::runtime_error("");//недопустимая операция
}

operand* calculator::calcMore(operand* v1, operand* v2)
{
	if (v1->getTypeId() == 1 && v2->getTypeId() == 1)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 1);
		int* a = new int(*((int*)(v1->getValue())) > *((int*)(v2->getValue())));
		tmpres->setValue((void*)(a));
		return dynamic_cast<operand*>(tmpres);
	}
	else if (v1->getTypeId() == 2 && v2->getTypeId() == 1)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 1);
		int* a = new int(*((double*)(v1->getValue())) > *((int*)(v2->getValue())));
		tmpres->setValue((void*)(a));
		return dynamic_cast<operand*>(tmpres);
	}
	else if (v1->getTypeId() == 1 && v2->getTypeId() == 2)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 1);
		int* a = new int(*((int*)(v1->getValue())) > *((double*)(v2->getValue())));
		tmpres->setValue((void*)(a));
		return dynamic_cast<operand*>(tmpres);
	}
	else if (v1->getTypeId() == 2 && v2->getTypeId() == 2)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 1);
		int* a = new int(*((double*)(v1->getValue())) > *((double*)(v2->getValue())));
		tmpres->setValue((void*)(a));
		return dynamic_cast<operand*>(tmpres);
	}
	else if (v1->getTypeId() == 3 && v2->getTypeId() == 3)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 1);
		int* a = new int(*((std::string*)(v1->getValue())) > *((std::string*)(v2->getValue())));
		tmpres->setValue((void*)(a));
		return dynamic_cast<operand*>(tmpres);
	}
	throw std::runtime_error("");//недопустимая операция
}

operand* calculator::calclessEqual(operand* v1, operand* v2)
{
	if (v1->getTypeId() == 1 && v2->getTypeId() == 1)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 1);
		int* a = new int(*((int*)(v1->getValue())) <= *((int*)(v2->getValue())));
		tmpres->setValue((void*)(a));
		return dynamic_cast<operand*>(tmpres);
	}
	else if (v1->getTypeId() == 2 && v2->getTypeId() == 1)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 1);
		int* a = new int(*((double*)(v1->getValue())) < *((int*)(v2->getValue())));
		tmpres->setValue((void*)(a));
		return dynamic_cast<operand*>(tmpres);
	}
	else if (v1->getTypeId() == 1 && v2->getTypeId() == 2)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 1);
		int* a = new int(*((int*)(v1->getValue())) <= *((double*)(v2->getValue())));
		tmpres->setValue((void*)(a));
		return dynamic_cast<operand*>(tmpres);
	}
	else if (v1->getTypeId() == 2 && v2->getTypeId() == 2)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 1);
		int* a = new int(*((double*)(v1->getValue())) <= *((double*)(v2->getValue())));
		tmpres->setValue((void*)(a));
		return dynamic_cast<operand*>(tmpres);
	}
	else if (v1->getTypeId() == 3 && v2->getTypeId() == 3)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 1);
		int* a = new int(*((std::string*)(v1->getValue())) <= *((std::string*)(v2->getValue())));
		tmpres->setValue((void*)(a));
		return dynamic_cast<operand*>(tmpres);
	}
	throw std::runtime_error("");//недопустимая операция
}

operand* calculator::calcMoreEqual(operand* v1, operand* v2)
{
	if (v1->getTypeId() == 1 && v2->getTypeId() == 1)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 1);
		int* a = new int(*((int*)(v1->getValue())) >= *((int*)(v2->getValue())));
		tmpres->setValue((void*)(a));
		return dynamic_cast<operand*>(tmpres);
	}
	else if (v1->getTypeId() == 2 && v2->getTypeId() == 1)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 1);
		int* a = new int(*((double*)(v1->getValue())) >= *((int*)(v2->getValue())));
		tmpres->setValue((void*)(a));
		return dynamic_cast<operand*>(tmpres);
	}
	else if (v1->getTypeId() == 1 && v2->getTypeId() == 2)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 1);
		int* a = new int(*((int*)(v1->getValue())) >= *((double*)(v2->getValue())));
		tmpres->setValue((void*)(a));
		return dynamic_cast<operand*>(tmpres);
	}
	else if (v1->getTypeId() == 2 && v2->getTypeId() == 2)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 1);
		int* a = new int(*((double*)(v1->getValue())) >= *((double*)(v2->getValue())));
		tmpres->setValue((void*)(a));
		return dynamic_cast<operand*>(tmpres);
	}
	else if (v1->getTypeId() == 3 && v2->getTypeId() == 3)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 1);
		int* a = new int(*((std::string*)(v1->getValue())) >= *((std::string*)(v2->getValue())));
		tmpres->setValue((void*)(a));
		return dynamic_cast<operand*>(tmpres);
	}
	throw std::runtime_error("");//недопустимая операция
}

operand* calculator::calcEqually(operand* v1, operand* v2)
{
	if (v1->getTypeId() == 1 && v2->getTypeId() == 1)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 1);
		int* a = new int(*((int*)(v1->getValue())) == *((int*)(v2->getValue())));
		tmpres->setValue((void*)(a));
		return dynamic_cast<operand*>(tmpres);
	}
	else if (v1->getTypeId() == 2 && v2->getTypeId() == 1)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 1);
		int* a = new int(*((double*)(v1->getValue())) == *((int*)(v2->getValue())));
		tmpres->setValue((void*)(a));
		return dynamic_cast<operand*>(tmpres);
	}
	else if (v1->getTypeId() == 1 && v2->getTypeId() == 2)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 1);
		int* a = new int(*((int*)(v1->getValue())) == *((double*)(v2->getValue())));
		tmpres->setValue((void*)(a));
		return dynamic_cast<operand*>(tmpres);
	}
	else if (v1->getTypeId() == 2 && v2->getTypeId() == 2)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 1);
		int* a = new int(*((double*)(v1->getValue())) == *((double*)(v2->getValue())));
		tmpres->setValue((void*)(a));
		return dynamic_cast<operand*>(tmpres);
	}
	else if (v1->getTypeId() == 3 && v2->getTypeId() == 3)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 1);
		int* a = new int(*((std::string*)(v1->getValue())) == *((std::string*)(v2->getValue())));
		tmpres->setValue((void*)(a));
		return dynamic_cast<operand*>(tmpres);
	}
	throw std::runtime_error("");//недопустимая операция
}

operand* calculator::calcNotEqually(operand* v1, operand* v2)
{
	if (v1->getTypeId() == 1 && v2->getTypeId() == 1)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 1);
		int* a = new int(*((int*)(v1->getValue())) != *((int*)(v2->getValue())));
		tmpres->setValue((void*)(a));
		return dynamic_cast<operand*>(tmpres);
	}
	else if (v1->getTypeId() == 2 && v2->getTypeId() == 1)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 1);
		int* a = new int(*((double*)(v1->getValue())) != *((int*)(v2->getValue())));
		tmpres->setValue((void*)(a));
		return dynamic_cast<operand*>(tmpres);
	}
	else if (v1->getTypeId() == 1 && v2->getTypeId() == 2)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 1);
		int* a = new int(*((int*)(v1->getValue())) != *((double*)(v2->getValue())));
		tmpres->setValue((void*)(a));
		return dynamic_cast<operand*>(tmpres);
	}
	else if (v1->getTypeId() == 2 && v2->getTypeId() == 2)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 1);
		int* a = new int(*((double*)(v1->getValue())) != *((double*)(v2->getValue())));
		tmpres->setValue((void*)(a));
		return dynamic_cast<operand*>(tmpres);
	}
	else if (v1->getTypeId() == 3 && v2->getTypeId() == 3)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 1);
		int* a = new int(*((std::string*)(v1->getValue())) != *((std::string*)(v2->getValue())));
		tmpres->setValue((void*)(a));
		return dynamic_cast<operand*>(tmpres);
	}
	throw std::runtime_error("");//недопустимая операция
}

operand* calculator::assignment(operand* v1, operand* v2)
{
	if (v1->getClass() != "variable") throw std::runtime_error("");
	if (v1->getTypeId() == 1 && v2->getTypeId() == 1)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 1);
		int* a = new int(*(int*)v2->getValue());
		v1->setValue(a);
	}
	else if (v1->getTypeId() == 2 && v2->getTypeId() == 2)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 2);
		double* a = new double(*(double*)v2->getValue());
		v1->setValue(a);
	}
	else if (v1->getTypeId() == 2 && v2->getTypeId() == 1)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 2);
		double* a = new double(*(int*)v2->getValue());
		v1->setValue(a);
	}
	else if (v1->getTypeId() == 3 && v2->getTypeId() == 3)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 2);
		std::string* a = new std::string(*(std::string*)v2->getValue());
		v1->setValue(a);
	}
	else std::runtime_error("");//недопустимая операция
	return v1;
}

operand* calculator::addAndAssign(operand* v1, operand* v2)
{
	return assignment(v1, calcPlus(v1, v2));
}

operand* calculator::NegativeAndAssign(operand* v1, operand* v2)
{
	return assignment(v1, calcBinaryNegative(v1, v2));
}

operand* calculator::MultiplicationAndAssign(operand* v1, operand* v2)
{
	return assignment(v1, calcMultiplication(v1, v2));
}

operand* calculator::DivisionAndAssign(operand* v1, operand* v2)
{
	return assignment(v1, calcDivision(v1, v2));
}

operand* calculator::DivisionWithRemainderAndAssign(operand* v1, operand* v2)
{
	return assignment(v1, calcDivisionWithRemainder(v1, v2));
}

operand* calculator::calcArithmetic(const std::vector<commonLexem*>& expr)
{
	std::stack<operand*> val; // constant or variable
	for (int i = 0; i < expr.size(); i++)
	{
		if (expr[i]->getClass() == "constant" || expr[i]->getClass() == "variable")
		{
			val.push(dynamic_cast<operand*>(expr[i]));
		}
		else if (expr[i]->getClass() == "operation")
		{
			operation* op = dynamic_cast<operation*>(expr[i]);
			if (expr[i]->getName() == "+" && val.size() >= 2)
			{
				operand* v1 = dynamic_cast<operand*>(val.top());
				val.pop();
				operand* v2 = dynamic_cast<operand*>(val.top());
				val.pop();
				val.push(calcPlus(v1, v2));
			}
			else if (expr[i]->getName() == "-" && val.size() >= 2 && dynamic_cast<operation*>(expr[i])->getPriority() != 0) //бинарный минус
			{
				operand* v1 = dynamic_cast<operand*>(val.top());
				val.pop();
				operand* v2 = dynamic_cast<operand*>(val.top());
				val.pop();
				val.push(calcBinaryNegative(v2, v1));
			}
			else if (expr[i]->getName() == "*" && val.size() >= 2)
			{
				operand* v1 = dynamic_cast<operand*>(val.top());
				val.pop();
				operand* v2 = dynamic_cast<operand*>(val.top());
				val.pop();
				val.push(calcMultiplication(v1, v2));
			}
			else if (expr[i]->getName() == "/" && val.size() >= 2)
			{
				operand* v1 = dynamic_cast<operand*>(val.top());
				val.pop();
				operand* v2 = dynamic_cast<operand*>(val.top());
				val.pop();
				val.push(calcDivision(v2, v1));
			}
			else if (expr[i]->getName() == "-" && val.size() >= 1 && dynamic_cast<operation*>(expr[i])->getPriority() == 0) //унарный минус
			{
				operand* v1 = dynamic_cast<operand*>(val.top());
				val.pop();
				val.push(calcUnaryNegative(v1));
			}
			else if (expr[i]->getName() == "%" && val.size() >= 2)
			{
				operand* v1 = dynamic_cast<operand*>(val.top());
				val.pop();
				operand* v2 = dynamic_cast<operand*>(val.top());
				val.pop();
				val.push(calcDivisionWithRemainder(v2, v1));
			}
			else if (expr[i]->getName() == "&&" && val.size() >= 2)
			{
				operand* v1 = dynamic_cast<operand*>(val.top());
				val.pop();
				operand* v2 = dynamic_cast<operand*>(val.top());
				val.pop();
				val.push(calcAnd(v2, v1));
			}
			else if (expr[i]->getName() == "||" && val.size() >= 2)
			{
				operand* v1 = dynamic_cast<operand*>(val.top());
				val.pop();
				operand* v2 = dynamic_cast<operand*>(val.top());
				val.pop();
				val.push(calcOr(v2, v1));
			}
			else if (expr[i]->getName() == "<" && val.size() >= 2)
			{
				operand* v1 = dynamic_cast<operand*>(val.top());
				val.pop();
				operand* v2 = dynamic_cast<operand*>(val.top());
				val.pop();
				val.push(calcOr(v2, v1));
			}
			else if (expr[i]->getName() == "<=" && val.size() >= 2)
			{
				operand* v1 = dynamic_cast<operand*>(val.top());
				val.pop();
				operand* v2 = dynamic_cast<operand*>(val.top());
				val.pop();
				val.push(calcOr(v2, v1));
			}
			else if (expr[i]->getName() == ">" && val.size() >= 2)
			{
				operand* v1 = dynamic_cast<operand*>(val.top());
				val.pop();
				operand* v2 = dynamic_cast<operand*>(val.top());
				val.pop();
				val.push(calcOr(v2, v1));
			}
			else if (expr[i]->getName() == ">=" && val.size() >= 2)
			{
				operand* v1 = dynamic_cast<operand*>(val.top());
				val.pop();
				operand* v2 = dynamic_cast<operand*>(val.top());
				val.pop();
				val.push(calcOr(v2, v1));
			}
			else if (expr[i]->getName() == "==" && val.size() >= 2)
			{
				operand* v1 = dynamic_cast<operand*>(val.top());
				val.pop();
				operand* v2 = dynamic_cast<operand*>(val.top());
				val.pop();
				val.push(calcOr(v2, v1));
			}
			else if (expr[i]->getName() == "!=" && val.size() >= 2)
			{
				operand* v1 = dynamic_cast<operand*>(val.top());
				val.pop();
				operand* v2 = dynamic_cast<operand*>(val.top());
				val.pop();
				val.push(calcOr(v2, v1));
				}
			else if (expr[i]->getName() == "=" && val.size() >= 2)
			{
				operand* v1 = dynamic_cast<operand*>(val.top());
				val.pop();
				operand* v2 = dynamic_cast<operand*>(val.top());
				val.pop();
				val.push(assignment(v2, v1));
			}
			else if (expr[i]->getName() == "+=" && val.size() >= 2)
			{
				operand* v1 = dynamic_cast<operand*>(val.top());
				val.pop();
				operand* v2 = dynamic_cast<operand*>(val.top());
				val.pop();
				val.push(assignment(v2, v1));
			}
			else if (expr[i]->getName() == "-=" && val.size() >= 2)
			{
				operand* v1 = dynamic_cast<operand*>(val.top());
				val.pop();
				operand* v2 = dynamic_cast<operand*>(val.top());
				val.pop();
				val.push(assignment(v2, v1));
			}
			else if (expr[i]->getName() == "*=" && val.size() >= 2)
			{
				operand* v1 = dynamic_cast<operand*>(val.top());
				val.pop();
				operand* v2 = dynamic_cast<operand*>(val.top());
				val.pop();
				val.push(assignment(v2, v1));
			}
			else if (expr[i]->getName() == "/=" && val.size() >= 2)
			{
				operand* v1 = dynamic_cast<operand*>(val.top());
				val.pop();
				operand* v2 = dynamic_cast<operand*>(val.top());
				val.pop();
				val.push(assignment(v2, v1));
			}
			else if (expr[i]->getName() == "%=" && val.size() >= 2)
			{
				operand* v1 = dynamic_cast<operand*>(val.top());
				val.pop();
				operand* v2 = dynamic_cast<operand*>(val.top());
				val.pop();
				val.push(assignment(v2, v1));
			}
			else
			{
				throw std::runtime_error("the operand for this operation is lost " + expr[i]->getPos());
			}
		}
	}
	if (val.size() != 1) throw std::runtime_error("The operation was lost");
	return val.top();
}

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

	//унарные минусы
	for (size_t pos = 0; pos < expression.size(); pos++)
	{
		if (expression[pos]->getName() == "-" && (pos == 0 || expression[pos - 1]->getName() == "(" || expression[pos - 1]->getClass() == "operation"))
		{
			dynamic_cast<operation*>(expression[pos])->setPriority(0);
		}
	}
	//expression - вектор из операнд, операций и ( )
	//можно переводить в постфикс
	for (size_t pos = 0; pos < expression.size(); pos++)
	{
		std::cout << expression[pos]->getName() << " ";
	}
	std::cout << std::endl;

	expression = toPostfix(expression);

	for (size_t pos = 0; pos < expression.size(); pos++)
	{
		std::cout << expression[pos]->getName() << " ";
	}
	std::cout << std::endl;
	for (size_t pos = 0; pos < expression.size(); pos++)
	{
		if (expression[pos]->getClass() == "constant")
		{
			//проверка, что значение константы задано корректно

			if (expression[pos]->getName()[0] == '\"' && expression[pos]->getName()[expression[pos]->getName().size() - 1] == '\"')
			{
				std::string* val = new std::string(expression[pos]->getName());
				dynamic_cast<operand*>(expression[pos])->setValue(val);
				dynamic_cast<operand*>(expression[pos])->setTypeId(3);
			}
			else
			{
				double* t = new double(std::stod(expression[pos]->getName()));
				if (*t - (int)*t > 0)
				{
					dynamic_cast<operand*>(expression[pos])->setValue(t);
					dynamic_cast<operand*>(expression[pos])->setTypeId(2);
				}
				else
				{
					int* t2 = new int(*t);
					dynamic_cast<operand*>(expression[pos])->setValue(t2);
					dynamic_cast<operand*>(expression[pos])->setTypeId(1);
					delete t;
				}
				
			}
		}
		else if (expression[pos]->getClass() == "variable")
		{
			if (vars.find(dynamic_cast<variable*>(expression[pos])) == vars.end())
			{
				//ошибка не нашли переменную
			}
			else
			{
				dynamic_cast<variable*>(expression[pos])->setValue((*vars.find(dynamic_cast<variable*>(expression[pos])))->getValue());
			}
		}
	}

	
	operand* tmp = calcArithmetic(expression);
	constant result("##UNNAMED##", -1, -1, tmp->getTypeId());
	result.setValue(tmp->getValue());
	return result;
}