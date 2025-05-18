#pragma once
#include <string>
#include "commonLexem.h"


class operand : public commonLexem {
private:
	char type; // 1 - int, 2 - double, 3 - string
	void* value;
public:
	static bool isValidCharForOperand(char c);
	operand(const std::string str, size_t ind, size_t pos, char _type);
	operand(const operand& op);
	virtual void showInfo() const = 0;
	virtual std::string getClass() const = 0;
	char getTypeId() const noexcept;
	void setTypeId(const char& _type) noexcept;
	void* getValue() const;
	void setValue(const void* v);
	void setValue(const std::string& v);
	bool isTrue() const;
	void copyValue(void* v);
	virtual ~operand();
};