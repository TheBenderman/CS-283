#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>

// Count the number of occurrences of a string within another string
int countNumberOfOccurrences(char * fileString, char * searchString)
{
  int count = 0;
  const char * temp = fileString;
  while (temp = strstr(temp, searchString))
  {
    count ++;
    temp ++;
  }

  return count;
}

// If first string exists, replace all occurences with second
// If first string does not exist, add the first string in front of all occurences of third
// If both first and third strings don't exist, do nothing
char * replaceString(char * orig, char * first, char * second, char * third)
{
  char * fileString = orig;
  // If the first string is not found, try the method looking for the third string
  if (strstr(fileString, first) == NULL)
  {
    printf("%s is not found in file. Looking for %s.\n", first, third);
    // If the third string is not found either, then there is nothing to do
    if (strstr(fileString, third) == NULL)
    {
      printf("%s is not found either. Nothing to do.\n", third);
      return NULL;
    }
    else
    {
      printf("%s is found in file. Prepending %s to all occurences.\n", third, first);

      int lenOfFirstString = strlen(first);
      int lenOfThirdString = strlen(third);

      // Count the number of times that the third string appears in the file
      int numOccurrences = countNumberOfOccurrences(fileString, third);

      int lenOfFileString = strlen(fileString);

      // Create the length that the new file string should be
      int lenOfNewFileString = lenOfFileString + (numOccurrences * lenOfFirstString);

      // Allocate space for the new file string.
      char *newFileString, *temp2;
      newFileString = temp2 = (char *) malloc((lenOfNewFileString + 1) * sizeof(char));

      char *temp, *original;
      original = fileString;
      temp = original;

      char * start;

      int numBytes = 0;

      // While there are still occurrences of the first string in the temporary
      // file string, replace all occurrences with the second string.
      while(temp = strstr(temp, third))
      {
        // Figure out the number of bytes between the previous occurrence of the
        // first string and the next occurrence.
        numBytes = (int)(temp - original);

        // Copy the string from the last occurrence (or start of the string) to
        // the next occurrence (everything between the two occurrences).
        if (numBytes != 0)
          strncpy(temp2, original, numBytes);

        // Move the temporary pointer to the start of the occurrence
        temp2 += numBytes;

        // Copy the first and third string into the temporary file string
        strncpy(temp2, first, lenOfFirstString);
        temp2 += lenOfFirstString;

        strncpy(temp2, third, lenOfThirdString);
        temp2 += lenOfThirdString;

        // Move the file pointer to the end of the second string
        temp += lenOfThirdString;

        original += numBytes;
        original += lenOfThirdString;
      }

      strcpy(temp2, original); // Append the rest of the file that appears after last occurrence

      printf("Done replacing strings in file.\n");

      return newFileString;
    }
  }
  else
  {
    printf("%s is present in file. Replacing occurences with %s.\n", first, second);

    int lenOfFirstString = strlen(first);
    int lenOfSecondString = strlen(second);
    int numOccurrences = countNumberOfOccurrences(fileString, first);

    int lenOfFileString = strlen(fileString);

    // The length of the new file string is the length of the original file string
    // minus all occurrences of the first string plus all occurrences of the second
    // string.
    int lenOfNewFileString = lenOfFileString - (numOccurrences * lenOfFirstString) + (numOccurrences * lenOfSecondString);

    // Allocate space for the new file string.
    char *newFileString, *temp2;
    newFileString = temp2 = (char *) malloc((lenOfNewFileString + 1) * sizeof(char));

    char *temp, *original;
    original = fileString;
    temp = original;

    char * start;

    int numBytes = 0;

    // While there are still occurrences of the first string in the temporary
    // file string, replace all occurrences with the second string.
    while(temp = strstr(temp, first))
    {
      // Figure out the number of bytes between the previous occurrence of the
      // first string and the next occurrence.
      numBytes = (int)(temp - original);

      // Copy the string from the last occurrence (or start of the string) to
      // the next occurrence (everything between the two occurrences).
      if (numBytes != 0)
        strncpy(temp2, original, numBytes);

      // Move the temporary pointer to the start of the occurrence
      temp2 += numBytes;

      // Copy the second string into the temporary file string
      strncpy(temp2, second, lenOfSecondString);

      // Move the file pointer to the end of the second string
      temp += lenOfFirstString;
      temp2 += lenOfSecondString;

      original += numBytes;
      original += lenOfFirstString;
    }

    strcpy(temp2, original); // append the rest of the file that appears after the last occurrence of the string

    printf("Done replacing strings in file.\n");

    return newFileString;
  }
}

