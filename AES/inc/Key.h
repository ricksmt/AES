/*
 * Key.h
 *
 *  Created on: Sep 7, 2013
 *      Author: ricksmt
 */

#ifndef KEY_H_
#define KEY_H_

#include "Word.h"

namespace AES {

	template <int Nb>
	class Key{

		typedef Word<Nb> Column;

		Word<Nb> array[];

	public:
		int Nk;

		Key(const Key& key): Nk(key.Nk) { for(int i = 0; i < Nk; i++) array[i] = key.array[i]; }
		Key(std::string& s) {
			// Initialize state
			Nk = s.length() / Nb;
			for(int i = 0; i < Nb; i++) array[i] = new FiniteField[Nk];
			for(int i = 0; i < s.length() / 4; i++) {
				for(int j = 0; j < Nb; j++) {
					array[i][j] = s[i * 4 + j];
				}
			}
		}

		FiniteField* operator [](int index) const {
			FiniteField row[Nk];
			for(int i = 0; i < Nk; i++) {
				Word<Nb> column = array[i];
				row[i] = column.array[index];
			}
			return row;
		}

		// The number of bits in this block
		int length() const { return bitLength(); }
		int bitLength() const { return byteLength() << 3; }
		int byteLength() const { return Nb * Nk(); }
		Column& word(int index) { return array + index; }
	};
}

#endif /* KEY_H_ */
