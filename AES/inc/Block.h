/*
 * Block.h
 *
 *  Created on: Sep 7, 2013
 *      Author: ricksmt
 */

#ifndef BLOCK_H_
#define BLOCK_H_

#include <istream>
#include <stack>

#include "FiniteField.h"
#include "Key.h"

namespace AES {

	template <int Nb>
	class Block {

		typedef Word<4> Column;
		typedef Word<Nb> Row;

		Column array[Nb];

	public:
		Block(const Block& block) { for(int i = 0; i < Nb; i++) array[i] = block[i]; }
		Block(std::istream& stream) {
			// Initialize state
			for(int i = 0; i < 4; i++) array[i] = new FiniteField[Nb];
			for(int i = 0; i < byteLength(); i++){
				if(stream.good()) stream.read((char*)(array[i & 0x03] + i >> 2), 1);
				else array[i & 0x03][i >> 2] = 0;// Zero out the excess space
			}
		}

		Block& operator =(const Block& b) {
			if (this != &b) {
				for(unsigned i = 0; i < Nb; i++) word(i) = b.word(i);
			}
			return *this;
		}
		bool operator ==(const Block<Nb>& block) const {
			for(int i = 0; i < Nb; i++) if(array[i] != block.array[i]) return false;
			return true;
		}
		bool operator !=(const Block& block) const { return !(*this == block); }
		const Row& operator[](unsigned index) {
			Row row;
			for(unsigned i = 0; i < Nb; i++) row[i] = array[i][index];
			return row;
		}

		// The number of bits in this block
		int length() const { return bitLength(); }
		int bitLength() const { return byteLength() << 3; }
		int byteLength() const { return Nb << 2; }
		Column& word(unsigned index) { return array + index; }

	protected:
		// Encryption methods
		void SubBytes() {
			for(int i = 0; i < 4; i++){
				for(int j = 0; j < Nb; j++){
					FiniteField c = array[i][j];
					int x = (c & 0xF0) >> 4, y = c & 0x0F;
					array[i][j] = AES::Sbox[x][y];
				}
			}
		}

		void ShiftRows() {
			std::stack<FiniteField> stack;
			for(int i = 1; i < 4; i++){
				for(int j = 0; j < i; j++) stack.push(array[i][j]);
				for(int j = i; j < Nb; j++) array[i][j] = array[i][j + i];
				for(int j = 0; j < i; j++, stack.pop()) array[i][Nb - 1 - j] = stack.top();
			}
		}

		void MixColumns() {
			for(int i = 0; i < Nb; i++){
				FiniteField temp[] = {array[0][i], array[1][i], array[2][i], array[3][i]};
				array[0][i] = 2 * temp[0] ^ 3 * temp[1] ^ 1 * temp[2] ^ 1 * temp[3];
				array[1][i] = 1 * temp[0] ^ 2 * temp[1] ^ 3 * temp[2] ^ 1 * temp[3];
				array[2][i] = 1 * temp[0] ^ 1 * temp[1] ^ 2 * temp[2] ^ 3 * temp[3];
				array[3][i] = 3 * temp[0] ^ 1 * temp[1] ^ 1 * temp[2] ^ 2 * temp[3];
			}
		}

		void AddRoundKey(const Key<Nb>& key) {
			for(int i = 0; i < 4; i++){
				for(int j = 0; j < Nb; j++){
					array[i][j] = array[i][j] + key[i][j];
				}
			}
		}

		// Decryption methods
		void InvSubBytes();

		void InvShiftRows();

		void InvMixColumns();
	};

	template <int Nb>
	int Nr(const Block<Nb>& b, const Key<Nb>& k) {
		// While technically a function of b and k...
		return 6 + k.Nk;
	}
}

#endif /* BLOCK_H_ */
