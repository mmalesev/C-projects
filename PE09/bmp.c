#include <stdlib.h>
#include <stdio.h>

#include "bmp.h"

/* check whether a header is valid
 * assume that header has been read from fptr
 * the position of the indicator of fptr is not certain
 * could be at the beginning of the file, end of the file or 
 * anywhere in the file
 * note that the check is only for this exercise/assignment
 * in general, the format is more complicated
 */

int Is_BMP_Header_Valid(BMP_Header* header, FILE *fptr) {
  // Make sure this is a BMP file
  if (header->type != 0x4d42) {
     return FALSE;
  }
  // skip the two unused reserved fields

  // check the offset from beginning of file to image data
  // essentially the size of the BMP header
  // BMP_HEADER_SIZE for this exercise/assignment
  if (header->offset != BMP_HEADER_SIZE) {
     return FALSE;
  }
      
  // check the DIB header size == DIB_HEADER_SIZE
  // For this exercise/assignment
  if (header->DIB_header_size != DIB_HEADER_SIZE) {
     return FALSE;
  }

  // Make sure there is only one image plane
  if (header->planes != 1) {
    return FALSE;
  }
  // Make sure there is no compression
  if (header->compression != 0) {
    return FALSE;
  }

  // skip the test for xresolution, yresolution

  // ncolours and importantcolours should be 0
  if (header->ncolours != 0) {
    return FALSE;
  }
  if (header->importantcolours != 0) {
    return FALSE;
  }
  
  // Make sure we are getting 24 bits per pixel
  // or 16 bits per pixel
  // only for this assignment
  if (header->bits != 24 && header->bits != 16) {
    return FALSE;
  }

  // fill in extra to check for file size, image size
  // based on bits, width, and height
  fseek(fptr, 0, SEEK_END);
  if(header->size != ftell(fptr)){
    return FALSE;
  }

  if(header->size != header-> imagesize + sizeof(*header)){
     return FALSE;
  }
  
  int padding;

  if(header->bits == 24){
    padding = (4 - ((header->width * 3) % 4)) % 4;
    if(header->imagesize != (header->width * 3 + padding) * header->height){
      return FALSE;
    }
  }
  else{
    padding = header->width % 2 * 2;
    if(header->imagesize != (header->width * 2 + padding) * header->height){
      return FALSE;
    }
  }

  return TRUE;
}

/* The input argument is the source file pointer. 
 * The function returns an address to a dynamically allocated BMP_Image only 
 * if the file * contains a valid image file 
 * Otherwise, return NULL
 * If the function cannot get the necessary memory to store the image, also 
 * return NULL
 * Any error messages should be printed to stderr
 */
BMP_Image *Read_BMP_Image(FILE* fptr) {

  // go to the beginning of the file
   BMP_Image *bmp_image = NULL;
   fseek(fptr, 0, SEEK_SET);

  //Allocate memory for BMP_Image*;
   bmp_image = malloc(sizeof(*bmp_image));
   if(bmp_image == NULL){
      return NULL;
   }
  //Read the first 54 bytes of the source into the header
   int status = fread(&(bmp_image -> header), 54, 1, fptr);
   if(status != 1){
      free(bmp_image);
      return NULL;
   }

  // if read successful, check validity of header
   if(!Is_BMP_Header_Valid(&(bmp_image -> header), fptr)){
      free(bmp_image);
      return NULL;
   }

  // Allocate memory for image data
   bmp_image -> data = malloc(bmp_image -> header.imagesize); 
   if(bmp_image -> data == NULL){
      free(bmp_image);
      return NULL;
   }
 
  // read in the image data
   fseek(fptr, 54, SEEK_SET);
   status = fread(bmp_image -> data, bmp_image -> header.imagesize, 1, fptr);
   if(status != 1){
      free(bmp_image -> data);
      free(bmp_image);
      return NULL;
   }

  return bmp_image;
}

/* The input arguments are the destination file pointer, BMP_Image *image.
 * The function write the header and image data into the destination file.
 * return TRUE if write is successful
 * FALSE otherwise
 */
int Write_BMP_Image(FILE* fptr, BMP_Image* image) 
{
   // go to the beginning of the file
   fseek(fptr, 0, SEEK_SET);

   // write header
   int size = fwrite(&(image -> header), 54, 1, fptr);
   if(size != 1){
     return FALSE;
   }
   // write image data
   size = fwrite((image -> data), image->header.imagesize, 1, fptr);
   if(size != 1){
     return FALSE;
   }
  
   return TRUE;
}

/* The input argument is the BMP_Image pointer. The function frees memory of 
 * the BMP_Image.
 */
void Free_BMP_Image(BMP_Image* image) {
   free(image->data);
   free(image);
}

// Given a valid BMP_Image, create a new image that retains the image in the
// box specified by left_x, right_x, bottom_y, top_y
//
BMP_Image *Crop_BMP_Image(BMP_Image *image, int left_x, 
                          int right_x, int bottom_y, 
                          int top_y)
{
   // getting the box coordinates to stay within range
   if(left_x < 0){
     left_x = 0;
   }
   if(right_x < 0){
     right_x = 0;
   }
   if(bottom_y < 0){
     bottom_y = 0;
   }
   if(top_y < 0){
     top_y = 0;
   }

   if(left_x > image->header.width - 1){
     left_x = image->header.width - 1;
   }
   if(right_x > image->header.width - 1){
     right_x = image->header.width - 1;
   }
   if(bottom_y > image->header.height - 1){
     bottom_y = image->header.height - 1;
   }
   if(top_y > image->header.height - 1){
     top_y = image->header.height - 1;
   }

   printf("l %d, r %d, b %d, t %d", left_x, right_x, bottom_y, top_y);
   // proceed only if left_x <= right_x and bottom_y <= top_y
   if(left_x > right_x || bottom_y > top_y){
     return NULL;
   }

   // create a new image cropped from the given image
   BMP_Image *t_image = NULL;
   t_image = malloc(sizeof(*t_image));
   if(t_image == NULL){
     return NULL;
   }
   t_image->header = image->header;
   int width = right_x - left_x + 1;
   int height = top_y - bottom_y + 1;

   t_image->header.width = width;
   t_image->header.height = height;

   int padding;

   if(t_image->header.bits == 24){
     padding = (4 - ((t_image->header.width * 3) % 4)) % 4;
     t_image->header.imagesize = (width * 3 + padding) * height;
   }
   else{
     padding = t_image->header.width % 2 * 2;
     t_image->header.imagesize = (width * 2 + padding) * height;
   }
   t_image->header.size = t_image->header.imagesize + 54;
   
   t_image->data = malloc(t_image->header.imagesize);
   if(t_image->data == NULL){
     return NULL;
   }
   int i, j, p;
   long int counter = 0;
   int n;
   if(t_image->header.bits == 24){
     n = 3;
   }
   else{
     n = 2;
   }
   for(i = 0; i < height; i++){
     for(j = 0; j < width * n; j++){
       t_image->data[counter] = image->data[(i + bottom_y) * (image->header.imagesize / image->header.height) + j + left_x * n];
       counter++;
     }
     for(p = 0; p < padding; p++){
       t_image->data[counter] = 0;
       counter++;
     }
   }

   return t_image;
}
