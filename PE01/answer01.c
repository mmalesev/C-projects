#include <stdlib.h>
#include "answer01.h"

int largest_partial_sum(int * array, int len)
{
   int max_sum; 
   int sum;
   int i;

   //if array has 0 elements max_sum is 0, otherwise we calculate it
   if (len == 0){
       max_sum = 0;
   }
   else{
       sum = array[0];
       max_sum = array[0];
       for (i = 1; i < len; ++i){
           sum += array[i];
           if (sum > max_sum){
               max_sum = sum;
           }
       }
   }

   return max_sum;
}

int largest_difference(int * array, int len)
{
   int max_diff;
   int max;
   int min;
   int i;

   if (len <= 1){
       max_diff = 0;
   }
   else{
       //finding max and min element of the array
       max = array[0];
       min = array[0];
       for (i = 1; i < len; ++i){
           if (array[i] > max){
               max = array[i];
           }
           else if (array[i] < min){
               min = array[i];
           }
       }
       max_diff = max - min;
    }

   return max_diff;
}
