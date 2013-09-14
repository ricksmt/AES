/*
 * AES.h
 *
 *  Created on: Sep 7, 2013
 *      Author: ricksmt
 */

#ifndef AES_H_
#define AES_H_

#include <iostream>

#include "Block.h"
#include "Key.h"

namespace AES{

	template <int Nb>
	Block<Nb> cipher(const Block<Nb>& b, const Key<Nb>& k) {
		Block<Nb> block = b;
		Key<Nb> key = k;

		block.AddRoundKey(key);

		for(int i = 1; i < Nr(b, k) - 1; i++) {
			key = key.expand(i);
			block.SubBytes();
			block.ShiftRows();
			block.MixColumns();
			block.AddRoundKey(key);
		}

		key = key.expand(Nr(b, k) - 1);
		block.SubBytes();
		block.ShiftRows();
		block.AddRoundKey(key);

		return block;
	}

	template <int Nb>
	Block<Nb> invCipher(const Block<Nb>& b, const Key<Nb>& k) {
		Block<Nb> block = b;
		Key<Nb> key = k;

		block.AddRoundKey(key);

		for(int i = 1; i < Nr(b, k) - 1; i++) {
			key = key.expand(i);
			block.InvShiftRows();
			block.InvSubBytes();
			block.AddRoundKey(key);
			block.InvMixColumns();
		}

		key = key.expand(Nr(b, k) - 1);
		block.InvShiftRows();
		block.InvSubBytes();
		block.AddRoundKey(key);

		return block;
	}
}

#endif /* AES_H_ */
