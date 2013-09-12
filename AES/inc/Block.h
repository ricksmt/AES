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

class Block {

protected:
	static const int Nb = AES::Nb;
private:
	FiniteField state[4][Nb];

public:
	Block(const Block& stream);
	Block(std::istream& stream);

	// The number of bits in this block
	int length() const { return bitLength(); }
	int bitLength() const { return byteLength() << 3; }
	int byteLength() const { return Nb << 2; }

protected:
	// Encryption methods
	void SubBytes();

	void ShiftRows();

	void MixColumns();

	void AddRoundKey(const Key& key);

	// Decryption methods
	void InvSubBytes();

	void InvShiftRows();

	void InvMixColumns();
};

#endif /* BLOCK_H_ */
