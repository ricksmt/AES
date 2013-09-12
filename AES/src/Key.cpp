/*
 * Key.cpp
 *
 *  Created on: Sep 11, 2013
 *      Author: ricksmt
 */

#include "Key.h"

	// Public methods
	Key::Key(std::istream& stream) {
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
