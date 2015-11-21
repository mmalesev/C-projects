#include <stdlib.h>
#include <stdio.h>

#include "bmp.h"
int quant_error(unsigned char color);

int quant_error(unsigned char color){
    unsigned char bit_16 = color >> 3;
    unsigned char bit_24 = bit_16 * (255.0 / 31);
    return((int)color - (int)bit_24);
}
BMP_Image *Convert_24_to_16_BMP_Image_with_Dithering(BMP_Image *image){
    BMP_Image *conv_image;
    conv_image = malloc(sizeof(*conv_image));
    if(conv_image == NULL){
        fprintf(stderr, "Error allocating memory\n");
        return NULL;
    }
    conv_image->header = image->header;
    conv_image->header.bits = 16;
    short padding = conv_image->header.width % 2 * 2;
    conv_image->header.imagesize = conv_image->header.height * (conv_image->header.width * 2 + padding);
    conv_image->header.size = conv_image->header.imagesize + BMP_HEADER_SIZE; 
    conv_image->data = malloc(conv_image->header.imagesize);
    int *q_error = calloc(image->header.imagesize, sizeof(int));
    if(conv_image->data == NULL){
        free(conv_image);
        fprintf(stderr, "Error allocating memory\n");
        return NULL;
    }
    short im_padding = (4 - image->header.width % 4) % 4;
    int i, j;
    unsigned short RGB;
    unsigned char color;
    short *sconv_image = (short *)&(conv_image->data[0]);
    int counter = 0;
    int pixel_error;
    int temp;
    for(i = conv_image->header.height - 1; i >= 0; i--){
        counter = 0;
        for(j = 0; j < conv_image->header.width * 3; j++){
            color = image->data[i * (image->header.width * 3 + im_padding) + j];
            pixel_error = quant_error(color);
            if(j < conv_image->header.width * 3 - 3){
                q_error[i * (image->header.width * 3 + im_padding) + j + 3] += 7 * pixel_error; 
            }
            if(j < conv_image->header.width * 3 - 3 && i > 0){
                q_error[(i - 1) * (image->header.width * 3 + im_padding) + j + 3] += pixel_error; 
            }
            if(i > 0){
                q_error[(i - 1) * (image->header.width * 3 + im_padding) + j] += 5 * pixel_error; 
            }
            if(j > 2 && i > 0){
                q_error[(i - 1) * (image->header.width * 3 + im_padding) + j - 3] += 3 * pixel_error; 
            }
            temp = (int)color + q_error[i * (image->header.width * 3 + im_padding) + j] / 16;
            if(temp < 0){
                color = 0;
            }
            else if(temp > 255){
                color = 255;
            }
            else{
                color += (unsigned char)(q_error[i * (image->header.width * 3 + im_padding) + j] / 16.0);
            }

            color >>= 3;
            RGB = color << BLUE_BIT; 
            j++;
            color = image->data[i * (image->header.width * 3 + im_padding) + j];
            pixel_error = quant_error(color);
            if(j < conv_image->header.width * 3 - 3){
                q_error[i * (image->header.width * 3 + im_padding) + j + 3] += 7 * pixel_error; 
            }
            if(j < conv_image->header.width * 3 - 3 && i > 0){
                q_error[(i - 1) * (image->header.width * 3 + im_padding) + j + 3] += pixel_error; 
            }
            if(i > 0){
                q_error[(i - 1) * (image->header.width * 3 + im_padding) + j] += 5 * pixel_error; 
            }
            if(j > 2 && i > 0){
                q_error[(i - 1) * (image->header.width * 3 + im_padding) + j - 3] += 3 * pixel_error; 
            }
            temp = (int)color + q_error[i * (image->header.width * 3 + im_padding) + j] / 16.0;
            if(temp < 0){
                color = 0;
            }
            else if(temp > 255){
                color = 255;
            }
            else{
                color += (unsigned char)(q_error[i * (image->header.width * 3 + im_padding) + j] / 16.0);
            }
            color >>= 3;
            RGB = RGB | (color << GREEN_BIT);
            j++;
            color = image->data[i * (image->header.width * 3 + im_padding) + j];
            pixel_error = quant_error(color);
            if(j < conv_image->header.width * 3 - 3){
                q_error[i * (image->header.width * 3 + im_padding) + j + 3] += 7 * pixel_error; 
            }
            if(j < conv_image->header.width * 3 - 3 && i > 0){
                q_error[(i - 1) * (image->header.width * 3 + im_padding) + j + 3] += pixel_error; 
            }
            if(i > 0){
                q_error[(i - 1) * (image->header.width * 3 + im_padding) + j] += 5 * pixel_error; 
            }
            if(j > 2 && i > 0){
                q_error[(i - 1) * (image->header.width * 3 + im_padding) + j - 3] += 3 * pixel_error; 
            }
            temp = (int)color + q_error[i * (image->header.width * 3 + im_padding) + j] / 16.0;
            if(temp < 0){
                color = 0;
            }
            else if(temp > 255){
                color = 255;
            }
            else{
                color += (unsigned char)(q_error[i * (image->header.width * 3 + im_padding) + j] / 16.0);
            }
            color >>= 3;
            RGB = RGB | (color << RED_BIT);
            sconv_image[i * (image->header.width + padding) + counter] = RGB;
            counter++;

            /*q_error = 0;
            color = image->data[i * (image->header.width * 3 + im_padding) + j];
            if(j < conv_image->header.width * 3 - 3 && i != conv_image->header.height - 1){
                q_error += 3 * quant_error(conv_image->data[(i + 1) * (image->header.width * 3 + im_padding) + j + 3]); 
            }
            if(i != conv_image->header.height - 1){
                q_error += 5 * quant_error(conv_image->data[(i + 1) * (image->header.width * 3 + im_padding) + j]); 
            }
            if(j > 2 && i != conv_image->header.height - 1){
                q_error += quant_error(conv_image->data[(i + 1) * (image->header.width * 3 + im_padding) + j - 3]); 
            }
            if(j > 2){
                q_error += 7 * quant_error(conv_image->data[i * (image->header.width * 3 + im_padding) + j - 3]); 
            }
            color += q_error / 16;
            if(color < 0){
                color = 0;
            }
            if(color > 255){
                color = 255;
            }
            color >>= 3;
            RGB = color << BLUE_BIT; 
            j++;
            q_error = 0;
            color = image->data[i * (image->header.width * 3 + im_padding) + j];
            if(j < conv_image->header.width * 3 - 2 && i != conv_image->header.height - 1){
                q_error += 3 * quant_error(conv_image->data[(i + 1) * (image->header.width * 3 + im_padding) + j + 3]); 
            }
            if(i != conv_image->header.height - 1){
                q_error += 5 * quant_error(conv_image->data[(i + 1) * (image->header.width * 3 + im_padding) + j]); 
            }
            if(j > 2 && i != conv_image->header.height - 1){
                q_error += quant_error(conv_image->data[(i + 1) * (image->header.width * 3 + im_padding) + j - 3]); 
            }
            if(j > 2){
                q_error += 7 * quant_error(conv_image->data[i * (image->header.width * 3 + im_padding) + j - 3]); 
            }
            color += q_error / 16;
            if(color < 0){
                color = 0;
            }
            if(color > 255){
                color = 255;
            }
            color >>= 3;
            RGB = RGB | (color << GREEN_BIT);
            j++;
            color = image->data[i * (image->header.width * 3 + im_padding) + j];
            q_error = 0;
            if(j < conv_image->header.width * 3 - 1 && i != conv_image->header.height - 1){
                q_error += 3 * quant_error(conv_image->data[(i + 1) * (image->header.width * 3 + im_padding) + j + 3]); 
            }
            if(i != conv_image->header.height - 1){
                q_error += 5 * quant_error(conv_image->data[(i + 1) * (image->header.width * 3 + im_padding) + j]); 
            }
            if(j > 2 && i != conv_image->header.height - 1){
                q_error += quant_error(conv_image->data[(i + 1) * (image->header.width * 3 + im_padding) + j - 3]); 
            }
            if(j > 2){
                q_error += 7 * quant_error(conv_image->data[i * (image->header.width * 3 + im_padding) + j - 3]); 
            }
            color += q_error / 16;
            if(color < 0){
                color = 0;
            }
            if(color > 255){
                color = 255;
            }
            color >>= 3;
            RGB = RGB | (color << RED_BIT);
            sconv_image[counter++] = RGB;*/
        }
        for(j=0; j<padding; j++){
            sconv_image[counter++] = 0;
        }
    }
    return conv_image;
}

