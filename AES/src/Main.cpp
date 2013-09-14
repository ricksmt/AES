/*
 * Main.cpp
 *
 *  Created on: Sep 13, 2013
 *      Author: ricksmt
 */

#include <dirent.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "AES.h"
#include "Block.h"
#include "Key.h"

using namespace std;
using namespace AES;

vector<string> getFileNames(string path = "tests") {

    DIR* dir;
    dirent* pdir;
    vector<string> files;

    dir = opendir(path.c_str());
    while ((pdir = readdir(dir))) files.push_back(pdir->d_name);
    return files;
}

int main() {
	vector<string> filenames = getFileNames();
	for(unsigned i = 0; i < filenames.size(); i++) {
		string filename = filenames[i];
		if(filename[0] == '.') continue;
		filename = "tests/" + filename;
		cout << filename << endl;

		// Setup
		ifstream file(filename.c_str());
		string s = "";
		getline(file, s);
		cout << s << endl;
		s = s.substr(s.find(' ') + 1);
		Block<Nb> input(s);
		getline(file, s);
		cout << s << endl;
		s = s.substr(s.find(' ') + 1);
		Key<Nb> key(s);
		getline(file, s);
		cout << s << endl;
		s = s.substr(s.find(' ') + 1);
		Block<Nb> output(s);

		// Testing
		cout << endl;
		cout << "ENCRYPTION";
		cout << '\t' << (cipher(input, key) == output ? "PASSED" : "FAILED");
		cout << "DECRYPTION";
		cout << '\t' << (invCipher(output, key) == input ? "PASSED" : "FAILED");
		cout << endl;
	}
	return 0;
}


