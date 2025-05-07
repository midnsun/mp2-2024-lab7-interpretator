#include <stack>
#include "calculator.h"

void printResult(const constant& res)
{
	if (res.getValue() == nullptr) return;
	if (res.getTypeId() == 1)
	{
		std::cout << "Result is: " << *(int*)res.getValue() << std::endl;
	}
	else if (res.getTypeId() == 2)
	{
		std::cout << "Result is: " << *(double*)res.getValue() << std::endl;
	}
	else if (res.getTypeId() == 3)
	{
		std::cout << "Result is: " << *(std::string*)res.getValue() << std::endl;
	}
	return;
}

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
			if (expr.size() > i + 1 && expr[i + 1]->getClass() == "operation" && expr[i]->getName() != "=")
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
		delete a;
		return dynamic_cast<operand*>(tmpres);
	}
	else if (v1->getTypeId() == 2 && v2->getTypeId() == 2)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 2);
		double* a = new double(*((double*)(v1->getValue())) + *((double*)(v2->getValue())));
		tmpres->setValue((void*)(a));
		delete a;
		return dynamic_cast<operand*>(tmpres);
	} 
	else if (v1->getTypeId() == 1 && v2->getTypeId() == 2)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 2);
		double* a = new double(*((int*)(v1->getValue())) + *((double*)(v2->getValue())));
		tmpres->setValue((void*)(a));
		delete a;
		return dynamic_cast<operand*>(tmpres);
	}
	else if (v1->getTypeId() == 2 && v2->getTypeId() == 1)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 2);
		double* a = new double(*((double*)(v1->getValue())) + *((int*)(v2->getValue())));
		tmpres->setValue((void*)(a));
		delete a;
		return dynamic_cast<operand*>(tmpres);
	}
	else if (v1->getTypeId() == 3 && v2->getTypeId() == 3)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 3);
		std::string* a = new std::string(*((std::string*)(v2->getValue())) + *((std::string*)(v1->getValue())));
		tmpres->setValue((void*)(a));
		delete a;
		return dynamic_cast<operand*>(tmpres);
	}
	throw std::runtime_error("it is not possible to perform this operation with the current operands");
}

operand* calculator::calcBinaryNegative(operand* v1, operand* v2)
{
	if (v1->getTypeId() == 1 && v2->getTypeId() == 1)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 1);
		int* a = new int((*((int*)(v1->getValue())) - *((int*)(v2->getValue()))));
		tmpres->setValue((void*)(a));
		delete a;
		return dynamic_cast<operand*>(tmpres);
	}
	else if (v1->getTypeId() == 2 && v2->getTypeId() == 2 || v1->getTypeId() == 2 && v2->getTypeId() == 1)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 2);
		double* a = new double((*((double*)(v1->getValue())) - *((double*)(v2->getValue()))));
		tmpres->setValue((void*)(a));
		delete a;
		return dynamic_cast<operand*>(tmpres);
	}
	else if (v1->getTypeId() == 1 && v2->getTypeId() == 2)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 2);
		double* a = new double((*((int*)(v1->getValue())) - *((double*)(v2->getValue()))));
		tmpres->setValue((void*)(a));
		delete a;
		return dynamic_cast<operand*>(tmpres);
	}
	else if (v1->getTypeId() == 2 && v2->getTypeId() == 1)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 2);
		double* a = new double((*((double*)(v1->getValue())) - *((int*)(v2->getValue()))));
		tmpres->setValue((void*)(a));
		delete a;
		return dynamic_cast<operand*>(tmpres);
	}
	throw std::runtime_error("it is not possible to perform this operation with the current operands");//недопустимая операция
}

operand* calculator::calcUnaryNegative(operand* v1)
{
	if (v1->getTypeId() == 1)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 1);
		int* a = new int(*((int*)(v1->getValue())));
		*a = -(*a);
		tmpres->setValue((void*)(a));
		delete a;
		return dynamic_cast<operand*>(tmpres);
	}
	else if (v1->getTypeId() == 2)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 2);
		double* a = new double(*((double*)(v1->getValue())));
		*a = -(*a);
		tmpres->setValue((void*)(a));
		delete a;
		return dynamic_cast<operand*>(tmpres);
	}
	throw std::runtime_error("it is not possible to perform this operation with the current operands");//недопустимая операция
}

