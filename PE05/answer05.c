#include "utility.h"
#include "answer05.h"

// do not remove #ifndef and #endif in this file, otherwise the evaluation
// would fail and you would definitely get 0

#ifndef NTEST_LOG

/* n is even */
/* *fn1 and *fn2 should store F(n) and F(n-1) */
void logFibonacci(int n, unsigned long *fn1, unsigned long *fn2)
{
  // increment the recursion counter 

  Increment_counter(&n);
   
  // terminating condition
  if (n == 2) {
      *fn1 = 1;
      *fn2 = 1;
  }
  else{
      long x;
      long y;
      long temp;
      if ((n / 2) % 2 == 0){
          logFibonacci(n / 2, fn1, fn2);
      }
      else{
          logFibonacci(n / 2 + 1, fn1, fn2);
          temp = *fn1;
          *fn1 = *fn2;
          *fn2 = temp - *fn2;
      }
      x = *fn1;
      y = *fn2;
      *fn1 = 2 * y * x + x * x;
      *fn2 = y * y + x * x;
  }

   Decrement_counter();
}

#endif

#ifndef NTEST_FIB

/* n is can be odd or even */
/* n is assumed to be non-negative */
/* don't have to check for that */
unsigned long Fibonacci(int n)
{
   /* for storing the results when calling logFibonacci */

   unsigned long fn1 = 0; //F(n) 
   unsigned long fn2 = 0; //F(n-1)

   /* simple cases */
   if (n == 0) {return 0;}
   if (n == 1) {return 1;}

   /* if even n, call logFibonacci with n and return fn1 */
   /* if odd n, call with n-1, and then combine fn1 and fn2 as F(n) */
   if (n % 2 == 0){
       logFibonacci(n, &fn1, &fn2);
       return fn1;
   }
   else{
       logFibonacci(n - 1, &fn1, &fn2);
       return fn1 + fn2;
   }
}

#endif
