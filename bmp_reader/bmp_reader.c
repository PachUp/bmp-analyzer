#include <stdio.h>
#include <stdlib.h>
#include "bmp_reader.h"
#include <string.h>
#define FILE_PATH "./rednwhite.bmp"
#define READ_METHOD "rb"
#define ZERO 0
#define EXIT -1
#define WIDTH_BYTES_START 18
#define WIDTH_BYTES_END 21
#define HEIGHT_BYTES_START 22
#define HEIGHT_BYTES_END 25
#define MAX_HEX_2_SIZE 2
#define COLOR_BYTES_START 54
#define BMP_24_BIT_MULTIPLIER 3
#define R_COLOR_POSITION 2
#define G_COLOR_POSITION 4
#define B_COLOR_POSITION 6
#define BASE_16 16
#define ONE 1
#define TWO 2
#define THREE 3

int main(void)
{
  FILE * fp;
  unsigned char* file_data;
  int file_len = ZERO;
  status rev_fun; // checks if the reverse function is successful
  char width_hex[MAX_WIDTH_HEX_LENGTH + ONE] = {ZERO}; // +1 for NULL
  char height_hex[MAX_HEIGHT_HEX_LENGTH + ONE] = {ZERO}; // +1 for NULL
  bmp bmp_info = {{ZERO, ZERO, ZERO}, ZERO, ZERO }; // the bmp info structure to put all the data in
  int width_hex_pos = ZERO; // follow the position of width_hex
  int height_hex_pos = ZERO; // follow the position of height_hex
  long int byte; // loop position
  int padding; // padding of the BMP file
  int color_count = ZERO; // count color amount
  fp = fopen(FILE_PATH, READ_METHOD);
  if (fp == NULL)
  {
    perror("Error with opening the BMP image file: ");
    return EXIT;
  }
  
  fseek(fp, ZERO, SEEK_END); // going to the end of the file
  file_len = ftell(fp); // getting the last postion of the file (size)
  rewind(fp); // going back the the beginning of the file
  file_data = (unsigned char*)malloc(file_len); // make sure there is enough memory for the file
  if (file_data == NULL)
  {
    return EXIT;
  }
  fread(file_data, file_len, ONE, fp); // Read in the entire file
  for(byte = ZERO; byte < file_len; byte++)
  {
    if (byte >= WIDTH_BYTES_START && byte <= WIDTH_BYTES_END)
    {
      char hex[MAX_HEX_2_SIZE + ONE]; // + 1 for null insert.
      sprintf(hex, "%02x", file_data[byte]); // deciaml to hex
      strcat(width_hex, hex);
    }
    
    if (byte >= HEIGHT_BYTES_START && byte <= HEIGHT_BYTES_END)
    {
      char hex[MAX_HEX_2_SIZE + ONE]; // + 1 for null insert.
      sprintf(hex, "%02x", file_data[byte]); // deciaml to hex
      strcat(height_hex, hex);
    }
  
  }

  int const width_hex_len = strlen(width_hex); // value untill the null terminator
  int const height_hex_len = strlen(height_hex);
  rev_fun = reverse_char_array(width_hex, width_hex_len);
  if(!rev_fun)
  {
    return EXIT;
  }
  
  rev_fun = reverse_char_array(height_hex, height_hex_len);
  if (!rev_fun)
  {
    return EXIT;
  }
  
  bmp_info.width = strtol(width_hex, NULL, BASE_16); // change from hexadecimal to deciaml again
  bmp_info.height = strtol(height_hex, NULL, BASE_16); // change from hexadecimal to deciaml again
  if (!(bmp_info.width || bmp_info.height)) {
    return EXIT;
  }
  printf("The width of the picture: %d\nThe height of the picture: %d", bmp_info.width, bmp_info.height);


  padding = bmp_info.width * THREE; // padding for 24-bit bmp file
  int color_pos = ZERO; // max numbers are 1-3
  for (byte = ZERO; byte < file_len - TWO; byte++)
  {
    if (byte >= COLOR_BYTES_START)
    {
      if (byte % padding != ZERO && byte != ZERO) {
        char hex[MAX_HEX_2_SIZE + ONE]; // + 1 for null insert.
        sprintf(hex, "%02x", file_data[byte]); // deciaml to hex
        color_pos++;
        if (color_pos == THREE) {
          strcpy(bmp_info.color[color_count].b, hex);
          int color_exist = check_if_color_exists(bmp_info.color[color_count], bmp_info, color_count);
          if (!color_exist)
          {
            color_count++;
          }
          color_pos = ZERO;
        }
        /*Because we modulo by 6 we need to check if the current number modulo 4 is actually 4*/
        else if (color_pos == TWO) {
          strcpy(bmp_info.color[color_count].g, hex);
        }

        else if (color_pos == ONE)
        {
          strcpy(bmp_info.color[color_count].r, hex);
        }
      }

    }

  }
  int i;
  for(i = ZERO; i < color_count; i++)
  {
    printf("\nfound color(s) in the image: #%.2s%.2s%.2s", bmp_info.color[i].b, bmp_info.color[i].g, bmp_info.color[i].r);
  }
  free(file_data);
  fclose(fp); // Close the file
  return ZERO;
}

/*
* The following function take 2 parameters and returns if it failed or didn't.
* param1: the hex characters to turn them to big endian
* param2: the length of the hex characters
*/
status reverse_char_array(char str[], int str_length)
{
  int str_pos;
  char *str_copy = malloc(str_length);
  if (str_copy == NULL) {
    return FAILURE;
  }
  
  memcpy(str_copy, str, str_length);
  /*The following loop is going though all the hex characters and switch them in order to make a big endian hex char array*/
  for(str_pos = ZERO; str_pos < str_length - ONE; str_pos= str_pos + TWO)
  {
    str[str_pos] = str_copy[str_length - str_pos - TWO];
    str[str_pos + ONE] = str_copy[str_length - str_pos - ONE];
  }
  
  free(str_copy);
  return SUCCESS;
}



/*
* The following function checks if a color exists in BMP_INFO
* param1: the color that needs to be checked
* param2: the struct where the color is going to be checked on
* param3: the current amount of colors in BMP_INFO
*/
int check_if_color_exists(rgb color, bmp bmp_info, int bmp_color_counter) {
  int color_pos;
  for(color_pos = ZERO; color_pos < bmp_color_counter; color_pos++)
  {
    if(strncmp(color.r, bmp_info.color[color_pos].r, MAX_HEX_2_SIZE) == ZERO && strncmp(color.g, bmp_info.color[color_pos].g, MAX_HEX_2_SIZE) == ZERO
     && strncmp(color.b, bmp_info.color[color_pos].b, MAX_HEX_2_SIZE) == ZERO)
     {
        return ONE;
     }
  }
  return ZERO;
}