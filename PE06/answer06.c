#include <stdio.h>
#include <stdlib.h>
#include "answer06.h"

/* Determine the dimensions of the maze contain in file. */
/* The maze will always of the correct format */
/* the number of rows is stored at location pointed to by nrow */ 
/* the number of ncol is stored at location pointed to by ncol */

void Find_maze_dimensions(FILE *fptr, int *nrow, int *ncol)
{
   *nrow = *ncol = 0;
   fseek(fptr, 0, SEEK_SET);
   int ch;
   while((ch = fgetc(fptr)) != EOF){
       if (ch == '\n'){
           *nrow += 1;
       }
       else{
           *ncol += 1;
       }
   }
   *ncol /= *nrow;
}

/* Determine the column location of the top opening */

int Find_opening_location(FILE *fptr)
{
   int opening = -1; //if there is no opening the function will return -1
   int ch;
   int column = 0;
   fseek(fptr, 0, SEEK_SET);
   while((ch = fgetc(fptr)) != '\n'){
       if (ch == ' '){
           opening = column; 
       }
       column++;
   }
   return opening;
}

/* Count the number of locations with grass */
/* A location has grass if it is a space, ' ', or GRASS */

int Count_grass_locations(FILE *fptr)
{
   fseek(fptr, 0, SEEK_SET);
   int grass = 0;
   int ch;
   while((ch = fgetc(fptr)) != EOF){
       if (ch == ' '){
           grass++;
       }
   }
   return grass;
}

/* Return the type of location: CORN or GRASS, i.e. 'X' or ' ' */
/* In other words, return the character in the file for that location */ 
/* The location is specified by its row-column coordinates. */
/* The coordinates are specified by row and col */
/* These coordinates will always be valid for the given maze */
/* You do not have to worry about the coordinates being out of range */

char Get_location_type(FILE *fptr, int row, int col) {
   fseek(fptr, 0, SEEK_SET);
   int nrows;
   int ncolumns;
   char c;
   Find_maze_dimensions(fptr, &nrows, &ncolumns);
   fseek(fptr, row * (ncolumns + 1) + col, SEEK_SET);
   c = (char)fgetc(fptr);
   return c;   
}
 
/* Given a filename, re-represent the maze in the file pointer fptr */
/* in a single line */
/* The return value should be the number of characters written into the */
/* the new file */
/* if the writing to a file fails, you should return -1 */ 

int Represent_maze_in_one_line(char *filename, FILE *fptr)
{
   int counter = 0;
   int ch;
   fseek(fptr, 0, SEEK_SET);
   FILE *newfptr = fopen(filename, "w");
   
   if (fptr == NULL){
       fprintf(stderr, "Writing to the file %s failed\n", filename);
       return -1;
   }

   while((ch = fgetc(fptr)) != EOF){
       if (ch != '\n'){
           fprintf(newfptr, "%c", ch);
           counter++;
       }
   }
   
   fclose(newfptr);
   
   return counter;
}
