#include<stdio.h>
#include<stdlib.h>
#include "answer08.h"

double Matrix_matrix_difference(double **matrixa, double **matrixb, int size){
    double diff = 0;
    int i;
    int j;

    for(i = 0; i < size; i++){
        for(j = 0; j < size; j++){
            if(matrixa[i][j] - matrixb[i][j] > 0){
                diff += matrixa[i][j] - matrixb[i][j];
            }
            else{
                diff -= matrixa[i][j] - matrixb[i][j];
            }
        }
    }

    return diff;
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

void Deallocate_matrix_space(double **matrix, int size){
   int i;
   for(i = 0; i < size; i++){
       free(matrix[i]);
   }
   free(matrix);
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