operand* calculator::calcMultiplication(operand* v1, operand* v2)
{
	if (v1->getTypeId() == 1 && v2->getTypeId() == 1)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 1);
		int* a = new int((*((int*)(v1->getValue())) * (*((int*)(v2->getValue())))));
		tmpres->setValue((void*)(a));
		delete a;
		return dynamic_cast<operand*>(tmpres);
	}
	else if (v1->getTypeId() == 2 && v2->getTypeId() == 2 || v1->getTypeId() == 2 && v2->getTypeId() == 1)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 2);
		double* a = new double((*((double*)(v1->getValue())) * *((double*)(v2->getValue()))));
		tmpres->setValue((void*)(a));
		delete a;
		return dynamic_cast<operand*>(tmpres);
	}
	else if (v1->getTypeId() == 1 && v2->getTypeId() == 2)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 2);
		double* a = new double((*((int*)(v1->getValue())) * *((double*)(v2->getValue()))));
		tmpres->setValue((void*)(a));
		delete a;
		return dynamic_cast<operand*>(tmpres);
	}
	else if (v1->getTypeId() == 2 && v2->getTypeId() == 1)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 2);
		double* a = new double((*((double*)(v1->getValue())) * *((int*)(v2->getValue()))));
		tmpres->setValue((void*)(a));
		delete a;
		return dynamic_cast<operand*>(tmpres);
	}
	throw std::runtime_error("it is not possible to perform this operation with the current operands");//недопустимая операция
}

operand* calculator::calcDivision(operand* v1, operand* v2)
{
	if (v1->getTypeId() == 1 && v2->getTypeId() == 1)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 1);
		int* a = new int (*((int*)(v1->getValue())) / *((int*)(v2->getValue())));
		tmpres->setValue((void*)(a));
		delete a;
		return dynamic_cast<operand*>(tmpres);
	}
	else if (v1->getTypeId() == 2 && v2->getTypeId() == 2 || v1->getTypeId() == 2 && v2->getTypeId() == 1)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 2);
		double* a = new double (*((double*)(v1->getValue())) / *((double*)(v2->getValue())));
		tmpres->setValue((void*)(a));
		delete a;
		return dynamic_cast<operand*>(tmpres);
	}
	else if (v1->getTypeId() == 1 && v2->getTypeId() == 2)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 2);
		double* a = new double (*((int*)(v1->getValue())) / *((double*)(v2->getValue())));
		tmpres->setValue((void*)(a));
		delete a;
		return dynamic_cast<operand*>(tmpres);
	}
	else if (v1->getTypeId() == 2 && v2->getTypeId() == 1)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 2);
		double* a = new double (*((double*)(v1->getValue())) / *((int*)(v2->getValue())));
		tmpres->setValue((void*)(a));
		delete a;
		return dynamic_cast<operand*>(tmpres);
	}
	throw std::runtime_error("it is not possible to perform this operation with the current operands");//недопустимая операция
}

operand* calculator::calcDivisionWithRemainder(operand* v1, operand* v2)
{
	if (v1->getTypeId() == 1 && v2->getTypeId() == 1)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 1);
		int* a = new int(*((int*)(v1->getValue())) % *((int*)(v2->getValue())));
		tmpres->setValue((void*)(a));
		delete a;
		return dynamic_cast<operand*>(tmpres);
	}
	throw std::runtime_error("it is not possible to perform this operation with the current operands");//недопустимая операция
}

operand* calculator::calcAnd(operand* v1, operand* v2)
{
	if (v1->getTypeId() == 1 && v2->getTypeId() == 1)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 1);
		int* a = new int(*((int*)(v1->getValue())) && *((int*)(v2->getValue())));
		tmpres->setValue((void*)(a));
		delete a;
		return dynamic_cast<operand*>(tmpres);
	}
	else if (v1->getTypeId() == 2 && v2->getTypeId() == 1)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 1);
		int* a = new int(*((double*)(v1->getValue())) && *((int*)(v2->getValue())));
		tmpres->setValue((void*)(a));
		delete a;
		return dynamic_cast<operand*>(tmpres);
	}
	else if (v1->getTypeId() == 1 && v2->getTypeId() == 2)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 1);
		int* a = new int(*((int*)(v1->getValue())) && *((double*)(v2->getValue())));
		tmpres->setValue((void*)(a));
		delete a;
		return dynamic_cast<operand*>(tmpres);
	}
	else if (v1->getTypeId() == 2 && v2->getTypeId() == 2)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 1);
		int* a = new int(*((double*)(v1->getValue())) && *((double*)(v2->getValue())));
		tmpres->setValue((void*)(a));
		delete a;
		return dynamic_cast<operand*>(tmpres);
	}
	throw std::runtime_error("it is not possible to perform this operation with the current operands");//недопустимая операция
}