// This function finds all of the regular files in a given directory with the file extension .txt and
// opens them. The contents of the files are loaded into buffers, the string replacement occurs,
// and then the replacement file is written back into the file.
void traverseDirectoryAndReplace(char * directoryPath, char * first, char * second, char * third)
{
  printf("Traversing directory at path %s, looking for text files to replace.\n", directoryPath);

  DIR *dir;
  struct dirent *entry;
  char path[1024];

  if (!(dir = opendir(directoryPath)))
  {
    printf("Can't find directory with path %s.\n", directoryPath);
    return;
  }

  if (!(entry = readdir(dir)))
  {
    printf("No entries found in directory %s.\n", directoryPath);
    return;
  }

  // For each of the regular files in the directory, open the files, read the contents into a variable
  // replace the strings using the specified strings, and then write them back to the file.
  do {
    if (entry->d_type == DT_REG) {
      if (strstr(entry->d_name, ".txt") != NULL)
      {
        printf("Found a text file! Replacing strings in file: %s.\n", entry->d_name);

        // Build the path for the file.
        char path[1024];
        int len = snprintf(path, sizeof(path)-1, "%s/%s", directoryPath, entry->d_name);
        path[len] = 0;

        // Open the file and get the size
        FILE *f1 = fopen(path, "r");
        fseek(f1, 0, SEEK_END);
        long fsize = ftell(f1);
        fseek(f1, 0, SEEK_SET);

        // Initialize a buffer to store the entire file, then read the file into the buffer
        char *string = calloc(sizeof(char), fsize + 1);
        if (!string)
        {
          printf("Memory allocation error!\n");
          return;
        }

        printf("Reading the file %s into the buffer.\n", entry->d_name);
        fread(string, fsize, 1, f1);
        fclose(f1);

        string[fsize] = 0;

        // Replace the strings in the buffer
        printf("Replacing the strings in the file %s.\n", entry->d_name);
	      char * newString = replaceString(string, first, second, third);

        if (newString != NULL)
        {
          // Write the new file string back to the file
          printf("Putting the new file string back in file %s.\n", entry->d_name);
          
          printf("Writing to file at %s.\n\n", path);

          FILE *f2 = fopen(path, "w+");;
          fwrite(newString, strlen(newString) + 1, 1, f2);;
	        fclose(f2);

          free(newString);
        }

        free(string);
      }
    }
  } while (entry = readdir(dir));

  closedir(dir);
}

// Check if a string starts with another string
int startsWith(const char *a, const char *b)
{
   if(strncmp(a, b, strlen(b)) == 0)
	   return 1;
   return 0;
}

int main(int argc, char ** argv)
{
  if (argc < 5)
  {
    printf("Usage:\n");
    printf("./%s <directory> <first> <second> <third>", argv[0]);
    exit(0);
  }

  char *first, *second, *third;
  char *directory = argv[1];
  char *correctedDirectory = directory;
  first = argv[2];
  second = argv[3];
  third = argv[4];

  if (startsWith(directory, "/") != 1 && startsWith(directory,"./") != 1)
  {
    printf("Correcting directory path. \n");
    long unsigned int newFileSize = strlen(directory) + 3;
	  correctedDirectory = (char *) malloc(newFileSize);
	  snprintf(correctedDirectory, newFileSize, "./%s", directory);
  }

  traverseDirectoryAndReplace(correctedDirectory, first, second, third);

  free(correctedDirectory);

/*
  replaceString("", "Test", "Hello", "YO");
  replaceString("", "Test", "Hello", "YO");
  replaceString("", "yo", "hi", "Test");
  replaceString("", "yo", "hi", "Test"); */

  exit(0);
}
