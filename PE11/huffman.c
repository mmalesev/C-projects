#include <stdio.h>
#include <stdlib.h>

#include "list_tree.h"
#include "huffman.h"

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



   // initialize the stack, here, we have a dummy called stack
   // the code in list_tree.c assumes the presence of a dummy
   // the size of the stack is 0

   lnode stack;
   stack.next = NULL;
   set_stack_size(&stack, 0);

   int token;  // token to be read from the infptr

   // while we are in the header region
   // break from the loop when you cannot continue to build a tree
   // e.g. encountering EOF, asked to build a tree from two trees
   // on the stack, but the stack contains 0 or 1 tree.

   while (1) { // replace if necessary the correct condition
      token = fgetc(infptr); // get a character from the infptr
      if (token == '1') {
	// what follows should be a character
        // read the character
        // build a tree that has a single tree node for that character
        // construct a list node with that tree node
        // push the list node onto the stack







      
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
         
      } else {
         break;
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

   stack_flush(&stack);
   return NULL;
}

// suggest that you write a recursive function which is called
// by this function to print the huffman codes of the ascii characters
// that appear in the tree in postorder fashion
//
void Post_order_print(tnode *root, FILE *outfptr)
{
   return;
}
