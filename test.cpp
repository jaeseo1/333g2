#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
#include <stdbool.h>
#include <iostream>

#include <string>

#define BUFFER_SIZE 64
#define CHAIN_SIZE 3 // n-chain

using namespace std;

string encode(string plain, string key);
int getKeyLength(string cipher);
int getOffsetMatch(string cipher, int offset);
int max(int array[], int length);
string getKey(string ciphertext, int keyLength);
char decodeChar(char c, char k);
int getMapRow(int cipher, int col);

#define MAP_SIZE 16

static char map[MAP_SIZE][MAP_SIZE] = {
	{0x7, 0x5, 0x0, 0x4, 0x2, 0x3, 0xb, 0x6, 0xa, 0x8, 0x9, 0xd, 0xc, 0xf, 0xe, 0x1},
	{0x3, 0x8, 0xd, 0xa, 0xc, 0xe, 0xf, 0xb, 0x7, 0x6, 0x4, 0x5, 0x1, 0x2, 0x0, 0x9},
	{0x4, 0x0, 0x3, 0x1, 0xb, 0xa, 0x8, 0x5, 0x9, 0xd, 0xc, 0xe, 0xf, 0x6, 0x7, 0x2},
	{0x9, 0xe, 0x7, 0xc, 0x6, 0x4, 0x5, 0xd, 0x1, 0x0, 0x2, 0x3, 0xb, 0x8, 0xa, 0xf},
	{0x1, 0x3, 0xa, 0x2, 0x8, 0x9, 0xd, 0x0, 0xc, 0xe, 0xf, 0x7, 0x6, 0x5, 0x4, 0xb},
	{0xe, 0x6, 0x5, 0x7, 0x1, 0x0, 0x2, 0xf, 0x3, 0xb, 0xa, 0x8, 0x9, 0xc, 0xd, 0x4},
	{0x2, 0xa, 0x9, 0xb, 0xd, 0xc, 0xe, 0x3, 0xf, 0x7, 0x6, 0x4, 0x5, 0x0, 0x1, 0x8},
	{0x6, 0x1, 0x2, 0x5, 0x3, 0xb, 0xa, 0x4, 0x8, 0x9, 0xd, 0xc, 0xe, 0x7, 0xf, 0x0},
	{0xb, 0x9, 0xc, 0x8, 0xe, 0xf, 0x7, 0xa, 0x6, 0x4, 0x5, 0x1, 0x0, 0x3, 0x2, 0xd},
	{0x0, 0xb, 0x8, 0x3, 0x9, 0xd, 0xc, 0x2, 0xe, 0xf, 0x7, 0x6, 0x4, 0x1, 0x5, 0xa},
	{0x8, 0xc, 0xf, 0xd, 0x7, 0x6, 0x4, 0x9, 0x5, 0x1, 0x0, 0x2, 0x3, 0xa, 0xb, 0xe},
	{0x5, 0x2, 0xb, 0x0, 0xa, 0x8, 0x9, 0x1, 0xd, 0xc, 0xe, 0xf, 0x7, 0x4, 0x6, 0x3},
	{0xd, 0xf, 0x6, 0xe, 0x4, 0x5, 0x1, 0xc, 0x0, 0x2, 0x3, 0xb, 0xa, 0x9, 0x8, 0x7},
	{0xc, 0x7, 0x4, 0xf, 0x5, 0x1, 0x0, 0xe, 0x2, 0x3, 0xb, 0xa, 0x8, 0xd, 0x9, 0x6},
	{0xa, 0xd, 0xe, 0x9, 0xf, 0x7, 0x6, 0x8, 0x4, 0x5, 0x1, 0x0, 0x2, 0xb, 0x3, 0xc},
	{0xf, 0x4, 0x1, 0x6, 0x0, 0x2, 0x3, 0x7, 0xb, 0xa, 0x8, 0x9, 0xd, 0xe, 0xc, 0x5}};



int getHigh(int text) {
	// returns the high 4 bits of the given int
	int high;
	high = (text >> 4);
	return high;
}

int getLow(int c) {
	// returns the low 4 bits of the given char
	int low;
	low = (c & 15);
	return low;
}

