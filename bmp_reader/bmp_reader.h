#ifndef BMP_READER_H
  #define BMP_READER_H
#include <stdio.h>

#define MAX_24_BIT_COLORS 50
#define MAX_HEX_2_SIZE 2

struct rgb {
  char r[MAX_HEX_2_SIZE];
  char g[MAX_HEX_2_SIZE];
  char b[MAX_HEX_2_SIZE];
};
typedef struct rgb rgb;

struct bmp_data {
  rgb color[MAX_24_BIT_COLORS];
  long height;
  long width;
};

enum bmp_dimensions_length {MAX_HEIGHT_HEX_LENGTH = 8, MAX_WIDTH_HEX_LENGTH = 8};
enum status {SUCCESS = 1, FAILURE = 0};

typedef struct bmp_data bmp;
typedef enum status status;

/*
* The following function take 2 parameters and returns if it failed or didn't.
* param1: the hex characters to turn them to big endian
* param2: the length of the hex characters
*/
status reverse_char_array(char*, int);

/*
* The following function checks if a color exists in BMP_INFO
* param1: the color that needs to be checked
* param2: the struct where the color is going to be checked on
* param3: the current amount of colors in BMP_INFO
*/
int check_if_color_exists(rgb, bmp, int);
#endif