operand* calculator::calcOr(operand* v1, operand* v2)
{
	if (v1->getTypeId() == 1 && v2->getTypeId() == 1)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 1);
		int* a = new int(*((int*)(v1->getValue())) || *((int*)(v2->getValue())));
		tmpres->setValue((void*)(a));
		delete a;
		return dynamic_cast<operand*>(tmpres);
	}
	else if (v1->getTypeId() == 2 && v2->getTypeId() == 1)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 1);
		int* a = new int(*((double*)(v1->getValue())) || *((int*)(v2->getValue())));
		tmpres->setValue((void*)(a));
		delete a;
		return dynamic_cast<operand*>(tmpres);
	}
	else if (v1->getTypeId() == 1 && v2->getTypeId() == 2)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 1);
		int* a = new int(*((int*)(v1->getValue())) || *((double*)(v2->getValue())));
		tmpres->setValue((void*)(a));
		delete a;
		return dynamic_cast<operand*>(tmpres);
	}
	else if (v1->getTypeId() == 2 && v2->getTypeId() == 2)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 1);
		int* a = new int(*((double*)(v1->getValue())) || *((double*)(v2->getValue())));
		tmpres->setValue((void*)(a));
		delete a;
		return dynamic_cast<operand*>(tmpres);
	}
	throw std::runtime_error("it is not possible to perform this operation with the current operands");//недопустимая операция
}

operand* calculator::calcLess(operand* v1, operand* v2)
{
	if (v1->getTypeId() == 1 && v2->getTypeId() == 1)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 1);
		int* a = new int(*((int*)(v1->getValue())) < *((int*)(v2->getValue())));
		tmpres->setValue((void*)(a));
		delete a;
		return dynamic_cast<operand*>(tmpres);
	}
	else if (v1->getTypeId() == 2 && v2->getTypeId() == 1)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 1);
		int* a = new int(*((double*)(v1->getValue())) < *((int*)(v2->getValue())));
		tmpres->setValue((void*)(a));
		delete a;
		return dynamic_cast<operand*>(tmpres);
	}
	else if (v1->getTypeId() == 1 && v2->getTypeId() == 2)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 1);
		int* a = new int(*((int*)(v1->getValue())) < *((double*)(v2->getValue())));
		tmpres->setValue((void*)(a));
		delete a;
		return dynamic_cast<operand*>(tmpres);
	}
	else if (v1->getTypeId() == 2 && v2->getTypeId() == 2)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 1);
		int* a = new int(*((double*)(v1->getValue())) < *((double*)(v2->getValue())));
		tmpres->setValue((void*)(a));
		delete a;
		return dynamic_cast<operand*>(tmpres);
	}
	else if (v1->getTypeId() == 3 && v2->getTypeId() == 3)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 1);
		int* a = new int(*((std::string*)(v1->getValue())) < *((std::string*)(v2->getValue())));
		tmpres->setValue((void*)(a));
		delete a;
		return dynamic_cast<operand*>(tmpres);
	}
	throw std::runtime_error("it is not possible to perform this operation with the current operands");//недопустимая операция
}

