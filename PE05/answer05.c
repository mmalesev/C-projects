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





  // recursion
  // you have to halve n, 
  // depending on whether n/2 is even or odd, recursively
  // call logFibonacci with appropriate parameters













   // decrement counter after all recursive calls
   // and before returning from the function.

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

   unsigned long fn1 = 0; 
   unsigned long fn2 = 0;

   /* simple cases */



   /* if even n, call logFibonacci with n and return fn1 */
   /* if odd n, call with n-1 or n+1, combine fn1 and fn2 as F(n) */





   return fn1 + fn2;  // should change this, doing this so that compiler 
                      // would not complain.

}

#endif
