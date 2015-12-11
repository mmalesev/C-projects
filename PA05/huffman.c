#include <stdio.h>
#include <stdlib.h>

#include "list_tree.h"
#include "huffman.h"

// return NULL if the tree cannot be constructed
// return the constructed huffman tree
//
// in general, the algorithm is the same as in PE11 and PE12, 
// except that you have character '0' or '1' to represent building
// a tree from two subtrees or building a tree for an ASCII character
// are now bit 0 or bit 1.  Consequently, as in the case of decoding,
// you have to process the header information using bits instead of
// bytes.  Note that when you have to build a tree for an ASCII character,
// the character may straddle two bytes.  Therefore, you have to combine
// some bits from two bytes to form an ASCII character.  Again, as in
// the case of decoding, we process the bits from a byte from the most
// significant position to the least significant position.
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

   unsigned char token;  // token to be read from the infptr

   // while we are in the header region
   // break from the loop when you cannot continue to build a tree
   // e.g. encountering EOF, asked to build a tree from two trees
   // on the stack, but the stack contains 0 or 1 tree.
   int bit_counter = 8;
   int char_counter = 0;
   unsigned char character;
   unsigned char mask;
   int ones = 0;
   int zeros = 0;
   int i;
   while (char_counter < header_size || ones != zeros) { // replace if necessary the correct condition
      if(bit_counter == 8){
          character = fgetc(infptr); // get a character from the infptr
          if(character == EOF || character < 0 || character > ASCII_COUNT){
              fprintf(stderr, "The file is corrupted\n");
              stack_flush(&stack_x);
              return NULL;
          }
          bit_counter = 0;
          char_counter++;
      }
      mask = 1 << (7 - bit_counter);
      token = character & mask;
      if(token != 0){
          token /= token;
      }
      bit_counter++;
      if (token == 1) {
	// what follows should be a character
        // read the character
        // build a tree that has a single tree node for that character
        // construct a list node with that tree node
        // push the list node onto the stack
        token = 0;
        ones++;
        unsigned char temp_char;
        for(i = 0; i < 8; i++){
            if(bit_counter == 8){
               character = fgetc(infptr); // get a character from the infptr
               if(character == EOF || character < 0 || character > ASCII_COUNT){
                   fprintf(stderr, "The file is corrupted\n");
                   stack_flush(&stack_x);
                   return NULL;
                }
                bit_counter = 0;
                char_counter++;
            }
            mask = 1 << (7 - bit_counter);
            temp_char = character & mask;
            if(temp_char != 0){
                temp_char /= temp_char;
            }
            temp_char = temp_char << (7 - i);
            token = token | temp_char;
            bit_counter++;
        }
        tnode *new_char;
        new_char = malloc(sizeof(*new_char));
        new_char->value = (int)token;
        new_char->left = NULL;
        new_char->right = NULL;

        lnode *new_node = node_construct(new_char);
        push(&stack_x, new_node);
      } else if (token == 0) {  
         // You have to build a bigger tree from two trees in the list nodes
         // popped from the stack
         // After that, you have to push a list node containing the bigger
         // tree onto the stack
         // Beware of:  (1) You can do the construction if there are 
         // 2 or more items in the stack 
         // (2) which of the two trees popped from the stack
         // is left and which is right of the bigger tree
         // (3) cleaning up so that you do not leak memory
         zeros++;
         if(zeros > ones){
             stack_flush(&stack_x);
             fprintf(stderr, "File is corrupted\n");
             return NULL;
         }

         if(stack_size(&stack_x) >= 2){
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
         }
      } else {
         fprintf(stderr, "File is corrupted\n");
         stack_flush(&stack_x);
         return NULL;
      }
   }

   // check for the conditions that says that you have successfully
   // constructed a huffman coding tree:
   // you have exhausted all bytes in the header region of the file
   // you have exactly 1 item on the stack
   // in that case, the constructed huffman is contained in the 
   // only list node on the stack, get it, and return the constructed tree
   // otherwise, return NULL
   // always clean up memory to avoid memory leak
   /*
   while(stack_size(&stack_x) != 1){
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
   }
   */
   if(stack_size(&stack_x) != 1){
     fprintf(stderr, "File is corrupted");
     stack_flush(&stack_x);
     return NULL;
   }
   lnode *final_node = pop(&stack_x);
   tnode *final_tree = final_node->tree;
   free(final_node);
   stack_flush(&stack_x);
   return final_tree;
}

// if you successfully decode, return 1
// otherwise return 0
// The definition of success: the third unsigned integer specifies the 
// number of characters to be decoded.  If you decoded all characters as 
// specified and there are no more characters left in the encoded file
// for you to decode, it is a success.
//
// Even if decoding fails, whatever you have written into outfptr 
// should remain.  You just have to return 0.
//

int Huffman_decoding(tnode *huffman, FILE *infptr, FILE *outfptr){
    fseek(infptr, 0, SEEK_SET);
    unsigned int file_size;
    int check;
    check = fread(&file_size, sizeof(int), 1, infptr);
    if(check != 1){
       fprintf(stderr, "The file is corrupted\n");
       return 0;
    }
    unsigned int header_size;
    check = fread(&header_size, sizeof(int), 1, infptr);
    if(check != 1){
       fprintf(stderr, "The file is corrupted\n");
       return 0;
    }
    unsigned int original_size;
    check = fread(&original_size, sizeof(int), 1, infptr);
    if(check != 1){
       fprintf(stderr, "The file is corrupted\n");
       return 0;
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
            return 0;
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
        return 0;
    }

    return 1;
}
