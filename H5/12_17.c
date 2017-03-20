/*

The problem in this program is that the program executes the statement "exit(0)"
which causes the program to exit before the call to printf even occurs within
the thread. The exit(0) function cancels all execution of all threads.

*/

/* WARNING: This code is buggy! */
#include "csapp.h"
void *thread(void *vargp);
int main()
{
   pthread_t tid;

   Pthread_create(&tid, NULL, thread, NULL);
   Pthread_join(tid, NULL); /* First option to fix the bug */

   pthread_exit(NULL); /* Second option to fix the bug */
   //exit(0);
}

/* Thread routine */
void *thread(void *vargp)
{
   Sleep(1);
   printf("Hello, world!\n");
   return NULL;
}
