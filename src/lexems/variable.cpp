#include "variable.h"

variable::variable(std::string str, size_t ind, size_t pos, char _type, int _arr) : operand(str, ind, pos, _type), sizes(_arr) {}

variable::variable(const variable& var) : operand(var.getName(), var.getInd(), var.getPos(), var.getTypeId()), sizes(var.sizes)
{
	this->setValue(var.getValue());
}

bool variable::isValidCharForVariable(char c)
{
	if (c <= 'z' && c >= 'a' || c <= 'Z' && c >= 'A' || c <= '9' && c >= '0') return true;
	return false;
}

bool variable::isValidVariable(const std::string& str)
{
	if (str.length() == 0) return false;
	if (std::isdigit(str[0])) return false;
	for (auto c : str)
		if (!isValidCharForVariable(c)) return false;
	return true;
}

void variable::setArr(int _arr) {
	sizes.resize(_arr);
}

std::vector<int> variable::getSizes() const {
	return sizes;
}

//выполняется один раз при объявление массива
//тип хранимых значений уже должен быть известен
void variable::setSizes(const std::vector<int>& v) {
	sizes = v;
	size_t cntItem = 1;
	for (size_t i = 0; i < sizes.size(); i++)
	{
		cntItem *= sizes[i];
	}
	if (getTypeId() == 1)
	{
		setValue(new int[cntItem]);
	}
	else if (getTypeId() == 2)
	{
		setValue(new double[cntItem]);
	} 
	else if (getTypeId() == 3)
	{
		setValue(new std::string[cntItem]);
	}
	else
	{
		throw std::runtime_error("you cannot declare an array of this type");
	}
	return;
}

void* variable::getValueArr(const std::vector<int>& ind)
{
	size_t shift = 0;
	if (ind.size() != sizes.size()) throw std::runtime_error("the dimension of the array is violated");
	for (size_t i = 0; i < ind.size(); i++)
	{
		size_t k = ind[i];
		for (size_t j = 0; j < sizes.size() - i - 1; j++)
		{
			k *= sizes[j];
		}
		shift += k;
	}
	if (getTypeId() == 1)
	{
		int* t = (int*)getValue();
		return (t + shift);
	}
	else if (getTypeId() == 2)
	{
		double* t = (double*)getValue();
		return (t + shift);
	}
	else
	{
		std::string* t = (std::string*)getValue();
		return (t + shift);
	}
}

void variable::showInfo() const
{
	std::cout << "class variable name: " << this->getName() << " LineIndex: " << this->getInd() << " PosIndex: " << this->getPos() << " type: " << this->getTypeId() << " sizes: " << sizes.size() << " ";
	for (size_t i = 0; i < sizes.size(); ++i) {
		std::cout << sizes[i] << " ";
	}
	return;
}

std::string variable::getClass() const { return "variable"; }

bool variableCMP::operator()(const variable* v1, const variable* v2) const
{
	std::string name1 = v1->getName();
	std::string name2 = v2->getName();
	for (size_t i = 0; i < std::min(name1.size(), name2.size()); ++i) {
		if (name1[i] < name2[i]) return true;
		if (name1[i] > name2[i]) return false;
	}
	return name1.size() < name2.size();
}
