#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <time.h>
#include <sys/stat.h>
#include "csapp.h"

void syncDirectoryAWithB(char * directoryA, char * directoryB);
void syncDirectoryBWithA(char * directoryA, char * directoryB);
void copyFile(char * from, char * to);
int findFile(char * directoryPath, char * fileName);
int findDirectory(char * directoryPath, char * directoryName);
void removeDirectory(char * dir);
int startsWith(const char *pre, const char *str);


// This function calls the two functions that sync two directories. It also explains the usage of the program.
void syncTwoDirectories(char * dirA, char * dirB)
{
  printf("Syncing directory %s with directory %s.\n", dirA, dirB);
  printf("If file exists in %s but not %s, add file to %s.\n", dirA, dirB, dirB);
  printf("If file exists in both, replace with the last modified version.\n");
  printf("If directory exists in %s but not %s, make directory in %s.\n", dirA, dirB, dirB);
  printf("Iterate into each directory found in %s.\n\n", dirA);
  syncDirectoryAWithB(dirA, dirB);
  
  printf("\nSyncing directory %s with directory %s.\n", dirB, dirA);
  printf("If file exists in %s but not %s, remove file from %s.\n", dirB, dirA, dirB);
  printf("If directory exists in %s but not %s, remove directory from %s.\n", dirB, dirA, dirB);
  printf("Iterate into each directory found in %s.\n\n", dirB);
  syncDirectoryBWithA(dirA, dirB);
}

// This function syncs the directory A with B. Loop over the directory entries in A.
// If the file is found in A, but not B, then copy the file from directory A to B. If
// the file is found in both A and B, then find which file was modified last, and copy
// to the other directory. If the directory entry is a directory, then call the function
// recursively. If a directory exists in A, but not B, then create a directory in B.
void syncDirectoryAWithB(char * dirA, char * dirB)
{
  DIR *directoryA, *directoryB;
  struct dirent *entryA, *entryB;

  // Open the directory for A
  if(!(directoryA = opendir(dirA)))
  {
    printf("Error! Can't open directory with path: %s\n", dirA);
    return;
  }

  // Open the directory for B
  if (!(directoryB = opendir(dirB)))
  {
    printf("Error! Can't open directory with path: %s\n", dirB);
    return;
  }

  // Read the first entry of directory A. If we can't read it, then close the directories
  if (!(entryA = readdir(directoryA)))
  {
	printf("Can't read directory %s\n", dirA);
	closedir(directoryA);
	closedir(directoryB);
	return;
  }
    

  do {
	printf("Reading directory entry %s\n", entryA->d_name);  
	  
	// If the entry is a directory
    if (entryA->d_type == DT_DIR)
    {
      // Build the complete file path for the entry in directory A
      char pathA[1024];
      int len = snprintf(pathA, sizeof(pathA)-1, "%s/%s", dirA, entryA->d_name);
      pathA[len] = 0;

	  // Make sure that the file path does not contain . (current directory) or .. (parent directory)
      if (strcmp(entryA->d_name, ".") == 0 || strcmp(entryA->d_name, "..") == 0)
	  {
		printf("Entry is . or .., iterating to next directory entry.\n");  
        continue;
	  }

	  // Build the complete file path for the entry in directory B
      char pathB[1024];
      snprintf(pathB, sizeof(pathB)-1, "%s/%s", dirB, entryA->d_name);

	  // Find the entry directory in B. If it is not found, createthe directory.
      int directoryBPath = findDirectory(dirB, entryA->d_name);
      if (directoryBPath == 0)
      {
		printf("Directory not found in %s. Making directory.\n", dirB);
        mkdir(pathB, 0700); // make directory
      }

	  // Go into the directory
	  printf("Going into directory %s.\n", pathA);
      syncDirectoryAWithB(pathA, pathB);
    }
    else
    {
	  // Build the file path for directory A
      char pathA[1024];
      int lenA = snprintf(pathA, sizeof(pathA)-1, "%s/%s", dirA, entryA->d_name);
      pathA[lenA] = 0;

	  // Build the file path for directory B
      char pathB[1024];
      int lenB = snprintf(pathB, sizeof(pathB)-1, "%s/%s", dirB, entryA->d_name);
      pathB[lenB] = 0;

	  // Find the file that was found in directory A in directory B
      if (findFile(dirB, entryA->d_name) == 1)
      {
		// File was found in both directories
		printf("Found file %s in %s.\n", entryA->d_name, dirB);
        struct stat statA, statB;

		// Get the stats of the two files
        int resultA = stat(pathA, &statA);
        int resultB = stat(pathB, &statB);

		// Compare the modification time of the two files
        double seconds = difftime(statA.st_mtime, statB.st_mtime);
        if (seconds > 0) // The file is newer in A
        {
		  printf("File is newer in %s, so copying to %s.\n", pathA, pathB);
          copyFile(pathA, pathB); // Copy file from A to B
        }
        else // The file is newer in B
        {
		  printf("File is newer in %s, so copying to %s.\n", pathB, pathA);
          copyFile(pathB, pathA); // Copy file from B to A
        }
      }
      else // File is not found in B
      {
		printf("File not found in %s, so copying there.\n", pathB);
        copyFile(pathA, pathB); // Copy file from A to B
      }
    }
  } while ( entryA = readdir(directoryA)); // Read next entry from directory A

  // Close the directories
  closedir(directoryA);
  closedir(directoryB);
}