operand* calculator::calcMore(operand* v1, operand* v2)
{
	if (v1->getTypeId() == 1 && v2->getTypeId() == 1)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 1);
		int* a = new int(*((int*)(v1->getValue())) > *((int*)(v2->getValue())));
		tmpres->setValue((void*)(a));
		delete a;
		return dynamic_cast<operand*>(tmpres);
	}
	else if (v1->getTypeId() == 2 && v2->getTypeId() == 1)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 1);
		int* a = new int(*((double*)(v1->getValue())) > *((int*)(v2->getValue())));
		tmpres->setValue((void*)(a));
		delete a;
		return dynamic_cast<operand*>(tmpres);
	}
	else if (v1->getTypeId() == 1 && v2->getTypeId() == 2)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 1);
		int* a = new int(*((int*)(v1->getValue())) > *((double*)(v2->getValue())));
		tmpres->setValue((void*)(a));
		delete a;
		return dynamic_cast<operand*>(tmpres);
	}
	else if (v1->getTypeId() == 2 && v2->getTypeId() == 2)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 1);
		int* a = new int(*((double*)(v1->getValue())) > *((double*)(v2->getValue())));
		tmpres->setValue((void*)(a));
		delete a;
		return dynamic_cast<operand*>(tmpres);
	}
	else if (v1->getTypeId() == 3 && v2->getTypeId() == 3)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 1);
		int* a = new int(*((std::string*)(v1->getValue())) > *((std::string*)(v2->getValue())));
		tmpres->setValue((void*)(a));
		delete a;
		return dynamic_cast<operand*>(tmpres);
	}
	throw std::runtime_error("it is not possible to perform this operation with the current operands");//недопустимая операция
}

operand* calculator::calclessEqual(operand* v1, operand* v2)
{
	if (v1->getTypeId() == 1 && v2->getTypeId() == 1)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 1);
		int* a = new int(*((int*)(v1->getValue())) <= *((int*)(v2->getValue())));
		tmpres->setValue((void*)(a));
		delete a;
		return dynamic_cast<operand*>(tmpres);
	}
	else if (v1->getTypeId() == 2 && v2->getTypeId() == 1)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 1);
		int* a = new int(*((double*)(v1->getValue())) < *((int*)(v2->getValue())));
		tmpres->setValue((void*)(a));
		delete a;
		return dynamic_cast<operand*>(tmpres);
	}
	else if (v1->getTypeId() == 1 && v2->getTypeId() == 2)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 1);
		int* a = new int(*((int*)(v1->getValue())) <= *((double*)(v2->getValue())));
		tmpres->setValue((void*)(a));
		delete a;
		return dynamic_cast<operand*>(tmpres);
	}
	else if (v1->getTypeId() == 2 && v2->getTypeId() == 2)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 1);
		int* a = new int(*((double*)(v1->getValue())) <= *((double*)(v2->getValue())));
		tmpres->setValue((void*)(a));
		delete a;
		return dynamic_cast<operand*>(tmpres);
	}
	else if (v1->getTypeId() == 3 && v2->getTypeId() == 3)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 1);
		int* a = new int(*((std::string*)(v1->getValue())) <= *((std::string*)(v2->getValue())));
		tmpres->setValue((void*)(a));
		delete a;
		return dynamic_cast<operand*>(tmpres);
	}
	throw std::runtime_error("it is not possible to perform this operation with the current operands");//недопустимая операция
}

operand* calculator::calcMoreEqual(operand* v1, operand* v2)
{
	if (v1->getTypeId() == 1 && v2->getTypeId() == 1)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 1);
		int* a = new int(*((int*)(v1->getValue())) >= *((int*)(v2->getValue())));
		tmpres->setValue((void*)(a));
		delete a;
		return dynamic_cast<operand*>(tmpres);
	}
	else if (v1->getTypeId() == 2 && v2->getTypeId() == 1)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 1);
		int* a = new int(*((double*)(v1->getValue())) >= *((int*)(v2->getValue())));
		tmpres->setValue((void*)(a));
		delete a;
		return dynamic_cast<operand*>(tmpres);
	}
	else if (v1->getTypeId() == 1 && v2->getTypeId() == 2)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 1);
		int* a = new int(*((int*)(v1->getValue())) >= *((double*)(v2->getValue())));
		tmpres->setValue((void*)(a));
		delete a;
		return dynamic_cast<operand*>(tmpres);
	}
	else if (v1->getTypeId() == 2 && v2->getTypeId() == 2)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 1);
		int* a = new int(*((double*)(v1->getValue())) >= *((double*)(v2->getValue())));
		tmpres->setValue((void*)(a));
		delete a;
		return dynamic_cast<operand*>(tmpres);
	}
	else if (v1->getTypeId() == 3 && v2->getTypeId() == 3)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 1);
		int* a = new int(*((std::string*)(v1->getValue())) >= *((std::string*)(v2->getValue())));
		tmpres->setValue((void*)(a));
		delete a;
		return dynamic_cast<operand*>(tmpres);
	}
	throw std::runtime_error("it is not possible to perform this operation with the current operands");//недопустимая операция
}

