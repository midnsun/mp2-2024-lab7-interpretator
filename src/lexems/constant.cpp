#include "constant.h"

constant::constant(const std::string& str, size_t ind, size_t pos, char type) : operand(str, ind, pos, type) { }

constant::constant(const constant& c) : operand(c.getName(), c.getInd(), c.getPos(), c.getTypeId())
{
	setValue(c.getValue());
}

constant::constant(constant&& c) : operand(c.getName(), c.getInd(), c.getPos(), c.getTypeId())
{
	setValue(c.getValue());
	c.setValue(nullptr);
	c.setName("");
	c.setInd(-1);
	c.setPos(-1);
	c.setTypeId(-1);
}

bool constant::isValidConstant(const std::string& str) // проверить, это число или нет
{
	char junk;
	double d;
	std::stringstream ss(str);
	return (ss >> d && !(ss >> junk));
}

bool constant::isInteger(const std::string& str)
{
	for (char c : str) if (!isdigit(c)) return false;
	return true;
}

void constant::showInfo() const
{
	std::cout << "class: constant " << "name: " << this->getName() << " LineIndex: " << this->getInd() << " PosIndex: " << this->getPos() << " type: " << this->getTypeId();
	return;
}

std::string constant::getClass() const { return "constant"; }

constant& constant::operator=(const constant& c) {
	/*if (getTypeId() == c.getTypeId())
	{
		setPos(c.getPos());
		setInd(c.getInd());
		setValue(c.getValue());
		setName(c.getName());
	}
	else if (getTypeId() == 2 && c.getTypeId() == 1)
	{
		setPos(c.getPos());
		setInd(c.getInd());
		setName(c.getName());
		double* a = new double(*(int*)c.getValue());
		setValue(a);
	}
	else
	{
		setPos(c.getPos());
		setInd(c.getInd());
		setName(c.getName());
		setValue(nullptr);
	}*/
	setTypeId(c.getTypeId());
	setPos(c.getPos());
	setInd(c.getInd());
	setValue(c.getValue());
	setName(c.getName());
	return *this;
}
