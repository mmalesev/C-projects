#include <errno.h>
#include <ctype.h>
#include <limits.h>

#include "answer02.h"

// Given a char c, return a numeric value
// '0'..'9' --> 0..9
// 'a'..'z' --> 10..35
// 'A'..'Z' --> 10..35
int char_to_int(char c)
{
   int ret_value;
   if (c >= 48 && c <= 57){ //numbers
       ret_value = c % 48;
   }
   else if (c >= 97 && c <= 122){ //upper case
       ret_value = c % 97 + 10;
   }
   else if (c >= 65 && c <= 90){ //lower case
       ret_value = c % 65 + 10;
   }
   else { //invalid symbol
       return INV_SYMBOL;
   }

   return ret_value;
}

long int str_to_long_int(const char *nptr, int base)
{
   long int ret_value = 0;  // return value
                            // if no conversion happens, return 0
   int ct = 0; //counter at which position is the string check
   int sign = 1; //positive = 1, negative = -1
   int conv_num; //integer received from char_to_int function
   int valid = 1; //1 = valid character, 0 = invalid character
   
   // if invalid base, set errno and return immediately
   if (base < 2 || base > 36){
       errno = EINVAL;
       return 0;
   }

   // valid base, skip over white space, white space includes tabs,
   // carriage return, etc (see function isspace).
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
   do{
       conv_num = char_to_int(nptr[ct++]); 
       if (nptr[ct - 1] == 0 || conv_num >= base){
           valid = 0;
       }
       else{
           ret_value = ret_value * base + conv_num;
       }
   } while (valid == 1);

   //taking account for the sign
   ret_value = ret_value * sign;

   return ret_value;
}