operand* calculator::calcEqually(operand* v1, operand* v2)
{
	if (v1->getTypeId() == 1 && v2->getTypeId() == 1)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 1);
		int* a = new int(*((int*)(v1->getValue())) == *((int*)(v2->getValue())));
		tmpres->setValue((void*)(a));
		delete a;
		return dynamic_cast<operand*>(tmpres);
	}
	else if (v1->getTypeId() == 2 && v2->getTypeId() == 1)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 1);
		int* a = new int(std::abs(*((double*)(v1->getValue())) - *((int*)(v2->getValue()))) < 1e-9);
		tmpres->setValue((void*)(a));
		delete a;
		return dynamic_cast<operand*>(tmpres);
	}
	else if (v1->getTypeId() == 1 && v2->getTypeId() == 2)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 1);
		int* a = new int(std::abs(*((int*)(v1->getValue())) - *((double*)(v2->getValue()))) < 1e-9);
		tmpres->setValue((void*)(a));
		delete a;
		return dynamic_cast<operand*>(tmpres);
	}
	else if (v1->getTypeId() == 2 && v2->getTypeId() == 2)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 1);
		int* a = new int(std::abs(*((double*)(v1->getValue())) - *((double*)(v2->getValue()))) < 1e-9);
		tmpres->setValue((void*)(a));
		delete a;
		return dynamic_cast<operand*>(tmpres);
	}
	else if (v1->getTypeId() == 3 && v2->getTypeId() == 3)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 1);
		int* a = new int(*((std::string*)(v1->getValue())) == *((std::string*)(v2->getValue())));
		tmpres->setValue((void*)(a));
		delete a;
		return dynamic_cast<operand*>(tmpres);
	}
	throw std::runtime_error("it is not possible to perform this operation with the current operands");//недопустимая операция
}

operand* calculator::calcNotEqually(operand* v1, operand* v2)
{
	if (v1->getTypeId() == 1 && v2->getTypeId() == 1)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 1);
		int* a = new int(*((int*)(v1->getValue())) != *((int*)(v2->getValue())));
		tmpres->setValue((void*)(a));
		delete a;
		return dynamic_cast<operand*>(tmpres);
	}
	else if (v1->getTypeId() == 2 && v2->getTypeId() == 1)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 1);
		int* a = new int(std::abs(*((double*)(v1->getValue())) - *((int*)(v2->getValue()))) > 1e-9);
		tmpres->setValue((void*)(a));
		delete a;
		return dynamic_cast<operand*>(tmpres);
	}
	else if (v1->getTypeId() == 1 && v2->getTypeId() == 2)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 1);
		int* a = new int(std::abs(*((int*)(v1->getValue())) - *((double*)(v2->getValue()))) > 1e-9);
		tmpres->setValue((void*)(a));
		delete a;
		return dynamic_cast<operand*>(tmpres);
	}
	else if (v1->getTypeId() == 2 && v2->getTypeId() == 2)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 1);
		int* a = new int(std::abs(*((double*)(v1->getValue())) - *((double*)(v2->getValue()))) > 1e-0);
		tmpres->setValue((void*)(a));
		delete a;
		return dynamic_cast<operand*>(tmpres);
	}
	else if (v1->getTypeId() == 3 && v2->getTypeId() == 3)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 1);
		int* a = new int(*((std::string*)(v1->getValue())) != *((std::string*)(v2->getValue())));
		tmpres->setValue((void*)(a));
		delete a;
		return dynamic_cast<operand*>(tmpres);
	}
	throw std::runtime_error("it is not possible to perform this operation with the current operands");//недопустимая операция
}

operand* calculator::assignment(operand* v1, operand* v2)
{
	if (v1->getClass() != "variable") throw std::runtime_error("");
	bool canAssign = v1->getTypeId() == 1 && v2->getTypeId() == 1 ||
					 v1->getTypeId() == 2 && v2->getTypeId() == 2 ||
					 v1->getTypeId() == 2 && v2->getTypeId() == 1 ||
					 v1->getTypeId() == 3 && v2->getTypeId() == 3;
	if (canAssign)
	{
		v1->setValue(v2->getValue());
		(*vars.find(dynamic_cast<variable*>(v1)))->setValue(v2->getValue());
	}
	else std::runtime_error("it is not possible to perform this operation with the current operands");//недопустимая операция
	return v1;
}

