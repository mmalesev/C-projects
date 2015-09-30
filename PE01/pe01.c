#include <stdio.h>
#include <stdlib.h>
#include "answer01.h"

void printArray(int * array, int len)
{
    printf("{");
    int ind;

    for(ind = 0; ind < len; ++ind) {
	printf("%d", array[ind]);
	if(ind != len - 1) {
	    printf(", ");
	}
    }
    printf("}");
    fflush(stdout); 
}

// Given a filename, this function reads the entries in the file 
// into an array, and returns the array and the size of the array
// the file is assumed to store only numbers.
// The function first determines the number of numbers in the file,
// then allocates the right array size, 
// and reads all the numbers into the array.
//
int *read_in_Array(char *filename, int *arraysize)
{
   if (filename == NULL) {
      *arraysize = 0;
      return NULL;
   } 
   // we have a valid filename
   // check if the file is present, open the file for read operation

   FILE *fp = fopen(filename, "r");
   if (fp == NULL) { // the file is not present, or we don't have the permission to read the file
      fprintf(stderr, "can't open input file, return NULL array\n");
      *arraysize = 0;
      return NULL;
   }

   // check the number of lines in the file

   int n_line = 0;
   while (!feof(fp)) { 
      int read_stat;
      int dummy;
      read_stat = fscanf(fp, "%d ", &dummy);
      if (read_stat != 1) { // no number in that line, return NULL;
	 fprintf(stderr, "input file in wrong format, read_stat %d, return NULL array\n", read_stat);
	 fclose(fp);
         *arraysize = 0;
         return NULL;
      }
      n_line++;
   }
   if (fseek(fp, 0, SEEK_SET) != 0) { // unsuccessfully rewind to the beginning of file   
      fprintf(stderr, "can't rewind input file, return NULL array\n");
      fclose(fp);
      *arraysize = 0;
      return NULL;
   }
   int *array = (int *)malloc(sizeof(int) * n_line);
   if (array == NULL) { // no memory available
      fprintf(stderr, "no memory available, return NULL array\n");
      fclose(fp);
      *arraysize = 0;
      return NULL;
   }

   // now, we can fill in the array
   n_line = 0;
   while (!feof(fp)) {
      int read_stat;
      read_stat = fscanf(fp, "%d ", &(array[n_line]));
      if (read_stat != 1) { // no number in that line, return NULL;
	 fprintf(stderr, "input file in wrong format, return NULL array\n");
	 free(array);
         fclose(fp);
         *arraysize = 0;
         return NULL;
      }
      n_line++;
   }
   fclose(fp);
   *arraysize = n_line;
   return array;
}

// test function for largest_partial_sum
//
void test_largest_partial_sum(int * array, int len, int expected)
{
    printArray(array, len);
    int sum = largest_partial_sum(array, len);
    printf(". partial sum = %d, expected = %d.", sum, expected);
    if(sum != expected)
	printf(" FAIL");
    printf("\n");
}

// test function for largest_partial_sum
//
void test_00_largest_partial_sum()
{
    printf("Testing largest_partial_sum(...)\n");

    int array1[] = { -4, -1, 0, 1, 5, 10, 20, 21 };
    int len1 = 8;
    test_largest_partial_sum(array1, len1, 52);

    int array2[] = {};
    int len2 = 0;
    test_largest_partial_sum(array2, len2, 0);

    int array3[] = { 1, 4, -1, 6, -5, 4}; 
    int len3 = 6;
    test_largest_partial_sum(array3, len3, 10);

    int array4[] = { -1, -2, -3, -4 };
    int len4 = 4;
    test_largest_partial_sum(array4, len4, -1);

    int array5[] = { -1, 13, 0, 0, -2, -3, -4 };
    int len5 = 7;
    test_largest_partial_sum(array5, len5, 12);

    printf("\n"); // Tidy output is easier to use
}

// test function for largest_difference
//
void test_largest_difference(int * array, int len, int expected)
{
    printArray(array, len);
    int diff = largest_difference(array, len);
    printf(". difference = %d, expected = %d.", diff, expected);
    if(diff != expected)
	printf(" FAIL");
    printf("\n");
}

//test function for largest_difference
void test_01_largest_difference()
{
    printf("Testing largest_difference(...)\n");

    int array1[] = { -4, -1, 0, 1, 5, 10, 20, 21 };
    int len1 = 8;
    test_largest_difference(array1, len1, 25);

    int array2[] = {};
    int len2 = 0;
    test_largest_difference(array2, len2, 0);

    int array3[] = { 1, 4, -1, 6, -5, 4}; 
    int len3 = 6;
    test_largest_difference(array3, len3, 11);

    int array4[] = { -1, -2, -3, -4 };
    int len4 = 4;
    test_largest_difference(array4, len4, 3);
    
    int array5[] = { -1, 13, 0, 0, -2, -3, -4 };
    int len5 = 7;
    test_largest_difference(array5, len5, 17);
    
    printf("\n");
}

int main(int argc, char * * argv)
{
    printf("Welcome to ECE264, we are working on PE01.\n\n");

    if (argc < 2) {  // when no arguments are given, we are in test mode

    	test_00_largest_partial_sum();
    	test_01_largest_difference();

    } else { 
	    int return_value = 0;
	    int *array = NULL;
	    int len = 9;

	// we expect two arguments:
	//
	// the first argument is an integer from the sets {0, 1}
	// 0: run largest_partial_sum(int * array, int len)
	// 1: run largest_difference(int * array, int len)
	// otherwise: do nothing;
	// for the cases of first argument being 0 or 1, 
	// store the results in the variable return_value, e.g., return_value = largest_partial_sum(...);
	// print the return_value using the statement printf("%d\n", return_value);
	// 
	// the second argument is a filename.  The function read_in_Array 
	// is called to fill in the array and to initialize len correctly.

	// check that we have an argument for the filename
	// call the function to fill in the array	
        if (argc == 3) {
	    array = read_in_Array(argv[2], &len); 
	    printArray(array, len);
	    }   
        else {
	        return EXIT_FAILURE;
        }

        // calling a function based on the second argument
        if (atoi(argv[1]) == 0){
            return_value = largest_partial_sum(array, len);
        }
        else if (atoi(argv[1]) == 1){
            return_value = largest_difference(array, len);
        }

	    printf("\n\n%d\n", return_value);

	    if (array != NULL) {
	        free(array);
	    }
    }
    return EXIT_SUCCESS;
}
