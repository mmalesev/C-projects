#include <stdio.h>
#include <stdlib.h>

#include "list_tree.h"
#include "huffman.h"
typedef struct i_stack{
    char value;
    struct i_stack *next;
}stack;

void push_stack(char x,stack *dummy);
char pop_stack(stack *dummy);
int size_stack(stack *dummy);
void Print_character(tnode *node, FILE *outputfptr, stack *dummy);
void stack_destroy(stack *dummy);

void stack_destroy(stack *dummy)
{
   stack *curr = dummy->next;
   stack *prev = curr;
   while (curr != NULL) {
       free(prev->next);
       prev = curr;
       curr = curr->next;
   }
   dummy->next = NULL;
}

int size_stack(stack *dummy){
    stack *curr;
    curr = dummy;
    int size = 0;
    while(curr->next != NULL){
        curr = curr->next;
        size++;
    }
    return size;
}

void stack_push(char x, stack *dummy){
    stack *new;
    new = (stack *)malloc(sizeof(*new));
    new->value = x;
    new->next = dummy->next;
    dummy->next = new;
}

char stack_pop(stack *dummy){
    stack *temp;
    if(dummy->next!= NULL){
        char x = dummy->next->value;
        temp = dummy->next;
        dummy->next = dummy->next->next;
        free(temp);
        return x;
    }
    else{
        return 0;
    }
}

void Print_character(tnode *node, FILE *outputfptr, stack *dummy){
    if(node->left != NULL){
        stack_push('0', dummy);
        Print_character(node->left, outputfptr, dummy);
        int garbage = stack_pop(dummy);
        (void)(garbage + 1);
    }
    if(node->right != NULL){
        stack_push('1', dummy);
        Print_character(node->right, outputfptr, dummy);
        int garbage = stack_pop(dummy);
        (void)(garbage + 1);
    }
    if(is_leaf_node(node)){
        char letter = (char)node->value;
        fwrite(&letter, 1, 1, outputfptr);
        fputc(':', outputfptr);

        int size = size_stack(dummy);
        int i;
        char *direction = malloc(size_stack(dummy));
        for(i = 0; i < size; i++){
            direction[i] = stack_pop(dummy);
        }
        for(i = size - 1; i >= 0; i--){
            fwrite(&direction[i], 1, 1, outputfptr);
            stack_push(direction[i], dummy);
        }
        free(direction);
        fputc('\n', outputfptr);
    }
    return;
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
   return final_tree;
}

// suggest that you write a recursive function which is called
// by this function to print the huffman codes of the ascii characters
// that appear in the tree in postorder fashion
//
void Post_order_print(tnode *root, FILE *outfptr)
{
   stack *dummy = malloc(sizeof(*dummy));
   dummy->next = NULL;
   Print_character(root, outfptr, dummy);
   stack_destroy(dummy);
   free(dummy);
   return;
}
