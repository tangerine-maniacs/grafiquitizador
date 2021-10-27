#include <stdio.h>
#include "bmpwriter.h"

#define WIDTH 66
#define HEIGHT 66

// Didn't want to include the math library only for this
double diff(double a, double b)
{
    return (a > b) ? a - b : b - a;
}

int main()
{ 
    // Pixel matrix
    int mat[HEIGHT][WIDTH] = {};
    
    // Graph variables
    double x_scale = 5, y_scale = 5;
    double f_tolerance = 3.0e-1;
    
    // Function variables
    double x, y, f;


    for (int _y = HEIGHT-1; _y >= 0; _y--)
    {
        /*
            current_column-row  = _y - _x       ** Note that _y starts counting from max
            total_columns-rows  = WIDTH - HEIGHT
            Offset              = - Scale / 2 

            x = current_column / total_columns * X_Scale - Offset
            y = current_row / total_rows * Y_Scale - Offset
        */

        y = (double)_y / (HEIGHT - 1) * y_scale - (y_scale/2.0);

        for (int _x = 0; _x < WIDTH; _x++)
        {
            x = (double)_x / (WIDTH - 1) * x_scale - (x_scale/2.0);
            
            // Function: y = - x² + 2
            f = - x * x + 2;

            if (diff(f, y) < f_tolerance)
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