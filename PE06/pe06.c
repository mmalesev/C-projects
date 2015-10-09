#include <stdio.h>
#include <stdlib.h>
#include "answer06.h"

int main(int argc, char **argv){
    int r;
    int c;
    int opening;
    int grass;
    char location;
    char newfile[] = "oneline";
    int counter;

    FILE *fptr = fopen(argv[1], "r");

    Find_maze_dimensions(fptr, &r, &c);
    opening = Find_opening_location(fptr);
    grass = Count_grass_locations(fptr);
    location = Get_location_type(fptr, 3, 4); 
    counter = Represent_maze_in_one_line(newfile, fptr);

    printf("Number of columns: %d \n Number of rows %d \n", c, r);
    printf("Location of the opening is %d\n", opening);
    printf("Number of the grass fields is %d\n", grass);
    printf("At the location 3, 4, there is a character %c\n", location);
    printf("Number of characters in the new file %d\n", counter);

    fclose(fptr);

    return EXIT_SUCCESS;
}
