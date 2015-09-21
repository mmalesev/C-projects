#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#include <limits.h>
#include "pa_answer01.h"

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
long int my_strtol(const char *nptr, char **endptr, int base)
{
   long int ret_value = 0;  // return value




   // if invalid base, set errno and return immediately





   // valid base, skip over white space




   // if encounter a sign, expect a number after that 
   // if a negative sign is encountered, change the sign





   
   // the exact algorithm is not given, but these are 
   // the tasks the function has to perform:  
   //
   // determine the actual base if the base is 0
   // if it starts with '0', it is octal
   // if '0' is followed by 'x' or 'X' it is hexadecimal or base 16
   // otherwise, it is decimal
   // therefore you have to determine the actual base and the starting
   // location to perform conversion
   //
   // for bases 2 to 15, perform conversion as in PE02
   //
   // for base 16, you have to decide when the conversion should
   // start: after the optional "0x" or "0X" or immediately
   // question: if you have "0xG", what should be converted?
   //
   // for bases 17 to 36, perform conversion as in PE02
   // 
   // after you have determine the real base, and where to start,
   // perform the conversion






















   // clean up, set ret_value properly if the conversion went out of range
   // set errno if necessary
   // set *endptr properly if the endptr is not NULL








   return ret_value;
}
