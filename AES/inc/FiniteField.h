/*
 * FiniteField.h
 *
 *  Created on: Sep 7, 2013
 *      Author: ricksmt
 */

#ifndef FINITEFIELD_H_
#define FINITEFIELD_H_

class FiniteField {

	static const char m = 0x011b;
	char element;

public:
	FiniteField(const char& c): element(c) { }
	operator char() const { return element; }

	char ffAdd(const char x) { return element = ffAdd(element, x); }
	static char ffAdd(const char a, const char b) { return a ^ b; }

	char ffMultiply(const char x) { return ffMultiply(element, x); }
	static char ffMultiply(const char a, const char b) {
		char sum = 0x00, running = a;
		for(char i = 0x01; i & 0x0FF; i = i << 1){
			if(i & b) sum += running;
			running = xtime(running);
		}
		return sum;
	}

private:

	static char xtime(const char c) {
		char x = c << 1;
		if(x & 0x0100) x = x ^ m;
		return x;
	}
};


#endif /* FINITEFIELD_H_ */
