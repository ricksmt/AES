/*
 * Key.h
 *
 *  Created on: Sep 7, 2013
 *      Author: ricksmt
 */

#ifndef KEY_H_
#define KEY_H_

template <int Nb = AES::Nb>
class Key{

	static FiniteField Rcon[] = { 0x00000000,// Rcon[] is 1-based, so the first entry is just a place holder
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

protected:
	int Nk;
private:
	FiniteField state[Nb][Nk];

	class Word{
		FiniteField word[Nb];

	public:
		FiniteField operator [](int index) const { return word[index]; }

	protected:
		void subWord() const {
			for(int i = 0; i < Nb; i++) {
				int x = (this[i] & 0xF0) >> 4, y = this[i] & 0x0F;
				this[i] = this[i] * AES::Sbox[x][y];
			}
		}

		void rotWord() const {
			FiniteField temp = this[0];
			for(int i = 1; i < Nb; i++) this[i - 1] = this[i];
			this[Nb - 1] = temp;
		}
	};

public:
	Key(const Key& key): Nk(key.Nk) {
		for(int i = 0; i < Nb; i++) {
			for(int j = 0; j < Nk; j++) {
				state[i][j] = key.state[i][j];
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
	    for(int i = 0; i < Nb; i++) state[i] = new FiniteField[Nk];
	    for(int i = 0; i < byteLength(); i++){
	    	if(stream.good()) stream.read((char*)(state[i & Nb] + i / Nb), 1);
	    	else state[i & Nb][i / Nb] = 0;// Zero out the excess space
	    }
	}
	const FiniteField[] operator[](const unsigned int& index) { return state[index]; }

	// The number of bits in this block
	int length() const { return bitLength(); }
	int bitLength() const { return byteLength() << 3; }
	int byteLength() const { return Nb * Nk; }

	Word getWord(int index) const {
		Word w;
		for(int i = 0; i < Nb; i++) w[i] = state[i][index];
		return w;
	}
	void setWord(Word w, int index) const {
		for(int i = 0; i < Nb; i++) state[i][index] = w[i];
	}

	//Methods
	Key expand(int rconIndex) const {
		Key key = this;
		Word word = getWord(Nk - 1);
		word.rotWord();
		word.subWord();
		word[0] = word[0] + Rcon[rconIndex] >> 24;
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


#endif /* KEY_H_ */
