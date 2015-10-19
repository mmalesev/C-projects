#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pa_answer02.h"

int main(int argc, char **argv){

   int status;
   char *mazefile;
   char *directionfile;
   char *mowedfile;
    
   if (argc > 1 && strcmp(argv[1], "-s") == 0){
       if (argc == 5){
           mazefile = argv[2];
           directionfile = argv[3];
           mowedfile = argv[4];

           status = Simulate_mowing(mazefile, directionfile, mowedfile);
           printf("%d\n", status);
       }
       else{
           return EXIT_FAILURE;
       }
   }
   else if(argc > 1 && strcmp(argv[1], "-m") == 0){
       if(argc == 4){
           mazefile = argv[2];
           directionfile = argv[3];

           Get_mowing_directions(mazefile, directionfile);
       }
       else{
           return EXIT_FAILURE;
       }
   }
   else{
       return EXIT_FAILURE;
   }

   return EXIT_SUCCESS;
}
