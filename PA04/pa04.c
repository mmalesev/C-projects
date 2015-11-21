#include<stdio.h>
#include<stdlib.h>
#include "bmp.h"
int main(int argc, char **argv){
    if(argc == 3){
      char *imagefile = argv[1];
      FILE *fptr = fopen(imagefile, "rb");
      if(fptr == NULL){
        fprintf(stderr, "Invalid image file\n");
        return EXIT_FAILURE;
      }
      char *outputfile = argv[2];
      FILE *fptrw = fopen(outputfile, "wb");
      if(fptrw == NULL){
        fprintf(stderr, "Could not open file for writing\n");
        fclose(fptr);
        return EXIT_FAILURE;
      }

      BMP_Image *image;
      image = Read_BMP_Image(fptr);
      if(image == NULL){
        fprintf(stderr, "Can't read image from file\n");
        fclose(fptr);
        fclose(fptrw);
        return EXIT_FAILURE;
      }
      BMP_Image *c_image;
      if(image->header.bits == 24){
          c_image = Convert_24_to_16_BMP_Image_with_Dithering(image);
          if(c_image == NULL){
              return EXIT_FAILURE;
          }
      }
      else{
          c_image = Convert_16_to_24_BMP_Image(image);
          if(c_image == NULL){
              return EXIT_FAILURE;
          }
      }

      int status = Write_BMP_Image(fptrw, c_image);
      fclose(fptr);
      fclose(fptrw);
      Free_BMP_Image(image);
      Free_BMP_Image(c_image);

      if(status == TRUE){
          return EXIT_SUCCESS;
      }
      else{
          return EXIT_FAILURE;
      }
    }
    else{
        fprintf(stderr, "Wrong number of arguments expected\n");
        return EXIT_FAILURE;
    }
}

