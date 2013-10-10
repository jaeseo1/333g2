#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
#include <stdbool.h>
#include <iostream>

#include <string>
#include <vector>

#define BUFFER_SIZE 64
#define CHAIN_SIZE 3 // n-chain

using namespace std;

int main(int argc, char* argv[]) {
	FILE *fp;
    int c;
    vector<int> ciphertext;

	long asskeys[256];
	
	for(int i=0; i<256; i++){
    	asskeys[i] = 0;
    }

	//ciphertext arg
    if (argc != 2) {
        fprintf(stderr, "specify ciphertext\n", argv[0]);
        exit(1);
    }
    if (!(fp = fopen(argv[1], "rt"))) {
        perror(argv[1]);
        exit(1);
    }
        
        
    int notPadding = 0;
    int padding = 0;    
        
    while ((c = fgetc(fp)) != EOF) {
		//ciphertext.push_back(c);
		
		//printf("%d ", c);
		/*if (c == 0)
			padding++;
		else 
			notPadding++;*/
			
		asskeys[c]++;
    }
    
    for(int i=0; i<256; i++){
    	printf("asskey[%d] = %lu\n", i, asskeys[i]);
    }
    
    fclose(fp);
        	
	return 0;
}
