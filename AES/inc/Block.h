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

#include "AES.h"

template <int Nb = AES::Nb>
class Block {

private:
	FiniteField state[4][Nb];

public:
	Block(const Block& block) {
	    // Initialize state
	}
	Block(std::istream& stream) {
	    // Initialize state
	    for(int i = 0; i < 4; i++) state[i] = new FiniteField[Nb];
	    for(int i = 0; i < byteLength(); i++){
	    	if(stream.good()) stream.read((char*)(state[i & 0x03] + i >> 2), 1);
	    	else state[i & 0x03][i >> 2] = 0;// Zero out the excess space
	    }
	}
	bool operator ==(const Block& block) const {
		if(length() != block.length()) return false;
		for(int i = 0; i < 4; i++){
			for(int j = 0; j < Nb; j++){
				if(state[i][j] != block.state[i][j]) return false;
			}
		}
		return true;
	}
	bool operator !=(const Block& block) const { return !(this == block); }
	const FiniteField[] operator[](const unsigned int& index) { return state[index]; }

	// The number of bits in this block
	int length() const { return bitLength(); }
	int bitLength() const { return byteLength() << 3; }
	int byteLength() const { return Nb << 2; }

protected:
	// Encryption methods
	void SubBytes() {
		for(int i = 0; i < 4; i++){
			for(int j = 0; j < Nb; j++){
				FiniteField c = state[i][j];
				int x = (c & 0xF0) >> 4, y = c & 0x0F;
				state[i][j] = AES::Sbox[x][y];
			}
		}
	}

	void ShiftRows() {
		std::stack<FiniteField> stack;
		for(int i = 1; i < 4; i++){
			for(int j = 0; j < i; j++) stack.push(state[i][j]);
			for(int j = i; j < Nb; j++) state[i][j] = state[i][j + i];
			for(int j = 0; j < i; j++, stack.pop()) state[i][Nb - 1 - j] = stack.top();
		}
	}

	void MixColumns() {
		for(int i = 0; i < Nb; i++){
			FiniteField temp[] = {state[0][i], state[1][i], state[2][i], state[3][i]};
			state[0][i] = 2 * temp[0] ^ 3 * temp[1] ^ 1 * temp[2] ^ 1 * temp[3];
			state[1][i] = 1 * temp[0] ^ 2 * temp[1] ^ 3 * temp[2] ^ 1 * temp[3];
			state[2][i] = 1 * temp[0] ^ 1 * temp[1] ^ 2 * temp[2] ^ 3 * temp[3];
			state[3][i] = 3 * temp[0] ^ 1 * temp[1] ^ 1 * temp[2] ^ 2 * temp[3];
		}
	}

	void AddRoundKey(const Key& key) {
		for(int i = 0; i < 4; i++){
			for(int j = 0; j < Nb; j++){
				state[i][j] = state[i][j] + key[i][j];
			}
		}
	}

	// Decryption methods
	void InvSubBytes();

	void InvShiftRows();

	void InvMixColumns();
};

#endif /* BLOCK_H_ */
