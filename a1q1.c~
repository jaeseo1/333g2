/*
 * Note: any line with improper indentation is test line.
 *
 * Overview of the algorithm:
 * We read in a file, and find every possible n consecutive characters (n-chain)
 * Then we count the number of times each n-chain appears in the file.
 * We also have to deal with \n and buffer size because things get chopped off)
 * When we are done with counting, we pick a few chains that appear the most 
 * often in the input file, and then try to find the largest common factor.
 * if we are lucky, we will be able to find the key length.
 * from the key length, we perform frequency analysis or possibly other
 * techniques to find the plaintext/key
 *
 * comments on variables:
 * chainList = list of n-chains. each chain is a string of size n. (and for now
 * i've decided that n=3 because it's the most common choice.) this list will
 * dynamically grow in size, and we will also have another list to keep track
 * of the counts. chainList and chainCountList have same sizes and indexes.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
#include <stdbool.h>

#define BUFFER_SIZE 64
#define CHAIN_SIZE 3 // n-chain

static int map[16][16] = {
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

int isInList(char **chainList, int chainListIndex, char *chain) {
	// checks if the chain with the given str already exists in the list.
	// if it's in the list, return the index. otherwise -1;
	
	return -1;
}

int getHigh(int text) {
	// returns the high 4 bits of the given int
	int high;
	high = (text >> 4);
	return high;
}

int getLow(int text) {
	// returns the low 4 bits of the given int
	int low;
	low = (text & 15);
	return low;
}

int main(int argc, char* argv[]) {

printf("point");

	// error check on arguments
	if (argc != 2) {
		fprintf(stderr, "need a ciphertext file\n");
		return(-1);
	}

	// read input file.
	FILE* fp;
	fp = fopen(argv[1], "r");
	if (fp == NULL) {
		fprintf(stderr, "input file is invalid\n");
		return(-1);
	}
printf("point");

	// find n consecutive characters (n-chain) that appear multiple times in the cipher text.
	char **chainList;
	int chainListIndex = 0;
	int chainListSize = 1;
	chainList = (char **) malloc(chainListSize * CHAIN_SIZE * sizeof(char));
	int *chainCounList;
	chainCounList = (int *) malloc(chainListSize * sizeof(int));
	char buffer[BUFFER_SIZE];
	int bufferIndex, j;
	char chainStr[CHAIN_SIZE];
	
	while (fgets(buffer, BUFFER_SIZE, fp) != NULL) {
		// note: the code works only for the first buffer.. will have to deal with
		// cipher text longer than the buffer. (will discuss in class or lab)
		for (bufferIndex = 0; bufferIndex < BUFFER_SIZE; bufferIndex++) {
		
printf("index: %i", bufferIndex);
		
			if (strcmp(&buffer[bufferIndex+CHAIN_SIZE-1], "\0") == 0) {
				// do something if buffer is running out
				// maybe introduce temporary string to store the str?
fprintf(stderr, "first buffer ran out\n");
return(-1);
			}
			strncpy(chainStr, &buffer[bufferIndex], CHAIN_SIZE);
			j = isInList(chainList, chainListIndex, chainStr);
			if (j != -1) {
				// The chain alread exists in the list.
				// increase the count.
				chainCounList[j] += 1;
printf("%s exists, for the %ith time\n", chainStr, chainCounList[j]);
			}
			else {
				// The chain does not exist in the list.
				// add this chain to the list and set the count to 1.
				if (chainListIndex == chainListSize) {
					// dynamically grow the list
					chainListSize *= 2;
					chainList = (char **) realloc(chainList, chainListSize * CHAIN_SIZE * sizeof(char));
					chainCounList = (int *) realloc(chainCounList, chainListSize * sizeof(int));
				}
				strncpy(chainList[chainListIndex], chainStr, CHAIN_SIZE);
				chainCounList[chainListIndex++] = 1;
			}
		}
	}
	
	// analyze the pattern that they appear. (ex. appear every x bytes)
	
	// find the key length and then apply frequency analysis.

	return 0;
}
