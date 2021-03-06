#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
#include <stdbool.h>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>

#include <string>
#include <vector>

#define BUFFER_SIZE 64
#define CHAIN_SIZE 3 // n-chain

using namespace std;

vector<int> encode(vector<int> plain, vector<int> key);
int getKeyLength(vector<int> cipher);
int getOffsetMatch(vector<int> cipher, int offset);
int max(int array[], int length);
int decodeChar(int c, int k);
int decodeKeyChar(int c, int p);
int getMapRow(int cipher, int col);
int getMapCol(int cipher, int row);
vector<int> decodeAll(vector<int> cipher, vector<int> key);
void printVectorString(vector<int> thing, string toFile);
vector<int> stringToVector(string str);
vector<int> getUpperKey(vector<int> ciphertext, vector<int> header, int keyLength);
vector<int> getLowerKey(vector<int> ciphertext, vector<int> footer, int keyLength);
vector<int> constructKey(vector<int> upperKey, vector<int> lowerKey, int keyLength);

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
    vector<int> ciphertext;

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
		ciphertext.push_back(c);
    }
    
    fclose(fp);
    
    int keyLength = getKeyLength(ciphertext);
    printf("keyLength is %d\n", keyLength);
    
    //mp3 header
    //int headerArray[] = {255,251,160,64};
    //vector<int> header (headerArray, headerArray + sizeof(headerArray) / sizeof(headerArray[0]));
    
    //jpeg header
    //int headerArray[] = {255, 216, 255, 224, 0, 16, 74, 70, 73, 70};
    //vector<int> header (headerArray, headerArray + sizeof(headerArray) / sizeof(headerArray[0]));
    
    //bmp header
    //vector<int> header = stringToVector("BM");
    
    //png header
    //int headerArray[] = {137, 80, 78, 71, 13, 10, 26, 10};
    //vector<int> header (headerArray, headerArray + sizeof(headerArray) / sizeof(headerArray[0]));

	//pdf hedaer
	/*int headerArray[] = {37, 80, 68, 70, 45, 49, 46, 53, 10, 37, 181,181,181,181,10};
    vector<int> header (headerArray, headerArray + sizeof(headerArray) / sizeof(headerArray[0]));
    vector<int> footer = stringToVector("footer");
        
    vector<int> upperKey = getUpperKey(ciphertext, header, keyLength);
    vector<int> lowerKey = getLowerKey(ciphertext, footer, keyLength);
    
    vector<int> key = constructKey(upperKey, lowerKey, keyLength);
    
	printf("Partial key is: ");
    printVectorString(key, "");*/

	/*static const int arr[] = {50,98,114,111,100,115,107,121};
	vector<int> key (arr, arr + sizeof(arr) / sizeof(arr[0]));*/

	vector<int> key = stringToVector("Large_Hadron_Collider_at_CERN_?\"?");
    vector<int> plaintext = decodeAll(ciphertext, key);

	printf("rest of the key should be:\n");
	int teDecode[3] = {103,116,104}; 
	
	for(int i= 31; i < 34; i++){
		int c = decodeKeyChar(ciphertext[i], teDecode[i-31]);
		printf("%c\n", c);
	}

	printf("Plaintext decoded with the partial key:\n");
	printVectorString(plaintext, "output.pdf");
	        	
	return 0;
}

//put together key using upper and lower part, with padding in between
vector<int> constructKey(vector<int> upperKey, vector<int> lowerKey, int keyLength){
	vector<int> key;
	
	for(int i=0; i < upperKey.size(); i++){
		key.push_back(upperKey[i]);
	}
	
	for(int i= upperKey.size(); i < (keyLength - lowerKey.size()); i++){
		key.push_back('x');
	}
	
	int j = 0;
	for(int i= (keyLength - lowerKey.size()); i < keyLength; i++){
		key.push_back(lowerKey[j]);
		j++;
	}
	
	return key;
}

//convert string to a vector of int ascii codes
vector<int> stringToVector(string str){
	vector<int> vec;
	for(int i = 0; i < str.length(); i++){
		vec.push_back(str[i]);
	}
	
	return vec;
}

