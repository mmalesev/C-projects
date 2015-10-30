#include "pa_answer03.h"

// Do not change this function
// otherwise, your matrices may not match what we are expecting

int Is_zero(double value)
{
   if (value < 0) {
      value = -value;
   }
   if (value < TOL) {
      return 1;
   } else {
      return 0;
   }
}

// Write your functions below this line
