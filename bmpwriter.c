#include <stdio.h>
#include <string.h>

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

int write_bmp(char *file_name, size_t rows, size_t columns, char mat[rows][columns], char debug)
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
  fprintf(fp, "BM");                     // signature
  print_uint(fp, 0x3e + rows * columns); // bmp size
  print_uint(fp, 0);                     // unused
  print_uint(fp, 0x3e);                  // bitmap image data offset

  // InfoHeader (size 40)
  print_uint(fp, 40);     // infoheader size
  print_int(fp, columns); // bitmap width
  print_int(fp, rows);    // bitmap height
  print_ushort(fp, 1);    // number of planes (don't know what it means)
  print_ushort(fp, 1);    // bits per pixel (1 = monochrome)
  print_uint(fp, 0);      // compression (0 = no compression)
  print_uint(fp, 0);      // size of image (0 because compression = 0)
  print_int(fp, 0);       // horizontal resolution (px/m)
  print_int(fp, 0);       // vertical resolution (px/m)
  print_uint(fp, 2);      // colours used
  print_uint(fp, 0);      // important colours (0 = all)

  // ColourTable (size 4*numcolours = 8 bytes)
  // colour 0 (black)
  print_byte(fp, 0x00); // red intensity
  print_byte(fp, 0x00); // green intensity
  print_byte(fp, 0x00); // blue intensity
  print_byte(fp, 0);    // unused
  // colour 1 (white)
  print_byte(fp, 0xff); // red intensity
  print_byte(fp, 0xff); // green intensity
  print_byte(fp, 0xff); // blue intensity
  print_byte(fp, 0);    // unused

  // in order to write bits, i first need to join them
  // together in 1-byte sequences, and then I can print them.
  // if i'm done reading bytes from the matrix but there
  // are still some bits in the char, i append however many zeroes
  // it's necessary and send that.
  size_t i = 0;
  unsigned char buf = 0;
  while (i < rows * columns)
  {
    size_t row = i / columns;
    size_t column = i % columns;

    buf <<= 1;
    if (mat[row][column])
      buf |= 0b00000001;

    // Debug statement
    char bufbinary[11] = "0b";
    bitchar(bufbinary, buf);
    if (debug)
      printf("i=%4zu, r=%2zu, c=%2zu, buf=%2x (%s)\n", i, row, column, buf, bufbinary);

    if ((i + 1) % 8 == 0)
    {
      fprintf(fp, "%c", buf);
      if (debug)
        printf("Flushed buffer, %x\n", buf);
      buf = 0;
    }

    i++;
  }

  // WARNING: This may not do what I intend for it to do.
  // If there are some bits leftover in the char,
  // push the leftmost of those bits to the left of the
  // byte.
  // As i is the number of iterations, it normally wouldn't be,
  // but when we leave the loop it's incremented one last time.
  // If it is not a multiple of 8, there are bits leftover, and
  // i % 8 tells us how many of those there are.
  // So 8 - (i % 8) tells us how many times we have
  // to shift them left.
  if ((i % 8) != 0)
  {
    // Debug statement
    char bufbinary[11] = "0b";
    bitchar(bufbinary, buf);

    printf("Flushing buffer after loop. current=%2x (%s)\n", buf, bufbinary);
    // I don't know whether this should be executed or not.
    // given a buf=0b00000111,
    // I don't know whether that should be
    // 0b0111000, 0b11100000, or 0b00000111, (or maybe even 0b00001110)
    buf <<= 8 - (i % 8);

    bufbinary[2] = '\0';
    bitchar(bufbinary, buf);
    printf("Flushed buffer, %2x (%s)\n", buf, bufbinary);
    fprintf(fp, "%c", buf);
  }

  fclose(fp);
  return 0;
}