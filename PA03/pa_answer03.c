#include "pa_answer03.h"
#include <stdlib.h>
#include <stdio.h>

// Do not change this function
// otherwise, your matrices may not match what we are expecting

int Is_zero(double value)
{
   if (value < 0) {
      value = -value;
   }
   if (value < TOL) {
      return 1;
   } else {
      return 0;
   }
}

// Write your functions below this line
double Deviation_from_identity(double **matrix, int size){
    double diff = 0;
    int i;
    int j;
    double **identity;

    identity = Allocate_matrix_space(size); 
    if(identity == NULL){
        fprintf(stderr, "Identity matrix could not be created\n");
        return -1;
    }
    
    for(i = 0; i < size; i++){
        identity[i][i] = 1;
    }

    for(i = 0; i < size; i++){
        for(j = 0; j < size; j++){
            if(matrix[i][j] - identity[i][j] > 0){
                diff += matrix[i][j] - identity[i][j];
            }
            else{
                diff -= matrix[i][j] - identity[i][j];
            }
        }
    }

    return diff;
}

void Switch_rows(double **matrix, int i, int j, int size){
    double temp;
    int a;
    for(a = 0; a < 2 * size; a++){
        temp = matrix[i][a];
        matrix[i][a] = matrix[j][a];
        matrix[j][a] = temp;
    }
}
double **Invert_matrix(double **matrix, int size){
   int i;
   int j;
   int inverse = 1;
   double divisor;
   int n;
   int m;
   double **inv_matrix;
   
   //realocating space for adding the identity matrix
   for(i = 0; i < size; i++){
       matrix[i] = (double *)realloc(matrix[i], sizeof(**matrix) * 2 *size);
       if(matrix[i] == NULL){
           for(j = 0; j < i; j++){
               free(matrix[j]);
           }
           free(matrix);
           fprintf(stderr, "The space for this matrix could not be created\n");
           return NULL;
       }
   }

   //adding the indentity matrix
   for(i = 0; i < size; i++){
       for(j = size; j < 2 * size; j++){
           if(i == j - size){
               matrix[i][j] = 1;
           }
           else{
               matrix[i][j] = 0;
           }
       }
   }
   
   for(i = 0; i < size; i++){
       //getting diagonals
       if(Is_zero(matrix[i][i])){
           inverse = 0;
           j = i;
           while(j < size && inverse == 0){
               if(!Is_zero(matrix[j][i])){
                   Switch_rows(matrix, i, j, size);
                   inverse = 1;
               }
               j++;
           }
           if(inverse == 0){
               fprintf(stderr, "The matrix does not have inverse\n");
               return NULL;
           }
       }
       divisor = matrix[i][i];
       for(j = 0; j < 2 * size; j++){
           matrix[i][j] /= divisor;
       }

       //bottom triangle
       if(i != size - 1){
           for(n = i + 1; n < size; n++){
               if(!Is_zero(matrix[n][i])){
                   divisor = matrix[n][i];
                   for(m = 0; m < 2 * size; m++){
                       matrix[n][m] = matrix[n][m] - divisor * matrix[i][m];
                   }
               }
           }
       }

       //upper triangle
       if(i != 0){
           for(n = i - 1; n >= 0; n--){
               if(!Is_zero(matrix[n][i])){
                   divisor = matrix[n][i];
                   for(m = 0; m < 2 * size; m++){
                       matrix[n][m] = matrix[n][m] - divisor * matrix[i][m];
                   }
               }
           }
       }
   }

   inv_matrix = Allocate_matrix_space(size);

   for(i = 0; i < size; i++){
       for(j = 0; j < size; j++){
           inv_matrix[i][j] = matrix[i][size + j];
       }
   }

   return inv_matrix;
}
double **Allocate_matrix_space(int size)
{
   double **maze;
   int i;
   maze = (double **)malloc(sizeof(*maze) * size);
   if(maze == NULL){
       return NULL;
   }
   for(i = 0; i < size; i++){
       maze[i] = (double *)malloc(sizeof(**maze) * size);
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

void Deallocate_matrix_space(double **matrix, int size){
   int i;
   for(i = 0; i < size; i++){
       free(matrix[i]);
   }
   free(matrix);
}

double **Matrix_matrix_multiply(double **matrixa, double **matrixb, int size){
    double **matrix;
    matrix = Allocate_matrix_space(size);
    if(matrix == NULL){
        fprintf(stderr, "The matrix is too big\n");
        return NULL;
    }
    int i;
    int j;
    int k;
    double sum;
    for(i = 0; i < size; i++){
        for(j = 0; j < size; j++){
            k = 0;
            sum = 0;
            while(k < size){
                sum += matrixa[i][k] * matrixb[k][j];
                k++;
            }
            matrix[i][j] = sum;
        }
    }
    return matrix;
}

int Write_matrix_to_file(char *filename, double **matrix, int size){
    FILE *fptr = fopen(filename, "w");
    if(fptr == NULL){
        fprintf(stderr, "The file %s could not be oppened properly\n", filename);
        return 0;
    }
    int elements_write;

    elements_write = fwrite(&size, sizeof(int), 1, fptr);

    if(elements_write != 1){
        fprintf(stderr, "The file could not be written properly\n");
        fclose(fptr);
        return 0;
    }

    int i;
    int j;
    for (i = 0; i < size; i++){
        for (j = 0; j < size; j++){
            elements_write = fwrite(&matrix[i][j], sizeof(double), 1, fptr);
            if(elements_write != 1){
                fprintf(stderr, "The file could not be written properly\n");
                fclose(fptr);
                return 0;
            }
        }
    }

    fclose(fptr);
    return 1;
}

double **Read_matrix_from_file(char *filename, int *size){
    FILE *fptr = fopen(filename, "r");
    if(fptr == NULL){
        fprintf(stderr, "The file %s could not be oppened properly\n", filename);
        return NULL;
    }
    int i;
    int j;
    double **matrix;
    int elements_read;

    elements_read = fread(size, sizeof(int), 1, fptr);
    if(elements_read != 1){
        fprintf(stderr, "The file does not contain enough information\n");
        fclose(fptr);
        return NULL;
    }

    matrix =  Allocate_matrix_space(*size);
    if(matrix == NULL){
        fclose(fptr);
        fprintf(stderr, "The matrix is too big\n");
        return NULL;
    }

    for(i = 0; i < *size; i++){
        for(j = 0; j < *size; j++){
            elements_read = fread(&matrix[i][j], sizeof(double), 1, fptr);
            if(elements_read != 1){
                fprintf(stderr, "The file does not contain enough information\n");
                fclose(fptr);
                Deallocate_matrix_space(matrix, *size);
                return NULL;
            }
        }
    }

    fclose(fptr);
    return matrix;
}
