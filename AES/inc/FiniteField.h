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
	FiniteField(const FiniteField& ff): element(ff.element) { }
	FiniteField(const char& c): element(c) { }
	operator char() const { return element; }

	bool operator ==(const FiniteField& ff) const { return element == ff.element; }
	bool operator !=(const FiniteField& ff) const { return !(this == ff); }
	FiniteField operator +(const FiniteField& ff) const { return element ^ ff.element; }
	FiniteField operator *(const FiniteField& ff) const { return ffMultiply(element, ff.element); }

private:
	FiniteField ffAdd(const FiniteField x) { return element = ffAdd(element, x); }
	static FiniteField ffAdd(const FiniteField a, const FiniteField b) { return a + b; }

	FiniteField ffMultiply(const FiniteField x) { return ffMultiply(element, x); }
	static FiniteField ffMultiply(const FiniteField a, const FiniteField b) {
		char sum = 0x00, running = a;
		for(char i = 0x01; i & 0x0FF; i = i << 1){
			if(i & b) sum += running;
			running = xtime(running);
		}
		return sum;
	}

	static FiniteField xtime(const FiniteField c) {
		bool overflow = c.element & 0x80;
		char x = c.element << 1;
		if(overflow) x = x ^ m;
		return x;
	}
};


#endif /* FINITEFIELD_H_ */
