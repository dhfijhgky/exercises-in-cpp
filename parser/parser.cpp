// A first (second, technically) attempt at a recursive descent parser.
// The idea is that we take some string as input written as a typical equation.
// Then, we parse the equation and solve it.
// The next step is making it work with 3D vector math (how do we return a double OR a vector?).

#include "parser.h"
#include <string>
#include <iostream>
#include <math.h>
#include <stdlib.h>

#define PI 3.14159265359
#define E 2.71828182845

// Default constructor
Parser::Parser() {
	str = "";
	failure = false;
	pos = ch = -1;
}

// Overloaded constructor
// Creates a Parser object with a string to parse.
Parser::Parser(std::string _str) {
	str = _str;
	failure = false;
	pos = ch = -1;
}

// Sets str
void Parser::set_string(std::string _str) {
	str = _str;
}

// Returns str
std::string Parser::get_string() {
	return str;
}

// Begins the parsing process, then returns the result.
// This is the only public function.
double Parser::parse() {
	next_char();
	double x = parse_expression();

	if (pos < str.length()) {
		std::string error = "Unexpected: ";
		error += (char)ch;
		fail(error);
	}

	return x;
}

// Sets ch to the next element in our string, or to -1 if there is nothing left.
void Parser::next_char() {
	ch = (++pos < str.length()) ? str[pos] : -1;
}

// Finds the next character that isn't a space in our string and checks if it matches a given character.
bool Parser::check_char(int char_to_check) {
	while (ch == ' ') next_char();
	if (ch == char_to_check) {
		next_char();
		return true;
	}
	return false;
}

// Function that is run on parse "failure" (doesn't know how to deal with the input for whatever reason).
// Currently spits out an error message and sets failure to true.
// This is a "first come, first served" type of deal and will not give an full list of errors, only the first.
void Parser::fail(std::string error) {
	if (!failure) {
		failure = true;
		std::cout << error << std::endl;
	}
}

// The first recursive function in our chain.
// This deals with the lowest priority operators, addition and subtraction.
double Parser::parse_expression() {
	double x = parse_term();
	for (;;) {
		if (check_char('+')) x += parse_term();
		else if (check_char('-')) x-= parse_term();
		else return x;
	}
}

// The second recursive function in our chain.
// This deals with the middling priority operators, multiplication and division.
double Parser::parse_term() {
	double x = parse_factor();
	for (;;) {
		if (check_char('*')) x *= parse_factor();
		else if (check_char('/')) x /= parse_factor();
		else return x;
	}
}

// The third and beefiest recursive function in our chain.
// This deals with high priority operators, parentheses and exponents, in addition to unary operators and a handful of common functions and constants.
double Parser::parse_factor() {
	// Unary operators
	if (check_char('+')) return parse_factor();
	if (check_char('-')) return -parse_factor();

	double x;
	int start_pos = pos;
	// A ( indicates the start of a new expression.
	if (check_char('(')) {
		x = parse_expression();
		// A ) indicates the end.
		check_char(')');
	}
	// If we find anything in our string that's just a number, we convert it to a double.
	else if ((ch >= '0' && ch <= '9') || ch == '.') {
		while ((ch >= '0' && ch <= '9') || ch == '.') next_char();
		x = atof((str.substr(start_pos, pos)).c_str());
		if (check_char('(')) {
			// This allows for shorthand multiplication with parantheses.
			// ie 4(3(5)) is now valid input.
			x *= parse_expression();
			check_char(')');
		}
	}
	// If we find any sequence of lower case characters, we treat it as a function.
	else if (ch >= 'a' && ch <= 'z') {
		while (ch >= 'a' && ch <= 'z') next_char();
		std::string function = str.substr(start_pos, pos - start_pos);
		// Since the values that functions modify come after, we check what is after our function first.
		x = parse_factor();

		// Then, we actually modify it.
		if (function == "sqrt") x = sqrt(x);
		else if (function == "sin") x = sin(x * (PI / 180));
		else if (function == "arcsin") x = asin(x * (PI / 180));
		else if (function == "cos") x = cos(x * (PI / 180));
		else if (function == "acos") x = acos(x * (PI / 180));
		else if (function == "tan") x = tan(x * (PI / 180));
		else if (function == "atan") x = atan(x * (PI / 180));
		else {
			std::string error = "Unknown function: ";
			error += function;
			fail(error);
		}
	}
	// If we find any sequence of upper case characters, we treat it as a constant.
	else if (ch >= 'A' && ch <= 'Z') {
		while (ch >= 'A' && ch <= 'Z') next_char();
		std::string constant = str.substr(start_pos, pos - start_pos);
		if (constant == "PI") x = PI;
		else if (constant == "E") x = E;
		else {
			std::string error = "Unknown constant: ";
			error += constant;
			fail(error);
		}
	}
	else {
		std::string error = "Unexpected symbol: ";
		error += (char)ch;
		fail(error);
	}

	// Finish off with an exponent operator.
	if (check_char('^')) x = pow(x, parse_factor());

	return x;
}