void printVectorString(vector<int> thing, string toFile){
	ofstream myFile;
	if(toFile != ""){
		myFile.open(toFile.c_str());
	}
	
	int k = 0;
	for(int i = 0; i < thing.size(); i++){
		if (k > 7) k = 0;
		
		if(toFile != ""){
			myFile << (char)thing[i];//thing[i] << " ";
		} else {
			printf("%c", thing[i]);
		}
		
		k++;
	}
	
	if(toFile != ""){
		myFile.close();
	}
	
	printf("\n");
}

//get a partial upper key given cipher text and a possible file header
vector<int> getUpperKey(vector<int> ciphertext, vector<int> header, int keyLength){
	vector<int> upperKey;
	
	int headerLength = header.size();
	
	for(int i = 0; i < headerLength; i++){
		upperKey.push_back(decodeKeyChar(ciphertext[i], header[i]));
	}
	
	return upperKey;
}

//get a partial lower key given cipher text and a possible file footer
vector<int> getLowerKey(vector<int> ciphertext, vector<int> footer, int keyLength){
	vector<int> lowerKey;
	
	int footerLength = footer.size();
	int cipherLength = ciphertext.size();
		
	int j = 0;
	for(int i = (cipherLength - footerLength); i < cipherLength; i++){
		lowerKey.push_back(decodeKeyChar(ciphertext[i], footer[j]));
		j++;
	}
	
	return lowerKey;
}

//decode entire cipher text using key
vector<int> decodeAll(vector<int> cipher, vector<int> key){
	vector<int> plain;
	int keyLength = key.size();
	int keyPos = 0;
	
	for (int i = 0; i < cipher.size(); i++){
		if (!(keyPos < keyLength)) keyPos = 0;

		int c = decodeChar(cipher[i], key[keyPos]);

		plain.push_back(c);
		keyPos++;
	}
	
	return plain;
}

//get a key character given ciphertext and plaintext chars
int decodeKeyChar(int c, int p){
	int ch = getHigh(c);
	int cl = getLow(c);
	
	int ph = getHigh(p);
	int pl = getLow(p);
	
	int kh = getMapCol(cl, pl);
	int kl = getMapCol(ch, ph);
	
	int k = (kh << 4) + kl;
	
	return k;
}

//decode single char given ciphertext and key char
int decodeChar(int c, int k){
	int ch = getHigh(c);
	int cl = getLow(c);
	
	int kh = getHigh(k);
	int kl = getLow(k);
	
	int ph = getMapRow(ch, kl);
	int pl = getMapRow(cl, kh);
	
	int p = (ph << 4) + pl;

	return p;
}

int getMapRow(int cipher, int col){
	for(int i = 0; i < MAP_SIZE; i++){
		if (map[i][col] == cipher)
			return i;
	}
	
	return -1;
}

int getMapCol(int cipher, int row){
	for(int i = 0; i < MAP_SIZE; i++){
		if (map[row][i] == cipher)
			return i;
	}
	
	return -1;
}

//encode plaintext with the key using the map above
vector<int> encode(vector<int> plain, vector<int> key){

	vector<int> cipher;
	int keyLength = key.size();
	int keyPos = 0;
	
	for (int i = 0; i < plain.size(); i++){
		if (!(keyPos < keyLength)) keyPos = 0;
		int p = plain[i];
		int k = int(key[keyPos]);
				
		int ch = map[getHigh(p)][getLow(k)];
		int cl = map[getLow(p)][getHigh(k)];
	
		int c = (ch << 4) + cl;
		
		cipher.push_back(c);
		keyPos++;
	}
	
	return cipher;
}

//get the key length by offseting the ciphertext by incrementing amounts
//and counting how many characters match
int getKeyLength(vector<int> cipher){
	int MAX_LENGTH =35;
	int matches[MAX_LENGTH];
	for (int i=1; i < MAX_LENGTH + 1; i++){
		matches[i] = getOffsetMatch(cipher, i);
		printf("matches for %d: %d\n", i, matches[i]);
	}
	
	return max(matches,MAX_LENGTH);
}

//get number of chars that match with the offset
int getOffsetMatch(vector<int> cipher, int offset){
	int cipherLength = cipher.size();
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
