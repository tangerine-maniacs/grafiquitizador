// GPL-3.0-only

#include <stdio.h>
#include <string.h>

// #define DEBUG

// Print variables to file_pointer
// (Little-endian)
void print_bytes(FILE *fp, void *p, size_t len)
{
  for (size_t i = 0; i < len; i++)
    fprintf(fp, "%c", ((unsigned char *)p)[i]);
}
void print_int(FILE *fp, int a)
{
  print_bytes(fp, &a, sizeof(a));
}
void print_uint(FILE *fp, unsigned int a)
{
  print_bytes(fp, &a, sizeof(a));
}
void print_short(FILE *fp, short a)
{
  print_bytes(fp, &a, sizeof(a));
}
void print_ushort(FILE *fp, unsigned short a)
{
  print_bytes(fp, &a, sizeof(a));
}
void print_byte(FILE *fp, char a)
{
  print_bytes(fp, &a, sizeof(a));
}

// For debugging
void bitchar(char *result, char c)
{
  for (size_t i = 0; i < 8; i++)
  {
    sprintf(result + strlen(result), "%d", !!((c << i) & 0x80));
  }
}

int write_bmp(char *file_name, size_t rows, size_t columns, char mat[rows][columns][3])
{
  FILE *fp;

  fp = fopen(file_name, "w");

  if (fp == NULL)
  {
    return -1; // Couldn't open file
  }

  //http://www.ece.ualberta.ca/~elliott/ee552/studentAppNotes/2003_w/misc/bmp_file_format/bmp_file_format.htm
  //https://medium.com/sysf/bits-to-bitmaps-a-simple-walkthrough-of-bmp-image-format-765dc6857393
  // Header (size 14)
  fprintf(fp, "BM");                            // signature
  print_uint(fp, 0x36 + (rows * columns) * 24); // bmp size
  print_uint(fp, 0);                            // unused
  print_uint(fp, 0x36);                         // bitmap image data offset

  // InfoHeader (size 40)
  print_uint(fp, 40);     // infoheader size
  print_int(fp, columns); // bitmap width
  print_int(fp, rows);    // bitmap height
  print_ushort(fp, 1);    // number of planes (don't know what it means)
  print_ushort(fp, 24);   // bits per pixel (1 = monochrome)
  print_uint(fp, 0);      // compression (0 = no compression)
  print_uint(fp, 0);      // size of image (0 because compression = 0)
  print_int(fp, 0);       // horizontal resolution (px/m)
  print_int(fp, 0);       // vertical resolution (px/m)
  print_uint(fp, 4);      // colours used
  print_uint(fp, 0);      // important colours (0 = all)

  // ColourTable not present because bits per pixel >= 8

  // in order to write bits, i first need to join them
  // together in 1-byte sequences, and then I can print them.
  // if i'm done reading bytes from the matrix but there
  // are still some bits in the char, i append however many zeroes
  // it's necessary and send that.
  size_t i = 0;
  while (i < rows * columns)
  {
    size_t row = i / columns;
    size_t column = i % columns;

    // Inverted because .bmp uses bgr and we use rgb.
    unsigned char pix[3];
    pix[0] = mat[row][column][2];
    pix[1] = mat[row][column][1];
    pix[2] = mat[row][column][0];

#ifdef DEBUG
    printf("i=%16zu, r=%8zu, c=%8zu, pix={ %2x, %2x, %2x }\n", i, row, column, pix[0], pix[1], pix[2]);
#endif
    fprintf(fp, "%c%c%c", pix[0], pix[1], pix[2]);

    i++;
  }

  fclose(fp);
  return 0;
}