#include "csapp.h"
#include <stdio.h>
void *thread(void *vargp);

int main(int argc, char ** argv)
{
   if (argc != 2) {
      printf("Usage: ./%s <# of threads>\n", argv[0]);
      exit(0);
   }

   int n = atoi(argv[1]);
   pthread_t tids[n];

   for (int i = 0; i < n; i++)
   {
      Pthread_create(&tids[i], NULL, thread, NULL);
   }

   for (int i = 0; i < n; i++)
   {
      Pthread_join(tids[i], NULL);
   }

   exit(0);
}

void *thread(void *vargp)
{
   printf("Hello World!\n");
   return NULL;
}