BMP_Image *Convert_16_to_24_BMP_Image(BMP_Image *image){
    BMP_Image *conv_image;
    conv_image = malloc(sizeof(*conv_image));
    if(conv_image == NULL){
        fprintf(stderr, "Error allocating memory\n");
        return NULL;
    }
    conv_image->header = image->header;
    conv_image->header.bits = 24;
    short padding = (4 - image->header.width % 4) % 4;
    conv_image->header.imagesize = conv_image->header.height * (conv_image->header.width * 3 + padding);
    conv_image->header.size = conv_image->header.imagesize + BMP_HEADER_SIZE; 
    conv_image->data = malloc(conv_image->header.imagesize);
    if(conv_image->data == NULL){
        free(conv_image);
        fprintf(stderr, "Error allocating memory\n");
        return NULL;
    }
    short im_padding = conv_image->header.width % 2 * 2;
    int i, j;
    unsigned short RGB;
    unsigned char color;
    short *s_image = (short *)&(image->data[0]);
    int counter = 0;
    for(i = 0; i < conv_image->header.height; i++){
        for(j = 0; j < conv_image->header.width; j++){
            RGB = s_image[i * (conv_image->header.width + im_padding) + j];
            color = RGB & BLUE_MASK;
            color *= (255.0/31);
            conv_image->data[counter++] = color;
            RGB >>= 5;
            color = RGB & BLUE_MASK;
            color *= (255.0/31);
            conv_image->data[counter++] = color;
            RGB >>= 5;
            color = RGB & BLUE_MASK;
            color *= (255.0/31);
            conv_image->data[counter++] = color;
        }
        for(j=0; j<padding; j++){
            conv_image->data[counter++] = 0;
        }
    }
    return conv_image;

}
BMP_Image *Convert_24_to_16_BMP_Image(BMP_Image *image){
    BMP_Image *conv_image;
    conv_image = malloc(sizeof(*conv_image));
    if(conv_image == NULL){
        fprintf(stderr, "Error allocating memory\n");
        return NULL;
    }
    conv_image->header = image->header;
    conv_image->header.bits = 16;
    short padding = conv_image->header.width % 2 * 2;
    conv_image->header.imagesize = conv_image->header.height * (conv_image->header.width * 2 + padding);
    conv_image->header.size = conv_image->header.imagesize + BMP_HEADER_SIZE; 
    conv_image->data = malloc(conv_image->header.imagesize);
    if(conv_image->data == NULL){
        free(conv_image);
        fprintf(stderr, "Error allocating memory\n");
        return NULL;
    }
    short im_padding = (4 - image->header.width % 4) % 4;
    int i, j;
    unsigned short RGB;
    unsigned char color;
    short *sconv_image = (short *)&(conv_image->data[0]);
    int counter = 0;
    for(i = 0; i < conv_image->header.height; i++){
        for(j = 0; j < conv_image->header.width * 3; j++){
            color = image->data[i * (image->header.width * 3 + im_padding) + j];
            color >>= 3;
            RGB = color << BLUE_BIT; 
            j++;
            color = image->data[i * (image->header.width * 3 + im_padding) + j];
            color >>= 3;
            RGB = RGB | (color << GREEN_BIT);
            j++;
            color = image->data[i * (image->header.width * 3 + im_padding) + j];
            color >>= 3;
            RGB = RGB | (color << RED_BIT);
            sconv_image[counter++] = RGB;
        }
        for(j=0; j<padding; j++){
            sconv_image[counter++] = 0;
        }
    }
    return conv_image;
}
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

void Free_BMP_Image(BMP_Image* image) {
   free(image->data);
   free(image);
}
