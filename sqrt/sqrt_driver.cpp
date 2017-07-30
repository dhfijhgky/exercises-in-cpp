#include<iostream>
#include"sqrt.h"

int main() {
	int num;
	std::cout << "Please enter a positive integer: ";
	std::cin >> num;
	std::cout << "The square root of " << num << " is roughly " << sqrt(num) << ".";
	return 0;
}

