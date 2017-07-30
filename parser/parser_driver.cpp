#include "parser.h"
#include <string>
#include <iostream>

// A recursive function for our recursive descent parser!
int get_input() {
	std::string input;
	std::cout << "Enter your equation: ";
	getline(std::cin, input);

	if (input == "q" || input == "quit") {
		return 0;
	}

	Parser* p = new Parser(input);
	double calculation = p->parse();
	if (p->failure) {	
		delete p;
		std::cout << "Try again?" << std::endl;
		get_input();
	}
	else {
		std::cout << calculation;
	}
}

int main() {
	get_input();
	return 0;
}
