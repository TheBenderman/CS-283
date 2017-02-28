// This function counts the number of bits in a long integer.
void countNumberOfBits(long int number){
	//get the memory address of the current number
	char * currentNumberAddr = (char*)(&number);

	//get the memory address of the next memory location
	char * nextAddr = (char*)(&number + 1);

	// By subtracting the number's memory location from the next memory
	// location, we know how many bytes were allocated for the variable.
	long int sizeOf = nextAddr - currentNumberAddr;
	
	// Each block is 4 bits, so we need to multiply the value returned by 4. 
	long int sizeInBits = 4 * sizeOf;

	printf("The number of bits in %ld is %ld.\n", number, sizeInBits);
}

int main(int argc, char *argv[]){
	if (argc == 1){
		printf("Please enter the long integer as a command line arg.\n"); 
		return 0;
	}

	printf("Counting the number of bits in long int: %s\n", argv[1]);

	countNumberOfBits(atol(argv[1]));
}
