/*
 * Block.cpp
 *
 *  Created on: Sep 7, 2013
 *      Author: ricksmt
 */

#include "Block.h"

	// Public methods
	Block::Block(const Block& block) {
	    // Initialize state
	}

	Block::Block(std::istream& stream) {
	    // Initialize state
	    for(int i = 0; i < 4; i++) state[i] = new FiniteField[Nb];
	    for(int i = 0; i < byteLength(); i++){
	    	if(stream.good()) stream.read((char*)(state[i & 0x03] + i >> 2), 1);
	    	else state[i & 0x03][i >> 2] = 0;// Zero out the excess space
	    }
	}

	// Encryption methods
	void Block::SubBytes() {
		for(int i = 0; i < 4; i++){
			for(int j = 0; j < Nb; j++){
				FiniteField c = state[i][j];
				int x = (c & 0xF0) >> 4, y = c & 0x0F;
				state[i][j] = AES::Sbox[x][y];
			}
		}
	}

	void Block::ShiftRows() {
		std::stack<FiniteField> stack;
		for(int i = 1; i < 4; i++){
			for(int j = 0; j < i; j++) stack.push(state[i][j]);
			for(int j = i; j < Nb; j++) state[i][j] = state[i][j + i];
			for(int j = 0; j < i; j++, stack.pop()) state[i][Nb - 1 - j] = stack.top();
		}
	}

	void Block::MixColumns() {
		for(int i = 0; i < Nb; i++){
			FiniteField temp[] = {state[0][i], state[1][i], state[2][i], state[3][i]};
			state[0][i] = 2 * temp[0] ^ 3 * temp[1] ^ 1 * temp[2] ^ 1 * temp[3];
			state[1][i] = 1 * temp[0] ^ 2 * temp[1] ^ 3 * temp[2] ^ 1 * temp[3];
			state[2][i] = 1 * temp[0] ^ 1 * temp[1] ^ 2 * temp[2] ^ 3 * temp[3];
			state[3][i] = 3 * temp[0] ^ 1 * temp[1] ^ 1 * temp[2] ^ 2 * temp[3];
		}
	}

	void Block::AddRoundKey(const Key& key) {
		for(int i = 0; i < 4; i++){
			for(int j = 0; j < Nb; j++){

			}
		}
	}

	// Decryption methods
