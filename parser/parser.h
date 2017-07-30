#ifndef PARSER_H
#define PARSER_H

#include <string>

class Parser {
	public:
		Parser();
		Parser(std::string _str);

		double parse();
		bool failure;
	
	private:
		void next_char();
		bool check_char(int char_to_check);
		void fail(std::string error);
		double parse_expression();
		double parse_term();
		double parse_factor();

		std::string str;
		int pos;
		int ch;
};

#endif
