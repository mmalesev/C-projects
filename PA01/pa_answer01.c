#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#include <limits.h>
#include "pa_answer01.h"

#define INV_SYMBOL 36


// you should try to re-use a majority of the algorithms in PE02 and PE03
// for this programming assignment.
// However, you have to take care of endptr, base 0, and base 16
// these are the three additional things you have to deal with in
// this assignment, as compared to the str_to_long_int function
// that you wrote for PE02 and PE03.
//
// it is really IMPORTANT that you extensively try out the function
// strtol on your own.
//

int char_to_int(char);

//character to integer conversion
int char_to_int(char c)
{
   int ret_value;

   if (c >= 48 && c <= 57){
       ret_value = c % 48;
   }
   else if (c >= 97 && c <= 122){
       ret_value = c % 97 + 10;
   }
   else if (c >= 65 && c <= 90){
       ret_value = c % 65 + 10;
   }
   else {
       return INV_SYMBOL;
   }

   return ret_value;
}

long int my_strtol(const char *nptr, char **endptr, int base)
{
   //local variables
   long int ret_value = 0;  // return value
   int ct = 0;
   int sign = 1;
   int conv_num;
   int valid = 1;
   
   // if invalid base, set errno and return immediately
   if ((base < 2 && base != 0) || base > 36){
       errno = EINVAL;
       return 0;
   }

   // valid base, skip over white space
   do{
       if(isspace(nptr[ct])){
           ct++;
       }
   } while(isspace(nptr[ct]));

   // if encounter a sign, expect a number after that 
   // if a negative sign is encountered, change the sign
   if(nptr[ct] == '-'){
       sign = -1;
       ct++;
   }
   else if(nptr[ct] == '+'){
       ct++;
   }
   
   if(base == 0){ //solving for the specific cases of base 0
       if(nptr[ct] == '0'){
           ct++;

           if((nptr[ct] == 'x' || nptr[ct] == 'X') && isxdigit(nptr[ct + 1])){
               base = 16;
               ct--;
           }
           else
           {
               base = 8;
           }
       }
       else{
           base = 10;
       }
   }

   //special case for base 16
   if(base == 16){
       if(nptr[ct] == '0' && (nptr[ct+1] == 'x' || nptr[ct+1] == 'X')){
           ct = ct + 2;
       }
   }

   //conversion
   do{
       conv_num = char_to_int(nptr[ct]); 
       if (nptr[ct] == 0 || conv_num >= base){
           valid = 0;
       }
       else{
           if (sign == 1){
               if (ret_value > LONG_MAX / base || conv_num > LONG_MAX - ret_value * base){
                   errno = ERANGE;
                   while(conv_num < base){
                        conv_num = char_to_int(nptr[ct]);
                        ct++;
                   }
                   *endptr = (char *) &nptr[ct - 1];
                   return LONG_MAX;
               }
           }
           else{
               if (ret_value < LONG_MIN / base || sign * conv_num < LONG_MIN + ret_value * base){
                   errno = ERANGE;
                   while(conv_num < base){
                        conv_num = char_to_int(nptr[ct]);
                        ct++;
                   }
                   *endptr = (char *) &nptr[ct - 1];
                   return LONG_MIN;
               }
           }
           ret_value = ret_value * base + conv_num;
       }
       ct++;
   } while (valid == 1);
   
   if(endptr != NULL){
       *endptr = (char *) &nptr[ct - 1];
   }

   ret_value = ret_value * sign;

   return ret_value;
}
