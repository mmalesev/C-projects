#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "answer02.h"

// an incomplete test function to call char_to_int
//
void test_char_to_int(char c)
{
    printf("%d\n", char_to_int(c));
    //printf("%d\n", char_to_int('z'));
    //printf("%d\n", char_to_int('Z'));
}

// an incomplete test function to call test_str_to_long_int
//
void test_str_to_long_int()
{
    int base;
    char test_str[] = "10";

    // test for a range of base, including an invalid base 37
    // should also test whether the string has been changed

    for (base = 2; base < 38; base++) {

       // before you call the function str_to_long_int
       // set errno to zero

       errno = 0;
       long int test = str_to_long_int(test_str, base);

       // print errno
       // typically, you want to check errno when the returned value
       // is LONG_MAX or LONG_MIN in a more useful program

       printf("%ld %d\n", test, errno);

       // call strtol to cross check the results 
       // note that the results should not match when base == 16.
       // before you call the function strtol
       // set errno to zero

       errno = 0;
       test = strtol(test_str, NULL, base);
       printf("%ld %d\n", test, errno);
    }

}

int main(int argc, char * * argv)
{
    printf("Welcome to ECE264, we are working on PE02.\n\n");
    test_str_to_long_int();
    // testing the char_to_int function
    // question:  how to do a comprehensive test

    // testing the str_to_long_int function
    // question:  how to do a comprehensive test

    //test_str_to_long_int();

    return EXIT_SUCCESS;
}
