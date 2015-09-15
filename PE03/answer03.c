#include <errno.h>
#include <ctype.h>
#include <limits.h>

#include "answer03.h"

#define INV_SYMBOL 36

// you are not allowed to change the contents at the location whose address in 
// stored in nptr (the way to interpret the type of nptr is that it is an address
// pointing to a constant char.)
//
// you have to check for whether the base in the valid range 2 through 36
// if not, set errno to EINVAL
// 
// This is a suggestion on how you should process the string
// (1)  skip over all white space
// (2)  if a +/- sign is encountered, set the polarity of the number accordingly
//      By default, the number is assumed to be positive if no sign is given.
// (3)  Convert the string until you encounter an invalid symbol for that 
//      base or when you reach the end of the string.
//
// examples (not exhaustive, of course):
//    "10a:" in base 10 is 10, in base 16 is 266, in base 36 is 1306
//    "   -10a:" in base 10 is -10, in base 16 is -266, and in base 36 is -1306 
//    " +10a:" in base 10 is 10, in base 16 is 266, in base 36 is 1306
//    "922337203685477580710304" in base 10 should return LONG_MAX, and errno 
//    should be set to ERANGE.
//    "-922337203685477580710304" in base 10 should return LONG_MIN, and errno 
//    should be set to ERANGE.
//    "garbage" should be 0 in base 10, 0 in base 16, and 35479055534 in base 36
//
int char_to_int(char);

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

long int str_to_long_int(const char *nptr, int base)
{
   long int ret_value = 0;  // return value
                            // if no conversion happens, return 0
   int ct = 0;
   int sign = 1;
   int conv_num;
   int valid = 1;

   // if invalid base, set errno and return immediately
   if (base < 2 || base > 36){
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
   // if a negative sign is encountered, the conversion must
   // produce a negative number
   if(nptr[ct] == '-'){
       sign = -1;
       ct++;
   }
   else if(nptr[ct] == '+'){
       ct++;
   }

   // now, convert the string to number
   // the conversion has to account for the +/- sign
   // need to check whether you have out-of-range ret_value 
   do{
       conv_num = char_to_int(nptr[ct]); 
       if (nptr[ct] == 0 || conv_num >= base){
           valid = 0;
       }
       else{
           ret_value = ret_value * base + conv_num;
           if (sign == 1){
               if (LONG_MAX - ret_value < nptr[ct + 1] && nptr[ct + 1] != 0){
                   errno = ERANGE;
                   return LONG_MAX;
               }
           }
           else{
               if (LONG_MIN + ret_value > (-1) * nptr[ct + 1] && nptr[ct + 1] != 0){
                   errno = ERANGE;
                   return LONG_MIN;
               }
           }
       }
       ct++;
   } while (valid == 1);

   // clean up before returning
   // need to set errno and ret_value correctly if the converted number 
   // is out of range
   ret_value = ret_value * sign;

   return ret_value;
}
