#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

void printFileContent(std::vector<std::string>& v) {
	for (int i = 0; i < v.size(); ++i) {
		cout << v[i] << endl;
	}
	cout << endl;
}

int main() {
	std::fstream file("D:/Users/maksi/Desktop/dz_38/UNN/mp2-2024-lab5-interpretator/code/code.cpp");
	std::string str;
	std::vector<std::string> file_content;
	while (std::getline(file, str)) {
		file_content.push_back(str);
	}
	printFileContent(file_content);

	return 0;
}