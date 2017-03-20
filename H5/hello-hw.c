#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define NUM_THREADS 8
char *messages[NUM_THREADS];

// Struct to hold the 3 arguments that we need for the function
struct hellohw
{
   int sum;
   char * message;
   int taskid;
};

void *PrintHello(void *threadarg)
{
  int taskid, sum;
  char *hello_msg;

  struct hellohw * hello = (struct hellohw *) threadarg;
  sum = hello->sum;
  hello_msg = hello->message;
  taskid = hello->taskid;

  printf("Thread %d %s Sum=%d\n", taskid, hello_msg, sum);
  pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
  pthread_t threads[NUM_THREADS];
  int rc, t, sum;
  sum=0;
  messages[0] = "Hello-0";
  messages[1] = "Hello-1";
  messages[2] = "Hello-2";
  messages[3] = "Hello-3";
  messages[4] = "Hello-4";
  messages[5] = "Hello-5";
  messages[6] = "Hello-6";
  messages[7] = "Hello-7";

  for(t = 0; t < NUM_THREADS; t++)
  {
      sum = sum + t;

      // Initialize structure to send to the thread function
      struct hellohw hello;
      hello.sum = sum;
      hello.message = malloc(strlen(messages[t]) * sizeof(char));
      strcpy(hello.message, messages[t]);
      hello.taskid = t;

      printf("Creating thread %d\n", t);

      // Create the thread. Then wait for the thread to be done.
      pthread_create(&threads[t], NULL, PrintHello, &hello);
      pthread_join(threads[t], NULL); // Might not be needed
   }

   pthread_exit(NULL);
}
