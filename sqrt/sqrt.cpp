// I wrote this program because I was curious how square roots were calculated.
// One of the big features is a really solid initial guess that exponential cuts down on operations needed to get to a satisfactory answer.
// Our truncate, factorial, and digits functions all help to find that first guess.

#include <iostream>

// Takes a number and chops off a few digits, then returns the result.
int truncate(int num, int digits) {

	// Since we're counting in base 10, we truncate using 10.
	int trunc = 10;

	for (;digits > 1; --digits) {
		trunc *= 10;
	}

	return (num - (num % trunc)) / trunc;
}

// Returns the factorial of some integer in as little operations as possible.
int factorial(int num) {
	int x = 1;
	for (;num > 1; --num) {
		x *= num;
	}
	return x;
}

// Uses our truncate(int, int) function to determine how many digits are in a number.
int digits(int num) {
	int x = 0;
	for (;num != 0; num = truncate(num, 1)) {
		x++;
	}
	return x;
}

// The meat of our calculations.
// This will estimate a square root.
double sqrt(int num) {

	// First we're going to want to find an initial guess.
	// The better this guess is, the faster our operation will be.
	// We find our first guess as follows:
	// If x = a * 10^(2n) where 1 <= a < 100 and n is an integer,
	// sqrt(x) = { 2 * 10^n if a < 10
	// 	     { 6 * 10^n if a >= 10
	
	// Start by getting our number below 100.
	int temp = num;
	int x = 0;
	for (;temp > 100; temp = truncate(temp, 2)) {
		x++;
	}

	// Check if a is < 10 or >= 10.
	// Our guess double should always be 2 or 6 (2! and 3!, respectively).
	double guess = factorial(digits(temp) + 1);

	// Multiply by 10^n.
	for (; x > 0; --x) {
		guess *= 10;
	}
	
	// Decide when we are satisfied with the result.
	double error_threshold = .01;

	// Actually get to calculating the square root.
	// We repeatedly find the average of our guess and our original number divided by our guess.
	// Eventually we will converge on the number we want.
	int operations = 0;
	while(!(guess * guess - num < error_threshold && guess * guess - num > error_threshold * -1)) {
		guess = (guess + (num / guess)) / 2;
		++operations;
	}
	std::cout << "With our initial guess, we found the square root in " << operations << " operations." << std::endl;

	// Do it again for good measure.
	operations = 0;
	guess = 1;
	while(!(guess * guess - num < error_threshold && guess * guess - num > error_threshold * -1)) {
		guess = (guess + (num / guess)) / 2;
		++operations;
	}
	std::cout << "Without it, it took " << operations << " operations." << std::endl;
	return guess;
}
