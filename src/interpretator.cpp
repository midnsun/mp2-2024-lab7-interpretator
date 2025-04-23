#include "interpretator.h"

// ���� ������� ������ �������� ������, ��������� ������� ������� ���� �� ��������, ������� �� �������� ������, �.�. �� ��������
	// ������:
	// int a = 1;
	// int b = 2;
	// b = a / b;
	// [ ["int", "a", "=", "1"], ["int", "b", "=", "2"], ["b", "=", "a", "/", "b"] ] - 
	// ������� ������ �������� ������ (�������) ������ ��������� (������� ������)
void interpretator::process(const std::vector<std::string>& source)
{
	// 1. ��������� �� ����� - ������ �������
		// 1.1 ��������� �� ������ ����� ��������
		// 
	std::string word;
	int wordPos, wordLen, tmpwordPos;
	std::vector <std::vector <std::pair <std::string, int> > > strProgram;
	std::vector< std::pair<std::string, int> > strCommand;
	size_t lineInd, pos, wordInd;
	for (lineInd = 0; lineInd < source.size(); ++lineInd) {
		strProgram.push_back(std::vector<std::pair <std::string, int>>());
		pos = 0;
		while (pos < source[lineInd].length()) {
			word = "";
			for (; pos < source[lineInd].length(); ++pos) if (source[lineInd][pos] != ' ') break; // no spaces
			for (; pos < source[lineInd].length(); ++pos) { // words
				if (source[lineInd][pos] == ' ') break;
				word += source[lineInd][pos];
			}
			strProgram[lineInd].push_back(make_pair(word, pos - word.length()));
		}
	}

	// 1.2 ��������� ����. ���������
	for (lineInd = 0; lineInd < strProgram.size(); ++lineInd) {
		strCommand.clear();
		for (wordInd = 0; wordInd < strProgram[lineInd].size(); ++wordInd) {
			word = strProgram[lineInd][wordInd].first;
			wordPos = strProgram[lineInd][wordInd].second;
			tmpwordPos = wordPos;
			for (pos = 0; pos < word.length(); ++pos) {
				if (specialLexem::isSpecialLexem(word[pos])) {
					if (pos - wordPos + tmpwordPos > 0) strCommand.push_back(make_pair(word.substr(wordPos - tmpwordPos, pos - wordPos + tmpwordPos), wordPos));
					strCommand.push_back(make_pair(word.substr(pos, 1), tmpwordPos + pos));
					wordPos = tmpwordPos + pos + 1;
				}
			}
			if (pos - wordPos + tmpwordPos > 0) strCommand.push_back(make_pair(word.substr(wordPos - tmpwordPos, pos - wordPos + tmpwordPos), wordPos));
		}
		strProgram[lineInd] = strCommand;
	}

	// 1.3 ��������� ����������� +, - etc..
	for (lineInd = 0; lineInd < strProgram.size(); ++lineInd) {
		strCommand.clear();
		for (wordInd = 0; wordInd < strProgram[lineInd].size(); ++wordInd) {
			word = strProgram[lineInd][wordInd].first;
			wordPos = strProgram[lineInd][wordInd].second;
			tmpwordPos = wordPos;
			for (pos = 0; pos < word.length(); ++pos) {
				if (!operand::isValidCharForOperand(word[pos])) {
					if (pos - wordPos + tmpwordPos > 0) strCommand.push_back(make_pair(word.substr(wordPos - tmpwordPos, pos - wordPos + tmpwordPos), wordPos));
					for (wordLen = 1; pos + wordLen < word.length(); ++wordLen) if (operand::isValidCharForOperand(word[pos + wordLen])) break;
					strCommand.push_back(make_pair(word.substr(pos, wordLen), tmpwordPos + pos));
					wordPos = tmpwordPos + pos + wordLen;
					pos += wordLen - 1;
				}
			}
			if (pos - wordPos + tmpwordPos > 0) strCommand.push_back(make_pair(word.substr(wordPos - tmpwordPos, pos - wordPos + tmpwordPos), wordPos));
		}
		strProgram[lineInd] = strCommand;
	}

	// variable : type, context, arr; functions: type, begin, end; datatype; myoperators: begin, end
	// special lexem, constant, operation
	// 2. ������������ ������� �� ��������������� �������. ��� �� ��������, ����� ������ � ����� - �����, ������ � ����� - ���������� (�������)

	for (auto& s : standartFuinction) {
		//			functions.insert(*(new function{ s, size_t(-1), size_t(-1), -1, size_t(-1), size_t(-1), size_t(-1), size_t(-1) }));
		functions.insert(*(new function{ s, 0, 0, -1, 0, 0 }));
	}

	size_t begin = 0, end = 0;
	size_t counter = 0;
	int arrCounter = -1;
	char dataTypeAppeared = -1;
	//		string context = "GLOBAL";
	for (lineInd = 0; lineInd < strProgram.size(); ++lineInd) {
		//			program.push_back(vector<commonLexem*>());
		for (wordInd = 0; wordInd < strProgram[lineInd].size(); ++wordInd) {
			word = strProgram[lineInd][wordInd].first;
			wordPos = strProgram[lineInd][wordInd].second;

			// SPECIAL LEXEM
			if (word.length() == 1 && specialLexem::isSpecialLexem(word[0])) program.push_back(new specialLexem{ word, lineInd, size_t(wordPos) });

			// OPERATIONS (+, -, ...)
			else if (operation::isOperation(word)) program.push_back(new operation{ word, lineInd, size_t(wordPos) });

			// DATA TYPES
			else if (dataType::isDataType(word)) {
				// remember its appearance
				dataType* tmpptr = new dataType{ word, lineInd, size_t(wordPos) };
				program.push_back(tmpptr);
				dataTypeAppeared = tmpptr->getTypeId();
			}

			// KEY WORD OPERATORS (if, else ...)
			else if (myoperators::isKeyWordOperator(word)) {
				// ���� ����������� figure ������, ���������� � begin, ���������� �������. ����� ����� ����������� � ������� ���� ����� - ��� end
				if (word != "return") {
					int bracketCounter = 0;
					bool exitFlag = false;
					begin = program.size();
					counter = 0;
					for (size_t i = lineInd; i < strProgram.size(); ++i) {
						size_t j;
						if (i == lineInd) j = wordInd;
						else j = 0;
						for (; j < strProgram[i].size(); ++j, ++counter) {
							if (strProgram[i][j].first == "{") {
								if (bracketCounter == 0) {
									//										lineBegin = i;
									//										wordBegin = j;
								}
								++bracketCounter;
							}
							if (strProgram[i][j].first == "}") {
								--bracketCounter;
								if (bracketCounter == 0) {
									end = counter + begin;
									exitFlag = true;
									break;
								}
							}
							if (bracketCounter < 0)  throw std::runtime_error("Line " + std::to_string(i) + ", symbol " + std::to_string(strProgram[i][j].second) + ": " + strProgram[i][j].first + " - Invalid bracket");
						}
						if (exitFlag) break;
					}
				}
				else {
					begin = 0;
					end = 0;
					//						context = "GLOBAL";
				}

				program.push_back(new myoperators{ word, lineInd, size_t(wordPos), begin, end });
			}

			// CONSTANT
			else if (constant::isValidConstant(word)) {
				program.push_back(new constant{ word, lineInd, size_t(wordPos) });
			}

			// VARIABLE OR FUNCTION
			else if (variable::isValidVariable(word)) {
				// function
				if (wordInd < strProgram[lineInd].size() - 1 && strProgram[lineInd][wordInd + 1].first == "(") {
					// ���� ����������� figure ������, ���������� � begin, ���������� �������. ����� ����� ����������� � ������� ���� ����� - ��� end

					function* tmpptr = new function{ word, lineInd, size_t(wordPos), dataTypeAppeared, begin, end };
					program.push_back(tmpptr);

					// creating function
					if (dataTypeAppeared != -1) {
						if (functions.find(*tmpptr) != functions.end()) throw std::runtime_error("Line " + std::to_string(lineInd) + ", symbol " + std::to_string(wordPos) + ": " + word + " - This function has already exists");

						int bracketCounter = 0;
						counter = 0;
						bool exitFlag = false;
						begin = program.size() - 1;
						for (size_t i = lineInd; i < strProgram.size(); ++i) {
							size_t j;
							if (i == lineInd) j = wordInd;
							else j = 0;
							for (; j < strProgram[i].size(); ++j, ++counter) {
								if (strProgram[i][j].first == "{") {
									if (bracketCounter == 0) {
										//											lineBegin = i;
										//											wordBegin = j;
									}
									++bracketCounter;
								}
								if (strProgram[i][j].first == "}") {
									--bracketCounter;
									if (bracketCounter == 0) {
										end = counter + begin;
										exitFlag = true;
										break;
									}
								}
								if (bracketCounter < 0)  throw std::runtime_error("Line " + std::to_string(i) + ", symbol " + std::to_string(strProgram[i][j].second) + ": " + strProgram[i][j].first + " - Invalid bracket");
							}
							if (exitFlag) break;
						}
						tmpptr->begin = begin;
						tmpptr->end = end;

						functions.insert(*tmpptr);
						dataTypeAppeared = -1;
						//							context = word;
					}
					// function is just being called
					else {
						if (functions.find(*tmpptr) == functions.end()) throw std::runtime_error("Line " + std::to_string(lineInd) + ", symbol " + std::to_string(wordPos) + ": " + word + " - There is no function with this name");
						tmpptr->type = (*functions.find(*tmpptr)).type;
						tmpptr->begin = (*functions.find(*tmpptr)).begin;
						tmpptr->end = (*functions.find(*tmpptr)).end;
					}
				}
				// variable
				else {
					arrCounter = 0;
					// ��������� ����������� �������
//						program[lineInd].push_back(new variable{ word, lineInd, size_t(wordPos), dataTypeAppeared, context, arrCounter });
					program.push_back(new variable{ word, lineInd, size_t(wordPos), dataTypeAppeared, arrCounter });
					dataTypeAppeared = -1;
				}
			}
			else {
				throw std::runtime_error("Line " + std::to_string(lineInd) + ", symbol " + std::to_string(wordPos) + ": " + word + " - Unknown word");
			}
		}
	}
	// ������� ��������, ��� ������������� ���������� ����� ����� �� -1 ��� ������, � ����� ����� ����� -1. ��� � �������!

	// ������, ����� ���������� ������� functions ������ �� �����, �.�. ��� ������� �������� �������� � ���������. ����� ������ ����������� �������.
	// ������� �������� ������� � �������� ������� ���������
	// ���������, ��� �������� � ���������


	for (wordInd = 0; wordInd < program.size(); ++wordInd) {
		std::cout << wordInd << ":	";
		program[wordInd]->showInfo();
		std::cout << std::endl;
	}



	// 3. �������� ������� � ���������� � ��������������� �������, ����������� ������ ��������
}

