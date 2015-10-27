#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "answer08.h"

int main(int argc, char **argv){

    if (argc < 2){
        fprintf(stderr, "Not enough arguments given to the file\n");
        return EXIT_FAILURE;
    }
    else{
        if (strcmp(argv[1], "-m") == 0){
            if (argc != 5){
                fprintf(stderr, "Wrong number of arguments given\n");
                return EXIT_FAILURE;
            }
            else{
                char *fmatrixa = argv[2];
                char *fmatrixb = argv[3];
                char *foutput = argv[4];
                double **matrixa; 
                int sizea;
                double **matrixb;
                int sizeb;
                double **matrixo;
                int status;

                matrixa = Read_matrix_from_file(fmatrixa, &sizea);
                if(matrixa == NULL){
                    fprintf(stderr, "Could not read the first matrix\n");
                    return EXIT_FAILURE;
                }
                matrixb = Read_matrix_from_file(fmatrixb, &sizeb);
                if(matrixb == NULL){
                    Deallocate_matrix_space(matrixa, sizea);
                    fprintf(stderr, "Could not read the second matrix\n");
                    return EXIT_FAILURE;
                }
                matrixo = Matrix_matrix_multiply(matrixa, matrixb, sizea);
                if(matrixo == NULL){
                    Deallocate_matrix_space(matrixa, sizea);
                    Deallocate_matrix_space(matrixb, sizeb);
                    fprintf(stderr, "Could not read the third matrix\n");
                    return EXIT_FAILURE;
                }
                status = Write_matrix_to_file(foutput, matrixo, sizea);
                Deallocate_matrix_space(matrixa, sizea);
                Deallocate_matrix_space(matrixb, sizeb);
                Deallocate_matrix_space(matrixo, sizea);
                if(status == 0){
                    return EXIT_FAILURE;
                    fprintf(stderr, "Could not subtract the matrices\n");
                }
                else{
                    return EXIT_SUCCESS;
                }
            }
        }
        else if(strcmp(argv[1], "-d") == 0){
            if(argc != 4){
                fprintf(stderr, "Wrong number of arguments given\n");
                return EXIT_FAILURE;
            }
            else{
                char *fmatrixa = argv[2];
                char *fmatrixb = argv[3];
                double **matrixa; 
                int sizea;
                double **matrixb;
                int sizeb;
                double diff;

                matrixa = Read_matrix_from_file(fmatrixa, &sizea);
                if(matrixa == NULL){
                    fprintf(stderr, "Could not read the first matrix\n");
                    return EXIT_FAILURE;
                }
                matrixb = Read_matrix_from_file(fmatrixb, &sizeb);
                if(matrixb == NULL){
                    fprintf(stderr, "Could not read the second matrix\n");
                    Deallocate_matrix_space(matrixa, sizea);
                    return EXIT_FAILURE;
                }
                diff = Matrix_matrix_difference(matrixa, matrixb, sizea);
                printf("%e\n", diff);
                return EXIT_SUCCESS;
            }
        }
        else{
            fprintf(stderr, "Second argument invalid\n");
            return EXIT_FAILURE;
        }
    }
}
