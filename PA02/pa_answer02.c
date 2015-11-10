#include <stdio.h>
#include <stdlib.h>
#include "pa_answer02.h"

void Find_maze_dimensions(FILE *fptr, int *nrow, int *ncol);
char **Allocate_maze_space(int nrow, int ncol);
void Deallocate_maze_space(char **maze, int nrow); 
char **Read_maze_from_2Dfile(FILE *fptr, int *nrow, int *ncol);
void Write_maze_to_2Dfile(char *filename, char **maze, int nrow, int ncol);
int Find_opening_location(FILE *fptr);
int Count_grass_locations(FILE *fptr);
void Find_direction(FILE *fptr, char **maze, int row, int col, int maze_r, int maze_c);

void Find_direction(FILE *fptr, char **maze, int row, int col, int maze_r, int maze_c){
    if(row < maze_r - 1 && col < maze_c - 1){
        if(maze[row+1][col] == ' '){
           fprintf(fptr, "S");
           maze[row+1][col] = '.';
           Find_direction(fptr, maze, row + 1, col, maze_r, maze_c);
           fprintf(fptr, "N");
        }
        if(row != 0 && maze[row-1][col] == ' '){
           fprintf(fptr, "N");
           maze[row-1][col] = '.';
           Find_direction(fptr, maze, row - 1, col, maze_r, maze_c);
           fprintf(fptr, "S");
        }
        if(maze[row][col+1] == ' '){
           fprintf(fptr, "E");
           maze[row][col+1] = '.';
           Find_direction(fptr, maze, row, col+1, maze_r, maze_c);
           fprintf(fptr, "W");
        }
        if(col != 0 && maze[row][col-1] == ' '){
           fprintf(fptr, "W");
           maze[row][col-1] = '.';
           Find_direction(fptr, maze, row, col-1, maze_r, maze_c);
           fprintf(fptr, "E");
        }
    }
    else{
        maze[row][col] = '.';
    }
}

void Get_mowing_directions(char *mazefile, char *directionfile){
   FILE *fptr_maze = fopen(mazefile, "r");
   if (fptr_maze == NULL){
       fprintf(stderr, "Opening the file %s failed\n", mazefile);
       return;
   }

   FILE *fptr_dir = fopen(directionfile, "w");
   if (fptr_dir == NULL){
       fclose(fptr_maze);
       fprintf(stderr, "Opening the file %s failed\n", directionfile);
       return;
   }

   int maze_r;
   int maze_c;
   char **maze;
   int row = 0;
   int col = Find_opening_location(fptr_maze);
   if(col == -1){
      fprintf(stderr, "Incorrect maze - no opening\n");
      return;
   }

   maze = Read_maze_from_2Dfile(fptr_maze, &maze_r, &maze_c);
   if(maze == NULL){
      return;
   }

   maze[row][col] = '.';
   Find_direction(fptr_dir, maze, row, col, maze_r, maze_c);

   Deallocate_maze_space(maze, maze_r); 
   fclose(fptr_maze);
   fclose(fptr_dir);
}

int Simulate_mowing(char *mazefile, char *directionfile, char *mowedfile){
   FILE *fptr_maze = fopen(mazefile, "r");
   if (fptr_maze == NULL){
       fprintf(stderr, "Opening the file %s failed\n", mazefile);
       return -1;
   }
   
   FILE *fptr_dir = fopen(directionfile, "r");
   if (fptr_dir == NULL){
       fclose(fptr_maze);
       fprintf(stderr, "Opening the file %s failed\n", directionfile);
       return -1;
   }
   
   int ch;
   char **maze;
   int maze_r;
   int maze_c;
   int counter = 0;
   int legal = 1;
   int row = 0;
   int col = Find_opening_location(fptr_maze);
   int grass = Count_grass_locations(fptr_maze);
   int opening = col;

   maze = Read_maze_from_2Dfile(fptr_maze, &maze_r, &maze_c);
   
   while((ch = fgetc(fptr_dir)) != EOF && legal == 1){
       if(maze[row][col] == ' '){
           maze[row][col] = '.';
           counter++;
       }
       if(ch != 'N' && ch != 'S' && ch != 'E' && ch != 'W'){ 
           legal = 0;
       }
       else{
           if(ch == 'N'){
               row--;
           }
           else if(ch == 'S'){
               row++;
           }
           else if(ch == 'E'){
               col++;
           }
           else if(ch == 'W'){
               col--;
           }
           if(row >= maze_r || row < 0 || col >= maze_c || col < 0 || maze[row][col] == 'X'){
               legal = 0;
           }
       }
   }

   fclose(fptr_maze);
   fclose(fptr_dir);

   if(row < maze_r && row >=0 && col < maze_c && col >= 0 && maze[row][col] == ' '){
       maze[row][col] = '.';
       counter++;
   }

   Write_maze_to_2Dfile(mowedfile, maze, maze_r, maze_c);

   Deallocate_maze_space(maze, maze_r); 

   if(legal == 1 && row == 0 && col == opening){
       return grass - counter;
   }
   else{
       return -1;
   }
}

int Find_opening_location(FILE *fptr)
{
   int opening; //if there is no opening the function will return -1
   int ch;
   int column = 0;
   fseek(fptr, 0, SEEK_SET);
   while((ch = fgetc(fptr)) != '\n'){
       if (ch == ' '){
           opening = column; 
           return opening;
       }
       column++;
   }
   return -1;
}

void Write_maze_to_2Dfile(char *filename, char **maze, int nrow, int ncol) 
{
   FILE *fptr = fopen(filename, "w");
   if (fptr == NULL){
       fprintf(stderr, "Writing to the file %s failed\n", filename);
   }
   int i;
   int j;
   for(i = 0; i < nrow; i++){
       for(j = 0; j < ncol; j++){
           fprintf(fptr, "%c", maze[i][j]);
       }
       fprintf(fptr, "%c", '\n');
   }
   fclose(fptr);
}

char **Read_maze_from_2Dfile(FILE *fptr, int *nrow, int *ncol)
{
   char **maze;
   int i;
   int j;
   *nrow = *ncol = 0;
   Find_maze_dimensions(fptr, nrow, ncol);
   fseek(fptr, 0, SEEK_SET);

   maze = Allocate_maze_space(*nrow, *ncol);
   if(maze == NULL){
      fprintf(stderr, "Space for the maze could not be allocated\n");
      return NULL;
   }
   
   for(i = 0; i < *nrow; i++){
       for(j = 0; j < *ncol; j++){
           maze[i][j] = (char)fgetc(fptr);
       }
       fseek(fptr, 1, SEEK_CUR);  
   }
    
   return maze;
}

void Deallocate_maze_space(char **maze, int nrow) 
{
   int i;
   for(i = 0; i < nrow; i++){
       free(maze[i]);
   }
   free(maze);
}

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
