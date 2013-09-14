/*
 * Word.h
 *
 *  Created on: Sep 13, 2013
 *      Author: ricksmt
 */

#ifndef WORD_H_
#define WORD_H_

#include "FiniteField.h"

namespace AES {

	template <int size>
	class Word {

		FiniteField array[size];

	public:
		Word(const Word<size>& w) { for(int i = 0; i < size; i++) array[i] = w.array[i]; }
		Word(const FiniteField values[size]) { for(int i = 0; i < size; i++) array[i] = values[i]; }
		Word(int value) { for(int i = size - 1; i >= 0; i--, value = value >> 8) array[i] = value & 0xFF; }
		Word(unsigned int value = 0) { for(int i = size - 1; i >= 0; i--, value = value >> 8) array[i] = value & 0xFF; }

		Word<size>& operator =(const Word<size>& w) {
			if (this != &w) {
				for(int i = 0; i < size; i++) array[i] = w.array[i];
			}
			return *this;
		}
		FiniteField& operator [](unsigned index) { return array + index; }
		bool operator ==(const Word<size>& w) const {
			for(int i = 0; i < size; i++) if(array[i] != w.array[i]) return false;
			return true;
		}
		bool operator !=(const Word<size>& w) const { return !(*this == w); }

	protected:
		void SubWord() {
			for(int i = 0; i < size; i++) {
				int x = (this[i] & 0xF0) >> 4, y = this[i] & 0x0F;
				this[i] = this[i] + Sbox[x][y];
			}
		}

		void RotWord() {
			FiniteField temp = this[0];
			for(int i = 1; i < size; i++) this[i - 1] = this[i];
			this[size - 1] = temp;
		}
	};

	const int Nb = 4;

	Word<Nb> Rcon[] = { 0x00000000,// Rcon[] is 1-based, so the first entry is just a place holder
		0x01000000, 0x02000000, 0x04000000, 0x08000000,
		0x10000000, 0x20000000, 0x40000000, 0x80000000,
		0x1B000000, 0x36000000, 0x6C000000, 0xD8000000,
		0xAB000000, 0x4D000000, 0x9A000000, 0x2F000000,
		0x5E000000, 0xBC000000, 0x63000000, 0xC6000000,
		0x97000000, 0x35000000, 0x6A000000, 0xD4000000,
		0xB3000000, 0x7D000000, 0xFA000000, 0xEF000000,
		0xC5000000, 0x91000000, 0x39000000, 0x72000000,
		0xE4000000, 0xD3000000, 0xBD000000, 0x61000000,
		0xC2000000, 0x9F000000, 0x25000000, 0x4A000000,
		0x94000000, 0x33000000, 0x66000000, 0xCC000000,
		0x83000000, 0x1D000000, 0x3A000000, 0x74000000,
		0xE8000000, 0xCB000000, 0x8D000000};
}

#endif /* WORD_H_ */
