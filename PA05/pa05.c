#include<stdio.h>
#include<stdlib.h>
#include "huffman.h"
int main(int argc, char **argv){
    if(argc != 3){
        fprintf(stderr, "Wrong number of arguments\n");
        return EXIT_FAILURE;
    }
    FILE *fptr = fopen(argv[1], "r");
    if(fptr == NULL){
        fprintf(stderr, "First file could not be oppened\n");
        return EXIT_FAILURE;
    }
    FILE *wfptr = fopen(argv[2], "w");
    if(wfptr == NULL){
        fprintf(stderr, "Second file could not be oppened\n");
        fclose(fptr);
        return EXIT_FAILURE;
    }
    tnode *tree = Build_huffman_tree(fptr);

    if(tree != NULL){
        int status = Huffman_decoding(tree, fptr, wfptr);
        if(status == -1){
            tree_destruct(tree);
            fclose(fptr);
            fclose(wfptr);
            return EXIT_FAILURE;
        }
        tree_destruct(tree);
        fclose(fptr);
        fclose(wfptr);
    }else{
        fclose(fptr);
        fclose(wfptr);
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}
