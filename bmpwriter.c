#include <stdio.h>

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
void print_short(FILE *fp, short a)
{
  print_bytes(fp, &a, sizeof(a));
}
void print_byte(FILE *fp, char a)
{
  print_bytes(fp, &a, sizeof(a));
}

int write_bmp(char *file_name, size_t rows, size_t columns, int mat[rows][columns])
{
  FILE *fp;

  fp = fopen(file_name, "w");

  if (fp == NULL)
  {
    return -1; // Couldn't open file
  }

  //http://www.ece.ualberta.ca/~elliott/ee552/studentAppNotes/2003_w/misc/bmp_file_format/bmp_file_format.htm
  // Header (size 14)
  fprintf(fp, "BM");   // signature
  print_int(fp, 1);    // bmp size
  print_int(fp, 0);    // unused
  print_int(fp, 0x3e); // bitmap image data offset

  // InfoHeader (size 40)
  print_int(fp, 40);      // infoheader size
  print_int(fp, columns); // bitmap width
  print_int(fp, rows);    // bitmap height
  print_short(fp, 1);     // number of planes (don't know what it means)
  print_short(fp, 1);     // bits per pixel (1 = monochrome)
  print_int(fp, 0);       // compression (0 = no compression)
  print_int(fp, 0);       // size of image (0 because compression = 0)
  print_int(fp, 0);       // horizontal resolution (px/m)
  print_int(fp, 0);       // vertical resolution (px/m)
  print_int(fp, 2);       // colours used
  print_int(fp, 0);       // important colours (0 = all)

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

  // in order to write bytes, i first need to join them
  // together in 8 bit sequences, and then I can print them.
  // if i'm done reading bytes from the matrix but there
  // are still some bits in the char, i append however many zeroes
  // it's necessary and send that.
  size_t i = 0;
  char buf = 0;
  while (i < rows * columns)
  {
    printf("Iteration %d\n", i);

    if (mat[i])
      buf |= 0b00000001;
    buf <<= 1;

    if ((i + 1) % 8 == 0)
    {
      fprintf(fp, "%c", buf);
      printf("Flushed buffer, %x\n", buf);
      buf = 0;
    }
    i++;
  }

  // WARNING: This may not do what I intend for it to do.
  // If there are some bits leftover in the char,
  // push the leftmost of those bits to the left of the
  // byte.
  // As i is the number of iterations - 1, i+1 is the
  // number of iterations. If it is not a multiple of 8,
  // there are bits leftover, and (i+1) % 8 tells us how
  // many of those there are.
  // So 8 - ((i+1) % 8) tells us how many times we have
  // to shift them left.
  // if ((i + 1) % 8 != 0)
  // {
  //   printf("Flushing buffer after loop\n");
  //   buf <<= 8 - ((i + 1) % 8);
  //   fprintf(fp, "%c", buf);
  // }

  fclose(fp);
  return 0;
}