int main(int argc, char* argv[]) {
	FILE *fp;
    int c;
    string ciphertext;

	//ciphertext arg
    if (argc != 2) {
        fprintf(stderr, "specify ciphertext\n", argv[0]);
        exit(1);
    }
    if (!(fp = fopen(argv[1], "rt"))) {
        perror(argv[1]);
        exit(1);
    }
        
    while ((c = fgetc(fp)) != EOF) {
		ciphertext += c;
		printf("%d\n", c);
    }
    
    fclose(fp);
    
    int keyLength = getKeyLength(ciphertext);
    printf("keyLength is %d\n", keyLength);
    string key = getKey(ciphertext, keyLength);
    
    printf("key is %s\n", key.c_str());
    //string plaintext = decodeString(ciphertext, key);
    	
	return 0;
}

string getKey(string ciphertext, int keyLength){
	string key = "";
	int possibleKeys[] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',
						  'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
						  '1','2','3','4','5','6','7','8','9','0'};

	int cipherLength = ciphertext.length();
	
	for(int i = 0; i < cipherLength; i++){
		printf("%d\n", ciphertext[i]);
	}
	
	//for each character of key to guess
	for (int i = 0; i < 1; i++){
		//for each possible character
		for(int j = 0; j < 62; j++){
			bool thisIsIt = true;
			
			int matched = 0;
			//for each position in ciphertext that matches with the i'th key character
			for(int k = 0; k < cipherLength; k += keyLength){
				printf("index of ciphertext %d is %d\n", ciphertext[k], k);
				int decoded = decodeChar(ciphertext[k], possibleKeys[j]);
				//printf("decoded for cipher %c, key %c: %d\n", ciphertext[k], possibleKeys[j], decoded);
				if (decoded < 32 || decoded > 126){
					thisIsIt = false;
					break;
				} else {
					matched ++;
				}
			}
			
			//printf("matched %d for key %c", matched, possibleKeys[j]);
			
			if(thisIsIt){
				//printf("key char %d is: %c\n", i, possibleKeys[j]);
				key += possibleKeys[j];
				break;
			} else {
				//printf("well it aint %c\n", possibleKeys[j]);
			}
		}
	}
	
	return key;
}

char decodeChar(char c, char k){
	int ch = getHigh(c);
	int cl = getLow(c);
	
	int kh = getHigh(k);
	int kl = getLow(k);
	
	int ph = getMapRow(ch, kl);
	int pl = getMapRow(cl, kh);
	
	int p = (ph << 4) + pl;
}

int getMapRow(int cipher, int col){
	for(int i = 0; i < MAP_SIZE; i++){
		if (map[i][col] == cipher)
			return i;
	}
	
	return -1;
}

//encode plaintext with the key using the map above
string encode(string plain, string key){

	string cipher = "";
	int keyLength = key.length();
	int keyPos = 0;
	
	for (int i = 0; i < plain.length(); i++){
		if (!(keyPos < keyLength)) keyPos = 0;
		int p = plain[i];
		int k = int(key[keyPos]);
				
		int ch = map[getHigh(p)][getLow(k)];
		int cl = map[getLow(p)][getHigh(k)];
	
		int c = (ch << 4) + cl;
		
		cipher += c;
		keyPos++;
	}
	
	return cipher;
}

//get the key length by offseting the ciphertext by incrementing amounts
//and counting how many characters match
int getKeyLength(string cipher){
	int MAX_LENGTH =12;
	int matches[MAX_LENGTH];
	for (int i=1; i < MAX_LENGTH + 1; i++){
		matches[i] = getOffsetMatch(cipher, i);
		printf("matches for %d: %d\n", i, matches[i]);
	}
	
	return max(matches,MAX_LENGTH);
}

//get number of chars that match with the offset
int getOffsetMatch(string cipher, int offset){
	int cipherLength = cipher.length();
	int matches = 0;
	
	//for each char in the offset cipher (gets cut off at the end)
	for(int i = 0; i < cipherLength - offset; i++){
		//compare to each char at original cipher, + offset chars
		if (cipher[i] == cipher[i+offset]) matches++;
	}
	
	return matches;
}

//get max of array
int max(int array[], int length){
	int max = 0;
	int index = 0;

	for(int i = 1; i < length; i++){
		if (array[i] > max){
			max = array[i];
			index = i;
		}
	}
	
	return index;
}
