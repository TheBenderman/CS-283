#include<stdio.h>
#include<stdlib.h>

#define BINARYSIZE 32

// This function converts a base 19 decimal number to it's binary form. This
// form is then outputted to the screen.
void convertDecimalToBinary(int decimal){
	
	// The number needs to be 1 larger than the binary to include the
	// null terminating character
	char binaryNumber[BINARYSIZE+1];

	// Start at the last most bit in the string
	int currentBit = BINARYSIZE - 1;
	int bit = 0;
	int quotient = 0;

	// Keep generating the binary number until you get all 32 bits
	while (currentBit >= 0){

		bit = decimal % 2; // This will tell whether or not the bit is set
		quotient = decimal / 2; // This will tell the remainder
		
		// If the bit is set, then put a 1 in the current place in the binary
		// number. Otherwise, place a 0.
		if (bit) {
			binaryNumber[currentBit] = '1';
		} else {
			binaryNumber[currentBit] = '0';
		}
		
		decimal = quotient; // Set the current number to the remainder

		currentBit = currentBit - 1; // Go to the previous bit.
	}

	binaryNumber[BINARYSIZE] = '\0'; // Signify the end of the binary number

	printf("The binary value is : %s\n", binaryNumber);
}

int main(int argc, char *argv[]){
	if (argc == 1){
		printf("Please input the decimal number to convert as a command line"
				" argument.\n");
		return 0;
	}
	
	printf("Converting %s from integer value to binary value.\n", argv[1]);

	convertDecimalToBinary(atoi(argv[1]));
	return 0;
}
