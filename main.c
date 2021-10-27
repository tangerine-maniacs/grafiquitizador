#include <stdio.h>
#include "bmpwriter.h"

#define h 128
#define w 128

int main()
{
  int mat[h][w];
  for (int i = 0; i < h; i++)
  {
    for (int j = 0; j < w; j++)
    {
      mat[i][j] = (i % 2) && (j % 2);
    }
  }

  // int a = 0x4241;

  // printf("\x41\x42");
  // printf("%c%c", ((unsigned char *)&a)[0], ((unsigned char *)&a)[1]);
  // print_bytes(&a, sizeof(a));
  write_bmp("fn.bmp", h, w, mat);

  return 0;
}