// Sync directory B with A. If the file exists in B but not A, then remove the file 
// from B. If the directory exists in B but not A, then we need to recursively 
// delete the directory from B.
void syncDirectoryBWithA(char * dirA, char * dirB)
{
  DIR *directoryA, *directoryB;
  struct dirent *entryA, *entryB;

  // Open directory A
  if(!(directoryA = opendir(dirA)))
  {
    printf("Error! Can't open directory with path: %s\n", dirA);
    return;
  }

  // Open directory B
  if (!(directoryB = opendir(dirB)))
  {
    printf("Error! Can't open directory with path: %s\n", dirB);
    return;
  }
  
  // read the first entry of directory B, close if can't open
  if (!(entryB = readdir(directoryB)))
  {
    printf("Can't read directory %s\n", dirB);
	closedir(directoryA);
	closedir(directoryB);
	return;
  }

  do {
	printf("Reading directory entry %s\n", entryB->d_name);
	
	// If the directory entry is a directory
    if (entryB->d_type == DT_DIR)
    {
	  // Build the directory path for the file in directory B
      char pathB[1024];
      int len = snprintf(pathB, sizeof(pathB)-1, "%s/%s", dirB, entryB->d_name);
      pathB[len] = 0;

	  // Continue if it is the current or parent directory symbols
      if (strcmp(entryB->d_name, ".") == 0 || strcmp(entryB->d_name, "..") == 0)
        continue;

	  // Build the directory path for the file in directory A
      char pathA[1024];
      snprintf(pathA, sizeof(pathB)-1, "%s/%s", dirA, entryB->d_name);

	  // Find the directory found in B in the directory A
      int directoryAPath = findDirectory(dirA, entryB->d_name);
      if (directoryAPath == 0) // Directory is not found in A, so remove it
      {
		printf("File %s not found in %s, so removing the directory.\n", entryB->d_name, dirA);
        removeDirectory(pathB);
      }
	  else // Directory was found in A, so we need to go into that directory and check those files
	  {
		printf("Going into directory %s.\n", pathB);
		syncDirectoryBWithA(pathA, pathB);
	  }
    }
    else // Entry is a file
    {
	  // Build the file path for directory A
      char pathA[1024];
      int lenA = snprintf(pathA, sizeof(pathA)-1, "%s/%s", dirA, entryB->d_name);
      pathA[lenA] = 0;

	  // Build the file path for directory B
      char pathB[1024];
      int lenB = snprintf(pathB, sizeof(pathB)-1, "%s/%s", dirB, entryB->d_name);
      pathB[lenB] = 0;

	  // Find the file in directory A. If it is not found, then remove it.
      if (findFile(dirA, entryB->d_name) != 1)
      {
		  printf("File %s not found in %s, so deleting the file.\n", entryB->d_name, dirA);
        unlink(pathB);
      }
		else
		{
			printf("File %s found, so nothing to do.\n", pathB);
		}
    }
  } while ( entryB = readdir(directoryB)); // Read the next entry from directory B

  // Close the directories
  closedir(directoryA);
  closedir(directoryB);
}

