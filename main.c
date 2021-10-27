#include <stdio.h>
#include "bmpwriter.h"

#define WIDTH 512
#define HEIGHT 512

// Didn't want to include the math library only for this
double diff(double a, double b)
{
  return (a > b) ? a - b : b - a;
}

int main()
{
  // Pixel matrix
  char mat[HEIGHT][WIDTH] = {};

  // Graph variables
  double x_scale = 5, y_scale = 5;
  double f_tolerance = 8.0e-2;

  // Function variables
  double x, y, f;

  for (int _y = HEIGHT - 1; _y >= 0; _y--)
  {
    /*
      current_column-row  = _y - _x       ** Note that _y starts counting from max
      total_columns-rows  = WIDTH - HEIGHT
      Offset              = - Scale / 2 

      x = current_column / total_columns * X_Scale - Offset
      y = current_row / total_rows * Y_Scale - Offset
    */

    y = (double)_y / (HEIGHT - 1) * y_scale - (y_scale / 2.0);

    for (int _x = 0; _x < WIDTH; _x++)
    {
      x = (double)_x / (WIDTH - 1) * x_scale - (x_scale / 2.0);

      // Function: y = x³ - 2x² + 1
      f = 2 * x * x * x - 2 * x * x + 2;

      if (diff(f, y) < f_tolerance) // Function
      {
        mat[_y][_x] = 1;
      }
      else if (diff(x, 0) < x_scale / WIDTH || diff(y, 0) < y_scale / HEIGHT) // Axes
      {
        mat[_y][_x] = 1;
      }
      else
      {
        mat[_y][_x] = 0;
      }
    }
  }

  write_bmp("fn.bmp", HEIGHT, WIDTH, mat);

  return 0;
}