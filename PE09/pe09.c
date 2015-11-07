#include<stdio.h>
#include<stdlib.h>
#include "bmp.h"
#include<string.h>
#include<limits.h>

int main(int argc, char **argv){

  if(argc < 3){
    fprintf(stderr, "Not enough arguments\n");
    return EXIT_FAILURE;
  }
  int i;
  int left_x = INT_MIN;
  int bottom_y = INT_MIN;
  int right_x = INT_MAX;
  int top_y = INT_MAX;
  for(i = 1; i < argc - 1; i++){
    if(strcmp(argv[i], "-l") == 0){
      if((strcmp(argv[i + 1], "-r") == 0) || (strcmp(argv[i + 1], "-b") == 0) || (strcmp(argv[i + 1], "-t") == 0) || (strcmp(argv[i + 1], "-l") == 0)){
        fprintf(stderr, "Invalid arguments\n");
        return EXIT_FAILURE;
      }
      left_x = atoi(argv[++i]);
    }
    if(strcmp(argv[i], "-r") == 0){
      if((strcmp(argv[i + 1], "-l") == 0) || (strcmp(argv[i + 1], "-b") == 0) || (strcmp(argv[i + 1], "-t") == 0) || (strcmp(argv[i + 1], "-r") == 0)){
        fprintf(stderr, "Invalid arguments\n");
        return EXIT_FAILURE;
      }
      right_x = atoi(argv[++i]);
    }
    if(strcmp(argv[i], "-b") == 0){
      if((strcmp(argv[i + 1], "-l") == 0) || (strcmp(argv[i + 1], "-r") == 0) || (strcmp(argv[i + 1], "-t") == 0) || (strcmp(argv[i + 1], "-b") == 0)){
        fprintf(stderr, "Invalid arguments\n");
        return EXIT_FAILURE;
      }
      bottom_y = atoi(argv[++i]);
    }
    if(strcmp(argv[i], "-t") == 0){
      if((strcmp(argv[i + 1], "-l") == 0) || (strcmp(argv[i + 1], "-b") == 0) || (strcmp(argv[i + 1], "-r") == 0) || (strcmp(argv[i + 1], "-t") == 0)){
        fprintf(stderr, "Invalid arguments\n");
        return EXIT_FAILURE;
      }
      top_y = atoi(argv[++i]);
    }
  }

  char *imagefile = argv[i - 1];
  char *outputfile = argv[i++];

  BMP_Image *image;
  BMP_Image *c_image;
  FILE *fptr = fopen(imagefile, "rb");
  if(fptr == NULL){
    fprintf(stderr, "Invalid image file\n");
    return EXIT_FAILURE;
  }
  FILE *fptrw = fopen(outputfile, "wb");
  if(fptrw == NULL){
    fprintf(stderr, "Could not open file for writing\n");
    fclose(fptr);
    return EXIT_FAILURE;
  }
  image = Read_BMP_Image(fptr);
  if(image == NULL){
    fprintf(stderr, "Can't read image from file\n");
    fclose(fptr);
    fclose(fptrw);
    return EXIT_FAILURE;
  }

  c_image = Crop_BMP_Image(image, left_x, right_x, bottom_y, top_y);
  if(c_image == NULL){
    fprintf(stderr, "Can't crop the image\n");
    Free_BMP_Image(image);
    fclose(fptr);
    fclose(fptrw);
    return EXIT_FAILURE;
  }

  int check = Write_BMP_Image(fptrw, c_image);
  if (check == FALSE){
    fprintf(stderr, "Error writing to the output file\n");
    Free_BMP_Image(c_image);
    Free_BMP_Image(image);
    fclose(fptr);
    fclose(fptrw);
  }

  Free_BMP_Image(image);
  Free_BMP_Image(c_image);
  fclose(fptr);
  fclose(fptrw);
  return EXIT_SUCCESS;
}
