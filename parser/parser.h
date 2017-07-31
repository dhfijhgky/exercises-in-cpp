#ifndef PARSER_H
#define PARSER_H

#include <string>

class Parser {
	public:
		// Constructors
		Parser();
		Parser(std::string _str);

		// Setters and getters
		void set_string(std::string _str);
		std::string get_string();

		// Drives the function
		double parse();

		// Is there a problem with input?
		bool failure;
	
	private:
		// Increments through the string
		void next_char();

		// Is current element equal to this?
		bool check_char(int char_to_check);

		// Executed when invalid input is found
		void fail(std::string error);

		// Recursive functions from lowest to highest priority
		double parse_expression();
		double parse_term();
		double parse_factor();

		// String to parse
		std::string str;

		// Current parsing position in str
		int pos;

		// Value at pos element in str
		int ch;
};

#endif
