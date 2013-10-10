Part1:

Build the executable by running "make a1q1" in the command line.
The source code is C++. Run the program with the command "./a1q1 ciphertext".

The part that determines the key length is lines 83-85, commented out. The key itself
is not actually obtained and saved in the variable since we ended up with multiple possible keys.
It is then hardcoded into the next part, ines 87-89, which uses the hardcoded key to decrypt the 
ciphertext.
