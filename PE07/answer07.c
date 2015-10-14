#include <stdio.h>
#include <stdlib.h>
#include "answer07.h"

// if you want to declare and define new functions, put them here
// or at the end of the file
void Find_maze_dimensions(FILE *fptr, int *nrow, int *ncol);

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

// do not remove #ifndef and #endif in this file, otherwise the evaluation
// would fail and you would definitely get 0
// any new functions you want to create and use in this file 
// should appear above these comments or at the end of this file

#ifndef NTEST_MEM

// allocate space for nrow x ncol maze, if allocation fails, return NULL
// if allocation fails, you are also responsible for freeing the memory
// allocated before the failure
// may assume that nrow and ncol are > 0

char **Allocate_maze_space(int nrow, int ncol)
{
   char **maze;
   int i;
   maze = (char **)malloc(sizeof(*maze) * nrow);
   if(maze == NULL){
       return NULL;
   }
   for(i = 0; i < nrow; i++){
       maze[i] = (char *)malloc(sizeof(**maze) * ncol);
       if(maze[i] == NULL){
           int j;
           for(j = 0; j < i; j++){
               free(maze[j]);
           }
           free(maze);
           return NULL;
       }
   }
   return maze;
}

// free the memory used for the maze
// you may assume that maze is not NULL, and all memory addresses are valid

void Deallocate_maze_space(char **maze, int nrow) 
{
   int i;
   for(i = 0; i < nrow; i++){
       free(maze[i]);
   }
   free(maze);
}

#endif /* NTEST_MEM */

#ifndef NTEST_READ

/* Read maze in a multi-line file into a 2D array of characters */
/* you may assume that the maze has odd row and column counts */

char **Read_maze_from_2Dfile(FILE *fptr, int *nrow, int *ncol)
{
   char **maze;
   int i;
   int j;
   *nrow = *ncol = 0;
   Find_maze_dimensions(fptr, nrow, ncol);
   fseek(fptr, 0, SEEK_SET);

   maze = Allocate_maze_space(*nrow, *ncol);
   
   for(i = 0; i < *nrow; i++){
       for(j = 0; j < *ncol; j++){
           maze[i][j] = (char)fgetc(fptr);
       }
       fseek(fptr, 1, SEEK_CUR);  
   }
    
   return maze;
}

#endif /* NTEST_READ */

#ifndef NTEST_WRITE

/* Write maze in a 2D array of characters into a multi-line file */
/* you may assume that the maze has odd row and column counts */

int Write_maze_to_2Dfile(char *filename, char **maze, int nrow, int ncol) 
{
   FILE *fptr = fopen(filename, "w");
   if (fptr == NULL){
       fprintf(stderr, "Writing to the file %s failed\n", filename);
       return -1;
   }
   int counter = 0;
   int i;
   int j;
   for(i = 0; i < nrow; i++){
       for(j = 0; j < ncol; j++){
           fprintf(fptr, "%c", maze[i][j]);
           counter++;
       }
       fprintf(fptr, "%c", '\n');
       counter++;
   }
   fclose(fptr);
   return counter;
}

#endif /* NTEST_WRITE */

#ifndef NTEST_ROW

/* Expand the maze from nrow x ncol to (2nrow - 1) x ncol */
/* the top half of the maze is the same as the original maze */
/* the bottom half of the maze is a reflection of the original maze */
/* *nrow and *rcol have to be updated with the new row and column counts */
/* You have to create a new maze */
/* if you can't expand the maze because of memory issue, NULL should be */
/* returned, and both *nrow and *rcol set to 0 */
/* the original maze has to stay intact */
/* you may assume that the original maze has odd row and column counts */

char **Expand_maze_row(char **maze, int nrow, int ncol, int *rrow, int *rcol)
{
   char **exp_maze;
   int i;
   int j;
   *rcol = ncol;
   *rrow = 2 * nrow - 1;
   
   exp_maze = Allocate_maze_space(*rrow, *rcol);

   if(exp_maze == NULL){
       *rrow = 0;
       *rcol = 0;
       return NULL;
   }

   for(i = 0; i < *rrow; i++){
       for(j = 0; j < *rcol; j++){
           if(i < nrow){
               exp_maze[i][j] = maze[i][j];
           }
           else{
               exp_maze[i][j] = maze[(nrow - 1) - (i - nrow + 1)][j];
           }
       }
   }
   return exp_maze;
}

#endif /* NTEST_ROW */

#ifndef NTEST_COL 

/* Expand the maze from nrow x ncol to nrow x 2ncol - 1 */
/* the left half of the maze is the same as the original maze */
/* the right half of the maze is a reflection of the original maze */
/* moreover, you have to create an opening between the left and right halves */
/* the opening should be at the middle row of the maze */
/* the opening must also be accessible from one of the grass-paths in the */
/* new maze */
/* to do that, you have to convert some locations with corns to grass */
/* starting from the new opening to the left and to the right until you reach */
/* a location that is adjacent to a location that has grass */
/*                                                                       */
/* *crow and *ccol have to be updated with the new row and column counts */
/* You have to create a new maze */
/* if you can't expand the maze because of memory issue, NULL should be */
/* returned, and both *crow and *ccol set to 0 */
/* the original maze has to stay intact */
/* you may assume that the original maze has odd row and column counts */

char **Expand_maze_column(char **maze, int nrow, int ncol, int *crow, int *ccol)
{
   char **exp_maze;
   int i;
   int j;
   int offset = 0;
   int valid = 0;
   *ccol = 2 * ncol - 1;
   *crow = nrow;
   
   exp_maze = Allocate_maze_space(*crow, *ccol);

   if(exp_maze == NULL){
       *crow = 0;
       *ccol = 0;
       return NULL;
   }

   for(i = 0; i < *crow; i++){
       for(j = 0; j < *ccol; j++){
           if(j < ncol){
               exp_maze[i][j] = maze[i][j];
           }
           else{
               exp_maze[i][j] = maze[i][(ncol - 1) - (j - ncol + 1)];
           }
       }
   }

   while(!valid){
       if(exp_maze[*crow / 2 + 1][ncol - 1 - offset] != ' ' && exp_maze[*crow / 2 - 1][ncol - 1 - offset] != ' '){
            exp_maze[*crow / 2][ncol - 1 - offset] = ' ';
            exp_maze[*crow / 2][ncol - 1 + offset] = ' ';
            offset++;
       }
       else{
           exp_maze[*crow / 2][ncol - 1 - offset] = ' ';
           exp_maze[*crow / 2][ncol - 1 + offset] = ' ';
           valid = 1;
       }
   }
   return exp_maze;
}

#endif /* NTEST_COL */

// if you want to declare and define new functions, put them here
// or at the beginning of the file
