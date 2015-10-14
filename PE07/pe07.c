#include<stdio.h>
#include<stdlib.h>
#include "answer07.h"

int main(int argc, char **argv){
   int nrow;
   int ncol;
   int rrow;
   int rcol;
   int counter;
   int counter2;
//   int i;
//   int j;
   char **maze;
   char **exp_maze;
   FILE *fptr = fopen(argv[1], "r");
   char filename[7] = "output";
   char filename2[8] = "coutput";

   //maze[2][3] = 'a';
   //printf("character %c\nstring %s\n", maze[2][3], maze[1]);
    
   maze = Read_maze_from_2Dfile(fptr, &nrow, &ncol);

   counter = Write_maze_to_2Dfile(filename, maze, nrow, ncol);

   printf("%d\n", counter);

   exp_maze = Expand_maze_column(maze, nrow, ncol, &rrow, &rcol);

   counter2 = Write_maze_to_2Dfile(filename2, exp_maze, rrow, rcol);

   printf("%d\n", counter2);
   Deallocate_maze_space(maze, nrow);
   Deallocate_maze_space(exp_maze, rrow);
   fclose(fptr);
   return 0;
}
