#include <stdio.h>
#include <stdlib.h>

#include "list_tree.h"
#include "huffman.h"
int Huffman_decoding(tnode *huffman, FILE *infptr, FILE *outfptr){
    fseek(infptr, 0, SEEK_SET);
    unsigned int file_size;
    int check;
    check = fread(&file_size, sizeof(int), 1, infptr);
    if(check != 1){
       fprintf(stderr, "The file is corrupted\n");
       return -1;
    }
    unsigned int header_size;
    check = fread(&header_size, sizeof(int), 1, infptr);
    if(check != 1){
       fprintf(stderr, "The file is corrupted\n");
       return -1;
    }
    unsigned int original_size;
    check = fread(&original_size, sizeof(int), 1, infptr);
    if(check != 1){
       fprintf(stderr, "The file is corrupted\n");
       return -1;
    }
    fseek(infptr, header_size, SEEK_CUR);
    unsigned char mask = 1;
    mask = mask << 7;
    unsigned int direction;
    tnode *curr = huffman;
    char ch;
    int j = 0;
    int i = 0;
    int counter = 0;
    while(j < original_size){
        if(counter == file_size - header_size - 3 * sizeof(int)){
            fprintf(stderr, "The file is corrupted\n");
            return -1;
        }
        if(i == 0){
            ch = fgetc(infptr);
        }
        mask = 1 << (7 - i);
        direction = mask & ch;
        if(direction == 0){
            curr = curr->left;
        }
        else{
            curr = curr->right;
        }
        if(is_leaf_node(curr)){
                fputc(curr->value, outfptr);
                curr = huffman;
                j++;
        }
        i++;
        if(i == 8){
            i = 0;
            counter++;
        }
    }
    fseek(infptr, 0, SEEK_END);
    if(ftell(infptr) != file_size){
        fprintf(stderr, "The file is corrupted\n");
        return -1;
    }

    return 1;
}

// return NULL if the tree cannot be constructed
// return the constructed huffman tree
// if you want to check for corruption of input file
// check whether you have fully exhausted all bytes in
// the header section to build the tree
//
tnode *Build_huffman_tree(FILE *infptr)
{
   // you have to use to fseek to get to the right location of the file
   // to build the Huffman coding tree
   // First unsigned int in the file is the file size
   // second unsigned int is the header size
   // third unsigned int is the number of characters in the original file
   fseek(infptr, 0, SEEK_SET);
   unsigned int file_size;
   int check;
   check = fread(&file_size, sizeof(int), 1, infptr);
   if(check != 1){
       fprintf(stderr, "The file is corrupted\n");
       return NULL;
   }
   unsigned int header_size;
   check = fread(&header_size, sizeof(int), 1, infptr);
   if(check != 1){
       fprintf(stderr, "The file is corrupted\n");
       return NULL;
   }
   unsigned int original_size;
   check = fread(&original_size, sizeof(int), 1, infptr);
   if(check != 1){
       fprintf(stderr, "The file is corrupted\n");
       return NULL;
   }

   // initialize the stack, here, we have a dummy called stack
   // the code in list_tree.c assumes the presence of a dummy
   // the size of the stack is 0

   lnode stack_x;
   stack_x.next = NULL;
   set_stack_size(&stack_x, 0);

   int token;  // token to be read from the infptr

   // while we are in the header region
   // break from the loop when you cannot continue to build a tree
   // e.g. encountering EOF, asked to build a tree from two trees
   // on the stack, but the stack contains 0 or 1 tree.
   int i = 0;
   while (i < header_size - 1) { // replace if necessary the correct condition
      token = fgetc(infptr); // get a character from the infptr
      if(token == EOF || token < 0 || token > ASCII_COUNT){
          fprintf(stderr, "The file is corrupted\n");
          stack_flush(&stack_x);
          return NULL;
      }
      if (token == '1') {
	// what follows should be a character
        // read the character
        // build a tree that has a single tree node for that character
        // construct a list node with that tree node
        // push the list node onto the stack
        token = fgetc(infptr); 
        if(token == EOF || token < 0 || token > ASCII_COUNT){
            fprintf(stderr, "The file is corrupted\n");
            stack_flush(&stack_x);
            return NULL;
        }
        tnode *new_char;
        new_char = malloc(sizeof(*new_char));
        new_char->value = token;
        new_char->left = NULL;
        new_char->right = NULL;

        lnode *new_node = node_construct(new_char);
        push(&stack_x, new_node);
        i++;
      } else if (token == '0') {  
         // You have to build a bigger tree from two trees in the list nodes
         // popped from the stack
         // After that, you have to push a list node containing the bigger
         // tree onto the stack
         // Beware of:  (1) You can do the construction if there are 
         // 2 or more items in the stack 
         // (2) which of the two trees popped from the stack
         // is left and which is right of the bigger tree
         // (3) cleaning up so that you do not leak memory
         if(stack_size(&stack_x) >= 2){//needs to be changed
             tnode *new_tree;
             new_tree = malloc(sizeof(*new_tree));
             lnode *right_node = pop(&stack_x);
             lnode *left_node = pop(&stack_x);
             new_tree->left = left_node->tree;
             new_tree->right = right_node->tree;
             free(left_node);
             free(right_node);
             lnode *new_node = node_construct(new_tree);
             push(&stack_x, new_node);
         }else{
             fprintf(stderr, "File is corrupted\n");
             return NULL;
         }
      } else {
         fprintf(stderr, "File is corrupted\n");
         return NULL;
      }
      i++;
   }

   // check for the conditions that says that you have successfully
   // constructed a huffman coding tree:
   // you have exhausted all bytes in the header region of the file
   // you have exactly 1 item on the stack
   // in that case, the constructed huffman is contained in the 
   // only list node on the stack, get it, and return the constructed tree
   // otherwise, return NULL
   // always clean up memory to avoid memory leak
   if(stack_size(&stack_x) != 1){
     fprintf(stderr, "File is corrupted");
     return NULL;
   }
   lnode *final_node = pop(&stack_x);
   tnode *final_tree = final_node->tree;
   free(final_node);
   stack_flush(&stack_x);
   //fseek(infptr, header_size, SEEK_SET);
   return final_tree;
}
