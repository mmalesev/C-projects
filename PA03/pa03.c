#include <stdlib.h>
#include <stdio.h>
#include "pa_answer03.h"
#include <string.h>

int main(int argc, char **argv){
    if(argc < 2){
        fprintf(stderr, "Number of arguments provided is not sufficient\n");
        return EXIT_FAILURE;
    }
    if(strcmp(argv[1], "-i") == 0){
        if(argc != 4){
            fprintf(stderr, "Number of arguments is in valid\n");
            return EXIT_FAILURE;
        }
        char *inputfile = argv[2];
        char *outputfile = argv[3];
        double **matrix;
        int size;
        double **inv_matrix;
        int status;

        matrix = Read_matrix_from_file(inputfile, &size);
        if(matrix == NULL){
            return EXIT_FAILURE;
        }
        inv_matrix = Invert_matrix(matrix, size);
        if(inv_matrix == NULL){
            Deallocate_matrix_space(matrix, size);
            return EXIT_FAILURE;
        }

        status = Write_matrix_to_file(outputfile, inv_matrix, size);

        Deallocate_matrix_space(matrix, size);
        Deallocate_matrix_space(inv_matrix, size);
        
        if(status == 0){
            return EXIT_FAILURE;
        }

        return EXIT_SUCCESS;
    }
    else if(strcmp(argv[1], "-m") == 0){
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
                return EXIT_FAILURE;
            }
            matrixb = Read_matrix_from_file(fmatrixb, &sizeb);
            if(matrixb == NULL){
                Deallocate_matrix_space(matrixa, sizea);
                return EXIT_FAILURE;
            }
            matrixo = Matrix_matrix_multiply(matrixa, matrixb, sizea);
            if(matrixo == NULL){
                Deallocate_matrix_space(matrixa, sizea);
                Deallocate_matrix_space(matrixb, sizeb);
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
            if(argc != 3){
                fprintf(stderr, "Wrong number of arguments given\n");
                return EXIT_FAILURE;
            }
            else{
                char *fmatrixa = argv[2];
                double **matrixa; 
                int sizea;
                double diff;

                matrixa = Read_matrix_from_file(fmatrixa, &sizea);
                if(matrixa == NULL){
                    return EXIT_FAILURE;
                }
                diff = Deviation_from_identity(matrixa, sizea);
                if(diff == -1){
                    return EXIT_FAILURE;
                }
                printf("%e\n", diff);
                Deallocate_matrix_space(matrixa, sizea);
                return EXIT_SUCCESS;
                }
    }
    else{
        fprintf(stderr, "Second argument is not valid\n");
        return EXIT_FAILURE;
    }
}