// Copy a file from one directory to another
void copyFile(char * from, char * to)
{
  char buf[1024];
  int fdA, fdB, n;
  rio_t rio;

  // Open the file that is being copied for reading
  if ((fdA = open(from, O_RDONLY, 0)) == -1)
  {
    printf("Unable to open file: %s\n", from);
    return;
  }

  // Open the file that is being copied to for writing. Create and destroy the file.
  if ((fdB = open(to, O_CREAT | O_TRUNC | O_WRONLY, 0644)) == -1)
  {
    printf("Unable to open file: %s\n", to);
    return;
  }

  // Initialize the file for reading. While there are lines to read, write them to the
  // new file.
  rio_readinitb(&rio, fdA);
  while((n = rio_readnb(&rio, buf, 1024)) > 0)
  {
    rio_writen(fdB, buf, n);
  }

  // Close the files
  close(fdA);
  close(fdB);
}

// Find a file in a given directory path
int findFile(char * directoryPath, char * fileName)
{
  struct stat stats;
  char path[1024];

  // Build the path for the file
  snprintf(path, sizeof(path)-1, "%s/%s", directoryPath, fileName);

  // Get the stats of the file
  int result = stat(path, &stats);

  // If the result is 0, then the stats were successfully gotten
  if (result == 0)
  {
	// Make sure that the file is a regular file. If it is, return true.
    if(S_ISREG(stats.st_mode))
	  return 1;
  }

  return 0; // Return that the file was not found
}

// Find a directory in a given directory
int findDirectory(char * directoryPath, char * directoryName)
{
  struct stat stats;
  char path[1024];

  // Build the file path for the directory that is being searched for
  snprintf(path, sizeof(path)-1, "%s/%s", directoryPath, directoryName);

  // Get the stats of the directory
  int result = stat(path, &stats);

  // If the result is 0, then the directory exists
  if (result == 0)
  {
	// If file found is a directory, then return true
    if(S_ISDIR(stats.st_mode))
      return 1;
  }

  return 0; // return that the file was not found
}

// Remove a directory recursively
void removeDirectory(char * dir)
{
  printf("Removing directory located at %s.\n", dir);		

  DIR *directory;
  struct dirent *entry;

  // Open the directory
  if(!(directory = opendir(dir)))
  {
    printf("Error! Can't open directory with path: %s\n", dir);
    return;
  }

  // Read the first entry of the directory. If it can't be read, close and return.
  if (!(entry = readdir(directory)))
  {
    printf("Can't read directory %s\n", dir);
	closedir(directory);
	return;
  }

  // Loop over directory entries.
  do {
	printf("Reading directory entry %s.\n", entry->d_name);  
	  
	// If the entry is a directory
    if (entry->d_type == DT_DIR)
    {
	  // Build the path for the found directory
      char path[1024];
      int len = snprintf(path, sizeof(path)-1, "%s/%s", dir, entry->d_name);
      path[len] = 0;

	  // Make sure that the entry is not the descriptor for current or parent directory
      if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
	  {
		printf("Entry is . or .., iterating to next directory entry.\n");  
        continue;
	  }

	  // Remove the directory recursively.
	  printf("Going into directory %s.\n", path);
	  removeDirectory(path); 
    }
    else // Otherwise it is a file
    {
	  // Build the path for the file
      char path[1024];
      int lenA = snprintf(path, sizeof(path)-1, "%s/%s", dir, entry->d_name);
      path[lenA] = 0;

	  // Remove the file
	  printf("Removing file located at %s.\n", path);
      unlink(path);
    }
  } while ( entry = readdir(directory));

  closedir(directory);
  
  printf("Removing empty directory at %s.\n", dir);
  rmdir(dir);
}

// Check if a string starts with another string
int startsWith(const char *a, const char *b)
{
   if(strncmp(a, b, strlen(b)) == 0) 
	   return 1;
   return 0;
}

int main(int argc, char * argv[])
{
  if (argc < 3){
    printf("You must add the two directories as command line args!\n");
    return 1;
  }
  
  char *a = argv[1];
  char *b = argv[2];
  
  char *correctedA = a;
  char *correctedB = b;
  
  if (startsWith(a, "/") != 1 && startsWith(a,"./") != 1)
  {
	  correctedA = malloc(sizeof(a) + 2);
	  snprintf(correctedA, sizeof(correctedA), "./%s", a);
  }
  
  if (startsWith(b, "/") != 1 && startsWith(b,"./") != 1)
  {
	  correctedB = malloc(sizeof(b) + 2);
	  snprintf(correctedB, sizeof(correctedB), "./%s", b);
  }
  
  syncTwoDirectories(correctedA, correctedB);

  return 0;
}
