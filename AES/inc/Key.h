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

		Word<Nb> array[];
	protected:
		int Nk;

	public:
		Key(const Key& key): Nk(key.Nk) {
			for(int i = 0; i < Nb; i++) {
				for(int j = 0; j < Nk; j++) {
					array[i][j] = key.array[i][j];
				}
			}
		}
		Key(std::istream& stream) {
			// Get the length
			stream.seekg(stream.end);
			int length = stream.tellg();
			stream.seekg (0, stream.beg);
			Nk = length / Nb + (length % Nb ? 1 : 0);

			// Initialize state
			for(int i = 0; i < Nb; i++) array[i] = new FiniteField[Nk];
			for(int i = 0; i < byteLength(); i++){
				if(stream.good()) stream.read((char*)(array[i & Nb] + i / Nb), 1);
				else array[i & Nb][i / Nb] = 0;// Zero out the excess space
			}
		}
		const FiniteField[] operator[](const unsigned int& index) { return array[index]; }

		// The number of bits in this block
		int length() const { return bitLength(); }
		int bitLength() const { return byteLength() << 3; }
		int byteLength() const { return Nb * Nk; }

		Word& word(int index) const {
			return array + index;
		}
		void setWord(Word w, int index) const {
			for(int i = 0; i < Nb; i++) array[i][index] = w[i];
		}

		//Methods
		Key expand(int rconIndex) const {
			Key key = this;
			Word word = getWord(Nk - 1);
			word.rotWord();
			word.subWord();
			word[0] = word[0] + AES::Rcon[rconIndex] >> 24;
			key.setWord(word, 0);
			for(int i = 1; i < Nk; i++) {
				for(int j = 0; j < Nb; j++) {
					FiniteField ff = key[j][i - 1];
					if(Nk == 8 && i == 4) {
						int x = (ff & 0xF0) >> 4, y = ff & 0x0F;
						ff = ff + AES::Sbox[x][y];
					}
					key[j][i] = key[j][i] + ff;
				}
			}
			return key;
		}
	};
}

#endif /* KEY_H_ */