operand* calculator::addAndAssign(operand* v1, operand* v2)
{
	return assignment(v1, calcPlus(v2, v1));
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

void calculator::executeScan(const size_t& sBegin, const size_t& sEnd)
{
	for (size_t pos = sBegin; pos < sEnd; pos++)
	{
		if (data[pos]->getClass() == "constant")
		{
			throw std::runtime_error("Line " + std::to_string(data[pos]->getInd()) + ", symbol " + std::to_string(data[pos]->getPos())
				+ ": " + data[pos]->getName() + " - the value cannot be considered as an rValue");
		}
		else if (data[pos - 1]->getName() == "," && data[pos]->getName() == ",")
		{
			throw std::runtime_error("Line " + std::to_string(data[pos]->getInd()) + ", symbol " + std::to_string(data[pos]->getPos())
				+ ": " + data[pos]->getName() + " - variable was be skipped");
		}
		else if (data[pos]->getClass() == "variable")
		{
			operand* tOp = new constant("##UNNAMED##", -1, -1, (dynamic_cast<variable*>(data[pos]))->getTypeId());
			if (tOp->getTypeId() == 1)
			{
				int t;
				std::cin >> t;
				tOp->setValue(&t);
			}
			else if (tOp->getTypeId() == 2)
			{
				double t;
				std::cin >> t;
				tOp->setValue(&t);
			}
			else if (tOp->getTypeId() == 3)
			{
				std::string t;
				std::cin >> t;
				tOp->setValue(&t);
			}
			else
			{
				throw std::runtime_error("Line " + std::to_string(data[pos]->getInd()) + ", symbol " + std::to_string(data[pos]->getPos())
					+ ": " + data[pos]->getName() + " - a variable of this type cannot be read");
			}
			assignment(dynamic_cast<operand*>(data[pos]), tOp);
		}
	}
	return;
}

std::vector<commonLexem*> calculator::calculatingFunctions(interpretator* inter)
{
	std::vector<commonLexem*> expression;
	for (size_t pos = begin; pos < end; pos++)
	{
		if (data[pos]->getClass() == "function")
		{
			function* func = dynamic_cast<function*>(data[pos]);
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

			if (func->getName() == "scan")
			{
				int cnt = 1;
				size_t sBegin = pos;
				while (pos < data.size() && cnt != 0)
				{
					if (data[pos]->getClass() == "specialLexem" && data[pos]->getName() != "," && data[pos]->getName() != ")")
					{
						throw std::runtime_error("Line " + std::to_string(data[pos]->getInd()) + ", symbol " + std::to_string(data[pos]->getPos()) + ": " + data[pos]->getName() + " - an unexpected symbol");
					}
					if (data[pos]->getName() == ")") cnt--;
					pos++;
				}
				executeScan(sBegin, pos - 1);
				continue;
			}
			std::vector<constant> args;
			bool flag = false;
			size_t argsCounter = 0;
			if (data[pos]->getName() == ")")
			{
				flag = true;
				pos++;
			}
			while (pos < data.size() && !flag)
			{
				std::vector<commonLexem*> tmpExp;
				size_t tBegin = pos;
				bool flFoo = false;
				int cnt = 0;
				while (pos < data.size() && data[pos]->getName() != "," && data[pos]->getName() != ")" || pos < data.size() && flFoo)
				{
					if (data[pos]->getClass() == "function")
					{
						flFoo = true;
						pos++;
					}
					if (flFoo && data[pos]->getName() == "(") cnt++;
					if (flFoo && data[pos]->getName() == ")") cnt--;
					if (flFoo && cnt == 0) flFoo = false;
					tmpExp.push_back(data[pos]);
					pos++;
				}
				calculator calc(data, tBegin, pos, vars);
				args.push_back(calc.calculate(inter));
				if (data[pos]->getName() == ")")
				{
					flag = true;
				}
				if (pos > 0 && data[pos - 1]->getName() == "," && data[pos]->getName() != ")")
				{
					throw std::runtime_error("Line " + std::to_string(data[pos]->getInd()) + ", symbol " + std::to_string(data[pos]->getPos()) + ": " + data[pos]->getName() + " - Variable is missing");
				}
				pos++;
			}
			constant* val = new constant("##UNNAMED##", -1, -1, func->type);
			constant t = inter->execute(func, args);
			if (func->getName() == "abs")
			{
				val->setTypeId(t.getTypeId());
			}
			val->setValue(t.getValue());
			//if (val->getTypeId() != -1) std::cout << "val = " <<  * (int*)val->getValue() << std::endl;
			//если функция типа void ничего не добаляем
			if (val->getTypeId() != -1 && val->getTypeId() != 0) expression.push_back(dynamic_cast<commonLexem*>(val));
			pos--;
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
	return expression;
}

void calculator::checkUnaryNegative()
{
	for (size_t pos = 0; pos < data.size(); pos++)
	{
		if (data[pos]->getName() == "-" && (pos == 0 || data[pos - 1]->getName() == "(" || data[pos - 1]->getClass() == "operation"))
		{
			dynamic_cast<operation*>(data[pos])->setPriority(0);
		}
	}
	return;
}

void calculator::initialConstantAndVarisble()
{
	for (size_t pos = begin; pos < end; pos++)
	{
		if (data[pos]->getClass() == "variable")
		{
			if (vars.find(dynamic_cast<variable*>(data[pos])) == vars.end())
			{
				throw std::runtime_error("Line " + std::to_string(data[pos]->getInd()) + ", symbol " + std::to_string(data[pos]->getPos()) + ": " + data[pos]->getName() + " - variable was not found"); 
			}
			else
			{
				dynamic_cast<variable*>(data[pos])->setTypeId((*vars.find(dynamic_cast<variable*>(data[pos])))->getTypeId());
				if ((*vars.find(dynamic_cast<variable*>(data[pos])))->getValue() == nullptr) continue;
				if (dynamic_cast<variable*>(data[pos])->getTypeId() == 1)
				{
					int* t = new int(*(int*)((*vars.find(dynamic_cast<variable*>(data[pos])))->getValue()));
					dynamic_cast<variable*>(data[pos])->setValue(t);
				}
				else if (dynamic_cast<variable*>(data[pos])->getTypeId() == 2)
				{
					double* t = new double((*(double*)(*vars.find(dynamic_cast<variable*>(data[pos])))->getValue()));
					dynamic_cast<variable*>(data[pos])->setValue(t);
				}
				else if (dynamic_cast<variable*>(data[pos])->getTypeId() == 3)
				{
					std::string* t = new std::string((*(std::string*)(*vars.find(dynamic_cast<variable*>(data[pos])))->getValue()));
					dynamic_cast<variable*>(data[pos])->setValue(t);
				}
			}
		}
		else if (data[pos]->getClass() == "constant")
		{
			//проверка, что значение константы задано корректно

			//if (data[pos]->getName()[0] == '\"' && data[pos]->getName()[data[pos]->getName().size() - 1] == '\"')
			//{
			//	std::string* val = new std::string(data[pos]->getName());
			//	dynamic_cast<operand*>(data[pos])->setTypeId(3);
			//	dynamic_cast<operand*>(data[pos])->setValue(val);
			//	ynamic_cast<operand*>(data[pos])->setValue();
			//	delete val;
			//}
			if (dynamic_cast<operand*>(data[pos])->getTypeId() != 3)
			{
				double* t = new double(std::stod(data[pos]->getName()));
				if (!constant::isInteger(data[pos]->getName()))
				{
					dynamic_cast<operand*>(data[pos])->setTypeId(2);
					dynamic_cast<operand*>(data[pos])->setValue(t);
				}
				else
				{
					int* t2 = new int(*t);
					dynamic_cast<operand*>(data[pos])->setTypeId(1);
					dynamic_cast<operand*>(data[pos])->setValue(t2);
					delete t2;
				}
				delete t;
			}
		}
	}
	return;
}

operand* calculator::calcArithmetic(const std::vector<commonLexem*>& expr)
{
	if (expr.size() == 0)
	{
		constant* tmpres = new constant("##UNNAMED##", -1, -1, 0);
		return dynamic_cast<operand*>(tmpres);
	}
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
			try
			{
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
					val.push(calcLess(v2, v1));
				}
				else if (expr[i]->getName() == "<=" && val.size() >= 2)
				{
					operand* v1 = dynamic_cast<operand*>(val.top());
					val.pop();
					operand* v2 = dynamic_cast<operand*>(val.top());
					val.pop();
					val.push(calclessEqual(v2, v1));
				}
				else if (expr[i]->getName() == ">" && val.size() >= 2)
				{
					operand* v1 = dynamic_cast<operand*>(val.top());
					val.pop();
					operand* v2 = dynamic_cast<operand*>(val.top());
					val.pop();
					val.push(calcMore(v2, v1));
				}
				else if (expr[i]->getName() == ">=" && val.size() >= 2)
				{
					operand* v1 = dynamic_cast<operand*>(val.top());
					val.pop();
					operand* v2 = dynamic_cast<operand*>(val.top());
					val.pop();
					val.push(calcMoreEqual(v2, v1));
				}
				else if (expr[i]->getName() == "==" && val.size() >= 2)
				{
					operand* v1 = dynamic_cast<operand*>(val.top());
					val.pop();
					operand* v2 = dynamic_cast<operand*>(val.top());
					val.pop();
					val.push(calcEqually(v2, v1));
				}
				else if (expr[i]->getName() == "!=" && val.size() >= 2)
				{
					operand* v1 = dynamic_cast<operand*>(val.top());
					val.pop();
					operand* v2 = dynamic_cast<operand*>(val.top());
					val.pop();
					val.push(calcNotEqually(v2, v1));
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
					val.push(addAndAssign(v2, v1));
				}
				else if (expr[i]->getName() == "-=" && val.size() >= 2)
				{
					operand* v1 = dynamic_cast<operand*>(val.top());
					val.pop();
					operand* v2 = dynamic_cast<operand*>(val.top());
					val.pop();
					val.push(NegativeAndAssign(v2, v1));
				}
				else if (expr[i]->getName() == "*=" && val.size() >= 2)
				{
					operand* v1 = dynamic_cast<operand*>(val.top());
					val.pop();
					operand* v2 = dynamic_cast<operand*>(val.top());
					val.pop();
					val.push(MultiplicationAndAssign(v2, v1));
				}
				else if (expr[i]->getName() == "/=" && val.size() >= 2)
				{
					operand* v1 = dynamic_cast<operand*>(val.top());
					val.pop();
					operand* v2 = dynamic_cast<operand*>(val.top());
					val.pop();
					val.push(DivisionAndAssign(v2, v1));
				}
				else if (expr[i]->getName() == "%=" && val.size() >= 2)
				{
					operand* v1 = dynamic_cast<operand*>(val.top());
					val.pop();
					operand* v2 = dynamic_cast<operand*>(val.top());
					val.pop();
					val.push(DivisionWithRemainderAndAssign(v2, v1));
				}
				else
				{
					throw std::runtime_error("the operand for this operation is lost");
				}
			}
			catch (std::exception& e)
			{
				throw std::runtime_error("Line " + std::to_string(expr[i]->getInd()) + ", symbol " + std::to_string(expr[i]->getPos()) + ": " + expr[i]->getName() + " - " + e.what());
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

void printExpression(const std::vector<commonLexem*>& expression)
{
	if (expression.size() == 0) return;
	for (size_t pos = 0; pos < expression.size(); pos++)
	{
		std::cout << expression[pos]->getName() << " ";
	}
	std::cout << std::endl;
	return;
}

//ш1 вычисляем значения всех функций
//ш2 переводим в польскую запись
//ш3 вычисляем арифметическое выражение
constant calculator::calculate(interpretator* inter)
{
	initialConstantAndVarisble();
	checkUnaryNegative();
	std::vector<commonLexem*> expression = calculatingFunctions(inter);

	//expression - вектор из операнд, операций и ( )
	//можно переводить в постфикс
	//printExpression(expression);
	expression = toPostfix(expression);
	//printExpression(expression);
	initialConstantAndVarisble();
	operand* tmp = calcArithmetic(expression);
	constant result("##UNNAMED##", -1, -1, tmp->getTypeId());
	result.setValue(tmp->getValue());
	//printResult(result);
	return result;
}