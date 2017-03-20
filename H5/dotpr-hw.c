#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#define NUMTHRDS 4
#define VECLEN 100000

pthread_t callThd[NUMTHRDS];

double *array_a;
double *array_b;
double big_sum;
int veclen;

pthread_mutex_t mutex; // Mutex for accessing the sum variable
pthread_attr_t attr;

void *dotprod(void *arg)
{
   int i, start, end;
   double *x, *y;
   double mysum;
   long currThread = (long) arg;

   // Calculate the starting and ending points for this thread

   // Starting point is the current thread number times how many elements
   // each thread should do - held in global variable veclen.
   start = currThread * (veclen);

   // End is just the starting index plus the number of elements
   end = start + (veclen);
   x = array_a;
   y = array_b;

   mysum = 0;
   for (i=start; i<end ; i++)
   {
      mysum += (x[i] * y[i]);
   }

   // Lock the big_sum variable so that 2 threads don't try to access it at
   // the same time.
   pthread_mutex_lock(&mutex); // lock the variable, wait if locked
   big_sum += mysum;
   pthread_mutex_unlock(&mutex); // unlock the variable, allow next thread

   return NULL;
}

int main (int argc, char *argv[])
{
   long i;

   double *a, *b;

   void *status;
   a = (double*) malloc (NUMTHRDS*VECLEN*sizeof(double));
   b = (double*) malloc (NUMTHRDS*VECLEN*sizeof(double));

   for (i=0; i<VECLEN*NUMTHRDS; i++)
   {
      a[i] = 1;
      b[i] = a[i];
   }

   veclen = VECLEN;
   array_a = a;
   array_b = b;
   big_sum = 0;

   pthread_mutex_init(&mutex, NULL); // initialize the mutex
   pthread_attr_init(&attr);
   pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

   /* create threads */
   for(i=0; i<NUMTHRDS; i++)
   {
      /* Each thread works on a different set of data.
      The offset is specified by 'i'. The size of
      the data for each thread is indicated by VECLEN.
      */

      // Create each thread
      pthread_create(&callThd[i], &attr, dotprod, (void *)i);
   }

   /* Wait on the other threads */
   for (i=0; i < NUMTHRDS; i++)
   {
      pthread_join(callThd[i], NULL); // wait for each thread to be done
   }

   printf ("Sum = %f \n", big_sum);
   free (a);
   free (b);

   pthread_attr_destroy(&attr); // destroy the attribute
   pthread_mutex_destroy(&mutex); // destroy the mutex
   pthread_exit(NULL);
}
