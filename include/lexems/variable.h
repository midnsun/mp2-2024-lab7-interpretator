#pragma once
#include <string>
#include <iostream>
#include "operand.h"
#include <vector>

class variable : public operand {
	// по факту для каждой функции и глобальной области будет своя таблица переменных из-за области видимости.
	// Иначе можно создать отдельную переменную принадлежности переменной функции, затем пробегаться по таблице и удалять после return
	// В таком случае внутри функции доступны только переменные, созданные внутри нее, и глобальные переменные
	//	string context; // what function it belongs. Functions are marked as 0 (as global), 1, 2, 3...
	int arr;
	std::vector<int> sizes;
	// В стеке вызовов хранится только точка возврата, по команде возврата удаляются все переменные с filed = номеру функции
public:
	//	variable(std::string str, size_t ind, size_t pos, char _type, string _context, int _arr) : operand{ str, ind, pos }, type(_type), context(_context), arr(_arr) {}
	variable(std::string str, size_t ind, size_t pos, char _type, int _arr);
	variable(const variable& var);
	static bool isValidCharForVariable(char c);
	static bool isValidVariable(const std::string& str);
	virtual void showInfo() const;
	virtual std::string getClass() const;
	void setArr(int _arr);
	std::vector<int> getSizes() const;
	void setSizes(const std::vector<int>& v);
};

struct variableCMP {
	bool operator ()(const variable* v1, const variable* v2) const;
};