#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define SIZE 128
#define MAXLINES 200 


// This function prints a file backwards recursively. Each iteration of the
// loop will move the file pointer to the next line. When there are no more
// lines in the file, the function will return. After the nested call
// returns, then the line is printed, effectively printing the file last
// line first, and first line last.
void printFileBackwardsRecursively(FILE *fp){
	char line[SIZE];

	// If there are no more lines, then return.
	if (fgets(line, SIZE, fp) == NULL){
		return;
	}	

	// Call the function recursively again, now that the file pointer is
	// pointing to the next line in the file.
	printFileBackwardsRecursively(fp);

	// Print the line of the file.
	printf("%s", line);	
}

// Print the file backwards using a 2D array to store the contents of the
// file. The function stores each of the lines iteratively in a 2D array,
// then loops over the array backwards and prints each line.
void printFileBackwards2DArray(FILE *fp){
	char line[SIZE];
	if (fp == NULL){
		return;
	}

	char lines[MAXLINES][SIZE]; 

	// While there are still lines left in the file, keep adding the line
	// into the 2D array. 
	int i = 0;
	while (fgets(line, SIZE, fp) != NULL){
		strcpy(lines[i], line);
		i = i + 1;	
	}

	// Starting from the last element of the array, print the lines. This
	// will print the lines in the opposite direction that they were read in
	// (backwards).
	for(int j=i-1; j >= 0; j--){
		printf("%s", lines[j]);
	}
}

// Print the file backwards using a pointer to pointer of characters. The
// function reads in the entire file and stores each line in the pointer to
// pointer of characters. Starting from the last line, print the file
// backwards.
void printFileBackwardsPointerToPointers(FILE *fp){
	char line[SIZE];
	if (fp == NULL){
		return;
	}
	
	// Initialize the pointer to pointer of characters. The maximum number of
	// lines is specified by the MAXLINES variable defined at the top of the
	// file.
	char ** lines = malloc(MAXLINES * sizeof(char *));

	if (lines == NULL){
		printf("Error! Unable to allocate memory!\n");
		return;
	}

	// While there are still lines left in the file, keep reading the lines
	// in and store them in the data structure.
	int i = 0;
	while (fgets(line, SIZE, fp) != NULL){
		// Allocate the right amount of memory for the current line.
		lines[i] = malloc(SIZE * sizeof(char));
		if (lines[i] == NULL){
			printf("Error! Unable to allocate memory!\n");
			return;
		}

		strcpy(lines[i], line);
		i = i + 1;
   }
	
	// Starting from the last element that was read in, print the lines. This
	// wll print the file backwards.
   for(int j=i-1; j >= 0; j--){
   	printf("%s", lines[j]);
	}

	// Free the memory that was allocated to store the lines.
	free(lines);
}

int main(int argc, char *argv[]) {
	// Make sure that the file name is provided in the command line.
	if (argc == 1){
		printf("You must input the file name as a command line argument.\n");
		return 0;
	}

	// Open the file that was specified in the first command line argument.
	// If the file cannot be opened, then it can't be found. Exit the
	// program.	
	FILE *fp = fopen(argv[1], "r");
	if (fp == NULL){
		printf("Unable to find file %s\n", argv[1]);
		return 1;
	}

	printf("Printing file named %s backwards. \n", argv[1]);

	printf("Printing the file backwards recursively:\n");
	printFileBackwardsRecursively(fp);

	printf("\n\nPrinting the file backwards using a 2D Array of Characters:\n");
	rewind(fp);
	printFileBackwards2DArray(fp);

	printf("\n\nPrinting the file backwards using a pointer to pointers:\n");

	rewind(fp);
	printFileBackwardsPointerToPointers(fp);

	fclose(fp);

	return 0;
}
