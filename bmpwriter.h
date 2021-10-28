// GPL-3.0-only

#ifndef BMPWRITER
#define BMPWRITER

// mat is a 2d rgb pixel array.
int write_bmp(char *file_name, size_t rows, size_t columns, char mat[rows][columns][3]);

#endif