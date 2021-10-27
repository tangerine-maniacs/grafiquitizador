#ifndef BMPWRITER
#define BMPWRITER

int write_bmp(char *file_name, size_t rows, size_t columns, char mat[rows][columns], char debug);
// struct BMP_HEADER
// {
//   short signature;
//   int bmp_size;
//   int _reserved;
//   int bitmap_offset;
// };
// struct BMP_INFO_HEADER
// {
//   int infoheader_size;       // infoheader size
//   int bitmap_width;          // bitmap width
//   int bitmap_height;         // bitmap height
//   short planes;              // number of planes (don't know what it means)
//   short bits_per_pixel;      // bits per pixel (1 = monochrome)
//   int compression;           // compression (0 = no compression)
//   int image_size;            // size of image (0 because compression = 0)
//   int horizontal_resolution; // horizontal resolution (px/m)
//   int vertical_resolution;   // vertical resolution (px/m)
//   int colours_used;          // colours used
//   int important_colours;     // important colours (0 = all)
// };

#endif