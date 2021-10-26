#include <stdio.h>
#include "bmpwriter.h"

int main()
{
  int mat[2][4] = {{1, 0, 1, 0}, {0, 1, 0, 1}};
  int a = 0x4241;
  // printf("\x41\x42");
  // printf("%c%c", ((unsigned char *)&a)[0], ((unsigned char *)&a)[1]);
  // print_bytes(&a, sizeof(a));
  write_bmp("fn.bmp", 2, 4, mat);

  return 0;
}