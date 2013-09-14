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
		Block(const Block<Nb>& block) { for(int i = 0; i < Nb; i++) array[i] = block.array[i]; }
		Block(std::string& s) {
			// Initialize state
			for(int i = 0; i < s.length() / 4; i++) {
				for(int j = 0; j < Nb; j++) {
					array[i][j] = s[i * 4 + j];
				}
			}
		}

		Block<Nb>& operator =(const Block<Nb>& b) {
			if (this != &b) {
				for(unsigned i = 0; i < Nb; i++) word(i) = b.array[i];
			}
			return *this;
		}
		bool operator ==(const Block<Nb>& block) const {
			for(int i = 0; i < Nb; i++) if(array[i] != block.array[i]) return false;
			return true;
		}
		bool operator !=(const Block<Nb>& block) const { return !(*this == block); }
		Row& operator[](unsigned index) {
			Row row;
			for(unsigned i = 0; i < Nb; i++) row[i] = array[i][index];
			return row;
		}

		// The number of bits in this block
		int length() const { return bitLength(); }
		int bitLength() const { return byteLength() << 3; }
		int byteLength() const { return Nb << 2; }
		Column& word(unsigned index) { return array + index; }

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
		void InvSubBytes() {
			for(int i = 0; i < 4; i++){
				for(int j = 0; j < Nb; j++){
					FiniteField c = array[i][j];
					int x = (c & 0xF0) >> 4, y = c & 0x0F;
					array[i][j] = AES::InvSbox[x][y];
				}
			}
		}

		void InvShiftRows() {
			std::stack<FiniteField> stack;
			for(int i = 1; i < 4; i++){
				for(int j = Nb; Nb - j < i; j--) stack.push(array[i][j - 1]);
				for(int j = Nb - i; j >= i; j--) array[i][j - 1] = array[i][j - 2];
				for(int j = i; j > 0; j--, stack.pop()) array[i][j - 1] = stack.top();
			}
		}

		void InvMixColumns() {
			for(int i = 0; i < Nb; i++){
				FiniteField temp[] = {array[0][i], array[1][i], array[2][i], array[3][i]};
				array[0][i] = 0x0E * temp[0] ^ 0x0B * temp[1] ^ 0x0D * temp[2] ^ 0x09 * temp[3];
				array[1][i] = 0x09 * temp[0] ^ 0x0E * temp[1] ^ 0x0B * temp[2] ^ 0x0D * temp[3];
				array[2][i] = 0x0D * temp[0] ^ 0x09 * temp[1] ^ 0x0E * temp[2] ^ 0x0B * temp[3];
				array[3][i] = 0x0B * temp[0] ^ 0x0D * temp[1] ^ 0x09 * temp[2] ^ 0x0E * temp[3];
			}
		}
	};

	template <int Nb>
	int Nr(const Block<Nb>& b, const Key<Nb>& k) {
		// While technically a function of b and k...
		return 6 + k.Nk;
	}
}

#endif /* BLOCK_H_ */
